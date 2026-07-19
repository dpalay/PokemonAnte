#!/usr/bin/env bash
# Launch mGBA headless with the GDB stub and drive the intro bot against it.
# Usage: ./run_intro.sh [path-to-rom] [path-to-mapfile]
set -u
cd "$(dirname "$0")"

ROM="${1:-../../game/pokefirered.gba}"
MAP="${2:-../../game/pokefirered.map}"
PORT="${GDB_PORT:-2345}"
MGBA="${MGBA:-$(command -v mgba || echo /usr/games/mgba)}"

[ -f "$ROM" ] || { echo "ROM not found: $ROM (build with 'make' in game/)"; exit 2; }
[ -f "$MAP" ] || { echo "Map file not found: $MAP"; exit 2; }

# Headless: dummy video/audio drivers; no throttling worries, the bot paces itself.
SDL_VIDEODRIVER="${SDL_VIDEODRIVER:-dummy}" SDL_AUDIODRIVER="${SDL_AUDIODRIVER:-dummy}" \
  "$MGBA" -g -l 0 "$ROM" &
MGBA_PID=$!
trap 'kill $MGBA_PID 2>/dev/null' EXIT
sleep 2

python3 intro_bot.py "$MAP" "$PORT" | tee intro_test.log
RESULT=${PIPESTATUS[0]}
exit "$RESULT"
