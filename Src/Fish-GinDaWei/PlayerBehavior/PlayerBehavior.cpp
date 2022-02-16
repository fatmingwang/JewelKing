/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "../EngineTest/PlayerBehaviorBase.h"
#include "PlayerData.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../ControlPanel/AllControlPanelInclude.h"


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cPlayerBehaviorBase::AddScore(Vector3 Pos, cMonster* Fish, int Money)
{
	m_iAddScoreCount++;
	AddScore2(Pos,Fish,Money);
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cPlayerManager::cPlayerManager()
{
	cClickMouseBehavior::m_sfRepeatTime = 0.1f;
	m_ipPlayerCount=0;
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cPlayerManager::~cPlayerManager()
{
	this->Destroy();
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cPlayerManager::Init()
{	
	Destroy();
	m_ipPlayerCount = &cFishApp::m_spControlPanel->m_iPlayerCount;
	if(this ->Count()==0)
	{

		bool l_b;
		cNodeISAX	l_NodeISAX;
		if( *m_ipPlayerCount == 1 )
		{
			l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYER_DATA1);
		}
		else if(*m_ipPlayerCount == 4)
		{
			l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYER_DATA4);
		}
		else if( *m_ipPlayerCount == 6 )
		{
			l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYER_DATA6);
		}
		else if( *m_ipPlayerCount == 8 )
		{
			l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYER_DATA8);
		}
		else if( *m_ipPlayerCount == 10 )
		{
			l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYER_DATA10);
		}
		else if( *m_ipPlayerCount == 14 )
		{
			l_b = l_NodeISAX.ParseDataIntoXMLNode(PLAYER_DATA14);
		} // end if

		if(l_b)
		{
			TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
			l_pElement = l_pElement->FirstChildElement();
			const WCHAR*l_strName = l_pElement->Value();
			while(l_pElement)
			{
				l_strName = l_pElement->Value();
				COMPARE_NAME("Player")
				{
					cPlayerBehaviorBase*playerData = new  cPlayerData(l_pElement);
					PARSE_ELEMENT_START(l_pElement)
					COMPARE_NAME("ID")
					{
						playerData->m_iPlayerID=VALUE_TO_INT;
					}
					else
					COMPARE_NAME("RotateSpeed")
					{
						playerData->m_fRotateSpeed=VALUE_TO_FLOAT;
					}
					PARSE_NAME_VALUE_END
					this->AddObjectNeglectExist(playerData);
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
		else
		{
			UT::ErrorMsg(L"PlayerData.xml",L"not exit");
		}
	}
	
	
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Init();
	}
	
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cPlayerManager::Update( float e_fElpaseTime )
{
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Update(e_fElpaseTime);
	}
	
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cPlayerManager::Render()
{
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Render();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void	cPlayerManager::Destroy()
{
	for(int i=0;i<this->Count();++i)
	{
		this->GetObject(i)->Destroy();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */