#include "../include/types.h"
#include "../include/cheats.h"

/**
 *  Cheat configuration block -- see include/cheats.h.
 *  Placed in .data of the always-resident arm9 extension so it has a stable
 *  RAM address (emitted into rom_gen.ld as gCheatConfig on every build).
 */
CheatConfig ALIGN4 gCheatConfig =
{
    .magic = CHEAT_MAGIC,
    .expMult = 0,
    .catch100 = 0,
    .alwaysRun = 0,
    .infRepel = 0,
    .tunerSpecies = 0,
    .tunerLevel = 0,
    .tunerGender = 0,
    .fleeTrainer = 0,
};
