/* ============================================================================================= *\
FILE NAME   : 
DESCRIPTION : 
CREATED BY  : 
HISTORY     : 
\* ============================================================================================= */
#include "stdafx.h"
#include "Bullet.h"

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cPlayerBullet::cPlayerBullet()
{

}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cPlayerBullet::~cPlayerBullet()
{
	DELETE_VECTOR(m_BulletDataList,sBulletData*);
	size_t	l_uiSize = m_AllBullet.size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		std::vector<cBullet*>*l_pList = m_AllBullet[i];
		size_t	l_uiSize2 = l_pList->size();
		for( size_t j=0;j<l_uiSize2;++j )
		{
			cBullet*l_pBullet = (*l_pList)[j];
			delete l_pBullet;
		}
		delete l_pList;
	}
	m_AllBullet.clear();
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cPlayerBullet::AddBulletData(sBulletData*e_pBulletData)
{
	m_BulletDataList.push_back(e_pBulletData);
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
cBullet*	cPlayerBullet::Require( int e_iLevel )
{
	assert( e_iLevel<=(int)m_AllBullet.size() );

	std::vector<cBullet*>* l_pBulletList = m_AllBullet[e_iLevel];

	size_t	l_uiSize = l_pBulletList->size();
	for( size_t i=0;i<l_uiSize;++i )
	{
		if((*l_pBulletList)[i]->m_eBulletStatus == eBS_IDLE)
		{
			return (*l_pBulletList)[i];
		} //end if
	} //next i
	return 0;
}

/* ============================================================================================= *\
FUNCTION NAME : 
MODIFY BY     : 
DESCRIPTION   : 
PARAMETERS    : 
RETURNS	      : 
\* ============================================================================================= */
void	cPlayerBullet::ProcessBullet(int e_iAmount)
{//fuck assign dynamicly.
	int	l_iSize = (int)m_BulletDataList.size();
	for( int j=0;j<l_iSize;++j )
	{
		sBulletData*l_pBulletData = m_BulletDataList[j];
		std::vector<cBullet*>*l_pLevelBulletList = new std::vector<cBullet*>;
		l_pLevelBulletList->reserve(e_iAmount);
		for( int i=0;i<e_iAmount;++i )
		{
			cBullet*l_pBullet = new cBullet( l_pBulletData->m_pBulletMPDI, l_pBulletData->m_pBulletCollision, l_pBulletData->m_fSpeed, l_pBulletData->m_pWebMPDI, l_pBulletData->m_pbtWebCollision );
			l_pLevelBulletList->push_back( l_pBullet );
		}
		m_AllBullet.push_back( l_pLevelBulletList );
	}
}

//void	cPlayerBullet::Init()
//{
//
//}

//void	cPlayerBullet::Update(float e_fElpaseTime)
//{
//
//}
//
//void	cPlayerBullet::Render()
//{
//
//}

//void	cPlayerBullet::Fire(cPlayer*e_pPlayer,Vector3 e_vStartPos,Vector3 e_vDirection,int e_iLevel)
//{
//
//}