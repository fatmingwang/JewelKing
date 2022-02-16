#include "../../stdafx.h"
#include "EventObject.h"
#include "EventMPDI.h"
#include "EventManager.h"
namespace   FATMING_CORE
{
    const wchar_t*         cEventMPDI::TypeID( L"cEventMPDI" );
    cEventMPDI::cEventMPDI(Vector3 e_vPos,cMultiPathDynamicImage*e_pMPDI,bool e_bClickToPlay,float e_fScale,bool e_bLoop):cEventObject(eEOT_MPDI,e_vPos)
    {
        m_pMPDI = new cMultiPathDynamicImage(e_pMPDI);
		m_pMPDI->SetLoop(e_bLoop);
		m_pMPDI->SetOwner(e_pMPDI->GetOwner());
        m_pMPDI->Init();
		m_pMPDI->Update(EPSIONAL);
		m_pMPDI->SetPos(e_vPos);
		m_pMPDI->SetScale(e_fScale);
        m_bClickToPlay = e_bClickToPlay;
        if( m_bClickToPlay )
        {
            m_bStop = true;
            m_pMPDI->SetLoop(false);
        }
        else
        {
            m_bStop = false;
			if( !m_pMPDI->IsStopAtLastFrame() )
				m_pMPDI->SetLoop(true);
			else
				m_pMPDI->SetLoop(false);
        }
    }
    
    cEventMPDI::cEventMPDI(cEventMPDI*e_pEvent_MPDI):cEventObject(e_pEvent_MPDI)
    {
        m_pMPDI = new cMultiPathDynamicImage(e_pEvent_MPDI->m_pMPDI);
		m_pMPDI->SetOwner(e_pEvent_MPDI->m_pMPDI->GetOwner());
        m_pMPDI->Init();
		m_pMPDI->Update(EPSIONAL);
        //m_pMPDI->SetLoop(true);
        m_bClickToPlay = e_pEvent_MPDI->m_bClickToPlay;
        m_bStop = e_pEvent_MPDI->m_bStop;
    }

	cEventMPDI::cEventMPDI(TiXmlElement*e_pTiXmlElement):cEventObject(eEOT_MPDI,Vector3::Zero)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cEventMPDI::TypeID);
		m_pMPDI = 0;
		cMPDIList*l_pMPDIList = 0;
		bool	l_bLoop = false;
		float	l_fScale = 1.f;
		std::string	l_strRelativePath;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("MPDIList")
			{
		        char*e_strMPDIName = cMPDIList::GetFileName((WCHAR*)l_strValue);
		        //char*    l_strMPDIListFullName = FileNameConbineCurrentPageDirectory(e_strMPDIName);
		        //bool    l_b = cGameApp::m_spAnimationParser->Parse(l_strMPDIListFullName);
		        std::string l_str = *g_pCurrentEventManager->m_pstrWorkingDirectory;
				if( l_strRelativePath.length() )
				{
					l_str += l_strRelativePath;
					l_str += "/";
				}
		        l_str += e_strMPDIName;
				l_pMPDIList = dynamic_cast<cMPDIList*>(cGameApp::m_spAnimationParser->GetMPDIListByFileName(l_str.c_str()));
			    assert(l_pMPDIList);
			}
			else
			COMPARE_NAME("MPDI")
			{
				if( l_pMPDIList )
				{
					cMPDI*l_pMPDI = l_pMPDIList->GetObject(l_strValue);
					if( !l_pMPDI )
					{
						UT::ErrorMsg(L"mpdi name error",l_strValue);
					}
					else
					{
						this->m_pMPDI = new cMPDI(l_pMPDI);
						m_pMPDI->SetOwner(l_pMPDI->GetOwner());
						l_bLoop = m_pMPDI->IsLoop();
					}
				}
			}
			else
			COMPARE_NAME("ClickToPlay")
			{
			    m_bClickToPlay = VALUE_TO_BOLLEAN;
			}
			else
			COMPARE_NAME("Pos")
			{
			    m_vPos = VALUE_TO_VECTOR3;
			}
			else
			COMPARE_NAME("RelativePath")
			{
				l_strRelativePath = UT::WcharToChar(l_strValue);
			}
			else
			COMPARE_NAME("StopAtLastFrame")
			{
				m_pMPDI->SetStopAtLastFrame(VALUE_TO_BOLLEAN);
			}
			else
			COMPARE_NAME("Scale")
			{
				l_fScale = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Loop")
			{
				l_bLoop = VALUE_TO_BOLLEAN;
			}
	    PARSE_NAME_VALUE_END
		if( m_pMPDI )
		{
			m_pMPDI->SetScale(l_fScale);
			m_pMPDI->SetLoop(l_bLoop);
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
		}
        if( m_bClickToPlay )
        {
            m_bStop = true;
            m_pMPDI->SetLoop(false);
        }
        else
        {
            m_bStop = false;
			if( !m_pMPDI->IsStopAtLastFrame() )
				m_pMPDI->SetLoop(true);
			else
				m_pMPDI->SetLoop(false);
        }
	}

    cEventMPDI::~cEventMPDI()
    {
        SAFE_DELETE(m_pMPDI);
    }
    
    void	cEventMPDI::InternalInit()
	{
        m_bStop = true;
		if( !m_bClickToPlay )
		{
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
		}
		else
		{
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
			m_bStop = false;
		}
	}

    void    cEventMPDI::InternalMouseUp(int e_iPosX,int e_iPosY)
    {
        m_bStop = false;
		if( m_bClickToPlay )
		{
			m_pMPDI->Init();
			m_pMPDI->Update(EPSIONAL);
		}
    }
    
    bool    cEventMPDI::InternalCollide(int e_iPosX,int e_iPosY)
    {
        if( m_bClickToPlay )
        {
            if(!this->m_pMPDI->Collide(e_iPosX,e_iPosY))
                return false;
			return true;
        }
        return false;
    }

    void    cEventMPDI::InternalUpdate(float e_fElpaseTime)
    {
        if( m_bClickToPlay )
        {
            if( !m_bStop )
            {
                m_pMPDI->Update(e_fElpaseTime);
                if( m_pMPDI->IsDone() )
                {
                    m_bStop = true;
                }
            }
        }
        else
        {
            m_pMPDI->Update(e_fElpaseTime);
        }
    }

	void    cEventMPDI::DebugRender()
	{
		if(this->m_pMPDI)
			this->m_pMPDI->RenderCollide();
	}

    void    cEventMPDI::InternalRender()
    {
		Vector3 l_vPos = GetPosWithPagePos();
		m_pMPDI->SetPos(l_vPos);
        if( m_bClickToPlay&&m_bStop )
			m_pMPDI->RenderFirsttFrameImage(true,Vector4(1.f,1.f,1.f,0.3f));
        else 
            m_pMPDI->Render();
    }

	bool	cEventMPDI::IsPlayDone()
	{
		return m_pMPDI->IsDone();
	}

    NamedTypedObject*   cEventMPDI::GetResourcePointer()
    {
		NamedTypedObject*l_pNamedTypedObject = m_pMPDI->GetOwner();
        return l_pNamedTypedObject;
    }
	//	<EventObject Type="MPDI ">
		//<Data Pos="0.00,0.00" MPDIList="SelectMode_mpdi" MPDI="Selected" Scale="1" ClickToPlay="0" StopAtLastFrame="1" Loop="1" />
	//	</EventObject>
	TiXmlElement*	cEventMPDI::ToTiXmlElement()
	{
		TiXmlElement*l_pDataTiXmlElement = new TiXmlElement(L"Data");
		l_pDataTiXmlElement->SetAttribute(L"Pos",ValueToStringW(*this->GetPos()));
		l_pDataTiXmlElement->SetAttribute(L"MPDIList",this->m_pMPDI->GetOwner()->GetName());
		l_pDataTiXmlElement->SetAttribute(L"MPDI",this->m_pMPDI->GetName());
		l_pDataTiXmlElement->SetAttribute(L"Scale",ValueToStringW(this->m_pMPDI->GetScale()));
		l_pDataTiXmlElement->SetAttribute(L"ClickToPlay",this->m_bClickToPlay?L"1":L"0");
		l_pDataTiXmlElement->SetAttribute(L"StopAtLastFrame",this->m_pMPDI->IsStopAtLastFrame()?L"1":L"0");
		l_pDataTiXmlElement->SetAttribute(L"Loop",this->m_pMPDI->IsLoop()?L"1":L"0");
		return l_pDataTiXmlElement;
		TiXmlElement*l_pEventObjectTiXmlElement = new TiXmlElement(L"EventObject");
		l_pEventObjectTiXmlElement->SetAttribute(EVENT_OBJECT_TYPE,L"MPDI");
		l_pEventObjectTiXmlElement->LinkEndChild(l_pDataTiXmlElement);
		return l_pEventObjectTiXmlElement;
	}
//end namespace
}