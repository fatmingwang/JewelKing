#include "../stdafx.h"
#include "../Image/PuzzleImage.h"
#include "Scroller.h"
#include "ImageButton.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	cScroller::cScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cImageButton*e_pCancelButton,bool e_bHorizontal,cPuzzleImageUnit*e_pIcon,Vector3* e_pvIconPos)
	:cTabPage(e_vTotalViewRect,e_pCancelButton,e_pIcon,e_pvIconPos)
    {
		this->SetName(e_pPI->GetName());
		m_CurrentMousePos.x = m_CurrentMousePos.y = m_MouseMoveing.x = m_MouseMoveing.y = 0;
        m_bDirectionIsHorizontal = e_bHorizontal;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_iSelectedIndex = -1;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
        this->AddData(e_pPI);
    }

	cScroller::cScroller(std::vector<cBaseImage*>*e_pImageList,Vector4 e_vTotalViewRect,cImageButton*e_pCancelButton,bool e_bHorizontal,cPuzzleImageUnit*e_pIcon,Vector3* e_pvIconPos)
	:cTabPage(e_vTotalViewRect,e_pCancelButton,e_pIcon,e_pvIconPos)
	{
		//this->SetName(e_pPI->GetName());
		m_CurrentMousePos.x = m_CurrentMousePos.y = m_MouseMoveing.x = m_MouseMoveing.y = 0;
        m_bDirectionIsHorizontal = e_bHorizontal;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_iSelectedIndex = -1;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
        this->AddData(e_pImageList);	
	}
    //
    cScroller::~cScroller()
    {
    
    }
	//
	bool	cScroller::CollideObjectIndex(cImageButton*e_pImageButton,int e_iPosX,int e_iPosY)
	{
		bool	l_bCollided = false;
		if( m_bDirectionIsHorizontal )
		{
			if(e_iPosX>e_pImageButton->GetPos()->x)
			{
				if( e_iPosX<e_pImageButton->GetPos()->x+*e_pImageButton->GetImageShowWidth() )
					l_bCollided = true;
			}
		}
		else
		{
			if(e_iPosY>e_pImageButton->GetPos()->y)
			{
				if( e_iPosY<e_pImageButton->GetPos()->y+*e_pImageButton->GetImageShowHeight() )
					l_bCollided = true;
			}					
		}
		return l_bCollided;
	}

    //
    void    cScroller::Init()
    {
        cClickMouseBehaviorList<cClickMouseBehavior>::Init();
		m_bRollBack = false;
		m_iSelectedIndex = -1;
		m_fTotalLength = 0.f;
        int l_iCOunt = this->Count();
        if( l_iCOunt == 0 )
            return;
		cImageButton*l_pImageButton = (cImageButton*)this->GetObject(0);
        POINT   l_Size = {*l_pImageButton->GetImageShowWidth(),*l_pImageButton->GetImageShowHeight()};
        Vector3 l_vPos = Vector3(m_vCollisionRange.x,m_vCollisionRange.y,0.f);
        int l_iCount = Count();
        if( l_iCount )
        {
            for( int i=0;i<l_iCount;++i )
            {
                l_pImageButton = (cImageButton*)GetObject(i);
                l_pImageButton->SetPos(l_vPos);
                if( m_bDirectionIsHorizontal )
				{
					m_fTotalLength += *l_pImageButton->GetImageShowWidth();
					l_vPos.x += *l_pImageButton->GetImageShowWidth();
				}
                else
				{
					m_fTotalLength += *l_pImageButton->GetImageShowHeight();
					l_vPos.y += *l_pImageButton->GetImageShowHeight();
				}
            }
        }
    }

	bool	cScroller::IsScrollable()
	{
		int l_iCount = this->Count();
		if( l_iCount<2 )
			return false;
		//cImageButton*l_pImageButton = (cImageButton*)this->GetObject(0);
		//float	l_fImageLength = 0.f;
		////check last if it's not at last pos
		//if( m_bDirectionIsHorizontal )
		//{
		//	l_fImageLength = (float)*l_pImageButton->GetImageShowWidth();
		//	if(l_iCount*l_fImageLength<=this->m_vCollisionRange.z-m_vCollisionRange.x)
		//	{//it less than total viewable length so do not scroller
		//		return false;
		//	}
		//}
		//else
		//{
		//	l_fImageLength = (float)*l_pImageButton->GetImageShowHeight();
		//	if(l_iCount*l_fImageLength<=this->m_vCollisionRange.w-m_vCollisionRange.y)
		//	{
		//		return false;
		//	}
		//}
		return true;
	}
    //first time mouse into
    void    cScroller::MouseDown(int e_iPosX,int e_iPosY)
    {
		if( m_bRollBack )
			return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseDown(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseDown(e_iPosX,e_iPosY);
		if( this->m_bOutOfWorkingZone )
			return;
        m_CurrentMousePos.x = e_iPosX;
        m_CurrentMousePos.y = e_iPosY;
		m_bRollBack = false;
        int l_iCount = this->Count();
        if( l_iCount )
        {
			if( this->m_MouseMoveData.DownUpDistance()<5.f )
			{
				for( int i=0;i<l_iCount;++i )
				{
					//CollideTextureWithTransform
					//if(this->GetObject(i)->CollideTextureWithTransform(e_iPosX,e_iPosY,false))
					cImageButton*l_pImageButton = (cImageButton*)this->GetObject(i);
					//fuck offset problem so I have to collide with fucking weired way
					//if(l_pImageButton->CollideTexture(e_iPosX,e_iPosY,false))
					if(CollideObjectIndex(l_pImageButton,e_iPosX,e_iPosY))
					{
						m_iSelectedIndex = i;
						m_pCurrentMouseDownCollidedObject = l_pImageButton;
						return;
					}
				}
			}
		}
    }
	//
    void    cScroller::MouseMove(int e_iPosX,int e_iPosY)
    {
		if( m_bRollBack )
			return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseMove(e_iPosX,e_iPosY);
		if( this->m_bOutOfWorkingZone )
			return;
		if( this->m_eObjectMouseBehavior == eOMB_NONE )
			return;
        this->m_MouseMoveing.x = e_iPosX-m_CurrentMousePos.x;
        this->m_MouseMoveing.y = e_iPosY-m_CurrentMousePos.y;
        m_CurrentMousePos.x = e_iPosX;
        m_CurrentMousePos.y = e_iPosY;
        Vector3 l_vPos;
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            cImageButton*l_pBaseImage = (cImageButton*)this->m_ObjectList[i];
            l_vPos = *l_pBaseImage->GetPos();
            if(this->m_bDirectionIsHorizontal)
                l_vPos.x += this->m_MouseMoveing.x;
            else
                l_vPos.y += this->m_MouseMoveing.y;
            l_pBaseImage->SetPos(l_vPos);
        }        
    }
    //while mouse clicked is up
    //while current index collide with its image position and width height
    //while focus to left get current focus image position and collide with left one image's position,start to next start
    //while focus to right get current focus image's position with its width and get right image's position to collide,nd to next end
    void    cScroller::MouseUp(int e_iPosX,int e_iPosY)
    {
		if( m_bRollBack )
			return;
		//if out of working zone do nothing.
		bool	l_bOutOfWorkingZone = m_bOutOfWorkingZone;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseUp(e_iPosX,e_iPosY);
		if( this->m_eObjectMouseBehavior == eOMB_NONE )
			return;
		cClickMouseBehavior::MouseUp(e_iPosX,e_iPosY);
		if( l_bOutOfWorkingZone )
			return;
        int l_iCount = this->Count();
        if( l_iCount )
        {
			if( this->m_MouseMoveData.DownUpDistance()<15.f )
			{
				for( int i=0;i<l_iCount;++i )
				{
					//CollideTextureWithTransform
					//if(this->GetObject(i)->CollideTextureWithTransform(e_iPosX,e_iPosY,false))
					cImageButton*l_pImageButton = (cImageButton*)this->GetObject(i);
					//if(l_pImageButton->CollideTexture(e_iPosX,e_iPosY,false))
					//fuck offset problem so I have to collide with fucking weired way
					if(CollideObjectIndex(l_pImageButton,e_iPosX,e_iPosY))
					{
						if(m_iSelectedIndex == i )
						{
							this->m_bClickMouseBehaviorSatisfied = true;
							this->m_pCurrentWorkingObject = l_pImageButton;
							return;
						}
						else
							m_iSelectedIndex = -1;
						return;
					}
				}
				return;
			}
			if(!IsScrollable())
				return;
			m_vAddedDis = Vector3::Zero;
			//check first if it over range
			cImageButton*l_pImageButton = (cImageButton*)this->GetObject(0);
			Vector3	l_vPos = *l_pImageButton->GetPos();
			float	l_fImageLength = 0.f;
			float	l_fDidOfFirstToStart = 0.f;
			float	l_fDisOfLastToEnd = 0.f;
			if( this->m_bDirectionIsHorizontal )
			{
				l_fImageLength = (float)*l_pImageButton->GetImageShowWidth();
				l_fDidOfFirstToStart = l_vPos.x-this->m_vCollisionRange.x;
				m_vDisToRightPos = Vector3(-l_fDidOfFirstToStart,0.f,0.f);
			}
			else
			{
				l_fImageLength = (float)*l_pImageButton->GetImageShowHeight();
				l_fDidOfFirstToStart = l_vPos.y-this->m_vCollisionRange.y;
				m_vDisToRightPos = Vector3(0.f,-l_fDidOfFirstToStart,0.f);
			}
			if( l_fDidOfFirstToStart>3.f )
			{
				this->m_bRollBack = true;
				this->m_RollBackTC.Start();
				return;
			}
			//if total length is not over slide range,check scroll to first or last
			//check last if it's not at last pos
			l_pImageButton = (cImageButton*)this->GetObject(this->Count()-1);
			cImageButton*l_pFirstImageButton = (cImageButton*)this->GetObject(0);
			l_vPos = *l_pImageButton->GetPos();
			if( m_bDirectionIsHorizontal )
			{
				if(this->m_vCollisionRange.z>(l_vPos.x-l_pFirstImageButton->GetPos()->x)+*l_pImageButton->GetImageShowWidth())
				{
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
					return;			
				}
			}
			else
			{
				if(this->m_vCollisionRange.w>(l_vPos.y-l_pFirstImageButton->GetPos()->y)+*l_pImageButton->GetImageShowHeight())
				{
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
					return;			
				}			
			}

			if( m_bDirectionIsHorizontal )
			{
				l_fDisOfLastToEnd = l_vPos.x-this->m_vCollisionRange.z+*l_pImageButton->GetImageShowWidth();
				this->m_vDisToRightPos = Vector3(-l_fDisOfLastToEnd,0.f,0.f);
			}
			else
			{
				l_fDisOfLastToEnd = l_vPos.y-this->m_vCollisionRange.w+*l_pImageButton->GetImageShowHeight();
				this->m_vDisToRightPos = Vector3(0.f,-l_fDisOfLastToEnd,0.f);
			}
			if( l_fDisOfLastToEnd < 3.f)
			{
				this->m_bRollBack = true;
				this->m_RollBackTC.Start();
				return;
			}
			//if down and up is smaller than 0.2f do a smooth scroller
			const float	l_fMinTime = 0.2f;
			const float	l_fMinPencentage = 0.0f;
			if( this->m_MouseMoveData.fDownToUpElpaseTime <l_fMinTime )
			{
				float	l_fViewRectLength;
				if(this->m_bDirectionIsHorizontal)
					l_fViewRectLength = this->m_vCollisionRange.z-this->m_vCollisionRange.x;
				else
					l_fViewRectLength = this->m_vCollisionRange.w-this->m_vCollisionRange.y;
				Vector2	l_vDownUpDirection;
				float	l_fDistance = this->m_MouseMoveData.DownUpDistance(&l_vDownUpDirection);
				float	l_fPercentage = l_fDistance/l_fViewRectLength;
				if( l_fPercentage>1.f )
					l_fPercentage = 1.f;
				float	l_fLengthToScroll = m_fTotalLength * l_fPercentage;
				if( l_fPercentage>l_fMinPencentage )
				{
					if( this->m_bDirectionIsHorizontal )
					{
						if(l_vDownUpDirection.x<0.f)//to right
						{
							if( l_fLengthToScroll>l_fDisOfLastToEnd )
								l_fLengthToScroll = l_fDisOfLastToEnd;
							m_vDisToRightPos = Vector3(-l_fLengthToScroll,0.f,0.f);
						}
						else//to left
						{
							if( l_fLengthToScroll>-l_fDidOfFirstToStart )
								l_fLengthToScroll = -l_fDidOfFirstToStart;
							m_vDisToRightPos = Vector3(l_fLengthToScroll,0.f,0.f);
						}
					}
					else
					{
						if(l_vDownUpDirection.y<0.f)//to down
						{
							if( l_fLengthToScroll>l_fDisOfLastToEnd )
								l_fLengthToScroll = l_fDisOfLastToEnd;
							m_vDisToRightPos = Vector3(0.f,-l_fLengthToScroll,0.f);
						}
						else//to top
						{
							if( l_fLengthToScroll>-l_fDidOfFirstToStart )
								l_fLengthToScroll = -l_fDidOfFirstToStart;
							m_vDisToRightPos = Vector3(0.f,l_fLengthToScroll,0.f);
						}

					}
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
				}
			}
        }
    }

    void    cScroller::Update(float e_fElpaseTime)
    {
		if(this->m_bRollBack)
        {
			m_RollBackTC.Update(e_fElpaseTime);
			Vector3	l_vDisToMove;
			if( m_RollBackTC.bTragetTimrReached )
			{
				
				l_vDisToMove = m_vDisToRightPos-m_vAddedDis;
				m_bRollBack = false;				
			}
			else
			{
				l_vDisToMove = this->m_vDisToRightPos*(e_fElpaseTime/m_RollBackTC.fTargetTime);
			}
			m_vAddedDis += l_vDisToMove;
			int	l_iCount = this->Count();
			for( int i=0;i<l_iCount;++i )
			{
				cImageButton*l_pImageButton = (cImageButton*)this->m_ObjectList[i];
				l_pImageButton->SetPos(*l_pImageButton->GetPos()+l_vDisToMove);
			}
            return;
        }
		if( m_pCurrentWorkingObject )
			m_pCurrentWorkingObject->Update(e_fElpaseTime);
    }
    
    void    cScroller::Render()
    {
		int	l_iCount = this->Count();
		if( l_iCount == 0 )
			return;
		cImageButton*l_pImage = 0;
		Vector3	l_vPos;
		Vector4	l_vOriginalColor;
		if( -1 != m_iSelectedIndex )
		{
			l_pImage = (cImageButton*)this->GetObject(m_iSelectedIndex);
			l_vOriginalColor = *l_pImage->GetColor();
			l_pImage->SetColor(g_vImageButtonClickedColor);
		}		glEnable(GL_SCISSOR_TEST);

		Vector4	l_vViewRect(this->m_vCollisionRange.x,this->m_vCollisionRange.y,
			this->m_vCollisionRange.Width(),m_vCollisionRange.Height());

		l_vViewRect = ViewRectToOpenGLScissor(l_vViewRect);
		glScissor((int)l_vViewRect.x,(int)l_vViewRect.y,(int)l_vViewRect.z,(int)l_vViewRect.w);
		if(this->m_bDirectionIsHorizontal)
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pImage = (cImageButton*)this->m_ObjectList[i];
				l_vPos = *l_pImage->GetPos();
				if(l_vPos.x+*l_pImage->GetImageShowWidth()>=this->m_vCollisionRange.x)
				{
					if( l_vPos.x>this->m_vCollisionRange.z )
					{
						break;
					}
					l_pImage->Render();
				}
			}
		}
		else
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pImage = (cImageButton*)this->m_ObjectList[i];
				l_vPos = *l_pImage->GetPos();
				if(l_vPos.y+*l_pImage->GetImageShowHeight()>=this->m_vCollisionRange.y)
				{
					if( l_vPos.y>this->m_vCollisionRange.w )
					{
						break;
					}
					l_pImage->Render();
				}
			}
		}
		if( -1 != m_iSelectedIndex )
		{
			l_pImage = (cImageButton*)this->GetObject(m_iSelectedIndex);
			l_pImage->SetColor(l_vOriginalColor);
		}
		glDisable(GL_SCISSOR_TEST);
		if( this->m_pCancelButton )
			m_pCancelButton->Render();
    }
    
    void    cScroller::DebugRender()
    {
        if(this->Count())
        {
            cImageButton*l_pBaseImage = (cImageButton*)GetObject(0);
            Vector2 l_vLinePos[4];
			if( this->m_bDirectionIsHorizontal )
			{
				l_vLinePos[0] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y);
				l_vLinePos[1] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.w);
				l_vLinePos[2] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.y);
				l_vLinePos[3] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.w);
			}
			else
			{
				l_vLinePos[0] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y);
				l_vLinePos[1] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.y);
				l_vLinePos[2] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.w);
				l_vLinePos[3] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.w);			
			}
            GLRender::RenderLine((float*)l_vLinePos,4,Vector4(1,1,0,1),2);
			GLRender::RenderLine((float*)l_vLinePos[2],4,Vector4(1,1,0,1),2);
        }
    }
//end namespace FATMING_CORE

//end namespace
};