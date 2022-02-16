#include "../stdafx.h"
#include "AnimationParser.h"
#include "../XML/AtgXmlWriter.h"
#include "MPDI.h"
#include "MPDIDataParse.h"
#include "SADataParse.h"


namespace FATMING_CORE
{
	const char*cAnimationParser::ExtensionNameID( ".mpdi" );
	const wchar_t*         cAnimationParser::TypeID( L"cAnimationParser" );
	cAnimationParser::cAnimationParser():ISAXCallback(true)
	{
		m_fMPDIVersion = 0.f;
		m_pCurrentImagePlayer = 0;
		m_pCurrentcMPDIList = 0;
		m_pCurrentMultiPathDynamicImage = 0;	
		m_pCurrentCueToStartCurvesWithTime = 0;
		m_eImagePlayerTypeList = eIPTL_MAX;
	}

	cAnimationParser::~cAnimationParser()
	{
		this->Destroy();
		m_AllBaseImageList.Destroy();
	}

	void	cAnimationParser::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const WCHAR*	l_strName = m_pCurrentTiXmlElement->Value();
		COMPARE_NAME("ImagePlayer")
		{
			m_eImagePlayerTypeList = eIPTL_COMMON;
		}
		else
		COMPARE_NAME("MPDIList")
		{
			m_eImagePlayerTypeList = eIPTL_MPDI;
		}
		else
		COMPARE_NAME("Version")
		{
			if( !m_pCurrentTiXmlElement->m_bDone )
			{
				const WCHAR*l_strVersion = m_pCurrentTiXmlElement->Attribute(L"MPDIVersion");
				if( l_strVersion )
					this->m_fMPDIVersion = (float)_wtof(l_strVersion);
			}
		}
		switch(m_eImagePlayerTypeList)
		{
			case eIPTL_COMMON:
				ProcessImagePlayer();
				break;
			case eIPTL_MPDI:
				ProcessMPDI();
				break;
		}
	}
	//kill imageplayer and image resource and puzzle data
	void	cAnimationParser::RemoveResourceObject(NamedTypedObject*e_pObject)
	{
		if(e_pObject->Type() == cMPDIList::TypeID)
		{
		
		}
		else
		if( e_pObject->Type() == cSAList::TypeID )
		{
		
		}
		else
		{
			assert(0&&"not cSAList and is not MPDIList");
			return;
		}
		DeleteBaseImage(e_pObject);
	}

	void	cAnimationParser::DeleteBaseImage(NamedTypedObject*e_pNamedTypedObject)
	{
		if( m_AllBaseImageList.Count() == 0 )
			return;
		cMulti_PI_Image*l_pMulti_PI_Image = dynamic_cast<cMulti_PI_Image*>(e_pNamedTypedObject);
		cObjectListByName<cPuzzleImage>*l_pPuzzleImageList = l_pMulti_PI_Image->GetPIList();
		//if use value is biger than one it mean the other people is using this puzzleimage still
		int	l_iPICount = l_pPuzzleImageList->Count();
		for( int i=0;i<l_iPICount;++i )
		{
			cPuzzleImage*l_pPI = l_pPuzzleImageList->GetObject(i);
			int	l_iNumMPDI = this->Count();
			bool	l_bInUse = false;
			for(int j=0;j<l_iNumMPDI;++j)
			{
				cMulti_PI_Image*l_pCompare = dynamic_cast<cMulti_PI_Image*>(this->m_ObjectList[j]);
				if( l_pCompare && l_pCompare != l_pMulti_PI_Image )
				{
					if( l_pCompare->IsContainPI(l_pPI) )
					{
						//j = l_iNumPI;
						l_bInUse = true;
						break;
					}
				}
			}
			if( !l_bInUse )
			{
				bool	l_b = this->m_AllBaseImageList.RemoveObject(l_pPI);
				if(!l_b)
				{
					m_strErrorMsg += e_pNamedTypedObject->GetName();
					m_strErrorMsg += L"not existed void	cAnimationParser::DeleteBaseImage(NamedTypedObject*e_pNamedTypedObject)\n";
				}				
			}
		}
	}

	cMPDIList*cAnimationParser::GetMPDIListByFileName(const char*e_strFileName)
	{
		std::string	l_strFileName = e_strFileName;
		return GetMPDIList(l_strFileName.c_str(),cMPDIList::FileToMPDIListName(e_strFileName));	
	}

	cMPDIList*cAnimationParser::GetMPDIListByFileName(const WCHAR*e_strFileName)
	{
		std::string	l_strFileName = UT::WcharToChar(e_strFileName);
		return GetMPDIList(l_strFileName.c_str(),cMPDIList::FileToMPDIListName(e_strFileName));
	}

	cMPDIList*cAnimationParser::GetMPDIList(const char*e_strFileName,const WCHAR*e_strName)
	{
		cMPDIList*l_pMPDIList = this->GetMPDIList(e_strName);
		if( !l_pMPDIList )
		{
			bool	l_b = this->Parse(e_strFileName);
			if( !l_b )
			{
				if(this->m_bShowErrorMsg)
					UT::ErrorMsg(e_strFileName,"parse failed");
				return 0;
			}
			l_pMPDIList = this->GetMPDIList(e_strName);
		}
		return l_pMPDIList;
	}

	int		cAnimationParser::NumObjectUsingPI(cPuzzleImage*e_pPI)
	{
		int	l_iNum = 0;
		int	l_iNumMPDI = this->Count();
		for(int j=0;j<l_iNumMPDI;++j)
		{
			cMulti_PI_Image*l_pCompare = dynamic_cast<cMulti_PI_Image*>(this->m_ObjectList[j]);
			if( l_pCompare->GetPuzzleImage(e_pPI->GetName()) )
			{
				++l_iNum;
			}
		}
		return l_iNum;
	}

	void	cAnimationParser::Export(char*e_strFileName)
	{
	#ifdef WIN32
		ATG::XMLWriter	l_XMLWriter(e_strFileName);
		//l_XMLWriter.StartElement("Version");
		//l_XMLWriter.AddAttribute("MPDIVersion","1.0");
		//l_XMLWriter.EndElement();
		for( int i=0;i<Count();++i )
		{
			NamedTypedObject* l_pNamedTypedObject = (*this)[i];
			const	WCHAR*l_strType = l_pNamedTypedObject->Type();
			const WCHAR*	l_strName = l_pNamedTypedObject->GetName();
			if( !wcscmp(l_strType,cSAList::TypeID) )
			{
				cSAList*l_pImagePlayer = dynamic_cast<cSAList*>(l_pNamedTypedObject);
				ExportSA(&l_XMLWriter,l_pImagePlayer);
			}
			else
			if( !wcscmp(l_strType,cMPDIList::TypeID) )
			{
				cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(l_pNamedTypedObject);
				ExportMPDI(&l_XMLWriter,l_pMPDIList);
			}
		}
		l_XMLWriter.Close();
	#endif
	}
}