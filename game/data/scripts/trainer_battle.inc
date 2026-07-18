EventScript_DoTrainerBattleFromApproach::
	lock
	special PlayTrainerEncounterMusic
	special EndTrainerApproach
	waitstate
	goto EventScript_DoTrainerBattle

EventScript_TryDoNormalTrainerBattle::
	lock
	faceplayer
	applymovement VAR_LAST_TALKED, Movement_RevealTrainer
	waitmovement 0
	specialvar VAR_RESULT, Script_HasTrainerBeenFought
	goto_if_ne VAR_RESULT, FALSE, EventScript_NoTrainerBattle
	special PlayTrainerEncounterMusic
	special SetUpTrainerMovement
	goto EventScript_DoTrainerBattle

EventScript_NoTrainerBattle::
	gotopostbattlescript

EventScript_TryDoDoubleTrainerBattle::
	lock
	faceplayer
	call EventScript_RevealTrainer
	specialvar VAR_RESULT, Script_HasTrainerBeenFought
	goto_if_ne VAR_RESULT, FALSE, EventScript_NoDoubleTrainerBattle
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, EventScript_NotEnoughMonsForDoubleBattle
	special PlayTrainerEncounterMusic
	special SetUpTrainerMovement
	goto EventScript_DoTrainerBattle

EventScript_NotEnoughMonsForDoubleBattle::
	special ShowTrainerCantBattleSpeech
	waitmessage
	waitbuttonpress
	release
	end

EventScript_NoDoubleTrainerBattle::
	gotopostbattlescript

EventScript_DoNoIntroTrainerBattle::
	applymovement VAR_LAST_TALKED, Movement_RevealTrainer
	waitmovement 0
	special PlayTrainerEncounterMusic
	goto_if_questlog EventScript_EndQuestLogBattle
	call EventScript_AnteCommitAndReveal
	dotrainerbattle
	call EventScript_AnteRegisterWonMon
	gotopostbattlescript

EventScript_TryDoRematchBattle::
	call EventScript_RevealTrainer
	specialvar VAR_RESULT, IsTrainerReadyForRematch
	goto_if_eq VAR_RESULT, FALSE, EventScript_NoRematchBattle
	special PlayTrainerEncounterMusic
	special SetUpTrainerMovement
	special ShowTrainerIntroSpeech
	waitmessage
	waitbuttonpress
	goto_if_questlog EventScript_EndQuestLogRematch
	call EventScript_AnteCommitAndReveal
	special StartRematchBattle
	waitstate
	call EventScript_AnteRegisterWonMon
	releaseall
	end

EventScript_NoRematchBattle::
	gotopostbattlescript

EventScript_TryDoDoubleRematchBattle::
	specialvar VAR_RESULT, IsTrainerReadyForRematch
	goto_if_eq VAR_RESULT, FALSE, EventScript_NoDoubleRematchBattle
	special HasEnoughMonsForDoubleBattle
	goto_if_ne VAR_RESULT, PLAYER_HAS_TWO_USABLE_MONS, EventScript_NotEnoughMonsForDoubleRematchBattle
	special PlayTrainerEncounterMusic
	special SetUpTrainerMovement
	special ShowTrainerIntroSpeech
	waitmessage
	waitbuttonpress
	goto_if_questlog EventScript_EndQuestLogRematch
	call EventScript_AnteCommitAndReveal
	special StartRematchBattle
	waitstate
	call EventScript_AnteRegisterWonMon
	releaseall
	end

EventScript_NoDoubleRematchBattle::
	gotopostbattlescript

EventScript_NotEnoughMonsForDoubleRematchBattle::
	special ShowTrainerCantBattleSpeech
	waitmessage
	waitbuttonpress
	release
	end

EventScript_EndQuestLogRematch::
	releaseall
	end

EventScript_RevealTrainer::
	applymovement VAR_LAST_TALKED, Movement_RevealTrainer
	waitmovement 0
	return

Movement_RevealTrainer::
	reveal_trainer
	step_end

@ Pokémon Ante: draw both stakes and reveal them before the fight starts.
@ AnteCommit returns FALSE when the battle must be played friendly (no ante).
@ The reveal ceremony shows each staked Pokémon's picture (with its cry)
@ while its side of the ante is announced. Species are buffered by
@ AnteCommit into VAR_0x8004 (player) and VAR_0x8005 (opponent).
EventScript_AnteCommitAndReveal::
	specialvar VAR_RESULT, AnteCommit
	goto_if_eq VAR_RESULT, FALSE, EventScript_AnteRevealDone
	msgbox Text_AnteReveal, MSGBOX_DEFAULT
	showmonpic VAR_0x8004, 10, 3
	msgbox Text_AnteStakeYours, MSGBOX_DEFAULT
	hidemonpic
	showmonpic VAR_0x8005, 10, 3
	msgbox Text_AnteStakeTheirs, MSGBOX_DEFAULT
	hidemonpic
EventScript_AnteRevealDone::
	return

Text_AnteReveal::
	.string "Both sides put up an ANTE!\p"
	.string "The stakes are set aside.\n"
	.string "Winner takes both!$"

Text_AnteStakeYours::
	.string "You stake {STR_VAR_1}!$"

Text_AnteStakeTheirs::
	.string "Your opponent stakes {STR_VAR_2}!$"

@ Pokémon Ante: every acquisition must be registered under a unique name.
@ Runs after a battle whose ante was won into the party; loops the naming
@ screen until the Registry accepts the name.
EventScript_AnteRegisterWonMon::
	specialvar VAR_RESULT, AnteGetLastWonPartySlot
	goto_if_eq VAR_RESULT, 255, EventScript_AnteRegisterDone
	copyvar VAR_0x8004, VAR_RESULT
	msgbox Text_AnteRegisterNewMon, MSGBOX_DEFAULT
EventScript_AnteRegisterPrompt::
	call EventScript_ChangePokemonNickname
	specialvar VAR_RESULT, AnteIsPartyMonNameValid
	goto_if_eq VAR_RESULT, TRUE, EventScript_AnteRegisterDone
	msgbox Text_AnteNameRejected, MSGBOX_DEFAULT
	goto EventScript_AnteRegisterPrompt
EventScript_AnteRegisterDone::
	return

Text_AnteRegisterNewMon::
	.string "The POKéMON you won must be\n"
	.string "registered under a unique name!$"

Text_AnteNameRejected::
	.string "That name is already on the\n"
	.string "REGISTRY! It must have its own.$"

EventScript_DoTrainerBattle::
	special ShowTrainerIntroSpeech
	waitmessage
	waitbuttonpress
	goto_if_questlog EventScript_EndQuestLogBattle
	call EventScript_AnteCommitAndReveal
	dotrainerbattle
	specialvar VAR_RESULT, GetTrainerBattleMode
	goto_if_eq VAR_RESULT, TRAINER_BATTLE_SINGLE, EventScript_EndTrainerBattle
	goto_if_eq VAR_RESULT, TRAINER_BATTLE_CONTINUE_SCRIPT, EventScript_EndTrainerBattleContinueScript
	goto_if_eq VAR_RESULT, TRAINER_BATTLE_CONTINUE_SCRIPT_NO_MUSIC, EventScript_EndTrainerBattleContinueScript
	goto_if_eq VAR_RESULT, TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE, EventScript_EndTrainerBattleContinueScript
	goto_if_eq VAR_RESULT, TRAINER_BATTLE_CONTINUE_SCRIPT_DOUBLE_NO_MUSIC, EventScript_EndTrainerBattleContinueScript
EventScript_EndTrainerBattle::
	call EventScript_AnteRegisterWonMon
	releaseall
	end

EventScript_EndTrainerBattleContinueScript::
	gotobeatenscript

EventScript_EndQuestLogBattle::
	special SetBattledTrainerFlag
	releaseall
	end

Std_MsgboxAutoclose::
	message 0x0
	waitmessage
	waitbuttonpress
	release
	return
