# Pokémon Ante — Design

A Fire Red ROM hack built around Magic: The Gathering's original **ante**
rule. Every trainer battle is played for keeps: each side stakes one
Pokémon, and the winner takes the loser's stake.

**Three pillars:**

1. **Stakes are real.** Losses hurt, persist, and are recoverable — at a
   cost. Permadeath exists only where the player opts into it.
2. **Collection is curation.** The binder is capped; every slot is a
   decision. There is no infinite safety stock.
3. **Information is a resource.** Battles are poker, not chess: partial
   scouting, public match history, bluffing — and each faction has its own
   relationship with the truth.

---

## Core rules

### The ante

- Every trainer battle is an **ante battle** unless explicitly sanctioned
  (see World). At battle start, both sides randomly reveal one Pokémon from
  their deck as their ante — before turn one. Winner keeps their own ante
  and takes the loser's.
- **The anted Pokémon fights.** It is not set aside (a deliberate break
  from MTG): your staked Pokémon can end up battling for its own freedom,
  and dead-weight deck slots weaken you.
- **Losing costs exactly the ante, never more.** Ante loss replaces the
  vanilla half-money whiteout penalty. Losing does NOT mark a trainer
  beaten — rematches must stay open. Fleeing or conceding forfeits the
  ante but protects the rest of the deck.
- **Autosave fires the moment antes are committed.** The potential loss is
  on disk before the battle resolves. (Casual mode may disable it;
  hardcore cannot.)

### Deck and binder

- **Bring 30, choose X ≤ 6.** The **binder** (max 30 slots — 5 pages of 6)
  is your collection; the **deck** is up to 6 Pokémon chosen from it. The
  ante draws only from the deck, so **choosing your deck is choosing your
  risk pool**: 1-in-X odds per brought Pokémon. Bring-1 is legal anywhere —
  the all-in duel: guaranteed ante, sole fighter. The deck persists between
  battles; deck-building is never forced.
- **Binder Pokémon are safe but can't fight.** This is the only ante
  protection in the game. No ante-proof slots, no protected starter.
- **Binder pages are bought from the Registry** at escalating prices — a
  money sink competing with packs. You start with 2 pages (12 slots).
  Winning an ante at full capacity forces an immediate choice: sell the
  winnings on the spot, or release/sell something to make room.
- **Ownership floor + Fund rescue:** you may never stake your last
  Pokémon — ante battles require owning at least 2 total (deck + binder).
  Busting down to 1 is **a rescue, not a fail state**: at the next heal
  (whiteout or nurse visit) the Registry flags your file and the
  Professor's Fund delivers a ranch common, registered like any
  acquisition. Rock bottom is a scripted comeback beat — no game-over,
  and no lingering floor state (which would otherwise invite risk-free
  friendly-battle farming at 1 owned Pokémon). The pity-priced Starter
  Pack remains the self-serve floor once shops exist.
- **Names are the Registry key — mandatory and unique.** Every
  acquisition (starter, pull, won ante) must be registered under a name
  no other Pokémon carries; the naming screen loops until the Registry
  accepts. Uniqueness spans party, storage, *and the bounty ledger* — a
  lost Pokémon keeps its name reserved, or the ledger turns ambiguous.
  Acquisitions that bypass the naming screen (a win sent straight to
  storage) get an auto-numbered species name as a backstop, re-nameable
  later. The **Name Rater is the Registry's clerk** and refuses
  duplicate filings. **OT and names persist across owners** — your
  binder is a trophy wall, and the traded-Pokémon XP boost applies
  naturally.

### Format level (progression without grinding)

All battles are fought at the **format level** — a flat level set by badge
count, applied to both sides (Battle Tower set-level style):

| Badges | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | Champion |
|--------|---|---|---|---|---|---|---|---|---|----------|
| Format level | 15 | 22 | 26 | 32 | 38 | 44 | 48 | 52 | 58 | 70 |

- **There is no EXP.** Removing wild encounters would break grinding
  anyway; instead the whole region plays the same power band, like a TCG
  format. Your edge is deck construction, species quality, and reads.
- **Movesets auto-update** to the best level-up moves at format level;
  TMs remain the customization layer and stay valuable.
- **Evolution triggers at format-level thresholds** (opt-out per Pokémon,
  Everstone-style). Stone evolutions use stones (found in packs); trade
  evolutions are converted to stone/item evolutions.
- Every won or pulled Pokémon is **immediately viable** — churn is fun,
  not punishing.

### Field rules

- **No wild encounters, no catching.** Acquisition is antes and packs.
- **HMs are key items:** badge + item = field move, no Pokémon required.
  Progression never depends on a Pokémon you can lose.
- **Eggs, in-game trades, event Pokémon: cut.** Link battles: out of scope.

---

## Economy

**Faucets:** battle winnings (double vanilla prize money — battles are the
only income). **Sinks:** packs, binder pages, scouting reports, called
shots, Broker fees.

- **Packs** pull from wild encounter tables of routes reached so far,
  preserving Kanto's species progression. Rarity (common/uncommon/rare/
  holo) maps to base-stat total and evolution stage. Badge-gated themed
  packs (the Mt. Moon pack, gym-town packs). A **singles market** opens
  late game: pay a premium for an exact species.
- **The Collector buys Pokémon** — duplicates become money becomes packs.
  Sold Pokémon retire to Oak's lab ranch (in-fiction: no landfill).
- **The Professor's Fund** is the anti-softlock floor: a pity-priced
  Starter Pack is always affordable, free if you're broke and down to your
  last Pokémon.
- **Initial prices** (tuning values, see IMPLEMENTATION.md): Base pack
  2,000; themed 4,500; premium 8,000; binder pages 10k / 25k / 50k;
  singles 5k–50k by tier.

---

## Information

### Scouting (binders are partially discoverable)

Binders are Registry-certified, and the Registry is a public ledger —
opposing binders are discoverable, never perfectly:

- **Free (Registry lookup):** binder size and type distribution. Levels
  are public by definition (format level).
- **Paid (Broker intel):** a report (500) lists likely deck picks; a
  dossier (1,500) gives the exact current deck and held items.
- **NPCs scout you back** and shape their decks to counter what your
  binder and public history show. Counter-pick intensity by trainer class
  is the difficulty curve: route trainer 15% → ace trainer 35% → gym
  leader 60% → Elite Four 75% → rival 90%.
- **Information honesty is factional.** Gym exhibitions play open
  teamsheets (full transparency, the VGC mirror). **Rocket falsifies its
  Registry entries** — scouting their tables returns lies, while they
  counter-pick from your *stolen full file*. **The Unbound are off the
  ledger entirely** — no data either way; you fight them blind.

### The Book (public match history)

Sanctioned exhibition matches (gyms, League) are played on open teamsheets
*and recorded*. The Registry keeps your last **8 exhibition decks** (the
most recent 3 weigh double), and that history is the region's free read on
"what you typically bring."

- **Fame is the difficulty ramp.** No history, no read — early anonymity
  is a real advantage; the region adapts as your book grows.
- **The book decays.** Old entries fall out of the ring buffer; a style
  switch re-anonymizes you, and a poisoned book doesn't stay poisoned.
- **You can play the book.** The Trainer Card shows your own file;
  bringing off-meta decks to exhibitions is deliberate misinformation.
- **The jackpot is priced in information:** gyms stake rare Pokémon
  because you pay in revealed strategy. Underground games are off the
  record — a standing reason trainers keep playing them.
- **You cannot launder your own book.** The Registry doesn't take
  requests. (The story forces it once — see Going off-ledger.)

---

## Bounties and loss

### The bounty chase

- A trainer who wins your Pokémon adds it to **their binder** — trainers
  are players too, with binders, per-match decks drawn from them, and
  random antes from those decks. Your lost Pokémon is *not* guaranteed to
  appear.
- **Trophy bias:** holders run recently won Pokémon in their deck ~75% of
  the time (trainers show off their prizes), with normal ante odds from
  there — reclaiming by luck takes several rematches, not one and not
  fifty.
- **Called shots:** the Broker forces a specific bounty into the
  opponent's stake — double-ante rules, fee scaled to the bounty's value
  tier (8k / 15k / 30k). The chase is free; certainty costs.
- The **Bounty Board** (a Registry feature on your dex) tracks which
  binder holds what, where that trainer is, and whether your Pokémon has
  been running in their deck lately.

### Bounty migration

NPC binders are capped too, so unclaimed bounties get sold on — to other
trainers, the Broker's table, or (via repossession) Rocket. Default clock:
a bounty becomes eligible to migrate only after you've had ≥3 rematch
opportunities *and* earned a badge since the loss; then 25% per badge.
(Hardcore: eligible after 1 badge, 50% per badge.) Every move is logged on
the Bounty Board. Migration is how the rival quietly assembles the pot
(see World).

### Permadeath (opt-in only)

- **The Unbound** rip a won Pokémon's Registry card — it is released
  beyond the ledger's reach, permanently. Always telegraphed (dress,
  pre-battle warning, unique theme); they stake items, TMs, and cash, not
  Pokémon. The rip is played solemn, not cruel: a quiet ceremony and one
  line — *"Be free."*
- **Ghost losses** during the off-ledger arc create no bounty (see World).
- Nothing else in the game deletes a Pokémon without the player's consent.
  Impounded Pokémon (audits) are always fee-locked, never lost.

### Nuzlocke stance

Ante is a nuzlocke where death is negotiable: the permanent-loss threat is
relocated from fainting to losing, and softened by the bounty loop.
Blanket faint-permadeath is rejected — it would gut the anti-spiral kit.
A **hardcore toggle** at new game bundles: locked autosave, set battle
mode, no bag items in battle, mandatory nicknames, fast migration clock.

### The anti-spiral kit (why losing doesn't soft-lock)

Ante is inherently rich-get-richer. Three pillars counter it — if any is
cut, revisit the others:

1. **Losses are bounties, not deletions** (the chase above).
2. **The ownership floor** and Professor's Fund.
3. **Format level** — every replacement Pokémon is instantly viable.

---

## World and story

### The ban

Ante is the **old rules** — how the region has always battled. Within
living memory the **League banned ante from sanctioned play** (mirroring
WotC's real ban). The ban is recent, contested, and a failure in progress:
it pushed high-stakes play underground, and Team Rocket grew in the shadow
it created. Sanctioned status doubles as the safety valve for battles that
could soft-lock the story (first rival fight, tutorials).

### Oak and the Registry

The professorship is the institution that tracks who owns what:

- **The Pokédex is a provenance ledger** — every Pokémon's chain of
  custody. The Bounty Board and the Book are Registry features. **The
  ledger is truth:** records don't describe ownership, they *are*
  ownership.
- **Oak is the ban's architect and an ex-ante legend** who quit at his
  peak — not because he lost, but because he won too much. He returned
  every Pokémon he'd ever won and pushed the ban through. **Agatha never
  forgave him** — not for quitting, for giving it all back.
- **Oak knows the ban failed.** He deputizes the player as a field agent:
  play the tables the League can't touch, record provenance, pull the
  thread on Rocket. His guilt is the story's moral engine.
- **The Professor's Fund** is his penance; the lab ranch takes in sold
  Pokémon; **the starters are the only three he kept** — one to you, one
  to his grandson.
- **Oak starts you with two Pokémon**: the starter plus a companion from
  the lab ranch. With one Pokémon the ownership floor keeps every battle
  friendly, so a single-mon start would mute the game's core mechanic —
  "you can't ante what you can't afford to lose."

### Team Rocket (prohibition profiteers)

Rocket doesn't steal — it **cheats**: rigged ante draws (grunts always
stake junk), debt-shark **repossession** (seeding proxy-bounty sidequests
to win NPCs' partners back), **counterfeit packs** laundering stolen
Pokémon (pull one with a living OT: return it through the Registry or
keep it — per-quest flags, no global karma meter), the Celadon Game Corner
as the underground's flagship table, and the **Silph arc as a
supply-monopoly play**. Giovanni holds a legitimate gym license — the
ban's hypocrisy in one man. **Mewtwo is counterfeiting rarity itself**;
postgame, it antes *itself* and names your whole party as the stake.

### Going off-ledger (the laundering beat)

Infiltrating Rocket's high tables requires a forger to seal your record —
for one contiguous arc (Game Corner through Silph), chosen Pokémon become
**ghosts**: unscoutable, invisible to the Book, unrecognized in sanctioned
play, and **bounty-less if lost** (the Board can't track what you never
owned). Registry inspectors can audit a fielded ghost as stolen goods —
impounded, fee-locked until re-registered. Afterward, the choice: restore
your record (requires Oak's personal countersignature, and that scene) or
keep the ghost identity — unscoutable forever, untraceable forever, one
audit from losing access to everything. The player lives, once, the way
Rocket and the Unbound live always — and learns that the ledger is what
makes losses recoverable.

### The Unbound (the ban's extremists)

If ante is immoral because Pokémon aren't property, every won Pokémon is
stolen goods — the Unbound act on it (the card rip, above). Proto-Plasma
ideology grown from Oak's own rhetoric: his words armed them, deepening
his guilt. Their partners fight beside them freely and can't be won.
Lorelei publicly disavows them; whether she privately sympathizes stays
ambiguous. **Three-faction triangle:** League (reform), Rocket (profit),
Unbound (extremism) — all downstream of Oak's ban.

### The League

**Gyms opt back into ante as prestige exhibitions** — open-teamsheet
jackpot fights, each staking a rare gym-themed Pokémon, each with a rule
twist:

| # | Leader | Exhibition rule |
|---|--------|-----------------|
| 1 | Brock | **By the book** — standard single ante, the ceremonial demonstration |
| 2 | Misty | **Generosity** — she stakes two, you stake one |
| 3 | Lt. Surge | **Double or nothing** — both sides ante two |
| 4 | Erika | **Honor rules** — no random draw; each side openly chooses its ante |
| 5 | Koga | **Blind stakes** — antes drawn but hidden until the battle ends |
| 6 | Sabrina | **The seer** — she *names* your ante; hers stays random. (Kept deliberately: your deck choice is the counterplay — bring only what you can afford to show her.) |
| 7 | Blaine | **The quiz** — correct answers bank ante redraws; wrong answers raise stakes |
| 8 | Giovanni | **The house** — he stakes three, including repossessed bounty Pokémon |

**The Elite Four are four stances on the ban:** Lorelei the abolitionist
(refuses ante — the late game's one pure sanctioned match), Bruno the
indifferent (fights for the fight), Agatha the old guard (demands ante in
open defiance; carries the Oak backstory), Lance the conflicted golden boy
(official exhibition, stakes a Dratini).

**The Champion is the rival — the finale stakes the pot.** Oak's grandson
embraced everything his grandfather abandoned. As bounties migrate, **he
has been buying yours** — the Bounty Board shows your losses drifting into
his collection. He isn't collecting Pokémon; he's collecting *you*. At the
Hall of Fame he stakes **every unreclaimed bounty in the region** against
your party. Win, and every open loss closes at once. Oak's canon
post-champion scolding scene plays as a man watching his history repeat.

---

## Design principles (condensed rationale)

- **The death spiral is the make-or-break issue**; the anti-spiral kit's
  three pillars are load-bearing.
- **The reset problem is its mirror**: spiral punishes losses too hard,
  resetting erases them; both edges must be handled or ante collapses
  into vanilla.
- **The ledger is truth.** Self-service laundering isn't stealth, it's
  identity destruction — which is why only the story does it, once.
- **Deck choice = risk choice** (1-in-X) is the poker layer the concept
  always wanted; **partial information keeps it poker, not chess**, and
  mapping info-honesty to factions makes the info system carry theme.
- **A deep binder is infinite lives** — the cap makes collection curation
  and doubles as the bring-30 pool; capped NPC binders make migration
  coherent.
- **Deck persistence is the UX firewall** — ceremony only for fights that
  deserve it.
- **Permadeath is consent.** The Unbound are telegraphed; impounds are
  fee-locked; nothing deletes silently.
- **Value-matched stakes: retired.** Deck-building superseded the mule
  problem — dead slots weaken you against counter-picks, and hedging with
  a cheap slot is legitimate poker, not an exploit.
- **Forced bounty-ante was replaced by the chase** — a guaranteed reclaim
  made losses a one-win refund; trophy bias + called shots keep the chase
  fair and the floor deterministic-at-a-price.

## Decisions log (formerly open questions)

- Format level table: fixed above; no EXP; evolution at thresholds.
- Binder: start 12 slots, max 30 (5×6); pages 10k/25k/50k.
- Bring-X: X ∈ [1,6] everywhere; 1v6 handicap self-balances all-in duels.
- Trophy bias 75%; called shots 8k/15k/30k by tier; Broker (and called
  shots) unlock after the Game Corner arc.
- Migration: default = 3 rematch chances + 1 badge, then 25%/badge;
  hardcore = 1 badge, then 50%/badge.
- Book: ring buffer of 8 exhibition decks, newest 3 double-weight;
  Broker matches are **off the record**.
- Scouting: free/500/1,500 tiers; counter-pick curve 15/35/60/75/90%.
- Sabrina's seer rule: kept (deck choice is the counterplay).
- Unbound: name kept; rip staged solemn ("Be free"); faction confirmed in.
- Off-ledger arc: kept, scoped to Game Corner→Silph; audits scripted +
  on entering major venues with ghosts; impounds fee-locked, never lost.
- Counterfeit-pack morality: per-quest flags, no reputation meter.
- Mule fix (value-matched stakes): retired, see principles.
