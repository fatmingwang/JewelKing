#ifndef MAP_CELL_EDITOR_OBJECT_DATA_H
#define MAP_CELL_EDITOR_OBJECT_DATA_H
//===============
//position and image and is selected
//===============
	class	cMap_Cell_ObjectData:virtual	public NamedTypedObject
	{
	protected:
		Vector3	m_vPos;
		GET_SET_DEC(cPuzzleImageUnit*,m_pImage,GetImage,SetImage);
		bool	m_bSelected;
		GET_SET_DEC(bool,m_bContainUserDefineData,IsContainUserDefineData,SetContainUserDefineData);
		//GET_SET_DEC(bool,m_bRenderByCenter,IsRenderByCenter,SetRenderByCenter);
	public:
		cMap_Cell_ObjectData();
		cMap_Cell_ObjectData(cMap_Cell_ObjectData*e_pMap_Cell_ObjectData);
		~cMap_Cell_ObjectData();
		virtual	Vector3	GetPos();

		virtual	void	SetPos(Vector3 e_vPos);
		virtual	void	SetSelected(bool e_bSelected);
		virtual	void	Render();

		//if two data is same return true
		bool	DataCompare(cMap_Cell_ObjectData*e_pMap_Cell_ObjectData)
		{
			if(	e_pMap_Cell_ObjectData->GetImage() == m_pImage &&
				e_pMap_Cell_ObjectData->GetPos() == m_vPos &&
				e_pMap_Cell_ObjectData->IsSameName(GetName()) )
				return true;
			return false;
		}
	};

#endif