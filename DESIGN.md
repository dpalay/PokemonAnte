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
Pokémon. At the Hall of Fame he defies the ban on the biggest stage:
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
- **Bounty migration (urgency without permadeath).** Bounties held by
  generic trainers don't sit still forever: unclaimed too long (badge-count
  based), the holder sells the Pokémon on — to another trainer, or to the
  Broker's high-stakes table where reclaiming costs a double-ante match.
  Rocket repossession pulls migrating bounties toward Rocket hands. Still
  recoverable, but losses now have a clock and a story.
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
- **The mule problem:** with a purely random symmetric draw, padding the
  party with low-value Pokémon dilutes ante risk. Badge scaling mitigates
  (levels equalize; a mule still costs a real fighting slot) but base-stat
  gaps remain. Candidate fix if it proves degenerate in playtesting:
  **value-matched stakes** — the trainer's ante is drawn at a value tier
  comparable to the player's drawn ante (level × BST), so staking a Rattata
  means playing for their Rattata.
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
