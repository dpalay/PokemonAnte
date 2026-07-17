# Pokémon Ante — Implementation Plan

Companion to DESIGN.md. Phases are vertical slices: each ends with
something playable, ordered so the riskiest systems get proven first.

## Foundation

- **Base: [pret/pokefirered](https://github.com/pret/pokefirered)** (the
  Fire Red decompilation). Every system in the design — battle hooks,
  save-layout changes, dynamic parties, custom shops and UIs — is ordinary
  C there and impractical as binary patching.
- **Repo layout:** the decomp is vendored at `game/` (snapshot of
  pret/pokefirered; pinned commit recorded in `game/UPSTREAM.txt`), so
  hack code is edited in-tree while design docs stay at the repo root.
- **Toolchain:** agbcc + devkitARM per pokefirered's INSTALL.md; verify a
  byte-matching vanilla build first, then diverge.
- **CI:** GitHub Action that builds the ROM from source on every push.
  **Never commit built ROMs or the original game's assets beyond what the
  pret repo itself carries.** Distribution, if ever, is patch files
  (UPS/BPS) only.
- **Testing:** mGBA for manual play; a `tests/saves/` directory of crafted
  save states for each system (full binder, one-Pokémon floor, held
  bounty, ghost flag set, etc.); a debug menu (dev builds only) to set
  badges, grant Pokémon, force ante draws, and fire migrations.

## Save-RAM budget (the binding constraint)

New persistent state, designed to fit in reclaimable SaveBlock slack:

| System | Layout | Size |
|--------|--------|------|
| Bounty list | 24 records × 20 B (species, personality, OT id, packed IVs/nature, holder trainer id, flags, badge-at-loss) | 480 B |
| The Book | 8 decks × 6 species ids (2 B) + head/meta | ~104 B |
| Binder | reuses one existing PC box (30 slots) — page cap is UI-enforced | 0 B |
| Ghost/impound flags | bitfields + impound holder refs | ~16 B |
| Trainer beat-counts | already needed for Vs Seeker-style rematches; 1 B × ~250 tracked trainers, packed | ~250 B |
| Misc (mode flags, migration timers, Fund state) | | ~32 B |
| **Total new** | | **< 1 KB** |

NPC binders and restocked decks are **never stored**: they generate
deterministically from `trainer_id + times_beaten` (plus counter-pick
weighting inputs read at battle setup). Only true bounty records — real
player Pokémon in NPC hands — occupy save RAM.

## Phases

### Phase 0 — Scaffold
Import pokefirered; reproduce a matching vanilla build locally and in CI;
add the debug menu skeleton.
**Exit:** green CI building an unmodified ROM.

### Phase 1 — Core ante loop (the proof of concept)
- Battle-start hook: draw one ante per side from party, reveal message
  before turn one; autosave at commit.
- Battle-end hook: transfer — player win adds the NPC ante (party if room,
  else box); loss removes the drawn Pokémon and writes a bounty record.
- Loss rules: no money penalty, trainer not marked beaten, whiteout keeps
  rematch open.
- Disable wild encounters and catching.
- Ownership-floor check gates ante battles; friendly-battle fallback.
**Exit:** Route 1 → Viridian playable end-to-end on ante rules alone.
This phase proves the concept is fun before anything else is built.

### Phase 2 — Format level
- Set-level battle transform at load (both sides to badge-table level),
  reusing the Battle Tower set-level pathway.
- Remove EXP gain; auto-moveset generation at format level; evolution
  thresholds with per-Pokémon opt-out; trade evos → stone items.
- HMs as key items; field-move gates check badge + item.
**Exit:** badges 0–2 arc playable with no grinding anywhere.

### Phase 3 — Deck, binder, and floor UI
- PC rework: binder = 30-slot box with page-purchase gating (12 → 30).
- Deck-select menu (choose X ∈ [1,6]); ante generalized to 1-in-X.
- Full-binder overflow flow (sell on the spot / make room).
- Nickname prompt on every acquisition.
**Exit:** the risk-dial loop (lean deck vs. safe deck) playable.

### Phase 4 — NPC economy: bounty chase and restocking
- Seeded NPC binder/deck generation from encounter tables; trophy-bias
  weighting pulls held bounties into decks (75%).
- Vs Seeker extension: restock on rematch, beat-count persistence.
- Bounty Board UI on the dex: holder, location, deck-activity hint.
- Migration timers + Board logging; Rocket-ward drift for repossession.
**Exit:** lose a Pokémon, chase it, reclaim it — the emotional core loop.

### Phase 5 — Shops and packs
- Pack items + opening UI; rarity mapping (BST/stage); route-table
  sourcing gated on story progress; themed packs behind badges.
- Collector sell-back; singles market (post-6 badges); Professor's Fund
  pity flow.
- Price table from DESIGN.md as one tuning header file.
**Exit:** full economy loop: win → sell → buy → open.

### Phase 6 — Information systems
- Scouting UI: free lookup, report, dossier; Broker NPC with called
  shots (forced-ante battle flag + fee) and double-ante tables.
- The Book: exhibition-deck ring buffer, Trainer Card page, counter-pick
  weighting by trainer class reading Book + binder.
- Faction flags: Rocket falsified data + true-file counter-picks;
  Unbound blanks.
**Exit:** scouting/bluffing visibly changes NPC decks across a gym run.

### Phase 7 — Story and factions content
Script and content pass, in story order:
- Ban-era dialogue rewrite; Oak/Registry scenes; Fund and ranch.
- Gym exhibition variants (8 battle-setup rules) and E4 stances.
- Rival bounty scripting across acts; pot finale at the Hall of Fame.
- Rocket: rigged-draw flags, proxy-bounty sidequests, counterfeit-pack
  quests, Game Corner tables.
- Off-ledger arc (ghost flag, audits, impound, restore-vs-keep choice).
- Unbound encounters (telegraphed permadeath, item stakes, rip scene).
- Mewtwo self-ante set piece.
**Exit:** full campaign playable start to Hall of Fame.

### Phase 8 — Modes, balance, polish
- Hardcore toggle bundle; casual defaults.
- Tuning passes against the DESIGN.md tables (format curve, prices,
  trophy bias, migration clocks, counter-pick curve, Book weights).
- Playtest cycles focused on the two failure modes: death spiral and
  loss-aversion reset behavior.
**Exit:** release-candidate patch file.

## Risk register

| Risk | Mitigation |
|------|------------|
| Per-gym battle-rule variants touch the battle engine 8 different ways | Implement as one "ante ruleset" struct consumed at battle setup; each gym is data, not code |
| UI work (binder, packs, scouting, Board) is the schedule long pole | All UIs reuse existing menu frameworks (PC, Mart, Trainer Card); no bespoke widget systems |
| Save slack insufficient on real hardware | Budget above is < 1 KB against known reclaimable space; verify in Phase 1, not Phase 7 |
| Format level makes battles samey | TMs, held items, and species rarity carry customization; revisit in Phase 8 tuning |
| The core loop just isn't fun | Phase 1 exists to answer this before anything else is built — kill or pivot cheaply |

## Sequencing note

Phases 1–4 are the game; 5–6 are the economy and meta around it; 7 is
content; 8 is polish. Nothing in 5+ should begin until Phase 4's exit
criterion — *losing a Pokémon and winning it back feels good* — has been
play-verified, because every later system leans on that loop.
