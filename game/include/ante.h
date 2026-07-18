#ifndef GUARD_ANTE_H
#define GUARD_ANTE_H

#include "global.h"

// Pokémon Ante core rules. See DESIGN.md at the repository root.
#define ANTE_WILD_ENCOUNTERS_DISABLED TRUE

#define ANTE_SLOT_NONE 0xFF

u16 AnteCommit(void);
void Ante_HandleTrainerBattleEnd(bool8 playerWon);

#endif // GUARD_ANTE_H
