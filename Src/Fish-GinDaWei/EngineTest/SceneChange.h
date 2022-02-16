/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _SCENE_CHANGE_H_
#define _SCENE_CHANGE_H_


//
//<BG ChangeCircle="300,500">
//	<NormaleBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//	<SpecialBG Name="BG1.png,BG2.png,BG3.png" Probability="50" />
//</BG>
//
enum	eSceneChangeStatus
{
	eSCS_NONE = 0,
	eSCS_PLAY_SCENE_CHANGE_MPDI,
	eSCS_WAIT_FISH_LEAVE,
	eSCS_BUBBLE_GO,
	eSCS_WAIT_BUBBLE_DISSAPEAR,
	eSCS_MAX,
};
class	cFishGroup;
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */

enum	eSpecialSceneChangeType
{
	eSSCT_FISG_GROUP = 0,
	eSSCT_FISG_GROUP2,
	eSSCT_BOSS,
	eSSCT_MAX
};

class	cSceneChange:public ISAXCallback
{
	float	m_fSpecialSceneChangeTime[eSSCT_MAX];
	bool	m_bSceneIsSpecial;
	eSceneChangeStatus	m_eSceneChangeStatus;
	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	void	ProcessSpecialBG(TiXmlElement*e_pTiXmlElement);
	void	ProcessNormalBG(TiXmlElement*e_pTiXmlElement);
	void	ProcessBG(TiXmlElement*e_pTiXmlElement);
	void	ProcessShowTime(TiXmlElement*e_pTiXmlElement);
	//
	struct	sBGData
	{
		std::vector<std::string>	ImageNameList;
		int							iProbability;
	};
	sBGData					m_NormalBG;
	sBGData					m_SpecialBG;
	UT::sMinMaxData<float>	m_TimeCircleRestrict;
	UT::sTimeCounter		m_ChangeSceneCircle;
	int						m_iTotalProbability;
	float					m_fFishRunAwaySpeed;
	//
	cBaseImage*				m_pBGImage;
	cBaseImage*				m_pLastBGImage;
	std::string				m_CurrentBGName;
	UT::sTimeCounter		m_BubbleMovingTC;
	UT::sTimeCounter		m_BubbleClamDownTC;
	cPrtEmitter*			m_pPrtEmitter;
	cMPDI*					m_pWaterMPDI;
	cMPDIList*				m_pWaterList;
	cMPDI*					m_pShowChangeSceneMPDI;
	//
	bool*					m_pbSceneChange;
	void					ChangeScene();
	void					ChangeSpecialScene();

	void					UpdateeSCS_NONE(float e_fElpaseTime);
	void					UpdateeeSCS_PLAY_SCENE_CHANGE_MPDI(float e_fElpaseTime);
	void					UpdateeeSCS_WAIT_FISH_LEAVE(float e_fElpaseTime);
	void					UpdateeeSCS_BUBBLE_GO(float e_fElpaseTime);
	void					UpdateeeSCS_WAIT_BUBBLE_DISSAPEAR(float e_fElpaseTime);
	//
	UT::sTimeCounter		m_StopGenerateFishTime;
	
	//
	GET_SET_DEC(bool,m_bStopChangeScene,IsStopChangeScene,SetStopChangeScene);
public:
	//
	cFishGroup*				m_pFishGroup;
	cFishGroup*				m_pFishGroup2;
	//
	cSceneChange();
	~cSceneChange();
	void	Init();
	void	Update(float e_fElpaseTime);
	void	Render();
	void	LastRender();
	void	DebugRender();
	bool	IsChangeScene();
};
//end
#endif