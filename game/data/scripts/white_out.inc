EventScript_AfterWhiteOutHeal::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	msgbox Text_FirstShouldRestoreMonsHealth
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
	call_if_unset FLAG_DEFEATED_BROCK, EventScript_AfterWhiteOutHealMsgPreBrock
	call_if_set FLAG_DEFEATED_BROCK, EventScript_AfterWhiteOutHealMsg
	call EventScript_AnteFundRescue
	applymovement VAR_LAST_TALKED, Movement_Bow
	waitmovement 0
	fadedefaultbgm
	releaseall
	end

@ Pokémon Ante: rock bottom is a rescue, not a fail state. A trainer down
@ to their last Pokémon gets a ranch common from the Professor's Fund the
@ moment they're back at a heal spot, then registers it.
EventScript_AnteFundRescue::
	specialvar VAR_RESULT, AnteTryFundRescue
	goto_if_eq VAR_RESULT, FALSE, EventScript_AnteFundRescueDone
	playfanfare MUS_LEVEL_UP
	msgbox Text_AnteFundRescue, MSGBOX_DEFAULT
	waitfanfare
	call EventScript_AnteRegisterWonMon
EventScript_AnteFundRescueDone::
	return

Text_AnteFundRescue::
	.string "…Oh! The REGISTRY has flagged\n"
	.string "your file.\p"
	.string "When a TRAINER is down to their\n"
	.string "last POKéMON, PROF. OAK's FUND\l"
	.string "sends one over from the lab ranch.\p"
	.string "A {STR_VAR_1} has been\n"
	.string "delivered to you!$"

EventScript_AfterWhiteOutHealMsgPreBrock::
	msgbox Text_MonsHealedShouldBuyPotions
	return

EventScript_AfterWhiteOutHealMsg::
	msgbox Text_MonsHealed
	return

EventScript_AfterWhiteOutMomHeal::
	lockall
	textcolor NPC_TEXT_COLOR_FEMALE
	applymovement LOCALID_MOM, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	msgbox Text_HadQuiteAnExperienceTakeRest
	call EventScript_OutOfCenterPartyHeal
	msgbox Text_MomExplainHPGetPotions
	call EventScript_AnteFundRescue
	fadedefaultbgm
	releaseall
	end

EventScript_FieldPoison::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	special TryFieldPoisonWhiteOut
	waitstate
	goto_if_eq VAR_RESULT, TRUE, EventScript_FieldWhiteOut
	releaseall
	end

EventScript_FieldWhiteOut::
	checkmoney 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_FieldWhiteOutNoMoney
	goto_if_eq VAR_RESULT, TRUE, EventScript_FieldWhiteOutHasMoney
	end

EventScript_FieldWhiteOutNoMoney::
	msgbox Text_WhitedOut
	goto EventScript_FieldWhiteOutFade
	end

EventScript_FieldWhiteOutHasMoney::
	special OverworldWhiteOutGetMoneyLoss
	msgbox Text_WhitedOutLostMoney
	goto EventScript_FieldWhiteOutFade
	end

EventScript_FieldWhiteOutFade::
	special Script_FadeOutMapMusic
	waitstate
	fadescreen FADE_TO_BLACK
	special SetCB2WhiteOut
	waitstate
	end
