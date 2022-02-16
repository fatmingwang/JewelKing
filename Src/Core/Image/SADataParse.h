//===================
//only could be included for AnimationParser.cpp
//===================
namespace FATMING_CORE
{
	void	cAnimationParser::ProcessImagePlayer()
	{
		COMPARE_ELEMENT_VALUE("ImagePlayer")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				cSAList*l_p = new cSAList();
				m_pCurrentImagePlayer = l_p;
				PARSE_NAME_VALUE_START
					COMPARE_NAME("Name")
					{
						l_p->SetName(l_strValue);
						bool	l_b = this->AddObject(l_p);
						if( !l_b )
						{
							if(m_bShowErrorMsg)
								ErrorMsg(l_p->GetName(),L"existed already");
						}
					}
					else
					COMPARE_NAME("ImageName")
					{
						char*	l_strFileName = StringAddDirectory(UT::WcharToChar(l_strValue));
						cPuzzleImage*l_pPuzzleImage = 0;
						l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_AllBaseImageList.GetObject(UT::GetFileNameWithoutFullPath(UT::CharToWchar(l_strFileName))));
						if(!l_pPuzzleImage)
						{
							bool l_b = m_AllBaseImageList.Parse(l_strFileName);
							assert(l_b&&"parse puzzleimage failed");
							l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_AllBaseImageList.GetObject(UT::GetFileNameWithoutFullPath(UT::CharToWchar(l_strFileName))));
						}
	#ifdef _DEBUG
	#ifdef WIN32
						else
						{
							OutputDebugString(L"using same texture");
							OutputDebugString(l_pPuzzleImage->GetName());
							OutputDebugString(L"\n");
						}
	#endif
	#endif
						l_p->SetPuzzleImage(l_pPuzzleImage);
					}
					l_pAttribute = l_pAttribute->Next();
				}
			}
			else
			{
	#ifdef _DEBUG
				int	l_iNumAnimationCount = -1;
				m_pCurrentTiXmlElement->Attribute(L"AnimationCount",&l_iNumAnimationCount);
				assert(l_iNumAnimationCount == m_pCurrentImagePlayer->Count()&&"animation count is not match" );
	#endif
				m_pCurrentImagePlayer = 0;
			}
		}
		else
		COMPARE_ELEMENT_VALUE("Animation")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				cStaticAnimation*l_pImageShowInfoState = new cStaticAnimation();
				l_pImageShowInfoState->SetPuzzleData(m_pCurrentImagePlayer->GetPuzzleData());
				l_pImageShowInfoState->SetPuzzleImage(m_pCurrentImagePlayer->GetPuzzleImage());
				PARSE_NAME_VALUE_START
					COMPARE_NAME("Name")
					{
						l_pImageShowInfoState->SetName(l_strValue);
						m_pCurrentImagePlayer->AddObject(l_pImageShowInfoState);
					}
					else
					COMPARE_NAME("MaxImageWidth")
					{
						l_pImageShowInfoState->SetSingleImageWidth(_wtoi(l_strValue));
						l_pImageShowInfoState->SetImageShowWidth(l_pImageShowInfoState->GetSingleImageWidth());
					}
					else
					COMPARE_NAME("MaxImageHeight")
					{
						l_pImageShowInfoState->SetSingleImageHeight(_wtoi(l_strValue));
						l_pImageShowInfoState->SetImageShowHeight(l_pImageShowInfoState->GetSingleImageHeight());
					}
					else
					COMPARE_NAME("PlayCount")
					{
						l_pImageShowInfoState->SetPlayCount(_wtoi(l_strValue));
					}
					else
					COMPARE_NAME("Retain")
					{
						l_pImageShowInfoState->SetRetain(_wtoi(l_strValue)?true:false);
					}
					
					l_pAttribute = l_pAttribute->Next();
				}
			}
	#ifdef _DEBUG
			else
			{
				cStaticAnimation*l_pImageShowInfoState = 0;
				PARSE_NAME_VALUE_START
					COMPARE_NAME("AnimationCount")
					{
						assert(l_pImageShowInfoState->m_ImageShowInfoVector.size() == _wtoi(l_strValue)&&"animation count is not match");
					}
					COMPARE_NAME("Name")
					{					
						l_pImageShowInfoState = m_pCurrentImagePlayer->GetObject(l_strValue);
					}
					l_pAttribute = l_pAttribute->Next();
				}
			}
	#endif
		}
		else
		COMPARE_ELEMENT_VALUE("AnimationData")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				cStaticAnimation*l_pImageShowInfoState = m_pCurrentImagePlayer->GetObject(m_pCurrentImagePlayer->Count()-1);
				sImageShowInfo	l_sImageShowInfo;
				PARSE_NAME_VALUE_START
					COMPARE_NAME("Angle")
					{
						l_sImageShowInfo.fAngle = (float)_wtof(l_strValue);
					}
					else
					COMPARE_NAME("ShowSec")
					{
						l_sImageShowInfo.fShowSec = (float)_wtof(l_strValue);
					}
					else
					COMPARE_NAME("ImageName")
					{
						int	l_iIndex = l_pImageShowInfoState->GetPuzzleImage()->GetObjectIndexByName(l_strValue);
						assert(l_iIndex!=-1);
						l_sImageShowInfo.iIndex = l_iIndex;
					}
					else
					COMPARE_NAME("ImageIndex")
					{
						l_sImageShowInfo.iIndex = _wtoi(l_strValue);
					}
					else
					COMPARE_NAME("OffsetPos")
					{
						l_sImageShowInfo.OffsetPos = GetPoint(l_strValue);
					}
					else
					COMPARE_NAME("Size")
					{
						l_sImageShowInfo.Size = GetPoint(l_strValue);
					}
					else
					COMPARE_NAME("Mirror")
					{
						l_sImageShowInfo.bMirror = _wtoi(l_strValue)?true:false;
					}
				PARSE_NAME_VALUE_END
				l_pImageShowInfoState->AddImageShowInfo(l_sImageShowInfo);
			}
		}
	#ifdef _DEBUG
		else
		{
			const WCHAR*	l_str = m_pCurrentTiXmlElement->Value();
			if( l_str )
			{
				if(m_bShowErrorMsg)
					UT::ErrorMsg((char*)l_str,"such element doed not support");
			}

		}
	#endif
	}
	#ifdef WIN32
	void	ExportSA(ATG::XMLWriter*e_pXMLWriter,cSAList*e_pImagePlayer)
	{
		char	l_strTemp[TEMP_SIZE*50];	
		e_pXMLWriter->StartElement("ImagePlayer");
		WCHAR	l_strImagePlayerName[MAX_PATH];
		swprintf(l_strImagePlayerName,MAX_PATH,L"%s_sa",e_pImagePlayer->GetName());
		e_pXMLWriter->AddAttribute("Name",l_strImagePlayerName);
		e_pXMLWriter->AddAttribute("AnimationCount",e_pImagePlayer->Count());
		if( e_pImagePlayer->GetPuzzleImage() )
		{
			sprintf(l_strTemp,"%s.pi\0",UT::WcharToChar( e_pImagePlayer->GetPuzzleImage()->GetName() ));
			e_pXMLWriter->AddAttribute("ImageName",l_strTemp);
		}
		else
		{
			MessageBox(0,L"Warning!!",L"there is no ImageName",MB_OK);
			e_pXMLWriter->AddAttribute("ImageName","FUCK??????InlegalImage???FUCK");
		}
		for(int i=0;i<e_pImagePlayer->Count();++i)
		{
			cStaticAnimation*l_pImageShowInfoState = e_pImagePlayer->GetObject(i);
			e_pXMLWriter->StartElement("Animation");
			e_pXMLWriter->AddAttribute("Name",l_pImageShowInfoState->GetName());
			e_pXMLWriter->AddAttribute("PlayCount",l_pImageShowInfoState->GetPlayCount());
			e_pXMLWriter->AddAttribute("AnimationCount",(int)l_pImageShowInfoState->m_ImageShowInfoVector.size());
			POINT	l_MaxSize = {0,0};
			for( UINT k=0;k<l_pImageShowInfoState->m_ImageShowInfoVector.size();++k )
			{
				if( l_MaxSize.x<l_pImageShowInfoState->m_ImageShowInfoVector[k].Size.x )
					l_MaxSize.x = l_pImageShowInfoState->m_ImageShowInfoVector[k].Size.x;
				if( l_MaxSize.y<l_pImageShowInfoState->m_ImageShowInfoVector[k].Size.y )
					l_MaxSize.y = l_pImageShowInfoState->m_ImageShowInfoVector[k].Size.y;
			}
			e_pXMLWriter->AddAttribute("MaxImageWidth",l_MaxSize.x);
			e_pXMLWriter->AddAttribute("MaxImageHeight",l_MaxSize.y);
			e_pXMLWriter->AddAttribute("Retain",l_pImageShowInfoState->IsRetain());
			std::vector<sImageShowInfo>*l_pImageShowInfoList =  &l_pImageShowInfoState->m_ImageShowInfoVector;
			for( UINT j=0;j<l_pImageShowInfoList->size();++j )
			{
				sImageShowInfo*l_pImageShowInfo = &(*l_pImageShowInfoList)[j];
				e_pXMLWriter->StartElement("AnimationData");
				e_pXMLWriter->AddAttribute("Angle",l_pImageShowInfo->fAngle);
				e_pXMLWriter->AddAttribute("ShowSec",l_pImageShowInfo->fShowSec);
				e_pXMLWriter->AddAttribute("ImageName",e_pImagePlayer->GetPuzzleData()[l_pImageShowInfo->iIndex]->strFileName);
				e_pXMLWriter->AddAttribute("OffsetPos",ValueToString(l_pImageShowInfo->OffsetPos));
				e_pXMLWriter->AddAttribute("Size",ValueToString(l_pImageShowInfo->Size));
				e_pXMLWriter->AddAttribute("Mirror",l_pImageShowInfo->bMirror?1:0);
				e_pXMLWriter->EndElement();//end element AnimationData
			}
			e_pXMLWriter->EndElement();//end element Animation
		}
		e_pXMLWriter->EndElement();//end element ImagePlayer	
	}
	#endif
//end namespace FATMING_CORE
}