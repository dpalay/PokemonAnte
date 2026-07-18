#include "global.h"
#include "ante.h"
#include "battle.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "save.h"
#include "string_util.h"
#include "party_menu.h"
#include "characters.h"
#include "constants/flags.h"
#include "constants/species.h"

static u16 CountOwnedMons(void);
static u8 DrawPlayerAnteSlot(void);
static u16 GetTrainerMonSpecies(u16 trainerId, u8 slot);
static void AnteGiveMonToPlayer(struct Pokemon *mon);
static void RecordBounty(struct Pokemon *mon, u16 trainerId);
static u8 GetBadgeCount(void);
static void WithdrawFirstStoredMon(void);
static bool8 IsNicknameInUse(const u8 *name, const struct Pokemon *exclude);
static void EnsureUniqueNickname(struct Pokemon *mon);

// Party slot (+1; 0 = none) of the most recently won ante, so the field
// script can run the Registry naming prompt after the battle ends.
static EWRAM_DATA u8 sLastWonPartySlot = 0;

static u16 CountOwnedMons(void)
{
    u16 count = CalculatePlayerPartyCount();
    u32 box, pos;

    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            if (GetBoxMonDataAt(box, pos, MON_DATA_SPECIES) != SPECIES_NONE)
                count++;
        }
    }
    return count;
}

static u8 DrawPlayerAnteSlot(void)
{
    u8 validSlots[PARTY_SIZE];
    u8 numValid = 0;
    u32 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
         && !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
            validSlots[numValid++] = i;
    }
    if (numValid == 0)
        return ANTE_SLOT_NONE;
    return validSlots[Random() % numValid];
}

static u16 GetTrainerMonSpecies(u16 trainerId, u8 slot)
{
    const struct Trainer *trainer = &gTrainers[trainerId];

    switch (trainer->partyFlags)
    {
    case 0:
        return trainer->party.NoItemDefaultMoves[slot].species;
    case F_TRAINER_PARTY_CUSTOM_MOVESET:
        return trainer->party.NoItemCustomMoves[slot].species;
    case F_TRAINER_PARTY_HELD_ITEM:
        return trainer->party.ItemDefaultMoves[slot].species;
    case F_TRAINER_PARTY_CUSTOM_MOVESET | F_TRAINER_PARTY_HELD_ITEM:
        return trainer->party.ItemCustomMoves[slot].species;
    }
    return SPECIES_NONE;
}

// Script special. Draws both antes for the upcoming battle against
// gTrainerBattleOpponent_A and buffers their names for the reveal message
// (player's in gStringVar1, opponent's in gStringVar2). Returns TRUE if the
// battle is an ante battle, FALSE if it must be played friendly.
u16 AnteCommit(void)
{
    struct AnteSaveData *ante = &gSaveBlock1Ptr->ante;
    u16 trainerId = gTrainerBattleOpponent_A;
    u8 enemyCount = gTrainers[trainerId].partySize;
    u8 playerSlot = DrawPlayerAnteSlot();

    // Ownership floor: the player may never stake their last Pokémon.
    if (playerSlot == ANTE_SLOT_NONE || enemyCount == 0 || CountOwnedMons() < 2)
    {
        ante->committed = FALSE;
        return FALSE;
    }

    ante->playerSlot = playerSlot;
    ante->enemySlot = Random() % enemyCount;
    ante->committed = TRUE;

    GetMonNickname(&gPlayerParty[playerSlot], gStringVar1);
    StringCopy(gStringVar2, gSpeciesNames[GetTrainerMonSpecies(trainerId, ante->enemySlot)]);

    // Species for the reveal ceremony's pictures (showmonpic reads vars).
    gSpecialVar_0x8004 = GetMonData(&gPlayerParty[playerSlot], MON_DATA_SPECIES, NULL);
    gSpecialVar_0x8005 = GetTrainerMonSpecies(trainerId, ante->enemySlot);

    // The stake is on disk before the battle resolves: resetting after a
    // loss reloads into a save where this ante is already committed.
    TrySavingData(SAVE_NORMAL);
    return TRUE;
}

// The Registry requires every owned Pokémon to carry a unique name: the
// name is the ledger key. Uniqueness spans the party, storage, and the
// bounty ledger (a lost Pokémon keeps its name reserved).
static bool8 IsNicknameInUse(const u8 *name, const struct Pokemon *exclude)
{
    u8 buf[POKEMON_NAME_LENGTH + 1];
    struct AnteSaveData *ante = &gSaveBlock1Ptr->ante;
    u32 i, box, pos;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (&gPlayerParty[i] == exclude)
            continue;
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            continue;
        GetMonNickname(&gPlayerParty[i], buf);
        if (StringCompare(buf, name) == 0)
            return TRUE;
    }
    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            struct BoxPokemon *boxMon = GetBoxedMonPtr(box, pos);

            if (GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL) == SPECIES_NONE)
                continue;
            GetBoxMonData(boxMon, MON_DATA_NICKNAME, buf);
            StringGet_Nickname(buf);
            if (StringCompare(buf, name) == 0)
                return TRUE;
        }
    }
    for (i = 0; i < ANTE_BOUNTY_COUNT; i++)
    {
        if (ante->bounties[i].species != SPECIES_NONE
         && StringCompare(ante->bounties[i].nickname, name) == 0)
            return TRUE;
    }
    return FALSE;
}

// Backstop for acquisitions that bypass the naming screen (e.g. a won ante
// sent straight to storage): species name, truncated and numbered until
// it is unique. The player can re-register it at the Name Rater.
static void EnsureUniqueNickname(struct Pokemon *mon)
{
    u8 name[POKEMON_NAME_LENGTH + 1];
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 n, len;

    GetMonNickname(mon, name);
    if (name[0] != EOS && !IsNicknameInUse(name, mon))
        return;

    for (n = 2; n < 100; n++)
    {
        StringCopy(name, gSpeciesNames[species]);
        len = StringLength(name);
        if (len > POKEMON_NAME_LENGTH - 2)
            len = POKEMON_NAME_LENGTH - 2;
        if (n < 10)
        {
            name[len] = CHAR_0 + n;
            name[len + 1] = EOS;
        }
        else
        {
            name[len] = CHAR_0 + n / 10;
            name[len + 1] = CHAR_0 + n % 10;
            name[len + 2] = EOS;
        }
        if (!IsNicknameInUse(name, mon))
            break;
    }
    SetMonData(mon, MON_DATA_NICKNAME, name);
}

// Script special. TRUE if the party mon in VAR_0x8004 carries a name the
// Registry will accept: non-empty and owned by no other Pokémon.
u16 AnteIsPartyMonNameValid(void)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x8004];
    u8 name[POKEMON_NAME_LENGTH + 1];

    GetMonNickname(mon, name);
    if (name[0] == EOS)
        return FALSE;
    return !IsNicknameInUse(name, mon);
}

// Script special. Party slot of the ante won in the battle that just
// ended (so the field script can run the naming prompt), or 0xFF.
// Clears on read.
u16 AnteGetLastWonPartySlot(void)
{
    u8 slot = sLastWonPartySlot;

    sLastWonPartySlot = 0;
    if (slot == 0)
        return 0xFF;
    return slot - 1;
}

// Like GiveMonToPlayer, but the previous owner's OT data is deliberately
// preserved: a won ante keeps its history.
static void AnteGiveMonToPlayer(struct Pokemon *mon)
{
    s32 i;

    EnsureUniqueNickname(mon);
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }
    if (i >= PARTY_SIZE)
    {
        SendMonToPC(mon);
        sLastWonPartySlot = 0;
    }
    else
    {
        CopyMon(&gPlayerParty[i], mon, sizeof(*mon));
        gPlayerPartyCount = i + 1;
        sLastWonPartySlot = i + 1;
    }
}

static u8 GetBadgeCount(void)
{
    u8 count = 0;
    u32 i;

    for (i = 0; i < 8; i++)
    {
        if (FlagGet(FLAG_BADGE01_GET + i))
            count++;
    }
    return count;
}

static void RecordBounty(struct Pokemon *mon, u16 trainerId)
{
    struct AnteSaveData *ante = &gSaveBlock1Ptr->ante;
    s32 slot = ANTE_BOUNTY_COUNT - 1;
    s32 i;

    for (i = 0; i < ANTE_BOUNTY_COUNT; i++)
    {
        if (ante->bounties[i].species == SPECIES_NONE)
        {
            slot = i;
            break;
        }
    }
    ante->bounties[slot].species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    ante->bounties[slot].holderTrainerId = trainerId;
    ante->bounties[slot].personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
    ante->bounties[slot].level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    GetMonNickname(mon, ante->bounties[slot].nickname);
    ante->bounties[slot].badgesAtLoss = GetBadgeCount();
}

// Called from the trainer battle end callbacks. Settles the committed ante:
// on a win the opponent's staked Pokémon joins the player (party if there is
// room, storage otherwise); on a loss the player's staked Pokémon is removed
// and recorded as a bounty held by the winning trainer.
void Ante_HandleTrainerBattleEnd(bool8 playerWon)
{
    struct AnteSaveData *ante = &gSaveBlock1Ptr->ante;

    if (!ante->committed)
        return;
    ante->committed = FALSE;

    if (playerWon)
    {
        struct Pokemon *mon = &gEnemyParty[ante->enemySlot];

        if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE)
            AnteGiveMonToPlayer(mon);
    }
    else
    {
        RecordBounty(&gPlayerParty[ante->playerSlot], gTrainerBattleOpponent_A);
        ZeroMonData(&gPlayerParty[ante->playerSlot]);
        CompactPartySlots();
        if (CalculatePlayerPartyCount() == 0)
            WithdrawFirstStoredMon();
    }
}

// Losing the last party member with others still in storage would leave an
// empty party; the ownership floor guarantees at least one Pokémon exists
// somewhere, so pull the first stored one into the party.
static void WithdrawFirstStoredMon(void)
{
    u32 box, pos;

    for (box = 0; box < TOTAL_BOXES_COUNT; box++)
    {
        for (pos = 0; pos < IN_BOX_COUNT; pos++)
        {
            if (GetBoxMonDataAt(box, pos, MON_DATA_SPECIES) != SPECIES_NONE)
            {
                BoxMonToMon(GetBoxedMonPtr(box, pos), &gPlayerParty[0]);
                ZeroBoxMonAt(box, pos);
                gPlayerPartyCount = 1;
                return;
            }
        }
    }
}
