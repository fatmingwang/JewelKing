#include "stdafx.h"
#include "ProbabilityResultCurve.h"



//
cDataAndTime::cDataAndTime(float*e_pData,int e_iCount)
{
	pCompressData = 0;
	pOriginalData = 0;
	m_fMaxValue = m_fMinValue = 0.f;
	SetData(e_pData,e_iCount);
}
//
cDataAndTime::~cDataAndTime()
{
	SAFE_DELETE(pOriginalData);
	SAFE_DELETE(pCompressData);
}
//
void	cDataAndTime::SetData(float*e_pData,int e_iCount)
{
	SAFE_DELETE(pOriginalData);
	SAFE_DELETE(pCompressData);
	pOriginalData = new sDataAndCount();
	pOriginalData->iOriginalCount = e_iCount;
	memcpy(pOriginalData->pOriginalfData,e_pData,sizeof(float)*e_iCount);
	AssignMinMaxValue(pOriginalData);
}
//
void	cDataAndTime::AssignMinMaxValue(sDataAndCount*e_pData)
{
	m_fMaxValue = (float)INT_MIN;
	m_fMinValue = (float)INT_MAX;
	for( int i=0;i<e_pData->iOriginalCount;++i )
	{
		float	l_fValue = e_pData->pOriginalfData[i];
		if( m_fMaxValue < l_fValue )
			m_fMaxValue = l_fValue;
		if( m_fMinValue > l_fValue )
			m_fMinValue = l_fValue;
	}
}
//
void	cDataAndTime::Compress(float e_fPercentage)
{

}
//
void	cDataAndTime::LimitCount(int e_iCount)
{

}
//
void	cDataAndTime::ShowSpecificCount(int e_iStartCount,int e_iEndCount)
{

}
//
sDataAndCount*	cDataAndTime::GetData()
{
	return pOriginalData;
}
//
void	cDataAndTime::Render(int e_iPosX,int e_iPosY,Vector4 e_vRenderRect)
{
	sDataAndCount*l_pSrc = pCompressData;
	if( !l_pSrc )
		l_pSrc = pOriginalData;
	if( !l_pSrc )
		return;
	float	l_fPointWidthDistance = e_vRenderRect.Width()/l_pSrc->iOriginalCount;
	float	l_fPointHeightDistance = e_vRenderRect.Height()/(m_fMaxValue-m_fMinValue);
	//for( int i=0;i< )
	//e_vRenderRect.Height();
}