# HeartGold Generations v3.1 — Cheat Guide (Delta / melonDS)

These cheats work **only with the v3.1 ROM** (`HeartGold-Generations-v3.1-cheats.nds`,
SHA-256 `a83d983e...d83442`). The addresses below are baked into this specific build.

## The Encounter Tuner (built into the game — no cheat code needed)

Press **R** anywhere in the overworld to open the Encounter Tuner. It uses the
game's touch keyboard to ask you, in order:

1. **Species number** — type it and confirm. Confirm an *empty* entry to
   switch the Tuner **off**. Numbering: Gen 1-4 species (dex 1-493) use their
   National Dex number as-is (`448` = Lucario); from Gen 5 onward add **+50**
   (the engine reserves IDs 494-543): Victini = `544`, Greninja = `708`,
   Zacian = `938`, Pecharunt = `1075`.
2. **Level** — `1`–`100`, or empty to keep the area's natural levels.
3. **Gender** — `0` = random, `1` = male, `2` = female.

While active, **every wild encounter** (grass, surf, fishing, caves) becomes the
Pokémon you configured. Held items, shiny odds and IVs roll normally. Turn it
off by opening it again and confirming an empty Pokédex number.

(Press **L** for the Portable PC, as in v2.0. While holding **B**, the L/R
shortcuts are ignored — that keeps them out of the walk-through-walls toggle.)

## Delta cheat codes

In Delta: pause the game → **Cheat Codes** → **+** → choose **Action Replay**,
paste the name and code. Tick to enable, untick to disable.

> **Important:** the native cheats below write a setting into the game's memory.
> Unticking the code stops *refreshing* the setting but the last value stays
> until you either enable **All Native Cheats OFF** for a moment or restart the
> game. Codes marked *(vanilla)* act instantly on tick/untick.

### XP multipliers (enable at most one)

| Name | Code |
|---|---|
| XP ×4   | `223DDE4C 00000004` |
| XP ×16  | `223DDE4C 00000010` |
| XP ×32  | `223DDE4C 00000020` |
| XP ×64  | `223DDE4C 00000040` |
| XP ×128 | `223DDE4C 00000080` |

Note: the hack's **level cap** still applies — extra XP is clamped at the cap,
so a multiplier gets you *to* the cap fast but never past it.

### Catching & battles

| Name | Code | Effect |
|---|---|---|
| 100% Catch Rate | `223DDE4D 00000001` | any ball always catches (all 4 shakes) |
| Always Run (wild) | `223DDE4E 00000001` | running from wild battles always works — traps, Mean Look, Arena Trap, speed all bypassed |

**About trainer battles:** Gen 4's engine hard-blocks fleeing trainer battles in
code this hack doesn't touch, and force-fleeing one would leave the trainer's
script in a broken state (white-out risk). To *avoid* trainer battles, use
Walk Through Walls below and route around their line of sight — combined with
the Encounter Tuner there's no need to grind through them.

### Overworld

| Name | Code | Effect |
|---|---|---|
| Infinite Repel | `223DDE4F 00000001` | an active repel never runs out (use one repel once) |
| Walk Through Walls *(vanilla)* | see below | press **R+B** to enable, **L+B** to disable |

Walk Through Walls (classic HGSS US code, verified against this build):

```
94000130 FCFD0200
1205DAA2 00000200
D2000000 00000000
94000130 FCFD0100
1205DAA2 00001C20
D2000000 00000000
```

Do **not** save while standing somewhere unreachable, and disable it (L+B)
before saving.

### Reset

| Name | Code | Effect |
|---|---|---|
| All Native Cheats OFF | `023DDE4C 00000000` | zeroes XP/catch/run/repel cheats at once |

### Event unlocks

Not needed: HeartGold Generations doesn't gate content behind distribution
events (no GS Ball / Azure Flute style scripts exist in the hack), and the
National Dex is enabled from the start. Every added species is obtainable in
the wild — and with the Encounter Tuner you can summon any of them directly.

## Cheat block internals (for the curious)

`gCheatConfig` lives at `0x023DDE48` (see `include/cheats.h`):

| Address | Field |
|---|---|
| `0x023DDE48` | magic `HGV3` |
| `0x023DDE4C` | XP multiplier (0/1 = off) |
| `0x023DDE4D` | 100% catch |
| `0x023DDE4E` | always run |
| `0x023DDE4F` | infinite repel |
| `0x023DDE50` | tuner species (u16) |
| `0x023DDE52` | tuner level |
| `0x023DDE53` | tuner gender |

The Encounter Tuner writes the last three itself; you can also poke them with
AR codes if you ever want a code-only encounter override.
