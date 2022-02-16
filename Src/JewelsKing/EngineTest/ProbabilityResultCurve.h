#ifndef _PROBABILITY_RESULT_CURVE_H_
#define _PROBABILITY_RESULT_CURVE_H_

struct	sDataAndCount
{
	int		iOriginalCount;
	float*	pOriginalfData;
	sDataAndCount(){ iOriginalCount = 0;pOriginalfData = 0;}
	~sDataAndCount(){ iOriginalCount = 0;SAFE_DELETE(pOriginalfData);}
};

class	cDataAndTime
{
	//original data
	sDataAndCount*	pOriginalData;
	//compress data,ofinal render data
	sDataAndCount*	pCompressData;
	//for render data
	float			m_fMaxValue;
	float			m_fMinValue;
	void			AssignMinMaxValue(sDataAndCount*e_pData);
public:
	cDataAndTime(float*e_pData,int e_iCount);
	~cDataAndTime();
	void	SetData(float*e_pData,int e_iCount);
	//compress pOriginalData to pCompressData,so we could show needed sample
	void	Compress(float e_fPercentage);
	//
	void	LimitCount(int e_iCount);
	//2 point view
	void	ShowSpecificCount(int e_iStartCount,int e_iEndCount);
	//
	sDataAndCount*	GetData();
	//
	void	Render(int e_iPosX,int e_iPosY,Vector4 e_vRenderRect);
};

#endif