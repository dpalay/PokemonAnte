# Pokémon Ante — Design Document

A Fire Red ROM hack built on the old Magic: The Gathering ante rule. Every
trainer battle is played for keeps: each side stakes one Pokémon, and the
winner takes the loser's stake.

## Core premise (from README)

- No catching wild Pokémon (possibly no wild encounters at all)
- Before a trainer battle, 1 of your party of 6 is randomly drawn as your ante
- Winner of the battle gets the loser's ante
- Trainers you defeat refill their teams from the local wild encounter table
- Trainers who beat you keep your Pokémon and may use it against you later
- A store sells "packs" of Pokémon

## The core insight

"Beaten trainers refill from the wild encounter table" + Fire Red's Vs Seeker
means wild encounter tables don't disappear — they move *inside trainer
parties*. Route trainers become a renewable source of local species, gated
behind winning battles instead of throwing balls. This is the regeneration
engine, grinding loop, and catch-up mechanic all in one.

## Three make-or-break problems

### 1. The reset problem
Players will soft-reset to dodge losses. Mitigations (use both):
- Autosave the instant antes are committed at battle start — the potential
  loss is saved before the fight resolves (roguelike-style).
- Make losses recoverable (revenge rematches, below) so resetting feels
  less necessary.

### 2. The softlock problem
With no catching, the player can hit zero Pokémon. Fixes:
- MTG-style floor: trainers only accept ante battles if the player has 2+
  Pokémon total (party + PC). At 1, all battles are friendly (no ante,
  reduced prize money).
- Charity floor: Oak provides a free basic pack if the player is broke and
  down to one mon.

### 3. The ante-fodder ("mule") problem
Uniform random draw from 6 invites carrying junk mons as sacrificial padding.
Fix: **value-matched stakes** — after the player's ante is drawn, the
trainer's ante is drawn from their team at a comparable value tier
(level × base stat total). Stake a Rattata, play for their Rattata.
A junk deck antes junk. Self-balancing, no hard rules needed.

## Core ruleset

- **Symmetric ante, revealed at battle start.** Both sides draw one; both are
  shown before turn one. Winner takes the loser's ante.
- **The anted mon fights.** (Deliberate break from MTG, where the ante card
  left the deck.) Your staked mon can end up fighting for its own freedom,
  and padding your team with junk actively weakens you.
- **Losing costs exactly the ante, never more.** The ante transfer replaces
  the whiteout money penalty. Fleeing/conceding forfeits the ante but
  protects the rest of the team (= scooping in MTG).
- **Revenge weighting.** A trainer holding a mon won from you has their ante
  draw forced (or heavily weighted) toward that mon on rematch. It appears
  in their team slightly leveled — they've been training it.
- **OT and nicknames persist.** Won mons keep their original trainer; the
  traded-mon XP boost applies naturally. Winning back your own mon restores
  it fully.
- **HMs become key items.** Progression must never depend on a specific mon
  the player might lose. Badge + HM item = field move usable.

## Structure and story

- **Rival as emotional spine:** mons he wins off you reappear in later
  scripted fights, trained and evolved.
- **Gym leaders ante their signature mon** (replaces gift Pokémon as marquee
  rewards). Each badge also unlocks that gym's themed pack in stores.
- **Endgame — Redemption:** the League holds "the pot": every mon the player
  lost and never reclaimed. Beating the Champion returns all of them.
- **Level caps per badge** (candidate) to prevent Vs Seeker snowballing.

## Pack store economy

- Tiered like TCG sets: cheap Base packs early; area-themed packs
  (Mt. Moon pack: Geodude/Zubat/Paras/fossils); gym packs behind badges.
- Rarity slots: 3 commons / 1 uncommon / 1 rare, small legendary-adjacent
  odds late game.
- **Singles market** late game: pay a premium for an exact species.
- Prize money roughly doubled — battles are the only income, packs the only
  sink.

## MTG flavor ports

- **High rollers** ante 2 (Contract from Below); some trainers stake
  TMs/items instead of mons (Timmerian Fiends).
- **Darkpact** (consumable, rare): after the reveal, swap your drawn ante
  for another party member.
- **Amulet of Quoz** NPC gimmick: skip the fight, coin-flip for the antes.
- **Bound Ribbon:** excludes holder from the ante draw — unique or
  single-use only, or stakes die.
- Key items named after the nine ante cards (Bronze Tablet, Jeweled Bird,
  Tempest Efreet, ...).

## Technical direction

Build on the **pret/pokefirered decompilation**, not binary patching.
Battle-end hooks, persistent per-trainer state, dynamic trainer parties, and
custom shops are ordinary C in the decomp. Hardest piece: trainer
persistence (~10–16 bytes of save data per remembered mon). Fire Red's save
block has slack for full persistence on notable trainers + beat-counts for
generic trainers; the decomp allows reshaping the save layout.

## Open decisions

1. Does the anted mon fight (recommended) or sit out (MTG-pure)?
2. Can the starter be anted? (Lean yes, with the 2-mon floor as safety net.)
3. Enforce autosave-on-ante, or leave save-scumming to player honor?
4. Persistence scope: all trainers, or full memory only for rival / gym
   leaders / recurring NPCs?
