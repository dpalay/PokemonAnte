"""Parse game/pokefirered.map for symbol addresses and per-object code ranges.

Because the ROM builds from source, every global's address is in the linker
map. The bot reads game state directly instead of screen-scraping, and it
identifies the current screen by which object file gMain.callback2 points
into (e.g. naming_screen.o, overworld.o, battle_main.o).
"""
import re
import sys

SYM_RE = re.compile(r"^\s+0x([0-9a-f]{8,16})\s+(\w+)\s*(?:=.*)?$")
OBJ_RE = re.compile(r"^ \S+\s+0x([0-9a-f]{8,16})\s+0x([0-9a-f]+)\s+(\S+\.o)$")


class MapFile:
    def __init__(self, path):
        self.symbols = {}
        self.ranges = []  # (start, end, objname)
        with open(path) as f:
            for line in f:
                m = OBJ_RE.match(line)
                if m:
                    start = int(m.group(1), 16)
                    size = int(m.group(2), 16)
                    if 0x08000000 <= start < 0x0A000000 and size > 0:
                        self.ranges.append((start, start + size, m.group(3)))
                    continue
                m = SYM_RE.match(line)
                if m:
                    self.symbols.setdefault(m.group(2), int(m.group(1), 16))
        self.ranges.sort()

    def sym(self, name):
        return self.symbols[name]

    def object_of(self, addr):
        addr &= ~1  # strip thumb bit
        lo, hi = 0, len(self.ranges)
        while lo < hi:
            mid = (lo + hi) // 2
            start, end, name = self.ranges[mid]
            if addr < start:
                hi = mid
            elif addr >= end:
                lo = mid + 1
            else:
                return name
        return "?"


if __name__ == "__main__":
    mf = MapFile(sys.argv[1] if len(sys.argv) > 1 else "../../game/pokefirered.map")
    for name in ("gMain", "gTestInputMailbox", "gPlayerParty", "gPlayerPartyCount",
                 "gSaveBlock1Ptr"):
        print(f"{name} = {mf.sym(name):#x}")
    print(f"{len(mf.ranges)} object ranges")
