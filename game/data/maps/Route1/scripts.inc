Route1_MapScripts::
	.byte 0

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message Text_ObtainedTheX
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end

Route1_EventScript_AnteTesterAl::
	trainerbattle_single TRAINER_ANTE_TESTER_AL, Route1_Text_AlIntro, Route1_Text_AlDefeat
	msgbox Route1_Text_AlPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_AnteTesterBet::
	trainerbattle_single TRAINER_ANTE_TESTER_BET, Route1_Text_BetIntro, Route1_Text_BetDefeat
	msgbox Route1_Text_BetPostBattle, MSGBOX_AUTOCLOSE
	end

Route1_EventScript_AnteTesterCal::
	trainerbattle_single TRAINER_ANTE_TESTER_CAL, Route1_Text_CalIntro, Route1_Text_CalDefeat
	msgbox Route1_Text_CalPostBattle, MSGBOX_AUTOCLOSE
	end
