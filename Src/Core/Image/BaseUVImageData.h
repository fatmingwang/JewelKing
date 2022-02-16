#ifndef BASE_UV_IMAGE_DATA
#define BASE_UV_IMAGE_DATA

namespace FATMING_CORE
{
	//==================
	//one pi is not enough,so we need more texture but the texture size has  restriction...
	//although it might slow(?) but more images could be edited for MPDI the animation would be better
	//default we assume only one PI is exists,so the data(cBaseUVImageData::m_pPuzzleImage) from ascent
	//is the first data we have added!make sure the function has called is the way u want.
	//because all PI data are from external but all of it will store at cObjectListByName<cBaseUVImageData>
	//if u want to change current m_pPuzzleImage make sure the one is what u want!.
	//==================
	class	cMulti_PI_Image
	{
	protected:
		cPuzzleImage* m_pPuzzleImage;
		bool	m_bFromResource;
		bool	AddObjectIfNotExist(cPuzzleImage*e_pPuzzleImage)
		{
			m_pPuzzleImage = e_pPuzzleImage;
			return m_PIList.AddObject(e_pPuzzleImage);
		}
		//all object only need one this,so it's a pointer only one instance for same MPDIList's children.
		//only cMPDIList has its instance!all cMPDIList's chilren(cCueToStartCurveWithTime,cMultiPathDynamicImage)
		//get from cMPDIList!
		GETP_SET_DEC(cObjectListByName<cPuzzleImage>,m_PIList,GetPIList,SetPIList);
	public:
		cMulti_PI_Image(){ m_PIList.SetFromResource(true); m_pPuzzleImage = 0; }
		cMulti_PI_Image(cMulti_PI_Image*e_pMulti_PI_Image)
		{
			assert(e_pMulti_PI_Image);
			this->m_pPuzzleImage = e_pMulti_PI_Image->m_pPuzzleImage;
			m_PIList.CopyListPointer(&e_pMulti_PI_Image->m_PIList);
		}
		~cMulti_PI_Image(){ }
		//make sure this one only call by original cMulti_PI_Image::m_pBaseUVImageDataList
		virtual	void	SetPuzzleImage(cPuzzleImage*e_pPuzzleImage)
		{
			AddObjectIfNotExist(e_pPuzzleImage);
		}
		virtual	void	Merge(cMulti_PI_Image*e_pMulti_PI_Image)
		{
			if( e_pMulti_PI_Image&& this == e_pMulti_PI_Image )
				return;
			m_PIList.AddListToMe(&e_pMulti_PI_Image->m_PIList);
		}
		int				GetNumImage(){return m_pPuzzleImage->GetNumImage();}
		sPuzzleData**	GetPuzzleData(){return m_pPuzzleImage?m_pPuzzleImage->GetPuzzleData():0;}
		cPuzzleImage*	GetPuzzleImage(){return m_pPuzzleImage;}
		cPuzzleImage*	GetPuzzleImage(const WCHAR*e_strPIName){ return m_PIList.GetObject(e_strPIName); }
		cPuzzleImage*	GetPuzzleImage(int e_iIndex){ return m_PIList.GetObject(e_iIndex); }
		inline  void    RemovePIReference(cPuzzleImage*e_pPI)
		{
		    int l_iIndex = m_PIList.GetObjectIndexByName(e_pPI->GetName());
		    if( l_iIndex != -1 )
		    {
		        if( this->m_pPuzzleImage == e_pPI )
		        {
		            m_pPuzzleImage = 0;
		        }
				m_PIList.RemoveObjectWithoutDelete(l_iIndex);;
		    }
		}
		bool	IsContainPI(cPuzzleImage*e_pPI)
		{
			if( m_PIList.GetObject(e_pPI->GetName()) )
				return true;
			return false;			
		}
	};
}//end namespace

#endif