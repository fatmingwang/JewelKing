#include "StdAfx.h"
#include "ObjectDataForm.h"
//all template data is here
cMap_CellData*g_pCurrentStageEditData = 0;
cMapLayer*g_pForListBoxTemplateCellData = 0;
cMap_Cell_ObjectData*g_pCurrentEditUserPropertyObjectData = 0;
cBaseImage*g_pCurrentSelectedImage = 0;
cBaseImage*g_pBackgroundImage = 0;
sTimeAndFPS	g_TimeAndFPS;
bool	g_bStageDataChanged = false;
//the position has been aligh the frid position
Vector3	g_vGridAlignPos(0,0,0);
Vector3	g_vMousePos(0,0,0);
//check auto aligh if true g_vGridAlignPos or g_vMousePos
Vector2	g_vCurrentPlacePos(0,0);


Vector3	g_vSelectedObjectOriginalPos(0,0,0);

#define	USER_DEFINE_DATA		"UserDefineData"
#define	USER_DEFINE_DATA_STAGE	"Stage"
#define	USER_DEFINE_DATA_LAYER	"Layer"
#define	USER_DEFINE_DATA_INDEX	"Index"
namespace LevelEditor
{
	void	ObjectDataForm::SaveTreeNodeFile(String^e_strFileName,System::Collections::Hashtable^e_pHashTable,bool e_bUserDefineData)
	{
		 //for all template data
		 String^l_strFileName = e_strFileName;
		 GCFORM::TreeNode^ l_pTemplateNode = gcnew GCFORM::TreeNode();
		 l_pTemplateNode->Text = "Root";
		 if( e_bUserDefineData )
		 {
			 for each (uint64 l_uiID in e_pHashTable->Keys)
			 {
				cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = this->m_pMapDataManager->GetListChildByUniqueID(l_uiID);
				cMapLayer*l_pMapLayer = dynamic_cast<cMapLayer*>(l_pMap_Cell_ObjectData->GetOwner());
				cMap_CellData*l_pMap_CellData = dynamic_cast<cMap_CellData*>(l_pMapLayer->GetOwner());

				GCFORM::TreeNode^l_pRootNode = gcnew GCFORM::TreeNode("UserDefineData");
				int	l_iIndex = l_pMapLayer->GetObjectIndexByPointer(l_pMap_Cell_ObjectData);
				l_pRootNode->Nodes->Add("Stage")->Name = DNCT::WcharToGcstring(l_pMap_CellData->GetName());
				l_pRootNode->Nodes->Add("Layer")->Name = DNCT::WcharToGcstring(l_pMapLayer->GetName());
				l_pRootNode->Nodes->Add("Index")->Name = l_iIndex.ToString();
				TreeNode^l_pTreeNode = (TreeNode^)e_pHashTable[l_uiID];
				l_pRootNode->Nodes->Add((TreeNode^)l_pTreeNode->Clone());
				l_pTemplateNode->Nodes->Add(l_pRootNode);
			 }
		 }
		 else
		 {
			 for each (TreeNode^ t in e_pHashTable->Values)
			 {
				l_pTemplateNode->Nodes->Add((TreeNode^)t->Clone());
			 }
		 }
		//SaveTreeNodeToFile(l_pTemplateNode,l_strFileName);
		 XmlDocument^ d = gcnew XmlDocument();
		 XmlNode^ n = d->CreateNode(XmlNodeType::Element,"root1","");
		 //for each (TreeNode^ t in m_TemplateNameAndAttributeNode->Values)
		 {
			 n->AppendChild(DNCT::getXmlNode(l_pTemplateNode, d));
		 }
		 for each(XmlNode^ e_ChildNode in n->ChildNodes)
			d->AppendChild(e_ChildNode);
		  d->Save(l_strFileName);
	 }

	System::Void	ObjectDataForm::AddObjectOnCell()
	{
		Vector2	l_vCurrentPos = m_pOrthogonalCamera->GetMouseWorldPos();
		if( Template_listBox->SelectedIndex == -1 )
			return;
		if( autoAllignGridToolStripMenuItem->Checked )
		{
			float	l_fStartPos[2] = {(float)StartX_numericUpDown->Value,(float)StartY_numericUpDown->Value};
			float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
			float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
			//if( l_vCurrentPos.x >= 0.f&&l_fWidth >= l_vCurrentPos.x&&l_vCurrentPos.y >= 0.f&&l_fHeight >= l_vCurrentPos.y )
			{
				int	l_iRow = (int)((l_vCurrentPos.x-l_fStartPos[0])/m_pOrthogonalCamera->GetGridWidth());
				int	l_iColumn = (int)((l_vCurrentPos.y-l_fStartPos[1])/m_pOrthogonalCamera->GetGridHeight());
				if(l_vCurrentPos.x < l_fStartPos[0] )
					l_iRow-=1;
				if(l_vCurrentPos.y < l_fStartPos[1] )
					l_iColumn-=1;
				l_vCurrentPos.x = l_iRow*m_pOrthogonalCamera->GetGridWidth()+l_fStartPos[0];
				l_vCurrentPos.y = l_iColumn*m_pOrthogonalCamera->GetGridHeight()+l_fStartPos[1];
			}
		}
		bool	l_bSamePositiob = false;
		WCHAR*l_strName = DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString());
		if( !AllowSamepositionObject_checkBox->Checked )//if same object is not allow to at same position
		{
			//if( l_vPos.x == l_vCurrentPos.x && l_vPos.y == l_vCurrentPos.y )
			int	l_iColldedIndex = g_pCurrentStageEditData->CollideForSamePosition(g_vCurrentPlacePos);
			if( l_iColldedIndex != -1 )
			{
				l_bSamePositiob = true;
				g_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iColldedIndex);
				StageDataInfo_listBox->Items->RemoveAt(l_iColldedIndex);
			}
		}
		AddObjectIntoCurrentStage(Vector3(l_vCurrentPos.x,l_vCurrentPos.y,0.f));
	}

	System::Void ObjectDataForm::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if( !Main_splitContainer->Focused )
			Main_splitContainer->Focus();
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));

		int	l_iSelectedObjectIndex = g_pCurrentStageEditData->Collide(m_pOrthogonalCamera->GetMouseWorldPos());
		if( l_iSelectedObjectIndex != -1 )
		{
			if( ObjectMove_checkBox->Checked&&!Erase_checkBox->Checked )
			{
				g_pCurrentStageEditData->SetSelectedObject(g_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex));
				g_vSelectedObjectOriginalPos = g_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex)->GetPos();
			}
		}
	}

	System::Void ObjectDataForm::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));

		Vector2	l_vPos = m_pOrthogonalCamera->GetMouseWorldPos();
		g_vMousePos = Vector3(l_vPos.x,l_vPos.y,0.f);
		float	l_fStartPos[2] = {(float)StartX_numericUpDown->Value,(float)StartY_numericUpDown->Value};
		if( autoAllignGridToolStripMenuItem->Checked )
		{
			float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
			float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
			//if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
			{
				int	l_iRow = (int)((l_vPos.x-l_fStartPos[0])/m_pOrthogonalCamera->GetGridWidth());
				int	l_iColumn = (int)((l_vPos.y-l_fStartPos[1])/m_pOrthogonalCamera->GetGridHeight());
				if(l_vPos.x < l_fStartPos[0] )
					l_iRow-=1;
				if(l_vPos.y < l_fStartPos[1] )
					l_iColumn-=1;
				g_vGridAlignPos.x = l_iRow*m_pOrthogonalCamera->GetGridWidth()+l_fStartPos[0];
				g_vGridAlignPos.y = l_iColumn*m_pOrthogonalCamera->GetGridHeight()+l_fStartPos[1];
			}
			g_vCurrentPlacePos = Vector2(g_vGridAlignPos.x,g_vGridAlignPos.y);
			m_pForm->Text = ((int)g_vCurrentPlacePos.x).ToString()+","+((int)g_vCurrentPlacePos.y).ToString();
		}
		else
		{
			m_pForm->Text = ((int)g_vMousePos.x).ToString()+","+((int)g_vMousePos.y).ToString();
			g_vCurrentPlacePos = Vector2(g_vMousePos.x,g_vMousePos.y);
		}
		if( g_pCurrentStageEditData )
		{
			NumObjectInCurrentStagelabel_label->Text = g_pCurrentStageEditData->Count().ToString();
			if( g_pCurrentStageEditData->GetSelectedObject() )
			{
				Vector3	l_vPos = g_pCurrentStageEditData->GetSelectedObject()->GetPos();
				label12->Text = "ObjectPos:"+((int)l_vPos.x).ToString()+","+((int)l_vPos.y).ToString();
			}
			Vector2	l_vCameraPos = m_pOrthogonalCamera->GetMouseWorldPos();
			if( g_pCurrentStageEditData->GetSelectedObject() )
			{
				g_pCurrentStageEditData->GetSelectedObject()->SetPos(l_vPos);
			}
			else
			{
				int	l_iSelectedObjectIndex = g_pCurrentStageEditData->Collide(m_pOrthogonalCamera->GetMouseWorldPos());
				if( l_iSelectedObjectIndex != -1 )
				{
					Vector3	l_vPos = g_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex)->GetPos();
					int	l_iNiceNextX = *g_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedObjectIndex)->GetImage()->GetImageShowWidth()+(int)l_vPos.x;
					int	l_iNiceNextY = (int)l_vPos.y;
					label12->Text = "ObjectPos:"+((int)l_vPos.x).ToString()+","+((int)l_vPos.y).ToString()+"NiceNext:"+l_iNiceNextX.ToString()+","+l_iNiceNextY.ToString();
					StageDataInfo_listBox->SelectedItems->Clear();
					StageDataInfo_listBox->SelectedIndex = l_iSelectedObjectIndex;
				}
				else
					label12->Text = "";
			}
		}
	}

	System::Void ObjectDataForm::MyMouseDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		Vector2	l_vMousePos = m_pOrthogonalCamera->GetMouseWorldPos();
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = 0;
		for(int i=0;i<g_pCurrentStageEditData->Count();++i)
		{
			l_pMap_Cell_ObjectData = g_pCurrentStageEditData->GetCurrentList()->GetObject(i);
			Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
			if( l_vPos.x <= l_vMousePos.x&&
				l_vPos.x+*l_pMap_Cell_ObjectData->GetImage()->GetImageShowWidth() >= l_vMousePos.x
				&& l_vPos.y <= l_vMousePos.y
				&& l_vPos.y+*l_pMap_Cell_ObjectData->GetImage()->GetImageShowHeight() >= l_vMousePos.y)
			{
				ObjectData_treeView->Nodes->Clear();
				bool	l_bUniqueData = false;
				GCFORM::TreeNode^l_pNode;
				if( m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(l_pMap_Cell_ObjectData->GetUniqueID()) )
				{
					l_bUniqueData = true;
					l_pNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_pMap_Cell_ObjectData->GetUniqueID()];
				}
				NodeName_textBox->Text = "";
				this->ObjectData_textBox->Text = "";
				g_pCurrentEditUserPropertyObjectData = l_pMap_Cell_ObjectData;
				if( l_pNode )
				{
					ObjectData_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
					ObjectData_treeView->Nodes[0]->Expand();
					GCFORM::MessageBox::Show("Start to Edit Object property","GO!",GCFORM::MessageBoxButtons::OK);
				}
				else
				{
					if( l_bUniqueData )
					{
						WARNING_MSG("user property is not find!!! ensure data exists");
					}
				}
				return;
			}
		}
	}

	System::Void ObjectDataForm::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		GCFORM::MouseButtons l_MouseButton = e->Button;
		m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
			,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,
			e->Delta,e->X,e->Y,
			Vector2((float)Main_splitContainer->Panel1->Size.Width,(float)Main_splitContainer->Panel1->Size.Height));
		//m_pOrthogonalCamera->Render();
		if( e->Button != GCFORM::MouseButtons::Left )
			return;
		if( !g_pCurrentStageEditData->GetCurrentList() )
		{
			WARNING_MSG("please add or select layer first!");
			return;
		}
		if( ObjectMove_checkBox->Checked )
		{
			if(g_pCurrentStageEditData->GetSelectedObject())
			{
				//int	l_iCollidedObjectIndex = g_pCurrentStageEditData->CollideWithoutSelected(g_vCurrentPlacePos);
				int	l_iCollidedObjectIndex = g_pCurrentStageEditData->CollideForSamePositionWithoutSelected(g_vCurrentPlacePos);
				if( l_iCollidedObjectIndex != -1 )
				{
					g_pCurrentStageEditData->GetSelectedObject()->SetPos(g_vSelectedObjectOriginalPos);
				}
				g_pCurrentStageEditData->SetSelectedObject(0);

			}
		}
		else
		if( Erase_checkBox->Checked )
		{
			int	l_iNum = g_pCurrentStageEditData->Count();
			for( int i=0;i<l_iNum;++i )
			{
				int	l_iCount = g_pCurrentStageEditData->GetCurrentList()->Count();
				for( int j=0;j<l_iCount;++j )
				{
					cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pCurrentStageEditData->GetCurrentList()->GetObject(j);
					Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
					if( l_vPos.x <= g_vMousePos.x&&
						l_vPos.x+*l_pMap_Cell_ObjectData->GetImage()->GetImageShowWidth() >= g_vMousePos.x
						&& l_vPos.y <= g_vMousePos.y
						&& l_vPos.y+*l_pMap_Cell_ObjectData->GetImage()->GetImageShowHeight() >= g_vMousePos.y)
					{
						RemoveUserDefineNode(l_pMap_Cell_ObjectData);
						g_pCurrentStageEditData->GetCurrentList()->RemoveObject(j);
						StageDataInfo_listBox->Items->RemoveAt(i);
						i = l_iNum;
						g_bStageDataChanged = true;
						g_pCurrentEditUserPropertyObjectData = 0;
						int	l_iOriginalSelectedIndex = Layer_listBox->SelectedIndex;
						Layer_listBox->SelectedIndex = -1;
						Layer_listBox->SelectedIndex = l_iOriginalSelectedIndex;
						return;
					}
				}
			}
		}
		else
			AddObjectOnCell();
	}

	System::Void ObjectDataForm::Render()
	{
		wglMakeCurrent(m_HdcMV, m_HGLRC);
		DEFAULT_SHADER = L"ObjectDataFormShader";
		NO_TEXTURE_SHADER = L"ObjectDataFormNoTextureShader";
		cSimpleGLTexture::m_suiLastUsingImageIndex = -1;
		UseShaderProgram(NO_TEXTURE_SHADER);
		g_TimeAndFPS.Update();
		glViewport(0,0,Main_splitContainer->Panel1->Width,Main_splitContainer->Panel1->Height);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glClearColor( 0,0,0,1 );
		glClearDepth(1.0f);											// Depth Buffer Setup
		float	l_fStartPos[2] = {(float)StartX_numericUpDown->Value,(float)StartY_numericUpDown->Value};
		m_pOrthogonalCamera->Render();
		m_pOrthogonalCamera->DrawGrid(l_fStartPos[0],l_fStartPos[1]);
		if(showGridToolStripMenuItem->Checked)
		{
			static Vector4 l_vColor = Vector4::One;
			//l_suiColor = rand()%0xffffff;
			//l_suiColor+=0x000000ff;
			l_vColor = Vector4::One;
			//float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
			//float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
			RenderRectangle(Vector2(l_fStartPos[0],l_fStartPos[1]),(float)MapWidth_numericUpDown->Value,(float)MapHeight_numericUpDown->Value,l_vColor,0);
			//RenderRectangle(Vector2(l_fStartPos[0],l_fStartPos[1]),l_fWidth,l_fHeight,l_suiColor);

			//// 
			//// label18
			//// 
			//this->label18->AutoSize = true;
			//this->label18->Location = System::Drawing::Point(2, 384);
			//this->label18->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			//this->label18->Name = L"label18";
			//this->label18->Size = System::Drawing::Size(138, 72);
			//this->label18->TabIndex = 63;
			//this->label18->Text = L"TemplateList_Type\r\nstill in progress...\r\nbecause TMP has\r\ntoo much members\r\nso se" 
			//	L"t a type category\r\nis easy to select what u want!\r\n";
		}
		if( showResolutionBorderToolStripMenuItem->Checked )
			RenderRectangle(Vector2(0.f,0.f),DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT,Vector4(0,1,1,0.5f),0);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		if( g_pBackgroundImage )
		{
			g_pBackgroundImage->SetColor(Vector4(1,1,1,0.6f));
			g_pBackgroundImage->Render(0,0);
		}
		for(int l_iSelectedIndex=0;l_iSelectedIndex<Layer_listBox->Items->Count;++l_iSelectedIndex)
		//for each (int l_iSelectedIndex in Layer_listBox->Items)
		//for each (int l_iSelectedIndex in Layer_listBox->SelectedIndices)
		{
			g_pCurrentStageEditData->RenderLayer(l_iSelectedIndex);
		}
		if( !ObjectMove_checkBox->Checked )
		{
			if( !Erase_checkBox->Checked )
			if( g_pCurrentSelectedImage )
			{
				Vector3	l_vPos = g_vCurrentPlacePos;
				g_pCurrentSelectedImage->SetColor(Vector4(1,1,1,0.5f));
				if( g_pCurrentSelectedImage->Type() == cPuzzleImageUnit::TypeID )
				{
					cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)g_pCurrentSelectedImage;
					if( PlaceObjectByImageCenter_checkBox->Checked )
					{
						l_vPos.x -= l_pPIUnit->GetPuzzleData()->OriginalSize.x/2.f;
						l_vPos.y -= l_pPIUnit->GetPuzzleData()->OriginalSize.y/2.f;
					}
				}
				else
				{
					if( PlaceObjectByImageCenter_checkBox->Checked )
					{
						l_vPos.x -= *g_pCurrentSelectedImage->GetImageShowWidth()/2.f;
						l_vPos.y -= *g_pCurrentSelectedImage->GetImageShowHeight()/2.f;
					}				
				}
				g_pCurrentSelectedImage->SetPos(l_vPos);
				g_pCurrentSelectedImage->Render();
				g_pCurrentSelectedImage->SetColor(Vector4::One);
			}
		}
		if( m_pFont )
			m_pFont->RenderFont(0,0,ValueToStringW(m_pOrthogonalCamera->GetMouseWorldPos()));
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		if( Erase_checkBox->Checked )
		{
			Vector2	l_vPos2 = m_pOrthogonalCamera->GetMouseWorldPos();
			RenderPoint(l_vPos2,5.f);
		}
		for each (int l_iIndex in StageDataInfo_listBox->SelectedIndices)
		{
			static float l_sfloatColor = 0.f;
			l_sfloatColor+=g_TimeAndFPS.fElpaseTime;
			if( l_sfloatColor>1.f )
				l_sfloatColor = 0.f;
			cPuzzleImageUnit*l_pPuzzleImageUnit = (*g_pCurrentStageEditData->GetCurrentList())[l_iIndex]->GetImage();
			int	l_iWidth = *l_pPuzzleImageUnit->GetImageShowWidth();
			int	l_iHeight = *l_pPuzzleImageUnit->GetImageShowHeight();
			Vector3	l_vPos = (*g_pCurrentStageEditData->GetCurrentList())[l_iIndex]->GetPos();
			RenderRectangle(Vector2(l_vPos.x,l_vPos.y),(float)l_iWidth,(float)l_iHeight,Vector4::One*l_sfloatColor+Vector4(0.5,0,0,0),0);
		}
		//GLRender::glDisable2D();
		SwapBuffers (m_HdcMV);
	}

	System::Void ObjectDataForm::RemoveUserDefineNode(cMap_Cell_ObjectData*e_pMap_Cell_ObjectData)
	{
		if(this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(e_pMap_Cell_ObjectData->GetUniqueID()))
		{
			this->m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(e_pMap_Cell_ObjectData->GetUniqueID());
		}
			
	}

	System::Void ObjectDataForm::RemoveUserDefineNode(cMap_CellData*e_pMap_CellData)
	{
		for( int i=0;i<e_pMap_CellData->Count();++i )
		{
			RemoveUserDefineNode((e_pMap_CellData->GetObject(i)));
		}
	}

	System::Void ObjectDataForm::RemoveUserDefineNode(cMapLayer*e_pMapLayer)
	{
		for( int i=0;i<e_pMapLayer->Count();++i )
		{
			RemoveUserDefineNode((*e_pMapLayer)[i]);
		}
	}

	System::Void ObjectDataForm::CopyUserDataToNewData(cMap_CellData*e_pSrc,cMap_CellData*e_pDest)
	{
		for( int i=0;i<e_pSrc->Count();++i )
		{
			cMapLayer*l_pMapLayer = e_pSrc->GetObject(i);
			for( int j=0;j<l_pMapLayer->Count();++j )
			{
				if(l_pMapLayer->GetObject(j)->IsContainUserDefineData())
				{
					e_pDest->GetObject(i)->GetObject(j)->SetContainUserDefineData(true);
					if(!this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey( l_pMapLayer->GetObject(j)->GetUniqueID() ))
					{
						WARNING_MSG("lost user define data!!!!fuck!!how come!!");
					}
					else
					{
						GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)this->m_UniqueUserDefineDataForMap_Cell_ObjectData[l_pMapLayer->GetObject(j)->GetUniqueID()];
						m_UniqueUserDefineDataForMap_Cell_ObjectData[e_pDest->GetObject(i)->GetObject(j)->GetUniqueID()] = l_pNode->Clone();
					}
				}
			}
		}
	}

	System::Void ObjectDataForm::TemplateDataInvertorReEdit()
	{
		if( Template_listBox->SelectedIndex != -1 )
		{
			String^l_strName = Template_listBox->SelectedItem->ToString();
			GCFORM::TreeNode^l_pNode;
			if(m_TemplateNameAndAttributeNode->ContainsKey(l_strName))
				l_pNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strName];
			xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Clear();
			if( l_pNode )
			{
				xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
				xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes[0]->Expand();
			}
			TemplateName_textBox->Text = l_strName;
			cPuzzleImageUnit*l_pPIUnit = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(l_strName))->GetImage();
            cPuzzleImage*l_pPI = l_pPIUnit->GetParent();
            String^l_strPIName = DNCT::WcharToGcstring(l_pPI->GetName());
            String^l_strPIUnit = DNCT::WcharToGcstring(l_pPIUnit->GetName());
            if( ImageList_treeView->Nodes[l_strPIName] )
            {
			    ImageList_treeView->SelectedNode = ImageList_treeView->Nodes[l_strPIName]->Nodes[l_strPIUnit];
                this->ImageList_treeView->SelectedNode->BackColor = System::Drawing::Color::Blue;
                this->ImageList_treeView->SelectedNode->ForeColor = System::Drawing::Color::White;
			}
			
			 if( !Layer_listBox->Items->Count )
			 {
                GridWidth_numericUpDown->Value = System::Decimal(l_pPIUnit->GetImageWidth());
                GridHeight_numericUpDown->Value = System::Decimal(l_pPIUnit->GetImageHeight());
			 }
			 LayerName_textBox->Text = l_strName;
		}
	}
	void	ObjectDataForm::AddObjectIntoCurrentStage(Vector3 e_vPos)
	{
		if( !Template_listBox->SelectedItem )
			return;
		if( !g_pCurrentStageEditData->GetCurrentList() )
		{
			WARNING_MSG("please add or select layer first!");
			return;
		}

		WCHAR*l_strName = DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString());
		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData(g_pForListBoxTemplateCellData->GetObject(l_strName));
		l_pMap_Cell_ObjectData->SetName(l_strName);
		cPuzzleImageUnit*l_pPIUnit = l_pMap_Cell_ObjectData->GetImage();
		if( PlaceObjectByImageCenter_checkBox->Checked )
		{
			e_vPos.x -= l_pPIUnit->GetPuzzleData()->OriginalSize.x/2.f;
			e_vPos.y -= l_pPIUnit->GetPuzzleData()->OriginalSize.y/2.f;
			//l_pMap_Cell_ObjectData->SetRenderByCenter(true);
		}
		l_pMap_Cell_ObjectData->SetPos(e_vPos);
		g_pCurrentStageEditData->GetCurrentList()->AddObjectNeglectExist(l_pMap_Cell_ObjectData);
		String^l_strImageInfo = g_pCurrentStageEditData->GetCurrentList()->Count().ToString()+":"+Template_listBox->SelectedItem->ToString();
		StageDataInfo_listBox->SelectedItems->Clear();
		StageDataInfo_listBox->Items->Add(l_strImageInfo);
		g_bStageDataChanged = true;
	}

	void	ObjectDataForm::OpenUserDefineData(String^e_strFileName)
	{
		GCFORM::TreeNode^l_pUserDefineTreeNode = DNCT::OpenXmlFileToNode(e_strFileName);
		if( l_pUserDefineTreeNode )
		for each( GCFORM::TreeNode^l_pNode in l_pUserDefineTreeNode->Nodes )
		{
			if(l_pNode->Text->Equals(USER_DEFINE_DATA))
			{
				assert(l_pNode->Nodes->Count == 4);
				cMap_CellData*l_pMap_CellData = 0;
				cMapLayer*l_pMapLayer = 0;
				cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = 0;
				for( int i=0;i<l_pNode->Nodes->Count;++i )
				{
					GCFORM::TreeNode^l_pTargetNode = l_pNode->Nodes[i];
					switch(i)
					{
						case 0:
							l_pMap_CellData = m_pMapDataManager->GetObject(DNCT::GcStringToWchar(l_pTargetNode->Name));
							break;
						case 1:
							l_pMapLayer = l_pMap_CellData->GetObject(DNCT::GcStringToWchar(l_pTargetNode->Name));
							break;
						case 2:
							l_pMap_Cell_ObjectData = l_pMapLayer->GetObject(System::Int16::Parse(l_pTargetNode->Name));
							break;
						case 3:
							l_pMap_Cell_ObjectData->SetContainUserDefineData(true);
							this->m_UniqueUserDefineDataForMap_Cell_ObjectData[l_pMap_Cell_ObjectData->GetUniqueID()] = l_pTargetNode->Clone();
							break;
					}
				}
			}
			else
			{
				WARNING_MSG("user define data error!");
			}
		}
	}
}