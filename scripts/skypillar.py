#!/usr/bin/env python3
"""Sky Pillar map insertion (M1 foundation).

Runs in the Makefile after move_narc, when base/ holds the freshly extracted
filesystem (destroyed and re-extracted every build, so appends here are
naturally idempotent) and base/arm9.bin is already decompressed.

M1 scope: claim the dead map header slot 538 (MAP_UNUSED) and turn it into an
enterable interior cloned from Bell Tower 3F (header 333).  Entry is via the
debug warp cheat byte; exit via Escape Rope.  No vanilla map is modified.

Format spec (verified against base ROM bytes + pret/pokeheartgold decomp):
  - map header table: arm9 @0xF6BE0, 540 entries x 24 bytes
  - matrix NARC a/0/4/1: interior = 01 01 00 00 <nameLen> <name> <u16 landId>
  - zone events NARC a/0/3/2: 4 count-prefixed arrays; empty map = 16 zero bytes
"""
import struct
import sys

import ndspy.narc

BASE = "base"
ARM9 = f"{BASE}/arm9.bin"
MATRIX_NARC = f"{BASE}/root/a/0/4/1"
EVENTS_NARC = f"{BASE}/root/a/0/3/2"
MAPNAME_TBL = f"{BASE}/root/fielddata/maptable/mapname.bin"

HEADER_TABLE_OFF = 0xF6BE0
HEADER_SIZE = 24
NUM_HEADERS = 540

DONOR_HEADER = 333        # MAP_BELL_TOWER_3F
TARGET_HEADER = 538       # MAP_UNUSED (verified dead: matrix 0, null scripts)
DONOR_LAND_DATA = 438     # Bell Tower 3F model+collision, reused as-is

EXPECT_MATRIX_COUNT = 288
EXPECT_EVENTS_COUNT = 491

ENC_NONE = 0xFF


def load_narc(path):
    with open(path, "rb") as f:
        return ndspy.narc.NARC(f.read())


def save_narc(narc, path):
    with open(path, "wb") as f:
        f.write(narc.save())


def main():
    # --- matrix: append a 1x1 interior pointing at the donor land data
    matrix = load_narc(MATRIX_NARC)
    if len(matrix.files) != EXPECT_MATRIX_COUNT:
        sys.exit(f"skypillar: matrix NARC has {len(matrix.files)} entries, expected {EXPECT_MATRIX_COUNT}")
    name = b"skypilr01_"
    matrix.files.append(bytes([1, 1, 0, 0, len(name)]) + name + struct.pack("<H", DONOR_LAND_DATA))
    new_matrix_id = len(matrix.files) - 1
    save_narc(matrix, MATRIX_NARC)

    # --- events: append an empty zone-event blob (no warps; Escape Rope exits)
    events = load_narc(EVENTS_NARC)
    if len(events.files) != EXPECT_EVENTS_COUNT:
        sys.exit(f"skypillar: events NARC has {len(events.files)} entries, expected {EXPECT_EVENTS_COUNT}")
    events.files.append(b"\x00" * 16)
    new_events_id = len(events.files) - 1
    save_narc(events, EVENTS_NARC)

    # --- header: clone donor, retarget matrix/events, strip encounters
    with open(ARM9, "rb") as f:
        arm9 = bytearray(f.read())
    donor_off = HEADER_TABLE_OFF + DONOR_HEADER * HEADER_SIZE
    target_off = HEADER_TABLE_OFF + TARGET_HEADER * HEADER_SIZE
    header = bytearray(arm9[donor_off:donor_off + HEADER_SIZE])
    if header[6] != 0x8B or header[7] != 0x00:
        sys.exit(f"skypillar: donor header sanity check failed (scriptsBank bytes {header[6]:02x}{header[7]:02x}, expected 8b00) — arm9 layout changed?")
    header[0] = ENC_NONE
    struct.pack_into("<H", header, 0x04, new_matrix_id)
    struct.pack_into("<H", header, 0x10, new_events_id)
    arm9[target_off:target_off + HEADER_SIZE] = header
    with open(ARM9, "wb") as f:
        f.write(arm9)

    # --- internal name table (cosmetic, keeps tooling sane)
    with open(MAPNAME_TBL, "rb") as f:
        names = bytearray(f.read())
    if len(names) != NUM_HEADERS * 16:
        sys.exit(f"skypillar: mapname.bin is {len(names)} bytes, expected {NUM_HEADERS * 16}")
    names[TARGET_HEADER * 16:(TARGET_HEADER + 1) * 16] = b"SKYPILR01".ljust(16, b"\x00")
    with open(MAPNAME_TBL, "wb") as f:
        f.write(names)

    print(f"skypillar: header {TARGET_HEADER} <- donor {DONOR_HEADER}, matrix {new_matrix_id}, events {new_events_id}, land {DONOR_LAND_DATA}, encounters off")


if __name__ == "__main__":
    main()
