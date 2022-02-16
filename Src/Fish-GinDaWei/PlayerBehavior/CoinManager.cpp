/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "Coin.h"

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cCoinManager::cCoinManager()
{

	float l_fFlyTime = 0.0f;
	cNodeISAX	l_NodeISAX;
	bool l_b = l_NodeISAX.ParseDataIntoXMLNode(COIN_DATA);
	if(l_b)
	{
		TiXmlElement*l_pElement = l_NodeISAX.GetRootElement();
		l_pElement = l_pElement->FirstChildElement();
		
		const WCHAR*l_strName = l_pElement->Value();

		while( l_pElement )
		{
			l_strName = l_pElement->Value();
			COMPARE_NAME("CoinData")
			{
				const WCHAR*l_strFT   = l_pElement->Attribute(L"FlyTime");
				l_fFlyTime = (float)_wtof(l_strFT);
			}
			COMPARE_NAME("Coin")
			{
				ProcessCoin(l_pElement);
			}
			else
			COMPARE_NAME("PlayerCoinStack")
			{
				ProcessCoinStack(l_pElement);
			}
			l_pElement = l_pElement->NextSiblingElement();
		} //end while
	}
	else
	{
		UT::ErrorMsg(L"PlayerData.xml",L"not exit");
	} //end if

	m_Coin = new cCoin*[COUNT_COINS];
	for(int i=0;i<COUNT_COINS;++i)
	{
		m_Coin[i] = new cCoin();
		m_Coin[i]->m_fFlyTime = l_fFlyTime;
	}	

}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
cCoinManager::~cCoinManager()
{	
	for(int i=0;i<COUNT_COINS;++i)
	{
		SAFE_DELETE(m_Coin[i]);
	}
	SAFE_DELETE(m_Coin);
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoinManager::Init()
{	
	for(int i=0;i<COUNT_COINS;++i)
	{
		m_Coin[i]->Init();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoinManager::Update(float e_fElpaseTime)
{
	int DataIdx[4] = {-1, -1, -1, -1 };

	for ( int j = 0; j < COUNT_COINS; j++ )
	{
		 bool bUpData(1); 

		 if ( m_Coin[j]->m_iStackPosIndex != -1 && m_Coin[j]->m_eCointStatus == eCoinStack )
		 {
			 if ( m_Coin[j]->m_NowAccessIdx == m_Coin[j]->m_iStackPosIndex )
				 DataIdx[0] = j; 
			 else
			 {
				 for ( int k = 1; k <= 3; k++ )
				 {
					  int NextVal = m_Coin[j]->m_NowAccessIdx + k;

					  if ( NextVal >= 50 )
						  NextVal -= 50;

					  if ( m_Coin[j]->m_iStackPosIndex == NextVal )
					  {
						  bUpData = false;
						  DataIdx[k] = j;
						  break;
					  }
				 }
			 }
		 }

		 if ( bUpData )
			 m_Coin[j]->Update( e_fElpaseTime );
	}

	for ( int i = 0; i < 4; ++i )
	{
		 if ( DataIdx[i] != -1 )
			 m_Coin[ DataIdx[ i ] ]->Update( e_fElpaseTime );
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoinManager::Render()
{
	int DataIdx[4] = { -1, -1, -1, -1 };

	for ( int j = 0; j < COUNT_COINS; j++ )
	{
		 bool bUpData(1); 

		 if ( m_Coin[j]->m_iStackPosIndex != -1 && m_Coin[j]->m_eCointStatus == eCoinStack )
		 {
			 if ( m_Coin[j]->m_NowAccessIdx == m_Coin[j]->m_iStackPosIndex )
				 DataIdx[0] = j; 
			 else
			 {
				 for ( int k = 1; k <= 3; k++ )
				 {
					  int NextVal = m_Coin[j]->m_NowAccessIdx + k;

					  if ( NextVal >= 50 )
						  NextVal -= 50;

					  if ( m_Coin[j]->m_iStackPosIndex == NextVal )
					  {
						  bUpData = false;
						  DataIdx[k] = j;
						  break;
					  }
				 }
			 }
		 }

		 if ( bUpData )
			 m_Coin[j]->Render();
	}

	for ( int i = 0; i < 4; ++i )
	{
		 if ( DataIdx[i] != -1 )
			 m_Coin[ DataIdx[ i ] ]->Render();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoinManager::Destroy()
{
	for(int i=0;i<COUNT_COINS;++i)
	{
		m_Coin[i]->Destroy();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoinManager::ProcessCoin(TiXmlElement*e_pElement)
{
	e_pElement = e_pElement->FirstChildElement();
	const WCHAR*l_strName = e_pElement->Value();
	while(e_pElement)
	{
		l_strName = e_pElement->Value();
		COMPARE_NAME("CoinLayer")
		{
			const WCHAR*l_strMin   = e_pElement->Attribute(L"Min");
			this->m_vMinFishPrize.push_back(_wtoi(l_strMin));
			const WCHAR*l_strMax   = e_pElement->Attribute(L"Max");
			this->m_vMaxFishPrize.push_back(_wtoi(l_strMax));
			const WCHAR*l_strIndex = e_pElement->Attribute(L"Index");
			this->m_vCoinIndex.push_back(_wtoi(l_strIndex));
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
void cCoinManager::ProcessCoinStack(TiXmlElement*e_pElement)
{
	e_pElement = e_pElement->FirstChildElement();
	const WCHAR*l_strName = e_pElement->Value();
	while(e_pElement)
	{
		l_strName = e_pElement->Value();
		COMPARE_NAME("CoinStack")
		{
			const WCHAR*l_strMin   = e_pElement->Attribute(L"Min");
			this->m_vMinStackNum.push_back(_wtoi(l_strMin));
			const WCHAR*l_strMax   = e_pElement->Attribute(L"Max");
			this->m_vMaxStackNum.push_back(_wtoi(l_strMax));
			const WCHAR*l_strIndex = e_pElement->Attribute(L"Index");
			this->m_vStackIndex.push_back(_wtoi(l_strIndex));
		}
		e_pElement = e_pElement->NextSiblingElement();
	}
}

/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
int cCoinManager::StackIndex(int e_iFishMoney)
{
	if( e_iFishMoney < 10)
	{
		e_iFishMoney = 10;
	}
	if(e_iFishMoney > 10000)
	{
		e_iFishMoney = 10;
	}
	for(int i=0;i< 4;++i){
		if(e_iFishMoney	>=	this->m_vMinStackNum[i] && e_iFishMoney <= this->m_vMaxStackNum[i])
		{
			 m_iStackIndex = this->m_vStackIndex[i];
		}
	}
	return	m_iStackIndex;
}


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
int cCoinManager::CoinIndex(int e_iFishPrize)
{

	if(e_iFishPrize<2)
	{
		 e_iFishPrize = 2;
	}
	if(e_iFishPrize > 90)
	{
		e_iFishPrize = 90;
	}

	for(int i=0;i< 4;++i){
		if(e_iFishPrize	>=	this->m_vMinFishPrize[i] && e_iFishPrize <= this->m_vMaxFishPrize[i])
		{
			 m_iCoinIndex = this->m_vCoinIndex[i];
		}
	}
	return	m_iCoinIndex;
}


/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   :
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
