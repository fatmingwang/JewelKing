#include "StdAfx.h"
#include "MapCellEditor_CellData.h"

const wchar_t*         cMapDataManager::TypeID( L"cMapDataManager" );

cMap_CellData::cMap_CellData()
{
	m_vAngle = Vector3::Zero;
	//m_fAngleX = 45.f;
	m_vPos.x = 0.f;
	m_vPos.y = 0.f;
	m_GridSize.x = 10;
	m_GridSize.y = 10;
	m_vSize.x = 100.f;
	m_vSize.y = 100.f;
	m_bUsingGridToAssgnData = true;
	m_bSelectedObject = 0;
}
//this one should only be called in the editor!?
cMap_CellData::cMap_CellData(cMap_CellData*e_pMap_CellData)
{
	m_vAngle = e_pMap_CellData->m_vAngle;
	m_bSelectedObject = 0;
	this->SetName(e_pMap_CellData->GetName());
	m_GridSize = e_pMap_CellData->GetGridSize();
	m_vSize = e_pMap_CellData->GetSize();
	m_bUsingGridToAssgnData = e_pMap_CellData->IsUsingGridToAssgnData();
	m_vPos = e_pMap_CellData->GetPos();
	int	l_iCount = e_pMap_CellData->Count();
	for( int i=0;i<l_iCount;++i )
	{
		cObjectListByNameWithData<cMap_Cell_ObjectData>*l_pOriginalData = e_pMap_CellData->GetObject(i);
		//cObjectListByNameWithData<cMap_Cell_ObjectData>*l_pCellData = new cObjectListByNameWithData<cMap_Cell_ObjectData>();
		//l_pCellData->SetName(l_pOriginalData->GetName());
		//for( int j=0;j<l_pOriginalData->Count();++j )
		//{
		//	cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData(l_pOriginalData->GetObject(j));
		//	l_pCellData->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
		//}
		//cObjectListByNameWithData<cMap_Cell_ObjectData>*l_pCellData = new cObjectListByNameWithData<cMap_Cell_ObjectData>(l_pOriginalData);
		cMapLayer*l_pCellData = NewTemplateList<cMapLayer,cMap_Cell_ObjectData>(l_pOriginalData,false);
		AddObjectNeglectExist(l_pCellData);
		
	}
}

cMap_CellData::~cMap_CellData()
{

}

void	cMap_CellData::Render()
{
	int	l_iRow = m_GridSize.x*2;
	int	l_iColumn = m_GridSize.y*2;
	float	l_fEndDown = this->m_vSize.y*m_GridSize.y;
	float	l_fEndRight = this->m_vSize.x*m_GridSize.x;
	float	l_fStartPosX = -l_fEndRight/2;
	float	l_fStartPosY = -l_fEndDown/2;
	if( l_iRow<=1||l_iColumn<=1 )
		return;
	int	l_iTotalCount = l_iRow+l_iColumn;
	Vector2*l_vAllVertices = new Vector2[l_iTotalCount];
	for( int i=0;i<l_iRow/2;++i )
	{
		l_vAllVertices[i*2] = Vector2(l_fStartPosX+i*m_vSize.x,l_fStartPosY);
		l_vAllVertices[i*2+1] = Vector2(l_fStartPosX+i*m_vSize.x,l_fEndDown+l_fStartPosY);
	}
	for( int j=0;j<l_iColumn/2;++j )
	{
		l_vAllVertices[l_iRow+j*2] = Vector2(l_fStartPosX,j*m_vSize.y+l_fStartPosY);
		l_vAllVertices[l_iRow+j*2+1] = Vector2(l_fEndRight+l_fStartPosX,j*m_vSize.y+l_fStartPosY);
	}
	cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(m_vPos.x+l_fEndRight/2,m_vPos.y+l_fEndDown/2,0.f))*Quaternion::EulerRotationQuaternion(D3DXToRadian(m_vAngle.x),D3DXToRadian(m_vAngle.y),D3DXToRadian(m_vAngle.z)).ToMatrix();
	GLRender::RenderLine((float*)&l_vAllVertices[0],l_iTotalCount,Vector4(0.f,0.5f,0.f,1.f),2,(float*)&l_mat);
	delete l_vAllVertices;
	Vector4	l_vColor(1,0.6f,1,1);
	RenderRectangle(l_fEndRight,l_fEndDown,l_mat,Vector4::One);
}


void	cMap_CellData::RenderGrid(POINT e_Index)
{
	RenderGrid(e_Index.x+e_Index.y*this->m_GridSize.x);
}

void	cMap_CellData::RenderGrid(int e_iIndex)
{
	if( e_iIndex == -1 )
		return;
	float	l_fEndDown = this->m_vSize.y*m_GridSize.y;
	float	l_fEndRight = this->m_vSize.x*m_GridSize.x;
	int	l_iIndexY = e_iIndex/this->m_GridSize.x;
	int	l_iIndexX = e_iIndex-(l_iIndexY*this->m_GridSize.x);
	//because rotation so minus half width and half height
	float	l_fStartPosX = -l_fEndRight/2+(l_iIndexX*this->m_vSize.x);
	float	l_fStartPosY = -l_fEndDown/2+(l_iIndexY*this->m_vSize.y);
	Vector2	l_fLinePoints[4] = {
	//leftup
		Vector2(l_fStartPosX,l_fStartPosY),
	//rightup
	Vector2(l_fStartPosX+this->m_vSize.x,l_fStartPosY),
	//leftdown
	Vector2(l_fStartPosX,l_fStartPosY+this->m_vSize.y),
	//rightdown
	Vector2(l_fStartPosX+this->m_vSize.x,l_fStartPosY+this->m_vSize.y)};
	Vector2	l_fFinallines[8] = {
		l_fLinePoints[0],l_fLinePoints[1],
		l_fLinePoints[2],l_fLinePoints[3],
		l_fLinePoints[0],l_fLinePoints[2],
		l_fLinePoints[1],l_fLinePoints[3]
	};
	cMatrix44	l_mat = cMatrix44::TranslationMatrix(Vector3(m_vPos.x+l_fEndRight/2,m_vPos.y+l_fEndDown/2,0.f))*
		Quaternion::EulerRotationQuaternion(D3DXToRadian(m_vAngle.x),D3DXToRadian(m_vAngle.y),D3DXToRadian(m_vAngle.z)).ToMatrix();
	GLRender::RenderLine((float*)l_fFinallines,8,Vector4(1.f,1.f,1.f,1.f),2,(float*)&l_mat);
}

void	cMap_CellData::RenderAllObject()
{
	int	l_iCount = Count();
	for(int i=0;i<l_iCount;++i)
	{
		cMapLayer*l_pLayer = (*this)[i];
		int	l_iCount2 = l_pLayer->Count();
		for(int j=0;j<l_iCount2;++j)
		{
			l_pLayer->GetObject(j)->Render();
		}
	}
}

void	cMap_CellData::RenderLayer(int e_iLayIndex)
{
	if( this->Count()<e_iLayIndex )
		return;
	cMapLayer*l_pLayer = (*this)[e_iLayIndex];
	int	l_iCount = l_pLayer->Count();
	for(int j=0;j<l_iCount;++j)
	{
		l_pLayer->GetObject(j)->Render();
	}
}

int		cMap_CellData::Collide(Vector2 e_vPos)
{
	if( !m_pCurrentList )
		return -1;
	int	l_iNum = m_pCurrentList->Count();
	for( int i=0;i<l_iNum;++i )
	{
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
		cBaseImage*l_pImage = l_pMap_Cell_ObjectData->GetImage();
		Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
		if( e_vPos.x>=l_vPos.x&&e_vPos.y>=l_vPos.y&&
			e_vPos.x<=(l_vPos.x+*l_pImage->GetImageShowWidth())&&
			e_vPos.y<=(l_vPos.y+*l_pImage->GetImageShowHeight()))
		{
			return i;
		}
	}
	return -1;
}

int		cMap_CellData::CollideForSamePosition(Vector2 e_vPos)
{
	if( !m_pCurrentList )
		return -1;
	int	l_iNum = m_pCurrentList->Count();
	for( int i=0;i<l_iNum;++i )
	{
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
		cBaseImage*l_pImage = l_pMap_Cell_ObjectData->GetImage();
		Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
		if( e_vPos.x == l_vPos.x&&e_vPos.y == l_vPos.y )
		{
			return i;
		}
	}
	return -1;
}

int		cMap_CellData::Collide(Vector3 e_vPos)
{
	if( !m_pCurrentList )
		return -1;
	int	l_iNum = m_pCurrentList->Count();
	for( int i=0;i<l_iNum;++i )
	{
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
		cBaseImage*l_pImage = l_pMap_Cell_ObjectData->GetImage();
		Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
		if( e_vPos.x>=l_vPos.x&&e_vPos.y>=l_vPos.y&&
			e_vPos.x<=(l_vPos.x+*l_pImage->GetImageShowWidth())&&
			e_vPos.y<=(l_vPos.y+*l_pImage->GetImageShowHeight())
			)
		{
			return i;
		}
	}
	return -1;
}

int		cMap_CellData::CollideWithoutSelected(Vector2 e_vPos)
{
	if( !m_pCurrentList )
		return -1;
	int	l_iNum = m_pCurrentList->Count();
	for( int i=0;i<l_iNum;++i )
	{
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
		if( l_pMap_Cell_ObjectData == m_bSelectedObject )
			continue;
		cBaseImage*l_pImage = l_pMap_Cell_ObjectData->GetImage();
		Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
		if( e_vPos.x>=l_vPos.x&&e_vPos.y>=l_vPos.y&&
			e_vPos.x<=(l_vPos.x+*l_pImage->GetImageShowWidth())&&
			e_vPos.y<=(l_vPos.y+*l_pImage->GetImageShowHeight())
			)
		{
			return i;
		}
	}
	return -1;
}

int		cMap_CellData::CollideForSamePositionWithoutSelected(Vector2 e_vPos)
{
	if( !m_pCurrentList )
		return -1;
	int	l_iNum = m_pCurrentList->Count();
	for( int i=0;i<l_iNum;++i )
	{
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = m_pCurrentList->GetObject(i);
		if( l_pMap_Cell_ObjectData == m_bSelectedObject )
			continue;
		cBaseImage*l_pImage = l_pMap_Cell_ObjectData->GetImage();
		Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
		if( e_vPos.x == l_vPos.x && e_vPos.y ==l_vPos.y )
		{
			return i;
		}
	}
	return -1;
}

Vector3	cMap_CellData::GetFiledSize()
{
	return Vector3(this->m_vSize.x*this->m_GridSize.x,this->m_vSize.y*this->m_GridSize.y,0.f);
}

bool	cMap_CellData::IsGridIndexLegal(POINT e_Index)
{
	if( m_GridSize.x>e_Index.x&&m_GridSize.y>e_Index.y )
	{
		//if( e_Index.x>-1&&e_Index>-1 )
			return true;
	}
	return false;
}

bool	cMap_CellData::IsPositionLegal(Vector3 e_vPos)
{
	POINT	l_HitIndex = this->GetHitIndex(e_vPos);
	if( l_HitIndex.x == -1 || l_HitIndex.y == -1)
	{
		return false;
	}
	return true;
}

void	cMap_CellData::SetGridSize(POINT e_GridSize)
{
	m_GridSize = e_GridSize;
	Set2DPos(m_v2DPos);
}

void	cMap_CellData::SetSize(Vector2 e_vSize)
{
	m_vSize = e_vSize;
	Set2DPos(m_v2DPos);
}

void	cMap_CellData::SetPos(Vector3 e_vPos)
{
	this->m_vPos = e_vPos;
	this->m_v2DPos = this->GetTransformedPos();
}

Vector3	cMap_CellData::GetPosbyGridIndex(POINT e_Index)
{
	Vector3	l_vNewRight,l_vNewUp,l_vNewRightUP,l_vNewLeftDown;
	Vector3 l_vNewLeftUp = GetTransformedPos(true,&l_vNewRight,&l_vNewUp,&l_vNewRightUP,&l_vNewLeftDown);
	Vector3	l_vPos = l_vNewLeftUp+((float)e_Index.x*l_vNewRight*m_vSize.x);
	float	l_fPosX = l_vPos.x;
	l_vPos += ((float)e_Index.y*l_vNewUp*m_vSize.y);
	return l_vPos;
}

void	cMap_CellData::Set2DPos(Vector3 e_vPos)
{
	//because its orthorgnal
	m_vPos = Vector3::Zero;
	this->m_v2DPos = e_vPos;
	Vector3	l_vPos1 = GetTransformedPos();
	m_vPos.x = e_vPos.x-l_vPos1.x;
	m_vPos.y = e_vPos.y-l_vPos1.y;
}

Vector3	cMap_CellData::GetTransformedPos(bool e_bStripZ,Vector3*e_pvRight,Vector3*e_pvUp,Vector3*e_pvRightUpPos,Vector3*e_pvLeftDownPos)
{
	//1st,find new start point
	//2nd,get new touch point
	//3rd collide new rectangle with touch point.
	Vector3	l_vSize(m_vSize.x*this->m_GridSize.x,m_vSize.y*this->m_GridSize.y,0.f);
	Vector3	l_vHalfSize = l_vSize/2.f;
	cMatrix44	l_mat = cMatrix44::TranslationMatrix(m_vPos)*
						Quaternion::EulerRotationQuaternion(D3DXToRadian(m_vAngle.x),D3DXToRadian(m_vAngle.y),D3DXToRadian(m_vAngle.z)).ToMatrix();
	//get new rect position
	Vector3 l_vNewLeftUp = l_mat.TransformCoordinate(-l_vHalfSize);
	l_vNewLeftUp += l_vHalfSize;
	if( e_pvRight )
	{
		Vector3	l_vNewRight = l_mat.GetAxis(MyMath::X);
		Vector3	l_vNewUp = l_mat.GetAxis(MyMath::Y);
		Vector3	l_vNewRightUP = l_vNewLeftUp+l_vNewRight*l_vSize.x;
		Vector3	l_vNewLeftDown = l_vNewLeftUp+(l_vNewUp*l_vSize.y);
		//Vector3	l_vNewRightDown = l_vNewRightUP+(l_vNewUp*l_vSize.y);
		*e_pvRight = l_vNewRight;
		*e_pvUp = l_vNewUp;
		*e_pvLeftDownPos = l_vNewLeftDown;
		*e_pvRightUpPos = l_vNewRightUP;
		if( e_bStripZ )
		{
			e_pvRightUpPos->z = e_pvLeftDownPos->z = 0.f;
		}
	}
	if( e_bStripZ )
		l_vNewLeftUp.z = 0.f;
	return l_vNewLeftUp;
}

POINT	cMap_CellData::GetHitIndex(Vector3 e_vPos)
{
	POINT	l_HitIndex = {-1,-1};
	Vector3	l_vNewRight;
	Vector3	l_vNewUp;
	Vector3	l_vNewRightUP;
	Vector3	l_vNewLeftDown;
	Vector3 l_vNewLeftUp = GetTransformedPos(true,&l_vNewRight,&l_vNewUp,&l_vNewRightUP,&l_vNewLeftDown);
	cLine	l_LineLU_To_RU(Vector3(l_vNewLeftUp.x,l_vNewLeftUp.y,0.f),Vector3(l_vNewRightUP.x,l_vNewRightUP.y,0.f));
	cLine	l_LineLU_To_LD(l_LineLU_To_RU.m_vBegine,Vector3(l_vNewLeftDown.x,l_vNewLeftDown.y,0.f));
	//just need a 2D coordinate.
	l_vNewUp.z = 0.f;
	l_vNewRight.z = 0.f;
	cLine	l_TouchToUPLine(e_vPos,e_vPos-(l_vNewUp*99999));
	cLine	l_TouchToLeftLine(e_vPos,e_vPos-(l_vNewRight*99999));
	Vector3	l_vUpIntersect;
	Vector3	l_vLeftIntersect;
	if(l_LineLU_To_RU.Intersect(l_TouchToUPLine,&l_vUpIntersect) != cLine::INTERESECTING )
		return l_HitIndex;
	if(l_LineLU_To_LD.Intersect(l_TouchToLeftLine,&l_vLeftIntersect) != cLine::INTERESECTING )
		return l_HitIndex;
	//just need a 2D coordinate.
	l_vLeftIntersect.z = l_vUpIntersect.z = 0.f;
	float	l_fXLength = (l_vNewLeftUp-l_vUpIntersect).Length();
	float	l_fYLength = (l_vNewLeftUp-l_vLeftIntersect).Length();
	//just need a 2D coordinate.
	l_vNewRightUP.z = l_vNewLeftUp.z = l_vNewLeftDown.z = 0.f;
	Vector3	l_vNewSize((l_vNewRightUP-l_vNewLeftUp).Length(),(l_vNewLeftDown-l_vNewLeftUp).Length(),0.f);
	Vector3	l_vNewHalfSize = l_vNewSize/2.f;
	Vector3	l_fNewEachGridSize(l_vNewSize.x/(float)this->m_GridSize.x,l_vNewSize.y/(float)this->m_GridSize.y,0.f);
	l_HitIndex.x = (int)(l_fXLength/l_fNewEachGridSize.x);
	l_HitIndex.y = (int)(l_fYLength/l_fNewEachGridSize.y);

	//m_vHitGridPos = l_LineLU_To_RU.m_vBegine+l_vNewRight*(l_fNewEachGridSize.x*l_HitIndex.x);
	//m_vHitGridPos += l_vNewUp*(l_fNewEachGridSize.y*l_HitIndex.y);
	m_vHitGridPos = l_LineLU_To_RU.m_vBegine+l_vNewRight*(this->m_vSize.x*l_HitIndex.x);
	m_vHitGridPos += l_vNewUp*(m_vSize.y*l_HitIndex.y);
	return l_HitIndex;
}

int		cMap_CellData::CollideWithTransform(Vector3 e_vPos,POINT*e_pXYIndex)
{
	POINT	l_HitIndex = GetHitIndex(e_vPos);
	if( l_HitIndex.x<0||l_HitIndex.y<0 )
		return  -1;
	if( l_HitIndex.x>this->m_GridSize.x||l_HitIndex.y>this->m_GridSize.y )
		return  -1;
	if( e_pXYIndex )
	{
		e_pXYIndex->x = l_HitIndex.x;
		e_pXYIndex->y = l_HitIndex.y;
	}
	l_HitIndex.y*=m_GridSize.x;
	return l_HitIndex.y+l_HitIndex.x;
}

cMapDataManager::cMapDataManager()
{
	m_pTemplateImage = new cMapLayer();
	m_pCurrentMap_CellData = 0;
	m_pCurrentMapLayer = 0;
	m_pLastParsedCell_ObjectData = 0;
	m_bIgnoreParseImageFile = false;
}

cMapDataManager::~cMapDataManager()
{
	SAFE_DELETE(m_pTemplateImage);
}

void	cMapDataManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = e_pTiXmlElement->Value();
	COMPARE_NAME("ImageList")
	{
		if( !m_bIgnoreParseImageFile )
			ProcessImageList();
	}
	else
	COMPARE_NAME("TemplateImage")//this one could be remove,because it only need while it's edit mode
	{
		ProcessCurrentTemplatet();
	}
	else
	COMPARE_NAME("StageData")
	{
		ProcesssStage();
	}
	else
	COMPARE_NAME("Data")
	{
		ProcesssData();
	}
	else
	COMPARE_NAME("StageDataLayer")
	{
		ProcesssStageLayer();
	}
}

void	cMapDataManager::ProcesssStage()
{
	cMap_CellData*l_pMap_CellData = new cMap_CellData;
	PARSE_NAME_VALUE_START
		COMPARE_NAME("Name")
		{
			l_pMap_CellData->SetName((l_strValue));
		}
		else
		COMPARE_NAME("LayerCount")
		{
			l_pMap_CellData->GetList()->reserve(_wtoi(l_strValue));
		}
		else
		COMPARE_NAME("Size")
		{
			Vector2	l_vSize = GetVector2(l_strValue);
			l_pMap_CellData->SetSize(l_vSize);
		}
		else
		COMPARE_NAME("GridSize")
		{
			l_pMap_CellData->SetGridSize(GetPoint(l_strValue));
		}
		else
		COMPARE_NAME("StartPos")
		{
			l_pMap_CellData->SetPos(GetVector3(l_strValue));
		}
	PARSE_NAME_VALUE_END
	assert(GetObject(l_pMap_CellData->GetName())==0);
	bool	l_b = this->AddObjectNeglectExist(l_pMap_CellData);
	m_pCurrentMap_CellData = l_pMap_CellData;
	assert(l_b);
}

//<ImageList PIFile="1,2," />
void	cMapDataManager::ProcessImageList()
{
	PARSE_NAME_VALUE_START
		assert( m_AllImageList.GetObject(l_strValue) == 0 &&"this pi existed!" );
	std::string l_strFileName = this->m_strCurrentDirectory;
	l_strFileName += UT::WcharToChar(l_strValue);
	bool	l_b = m_AllImageList.Parse((char*)l_strFileName.c_str());
		assert(l_b);
	TO_NEXT_VALUE
}
//<TemplateImage PIFile="" Name="Demon_1" Image="79979"  />
void	cMapDataManager::ProcessCurrentTemplatet()
{
	cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData();
	cPuzzleImage*l_pPI = 0;
	PARSE_NAME_VALUE_START
		COMPARE_NAME("PIFile")
		{
			if( !m_bIgnoreParseImageFile )
				l_pPI = dynamic_cast<cPuzzleImage*>(this->m_AllImageList.GetObject(l_strValue));
		}
		else
		COMPARE_NAME("Name")
		{
			l_pMap_Cell_ObjectData->SetName(l_strValue);
		}
		else
		COMPARE_NAME("Image")
		{
			if( !m_bIgnoreParseImageFile )
			{
				cPuzzleImageUnit*l_pPuzzleImageUnit = l_pPI->GetObject(l_strValue);
				assert(l_pPuzzleImageUnit);
				l_pMap_Cell_ObjectData->SetImage(l_pPuzzleImageUnit);
			}
		}
	PARSE_NAME_VALUE_END
	assert(!m_pTemplateImage->GetObject(l_pMap_Cell_ObjectData->GetName()));
	bool	l_b = m_pTemplateImage->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
	assert(l_b);
}
// <StageDataLayer Name="Layer1" Count="3" UserData="Test">
void	cMapDataManager::ProcesssStageLayer()
{
	cMapLayer*l_pMapLayer = new cMapLayer();
	PARSE_NAME_VALUE_START
		COMPARE_NAME("Name")
		{
			l_pMapLayer->SetName(l_strValue);
		}
		else
		COMPARE_NAME("Count")
		{
			l_pMapLayer->GetList()->reserve(VALUE_TO_INT);
		}
		else
		COMPARE_NAME("UserData")
		{
			std::wstring*l_pData = new std::wstring;
			*l_pData = l_strValue;
			l_pMapLayer->SetData(l_pData);
		}
	PARSE_NAME_VALUE_END
	assert(!m_pCurrentMap_CellData->GetObject(l_pMapLayer->GetName()));
	this->m_pCurrentMapLayer = l_pMapLayer;
	this->m_pCurrentMap_CellData->AddObjectNeglectExist(m_pCurrentMapLayer);
}

void	cMapDataManager::ProcesssData()
{
	//cMap_CellData*l_pCurrentMap_CellData = (*this)[Count()-1];
	//cMap_Cell_ObjectData*l_pTargetObject = m_ForListBoxTemplateCellData[0];
	//PARSE_NAME_VALUE_START
	//	char*l_strObjectName = strtok((char*)l_strValue,",");
	//	WCHAR	*l_strObjectNamew = UT::CharToWchar(l_strObjectName);
	//	if(wcscmp(l_strObjectNamew,l_pTargetObject->GetName()))
	//	{//not same object
	//		l_pTargetObject = m_ForListBoxTemplateCellData.GetObject(l_strObjectNamew);
	//	}
	//	cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData(l_pTargetObject);
	//	l_strObjectName = strtok(0,"\0");
	//	Vector2	l_vPos = GetVector2((char*)l_strObjectName);
	//	l_pMap_Cell_ObjectData->SetPos(l_vPos);
	//	l_pCurrentMap_CellData->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
	//TO_NEXT_VALUE

	cMap_Cell_ObjectData*l_pTargetObject = m_pLastParsedCell_ObjectData;
	cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = 0;
	PARSE_NAME_VALUE_START
		COMPARE_NAME("TemplateName")
		{
			WCHAR*l_strObjectNamew = (WCHAR*)(l_strValue);
			if(!l_pTargetObject||wcscmp(l_strObjectNamew,l_pTargetObject->GetName()))
				l_pTargetObject = m_pTemplateImage->GetObject(l_strObjectNamew);
			if( !l_pTargetObject )
				UT::ErrorMsg(L"Error not such object",l_strObjectNamew);
			l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData(l_pTargetObject);
		}
		else
		COMPARE_NAME("Pos")
		{
			l_pMap_Cell_ObjectData->SetPos(GetVector3(l_strValue));
		}
		else
		COMPARE_NAME("UserDefine")
		{
			//swprintf(l_pMap_Cell_ObjectData->m_strUniqueUserProperty,L"%s\0",l_strValue);
		}
	PARSE_NAME_VALUE_END
	m_pCurrentMapLayer->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
}


cMap_Cell_ObjectData*cMapDataManager::GetListChildByUniqueID(uint64 e_uiID)
{
	for( int i=0;i<this->Count();++i )
	{
		cMap_Cell_ObjectData*l_pData = (*this)[i]->GetListChildByUniqueID(e_uiID);
		if( l_pData )
			return l_pData;
	}
	return 0;
}

bool	cMapDataManager::Export(char*e_strFileName)
{
#ifdef WIN32
	if( !m_pTemplateImage->Count() )
		return false;
	ATG::XMLWriter     l_XMLWriter(e_strFileName);
	l_XMLWriter.StartElement("LevelEditorData");
	{
		l_XMLWriter.StartElement("ImageList");
		int	l_iNum = this->m_AllImageList.Count();
		if( l_iNum == 0 )
			return false;;
		//<ImageList PIFile1="1," PIFile2="1," />
		char	l_strImageAttributeName[MAX_PATH];
		for( int i=0;i<l_iNum;++i )
		{
			WCHAR*	l_strName = m_AllImageList[i]->GetName();
			std::wstring	l_strPIName = l_strName;
			l_strPIName += L".pi";

			sprintf(l_strImageAttributeName,"Name%d\0",i);
			l_XMLWriter.AddAttribute(l_strImageAttributeName,l_strPIName.c_str());
		}
		l_XMLWriter.EndElement();
		//<TemplateImage PIFile="" Name="Demon_1" Image="79979"  />
		l_iNum = m_pTemplateImage->Count();
		for( int i=0;i<l_iNum;++i )
		{
			l_XMLWriter.StartElement("TemplateImage");
			l_XMLWriter.AddAttribute("PIFile",dynamic_cast<cPuzzleImageUnit*>((*m_pTemplateImage)[i]->GetImage())->GetParent()->GetName());
			l_XMLWriter.AddAttribute("Name",(*m_pTemplateImage)[i]->GetName());
			l_XMLWriter.AddAttribute("Image",(*m_pTemplateImage)[i]->GetImage()->GetName());
			l_XMLWriter.EndElement();
		}
		l_iNum = this->Count();
		for( int i=0;i<l_iNum;++i )
		{
			cMap_CellData*l_pMap_CellData = (*this)[i];
			Vector2	l_vSize = l_pMap_CellData->GetSize();
			Vector3	l_vPos = l_pMap_CellData->GetPos();
			l_XMLWriter.StartElement("StageData");
 			l_XMLWriter.AddAttribute("Name",l_pMap_CellData->GetName());
			l_XMLWriter.AddAttribute("LayerCount",l_pMap_CellData->Count());
			l_XMLWriter.AddAttribute("StartPos",ValueToString(l_vPos));
			l_XMLWriter.AddAttribute("Size",ValueToString(l_vSize));
			l_XMLWriter.AddAttribute("GridSize",ValueToString(l_pMap_CellData->GetGridSize()));
			//if( l_pMap_CellData->IsUsingGridToAssgnData() )
			//{
			//	for( int j=0;j<l_pMap_CellData->GetGridSize().x;j++ )
			//	{
			//		for( int k=0;k<l_pMap_CellData->GetGridSize().y;k++ )
			//		{
			//			l_vGridSize.x;
			//			l_vGridSize.y;
			//		}
			//	}
			//}
			//else
			{
				for( int k=0;k<l_pMap_CellData->Count();k++ )
				{
					// <StageDataLayer Name="Layer1" Count="3" UserData="Test">
					cMapLayer*l_pMapLayer = l_pMap_CellData->GetObject(k);
					l_XMLWriter.StartElement("StageDataLayer");
					l_XMLWriter.AddAttribute("Name",l_pMapLayer->GetName());
					l_XMLWriter.AddAttribute("Count",l_pMapLayer->Count());
					std::wstring*l_pstrUserData = (std::wstring*)l_pMapLayer->GetData();
					if( l_pstrUserData )
					    l_XMLWriter.AddAttribute("UserData",l_pstrUserData->c_str());
					for( int j=0;j<l_pMapLayer->Count();j++ )
					{
						cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = l_pMapLayer->GetObject(j);
						l_XMLWriter.StartElement("Data");
						l_XMLWriter.AddAttribute("TemplateName",l_pMap_Cell_ObjectData->GetName());
						l_XMLWriter.AddAttribute("Pos",ValueToString(l_pMap_Cell_ObjectData->GetPos()));
						//if( wcslen(l_pMap_Cell_ObjectData->m_strUniqueUserProperty)>0 )
						{
							//l_XMLWriter.AddAttribute("UserDefine",l_pMap_Cell_ObjectData->m_strUniqueUserProperty);
						}
						//l_XMLWriter.AddAttribute(l_strName,l_strData);
						l_XMLWriter.EndElement();//end data
					}
					l_XMLWriter.EndElement();//end MapLayer
				}
			}
			l_XMLWriter.EndElement();//end StageData
		}
	}
	l_XMLWriter.EndElement();//end LevelEditorData
	l_XMLWriter.Close();
	return true;
#else
	assert(0&&"only win32 support");
	return false;
#endif
}