#ifndef LEVEL_EDITOR_PARSER_SAMPLE_H
#define LEVEL_EDITOR_PARSER_SAMPLE_H

	struct	sNameAndPos//for 
	{
		WCHAR	strName[MAX_PATH];
		Vector3	vPos;
		WCHAR	strUserData[MAX_PATH];
		const WCHAR*	strImageName;
		sNameAndPos(){memset(this,0,sizeof(sNameAndPos));strImageName = 0;}
		~sNameAndPos(){}
	};

	class	cStageData:public NamedTypedObject
	{
	public:
		std::vector<sNameAndPos>	m_AllNameAndPos;
		Vector3	m_vStartPosition;
		Vector2	m_vGridSize;//width = m_vResolution.x/m_vGridSize.x.
		Vector2	m_vResolution;
	};

	//class	cLevelEditorParserSample:public ISAXCallback,public cObjectListByName<cStageData*>
	//{
	//	virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
	//	//void	ProcessImageList();//fomr object name to get image name
	//	void	ProcessCurrentTemplatet();//get xml data by object name
	//	void	ProcesssStage();//for AllStageData
	//	void	ProcesssData();//for sNameAndPos
	//public:
	//	DEFINE_TYPE_INFO();
	//	cLevelEditorParserSample(){}
	//	~cLevelEditorParserSample(){}
	//	//to store image name and template name
	//	cObjectListByName<cDataAndName<std::wstring>*>	m_TemplateNameAndImageName;
	//};
//sample
	//cStageData	l_cStageData;
	//l_cStageData.Parser("FileName");
	//int	l_iNum = l_cStageData.Count();
	//for(int i=0;i<l_iNum;++i)
	//{
	//	cStageData*l_pStageData = l_cStageData[i];
	//	l_pStageData->m_AllNameAndPos;
	//}
#endif