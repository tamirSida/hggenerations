#include "../include/types.h"
#include "../include/cheats.h"

/**
 *  Cheat configuration block storage -- see include/cheats.h.
 *  Pinned into the dedicated .cheatcfg section, which src/linker.ld places
 *  first in the always-resident arm9 extension: this storage sits at exactly
 *  0x023D8600 (= CHEAT_CONFIG_ADDR) in every build.  All code accesses it
 *  through the gCheatConfig absolute-address macro, never this symbol.
 */
CheatConfig __attribute__((section(".cheatcfg"))) ALIGN4 gCheatConfigStorage =
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
    .shinyWild = 0,
    .uncapLevel = 0,
};
