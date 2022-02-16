#ifndef _MAP_GRID_EDITOR_GRID_DATA_H_
#define _MAP_GRID_EDITOR_GRID_DATA_H_

	#include "MapCellEditor_ObjectData.h"
	//data is a std::wstring  to fetch xml data.
	typedef cObjectListByNameWithData<cMap_Cell_ObjectData>							cMapLayer;
	typedef cObjectListTree<cMapLayer,cMap_Cell_ObjectData>						cMapLayerList;
	//==========================
	//it's possible be a root
	//this is the cell that store in the MapData
	//a part of 2D data,just a cell.
	//!!!!!!!!!!!ensure u hvae set current list to set layer!!!!!!!!!!!
	//==========================
	class	cMap_CellData:public cMapLayerList
	{
		GET_SET_DEC(POINT,m_GridSize,GetGridSize,SetGridSize);//row,column
		//the width and height,so we do know this cell size,it's not filed's size,it only present each cell's size
		GET_SET_DEC(Vector2,m_vSize,GetSize,SetSize);
		//
		GET_SET_DEC(Vector3,m_vPos,GetPos,SetPos);
		//if true the data's position is assign by grid position
		GET_SET_DEC(bool,m_bUsingGridToAssgnData,IsUsingGridToAssgnData,SetUsingGridToAssgnData);
		GET_SET_DEC(float,m_fAngle,GetAngle,SetAngle);
		//if the object is selected
		GET_SET_DEC(cMap_Cell_ObjectData*,m_bSelectedObject,GetSelectedObject,SetSelectedObject);
	public:
		cMap_CellData();
		//this one should only be called in the editor!?
		cMap_CellData(cMap_CellData*e_pMap_CellData);
		~cMap_CellData();
		void	Render();
		void	RenderHitGrid(int e_iIndex);
		void	RenderAllObject();
		void	RenderLayer(int e_iLayIndex);
		//if return value is not -1,it collided
		int		Collide(Vector2 e_vPos);
		int		CollideForSamePosition(Vector2 e_vPos);
		int		Collide(Vector3 e_vPos);
		//if angle is rotated call this function.
		int		CollideWithAngle(Vector3 e_vPos,POINT*e_pXYIndex = 0);
		//it might could be object move mode,so do not collide with selected one(m_bSelectedObject)
		int		CollideWithoutSelected(Vector2 e_vPos);
		int		CollideForSamePositionWithoutSelected(Vector2 e_vPos);
		Vector3	GridIndexToPos(POINT e_GridIndex);
		Vector3	GetFiledSize();
		bool	IsGridIndexLegal(POINT e_Index);
	};
	//=======================
	//all map data store in here
	//cell could contain cell data,
	//the hirechy Object->Cell
	//======================
  //<ImageList PIFile1="1," PIFile2="1," />
  //<TemplateImage PIFile="" Name="Demon_1" Image="79979"  />
  //<TemplateImage PIFile="" Name="Demon_1" Image="79979"  />
  //<StageData Name="3" LayerCount="2" StartPos="0.00,0.00,-0.00," Size="840.00,600.00" GridSize="60,60">
	 // <StageDataLayer Name="Layer1" Count="3" UserData="Test">
		//<Data TemplateName="Env_Road" Pos="10.00,0.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="20.00,0.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="30.00,0.00,0.00," />
	 // <StageDataLayer/>
	 // <StageDataLayer Name="Layer2" Count="3" UserData="Test">
		//<Data TemplateName="Env_Road" Pos="10.00,10.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="20.00,20.00,0.00," />
		//<Data TemplateName="Env_Road" Pos="30.00,30.00,0.00," />
	 // <StageDataLayer/>
  //</StageData>
	class	cMapDataManager:public ISAXCallback,public cObjectListByName<cMap_CellData>
	{
		//
		cMap_CellData*	m_pCurrentMap_CellData;
		cMapLayer*		m_pCurrentMapLayer;
		//last data to quickly assign data
		cMap_Cell_ObjectData*m_pLastParsedCell_ObjectData;

		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		void	ProcessImageList();
		void	ProcessCurrentTemplatet();
		void	ProcesssStage();
		void	ProcesssStageLayer();
		void	ProcesssData();
	public:
		//in the game we might don't need image file,because we just need name to get correct animation
		bool	m_bIgnoreParseImageFile;
		//store all templateImage
		cMapLayer*		m_pTemplateImage;
		DEFINE_TYPE_INFO();
		cMapDataManager();
		~cMapDataManager();
		cImageParser	m_AllImageList;
		cMap_Cell_ObjectData*GetListChildByUniqueID(uint64 e_uiID);
		//virtual	bool	ParseDataIntoXMLNode(char*e_strFileName);
		bool	Export(char*e_strFileName);
	};

#endif