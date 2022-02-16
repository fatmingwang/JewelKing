#ifndef _SLOT_TEMPRATURE_SCORE_H_
#define _SLOT_TEMPRATURE_SCORE_H_

#include "SlotTimeStamp.h"



//ex:balance is 1000	1000/1000= 1		1/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*0.1 = 0.005
//ex:balance is 10000	10000/1000= 10		10/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*1 = 0.05
//ex:balance is 100000	100000/1000= 100	100/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*10 = 0.5
//ex:balance is 100000	1000000/1000= 1000	1000/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*100 = 5  but 5 is bigger than 0.5 so maximum is 0.5


//so the last score is Balance to ScroeMultiplier+(income/(out+1))*CurrentScore = FinalMultiplier
//if FinalMultiplier is bigger than 2 and it's 2.
//
//ExtraPercentageBase�B�~�ʤ��� = ����/BalanceMoneyStartBase(����}�l���)/NextLevelMultipler(����ŶZ���)*ExtraPercentageBase(�B�~�ʤ�����)
//ex:balance is 100000	100000/1000= 100	100/10(NextLevelMultipler) extrace percentage is 0.05(ExtraPercentageBase)*10 = 0.5
//
//�̤j���ƭ��v(MaxMultiplier) = �B�~�ʤ���+(���J/(��X+1))
//ex:0.5+(10000/(5000+1)) = 2.49
//2.49�j��MaxMultiplier �ҥH�̤j���ƭ��v��2

//���J6000��X5000 �B�~�ʤ���(6000-5000)/1000/10*0.05 = 0.005
//
//ex:0.005+(6000/(5000+1)) = 1.2047
//�ҥH�̤j���ƭ��v��1.2047
//�p�G�̤j���ƭ��v�j��2�h�̤j���ƭ��v��2

//�̤j���ƭ��vx�{�b���� �N�O�̫᪺���Ƽз�


//<!-- BalanceMoneyStartBase�B�~�ʤ���������   NextLevelMultipler�U�@�ӭ��ƪ���Ǫ��� ExtraPercentage�n���W����Ǧʤ��� MaxExtractPencentage�̤j���B�~�ʤ��� MaxMultiplier�̤j�i�઺���ƭ��ƭ�-->
//<ScroeMultiplier BalanceMoneyStartBase="1000" NextLevelMultipler="10" ExtraPercentageBase="0.05" MaxExtractPencentage="0.5" MaxMultiplier="2" >


struct	sScroeMultiplier
{
	int		iBalanceMoneyStartBase;
	int		iNextLevelMultipler;
	float	fExtraPercentageBase;
	float	fMaxExtractPencentage;
	float	fMaxMultiplier;
	sScroeMultiplier(TiXmlElement*e_pTiXmlElement);
	float	GetMultiplier(int e_iIncome,int e_iOut);

//<ScroeMultiplier BalanceMoneyStartBase="1000" NextLevelMultipler="10" ExtraPercentageBase="0.05" MaxExtractPencentage="0.5" MaxMultiplier="2" >
};

//<!-- ��Ĺ���ɭԪ��[��� -->
//<ConditionScoreData MoneyChange="100" Score="-100" Description="Ĺ�@�ʶ����ɭԦ��@�ʤ�">
//<ConditionScoreData MoneyChange="-100" Score="100" Description="��@�ʶ����ɭԥ[�@�ʤ�">

struct	sConditionScoreDataList
{
	//find out proper one and set score
	struct	sConditionScoreData
	{
		int	iMoney;
		int	iScore;
	};
	sConditionScoreDataList(TiXmlElement*e_pTiXmlElement);
	std::vector<sConditionScoreData>	ConditionScoreDataList;
	int	GetScore(int e_iCurrentBetBalance);
};


class	cSlotGoodTables;
class	cSlotProbability;
class	cSlotTemperatureScore
{
	friend class	cSlotGoodTables;
	friend class	cSlotProbability;
	int	m_iCurrentScore;
	sSlotTimeStamp				m_SlotTimeStamp;
	sScroeMultiplier*			m_pScroeMultiplier;
	sConditionScoreDataList*	m_pConditionScoreDataList;
public:
	cSlotTemperatureScore();
	~cSlotTemperatureScore();
	void	Update(cBetBehaviorResult*e_pBetBehaviorResult);
	void	Render();
};


#endif