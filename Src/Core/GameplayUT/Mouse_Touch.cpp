#include "../stdafx.h"
#include "Mouse_Touch.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	float cClickMouseBehavior::m_sfRepeatTime = 0.1f;
	void	sMouseMoveData::MouseDown(int e_iPosX,int e_iPosY)
	{
		FrameMovePos.x = FrameMovePos.y = 0;
        DownPos.x = e_iPosX;
        DownPos.y = e_iPosY;
        MovePos = DownPos;
        UpPos = DownPos;
        MouseDownTime = cGameApp::m_sTimeAndFPS.uiCurrentTime;
        fDownToUpElpaseTime = 0.f;		
	}
	void	sMouseMoveData::MouseMove(int e_iPosX,int e_iPosY)
	{
		FrameMovePos.x = e_iPosX-MovePos.x;
		FrameMovePos.y = e_iPosY-MovePos.y;
        MovePos.x = e_iPosX;
        MovePos.y = e_iPosY;
	}

	bool	sMouseMoveData::MouseUp(int e_iPosX,int e_iPosY)
	{
		bool	l_bDoubleclick = false;
        UpPos.x = e_iPosX;
        UpPos.y = e_iPosY;        
#ifndef __IPHONE__
        fDownToUpElpaseTime = (cGameApp::m_sTimeAndFPS.uiCurrentTime-MouseDownTime)/1000.f;
#else
        UINT    l_uiElpaseTime = (cGameApp::m_sTimeAndFPS.uiCurrentTime-MouseDownTime);
        fDownToUpElpaseTime = (float)((double)l_uiElpaseTime*cGameApp::m_sTimeAndFPS.dbConversion);
#endif
		float	l_fMoveDis = DownUpDistance();
		//is first mouse up ?check distance and time.
		if( l_fMoveDis<DOUBLE_CLICK_RESTRICTION_DISTANCE )
		{
			if( uiLastMouseUpClickElpaseTime == 0 )//mouse up
			{
				uiLastMouseUpClickElpaseTime = cGameApp::m_sTimeAndFPS.uiCurrentTime;
			}//for double click check
			else
			{
#ifndef __IPHONE__
				float	l_fLastMouseUptime = (cGameApp::m_sTimeAndFPS.uiCurrentTime-uiLastMouseUpClickElpaseTime)/1000.f;
#else
				UINT    l_uiElpaseTime = (cGameApp::m_sTimeAndFPS.uiCurrentTime-uiLastMouseUpClickElpaseTime);
				float	l_fLastMouseUptime = (float)((double)l_uiElpaseTime*cGameApp::m_sTimeAndFPS.dbConversion);
#endif
				if( l_fLastMouseUptime<=DOUBLE_CLICK_TIME_SPAN )
				{
					l_bDoubleclick = true;
				}
				uiLastMouseUpClickElpaseTime = 0;
			}
		}
		return l_bDoubleclick;
	}
        //cClickMouseBehavior(RECT e_RECT){ m_CollisionRange = e_RECT; m_bCollided = false; m_eObjectMouseBehavior = eOMB_NONE; }
        cClickMouseBehavior::cClickMouseBehavior()
        {
			m_TCForRepeat.SetTargetTime(m_sfRepeatTime);
			m_bPressedRepeatMouseUp = false;
			m_bKeyDown = false;
			m_ucKeyData = -1;
			m_pDoButtonGoal_Callback = 0;
            m_bCollided = false;
            m_eObjectMouseBehavior = eOMB_NONE;
			m_vCollisionRange.x = m_vCollisionRange.y = m_vCollisionRange.z = m_vCollisionRange.w = -79979.f;
        }
		cClickMouseBehavior::cClickMouseBehavior(cClickMouseBehavior*e_pClickMouseBehavior)
		{
			m_TCForRepeat.SetTargetTime(e_pClickMouseBehavior->m_TCForRepeat.fTargetTime);
			this->SetName(e_pClickMouseBehavior->GetName());
			m_bPressedRepeatMouseUp = e_pClickMouseBehavior->m_bPressedRepeatMouseUp;
			m_bKeyDown = false;
			m_ucKeyData = e_pClickMouseBehavior->m_ucKeyData;
			m_pDoButtonGoal_Callback = e_pClickMouseBehavior->m_pDoButtonGoal_Callback;
            m_bCollided = false;
            m_eObjectMouseBehavior = eOMB_NONE;
			m_vCollisionRange = e_pClickMouseBehavior->m_vCollisionRange;
		}
        cClickMouseBehavior::~cClickMouseBehavior()
        {
        
        }

		bool    cClickMouseBehavior::IsSatisfiedCondition()
		{
			if( this->m_bPressedRepeatMouseUp && this->m_eObjectMouseBehavior == eOMB_HORVER )
			{
				return this->m_TCForRepeat.bTragetTimrReached;
			}
			return m_eObjectMouseBehavior == eOMB_UP||
				m_eObjectMouseBehavior == eOMB_DOUBLU_CLICK_UP?
				true:false; 
		}
		void    cClickMouseBehavior::Init()
		{
			m_TCForRepeat.Start();
			InternalMouseLeave(-1,-1);
			if( this->m_ucKeyData != -1 && cGameApp::m_sucKeyData[this->m_ucKeyData] )
			{
				m_bKeyDown = true;
			}
			else
			{
				m_bKeyDown = false;
			}
			this->m_bCollided = false;
			if( this->m_bPressedRepeatMouseUp && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ) )
			{
				m_eObjectMouseBehavior = eOMB_HORVER;
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_NONE;
			}
		}
        //first time into
        void    cClickMouseBehavior::MouseDown(int e_iPosX,int e_iPosY)
        {
			//ensure any object inhireechy u have to call cClickMouseBehavior::Init() while init is called.
			if( m_eObjectMouseBehavior != eOMB_NONE )
				return;
			m_TCForRepeat.Start(0.f);
			m_MouseMoveData.MouseDown(e_iPosX,e_iPosY);
            //ensure first time mouse down
            if(InternalCollide(e_iPosX,e_iPosY))
            {
                m_bCollided = true;
                m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
                InternalMouseDown(e_iPosX,e_iPosY);
            }
            else
            {
                //avoid mouse up error
				m_bCollided = false;
                m_eObjectMouseBehavior = eOMB_NONE;
            }
        }
		//
        void    cClickMouseBehavior::MouseMove(int e_iPosX,int e_iPosY)
        {
			//must first click then into here!.
			if( m_eObjectMouseBehavior != eOMB_HORVER || m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
				return;
			m_MouseMoveData.MouseMove(e_iPosX,e_iPosY);
            if(InternalCollide(e_iPosX,e_iPosY))
            {
                //m_bCollided is possible be false,because it mouse down at orther place and move to this object!.
                if( m_eObjectMouseBehavior != eOMB_HORVER )
                {
                    m_eObjectMouseBehavior = eOMB_HORVER;
                }
                InternalMouseMove(e_iPosX,e_iPosY);
            }
            else
            {
                if(m_eObjectMouseBehavior == eOMB_HORVER||m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO)
                {
                    MouseLeave(e_iPosX,e_iPosY);
                    m_bCollided = false;
					m_eObjectMouseBehavior = eOMB_NONE;
                }
            }
        }
        //
        void    cClickMouseBehavior::MouseUp(int e_iPosX,int e_iPosY)
        {
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
				return;
			bool	l_bDoubleClick = m_MouseMoveData.MouseUp(e_iPosX,e_iPosY);
            if(!m_bCollided)//is it into this area already?
            {
                InternalMouseLeave(e_iPosX,e_iPosY);
                m_eObjectMouseBehavior = eOMB_NONE;
                return;
            }
			if( l_bDoubleClick )
			{
				m_eObjectMouseBehavior = eOMB_DOUBLU_CLICK_UP;
				InternalMouseDoubleClickUp(e_iPosX,e_iPosY);
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_UP;
				InternalMouseUp(e_iPosX,e_iPosY);
			}
			m_bCollided = false;
        }
		//
		void	cClickMouseBehavior::ButtonClick()
		{
			if( m_eObjectMouseBehavior != eOMB_NONE )
				return;
			m_MouseMoveData.MouseDown(0,0);
			m_bKeyDown = true;
			m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
			InternalMouseDown(0,0);
		}

		void	cClickMouseBehavior::ButtonPressed()
		{
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
				return;
			m_MouseMoveData.MouseMove(0,0);
			m_eObjectMouseBehavior = eOMB_HORVER;
			InternalMouseMove(0,0);
		}

		void	cClickMouseBehavior::ButtonUp()
		{
			m_bKeyDown = false;
			if( m_eObjectMouseBehavior != eOMB_HORVER && m_eObjectMouseBehavior != eOMB_FIRST_TIME_INTO )
			{
				m_eObjectMouseBehavior = eOMB_NONE;
				return;
			}
			bool	l_bDoubleClick = m_MouseMoveData.MouseUp(0,0);
			if( l_bDoubleClick )
			{
				m_eObjectMouseBehavior = eOMB_DOUBLU_CLICK_UP;
				InternalMouseDoubleClickUp(0,0);
			}
			else
			{
				m_eObjectMouseBehavior = eOMB_UP;
				InternalMouseUp(0,0);
			}
		}

		void	cClickMouseBehavior::SingnalProcess()
		{
			if( cGameApp::m_sucKeyData[this->m_ucKeyData] )
			{
				if( !m_bKeyDown )
				{
					ButtonClick();
					//OutputDebugString(ValueToStringW(m_ucKeyData));
					//OutputDebugString(L":key down\n");
				}
				else
				{
					ButtonPressed();
					//OutputDebugString(ValueToStringW(m_ucKeyData));
					//OutputDebugString(L":key pressed\n");
				}
				return;
			}
			if( m_bKeyDown )
			{
				//OutputDebugString(ValueToStringW(m_ucKeyData));
				//OutputDebugString(L":key up\n");
				//std::wstring	l_str = this->GetName();
				//l_str += L"Up\n";
				//OutputDebugString(l_str.c_str());
				ButtonUp();
			}
		}

		void	cClickMouseBehavior::SingnalProcess(unsigned char e_usKey)
		{
			if( e_usKey == this->m_ucKeyData )
			{
				if( !m_bCollided )
					ButtonClick();
				else
					ButtonPressed();
				return;
			}
			if( m_bCollided )
				ButtonUp();
		}

		void    cClickMouseBehavior::Update(float e_fElpaseTime)
		{
			if( this->m_bPressedRepeatMouseUp && (this->m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO ))
			{
				if( m_TCForRepeat.bTragetTimrReached )
				{
					m_TCForRepeat.Start();
					m_eObjectMouseBehavior = eOMB_HORVER;
				}
				else
					this->m_TCForRepeat.Update(e_fElpaseTime);
			}		
		}

		void    cClickMouseBehavior::DebugRender()
		{
			GLRender::RenderRectangle(Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y),m_vCollisionRange.Width(),m_vCollisionRange.Height(),Vector4::One,0);
		}

		void	cClickMouseBehavior::SetPos(Vector3 e_vPos)
		{
			Vector2	l_Size = m_vCollisionRange.Size();
			m_vCollisionRange = Vector4(e_vPos.x,e_vPos.y,e_vPos.x+l_Size.x,e_vPos.y+l_Size.y);
		}

		void	cClickMouseBehavior::SetRepeatTime(float e_fRepeatTime)
		{
			this->m_TCForRepeat.SetTargetTime(e_fRepeatTime);
		}
//end namepsace namespace FATMING_CORE
}