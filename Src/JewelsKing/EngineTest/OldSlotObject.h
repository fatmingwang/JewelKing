#ifndef _OLD_SLOT_OBJECT_H_
#define _OLD_SLOT_OBJECT_H_
//=========================================
//here for constant spin object,for real machine spin
//=========================================
#include "SlotRule.h"
class	cOldSlotObjectManager;


class	cSlotObject:public cBaseImage
{
	cBasicSound*m_pSpinSound;
	cOldSlotObjectManager*m_pSlotLineObjectManager;
	//
	float	m_fCurrentPos;
public:
	cSlotObject(cOldSlotObjectManager*e_pSlotLineObjectManager,cBaseImage*e_pBaseImage,Vector3 e_vPos,float e_fCurrentPos);
	~cSlotObject();
	//
	virtual	void	Update(float e_fElpaseTime);
	virtual	void	Render();
	virtual	void	DebugRender();
	bool			IsAtShowArea();
	void			RelativeMove(float e_fDis);
	//I am lazy to optmize it now...
	void			GetUVAndVertexData(float*e_pfVertexData,float*e_pfUVData);
	float	GetCurrentPos(){return m_fCurrentPos;}
	void	SetCurrentPos(float);
	//just change image's UV
	void	ChangeImage(cBaseImage*e_pBaseImage);
};
//=====================================
//spin object list order as below
//0
//1
//2
//3
//4
//5
//the m_fSpinObjectIndex0StartPos is a image height
//so object 0 could be see nicely
//so the object will show in the screen is start from object index 1.
//===============================
//if we expect a animated move object just use SubMPDI to perform it while last animated object expected to show
//===============================
//=====================================
#define	MAX_VISBILE_OBJECT	20
class	cOldSlotObjectManager:public cObjectListByName<cSlotObject>
{
	cPuzzleImage*	m_pAllSpinObjectPI;
	//
	friend class	cSlotObject;
	//all cSlotObject have to working at this range
	float			m_fAllSpinObjectLength;
	//
	float			m_fSpinObjectIndex_0_OffsetStartPos;
	//c above comment now I set 1 as default ResultSpinObjectIndex
	static int		m_siResultSpinObjectIndex;
	//where it show and the screen size to show
	Vector4			m_vShowArea;
	//the object could be show at m_vShowArea
	int				m_ShowObjectIndex[20];
	//==================spin data================================================
	//how long will it soin
	sTimeCounter	m_SpinTime;
	//the distance will spin at least
	float			m_fLeastSpinDis;
	//total spin distance( m_fLeastSpinDis + m_iStopWorkingIndex )
	float			m_fTargetSpinDis;
	//
	float			m_fCurrentSpeed;
	//final working index to show
	int				m_iResultIndexToStopAtTopShow;
	//do a trick moving let it look like a macmenic spin
	void			SpinToWorkingIndex();
	//
	void			AdjustOffsetPosition();
	void			UpdateSpinVisibleResult(float e_fElpaseTime);
public:
	cOldSlotObjectManager(Vector4 e_vShowArea,float e_fSpinTime,float	e_fLeastSpinDis);
	~cOldSlotObjectManager();
	//setup for all cSlotObject's position as well
	void			SetupData(SlotLineData e_SlotLineData,cPuzzleImage*e_pPI);
	void			Update(float e_fElpaseTime);
	void			Render();
	void			DebugRender();
	//
	bool			IsSpinedDone();
	float			GetRestTime();
	virtual	void	Init(){}
	//=======================================
	//this one for setup result data as we expect,it will change the count of top view's object' data
	//but it still need m_iResultIndexToStopAtTopShow to set right position,but m_iResultIndexToStopAtTopShow
	//always be the current top of view's previous object index minus e_iCount.
	void			StartSpinData(std::vector<std::wstring>e_strResult);
	//this one for set target index to stop at top of view
	void			StartSpinData(int e_iStopWorkingIndex);
	int				GetStopAtTopShowResultIndex(){return m_iResultIndexToStopAtTopShow;}
	//after free game we have to set 
	void			ReplaceObject(int e_iIndex,WCHAR*e_strNewObjectName);
	//
	cPuzzleImage*	GetAllSpinObjectPI(){return m_pAllSpinObjectPI;}
};

//=====================
//for result inex is set by
//0, 1, 2, 3, 4
//5, 6, 7, 8 ,9
//10,11,12,13,14....
//=====================
std::wstring	GetSpinObjectNameByIndex(cBehaviorObjectList<cOldSlotObjectManager>*e_pData,int e_iIndex);
int				GetSpinObjectManagerChildIndexByLineRuleIndex(cOldSlotObjectManager*e_pSlotObjectManager,int e_iLineRuleIndex);


#endif