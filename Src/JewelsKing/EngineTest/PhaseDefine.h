#ifndef _PHASE_DEFINE_H_H
#define _PHASE_DEFINE_H_H

#define	SPIN_PHASE_NAME					L"0"
#define	BET_PHASE_NAME					L"1"
#define	CALCULATE_SCORE_PHASE_NAME		L"2"
#define	EXTRA_GAME_PHASE_NAME			L"3"


#define	BET_PHASE_DATA									"JewelsKing/PhaseData/BetPhase/BetPhase.xml"
#define	SPIN_PHASE_DATA									"JewelsKing/PhaseData/SpinPhase/SpinPhase.xml"
#define	EXTRA_GAME_PHASE_DATA							"JewelsKing/PhaseData/ExtraGamePhase/ExtraGamePhase.xml"
#define	CALCULATE_SCORE_PHASE_DATA						"JewelsKing/PhaseData/CalculateScorePhase/CalculateScorePhase.xml"

extern char*	SLOT_BET_DATA;


extern WCHAR*	RESULT_LINES_MPDILIST_FILE_NAME;
extern WCHAR*	JEWELS_RAIN__MPDILIST_FILE_NAME;
extern WCHAR*	UI_MPDILIST_FILE_NAME;
extern WCHAR*	NG_MPDILIST_NAME;
extern WCHAR*	ANIMATED_MOVE_SOIN_OBJECT_MPDILIST_FILE_NAME;
extern WCHAR*	WIN_MONEY_SPIN_OBJECT_MPDI_MPDILIST_FILE_NAME;
extern WCHAR*	KING_MPDI_MPDILIST_FILE_NAME;
extern WCHAR*	FREE_GAME_MINING_MPDILIST_FILE_NAME;
extern WCHAR*	FREE_GAME_BONUS_MPDILIST_FILE_NAME;
extern WCHAR*	FREE_GAME_NUMERIAL_MPDILIST_FILE_NAME;
extern WCHAR*	FREE_GAME_JP_MPDILIST_FILE_NAME;
extern WCHAR*	BIG_WIN_MPDILIST_FILE_NAME;

extern char*	COIN_FOUNTAIN;

#define			FREE_GAME_JP_1_IN_MPDILIST_NAME				L"Jackpot1In"
#define			FREE_GAME_JP_1_IDLE_MPDILIST_NAME			L"Jackpot1Idle"
#define			FREE_GAME_JP_2_IN_MPDILIST_NAME				L"Jackpot2In"
#define			FREE_GAME_JP_2_IDLE_MPDILIST_NAME			L"Jackpot2Idle"


#define	PERLINE_BET_EFFECT								L"LineNum"

#define	REEL_MPDI_NAME									L"reel"
#define	NUM_PI_NAME										L"Num"


#define	EXPLORE_MPDI_NAME								L"Boom"
#define	JP_WHOLE_TARGET_SAME_MPDI_NAME					L"JP"
#define	FREE_GAME_PICKAXE_MPDI_NAME						L"SB_6"
#define	SPIN_OBJECT										L"SpinObject"
#define	SPIN_OBJECT_PI_NAME								L"SpinObject"


#define	SPIN_BUTTON_NAME								L"SPIN"
#define	AUTO_SPIN_BUTTON_NAME							L"AUTO SPIN"
#define	MENU_BUTTON_NAME								L"MENU"
#define	EXCHANGE_BUTTON_NAME							L"Exchange"

#define	LINE_ADD_BUTTON_NAME							L"LineAdd"
#define	LINE_MNUS_BUTTON_NAME							L"LineMinus"
#define	ADD_BET_BUTTON_NAME								L"AddBet"
#define	MINES_BET_BUTTON_NAME							L"MinusBet"
#define	BET_MAX_BUTTON_NAME								L"BetMax"

#define	ANIMATED_MOVE_SOIN_OBJECT_NAME					L"AnimatedMoveSpinObject"

#define	NETWORK_JP_OPEN_COUNT_VARIABLE_NAME				L"NetWorkJPOpenCount"
#define	LOCAL_JP_OPEN_COUNT_VARIABLE_NAME				L"LocalJPOpenCount"
//line now I just give it 100 lines,ifit's not enough add it
#define	TOTAL_LINE_RESULT		100



#define	SOUND_FIL_ENAME							"JewelsKing/Sound/Sound.xml"

#define	MAX_SPIN_SOUND_NAME						L"Button_Bet"			//1
//#define	SPIN_SOUND_NAME							L"Button_Spin"
#define	BET_SOUND_NAME							L"Button_Bet"
//#define	AUTO_SPIN_SOUND_NAME					L"Button_AutoSpin"
#define	COIN_REFOUND_SPIN_SOUND_NAME			L"Button_Coin"		//5
#define	PRIZE_1_500_SOUND_NAME					L"Prize1_500"
#define	PRIZE_501_1000_SOUND_NAME				L"Prize501_1000"
#define	PRIZE_1001_5000_SOUND_NAME				L"Prize1001_5000"
#define	PRIZE_5000_SOUND_NAME					L"Prize_5000"
#define	JP_SOUND_NAME							L"JP"				//10
//#define	FREE_GAME_EXTRA_PAY_RATE_SOUND_NAME		L"FG_ Bonus"
#define	SPIN_OBJECT_STOP_SOUND_NAME				L"Stop"
#define	SPIN_OBJECT_EXPLOSION_SOUND_NAME		L"Eliminate_ Prompt-"
#define	PLAY_FREE_GAME_SOUND_NAME				L"FG_ Prompt-"
#define	FREE_GAME_WIN_MONEY_SOUND_NAME			L"FG_Get"	//15
#define	FREE_GAME_CONDITION_OK					L"FG_In"	//
#define	COIN_FOUNTAIN_SOUND_NAME				L"Gold"

//BGM
#define	NORMAL_BGM_SOUND_NAME					L"NG"
#define	FREE_GAME_BGM_SOUND_NAME				L"FG"
//King
#define	KING_IDLE_SOUND_NAME					L"KingIdle"
#define	KING_WALKING_SOUND_NAME					L"KingWalking"


#define	UPPER_UI								L"UI"



#define	MAXIMUM_EXPLOSION_COUNT					10














#define	ERROR_CODE_NO_ERROR					0
#define	ERROR_CODE_NO_COIN					1



#define	BUTTON_LIGHT_LEFT_UP_1		L"LeftUp1"
#define	BUTTON_LIGHT_LEFT_UP_2		L"LeftUp2"
#define	BUTTON_LIGHT_LEFT_UP_3		L"LeftUp3"
#define	BUTTON_LIGHT_LEFT_UP_4		L"LeftUp4"
#define	BUTTON_LIGHT_LEFT_UP_5		L"LeftUp5"
#define	BUTTON_LIGHT_LEFT_DOWN_3	L"LeftDown3"
#define	BUTTON_LIGHT_LEFT_DOWN_4	L"LeftDown4"
#define	BUTTON_LIGHT_LEFT_DOWN_6	L"LeftDown6"
#define	BUTTON_LIGHT_RIGHT			L"Right"

#define	BUTTON_LIGHT_COUNT			9
//free game light
#define	LIGHT_FLASH_ID	13
			
#endif