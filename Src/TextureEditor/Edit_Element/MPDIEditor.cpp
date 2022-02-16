#include "StdAfx.h"
#include "MPDIEditor.h"


namespace FMCEditor
{
	String^CueToStartCurveWithTimeDataCheck(cCueToStartCurveWithTime*e_pCueToStartCurveWithTime)
	{
		String^l_strErrorMsg;
		cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = e_pCueToStartCurveWithTime;
		for( UINT i=0;i<l_pCueToStartCurvesWithTime->GetOriginalPointList().size();++i )
		{
			if( (*l_pCueToStartCurvesWithTime->GetPointDataList())[i]->iImageIndex == -1 )
			{
				l_strErrorMsg += DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName())+" image index is empty!!"+DNCT::GetChanglineString();
			}
			if( i == 0 )
			{
				float   l_fTime = l_pCueToStartCurvesWithTime->GetOriginalTimeList()[0];
				//if( l_fTime > 0.f)
				{
				//	l_strErrorMsg += DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName())+" first point time must be 0!!"+DNCT::GetChanglineString();
				}
			}
			else
			{
				if( (l_pCueToStartCurvesWithTime->GetOriginalTimeList()[i] - l_pCueToStartCurvesWithTime->GetOriginalTimeList()[i-1]) <= 0.f )
				{
					l_strErrorMsg += String(l_pCueToStartCurvesWithTime->GetCharName()).ToString()+" index"+i.ToString()+"is error,later index point time must bigger than early one!"+DNCT::GetChanglineString();
				}
			}
			if( i == l_pCueToStartCurvesWithTime->GetOriginalPointList().size()-1 )
			{
				float   l_fTime = l_pCueToStartCurvesWithTime->GetOriginalTimeList()[i];
				if( l_fTime <= 0.f)
				{
					l_strErrorMsg += DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName())+" last point time can not be 0!!"+DNCT::GetChanglineString();
				}
			}
		}
		return l_strErrorMsg;
	}

	String^MultiPathDynamicImageDataCheck(cMultiPathDynamicImage*e_pMultiPathDynamicImage)
	{
		String^l_strErrorMsg;
		for(int j=0;j<e_pMultiPathDynamicImage->Count();++j)
		{
			l_strErrorMsg += CueToStartCurveWithTimeDataCheck((*e_pMultiPathDynamicImage)[j]);
		}
		return l_strErrorMsg;
	}

	String^MPDIListDataCheck(cMPDIList*e_pMPDIList)
	{
		String^l_strErrorMsg;
		for(int i=0;i<e_pMPDIList->Count();++i)
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = (cMultiPathDynamicImage*)(*e_pMPDIList)[i];
			l_strErrorMsg += MultiPathDynamicImageDataCheck(l_pMultiPathDynamicImage);
		}
		return l_strErrorMsg;
	}

	eAnimationEvent	IntToAnimationEvent(int e_iIndex)
	{
		switch(e_iIndex)
		{
			case 0:
				return eAE_NONE;
			case 1:
				return eAE_PARTICLE;
			case 2:
				return eAE_SOUND;
		}
		//it'd not be here.
		return eAE_MAX;
	}

	char*	eAnimationEventToAnimationEventString(eAnimationEvent e_eAnimationEvent)
	{
		switch(e_eAnimationEvent)
		{
			case eAE_NONE:
				return "None";
			case eAE_PARTICLE:
				return "Particle";
			case eAE_SOUND:
				return "Sound";
		}
		//it'd not be here.
		return "Max";
	}
	GLenum	IndexToBlendingIndex( int e_iIndex )
	{
		switch( e_iIndex )
		{
			case 0:
				return GL_ZERO;
			case 1:
				return GL_ONE;
			case 2:
				return GL_ONE_MINUS_SRC_COLOR;
			case 3:
				return GL_DST_COLOR;
			case 4:
				return GL_ONE_MINUS_DST_COLOR;
			case 5:
				return GL_SRC_ALPHA;
			case 6:
				return GL_ONE_MINUS_SRC_ALPHA;
			case 7:
				return GL_DST_ALPHA;
			case 8:
				return GL_ONE_MINUS_DST_ALPHA;
		}
		assert(0&&"combox has not follow rule");
		return 0;
	}

	int BlendingIndexToIndex( GLenum e_eBlendingIndex)
	{
		switch( e_eBlendingIndex )
		{
			case GL_ZERO:
				return 0;
			case GL_ONE:
				return 1;
			case GL_ONE_MINUS_SRC_COLOR:
				return 2;
			case GL_DST_COLOR:
				return 3;
			case GL_ONE_MINUS_DST_COLOR:
				return 4;
			case GL_SRC_ALPHA:
				return 5;
			case GL_ONE_MINUS_SRC_ALPHA:
				return 6;
			case GL_DST_ALPHA:
				return 7;
			case GL_ONE_MINUS_DST_ALPHA:
				return 8;
		}
		assert(0&&"combox has not follow rule");
		return -1;
	}

	MPDIEditor::MPDIEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm)
	{
		++m_siOpenCount;
		m_pTimeAndFPS = new UT::sTimeAndFPS;
		m_sfactor = GL_SRC_ALPHA;
		m_dfactor = GL_ONE_MINUS_SRC_ALPHA;
		m_bDataChanged = false;
		m_pOrthogonalCamera = new cOrthogonalCamera();
		InitializeComponent();
		m_fAlpha = 0.f;
		m_pMultiPathDynamicImageAWP = 0;
		m_pImagePlayManagerAWP = 0;
		m_pReferenceCurveManagerAWP = 0;
		m_pBKImageAWP  = 0;
		m_pMPDIList = 0;
		m_iCurrentSelectedPathListIndexAWP = -1;
		m_iCurrentSelectedPathNodeIndexAWP = -1;
		m_pInterlUseForShowImagePos = 0;
		m_pForm = e_pForm;
		m_pParentHandle = 0;
		if( m_pForm->Parent )
		{
			m_pParentHandle = (HWND)m_pForm->Parent->Handle.ToPointer();
		}
		this->Dock = GCFORM::DockStyle::Fill;
		//
		//
		//
		m_pCurrentWorkingPI = 0;
		m_bPreventChangeAllTimeValueSetup = false;
		m_bPreventChangeValue = false;
		m_bMouseEnterPathNodeListbox = false;
		if( e_ptabControl )
		{
			m_pTabPage = gcnew GCFORM::TabPage();
			m_pTabPage->Text = "MPDI/路徑圖片編輯";
			m_pTabPage->AutoScroll = true;
			m_pTabPage->Controls->Add(this);
			e_ptabControl->TabPages->Add(m_pTabPage);
			m_pTabControl = e_ptabControl;
			m_pTabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::tabControl_SelectedIndexChanged);
		}
		else
		{
			e_pForm->Controls->Add(this);
		}

		splitContainer3->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseMove);
		splitContainer3->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseDown);
		splitContainer3->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseUp);
		splitContainer3->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::MyMouseUp);
		m_pMPDIExtraData = gcnew System::Collections::Hashtable;

		UIInitialize();
		m_bAnimationPauseByTrackValue = false;
		m_HdcMV = GetDC((HWND)this->splitContainer3->Panel1->Handle.ToPointer());
		m_HGLRCMV = UT::InitOpenGL((HWND)this->splitContainer3->Panel1->Handle.ToPointer(),true,m_HdcMV);
		//m_HdcMV = GetDC((HWND)splitContainer3->Panel1->Handle.ToPointer());		
		m_bMutiSampleSupported = InitMultisample((HINSTANCE)System::Runtime::InteropServices::Marshal::GetHINSTANCE(splitContainer3->Panel1->GetType()->Module).ToPointer(),(HWND)this->splitContainer3->Panel1->Handle.ToPointer());
		m_pDebugFont = 0;
		if(System::IO::File::Exists("Font.png"))
		{
			m_pDebugFont = new cGlyphFontRender("Font");
			if( !m_pDebugFont->GetFontImage() )
				SAFE_DELETE(m_pDebugFont);
		}
		GroupPositionMoveType_comboBox->SelectedIndex = 0;
		GroupRotationType_comboBox->SelectedIndex = 0;
		m_pTimeCounterForAutoSaveFile = new UT::sTimeCounter();
		m_pTimeCounterForAutoSaveFile->SetTargetTime(300.f);
		CreateShader(g_bCommonVSClientState,L"MPDIEditorShader");
		CreateShader(g_bCommonVSNoTextureClientState,L"MPDIEditor_NoTextureShader");

		RotationPosOffset_xyzNumeriaclContro->MyNumericUpDown1->Increment = 1;
		RotationPosOffset_xyzNumeriaclContro->MyNumericUpDown2->Increment = 1;
		RotationPosOffset_xyzNumeriaclContro->MyNumericUpDown3->Increment = 1;
		ImageAnchor_comboBox->SelectedIndex = 0;

		toolTip1->SetToolTip( this->WholePuzzleImageUnit_listBox, "select images and click right mouse button to add path" );
		toolTip1->SetToolTip( this->PathNode_listBox, "click right mouse button to Fade in Fade out " );
		toolTip1->SetToolTip( this->WholePath_listBox, "click right mouse button to add game UI" );

		
		System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
		l_pElementHots->Dock = DockStyle::Fill;
		if( 1 )
		{
			CoreWPF::VideoPlayer::VideoPlayer^l_pData = gcnew CoreWPF::VideoPlayer::VideoPlayer();
			l_pElementHots->Child = l_pData;
			m_pMPDIThread = gcnew cMPDIThread();
			m_pMPDIThread->SetVideoPlayer(l_pData);
			l_pElementHots->Child = l_pData;
		}
		else
		{
			TextureEditorWPF::MPDI_PathPointData^l_pData = gcnew TextureEditorWPF::MPDI_PathPointData;
			l_pElementHots->Child = l_pData;
		}
		Video_groupBox->Controls->Add(l_pElementHots);
		this->timer1->Enabled = true;
	}

	MPDIEditor::~MPDIEditor()
	{
		--m_siOpenCount;
		DeleteShader(L"MPDIEditorShader");
		DeleteShader(L"MPDIEditor_NoTextureShader");

		SAFE_DELETE(m_pMultiPathDynamicImageAWP);
		if( m_pMPDIList&& !m_pMPDIList->GetOwner() )
		{
			SAFE_DELETE(m_pMPDIList);
		}
		SAFE_DELETE(m_pBKImageAWP);		
		SAFE_DELETE(m_pTimeAndFPS);
		SAFE_DELETE(m_pDebugFont);
		SAFE_DELETE(m_pReferenceCurveManagerAWP);
		SAFE_DELETE(m_pInterlUseForShowImagePos);
		SAFE_DELETE(m_pTimeCounterForAutoSaveFile);
		SAFE_DELETE(m_pOrthogonalCamera);
		SAFE_DELETE(m_pImagePlayManagerAWP);
		if( m_pMPDIThread )
			delete m_pMPDIThread;
		if (components)
		{
			delete components;
		}
	}


	void	MPDIEditor::RenderHintPoint(cCueToStartCurveWithTime*e_pObject,bool e_bShowBorder)
	{
		if( e_pObject )
		{
			if( e_pObject->GetOriginalPointList().size() )
			{
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointList = e_pObject->GetPointDataList();
				cPuzzleImage*l_pPI = 0;
				for(UINT i=0;i<e_pObject->GetOriginalPointList().size();++i)
				{
					sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pPointList)[i];
					l_pTexBehaviorDataWithImageIndexData->DumpData(e_pObject->GetCurrentPointData());
					e_pObject->SetCurrentPosition(e_pObject->GetOriginalPointList()[i]);
					if( l_pPI != l_pTexBehaviorDataWithImageIndexData->pPI )
					{
						l_pPI = l_pTexBehaviorDataWithImageIndexData->pPI;
					}
					e_pObject->Render(l_pPI->GetPuzzleData()[l_pTexBehaviorDataWithImageIndexData->iImageIndex]);
				}
				if( e_bShowBorder )
				{
					cCueToStartCurveWithTime	l_cCueToStartCurveWithTime(e_pObject);
					l_cCueToStartCurveWithTime.Init();
					for(UINT i=0;i<e_pObject->GetOriginalPointList().size();++i)
					{
						float	l_fTime = e_pObject->GetOriginalTimeList()[i];
						l_cCueToStartCurveWithTime.Update(l_fTime);
						l_cCueToStartCurveWithTime.RenderCollide();
					}
				}
			}
		}	
	}
	void	MPDIEditor::AssignLatestPI()
	{
		if(AllPI_listBox->Items->Count)
		{
			AllPI_listBox->SelectedItems->Clear();
			AllPI_listBox->SelectedIndex = AllPI_listBox->Items->Count-1;
		}
		else
		{
			AllPI_listBox->SelectedIndex = -1;
		}
	}

	void	MPDIEditor::UIInitialize()
	{
		LanguageSwitch(this->Controls,"/",this->Handle);
		LanguageSwitch(splitContainer1->Panel1->Controls,"/",this->Handle);
		//LanguageSwitch(EditData_groupBox->Controls,"/",this->Handle);
		//LanguageSwitch(this->PIData_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(this->splitContainer1->Controls,"/",this->Handle);
		LanguageSwitch(AnimationData_collpaseForm->Controls,"/",this->Handle);

		LanguageSwitch(Path_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(ImageHintPoint_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(HintPoint_groupBox->Controls,"/",this->Handle);


		LanguageSwitch(Size_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(Color_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(Position_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(ColorBlending_groupBox->Controls,"/",this->Handle);
		LanguageSwitch(Rotation_groupBox->Controls,"/",this->Handle);

		//HintPoint_collpaseForm->DoCollpease();
		//AllPathList_collpaseForm->DoCollpease();
//		HintPoint_collpaseForm->DoCollpease();
//		AllPathList_collpaseForm->DoCollpease();
//		GroupData_collpaseForm->DoCollpease();
		splitContainer2->SplitterDistance = 35;
	}

	void	MPDIEditor::DelPoint(System::Object^  sender, System::EventArgs^  e)
	{
		if( sender == InsertPoint_button )
		{
			 if( WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( PathNode_listBox->SelectedIndex != -1 )
				 {
					 if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve&& (int)m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList().size()>PathNode_listBox->SelectedIndex)
					{
						int	l_iNumPoint = (int)NumPointsToInsert_numericUpDown->Value;
						Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[PathNode_listBox->SelectedIndex];
						float	l_fTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[PathNode_listBox->SelectedIndex];
						//relate  to select point
						float	l_fNextPointTime = l_fTime;
						if( PathNode_listBox->Items->Count-1>PathNode_listBox->SelectedIndex )
						{
							l_fNextPointTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[PathNode_listBox->SelectedIndex+1];
						}
						float	l_fStepX = 0.f;
						float	l_fStepY = 0.f;
						if( PathNode_listBox->SelectedIndex+1<PathNode_listBox->Items->Count )
						{
							Vector3	l_vDestPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[PathNode_listBox->SelectedIndex+1];
							l_fStepX = (l_vDestPos.x-l_vPos.x)/l_iNumPoint;
							l_fStepY = (l_vDestPos.y-l_vPos.y)/l_iNumPoint;
						}
						l_iNumPoint--;
						float	l_fStepTime = (l_fNextPointTime-l_fTime)/(l_iNumPoint+1);
						for( int i=0;i<l_iNumPoint;++i )
						{
							l_vPos.x+=l_fStepX;
							l_vPos.y+=l_fStepY;
							sTexBehaviorDataWithImageIndexData	*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[PathNode_listBox->SelectedIndex];
							m_pMultiPathDynamicImageAWP->m_pCurrentCurve->InsertPoint(l_vPos,(l_fStepTime*(i+1))+l_fTime,l_pTexBehaviorDataWithImageIndexData,PathNode_listBox->SelectedIndex+1+i);
						}
						int	l_iIndex = WholePath_listBox->SelectedIndex;
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
			 }
		}
		else
		{
			 if( WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( PathNode_listBox->SelectedIndex != -1 )
				 {
					if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve)
					{
						 System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
						 l_MyList->AddRange(PathNode_listBox->SelectedItems);
						 for each (System::Object^l_pObject in l_MyList)
						 {
							int	l_iIndex = PathNode_listBox->Items->IndexOf(l_pObject);
							if( l_iIndex == -1 )
								continue;
							PathNode_listBox->Items->Remove(l_pObject);
							m_pMultiPathDynamicImageAWP->m_pCurrentCurve->DelPoint(l_iIndex);
						 }
						int	l_iIndex = WholePath_listBox->SelectedIndex;
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
			 }
		}	
	}

	void	MPDIEditor::PathSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( WholePath_listBox->SelectedIndex == -1 )
		{
			m_pMultiPathDynamicImageAWP->m_pCurrentCurve = 0;
			PathNode_listBox->Items->Clear();
			AllEventPointHint_listBox->Items->Clear();
			return;
		}
		m_bPreventChangeValue = true;
		m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(WholePath_listBox->SelectedIndex);
		if( !m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
			return;
		cCurveWithTime*l_cCurveWithTime = (*m_pMultiPathDynamicImageAWP)[m_pMultiPathDynamicImageAWP->GetCurrentSelectedIndex()];

		Loop_checkBox->Checked = l_cCurveWithTime->IsCurveLoop();

		std::vector<Vector3>	l_PointList = l_cCurveWithTime->GetOriginalPointList();
		std::vector<float>		l_TimeList = l_cCurveWithTime->GetOriginalTimeList();
		if( sender == WholePath_listBox )
		{
			m_bPreventChangeAllTimeValueSetup = true;
			if( m_pMultiPathDynamicImageAWP->Count()>WholePath_listBox->SelectedIndex )
			{
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve = (*m_pMultiPathDynamicImageAWP)[WholePath_listBox->SelectedIndex];
				ImageAnchor_comboBox->SelectedIndex = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType();
				EnableColorBlending_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsColorBlending();
				BehaviorUpdateActive_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsBehaviorUpdateActive();
				RotationPosOffset_xyzNumeriaclContro->SetValue(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
			}
			PathLOD_numericUpDown->Value = l_cCurveWithTime->GetLOD();
			StayAtLastFrame_checkBox->Checked = m_pMultiPathDynamicImageAWP->IsStopAtLastFrame();
			PathNode_listBox->Items->Clear();
			int	l_iNum = l_PointList.size();
			for( int i=0;i<l_iNum;++i )
			{
				Vector3	l_v2 = l_PointList[i];
				//String^l_strTime = l_TimeList[i].ToString();
				//PathNode_listBox->Items->Add( i.ToString()+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[i].ToString() );
				if( m_pImageResourceManagerAWP->Count() )
				{
					cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve;
					int	l_iIndex = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[i]->iImageIndex;
					if( l_iIndex != -1 )
					{
					    sPuzzleData**l_ppsPuzzleData = 0;
					    if( (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[i]->pPI )
					    {
					        l_ppsPuzzleData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[i]->pPI->GetPuzzleData();
					    }
					    else
					        l_ppsPuzzleData = m_pMPDIList->GetPuzzleData();
						PathNode_listBox->Items->Add( i.ToString()+":"+DNCT::WcharToGcstring( l_ppsPuzzleData[l_iIndex]->strFileName )+":X="+((int)l_v2.x).ToString()+",y="+((int)l_v2.y).ToString()+",z="+((int)l_v2.z).ToString()+",Time="+l_TimeList[i].ToString() );
					}
				}
			}
			if( l_TimeList.size() && !AllPlay_checkBox->Checked )
			{
				CurrentProgress_trackBar->Maximum = (int)(l_TimeList[l_TimeList.size()-1]*1000);
			}
			//OffsetPos_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsAvtivePosOffset();
			StartTime_numericUpDown->Value = (int)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetStartTime()*1000);
			TotalPlayTime_numericUpDown->Value = (int)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetLastTime()*1000);
			m_bPreventChangeValue = false;
			m_bPreventChangeAllTimeValueSetup = false;
			AllEventPointHint_listBox->Items->Clear();
			cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_pMultiPathDynamicImageAWP->GetObject(WholePath_listBox->SelectedIndex);
			std::vector<sHintPoint>*l_pVectorPointer = l_pCueToStartCurvesWithTime->GetHintPointVector();
			for( UINT i=0;i<l_pVectorPointer->size();++i )
			{
				sHintPoint*l_pHintPoint = &(*l_pVectorPointer)[i];
				AllEventPointHint_listBox->Items->Add(
					l_pHintPoint->iPointIndex.ToString()+":"+
					gcnew String(eAnimationEventToAnimationEventString(l_pHintPoint->AnimationEvent))+","+
					DNCT::WcharToGcstring(l_pHintPoint->strEventName));
			}
			if( moveAllPathToolStripMenuItem->Checked ||  rotateAllPathToolStripMenuItem->Checked )
			{
				m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.clear();
				for each (int l_iSelectedIndex in WholePath_listBox->SelectedIndices)
				{
					m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.push_back(l_iSelectedIndex);
				}
			}
			if( sender == WholePath_listBox )
			{
				if( PathNode_listBox->Items->Count == 1 )
					PathNode_listBox->SelectedIndex = 0;
			}
		}
		else
		if( sender == PathNode_listBox )
		{
			if( PathNode_listBox->SelectedIndex == -1 || (m_iCurrentSelectedPathNodeIndexAWP == PathNode_listBox->SelectedIndex && m_iCurrentSelectedPathListIndexAWP == WholePath_listBox->SelectedIndex) )
			{
				m_bPreventChangeValue = false;
				return;
			}
			if( PathNode_listBox->SelectedIndices->Count!=1 )
			{
				m_bPreventChangeValue = false;
				return;
			}
			if( !m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve = (*m_pMultiPathDynamicImageAWP)[WholePath_listBox->SelectedIndex];
			sTexBehaviorDataWithImageIndexData	*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[PathNode_listBox->SelectedIndex];
			m_iCurrentSelectedPathNodeIndexAWP = PathNode_listBox->SelectedIndex;
			m_iCurrentSelectedPathListIndexAWP = WholePath_listBox->SelectedIndex;
			//WCHAR	l_str[TEMP_SIZE];
			//swprintf(l_str,TEMP_SIZE,L"Index = %d\n\0",PathNode_listBox->SelectedIndex);
			//OutputDebugString(l_str);
			PathNodePosX_numericUpDown->Value = (int)l_PointList[PathNode_listBox->SelectedIndex].x;
			PathNodePosY_numericUpDown->Value = (int)l_PointList[PathNode_listBox->SelectedIndex].y;
			PathNodePosZ_numericUpDown->Value = (int)l_PointList[PathNode_listBox->SelectedIndex].z;
			AngleX_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->vAngle.x;
			AngleY_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->vAngle.y;
			AngleZ_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->vAngle.z;
			Width_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->Size.x;
			Height_numericUpDown->Value = (int)l_pTexBehaviorDataWithImageIndexData->Size.y;
			Vector4	l_vColor = l_pTexBehaviorDataWithImageIndexData->vColor;
			RedColor_numericUpDown->Value = (int)(l_vColor.x*255);
			GreenColor_numericUpDown->Value = (int)(l_vColor.y*255);
			BlueColor_numericUpDown->Value = (int)(l_vColor.z*255);
			AlphaColor_numericUpDown->Value = (int)(l_vColor.w*255);
			this->Mirror_checkBox->Checked = l_pTexBehaviorDataWithImageIndexData->bMirror;
			int l_Value = (int)(l_TimeList[PathNode_listBox->SelectedIndex]*1000.f);
			PathNodeTime_numericUpDown->Value = (int)l_Value;
			StartTime_numericUpDown->Value = (int)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetStartTime()*1000);
			BehaviorUpdateActive_checkBox->Checked = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsBehaviorUpdateActive();

			if( sender == AddShowImageOnScreenObject_button )
			{
				for each( int l_iIndex in PathNode_listBox->SelectedIndices )
				{
					sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
					Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iIndex];
					m_pInterlUseForShowImagePos->AddPoint(l_vPos,0,l_pTexBehaviorDataWithImageIndexData->Size,l_pTexBehaviorDataWithImageIndexData->vAngle,l_pTexBehaviorDataWithImageIndexData->vColor,l_pTexBehaviorDataWithImageIndexData->iImageIndex,l_pTexBehaviorDataWithImageIndexData->bMirror,l_pTexBehaviorDataWithImageIndexData->pPI);
					ShowImageOnScreen_listBox->Items->Add(WholePath_listBox->SelectedItem->ToString()+":"+l_iIndex.ToString());
					m_pInterlUseForShowImagePos->SetAnimationDone(false);
					m_pInterlUseForShowImagePos->SetStart(true);
				}
			}
		}
		m_bPreventChangeValue = false;
	}

	void	MPDIEditor::PlayCheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		 if( !m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
			 return;
		 if( sender == Play_checkBox )
		 {
			 if( Play_checkBox->Checked )
			 {
				 if( !m_pMultiPathDynamicImageAWP )
				 {
					 Play_checkBox->Checked = false;
					 return;
				 }
				 TotalPlayTime_numericUpDown->Enabled = false;
				 m_pMultiPathDynamicImageAWP->RefreshTotalPlayTime();
				 ((cFatmingAnimationRule*)(m_pMultiPathDynamicImageAWP->m_pCurrentCurve))->Init();
				 m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetStart(true);
				 AllPlay_checkBox->Checked = false;
				 m_fTimeForCurrentProgess = 0.f;
				 UINT	l_iSize = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList().size();
				 if( l_iSize )
				 {
					 CurrentProgress_trackBar->Minimum = 0;
					 CurrentProgress_trackBar->Maximum = (int)(m_pMultiPathDynamicImageAWP->GetTotalPlayTime()*1000);
				 }
			 }
			 else
				 TotalPlayTime_numericUpDown->Enabled = true;
			 m_bAnimationPauseByTrackValue = !Play_checkBox->Checked;
			 this->m_pMPDIThread->GetVideo()->Play(sender,e);
		 }
		 else
		 if( sender == Loop_checkBox )
		 {
			 if( m_bPreventChangeValue == false )
			 {
				for each(int l_iSelectedIndex in WholePath_listBox->SelectedIndices)
					m_pMultiPathDynamicImageAWP->GetObject(l_iSelectedIndex)->SetAnimationLoop(Loop_checkBox->Checked);
			 }
		 }
		 else
		 if( sender == AllPlay_checkBox )
		 {
			 if( !m_pMultiPathDynamicImageAWP )
			 {
				 AllPlay_checkBox->Checked = false;
				 return;
			 }
			 if( AllPlay_checkBox->Checked )
			 {
				 m_pMultiPathDynamicImageAWP->Init();
				 m_fTimeForCurrentProgess = 0.f;
				 TotalPlayTime_numericUpDown->Enabled = false;
			 }
			 else
			 {
				 TotalPlayTime_numericUpDown->Enabled = true;
			 }
			 if( AllPlay_checkBox->Checked )
				Play_checkBox->Checked = false;
			 m_pMultiPathDynamicImageAWP->RefreshTotalPlayTime();
			 CurrentProgress_trackBar->Minimum = 0;
			 CurrentProgress_trackBar->Maximum = (int)(m_pMultiPathDynamicImageAWP->GetTotalPlayTime()*1000);
			 m_bAnimationPauseByTrackValue = !AllPlay_checkBox->Checked;
			 if( m_pMPDIThread&&this->m_pMPDIThread->GetVideo() )
				this->m_pMPDIThread->GetVideo()->Play(sender,e);
		 }
		 else
		 if( sender == StayAtLastFrame_checkBox )
		 {
			 if( m_pMultiPathDynamicImageAWP )
			 {
				m_pMultiPathDynamicImageAWP->SetStopAtLastFrame(StayAtLastFrame_checkBox->Checked);
			 }
		 }
		 else
		 if( sender == AllCurveLoop_checkBox )
		 {
			 if( m_pMultiPathDynamicImageAWP )
			 {
				 if(AllCurveLoop_checkBox->Checked)
					 m_pMultiPathDynamicImageAWP->SetPlayCount(-1);
				 else
					 m_pMultiPathDynamicImageAWP->SetPlayCount(1);
			 }
		 }
	}

	void	MPDIEditor::AddPath(String^e_strPathName)
	{
		if( !m_pMPDIList )
			return;
		if(!DNCT::CheckListContainStringAndAdd(this->WholePath_listBox,e_strPathName))
		{
			WholePath_listBox->SelectedItems->Clear();
			cCueToStartCurveWithTime*l_pCueToStartCurvesWithTimeAWP = new cCueToStartCurveWithTime(m_pMPDIList);
			l_pCueToStartCurvesWithTimeAWP->SetName(DNCT::GcStringToWchar(e_strPathName));			
			if( !m_pMultiPathDynamicImageAWP )
				m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList);
			m_pMultiPathDynamicImageAWP->AddObject(l_pCueToStartCurvesWithTimeAWP);
			m_pMultiPathDynamicImageAWP->RefreshData();
			m_pMultiPathDynamicImageAWP->m_pCurrentCurve = l_pCueToStartCurvesWithTimeAWP;
			RotationPosOffset_xyzNumeriaclContro->SetValue(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
			l_pCueToStartCurvesWithTimeAWP->SetLOD((int)PathLOD_numericUpDown->Value);
			WholePath_listBox->SelectedIndex = WholePath_listBox->Items->Count-1;
		}
		else
			WARNING_MSG("such name existed already");	
	}

	void	MPDIEditor::AddPath(System::Object^  sender, System::EventArgs^  e)
	{
		 if( DelPath_button == sender )
		 {
			if( WholePath_listBox->SelectedItem )
			{
				AllPlay_checkBox->Checked = false;
				m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(-1);

				for each (String^l_strSelectedName in WholePath_listBox->SelectedItems)
				{
					m_pMultiPathDynamicImageAWP->RemoveObject(DNCT::GcStringToWchar(l_strSelectedName));
				}
				while(WholePath_listBox->SelectedItems->Count>0)
					WholePath_listBox->Items->Remove(WholePath_listBox->SelectedItem);
				if( m_pMultiPathDynamicImageAWP->Count() == 0 )
				{
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve = 0;
					RotationPosOffset_xyzNumeriaclContro->SetValue(0);
				}
				else
				{
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve = m_pMultiPathDynamicImageAWP->GetObject(0);
					RotationPosOffset_xyzNumeriaclContro->SetValue(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
				}
				m_pMultiPathDynamicImageAWP->RefreshData();
				PathNode_listBox->Items->Clear();
				if( AllPlay_checkBox->Checked )
				{
					CurrentProgress_trackBar->Maximum = (int)(m_pMultiPathDynamicImageAWP->GetTotalPlayTime()*1000);
					if( CurrentProgress_trackBar->Minimum>=CurrentProgress_trackBar->Maximum )
						CurrentProgress_trackBar->Minimum = 0;
				}
			}
		 }
		 else
		 if( sender == AddPath_button )
		 {
			if( !m_pMPDIList )
				return;
	 		FMCEditor::InputNameDialog^l_pForm = gcnew FMCEditor::InputNameDialog;
			if(WholePuzzleImageUnit_listBox->SelectedItem)
			{
				l_pForm->SetupTextBoxData(WholePuzzleImageUnit_listBox->SelectedItem->ToString());
			}
			l_pForm->ShowDialog();
			if( strlen(l_pForm->m_strTextName->c_str()) )
			{
				AddPath(String(l_pForm->m_strTextName->c_str()).ToString());
			}
			ImageAnchor_comboBox->SelectedIndex = eIAT_CENTER_CENTER;
			Play_checkBox->Checked = false;
			delete l_pForm;
		 }
		 else
		 if( sender == ClonePath_button )
		 {
			if( WholePath_listBox->SelectedItem )
			{
	 			FMCEditor::InputNameDialog^l_pForm = gcnew FMCEditor::InputNameDialog;

				if(WholePuzzleImageUnit_listBox->SelectedItem)
				{
					l_pForm->SetupTextBoxData(WholePuzzleImageUnit_listBox->SelectedItem->ToString());
				}
				else
					l_pForm->SetupTextBoxData(WholePath_listBox->SelectedItem->ToString());
				l_pForm->ShowDialog();
				if( strlen(l_pForm->m_strTextName->c_str()) )
				{
					if(!DNCT::CheckListContainStringAndAdd(this->WholePath_listBox,String(l_pForm->m_strTextName->c_str()).ToString()))
					{
						WCHAR*	l_strName = DNCT::GcStringToWchar(WholePath_listBox->SelectedItem->ToString());
						cCueToStartCurveWithTime*l_pOriginalCueToStartCurvesWithTimeAWP = m_pMultiPathDynamicImageAWP->GetObject(l_strName);
						cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = new cCueToStartCurveWithTime(l_pOriginalCueToStartCurvesWithTimeAWP);

						l_pCueToStartCurvesWithTime->SetName(UT::CharToWchar(l_pForm->m_strTextName->c_str()));

						m_pMultiPathDynamicImageAWP->AddObject(l_pCueToStartCurvesWithTime);
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve = l_pCueToStartCurvesWithTime;
						l_pCueToStartCurvesWithTime->SetLOD((int)PathLOD_numericUpDown->Value);
						WholePath_listBox->SelectedIndices->Clear();
						WholePath_listBox->SelectedIndex = WholePath_listBox->Items->Count-1;
					}
					else
						WARNING_MSG("such name existed already");
				}
				delete l_pForm;
			}
		 }
		 else
		 if( sender == InvertPath_button )
		 {
			 if( m_pMultiPathDynamicImageAWP )
			 {
				 for each( int l_iSelectedIndex in WholePath_listBox->SelectedIndices )
				 {
					 m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(l_iSelectedIndex);
					 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					 {
						 m_pMultiPathDynamicImageAWP->m_pCurrentCurve->InvertOrder();
					 }
				 }
				 WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e);
			 }
		 }
		 else
		 if( InvertAllPath_button == sender )
		 {
			 if( WholePath_listBox->SelectedIndex != -1 )
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				 {
					 for( int i=0;i<m_pMultiPathDynamicImageAWP->Count();++i )
					 {
						(*m_pMultiPathDynamicImageAWP)[i]->InvertOrder();
					 }
					 WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e);
				 }
			 }
		 }
	}

	void	MPDIEditor::AddIntoList()
	{
		 if(!m_pMultiPathDynamicImageAWP)
		 {
			WARNING_MSG("make a animation before u add new one");
			return;
		 }
		 String^l_strErrorMsg = MultiPathDynamicImageDataCheck(m_pMultiPathDynamicImageAWP);
		 if( l_strErrorMsg&&l_strErrorMsg->Length )
		 {
			WARNING_MSG(l_strErrorMsg);
		 }
		 bool	l_bNewObject = true;
		 WCHAR*l_strName = DNCT::GcStringToWchar( AnimationName_textBox->Text );
		 if( CheckListContainStringAndAdd(WholeAnimationList_listBox,AnimationName_textBox->Text ) )
		 {
			 WARING_YES_NO_TO_NO("such object name already exists,would u like to replace?"+DNCT::GetChanglineString()+
				 "該物件名稱已存在,要覆蓋嗎?")
			 {
				return;
			 }
			 else
			 {
				l_bNewObject = false;
				m_pMPDIList->RemoveObject(l_strName);
				WholeAnimationList_listBox->Items->Remove(AnimationName_textBox->Text);
			 }
		 }
		 cMultiPathDynamicImage*l_pMultiPathDynamicImageAWP = new cMultiPathDynamicImage(m_pMultiPathDynamicImageAWP);
		 l_pMultiPathDynamicImageAWP->SetName(l_strName);
		 m_pMPDIList->AddObject(l_pMultiPathDynamicImageAWP);
		 if( !l_bNewObject )
			WholeAnimationList_listBox->Items->Add(AnimationName_textBox->Text);
		 if(this->m_pMPDIExtraData->ContainsKey(AnimationName_textBox->Text))
		 {
			 m_pMPDIExtraData->Remove(AnimationName_textBox->Text);
		 }
		 if( MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Count )
		 {
			 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Text = AnimationName_textBox->Text;
			 m_pMPDIExtraData[AnimationName_textBox->Text] = MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes[0]->Clone();
		 }
		 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();

		 if(m_strCurrentSaveFileName&&System::IO::File::Exists(m_strCurrentSaveFileName))
		 {
			Save(m_strCurrentSaveFileName);
		 }
		 else
		 {
			System::EventArgs^  e;
			SaveAndOpen_MPDI(saveToolStripMenuItem,e);
		 }
	}

	void	MPDIEditor::Render()
	{
		if( m_bMutiSampleSupported )
			glEnable(GL_MULTISAMPLE_ARB);
		wglMakeCurrent( m_HdcMV,m_HGLRCMV );
		m_fAlpha += 0.03f;
		if( m_fAlpha>1.f )
			m_fAlpha = 0.f;
		glViewport(0,0,splitContainer3->Panel1->Width,splitContainer3->Panel1->Height);
		m_pTimeAndFPS->Update();
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( g_vBackgroundColor.x,g_vBackgroundColor.y,g_vBackgroundColor.z,g_vBackgroundColor.w );
		glClearDepth(1.0f);											// Depth Buffer Setup
		glAlphaFunc(GL_GREATER,0.0001f);
		glShadeModel(GL_SMOOTH);									// Select Smooth Shading
		POINT l_ViewportSize = { splitContainer3->Panel1->Width,splitContainer3->Panel1->Height };
		m_pOrthogonalCamera->Render();
		if(this->showGridToolStripMenuItem->Checked)
			m_pOrthogonalCamera->DrawGrid(0,0,Vector4(0.3f,0.7f,0.3f,0.7f));
		glEnable(GL_TEXTURE_2D);
		if( m_pMultiPathDynamicImageAWP )
		{
			POINT	l_CameraMousePos = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(m_sfactor,m_dfactor);
			glEnable(GL_TEXTURE_2D);
			if(m_pBKImageAWP)
			{
				if(this->showBGToolStripMenuItem->Checked)
					m_pBKImageAWP->Render();
			}
			if(EnableColorBlending_checkBox->Checked)
			{
				glBlendFunc(IndexToBlendingIndex(this->SrcBlending_comboBox->SelectedIndex),IndexToBlendingIndex(this->DestBlending_comboBox->SelectedIndex));
			}
			if(EnableColorBlending_checkBox->Checked)
			{
				glBlendFunc(m_sfactor,m_dfactor);		
			}
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			if( ShowAllPath_checkBox->Checked )
			{
				m_pMultiPathDynamicImageAWP->RenderALLCurve();
			}
			m_pOrthogonalCamera->Render();
			//???it should call in mouse move event???
			//if( fixPointModeToolStripMenuItem->Checked)
			//{
			//	m_pMultiPathDynamicImageAWP->MouseMove(l_CameraMousePos);
			//}
			if( m_pMPDIThread )
			{
				m_pMPDIThread->GetVideo()->SetCurrentVideoTime(CurrentProgress_trackBar->Value/1000.f);
				m_pMPDIThread->GetVideo()->UpdateTrackBar();
				m_pMPDIThread->Render();
			}
			if( Play_checkBox->Checked )
			{
				if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				{
					UINT	l_iSize = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList().size();
					if( l_iSize )
					{
						float	l_fTotalPlayTime = m_pMultiPathDynamicImageAWP->GetTotalPlayTime();
						
						if( m_bAnimationPauseByTrackValue == false )
							m_fTimeForCurrentProgess+=m_pTimeAndFPS->fElpaseTime;
						label9->Text = "Progress:"+CurrentProgress_trackBar->Value.ToString();
						if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->IsAnimationLoop())
						{
							if( m_fTimeForCurrentProgess>=l_fTotalPlayTime )
								m_fTimeForCurrentProgess = 0.f;
						}
						if( m_fTimeForCurrentProgess>=l_fTotalPlayTime )
						{
							int	l_iDestValue = (int)(l_fTotalPlayTime*1000.f);
							if( CurrentProgress_trackBar->Maximum >= l_iDestValue )							
								CurrentProgress_trackBar->Value = l_iDestValue;
						}
						else
						{
							int	l_iDestValue = (int)(m_fTimeForCurrentProgess*1000.f);
							if( CurrentProgress_trackBar->Maximum >= l_iDestValue )
								CurrentProgress_trackBar->Value = l_iDestValue;
						}
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPositionByTime(m_fTimeForCurrentProgess);
						//m_pMultiPathDynamicImageAWP->Update(m_pTimeAndFPS->fElpaseTime);
						glEnable(GL_TEXTURE_2D);
						glEnable(GL_ALPHA_TEST);
						//m_pMultiPathDynamicImageAWP->RenderWithImage();
						if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex() !=-1 )
						{
							int	l_i = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex();
							//WCHAR*l_strPIName = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPuzzleData()[m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex()]->strFileName;
							//m_pImageResourceManagerAWP->GetPuzzleImage(l_strPIName)->ApplyImage();
							m_pMultiPathDynamicImageAWP->m_pCurrentCurve->Render(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPuzzleData()[m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentImageIndex()]);
						}
						glDisable(GL_TEXTURE_2D);
						glDisable(GL_ALPHA_TEST);
					}
				}
			}
			else
			//if( AllPlay_checkBox->Checked )
			{
				if( m_pMultiPathDynamicImageAWP )
				{
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_ALPHA_TEST);
					m_pMultiPathDynamicImageAWP->UpdateByGlobalTime(m_fTimeForCurrentProgess);
					label9->Text = "Progress:"+CurrentProgress_trackBar->Value.ToString();


					Vector4*l_pViewPort = m_pMultiPathDynamicImageAWP->GetViewPort();
					if( l_pViewPort )
					{
						glEnable(GL_SCISSOR_TEST);
						Vector4 l_ResultViewPort;

						Vector2	l_vViewPort((float)l_ViewportSize.x,(float)l_ViewportSize.y);
						//fuck here do this for scissor
						cGameApp::m_svViewPortSize = Vector2((float)splitContainer3->Panel1->Width,(float)splitContainer3->Panel1->Height);
						cGameApp::m_svGameResolution = m_pOrthogonalCamera->GetScreenViewPortSize();
						if(m_pOrthogonalCamera->ViewportConvert(*l_pViewPort,l_vViewPort,&l_ResultViewPort))
						{
							glScissor((int)l_ResultViewPort.x,(int)l_ResultViewPort.y,(int)l_ResultViewPort.z,(int)l_ResultViewPort.w);
							MyGlErrorTest();
						}
					}
					m_pMultiPathDynamicImageAWP->RenderByGlobalTime();
					if( l_pViewPort )
					{
						glDisable(GL_SCISSOR_TEST);
					}
					if( m_bAnimationPauseByTrackValue == false )
						m_fTimeForCurrentProgess+=m_pTimeAndFPS->fElpaseTime;
					if( m_pMultiPathDynamicImageAWP->GetCurrentProgress()<=1.f )
					{
						int	l_iDestValue = (int)(m_pMultiPathDynamicImageAWP->GetTotalPlayTime()*1000*m_pMultiPathDynamicImageAWP->GetCurrentProgress());
						if( CurrentProgress_trackBar->Maximum >= l_iDestValue )
						{
							CurrentProgress_trackBar->Value = l_iDestValue;
						}
					}
					if( m_pMultiPathDynamicImageAWP->GetPlayCount() == -1 )
					if( m_fTimeForCurrentProgess >= m_pMultiPathDynamicImageAWP->GetTotalPlayTime() )
					{
						m_fTimeForCurrentProgess = 0.f;
					}
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_ALPHA_TEST);
				}
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			RenderHintPoint(m_pInterlUseForShowImagePos,showImageBoderToolStripMenuItem->Checked);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			if( showPIImageToolStripMenuItem->Checked&&addPointModeToolStripMenuItem->Checked )
			{
				if( m_pCurrentWorkingPI&&m_pMultiPathDynamicImageAWP->m_pBasemageForShow )
				{
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_ALPHA_TEST);
					sPuzzleData**l_ppPuzzleData = this->m_pCurrentWorkingPI->GetPuzzleData();
					sPuzzleData*l_pPuzzleData = l_ppPuzzleData[WholePuzzleImageUnit_listBox->SelectedIndex];
					POINT	l_OriginalSize = l_pPuzzleData->OriginalSize;
					POINT	l_ShowPos = {l_CameraMousePos.x,l_CameraMousePos.y};
					POINT	l_OffsetPos = l_pPuzzleData->OffsetPos;
					m_pMultiPathDynamicImageAWP->m_pBasemageForShow->Render(l_ShowPos.x+l_OffsetPos.x,l_ShowPos.y+l_OffsetPos.y);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_ALPHA_TEST);
					if(ShowSelectedImageBorder_checkBox->Checked)
					{
						l_ShowPos.x = l_CameraMousePos.x;
						l_ShowPos.y = l_CameraMousePos.y;
						RenderRectangle(l_ShowPos,l_OriginalSize.x,l_OriginalSize.y,Vector4(1.f,1.f,0.3f,0.8f));
					}
				}
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			if( showSelectedImageToolStripMenuItem->Checked )
			{
				if( m_pMultiPathDynamicImageAWP->m_pBasemageForShow )
				{
					Vector2	l_vCameraPos = m_pOrthogonalCamera->GetCameraPos();
					m_pMultiPathDynamicImageAWP->m_pBasemageForShow->Render((int)(l_vCameraPos.x-m_pOrthogonalCamera->GetResolution().x/2*m_pOrthogonalCamera->GetScale()),
						(int)(l_vCameraPos.y-m_pOrthogonalCamera->GetResolution().y/2*m_pOrthogonalCamera->GetScale()));
				}
			}
			cCueToStartCurveWithTime	l_CueToStartCurvesWithTime(m_pMPDIList);
			{//render selected node
				if( !AllPlay_checkBox->Checked )
				{
					for each( int l_iIndex in PathNode_listBox->SelectedIndices )
					{
						if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve && (int)m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size()>l_iIndex )
						{
							sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
							Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iIndex];
							Vector4	l_vColor = l_pTexBehaviorDataWithImageIndexData->vColor;
							if( flashSelectedImageToolStripMenuItem->Checked )
								l_vColor.w = m_fAlpha;
							l_CueToStartCurvesWithTime.AddPoint(l_vPos,0,l_pTexBehaviorDataWithImageIndexData->Size,
								l_pTexBehaviorDataWithImageIndexData->vAngle,l_vColor,l_pTexBehaviorDataWithImageIndexData->iImageIndex,l_pTexBehaviorDataWithImageIndexData->bMirror,l_pTexBehaviorDataWithImageIndexData->pPI,false);
						}
					}
					if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					{
						l_CueToStartCurvesWithTime.SetRotationAnglePosOffset(*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRotationAnglePosOffset());
						l_CueToStartCurvesWithTime.SetAnchorType(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType());
					}
					l_CueToStartCurvesWithTime.SetAnimationDone(false);
					l_CueToStartCurvesWithTime.SetStart(true);
					RenderHintPoint(&l_CueToStartCurvesWithTime,true);
				}
			}
			glDisable(GL_ALPHA_TEST);
		}
		else
		{
			if( m_pMPDIThread )
			{
				m_pMPDIThread->Update(m_pTimeAndFPS->fElpaseTime);
				m_pMPDIThread->Render();
			}
		}
		if( m_pDebugFont )
		{
			glEnable(GL_TEXTURE_2D);
			glEnable2D(1280,720);
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			WCHAR*l_strMousePos;
			l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString()+","+ptCursor.y.ToString());
			m_pDebugFont->RenderFont(0,0,l_strMousePos);
			for(int i=0;i<PathNode_listBox->SelectedIndices->Count;++i)
			{
				int	l_iSelectedIndex = PathNode_listBox->SelectedIndices[i];
				sTexBehaviorDataWithImageIndexData	*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSelectedIndex];
				sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData()[l_pTexBehaviorDataWithImageIndexData->iImageIndex];
				float	l_fVertices[8];
				float	l_fUV[8];
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPositionByTime(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[l_iSelectedIndex]);
				cMatrix44	l_mat2DTransform = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetRenderPuzzleDataAndMatrix(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetCurrentPointData(),2,l_fVertices,l_fUV,l_pPuzzleData,m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iSelectedIndex],l_pTexBehaviorDataWithImageIndexData->vAngle);
				Vector3	l_vPos(l_fVertices[0],l_fVertices[1],0.f);
				l_vPos = l_mat2DTransform.TransformCoordinate(l_vPos);
				std::wstring	l_strLeftUpPos = L"Pos:";
				l_strLeftUpPos += ValueToStringW(l_vPos);
				m_pDebugFont->RenderFont(0,0+30*i+30,l_strLeftUpPos.c_str());
			}
			glDisable(GL_TEXTURE_2D);
		}
		m_pOrthogonalCamera->Render();
		if( m_bMutiSampleSupported )
			glDisable(GL_MULTISAMPLE_ARB);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		if( showPath_ToolStripMenuItem->Checked )
		{
			//if( flashSelectedImageToolStripMenuItem->Checked )
			//	m_fAlpha = 1.f;
			if( m_pMultiPathDynamicImageAWP )
				m_pMultiPathDynamicImageAWP->Render(Vector4(1,m_fAlpha,m_fAlpha,0.5f));
		}
		POINT	l_Pos = {0,0};
		RenderRectangle(l_Pos,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT,Vector4(1.f,0.f,0.f,1.f));
		GLRender::glDisable2D();
		SwapBuffers (m_HdcMV);
	}

	void	MPDIEditor::AssignValueChange(System::Object^  sender)
	{
		int	l_iWholePathSelectedIndec = WholePath_listBox->SelectedIndex;
		if( WholePath_listBox->SelectedIndex != -1 && WholePath_listBox->SelectedItems->Count == 1 &&
			PathNode_listBox->SelectedIndices->Count >0)//only for one path,and specific nodes.
		{
			if( StartTime_numericUpDown == sender )
			{
				for each(int l_iSelectedIndex in WholePath_listBox->SelectedIndices)
				{
					m_pMultiPathDynamicImageAWP->GetObject(l_iSelectedIndex)->SetStartTime((float)StartTime_numericUpDown->Value/1000.f);
				}
				return;
			}
			if(PathNode_listBox->SelectedIndex == -1)
				return;
			int	l_iSmallestIndex = PathNode_listBox->Items->Count;
			for each ( int l_SelectedIndex in PathNode_listBox->SelectedIndices )
				if(l_iSmallestIndex>l_SelectedIndex)
					l_iSmallestIndex = l_SelectedIndex;
			//find first data want to change,and get it's original data to do a data mining
			sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSmallestIndex];
			m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(WholePath_listBox->SelectedIndex);
			bool	l_bAeverageAssign = AverageAssign_checkBox->Checked;
			Vector3	l_vAngle((float)AngleX_numericUpDown->Value,(float)AngleY_numericUpDown->Value,(float)AngleZ_numericUpDown->Value);
			float	l_fStartAngle = l_pTexBehaviorDataWithImageIndexData->vAngle.z;

			Vector4	l_vColor = Vector4((float)RedColor_numericUpDown->Value/255.f,
									  (float)GreenColor_numericUpDown->Value/255.f,
									  (float)BlueColor_numericUpDown->Value/255.f,
									  (float)AlphaColor_numericUpDown->Value/255.f);
			Vector3	l_vFinalolor(0.f,0.f,0.f);
			float	l_fScaleValue = 1.f;
			Vector2	l_vStartSize = l_pTexBehaviorDataWithImageIndexData->Size;
			float	l_fWantedSize = 0.f;//for width or height
			if( sender == Width_numericUpDown)
			{
				l_fWantedSize = (float)Width_numericUpDown->Value;
			}
			else
			if( sender == Height_numericUpDown)
			{
				l_fWantedSize = (float)Height_numericUpDown->Value;
			}
			if( l_bAeverageAssign )
			{
				if( PathNode_listBox->SelectedIndices->Count>1 )
					l_fWantedSize/=(PathNode_listBox->SelectedIndices->Count-1);
				else
					l_fWantedSize/=PathNode_listBox->SelectedIndices->Count;
			}
			//to get each step value if l_bAeverageAssign is true
			if( sender == SizeScale_numericUpDown )
			{
				l_fScaleValue = (float)SizeScale_numericUpDown->Value/100.f;
				if( l_bAeverageAssign )
				{
					if( PathNode_listBox->SelectedIndices->Count>1 )
					{
						l_fScaleValue/=(PathNode_listBox->SelectedIndices->Count-1);
					}
					else
					{
						l_fScaleValue/=PathNode_listBox->SelectedIndices->Count;
					}
				}
			}
			
			if( sender == AngleX_numericUpDown || sender == AngleY_numericUpDown || sender == AngleZ_numericUpDown )
			{
				//here is a little bit tricky,ex:the points is 6 and angle is 360,so angle for each point 360/6 = 60,
				//but the final point we won't do it,because it's end point,so we have to do as below
				//each point 360/(6-1) = 72,
				if( l_bAeverageAssign )
				{
					if( PathNode_listBox->SelectedIndices->Count>1 )
					{
						l_vAngle.x/=(PathNode_listBox->SelectedIndices->Count-1);
						l_vAngle.y/=(PathNode_listBox->SelectedIndices->Count-1);
						l_vAngle.z/=(PathNode_listBox->SelectedIndices->Count-1);
					}
					else
					{
						l_vAngle.x/=PathNode_listBox->SelectedIndices->Count;
						l_vAngle.y/=PathNode_listBox->SelectedIndices->Count;
						l_vAngle.z/=PathNode_listBox->SelectedIndices->Count;
					}
				}
			}
			if( RedColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.x / (float)(PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.y / (float)(PathNode_listBox->SelectedIndices->Count);
				}
			}
			if( GreenColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.y / (float)(PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.y / (float)(PathNode_listBox->SelectedIndices->Count);
				}
			}
			if( BlueColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.z / (float)(PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.z / (float)(PathNode_listBox->SelectedIndices->Count);
				}
			}
			if( AlphaColor_numericUpDown == sender )
			{
				if( l_bAeverageAssign )
				{
					if( PathNode_listBox->SelectedIndices->Count>1 )
						l_vFinalolor.z = l_vColor.w / (float)(PathNode_listBox->SelectedIndices->Count-1);
					else
						l_vFinalolor.z = l_vColor.w / (float)(PathNode_listBox->SelectedIndices->Count);
				}
			}
			System::Collections::ArrayList^l_pSelectedIdices = gcnew System::Collections::ArrayList();
			for(int i=0;i<PathNode_listBox->SelectedIndices->Count;++i)
			{
				int l_iSelectedIndex =  PathNode_listBox->SelectedIndices[i];
				l_pSelectedIdices->Add(l_iSelectedIndex);
				l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSelectedIndex];
				if( sender == AngleX_numericUpDown||sender == AngleY_numericUpDown||sender == AngleZ_numericUpDown )
				{
					if( l_bAeverageAssign )
					{
						l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_fStartAngle+l_vAngle.x*i;
						l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_fStartAngle+l_vAngle.y*i;
						l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_fStartAngle+l_vAngle.z*i;
					}
					else
					{
						l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_vAngle.x;
						l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_vAngle.y;
						l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_vAngle.z;
					}
				}
				Vector3	l_Pos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iSelectedIndex];
				float	l_fTime = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList()[l_iSelectedIndex];
				if( sender == PathNodeTime_numericUpDown)
					l_fTime = (float)PathNodeTime_numericUpDown->Value/1000.f;
				if( sender == PathNodePosX_numericUpDown)
					l_Pos.x = (float)PathNodePosX_numericUpDown->Value;
				if( sender == PathNodePosY_numericUpDown)
					l_Pos.y = (float)PathNodePosY_numericUpDown->Value;
				else
				if( sender == PathNodePosZ_numericUpDown)
					l_Pos.z = (float)PathNodePosZ_numericUpDown->Value;
				if( sender == Width_numericUpDown)
				{
					//float	l_fOriginaleWidth = l_pTexBehaviorDataWithImageIndexData->Size.x;
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->Size.x = l_vStartSize.x+l_fWantedSize*i;
					else
						l_pTexBehaviorDataWithImageIndexData->Size.x = l_fWantedSize;
					//float	l_fChangedWidth = l_pTexBehaviorDataWithImageIndexData->Size.x-l_fOriginaleWidth;
					//switch(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType())
					//{
					//	case eIAT_LEFT_UP:
					//	case eIAT_LEFT_CENTER:
					//	case eIAT_LEFT_DOWN:
					//		l_Pos.x -= l_fChangedWidth;
					//		break;
					//	case eIAT_CENTER_DOWN:
					//	case eIAT_CENTER_CENTER:
					//	case eIAT_CENTER_UP:
					//		break;
					//	case eIAT_RIGHT_UP:
					//	case eIAT_RIGHT_CENTER:
					//	case eIAT_RIGHT_DOWN:
					//		l_Pos.x += l_fChangedWidth;
					//		break;
					//}
				}
				if( sender == Height_numericUpDown)
				{
					//float	l_fOriginaleHeight = l_pTexBehaviorDataWithImageIndexData->Size.y;
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->Size.y = l_vStartSize.y+l_fWantedSize*i;
					else
						l_pTexBehaviorDataWithImageIndexData->Size.y = l_fWantedSize;
					//float	l_fChangedHeight = l_pTexBehaviorDataWithImageIndexData->Size.y-l_fOriginaleHeight;
					//switch(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetAnchorType())
					//{
					//	case eIAT_LEFT_UP:
					//	case eIAT_RIGHT_UP:
					//	case eIAT_CENTER_UP:
					//		l_Pos.y -= l_fChangedHeight;
					//		break;
					//	case eIAT_RIGHT_CENTER:
					//	case eIAT_LEFT_CENTER:
					//	case eIAT_CENTER_CENTER:
					//		break;
					//	case eIAT_CENTER_DOWN:
					//	case eIAT_RIGHT_DOWN:
					//	case eIAT_LEFT_DOWN:
					//		l_Pos.y += l_fChangedHeight;
					//		break;
					//}
				}
				
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(l_iSelectedIndex,l_Pos,l_fTime);
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->Refresh();
				if( RedColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.x = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.x = l_vColor.x;
				}
				if( GreenColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.y = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.y = l_vColor.y;
				}
				if( BlueColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.z = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.z = l_vColor.z;
				}
				if( AlphaColor_numericUpDown == sender )
				{
					if( l_bAeverageAssign )
						l_pTexBehaviorDataWithImageIndexData->vColor.w = (l_vFinalolor.z*i);
					else
						l_pTexBehaviorDataWithImageIndexData->vColor.w = l_vColor.w;
				}
				if( sender == SizeScale_numericUpDown )
				{
					sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData()[l_pTexBehaviorDataWithImageIndexData->iImageIndex];
					if( l_bAeverageAssign )
					{
						
						l_pTexBehaviorDataWithImageIndexData->Size = Vector2(l_pPuzzleData->Size.x*(i*l_fScaleValue),l_pPuzzleData->Size.y*(i*l_fScaleValue));
					}
					else
					{
						l_pTexBehaviorDataWithImageIndexData->Size = Vector2(l_pPuzzleData->Size.x*l_fScaleValue,l_pPuzzleData->Size.y*l_fScaleValue);
					}
				}
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(l_iSelectedIndex,l_Pos,l_fTime);
				//PathNode_listBox->Items[l_iSelectedIndex] = gcnew String( l_iSelectedIndex.ToString()+":X="+l_Pos.x.ToString()+",y="+l_Pos.y.ToString()+",Time="+l_fTime.ToString() );
			}
			WholePath_listBox->SelectedIndex = -1;
			WholePath_listBox->SelectedIndex = l_iWholePathSelectedIndec;
			while(l_pSelectedIdices->Count)
			{
				PathNode_listBox->SelectedIndices->Add(System::Int16::Parse(l_pSelectedIdices[0]->ToString()));
				l_pSelectedIdices->RemoveAt(0);
			}
		 }
		 else
		 if( WholePath_listBox->Items->Count>=1 )
		 {
			for each(int l_iSelecteIndex in WholePath_listBox->SelectedIndices)
			{
				m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(l_iSelecteIndex);//set current ccurve as our expection
				cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_pMultiPathDynamicImageAWP->GetObject(l_iSelecteIndex);
				if( StartTime_numericUpDown == sender )
				{
					l_pCueToStartCurvesWithTime->SetStartTime((float)StartTime_numericUpDown->Value/1000.f);
				}
				bool	l_bAeverageAssign = AverageAssign_checkBox->Checked;
				int	l_iNumPoint = (int)l_pCueToStartCurvesWithTime->GetOriginalPointList().size();
				float	l_fStartAngle = 0.f;
				Vector2	l_vStartSize;
				float	l_fWantedSize = 0.f;//for width or height
				if( sender == Width_numericUpDown )
					l_fWantedSize = (float)Width_numericUpDown->Value;
				else
				if( Height_numericUpDown == sender )
					l_fWantedSize = (float)Height_numericUpDown->Value;
				if( l_bAeverageAssign )
				{
					if( l_iNumPoint>1 )
						l_fWantedSize /= (l_iNumPoint-1);
					else
						l_fWantedSize /= l_iNumPoint;
				}
				for(int i=0;i<l_iNumPoint;++i)
				{
					int l_SelectedIndex =  i;
					sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pCueToStartCurvesWithTime->GetPointDataList())[l_SelectedIndex];
					if( i == 0 )
					{
						l_fStartAngle = l_pTexBehaviorDataWithImageIndexData->vAngle.z;
						l_vStartSize = l_pTexBehaviorDataWithImageIndexData->Size;
					}
					Vector3	l_vAngle((float)AngleX_numericUpDown->Value,(float)AngleY_numericUpDown->Value,(float)AngleZ_numericUpDown->Value);
					Vector4	l_vColor = Vector4((float)RedColor_numericUpDown->Value/255.f,
											  (float)GreenColor_numericUpDown->Value/255.f,
											  (float)BlueColor_numericUpDown->Value/255.f,
											  (float)AlphaColor_numericUpDown->Value)/255.f;
					Vector3	l_vFinalolor(0.f,0.f,0.f);
					float	l_fScaleValue = (float)SizeScale_numericUpDown->Value/100.f;
					if( sender == SizeScale_numericUpDown )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_fScaleValue/=(l_iNumPoint-1);
							else
								l_fScaleValue/=l_iNumPoint;
						}
					}
					if( sender == AngleX_numericUpDown ||sender == AngleY_numericUpDown ||sender == AngleZ_numericUpDown )
					{
						//here is a little bit tricky,ex:the points is 6 and angle is 360,so angle for each point 360/6 = 60,
						//but the final point we won't do it,because it's end point,so we have to do as below
						//each point 360/(6-1) = 72,
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
							{
								l_vAngle.x/=(l_iNumPoint-1);
								l_vAngle.y/=(l_iNumPoint-1);
								l_vAngle.z/=(l_iNumPoint-1);
							}
							else
							{
								l_vAngle.x/=l_iNumPoint;
								l_vAngle.y/=l_iNumPoint;
								l_vAngle.z/=l_iNumPoint;
							}
						}
					}
					if( RedColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.x / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.x / (float)(l_iNumPoint);
						}
					}
					if( GreenColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.y / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.y / (float)(l_iNumPoint);
						}
					}
					if( BlueColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.z / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.z / (float)(l_iNumPoint);
						}
					}
					if( AlphaColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
						{
							if( l_iNumPoint>1 )
								l_vFinalolor.z = l_vColor.w / (float)(l_iNumPoint-1);
							else
								l_vFinalolor.z = l_vColor.w / (float)(l_iNumPoint);
						}
					}
					if( sender == AngleX_numericUpDown||sender == AngleY_numericUpDown||sender == AngleZ_numericUpDown )
					{
						if( l_bAeverageAssign )
						{
							l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_fStartAngle+l_vAngle.x*i;
							l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_fStartAngle+l_vAngle.y*i;
							l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_fStartAngle+l_vAngle.z*i;
						}
						else
						{
							l_pTexBehaviorDataWithImageIndexData->vAngle.x = l_vAngle.x;
							l_pTexBehaviorDataWithImageIndexData->vAngle.y = l_vAngle.y;
							l_pTexBehaviorDataWithImageIndexData->vAngle.z = l_vAngle.z;
						}
					}
					Vector3	l_Pos = l_pCueToStartCurvesWithTime->GetOriginalPointList()[l_SelectedIndex];
					float	l_fTime = l_pCueToStartCurvesWithTime->GetOriginalTimeList()[l_SelectedIndex];
					if( sender == PathNodeTime_numericUpDown)
						l_fTime = (float)PathNodeTime_numericUpDown->Value/1000.f;
					if( sender == PathNodePosX_numericUpDown)
						l_Pos.x = (float)PathNodePosX_numericUpDown->Value;
					else
					if( sender == PathNodePosY_numericUpDown)
						l_Pos.y = (float)PathNodePosY_numericUpDown->Value;
					else
					if( sender == PathNodePosZ_numericUpDown)
						l_Pos.z = (float)PathNodePosZ_numericUpDown->Value;
					if( sender == Width_numericUpDown)
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->Size.x = l_vStartSize.x+l_fWantedSize*i;
						else
							l_pTexBehaviorDataWithImageIndexData->Size.x = l_fWantedSize;
							
					}
					if( sender == Height_numericUpDown)
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->Size.y = l_vStartSize.x+l_fWantedSize*i;
						else
							l_pTexBehaviorDataWithImageIndexData->Size.y = l_fWantedSize;
					}
					
					l_pCueToStartCurvesWithTime->FixPoint(l_SelectedIndex,l_Pos,l_fTime);
					l_pCueToStartCurvesWithTime->Refresh();
					if( RedColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.x = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.x = l_vColor.x;
					}
					if( GreenColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.y = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.y = l_vColor.y;
					}
					if( BlueColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.y = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.y = l_vColor.z;
					}
					if( AlphaColor_numericUpDown == sender )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->vColor.x = (l_vFinalolor.z*i);
						else
							l_pTexBehaviorDataWithImageIndexData->vColor.x = l_vColor.x;
					}
					if( sender == SizeScale_numericUpDown )
					{
						if( l_bAeverageAssign )
							l_pTexBehaviorDataWithImageIndexData->Size*=(i*l_fScaleValue);
						else
							l_pTexBehaviorDataWithImageIndexData->Size*=l_fScaleValue;
					}
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(l_SelectedIndex,l_Pos,l_fTime);
				}
			}
		}
	}
	void	MPDIEditor::ImageApply()
	{
		 if( WholePuzzleImageUnit_listBox->SelectedIndex !=-1 )
		 {
			 if(WholePath_listBox->SelectedIndex!=-1&&PathNode_listBox->SelectedIndex!=-1)
			 {
				 sPuzzleData**l_ppPuzzleData = this->m_pCurrentWorkingPI->GetPuzzleData();
				 sPuzzleData*l_pPuzzleData = l_ppPuzzleData[WholePuzzleImageUnit_listBox->SelectedIndex];
				 int	l_iStartIndex = PathNode_listBox->Items->Count;
				 int	l_iEndIndex = 0;
				 int	l_iStartImageIndex = 0;
				 int	l_iEndImageIndex = 0;
				 int	l_iCurrentImageIndex = 0;
				 //auto assign image
				 if( WholePuzzleImageUnit_listBox->SelectedIndices->Count>2 )
				 {
					WARNING_MSG("only could select 2 index");
					return;
				 }
				 for each( int l_iIndex in PathNode_listBox->SelectedIndices )
				 {
					if( l_iStartIndex > l_iIndex)
						l_iStartIndex = l_iIndex;
					if( l_iEndIndex < l_iIndex)
						l_iEndIndex = l_iIndex;
				 }
				 //assign by range
				 if( WholePuzzleImageUnit_listBox->SelectedIndices->Count == 2 )
				 {
					 l_iStartImageIndex = WholePuzzleImageUnit_listBox->SelectedIndices[0];
					 l_iEndImageIndex = WholePuzzleImageUnit_listBox->SelectedIndices[1];
					 if(l_iStartImageIndex>l_iEndImageIndex)
					 {
						l_iEndImageIndex = l_iStartImageIndex;
						l_iStartImageIndex = WholePuzzleImageUnit_listBox->SelectedIndices[1];
					 }
					 l_iCurrentImageIndex = l_iStartImageIndex;
					 for( int l_iIndex = l_iStartIndex;l_iIndex<l_iEndIndex+1;++l_iIndex )
					 {
						if( this->m_pCurrentWorkingPI->Count() <= l_iCurrentImageIndex )
							l_iCurrentImageIndex = l_iStartImageIndex;
						sTexBehaviorDataWithImageIndexData	*l_pData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
						l_pData->iImageIndex = l_iCurrentImageIndex;
						l_pData->pPI = m_pCurrentWorkingPI;
						l_pData->Size = Vector2((float)l_ppPuzzleData[l_iCurrentImageIndex]->Size.x,(float)l_ppPuzzleData[l_iCurrentImageIndex]->Size.y);
						l_iCurrentImageIndex++;
						if( l_iCurrentImageIndex > l_iEndImageIndex )
							l_iCurrentImageIndex = l_iStartImageIndex;
					 }
				 }
				 else
				 if( WholePuzzleImageUnit_listBox->SelectedIndices->Count == 1 )
				 {					 
					l_iStartImageIndex = WholePuzzleImageUnit_listBox->SelectedIndex;
					l_iCurrentImageIndex = l_iStartImageIndex;
					for each( int l_iIndex in PathNode_listBox->SelectedIndices )
					{
						sTexBehaviorDataWithImageIndexData	*l_pData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
						l_pData->pPI = m_pCurrentWorkingPI;
						l_pData->iImageIndex = l_iCurrentImageIndex;
						l_pData->Size = Vector2((float)l_ppPuzzleData[l_iCurrentImageIndex]->Size.x,(float)l_ppPuzzleData[l_iCurrentImageIndex]->Size.y);
					}
				 }
				 int	l_iLastIndex = PathNode_listBox->SelectedIndex;
				 PathNode_listBox->SelectedIndex = -1;
				 PathNode_listBox->SelectedIndex = l_iLastIndex;
			 }
		 }
	}

	void	MPDIEditor::SaveAndOpen_MPDI(System::Object^  sender, System::EventArgs^  e)
	{
		if( saveToolStripMenuItem == sender )
		{
			if( !m_pImagePlayManagerAWP )
				return;
			if( m_pMPDIList->Count() == 0 )
			{
				WARNING_MSG("please save to animation list please!");
				return;
			}
			String^l_strFileName = DNCT::SaveFileAndGetName(DNCT::ConvertExtensionToFilter(m_pImagePlayManagerAWP->ExtensionName()));
			Save(l_strFileName);
		}
		else
		{
			String^l_strFatFileName = DNCT::OpenFileAndGetName(DNCT::ConvertExtensionToFilter(cAnimationParser::ExtensionNameID));
			if( !l_strFatFileName )
			{
				return;
			}
			OpenMPDIFile(l_strFatFileName);
		}
	}

	System::Void MPDIEditor::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if(!timer1->Enabled)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
		if( !addPointModeToolStripMenuItem->Checked )//1 for fix point, 0 for add point
		{
			Vector3	l_vMouseWorldPos((float)(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(float)(int)m_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
			m_pMultiPathDynamicImageAWP->MouseMove(l_vMouseWorldPos);
			if( m_pMultiPathDynamicImageAWP->m_iFocusPoint != -1&&
				m_pMultiPathDynamicImageAWP->GetPathMode() == ePM_FIX_WITH_MOUSE_DRAG)
			{
				Vector2	l_vPos = Vector2(l_vMouseWorldPos.x,l_vMouseWorldPos.y);
				m_pMultiPathDynamicImageAWP->m_pCurrentCurve->FixPoint(m_pMultiPathDynamicImageAWP->m_iFocusPoint,l_vPos);
			}
		}
	}

	System::Void MPDIEditor::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		splitContainer3->Panel1->Focus();
		if(!timer1->Enabled||!m_pMultiPathDynamicImageAWP)
			return;	
		if( e->Button != System::Windows::Forms::MouseButtons::Left )
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
		Vector3	l_vMouseWorldPos((float)(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(float)(int)m_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
		if( addPointModeToolStripMenuItem->Checked )
		{
			if(WholePath_listBox->SelectedIndex!=-1)
			{
    			cPuzzleImage*l_pPI = this->m_pCurrentWorkingPI;
				int	l_iSelectedIndex = 0;
				if( WholePuzzleImageUnit_listBox->SelectedIndex != -1 )
					l_iSelectedIndex = l_pPI->GetObjectIndexByName(DNCT::GcStringToWchar(WholePuzzleImageUnit_listBox->Items[WholePuzzleImageUnit_listBox->SelectedIndex]->ToString()));
				POINT	l_Size = l_pPI->GetPuzzleData()[l_iSelectedIndex]->Size;
				//POINT	l_Size = l_pPI->GetPuzzleData()[l_iSelectedIndex]->OriginalSize;
				int	l_iNumPoint = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size();
				if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size() == 0 )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->AddPoint(Vector2(l_vMouseWorldPos.x,l_vMouseWorldPos.y),0.000f,Vector2((float)l_Size.x,(float)l_Size.y),Vector3::Zero,Vector4::One,l_iSelectedIndex,Mirror_checkBox->Checked,l_pPI,false);
				else
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->AddPoint(Vector2(l_vMouseWorldPos.x,l_vMouseWorldPos.y),m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetTimeList()[l_iNumPoint-1]+0.1f,Vector2((float)l_Size.x,(float)l_Size.y),Vector3::Zero,Vector4::One,l_iSelectedIndex,Mirror_checkBox->Checked,l_pPI);
				System::EventArgs^  e3;
				WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e3);
			}
		}
		else
		{
			if( moveWholePathToolStripMenuItem->Checked || rotatePathToolStripMenuItem->Checked )
			{
				m_pMultiPathDynamicImageAWP->m_SelectedNodes.clear();
				for each(int l_iIndex in PathNode_listBox->SelectedIndices)
					m_pMultiPathDynamicImageAWP->m_SelectedNodes.push_back(l_iIndex);
			}
			else
			if( sender == moveAllPathToolStripMenuItem )
			{
			
			}
			m_pMultiPathDynamicImageAWP->MouseDown(l_vMouseWorldPos);
		}
	}

	System::Void MPDIEditor::MyMouseHover(System::Object^  sender, System::EventArgs^  e)
	{
		 if(!timer1->Enabled||!m_pMultiPathDynamicImageAWP)
			return;
	}

	System::Void MPDIEditor::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	 {
		 if(!this->Visible)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));

		Vector3	l_vMouseWorldPos((float)(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(float)(int)m_pOrthogonalCamera->GetMouseWorldPos().y,0.f);
		if( m_pMultiPathDynamicImageAWP )
		{
			m_pMultiPathDynamicImageAWP->MouseUp(l_vMouseWorldPos);
			if( l_MouseButton==System::Windows::Forms::MouseButtons::Left && m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				m_bDataChanged = true;
			if( moveWholePathToolStripMenuItem->Checked || rotatePathToolStripMenuItem->Checked )
			{
				if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				{
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetOriginalPointList(m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList());
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->DoLOD();

				}
			}
			else
			if( moveAllPathToolStripMenuItem->Checked || rotateAllPathToolStripMenuItem->Checked  )
			{
				if( m_pMultiPathDynamicImageAWP )
				{
					int	l_i = m_pMultiPathDynamicImageAWP->Count();
					for( int i=0;i<l_i;++i )
					{
						(*m_pMultiPathDynamicImageAWP)[i]->SetOriginalPointList((*m_pMultiPathDynamicImageAWP)[i]->GetOriginalPointList());
						(*m_pMultiPathDynamicImageAWP)[i]->DoLOD();
					}
				}
			}
		}
	 }

	void	MPDIEditor::PathMoveModifyMode(System::Object^  sender, System::EventArgs^  e)
	{
		if( !m_pMultiPathDynamicImageAWP )
			return;
		 fixPointModeToolStripMenuItem->Checked= false;
		 addPointModeToolStripMenuItem->Checked = false;
		 moveWholePathToolStripMenuItem->Checked = false;
		 moveAllPathToolStripMenuItem->Checked = false;
		 rotatePathToolStripMenuItem->Checked = false;
		 rotateAllPathToolStripMenuItem->Checked = false;
		 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
		 l_pSender->Checked = true;
		 if( sender == fixPointModeToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_FIX);
		 }
		 else
		 if( sender == moveWholePathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_PATH_MOVING);
		 }
		 else
		 if( sender == moveAllPathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_PATH_MOVING_ALL);
			m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.clear();
			for each (int l_iSelectedIndex in WholePath_listBox->SelectedIndices)//haha......fucking sux here
			{
				m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.push_back(l_iSelectedIndex);
			}
		 }
		 else
		 if( sender == rotatePathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_PATH_ROTATION);
		 }
		 else
		 if( sender == rotateAllPathToolStripMenuItem )
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_ALL_PATH_ROTATION);
			m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.clear();
			for each (int l_iSelectedIndex in WholePath_listBox->SelectedIndices)//haha......fucking sux here
			{
				m_pMultiPathDynamicImageAWP->m_SelectedIndicesToMovingOrRotating.push_back(l_iSelectedIndex);
			}
		 }
		 else
		 {
			m_pMultiPathDynamicImageAWP->SetPathMode(ePM_EDIT);
		 }
	}

	void	MPDIEditor::PathModifyFunction(System::Object^  sender, System::EventArgs^  e)
	{
		 if( sender == AddAnimationToList_button )
		 {
			 if( !AnimationName_textBox->Text->Length )
				 return;
			AddIntoList();
		 }
		 else
		 if( WholeAnimationList_listBox->SelectedIndex != -1 )
		 {
			 if( sender == DeleteListObject_button )
			 {
				 String^l_strName = DNCT::WcharToGcstring(m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex)->GetName());
				 m_pMPDIList->RemoveObject(WholeAnimationList_listBox->SelectedIndex);
				 WholeAnimationList_listBox->Items->RemoveAt(WholeAnimationList_listBox->SelectedIndex);
				 if(this->m_pMPDIExtraData->ContainsKey(l_strName))
				 {
					 m_pMPDIExtraData->Remove(l_strName);
				 }
				 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
				 Save(m_strCurrentSaveFileName);
			 }
			 else
			 if( sender == AnimationInvert_button )
			 {
				 ePathMode	l_ePathMode = m_pMultiPathDynamicImageAWP->GetPathMode();
				 if( WholeAnimationList_listBox->SelectedIndex != -1)
				 {
					 m_bPreventChangeAllTimeValueSetup = true;
					 SAFE_DELETE(m_pMultiPathDynamicImageAWP);
					 if( m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex)->Count()>0 )
					 {
						 TotalPlayTime_numericUpDown->Value = (int)(m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex)->GetObject(0)->GetLastTime()*1000);
					 }
					 m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex));
					 Vector3 scale; Vector3 rotation; Vector3 translation; float inverted;
					 CameraRotationX_numericUpDown->Value = (int)0;
					 CameraRotationY_numericUpDown->Value = (int)0;
					 CameraRotationZ_numericUpDown->Value = (int)0;
					 CameraRotationCenterX_numericUpDown->Value = (int)0;
					 CameraRotationCenterY_numericUpDown->Value = (int)0;
					 CameraRotationCenterZ_numericUpDown->Value = (int)0;
					 if( m_pMultiPathDynamicImageAWP->GetCameraView() )
					 {
						 m_pMultiPathDynamicImageAWP->GetCameraView()->Decompose(scale,rotation,translation,inverted);
						 rotation = Vector3(D3DXToDegree(rotation.x),D3DXToDegree(rotation.y),D3DXToDegree(rotation.z));
						 if( rotation.x-(int)rotation.x>0.f ) rotation.x = (int)rotation.x+1.f;
						 if( rotation.y-(int)rotation.y>0.f ) rotation.y = (int)rotation.y+1.f;
						 if( rotation.z-(int)rotation.z>0.f ) rotation.z = (int)rotation.z+1.f;
						 CameraRotationX_numericUpDown->Value = (int)rotation.x;
						 CameraRotationY_numericUpDown->Value = (int)rotation.y;
						 CameraRotationZ_numericUpDown->Value = (int)rotation.z;
						 CameraRotationCenterX_numericUpDown->Value = (int)translation.x;
						 CameraRotationCenterY_numericUpDown->Value = (int)translation.y;
						 CameraRotationCenterZ_numericUpDown->Value = (int)translation.z;
					 }

					 WholePath_listBox->Items->Clear();
					 for(int i=0;i<m_pMultiPathDynamicImageAWP->Count();++i)
					 {
						cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*m_pMultiPathDynamicImageAWP)[i];
						WholePath_listBox->Items->Add(DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName()));
						if( i == 0 )
							Loop_checkBox->Checked = l_pCueToStartCurvesWithTime->IsCurveLoop();
					 }
					 if( WholePath_listBox->Items->Count )
						WholePath_listBox->SelectedIndex = 0;
					 AllCurveLoop_checkBox->Checked = m_pMultiPathDynamicImageAWP->IsLoop();
					 AllPlay_checkBox->Checked = true;
					 if( m_pMultiPathDynamicImageAWP->Count() )
						 this->Loop_checkBox->Checked = m_pMultiPathDynamicImageAWP->GetObject(WholePath_listBox->SelectedIndex)->IsAnimationLoop();
					 Play_checkBox_CheckedChanged(AllPlay_checkBox,e);

					 MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Clear();
					 String^l_strSelectedMPDIName = DNCT::WcharToGcstring(m_pMultiPathDynamicImageAWP->GetName());
					 if(m_pMPDIExtraData->ContainsKey(l_strSelectedMPDIName))
					 {
						GCFORM::TreeNode^l_pExtractDataNode = (GCFORM::TreeNode^)m_pMPDIExtraData[l_strSelectedMPDIName];
						MPDIExtra_xmlNodeEditComponent->CurrentAttribute_treeView->Nodes->Add(l_pExtractDataNode);
					 }					 
					switch(l_ePathMode)
					{
						case ePM_EDIT:
							PathMoveModifyMode(addPointModeToolStripMenuItem,e);
							break;
						case ePM_FIX:
							PathMoveModifyMode(fixPointModeToolStripMenuItem,e);
							break;
						case ePM_PATH_MOVING:
							PathMoveModifyMode(moveWholePathToolStripMenuItem,e);
							break;
						case ePM_PATH_MOVING_ALL:
							PathMoveModifyMode(moveAllPathToolStripMenuItem,e);
							break;
						case ePM_PATH_ROTATION:
							PathMoveModifyMode(rotatePathToolStripMenuItem,e);
							break;
						case ePM_ALL_PATH_ROTATION:
							PathMoveModifyMode(rotateAllPathToolStripMenuItem,e);
							break;
					}
					 m_bPreventChangeAllTimeValueSetup = false;
					 WholePuzzleImageUnit_listBox_SelectedIndexChanged(sender,e);
				 }
			 }
		 }	
	}

	 void	MPDIEditor::ReferencePathModify(System::Object^  sender, System::EventArgs^  e)
	 {
		 if( !m_pReferenceCurveManagerAWP )
		 {
			m_pReferenceCurveManagerAWP = new cCurveManager;
		 }
		 if( !m_pMultiPathDynamicImageAWP&&!m_pMPDIList )
			 return;
		 if( sender == UsingIntoCurrentPath_button )
		 {
			 if( AllPath_listBox->SelectedIndex != -1 )
			 {
				AddPath(AddPath_button,e);
				cCurveWithTime*l_pCurvesWithTime = m_pReferenceCurveManagerAWP->GetObject(AllPath_listBox->SelectedIndex);
				if(WholePath_listBox->SelectedIndex!=-1)
				{
					cCurveWithTime*l_pDestCurvesWithTime =	m_pMultiPathDynamicImageAWP->GetObject(m_pMultiPathDynamicImageAWP->Count()-1);
					l_pDestCurvesWithTime->SetOriginalPointList(l_pCurvesWithTime->GetOriginalPointList());
					l_pDestCurvesWithTime->SetOriginalTimeList(l_pCurvesWithTime->GetOriginalTimeList());
					l_pDestCurvesWithTime->SetPointList(l_pCurvesWithTime->GetPointList());
					l_pDestCurvesWithTime->SetTimeList(l_pCurvesWithTime->GetTimeList());
					l_pDestCurvesWithTime->SetLOD(l_pCurvesWithTime->GetLOD());
					l_pDestCurvesWithTime->SetCurveLoop(l_pCurvesWithTime->IsCurveLoop());

					POINT	l_Size = m_pMultiPathDynamicImageAWP->GetPuzzleData()[0]->Size;
					UINT	l_iuNumPoint = l_pDestCurvesWithTime->GetOriginalPointList().size();
					cPuzzleImage*l_pPI = m_pInterlUseForShowImagePos->GetPuzzleImage(DNCT::GcStringToWchar(AllPI_listBox->SelectedItem->ToString()));
					for(UINT i=0;i<l_iuNumPoint;++i)
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->AddPoint(l_pDestCurvesWithTime->GetOriginalPointList()[i],l_pCurvesWithTime->GetOriginalTimeList()[i],
						Vector2((float)l_Size.x,(float)l_Size.y),Vector3::Zero,Vector4::One,0,Mirror_checkBox->Checked,l_pPI);
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->DoLOD();
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetAnimationLoop(l_pCurvesWithTime->IsCurveLoop());
					WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e);
				}
			 }
		 }
		 else
		 if( sender == AddPathIntoAllPath_button )
		 {
			 if( WholePath_listBox->SelectedIndex !=-1 )
			 {
				FMCEditor::InputNameDialog^l_pForm = gcnew FMCEditor::InputNameDialog;
				l_pForm->SetupTextBoxData(WholePath_listBox->SelectedItem->ToString());
				l_pForm->ShowDialog();
				if( strlen(l_pForm->m_strTextName->c_str()) )
				{
					String^l_strName = gcnew String(l_pForm->m_strTextName->c_str());
					if( AllPath_listBox->Items->Contains(l_strName) )
					{
						WARING_YES_NO_TO_NO("replace original?覆蓋原本物件?")
						{
							return;
						}
					}
					else
						AllPath_listBox->Items->Add(l_strName);
					cCurveWithTime*l_pNewCurvesWithTime = new cCurveWithTime();
					l_pNewCurvesWithTime->SetName(UT::CharToWchar(l_pForm->m_strTextName->c_str()));
					cCurveWithTime*l_pSrcCurveWithTime =	m_pMultiPathDynamicImageAWP->GetObject(WholePath_listBox->SelectedIndex);
					l_pNewCurvesWithTime->SetOriginalPointList(l_pSrcCurveWithTime->GetOriginalPointList());
					l_pNewCurvesWithTime->SetOriginalTimeList(l_pSrcCurveWithTime->GetOriginalTimeList());
					l_pNewCurvesWithTime->SetPointList(l_pSrcCurveWithTime->GetPointList());
					l_pNewCurvesWithTime->SetTimeList(l_pSrcCurveWithTime->GetTimeList());
					l_pNewCurvesWithTime->SetLOD(l_pSrcCurveWithTime->GetLOD());
					l_pNewCurvesWithTime->SetCurveLoop(l_pSrcCurveWithTime->IsCurveLoop());
					m_pReferenceCurveManagerAWP->AddObject(l_pNewCurvesWithTime);	
				}
				delete l_pForm;
			 }
		 }
		 else
		 if( sender == DelPathIntoAllPath_button )
		 {
			 if( AllPath_listBox->SelectedIndex == -1)
				 return;
			 m_pReferenceCurveManagerAWP->RemoveObject(AllPath_listBox->SelectedIndex);
			 AllPath_listBox->Items->RemoveAt(AllPath_listBox->SelectedIndex);
		 }
	 }

	System::Void MPDIEditor::EventPoint(System::Object^  sender, System::EventArgs^  e)
	{
		 std::vector<sHintPoint>*l_pVectorPointer = 0;
		 if( WholePath_listBox->SelectedIndex!=-1 )
		 {
			cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = m_pMultiPathDynamicImageAWP->GetObject(WholePath_listBox->SelectedIndex);
			l_pVectorPointer = l_pCueToStartCurvesWithTime->GetHintPointVector();
		 }
		 if( sender == AddEventPOint_button )
		 {
			 if( PathNode_listBox->SelectedIndex != -1 )
			 {
				 eAnimationEvent	l_eIndex = IntToAnimationEvent(HintPointType_comboBox->SelectedIndex);
				 if( l_eIndex == eAE_MAX )
				 {
					 WARNING_MSG("please select type!!");
					 return;
				 }
				 WCHAR*l_strText = L"__empty";
				 if( l_eIndex != eAE_NONE )
				 {
					if(EventPointName_textBox->Text->Length == 0)
					{
						WARNING_MSG("please input the name for hint point!!");
						return;
					}
					l_strText = DNCT::GcStringToWchar(EventPointName_textBox->Text);
				 }
				 sHintPoint l_HintPoint(PathNode_listBox->SelectedIndex,l_strText,l_eIndex);
				 l_pVectorPointer->push_back(l_HintPoint);
				 AllEventPointHint_listBox->Items->Add(PathNode_listBox->SelectedIndex.ToString()+":"+HintPointType_comboBox->SelectedItem->ToString()+","+EventPointName_textBox->Text);
			 }
		 }
		 else
		 if( sender == DelEventPoint_button )
		 {
			 if( l_pVectorPointer&&AllEventPointHint_listBox->SelectedIndex != -1  )
			 {
				 l_pVectorPointer->erase(l_pVectorPointer->begin()+AllEventPointHint_listBox->SelectedIndex);
				 AllEventPointHint_listBox->Items->RemoveAt(AllEventPointHint_listBox->SelectedIndex);
			 }
		 }
	}
	System::Void MPDIEditor::AddGameUI(System::Object^  sender)
	{
		#define	TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND			L"BG"				//
		#define	TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT		L"SliderObject"		//each object's position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_MULTIPAGE_OBJECT	L"MultiPageObject"	//each object's position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON		L"CB"				//
		#define	TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE			L"PriorPage"		//proio page button position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE			L"NextPage"			//next page button position
		#define	TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS	L"IconObjectPos"	//icon object's position
		if( sender == AddSliderBarUI_toolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation");
		}
		else
		if( sender == AddMultiPageUI_toolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation\nPriorPage for prior page button NextPage for next page button\nIconObjectPos for tabPages's category icon");
		}
		else
		if( sender == addMultipageWithTabPagesUIToolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_PRIOR_PAGE).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_NEXT_PAGE).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation\nPriorPage for prior page button NextPage for next page button");
		}
		else
		if( sender == addSliderBarWithTabPagesUIToolStripMenuItem )
		{
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_CANCEL_BUTTON).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_BACKGROUND).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_SLIDER_OBJECT).ToString());
			AddPath(String(TABPAGE_CONTROLLER_DATA_MPDI_ICON_OBJECT_POS).ToString());
			WARNING_MSG("BG for background,CB for cancel button,SliderObject for SliderPosition and orientation\nIconObjectPos for tabPages's category icon");
		}
		else
		if( sender == yesNoDialogToolStripMenuItem )
		{
			#define		YES_NO_DIALOG_YES_BUTTON	"Yes"
			#define		YES_NO_DIALOG_NO_BUTTON		"No"
			#define		YES_NO_DIALOG_BG			"BG"
			#define		YES_NO_DIALOG_FONT			"Font"
			AddPath(String(YES_NO_DIALOG_YES_BUTTON).ToString());
			AddPath(String(YES_NO_DIALOG_NO_BUTTON).ToString());
			AddPath(String(YES_NO_DIALOG_BG).ToString());
			AddPath(String(YES_NO_DIALOG_FONT).ToString());
			WARNING_MSG("BG for background,Yes and No for button,font for description show position");
		}
	}

	void	MPDIEditor::AddPIFile(String^e_strFileName)
	{
		WCHAR*l_strName = DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(e_strFileName));
		if(m_pImageResourceManagerAWP->GetObject(l_strName))
		{
			WARNING_MSG("this pi's name has already added!");
			return;
		}
		bool	l_b = m_pImageResourceManagerAWP->Parse(DNCT::GcStringToChar(e_strFileName));
		if( l_b )
		{
			cPuzzleImage*l_pPuzzleImageAWP = dynamic_cast<cPuzzleImage*>(m_pImageResourceManagerAWP->GetObject(m_pImageResourceManagerAWP->Count()-1));
			m_pMPDIList->SetPuzzleImage(l_pPuzzleImageAWP);
			AllPI_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetName()));
		}
		else
		{
		
		}		   
	}

	void	MPDIEditor::OpenPIFile(String^e_strFileName)
	{
		WholeAnimationList_listBox->Items->Clear();
		SAFE_DELETE(m_pImagePlayManagerAWP);
		SAFE_DELETE(m_pInterlUseForShowImagePos);
		ShowImageOnScreen_listBox->Items->Clear();
		AllPI_listBox->Items->Clear();
		WholePath_listBox->Items->Clear();
		PathNode_listBox->Items->Clear();
		WholePuzzleImageUnit_listBox->Items->Clear();
		AddPath_button->Enabled = false;
		m_pImagePlayManagerAWP = new cAnimationParser();
		m_pImageResourceManagerAWP = m_pImagePlayManagerAWP->GetAllBaseImageList();
		bool	l_b = m_pImageResourceManagerAWP->Parse(DNCT::GcStringToChar(e_strFileName));
		if( l_b )
		{
			if( !m_pImageResourceManagerAWP->Count() )
			{
				return;
			}
			cPuzzleImage*l_pPuzzleImageAWP = 0;
			m_pMPDIList = new cMPDIList();
			m_pInterlUseForShowImagePos = new cCueToStartCurveWithTime(m_pMPDIList);
			m_pImagePlayManagerAWP->AddObject(m_pMPDIList);
			l_pPuzzleImageAWP = dynamic_cast<cPuzzleImage*>(m_pImageResourceManagerAWP->GetObject(0));
			l_pPuzzleImageAWP->GenerateAllPuzzleImageUnit();

			m_pMPDIList->SetPuzzleImage(l_pPuzzleImageAWP);

			m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList);

			for( int i=0;i<l_pPuzzleImageAWP->Count();++i )
			{
				WholePuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetObject(i)->GetName()));
			}
			AddPath_button->Enabled = true;
			Play_checkBox->Enabled = true;
			AllPlay_checkBox->Enabled = true;
			Loop_checkBox->Enabled = true;
			AllCurveLoop_checkBox->Enabled = true;
			AllPI_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetName()));
		}
		AssignLatestPI();	   
	}

	void	MPDIEditor::Save(String^e_strFileName)
	{
		if( !m_pImagePlayManagerAWP )
			return;
		if( m_pMPDIList->Count() == 0 )
		{
			WARNING_MSG("please save to animation list please!");
			return;
		}
		 String^l_strFileName = e_strFileName;
		 l_strFileName = ForceAddExtenName(l_strFileName,(char*)m_pImagePlayManagerAWP->ExtensionName());
		 if( l_strFileName )
		 {
			 m_strCurrentSaveFileName = l_strFileName;
			 m_pMPDIList->SetName(DNCT::GcStringToWchar(DNCT::GetFileNameWithoutFullPath(l_strFileName,true)));
			 m_pImagePlayManagerAWP->Export(DNCT::GcStringToChar(l_strFileName));
			 if(this->m_pMPDIExtraData->Count)
			 {
				 GCFORM::TreeNode^l_pNodes = gcnew GCFORM::TreeNode(DNCT::WcharToGcstring(m_pMPDIList->GetName()));
				 for each ( GCFORM::TreeNode^l_pTargetNode in m_pMPDIExtraData->Values )
				 {
					 l_pNodes->Nodes->Add((GCFORM::TreeNode^)l_pTargetNode->Clone());
				 }
				 DNCT::SaveTreeNodeToFile(l_pNodes,l_strFileName+"_extra");
			 }
			 this->m_pForm->Text = l_strFileName+"SaveTime:"+System::DateTime::Now.ToString();
			 m_bDataChanged = false;
//				 FileToUnicode(l_strFileName);	
		 }
	}

	void	MPDIEditor::Merge(String^e_strFileName)
    {
		 if( !e_strFileName )
			 return;
		 int	l_iCount = m_pImagePlayManagerAWP->Count();
		 if( !m_pImagePlayManagerAWP || m_pImagePlayManagerAWP->Count() == 0 )
			 return;
		 WCHAR*l_strMPDIListName = cMPDIList::FileToMPDIListName(DNCT::GcStringToWchar(e_strFileName));
		 if(!m_pImagePlayManagerAWP->GetObject(l_strMPDIListName))
		 {
			 WholeAnimationList_listBox->Items->Clear();
			 AllPI_listBox->Items->Clear();
			 char*	l_str = DNCT::GcStringToChar(e_strFileName);
			 if(m_pImagePlayManagerAWP->Parse(l_str))
			 {
				 cMPDIList*l_pOriginaleMPDIList = m_pMPDIList;
				 cMPDIList*l_pMPDIList = m_pImagePlayManagerAWP->GetMPDIList(l_strMPDIListName);
				 l_pOriginaleMPDIList->Merge(l_pMPDIList);
				 l_iCount = m_pImagePlayManagerAWP->Count();

				 for( int i=0;i<m_pMPDIList->Count();++i )
				 {
					cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*m_pMPDIList)[i];
					WholeAnimationList_listBox->Items->Add(DNCT::WcharToGcstring(l_pMultiPathDynamicImage->GetName()));
				 }
				 for(int i=0;i<m_pMPDIList->GetPIList()->Count();++i)
					AllPI_listBox->Items->Add(DNCT::WcharToGcstring((*m_pMPDIList->GetPIList())[i]->GetName()));
				 m_pImagePlayManagerAWP->RemoveObject(l_pMPDIList->GetName());
				 int	l_iCount = m_pImagePlayManagerAWP->Count();
			 }
		 }
		 else
		 {
			WARNING_MSG("this mpdi already exists");
		 }
    }


	void	MPDIEditor::OnlyRenderWindow()
	{
		this->menuStrip1->Visible = !this->menuStrip1->Visible;
		this->splitContainer1->Panel1Collapsed = !this->splitContainer1->Panel1Collapsed;
		this->splitContainer2->Panel1Collapsed = !this->splitContainer2->Panel1Collapsed;
	}

	void	MPDIEditor::OpenMPDIFile(String^e_strFileName)
	{
		m_pMPDIList = 0;
		SAFE_DELETE(m_pInterlUseForShowImagePos);
		SAFE_DELETE(m_pImagePlayManagerAWP);
		SAFE_DELETE(m_pMultiPathDynamicImageAWP);
		this->m_pMPDIExtraData->Clear();
		m_pImagePlayManagerAWP = new cAnimationParser();
		WholePuzzleImageUnit_listBox->Items->Clear();
		WholeAnimationList_listBox->Items->Clear();
		WholePath_listBox->Items->Clear();
		AllPI_listBox->Items->Clear();
		ShowImageOnScreen_listBox->Items->Clear();
		AllEventPointHint_listBox->Items->Clear();
		PathNode_listBox->Items->Clear();
		System::Object^  l_pSender;
		System::EventArgs^l_pEventArgsE;
		WholePath_listBox_SelectedIndexChanged(l_pSender,l_pEventArgsE);
		char*	l_strFileName = DNCT::GcStringToChar(e_strFileName);
		m_pImagePlayManagerAWP->m_bShowErrorMsg = showErrorMsh_ToolStripMenuItem->Checked;
		bool	l_b = m_pImagePlayManagerAWP->Parse(l_strFileName);
		if( l_b )
		{
			CameraRotationX_numericUpDown->Value = 0;
			CameraRotationY_numericUpDown->Value = 0;
			CameraRotationZ_numericUpDown->Value = 0;
			m_strCurrentSaveFileName = e_strFileName;
			if( m_pImagePlayManagerAWP->Count() == 0 )
				return;
			if( System::IO::File::Exists(e_strFileName+"_extra") )
			{
				GCFORM::TreeNode^l_pRootNode = OpenXmlFileToNode(e_strFileName+"_extra",false);
				if( l_pRootNode->Nodes->Count )
				{
					int	l_iCount = l_pRootNode->Nodes->Count;
					for each(GCFORM::TreeNode^l_pMPDIDataNode in l_pRootNode->Nodes)
					{
						String^l_strKey = l_pMPDIDataNode->Text;
						m_pMPDIExtraData[l_strKey] = l_pMPDIDataNode;
					}
				}
			}
			m_pMPDIList = dynamic_cast<cMPDIList*>((*m_pImagePlayManagerAWP)[0]);
			m_pMultiPathDynamicImageAWP = new cMPDIForEditor(m_pMPDIList);
			m_pInterlUseForShowImagePos = new cCueToStartCurveWithTime(m_pMPDIList);
			m_strCurrentFileName = DNCT::WcharToGcstring(m_pMPDIList->GetName());
			m_pImageResourceManagerAWP = m_pImagePlayManagerAWP->GetAllBaseImageList();
			for( int i=0;i<m_pMPDIList->Count();++i )
			{
				cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*m_pMPDIList)[i];
				WholeAnimationList_listBox->Items->Add(DNCT::WcharToGcstring(l_pMultiPathDynamicImage->GetName()));
			}
			for(int i=0;i<m_pMPDIList->GetPIList()->Count();++i)
				AllPI_listBox->Items->Add(DNCT::WcharToGcstring((*m_pMPDIList->GetPIList())[i]->GetName()));
			AllPI_listBox->SelectedIndex = 0;
			sPuzzleData**l_ppPuzzleData = m_pMPDIList->GetPuzzleData();
			if( l_ppPuzzleData )
			{
				for( int i=0;i<m_pMPDIList->GetNumImage();++i )
				{
					WholePuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_ppPuzzleData[i]->strFileName));
				}
			}
			AddPath_button->Enabled = true;
			Play_checkBox->Enabled = true;
			AllPlay_checkBox->Enabled = true;
			Loop_checkBox->Enabled = true;
			AllCurveLoop_checkBox->Enabled = true;
			m_bDataChanged = false;
			AssignLatestPI();
			if( showErrorMsh_ToolStripMenuItem->Checked )
			{
				String^l_strErrorMsg = MPDIListDataCheck(m_pMPDIList);
				if( l_strErrorMsg->Length )
				{
					WARNING_MSG(l_strErrorMsg);
				}
			}
			this->m_pForm->Text = e_strFileName+"   :OpenTime:"+System::DateTime::Now.ToString();
		}
		else
		{
			WARNING_MSG("file format error");
		}		   
	}
	System::Void MPDIEditor::OpenImageFile_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				String^l_strFileName =  DNCT::OpenFileAndGetName("(*.pi)|*.pi");
				if( !l_strFileName )
				{
					return;
				}
				//ensure this is add or call first time add
				if( sender == AddPI_toolStripMenuItem && m_pImageResourceManagerAWP && m_pImageResourceManagerAWP->Count() >= 1 )
				{
					AddPIFile(l_strFileName);
				}
				else
				//first time add
				{
					OpenPIFile(l_strFileName);
				 }
				AssignLatestPI();
			 }

	System::Void MPDIEditor::BKImage_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 String^l_strFileName =  DNCT::OpenFileAndGetName("(*.png)|*.png");
				 if( l_strFileName )
				 {
					SAFE_DELETE(m_pBKImageAWP);
					m_pBKImageAWP = new cBaseImage(DNCT::GcStringToChar(l_strFileName));
				 }
			 }

	System::Void MPDIEditor::AddPath_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				 m_bDataChanged = true;
				 AddPath(sender,e);
				this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::WholePath_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( WholePath_listBox->SelectedItems->Count != 0 )
					PathSelectedIndexChanged(sender,e);
			 }
	System::Void MPDIEditor::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				if(!this->Visible)
					 return;
				DEFAULT_SHADER = L"MPDIEditorShader";
				NO_TEXTURE_SHADER = L"MPDIEditor_NoTextureShader";
				UseShaderProgram();
				if(g_bResolutionChanged)
				{
					g_bResolutionChanged = false;
					if( splitContainer2->Width>DRAW_PANEL_RESOLUTION_WIDTH&&splitContainer1->Height>DRAW_PANEL_RESOLUTION_HEIGHT )
					{
						//width
						splitContainer2->SplitterDistance = splitContainer2->Width-(int)DRAW_PANEL_RESOLUTION_WIDTH;
						//height
						splitContainer1->SplitterDistance = splitContainer1->Height-(int)DRAW_PANEL_RESOLUTION_HEIGHT;
					}
					else
					{
						float   l_fNewWidth = DRAW_PANEL_RESOLUTION_WIDTH/2;
						float   l_fNewHeight = DRAW_PANEL_RESOLUTION_HEIGHT/2;
						//width
						splitContainer2->SplitterDistance = splitContainer2->Width-(int)l_fNewWidth;
						//height
						splitContainer1->SplitterDistance = splitContainer1->Height-(int)l_fNewHeight;
					}
				}
				Render();
				m_pTimeCounterForAutoSaveFile->Update(m_pTimeAndFPS->fElpaseTime);
				//for tempo file if the editor occur crash...
				if( m_pTimeCounterForAutoSaveFile->bTragetTimrReached )
				{
					if( m_pMultiPathDynamicImageAWP&&m_pMPDIList&&m_pImagePlayManagerAWP )
					if( m_pMPDIList->Count() )
					{
						//this->timer1->Enabled = false;
						//String^l_strFileName = "_temp.mpdi";
						//m_pMultiPathDynamicImageAWP->SetName(L"_temp");
						//m_pMPDIList->AddObjectNeglectExist(m_pMultiPathDynamicImageAWP);
						//m_pMPDIList->SetName(DNCT::GcStringToWchar(l_strFileName));
						//m_pImagePlayManagerAWP->Export(DNCT::GcStringToChar(l_strFileName));
						//m_pMPDIList->RemoveObjectWithoutDelete(m_pMPDIList->GetObjectIndexByPointer(m_pMultiPathDynamicImageAWP));
						//this->timer1->Enabled = true;
					}
					m_pTimeCounterForAutoSaveFile->Start();
				}
			 }

	System::Void MPDIEditor::tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if(this->Visible)
				{
					wglMakeCurrent( m_HdcMV,m_HGLRCMV );
					glViewport(0,0,splitContainer3->Panel1->Width,splitContainer3->Panel1->Height);
					timer1->Enabled = true;
				}
				else
				{
					timer1->Enabled = false;
				}
			 }
	System::Void MPDIEditor::PathLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
				if(WholePath_listBox->SelectedIndex != -1)
				{
					m_pMultiPathDynamicImageAWP->GetObject(WholePath_listBox->SelectedIndex)->SetLOD((int)PathLOD_numericUpDown->Value);
					System::EventArgs^  e3;
					WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e3);
				}
			 }
	System::Void MPDIEditor::DelPoint_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				DelPoint(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::AssignImage_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				ImageApply();
				this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::AlphaColor_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				Play_checkBox->Checked = false;
				if( !m_bPreventChangeValue )
					AssignValueChange(sender);
				this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::Play_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				PlayCheckedChanged(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::TotalPlayTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_bPreventChangeAllTimeValueSetup )
					 return;
				 if( WholePath_listBox->SelectedIndex != -1 )
				 {
					if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve)
					{
						std::vector<float> l_fTimeList = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalTimeList();
						int	l_iNum = l_fTimeList.size();
						if(l_iNum == 1)//avoid only one point,we still have to setup data
						{
							l_iNum = 2;
							float	l_fTimeGap = (float)TotalPlayTime_numericUpDown->Value/(float)(l_iNum-1)/1000.f;
							l_fTimeList[0] = l_fTimeGap;
						}
						else
						{
							float	l_fTimeGap = (float)TotalPlayTime_numericUpDown->Value/(float)(l_iNum-1)/1000.f;
							for( int i=0;i<l_iNum;++i )
								l_fTimeList[i] = i*l_fTimeGap;
						}
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetOriginalTimeList(l_fTimeList);
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->Refresh();
						int	l_iIndex = WholePath_listBox->SelectedIndex;
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
			 }

	System::Void MPDIEditor::AddAnimationToList_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				PathModifyFunction(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				SaveAndOpen_MPDI(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::CurrentProgress_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
				m_fTimeForCurrentProgess = (float)CurrentProgress_trackBar->Value/1000.f;

			 }
	System::Void MPDIEditor::ApplyLODToPoints_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 if( WholePath_listBox->SelectedIndex != -1 )
				 {
					if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					{
						m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetOriginalToFinal();
						int	l_iIndex = WholePath_listBox->SelectedIndex;
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = l_iIndex;
					}
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::BehaviorUpdateActive_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetBehaviorUpdateActive( BehaviorUpdateActive_checkBox->Checked );
			 }

	System::Void MPDIEditor::AddShowImageOnScreenObject_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				if( sender == DelShowImageOnScreenObject_button )
				{
					System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
					l_MyList->AddRange(ShowImageOnScreen_listBox->SelectedItems);
					for each (System::Object^l_pObject in l_MyList)
					{
						int	l_iIndex = ShowImageOnScreen_listBox->Items->IndexOf(l_pObject);
						m_pInterlUseForShowImagePos->DelPoint(l_iIndex);
						ShowImageOnScreen_listBox->Items->RemoveAt(l_iIndex);
					}
				}
				else
				{
					if( WholePath_listBox->SelectedItem )
					if( PathNode_listBox->SelectedItem )
					{
						for each( int l_iIndex in PathNode_listBox->SelectedIndices )
						{
							if( sender == AddShowImageOnScreenObject_button )
							{
								sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iIndex];
								Vector3	l_vPos = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetOriginalPointList()[l_iIndex];
								m_pInterlUseForShowImagePos->AddPoint(l_vPos,0,l_pTexBehaviorDataWithImageIndexData->Size,l_pTexBehaviorDataWithImageIndexData->vAngle,l_pTexBehaviorDataWithImageIndexData->vColor,l_pTexBehaviorDataWithImageIndexData->iImageIndex,l_pTexBehaviorDataWithImageIndexData->bMirror,l_pTexBehaviorDataWithImageIndexData->pPI);
								ShowImageOnScreen_listBox->Items->Add(WholePath_listBox->SelectedItem->ToString()+":"+l_iIndex.ToString());
								m_pInterlUseForShowImagePos->SetAnimationDone(false);
								m_pInterlUseForShowImagePos->SetStart(true);
							}
						}
					}
				}
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::WholePuzzleImageUnit_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 if( WholePuzzleImageUnit_listBox->SelectedIndex != -1 )
				 {
					 SAFE_DELETE(m_pMultiPathDynamicImageAWP->m_pBasemageForShow);
					 if( m_pCurrentWorkingPI )
					 {					 
						 m_pMultiPathDynamicImageAWP->m_pBasemageForShow = new cBaseImage(m_pCurrentWorkingPI->GetObject(WholePuzzleImageUnit_listBox->SelectedIndex));
						 m_pMultiPathDynamicImageAWP->m_bBaseImageFromResource = false;
					 }
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::addPointModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				PathMoveModifyMode(sender,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::PathPiorityUp_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 ListBoxSelectedItemWithObjectListMove<cMultiPathDynamicImage,cMultiPathDynamicImage>(sender == PathPiorityDown_button?false:true,WholePath_listBox,m_pMultiPathDynamicImageAWP,0);
				 this->timer1->Enabled = true;
			 }

	System::Void MPDIEditor::Mirror_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( WholePath_listBox->SelectedIndex != -1 && WholePath_listBox->SelectedIndices->Count == 1 )
				{
					if(PathNode_listBox->SelectedIndex == -1)
						return;
					int	l_iSmallestIndex = PathNode_listBox->Items->Count;
					for each ( int l_SelectedIndex in PathNode_listBox->SelectedIndices )
						if(l_iSmallestIndex>l_SelectedIndex)
							l_iSmallestIndex = l_SelectedIndex;
					sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_iSmallestIndex];
					m_pMultiPathDynamicImageAWP->SetCurrentSelectedIndex(WholePath_listBox->SelectedIndex);
					for(int i=0;i<PathNode_listBox->SelectedIndices->Count;++i)
					{
						int l_SelectedIndex =  PathNode_listBox->SelectedIndices[i];
						*l_pTexBehaviorDataWithImageIndexData = *(*m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList())[l_SelectedIndex];
						l_pTexBehaviorDataWithImageIndexData->bMirror = Mirror_checkBox->Checked;
					}
				 }
				else
				if( WholePath_listBox->SelectedIndices->Count > 1 )
				{
				
				}
			  }
	System::Void MPDIEditor::WholeAnimationList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( WholeAnimationList_listBox->SelectedItem )
					AnimationName_textBox->Text = WholeAnimationList_listBox->SelectedItem->ToString();
			 }
	System::Void MPDIEditor::WholeAnimationList_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				AddAnimationToList_button_Click(AnimationInvert_button,e);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::EnableColorBlending_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( WholePath_listBox->SelectedIndex != -1 )
				 {
					 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					 {
						 SrcBlending_comboBox->SelectedIndex = BlendingIndexToIndex( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetSrcBlendingMode() );
						 DestBlending_comboBox->SelectedIndex = BlendingIndexToIndex( m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetDestBlendingMode() );
						 m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetColorBlending(EnableColorBlending_checkBox->Checked);
						 SrcBlending_comboBox->Enabled = EnableColorBlending_checkBox->Checked;
						 DestBlending_comboBox->Enabled = EnableColorBlending_checkBox->Checked;
					 }
				 }
			 }
	System::Void MPDIEditor::WholeAnimationList_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 if(e->KeyCode == Keys::Enter)
				 {
					AddAnimationToList_button_Click(AnimationInvert_button,e);
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::SrcBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetSrcBlendingMode(IndexToBlendingIndex(this->SrcBlending_comboBox->SelectedIndex));
			 }
	System::Void MPDIEditor::DestBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
					m_pMultiPathDynamicImageAWP->m_pCurrentCurve->SetDestBlendingMode(IndexToBlendingIndex(this->DestBlending_comboBox->SelectedIndex));
			 }

	System::Void MPDIEditor::PathNode_listBox_MouseEnter(System::Object^  sender, System::EventArgs^  e)
			 {
				 m_bMouseEnterPathNodeListbox = true;
			 }
	System::Void MPDIEditor::PathNode_listBox_MouseLeave(System::Object^  sender, System::EventArgs^  e)
			 {
				 m_bMouseEnterPathNodeListbox = false;
			 }
	System::Void MPDIEditor::openPathFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( sender == saveGamePathToolStripMenuItem )
				{
					String^l_str = DNCT::SaveFileAndGetName();
					if( l_str )
					{
						cCurveManager*l_pCurveManager = new cCurveManager;
						if( m_pMPDIList )
						{
							int	l_iNum = m_pMPDIList->Count();
							for( int i=0;i<l_iNum;++i )
							{
								cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*m_pMPDIList)[i];
								int	l_iNum2 = l_pMultiPathDynamicImage->Count();
								for( int j=0;j<l_iNum2;++j )
								{
									cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*l_pMultiPathDynamicImage)[j];
									cCurveWithTime*l_pNewCurve = new cCueToStartCurveWithTime(m_pMPDIList);
									*l_pNewCurve = *l_pCueToStartCurvesWithTime;
									if(!l_pCurveManager->GetObject(l_pNewCurve->GetName()))
										l_pCurveManager->AddObject(l_pNewCurve);
									else
									{
										static int	l_siSeriesNumber = 0;
										WCHAR	l_strNewCurveName[MAX_PATH];
										while(l_pCurveManager->GetObject(l_pNewCurve->GetName()))
										{
											++l_siSeriesNumber;
											swprintf_s(l_strNewCurveName,MAX_PATH,L"%s%d\0",l_pNewCurve->GetName(),l_siSeriesNumber);
											l_pNewCurve->SetName(l_strNewCurveName);
										}
										l_pCurveManager->AddObject(l_pNewCurve);
									}
								}
							}
						}
						l_pCurveManager->ExportData(DNCT::GcStringToChar(l_str));
						SAFE_DELETE(l_pCurveManager);
					}			
				}
				else
				if( sender == savePathFileToolStripMenuItem )
				{
					if( !m_pReferenceCurveManagerAWP )
						return;
					String^l_str = DNCT::SaveFileAndGetName();
					if( l_str )
						m_pReferenceCurveManagerAWP->ExportData(DNCT::GcStringToChar(l_str));
				}
				else
				if( sender == openPathFileToolStripMenuItem )
				{
					String^l_str = DNCT::OpenFileAndGetName("(*path)|*.path");
					if( l_str )
					{
						if( !m_pReferenceCurveManagerAWP )
							m_pReferenceCurveManagerAWP = new cCurveManager();
						m_pReferenceCurveManagerAWP->Destroy();
						m_pReferenceCurveManagerAWP->SetCurrentSelectedIndex(-1);
						m_pReferenceCurveManagerAWP->Parse(DNCT::GcStringToChar(l_str));
						AllPath_listBox->Items->Clear();
						for( int i =0;i<m_pReferenceCurveManagerAWP->Count();++i )
						{
							AllPath_listBox->Items->Add(String(UT::WcharToChar((*m_pReferenceCurveManagerAWP)[i]->GetName())).ToString());
						}
					}
				}
			 }
	System::Void MPDIEditor::AddPathIntoAllPath_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 ReferencePathModify(sender,e);
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::AddEventPOint_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 EventPoint(sender,e);
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::RearrangeWholePathTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				if( m_pMultiPathDynamicImageAWP )
				{
					float	l_fNewTime = (float)RearrangeWholePathTime_numericUpDown->Value/1000.f;
					m_pMultiPathDynamicImageAWP->RearrangeTime(l_fNewTime);
					this->WholePath_listBox->SelectedIndex = -1;
				}
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::showImageBoderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pObject = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pObject->Checked = !l_pObject->Checked;
				 if( sender == cameraFitToolStripMenuItem )
				 {
					splitContainer3_Panel1_Resize(sender,e);
				 }
			 }
	System::Void MPDIEditor::MergeMPDIData_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				 if( WholeAnimationList_listBox->SelectedIndex != -1 )
				 {
					 if( WholeAnimationList_listBox->SelectedIndex != -1)
					 {
						 if( m_pMultiPathDynamicImageAWP )
						 {
							 m_bPreventChangeAllTimeValueSetup = true;
							 cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pMPDIList->GetObject(WholeAnimationList_listBox->SelectedIndex);
							 //set new offset time if need
							 WARING_YES_NO_TO_YES("add start time to all curve")
							 {
								float	l_fStartTime = CreateNumericFormAndGetValue(1000000.f,100.f,0);
								l_fStartTime /= 1000.f;
								int	l_iSelectedMPDICount = l_pMultiPathDynamicImage->Count();
								int	l_iTargetMPDICount = m_pMultiPathDynamicImageAWP->Count();
								int	l_iTargetMPDIOriginalCount = l_iTargetMPDICount-l_iSelectedMPDICount;
								for( int i=0;i<l_iSelectedMPDICount;++i )
								{
									cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->GetObject(i+l_iTargetMPDIOriginalCount);
									l_pCueToStartCurveWithTime->SetStartTime(l_pMultiPathDynamicImage->GetObject(i)->GetStartTime()+l_fStartTime);
								}
							 }
							 m_pMultiPathDynamicImageAWP->Merge(l_pMultiPathDynamicImage);
							 WholePath_listBox->Items->Clear();
							 for(int i=0;i<m_pMultiPathDynamicImageAWP->Count();++i)
							 {
								cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*m_pMultiPathDynamicImageAWP)[i];
								String^l_strObjectName = DNCT::WcharToGcstring(l_pCueToStartCurvesWithTime->GetName());
								WholePath_listBox->Items->Add(l_strObjectName);
							 }
							 AllCurveLoop_checkBox->Checked = m_pMultiPathDynamicImageAWP->IsLoop();
							 AllPlay_checkBox->Checked = false;
							 Play_checkBox_CheckedChanged(AllPlay_checkBox,e);
							 m_bPreventChangeAllTimeValueSetup = false;
						 }
					 }
				 }
				 this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::CurrentProgress_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if(CurrentProgress_trackBar->Focused)
					m_bAnimationPauseByTrackValue = true;
			 }
	System::Void MPDIEditor::saveAviFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( !m_pMultiPathDynamicImageAWP )
					return;
				if( m_pMultiPathDynamicImageAWP->Count() <1 )
					return;
				this->timer1->Enabled = false;

				glAlphaFunc(GL_GREATER,0.00001f);
				glShadeModel(GL_SMOOTH);									// Select Smooth Shading
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(m_sfactor,m_dfactor);
				glEnable(GL_TEXTURE_2D);
				glEnable2D(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT);
				float	l_fElpasedTime = 1/30.f;
				int	l_iWidth = splitContainer3->Panel1->Width;
				//set opengl aligment
				int	l_iRestOfWidthOffset = l_iWidth%4;
				int	l_iHeight = splitContainer3->Panel1->Height;
				//force to align for 4.
				while(l_iRestOfWidthOffset != 0)
				{
					splitContainer2->SplitterDistance = splitContainer2->Panel1->Width-1;
					l_iWidth = splitContainer3->Panel1->Width;
					l_iRestOfWidthOffset = l_iWidth%4;
				}
				l_iWidth = splitContainer3->Panel1->Width;
				l_iHeight = splitContainer3->Panel1->Height;
				//int	l_iWidth = (int)DRAW_PANEL_RESOLUTION_WIDTH;
				//int	l_iHeight = (int)DRAW_PANEL_RESOLUTION_HEIGHT;
				bool	l_bConpress = false;
				String^l_strAviName = DNCT::SaveFileAndGetName("avi files (*.avi)|*.avi|All files (*.*)|*.*");
				if( !l_strAviName )
					return;
				HAVI avi = CreateAvi(DNCT::GcStringToWchar(l_strAviName),(int)(l_fElpasedTime*1000),NULL); // 1000ms is the period between frames
				Bitmap^ systemBitmap = gcnew Bitmap(l_iWidth, l_iHeight, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
				int	l_ViewPort[4];
				glGetIntegerv(GL_VIEWPORT, l_ViewPort);

				int	l_iTotalSize = l_iWidth*l_iHeight*3;
				unsigned char *l_pPixelData = new unsigned char[l_iTotalSize];
				array<Byte>^ values = gcnew array<Byte>(l_iTotalSize);  
				m_pOrthogonalCamera->Render();

				cMPDI*l_pMPDI = new cMPDI(m_pMultiPathDynamicImageAWP);
				l_pMPDI->Init();
				l_pMPDI->SetLoop(false);
				while(!l_pMPDI->IsDone())
				{
					glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
					glClearColor( g_vBackgroundColor.x,g_vBackgroundColor.y,g_vBackgroundColor.z,g_vBackgroundColor.w );
					glClearDepth(1.0f);											// Depth Buffer Setup
					if(m_pBKImageAWP)
					{
						m_pBKImageAWP->Render();
					}
					l_pMPDI->Update(l_fElpasedTime);
					l_pMPDI->Render();
					SwapBuffers (m_HdcMV);
					glPixelStorei(GL_PACK_ALIGNMENT, 1);
					glReadPixels( 0, 0, l_iWidth, l_iHeight, GL_BGR_EXT , GL_UNSIGNED_BYTE, l_pPixelData );
					System::Runtime::InteropServices::Marshal::Copy((IntPtr)l_pPixelData, values, 0, l_iTotalSize);
					//create bitmap data, lock pixels to be written.
					System::Drawing::Imaging::BitmapData^ bitmapData = systemBitmap->LockBits(System::Drawing::Rectangle(0, 0, l_iWidth, l_iHeight), System::Drawing::Imaging::ImageLockMode::WriteOnly, systemBitmap->PixelFormat);
					System::Runtime::InteropServices::Marshal::Copy(values, 0, bitmapData->Scan0, l_iTotalSize);
					systemBitmap->UnlockBits(bitmapData);
					System::Drawing::Bitmap^l_pBitmap = systemBitmap;
					l_pBitmap->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);

					//System::Drawing::Bitmap^l_pBitmap = gcnew System::Drawing::Bitmap(l_iWidth,l_iHeight,24,System::Drawing::Imaging::PixelFormat::Format24bppRgb,System::IntPtr((void*)l_pPixelData));
					HBITMAP l_Bitmap  = (HBITMAP)l_pBitmap->GetHbitmap().ToPointer();
					if (!l_bConpress) // Set up compression just before the first frame
					{ 
						l_bConpress = true;
						AVICOMPRESSOPTIONS opts; ZeroMemory(&opts,sizeof(opts));
						opts.fccHandler=mmioFOURCC('d','i','v','x');
						if( SetAviVideoCompression(avi,l_Bitmap,&opts,true,0)!= AVIERR_OK)
						{
							this->timer1->Enabled = true;
							return;
						}
					}
					AddAviFrame(avi,l_Bitmap);
					DeleteObject(l_Bitmap);
					if( 0 )
					{
						static int ll = 0;++ll;
						String^l_strtt = ll.ToString()+"test.png";
						SaveCurrentBufferToImage(DNCT::GcStringToChar(l_strtt),l_iWidth,l_iHeight);
						l_pBitmap->Save(l_strtt);
					}
				}
				CloseAvi(avi);
				delete l_pPixelData;
				delete l_pMPDI;
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::showPath_ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pToolStripMenuItem = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pToolStripMenuItem->Checked = !l_pToolStripMenuItem->Checked;
				 if( showPanelToolStripMenuItem == sender)
				 {
					splitContainer2->Panel1Collapsed = l_pToolStripMenuItem->Checked;
					splitContainer1->Panel1Collapsed = l_pToolStripMenuItem->Checked;
				 }
			 }
	System::Void MPDIEditor::DelPI_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				while(AllPI_listBox->SelectedItems->Count)
				{
					String^l_strPIName = AllPI_listBox->SelectedItems[0]->ToString();
					cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_pImageResourceManagerAWP->GetObject(DNCT::GcStringToWchar(l_strPIName)));
					bool    l_bInUsing = false;
					if( m_pMultiPathDynamicImageAWP )
					if( m_pMultiPathDynamicImageAWP->Count() != 0 )
					{
						if(m_pMultiPathDynamicImageAWP->IsUsingPI(l_pPuzzleImage,true))
						{
							l_bInUsing = true;
						}
					}
					std::wstring	l_strInUsingInfo;
					if( !l_bInUsing&&m_pMPDIList->RemovePIReference(l_pPuzzleImage,&l_strInUsingInfo) )
					{
						if( m_pMultiPathDynamicImageAWP )
							m_pMultiPathDynamicImageAWP->RemovePIReference(l_pPuzzleImage);
						m_pImageResourceManagerAWP->RemoveObject(l_pPuzzleImage->GetName());
						AllPI_listBox->Items->Remove(l_strPIName);
					}
					else
					{
						l_bInUsing = true;
					}
					if(l_bInUsing)
					{

						WARNING_MSG(l_strPIName+
							" in used or please remove current edit path data\n"
							+
							String(UT::WcharToChar(l_strInUsingInfo.c_str())).ToString()
							);
					}
					AllPI_listBox->SelectedItems->Remove(l_strPIName);
				}
				m_pCurrentWorkingPI = 0;
				if( AllPI_listBox->Items->Count )
					AllPI_listBox->SelectedIndex = 0;
				if( AllPI_listBox->Items->Count == 0)
					AddPath_button->Enabled = false;
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::AllPI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				WholePuzzleImageUnit_listBox->Items->Clear();
				if( AllPI_listBox->SelectedIndex != -1 )
				{
					cPuzzleImage*l_pPuzzleImageAWP = dynamic_cast<cPuzzleImage*>(m_pImageResourceManagerAWP->GetObject(DNCT::GcStringToWchar(AllPI_listBox->SelectedItem->ToString())));
					for( int i=0;i<l_pPuzzleImageAWP->Count();++i )
					{
						WholePuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring(l_pPuzzleImageAWP->GetObject(i)->GetName()));
					}
					if( m_pMPDIList )
					{
						m_pCurrentWorkingPI = m_pMPDIList->GetPuzzleImage(DNCT::GcStringToWchar(AllPI_listBox->SelectedItem->ToString()));
						if( m_pCurrentWorkingPI->Count() )
							WholePuzzleImageUnit_listBox->SelectedIndex = 0;
					}
				}
				else
				{
					WholePuzzleImageUnit_listBox->Items->Clear();
					m_pCurrentWorkingPI = 0;
				}
			 }
	System::Void MPDIEditor::ApplyGroupDataChange_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				if( !m_pMultiPathDynamicImageAWP )
					return;
				Vector3	l_vPos = Vector3((float)GroupPosX_numericUpDown->Value,(float)GroupPosY_numericUpDown->Value,(float)GroupPosZ_numericUpDown->Value);
				switch(GroupPositionMoveType_comboBox->SelectedIndex)
				{
					case 1://relative
						m_pMultiPathDynamicImageAWP->SetRelativePosToChild(l_vPos);
						break;
					case 2://world
						m_pMultiPathDynamicImageAWP->SetCenterPosToChild(l_vPos);
						break;
				}
				float	l_fRotationXAngle = (float)GroupRotationX_numericUpDown->Value;
				float	l_fRotationYAngle = (float)GroupRotationY_numericUpDown->Value;
				float	l_fRotationZAngle = (float)GroupRotationZ_numericUpDown->Value;
				Quaternion l_Quaternion = Quaternion::EulerRotationQuaternion(D3DXToRadian(l_fRotationXAngle),D3DXToRadian(l_fRotationYAngle),D3DXToRadian(l_fRotationZAngle));
				switch(GroupRotationType_comboBox->SelectedIndex)
				{
					case 1://self
						m_pMultiPathDynamicImageAWP->SetSelfRotationToChild(l_Quaternion,GroupImageRotate_checkBox->Checked);
						break;
					case 2://world
						m_pMultiPathDynamicImageAWP->SetCenterRotationToChild(l_Quaternion,GroupImageRotate_checkBox->Checked);
						break;
				}
				int l_iScale = (int)GroupScale_numericUpDown->Value;
				m_pMultiPathDynamicImageAWP->SetScaleForAllChildPointData(l_iScale/100.f);
				
			 }
	System::Void MPDIEditor::ScaleWithPIAlphaStripPixel_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {

				 //make selected image at center.
				 WARING_YES_NO_TO_YES("ensure you have backup the file,because this convert may has problem")
				 {
					 if( !m_pMPDIList )
						 return;
					 int	l_i = m_pMPDIList->Count();
					 for( int i=0;i<l_i;++i )
					 {
						 cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pMPDIList->GetObject(i);
						 int	l_i2 = l_pMultiPathDynamicImage->Count();
						 for( int j=0;j<l_i2;++j )
						 {
							cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = l_pMultiPathDynamicImage->GetObject(j);
							size_t l_i3 = l_pCueToStartCurveWithTime->GetPointDataList()->size();
							std::vector<Vector3>	l_OriginalPointList = l_pCueToStartCurveWithTime->GetOriginalPointList();
							for( size_t k=0;k<l_i3;++k )
							{
								//change size
								sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pCueToStartCurveWithTime->GetPointDataList())[k];
								sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData()[l_pTexBehaviorDataWithImageIndexData->iImageIndex];
								if( l_pPuzzleData->Size.x != l_pPuzzleData->OriginalSize.x ||
									l_pPuzzleData->Size.y != l_pPuzzleData->OriginalSize.y)
								{
									Vector2	l_vImageCenterOffset = l_pPuzzleData->GetCenterOffsetxWithRenderSize(l_pTexBehaviorDataWithImageIndexData->Size);
									Vector3	l_vPos = l_OriginalPointList[k];
									l_vPos.x -= l_vImageCenterOffset.x;	l_vPos.y -= l_vImageCenterOffset.y;
									//l_OriginalPointList[k] = l_vPos;
									if( l_pPuzzleData->OriginalSize.x == (int)l_pTexBehaviorDataWithImageIndexData->Size.x &&
										l_pPuzzleData->OriginalSize.y == (int)l_pTexBehaviorDataWithImageIndexData->Size.y)
									{
  										l_pTexBehaviorDataWithImageIndexData->Size.x = (float)l_pPuzzleData->Size.x;
										l_pTexBehaviorDataWithImageIndexData->Size.y = (float)l_pPuzzleData->Size.y;
									}
									else
									{
										Vector2	l_vScale((float)l_pPuzzleData->Size.x/(float)l_pPuzzleData->OriginalSize.x,(float)l_pPuzzleData->Size.y/(float)l_pPuzzleData->OriginalSize.y);
										l_pTexBehaviorDataWithImageIndexData->Size.x *= l_vScale.x;
										l_pTexBehaviorDataWithImageIndexData->Size.y *= l_vScale.y;								
									}
								}
								//change position
							}
							l_pCueToStartCurveWithTime->SetOriginalPointList(l_OriginalPointList);
						 }
					 }
				 }
			 }
	System::Void MPDIEditor::SelectFirstAndLast_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( PathNode_listBox->Items->Count>1 )
				 {
					 PathNode_listBox->SelectedIndices->Clear();
					 PathNode_listBox->SelectedIndices->Add(0);
					 PathNode_listBox->SelectedIndices->Add(PathNode_listBox->Items->Count-1);
				 }
			 }
	System::Void MPDIEditor::WholeAnimationListUp_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				ListBoxSelectedItemWithObjectListMove<cMPDIList,cMPDIList>(sender == WholeAnimationListUp_button?true:false,WholeAnimationList_listBox,m_pMPDIList,0);
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::saveImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				this->timer1->Enabled = false;
				Sleep(10);
				m_pOrthogonalCamera->Reset();
				SaveCurrentBufferToImage("test.png",splitContainer3->Panel1->Width,splitContainer3->Panel1->Height);
				Sleep(10);
				System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap("test.png");
				Sleep(10);
				System::Drawing::Image^l_pNewImage = FixedSize2(l_pBitMap,(int)m_pOrthogonalCamera->GetResolution().x,(int)m_pOrthogonalCamera->GetResolution().y);
				if(System::IO::File::Exists("test2.png"))
					System::IO::File::Delete("test2.png");
				Sleep(10);
				l_pNewImage->Save("test2.png");
				delete l_pNewImage;
				delete l_pBitMap;            
				Sleep(10);
				System::IO::File::Delete("test.png");
				this->timer1->Enabled = true;
			 }
	System::Void MPDIEditor::WholePath_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			{
				if( e->KeyCode == GCFORM::Keys::Enter )
				{
					Play_checkBox->Checked = !Play_checkBox->Checked;
					//PlayCheckedChanged(Play_checkBox,e);
				}
			}
	System::Void MPDIEditor::SelectAllPoints_button_Click(System::Object^  sender, System::EventArgs^  e)
			{
				if(PathNode_listBox->Items->Count)
				{
					PathNode_listBox->SelectedIndices->Clear();
					for(int i=0;i<PathNode_listBox->Items->Count;++i)
						PathNode_listBox->SelectedIndices->Add(i);
				}
			}
	System::Void MPDIEditor::renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !m_pMPDIList )
					 return;
				 int	l_i = m_pMPDIList->Count();
				 for( int i=0;i<l_i;++i )
				 {
					 cMultiPathDynamicImage*l_pMultiPathDynamicImage = m_pMPDIList->GetObject(i);
					 int	l_i2 = l_pMultiPathDynamicImage->Count();
					 for( int j=0;j<l_i2;++j )
					 {
						cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = l_pMultiPathDynamicImage->GetObject(j);
						size_t l_i3 = l_pCueToStartCurveWithTime->GetPointDataList()->size();
						std::vector<Vector3>	l_OriginalPointList = l_pCueToStartCurveWithTime->GetOriginalPointList();
						for( size_t k=0;k<l_i3;++k )
						{
							//change size
							sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = (*l_pCueToStartCurveWithTime->GetPointDataList())[k];
							sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData()[l_pTexBehaviorDataWithImageIndexData->iImageIndex];
							if( l_pPuzzleData->Size.x != l_pPuzzleData->OriginalSize.x ||
								l_pPuzzleData->Size.y != l_pPuzzleData->OriginalSize.y)
							{
								Vector2	l_vImageCenterOffset = l_pPuzzleData->GetCenterOffsetxWithRenderSize(l_pTexBehaviorDataWithImageIndexData->Size);
								Vector3	l_vPos = l_OriginalPointList[k];
								l_pTexBehaviorDataWithImageIndexData->Size.x = (float)l_pPuzzleData->Size.x;
								l_pTexBehaviorDataWithImageIndexData->Size.y = (float)l_pPuzzleData->Size.y;
							}
							//change position
						}
						l_pCueToStartCurveWithTime->SetOriginalPointList(l_OriginalPointList);
					 }
				 }
			 }
	System::Void MPDIEditor::toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( WholePuzzleImageUnit_listBox->SelectedItem )
				 {
					 for each( String^l_strItem in WholePuzzleImageUnit_listBox->SelectedItems )
					 {
						AddPath(l_strItem);
					 }
				 }
			 }
	System::Void MPDIEditor::splitContainer3_Panel1_Resize(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( cameraFitToolStripMenuItem->Checked )
				 {
					POINT l_ViewportSize = { splitContainer3->Panel1->Width,splitContainer3->Panel1->Height };
					Vector2	l_vOriginalResolution = m_pOrthogonalCamera->GetResolution();
					Vector2	l_vOriginalLeftUpPos = m_pOrthogonalCamera->GetCameraPos()-(l_vOriginalResolution/2.f);
					m_pOrthogonalCamera->SetResolution(Vector2((float)l_ViewportSize.x,(float)l_ViewportSize.y));
					m_pOrthogonalCamera->SetCameraPos(l_vOriginalLeftUpPos+m_pOrthogonalCamera->GetResolution()/2.f);
					m_pOrthogonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
				 }
				 else
				 {
					m_pOrthogonalCamera->SetResolution(Vector2(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT));
					m_pOrthogonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
				 }
			 }
	System::Void MPDIEditor::PathNode_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				 if(e->Control)
				 {
					 if( e->KeyCode == GCFORM::Keys::P )
					 {
						 PathNodePosX_numericUpDown->Focus();
					 }
					 else
					 if( e->KeyCode == GCFORM::Keys::S )
					 {
						 Width_numericUpDown->Focus();
					 }
					 else
					 if( e->KeyCode == GCFORM::Keys::C )
					 {
						 RedColor_numericUpDown->Focus();
					 }
					 else
					 if( e->KeyCode == GCFORM::Keys::R )
					 {
						AngleZ_numericUpDown->Focus();
					 }
					 else
					 if( e->KeyCode == GCFORM::Keys::D1 )
					 {
						 WholePath_listBox->Focus();
					 }
					 else
					 if( e->KeyCode == GCFORM::Keys::D2 )
						WholePuzzleImageUnit_listBox->Focus();
					 else
					 if( e->KeyCode == GCFORM::Keys::D3 )
					 {
						 PathNode_listBox->Focus();
					 }
				 }
			 }
	System::Void MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 MPDIEditor::AddGameUI(sender);
			 }
	System::Void MPDIEditor::addPointDataWithImageByPathNameToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 toolStripMenuItem1_Click(sender,e);
				 WholePath_listBox->SelectedIndices->Clear();
				 for (int l_iIndex = 0;l_iIndex<WholePath_listBox->Items->Count;++l_iIndex  )
					 WholePath_listBox->SelectedIndices->Add(l_iIndex);
				 if( WholePath_listBox->SelectedIndices->Count )
				 {
					if( !m_pCurrentWorkingPI )
					{
						WARNING_MSG("please select a pi");
						return;
					}
					bool	l_bAdded = false;
					for each( int l_iIndex in WholePath_listBox->SelectedIndices )
					{
						l_bAdded = true;
						WCHAR*l_strImageName = DNCT::GcStringToWchar(WholePath_listBox->Items[l_iIndex]->ToString());
						cCueToStartCurveWithTime*l_pCueToStartCurveWithTime = m_pMultiPathDynamicImageAWP->GetObject(l_iIndex);
						cPuzzleImageUnit*l_pPIUnit = m_pCurrentWorkingPI->GetObject(l_strImageName);
						if( l_pCueToStartCurveWithTime->GetOriginalPointList().size() == 0 )
						{
							if( l_pPIUnit )
							{
								int	l_iImageIndex = m_pCurrentWorkingPI->GetObjectIndexByPointer(l_pPIUnit);
								POINT	l_Size = m_pCurrentWorkingPI->GetPuzzleData()[l_iImageIndex]->Size;
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,0,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,l_iImageIndex,false,m_pCurrentWorkingPI);
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,1.f,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,l_iImageIndex,false,m_pCurrentWorkingPI);
							}
							else
							{
								POINT	l_Size = m_pCurrentWorkingPI->GetPuzzleData()[0]->Size;
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,0,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,0,false,m_pCurrentWorkingPI);
								l_pCueToStartCurveWithTime->AddPoint(Vector3::Zero,1.f,Vector2((float)l_Size.x,(float)l_Size.y),0,Vector4::One,0,false,m_pCurrentWorkingPI);						
							}
						}
					}
					if(l_bAdded)
					{
						WholePath_listBox->SelectedIndex = -1;
						WholePath_listBox->SelectedIndex = WholePath_listBox->Items->Count-1;
					}
				 }
			 }
	System::Void MPDIEditor::FadeIn_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(PathNode_listBox->Items->Count)
				 {
					if(m_pMultiPathDynamicImageAWP->m_pCurrentCurve)
					{
						int	l_iSize = (int)m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointDataList()->size();
						float	l_fStep = 1.f/l_iSize;
						if( FadeIn_toolStripMenuItem == sender )
						{
							float	l_fAlpha = 1.f;
							for( int i=0;i<l_iSize;++i )
							{
								sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointData(i);
								l_pTexBehaviorDataWithImageIndexData->vColor.w = l_fAlpha;
								l_fAlpha -= l_fStep;
							}
						}
						else
						if( sender == FadeOut_toolStripMenuItem )
						{
							float	l_fAlpha = 0;
							for( int i=0;i<l_iSize;++i )
							{
								sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = m_pMultiPathDynamicImageAWP->m_pCurrentCurve->GetPointData(i);
								l_pTexBehaviorDataWithImageIndexData->vColor.w = l_fAlpha;
								l_fAlpha += l_fStep;						
							}
						}
					}
					}
			 }
	System::Void MPDIEditor::mergeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 String^l_strFileName = DNCT::OpenFileAndGetName("mpdi files (*.mpdi)|*.mpdi|All files (*.*)|*.*");
				 if( l_strFileName )
				 {
					 Merge(l_strFileName);				 
				 }
				 
			 }
	System::Void MPDIEditor::UseViewPort_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !m_pMultiPathDynamicImageAWP )
					 return;
				 Vector4*l_pViewPort = 0;
				 if( UseViewPort_checkBox->Checked )
				 {
					 if(!m_pMultiPathDynamicImageAWP->GetViewPort())
					 {
						m_pMultiPathDynamicImageAWP->SetViewPort(Vector4(0,0,DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT));
					 }
					l_pViewPort = m_pMultiPathDynamicImageAWP->GetViewPort();
					ViewPort_xyzwNumericalControl->SetValue(&l_pViewPort->x,&l_pViewPort->y,&l_pViewPort->z,&l_pViewPort->w);
				 }
				 else
				 {
					ViewPort_xyzwNumericalControl->SetValue(0,0,0,0);
					m_pMultiPathDynamicImageAWP->RemoveViewPort();
				 }
			 }
	System::Void MPDIEditor::ImageAnchor_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(m_bPreventChangeAllTimeValueSetup)
					 return;
				if( m_pMultiPathDynamicImageAWP && m_pMultiPathDynamicImageAWP->m_pCurrentCurve )
				{
					eImageAnchorType	l_eImageAnchorType = (eImageAnchorType)ImageAnchor_comboBox->SelectedIndex;;

					for each(int l_iSelectedIndex in WholePath_listBox->SelectedIndices)
					{
						m_pMultiPathDynamicImageAWP->GetObject(l_iSelectedIndex)->SetAnchorType(l_eImageAnchorType);
					}
				}
			 }
	System::Void MPDIEditor::floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if(m_pForm)
				 {
					 HWND	l_pHwnd = (HWND)m_pForm->Handle.ToPointer();
					 if( sender == floatToolStripMenuItem )
					 {
						 SetParent(l_pHwnd,0);
						 m_pForm->TopLevel = true;
					 }
					 else
					 if( sender == childToolStripMenuItem )
					 {
						 m_pForm->TopLevel = false;
						SetParent(l_pHwnd,m_pParentHandle);
					 }
				 }
			 }
	System::Void MPDIEditor::MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
	
	}

	System::Void MPDIEditor::CameraRotationX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if( m_bPreventChangeAllTimeValueSetup )
			return;
		if( m_pMultiPathDynamicImageAWP )
		{
			cMatrix44*l_pMat = m_pMultiPathDynamicImageAWP->GetCameraView();
			Vector3	l_vRotation((float)CameraRotationX_numericUpDown->Value,
								(float)CameraRotationY_numericUpDown->Value,
								(float)CameraRotationZ_numericUpDown->Value);
			Vector3	l_vRotationCenter((float)CameraRotationCenterX_numericUpDown->Value,
								(float)CameraRotationCenterY_numericUpDown->Value,
								(float)CameraRotationCenterZ_numericUpDown->Value);
			if( l_vRotation.x !=0 || l_vRotation.y !=0 || l_vRotation.z !=0 || 
				l_vRotationCenter.x != 0 || l_vRotationCenter.y !=0 || l_vRotationCenter.z !=0)
			{
				if( !l_pMat )
					l_pMat = new cMatrix44;
				*l_pMat = cMatrix44::RotationMatrix(l_vRotation);
				l_pMat->m[3][0] = l_vRotationCenter.x;
				l_pMat->m[3][1] = l_vRotationCenter.y;
				l_pMat->m[3][2] = l_vRotationCenter.z;
			}
			m_pMultiPathDynamicImageAWP->SetCameraView(l_pMat);
		}
	}
//end namespace
}