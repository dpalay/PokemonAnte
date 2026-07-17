# Pokémon Ante — Design Document

A Fire Red ROM hack built around Magic: The Gathering's original **ante** rule:
before a match, each player sets aside a random card from their deck; the winner
takes the opponent's ante.

## Premise

- **No wild catching.** Wild encounters are removed (or replaced — see Packs).
- **Every trainer battle is an ante battle.** Each side randomly stakes one
  Pokémon from their party; winner keeps their own ante and takes the loser's.
- **Trainers keep what they win.** Lose your Charizard and that Bug Catcher owns
  it now — and may field it against you next time.
- **Beaten trainers restock.** Rematch a trainer enough times and their roster
  refills with random pulls from the local wild encounter tables.
- **Packs.** A shop sells booster packs of Pokémon, pulled from encounter
  tables of routes you've reached.

## The central design problem: the death spiral

Ante is inherently rich-get-richer: losing makes you weaker, which makes you
likelier to lose again. In single-player this can soft-lock a save. Every rule
below is tuned to make losses sting without spiraling.

### Anti-spiral kit

1. **Losses are bounties, not deletions.** A trainer who takes your Pokémon
   keeps it and can use it. But the rematch always stays open, and when you
   beat a trainer who holds one of your former Pokémon, *that* Pokémon is
   their ante. A **Bounty Board** (Pokédex-style menu) tracks who holds what
   and where they are.
2. **Party floor.** You cannot enter an ante battle with fewer than 2 party
   Pokémon. At 1, you're directed to the pack shop; a pity-priced **Starter
   Pack** is always affordable.
3. **Badge-scaled levels.** All Pokémon — yours and won antes — are capped or
   scaled to a level determined by badge count. Critical rule: it makes every
   won Pokémon immediately viable (churn is fun, not punishing) and replaces
   grinding entirely, which no-wild-encounters would otherwise break.

### The reset problem (the spiral's mirror image)

The anti-spiral kit only matters if losses actually stick: players will
soft-reset before a loss saves. Countermeasures:

- **Autosave when antes are committed** at battle start, before the fight
  resolves (roguelike-style) — the potential loss is already on disk.
- The bounty loop is also anti-reset by design: a loss that stays winnable
  is a loss players are more willing to keep.
- Enforcement level (hard autosave vs. player honor) is an open question —
  it decides whether this is a hardcore hack or a casual one.

## Ante mechanics

- **Symmetric ante, MTG-style.** At battle start, both sides randomly reveal
  one party Pokémon as their ante. Winner takes the opponent's ante and keeps
  their own.
- **The ante still fights.** Unlike MTG (where the anted card leaves the deck),
  the staked Pokémon participates — it's fighting for its freedom. Keeps
  battles 6v6 and maximizes drama when your ace is drawn.
- **Reveal before turn 1.** Both antes are shown at battle start, before the
  first move. The tension of "my starter is on the line" is the whole game.
- **Party = deck, PC = binder.** Ante draws only from the party of 6. The PC
  box is your collection — perfectly safe, but boxed Pokémon can't fight.
  This is the *only* ante protection in the game. No ante-proof slots, no
  protected starter: strength requires exposure. The bounty loop is the
  humane version of protection.
- **OT and nicknames persist.** Won Pokémon keep their original trainer and
  nickname — your binder becomes a trophy wall of defeated trainers' aces,
  and the traded-Pokémon XP boost applies naturally. Winning back your own
  Pokémon restores it fully.

## Lore: the League banned ante (like Wizards did)

Historically, WotC banned ante from sanctioned play. Mirror it:

- The **Pokémon League has banned ante battles**. Gym and League matches are
  "sanctioned." Route trainers play by the old rules — ante is simply how the
  region battles.
- **Gyms opt back in as prestige exhibitions:** each leader stakes a
  guaranteed rare, gym-themed Pokémon. Gyms are jackpot fights, not the one
  place the mechanic disappears. (Sanctioned/no-ante status is reserved as a
  safety valve for battles that could soft-lock the story: first rival fight,
  forced tutorial battles.)
- **Team Rocket are ante sharks** — rigged high-stakes games, "repossession"
  of Pokémon. Villain reframe with minimal map changes.
- **The rival is the bounty system's showcase.** Pokémon the rival wins from
  you reappear in later scripted fights, trained and evolved — "your
  Charmander, now their Charizard" is the emotional spine of the story.
  Whether the rival can hold a bounty for a whole act is an open question.

## Economy

- **Packs** pull from wild encounter tables of routes reached so far,
  preserving Kanto's route-progression species curve. TCG rarity tiers
  (common/uncommon/rare/holo) mapped to base-stat total and evolution stage.
  Badge-gated themed packs.
- **Collector NPC buys Pokémon.** Converts duplicate ante winnings into money
  into packs. Without sell-back, the PC is a landfill and winning feels
  worthless.
- **Ante-manipulation items** (homage to MTG's ante cards), sold at the shop:
  - *Jeweled Charm* (Jeweled Bird): held item; if the holder is drawn as ante
    and the battle is lost, the stake converts to money paid to the winner.
    Consumed on trigger, priced against the holder's value — a costly
    one-shot escape valve, not standing protection (see rationale: the PC
    remains the only *free* protection).
  - *Broker* NPC (Demonic Attorney): offers double-ante, double-stakes
    rematches.
  - *Redraw Contract* (Contract from Below): consumable; re-draw your ante
    once, at a price.

## Core loop

Vs Seeker is the engine. Rematch → trainer restocks from wild tables → farm
antes → sell duplicates → buy packs → build the binder. Trainer rematches
fully replace wild encounters as the source of both species and progress.

## Rulings on edge cases

- **Whiteout:** losing does NOT mark a trainer beaten — rematches must stay
  open or lost Pokémon become unrecoverable. Ante loss replaces the vanilla
  half-money penalty.
- **HMs:** no catching means no HM mules. Remove field-HM gates or convert
  HMs to key-item abilities.
- **Eggs / in-game trades / event Pokémon:** cut. Packs cover acquisition.
- **Link battles:** out of scope.

## Technical notes (Fire Red)

- Build on the **pret/pokefirered decomp**, not binary hacking — dynamic
  trainer parties, ante logic, and the pack shop are all C-level features.
- **Save RAM is the scarce resource.** Do not store restocked trainer parties:
  generate them **deterministically from trainer ID + times-beaten counter**
  (seed-based), so they cost zero save space and are stable across reloads.
  Real storage is only needed for the (small, capped) list of trainers
  currently holding a former player Pokémon — full BoxPokemon structs, ring
  buffer if necessary.
- Vs Seeker already provides the rematch scaffolding; extend rather than
  replace.

## Open questions

- Exact badge → level-cap curve.
- Pack pricing / money faucets and sinks balance.
- Should the Broker's double-ante rematches be gated behind story progress?
- Does the rival participate in the bounty system (can they permanently hold
  your starter for an act)?
- Autosave enforcement: hard autosave-on-ante (hardcore) or player honor
  (casual)? Possibly a New Game mode toggle.
- **The mule problem:** with a purely random symmetric draw, padding the
  party with low-value Pokémon dilutes ante risk. Badge scaling mitigates
  (levels equalize; a mule still costs a real fighting slot) but base-stat
  gaps remain. Candidate fix if it proves degenerate in playtesting:
  **value-matched stakes** — the trainer's ante is drawn at a value tier
  comparable to the player's drawn ante (level × BST), so staking a Rattata
  means playing for their Rattata.
- Are Elite Four / Champion matches sanctioned (no ante) per the ban lore,
  or the ultimate exhibition? A candidate finale: the Champion stakes the
  pot — any bounties never reclaimed — so beating the League closes every
  open loss at once.

## Design rationale (preserved from discussion)

- The **death spiral** is the single make-or-break issue; the bounty loop +
  party floor + badge scaling exist specifically to counter it. If any of
  the three is cut, revisit the other two.
- **Badge-scaled levels double as the grinding replacement** — removing wild
  encounters otherwise breaks EXP progression entirely.
- The **"League banned ante" lore mirrors real MTG history** (WotC banned
  ante from sanctioned play), which is why gyms are framed as sanctioned
  matches that opt back in as exhibitions.
- **Deliberately rejected ideas:**
  - An ante-proof starter slot or protective held item that excludes a
    Pokémon from the draw (deflates the premise — the bounty loop is the
    humane protection instead; Jeweled Charm skirts this only because it
    still costs the winner's payout and is consumed on use).
  - Winner-take-loser's-ante-only (asymmetric, doesn't match MTG).
  - Removing the anted Pokémon from the battle MTG-style (hides the drama
    and makes battles 5v6).
- **Seed-based trainer restocking** (trainer ID + times-beaten as RNG seed)
  is the key technical trick — it makes "trainers refill from wild tables"
  cost zero save RAM, which is the binding constraint in a Fire Red hack.
- The **reset problem is the spiral's mirror image**: the spiral punishes
  losses too hard, resetting erases them entirely; the design needs both
  edges handled or the ante mechanic collapses into vanilla.
