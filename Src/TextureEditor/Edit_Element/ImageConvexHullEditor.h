#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMCEditor {


	/// <summary>
	/// Summary for ImageConvexHullEditor
	/// </summary>
	public ref class ImageConvexHullEditor : public System::Windows::Forms::UserControl
	{
	public:
		ImageConvexHullEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pTimeAndFPS = new UT::sTimeAndFPS;
			this->Dock = GCFORM::DockStyle::Fill;
			m_HdcMV = GetDC((HWND)this->splitContainer1->Panel2->Handle.ToPointer());
			m_HGLRCMV = UT::InitOpenGL((HWND)this->splitContainer1->Panel2->Handle.ToPointer(),true,m_HdcMV);
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			if( e_ptabControl )
			{
				m_pTabPage = gcnew GCFORM::TabPage();
				m_pTabPage->Text = "CollisionShapeEditor/碰撞範圍編輯";
				m_pTabPage->AutoScroll = true;
				m_pTabPage->Controls->Add(this);
				e_ptabControl->TabPages->Add(m_pTabPage);
				m_pTabControl = e_ptabControl;
				e_ptabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &ImageConvexHullEditor::tabControl_SelectedIndexChanged);
			}
			else
			{
				m_pForm = e_pForm;
				e_pForm->Controls->Add(this);			
			}
			splitContainer1->Panel2->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ImageConvexHullEditor::MyMouseDown);
			splitContainer1->Panel2->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &ImageConvexHullEditor::MyMouseUp);
			splitContainer1->Panel2->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &ImageConvexHullEditor::MyMouseUp);

			splitContainer1->Panel2->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &ImageConvexHullEditor::MyMouseMove);
//			splitContainer1->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ImageConvexHullEditor::MyKeyUp);
//			splitContainer1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ImageConvexHullEditor::MyKeyPress);
//			splitContainer1->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ImageConvexHullEditor::MyKeyDown);
			LanguageSwitch(this->Controls,"/",this->Handle);
			LanguageSwitch(splitContainer1->Panel1->Controls,"/",this->Handle);
			m_pDebugFont = 0;
			if(System::IO::File::Exists("Font.png"))
			{
				m_pDebugFont = new cGlyphFontRender("Font");
				if( !m_pDebugFont->GetFontImage() )
					SAFE_DELETE(m_pDebugFont);
			}
			m_pPuzzleImage = 0;
			m_pPuzzleImageUnit = 0;
			CollisionType_comboBox->SelectedIndex = 0;
			Mode_comboBox->SelectedIndex = 0;
			m_p2DImageCollisionDataTestForConverxCollide = 0;
			m_p2DImageCollisionData = new c2DImageCollisionDataForEditor();
			m_pOrthogonalCamera = new cOrthogonalCamera;
			InitBT();
			CreateShader(g_bCommonVSClientState,L"BTPathShader");
			CreateShader(g_bCommonVSNoTextureClientState,L"BT_NoTextureShader");
			this->timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ImageConvexHullEditor()
		{
			ExitBT();
			DeleteShader(L"BTPathShader");
			DeleteShader(L"BT_NoTextureShader");
			SAFE_DELETE(m_p2DImageCollisionDataTestForConverxCollide);
			SAFE_DELETE(m_pTimeAndFPS);
			SAFE_DELETE(m_pDebugFont);
			SAFE_DELETE(m_pPuzzleImage);
			SAFE_DELETE(m_p2DImageCollisionData);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openPiFileToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^  Mode_comboBox;
	private: System::Windows::Forms::ListBox^  PuzzleImageUnit_listBox;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  AllPointData_listBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Button^  ApplyPointsToAllImages_button;
	private: System::Windows::Forms::NumericUpDown^  SphereRadius_numericUpDown;
	private: System::Windows::Forms::CheckBox^  CollisionTest_checkBox;
	private: System::Windows::Forms::ListBox^  AllCollisionShape_listBox;
	private: System::Windows::Forms::Button^  CollisionDataDel_button;
	private: System::Windows::Forms::Button^  CollisionDataAdd_button;
	private: System::Windows::Forms::ComboBox^  CollisionType_comboBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  CollisionDataSphereRadius_numericUpDown;
	//my
	private: GCFORM::Form^m_pForm;
	private: GCFORM::TabPage^m_pTabPage;					//for attach from.
private: System::Windows::Forms::CheckBox^  ConverxCollisionTest_checkBox;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::NumericUpDown^  ConverxIndex_numericUpDown;
private: System::Windows::Forms::Button^  OpenCollisionFile_button;

	private: GCFORM::TabControl^m_pTabControl;				//to determin is tabpage clicked.
	public:	 void	OpenCollisionFile(String^e_strFileName);
	UT::sTimeAndFPS			*m_pTimeAndFPS;
	HGLRC	 m_HGLRCMV;
	HDC		 m_HdcMV;
	cGlyphFontRender*m_pDebugFont;
	cPuzzleImage*m_pPuzzleImage;
	cPuzzleImageUnit*m_pPuzzleImageUnit;
	c2DImageCollisionDataForEditor*m_p2DImageCollisionData;
	c2DImageCollisionData*			m_p2DImageCollisionDataTestForConverxCollide;
	cOrthogonalCamera*				m_pOrthogonalCamera;
	void	OpenPIFile(char*e_strFileName,bool e_bErasePathData)
	{
		cImageParser	l_ImageParser;
		l_ImageParser.SetFromResource(true);
		if(l_ImageParser.Parse(e_strFileName))
		{
			if( l_ImageParser.Count() )
			{
				SAFE_DELETE(m_pPuzzleImage);
				PuzzleImageUnit_listBox->Items->Clear();
				m_pPuzzleImage = dynamic_cast<cPuzzleImage*>(l_ImageParser[0]);
				m_pPuzzleImageUnit = 0;
				if( e_bErasePathData )
				{
					m_p2DImageCollisionData->ClearImageCollisionDataVector();
					for( int i=0;i<m_pPuzzleImage->Count();++i )
					{
						PuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring((*m_pPuzzleImage)[i]->GetName()));
						c2DImageCollisionDataForEditor::sImageCollisionData*l_pImageCollisionData = new c2DImageCollisionDataForEditor::sImageCollisionData();
						m_p2DImageCollisionData->m_ImageCollisionDataVector.push_back(l_pImageCollisionData);
					}
				}
				else
				{
					for( int i=0;i<m_pPuzzleImage->Count();++i )
					{
						PuzzleImageUnit_listBox->Items->Add(DNCT::WcharToGcstring((*m_pPuzzleImage)[i]->GetName()));
						//if(!m_pCurveManager->GetObject((*m_pPuzzleImage)[i]->GetName()))
						//	WARNING_MSG("pi File is not match");
					}
				}
			}
		}
	}
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openPiFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Mode_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->PuzzleImageUnit_listBox = (gcnew System::Windows::Forms::ListBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->ConverxIndex_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ConverxCollisionTest_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->CollisionTest_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->CollisionDataSphereRadius_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SphereRadius_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AllCollisionShape_listBox = (gcnew System::Windows::Forms::ListBox());
			this->CollisionDataDel_button = (gcnew System::Windows::Forms::Button());
			this->CollisionDataAdd_button = (gcnew System::Windows::Forms::Button());
			this->CollisionType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->ApplyPointsToAllImages_button = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->AllPointData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->OpenCollisionFile_button = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ConverxIndex_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CollisionDataSphereRadius_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SphereRadius_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1304, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem, this->openPiFileToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(145, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(145, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::openToolStripMenuItem_Click);
			// 
			// openPiFileToolStripMenuItem
			// 
			this->openPiFileToolStripMenuItem->Name = L"openPiFileToolStripMenuItem";
			this->openPiFileToolStripMenuItem->Size = System::Drawing::Size(145, 22);
			this->openPiFileToolStripMenuItem->Text = L"Open pi File";
			this->openPiFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::openPiFileToolStripMenuItem_Click);
			// 
			// Mode_comboBox
			// 
			this->Mode_comboBox->ForeColor = System::Drawing::Color::Black;
			this->Mode_comboBox->FormattingEnabled = true;
			this->Mode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"AddPoint", L"FixPoint"});
			this->Mode_comboBox->Location = System::Drawing::Point(124, 9);
			this->Mode_comboBox->Margin = System::Windows::Forms::Padding(4);
			this->Mode_comboBox->Name = L"Mode_comboBox";
			this->Mode_comboBox->Size = System::Drawing::Size(160, 23);
			this->Mode_comboBox->TabIndex = 1;
			this->Mode_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ImageConvexHullEditor::Mode_comboBox_SelectedIndexChanged);
			// 
			// PuzzleImageUnit_listBox
			// 
			this->PuzzleImageUnit_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->PuzzleImageUnit_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PuzzleImageUnit_listBox->FormattingEnabled = true;
			this->PuzzleImageUnit_listBox->ItemHeight = 15;
			this->PuzzleImageUnit_listBox->Location = System::Drawing::Point(9, 71);
			this->PuzzleImageUnit_listBox->Margin = System::Windows::Forms::Padding(4);
			this->PuzzleImageUnit_listBox->Name = L"PuzzleImageUnit_listBox";
			this->PuzzleImageUnit_listBox->Size = System::Drawing::Size(215, 199);
			this->PuzzleImageUnit_listBox->TabIndex = 2;
			this->PuzzleImageUnit_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ImageConvexHullEditor::PuzzleImageUnit_listBox_SelectedIndexChanged);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &ImageConvexHullEditor::timer1_Tick);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(4);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->splitContainer1->Panel1->Controls->Add(this->OpenCollisionFile_button);
			this->splitContainer1->Panel1->Controls->Add(this->label4);
			this->splitContainer1->Panel1->Controls->Add(this->ConverxIndex_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->ConverxCollisionTest_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->CollisionTest_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->CollisionDataSphereRadius_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->SphereRadius_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->AllCollisionShape_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->CollisionDataDel_button);
			this->splitContainer1->Panel1->Controls->Add(this->CollisionDataAdd_button);
			this->splitContainer1->Panel1->Controls->Add(this->CollisionType_comboBox);
			this->splitContainer1->Panel1->Controls->Add(this->ApplyPointsToAllImages_button);
			this->splitContainer1->Panel1->Controls->Add(this->label6);
			this->splitContainer1->Panel1->Controls->Add(this->label5);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->AllPointData_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->Mode_comboBox);
			this->splitContainer1->Panel1->Controls->Add(this->PuzzleImageUnit_listBox);
			this->splitContainer1->Size = System::Drawing::Size(1304, 738);
			this->splitContainer1->SplitterDistance = 438;
			this->splitContainer1->SplitterWidth = 5;
			this->splitContainer1->TabIndex = 3;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(260, 165);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(87, 15);
			this->label4->TabIndex = 25;
			this->label4->Text = L"ConverxIndex";
			// 
			// ConverxIndex_numericUpDown
			// 
			this->ConverxIndex_numericUpDown->ForeColor = System::Drawing::Color::Black;
			this->ConverxIndex_numericUpDown->Location = System::Drawing::Point(262, 219);
			this->ConverxIndex_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->ConverxIndex_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1920, 0, 0, 0});
			this->ConverxIndex_numericUpDown->Name = L"ConverxIndex_numericUpDown";
			this->ConverxIndex_numericUpDown->Size = System::Drawing::Size(159, 25);
			this->ConverxIndex_numericUpDown->TabIndex = 24;
			// 
			// ConverxCollisionTest_checkBox
			// 
			this->ConverxCollisionTest_checkBox->AutoSize = true;
			this->ConverxCollisionTest_checkBox->ForeColor = System::Drawing::Color::White;
			this->ConverxCollisionTest_checkBox->Location = System::Drawing::Point(262, 135);
			this->ConverxCollisionTest_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->ConverxCollisionTest_checkBox->Name = L"ConverxCollisionTest_checkBox";
			this->ConverxCollisionTest_checkBox->Size = System::Drawing::Size(149, 19);
			this->ConverxCollisionTest_checkBox->TabIndex = 23;
			this->ConverxCollisionTest_checkBox->Text = L"ConverxCollisionTest";
			this->ConverxCollisionTest_checkBox->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::White;
			this->label3->Location = System::Drawing::Point(260, 345);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(84, 15);
			this->label3->TabIndex = 22;
			this->label3->Text = L"SphereRadius";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(11, 466);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 15);
			this->label2->TabIndex = 21;
			this->label2->Text = L"Radius";
			// 
			// CollisionTest_checkBox
			// 
			this->CollisionTest_checkBox->AutoSize = true;
			this->CollisionTest_checkBox->ForeColor = System::Drawing::Color::White;
			this->CollisionTest_checkBox->Location = System::Drawing::Point(262, 321);
			this->CollisionTest_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->CollisionTest_checkBox->Name = L"CollisionTest_checkBox";
			this->CollisionTest_checkBox->Size = System::Drawing::Size(101, 19);
			this->CollisionTest_checkBox->TabIndex = 14;
			this->CollisionTest_checkBox->Text = L"CollisionTest";
			this->CollisionTest_checkBox->UseVisualStyleBackColor = true;
			// 
			// CollisionDataSphereRadius_numericUpDown
			// 
			this->CollisionDataSphereRadius_numericUpDown->ForeColor = System::Drawing::Color::Black;
			this->CollisionDataSphereRadius_numericUpDown->Location = System::Drawing::Point(61, 454);
			this->CollisionDataSphereRadius_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CollisionDataSphereRadius_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1920, 0, 0, 
				0});
			this->CollisionDataSphereRadius_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->CollisionDataSphereRadius_numericUpDown->Name = L"CollisionDataSphereRadius_numericUpDown";
			this->CollisionDataSphereRadius_numericUpDown->Size = System::Drawing::Size(159, 25);
			this->CollisionDataSphereRadius_numericUpDown->TabIndex = 20;
			this->CollisionDataSphereRadius_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {12, 0, 0, 0});
			this->CollisionDataSphereRadius_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &ImageConvexHullEditor::CollisionDataSphereRadius_numericUpDown_ValueChanged);
			// 
			// SphereRadius_numericUpDown
			// 
			this->SphereRadius_numericUpDown->ForeColor = System::Drawing::Color::Black;
			this->SphereRadius_numericUpDown->Location = System::Drawing::Point(262, 364);
			this->SphereRadius_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->SphereRadius_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1920, 0, 0, 0});
			this->SphereRadius_numericUpDown->Name = L"SphereRadius_numericUpDown";
			this->SphereRadius_numericUpDown->Size = System::Drawing::Size(159, 25);
			this->SphereRadius_numericUpDown->TabIndex = 15;
			this->SphereRadius_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {12, 0, 0, 0});
			// 
			// AllCollisionShape_listBox
			// 
			this->AllCollisionShape_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AllCollisionShape_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllCollisionShape_listBox->FormattingEnabled = true;
			this->AllCollisionShape_listBox->ItemHeight = 15;
			this->AllCollisionShape_listBox->Location = System::Drawing::Point(13, 348);
			this->AllCollisionShape_listBox->Margin = System::Windows::Forms::Padding(4);
			this->AllCollisionShape_listBox->Name = L"AllCollisionShape_listBox";
			this->AllCollisionShape_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllCollisionShape_listBox->Size = System::Drawing::Size(207, 94);
			this->AllCollisionShape_listBox->TabIndex = 19;
			this->AllCollisionShape_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ImageConvexHullEditor::AllCollisionShape_listBox_SelectedIndexChanged);
			// 
			// CollisionDataDel_button
			// 
			this->CollisionDataDel_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->CollisionDataDel_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CollisionDataDel_button->Location = System::Drawing::Point(121, 311);
			this->CollisionDataDel_button->Margin = System::Windows::Forms::Padding(4);
			this->CollisionDataDel_button->Name = L"CollisionDataDel_button";
			this->CollisionDataDel_button->Size = System::Drawing::Size(100, 29);
			this->CollisionDataDel_button->TabIndex = 18;
			this->CollisionDataDel_button->Text = L"Delete";
			this->CollisionDataDel_button->UseVisualStyleBackColor = false;
			this->CollisionDataDel_button->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::CollisionDataAdd_button_Click);
			// 
			// CollisionDataAdd_button
			// 
			this->CollisionDataAdd_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->CollisionDataAdd_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CollisionDataAdd_button->Location = System::Drawing::Point(13, 311);
			this->CollisionDataAdd_button->Margin = System::Windows::Forms::Padding(4);
			this->CollisionDataAdd_button->Name = L"CollisionDataAdd_button";
			this->CollisionDataAdd_button->Size = System::Drawing::Size(100, 29);
			this->CollisionDataAdd_button->TabIndex = 17;
			this->CollisionDataAdd_button->Text = L"Add";
			this->CollisionDataAdd_button->UseVisualStyleBackColor = false;
			this->CollisionDataAdd_button->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::CollisionDataAdd_button_Click);
			// 
			// CollisionType_comboBox
			// 
			this->CollisionType_comboBox->ForeColor = System::Drawing::Color::Black;
			this->CollisionType_comboBox->FormattingEnabled = true;
			this->CollisionType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Sphere", L"ConvexHull"});
			this->CollisionType_comboBox->Location = System::Drawing::Point(13, 276);
			this->CollisionType_comboBox->Margin = System::Windows::Forms::Padding(4);
			this->CollisionType_comboBox->Name = L"CollisionType_comboBox";
			this->CollisionType_comboBox->Size = System::Drawing::Size(197, 23);
			this->CollisionType_comboBox->TabIndex = 16;
			// 
			// ApplyPointsToAllImages_button
			// 
			this->ApplyPointsToAllImages_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->ApplyPointsToAllImages_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ApplyPointsToAllImages_button->Location = System::Drawing::Point(125, 41);
			this->ApplyPointsToAllImages_button->Margin = System::Windows::Forms::Padding(4);
			this->ApplyPointsToAllImages_button->Name = L"ApplyPointsToAllImages_button";
			this->ApplyPointsToAllImages_button->Size = System::Drawing::Size(100, 25);
			this->ApplyPointsToAllImages_button->TabIndex = 13;
			this->ApplyPointsToAllImages_button->Text = L"ToAllImages";
			this->ApplyPointsToAllImages_button->UseVisualStyleBackColor = false;
			this->ApplyPointsToAllImages_button->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::ApplyPointsToAllImages_button_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->ForeColor = System::Drawing::Color::White;
			this->label6->Location = System::Drawing::Point(12, 492);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(178, 15);
			this->label6->TabIndex = 12;
			this->label6->Text = L"CollisionPointData/碰撞資料";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(7, 52);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(81, 15);
			this->label5->TabIndex = 11;
			this->label5->Text = L"Images/圖片";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(15, 12);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(74, 15);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Mode/模式";
			// 
			// AllPointData_listBox
			// 
			this->AllPointData_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->AllPointData_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPointData_listBox->FormattingEnabled = true;
			this->AllPointData_listBox->ItemHeight = 15;
			this->AllPointData_listBox->Location = System::Drawing::Point(13, 511);
			this->AllPointData_listBox->Margin = System::Windows::Forms::Padding(4);
			this->AllPointData_listBox->Name = L"AllPointData_listBox";
			this->AllPointData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllPointData_listBox->Size = System::Drawing::Size(233, 214);
			this->AllPointData_listBox->TabIndex = 6;
			this->AllPointData_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &ImageConvexHullEditor::AllPointData_listBox_KeyUp);
			// 
			// OpenCollisionFile_button
			// 
			this->OpenCollisionFile_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(114)), 
				static_cast<System::Int32>(static_cast<System::Byte>(114)), static_cast<System::Int32>(static_cast<System::Byte>(114)));
			this->OpenCollisionFile_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->OpenCollisionFile_button->Location = System::Drawing::Point(263, 186);
			this->OpenCollisionFile_button->Margin = System::Windows::Forms::Padding(4);
			this->OpenCollisionFile_button->Name = L"OpenCollisionFile_button";
			this->OpenCollisionFile_button->Size = System::Drawing::Size(100, 25);
			this->OpenCollisionFile_button->TabIndex = 26;
			this->OpenCollisionFile_button->Text = L"OpenCollisionFile";
			this->OpenCollisionFile_button->UseVisualStyleBackColor = false;
			this->OpenCollisionFile_button->Click += gcnew System::EventHandler(this, &ImageConvexHullEditor::OpenCollisionFile_button_Click);
			// 
			// ImageConvexHullEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"ImageConvexHullEditor";
			this->Size = System::Drawing::Size(1304, 762);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ConverxIndex_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CollisionDataSphereRadius_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SphereRadius_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( sender == saveToolStripMenuItem )
				 {
					 if(m_pPuzzleImage&&m_p2DImageCollisionData->m_ImageCollisionDataVector.size())
					 {
						System::String^l_strFileName = DNCT::SaveFileAndGetName("Collision files (*.collision)|*.collision|All files (*.*)|*.*");
						if( l_strFileName )
						{
							std::string	l_strFileNmae = UT::WcharToChar(m_pPuzzleImage->GetName());
							m_p2DImageCollisionData->Export(DNCT::GcStringToChar(l_strFileName),m_pPuzzleImage);
						}
					 }
				 }
				 else
				 {
					String^l_strFileName = DNCT::OpenFileAndGetName("Collision files (*.collision)|*.collision|All files (*.*)|*.*");
					OpenCollisionFile(l_strFileName);
				 }
			 }
	private: System::Void openPiFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				String^l_strFileName = DNCT::OpenFileAndGetName("pi files (*.pi)|*.pi|All files (*.*)|*.*");
				if( l_strFileName )
				{
					cImageParser	l_ImageParser;
					l_ImageParser.SetFromResource(true);
					OpenPIFile(DNCT::GcStringToChar(l_strFileName),true);
				}
			 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			wglMakeCurrent( m_HdcMV,m_HGLRCMV );
			DEFAULT_SHADER = L"BTPathShader";
			NO_TEXTURE_SHADER = L"BT_NoTextureShader";
			UseShaderProgram();
			glViewport(0,0,splitContainer1->Panel2->Width,splitContainer1->Panel2->Height);
			m_pTimeAndFPS->Update();
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor( g_vBackgroundColor.x,g_vBackgroundColor.y,g_vBackgroundColor.z,g_vBackgroundColor.w );
			glClearDepth(1.0f);											// Depth Buffer Setup
			POINT l_ViewportSize = { splitContainer1->Panel2->Width,splitContainer1->Panel2->Height };
			m_pOrthogonalCamera->Render();
			m_pOrthogonalCamera->DrawGrid();
			POINT	l_Pos;l_Pos.x = l_Pos.y = 0;
			RenderRectangle(l_Pos,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT,Vector4(1.f,0.3f,0.3f,0.8f));
			POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
			Vector4	l_vColor(1,1,1,1);
			if( CollisionTest_checkBox->Checked )
			{
				float	l_fRadius = (float)SphereRadius_numericUpDown->Value;
				if(m_p2DImageCollisionData->m_pCurrentImageCollisionData)
				{
					cMatrix44	l_matSphere = cMatrix44::TranslationMatrix(Vector3((float)ptCursor.x,(float)ptCursor.y,0.f));
					if(m_p2DImageCollisionData->m_pCurrentImageCollisionData->Collide(l_fRadius,l_matSphere) != -1)
					{
						l_vColor.y = 0.f;
						l_vColor.z = 0.f;
					}
					//if(m_p2DImageCollisionData->Collide(l_fRadius,l_matSphere) )
					//{
					//	l_vColor.y = 0.f;
					//	l_vColor.z = 0.f;	
					//}
				}
				RenderCircle(Vector2((float)ptCursor.x,(float)ptCursor.y),l_fRadius,l_vColor);
			}

			if( ConverxCollisionTest_checkBox->Checked )
			{
				if( ConverxIndex_numericUpDown->SelectedIndex <	m_p2DImageCollisionDataTestForConverxCollide->Count() )
				{
					m_p2DImageCollisionDataTestForConverxCollide->GetObject();
				}
			}

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			if(m_pPuzzleImageUnit)
			{
				sPuzzleData*l_pPuzzleData = m_pPuzzleImageUnit->GetPuzzleData();
				m_pPuzzleImageUnit->Render(-l_pPuzzleData->OriginalSize.x/2+l_pPuzzleData->OffsetPos.x,-l_pPuzzleData->OriginalSize.y/2+l_pPuzzleData->OffsetPos.y);
				if( m_p2DImageCollisionData->GetSelectedIndex() != -1)
				{
					glDisable(GL_TEXTURE_2D);
					//m_p2DImageCollisionData->Render(Vector4((l_vColor.x*255),(l_vColor.y*255),(l_vColor.z*255),1.f));
					m_p2DImageCollisionData->Render(l_vColor);
				}
			}
			if( m_pDebugFont )
			{
				glEnable(GL_TEXTURE_2D);
				WCHAR*l_strMousePos;
				l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString()+","+ptCursor.y.ToString());
				m_pDebugFont->RenderFont((int)(m_pOrthogonalCamera->GetCameraPos().x-m_pOrthogonalCamera->GetScreenViewPortSize().x/2),(int)(m_pOrthogonalCamera->GetCameraPos().y-m_pOrthogonalCamera->GetScreenViewPortSize().y/2),l_strMousePos);
				glDisable(GL_TEXTURE_2D);
			}
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_ALPHA_TEST);
			//GLRender::glDisable2D();
			SwapBuffers (m_HdcMV);
			//wglMakeCurrent( 0,0 );
		 }
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if(!timer1->Enabled)
					return;
				//
				POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer1->Panel2->Size.Width,(float)splitContainer1->Panel2->Size.Height));
				//m_pCurveManager->MouseMove(ptCursor);
				m_p2DImageCollisionData->MouseMove(ptCursor,Mode_comboBox->SelectedIndex);
			 }
	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if(!timer1->Enabled)
					return;
				splitContainer1->Panel2->Focus();
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer1->Panel2->Size.Width,(float)splitContainer1->Panel2->Size.Height));

				if( PuzzleImageUnit_listBox->SelectedIndex != -1 )
				{
					if( e->Button == GCFORM::MouseButtons::Left )
					{
						POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
						if( Mode_comboBox->SelectedIndex == 0 )//1 for fix point, 0 for add point
						{
							//m_pCurveManager->GetObject(PuzzleImageUnit_listBox->SelectedIndex)->AddPoint(Vector2((float)ptCursor.x,(float)ptCursor.y),0);
							if( m_p2DImageCollisionData->m_pCurrentImageCollisionData && m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData )
							{
								if( m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->bConvex ||
									m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->pCircleData == 0)
								{
									AllPointData_listBox->Items->Add(AllPointData_listBox->Items->Count.ToString()+":"+"Pos:"+ptCursor.x.ToString()+","+ptCursor.y.ToString());
								}
								else
								if(m_p2DImageCollisionData->m_pCurrentImageCollisionData&&
									 m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData&&
									 m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->pCircleData)
								{
									AllPointData_listBox->Items->Clear();
									AllPointData_listBox->Items->Add(AllPointData_listBox->Items->Count.ToString()+":"+"Pos:"+ptCursor.x.ToString()+","+ptCursor.y.ToString());
								}

							}
							//System::EventArgs^  e3;
							//WholePath_listBox_SelectedIndexChanged(WholePath_listBox,e3);
						}
						else
						{
							//m_pCurveManager->MouseDown(ptCursor);
						}
						m_p2DImageCollisionData->MouseDown(ptCursor,Mode_comboBox->SelectedIndex);
					}
				}
			 }
	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				if(!timer1->Enabled)
					return;
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer1->Panel2->Size.Width,(float)splitContainer1->Panel2->Size.Height));
				POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
				//m_pCurveManager->MouseUp(ptCursor);
				m_p2DImageCollisionData->MouseUp(ptCursor,Mode_comboBox->SelectedIndex);
			 }
	//unform
	private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if( m_pTabPage != m_pTabControl->SelectedTab)
					timer1->Enabled = false;
				else
				{
					timer1->Enabled = true;
					wglMakeCurrent( m_HdcMV,m_HGLRCMV );
					glViewport(0,0,splitContainer1->Panel1->Width,splitContainer1->Panel1->Height);
				}
			 }

private: System::Void PuzzleImageUnit_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 AllPointData_listBox->Items->Clear();
			 AllCollisionShape_listBox->Items->Clear();
			 m_p2DImageCollisionData->SetSelectedIndex( PuzzleImageUnit_listBox->SelectedIndex );
			 if( PuzzleImageUnit_listBox->SelectedIndex == -1 )
			 {
				m_pPuzzleImageUnit = 0;
			 }
			 else
			 {
				 WCHAR*l_strName = DNCT::GcStringToWchar(PuzzleImageUnit_listBox->Items[PuzzleImageUnit_listBox->SelectedIndex]->ToString());
				 m_pPuzzleImageUnit = m_pPuzzleImage->GetObject(l_strName);
				 if( m_pPuzzleImageUnit )
				 {
					 if(m_p2DImageCollisionData->m_ImageCollisionDataVector.size() > m_p2DImageCollisionData->GetSelectedIndex())
					 {
						 c2DImageCollisionDataForEditor::sImageCollisionData*l_pImageCollisionData = m_p2DImageCollisionData->m_ImageCollisionDataVector[m_p2DImageCollisionData->GetSelectedIndex()];
						 int	l_iNum = (int)l_pImageCollisionData->CollisionDataVector.size();
						 for( int i=0;i<l_iNum;++i )
						 {
							AllCollisionShape_listBox->Items->Add(l_pImageCollisionData->CollisionDataVector[i]->bConvex?"ConvexShape":"Sphere");
						 }
					 }
				 }
			 }
		 }

private: System::Void Mode_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 //if( m_pCurveManager )
			//	m_pCurveManager->SetPathMode((ePathMode)Mode_comboBox->SelectedIndex);
		 }
private: System::Void ApplyPointsToAllImages_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( PuzzleImageUnit_listBox->SelectedIndex != -1 )
			 {
				 if( m_p2DImageCollisionData->GetSelectedIndex() != -1 )
				 {
					WARING_YES_NO_TO_YES("apply first collision data to all images?")
					{
						int	l_iNum = (int)m_p2DImageCollisionData->m_ImageCollisionDataVector.size();
						if( l_iNum )
						{
							c2DImageCollisionDataForEditor::sImageCollisionData*l_pImageCollisionData = m_p2DImageCollisionData->m_ImageCollisionDataVector[0];
							for( int i=1;i<l_iNum;++i )
							{
								c2DImageCollisionDataForEditor::sImageCollisionData*l_pOldData = m_p2DImageCollisionData->m_ImageCollisionDataVector[i];
								SAFE_DELETE(l_pOldData);
								c2DImageCollisionDataForEditor::sImageCollisionData*l_pNewData = new c2DImageCollisionDataForEditor::sImageCollisionData(l_pImageCollisionData);
								m_p2DImageCollisionData->m_ImageCollisionDataVector[i] = l_pNewData;
							}
						}
					}
				 }
			 }
		 }
private: System::Void AllPointData_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if( e->KeyCode == GCFORM::Keys::Delete )
			 {
				 while(this->AllPointData_listBox->SelectedIndices->Count)
				 {
					int	l_iIndex = AllPointData_listBox->SelectedIndices[0];
					AllPointData_listBox->Items->RemoveAt(l_iIndex);
					if(m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->bConvex)
					{
						m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData.erase(
							m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData.begin()+l_iIndex);
					}
				 }
			 }
		 }

private: System::Void CollisionDataAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == CollisionDataDel_button )
			 {
				 while(this->AllCollisionShape_listBox->SelectedIndices->Count)
				 {
					int	l_iIndex = AllCollisionShape_listBox->SelectedIndices[0];
					AllCollisionShape_listBox->Items->RemoveAt(l_iIndex);
					m_p2DImageCollisionData->m_pCurrentImageCollisionData->CollisionDataVector.erase(m_p2DImageCollisionData->m_pCurrentImageCollisionData->CollisionDataVector.begin()+l_iIndex);
				 }
			 }
			 else
			 {
				 if( m_p2DImageCollisionData->GetSelectedIndex() != -1)
				 {
					 if( CollisionType_comboBox->SelectedIndex != -1 )
					 {
						 c2DImageCollisionDataForEditor::sImageCollisionData::sCollisionData*l_pCollisionData = new c2DImageCollisionDataForEditor::sImageCollisionData::sCollisionData;
						 //0 for sphere 1 for convex
						 if( CollisionType_comboBox->SelectedIndex == 1 )
							l_pCollisionData->bConvex = true;
						 else
							l_pCollisionData->bConvex = false;
						 m_p2DImageCollisionData->m_ImageCollisionDataVector[m_p2DImageCollisionData->GetSelectedIndex()]->CollisionDataVector.push_back(l_pCollisionData);
						 AllCollisionShape_listBox->Items->Add(CollisionType_comboBox->SelectedIndex==0?"Sphere":"ConvexShape");
						 AllCollisionShape_listBox->SelectedIndices->Clear();
						 AllCollisionShape_listBox->SelectedIndex = AllCollisionShape_listBox->Items->Count-1;
					 }
				 }
			 }
		 }

private: System::Void AllCollisionShape_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 AllPointData_listBox->Items->Clear();
			 if(m_p2DImageCollisionData->m_pCurrentImageCollisionData)
			 {
				 m_p2DImageCollisionData->m_pCurrentImageCollisionData->SetSelectedIndex( AllCollisionShape_listBox->SelectedIndex );
				 if(m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData)
				 {
					 if( m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->bConvex )
					 {
						 int	l_iNum = (int)m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData.size();
						 for( int i=0;i<l_iNum;++i )
						 {
							 POINT	l_Pos;
							 l_Pos.x = (int)m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData[i].x;
							 l_Pos.y = (int)m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->ConvexHullPointData[i].y;
							 AllPointData_listBox->Items->Add(AllPointData_listBox->Items->Count.ToString()+":"+"Pos:"+l_Pos.x.ToString()+","+l_Pos.y.ToString());
						 }
					 }
					 else
					 {
						 if( m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->pCircleData )
						 {
							 Vector3	l_vPos = m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->pCircleData->vPos;
							 AllPointData_listBox->Items->Add(AllPointData_listBox->Items->Count.ToString()+":"+"Pos:"+((int)l_vPos.x).ToString()+","+((int)l_vPos.y).ToString());
						 }
					 }
				 }
			 }
		 }
private: System::Void CollisionDataSphereRadius_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_p2DImageCollisionData->m_pCurrentImageCollisionData&&
				 m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData&&
				 m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->pCircleData)
			 {
				 m_p2DImageCollisionData->m_pCurrentImageCollisionData->pCurrentCollisionData->pCircleData->fRadius = (float)CollisionDataSphereRadius_numericUpDown->Value;
			 }
		 }
private: System::Void OpenCollisionFile_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 SAFE_DELETE(m_p2DImageCollisionDataTestForConverxCollide);
			 String^l_strFileName = DNCT::OpenFileAndGetName("Collision files (*.collision)|*.collision|All files (*.*)|*.*");
			 m_p2DImageCollisionDataTestForConverxCollide = new c2DImageCollisionData;
			 m_p2DImageCollisionDataTestForConverxCollide->Parse(DNCT::GcStringToWchar(l_strFileName));
		 }
};
}
