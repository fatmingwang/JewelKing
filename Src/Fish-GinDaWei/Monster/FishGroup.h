/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#ifndef _FISH_GROUP_H_
#define _FISH_GROUP_H_

class	cMonsterManager;
/* ============================================================================================= *\
FUNCTION NAME :
MODIFY BY     :
DESCRIPTION   : 
PARAMETERS    :
RETURNS	      :
\* ============================================================================================= */
class	cFishGroup
{
	friend class	cMonsterManager;
	//
	void	FishGroupGo(int e_iIndex);
	void	FishGroupGo(int e_iIndex, cCurveWithTime e_TargetPath);
	void	f_PrizeII_Go( cCurveWithTime e_TargetPath );
	//
	class	cFishGroupPointData:public NamedTypedObject
	{
	public:
		struct	sFishPosList
		{
			int						iFishIndex;
			std::vector<Vector3>		vPosList;
		};
		std::vector<cCurveWithTime>	vCurveList;
		std::vector<sFishPosList>	m_FishGroupData;
		Vector2						m_vDrawSize;
	};
	cObjectListByName<cFishGroupPointData>	m_FishGroup;
	//
	cMPDI*						m_pMPDI_Winnings1;								//±mª÷ II
	//
public:
	cFishGroup( char* e_strFishGroupMPDI );
	cFishGroup( int e_nId, char* e_strFishGroupMPDI );
	~cFishGroup();
	void	FishGroupGoLeftToRight(int e_iIndex,float e_fTime);
	void	FishGroupGRightTooLeft(int e_iIndex,float e_fTime);
	void	f_PrizeII_Call( float e_fTime );
};

#endif