#include "stdafx.h"
#include "OldSlotObject.h"
#include "BetBehavior.h"
#include "GameApp.h"
cSlotObject::cSlotObject(cOldSlotObjectManager*e_pSlotLineObjectManager,cBaseImage*e_pBaseImage,Vector3 e_vPos,float e_fCurrentPos)
:cBaseImage(e_pBaseImage)
{
	m_pSlotLineObjectManager = e_pSlotLineObjectManager;
	m_fCurrentPos = e_fCurrentPos;
	m_vPos = e_vPos;
	m_pSpinSound = 0;
	//m_pSpinSound = cGameApp::m_spSoundParser->GetObject(SPIN_SOUND_NAME);
}

cSlotObject::~cSlotObject()
{

}

void	cSlotObject::Update(float e_fElpaseTime)
{
	RelativeMove(this->m_pSlotLineObjectManager->m_fCurrentSpeed*e_fElpaseTime);
}

void	cSlotObject::Render()
{
	cBaseImage::Render();
}

void	cSlotObject::DebugRender()
{

}

bool	cSlotObject::IsAtShowArea()
{
	Vector2	l_vPos1(m_pSlotLineObjectManager->m_vShowArea.x,m_fCurrentPos);
	Vector2	l_vPos2(m_pSlotLineObjectManager->m_vShowArea.x,m_fCurrentPos+*this->GetImageShowHeight());
	if(m_pSlotLineObjectManager->m_vShowArea.CollidePoint(l_vPos1.x,l_vPos1.y)||
		m_pSlotLineObjectManager->m_vShowArea.CollidePoint(l_vPos2.x,l_vPos2.y))
	{
		//this->m_vPos.y = m_fCurrentPos;
		return true;
	}
	return false;
}

void	cSlotObject::RelativeMove(float e_fDis)
{
	float	l_fNewPos = m_fCurrentPos+e_fDis;

	float	l_fRlativePosToEnd = l_fNewPos-this->m_pSlotLineObjectManager->m_fAllSpinObjectLength;
	if( l_fRlativePosToEnd>0.f )
	{
		SetCurrentPos(l_fRlativePosToEnd);
		//if( m_pSpinSound )
		//	m_pSpinSound->Play(true);
	}
	else
	{
		SetCurrentPos(l_fNewPos);
	}
}

void	cSlotObject::SetCurrentPos(float e_fPos)
{
	m_fCurrentPos = e_fPos;
	this->m_vPos.y = m_pSlotLineObjectManager->m_vShowArea.y+m_fCurrentPos-m_pSlotLineObjectManager->m_fSpinObjectIndex_0_OffsetStartPos;
}

void	cSlotObject::ChangeImage(cBaseImage*e_pBaseImage)
{
	this->SetName(e_pBaseImage->GetName());
	SetUV(e_pBaseImage->GetUV());
}

void	cSlotObject::GetUVAndVertexData(float*e_pfVertexData,float*e_pfUVData)
{
	//if( !this->IsAtShowArea() )
	//{
	//	return;
	//}
	//Vector3	l_Vertices[4];
	//Vector2	l_UV[4];

	//l_Vertices[0] = Vector3(this->m_pSlotLineObjectManager->m_vPos.x,this->m_fCurrentPos,0.f);
	//l_Vertices[1] = Vector3(this->m_pSlotLineObjectManager->m_vPos.x+*this->GetImageShowWidth(),this->m_fCurrentPos,0.f);
	//l_Vertices[2] = Vector3(this->m_pSlotLineObjectManager->m_vPos.x,this->m_fCurrentPos+*this->GetImageShowHeight(),0.f);
	//l_Vertices[3] = Vector3(this->m_pSlotLineObjectManager->m_vPos.x+*this->GetImageShowWidth(),this->m_fCurrentPos+*this->GetImageShowHeight(),0.f);
	////left down
	//e_pfVertexData[0] = l_Vertices[2].x;
	//e_pfVertexData[1] = l_Vertices[2].y;
	//e_pfVertexData[2] = l_Vertices[2].z;
	////right down
	//e_pfVertexData[3] = l_Vertices[3].x;
	//e_pfVertexData[4] = l_Vertices[3].y;
	//e_pfVertexData[5] = l_Vertices[3].z;
	////left upper
	//e_pfVertexData[6] = l_Vertices[0].x;
	//e_pfVertexData[7] = l_Vertices[0].y;
	//e_pfVertexData[8] = l_Vertices[0].z;
	////left upper
	//e_pfVertexData[9] = l_Vertices[0].x;
	//e_pfVertexData[10] = l_Vertices[0].y;
	//e_pfVertexData[11] = l_Vertices[0].z;
	////right down
	//e_pfVertexData[12] = l_Vertices[3].x;
	//e_pfVertexData[13] = l_Vertices[3].y;
	//e_pfVertexData[14] = l_Vertices[3].z;
	////right upper	
}

cBaseImage*g_pTestImage = 0;
int	cOldSlotObjectManager::m_siResultSpinObjectIndex = 1;
cOldSlotObjectManager::cOldSlotObjectManager(Vector4 e_vShowArea,float	e_fSpinTime,float	e_fLeastSpinDis)
{
	m_pAllSpinObjectPI = 0;
	if( !g_pTestImage )
		g_pTestImage = new cBaseImage("JewelsKing/Test.png");
	m_fLeastSpinDis = e_fLeastSpinDis;
	m_fSpinObjectIndex_0_OffsetStartPos = 0.f;
	m_vShowArea = e_vShowArea;
	m_SpinTime.SetTargetTime(e_fSpinTime);
	m_fTargetSpinDis = 0.f;
	memset(m_ShowObjectIndex,-1,sizeof(int)*MAX_VISBILE_OBJECT);
	m_iResultIndexToStopAtTopShow = -1;
}

cOldSlotObjectManager::~cOldSlotObjectManager()
{
	SAFE_DELETE(g_pTestImage);
}

void	cOldSlotObjectManager::SetupData(SlotLineData e_SlotLineData,cPuzzleImage*e_pPI)
{
	m_pAllSpinObjectPI = e_pPI;
	//========================================================
	this->Destroy();
	int	l_iCount = (int)e_SlotLineData.size();
	if( l_iCount )
	{
		float	l_fImageHeight = (float)*e_pPI->GetObject(0)->GetImageShowHeight();
		m_fSpinObjectIndex_0_OffsetStartPos = l_fImageHeight;
		m_fAllSpinObjectLength = (l_iCount)*l_fImageHeight;
		Vector3	l_vPos(m_vShowArea.x,m_vShowArea.y,0.f);
		for( int i=0;i<l_iCount;++i )
		{
			l_vPos.y = m_vShowArea.y+(l_fImageHeight*i)-m_fSpinObjectIndex_0_OffsetStartPos;
			const WCHAR*l_strPIUnitName = e_SlotLineData[i].c_str();
			cSlotObject*l_p = new cSlotObject(this,e_pPI->GetObject(l_strPIUnitName),l_vPos,l_vPos.y);
			this->AddObjectNeglectExist(l_p);
		}
	}
}

void	cOldSlotObjectManager::StartSpinData(std::vector<std::wstring>e_strResult)
{
	int	l_iSize = (int)e_strResult.size();
	int	l_iCount = this->Count();
	if(l_iCount == 0 )
		return;
	const int	l_iOffsetIndex = 2;
	assert(l_iCount-l_iSize>=l_iSize&&"slot object's count is too small");
	for( int i=0;i<l_iSize;++i )
	{
		const WCHAR*l_strNmae = e_strResult[i].c_str();
		int	l_iTargetIndex = UT::GetLoopIndex(this->m_iResultIndexToStopAtTopShow-i-l_iOffsetIndex,l_iCount);
		cSlotObject*l_pOriginalSlotObject = this->GetObject(l_iTargetIndex);
		if( wcscmp(this->GetObject(l_iTargetIndex)->GetName(),e_strResult[i].c_str()) )
		{
			Vector3 l_vPos = *l_pOriginalSlotObject->GetPos();
			float	l_fCurrentPos = l_pOriginalSlotObject->GetCurrentPos();
			cSlotObject*l_pSlotObject = new cSlotObject(this,m_pAllSpinObjectPI->GetObject(l_strNmae),l_vPos,l_fCurrentPos);
			this->m_ObjectList[l_iTargetIndex] = l_pSlotObject;
			delete l_pOriginalSlotObject;
		}
	}
	int	l_iResultIndexToStopAtTopShow = UT::GetLoopIndex(this->m_iResultIndexToStopAtTopShow-l_iSize-1,l_iCount);
	StartSpinData(l_iResultIndexToStopAtTopShow);
}

void	cOldSlotObjectManager::StartSpinData(int e_iStopWorkingIndex)
{
	int	l_iCount = this->Count();
	if(l_iCount == 0 )
		return;
	float	l_fOneImageHeight = (float)*this->GetObject(0)->GetImageShowHeight();
	float	l_fResultPos = m_siResultSpinObjectIndex*l_fOneImageHeight;
	float	l_fCurrentResultPos = GetObject(e_iStopWorkingIndex)->GetCurrentPos();
	float	l_fNormalSpinPos = l_fCurrentResultPos+this->m_fLeastSpinDis;
	l_fNormalSpinPos = UT::GetFloatModulus(l_fNormalSpinPos,m_fAllSpinObjectLength);
	float	l_fOffset = l_fResultPos-l_fNormalSpinPos;
	if( l_fOffset<0.f )
		this->m_fTargetSpinDis = m_fLeastSpinDis+(m_fAllSpinObjectLength-l_fNormalSpinPos+l_fResultPos);
	else
	{
		this->m_fTargetSpinDis = m_fLeastSpinDis+(l_fResultPos-l_fNormalSpinPos);
	}
	m_iResultIndexToStopAtTopShow = e_iStopWorkingIndex;
	this->m_fCurrentSpeed = this->m_fTargetSpinDis/this->m_SpinTime.fTargetTime;
	m_SpinTime.Start();
}

void	cOldSlotObjectManager::AdjustOffsetPosition()
{
	int		l_iCount = this->Count();
	if( l_iCount <1 )
		return;
	int	l_iWorkingCount = 0;
	float	l_fImageHeight = (float)*GetObject(0)->GetImageShowHeight();
	int	l_iStartIndex = UT::GetLoopIndex(m_iResultIndexToStopAtTopShow-1,l_iCount);
	for( int i=0;i<l_iCount;++i )
	{
		int	l_iTargetIndex = UT::GetLoopIndex(l_iStartIndex,l_iCount);
		this->m_ObjectList[l_iTargetIndex]->SetCurrentPos(l_fImageHeight*i);
		if(this->m_ObjectList[l_iTargetIndex]->IsAtShowArea())
		{
			m_ShowObjectIndex[l_iWorkingCount] = l_iTargetIndex;
			++l_iWorkingCount;
		}
		++l_iStartIndex;
	}
}

void	cOldSlotObjectManager::UpdateSpinVisibleResult(float e_fElpaseTime)
{
	memset(m_ShowObjectIndex,-1,sizeof(int)*MAX_VISBILE_OBJECT);
	int	l_iWorkingCount = 0;
	int	l_iCount = this->Count();
	for( int i=0;i<l_iCount;++i )
	{
		this->m_ObjectList[i]->Update(e_fElpaseTime);
		if(this->m_ObjectList[i]->IsAtShowArea())
		{
			m_ShowObjectIndex[l_iWorkingCount] = i;
			++l_iWorkingCount;
		}
	}
}

void	cOldSlotObjectManager::SpinToWorkingIndex()
{
	
}

void	cOldSlotObjectManager::Update(float e_fElpaseTime)
{
	m_SpinTime.Update(e_fElpaseTime);
	if(!m_SpinTime.bTragetTimrReached)
	{
		UpdateSpinVisibleResult(e_fElpaseTime);
	}
	else
	{//adjust position to correct to minus position error
		AdjustOffsetPosition();
		//UpdateSpinVisibleResult(e_fElpaseTime);
	}
}

void	cOldSlotObjectManager::Render()
{
	//int	l_iCount = this->Count();
	//for( int i=0;i<l_iCount;++i )
	//{
	//	this->m_ObjectList[i]->Render();
	//	if( i == this->m_iResultIndexToStopAtTopShow )
	//	{
	//		g_pTestImage->SetColor(Vector4(1,1,1,0.2f));
	//		g_pTestImage->SetPos(*this->m_ObjectList[i]->GetPos());
	//		g_pTestImage->Render();
	//	}
	//}
	//return ;
	int	l_iWorkingCount = 0;
	while( m_ShowObjectIndex[l_iWorkingCount] != -1 )
	{
		if( m_ShowObjectIndex[l_iWorkingCount] == this->m_iResultIndexToStopAtTopShow )
		{
			this->m_ObjectList[m_ShowObjectIndex[l_iWorkingCount]]->Render();
			g_pTestImage->SetColor(Vector4(1,1,1,0.2f));
			g_pTestImage->SetPos(*this->m_ObjectList[m_ShowObjectIndex[l_iWorkingCount]]->GetPos());
			g_pTestImage->Render();
		}
		else
			this->m_ObjectList[m_ShowObjectIndex[l_iWorkingCount]]->Render();
		++l_iWorkingCount;
	}
}

void	cOldSlotObjectManager::DebugRender()
{

}

bool	cOldSlotObjectManager::IsSpinedDone()
{
	return m_SpinTime.bTragetTimrReached;
}

float	cOldSlotObjectManager::GetRestTime()
{
	return m_SpinTime.fRestTime;
}

void	cOldSlotObjectManager::ReplaceObject(int e_iIndex,WCHAR*e_strNewObjectName)
{
	cSlotObject*l_pOriginalSlotObject = this->GetObject(e_iIndex);
	if( wcscmp(this->GetObject(e_iIndex)->GetName(),e_strNewObjectName) )
	{
		Vector3 l_vPos = *l_pOriginalSlotObject->GetPos();
		float	l_fCurrentPos = l_pOriginalSlotObject->GetCurrentPos();
		cSlotObject*l_pSlotObject = new cSlotObject(this,m_pAllSpinObjectPI->GetObject(e_strNewObjectName),l_vPos,l_fCurrentPos);
		this->m_ObjectList[e_iIndex] = l_pSlotObject;
		delete l_pOriginalSlotObject;
	}
}


std::wstring	GetSpinObjectNameByIndex(cBehaviorObjectList<cOldSlotObjectManager>*e_pData,int e_iIndex)
{
	std::wstring	l_str;
	int	l_iCount = e_pData->Count();
#ifdef _DEBUG
	assert(e_iIndex>=0);
	int	l_iAllObjectSize = 0;
	for( int i=0;i<l_iCount;++i )
	{
		l_iAllObjectSize += e_pData->GetObject(i)->Count();
	}
	assert(e_iIndex<l_iAllObjectSize);
#endif
	int	l_iTargetIndex = 0;
	int	l_iRowIndex = 0;
	while( 1 )
	{
		for( int i=0;i<l_iCount;++i )
		{
			if( l_iTargetIndex == e_iIndex )
			{
				cOldSlotObjectManager*l_pSlotObjectManager = e_pData->GetObject(i);
				int	l_iFinalIndex = UT::GetLoopIndex(l_pSlotObjectManager->GetStopAtTopShowResultIndex()+l_iRowIndex,l_pSlotObjectManager->Count());
				l_str = l_pSlotObjectManager->GetObject(l_iFinalIndex)->GetName();
				return l_str;
			}

			++l_iTargetIndex;
		}
		++l_iRowIndex;
	}
	return l_str;
}


int		GetSpinObjectManagerChildIndexByLineRuleIndex(cOldSlotObjectManager*e_pSlotObjectManager,int e_iLineRuleIndex)
{
	//cSlotRuleData*l_pSlotRuleData = cEngineTestApp::m_spSlotRuleData;
	//int	l_iNumLines = (int)l_pSlotRuleData->m_LineRuleList.size();
	//int	l_iTargetSpinObjectMnnagerIndex = e_iLineRuleIndex&l_iNumLines;
	//cOldSlotObjectManager*l_pSlotObjectManager = cEngineTestApp::m_spSlotLineObjectManagerList->GetObject(l_iTargetSpinObjectMnnagerIndex);
	//if( e_pSlotObjectManager != l_pSlotObjectManager )
	//{
	//	return -1;
	//}
	//int	l_iRowIndex = e_iLineRuleIndex/l_iNumLines;
	//return UT::GetLoopIndex(l_pSlotObjectManager->GetStopAtTopShowResultIndex()+l_iRowIndex,l_pSlotObjectManager->Count());
	return -1;
}