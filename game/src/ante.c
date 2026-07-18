#include "global.h"
#include "ante.h"
#include "battle.h"
#include "data.h"
#include "event_data.h"
#include "pokemon.h"
#include "pokemon_storage_system.h"
#include "random.h"
#include "string_util.h"
#include "party_menu.h"
#include "constants/flags.h"
#include "constants/species.h"

static u16 CountOwnedMons(void);
static u8 DrawPlayerAnteSlot(void);
static u16 GetTrainerMonSpecies(u16 trainerId, u8 slot);
static void AnteGiveMonToPlayer(struct Pokemon *mon);
static void RecordBounty(struct Pokemon *mon, u16 trainerId);
static u8 GetBadgeCount(void);

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

    if (playerSlot == ANTE_SLOT_NONE || enemyCount == 0)
    {
        ante->committed = FALSE;
        return FALSE;
    }

    ante->playerSlot = playerSlot;
    ante->enemySlot = Random() % enemyCount;
    ante->committed = TRUE;

    GetMonNickname(&gPlayerParty[playerSlot], gStringVar1);
    StringCopy(gStringVar2, gSpeciesNames[GetTrainerMonSpecies(trainerId, ante->enemySlot)]);
    return TRUE;
}

// Like GiveMonToPlayer, but the previous owner's OT data is deliberately
// preserved: a won ante keeps its history.
static void AnteGiveMonToPlayer(struct Pokemon *mon)
{
    s32 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            break;
    }
    if (i >= PARTY_SIZE)
    {
        SendMonToPC(mon);
    }
    else
    {
        CopyMon(&gPlayerParty[i], mon, sizeof(*mon));
        gPlayerPartyCount = i + 1;
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
        CalculatePlayerPartyCount();
    }
}
