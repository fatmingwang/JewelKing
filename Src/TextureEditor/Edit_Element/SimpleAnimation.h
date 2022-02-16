#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMCEditor 
{

	/// <summary>
	/// SimpleAnimation 的摘要
	/// </summary>
	public ref class SimpleAnimation : public System::Windows::Forms::UserControl
	{
	public:
		SimpleAnimation(GCFORM::TabControl^e_ptabControl)
		{
			InitializeComponent();
			this->Dock = GCFORM::DockStyle::Fill;
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pTimeAndFPS = new UT::sTimeAndFPS;
			m_bDragObject = false;
			m_pTabPage = gcnew GCFORM::TabPage();
			m_pTabPage->Text = "SimpleAnimation/動畫";
			m_pTabPage->AutoScroll = true;
			m_pTabPage->Controls->Add(this);
			e_ptabControl->TabPages->Add(m_pTabPage);
			m_pTabControl = e_ptabControl;
			e_ptabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &SimpleAnimation::tabControl_SelectedIndexChanged);
			splitContainer3->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &SimpleAnimation::MyMouseDown);
			splitContainer3->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &SimpleAnimation::MyMouseMove);
			splitContainer3->Panel1->MouseUp   += gcnew System::Windows::Forms::MouseEventHandler(this, &SimpleAnimation::MyMouseUp);
			splitContainer3->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &SimpleAnimation::MyMouseUp);
			m_bPreventValueChanged = true;
			m_pImageShowInfoState = new cStaticAnimation();
			m_pOrthogonalCamera = new cOrthogonalCamera();
			//AutoScaleChildrenForm^l_pAutoScaleChildrenForm = gcnew AutoScaleChildrenForm(m_pTabPage,this);
			LanguageSwitch(this->Controls,"/",this->Handle);
			LanguageSwitch(splitContainer2->Panel1->Controls,"/",this->Handle);
			LanguageSwitch(Data_groupBox->Controls,"/",this->Handle);
			m_HdcMV = GetDC((HWND)this->splitContainer3->Panel1->Handle.ToPointer());
			m_HGLRCMV = UT::InitOpenGL((HWND)this->splitContainer3->Panel1->Handle.ToPointer(),true,m_HdcMV);
			m_pDebugFont = 0;
			if(System::IO::File::Exists("Font.png"))
			{
				m_pDebugFont = new cGlyphFontRender("Font");
				if( !m_pDebugFont->GetFontImage() )
					SAFE_DELETE(m_pDebugFont);
			}
			CreateShader(g_bCommonVSClientState,L"SAPathShader");
			CreateShader(g_bCommonVSNoTextureClientState,L"SA_NoTextureShader");
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~SimpleAnimation()
		{
			SAFE_DELETE(m_pTimeAndFPS);
			DeleteShader(L"SAPathShader");
			DeleteShader(L"SA_NoTextureShader");
			SAFE_DELETE(m_pDebugFont);
			if (components)
			{
				delete components;
			}
			//SAFE_DELETE(m_pImagePlayManager);
			//SAFE_DELETE(m_pImageShowInfoState);
		}
	private: System::Windows::Forms::Button^  AddImageToPlayList_button;
	private: System::Windows::Forms::Button^  DeletePlayList_button;
	private: System::Windows::Forms::ListBox^  PlayList_listBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  PlayCount_numericUpDown;
	private: System::Windows::Forms::CheckBox^  EnableLoop_checkBox;
	private: System::Windows::Forms::ListBox^  ImagePlaySec_listBox;
	private: System::Windows::Forms::Button^  PlayImageanimation_button;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  ImageMoveown_button;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  perImagePlaySec_numericUpDown;
	private: System::Windows::Forms::ListBox^  WholeImageInfo_listBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  AllPlaySec_numericUpDown;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  ImageMoveUp_button;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::NumericUpDown^  OffsetPosX_numericUpDown;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  OffsetPosY_numericUpDown;
	private: System::Windows::Forms::Button^  DeleteListObject_button;
	private: System::Windows::Forms::Button^  AddAnimationToList_button;
	private: System::Windows::Forms::ListBox^  WholeAnimationList_listBox;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  arningForSecond_label;
	private: System::Windows::Forms::TextBox^  AnimationName_textBox;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Button^  AnimationInvert_button;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::NumericUpDown^  Angle_numericUpDown;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openImageFilesToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  composeImageDataToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  Mirror_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::GroupBox^  Data_groupBox;
	private: System::Windows::Forms::GroupBox^  WholeImage_groupBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::ToolStripMenuItem^  openImageDataToolStripMenuItem;
	//my
	private: GCFORM::TabControl^m_pTabControl;				//to determin is tabpage clicked.
	HGLRC	 m_HGLRCMV;
	HDC		 m_HdcMV;
	cGlyphFontRender*m_pDebugFont;
    bool	m_bPreventValueChanged;					//we do not hope the value is changed while we just click imagelist,we would like to change the value if the numeralbar value is changed
	bool	m_bDragObject;							//if we drag the object it's true
	String^m_strCurrentFileName;
	cOrthogonalCamera*				m_pOrthogonalCamera;
	cSAList*					m_pImagePlayerSA;
	cStaticAnimation*		m_pImageShowInfoState;
	cBaseImage*				m_pBKImageSA;
	cAnimationParser*		m_pImagePlayManager;
	bool					m_bPreventMuthreadError;
	UT::sTimeAndFPS			*m_pTimeAndFPS;
private: System::Windows::Forms::CheckBox^  Retain_checkBox;

	private: GCFORM::TabPage^m_pTabPage;					//for attach from.
	private: System::Void SaveImagePlayerData();

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
			this->AddImageToPlayList_button = (gcnew System::Windows::Forms::Button());
			this->DeletePlayList_button = (gcnew System::Windows::Forms::Button());
			this->PlayList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->PlayCount_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->EnableLoop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImagePlaySec_listBox = (gcnew System::Windows::Forms::ListBox());
			this->PlayImageanimation_button = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->ImageMoveown_button = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->perImagePlaySec_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->WholeImageInfo_listBox = (gcnew System::Windows::Forms::ListBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->AllPlaySec_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ImageMoveUp_button = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->OffsetPosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->OffsetPosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DeleteListObject_button = (gcnew System::Windows::Forms::Button());
			this->AddAnimationToList_button = (gcnew System::Windows::Forms::Button());
			this->WholeAnimationList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->arningForSecond_label = (gcnew System::Windows::Forms::Label());
			this->AnimationName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->AnimationInvert_button = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->Angle_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openImageDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openImageFilesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->composeImageDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->backgroundImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Mirror_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->Data_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->WholeImage_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->Retain_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PlayCount_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->perImagePlaySec_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AllPlaySec_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OffsetPosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OffsetPosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Angle_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->Data_groupBox->SuspendLayout();
			this->WholeImage_groupBox->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->SuspendLayout();
			// 
			// AddImageToPlayList_button
			// 
			this->AddImageToPlayList_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddImageToPlayList_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddImageToPlayList_button->Location = System::Drawing::Point(3, 3);
			this->AddImageToPlayList_button->Name = L"AddImageToPlayList_button";
			this->AddImageToPlayList_button->Size = System::Drawing::Size(46, 42);
			this->AddImageToPlayList_button->TabIndex = 49;
			this->AddImageToPlayList_button->Text = L"AddToList/加至列表";
			this->AddImageToPlayList_button->UseVisualStyleBackColor = false;
			this->AddImageToPlayList_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::AddImageToPlayList_button_Click);
			// 
			// DeletePlayList_button
			// 
			this->DeletePlayList_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeletePlayList_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeletePlayList_button->Location = System::Drawing::Point(3, 66);
			this->DeletePlayList_button->Name = L"DeletePlayList_button";
			this->DeletePlayList_button->Size = System::Drawing::Size(46, 32);
			this->DeletePlayList_button->TabIndex = 48;
			this->DeletePlayList_button->Text = L"Delete/刪除列表圖檔";
			this->DeletePlayList_button->UseVisualStyleBackColor = false;
			this->DeletePlayList_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::AddImageToPlayList_button_Click);
			// 
			// PlayList_listBox
			// 
			this->PlayList_listBox->AllowDrop = true;
			this->PlayList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PlayList_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PlayList_listBox->FormattingEnabled = true;
			this->PlayList_listBox->ItemHeight = 12;
			this->PlayList_listBox->Location = System::Drawing::Point(56, 2);
			this->PlayList_listBox->MinimumSize = System::Drawing::Size(4, 110);
			this->PlayList_listBox->Name = L"PlayList_listBox";
			this->PlayList_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->PlayList_listBox->Size = System::Drawing::Size(170, 100);
			this->PlayList_listBox->TabIndex = 47;
			this->PlayList_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &SimpleAnimation::PlayList_listBox_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::White;
			this->label5->Location = System::Drawing::Point(447, 158);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(105, 12);
			this->label5->TabIndex = 44;
			this->label5->Text = L"PlayCount/撥放次數";
			// 
			// PlayCount_numericUpDown
			// 
			this->PlayCount_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PlayCount_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PlayCount_numericUpDown->Location = System::Drawing::Point(562, 158);
			this->PlayCount_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->PlayCount_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, System::Int32::MinValue});
			this->PlayCount_numericUpDown->Name = L"PlayCount_numericUpDown";
			this->PlayCount_numericUpDown->Size = System::Drawing::Size(101, 22);
			this->PlayCount_numericUpDown->TabIndex = 43;
			this->PlayCount_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->PlayCount_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::PlayCount_numericUpDown_ValueChanged);
			// 
			// EnableLoop_checkBox
			// 
			this->EnableLoop_checkBox->AutoSize = true;
			this->EnableLoop_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->EnableLoop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EnableLoop_checkBox->Location = System::Drawing::Point(575, 79);
			this->EnableLoop_checkBox->Name = L"EnableLoop_checkBox";
			this->EnableLoop_checkBox->Size = System::Drawing::Size(100, 16);
			this->EnableLoop_checkBox->TabIndex = 42;
			this->EnableLoop_checkBox->Text = L"Loop/無限撥放";
			this->EnableLoop_checkBox->UseVisualStyleBackColor = false;
			this->EnableLoop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &SimpleAnimation::EnableLoop_checkBox_CheckedChanged);
			// 
			// ImagePlaySec_listBox
			// 
			this->ImagePlaySec_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImagePlaySec_listBox->Enabled = false;
			this->ImagePlaySec_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImagePlaySec_listBox->FormattingEnabled = true;
			this->ImagePlaySec_listBox->ItemHeight = 12;
			this->ImagePlaySec_listBox->Location = System::Drawing::Point(242, 2);
			this->ImagePlaySec_listBox->MinimumSize = System::Drawing::Size(4, 110);
			this->ImagePlaySec_listBox->Name = L"ImagePlaySec_listBox";
			this->ImagePlaySec_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->ImagePlaySec_listBox->Size = System::Drawing::Size(166, 100);
			this->ImagePlaySec_listBox->TabIndex = 41;
			// 
			// PlayImageanimation_button
			// 
			this->PlayImageanimation_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PlayImageanimation_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PlayImageanimation_button->Location = System::Drawing::Point(108, 144);
			this->PlayImageanimation_button->Name = L"PlayImageanimation_button";
			this->PlayImageanimation_button->Size = System::Drawing::Size(256, 29);
			this->PlayImageanimation_button->TabIndex = 40;
			this->PlayImageanimation_button->Text = L"Play/撥放";
			this->PlayImageanimation_button->UseVisualStyleBackColor = false;
			this->PlayImageanimation_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::PlayImageanimation_button_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(280, 112);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(132, 12);
			this->label4->TabIndex = 38;
			this->label4->Text = L"Down/變更撥放順序向下";
			// 
			// ImageMoveown_button
			// 
			this->ImageMoveown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageMoveown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageMoveown_button->Location = System::Drawing::Point(242, 103);
			this->ImageMoveown_button->Name = L"ImageMoveown_button";
			this->ImageMoveown_button->Size = System::Drawing::Size(28, 32);
			this->ImageMoveown_button->TabIndex = 37;
			this->ImageMoveown_button->Text = L"v\r\nv";
			this->ImageMoveown_button->UseVisualStyleBackColor = false;
			this->ImageMoveown_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::ImageMoveUp_button_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::Color::White;
			this->label2->Location = System::Drawing::Point(368, 100);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(190, 12);
			this->label2->TabIndex = 36;
			this->label2->Text = L"perImagePlaySecond/此圖片撥放秒數";
			// 
			// perImagePlaySec_numericUpDown
			// 
			this->perImagePlaySec_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->perImagePlaySec_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->perImagePlaySec_numericUpDown->Location = System::Drawing::Point(562, 98);
			this->perImagePlaySec_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->perImagePlaySec_numericUpDown->Name = L"perImagePlaySec_numericUpDown";
			this->perImagePlaySec_numericUpDown->Size = System::Drawing::Size(101, 22);
			this->perImagePlaySec_numericUpDown->TabIndex = 35;
			this->perImagePlaySec_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::perImagePlaySec_numericUpDown_ValueChanged);
			// 
			// WholeImageInfo_listBox
			// 
			this->WholeImageInfo_listBox->AllowDrop = true;
			this->WholeImageInfo_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeImageInfo_listBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->WholeImageInfo_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeImageInfo_listBox->FormattingEnabled = true;
			this->WholeImageInfo_listBox->ItemHeight = 12;
			this->WholeImageInfo_listBox->Location = System::Drawing::Point(2, 17);
			this->WholeImageInfo_listBox->MinimumSize = System::Drawing::Size(4, 41);
			this->WholeImageInfo_listBox->Name = L"WholeImageInfo_listBox";
			this->WholeImageInfo_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->WholeImageInfo_listBox->Size = System::Drawing::Size(232, 136);
			this->WholeImageInfo_listBox->TabIndex = 34;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::White;
			this->label1->Location = System::Drawing::Point(406, 130);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(148, 12);
			this->label1->TabIndex = 33;
			this->label1->Text = L"AllPlaySecond/全部撥放秒數";
			// 
			// AllPlaySec_numericUpDown
			// 
			this->AllPlaySec_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AllPlaySec_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPlaySec_numericUpDown->Location = System::Drawing::Point(562, 130);
			this->AllPlaySec_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000000, 0, 0, 0});
			this->AllPlaySec_numericUpDown->Name = L"AllPlaySec_numericUpDown";
			this->AllPlaySec_numericUpDown->Size = System::Drawing::Size(101, 22);
			this->AllPlaySec_numericUpDown->TabIndex = 32;
			this->AllPlaySec_numericUpDown->UseWaitCursor = true;
			this->AllPlaySec_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->AllPlaySec_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::AllPlaySec_numericUpDown_ValueChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(78, 112);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(118, 12);
			this->label3->TabIndex = 51;
			this->label3->Text = L"Up/變更撥放順序向上";
			// 
			// ImageMoveUp_button
			// 
			this->ImageMoveUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageMoveUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageMoveUp_button->Location = System::Drawing::Point(195, 103);
			this->ImageMoveUp_button->Name = L"ImageMoveUp_button";
			this->ImageMoveUp_button->Size = System::Drawing::Size(29, 32);
			this->ImageMoveUp_button->TabIndex = 50;
			this->ImageMoveUp_button->Text = L"^\r\n^\r\n";
			this->ImageMoveUp_button->UseVisualStyleBackColor = false;
			this->ImageMoveUp_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::ImageMoveUp_button_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &SimpleAnimation::timer1_Tick);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->ForeColor = System::Drawing::Color::White;
			this->label7->Location = System::Drawing::Point(375, 3);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(88, 12);
			this->label7->TabIndex = 55;
			this->label7->Text = L"OffsetX/偏移量X";
			// 
			// OffsetPosX_numericUpDown
			// 
			this->OffsetPosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->OffsetPosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->OffsetPosX_numericUpDown->Location = System::Drawing::Point(464, 2);
			this->OffsetPosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->OffsetPosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, System::Int32::MinValue});
			this->OffsetPosX_numericUpDown->Name = L"OffsetPosX_numericUpDown";
			this->OffsetPosX_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->OffsetPosX_numericUpDown->TabIndex = 54;
			this->OffsetPosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::OffsetPosX_numericUpDown_ValueChanged);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::Color::White;
			this->label8->Location = System::Drawing::Point(508, 4);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(88, 12);
			this->label8->TabIndex = 53;
			this->label8->Text = L"OffsetY/偏移量Y";
			// 
			// OffsetPosY_numericUpDown
			// 
			this->OffsetPosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->OffsetPosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->OffsetPosY_numericUpDown->Location = System::Drawing::Point(598, 2);
			this->OffsetPosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->OffsetPosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000000, 0, 0, System::Int32::MinValue});
			this->OffsetPosY_numericUpDown->Name = L"OffsetPosY_numericUpDown";
			this->OffsetPosY_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->OffsetPosY_numericUpDown->TabIndex = 52;
			this->OffsetPosY_numericUpDown->UseWaitCursor = true;
			this->OffsetPosY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->OffsetPosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::OffsetPosX_numericUpDown_ValueChanged);
			// 
			// DeleteListObject_button
			// 
			this->DeleteListObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeleteListObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteListObject_button->Location = System::Drawing::Point(76, 39);
			this->DeleteListObject_button->Name = L"DeleteListObject_button";
			this->DeleteListObject_button->Size = System::Drawing::Size(85, 22);
			this->DeleteListObject_button->TabIndex = 57;
			this->DeleteListObject_button->Text = L"DeleteListObject/刪除物件";
			this->DeleteListObject_button->UseVisualStyleBackColor = false;
			this->DeleteListObject_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::AddAnimationToList_button_Click);
			// 
			// AddAnimationToList_button
			// 
			this->AddAnimationToList_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddAnimationToList_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAnimationToList_button->Location = System::Drawing::Point(5, 39);
			this->AddAnimationToList_button->Name = L"AddAnimationToList_button";
			this->AddAnimationToList_button->Size = System::Drawing::Size(69, 22);
			this->AddAnimationToList_button->TabIndex = 56;
			this->AddAnimationToList_button->Text = L"AddToList/增加到列表";
			this->AddAnimationToList_button->UseVisualStyleBackColor = false;
			this->AddAnimationToList_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::AddAnimationToList_button_Click);
			// 
			// WholeAnimationList_listBox
			// 
			this->WholeAnimationList_listBox->AllowDrop = true;
			this->WholeAnimationList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeAnimationList_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationList_listBox->FormattingEnabled = true;
			this->WholeAnimationList_listBox->ItemHeight = 12;
			this->WholeAnimationList_listBox->Location = System::Drawing::Point(5, 68);
			this->WholeAnimationList_listBox->MinimumSize = System::Drawing::Size(4, 69);
			this->WholeAnimationList_listBox->Name = L"WholeAnimationList_listBox";
			this->WholeAnimationList_listBox->ScrollAlwaysVisible = true;
			this->WholeAnimationList_listBox->Size = System::Drawing::Size(220, 100);
			this->WholeAnimationList_listBox->TabIndex = 58;
			this->WholeAnimationList_listBox->DoubleClick += gcnew System::EventHandler(this, &SimpleAnimation::WholeAnimationList_listBox_DoubleClick);
			this->WholeAnimationList_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &SimpleAnimation::WholeAnimationList_listBox_KeyUp);
			// 
			// arningForSecond_label
			// 
			this->arningForSecond_label->AutoSize = true;
			this->arningForSecond_label->Location = System::Drawing::Point(684, 141);
			this->arningForSecond_label->Name = L"arningForSecond_label";
			this->arningForSecond_label->Size = System::Drawing::Size(0, 12);
			this->arningForSecond_label->TabIndex = 59;
			// 
			// AnimationName_textBox
			// 
			this->AnimationName_textBox->Location = System::Drawing::Point(112, 13);
			this->AnimationName_textBox->Name = L"AnimationName_textBox";
			this->AnimationName_textBox->Size = System::Drawing::Size(114, 22);
			this->AnimationName_textBox->TabIndex = 61;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(-39, 16);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(147, 12);
			this->label9->TabIndex = 62;
			this->label9->Text = L"Animation Name/此動畫名稱";
			// 
			// AnimationInvert_button
			// 
			this->AnimationInvert_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationInvert_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->AnimationInvert_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationInvert_button->Location = System::Drawing::Point(167, 39);
			this->AnimationInvert_button->Name = L"AnimationInvert_button";
			this->AnimationInvert_button->Size = System::Drawing::Size(65, 22);
			this->AnimationInvert_button->TabIndex = 63;
			this->AnimationInvert_button->Text = L"ReEdit/重新編輯";
			this->AnimationInvert_button->UseVisualStyleBackColor = false;
			this->AnimationInvert_button->Click += gcnew System::EventHandler(this, &SimpleAnimation::AnimationInvert_button_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->ForeColor = System::Drawing::Color::White;
			this->label10->Location = System::Drawing::Point(535, 58);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(60, 12);
			this->label10->TabIndex = 65;
			this->label10->Text = L"Angle/角度";
			// 
			// Angle_numericUpDown
			// 
			this->Angle_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Angle_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Angle_numericUpDown->Location = System::Drawing::Point(597, 53);
			this->Angle_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->Angle_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {10000000, 0, 0, System::Int32::MinValue});
			this->Angle_numericUpDown->Name = L"Angle_numericUpDown";
			this->Angle_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->Angle_numericUpDown->TabIndex = 64;
			this->Angle_numericUpDown->UseWaitCursor = true;
			this->Angle_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->Angle_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::OffsetPosX_numericUpDown_ValueChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->ForeColor = System::Drawing::Color::White;
			this->label11->Location = System::Drawing::Point(388, 31);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(73, 12);
			this->label11->TabIndex = 69;
			this->label11->Text = L"Width/圖片寬";
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Width_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Width_numericUpDown->Location = System::Drawing::Point(462, 30);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->Width_numericUpDown->TabIndex = 68;
			this->Width_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::OffsetPosX_numericUpDown_ValueChanged);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->ForeColor = System::Drawing::Color::White;
			this->label12->Location = System::Drawing::Point(520, 31);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(75, 12);
			this->label12->TabIndex = 67;
			this->label12->Text = L"Height/圖片高";
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Height_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Height_numericUpDown->Location = System::Drawing::Point(597, 28);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(69, 22);
			this->Height_numericUpDown->TabIndex = 66;
			this->Height_numericUpDown->UseWaitCursor = true;
			this->Height_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->Height_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &SimpleAnimation::OffsetPosX_numericUpDown_ValueChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->backgroundImageToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(960, 24);
			this->menuStrip1->TabIndex = 70;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->openImageDataToolStripMenuItem, 
				this->openImageFilesToolStripMenuItem, this->composeImageDataToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(34, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openImageDataToolStripMenuItem
			// 
			this->openImageDataToolStripMenuItem->Name = L"openImageDataToolStripMenuItem";
			this->openImageDataToolStripMenuItem->Size = System::Drawing::Size(264, 22);
			this->openImageDataToolStripMenuItem->Text = L"Open Image Data/開檔(文字檔)";
			this->openImageDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &SimpleAnimation::OpenImageData_button_Click);
			// 
			// openImageFilesToolStripMenuItem
			// 
			this->openImageFilesToolStripMenuItem->Name = L"openImageFilesToolStripMenuItem";
			this->openImageFilesToolStripMenuItem->Size = System::Drawing::Size(264, 22);
			this->openImageFilesToolStripMenuItem->Text = L"OpenImageFiles/開啟合併圖檔(文字檔)";
			this->openImageFilesToolStripMenuItem->Click += gcnew System::EventHandler(this, &SimpleAnimation::OpenImageFilesBySelect_button_Click);
			// 
			// composeImageDataToolStripMenuItem
			// 
			this->composeImageDataToolStripMenuItem->Name = L"composeImageDataToolStripMenuItem";
			this->composeImageDataToolStripMenuItem->Size = System::Drawing::Size(264, 22);
			this->composeImageDataToolStripMenuItem->Text = L"Compose Image Data/存檔";
			this->composeImageDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &SimpleAnimation::ComposeImageData_button_Click);
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(158, 20);
			this->backgroundImageToolStripMenuItem->Text = L"Background Image/背景圖片";
			this->backgroundImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &SimpleAnimation::BKImage_button_Click);
			// 
			// Mirror_checkBox
			// 
			this->Mirror_checkBox->AutoSize = true;
			this->Mirror_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Mirror_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Mirror_checkBox->Location = System::Drawing::Point(480, 58);
			this->Mirror_checkBox->Name = L"Mirror_checkBox";
			this->Mirror_checkBox->Size = System::Drawing::Size(55, 16);
			this->Mirror_checkBox->TabIndex = 71;
			this->Mirror_checkBox->Text = L"Mirror";
			this->Mirror_checkBox->UseVisualStyleBackColor = false;
			this->Mirror_checkBox->CheckedChanged += gcnew System::EventHandler(this, &SimpleAnimation::Mirror_checkBox_CheckedChanged);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->flowLayoutPanel1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Panel2->Controls->Add(this->arningForSecond_label);
			this->splitContainer1->Size = System::Drawing::Size(960, 552);
			this->splitContainer1->SplitterDistance = 241;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 72;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->flowLayoutPanel1->Controls->Add(this->Data_groupBox);
			this->flowLayoutPanel1->Controls->Add(this->WholeImage_groupBox);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(237, 548);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// Data_groupBox
			// 
			this->Data_groupBox->Controls->Add(this->label9);
			this->Data_groupBox->Controls->Add(this->AddAnimationToList_button);
			this->Data_groupBox->Controls->Add(this->DeleteListObject_button);
			this->Data_groupBox->Controls->Add(this->WholeAnimationList_listBox);
			this->Data_groupBox->Controls->Add(this->AnimationName_textBox);
			this->Data_groupBox->Controls->Add(this->AnimationInvert_button);
			this->Data_groupBox->ForeColor = System::Drawing::Color::White;
			this->Data_groupBox->Location = System::Drawing::Point(0, 0);
			this->Data_groupBox->Margin = System::Windows::Forms::Padding(0);
			this->Data_groupBox->Name = L"Data_groupBox";
			this->Data_groupBox->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Data_groupBox->Size = System::Drawing::Size(238, 174);
			this->Data_groupBox->TabIndex = 73;
			this->Data_groupBox->TabStop = false;
			this->Data_groupBox->Text = L"Data";
			// 
			// WholeImage_groupBox
			// 
			this->WholeImage_groupBox->Controls->Add(this->WholeImageInfo_listBox);
			this->WholeImage_groupBox->ForeColor = System::Drawing::Color::White;
			this->WholeImage_groupBox->Location = System::Drawing::Point(2, 176);
			this->WholeImage_groupBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->WholeImage_groupBox->Name = L"WholeImage_groupBox";
			this->WholeImage_groupBox->Padding = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->WholeImage_groupBox->Size = System::Drawing::Size(236, 157);
			this->WholeImage_groupBox->TabIndex = 73;
			this->WholeImage_groupBox->TabStop = false;
			this->WholeImage_groupBox->Text = L"WholeImage";
			// 
			// splitContainer2
			// 
			this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->splitContainer2->Panel1->Controls->Add(this->Retain_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->AddImageToPlayList_button);
			this->splitContainer2->Panel1->Controls->Add(this->label8);
			this->splitContainer2->Panel1->Controls->Add(this->ImageMoveUp_button);
			this->splitContainer2->Panel1->Controls->Add(this->OffsetPosY_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->DeletePlayList_button);
			this->splitContainer2->Panel1->Controls->Add(this->label3);
			this->splitContainer2->Panel1->Controls->Add(this->Mirror_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->PlayList_listBox);
			this->splitContainer2->Panel1->Controls->Add(this->AllPlaySec_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label5);
			this->splitContainer2->Panel1->Controls->Add(this->OffsetPosX_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->PlayCount_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label1);
			this->splitContainer2->Panel1->Controls->Add(this->ImagePlaySec_listBox);
			this->splitContainer2->Panel1->Controls->Add(this->label7);
			this->splitContainer2->Panel1->Controls->Add(this->EnableLoop_checkBox);
			this->splitContainer2->Panel1->Controls->Add(this->label11);
			this->splitContainer2->Panel1->Controls->Add(this->Angle_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->perImagePlaySec_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->PlayImageanimation_button);
			this->splitContainer2->Panel1->Controls->Add(this->Height_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label10);
			this->splitContainer2->Panel1->Controls->Add(this->label2);
			this->splitContainer2->Panel1->Controls->Add(this->label4);
			this->splitContainer2->Panel1->Controls->Add(this->Width_numericUpDown);
			this->splitContainer2->Panel1->Controls->Add(this->label12);
			this->splitContainer2->Panel1->Controls->Add(this->ImageMoveown_button);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->splitContainer3);
			this->splitContainer2->Size = System::Drawing::Size(716, 552);
			this->splitContainer2->SplitterDistance = 196;
			this->splitContainer2->SplitterWidth = 3;
			this->splitContainer2->TabIndex = 72;
			// 
			// Retain_checkBox
			// 
			this->Retain_checkBox->AutoSize = true;
			this->Retain_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Retain_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Retain_checkBox->Location = System::Drawing::Point(480, 79);
			this->Retain_checkBox->Name = L"Retain_checkBox";
			this->Retain_checkBox->Size = System::Drawing::Size(54, 16);
			this->Retain_checkBox->TabIndex = 72;
			this->Retain_checkBox->Text = L"Retain";
			this->Retain_checkBox->UseVisualStyleBackColor = false;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->splitContainer3->Name = L"splitContainer3";
			this->splitContainer3->Panel2Collapsed = true;
			this->splitContainer3->Size = System::Drawing::Size(712, 349);
			this->splitContainer3->SplitterDistance = 316;
			this->splitContainer3->SplitterWidth = 3;
			this->splitContainer3->TabIndex = 0;
			// 
			// SimpleAnimation
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"SimpleAnimation";
			this->Size = System::Drawing::Size(960, 576);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PlayCount_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->perImagePlaySec_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AllPlaySec_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OffsetPosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OffsetPosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Angle_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			this->splitContainer1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->Data_groupBox->ResumeLayout(false);
			this->Data_groupBox->PerformLayout();
			this->WholeImage_groupBox->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->splitContainer3->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void OpenImageFilesBySelect_button_Click(System::Object^  sender, System::EventArgs^  e) {
				 String^l_FileName = DNCT::OpenFileAndGetName("(*.pi)|*.pi");
				 if( l_FileName )
				 {
					 //if the data is exited,kill old one
					 //fetch data from store if it does exited just kill father.else kill the UV
					SAFE_DELETE(m_pImagePlayManager);
					SAFE_DELETE(m_pImageShowInfoState);
					m_pImagePlayerSA = new cSAList();
					m_pImageShowInfoState = new cStaticAnimation();
					m_pImagePlayManager = new cAnimationParser();
					m_pImagePlayManager->AddObject(m_pImagePlayerSA);

					WholeImageInfo_listBox->Items->Clear();
					WholeAnimationList_listBox->Items->Clear();
					PlayList_listBox->Items->Clear();
					ImagePlaySec_listBox->Items->Clear();

					bool	l_b = m_pImagePlayManager->GetAllBaseImageList()->Parse(DNCT::GcStringToChar(l_FileName));
					if( l_b && m_pImagePlayManager->Count())
					{
						cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>((*m_pImagePlayManager->GetAllBaseImageList())[0]);
						l_pPuzzleImage->GenerateAllPuzzleImageUnit();
						if( l_pPuzzleImage->GetImageIndexOfAnimationList() )
						{
							SAFE_DELETE(m_pImagePlayerSA);
							m_pImagePlayManager->RemoveObjectWithoutDelete(0);
							m_pImagePlayerSA = l_pPuzzleImage->GetSAByImageData();
							m_pImagePlayManager->AddObject(m_pImagePlayerSA);
						}
						assert(l_b&&"parse puzzle Image Data error");
						m_pImageShowInfoState->SetPuzzleImage(l_pPuzzleImage);
						m_pImagePlayerSA->SetPuzzleImage(l_pPuzzleImage);
						for(int i=0;i<m_pImagePlayerSA->Count();++i)
							WholeAnimationList_listBox->Items->Add( DNCT::WcharToGcstring(m_pImagePlayerSA->GetObject(i)->GetName()) );
						for(int i=0;i<m_pImagePlayerSA->GetNumImage();++i)
							WholeImageInfo_listBox->Items->Add(DNCT::WcharToGcstring(m_pImagePlayerSA->GetImageName(i)));
					}
					else
						WARNING_MSG("open file failed");
				 }
			 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			DEFAULT_SHADER = L"SAPathShader";
			NO_TEXTURE_SHADER = L"SA_NoTextureShader";
			 m_pTimeAndFPS->Update();
			 float	l_fElpaseTime = m_pTimeAndFPS->fElpaseTime;
			 if( !m_bPreventMuthreadError )
			 {
				glViewport(0,0,splitContainer3->Panel1->Width,splitContainer3->Panel1->Height);
//				float	l_fElpaseTime = m_pTimeAndFPS->fElpaseTime;
				glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				glClearColor( g_vBackgroundColor.x,g_vBackgroundColor.y,g_vBackgroundColor.z,g_vBackgroundColor.w );
				glClearDepth(1.0f);											// Depth Buffer Setup
				POINT l_ViewportSize = { splitContainer3->Panel1->Width,splitContainer3->Panel1->Height };
				m_pOrthogonalCamera->Render();
				m_pOrthogonalCamera->DrawGrid();
				POINT	l_Pos = {0,0};
				RenderRectangle(l_Pos,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT,Vector4(1.f,0.3f,0.3f,0.8f));
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);
				if( m_pBKImageSA )
					m_pBKImageSA->Render(0,0);
				if( !m_pImageShowInfoState->IsDone() )
				{
					m_pImageShowInfoState->Update(l_fElpaseTime);
					m_pImageShowInfoState->Render(0,0);
				}
				else
				{
					if( m_pImagePlayManager )
					if(  m_pImagePlayManager->GetAllBaseImageList()->Count()&&PlayList_listBox->SelectedItem )
					{
						m_pImageShowInfoState->Render(PlayList_listBox->SelectedIndex);
						//sPuzzleData**l_ppPuzzleData = m_pImagePlayerSA->GetPuzzleData();
						//for( int i=0;i<m_pImagePlayerSA->GetNumImage();++i )
						//{
						//	if(!wcscmp(l_ppPuzzleData[i]->strFileName,DNCT::GcStringToWchar(PlayList_listBox->SelectedItem->ToString())))
						//	{
						//		m_pImageShowInfoState->SetCurrentPlayImageIndex( i );
						//		m_pImageShowInfoState->Render(0,0);
						//		return;
						//	}
						//}
					}
				}
				//cPuzzleImage*l_pPuzzleImage = (cPuzzleImage*)(*m_pImagePlayManager->GetAllBaseImageList())[0];
				//for( int i=0;i<l_pPuzzleImage->Count();++i )
				//{
				//	POINT	l_Pos = {i*200,i*100};
				//	l_pPuzzleImage->SetPos(l_Pos);
				//	l_pPuzzleImage->Render(i);
				//}
				if( m_pDebugFont )
				{
					glEnable(GL_TEXTURE_2D);
					POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
					WCHAR*l_strMousePos;
					l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString()+","+ptCursor.y.ToString());
					m_pDebugFont->RenderFont((int)(m_pOrthogonalCamera->GetCameraPos().x-m_pOrthogonalCamera->GetScreenViewPortSize().x/2),(int)(m_pOrthogonalCamera->GetCameraPos().y-m_pOrthogonalCamera->GetScreenViewPortSize().y/2),l_strMousePos);
					glDisable(GL_TEXTURE_2D);
				}
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_TEXTURE_2D);
				//GLRender::glDisable2D();
				SwapBuffers (m_HdcMV);
			 }
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
				glViewport(0,0,splitContainer3->Panel1->Width,splitContainer3->Panel1->Height);
			}
		 }

	template <class Type>  inline	void	EraseVectorMemberByList(std::vector<Type>*e_pTypeVector,std::list<int> *e_pEraseIndexList)
	{
		e_pEraseIndexList->sort();
		std::vector<int> l_IndexList;
		while(e_pEraseIndexList->size())
		{
			int	l_i = e_pEraseIndexList->back();
			e_pEraseIndexList->pop_back();
			l_IndexList.push_back(l_i);
		}
		for( UINT i=0;i<l_IndexList.size();++i )
		{
			e_pTypeVector->erase(e_pTypeVector->begin()+l_IndexList[i]);
		}
	};
private: System::Void AddImageToPlayList_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( sender == AddImageToPlayList_button )
			 {
				 //for each (System::Object^l_pObject in WholeImageInfo_listBox->SelectedItems)
				 for(int i=0;i<WholeImageInfo_listBox->SelectedItems->Count;++i)
				 {
					System::Object^l_pObject = WholeImageInfo_listBox->SelectedItems[i];
					PlayList_listBox->Items->Add(l_pObject);
					ImagePlaySec_listBox->Items->Add(String("0.1f").ToString());
					int	l_iIndex = WholeImageInfo_listBox->Items->IndexOf(l_pObject);
					int	l_iImageIndex = m_pImagePlayerSA->GetPuzzleImage()->GetObjectIndexByName(DNCT::GcStringToWchar(l_pObject->ToString()));
					sPuzzleData*l_pPuzzleData = m_pImagePlayerSA->GetPuzzleData()[l_iImageIndex];
					sImageShowInfo	l_sImageShowInfo;
					l_sImageShowInfo.fShowSec = 0.1f;
					l_sImageShowInfo.iIndex = l_iImageIndex;
					l_sImageShowInfo.OffsetPos = l_pPuzzleData->OffsetPos;
					l_sImageShowInfo.Size = l_pPuzzleData->Size;
					if( Mirror_checkBox->Checked )
						l_sImageShowInfo.bMirror = true;
					m_pImageShowInfoState->AddImageShowInfo(l_sImageShowInfo);
				 }
				 m_pImageShowInfoState->ReCalculateTotalPlaySecond();
				 AllPlaySec_numericUpDown->Value = (int)(m_pImageShowInfoState->GetTotalPlaySec()*1000);
			 }
			 else
			 {
				 m_bPreventMuthreadError = true;
				 std::list<int>	l_iErseIndexList;
				 System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
				 l_MyList->AddRange(PlayList_listBox->SelectedItems);
				 for each (System::Object^l_pObject in l_MyList)
					l_iErseIndexList.push_back(PlayList_listBox->Items->IndexOf(l_pObject));
				 for each (System::Object^l_pObject in l_MyList)
				 {
					int	l_iIndex = PlayList_listBox->Items->IndexOf(l_pObject);
					PlayList_listBox->Items->Remove(l_pObject);
					ImagePlaySec_listBox->Items->RemoveAt(l_iIndex);
				 }
				 //m_pImageShowInfoState->m_ImageShowInfoVector
				 EraseVectorMemberByList<sImageShowInfo>(&m_pImageShowInfoState->m_ImageShowInfoVector,&l_iErseIndexList);
				 m_bPreventMuthreadError = false;
				 m_pImageShowInfoState->ReCalculateTotalPlaySecond();
				 AllPlaySec_numericUpDown->Value = (int)(m_pImageShowInfoState->GetTotalPlaySec()*1000);
				 if( m_pImageShowInfoState->m_ImageShowInfoVector.size() == 0 )
					 m_pImageShowInfoState->SetCurrentPlayImageIndex(-1);
			 }
		 }

private: System::Void AllPlaySec_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(m_bPreventValueChanged)
				 return;
			 int	l_iNum  = ImagePlaySec_listBox->Items->Count;
			 float	l_fSecond = (float)AllPlaySec_numericUpDown->Value/l_iNum/1000.f;
			 if(l_fSecond-(int)l_fSecond>abs(0.00001f))
				 arningForSecond_label->Text = ("count not even,it's not  precise");
			 else
				 arningForSecond_label->Text = "";
			 for(int i=0;i<l_iNum;++i)
			 {
				 ImagePlaySec_listBox->Items[i] = l_fSecond.ToString();
				 m_pImageShowInfoState->m_ImageShowInfoVector[i].fShowSec = l_fSecond;
			 }
		 }

private: System::Void PlayCount_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 m_pImageShowInfoState->SetPlayCount((int)PlayCount_numericUpDown->Value);
			 m_pImageShowInfoState->ReCalculateTotalPlaySecond();
		 }

private: System::Void EnableLoop_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(EnableLoop_checkBox->Checked)
			 {
				 m_pImageShowInfoState->SetPlayCount(-1);
				 PlayCount_numericUpDown->Value = -1;
			 }
			 else
			 {
				if(  PlayCount_numericUpDown->Value == -1 )
				{
					m_pImageShowInfoState->SetPlayCount(1);
					PlayCount_numericUpDown->Value = 1;
				}
				else
					m_pImageShowInfoState->SetPlayCount((int)PlayCount_numericUpDown->Value);
			 }
		 }

private: System::Void PlayImageanimation_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( m_pImageShowInfoState->m_ImageShowInfoVector.size() )
			 {
				PlayList_listBox->SelectedIndex = -1;
				if( m_pImageShowInfoState->GetCurrentPlayImageIndex()!=-1 )
					m_pImageShowInfoState->SetCurrentPlayImageIndex(-1);
				else
					m_pImageShowInfoState->Start(0);
			 }
		 }

private: System::Void ImageMoveUp_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 int	l_iSelectIndex = PlayList_listBox->SelectedIndex;
			 if( l_iSelectIndex == -1 )
				return;
			 if( sender == ImageMoveown_button )
			 {
				 //it's bottom
				if( l_iSelectIndex == PlayList_listBox->Items->Count-1 )
					 return;
				sImageShowInfo l_sImageShowInfo = m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex];
				sImageShowInfo l_sImageShowInfo2 = m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex+1];
				m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex+1] = l_sImageShowInfo;
				m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex] = l_sImageShowInfo2;
				DNCT::SwapListBoxValue(l_iSelectIndex,l_iSelectIndex+1,PlayList_listBox);
				DNCT::SwapListBoxValue(l_iSelectIndex,l_iSelectIndex+1,ImagePlaySec_listBox);
				PlayList_listBox->SelectedIndex = -1;
				PlayList_listBox->SelectedIndex = l_iSelectIndex+1;
			 }
			 else
			 {
				 //it's top
				 if( l_iSelectIndex == 0 )
					 return;
				sImageShowInfo l_sImageShowInfo = m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex];
				sImageShowInfo l_sImageShowInfo2 = m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex-1];
				m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex-1] = l_sImageShowInfo;
				m_pImageShowInfoState->m_ImageShowInfoVector[l_iSelectIndex] = l_sImageShowInfo2;
				DNCT::SwapListBoxValue(l_iSelectIndex,l_iSelectIndex-1,PlayList_listBox);
				DNCT::SwapListBoxValue(l_iSelectIndex,l_iSelectIndex-1,ImagePlaySec_listBox);
				PlayList_listBox->SelectedIndex = -1;
				PlayList_listBox->SelectedIndex = l_iSelectIndex-1;
			 }
		 }

private: System::Void PlayList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( PlayList_listBox->SelectedItems->Count == 1 )
			 {
				 m_bPreventValueChanged = true;
				 sImageShowInfo	l_sImageShowInfo = m_pImageShowInfoState->m_ImageShowInfoVector[PlayList_listBox->SelectedIndex];
				 OffsetPosX_numericUpDown->Value = l_sImageShowInfo.OffsetPos.x;
				 OffsetPosY_numericUpDown->Value = l_sImageShowInfo.OffsetPos.y;
				 Angle_numericUpDown->Value = (int)(l_sImageShowInfo.fShowSec*1000);
				 Width_numericUpDown->Value = l_sImageShowInfo.Size.x;
				 Height_numericUpDown->Value = l_sImageShowInfo.Size.y;
				 if( l_sImageShowInfo.bMirror )
					 Mirror_checkBox->Checked = true;
				 ImagePlaySec_listBox->SelectedIndices->Clear();
				 for each(int l_iIndex in PlayList_listBox->SelectedIndices)
					ImagePlaySec_listBox->SelectedIndices->Add(l_iIndex);
			 }
			 m_bPreventValueChanged = false;
			 m_bDragObject = false;
		 }

private: System::Void perImagePlaySec_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( PlayList_listBox->SelectedIndex == -1 )
				 return;
			 for each(int l_iIndex in PlayList_listBox->SelectedIndices)
			 {
				 float	l_fSec = (float)perImagePlaySec_numericUpDown->Value/1000.f;
				 m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].fShowSec = l_fSec;
				 ImagePlaySec_listBox->Items[l_iIndex] = m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].fShowSec;
			 }
		 }

private: System::Void OffsetPosX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( m_bPreventValueChanged )
				 return;
			 for each(int l_iIndex in PlayList_listBox->SelectedIndices)
			 {
				 if( sender == Angle_numericUpDown )
					m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].fAngle = (float)Angle_numericUpDown->Value;
				 else
				 if( sender == Width_numericUpDown )
				 {
					m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].Size.x = (int)Width_numericUpDown->Value;
				 }
				 else
				 if( sender == Height_numericUpDown )
				 {
					m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].Size.y = (int)Height_numericUpDown->Value;
				 }
				 else
				 {
					 POINT	l_OffsetPos = m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].OffsetPos;
					 if( OffsetPosX_numericUpDown == sender )
						 l_OffsetPos.x = (int)OffsetPosX_numericUpDown->Value;
					 if( OffsetPosY_numericUpDown == sender )
						 l_OffsetPos.y = (int)OffsetPosY_numericUpDown->Value;
					 m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex].OffsetPos = l_OffsetPos;				 
				 }
			 }
		 }

private: System::Void BKImage_button_Click(System::Object^  sender, System::EventArgs^  e) {
			String^l_FileName = DNCT::OpenFileAndGetName("(*.png)|*.png");
			if( l_FileName )
			{
				SAFE_DELETE(m_pBKImageSA);
				m_pBKImageSA = new cBaseImage(DNCT::GcStringToChar(l_FileName));
			}
		 }

private: System::Void AddAnimationToList_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == DeleteListObject_button )
			 {
				 if( WholeAnimationList_listBox->SelectedIndex == -1)
					 return;
				 m_pImagePlayerSA->RemoveObject(WholeAnimationList_listBox->SelectedIndex);
				 WholeAnimationList_listBox->Items->RemoveAt(WholeAnimationList_listBox->SelectedIndex);
			 }
			 else
			 {
				 if(!AnimationName_textBox->Text->Length)
					 return;
				 if(!DNCT::CheckListContainStringAndAdd(WholeAnimationList_listBox,AnimationName_textBox->Text))
				 {
					 m_pImageShowInfoState->SetName(DNCT::GcStringToWchar(AnimationName_textBox->Text));
					 m_pImageShowInfoState->SetRetain(Retain_checkBox->Checked);
					 bool	l_b = m_pImagePlayerSA->AddObject(new cStaticAnimation(m_pImageShowInfoState));
					 assert(l_b&&"such name exited!!");
					 m_pImageShowInfoState->m_ImageShowInfoVector.clear();
					 m_pImageShowInfoState->SetRestPlaySec(0.f);
					 ImagePlaySec_listBox->Items->Clear();
					 PlayList_listBox->Items->Clear();
					 PlayCount_numericUpDown->Value = 1;
				 }
				 else
				 {
					 WARING_YES_NO_TO_YES("such object name already exists,would u like to replace?"+DNCT::GetChanglineString()+
						 "該物件名稱已存在,要覆蓋嗎?")
					 {
						 m_pImageShowInfoState->SetName(DNCT::GcStringToWchar(AnimationName_textBox->Text));
						 m_pImagePlayerSA->RemoveObject(DNCT::GcStringToWchar(AnimationName_textBox->Text));
						 m_pImagePlayerSA->AddObject(new cStaticAnimation(m_pImageShowInfoState));
						 WholeAnimationList_listBox->Items->Remove(AnimationName_textBox->Text);
						 WholeAnimationList_listBox->Items->Add(AnimationName_textBox->Text);
						 PlayCount_numericUpDown->Value = 1;
						 m_pImageShowInfoState->m_ImageShowInfoVector.clear();
						 ImagePlaySec_listBox->Items->Clear();
						 PlayList_listBox->Items->Clear();
					 }				 
				 }
			 }
		 }
private: System::Void AnimationInvert_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( WholeAnimationList_listBox->SelectedIndex == -1 )
				 return;
			 AnimationName_textBox->Text = WholeAnimationList_listBox->SelectedItem->ToString();
			 ImagePlaySec_listBox->Items->Clear();
			 PlayList_listBox->Items->Clear();
			 //here should be safe delete and new again
			 m_pImageShowInfoState->m_ImageShowInfoVector = m_pImagePlayerSA->GetObject(WholeAnimationList_listBox->SelectedIndex)->m_ImageShowInfoVector;
			 m_pImageShowInfoState->SetPlayCount(m_pImagePlayerSA->GetObject(WholeAnimationList_listBox->SelectedIndex)->GetPlayCount());
			 sPuzzleData**l_ppPuzzleData = m_pImagePlayerSA->GetPuzzleData();
			 cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>((*m_pImagePlayManager->GetAllBaseImageList())[0]);
			 for( UINT i=0;i<m_pImageShowInfoState->m_ImageShowInfoVector.size();++i )
			 {
				 sImageShowInfo*l_pImageShowInfo = &m_pImageShowInfoState->m_ImageShowInfoVector[i];
				 PlayList_listBox->Items->Add(DNCT::WcharToGcstring(l_ppPuzzleData[l_pImageShowInfo->iIndex]->strFileName));
				 ImagePlaySec_listBox->Items->Add(l_pImageShowInfo->fShowSec.ToString());
			 }
			 m_pImageShowInfoState->ReCalculateTotalPlaySecond();
			 m_bPreventValueChanged = true;
			 AllPlaySec_numericUpDown->Value = (int)(m_pImageShowInfoState->GetTotalPlaySec()*1000);
			 PlayCount_numericUpDown->Value = m_pImageShowInfoState->GetPlayCount();
			 m_bPreventValueChanged = false;
			 //haha....fuck
			 Retain_checkBox->Checked = m_pImagePlayerSA->GetObject(WholeAnimationList_listBox->SelectedIndex)->IsRetain();
			 EnableLoop_checkBox->Checked = m_pImagePlayerSA->GetObject(WholeAnimationList_listBox->SelectedIndex)->GetPlayCount()==-1?true:false;
		 }
private: System::Void ComposeImageData_button_Click(System::Object^  sender, System::EventArgs^  e) {
			SaveImagePlayerData();
		 }
private: System::Void OpenImageData_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 String^l_strFileName = DNCT::OpenFileAndGetName("(*.sa)|*.sa");
			 if( l_strFileName )
			 {
				SAFE_DELETE(m_pImagePlayManager);
				SAFE_DELETE(m_pImageShowInfoState);
				m_pImageShowInfoState = new cStaticAnimation();
				m_pImagePlayManager = new cAnimationParser();
				bool	l_b = m_pImagePlayManager->Parse( DNCT::GcStringToChar(l_strFileName) );
				if( l_b )
				{
					m_pImagePlayerSA = dynamic_cast<cSAList*>((*m_pImagePlayManager)[0]);
					m_strCurrentFileName = DNCT::WcharToGcstring(m_pImagePlayerSA->GetName());
					WholeImageInfo_listBox->Items->Clear();
					WholeAnimationList_listBox->Items->Clear();
					PlayList_listBox->Items->Clear();
					ImagePlaySec_listBox->Items->Clear();
					m_pImageShowInfoState->SetPuzzleImage(m_pImagePlayerSA->GetPuzzleImage());
					m_pImageShowInfoState->SetPuzzleData(m_pImagePlayerSA->GetPuzzleData());
					for(int i=0;i<m_pImagePlayerSA->Count();++i)
					{
						WholeAnimationList_listBox->Items->Add( DNCT::WcharToGcstring(m_pImagePlayerSA->GetObject(i)->GetName()) );
					}
					for(int i=0;i<m_pImagePlayerSA->GetNumImage();++i)
						WholeImageInfo_listBox->Items->Add(DNCT::WcharToGcstring(m_pImagePlayerSA->GetImageName(i)));
				}
			 }
		 }
private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
				if(!timer1->Enabled)
					return;
				splitContainer3->Panel1->Focus();
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
				if( !m_bDragObject )
				{
					if( PlayList_listBox->SelectedItems->Count == 1 )
					{
						POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
						sImageShowInfo	l_sImageShowInfo = m_pImageShowInfoState->m_ImageShowInfoVector[PlayList_listBox->SelectedIndex];
						if( ptCursor.x>=l_sImageShowInfo.OffsetPos.x&&ptCursor.x<=l_sImageShowInfo.OffsetPos.x+l_sImageShowInfo.Size.x )
						{
							if( ptCursor.y>=l_sImageShowInfo.OffsetPos.y&&ptCursor.y<=l_sImageShowInfo.OffsetPos.y+l_sImageShowInfo.Size.y )
							{
								m_bDragObject = true;
							}
						}
						else
							m_bDragObject = false;
					}
				}
				else
					m_bDragObject = false;
		 }
private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
				if(!timer1->Enabled)
					return;
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
		 }
private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
				if(!timer1->Enabled)
					return;
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer3->Panel1->Size.Width,(float)splitContainer3->Panel1->Size.Height));
				if( m_bDragObject )
				{
					if( PlayList_listBox->SelectedItems->Count == 1 )
					{
						POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
						sImageShowInfo	*l_pImageShowInfo = &m_pImageShowInfoState->m_ImageShowInfoVector[PlayList_listBox->SelectedIndex];					
						l_pImageShowInfo->OffsetPos.x = ptCursor.x;
						l_pImageShowInfo->OffsetPos.y = ptCursor.y;
					}
				}
		 }
private: System::Void Mirror_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 for each( int l_iIndex in PlayList_listBox->SelectedIndices )
			 {
				sImageShowInfo	*l_pImageShowInfo = &m_pImageShowInfoState->m_ImageShowInfoVector[l_iIndex];
				l_pImageShowInfo->bMirror = Mirror_checkBox->Checked;
			 }
		 }
private: System::Void WholeAnimationList_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e)
		 {
			 AnimationInvert_button_Click(sender,e);
		 }
private: System::Void WholeAnimationList_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if(e->KeyCode == Keys::Enter)
			 {
				System::EventArgs^  e2;
				AnimationInvert_button_Click(AnimationInvert_button,e2);
			 }
		 }
};
}