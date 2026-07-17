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

1. **Losses are bounties, not deletions — but reclaiming is a chase.**
   A trainer who takes your Pokémon adds it to their **binder** (trainers
   are players too — see Ante mechanics). It is *not* guaranteed to appear:
   their deck of 6 draws from their binder for each match, and their ante is
   still random from that deck. Two things keep the chase fair rather than
   hopeless:
   - **Trophy bias:** deck draws are weighted toward recently won Pokémon —
     trainers like showing off their prizes — so your lost mon shows up in
     their deck often, and has the normal 1-in-6 ante odds from there.
   - **Called shots:** the Broker can arrange a match where a specific
     bounty is forced into the opponent's stake — at double-ante rules and
     a steep fee. The RNG chase is free; certainty costs money.
   A **Bounty Board** (Registry feature) tracks which binder holds what and
   where that trainer is.
2. **Ownership floor.** You may never stake your last Pokémon: ante battles
   require owning at least 2 total (deck + binder). At 1, you're directed to
   the pack shop; a pity-priced **Starter Pack** is always affordable.
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
- **Deck-building: bring 30, choose X ≤ 6.** Your binder is the pool; your
  **deck** is up to 6 Pokémon chosen from it (VGC's "bring 6 pick 4,"
  scaled up). Ante draws only from the deck — so **choosing your deck is
  choosing your risk pool**: bring 6 and each mon carries 1-in-6 ante odds;
  bring 3 for a focused counter-team and the odds rise to 1-in-3. Bringing
  your ace means exposing it; leaving it home means fighting without it.
  "Strength requires exposure" becomes a per-match decision. (Bring-1 is
  the all-in duel — guaranteed ante, sole fighter, fighting for its own
  freedom. Whether that's open everywhere or Broker-gated is an open
  question.)
- **Binder Pokémon are safe but can't fight.** This is the *only* ante
  protection in the game. No ante-proof slots, no protected starter. The
  bounty loop is the humane version of protection.
- **The floor, restated:** you may never stake your last Pokémon — ante
  battles require owning at least 2 (deck + binder combined). Below that,
  it's the Professor's Fund and the pack shop.
- **The binder has a cap.** Registry-certified storage is measured in
  **binder pages** (a few slots each). You start with one page and buy more
  from the Registry — an escalating money sink competing with packs for the
  same wallet — up to a hard maximum well below "one of everything."
  Without a cap, a deep binder is infinite lives (badge scaling makes every
  stored mon instantly viable) and losses stop mattering; with one,
  collection becomes **curation**, and every slot is a decision. Winning an
  ante with a full party and full binder forces an immediate choice: sell
  the winnings on the spot, or release/sell something to make room.
- **Trainers are players too.** Every NPC trainer has a binder and draws a
  deck of 6 from it per match; their ante is random from that deck. Won
  player Pokémon go into the binder like anything else. Restocking from
  wild tables is the visible face of NPCs playing the same economy — and
  their binders are capped too, which is *why* they sell Pokémon on
  (see bounty migration).
- **OT and nicknames persist.** Won Pokémon keep their original trainer and
  nickname — your binder becomes a trophy wall of defeated trainers' aces,
  and the traded-Pokémon XP boost applies naturally. Winning back your own
  Pokémon restores it fully.

### Scouting: binders are partially discoverable

Binders are Registry-certified, and the Registry is a *public ledger* — so
opposing binders are discoverable, but never with perfect information.
Information is a resource with tiers, and both sides use it:

- **Free (Registry lookup):** an opponent's binder size and type
  distribution. Levels are effectively public anyway (badge scaling).
- **Paid (Broker intel):** likely deck picks, exact species, held items.
  Scouting reports are another money sink, and the Broker sells the table
  both ways.
- **NPCs scout you back.** Trainers shape their decks to counter what your
  binder shows. Counter-pick intensity is the difficulty knob: route
  trainers barely scout, gym leaders study you, the rival *always* knows
  your book.
- **Information honesty is factional.** Sanctioned gym exhibitions play
  open teamsheets — full transparency, the VGC mirror. **Rocket falsifies
  its Registry entries**: scouting a Rocket table returns lies (the rig
  extends to information). **The Unbound are off the ledger entirely** — no
  scouting data exists; you fight them blind, which is part of their
  menace.
- **The Book: exhibition history is public record.** Sanctioned exhibition
  matches (gyms, League) are played on open teamsheets *and recorded* — the
  Registry keeps every deck you've ever brought to one. That history is the
  region's free scouting data on you: NPCs counter-pick from "what you
  typically bring," not just your binder's contents. Consequences:
  - **Fame is a difficulty ramp.** With no history, nobody has a read on
    you — early-game anonymity is a real advantage. Every badge adds pages
    to the book, and the region adapts. Champions are known quantities.
  - **The jackpot is priced in information.** Gym exhibitions stake rare
    Pokémon *because* you pay in revealed strategy. Underground ante games
    are off the record — one more honest reason trainers keep playing
    them.
  - **You can play the book.** Bring an off-meta deck to an exhibition and
    you've poisoned the data; show your ace rarely and it stays a
    surprise. Bluffing is possible because you can read your own file —
    the Trainer Card gains a "what the region knows about you" page.
  - **The book decays.** Reads are recency-weighted: old exhibitions fade,
    so a style switch eventually re-anonymizes you — and a poisoned book
    doesn't stay poisoned. Bluffs are an investment with a shelf life.
  - **Rocket has your full file** — stolen Registry data, not the public
    book — so their tables counter-pick unnervingly well. The rig, again.
  - **The Unbound refuse to read it.** They consult no ledger on
    principle: blind both ways.
- **Scouting serves the bounty chase:** looking up a bounty holder shows
  whether your Pokémon has been running in their deck lately.
- **UX guard:** your deck persists between battles — deck-building is never
  forced. Route-trainer spam uses your current deck with zero friction;
  the full preview-and-reselect ceremony is reserved for marquee fights
  (gyms, rival, Broker matches) and on-demand scouting.

## Story & world: the region that plays for keeps

Historically, WotC banned ante from sanctioned play; gambling law and player
sentiment killed it. The region mirrors that history mid-transition.

### The ban

Ante is the **old rules** — how this region has always battled. Within living
memory, the **Pokémon League banned ante from sanctioned play**: gym and
League matches are "sanctioned"; everywhere else, ante is simply how trainers
battle. The ban is recent, contested, and — critically — **a failure in
progress**: it didn't end ante, it pushed the high-stakes tables underground,
and Team Rocket grew in the shadow it created. Prohibition dynamics are the
region's central conflict, and the player walks into the middle of it.
(Sanctioned/no-ante status doubles as the mechanical safety valve for battles
that could soft-lock the story: first rival fight, forced tutorial battles.)

### Oak and the professorship: the Registry

In an economy where Pokémon change hands constantly, someone has to track
who owns what. That is what the professorship *is* in this region:

- **The Pokédex is a provenance ledger.** Every Pokémon's chain of custody —
  original trainer, every ante that moved it, every pack it was sealed into —
  is Registry data. This is the in-world reason OT and nicknames persist,
  and the **Bounty Board is a Registry feature** Oak installs on your dex.
  "Completing the dex" means documenting species *and their stories*.
- **Oak is the architect of the ban — and an ex-ante legend.** The greatest
  ante player of his generation, he quit at his peak: not because he lost,
  but because he won too much. His trophy shelf was a graveyard of other
  people's partners. He returned every Pokémon he ever won, took the
  professorship, and pushed the ban through the League.
- **Agatha never forgave him** — not for quitting, but for giving it all
  back: "he disrespected every trainer who ever staked their soul against
  him." Her canon FRLG grudge ("Oak's gotten soft") ports over untouched.
- **Oak knows the ban failed.** The Registry can't police an underground it
  helped create — so he deputizes the player as a field agent: play the
  tables the League can't touch, record provenance, and pull the thread on
  Rocket's rigged and counterfeit operations. He is compromised and knows
  it; his guilt is the story's moral engine.
- **The Professor's Fund** is Oak's penance program and the lore for the
  party floor: the pity-priced Starter Pack, and a lab ranch where
  Collector-sold Pokémon retire (so sell-back isn't a landfill in-fiction).
- **The starters are the only three Pokémon Oak kept** from his ante days.
  He gives one to you and one to his grandson — staking his legacy on the
  reform path and the old ways at once.

### Team Rocket: what ante culture curdles into

Rocket doesn't steal Pokémon in a world where you can win them legally — it
**cheats**. Every canon Rocket beat reframes with minimal map changes:

- **Rigged tables.** Rocket trainers' ante draws are loaded: grunts always
  stake junk while your draw stays honest. The player can feel the rig
  before any NPC names it.
- **Repossession.** Debt-sharking with Pokémon as collateral. NPCs who lost
  everything at Rocket tables seed **proxy bounty** sidequests: win their
  partners back for them.
- **Laundering via counterfeit packs.** Stolen and repossessed Pokémon are
  sealed into fake packs and sold. Registry-certified packs carry Oak's
  seal; counterfeits don't. Open a counterfeit and you may pull a Pokémon
  with a living OT on someone's bounty list — **return it through the
  Registry, or keep it**. (Moral-choice hook; scope TBD.)
- **The Celadon Game Corner is the underground's flagship table** — canon
  already puts the Rocket hideout beneath it.
- **The Silph arc is a supply-monopoly play.** Silph manufactures the
  containment tech that packs are built on. Rocket seizing Silph means
  controlling the entire region's Pokémon supply — in a no-catching world,
  that's everything.
- **Going off-ledger (the laundering beat).** You cannot launder your own
  book — the Registry doesn't take requests. But the infiltration of
  Rocket's high tables *requires* it: a forger seals your record, and for
  that story stretch you live the way Rocket and the Unbound live.
  **The ledger is truth in this world**, so the forgery is binding:
  - Sealed Pokémon are **ghosts** — unscoutable, invisible to the Book,
    and unrecognized in sanctioned play (a gym cannot legally accept a
    stake you don't own).
  - Lose a ghost at an underground table and **no bounty is created** —
    the Bounty Board can't track what you never owned. Off-ledger losses
    are permadeath-adjacent, exactly like an Unbound rip.
  - **Audits.** Registry inspectors exist, and your fixed book backs the
    lie: field a ghost in the wrong place and it reads as stolen goods —
    impounded pending re-registration (fees, or a quest).
  - Afterward you're **offered the choice**: restore your record — which
    requires Oak's personal countersignature, and that scene — or keep the
    ghost identity: unscoutable forever, untraceable forever, and one
    audit away from losing everything. The player gets to feel, once, why
    the ledger is what makes losses recoverable — the Registry justifies
    itself by its absence.
- **Mewtwo is counterfeiting rarity itself.** If you can't win it and can't
  pull it, print it. Postgame, Mewtwo is the misprint with no provenance —
  it antes *itself*, and names your whole party as the stake.
- **Giovanni holds a legitimate gym license** while running the underworld —
  the ban's hypocrisy in one man. The League's compromise made him
  respectable.

### The Unbound: the ban's extremists (permadeath faction)

The ban movement has a radical fringe. If ante is immoral because Pokémon
aren't property, then *every* won Pokémon is stolen goods — and the *Unbound*
(working name) act on it. Any Pokémon they win by ante, they **rip its
Registry card** on the spot and release it beyond the ledger's reach.
No bounty is created. It is gone, permanently.

- **They think they're the heroes.** Proto-Plasma ideology in Kanto: Oak's
  own rhetoric, taken further than he ever meant it — which deepens his
  guilt arc. His words armed them.
- **They don't stake Pokémon** (they don't "own" any to stake — their
  partners fight beside them freely and can't be won). They stake rare
  items, TMs, and large cash instead: total-loss risk against premium
  non-Pokémon reward.
- **Always telegraphed.** Distinct dress, a pre-battle warning, their own
  battle theme. The player consents to permadeath by engaging; forced
  Unbound battles are rare, late, and heavily signposted.
- **Lorelei's shadow.** The abolitionist E4 member publicly disavows them —
  whether she privately sympathizes is left to dialogue ambiguity.

### The League: eight exhibitions, four stances, one pot

**Gyms opt back into ante as prestige exhibitions** — jackpot fights, each
staking a guaranteed rare, gym-themed Pokémon. Each leader's *twist on the
ante rule* is their gym gimmick (format variants, in TCG terms):

| # | Leader | Exhibition rule |
|---|--------|-----------------|
| 1 | Brock | **By the book** — standard single ante; the ceremonial demonstration of how exhibitions work |
| 2 | Misty | **Generosity** — she stakes two, you stake one (exhibitions must look good for the League) |
| 3 | Lt. Surge | **Double or nothing** — both sides ante two |
| 4 | Erika | **Honor rules** — no random draw; each side openly *chooses* its ante (the old ceremonial form) |
| 5 | Koga | **Blind stakes** — antes drawn but not revealed until the battle ends |
| 6 | Sabrina | **The seer** — she *names* your ante; hers stays random |
| 7 | Blaine | **The quiz** — each correct answer banks one ante redraw; wrong answers raise the stakes |
| 8 | Giovanni | **The house** — he stakes three, including Pokémon repossessed from NPCs; beating him fairly at his own table is the point |

**The Elite Four are four stances on the ban:**

- **Lorelei, the abolitionist** — refuses ante entirely; the only pure
  sanctioned match in the late game, and a deliberate breather.
- **Bruno, the indifferent** — fights for the fight; standard ante, asks for
  a rematch win or lose.
- **Agatha, the old guard** — demands ante in open defiance of the ban and
  dares the League to stop her; her dialogue carries the Oak backstory.
- **Lance, the conflicted golden boy** — official exhibition; stakes a
  Dratini, generous and rare.

**The Champion is the rival — and the finale stakes the pot.** Oak's
grandson embraced everything his grandfather abandoned: he played ante all
game, held your bounties across whole acts, and climbed to the top on won
Pokémon. As bounties migrate between binders, **he has been buying yours** —
the Bounty Board shows your losses drifting, one by one, into his
collection. He isn't collecting Pokémon; he's collecting *you*. At the Hall
of Fame he defies the ban on the biggest stage:
**he stakes every unreclaimed bounty in the region against your party**.
Beat him and every open loss in the game closes at once. Oak's canon
post-champion scene — arriving to dress down his grandson about treating
Pokémon with love — now plays as a man watching his own history repeat.

### New mechanics implied by the story (scope ledger)

- Rigged ante draws for Rocket trainers (trivial).
- Proxy bounties for NPC Pokémon (moderate — extends bounty storage).
- Counterfeit packs + return-or-keep choice (moderate).
- Per-gym ante rule variants (moderate — battle-setup variants).
- Pot finale (cheap — bounty history is already stored).
- Mewtwo full-party stake (special case).
- Unbound trainers: permadeath on ante loss, item/cash stakes (small —
  a battle flag plus skipping bounty creation).
- Bounty migration (moderate — see Nuzlocke section).
- NPC binder/deck model (cheap — deck draw stays seed-generated; only the
  binder's bounty entries need real storage, which the bounty list already
  provides; trophy bias and counter-picking are weightings on the seeded
  draw).
- Broker called shots (small — a forced-ante battle flag plus a fee).
- Binder page cap + full-binder overflow flow (moderate — PC UI rework).
- Deck selection from binder + bring-X ante odds (moderate — party menu
  rework; ante draw generalizes from 1-in-6 to 1-in-X).
- Scouting reports UI + info tiers (moderate; Rocket falsification and
  Unbound blanks are content flags on top of it).
- The Book (cheap on save RAM — a small ring buffer of the last N
  exhibition decks, which gives recency decay for free; counter-pick
  weighting reads it; Trainer Card page renders it).
- Off-ledger state (moderate — a per-Pokémon ghost flag feeding scouting,
  sanctioned-play eligibility, and bounty creation; audit events; an
  impound holding list).

## Nuzlocke-style rules

Ante is already a nuzlocke where death is negotiable: the permanent-loss
threat is real, but relocated from fainting to losing battles, and softened
by the bounty loop. That framing decides what to borrow:

- **No blanket faint = death.** It would gut the anti-spiral kit — the
  bounty loop only works if losses are normally recoverable. The ante loss
  *is* this game's nuzlocke loss.
- **Targeted permadeath instead: the Unbound** (above). Some losses are
  forever, but only against a clearly telegraphed faction the player
  consciously engages. Risk is opt-in, which is what makes it dramatic
  rather than spiteful.
- **Nickname clause: adopt.** Prompt a nickname on every acquisition
  (starter, pack pulls, won antes). Provenance lore already says names
  persist across owners — named stakes hurt more, and that's the game.
- **First-encounter-per-route: skip.** Its function (scarcity + forced
  roster variety) is already served by pack RNG and random ante draws.
- **Bounty migration (urgency without permadeath).** NPC binders are capped
  too, so bounties don't sit still: unclaimed too long (badge-count based),
  the holder sells the Pokémon on to make room — to another trainer, to the
  Broker's high-stakes table, or (via repossession) into Rocket hands. The
  Bounty Board tracks the moves. Still recoverable, but losses have a clock
  and a story — and the rival is quietly buying your bounties up (see the
  finale).
- **Hardcore toggle at new game** bundling the classic nuzlocke austerity
  for players who want it: enforced autosave-on-ante, set battle mode, no
  bag items in battle, mandatory nicknames, and a tight bounty-migration
  clock. Default mode keeps autosave optional and migration generous.

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
    rematches — and **called shots**: for a steep fee, arranges a match
    where a specific bounty from the opponent's binder is forced into
    their stake. The paid, deterministic path to reclaiming a lost Pokémon.
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
- **The mule problem:** with a purely random symmetric draw, padding the
  party with low-value Pokémon dilutes ante risk. Badge scaling mitigates
  (levels equalize; a mule still costs a real fighting slot) but base-stat
  gaps remain. Candidate fix if it proves degenerate in playtesting:
  **value-matched stakes** — the trainer's ante is drawn at a value tier
  comparable to the player's drawn ante (level × BST), so staking a Rattata
  means playing for their Rattata.
- Binder tuning: starting page count, slots per page, page price curve, and
  the hard maximum (needs to stay well below "one of everything").
- Trophy-bias strength: how often should a held bounty actually appear in
  the holder's deck? (Sets the expected rematch count to reclaim by luck.)
- Called-shot pricing: flat fee, or scaled to the bounty's value tier?
- Bring-1 all-in duels: open everywhere, or Broker-gated set pieces?
- Scouting-report pricing and exact info tiers (what's free vs. paid).
- Counter-pick intensity curve by trainer class (route trainer → gym
  leader → rival).
- The Book's decay rate: how many exhibitions until a read goes stale?
  (Decided: decay exists; tune the half-life.)
- The off-ledger beat: how long is the forced stretch, how often do audits
  fire for a player who keeps the ghost identity, and can an impounded
  Pokémon be permanently lost or only fee-locked?
- Are Broker matches on or off the record? (Off keeps the underground's
  information appeal consistent.)
- Does the mule problem survive deck-building? Dead slots now cost more
  against counter-picked teams, which may retire the value-matched-stakes
  fix on its own.
- Final name and tone for the Unbound (how dark is the card-rip scene?).
- Is Sabrina's "she names your ante" twist too punishing for gym 6, or
  exactly punishing enough?
- Bounty-migration clock in default (non-hardcore) mode: how generous?
- Counterfeit-pack return-or-keep: full reputation system, or simple
  per-quest flags?

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
- **Nuzlocke stance:** blanket faint-permadeath was rejected because the
  bounty loop is the design's spine — permanent loss exists, but as an
  opt-in faction (the Unbound) and a slow migration clock, never as the
  default consequence of an ordinary loss.
- **The Unbound rip the Registry card, not the Pokémon.** Destroying the
  provenance record releases the Pokémon beyond the ledger's reach —
  mechanically permadeath, tonally Pokémon-safe, and it preserves the
  TCG card-ripping image exactly.
- **Three-faction triangle:** the League (reform), Rocket (underground
  profit), the Unbound (abolitionist extremism) — all three are downstream
  of Oak's ban, which is why Oak's guilt can carry the whole story.
- **Forced bounty-ante was replaced by the bounty chase.** Guaranteeing
  your lost Pokémon as the holder's ante made losses too cheap — one
  rematch win was a full refund. NPC binders + random deck/ante draws make
  reclaiming a chase; trophy bias keeps the chase fair, and the Broker's
  called shots keep recoverability deterministic *at a price*. Net effect:
  the anti-spiral pillar survives, but slower and costlier — watch it in
  playtesting.
- **The binder cap exists because a deep binder is infinite lives.** Badge
  scaling makes every stored Pokémon instantly viable, so an uncapped
  collection dissolves all stakes. The cap turns collecting into curation,
  page purchases give money a second sink competing with packs, and capped
  NPC binders are what make bounty migration coherent rather than
  arbitrary. The cap also doubles as the "bring" pool for deck-building —
  bring 30, choose X ≤ 6.
- **Deck choice = risk choice.** Letting the player build a deck of X ≤ 6
  from the binder turns the ante draw into a dial: fewer brought means
  better counter-teams but 1-in-X ante odds. This converts "strength
  requires exposure" from a fixed rule into a per-match decision, which is
  the poker layer the concept always wanted.
- **Information is deliberately imperfect and factionally themed.** Perfect
  scouting would make battles chess (solved counter-picking); zero
  scouting wastes the deck-building layer. Partial info keeps it poker —
  and mapping information honesty to the factions (League = open
  teamsheets, Rocket = falsified entries, Unbound = off the ledger) makes
  the info system carry theme, not just mechanics.
- **Deck persistence is the UX firewall.** Deck-building before every
  route-trainer fight would be misery; the current deck rides along by
  default and full ceremony is reserved for fights that deserve it.
- **The Book makes fame the difficulty curve.** Recording only sanctioned
  exhibition decks means NPC reads on the player grow with public play:
  anonymity early, adaptation late — organic difficulty scaling with zero
  level math. It also gives gyms an information *price* for their jackpot
  stakes and gives the underground a reason to exist beyond the ban.
- **The ledger is truth.** Records don't describe ownership; they *are*
  ownership. This is why self-service laundering was rejected — not
  because falsifying the book is too strong, but because in this world it
  isn't stealth, it's identity destruction. The story makes the player do
  it once (Rocket infiltration) so the consequences teach what the
  Registry is *for*: the ledger is the thing that makes losses
  recoverable. Rocket and the Unbound both live outside it; the player
  gets one supervised taste.
