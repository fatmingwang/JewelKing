#include "stdafx.h"
#include "SlotRule.h"
//
//<SlotLineData	Data="1,2,3,4,5,6,7,8,9,10" />
SlotLineData	GetSlotLineData(TiXmlElement*e_pTiXmlElement)
{
	SlotLineData	l_SlotLineData;
	const WCHAR*l_strData = e_pTiXmlElement->FirstAttribute()->Value();
	WCHAR*	l_str = wcstok((WCHAR*)l_strData,L", ");
	while(l_str)
	{
		std::wstring	l_strData = l_str;
		l_SlotLineData.push_back(l_strData);
		l_str = wcstok(0,L", ");
	}
	return l_SlotLineData;
}

//<LineRule IndexForOneLine="3,6,9,12,15" />
sLineRule::sLineRule(TiXmlElement*e_pElement)
{
	const WCHAR*l_strIndexForOneLine = e_pElement->Attribute(L"IndexForOneLine");
	std::vector<char>	l_Data = GetValueListByCommaDivide<char>(l_strIndexForOneLine);
	cOneLineCount = (char)l_Data.size();
	pcLineObjectIndex = new char[cOneLineCount];
	for( char i=0;i<cOneLineCount;++i )
	{
		pcLineObjectIndex[i] = l_Data[i];
	}
}
sLineRule::~sLineRule()
{
	SAFE_DELETE(pcLineObjectIndex);
}


cSlotRuleData::cSlotRuleData()
{
	m_iNumFreeGameToAchevied = 3;
}


cSlotRuleData::~cSlotRuleData()
{
	Destroy();
}

bool	cSlotRuleData::MyParse(TiXmlElement*e_pRoot)
{
	const WCHAR*l_strTableCount = e_pRoot->Attribute(L"TableCount");
	const WCHAR*l_strShowRect = e_pRoot->Attribute(L"ShowRect");
	m_TableCount = GetPoint(l_strTableCount);
	m_vShowRect = GetVector4(l_strShowRect);
	TiXmlElement*l_pElement = e_pRoot->FirstChildElement();
	
	while( l_pElement )
	{
		const WCHAR*l_strValue = l_pElement->Value();
		if( !wcscmp(l_strValue,L"SpineObjectPrizeTable") )
			ProcessSpineObjectPrizeTable(l_pElement);
		else
		if( !wcscmp(l_strValue,L"SlotLineObjectData") )
			ProcessSlotLineData(l_pElement);
		else
		if( !wcscmp(l_strValue,L"LineRule") )
			ProcessLineRule(l_pElement);
		else
		if( !wcscmp(l_strValue,L"BetMoneyRule") )
			ProcessPerLineBetMoneyRule(l_pElement);
		l_pElement = l_pElement->NextSiblingElement();
	}
	return true;
}
	//<SpineObjectPrizeTable>
	//	<SpinObjectData Name="1" >
	//		<Prize Count="2" Odds="1" />
	//		<Prize Count="3" Odds="2" />
	//		<Prize Count="4" Odds="3" />
	//		<Prize Count="5" Odds="4" />
	//	</SpinObjectData>
	//</SpineObjectPrizeTable>
void	cSlotRuleData::ProcessSpineObjectPrizeTable(TiXmlElement*e_pElement)
{
	const WCHAR*	l_strSpecialeSpinObject = e_pElement->Attribute(L"SpecialeSpinObject");
	if( l_strSpecialeSpinObject )
		this->m_strSpecilaSpinObjectName = l_strSpecialeSpinObject;
	const WCHAR*	l_strFreeGameImage = e_pElement->Attribute(L"FreeGameImage");
	m_strFreeGameImageName = l_strFreeGameImage;
	TiXmlElement*l_pElement = e_pElement->FirstChildElement();
	while(l_pElement)
	{
		cSpineObjectPrizeTable*l_pSpineObjectPrizeTable = new cSpineObjectPrizeTable(l_pElement);
		if(!m_SpineObjectPrizeTableList.AddObject(l_pSpineObjectPrizeTable))
		{
			assert(0&&"SpineObjectPrizeTable exists");
		}
		l_pElement = l_pElement->NextSiblingElement();
	}
	m_SpineObjectPrizeTableList.CalculateTotalProbabilityAmount();
}
//<SlotLineObjectData>
//	<SlotLineData	Data="1,2,3,4,5,6,7,8,9,10" />
//	<SlotLineData	Data="1,2,3,4,5,6,7,8,9,10" />
//	<SlotLineData	Data="1,2,3,4,5,6,7,8,9,10" />
//	<SlotLineData	Data="1,2,3,4,5,6,7,8,9,10" />
//</SlotLineObjectData>
void	cSlotRuleData::ProcessSlotLineData(TiXmlElement*e_pElement)
{
	TiXmlElement*l_pElement = e_pElement->FirstChildElement();
	while(l_pElement)
	{
		SlotLineData	l_SlotLineData = GetSlotLineData(l_pElement);
		m_SlotLineDataList.push_back(l_SlotLineData);
		l_pElement = l_pElement->NextSiblingElement();
	}
}
//<LineRule>
//	<LineRule IndexForOneLine="1,4,7,10,13" />
//	<LineRule IndexForOneLine="2,5,8,11,14" />
//	<LineRule IndexForOneLine="3,6,9,12,15" />
//</LineRule>
void	cSlotRuleData::ProcessLineRule(TiXmlElement*e_pElement)
{
	TiXmlElement*l_pElement = e_pElement->FirstChildElement();
	while(l_pElement)
	{
		sLineRule*l_pLineRule = new sLineRule(l_pElement);
		m_LineRuleList.push_back(l_pLineRule);
		l_pElement = l_pElement->NextSiblingElement();
	}
}
//<BetMoneyRule BetPerLine="1,5,10"/>
void	cSlotRuleData::ProcessPerLineBetMoneyRule(TiXmlElement*e_pElement)
{
	const WCHAR*l_strValue = e_pElement->Attribute(L"BetPerLine");
	m_PerLineBetMoneyRule = GetValueListByCommaDivide<int>(l_strValue);
}

void	cSlotRuleData::Destroy()
{
	DELETE_VECTOR(m_LineRuleList,sLineRule*);
	m_SpineObjectPrizeTableList.Destroy();
	m_SlotLineDataList.clear();
}

int		cSlotRuleData::GetFreeSpinObjectIndex()
{
	return m_SpineObjectPrizeTableList.GetObjectIndexByName(m_strFreeGameImageName.c_str());
}

int		cSlotRuleData::GetWildObjectIndex()
{
	return m_SpineObjectPrizeTableList.GetObjectIndexByName(m_strSpecilaSpinObjectName.c_str());
}

int		cSlotRuleData::GetMinScore(int e_iMultiplier)
{
	return this->m_SpineObjectPrizeTableList.GetMinScore()*e_iMultiplier;
}

int		cSlotRuleData::GetMaxScore(int e_iMultiplier)
{
	return this->m_SpineObjectPrizeTableList.GetMaxScore()*e_iMultiplier;
}

int		cSlotRuleData::GetMinScoreSpinObjectIndex()
{
	return m_SpineObjectPrizeTableList.GetMinScoreSpinObjectIndex();
}

int		cSlotRuleData::GetMaxScoreSpinObjectIndex()
{
	return m_SpineObjectPrizeTableList.GetMaxScoreSpinObjectIndex();
}

int		cSlotRuleData::GetRandomSpinObject(bool e_bAvoidFree,bool e_bAvoidJP)
{
	int	l_iSpinObjectIndex = this->m_SpineObjectPrizeTableList.GetSpinObjectIndexByProbability();
	int	l_iFree = GetFreeSpinObjectIndex();
	int	l_iWild = GetWildObjectIndex();
	while( (e_bAvoidFree&&(l_iSpinObjectIndex == l_iFree))||
		(e_bAvoidJP&&(l_iSpinObjectIndex == l_iWild)))
	{
		l_iSpinObjectIndex = this->m_SpineObjectPrizeTableList.GetSpinObjectIndexByProbability();
	}
	return l_iSpinObjectIndex;
}

int		cSlotRuleData::GetMaxWinMoney(int e_iMultiplier)
{
	return this->m_LineRuleList.size()*m_SpineObjectPrizeTableList.GetMaxScore()*e_iMultiplier;
}

POINT	cSlotRuleData::SpinObjectIndexToRowColumn(int e_iIndex)
{
	POINT l_POINT = {-1,-1};
	//row,SpinObjectManager index
	l_POINT.x = e_iIndex%this->m_TableCount.x;
	//column
	l_POINT.y = e_iIndex/this->m_TableCount.x;
	return l_POINT;
}