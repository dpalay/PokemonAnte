# Instructions for Claude Code: running and extending the playtest harness

You are working with an automated playtester for a Fire Red ROM hack.
Read `README.md` in this directory first for the architecture.

## Standard playtest workflow

1. Build the ROM: `cd game && make -j$(nproc)` (needs agbcc installed per
   `game/INSTALL.md`; if `pokefirered.gba` and `pokefirered.map` already
   exist and are current, skip).
2. Run: `cd tests/harness && ./run_intro.sh`. Give it up to ~10 minutes.
3. Read `intro_test.log`. Report to the user:
   - Each `CHECK` line and whether the run reached `SUMMARY: ALL PASS`.
   - On `TIMEOUT`, the last `BOT: screen=… pos=…` lines show where the
     run stalled — report the screen/position so the stall is locatable.

## Interpreting failures

- `rival_ante_settled … 3/0=BUG`: the ante settlement did not transfer
  any Pokémon after the rival battle. This is a game bug — investigate
  `Ante_HandleTrainerBattleEnd` and its callers in `game/src/battle_setup.c`.
- `names_unique_nonempty` failing: the Registry naming enforcement has a
  hole — check `AnteIsPartyMonNameValid` / `EnsureUniqueNickname` in
  `game/src/ante.c`.
- Bot stalls on `naming_screen`: the typing sequence in
  `do_naming_screen` may be fighting a dialog; add a state print and
  adjust the key sequence.
- Bot stalls in overworld at fixed coordinates: it is walking into an
  obstacle. Adjust the waypoint table (`WAYPOINTS`) — add intermediate
  waypoints rather than making movement smarter.

## Extending

- The bot is a state machine over "which object file is gMain.callback2
  in" — keep it that way; it is robust against timing.
- Prefer memory assertions over more gameplay: any global in
  `game/pokefirered.map` is readable. Party mons are 100 bytes each at
  `gPlayerParty`; nicknames (offset 8) are plaintext; the ante ledger is
  at `*gSaveBlock1Ptr + 0x348C` (committed flag, then 16 × 24-byte
  bounty records).
- When the save layout in `include/global.h` changes, update the offset
  constants at the top of `intro_bot.py`.
- Keep runs headless (`SDL_VIDEODRIVER=dummy`). If you need to *watch* a
  run for debugging, unset it and mGBA opens a window; the bot drives
  regardless of what the real keyboard does once the mailbox is active.

## Ground rules

- Never commit `intro_test.log`, save files, or the ROM.
- The input mailbox in `src/main.c` must stay inert without its magic
  word; do not repurpose it for gameplay features.
- If you change game logic to fix a failing check, re-run the harness to
  confirm before reporting the fix.
