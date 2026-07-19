# Pokémon Ante — automated playtest harness

Drives the game through mGBA's GDB stub and asserts on game state read
directly from memory. No screen-scraping: because the ROM builds from
source, `game/pokefirered.map` gives the address of every global, and the
current screen is identified by which object file `gMain.callback2` points
into.

## Requirements

- **mGBA** (any 0.10+ build with the GDB stub — all standard builds have it)
- **Python 3.8+** (stdlib only)
- A built ROM + map file (`make` in `game/`, or download the ROM from the
  repo's rolling `latest-build` release — but then you still need the
  matching `.map`, so building locally is strongly preferred)

## Running the intro test

```sh
cd tests/harness
./run_intro.sh                # builds nothing; expects game/pokefirered.gba
```

The bot plays a fresh save through: intro → naming screens → bedroom →
Pallet Town → Oak's escort → starter choice → Starter Pack pulls (with
mandatory Registry naming) → the rival ante battle → settlement. It then
prints `CHECK <name>: PASS/FAIL` lines and a `SUMMARY`, and exits nonzero
on failure. Full output is mirrored to `intro_test.log`.

Checks include: the rival ante actually settled (party 4 + no bounties on
a win; party 2–3 + one bounty on a loss; party 3 + no bounties = the
settlement-never-ran bug), nickname uniqueness, and the committed-ante
flag clearing.

## How input works

`src/main.c` compiles in a small mailbox (`gTestInputMailbox`). When the
magic word `0x414E5445` ("ANTE") is written to it over the GDB stub, the
`keys` field replaces the real joypad every frame. The bot writes key
masks, resumes emulation briefly, re-interrupts, and reads state — a
closed loop at roughly 5–10 decisions per second. Without the magic word
the mailbox is inert and the game plays normally.

## Architecture

| File | Role |
|------|------|
| `gdb_rsp.py` | Minimal GDB remote-protocol client (read/write memory, interrupt/continue) |
| `addrs.py` | Linker-map parser: symbol addresses + per-object code ranges |
| `intro_bot.py` | The opening-sequence driver and its assertions |
| `run_intro.sh` | Launches headless mGBA (`SDL_VIDEODRIVER=dummy`) + the bot |

Struct offsets (party mon layout, SaveBlock1 ante offset) are declared at
the top of `intro_bot.py` and must be kept in sync with `include/global.h`
if the save layout changes.
