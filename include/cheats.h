#ifndef CHEATS_H
#define CHEATS_H

#include "types.h"

/**
 *  Cheat configuration block.  Lives at a fixed RAM address in the always-resident
 *  arm9 extension (overlay 129) -- see rom_gen.ld for the resolved address of
 *  gCheatConfig after a build.  Each field is designed to be poked from an
 *  Action Replay constant-write code; everything defaults to "off" so the ROM
 *  behaves identically to stock when no codes are active.
 */
typedef struct CheatConfig
{
    u32 magic;         // 'HGV3' = 0x33564748, used to locate/sanity-check the block in RAM
    u8 expMult;        // experience multiplier; 0/1 = off (AR writes 4/16/32/64/128)
    u8 catch100;       // nonzero = every ball always catches
    u8 alwaysRun;      // nonzero = running from wild battles always succeeds (traps/abilities bypassed)
    u8 infRepel;       // nonzero = an active repel never runs out
    u16 tunerSpecies;  // Encounter Tuner: species/dex number to force; 0 = tuner off
    u8 tunerLevel;     // Encounter Tuner: level 1-100; 0 = keep natural level
    u8 tunerGender;    // Encounter Tuner: 0 = random, 1 = male, 2 = female
} CheatConfig;

#define CHEAT_MAGIC 0x33564748  // 'HGV3'

extern CheatConfig gCheatConfig;

#endif // CHEATS_H
