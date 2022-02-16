#include "stdafx.h"
#include "ControlPanel.h"
#include "../../Core/GameplayUT/Event/EventButtonTrigger.h"
extern cControlPanel*g_pControlPanel;
#define	MAGIC_NUMBER_FOR_ENCODE	0xabcdef01
#define	MAXIMUM_DATA_COUNT	498
unsigned int	DeCodeHintCode( unsigned int e_iValue)
{
	unsigned int	l_iValue1 = e_iValue^0xffffffff;
	unsigned int	l_iValue2 = l_iValue1+MAGIC_NUMBER_FOR_ENCODE;
	return l_iValue2;
}

 int	GenerationHintCode( int e_iValue)
{
	unsigned int	l_iValue1 = (unsigned int)e_iValue-MAGIC_NUMBER_FOR_ENCODE;
	unsigned int	l_iValue2 = l_iValue1^0xffffffff;
	//if( e_iValue != DeCodeHintCode(l_iValue2) )
	//{
	//	int a=0;
	//}
	return l_iValue2;
}

void	GeneratingVendorHintCode()
{
	cEventVariable*l_pEventVariable = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"VendorHintCode");
	cEventVariable*l_pEventVariable2 = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"VendorPassword");
	std::wstring	l_str = l_pEventVariable2->ConvertToString();
	std::vector<int>	l_PasswordVector = GetValueListByCommaDivide<int>(l_str.c_str());
	int	l_iValue = 0;
	int	l_iDecimal = 1;
	for(int i=l_PasswordVector.size()-1;i>-1;--i)
	{
		l_iValue += l_iDecimal*l_PasswordVector[i];
		l_iDecimal *= 10;
	}
	int	l_Encode = GenerationHintCode(l_iValue);
	//int	l_iDecode = DeCodeHintCode(l_Encode);
	l_pEventVariable->SetInt(l_Encode);
}

void	GeneratingAuditHintCode()
{
	cEventVariable*l_pEventVariable = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"AuditHintCode");
	cEventVariable*l_pEventVariable2 = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"AuditPassword");
	std::wstring	l_str = l_pEventVariable2->ConvertToString();
	std::vector<int>	l_PasswordVector = GetValueListByCommaDivide<int>(l_str.c_str());
	int	l_iValue = 0;
	int	l_iDecimal = 1;
	for(int i=l_PasswordVector.size()-1;i>-1;--i)
	{
		l_iValue += l_iDecimal*l_PasswordVector[i];
		l_iDecimal *= 10;
	}
	int	l_Encode = GenerationHintCode(l_iValue);
	//int	l_iDecode = DeCodeHintCode(l_Encode);
	l_pEventVariable->SetInt(l_Encode);
}

void	EarningOver1000Score(int e_iMoney)
{
	int	l_iMoney = e_iMoney;
	std::wstring l_Time = GetSystemTime(true);

	cNodeISAX	l_NodeISAX;
	TiXmlElement *l_pRoot = 0;
	TiXmlDocument l_Doc;
	std::string	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(WIN_BIG_MONEY_FILE_NAME);
	if(UT::IsFileExists(l_strFileName.c_str()))
	{//get old file
		l_NodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str());
		if( l_NodeISAX.GetRootElement() )
			l_pRoot = new TiXmlElement(*l_NodeISAX.GetRootElement());
	}
	if( !l_pRoot )
	{//new file
		l_pRoot = new TiXmlElement(L"Root");
	}
	l_Doc.LinkEndChild( l_pRoot );
	TiXmlElement *l_pTime = new TiXmlElement(L"Data");
	l_pTime->SetAttribute(L"Time",l_Time.c_str());
	l_pTime->SetAttribute(L"Score",e_iMoney);
	l_pRoot->LinkEndChild(l_pTime);
	int	l_iCount = l_pRoot->ChildElementCount();
	if( l_iCount > MAXIMUM_DATA_COUNT )
	{
		int	l_iStripValue = l_iCount-MAXIMUM_DATA_COUNT;
		int	l_iIndex = 0;
		TiXmlElement*l_pTiXmlElement = l_pRoot->FirstChildElement();
		while(l_pTiXmlElement)
		{
			if( l_iIndex == l_iStripValue )
			{
				break;
			}
			TiXmlElement*l_pDeleteTiXmlElement = l_pTiXmlElement;
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			l_pRoot->RemoveChild(l_pDeleteTiXmlElement);
			++l_iIndex;
		}
	}
	l_Doc.SaveFile(l_strFileName.c_str());
}

void	WorkerShitfChange(void*e_pData)
{
	cEventManager*l_pEventManager = g_pControlPanel->GetEventManager();
	cEventVariable*l_pEventVariable3 = l_pEventManager->m_pEventVariableManager->GetObject(L"GameRecord_Combo3");
	cEventVariable*l_pEventVariable4 = l_pEventManager->m_pEventVariableManager->GetObject(L"GameRecord_Combo4");
	cEventVariable*l_pEventVariable5 = l_pEventManager->m_pEventVariableManager->GetObject(L"GameRecord_Combo5");
	cEventVariable*l_pGamePrize_Combo3 = l_pEventManager->m_pEventVariableManager->GetObject(L"GamePrize_Combo3");
	cEventVariable*l_pGamePrize_Combo4 = l_pEventManager->m_pEventVariableManager->GetObject(L"GamePrize_Combo4");
	cEventVariable*l_pGamePrize_Combo5 = l_pEventManager->m_pEventVariableManager->GetObject(L"GamePrize_Combo5");	
	cEventVariable*l_pGameRecord_LocalJP = l_pEventManager->m_pEventVariableManager->GetObject(L"GameRecord_LocalJP");
	cEventVariable*l_pGamePrize_LocalJP = l_pEventManager->m_pEventVariableManager->GetObject(L"GamePrize_LocalJP");
	cEventVariable*l_pGameRecord_NetWorkJP = l_pEventManager->m_pEventVariableManager->GetObject(L"GameRecord_NetWorkJP");
	cEventVariable*l_pGamePrize_NetWorkJP = l_pEventManager->m_pEventVariableManager->GetObject(L"GamePrize_NetWorkJP");
	
	l_pGamePrize_LocalJP->SetInt(0);
	l_pGameRecord_LocalJP->SetInt(0);
	l_pGamePrize_NetWorkJP->SetInt(0);
	l_pGameRecord_NetWorkJP->SetInt(0);

	l_pEventVariable3->SetInt(0);
	l_pEventVariable4->SetInt(0);
	l_pEventVariable5->SetInt(0);
	l_pGamePrize_Combo3->SetInt(0);
	l_pGamePrize_Combo4->SetInt(0);
	l_pGamePrize_Combo5->SetInt(0);

	cEventVariable*l_p1 = l_pEventManager->m_pEventVariableManager->GetObject(L"CurrentShift_ToastCoin");
	cEventVariable*l_p2 = l_pEventManager->m_pEventVariableManager->GetObject(L"CurrentShift_RefundCoin");
	cEventVariable*l_p3 = l_pEventManager->m_pEventVariableManager->GetObject(L"CurrentShift_ExchangeIn");
	cEventVariable*l_p4 = l_pEventManager->m_pEventVariableManager->GetObject(L"CurrentShift_ExchangeOut");
	cEventVariable*l_p5 = l_pEventManager->m_pEventVariableManager->GetObject(L"CurrentShift_Revenue");

	cEventVariable*l_p11 = l_pEventManager->m_pEventVariableManager->GetObject(L"PriorShift_ToastCoin");
	cEventVariable*l_p22 = l_pEventManager->m_pEventVariableManager->GetObject(L"PriorShift_RefundCoin");
	cEventVariable*l_p33 = l_pEventManager->m_pEventVariableManager->GetObject(L"PriorShift_ExchangeIn");
	cEventVariable*l_p44 = l_pEventManager->m_pEventVariableManager->GetObject(L"PriorShift_ExchangeOut");
	cEventVariable*l_p55 = l_pEventManager->m_pEventVariableManager->GetObject(L"PriorShift_Revenue");

	cNodeISAX	l_NodeISAX;
	TiXmlElement *l_pRoot = 0;
	TiXmlDocument l_Doc;
	std::string	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(WORKER_SHIFT_RECORD_FILE_NAME);
	if(UT::IsFileExists(l_strFileName.c_str()))
	{//get old file
		l_NodeISAX.ParseDataIntoXMLNode(l_strFileName.c_str());
		if( l_NodeISAX.GetRootElement() )
			l_pRoot = new TiXmlElement(*l_NodeISAX.GetRootElement());
	}
	if( !l_pRoot )
	{//new file
		l_pRoot = new TiXmlElement(L"Root");
	}
	l_Doc.LinkEndChild( l_pRoot );
	TiXmlElement *l_pTime = new TiXmlElement(L"Time");
	std::wstring  l_strTime = UT::GetSystemTime(false);
	l_pTime->SetAttribute(L"Time",l_strTime.c_str());
	l_pRoot->LinkEndChild(l_pTime);
	WCHAR*l_strDescription[] = {L"投幣",L"退幣",L"洗分",L"開分",L"收益"};
	int	l_iData[] = {*l_p1->GetInt(),*l_p2->GetInt(),*l_p3->GetInt(),*l_p4->GetInt(),*l_p5->GetInt()};
	for( int i=0;i<5;++i )
	{
		TiXmlElement*l_pTiXmlElement = new TiXmlElement( L"Data" );
		l_pTiXmlElement->SetAttribute(L"Description",l_strDescription[i]);
		l_pTiXmlElement->SetAttribute(L"Value",l_iData[i]);
		l_pRoot->LinkEndChild(l_pTiXmlElement);
	}
	//6xml data for a batch,so strip count over 498(6*83)
	int	l_iCount = l_pRoot->ChildElementCount();
	if( l_iCount > MAXIMUM_DATA_COUNT )
	{
		int	l_iStripValue = l_iCount-MAXIMUM_DATA_COUNT;
		int	l_iIndex = 0;
		TiXmlElement*l_pTiXmlElement = l_pRoot->FirstChildElement();
		while(l_pTiXmlElement)
		{
			if( l_iIndex == l_iStripValue )
			{
				break;
			}
			TiXmlElement*l_pDeleteTiXmlElement = l_pTiXmlElement;
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			l_pRoot->RemoveChild(l_pDeleteTiXmlElement);
			++l_iIndex;
		}
	}
	l_Doc.SaveFile(l_strFileName.c_str());

	l_p11->SetInt(*l_p1->GetInt());
	l_p22->SetInt(*l_p2->GetInt());
	l_p33->SetInt(*l_p3->GetInt());
	l_p44->SetInt(*l_p4->GetInt());
	l_p55->SetInt(*l_p5->GetInt());
	l_p1->SetInt(0);
	l_p2->SetInt(0);
	l_p3->SetInt(0);
	l_p4->SetInt(0);
	l_p5->SetInt(0);
}


void	ClearVendorTotalRecord_CurrendBalance(void*e_pData)
{
	cEventManager*l_pEventManager = g_pControlPanel->GetEventManager();
	cEventVariable*l_p1 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalIncome");
	cEventVariable*l_p2 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_TotalPay");
	cEventVariable*l_p3 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_Revenue");
	cEventVariable*l_p4 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ExchangeIn");
	cEventVariable*l_p5 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ExchangeOut");
	cEventVariable*l_p6 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_ToastCoin");
	cEventVariable*l_p7 = l_pEventManager->m_pEventVariableManager->GetObject(L"Latest_RefundCoin");

	//cEventVariable*l_p11 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_TotalIncome");
	//cEventVariable*l_p22 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_TotalPay");
	//cEventVariable*l_p33 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_Revenue");
	//cEventVariable*l_p44 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_ExchangeIn");
	//cEventVariable*l_p55 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_ExchangeOut");
	//cEventVariable*l_p66 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_ToastCoin");
	//cEventVariable*l_p77 = l_pEventManager->m_pEventVariableManager->GetObject(L"History_RefundCoin");

	//l_p11->SetInt(0);
	//l_p22->SetInt(0);
	//l_p33->SetInt(0);
	//l_p44->SetInt(0);
	//l_p55->SetInt(0);
	//l_p66->SetInt(0);
	//l_p77->SetInt(0);

	l_p1->SetInt(0);
	l_p2->SetInt(0);
	l_p3->SetInt(0);
	l_p4->SetInt(0);
	l_p5->SetInt(0);
	l_p6->SetInt(0);
	l_p7->SetInt(0);
}

void			ResetRecordData()
{
	char*	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(L"DefaultVariableData.xml");
	cEventVariableManager	l_EventVariableManager;
	l_EventVariableManager.Parse(l_strFileName);

	cEventVariableManager*l_pEventVariableManager = g_pControlPanel->GetEventManager()->m_pEventVariableManager;
	int	l_iCount = l_pEventVariableManager->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cEventVariable*l_pEventVariable1 = l_pEventVariableManager->GetObject(i);
		cEventVariable*l_pEventVariable2 = l_EventVariableManager.GetObject(l_pEventVariable1->GetName());
		std::wstring	l_str = l_pEventVariable2->ConvertToString();
		l_pEventVariable1->SetDataByString(l_str.c_str());
	}
}

void			ResetDefaultControlSetup(void*e_pData)
{
	ResetRecordData();
	char*	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(WORKER_SHIFT_RECORD_FILE_NAME);
	RemoveFile(l_strFileName);
	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(WIN_BIG_MONEY_FILE_NAME);
	RemoveFile(l_strFileName);
	RemoveFile("PlayerMoney.txt");
	g_pControlPanel->SetGameReload(true);
}

void			ClearRecord(void*e_pData)
{
	std::string	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(WORKER_SHIFT_RECORD_FILE_NAME);
	RemoveFile(l_strFileName.c_str());

	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(WIN_BIG_MONEY_FILE_NAME);
	RemoveFile(l_strFileName.c_str());


	
	l_strFileName = g_pControlPanel->GetEventManager()->GetFullpathFileName(SETUP_CHANGE_FILE_NAME);
	RemoveFile(l_strFileName.c_str());

	g_pControlPanel->StoreOldData();
	ResetRecordData();
	cEventVariable*l_pAuditPassword = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"AuditPassword");
	cEventVariable*l_pVendorPassword = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"VendorPassword");
	cEventVariable*l_pLinePlayable = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"LinePlayable");
	cEventVariable*l_pLanguage = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Language");
	cEventVariable*l_pProbability = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Probability");
	cEventVariable*l_pSound = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Sound");
	cEventVariable*l_pCoinToScore = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"CoinToScore");
	cEventVariable*l_pMoneyLimit = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"MoneyLimit");
	cEventVariable*l_pJPShareScorePercentage = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"JPShareScorePercentage");
	cEventVariable*l_pLuckyTime = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"LuckyTime");
	cEventVariable*l_pExchange_In = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Exchange_In");
	cEventVariable*l_pExchange_Out = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Exchange_Out");
	cEventVariable*l_pExchange_Ticket = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"Exchange_Ticket");
	cEventVariable*l_pLeastJP = g_pControlPanel->GetEventManager()->m_pEventVariableManager->GetObject(L"JPLeastMoney");

	l_pAuditPassword->SetWString(g_pControlPanel->m_strAuditPassword);
	l_pVendorPassword->SetWString(g_pControlPanel->m_strVendorPassword);
	l_pLinePlayable->SetInt(g_pControlPanel->m_iLineBetable);
	l_pLanguage->SetWString(g_pControlPanel->m_strLanguage);
	l_pProbability->SetFloat(g_pControlPanel->m_fProbability);
	l_pSound->SetInt(g_pControlPanel->m_iSound);
	l_pCoinToScore->SetInt(g_pControlPanel->m_iCoinToScore);
	l_pMoneyLimit->SetInt(g_pControlPanel->m_iMoneyLimit);
	l_pJPShareScorePercentage->SetFloat(g_pControlPanel->m_fJPShareScorePercentage);
	l_pLuckyTime->SetWString(g_pControlPanel->m_bLuckyTime?L"On":L"Off");
	l_pExchange_In->SetInt(g_pControlPanel->m_iExchangeIn);
	l_pExchange_Out->SetInt(-g_pControlPanel->m_iExchangeOut);
	l_pExchange_Ticket->SetInt(g_pControlPanel->m_iExchange_Ticket);
	l_pLeastJP->SetInt(g_pControlPanel->m_iLeastJP);

}

void			SaveSetupData(void*e_pData)
{
	g_pControlPanel->SaveData();
}