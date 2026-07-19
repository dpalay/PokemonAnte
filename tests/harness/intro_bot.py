#!/usr/bin/env python3
"""Plays a fresh save through the Pokémon Ante opening and reports checks.

Drives the game through mGBA's GDB stub using the in-ROM input mailbox
(gTestInputMailbox in src/main.c). Screen detection is by which object file
gMain.callback2 points into; game state is read straight from RAM via the
linker map. Run via run_intro.sh.

Exit code 0 = intro completed and all checks passed.
"""
import sys
import time

from addrs import MapFile
from gdb_rsp import GdbClient

# GBA key masks
A, B, SELECT, START = 1, 2, 4, 8
RIGHT, LEFT, UP, DOWN = 0x10, 0x20, 0x40, 0x80

MAGIC = 0x414E5445  # 'ANTE'

# struct offsets (see include/main.h, include/global.h)
MAIN_CALLBACK2 = 4
SB1_POS_X, SB1_POS_Y = 0, 2
SB1_MAP_GROUP, SB1_MAP_NUM = 4, 5
SB1_ANTE = 0x348C          # struct AnteSaveData
ANTE_BOUNTIES = 4          # 16 × 24-byte records
BOUNTY_SIZE, BOUNTY_COUNT = 24, 16
MON_SIZE = 100
MON_NICKNAME = 8           # plaintext in BoxPokemon header

# maps (group, num)
HOUSE_2F, HOUSE_1F = (4, 1), (4, 0)
PALLET = (3, 0)
LAB = (4, 3)

# waypoints: (map) -> target tile
WAYPOINTS = {
    HOUSE_2F: (10, 2),   # stairs down
    HOUSE_1F: (4, 8),    # front door
    PALLET: (12, 1),     # Oak trigger at the grass edge
}
LAB_BALL = (9, 5)        # stand below the middle ball, face up
LAB_EXIT = (6, 12)

CHARMAP = {0xFF: "", 0x00: " "}
for i, c in enumerate("ABCDEFGHIJKLMNOPQRSTUVWXYZ"):
    CHARMAP[0xBB + i] = c
for i, c in enumerate("abcdefghijklmnopqrstuvwxyz"):
    CHARMAP[0xD5 + i] = c
for i, c in enumerate("0123456789"):
    CHARMAP[0xA1 + i] = c


def decode(raw: bytes) -> str:
    out = []
    for b in raw:
        if b == 0xFF:
            break
        out.append(CHARMAP.get(b, "?"))
    return "".join(out)


class IntroBot:
    def __init__(self, mapfile, host, port):
        self.m = MapFile(mapfile)
        self.gdb = GdbClient(host, port)
        self.mailbox = self.m.sym("gTestInputMailbox")
        self.gmain = self.m.sym("gMain")
        self.party = self.m.sym("gPlayerParty")
        self.party_count = self.m.sym("gPlayerPartyCount")
        self.sb1ptr = self.m.sym("gSaveBlock1Ptr")
        self.naming_visits = 0
        self.naming_step = 0
        self.checks = []
        self.log_state = None

    # -- input ---------------------------------------------------------------
    def press(self, keys, hold_polls=1):
        self.gdb.write_u32(self.mailbox, MAGIC)
        self.gdb.write_u16(self.mailbox + 4, keys)
        self._resume(hold_polls * 0.12)
        # release between presses so newKeys edges fire
        self.gdb.write_u16(self.mailbox + 4, 0)
        self._resume(0.08)

    def _resume(self, seconds):
        self.gdb.cont()
        time.sleep(seconds)
        self.gdb.interrupt()

    # -- state ---------------------------------------------------------------
    def screen(self):
        cb2 = self.gdb.read_u32(self.gmain + MAIN_CALLBACK2)
        return self.m.object_of(cb2)

    def sb1(self):
        return self.gdb.read_u32(self.sb1ptr)

    def where(self):
        sb1 = self.sb1()
        if not (0x02000000 <= sb1 < 0x02040000):
            return None
        x = self.gdb.read_u16(sb1 + SB1_POS_X)
        y = self.gdb.read_u16(sb1 + SB1_POS_Y)
        grp = self.gdb.read_u8(sb1 + SB1_MAP_GROUP)
        num = self.gdb.read_u8(sb1 + SB1_MAP_NUM)
        return (grp, num), x, y

    def party_size(self):
        return self.gdb.read_u8(self.party_count)

    def nicknames(self):
        n = self.party_size()
        return [decode(self.gdb.read_mem(self.party + i * MON_SIZE + MON_NICKNAME, 10))
                for i in range(n)]

    def bounties(self):
        base = self.sb1() + SB1_ANTE + ANTE_BOUNTIES
        out = []
        for i in range(BOUNTY_COUNT):
            rec = self.gdb.read_mem(base + i * BOUNTY_SIZE, BOUNTY_SIZE)
            species = int.from_bytes(rec[0:2], "little")
            if species:
                out.append((species, decode(rec[13:23])))
        return out

    def ante_committed(self):
        return self.gdb.read_u8(self.sb1() + SB1_ANTE)

    # -- behaviors -----------------------------------------------------------
    def do_naming_screen(self):
        # Type one character (varied per visit so uniqueness rejections
        # resolve), then START to jump to OK, then A to confirm.
        if self.naming_step == 0:
            self.naming_visits += 1
            for _ in range(self.naming_visits % 5):
                self.press(RIGHT)
            self.press(A)
            self.naming_step = 1
        elif self.naming_step == 1:
            self.press(START)
            self.naming_step = 2
        else:
            self.press(A)

    def do_overworld(self, tick):
        # Alternate movement with A taps: scripted cutscenes lock movement
        # but still need dialogue advanced, and the tap is harmless while
        # walking.
        if tick % 2:
            self.press(A)
            return
        loc = self.where()
        if loc is None:
            self.press(A)
            return
        (gm, x, y) = loc
        if gm == LAB:
            if self.party_size() == 0:
                tx, ty = LAB_BALL
                if (x, y) == (tx, ty):
                    self.press(UP)
                    self.press(A, 2)
                    return
            else:
                tx, ty = LAB_EXIT
        elif gm in WAYPOINTS:
            tx, ty = WAYPOINTS[gm]
        else:
            self.press(A)
            return
        if x < tx:
            self.press(RIGHT, 3)
        elif x > tx:
            self.press(LEFT, 3)
        elif y < ty:
            self.press(DOWN, 3)
        elif y > ty:
            self.press(UP, 3)
        else:
            self.press(A, 2)  # on target: interact/advance

    # -- checks --------------------------------------------------------------
    def check(self, name, ok, detail=""):
        self.checks.append((name, ok))
        print(f"CHECK {name}: {'PASS' if ok else 'FAIL'} {detail}", flush=True)

    def final_checks(self):
        n = self.party_size()
        names = self.nicknames()
        bounties = self.bounties()
        # Won the rival ante: 3 + his stake = 4, no bounties.
        # Lost it: 3 - stake = 2, one bounty (3 again after a Fund rescue).
        # 3 with no bounties would mean settlement never ran — the bug case.
        valid = (n == 4 and len(bounties) == 0) or (n in (2, 3) and len(bounties) == 1)
        self.check("rival_ante_settled", valid,
                   f"party={n} bounties={bounties} "
                   "(4/0=won, 2/1=lost, 3/1=lost+Fund rescue, 3/0=BUG)")
        self.check("names_unique_nonempty",
                   len(names) == len(set(names)) and all(names),
                   f"names={names}")
        self.check("ante_not_committed", self.ante_committed() == 0)

    # -- main loop -----------------------------------------------------------
    def run(self, budget=600):
        print("BOT: waiting for boot…", flush=True)
        self.gdb.cont()
        time.sleep(4)
        self.gdb.interrupt()
        deadline = time.time() + budget
        battle_seen = False
        settle_countdown = -1
        tick = 0
        while time.time() < deadline:
            tick += 1
            scr = self.screen()
            if tick % 40 == 0:
                print(f"BOT: tick={tick} screen={scr} pos={self.where()} "
                      f"party={self.party_size()}", flush=True)
            if scr != self.log_state:
                pos = self.where()
                print(f"BOT: screen={scr} pos={pos} party={self.party_size()}",
                      flush=True)
                self.log_state = scr
            if "naming_screen" in scr:
                self.do_naming_screen()
                continue
            self.naming_step = 0
            if "battle_main" in scr or "battle_controller" in scr:
                battle_seen = True
                self.press(A, 2)
                continue
            if "overworld" in scr or "field" in scr:
                if battle_seen and settle_countdown < 0:
                    # battle done: let post-battle scripts (registration,
                    # whiteout, Fund rescue) play out before asserting
                    settle_countdown = 30
                if settle_countdown > 0:
                    settle_countdown -= 1
                if settle_countdown == 0:
                    print("BOT: rival battle settled — running checks", flush=True)
                    self.final_checks()
                    return all(ok for _, ok in self.checks)
                self.do_overworld(tick)
                continue
            self.press(A)
        print("BOT: TIMEOUT — last state dump follows", flush=True)
        print(f"  screen={self.screen()} pos={self.where()} "
              f"party={self.party_size()} names={self.nicknames()}", flush=True)
        return False


if __name__ == "__main__":
    mapfile = sys.argv[1] if len(sys.argv) > 1 else "../../game/pokefirered.map"
    port = int(sys.argv[2]) if len(sys.argv) > 2 else 2345
    bot = IntroBot(mapfile, "127.0.0.1", port)
    ok = bot.run()
    print(f"SUMMARY: {'ALL PASS' if ok else 'FAILURES OR INCOMPLETE'}", flush=True)
    sys.exit(0 if ok else 1)
