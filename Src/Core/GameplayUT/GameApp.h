#ifndef _GAME_APP_H_
#define _GAME_APP_H_

#include "PhaseControl.h"
#include "MultiTouchPoints.h"
#include "GameUTDefine.h"
#include "../GLSL/Shader.h"
#include "../AllImageInclude.h"
#include "../AllMathInclude.h"
class	cFUThreadPool;
namespace	FATMING_CORE
{
	class	cSoundParser;
	class	cPaticleManager;
	class	cBehaviorPaticleManager;
	class	cParticleEmitterGroup;
	class	cPrtEmitter;
	class	cClickMouseBehavior;
	class	cBinaryFile;
	//in windows make sure touch points convert as well!.
	//view port x and y might swap if direction is lanscape.
	POINT	ConvertCoordinate(int e_iPosX,int e_iPosY,Vector2 e_vViewPort);


	class	cGameApp
	{
	protected:
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Render();
#ifdef WIN32
		virtual	void			OpenglInit(HWND e_Hwnd);
#endif
	public:
#ifdef WIN32
		cGameApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920.f,1080.f),Vector2 e_vViewportSize = Vector2(1920.f,1080.f));
#else
		cGameApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#endif
		virtual	~cGameApp();
		virtual	void				Destroy();
		//initial shader and phase manager
		virtual	void				Init();
		//update and render
		virtual	void				Run();
        //basicly we don't use below function,instead call by TouchSignalProcess
		virtual	void				MouseDown(int e_iPosX,int e_iPosY);
		virtual	void				MouseMove(int e_iPosX,int e_iPosY);
		virtual	void				MouseUp(int e_iPosX,int e_iPosY);
		//it will automaticly to call mouse relateive function for their behavior
		//this one for single touch
		//while 2 touche point is clicked,keep doing 2 touch event but if first touch point is mouse up,keep doing mouse2 event until mouseup2 is called,
		//make m_b2Touched is set to false.
		virtual void				TouchSignalProcess(int e_iPosX,int e_iPosY,bool e_bTouch);
		//virtual void    TouchTwoProcess(int e_iPosX,int e_iPosY,int e_iPos2X,int e_iPos2Y,bool e_bTouch1,bool e_bTouch2);
		virtual	void				KeyPress(char e_char);
		virtual	void				KeyDown(char e_char);
		virtual	void				KeyUp(char e_char);
//static objects
#ifdef WIN32
		static	HDC									m_sHdc;
		static	HANDLE								m_sHandle;
#endif
		static	UT::sTimeAndFPS						m_sTimeAndFPS;
		static	Vector2								m_svViewPortSize;
		//while game resolution is change we want to re scale all data check this one
		static	Vector2								m_svGameScale;
		//viewport resolution or game resolution,both them could be same,but depend on the game type.
		static	Vector2								m_svGameResolution;
		static	cPhaseManager*						m_spPhaseManager;
		static  cAnimationParser*					m_spAnimationParser;
		static  cSoundParser*						m_spSoundParser;
		static  cImageParser*						m_spImageParser;
		static	cPaticleManager*					m_spPaticleManager;
		static	cBehaviorPaticleManager*			m_spBehaviorPaticleManager;
		static	cGlyphFontRender*					m_spGlyphFontRender;
		static	sMultiTouchPoints*					m_spMultiTouchPoints;
		static	cObjectListByName<cCurveManager>*	m_spPathFileList;
	    static	bool    							m_bMouseHover;
		static  POINT								m_sMousePosition;
		static	bool								m_sbFullScreen;
		static	bool								m_sbTouched;//if true mouse down
		static	bool								m_sb2Touched;//if true mouse down
		static	bool								m_sbGamePause;
		static	eDeviceDirection					m_seDeviceDirection;
		static	cFUThreadPool*						m_spThreadPool;
		//if windows messagebox will pop up or just a outputdebugstring
		static	bool								m_sbShowErrorMsgBox;
		static	std::wstring*						m_spstrErrorMsgString;
		//will be effect by key + and -,so keep - and + reserved
		static	float								m_sfDebugValue;
		//if true game exit
		static bool									m_sbLeave;
		//if true it will control by key + and -
		static bool									m_sbSpeedControl;
		static bool									m_sbDebugFunctionWorking;
		//all key data is pressed or not
		static bool									m_sucKeyData[MAX_PATH];
		//for io
		static bool									m_sbDeviceExist;
		//
		static bool									m_sbEnableMouseSingnal;
		static bool									m_sbEnableKeyboardSingnal;
		static cBinaryFile*							m_spLogFile;
		//mouse
		//static cClickMouseBehavior*		m_spClickMouseBehavior;

		//=================================================
		static	void	ResoluctionParse(char*e_strFileName);

		static	Vector2					GetViewPortAndGameResolutionScale();

		static	cPuzzleImage*			GetPuzzleImage(const WCHAR*e_strName);
		static	cBaseImage*				GetBaseImage(const WCHAR*e_strName);
		static	cPuzzleImage*			GetPuzzleImageByFileName(const WCHAR*e_strFileName);

		static	cMPDIList*				GetMPDIList(const WCHAR*e_strName);
		static	cMPDIList*				GetMPDIListByFileName(const WCHAR*e_strFileName);
		static	cMPDIList*				GetMPDIList(const char*e_strFileName,const WCHAR*e_strName);
		static	cMPDIList*				GetMPDIList(int e_iIndex);
		static	cCurveManager*			GetCurveManagerByFileName(const WCHAR*e_strFileName);

		static	cParticleEmitterGroup*	GetPRTG(const char*e_strFileName,const WCHAR*e_strName);
		static	cParticleEmitterGroup*	GetPRTG(const WCHAR*e_strName);
		static	cPrtEmitter*			GetPrtEmitter(const char*e_strFileName,const WCHAR*e_strName);
		static	cPrtEmitter*			GetPrtEmitter(const WCHAR*e_strName);
		static	bool					SoundPlay(const WCHAR*e_strSoundName,bool e_bPlay);
	};
//end namespace
}

#ifdef WIN32
#include "Psapi.h"
void PrintMemoryInfo( WCHAR*e_strName = L"Memory" );
#endif

#endif