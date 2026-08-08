# HeartGold Generations v3.4 — Cheat Guide (Delta / melonDS)

Current build: `HeartGold-Generations-v3.4-cheats.nds`
(SHA-256 starts `44bc5876`). Addresses are pinned — codes are identical for v3.3/v3.4.

> **v3.3 changed every cheat address — once.** The cheat block is now pinned to
> a dedicated spot at the start of the engine's code region (`0x023D8600`), so
> these codes stay valid in every future build. Codes from the v3.1/v3.2 guides
> no longer work; re-enter the ones below.

## The Encounter Tuner (built into the game — no cheat code needed)

Press **R** anywhere in the overworld. It uses the game's touch keyboard to
ask, in order:

1. **Species number** — type it and confirm. Confirm an *empty* entry to
   switch the Tuner **off**. Numbering: Gen 1-4 species (dex 1-493) use their
   National Dex number as-is (`448` = Lucario); from Gen 5 onward add **+50**
   (the engine reserves IDs 494-543): Victini = `544`, Greninja = `708`,
   Zacian = `938`, Pecharunt = `1075`.
2. **Level** — `1`–`100`, or empty to keep the area's natural levels.
3. **Gender** — `0` = random, `1` = male, `2` = female.

While active, every wild encounter (grass, surf, fishing, caves) becomes the
configured Pokémon. Press **L** for the Portable PC, as in v2.0. While holding
**B**, the L/R shortcuts are ignored (keeps them clear of the
walk-through-walls toggle).

## Adding codes in Delta

Pause → **Cheat Codes** → **+** → **Action Replay** → paste name + code.
Tick to enable, untick to disable.

> **Important:** these cheats write a setting into game memory. Unticking stops
> *refreshing* the setting, but the last value stays until you briefly enable
> **All Native Cheats OFF** or restart the game. The walk-through-walls code is
> the exception (its L+B combo restores vanilla behavior directly).

## XP (enable at most one multiplier)

| Name | Code |
|---|---|
| XP ×4   | `223D8604 00000004` |
| XP ×16  | `223D8604 00000010` |
| XP ×32  | `223D8604 00000020` |
| XP ×64  | `223D8604 00000040` |
| XP ×128 | `223D8604 00000080` |
| Ignore Level Cap (v3.3+) | `223D860E 00000001` |

The hack raises its level cap per badge and **discards** any experience above
the current cap — that is why multipliers alone seem to "stop working" as a
mon approaches the cap. Enable **Ignore Level Cap** to lift the ceiling to 100
(also unblocks Rare Candies at the cap).

## Catching & battles

| Name | Code | Effect |
|---|---|---|
| 100% Catch Rate | `223D8605 00000001` | any ball always catches |
| Always Run (wild) | `223D8606 00000001` | running from wild battles always works — traps, Mean Look, Arena Trap, speed all bypassed |
| Shiny Wild Encounters (v3.3+) | `223D860D 00000001` | every wild encounter is shiny; combines with the Encounter Tuner |
| Flee Trainer Battles (v3.4+) | `223D860C 00000001` | **press L+R at any point during the battle**; the escape fires at the next turn start ("Got away safely!") |

**Flee Trainer Battles, reworked in v3.3:** the earlier version forced the
battle outcome from any frame, which could yank the battle engine out of an
animation mid-flight and crash. It now waits for the turn-start state (all
menus closed — the same state a genuine wild flee runs from) and executes the
engine's own escape script. The trainer is not marked defeated and will
re-engage on sight. Still the most experimental cheat: save first.

## Overworld

| Name | Code | Effect |
|---|---|---|
| Infinite Repel | `223D8607 00000001` | an active repel never runs out (use one repel once) |
| Walk Through Walls *(vanilla)* | see below | **R+B** on, **L+B** off |

```
94000130 FCFD0200
1205DAA2 00000200
D2000000 00000000
94000130 FCFD0100
1205DAA2 00001C20
D2000000 00000000
```

Don't save while standing somewhere unreachable; switch it off (L+B) before
saving.

## Reset

| Name | Code | Effect |
|---|---|---|
| All Native Cheats OFF | `023D8604 00000000` + `023D860C 00000000` (one code, two lines) | zeroes XP/catch/run/repel + flee/shiny/uncap at once |

## Event unlocks

Not needed: the hack has no distribution-event gating (no GS Ball / Azure
Flute scripts exist), and the National Dex is on from the start. The Tuner can
summon any species directly.

## Cheat block internals

`gCheatConfig` is pinned at `0x023D8600` (see `include/cheats.h`,
`src/linker.ld`):

| Address | Field |
|---|---|
| `0x023D8600` | magic `HGV3` (u32) |
| `0x023D8604` | XP multiplier (0/1 = off) |
| `0x023D8605` | 100% catch |
| `0x023D8606` | always run (wild) |
| `0x023D8607` | infinite repel |
| `0x023D8608` | tuner species (u16) |
| `0x023D860A` | tuner level |
| `0x023D860B` | tuner gender |
| `0x023D860C` | flee trainer battles |
| `0x023D860D` | shiny wild encounters |
| `0x023D860E` | ignore level cap |

The Encounter Tuner writes its own three fields in-game; everything is also
pokeable via AR codes.
