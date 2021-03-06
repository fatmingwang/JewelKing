#include "../stdafx.h"
#include "ParticleBase.h"
#include "PrtLife.h"
namespace FATMING_CORE
{
	const wchar_t*         cPrtLifeInitrSetLife::TypeID( L"cPrtLifeInitrSetLife" );
	const wchar_t*         cPrtLifeActDyingByGameTime::TypeID( L"cPrtLifeActDyingByGameTime" );
	//CONST WCHAR* cPrtLifeActRebirth::TypeID( L"cPrtLifeActRebirth" );
	char*	cPrtLifeInitrSetLife::UsageExplanation =
	{
		"LifeMin for the base file,\nLifeRange will add with baselife,\nif Random is active the randge will fetch a random betwnn 0 to m_fLifeRange\n			LifeMin 為最小生命值\nLifeRange為該生命可增加的範圍,當random被作用將會將lifeRange做一0到指定數值的加給"

	};
	char*	cPrtLifeActDyingByGameTime::UsageExplanation =
	{
		"life time minuse elpase time ,將生命減少"
	};

	NamedTypedObject*  cPrtLifeInitrSetLife::Clone()
	{
		cPrtLifeInitrSetLife* l_p = new cPrtLifeInitrSetLife; 
		l_p->SetLifeMin(this->m_fLifeMin);
		l_p->SetLifeRange(this->m_fLifeRange);
		l_p->SetRandom(this->m_bRandom);
		l_p->SetName(GetName());
		return l_p; 
	};

	char*	cPrtLifeInitrSetLife::GetDataInfo()
	{

		sprintf(m_sTemp,"LifeMin:=%.2f\nLifeRange:%.2f\nRandom:%s\0",m_fLifeMin,m_fLifeRange,m_bRandom?"true":"false");
		return m_sTemp;
	}
	//this one for output data value by "," to separate value
	char*	cPrtLifeInitrSetLife::GetOutputDataString()
	{
		sprintf(m_sTemp,"%.2f,%.2f,%d\0",m_fLifeMin,m_fLifeRange,m_bRandom?1:0);
		return m_sTemp;
	}
	//input the output data string,and analyze it
	bool	cPrtLifeInitrSetLife::SetDataByDataString(char*e_pString)
	{
		char*Nexttoken = 0;
		char* l_pString = strtok(e_pString,",");
		this->m_fLifeMin = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_fLifeRange = (float)atof(l_pString);
		l_pString = strtok(0,",");
		this->m_bRandom = atoi(l_pString)?true:false;
		if( strtok(0,",") )
			return false;
		return true;
	}
}