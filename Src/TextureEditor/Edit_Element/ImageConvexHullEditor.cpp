#include "StdAfx.h"
#include "ImageConvexHullEditor.h"

namespace FMCEditor
{

	void	ImageConvexHullEditor::OpenCollisionFile(String^e_strFileName)
	{
		if( e_strFileName )
		{
			c2DImageCollisionData	l_c2DImageCollisionData;
			l_c2DImageCollisionData.Parse(DNCT::GcStringToChar(e_strFileName));
			//SAFE_DELETE(m_p2DImageCollisionData);
			m_p2DImageCollisionData->ClearImageCollisionDataVector();
			SAFE_DELETE(m_pPuzzleImage);
			PuzzleImageUnit_listBox->Items->Clear();
			m_pPuzzleImageUnit = 0;
			if(m_p2DImageCollisionData->Parse(DNCT::GcStringToChar(e_strFileName)))
			{
				if( m_p2DImageCollisionData->m_ImageCollisionDataVector.size() )
				{
					String^l_strPIFIleName = System::IO::Path::GetDirectoryName(e_strFileName)+"/"+DNCT::WcharToGcstring(m_p2DImageCollisionData->m_strForCollidedpiFile)+".pi";
					OpenPIFile(DNCT::GcStringToChar(l_strPIFIleName),false);
				}
			}
		}	
	}
	//void	ImageConvexHullEditor::OpenPIFile(char*e_strFileName)
	//{
	//	cImageParser	l_ImageParser;
	//	l_ImageParser.SetFromResource(true);
	//	if(l_ImageParser.Parse(e_strFileName))
	//	{
	//		if( l_ImageParser.Count() )
	//		{
	//			SAFE_DELETE(m_pPuzzleImage);
	//			m_pCurveManager->Clear();
	//			PuzzleImageUnit_listBox->Items->Clear();
	//			m_pPuzzleImage = (cPuzzleImage*)l_ImageParser[0];
	//			for( int i=0;i<m_pPuzzleImage->Count();++i )
	//			{
	//				PuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring((*m_pPuzzleImage)[i]->GetName()));
	//				cCurveWithTime*l_pCurvesWithTime = new cCurveWithTime();
	//				l_pCurvesWithTime->SetName((*m_pPuzzleImage)[i]->GetName());
	//				m_pCurveManager->AddObject(l_pCurvesWithTime);
	//			}
	//		}
	//	}
	//}

}