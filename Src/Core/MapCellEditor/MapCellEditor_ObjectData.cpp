#include "StdAfx.h"
#include "MapCellEditor_ObjectData.h"

cMap_Cell_ObjectData::cMap_Cell_ObjectData()
{
	//m_bRenderByCenter = false;
	m_bContainUserDefineData = false;
	m_bSelected = false;
	m_pImage = 0;
}

cMap_Cell_ObjectData::cMap_Cell_ObjectData(cMap_Cell_ObjectData*e_pMap_Cell_ObjectData)
{
	//m_bRenderByCenter = e_pMap_Cell_ObjectData->m_bRenderByCenter;
	m_bContainUserDefineData = e_pMap_Cell_ObjectData->m_bContainUserDefineData;
	m_bSelected = false;
	m_pImage = e_pMap_Cell_ObjectData->GetImage();
	m_vPos = e_pMap_Cell_ObjectData->GetPos();
	SetName(e_pMap_Cell_ObjectData->GetName());
}

cMap_Cell_ObjectData::~cMap_Cell_ObjectData()
{
}
void	cMap_Cell_ObjectData::SetPos(Vector3 e_vPos)
{
	m_vPos = e_vPos;
}

Vector3	cMap_Cell_ObjectData::GetPos()
{
	return m_vPos;
}
void	cMap_Cell_ObjectData::SetSelected(bool e_bSelected)
{
	m_bSelected = e_bSelected;
	if( m_bSelected )
		m_pImage->SetColor(Vector4(0.5f,0.5f,0.5f,1));
	else
		m_pImage->SetColor(Vector4::One);
}

void	cMap_Cell_ObjectData::Render()
{
	if( m_pImage )
	{
		Vector3	l_vOriginalePos = *m_pImage->GetPos();
		//if( m_bRenderByCenter )
		//{
		//	Vector3	l_vPos = m_vPos;
		//	l_vPos.x -= m_pImage->GetPuzzleData()->OriginalSize.x/2.f;
		//	l_vPos.y -= m_pImage->GetPuzzleData()->OriginalSize.y/2.f;
		//	m_pImage->SetPos(l_vPos);
		//	m_pImage->Render();
		//}
		//else
			m_pImage->SetPos(m_vPos);
		m_pImage->Render();
		m_pImage->SetPos(l_vOriginalePos);
	}
	else
	{
		//render a rectangle here
		POINT	l_Pos = {(int)m_vPos.x,(int)m_vPos.y};
		GLRender::RenderRectangle(l_Pos,50,50,Vector4::One);
	}
}