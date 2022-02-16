#include "StdAfx.h"
#include "LevelEditorParserSample.h"

//TYPDE_DEFINE_MARCO(cLevelEditorParserSample)
//void	cLevelEditorParserSample::HandleElementData(TiXmlElement*e_pTiXmlElement)
//{
//	const WCHAR*l_strName = e_pTiXmlElement->Value();
////	if( !strcmp(l_strName,"ImageList") )
////	{
////		ProcessImageList();
////	}
//	//else
//	COMPARE_NAME("TemplateImage")//this one could be remove,because it only need while it's edit mode
//	{
//		ProcessCurrentTemplatet();
//	}
//	else
//	COMPARE_NAME("StageData")
//	{
//		ProcesssStage();
//	}
//	else
//	COMPARE_NAME("Data")
//	{
//		ProcesssData();
//	}
//}
//
////void	cLevelEditorParserSample::ProcessImageList()
////{
//	//PARSE_NAME_VALUE_START
//	//	std::wstring*l_pImageName = new std::wstring(UT::CharToWchar(l_strValue));
//	//	m_ImageNameList.push_back(l_pImageName);
//
//	//TO_NEXT_VALUE
////}
//
//void	cLevelEditorParserSample::ProcessCurrentTemplatet()
//{
//	PARSE_NAME_VALUE_START
//		WCHAR*l_strObjectName = wcstok((WCHAR*)l_strValue,L",");
//		WCHAR*l_strImageName = wcstok(0,L",");
//		cDataAndName<std::wstring>*l_pTemplateNameAndImageName = new cDataAndName<std::wstring>(l_strObjectName);
//		l_pTemplateNameAndImageName->m_Data = l_strImageName;
//		bool	l_b = m_TemplateNameAndImageName.AddObject(l_pTemplateNameAndImageName);
//		assert(l_b&&"same template name!!");
//	TO_NEXT_VALUE
//}
//
//void	cLevelEditorParserSample::ProcesssStage()
//{
//	cStageData*l_pStageData = new cStageData;
//	PARSE_NAME_VALUE_START
//		COMPARE_NAME("Name")
//		{
//			l_pStageData->SetName((l_strValue));
//		}
//		else
//		COMPARE_NAME("Count")
//		{
//			l_pStageData->m_AllNameAndPos.reserve(_wtoi(l_strValue));
//		}
//		else
//		COMPARE_NAME("Size")
//		{
//			l_pStageData->m_vResolution = GetVector2(l_strValue);
//		}
//		else
//		COMPARE_NAME("GridSize")
//		{
//			l_pStageData->m_vGridSize = GetVector2(l_strValue);
//		}
//		else
//		COMPARE_NAME("StartPos")
//		{
//			l_pStageData->m_vStartPosition = GetVector3(l_strValue);
//		}
//	PARSE_NAME_VALUE_END
//	bool	l_b = this->AddObject(l_pStageData);
//	assert(l_b);
//}
//
//void	cLevelEditorParserSample::ProcesssData()
//{
//	cStageData*l_pStageData = GetObject(this->Count()-1);//get last stage data
//	sNameAndPos	l_NameAndPos;
//	PARSE_NAME_VALUE_START
//		COMPARE_NAME("TemplateName")
//		{
//			swprintf(l_NameAndPos.strName,MAX_PATH,L"%ls\0",l_strValue);
//		}
//		else
//		COMPARE_NAME("Pos")
//		{
//			l_NameAndPos.vPos = GetVector3(l_strValue);
//		}
//		else
//		COMPARE_NAME("UserDefine")
//		{
//			swprintf(l_NameAndPos.strUserData,MAX_PATH,L"%ls\0",(l_strValue));
//		}
//	PARSE_NAME_VALUE_END
//	l_pStageData->m_AllNameAndPos.push_back(l_NameAndPos);
//}