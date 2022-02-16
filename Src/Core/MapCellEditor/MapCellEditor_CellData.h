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
	//it's orthongal projection!
	//==========================
	class	cMap_CellData:public cMapLayerList
	{
		//this one to store the real position in the 2D cooridinate
		Vector3	m_v2DPos;
		//this one and above are bind together,use this object please instead SetPos
		Vector3	m_vPos;
	protected:
		//for get hit data.
		Vector3	GetTransformedPos(bool e_bStripZ = true,Vector3*e_pvRight = 0,Vector3*e_pvUp = 0,Vector3*e_pvRightUpPos = 0,Vector3*e_pvLeftDownPos = 0);
		//
		Vector3	m_vHitGridPos;
		//
		//the width and height,so we do know this cell size,it's not filed's size,it only present each cell's size
		POINT	m_GridSize;//row,column
		Vector2	m_vSize;
		//if true the data's position is assign by grid position
		GET_SET_DEC(bool,m_bUsingGridToAssgnData,IsUsingGridToAssgnData,SetUsingGridToAssgnData);
		GET_SET_DEC(Vector3,m_vAngle,GetAngle,SetAngle);
		//if the object is selected
		GET_SET_DEC(cMap_Cell_ObjectData*,m_bSelectedObject,GetSelectedObject,SetSelectedObject);
	public:
		cMap_CellData();
		//this one should only be called in the editor!?
		cMap_CellData(cMap_CellData*e_pMap_CellData);
		~cMap_CellData();
		Vector3	GetPos(){ return m_vPos; }
		void	SetPos(Vector3 e_vPos);
		virtual	void	Render();
		//render all layer
		void	RenderAllObject();
		void	RenderLayer(int e_iLayIndex);
		//render specific grid.
		void	RenderGrid(int e_iIndex);
		void	RenderGrid(POINT e_Index);
		//===============================================
		//only CollideWithTransform support xyz angle rotation collided.
		//===============================================
		//if angle is rotated call this function.
		int		CollideWithTransform(Vector3 e_vPos,POINT*e_pXYIndex);
		//
		//if return value is not -1,it collided
		int		Collide(Vector2 e_vPos);
		int		CollideForSamePosition(Vector2 e_vPos);
		int		Collide(Vector3 e_vPos);
		//it might could be object move mode,so do not collide with selected one(m_bSelectedObject)
		int		CollideWithoutSelected(Vector2 e_vPos);
		int		CollideForSamePositionWithoutSelected(Vector2 e_vPos);
		//
		Vector3	GetFiledSize();
		//check index and position is inside
		bool	IsGridIndexLegal(POINT e_Index);
		bool	IsPositionLegal(Vector3 e_vPos);
		void	Set2DPos(Vector3 e_vPos);
		POINT	GetGridSize(){return m_GridSize;}
		Vector2	GetSize(){return m_vSize;}
		//aftern set grid data reset position.
		void	SetGridSize(POINT e_GridSize);
		void	SetSize(Vector2 e_vSize);
		//after call CollideWithTransform and it's collided,it will return a posiition where collided of grid
		Vector3	GetHitGridPos(){return m_vHitGridPos;}
		// e_bForceReturn will return result even grid is not legal
		POINT	GetHitIndex(Vector3 e_vPos);
		//get a transfomred 2D position
		Vector3	GetPosbyGridIndex(POINT e_Index);
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