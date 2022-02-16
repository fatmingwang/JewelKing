#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
extern float	DRAW_PANEL_RESOLUTION_WIDTH;
extern float	DRAW_PANEL_RESOLUTION_HEIGHT;
//need name to fetch relate xml node to export data,and get image to assign to the new object
extern cMapLayer*g_pForListBoxTemplateCellData;
extern cMap_CellData*g_pCurrentStageEditData;			//for current stage
extern cMap_Cell_ObjectData*g_pCurrentEditUserPropertyObjectData;
extern cBaseImage*g_pCurrentSelectedImage;
extern cBaseImage*g_pBackgroundImage;
extern bool	g_bStageDataChanged;

extern Vector2	g_vCurrentPlacePos;
namespace LevelEditor {

	/// <summary>
	/// ObjectDataForm 的摘要
	/// </summary>
	public ref class ObjectDataForm : public System::Windows::Forms::UserControl
	{
	public:
		ObjectDataForm(GCFORM::TabControl^e_ptabControl,String^e_pTabText,GCFORM::Form^e_pForm)
		{
			InitializeComponent();
			m_pOrthogonalCamera = new cOrthogonalCamera;
			//
			//TODO: 在此加入建構函式程式碼
			//
			GCFORM::TabPage^l_pPage = gcnew GCFORM::TabPage;
			l_pPage->Text = "ObjectDataForm";
			l_pPage->Controls->Add(this);
			e_ptabControl->TabPages->Add(l_pPage);
			m_pForm = e_pForm;
			m_Hwnd = (HWND)Main_splitContainer->Panel1->Handle.ToPointer();
			m_HdcMV = GetDC(m_Hwnd);
			m_HGLRC = UT::InitOpenGL(m_Hwnd,true,m_HdcMV);
			
			Main_splitContainer->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseMove);
			Main_splitContainer->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseDown);
			Main_splitContainer->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseUp);
			Main_splitContainer->Panel1->DoubleClick += gcnew System::EventHandler(this, &ObjectDataForm::MyMouseDoubleClick);
			//make sure u hvae call focus while the panel is mouse down
			Main_splitContainer->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::MyMouseUp);

			e_pForm->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ObjectDataForm::MyKeyPress);

			m_pMapDataManager = new cMapDataManager;
			g_pCurrentStageEditData = new cMap_CellData;
			g_pForListBoxTemplateCellData = m_pMapDataManager->m_pTemplateImage;
			m_UniqueUserDefineDataForMap_Cell_ObjectData = gcnew System::Collections::Hashtable;
			m_TemplateNameAndAttributeNode = gcnew System::Collections::Hashtable;
			this->UserData_xmlForm->groupBox1->Text = "UserDefineData";

			CreateShader(g_bCommonVSClientState,L"ObjectDataFormShader");
			CreateShader(g_bCommonVSNoTextureClientState,L"ObjectDataFormNoTextureShader");
			m_pFont = 0;
			if( System::IO::File::Exists("Font.png") )
				m_pFont = new cGlyphFontRender("Font");
			this->timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~ObjectDataForm()
		{
			DeleteShader(L"ObjectDataFormShader");
			DeleteShader(L"ObjectDataFormNoTextureShader");
			if (components)
			{
				delete components;
			}
			SAFE_DELETE(m_pOrthogonalCamera);
			SAFE_DELETE(m_pMapDataManager);
			SAFE_DELETE(g_pCurrentStageEditData);
			SAFE_DELETE(m_pFont);
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openTemplateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  imageToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  assistanceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showGridToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  autoAllignGridToolStripMenuItem;
	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::ContextMenuStrip^  UserDefineData__contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  refreshToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addNewNodeFromFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteNodeToolStripMenuItem;
	private: System::Windows::Forms::Label^  label1;
	private: DotNetComponent::XmlNodeEditComponent^  xmlNodeEditComponent1;
	private: System::Windows::Forms::ListBox^  AllStage_listBox;
	private: System::Windows::Forms::TextBox^  StageName_textBox;
	private: System::Windows::Forms::Button^  AddStage_button;
	private: System::Windows::Forms::Button^  DelStage_button;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::Button^  ReRditStagebutton;
	private: System::Windows::Forms::ToolStripMenuItem^  camerResetToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showResolutionBorderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  Erase_checkBox;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::NumericUpDown^  StartY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  StartX_numericUpDown;
	private: System::Windows::Forms::Button^  AddToLayer_button;
	private: System::Windows::Forms::Button^  LayerDown_button;
	private: System::Windows::Forms::Button^  LayerUp_button;
	private: System::Windows::Forms::ListBox^  Layer_listBox;
	private: System::Windows::Forms::Button^  DeleteLayer_button;
	private: System::Windows::Forms::ToolStripMenuItem^  autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::CheckBox^  ObjectMove_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraResetToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  StageDataInfo_listBox;
	private: System::Windows::Forms::Button^  DeleteObject_button;
	private: System::Windows::Forms::ToolStripMenuItem^  showWarningMsgToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  ObjectData_textBox;
	private: System::Windows::Forms::TreeView^  ObjectData_treeView;
	private: System::Windows::Forms::TextBox^  NodeName_textBox;
	private: System::Windows::Forms::Button^  SaveObjectData_button;
	private: DotNetComponent::CollpaseForm^  collpaseForm1;
	private: DotNetComponent::CollpaseForm^  collpaseForm3;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::CheckBox^  AllowSamepositionObject_checkBox;
	private: System::Windows::Forms::NumericUpDown^  GridHeight_numericUpDown;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  GridWidth_numericUpDown;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  MapHeight_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  MapWidth_numericUpDown;
	private: DotNetComponent::CollpaseForm^  collpaseForm4;
	private: System::Windows::Forms::Button^  AddObjectToStage_button;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Button^  DelTemplate_button;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::ListBox^  Template_listBox;
	private: System::Windows::Forms::Button^  AddTemplate_button;
	private: System::Windows::Forms::NumericUpDown^  AddObjectY_numericUpDown;
	private: System::Windows::Forms::Button^  DelImage_button;
	private: System::Windows::Forms::NumericUpDown^  AddObjectX_numericUpDown;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  TemplateName_textBox;
	private: System::Windows::Forms::Label^  NumObjectInCurrentStagelabel_label;
	private: System::Windows::Forms::Button^  Clear_button;
	private: System::Windows::Forms::Button^  ReEditTemplate_button;
	private: DotNetComponent::CollpaseForm^  collpaseForm5;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::TextBox^  LayerName_textBox;
	private: System::Windows::Forms::TreeView^  ImageList_treeView;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ContextMenuStrip^  UserDefineData_contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  addToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  editToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  deleteToolStripMenuItem;
	private: System::Windows::Forms::ContextMenuStrip^  LayerData_contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  selectAllThisTypeTemplateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  refreshUserDataFromToolStripMenuItem;
    private: System::Windows::Forms::Button^  ObjectDataChange_button;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::NumericUpDown^  AddObjectZ_numericUpDown;
	private: System::Windows::Forms::Button^  RevertData_button;
	protected://my
			cMapDataManager*m_pMapDataManager;
			GCFORM::Form^m_pForm;
			cOrthogonalCamera	*m_pOrthogonalCamera;
			//String,TreeNodes
			System::Collections::Hashtable^m_TemplateNameAndAttributeNode;
	private: System::Windows::Forms::Button^  RefreshUserDefineData_button;

	private: DotNetComponent::CollpaseForm^  collpaseForm2;
	private: System::Windows::Forms::Button^  SyncUserDefineDataByFile_button;
	private: XMLDataEditor::XMLForm^  UserData_xmlForm;
			 HWND	m_Hwnd;
			 HDC	m_HdcMV;
			 HGLRC	m_HGLRC;
			 cGlyphFontRender*m_pFont;
private: System::Windows::Forms::Timer^  timer1;
private: System::Windows::Forms::SplitContainer^  Main_splitContainer;
private: System::Windows::Forms::CheckBox^  PlaceObjectByImageCenter_checkBox;
		 //object's unique ID with a treenode
			System::Collections::Hashtable^m_UniqueUserDefineDataForMap_Cell_ObjectData;
			void	SaveTreeNodeFile(String^e_strFileName,System::Collections::Hashtable^e_pHashTable,bool e_bUserDefineData);
			//if e_bEraseContiously is true,we won't add object if blank is empty
	private:System::Void AddObjectOnCell();
	private:System::Void MyMouseDoubleClick(System::Object^  sender, System::EventArgs^  e);
	private:System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
			System::Void Render();
			//template data click
			System::Void TemplateDataInvertorReEdit();
			//for user data
			System::Void RemoveUserDefineNode(cMapLayer*e_pMapLayer);
			System::Void RemoveUserDefineNode(cMap_CellData*e_pMap_CellData);
			System::Void RemoveUserDefineNode(cMap_Cell_ObjectData*e_pMap_Cell_ObjectData);
			//while stage data reedited is called,to check the user data is re generate
			System::Void CopyUserDataToNewData(cMap_CellData*e_pSrc,cMap_CellData*e_pDest);
			void	AddObjectIntoCurrentStage(Vector3 e_vPos);
			void	OpenUserDefineData(String^e_strFileName);
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->collpaseForm5 = (gcnew DotNetComponent::CollpaseForm());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->UserDefineData__contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->refreshToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addNewNodeFromFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteNodeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->collpaseForm3 = (gcnew DotNetComponent::CollpaseForm());
			this->xmlNodeEditComponent1 = (gcnew DotNetComponent::XmlNodeEditComponent());
			this->collpaseForm4 = (gcnew DotNetComponent::CollpaseForm());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->AddImage_button = (gcnew System::Windows::Forms::Button());
			this->ImageList_treeView = (gcnew System::Windows::Forms::TreeView());
			this->DelImage_button = (gcnew System::Windows::Forms::Button());
			this->NumObjectInCurrentStagelabel_label = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->DelTemplate_button = (gcnew System::Windows::Forms::Button());
			this->AddTemplate_button = (gcnew System::Windows::Forms::Button());
			this->ReEditTemplate_button = (gcnew System::Windows::Forms::Button());
			this->Template_listBox = (gcnew System::Windows::Forms::ListBox());
			this->TemplateName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->collpaseForm1 = (gcnew DotNetComponent::CollpaseForm());
			this->StageName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->ReRditStagebutton = (gcnew System::Windows::Forms::Button());
			this->AddStage_button = (gcnew System::Windows::Forms::Button());
			this->DelStage_button = (gcnew System::Windows::Forms::Button());
			this->AllStage_listBox = (gcnew System::Windows::Forms::ListBox());
			this->collpaseForm2 = (gcnew DotNetComponent::CollpaseForm());
			this->SyncUserDefineDataByFile_button = (gcnew System::Windows::Forms::Button());
			this->RefreshUserDefineData_button = (gcnew System::Windows::Forms::Button());
			this->UserData_xmlForm = (gcnew XMLDataEditor::XMLForm());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->AddObjectZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StageDataInfo_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DeleteObject_button = (gcnew System::Windows::Forms::Button());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->LayerName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->RevertData_button = (gcnew System::Windows::Forms::Button());
			this->ObjectDataChange_button = (gcnew System::Windows::Forms::Button());
			this->SaveObjectData_button = (gcnew System::Windows::Forms::Button());
			this->NodeName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->ObjectData_textBox = (gcnew System::Windows::Forms::TextBox());
			this->ObjectData_treeView = (gcnew System::Windows::Forms::TreeView());
			this->ObjectMove_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AddObjectToStage_button = (gcnew System::Windows::Forms::Button());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->DeleteLayer_button = (gcnew System::Windows::Forms::Button());
			this->AddToLayer_button = (gcnew System::Windows::Forms::Button());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->LayerDown_button = (gcnew System::Windows::Forms::Button());
			this->LayerUp_button = (gcnew System::Windows::Forms::Button());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->Layer_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->AddObjectY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StartY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->StartX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AddObjectX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Clear_button = (gcnew System::Windows::Forms::Button());
			this->Erase_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->AllowSamepositionObject_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->GridHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->GridWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->MapHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->MapWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Main_splitContainer = (gcnew System::Windows::Forms::SplitContainer());
			this->assistanceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoAllignGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->camerResetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showResolutionBorderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showWarningMsgToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openTemplateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->imageToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraResetToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->UserDefineData_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->addToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->LayerData_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->selectAllThisTypeTemplateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->refreshUserDataFromToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->PlaceObjectByImageCenter_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->collpaseForm5->SuspendLayout();
			this->UserDefineData__contextMenuStrip->SuspendLayout();
			this->collpaseForm3->SuspendLayout();
			this->collpaseForm4->SuspendLayout();
			this->collpaseForm1->SuspendLayout();
			this->collpaseForm2->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectZ_numericUpDown))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridHeight_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridWidth_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapHeight_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapWidth_numericUpDown))->BeginInit();
			this->Main_splitContainer->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->UserDefineData_contextMenuStrip->SuspendLayout();
			this->LayerData_contextMenuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->AllowDrop = true;
			this->splitContainer1->Panel1->AutoScroll = true;
			this->splitContainer1->Panel1->Controls->Add(this->flowLayoutPanel1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(1900, 937);
			this->splitContainer1->SplitterDistance = 210;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 0;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->Controls->Add(this->collpaseForm5);
			this->flowLayoutPanel1->Controls->Add(this->collpaseForm3);
			this->flowLayoutPanel1->Controls->Add(this->collpaseForm4);
			this->flowLayoutPanel1->Controls->Add(this->collpaseForm1);
			this->flowLayoutPanel1->Controls->Add(this->collpaseForm2);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(2);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(208, 935);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// collpaseForm5
			// 
			this->collpaseForm5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->collpaseForm5->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->collpaseForm5->CollpaseButtonName = L"XML Data";
			this->collpaseForm5->Controls->Add(this->treeView1);
			this->collpaseForm5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->collpaseForm5->Location = System::Drawing::Point(2, 2);
			this->collpaseForm5->Margin = System::Windows::Forms::Padding(2);
			this->collpaseForm5->Name = L"collpaseForm5";
			this->collpaseForm5->Size = System::Drawing::Size(207, 388);
			this->collpaseForm5->TabIndex = 44;
			// 
			// treeView1
			// 
			this->treeView1->AllowDrop = true;
			this->treeView1->ContextMenuStrip = this->UserDefineData__contextMenuStrip;
			this->treeView1->Location = System::Drawing::Point(2, 23);
			this->treeView1->Margin = System::Windows::Forms::Padding(2);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(203, 275);
			this->treeView1->TabIndex = 0;
			this->treeView1->DoubleClick += gcnew System::EventHandler(this, &ObjectDataForm::treeView1_DoubleClick);
			this->treeView1->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::treeView1_DragDrop);
			this->treeView1->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &ObjectDataForm::treeView1_DragEnter);
			this->treeView1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ObjectDataForm::treeView1_KeyUp);
			// 
			// UserDefineData__contextMenuStrip
			// 
			this->UserDefineData__contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->refreshToolStripMenuItem, 
				this->addNewNodeFromFileToolStripMenuItem, this->deleteNodeToolStripMenuItem});
			this->UserDefineData__contextMenuStrip->Name = L"contextMenuStrip1";
			this->UserDefineData__contextMenuStrip->Size = System::Drawing::Size(206, 70);
			// 
			// refreshToolStripMenuItem
			// 
			this->refreshToolStripMenuItem->Name = L"refreshToolStripMenuItem";
			this->refreshToolStripMenuItem->Size = System::Drawing::Size(205, 22);
			this->refreshToolStripMenuItem->Text = L"Assign";
			this->refreshToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::refreshToolStripMenuItem_Click);
			// 
			// addNewNodeFromFileToolStripMenuItem
			// 
			this->addNewNodeFromFileToolStripMenuItem->Name = L"addNewNodeFromFileToolStripMenuItem";
			this->addNewNodeFromFileToolStripMenuItem->Size = System::Drawing::Size(205, 22);
			this->addNewNodeFromFileToolStripMenuItem->Text = L"AddNewNodeFromFile";
			this->addNewNodeFromFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::addNewNodeFromFileToolStripMenuItem_Click);
			// 
			// deleteNodeToolStripMenuItem
			// 
			this->deleteNodeToolStripMenuItem->Name = L"deleteNodeToolStripMenuItem";
			this->deleteNodeToolStripMenuItem->Size = System::Drawing::Size(205, 22);
			this->deleteNodeToolStripMenuItem->Text = L"DeleteNode";
			this->deleteNodeToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::deleteNodeToolStripMenuItem_Click);
			// 
			// collpaseForm3
			// 
			this->collpaseForm3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->collpaseForm3->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->collpaseForm3->CollpaseButtonName = L"TemplateUserDefineData/範本定義資料";
			this->collpaseForm3->Controls->Add(this->xmlNodeEditComponent1);
			this->collpaseForm3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->collpaseForm3->Location = System::Drawing::Point(2, 394);
			this->collpaseForm3->Margin = System::Windows::Forms::Padding(2);
			this->collpaseForm3->Name = L"collpaseForm3";
			this->collpaseForm3->Size = System::Drawing::Size(208, 388);
			this->collpaseForm3->TabIndex = 42;
			// 
			// xmlNodeEditComponent1
			// 
			this->xmlNodeEditComponent1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->xmlNodeEditComponent1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->xmlNodeEditComponent1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->xmlNodeEditComponent1->Location = System::Drawing::Point(0, 24);
			this->xmlNodeEditComponent1->Margin = System::Windows::Forms::Padding(0);
			this->xmlNodeEditComponent1->Name = L"xmlNodeEditComponent1";
			this->xmlNodeEditComponent1->Size = System::Drawing::Size(194, 274);
			this->xmlNodeEditComponent1->TabIndex = 24;
			// 
			// collpaseForm4
			// 
			this->collpaseForm4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->collpaseForm4->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->collpaseForm4->CollpaseButtonName = L"TemplateList/範本列表";
			this->collpaseForm4->Controls->Add(this->label12);
			this->collpaseForm4->Controls->Add(this->label1);
			this->collpaseForm4->Controls->Add(this->AddImage_button);
			this->collpaseForm4->Controls->Add(this->ImageList_treeView);
			this->collpaseForm4->Controls->Add(this->DelImage_button);
			this->collpaseForm4->Controls->Add(this->NumObjectInCurrentStagelabel_label);
			this->collpaseForm4->Controls->Add(this->label9);
			this->collpaseForm4->Controls->Add(this->DelTemplate_button);
			this->collpaseForm4->Controls->Add(this->AddTemplate_button);
			this->collpaseForm4->Controls->Add(this->ReEditTemplate_button);
			this->collpaseForm4->Controls->Add(this->Template_listBox);
			this->collpaseForm4->Controls->Add(this->TemplateName_textBox);
			this->collpaseForm4->Controls->Add(this->label5);
			this->collpaseForm4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->collpaseForm4->Location = System::Drawing::Point(2, 786);
			this->collpaseForm4->Margin = System::Windows::Forms::Padding(2);
			this->collpaseForm4->Name = L"collpaseForm4";
			this->collpaseForm4->Size = System::Drawing::Size(207, 475);
			this->collpaseForm4->TabIndex = 43;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"PMingLiU", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label12->Location = System::Drawing::Point(219, 412);
			this->label12->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(0, 22);
			this->label12->TabIndex = 51;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(2, 23);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(51, 12);
			this->label1->TabIndex = 24;
			this->label1->Text = L"ImageList";
			// 
			// AddImage_button
			// 
			this->AddImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddImage_button->Location = System::Drawing::Point(37, 188);
			this->AddImage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(58, 22);
			this->AddImage_button->TabIndex = 23;
			this->AddImage_button->Text = L"Add/增加";
			this->AddImage_button->UseVisualStyleBackColor = false;
			this->AddImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddImage_button_Click);
			// 
			// ImageList_treeView
			// 
			this->ImageList_treeView->Location = System::Drawing::Point(2, 38);
			this->ImageList_treeView->Name = L"ImageList_treeView";
			this->ImageList_treeView->Size = System::Drawing::Size(194, 145);
			this->ImageList_treeView->TabIndex = 60;
			this->ImageList_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &ObjectDataForm::ImageList_treeView_AfterSelect);
			this->ImageList_treeView->BeforeSelect += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &ObjectDataForm::ImageList_treeView_BeforeSelect);
			// 
			// DelImage_button
			// 
			this->DelImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DelImage_button->Location = System::Drawing::Point(102, 188);
			this->DelImage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelImage_button->Name = L"DelImage_button";
			this->DelImage_button->Size = System::Drawing::Size(58, 22);
			this->DelImage_button->TabIndex = 22;
			this->DelImage_button->Text = L"Del/移除";
			this->DelImage_button->UseVisualStyleBackColor = false;
			this->DelImage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddImage_button_Click);
			// 
			// NumObjectInCurrentStagelabel_label
			// 
			this->NumObjectInCurrentStagelabel_label->AutoSize = true;
			this->NumObjectInCurrentStagelabel_label->Font = (gcnew System::Drawing::Font(L"PMingLiU", 16.2F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(136)));
			this->NumObjectInCurrentStagelabel_label->Location = System::Drawing::Point(419, 220);
			this->NumObjectInCurrentStagelabel_label->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->NumObjectInCurrentStagelabel_label->Name = L"NumObjectInCurrentStagelabel_label";
			this->NumObjectInCurrentStagelabel_label->Size = System::Drawing::Size(0, 22);
			this->NumObjectInCurrentStagelabel_label->TabIndex = 37;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(64, 218);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(0, 12);
			this->label9->TabIndex = 61;
			// 
			// DelTemplate_button
			// 
			this->DelTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->DelTemplate_button->Location = System::Drawing::Point(129, 315);
			this->DelTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->DelTemplate_button->Name = L"DelTemplate_button";
			this->DelTemplate_button->Size = System::Drawing::Size(70, 22);
			this->DelTemplate_button->TabIndex = 16;
			this->DelTemplate_button->Text = L"Del";
			this->DelTemplate_button->UseVisualStyleBackColor = false;
			this->DelTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddTemplate_button_Click);
			// 
			// AddTemplate_button
			// 
			this->AddTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->AddTemplate_button->Location = System::Drawing::Point(129, 270);
			this->AddTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->AddTemplate_button->Name = L"AddTemplate_button";
			this->AddTemplate_button->Size = System::Drawing::Size(70, 22);
			this->AddTemplate_button->TabIndex = 17;
			this->AddTemplate_button->Text = L"Add";
			this->AddTemplate_button->UseVisualStyleBackColor = false;
			this->AddTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddTemplate_button_Click);
			// 
			// ReEditTemplate_button
			// 
			this->ReEditTemplate_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->ReEditTemplate_button->Location = System::Drawing::Point(129, 359);
			this->ReEditTemplate_button->Margin = System::Windows::Forms::Padding(2);
			this->ReEditTemplate_button->Name = L"ReEditTemplate_button";
			this->ReEditTemplate_button->Size = System::Drawing::Size(70, 22);
			this->ReEditTemplate_button->TabIndex = 39;
			this->ReEditTemplate_button->Text = L"ReEdit";
			this->ReEditTemplate_button->UseVisualStyleBackColor = false;
			this->ReEditTemplate_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::ReEditTemplate_button_Click);
			// 
			// Template_listBox
			// 
			this->Template_listBox->FormattingEnabled = true;
			this->Template_listBox->ItemHeight = 12;
			this->Template_listBox->Location = System::Drawing::Point(2, 234);
			this->Template_listBox->Margin = System::Windows::Forms::Padding(2);
			this->Template_listBox->Name = L"Template_listBox";
			this->Template_listBox->Size = System::Drawing::Size(121, 148);
			this->Template_listBox->Sorted = true;
			this->Template_listBox->TabIndex = 14;
			this->Template_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::Template_listBox_SelectedIndexChanged);
			// 
			// TemplateName_textBox
			// 
			this->TemplateName_textBox->Location = System::Drawing::Point(128, 236);
			this->TemplateName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->TemplateName_textBox->Name = L"TemplateName_textBox";
			this->TemplateName_textBox->Size = System::Drawing::Size(69, 22);
			this->TemplateName_textBox->TabIndex = 10;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Enabled = false;
			this->label5->Location = System::Drawing::Point(127, 220);
			this->label5->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(75, 12);
			this->label5->TabIndex = 13;
			this->label5->Text = L"TemplateName";
			// 
			// collpaseForm1
			// 
			this->collpaseForm1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(144)), static_cast<System::Int32>(static_cast<System::Byte>(144)), 
				static_cast<System::Int32>(static_cast<System::Byte>(144)));
			this->collpaseForm1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->collpaseForm1->CollpaseButtonName = L"StageData";
			this->collpaseForm1->Controls->Add(this->StageName_textBox);
			this->collpaseForm1->Controls->Add(this->ReRditStagebutton);
			this->collpaseForm1->Controls->Add(this->AddStage_button);
			this->collpaseForm1->Controls->Add(this->DelStage_button);
			this->collpaseForm1->Controls->Add(this->AllStage_listBox);
			this->collpaseForm1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->collpaseForm1->Location = System::Drawing::Point(2, 1265);
			this->collpaseForm1->Margin = System::Windows::Forms::Padding(2);
			this->collpaseForm1->Name = L"collpaseForm1";
			this->collpaseForm1->Size = System::Drawing::Size(207, 475);
			this->collpaseForm1->TabIndex = 1;
			// 
			// StageName_textBox
			// 
			this->StageName_textBox->Location = System::Drawing::Point(2, 24);
			this->StageName_textBox->Margin = System::Windows::Forms::Padding(2);
			this->StageName_textBox->Name = L"StageName_textBox";
			this->StageName_textBox->Size = System::Drawing::Size(196, 22);
			this->StageName_textBox->TabIndex = 36;
			// 
			// ReRditStagebutton
			// 
			this->ReRditStagebutton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->ReRditStagebutton->Location = System::Drawing::Point(128, 50);
			this->ReRditStagebutton->Margin = System::Windows::Forms::Padding(2);
			this->ReRditStagebutton->Name = L"ReRditStagebutton";
			this->ReRditStagebutton->Size = System::Drawing::Size(68, 22);
			this->ReRditStagebutton->TabIndex = 39;
			this->ReRditStagebutton->Text = L"Reedit";
			this->ReRditStagebutton->UseVisualStyleBackColor = false;
			this->ReRditStagebutton->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// AddStage_button
			// 
			this->AddStage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AddStage_button->Location = System::Drawing::Point(2, 50);
			this->AddStage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddStage_button->Name = L"AddStage_button";
			this->AddStage_button->Size = System::Drawing::Size(58, 22);
			this->AddStage_button->TabIndex = 38;
			this->AddStage_button->Text = L"Add";
			this->AddStage_button->UseVisualStyleBackColor = false;
			this->AddStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// DelStage_button
			// 
			this->DelStage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->DelStage_button->Location = System::Drawing::Point(65, 50);
			this->DelStage_button->Margin = System::Windows::Forms::Padding(2);
			this->DelStage_button->Name = L"DelStage_button";
			this->DelStage_button->Size = System::Drawing::Size(58, 22);
			this->DelStage_button->TabIndex = 37;
			this->DelStage_button->Text = L"Del";
			this->DelStage_button->UseVisualStyleBackColor = false;
			this->DelStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddStage_button_Click);
			// 
			// AllStage_listBox
			// 
			this->AllStage_listBox->FormattingEnabled = true;
			this->AllStage_listBox->ItemHeight = 12;
			this->AllStage_listBox->Location = System::Drawing::Point(2, 78);
			this->AllStage_listBox->Margin = System::Windows::Forms::Padding(2);
			this->AllStage_listBox->Name = L"AllStage_listBox";
			this->AllStage_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllStage_listBox->Size = System::Drawing::Size(203, 304);
			this->AllStage_listBox->Sorted = true;
			this->AllStage_listBox->TabIndex = 35;
			this->AllStage_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &ObjectDataForm::AllStage_listBox_MouseDoubleClick);
			this->AllStage_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::AllStage_listBox_SelectedIndexChanged);
			this->AllStage_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ObjectDataForm::AllStage_listBox_KeyUp);
			// 
			// collpaseForm2
			// 
			this->collpaseForm2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->collpaseForm2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->collpaseForm2->CollpaseButtonName = L"UserDefineData";
			this->collpaseForm2->Controls->Add(this->SyncUserDefineDataByFile_button);
			this->collpaseForm2->Controls->Add(this->RefreshUserDefineData_button);
			this->collpaseForm2->Controls->Add(this->UserData_xmlForm);
			this->collpaseForm2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->collpaseForm2->Location = System::Drawing::Point(2, 1744);
			this->collpaseForm2->Margin = System::Windows::Forms::Padding(2);
			this->collpaseForm2->Name = L"collpaseForm2";
			this->collpaseForm2->Size = System::Drawing::Size(479, 464);
			this->collpaseForm2->TabIndex = 1;
			// 
			// SyncUserDefineDataByFile_button
			// 
			this->SyncUserDefineDataByFile_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->SyncUserDefineDataByFile_button->Location = System::Drawing::Point(231, 352);
			this->SyncUserDefineDataByFile_button->Margin = System::Windows::Forms::Padding(2);
			this->SyncUserDefineDataByFile_button->Name = L"SyncUserDefineDataByFile_button";
			this->SyncUserDefineDataByFile_button->Size = System::Drawing::Size(217, 19);
			this->SyncUserDefineDataByFile_button->TabIndex = 69;
			this->SyncUserDefineDataByFile_button->Text = L"SyncUserDefineDataByFile";
			this->SyncUserDefineDataByFile_button->UseVisualStyleBackColor = false;
			this->SyncUserDefineDataByFile_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::SyncUserDefineDataByFile_button_Click);
			// 
			// RefreshUserDefineData_button
			// 
			this->RefreshUserDefineData_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->RefreshUserDefineData_button->Location = System::Drawing::Point(10, 352);
			this->RefreshUserDefineData_button->Margin = System::Windows::Forms::Padding(2);
			this->RefreshUserDefineData_button->Name = L"RefreshUserDefineData_button";
			this->RefreshUserDefineData_button->Size = System::Drawing::Size(217, 19);
			this->RefreshUserDefineData_button->TabIndex = 68;
			this->RefreshUserDefineData_button->Text = L"RefreshUserDefineData";
			this->RefreshUserDefineData_button->UseVisualStyleBackColor = false;
			this->RefreshUserDefineData_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::RefreshUserDefineData_button_Click);
			// 
			// UserData_xmlForm
			// 
			this->UserData_xmlForm->AllowDrop = true;
			this->UserData_xmlForm->CollpaseButtonName = L"XMLData";
			this->UserData_xmlForm->Location = System::Drawing::Point(7, 23);
			this->UserData_xmlForm->Margin = System::Windows::Forms::Padding(0);
			this->UserData_xmlForm->Name = L"UserData_xmlForm";
			this->UserData_xmlForm->Size = System::Drawing::Size(456, 323);
			this->UserData_xmlForm->TabIndex = 45;
			// 
			// splitContainer2
			// 
			this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->PlaceObjectByImageCenter_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->label14);
			this->splitContainer2->Panel1->Controls->Add(this->AddObjectZ_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->StageDataInfo_listBox);
			this->splitContainer2->Panel1->Controls->Add(this->DeleteObject_button);
			this->splitContainer2->Panel1->Controls->Add(this->label17);
			this->splitContainer2->Panel1->Controls->Add(this->LayerName_textBox);
			this->splitContainer2->Panel1->Controls->Add(this->groupBox1);
			this->splitContainer2->Panel1->Controls->Add(this->ObjectMove_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->AddObjectToStage_button);
			this->splitContainer2->Panel1->Controls->Add(this->label11);
			this->splitContainer2->Panel1->Controls->Add(this->DeleteLayer_button);
			this->splitContainer2->Panel1->Controls->Add(this->AddToLayer_button);
			this->splitContainer2->Panel1->Controls->Add(this->label15);
			this->splitContainer2->Panel1->Controls->Add(this->LayerDown_button);
			this->splitContainer2->Panel1->Controls->Add(this->LayerUp_button);
			this->splitContainer2->Panel1->Controls->Add(this->label13);
			this->splitContainer2->Panel1->Controls->Add(this->Layer_listBox);
			this->splitContainer2->Panel1->Controls->Add(this->label10);
			this->splitContainer2->Panel1->Controls->Add(this->label4);
			this->splitContainer2->Panel1->Controls->Add(this->AddObjectY_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->StartY_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->StartX_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->AddObjectX_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->Clear_button);
			this->splitContainer2->Panel1->Controls->Add(this->Erase_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->label3);
			this->splitContainer2->Panel1->Controls->Add(this->AllowSamepositionObject_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->GridHeight_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label6);
			this->splitContainer2->Panel1->Controls->Add(this->GridWidth_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label7);
			this->splitContainer2->Panel1->Controls->Add(this->label2);
			this->splitContainer2->Panel1->Controls->Add(this->MapHeight_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->MapWidth_numericUpDown);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->Main_splitContainer);
			this->splitContainer2->Size = System::Drawing::Size(1685, 935);
			this->splitContainer2->SplitterDistance = 238;
			this->splitContainer2->SplitterWidth = 2;
			this->splitContainer2->TabIndex = 0;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(146, 62);
			this->label14->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(10, 12);
			this->label14->TabIndex = 65;
			this->label14->Text = L"z";
			// 
			// AddObjectZ_numericUpDown
			// 
			this->AddObjectZ_numericUpDown->Location = System::Drawing::Point(161, 60);
			this->AddObjectZ_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectZ_numericUpDown->Name = L"AddObjectZ_numericUpDown";
			this->AddObjectZ_numericUpDown->Size = System::Drawing::Size(47, 22);
			this->AddObjectZ_numericUpDown->TabIndex = 64;
			// 
			// StageDataInfo_listBox
			// 
			this->StageDataInfo_listBox->FormattingEnabled = true;
			this->StageDataInfo_listBox->HorizontalScrollbar = true;
			this->StageDataInfo_listBox->ItemHeight = 12;
			this->StageDataInfo_listBox->Location = System::Drawing::Point(538, 20);
			this->StageDataInfo_listBox->Margin = System::Windows::Forms::Padding(2);
			this->StageDataInfo_listBox->Name = L"StageDataInfo_listBox";
			this->StageDataInfo_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->StageDataInfo_listBox->Size = System::Drawing::Size(155, 172);
			this->StageDataInfo_listBox->TabIndex = 0;
			this->StageDataInfo_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::StageDataInfo_listBox_SelectedIndexChanged);
			// 
			// DeleteObject_button
			// 
			this->DeleteObject_button->Location = System::Drawing::Point(538, 201);
			this->DeleteObject_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteObject_button->Name = L"DeleteObject_button";
			this->DeleteObject_button->Size = System::Drawing::Size(77, 21);
			this->DeleteObject_button->TabIndex = 1;
			this->DeleteObject_button->Text = L"Delete";
			this->DeleteObject_button->UseVisualStyleBackColor = true;
			this->DeleteObject_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::DeleteObject_button_Click);
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(220, 10);
			this->label17->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(62, 12);
			this->label17->TabIndex = 63;
			this->label17->Text = L"Layer Name";
			// 
			// LayerName_textBox
			// 
			this->LayerName_textBox->Location = System::Drawing::Point(221, 25);
			this->LayerName_textBox->Name = L"LayerName_textBox";
			this->LayerName_textBox->Size = System::Drawing::Size(100, 22);
			this->LayerName_textBox->TabIndex = 62;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->RevertData_button);
			this->groupBox1->Controls->Add(this->ObjectDataChange_button);
			this->groupBox1->Controls->Add(this->SaveObjectData_button);
			this->groupBox1->Controls->Add(this->NodeName_textBox);
			this->groupBox1->Controls->Add(this->ObjectData_textBox);
			this->groupBox1->Controls->Add(this->ObjectData_treeView);
			this->groupBox1->Location = System::Drawing::Point(711, 13);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(262, 208);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"ObjectData";
			// 
			// RevertData_button
			// 
			this->RevertData_button->Location = System::Drawing::Point(146, 142);
			this->RevertData_button->Name = L"RevertData_button";
			this->RevertData_button->Size = System::Drawing::Size(103, 19);
			this->RevertData_button->TabIndex = 10;
			this->RevertData_button->Text = L"revert data/回復";
			this->RevertData_button->UseVisualStyleBackColor = true;
			this->RevertData_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::SaveObjectData_button_Click);
			// 
			// ObjectDataChange_button
			// 
			this->ObjectDataChange_button->Location = System::Drawing::Point(148, 85);
			this->ObjectDataChange_button->Name = L"ObjectDataChange_button";
			this->ObjectDataChange_button->Size = System::Drawing::Size(103, 19);
			this->ObjectDataChange_button->TabIndex = 9;
			this->ObjectDataChange_button->Text = L"ObjectDataChange";
			this->ObjectDataChange_button->UseVisualStyleBackColor = true;
			this->ObjectDataChange_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::DataChange_button_Click);
			// 
			// SaveObjectData_button
			// 
			this->SaveObjectData_button->Location = System::Drawing::Point(148, 173);
			this->SaveObjectData_button->Name = L"SaveObjectData_button";
			this->SaveObjectData_button->Size = System::Drawing::Size(103, 19);
			this->SaveObjectData_button->TabIndex = 8;
			this->SaveObjectData_button->Text = L"Save";
			this->SaveObjectData_button->UseVisualStyleBackColor = true;
			this->SaveObjectData_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::SaveObjectData_button_Click);
			// 
			// NodeName_textBox
			// 
			this->NodeName_textBox->Enabled = false;
			this->NodeName_textBox->Location = System::Drawing::Point(148, 19);
			this->NodeName_textBox->Name = L"NodeName_textBox";
			this->NodeName_textBox->Size = System::Drawing::Size(103, 22);
			this->NodeName_textBox->TabIndex = 4;
			// 
			// ObjectData_textBox
			// 
			this->ObjectData_textBox->Location = System::Drawing::Point(148, 54);
			this->ObjectData_textBox->Name = L"ObjectData_textBox";
			this->ObjectData_textBox->Size = System::Drawing::Size(103, 22);
			this->ObjectData_textBox->TabIndex = 3;
			// 
			// ObjectData_treeView
			// 
			this->ObjectData_treeView->Location = System::Drawing::Point(6, 13);
			this->ObjectData_treeView->Name = L"ObjectData_treeView";
			this->ObjectData_treeView->Size = System::Drawing::Size(134, 185);
			this->ObjectData_treeView->TabIndex = 0;
			this->ObjectData_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &ObjectDataForm::ObjectData_treeView_AfterSelect);
			// 
			// ObjectMove_checkBox
			// 
			this->ObjectMove_checkBox->AutoSize = true;
			this->ObjectMove_checkBox->Location = System::Drawing::Point(148, 144);
			this->ObjectMove_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->ObjectMove_checkBox->Name = L"ObjectMove_checkBox";
			this->ObjectMove_checkBox->Size = System::Drawing::Size(81, 16);
			this->ObjectMove_checkBox->TabIndex = 52;
			this->ObjectMove_checkBox->Text = L"MoveObject";
			this->ObjectMove_checkBox->UseVisualStyleBackColor = true;
			this->ObjectMove_checkBox->CheckedChanged += gcnew System::EventHandler(this, &ObjectDataForm::ObjectMove_checkBox_CheckedChanged);
			// 
			// AddObjectToStage_button
			// 
			this->AddObjectToStage_button->Location = System::Drawing::Point(144, 87);
			this->AddObjectToStage_button->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectToStage_button->Name = L"AddObjectToStage_button";
			this->AddObjectToStage_button->Size = System::Drawing::Size(64, 22);
			this->AddObjectToStage_button->TabIndex = 60;
			this->AddObjectToStage_button->Text = L"AddObject";
			this->AddObjectToStage_button->UseVisualStyleBackColor = true;
			this->AddObjectToStage_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddObjectToStage_button_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(331, 7);
			this->label11->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(52, 12);
			this->label11->TabIndex = 50;
			this->label11->Text = L"Layer List";
			// 
			// DeleteLayer_button
			// 
			this->DeleteLayer_button->Location = System::Drawing::Point(222, 87);
			this->DeleteLayer_button->Margin = System::Windows::Forms::Padding(2);
			this->DeleteLayer_button->Name = L"DeleteLayer_button";
			this->DeleteLayer_button->Size = System::Drawing::Size(99, 22);
			this->DeleteLayer_button->TabIndex = 49;
			this->DeleteLayer_button->Text = L"DeleteLayer";
			this->DeleteLayer_button->UseVisualStyleBackColor = true;
			this->DeleteLayer_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddToLayer_button_Click);
			// 
			// AddToLayer_button
			// 
			this->AddToLayer_button->Location = System::Drawing::Point(221, 52);
			this->AddToLayer_button->Margin = System::Windows::Forms::Padding(2);
			this->AddToLayer_button->Name = L"AddToLayer_button";
			this->AddToLayer_button->Size = System::Drawing::Size(100, 22);
			this->AddToLayer_button->TabIndex = 48;
			this->AddToLayer_button->Text = L"AddToLayer";
			this->AddToLayer_button->UseVisualStyleBackColor = true;
			this->AddToLayer_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::AddToLayer_button_Click);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(147, 37);
			this->label15->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(11, 12);
			this->label15->TabIndex = 58;
			this->label15->Text = L"y";
			// 
			// LayerDown_button
			// 
			this->LayerDown_button->Location = System::Drawing::Point(503, 63);
			this->LayerDown_button->Margin = System::Windows::Forms::Padding(2);
			this->LayerDown_button->Name = L"LayerDown_button";
			this->LayerDown_button->Size = System::Drawing::Size(20, 34);
			this->LayerDown_button->TabIndex = 47;
			this->LayerDown_button->Text = L"v\r\nv";
			this->LayerDown_button->UseVisualStyleBackColor = true;
			this->LayerDown_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::LayerDown_button_Click);
			// 
			// LayerUp_button
			// 
			this->LayerUp_button->Location = System::Drawing::Point(504, 20);
			this->LayerUp_button->Margin = System::Windows::Forms::Padding(2);
			this->LayerUp_button->Name = L"LayerUp_button";
			this->LayerUp_button->Size = System::Drawing::Size(20, 39);
			this->LayerUp_button->TabIndex = 46;
			this->LayerUp_button->Text = L"^\r\n^";
			this->LayerUp_button->UseVisualStyleBackColor = true;
			this->LayerUp_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::LayerDown_button_Click);
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(147, 10);
			this->label13->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(11, 12);
			this->label13->TabIndex = 56;
			this->label13->Text = L"x";
			// 
			// Layer_listBox
			// 
			this->Layer_listBox->FormattingEnabled = true;
			this->Layer_listBox->ItemHeight = 12;
			this->Layer_listBox->Location = System::Drawing::Point(329, 20);
			this->Layer_listBox->Margin = System::Windows::Forms::Padding(2);
			this->Layer_listBox->Name = L"Layer_listBox";
			this->Layer_listBox->Size = System::Drawing::Size(171, 172);
			this->Layer_listBox->TabIndex = 45;
			this->Layer_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ObjectDataForm::Layer_listBox_SelectedIndexChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Enabled = false;
			this->label10->Location = System::Drawing::Point(9, 44);
			this->label10->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(34, 12);
			this->label10->TabIndex = 44;
			this->label10->Text = L"StartY";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Enabled = false;
			this->label4->Location = System::Drawing::Point(10, 20);
			this->label4->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(34, 12);
			this->label4->TabIndex = 43;
			this->label4->Text = L"StartX";
			// 
			// AddObjectY_numericUpDown
			// 
			this->AddObjectY_numericUpDown->Location = System::Drawing::Point(162, 35);
			this->AddObjectY_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectY_numericUpDown->Name = L"AddObjectY_numericUpDown";
			this->AddObjectY_numericUpDown->Size = System::Drawing::Size(47, 22);
			this->AddObjectY_numericUpDown->TabIndex = 55;
			// 
			// StartY_numericUpDown
			// 
			this->StartY_numericUpDown->Location = System::Drawing::Point(65, 37);
			this->StartY_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StartY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, System::Int32::MinValue});
			this->StartY_numericUpDown->Name = L"StartY_numericUpDown";
			this->StartY_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->StartY_numericUpDown->TabIndex = 42;
			this->StartY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::StartX_numericUpDown_ValueChanged);
			// 
			// StartX_numericUpDown
			// 
			this->StartX_numericUpDown->Location = System::Drawing::Point(65, 10);
			this->StartX_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->StartX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, 0});
			this->StartX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000, 0, 0, System::Int32::MinValue});
			this->StartX_numericUpDown->Name = L"StartX_numericUpDown";
			this->StartX_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->StartX_numericUpDown->TabIndex = 41;
			this->StartX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::StartX_numericUpDown_ValueChanged);
			// 
			// AddObjectX_numericUpDown
			// 
			this->AddObjectX_numericUpDown->Location = System::Drawing::Point(162, 10);
			this->AddObjectX_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->AddObjectX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->AddObjectX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, System::Int32::MinValue});
			this->AddObjectX_numericUpDown->Name = L"AddObjectX_numericUpDown";
			this->AddObjectX_numericUpDown->Size = System::Drawing::Size(47, 22);
			this->AddObjectX_numericUpDown->TabIndex = 53;
			// 
			// Clear_button
			// 
			this->Clear_button->Location = System::Drawing::Point(623, 201);
			this->Clear_button->Margin = System::Windows::Forms::Padding(2);
			this->Clear_button->Name = L"Clear_button";
			this->Clear_button->Size = System::Drawing::Size(70, 22);
			this->Clear_button->TabIndex = 38;
			this->Clear_button->Text = L"Clear";
			this->Clear_button->UseVisualStyleBackColor = true;
			this->Clear_button->Click += gcnew System::EventHandler(this, &ObjectDataForm::Clear_button_Click);
			// 
			// Erase_checkBox
			// 
			this->Erase_checkBox->AutoSize = true;
			this->Erase_checkBox->Location = System::Drawing::Point(149, 117);
			this->Erase_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->Erase_checkBox->Name = L"Erase_checkBox";
			this->Erase_checkBox->Size = System::Drawing::Size(79, 16);
			this->Erase_checkBox->TabIndex = 40;
			this->Erase_checkBox->Text = L"EraseObject";
			this->Erase_checkBox->UseVisualStyleBackColor = true;
			this->Erase_checkBox->CheckedChanged += gcnew System::EventHandler(this, &ObjectDataForm::Erase_checkBox_CheckedChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Enabled = false;
			this->label3->Location = System::Drawing::Point(9, 67);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 12);
			this->label3->TabIndex = 28;
			this->label3->Text = L"GridWidth";
			// 
			// AllowSamepositionObject_checkBox
			// 
			this->AllowSamepositionObject_checkBox->AutoSize = true;
			this->AllowSamepositionObject_checkBox->Location = System::Drawing::Point(11, 185);
			this->AllowSamepositionObject_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->AllowSamepositionObject_checkBox->Name = L"AllowSamepositionObject_checkBox";
			this->AllowSamepositionObject_checkBox->Size = System::Drawing::Size(144, 16);
			this->AllowSamepositionObject_checkBox->TabIndex = 36;
			this->AllowSamepositionObject_checkBox->Text = L"AllowSamepositionObject";
			this->AllowSamepositionObject_checkBox->UseVisualStyleBackColor = true;
			// 
			// GridHeight_numericUpDown
			// 
			this->GridHeight_numericUpDown->Location = System::Drawing::Point(65, 95);
			this->GridHeight_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->GridHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GridHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridHeight_numericUpDown->Name = L"GridHeight_numericUpDown";
			this->GridHeight_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->GridHeight_numericUpDown->TabIndex = 25;
			this->GridHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->GridHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::GridHeight_numericUpDown_ValueChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Enabled = false;
			this->label6->Location = System::Drawing::Point(9, 127);
			this->label6->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(55, 12);
			this->label6->TabIndex = 32;
			this->label6->Text = L"MapWidth";
			// 
			// GridWidth_numericUpDown
			// 
			this->GridWidth_numericUpDown->Location = System::Drawing::Point(65, 65);
			this->GridWidth_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->GridWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {99999, 0, 0, 0});
			this->GridWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GridWidth_numericUpDown->Name = L"GridWidth_numericUpDown";
			this->GridWidth_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->GridWidth_numericUpDown->TabIndex = 26;
			this->GridWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->GridWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::GridHeight_numericUpDown_ValueChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Enabled = false;
			this->label7->Location = System::Drawing::Point(9, 155);
			this->label7->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(57, 12);
			this->label7->TabIndex = 31;
			this->label7->Text = L"MapHeight";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Enabled = false;
			this->label2->Location = System::Drawing::Point(9, 99);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(57, 12);
			this->label2->TabIndex = 27;
			this->label2->Text = L"GridHeight";
			// 
			// MapHeight_numericUpDown
			// 
			this->MapHeight_numericUpDown->Location = System::Drawing::Point(65, 155);
			this->MapHeight_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->MapHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MapHeight_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MapHeight_numericUpDown->Name = L"MapHeight_numericUpDown";
			this->MapHeight_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->MapHeight_numericUpDown->TabIndex = 30;
			this->MapHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {320, 0, 0, 0});
			this->MapHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::Row_numericUpDown_ValueChanged);
			// 
			// MapWidth_numericUpDown
			// 
			this->MapWidth_numericUpDown->Location = System::Drawing::Point(65, 123);
			this->MapWidth_numericUpDown->Margin = System::Windows::Forms::Padding(2);
			this->MapWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->MapWidth_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MapWidth_numericUpDown->Name = L"MapWidth_numericUpDown";
			this->MapWidth_numericUpDown->Size = System::Drawing::Size(65, 22);
			this->MapWidth_numericUpDown->TabIndex = 29;
			this->MapWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {480, 0, 0, 0});
			this->MapWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ObjectDataForm::Row_numericUpDown_ValueChanged);
			// 
			// Main_splitContainer
			// 
			this->Main_splitContainer->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Main_splitContainer->Location = System::Drawing::Point(0, 0);
			this->Main_splitContainer->Name = L"Main_splitContainer";
			this->Main_splitContainer->Panel2Collapsed = true;
			this->Main_splitContainer->Size = System::Drawing::Size(1681, 691);
			this->Main_splitContainer->SplitterDistance = 560;
			this->Main_splitContainer->TabIndex = 0;
			// 
			// assistanceToolStripMenuItem
			// 
			this->assistanceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->showGridToolStripMenuItem, 
				this->autoAllignGridToolStripMenuItem, this->camerResetToolStripMenuItem, this->showResolutionBorderToolStripMenuItem, this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem, 
				this->showWarningMsgToolStripMenuItem});
			this->assistanceToolStripMenuItem->Name = L"assistanceToolStripMenuItem";
			this->assistanceToolStripMenuItem->Size = System::Drawing::Size(76, 20);
			this->assistanceToolStripMenuItem->Text = L"Assistance";
			// 
			// showGridToolStripMenuItem
			// 
			this->showGridToolStripMenuItem->Checked = true;
			this->showGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showGridToolStripMenuItem->Name = L"showGridToolStripMenuItem";
			this->showGridToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->showGridToolStripMenuItem->Text = L"ShowGrid";
			this->showGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// autoAllignGridToolStripMenuItem
			// 
			this->autoAllignGridToolStripMenuItem->Checked = true;
			this->autoAllignGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->autoAllignGridToolStripMenuItem->Name = L"autoAllignGridToolStripMenuItem";
			this->autoAllignGridToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->autoAllignGridToolStripMenuItem->Text = L"AutoAllignGrid";
			this->autoAllignGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// camerResetToolStripMenuItem
			// 
			this->camerResetToolStripMenuItem->Name = L"camerResetToolStripMenuItem";
			this->camerResetToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->camerResetToolStripMenuItem->Text = L"CamerReset";
			this->camerResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// showResolutionBorderToolStripMenuItem
			// 
			this->showResolutionBorderToolStripMenuItem->Checked = true;
			this->showResolutionBorderToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showResolutionBorderToolStripMenuItem->Name = L"showResolutionBorderToolStripMenuItem";
			this->showResolutionBorderToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->showResolutionBorderToolStripMenuItem->Text = L"ShowResolutionBorder";
			this->showResolutionBorderToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem
			// 
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Name = L"autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem";
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Text = L"AutoReadjustPositionWihleGridSizeIsChange";
			this->autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// showWarningMsgToolStripMenuItem
			// 
			this->showWarningMsgToolStripMenuItem->Name = L"showWarningMsgToolStripMenuItem";
			this->showWarningMsgToolStripMenuItem->Size = System::Drawing::Size(326, 22);
			this->showWarningMsgToolStripMenuItem->Text = L"ShowWarningMsg";
			this->showWarningMsgToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::showGridToolStripMenuItem_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->fileToolStripMenuItem, 
				this->imageToolStripMenuItem, this->imageToolStripMenuItem1, this->assistanceToolStripMenuItem, this->cameraResetToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1900, 24);
			this->menuStrip1->TabIndex = 5;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(106, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::saveToolStripMenuItem_Click);
			// 
			// imageToolStripMenuItem
			// 
			this->imageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openTemplateToolStripMenuItem});
			this->imageToolStripMenuItem->Name = L"imageToolStripMenuItem";
			this->imageToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->imageToolStripMenuItem->Text = L"Xml";
			// 
			// openTemplateToolStripMenuItem
			// 
			this->openTemplateToolStripMenuItem->Name = L"openTemplateToolStripMenuItem";
			this->openTemplateToolStripMenuItem->Size = System::Drawing::Size(205, 22);
			this->openTemplateToolStripMenuItem->Text = L"AddNewNodeFromFile";
			this->openTemplateToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::addNewNodeFromFileToolStripMenuItem_Click);
			// 
			// imageToolStripMenuItem1
			// 
			this->imageToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->backgroundImageToolStripMenuItem});
			this->imageToolStripMenuItem1->Name = L"imageToolStripMenuItem1";
			this->imageToolStripMenuItem1->Size = System::Drawing::Size(55, 20);
			this->imageToolStripMenuItem1->Text = L"Image";
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(178, 22);
			this->backgroundImageToolStripMenuItem->Text = L"BackgroundImage";
			this->backgroundImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::backgroundImageToolStripMenuItem_Click);
			// 
			// cameraResetToolStripMenuItem
			// 
			this->cameraResetToolStripMenuItem->Name = L"cameraResetToolStripMenuItem";
			this->cameraResetToolStripMenuItem->Size = System::Drawing::Size(94, 20);
			this->cameraResetToolStripMenuItem->Text = L"CameraReset";
			this->cameraResetToolStripMenuItem->Click += gcnew System::EventHandler(this, &ObjectDataForm::cameraResetToolStripMenuItem_Click);
			// 
			// UserDefineData_contextMenuStrip
			// 
			this->UserDefineData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->addToolStripMenuItem, 
				this->editToolStripMenuItem, this->deleteToolStripMenuItem});
			this->UserDefineData_contextMenuStrip->Name = L"UserDefineData_contextMenuStrip";
			this->UserDefineData_contextMenuStrip->Size = System::Drawing::Size(112, 70);
			// 
			// addToolStripMenuItem
			// 
			this->addToolStripMenuItem->Name = L"addToolStripMenuItem";
			this->addToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->addToolStripMenuItem->Text = L"Add";
			// 
			// editToolStripMenuItem
			// 
			this->editToolStripMenuItem->Name = L"editToolStripMenuItem";
			this->editToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->editToolStripMenuItem->Text = L"Edit";
			// 
			// deleteToolStripMenuItem
			// 
			this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
			this->deleteToolStripMenuItem->Size = System::Drawing::Size(111, 22);
			this->deleteToolStripMenuItem->Text = L"Delete";
			// 
			// LayerData_contextMenuStrip
			// 
			this->LayerData_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->selectAllThisTypeTemplateToolStripMenuItem, 
				this->refreshUserDataFromToolStripMenuItem});
			this->LayerData_contextMenuStrip->Name = L"LayerData_contextMenuStrip";
			this->LayerData_contextMenuStrip->Size = System::Drawing::Size(251, 48);
			// 
			// selectAllThisTypeTemplateToolStripMenuItem
			// 
			this->selectAllThisTypeTemplateToolStripMenuItem->Name = L"selectAllThisTypeTemplateToolStripMenuItem";
			this->selectAllThisTypeTemplateToolStripMenuItem->Size = System::Drawing::Size(250, 22);
			this->selectAllThisTypeTemplateToolStripMenuItem->Text = L"SelectAllThisTypeTemplate";
			// 
			// refreshUserDataFromToolStripMenuItem
			// 
			this->refreshUserDataFromToolStripMenuItem->Name = L"refreshUserDataFromToolStripMenuItem";
			this->refreshUserDataFromToolStripMenuItem->Size = System::Drawing::Size(250, 22);
			this->refreshUserDataFromToolStripMenuItem->Text = L"RefreshUserDataFromXMLData";
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &ObjectDataForm::timer1_Tick);
			// 
			// PlaceObjectByImageCenter_checkBox
			// 
			this->PlaceObjectByImageCenter_checkBox->AutoSize = true;
			this->PlaceObjectByImageCenter_checkBox->Location = System::Drawing::Point(158, 176);
			this->PlaceObjectByImageCenter_checkBox->Margin = System::Windows::Forms::Padding(2);
			this->PlaceObjectByImageCenter_checkBox->Name = L"PlaceObjectByImageCenter_checkBox";
			this->PlaceObjectByImageCenter_checkBox->Size = System::Drawing::Size(152, 16);
			this->PlaceObjectByImageCenter_checkBox->TabIndex = 66;
			this->PlaceObjectByImageCenter_checkBox->Text = L"PlaceObjectByImageCenter";
			this->PlaceObjectByImageCenter_checkBox->UseVisualStyleBackColor = true;
			// 
			// ObjectDataForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"ObjectDataForm";
			this->Size = System::Drawing::Size(1900, 961);
			this->Load += gcnew System::EventHandler(this, &ObjectDataForm::ObjectDataForm_Load);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->collpaseForm5->ResumeLayout(false);
			this->UserDefineData__contextMenuStrip->ResumeLayout(false);
			this->collpaseForm3->ResumeLayout(false);
			this->collpaseForm4->ResumeLayout(false);
			this->collpaseForm4->PerformLayout();
			this->collpaseForm1->ResumeLayout(false);
			this->collpaseForm1->PerformLayout();
			this->collpaseForm2->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectZ_numericUpDown))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AddObjectX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridHeight_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GridWidth_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapHeight_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MapWidth_numericUpDown))->EndInit();
			this->Main_splitContainer->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->UserDefineData_contextMenuStrip->ResumeLayout(false);
			this->LayerData_contextMenuStrip->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(this->Visible)
				Render();
		 }
private: System::Void treeView1_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
			 {
				e->Effect = DragDropEffects::Copy;
			 }
			 else
				 e->Effect = DragDropEffects::None;
		 }
private: System::Void treeView1_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 //this->Text += System::IO::Path::GetFileName(l_str);
					 treeView1->Nodes->Add(l_pNode);
				 }
			}
		 }
private: System::Void AssignData()
		 {
			 if( treeView1->SelectedNode )
			 {
				 if( treeView1->SelectedNode->Nodes->Count != 0 )
				 {
					 if(xmlNodeEditComponent1->CurrentAttribute_treeView->SelectedNode)
					 {
						if(xmlNodeEditComponent1->CurrentAttribute_treeView->SelectedNode->Name->Length != 0)
						{
							WARNING_MSG("this is not a element");
							return;
						}
						xmlNodeEditComponent1->CurrentAttribute_treeView->SelectedNode->Nodes->Add((GCFORM::TreeNode^)treeView1->SelectedNode->Clone());
					 }
					 else
					 {
						//xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Clear();
						xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Add((GCFORM::TreeNode^)treeView1->SelectedNode->Clone());
					 }
				 }
				 else
				 {
					WARNING_MSG("there is no attributes could be edit");
				 }
			 }		 
		 }
private: System::Void treeView1_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignData();
		 }
private: System::Void refreshToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 AssignData();
		 }
private: System::Void addNewNodeFromFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames();
			if(!l_strFileNames)
				return;
			for each(String^l_str in l_strFileNames)
			{
				 GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_str);
				 if( l_pNode )
				 {
					 //this->Text += System::IO::Path::GetFileName(l_str);
					 treeView1->Nodes->Add(l_pNode);
				 }
			}
		 }
		 void	RemoveSelectedNode()
		 {
			 if( treeView1->SelectedNode )
			 {
				 treeView1->Nodes->Remove(treeView1->SelectedNode);
			 }			 
		 }
private: System::Void deleteNodeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			RemoveSelectedNode();
		 }
private: System::Void treeView1_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Delete )
			 {
				RemoveSelectedNode();
			 }
		 }

private: System::Void AddImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddImage_button )
			{
				array<String^>^l_strFileNames = DNCT::OpenFileAndGetNames("(*.pi)|*.pi");
				if(l_strFileNames)
				for each(String^l_strName in l_strFileNames)
				{
					if(m_pMapDataManager->m_AllImageList.GetObject(DNCT::GcStringToWchar(System::IO::Path::GetFileNameWithoutExtension(l_strName))))
					{
						WARNING_MSG(l_strName+" exists!");
						continue;
					}
					if(m_pMapDataManager->m_AllImageList.Parse(DNCT::GcStringToChar(l_strName)))
					{
						ImageList_treeView->Nodes->Add(DNCT::PIToTreeNode(dynamic_cast<cPuzzleImage*>(m_pMapDataManager->m_AllImageList.GetLastObject())));
					}
					else
					{
						WARNING_MSG(l_strName+" open failed");
					}
				}
			}
			else
			if( sender == DelImage_button )
			{
				if(RemoveTextureFromSelectedNodesTreeView(ImageList_treeView,&m_pMapDataManager->m_AllImageList))
					ImageList_treeView->SelectedNode->Remove();
			}
		 }

private: System::Void AddTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DelTemplate_button )
			 {
				 bool	l_bFirstTime = true;
				 bool	l_bEraseOldData = false;//if yes and template is conflict,erase the data
				 while(Template_listBox->SelectedIndices->Count)
				 {
					 String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndices[0]]->ToString();
					 //check for old data using this image
					 cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(l_strName));
					 assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
					 for(int i=0;i<m_pMapDataManager->Count();++i)
					 {
						 cMap_CellData*l_pMap_CellData = m_pMapDataManager->GetObject(i);
						 for(int j=0;j<l_pMap_CellData->Count();++j)
						 {//this template has been assigned to the orther data,check for delete or reserved
							 cMapLayer*l_pMapLayer = (*l_pMap_CellData)[j];
							 for( int k=0;k<l_pMapLayer->Count();++k )
							 {
								 if(l_pMapLayer->GetObject(k)->GetImage() == l_pMap_Cell_ObjectData->GetImage())
								 {
									 if( !l_bFirstTime )
									 {
										l_bFirstTime = true;
										WARING_YES_NO_TO_YES("would u like to erase the data has been assigned by this data?")
										{
											l_bEraseOldData = true;
										}
										else
										{
											WARNING_MSG("the data has been referenced can't be delete");
											return;
										}
									 }
									 if( l_bEraseOldData )
									 {
										 l_pMapLayer->RemoveObject(k);
										 --k;
									 }
								 }
							 }
						 }
					 }
					 Template_listBox->Items->RemoveAt(Template_listBox->SelectedIndices[0]);
					 m_TemplateNameAndAttributeNode->Remove(l_strName);
					 g_pForListBoxTemplateCellData->RemoveObject(DNCT::GcStringToWchar(l_strName));
				 }
			 }
			 else
			 if(sender == AddTemplate_button)
			 {
				 if( !ImageList_treeView->SelectedNode )
				 {
					 WARNING_MSG("please select Image");
					 return;
				 }
				 if(!ImageList_treeView->SelectedNode->Parent)
				 {
					 WARNING_MSG("please pi's child");
					 return;				 
				 }
				 String^l_strTemplateName = TemplateName_textBox->Text->Trim();
				 if( l_strTemplateName->Length >0 )
				 {
					 if( l_strTemplateName[0]>=L'0'&&l_strTemplateName[0]<=L'9' )
					 {
						WARNING_MSG("first character can't be numerial");
						return;
					 }
					 GCFORM::TreeNode^l_pNode;
					 if(g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(l_strTemplateName))||
						 m_TemplateNameAndAttributeNode->ContainsKey(l_strTemplateName))
					 {
						WARING_YES_NO_TO_NO("replace old data!?")
						{
							return;
						}
						Template_listBox->Items->Remove(l_strTemplateName);
						m_TemplateNameAndAttributeNode->Remove(l_strTemplateName);
						g_pForListBoxTemplateCellData->RemoveObject(DNCT::GcStringToWchar(l_strTemplateName));
					 }
					 //if(!m_pMapDataManager->GetObject(DNCT::GcStringToWchar(TemplateName_textBox->Text->Trim()))
					 {
						 cPuzzleImage*l_pPi = m_pMapDataManager->m_AllImageList.GetPuzzleImage(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text));
						 cPuzzleImageUnit*l_pPuzzleImageUnit = l_pPi->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text));
						if( !l_pPuzzleImageUnit )
						{
							WARNING_MSG("image is not exists");
							return;
						}
						 if(xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Count!=0)
						 {
							 l_pNode = (GCFORM::TreeNode^)xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes[0]->Clone();
							 l_pNode->Text = l_strTemplateName;
						 }
						 m_TemplateNameAndAttributeNode[l_strTemplateName] = l_pNode;
						 Template_listBox->Items->Add(l_strTemplateName);
						 cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = new cMap_Cell_ObjectData();
						 l_pMap_Cell_ObjectData->SetName(DNCT::GcStringToWchar(l_strTemplateName));
						 l_pMap_Cell_ObjectData->SetImage(l_pPuzzleImageUnit);
						 bool	l_b = g_pForListBoxTemplateCellData->AddObject(l_pMap_Cell_ObjectData);
						 assert(l_b&&"this template exists!!");
						 xmlNodeEditComponent1->CurrentAttribute_treeView->Nodes->Clear();
						 Template_listBox->SelectedIndex = Template_listBox->Items->Count-1;
					 }
				 }
			 }
		 }

private: System::Void AddStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( sender == AddStage_button )
			{
				WCHAR*	l_strName = DNCT::GcStringToWchar(StageName_textBox->Text);
				if( this->m_pMapDataManager->GetObject(l_strName) )
				{
					WARING_YES_NO_TO_NO("replace data?")
					{
						return;
					}
					this->RemoveUserDefineNode(this->m_pMapDataManager->GetObject(l_strName));
					this->m_pMapDataManager->RemoveObject(l_strName);
					AllStage_listBox->Items->Remove(StageName_textBox->Text);
					g_bStageDataChanged = false;
				}
				g_pCurrentStageEditData->SetName(l_strName);
				Row_numericUpDown_ValueChanged(sender,e);
				GridHeight_numericUpDown_ValueChanged(sender,e);
				StartX_numericUpDown_ValueChanged(sender,e);
				cMap_CellData*l_pMap_CellData = new cMap_CellData(g_pCurrentStageEditData);
				this->CopyUserDataToNewData(g_pCurrentStageEditData,l_pMap_CellData);
				this->RemoveUserDefineNode(g_pCurrentStageEditData);
				this->m_pMapDataManager->AddObject(l_pMap_CellData);
				g_pCurrentStageEditData->Destory();
				g_pCurrentStageEditData->SetName(L"");
				AllStage_listBox->Items->Add(StageName_textBox->Text);
				StageDataInfo_listBox->Items->Clear();
				Layer_listBox->Items->Clear();
			}
			else
			{
				if( AllStage_listBox->SelectedIndex == -1 )
					return;
				//if( g_bStageDataChanged )
				//{
				//	WARING_YES_NO_TO_NO("ignore current edit data?")
				//	{
				//		return;
				//	}
				//}
				if( sender == ReRditStagebutton )
				{//fuck......it will lost unique user define data..go check m_UniqueUserDefineDataForMap_Cell_ObjectData
					Layer_listBox->Items->Clear();
					SAFE_DELETE(g_pCurrentStageEditData);
					g_pCurrentStageEditData = new cMap_CellData(m_pMapDataManager->GetObject(DNCT::GcStringToWchar(AllStage_listBox->SelectedItem->ToString())));
					CopyUserDataToNewData(m_pMapDataManager->GetObject(DNCT::GcStringToWchar(AllStage_listBox->SelectedItem->ToString())),g_pCurrentStageEditData);
					Vector2	l_vSize = g_pCurrentStageEditData->GetSize();
					POINT	l_GridSize = g_pCurrentStageEditData->GetGridSize();
					Vector3	l_vPos = g_pCurrentStageEditData->GetPos();
					this->StartX_numericUpDown->Value = (int)l_vPos.x;
					this->StartY_numericUpDown->Value = (int)l_vPos.y;
					GridWidth_numericUpDown->Value = (int)l_GridSize.x;
					GridHeight_numericUpDown->Value = (int)l_GridSize.y;
					MapWidth_numericUpDown->Value = (int)l_vSize.x;
					MapHeight_numericUpDown->Value = (int)l_vSize.y;
					StageDataInfo_listBox->Items->Clear();
					for(int i=0;i<g_pCurrentStageEditData->Count();++i)
					{
						Layer_listBox->Items->Add(DNCT::WcharToGcstring(g_pCurrentStageEditData->GetObject(i)->GetName()));
					}
					if( Layer_listBox->Items->Count )
					{
						Layer_listBox->SelectedIndex = 0;
					}
				}
				else
				if( sender == DelStage_button )
				{
					this->RemoveUserDefineNode(this->m_pMapDataManager->GetObject(AllStage_listBox->SelectedIndex));
					this->m_pMapDataManager->RemoveObject(AllStage_listBox->SelectedIndex);
					AllStage_listBox->Items->RemoveAt(AllStage_listBox->SelectedIndex);
				}
			}
		 }
private: System::Void GridHeight_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_vSize = g_pCurrentStageEditData->GetSize();
			 l_vSize.x = (float)GridWidth_numericUpDown->Value;
			 l_vSize.y = (float)GridHeight_numericUpDown->Value;
			 POINT	l_GridSize = {(int)GridWidth_numericUpDown->Value,(int)GridHeight_numericUpDown->Value};
			 g_pCurrentStageEditData->SetGridSize(l_GridSize);
			 if( autoReadjustPositionWihleGridSizeIsChangeToolStripMenuItem->Checked )
			 {
				 float	l_fWidth = m_pOrthogonalCamera->GetGridWidth()*(int)MapWidth_numericUpDown->Value;
				 float	l_fHeight = m_pOrthogonalCamera->GetGridHeight()*(int)MapHeight_numericUpDown->Value;
				 for( int i=0;i<g_pCurrentStageEditData->Count();++i )
				 {
					cMapLayer*l_pMapLayer = (*g_pCurrentStageEditData)[i];
					for( int j=0;j<l_pMapLayer->Count();++j )
					{
						cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = (*l_pMapLayer)[i];
						Vector3	l_vPos = l_pMap_Cell_ObjectData->GetPos();
						if( autoAllignGridToolStripMenuItem->Checked )//by grid position
						{
							//if( l_vPos.x >= 0.f&&l_fWidth >= l_vPos.x&&l_vPos.y >= 0.f&&l_fHeight >= l_vPos.y )
							{
								int	l_iRow = (int)(l_vPos.x/m_pOrthogonalCamera->GetGridWidth());
								int	l_iColumn = (int)(l_vPos.y/m_pOrthogonalCamera->GetGridHeight());
								l_vPos.x = l_iRow*l_vSize.x;
								l_vPos.y = l_iColumn*l_vSize.y;
							}
						}
						else//by scale rate
						{
							float	l_fScaleRateX = l_vSize.x/m_pOrthogonalCamera->GetGridWidth();
							float	l_fScaleRateY = l_vSize.y/m_pOrthogonalCamera->GetGridHeight();
							l_vPos.x *= l_fScaleRateX;
							l_vPos.y *= l_fScaleRateY;
						}
						l_pMap_Cell_ObjectData->SetPos(l_vPos);
					}
				 }
			 }
			 m_pOrthogonalCamera->SetGridHeight((float)GridHeight_numericUpDown->Value);
			 m_pOrthogonalCamera->SetGridWidth((float)GridWidth_numericUpDown->Value);
		 }
private: System::Void showGridToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 //if( sender == showGridToolStripMenuItem )
			 {
				 System::Windows::Forms::ToolStripMenuItem^l_pSender = (System::Windows::Forms::ToolStripMenuItem^)sender;
				 l_pSender->Checked = !l_pSender->Checked;
			 }
		 }
private: System::Void Row_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector2	l_Size = g_pCurrentStageEditData->GetSize();
			 l_Size.x = (float)MapWidth_numericUpDown->Value;
			 l_Size.y = (float)MapHeight_numericUpDown->Value;
			 g_pCurrentStageEditData->SetSize(l_Size);
		 }
private: System::Void Template_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( Template_listBox->SelectedIndex != -1 )
			 {
				 g_pCurrentSelectedImage = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(Template_listBox->SelectedItem->ToString()))->GetImage();
				 TemplateDataInvertorReEdit();
			 }
		 }
private: System::Void Clear_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( g_pCurrentStageEditData->GetCurrentList() )
				g_pCurrentStageEditData->GetCurrentList()->Destory();
			StageDataInfo_listBox->Items->Clear();
		 }

private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::SaveFileAndGetName();
			 if( l_strFileName )
			 {
				m_pMapDataManager->Export(DNCT::GcStringToChar(l_strFileName));
				//for template file
				ObjectDataForm::SaveTreeNodeFile( System::IO::Path::ChangeExtension(l_strFileName,"tmp"),m_TemplateNameAndAttributeNode,false);
				//for unique user define data
				ObjectDataForm::SaveTreeNodeFile( System::IO::Path::ChangeExtension(l_strFileName,"userdefine"),m_UniqueUserDefineDataForMap_Cell_ObjectData,true);
			 }
		 }
private: System::Void backgroundImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				SAFE_DELETE(g_pBackgroundImage);
				g_pBackgroundImage = new cBaseImage(DNCT::GcStringToChar(l_strFileName));
			}
		 }

private: System::Void ReEditTemplate_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 TemplateDataInvertorReEdit();
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			timer1->Enabled = false;
			String^l_strFileName = DNCT::OpenFileAndGetName();
			if( l_strFileName )
			{
				Layer_listBox->Items->Clear();
				AllStage_listBox->Items->Clear();
				g_pCurrentSelectedImage = 0;
				g_pCurrentStageEditData->Destory();
				g_pForListBoxTemplateCellData->Destory();
				m_pMapDataManager->m_AllImageList.Destory();
				m_pMapDataManager->Destory();
				this->ImageList_treeView->Nodes->Clear();
				StageDataInfo_listBox->Items->Clear();
				this->m_TemplateNameAndAttributeNode->Clear();
				this->Template_listBox->Items->Clear();
				m_UniqueUserDefineDataForMap_Cell_ObjectData->Clear();
				bool	l_b = m_pMapDataManager->Parse(DNCT::GcStringToChar(l_strFileName));
				if( l_b )
				{
					for(int i=0;i<m_pMapDataManager->m_AllImageList.Count();++i)
					{
						ImagerParserAddIntiTreeView(&m_pMapDataManager->m_AllImageList,ImageList_treeView,true);
					}
					l_strFileName = System::IO::Path::ChangeExtension(l_strFileName,"tmp");
					treeView1->Nodes->Clear();
					GCFORM::TreeNode^l_pNode = DNCT::OpenXmlFileToNode(l_strFileName);
					if(l_pNode)
					{
						for each(GCFORM::TreeNode^l_pNode in l_pNode->Nodes)
						{
							treeView1->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
							m_TemplateNameAndAttributeNode->Add(l_pNode->Text,l_pNode->Clone());
						}
					}
					for(int i=0;i<m_pMapDataManager->m_pTemplateImage->Count();++i)
					{
						this->Template_listBox->Items->Add(DNCT::WcharToGcstring((*m_pMapDataManager->m_pTemplateImage)[i]->GetName()));
					}
					for(int i=0;i<m_pMapDataManager->Count();++i)
					{
						AllStage_listBox->Items->Add(DNCT::WcharToGcstring(m_pMapDataManager->GetObject(i)->GetName()));
					}
					l_strFileName = System::IO::Path::ChangeExtension(l_strFileName,"userdefine");
					OpenUserDefineData(l_strFileName);
				}
			}
			timer1->Enabled = true;
		 }
private: System::Void StartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 g_pCurrentStageEditData->SetPos(Vector3((float)this->StartX_numericUpDown->Value,(float)this->StartY_numericUpDown->Value,0.f));
		 }
private: System::Void AddToLayer_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( DeleteLayer_button == sender )
			 {
				 timer1->Enabled = false;
				 Sleep(10);
				 StageDataInfo_listBox->Items->Clear();
				 while(Layer_listBox->SelectedIndices->Count>0)
				 {
					int	l_iSelectedIndex = Layer_listBox->SelectedIndices[0];
					RemoveUserDefineNode(g_pCurrentStageEditData->GetObject(l_iSelectedIndex));
					g_pCurrentStageEditData->RemoveObject(l_iSelectedIndex);
					Layer_listBox->Items->RemoveAt(l_iSelectedIndex);
					g_pCurrentStageEditData->SetCurrentList((cMapLayer*)0);
				 }
				 timer1->Enabled = true;
			 }
			 else
			 if( sender == AddToLayer_button )
			 {
				 if(LayerName_textBox->Text->Length)
				 {
					 if(!DNCT::CheckListContainStringAndAdd(Layer_listBox,LayerName_textBox->Text,true))
					 {
						 cMapLayer*l_pMapLayer = new cMapLayer();
						 l_pMapLayer->SetName(DNCT::GcStringToWchar(LayerName_textBox->Text));
						 g_pCurrentStageEditData->AddObject(l_pMapLayer);
						 Layer_listBox->SelectedIndices->Clear();
						 for( int i=0;i<Layer_listBox->Items->Count;++i )
							Layer_listBox->SelectedIndices->Add(i);
						 g_pCurrentStageEditData->SetCurrentList(Layer_listBox->Items->Count-1);
					 }
				 }
			 }
		 }

private: System::Void AllStage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			if( g_pCurrentStageEditData->Count() )
			{
				if( showWarningMsgToolStripMenuItem->Checked  )
				{
					WARING_YES_NO_TO_NO("it will clear exist data!would u like to do it?")
						return;
				}
			}
			AddStage_button_Click(ReRditStagebutton,e);
		 }
private: System::Void LayerDown_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( LayerUp_button == sender )
			 {
				 if( Layer_listBox->SelectedIndex != 0 && Layer_listBox->Items->Count>=2 )
				 {
					 int	l_iSelectedIndex = Layer_listBox->SelectedIndex;
					 String^l_strName1 = Layer_listBox->Items[l_iSelectedIndex]->ToString();
					 String^l_strName2 = Layer_listBox->Items[l_iSelectedIndex-1]->ToString();
					 Layer_listBox->Items[l_iSelectedIndex] = l_strName2;
					 Layer_listBox->Items[l_iSelectedIndex-1] = l_strName1;
				 }
			 }
			 else
			 if( sender == LayerDown_button )
			 {
				 if( Layer_listBox->SelectedIndex != Layer_listBox->Items->Count-1&& Layer_listBox->Items->Count>=2 )
				 {
					 int	l_iSelectedIndex = Layer_listBox->SelectedIndex;
					 String^l_strName1 = Layer_listBox->Items[l_iSelectedIndex]->ToString();
					 String^l_strName2 = Layer_listBox->Items[l_iSelectedIndex+1]->ToString();
					 Layer_listBox->Items[l_iSelectedIndex] = l_strName2;
					 Layer_listBox->Items[l_iSelectedIndex+1] = l_strName1;				 
				 }				
			 }
		 }
private: System::Void AllStage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(AllStage_listBox->SelectedItem)
			 {
				 StageName_textBox->Text = AllStage_listBox->SelectedItem->ToString();
			 }
		 }
private: System::Void ObjectMove_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( ObjectMove_checkBox->Checked )
				Erase_checkBox->Checked = false;
				 
		 }
private: System::Void Erase_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(Erase_checkBox->Checked )
				ObjectMove_checkBox->Checked = false;
		 }
private: System::Void cameraResetToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			m_pOrthogonalCamera->SetResolution(Vector2(DRAW_PANEL_RESOLUTION_WIDTH,DRAW_PANEL_RESOLUTION_HEIGHT));
			m_pOrthogonalCamera->Reset();
		 }
private: System::Void AddObjectToStage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 Vector3	l_vPos = Vector3((float)AddObjectX_numericUpDown->Value,(float)AddObjectY_numericUpDown->Value,(float)AddObjectZ_numericUpDown->Value);
			 ObjectDataForm::AddObjectIntoCurrentStage(l_vPos);
		 }
private: System::Void DeleteObject_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 while(StageDataInfo_listBox->SelectedIndices->Count>0)
			 {
				int	l_iSelectedIndex = StageDataInfo_listBox->SelectedIndices[0];
				RemoveUserDefineNode(g_pCurrentStageEditData->GetCurrentList()->GetObject(l_iSelectedIndex));
				g_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iSelectedIndex);
				StageDataInfo_listBox->Items->RemoveAt(l_iSelectedIndex);
			 }
		 }
private: System::Void MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		 {
		    if( this->splitContainer2->Focused )
			{
                if( e->KeyChar == L'D'||
                    e->KeyChar == L'd' )
				{
					int	l_iColldedIndex = g_pCurrentStageEditData->CollideForSamePosition(g_vCurrentPlacePos);
			        if( l_iColldedIndex != -1 )
			        {
				        g_pCurrentStageEditData->GetCurrentList()->RemoveObject(l_iColldedIndex);
				        StageDataInfo_listBox->Items->RemoveAt(l_iColldedIndex);
			        }
				}
				else
				if( e->KeyChar == L'a'||
				    e->KeyChar == L'A' )
				{
				    if( !AllowSamepositionObject_checkBox->Checked )
					    AddObjectOnCell();
				}
            }
		 }
	private: System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				if( e->KeyCode == Keys::Add  )
				{

				}
			 }
private: System::Void ObjectData_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
			 if(ObjectData_treeView->SelectedNode)
			 {
				 if(ObjectData_treeView->SelectedNode->Name->Length>0)
				 {
					ObjectData_textBox->Text = ObjectData_treeView->SelectedNode->Name;
					NodeName_textBox->Text = ObjectData_treeView->SelectedNode->Name;
				 }
			 }
		 }

private: System::Void DataChange_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(ObjectData_treeView->SelectedNode)
			 {
				 if(ObjectData_treeView->SelectedNode->Name->Length>0)
				 {
					ObjectData_treeView->SelectedNode->Name = ObjectData_textBox->Text;
					NodeName_textBox->Text = ObjectData_treeView->SelectedNode->Name;
				 }
				 else
					 WARNING_MSG("node is element not attribute");
			 }
		 }

private: System::Void SaveObjectData_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( !g_pCurrentEditUserPropertyObjectData )
				 return;
			if( sender == RevertData_button )
			{
				g_pCurrentEditUserPropertyObjectData->SetContainUserDefineData(false);
				if(this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(g_pCurrentEditUserPropertyObjectData->GetUniqueID()))
					m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(g_pCurrentEditUserPropertyObjectData->GetUniqueID());
				RefreshData_button_Click(sender,e);
			}
			else
			{
				GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)ObjectData_treeView->Nodes[0]->Clone();
				if(this->m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(g_pCurrentEditUserPropertyObjectData->GetUniqueID()))
					m_UniqueUserDefineDataForMap_Cell_ObjectData->Remove(g_pCurrentEditUserPropertyObjectData->GetUniqueID());
				m_UniqueUserDefineDataForMap_Cell_ObjectData[g_pCurrentEditUserPropertyObjectData->GetUniqueID()] = l_pNode;
				g_bStageDataChanged = true;
				g_pCurrentEditUserPropertyObjectData->SetContainUserDefineData(true);
				GCFORM::MessageBox::Show("Add Ok");
			}
		 }

private: System::Void RefreshData_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			if( !g_pCurrentEditUserPropertyObjectData )
				return;
			ObjectData_treeView->Nodes->Clear();
			String^l_strName;
			l_strName = DNCT::WcharToGcstring(g_pCurrentEditUserPropertyObjectData->GetName());
			NodeName_textBox->Text = "";
			this->ObjectData_textBox->Text = "";
			GCFORM::TreeNode^l_pNode;
			if(m_TemplateNameAndAttributeNode->ContainsKey(l_strName))
				l_pNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strName];
			if( l_pNode )
			{
				ObjectData_treeView->Nodes->Clear();
				ObjectData_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
				ObjectData_treeView->Nodes[0]->Expand();
			}
		 }
private: System::Void ObjectDataForm_Load(System::Object^  sender, System::EventArgs^  e)
        {
            splitContainer1->SplitterDistance = 213;
        }
private: System::Void ImageList_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
		 {
            if( ImageList_treeView->SelectedNode && ImageList_treeView->SelectedNode->Parent )
            {
				 TemplateName_textBox->Text = ImageList_treeView->SelectedNode->Text;
                 cPuzzleImage*l_pPI = dynamic_cast<cPuzzleImage*>(m_pMapDataManager->m_AllImageList.GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Parent->Text)));
                 cSimpleGLTexture*l_pSimpleGLTexture = dynamic_cast<cSimpleGLTexture*>(l_pPI->GetObject(DNCT::GcStringToWchar(ImageList_treeView->SelectedNode->Text)));
				 label9->Text = DNCT::WcharToGcstring(l_pSimpleGLTexture->DescriptionInfo());
			}
			else
			{
			    label9->Text = "";
			}
			//if( ImageList_listBox->SelectedIndex!=-1 )
			//{
			//	cBaseImage*l_pNewImage = m_pMapDataManager->m_AllImageList.GetObject(DNCT::GcStringToWchar(ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString()));
			//	if( Template_listBox->SelectedIndex != -1 )
			//	{//change old data image as new
			//		String^l_strName = Template_listBox->Items[Template_listBox->SelectedIndex]->ToString();
			//		cMap_Cell_ObjectData*l_pMap_Cell_ObjectData = g_pForListBoxTemplateCellData->GetObject(DNCT::GcStringToWchar(l_strName));
			//		assert(l_pMap_Cell_ObjectData&&"call fatming!! if( sender == DelTemplate_button )");
			//		for(int i=0;i<m_pMapDataManager->Count();++i)
			//		{
			//			cMap_CellData*l_pMap_CellData = m_pMapDataManager->GetObject(i);
			//			for(int j=0;j<l_pMap_CellData->Count();++j)
			//			{
			//				if((*l_pMap_CellData)[j]->GetImage() == l_pMap_Cell_ObjectData->GetImage())
			//				{
			//					(*l_pMap_CellData)[j]->SetImage(l_pNewImage);
			//				}
			//			}
			//		}
			//	}
			//	Image_textBox->Text = ImageList_listBox->Items[ImageList_listBox->SelectedIndex]->ToString();
			//	//g_pCurrentSelectedImage = l_pNewImage;
			//}
			////else
			////	g_pCurrentSelectedImage = 0;
		 }
private: System::Void Layer_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 StageDataInfo_listBox->Items->Clear();
			 if( Layer_listBox->SelectedIndex == -1 )
			 {
				 g_pCurrentStageEditData->SetCurrentList((cMapLayer*)0);
			 }
			 else
			 {
				g_pCurrentStageEditData->SetCurrentList(Layer_listBox->SelectedIndex);
				DumpObjectListIntoListBox(StageDataInfo_listBox,g_pCurrentStageEditData->GetCurrentList(),true);
			 }
		 }
private: System::Void ImageList_treeView_BeforeSelect(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e)
         {
            if(this->ImageList_treeView->SelectedNode)
            {
                this->ImageList_treeView->SelectedNode->BackColor = System::Drawing::Color::White;
                this->ImageList_treeView->SelectedNode->ForeColor = System::Drawing::Color::Black;
            }
         }
private: System::Void AllStage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Enter )
			 {
				System::Windows::Forms::MouseEventArgs^  e2;
				AllStage_listBox_MouseDoubleClick(sender,e2);
			 }
		 }
private: System::Void StageDataInfo_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
        {
            if( StageDataInfo_listBox->SelectedIndex != -1)
            {
                ObjectData_treeView->Nodes->Clear();
                g_pCurrentEditUserPropertyObjectData = g_pCurrentStageEditData->GetCurrentList()->GetObject(StageDataInfo_listBox->SelectedIndex);
                if( m_UniqueUserDefineDataForMap_Cell_ObjectData->ContainsKey(g_pCurrentEditUserPropertyObjectData->GetUniqueID()) )
                {
                    GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_UniqueUserDefineDataForMap_Cell_ObjectData[g_pCurrentEditUserPropertyObjectData->GetUniqueID()];
                    if( l_pNode )
                    {
                        ObjectData_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                    }
                }
                else
                {
                    String^l_strTemplateName = DNCT::WcharToGcstring(g_pCurrentEditUserPropertyObjectData->GetName());
                    if( m_TemplateNameAndAttributeNode->ContainsKey(l_strTemplateName) )
                    {
                        GCFORM::TreeNode^l_pNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strTemplateName];
                        if( l_pNode )
                        {
                            ObjectData_treeView->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                        }
                    }
                }
            }
			else
			{
				ObjectData_treeView->Nodes->Clear();
				NodeName_textBox->Text = "";
				ObjectData_textBox->Text = "";
				g_pCurrentEditUserPropertyObjectData = 0;
			}
        }
private: System::Void RefreshUserDefineData_button_Click(System::Object^  sender, System::EventArgs^  e)          
        {
            UserData_xmlForm->CurrentAttribute_treeView->Nodes->Clear();
            for each (GCFORM::TreeNode^l_pNode in m_UniqueUserDefineDataForMap_Cell_ObjectData->Values  )
            {
                UserData_xmlForm->CurrentAttribute_treeView->Nodes->Add(l_pNode);
            }
        }
private: System::Void SyncUserDefineDataByFile_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
            WARNING_MSG("select event data to sync userdefine data,it's possible edit event data at another file,and want to sync current user define data");
            String^l_strFileName = DNCT::OpenFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
            if( l_strFileName )
            {
                GCFORM::TreeNode^l_pEventDefinedEventNode = DNCT::OpenXmlFileToNode(nullptr);
                for each(String^l_strKey in this->m_TemplateNameAndAttributeNode->Keys)
                {
                    //if the userdefine data is define by event editor it will contain a name has named in the editor
                    //now we find it out
                    //<UserDefineData Stage="cc" Layer="a1" Index="9">
                    GCFORM::TreeNode^l_pUserDefineNode = (GCFORM::TreeNode^)m_TemplateNameAndAttributeNode[l_strKey];
                    if( l_pUserDefineNode->Nodes->Count == 1 )//UserDefineData
                    {
                        assert(l_pUserDefineNode->Nodes[0]->Nodes->Count>=4&&"this user define data is empty!?");
                        //get event node?(because its possible not)
                        GCFORM::TreeNode^l_pUserdefineNodeFromEventNode = l_pUserDefineNode->Nodes[0]->Nodes[3];
                        if( l_pUserdefineNodeFromEventNode->Name->Length )
                        {//it is a attribute,it's not from event editor
                            continue;
                        }
                        //ok it's possible defined by event editor,now check the name if it exist set the node
                        for each( GCFORM::TreeNode^l_pNode in l_pEventDefinedEventNode->Nodes )
                        {
                            //ensure it's edit in the event the first node is a element without attrbute
                            if( l_pNode->Name->Length == 0 )
                            {
                                //ok the name is matched, it might  edited by event editor
                                if(l_pUserdefineNodeFromEventNode->Text->Equals(l_pNode->Text))
                                {
                                    //remove old data sync event file to this
                                    l_pUserDefineNode->Nodes[0]->Nodes[3]->Remove();
                                    l_pUserDefineNode->Nodes[0]->Nodes->Add((GCFORM::TreeNode^)l_pNode->Clone());
                                }
                            }
                        }
                    }
                }
            }
        }
};
}