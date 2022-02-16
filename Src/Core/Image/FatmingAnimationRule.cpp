#include "../stdafx.h"
#include "FatmingAnimationRule.h"
namespace FATMING_CORE
{
	cFatmingAnimationRule::cFatmingAnimationRule()
    {
	    m_vPos = Vector3::Zero;
	    m_fStartTime = 0.f;
	    m_bStart = false;
	    m_fPastTime = 0.f; 
	    m_bAnimationDone = true;
	    m_bAnimationLoop = false;
    }
	cFatmingAnimationRule::cFatmingAnimationRule(cFatmingAnimationRule*e_pFatmingAnimationRule)
    {
	    m_vPos = e_pFatmingAnimationRule->m_vPos;
	    m_fStartTime = e_pFatmingAnimationRule->m_fStartTime;
	    m_bStart = e_pFatmingAnimationRule->m_bStart;
	    m_fPastTime = e_pFatmingAnimationRule->m_fPastTime;
	    m_bAnimationDone = e_pFatmingAnimationRule->m_bAnimationDone;
	    m_bAnimationLoop = e_pFatmingAnimationRule->m_bAnimationLoop;
    }
    cFatmingAnimationRule::~cFatmingAnimationRule(){}
    void	cFatmingAnimationRule::Init()
    {
	    m_bStart = false;
	    m_bAnimationDone = false;
	    m_fPastTime = 0.f;
	    InternalInit();
	    //if( e_fElpaseTime !=0.f )
		  //  Update(e_fElpaseTime);
    }

    void	cFatmingAnimationRule::Update(float e_fElpaseTime)
    {
	    if( m_bAnimationDone )
		    return;
	    m_fPastTime += e_fElpaseTime;
	    if( !m_bStart )
	    {
		    if(m_fPastTime>=m_fStartTime)
		    {
			    m_bStart = true;
			    InternalUpdate(m_fPastTime-m_fStartTime);
		    }
	    }
	    else
	    {
		    InternalUpdate(e_fElpaseTime);
	    }
    }

    void	cFatmingAnimationRule::Render()
    {
	    if( m_bAnimationDone )
		    return;
	    InternalRender();
    }
    bool	cFatmingAnimationRule::IsAnimationLoop(){ return m_bAnimationLoop;}
    bool	cFatmingAnimationRule::IsAnimationDone(){ return m_bAnimationDone; }
    float	cFatmingAnimationRule::GetPastTime(){ return m_fPastTime; }
    void	cFatmingAnimationRule::SetAnimationLoop(bool e_bLoop){ m_bAnimationLoop = e_bLoop; }
    void	cFatmingAnimationRule::SetAnimationDone(bool e_bAnimationDone){ m_bAnimationDone = e_bAnimationDone; }
//end namespace FATMING_CORE
}