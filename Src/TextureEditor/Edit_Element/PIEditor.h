#pragma once

#include "GlobalDeclartion.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;


namespace FMCEditor 
{
	/// <summary>
	/// cPIEditor 的摘要
	/// </summary>
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	public ref class cPIEditor : public System::Windows::Forms::UserControl
	{
	public:
		cPIEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm)
		{
			m_pTimeAndFPS = new UT::sTimeAndFPS;
			m_pImageIndexOfAnimation = new cImageIndexOfAnimation(true);
			m_pImageIndexOfAnimationList = new cObjectListByName<cImageIndexOfAnimation>;
			m_pImageomposerIRM = new cImageParser(true);
			m_pOrthogonalCamera = new cOrthogonalCamera();
			InitializeComponent();
			this->Dock = GCFORM::DockStyle::Fill;
			//
			//TODO: 在此加入建構函式程式碼
			//
			m_pForm = e_pForm;
			m_pParentHandle = 0;
			if( m_pForm->Parent )
			{
				m_pParentHandle = (HWND)m_pForm->Parent->Handle.ToPointer();
			}
			m_HdcMV = GetDC((HWND)this->splitContainer2->Panel1->Handle.ToPointer());
			m_HGLRCMV = UT::InitOpenGL((HWND)this->splitContainer2->Panel1->Handle.ToPointer(),true,m_HdcMV);
			//m_HdcMV = GetDC((HWND)splitContainer2->Panel1->Handle.ToPointer());
			m_ImageTale = gcnew System::Collections::Hashtable;
			if( e_ptabControl )
			{
				m_pTabPage = gcnew GCFORM::TabPage();
				m_pTabPage->Text = "PuzzleImage/圖檔合併";
				m_pTabPage->AutoScroll = true;
				m_pTabPage->Controls->Add(this);
				e_ptabControl->TabPages->Add(m_pTabPage);
				m_pTabControl = e_ptabControl;
				e_ptabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::tabControl_SelectedIndexChanged);
			}
			else
			{
				m_pForm->Controls->Add(this);
			}
			splitContainer2->Panel1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseMove);
			splitContainer2->Panel1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseDown);
			splitContainer2->Panel1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseUp);
			splitContainer2->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::MyMouseUp);
			splitContainer2->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::MyKeyUp);
			splitContainer2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &cPIEditor::MyKeyPress);
			splitContainer2->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::MyKeyDown);
			//
			LanguageSwitch(this->Controls,"/",this->Handle);
			LanguageSwitch(splitContainer1->Panel1->Controls,"/",this->Handle);
			m_pDebugFont = 0;
			if(System::IO::File::Exists("Font.png"))
			{
				m_pDebugFont = new cGlyphFontRender("Font");
				if( !m_pDebugFont->GetFontImage() )
					SAFE_DELETE(m_pDebugFont);
			}

			CreateShader(g_bCommonVSClientState,L"ICPathShader");
			CreateShader(g_bCommonVSNoTextureClientState,L"IC_NoTextureShader");
			m_bAvoidDataAssignForPIUintChild = false;
			this->timer1->Enabled = true;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~cPIEditor()
		{
			DeleteShader(L"ICPathShader");
			DeleteShader(L"IC_NoTextureShader");
			SAFE_DELETE(m_pDebugFont);
			SAFE_DELETE(m_pImageIndexOfAnimation);
			SAFE_DELETE(m_pImageIndexOfAnimationList);
			SAFE_DELETE(m_pImageomposerIRM);
			SAFE_DELETE(m_pTimeAndFPS);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  TotalPixel_label;
	private: System::Windows::Forms::CheckBox^  AllShowBoundry_checkBox;
	private: System::Windows::Forms::Button^  ImageObjectDown_button;
	private: System::Windows::Forms::Button^  ImageObjectUP_button;
	private: System::Windows::Forms::CheckBox^  ShowBoundary_checkBox;
	private: System::Windows::Forms::TextBox^  ImageDetail_textBox;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::NumericUpDown^  ImageHeight_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImageWidth_numericUpDown;
	private: System::Windows::Forms::Button^  AutoOrderPosition_button;
	private: System::Windows::Forms::Button^  DeleteImage_button;
	private: System::Windows::Forms::Button^  AddImage_button;
	private: System::Windows::Forms::ListBox^  AllImage_listBox;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openXMLToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  invertPuzzleImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  generatePowOf2ImageToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::Button^  AnimationImageDown_button;
	private: System::Windows::Forms::Button^  AnimationImageUp_button;
	private: System::Windows::Forms::Button^  DelAnimationImage_button;
	private: System::Windows::Forms::Button^  AddAnimationImage_button;
	private: System::Windows::Forms::ListBox^  AnimationData_listBox;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ListBox^  AnimationDataList_listBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  AnimationDatDel_button;
	private: System::Windows::Forms::Button^  AnimationDatAdd_button;
	private: System::Windows::Forms::TextBox^  AnimationDataName_textBox;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::ToolStripMenuItem^  generateAllImageWithPowerOfTwoToolStripMenuItem;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  ImagePosX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  ImagePosY_numericUpDown;
    private: System::Windows::Forms::Button^  ProperStripPixel_button;
    private: System::Windows::Forms::Button^  ToOriginalImage_button;
    private: System::Windows::Forms::Button^  StripAlpha_button;
    private: System::Windows::Forms::CheckBox^  DebugLine_checkBox;
    private: System::Windows::Forms::NumericUpDown^  ImageDistanceX_numericUpDown;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::NumericUpDown^  ImageDistanceY_numericUpDown;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitStartX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitEndY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitEndX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  NewPIUnitStartY_numericUpDown;
	private: System::Windows::Forms::Button^  AddNewPIUnitImage_button;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  NewPIUnitName_textBox;
	private: System::Windows::Forms::CheckBox^  ImageSaveAsDDS_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  dockToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  childToolStripMenuItem;
	//my
	private: GCFORM::Form^	m_pForm;
	public:HWND				m_pParentHandle;
	bool	 m_bAvoidDataAssignForPIUintChild;
	HGLRC	 m_HGLRCMV;
	HDC		 m_HdcMV;
	cGlyphFontRender*m_pDebugFont;
	cImageParser*								m_pImageomposerIRM;
	int											m_iCurrentSelectedObjectIndex;
	cImageIndexOfAnimation*						m_pImageIndexOfAnimation;
	cObjectListByName<cImageIndexOfAnimation>*	m_pImageIndexOfAnimationList;
	cOrthogonalCamera*							m_pOrthogonalCamera;
	UT::sTimeAndFPS*							m_pTimeAndFPS;
	private: GCFORM::TabPage^m_pTabPage;					//for attach from.
	private: GCFORM::TabControl^m_pTabControl;				//to determin is tabpage clicked.
	private: System::Collections::Hashtable^m_ImageTale;	//key:string,value:System::Drawing::Bitmap.,if m_pImageomposerIRM's child(UIImage) has owner,then m_ImageTale do not has its data
	private: System::Void	SavePuzzleFile();
	private: cPuzzleImage*	OpenPuzzleFile(String^e_strFileName);
	public:  void			OpenPIFile(String^e_strFileName);
	private: bool			ParsePuzzleImage(cPuzzleImage*e_pPuzzleImage,String^e_strFileName);
	private: System::Void	GeneratePowOf2Image(bool e_bPowerOfTwo);
	private: System::Void	MouseCollideForPickUpObject( System::Windows::Forms::MouseEventArgs^  e,System::Windows::Forms::Panel^e_pPanel);
	private: System::Void	AssignAnimationData();
	private: String^		GetTotalPixelExisted();	//all UIImage pixel size
			 String^m_strCurrentFileName;
			 cUIImage*	GetNewUIImageByBitMap(System::Drawing::Bitmap^e_pBitMap,WCHAR*e_strName);
			 void		AttachObjectPosAdjust();
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
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->TotalPixel_label = (gcnew System::Windows::Forms::Label());
			this->AllShowBoundry_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageObjectDown_button = (gcnew System::Windows::Forms::Button());
			this->ImageObjectUP_button = (gcnew System::Windows::Forms::Button());
			this->ShowBoundary_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ImageDetail_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->ImageHeight_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ImageWidth_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AutoOrderPosition_button = (gcnew System::Windows::Forms::Button());
			this->DeleteImage_button = (gcnew System::Windows::Forms::Button());
			this->AddImage_button = (gcnew System::Windows::Forms::Button());
			this->AllImage_listBox = (gcnew System::Windows::Forms::ListBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openXMLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->invertPuzzleImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->generatePowOf2ImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->generateAllImageWithPowerOfTwoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->ImageSaveAsDDS_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->NewPIUnitName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->AddNewPIUnitImage_button = (gcnew System::Windows::Forms::Button());
			this->NewPIUnitStartX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->NewPIUnitEndY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->NewPIUnitEndX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->NewPIUnitStartY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->ImageDistanceY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->ImageDistanceX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->DebugLine_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ToOriginalImage_button = (gcnew System::Windows::Forms::Button());
			this->StripAlpha_button = (gcnew System::Windows::Forms::Button());
			this->ProperStripPixel_button = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ImagePosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->ImagePosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AnimationDatDel_button = (gcnew System::Windows::Forms::Button());
			this->AnimationDatAdd_button = (gcnew System::Windows::Forms::Button());
			this->AnimationDataName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->AnimationDataList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AnimationImageDown_button = (gcnew System::Windows::Forms::Button());
			this->AnimationImageUp_button = (gcnew System::Windows::Forms::Button());
			this->DelAnimationImage_button = (gcnew System::Windows::Forms::Button());
			this->AddAnimationImage_button = (gcnew System::Windows::Forms::Button());
			this->AnimationData_listBox = (gcnew System::Windows::Forms::ListBox());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->dockToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->floatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->childToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageHeight_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageWidth_numericUpDown))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitStartX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitEndY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitEndX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitStartY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageDistanceY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageDistanceX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImagePosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImagePosY_numericUpDown))->BeginInit();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &cPIEditor::timer1_Tick);
			// 
			// TotalPixel_label
			// 
			this->TotalPixel_label->AutoSize = true;
			this->TotalPixel_label->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->TotalPixel_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->TotalPixel_label->Location = System::Drawing::Point(381, 161);
			this->TotalPixel_label->Name = L"TotalPixel_label";
			this->TotalPixel_label->Size = System::Drawing::Size(91, 12);
			this->TotalPixel_label->TabIndex = 46;
			this->TotalPixel_label->Text = L"TotalPixel:640000";
			// 
			// AllShowBoundry_checkBox
			// 
			this->AllShowBoundry_checkBox->AutoSize = true;
			this->AllShowBoundry_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AllShowBoundry_checkBox->Checked = true;
			this->AllShowBoundry_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->AllShowBoundry_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllShowBoundry_checkBox->Location = System::Drawing::Point(193, 158);
			this->AllShowBoundry_checkBox->Name = L"AllShowBoundry_checkBox";
			this->AllShowBoundry_checkBox->Size = System::Drawing::Size(198, 16);
			this->AllShowBoundry_checkBox->TabIndex = 45;
			this->AllShowBoundry_checkBox->Text = L"AllShowBoundary/顯示所有圖片框";
			this->AllShowBoundry_checkBox->UseVisualStyleBackColor = false;
			// 
			// ImageObjectDown_button
			// 
			this->ImageObjectDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageObjectDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageObjectDown_button->Location = System::Drawing::Point(287, 93);
			this->ImageObjectDown_button->Name = L"ImageObjectDown_button";
			this->ImageObjectDown_button->Size = System::Drawing::Size(21, 41);
			this->ImageObjectDown_button->TabIndex = 42;
			this->ImageObjectDown_button->Text = L"v\r\nv";
			this->ImageObjectDown_button->UseVisualStyleBackColor = false;
			this->ImageObjectDown_button->Click += gcnew System::EventHandler(this, &cPIEditor::button2_Click);
			// 
			// ImageObjectUP_button
			// 
			this->ImageObjectUP_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageObjectUP_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageObjectUP_button->Location = System::Drawing::Point(287, 17);
			this->ImageObjectUP_button->Name = L"ImageObjectUP_button";
			this->ImageObjectUP_button->Size = System::Drawing::Size(21, 41);
			this->ImageObjectUP_button->TabIndex = 41;
			this->ImageObjectUP_button->Text = L"^\r\n^";
			this->ImageObjectUP_button->UseVisualStyleBackColor = false;
			this->ImageObjectUP_button->Click += gcnew System::EventHandler(this, &cPIEditor::button2_Click);
			// 
			// ShowBoundary_checkBox
			// 
			this->ShowBoundary_checkBox->AutoSize = true;
			this->ShowBoundary_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ShowBoundary_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ShowBoundary_checkBox->Location = System::Drawing::Point(3, 158);
			this->ShowBoundary_checkBox->Name = L"ShowBoundary_checkBox";
			this->ShowBoundary_checkBox->Size = System::Drawing::Size(184, 16);
			this->ShowBoundary_checkBox->TabIndex = 40;
			this->ShowBoundary_checkBox->Text = L"ShowBoundary/顯示目前圖片框";
			this->ShowBoundary_checkBox->UseVisualStyleBackColor = false;
			// 
			// ImageDetail_textBox
			// 
			this->ImageDetail_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageDetail_textBox->Enabled = false;
			this->ImageDetail_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->ImageDetail_textBox->Location = System::Drawing::Point(601, 52);
			this->ImageDetail_textBox->MinimumSize = System::Drawing::Size(4, 29);
			this->ImageDetail_textBox->Multiline = true;
			this->ImageDetail_textBox->Name = L"ImageDetail_textBox";
			this->ImageDetail_textBox->Size = System::Drawing::Size(140, 78);
			this->ImageDetail_textBox->TabIndex = 39;
			this->ImageDetail_textBox->Text = L"79979";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label4->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label4->Location = System::Drawing::Point(743, 53);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(99, 12);
			this->label4->TabIndex = 38;
			this->label4->Text = L"Height/產生圖片高";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label5->Location = System::Drawing::Point(746, 5);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(97, 12);
			this->label5->TabIndex = 37;
			this->label5->Text = L"Width/產生圖片寬";
			// 
			// ImageHeight_numericUpDown
			// 
			this->ImageHeight_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageHeight_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageHeight_numericUpDown->Location = System::Drawing::Point(746, 67);
			this->ImageHeight_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->ImageHeight_numericUpDown->Name = L"ImageHeight_numericUpDown";
			this->ImageHeight_numericUpDown->Size = System::Drawing::Size(62, 22);
			this->ImageHeight_numericUpDown->TabIndex = 36;
			this->ImageHeight_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {512, 0, 0, 0});
			this->ImageHeight_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImageWidth_numericUpDown_ValueChanged);
			// 
			// ImageWidth_numericUpDown
			// 
			this->ImageWidth_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImageWidth_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImageWidth_numericUpDown->Location = System::Drawing::Point(746, 23);
			this->ImageWidth_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->ImageWidth_numericUpDown->Name = L"ImageWidth_numericUpDown";
			this->ImageWidth_numericUpDown->Size = System::Drawing::Size(62, 22);
			this->ImageWidth_numericUpDown->TabIndex = 35;
			this->ImageWidth_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->ImageWidth_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImageWidth_numericUpDown_ValueChanged);
			// 
			// AutoOrderPosition_button
			// 
			this->AutoOrderPosition_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AutoOrderPosition_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AutoOrderPosition_button->Location = System::Drawing::Point(601, 4);
			this->AutoOrderPosition_button->Name = L"AutoOrderPosition_button";
			this->AutoOrderPosition_button->Size = System::Drawing::Size(139, 36);
			this->AutoOrderPosition_button->TabIndex = 34;
			this->AutoOrderPosition_button->Text = L"ImagePositionAutoAssign/圖片自動校正位置";
			this->AutoOrderPosition_button->UseVisualStyleBackColor = false;
			this->AutoOrderPosition_button->Click += gcnew System::EventHandler(this, &cPIEditor::AutoOrderPosition_button_Click);
			// 
			// DeleteImage_button
			// 
			this->DeleteImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeleteImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteImage_button->Location = System::Drawing::Point(3, 37);
			this->DeleteImage_button->Name = L"DeleteImage_button";
			this->DeleteImage_button->Size = System::Drawing::Size(100, 25);
			this->DeleteImage_button->TabIndex = 29;
			this->DeleteImage_button->Text = L"DeleteImage/刪除圖片";
			this->DeleteImage_button->UseVisualStyleBackColor = false;
			this->DeleteImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::DeleteImage_button_Click);
			// 
			// AddImage_button
			// 
			this->AddImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddImage_button->Location = System::Drawing::Point(3, 4);
			this->AddImage_button->Name = L"AddImage_button";
			this->AddImage_button->Size = System::Drawing::Size(99, 28);
			this->AddImage_button->TabIndex = 28;
			this->AddImage_button->Text = L"AddImage/增加圖片";
			this->AddImage_button->UseVisualStyleBackColor = false;
			this->AddImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::DeleteImage_button_Click);
			// 
			// AllImage_listBox
			// 
			this->AllImage_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AllImage_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllImage_listBox->FormattingEnabled = true;
			this->AllImage_listBox->ItemHeight = 12;
			this->AllImage_listBox->Location = System::Drawing::Point(107, 4);
			this->AllImage_listBox->MinimumSize = System::Drawing::Size(4, 81);
			this->AllImage_listBox->Name = L"AllImage_listBox";
			this->AllImage_listBox->ScrollAlwaysVisible = true;
			this->AllImage_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllImage_listBox->Size = System::Drawing::Size(174, 148);
			this->AllImage_listBox->TabIndex = 27;
			this->AllImage_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::AllImage_listBox_MouseDoubleClick);
			this->AllImage_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &cPIEditor::AllImage_listBox_SelectedIndexChanged);
			this->AllImage_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::AllImage_listBox_KeyUp_1);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->dockToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1280, 24);
			this->menuStrip1->TabIndex = 26;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->openXMLToolStripMenuItem, 
				this->saveToolStripMenuItem, this->invertPuzzleImageToolStripMenuItem, this->generatePowOf2ImageToolStripMenuItem, this->generateAllImageWithPowerOfTwoToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openXMLToolStripMenuItem
			// 
			this->openXMLToolStripMenuItem->Name = L"openXMLToolStripMenuItem";
			this->openXMLToolStripMenuItem->Size = System::Drawing::Size(283, 22);
			this->openXMLToolStripMenuItem->Text = L"Open";
			this->openXMLToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::openXMLToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(283, 22);
			this->saveToolStripMenuItem->Text = L"GeneratePuzzleImage/產生單張圖片";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::Test_button_Click);
			// 
			// invertPuzzleImageToolStripMenuItem
			// 
			this->invertPuzzleImageToolStripMenuItem->Name = L"invertPuzzleImageToolStripMenuItem";
			this->invertPuzzleImageToolStripMenuItem->Size = System::Drawing::Size(283, 22);
			this->invertPuzzleImageToolStripMenuItem->Text = L"InvertPuzzleImage/將資料反轉成張圖片";
			this->invertPuzzleImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::InvertPuzzleImage_button_Click);
			// 
			// generatePowOf2ImageToolStripMenuItem
			// 
			this->generatePowOf2ImageToolStripMenuItem->Name = L"generatePowOf2ImageToolStripMenuItem";
			this->generatePowOf2ImageToolStripMenuItem->Size = System::Drawing::Size(283, 22);
			this->generatePowOf2ImageToolStripMenuItem->Text = L"Generate All Image/產生所有圖片";
			this->generatePowOf2ImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click);
			// 
			// generateAllImageWithPowerOfTwoToolStripMenuItem
			// 
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Name = L"generateAllImageWithPowerOfTwoToolStripMenuItem";
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Size = System::Drawing::Size(283, 22);
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Text = L"Generate All Image With PowerOfTwo";
			this->generateAllImageWithPowerOfTwoToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::button1_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 24);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->splitContainer1->Panel1->Controls->Add(this->ImageSaveAsDDS_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label12);
			this->splitContainer1->Panel1->Controls->Add(this->NewPIUnitName_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->AddNewPIUnitImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->NewPIUnitStartX_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->NewPIUnitEndY_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->NewPIUnitEndX_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->NewPIUnitStartY_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->label11);
			this->splitContainer1->Panel1->Controls->Add(this->label10);
			this->splitContainer1->Panel1->Controls->Add(this->label9);
			this->splitContainer1->Panel1->Controls->Add(this->label8);
			this->splitContainer1->Panel1->Controls->Add(this->label7);
			this->splitContainer1->Panel1->Controls->Add(this->ImageDistanceY_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->label6);
			this->splitContainer1->Panel1->Controls->Add(this->ImageDistanceX_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->DebugLine_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->ToOriginalImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->StripAlpha_button);
			this->splitContainer1->Panel1->Controls->Add(this->ProperStripPixel_button);
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->ImagePosX_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->ImagePosY_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationDatDel_button);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationDatAdd_button);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationDataName_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationDataList_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationImageDown_button);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationImageUp_button);
			this->splitContainer1->Panel1->Controls->Add(this->DelAnimationImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->AddAnimationImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->AnimationData_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->AllImage_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->TotalPixel_label);
			this->splitContainer1->Panel1->Controls->Add(this->AddImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->AllShowBoundry_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->DeleteImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->ImageObjectDown_button);
			this->splitContainer1->Panel1->Controls->Add(this->AutoOrderPosition_button);
			this->splitContainer1->Panel1->Controls->Add(this->ImageObjectUP_button);
			this->splitContainer1->Panel1->Controls->Add(this->ImageWidth_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->ShowBoundary_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->ImageHeight_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->ImageDetail_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->label5);
			this->splitContainer1->Panel1->Controls->Add(this->label4);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(1280, 1000);
			this->splitContainer1->SplitterDistance = 250;
			this->splitContainer1->SplitterWidth = 3;
			this->splitContainer1->TabIndex = 49;
			// 
			// ImageSaveAsDDS_checkBox
			// 
			this->ImageSaveAsDDS_checkBox->AutoSize = true;
			this->ImageSaveAsDDS_checkBox->Location = System::Drawing::Point(550, 158);
			this->ImageSaveAsDDS_checkBox->Name = L"ImageSaveAsDDS_checkBox";
			this->ImageSaveAsDDS_checkBox->Size = System::Drawing::Size(109, 16);
			this->ImageSaveAsDDS_checkBox->TabIndex = 83;
			this->ImageSaveAsDDS_checkBox->Text = L"ImageSaveAsDDS";
			this->ImageSaveAsDDS_checkBox->UseVisualStyleBackColor = true;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label12->Location = System::Drawing::Point(404, 86);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(170, 12);
			this->label12->TabIndex = 82;
			this->label12->Text = L"NewPIUnitName/新圖片單元名稱";
			// 
			// NewPIUnitName_textBox
			// 
			this->NewPIUnitName_textBox->Location = System::Drawing::Point(406, 101);
			this->NewPIUnitName_textBox->Name = L"NewPIUnitName_textBox";
			this->NewPIUnitName_textBox->Size = System::Drawing::Size(97, 22);
			this->NewPIUnitName_textBox->TabIndex = 81;
			// 
			// AddNewPIUnitImage_button
			// 
			this->AddNewPIUnitImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddNewPIUnitImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddNewPIUnitImage_button->Location = System::Drawing::Point(406, 123);
			this->AddNewPIUnitImage_button->Name = L"AddNewPIUnitImage_button";
			this->AddNewPIUnitImage_button->Size = System::Drawing::Size(99, 35);
			this->AddNewPIUnitImage_button->TabIndex = 80;
			this->AddNewPIUnitImage_button->Text = L"AddNewPIUnitImage/增加新拼圖單元";
			this->AddNewPIUnitImage_button->UseVisualStyleBackColor = false;
			this->AddNewPIUnitImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddNewPIUnitImage_button_Click);
			// 
			// NewPIUnitStartX_numericUpDown
			// 
			this->NewPIUnitStartX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitStartX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitStartX_numericUpDown->Location = System::Drawing::Point(316, 17);
			this->NewPIUnitStartX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->NewPIUnitStartX_numericUpDown->Name = L"NewPIUnitStartX_numericUpDown";
			this->NewPIUnitStartX_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitStartX_numericUpDown->TabIndex = 79;
			this->NewPIUnitStartX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitEndY_numericUpDown
			// 
			this->NewPIUnitEndY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitEndY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitEndY_numericUpDown->Location = System::Drawing::Point(314, 125);
			this->NewPIUnitEndY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->NewPIUnitEndY_numericUpDown->Name = L"NewPIUnitEndY_numericUpDown";
			this->NewPIUnitEndY_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitEndY_numericUpDown->TabIndex = 78;
			this->NewPIUnitEndY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitEndX_numericUpDown
			// 
			this->NewPIUnitEndX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitEndX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitEndX_numericUpDown->Location = System::Drawing::Point(316, 86);
			this->NewPIUnitEndX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->NewPIUnitEndX_numericUpDown->Name = L"NewPIUnitEndX_numericUpDown";
			this->NewPIUnitEndX_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitEndX_numericUpDown->TabIndex = 77;
			this->NewPIUnitEndX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// NewPIUnitStartY_numericUpDown
			// 
			this->NewPIUnitStartY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->NewPIUnitStartY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->NewPIUnitStartY_numericUpDown->Location = System::Drawing::Point(316, 50);
			this->NewPIUnitStartY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->NewPIUnitStartY_numericUpDown->Name = L"NewPIUnitStartY_numericUpDown";
			this->NewPIUnitStartY_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->NewPIUnitStartY_numericUpDown->TabIndex = 76;
			this->NewPIUnitStartY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label11->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label11->Location = System::Drawing::Point(314, 111);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(202, 12);
			this->label11->TabIndex = 75;
			this->label11->Text = L"NewPIUnitEndY/新拼圖單元結束位置Y";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label10->Location = System::Drawing::Point(314, 75);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(202, 12);
			this->label10->TabIndex = 74;
			this->label10->Text = L"NewPIUnitEndX/新拼圖單元結束位置X";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label9->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label9->Location = System::Drawing::Point(314, 38);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(204, 12);
			this->label9->TabIndex = 73;
			this->label9->Text = L"NewPIUnitStartY/新拼圖單元開始位置Y";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label8->Location = System::Drawing::Point(314, 3);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(204, 12);
			this->label8->TabIndex = 72;
			this->label8->Text = L"NewPIUnitStartX/新拼圖單元開始位置X";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label7->Location = System::Drawing::Point(509, 118);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(140, 12);
			this->label7->TabIndex = 71;
			this->label7->Text = L"ImageDistanceY/圖片間距Y";
			// 
			// ImageDistanceY_numericUpDown
			// 
			this->ImageDistanceY_numericUpDown->Location = System::Drawing::Point(531, 133);
			this->ImageDistanceY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->ImageDistanceY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->ImageDistanceY_numericUpDown->Name = L"ImageDistanceY_numericUpDown";
			this->ImageDistanceY_numericUpDown->Size = System::Drawing::Size(61, 22);
			this->ImageDistanceY_numericUpDown->TabIndex = 70;
			this->ImageDistanceY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label6->Location = System::Drawing::Point(507, 76);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(140, 12);
			this->label6->TabIndex = 69;
			this->label6->Text = L"ImageDistanceX/圖片間距X";
			// 
			// ImageDistanceX_numericUpDown
			// 
			this->ImageDistanceX_numericUpDown->Location = System::Drawing::Point(531, 90);
			this->ImageDistanceX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->ImageDistanceX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->ImageDistanceX_numericUpDown->Name = L"ImageDistanceX_numericUpDown";
			this->ImageDistanceX_numericUpDown->Size = System::Drawing::Size(61, 22);
			this->ImageDistanceX_numericUpDown->TabIndex = 68;
			this->ImageDistanceX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// DebugLine_checkBox
			// 
			this->DebugLine_checkBox->AutoSize = true;
			this->DebugLine_checkBox->Location = System::Drawing::Point(478, 157);
			this->DebugLine_checkBox->Name = L"DebugLine_checkBox";
			this->DebugLine_checkBox->Size = System::Drawing::Size(76, 16);
			this->DebugLine_checkBox->TabIndex = 67;
			this->DebugLine_checkBox->Text = L"DebugLine";
			this->DebugLine_checkBox->UseVisualStyleBackColor = true;
			// 
			// ToOriginalImage_button
			// 
			this->ToOriginalImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ToOriginalImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ToOriginalImage_button->Location = System::Drawing::Point(3, 101);
			this->ToOriginalImage_button->Name = L"ToOriginalImage_button";
			this->ToOriginalImage_button->Size = System::Drawing::Size(100, 25);
			this->ToOriginalImage_button->TabIndex = 66;
			this->ToOriginalImage_button->Text = L"ToOriginalImag/回復原狀";
			this->ToOriginalImage_button->UseVisualStyleBackColor = false;
			this->ToOriginalImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::StripAlpha_button_Click);
			// 
			// StripAlpha_button
			// 
			this->StripAlpha_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->StripAlpha_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->StripAlpha_button->Location = System::Drawing::Point(3, 73);
			this->StripAlpha_button->Name = L"StripAlpha_button";
			this->StripAlpha_button->Size = System::Drawing::Size(100, 25);
			this->StripAlpha_button->TabIndex = 65;
			this->StripAlpha_button->Text = L"StripAlpha/砍掉alpha";
			this->StripAlpha_button->UseVisualStyleBackColor = false;
			this->StripAlpha_button->Click += gcnew System::EventHandler(this, &cPIEditor::StripAlpha_button_Click);
			// 
			// ProperStripPixel_button
			// 
			this->ProperStripPixel_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ProperStripPixel_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ProperStripPixel_button->Location = System::Drawing::Point(3, 129);
			this->ProperStripPixel_button->Name = L"ProperStripPixel_button";
			this->ProperStripPixel_button->Size = System::Drawing::Size(100, 25);
			this->ProperStripPixel_button->TabIndex = 64;
			this->ProperStripPixel_button->Text = L"ProperStripPixel/找出最大可用圖片";
			this->ProperStripPixel_button->UseVisualStyleBackColor = false;
			this->ProperStripPixel_button->Click += gcnew System::EventHandler(this, &cPIEditor::ProperStripPixel_button_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(507, 8);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(24, 12);
			this->label3->TabIndex = 62;
			this->label3->Text = L"Pos:";
			// 
			// ImagePosX_numericUpDown
			// 
			this->ImagePosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImagePosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImagePosX_numericUpDown->Location = System::Drawing::Point(509, 23);
			this->ImagePosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->ImagePosX_numericUpDown->Name = L"ImagePosX_numericUpDown";
			this->ImagePosX_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->ImagePosX_numericUpDown->TabIndex = 60;
			this->ImagePosX_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1024, 0, 0, 0});
			this->ImagePosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImagePosX_numericUpDown_ValueChanged);
			// 
			// ImagePosY_numericUpDown
			// 
			this->ImagePosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ImagePosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ImagePosY_numericUpDown->Location = System::Drawing::Point(509, 51);
			this->ImagePosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000, 0, 0, 0});
			this->ImagePosY_numericUpDown->Name = L"ImagePosY_numericUpDown";
			this->ImagePosY_numericUpDown->Size = System::Drawing::Size(85, 22);
			this->ImagePosY_numericUpDown->TabIndex = 61;
			this->ImagePosY_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {512, 0, 0, 0});
			this->ImagePosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &cPIEditor::ImagePosX_numericUpDown_ValueChanged);
			// 
			// AnimationDatDel_button
			// 
			this->AnimationDatDel_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationDatDel_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDatDel_button->Location = System::Drawing::Point(1041, 108);
			this->AnimationDatDel_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AnimationDatDel_button->Name = L"AnimationDatDel_button";
			this->AnimationDatDel_button->Size = System::Drawing::Size(33, 20);
			this->AnimationDatDel_button->TabIndex = 59;
			this->AnimationDatDel_button->Text = L"Del";
			this->AnimationDatDel_button->UseVisualStyleBackColor = false;
			this->AnimationDatDel_button->Click += gcnew System::EventHandler(this, &cPIEditor::AnimationDatAdd_button_Click);
			// 
			// AnimationDatAdd_button
			// 
			this->AnimationDatAdd_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationDatAdd_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDatAdd_button->Location = System::Drawing::Point(1041, 56);
			this->AnimationDatAdd_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AnimationDatAdd_button->Name = L"AnimationDatAdd_button";
			this->AnimationDatAdd_button->Size = System::Drawing::Size(33, 25);
			this->AnimationDatAdd_button->TabIndex = 58;
			this->AnimationDatAdd_button->Text = L"Add";
			this->AnimationDatAdd_button->UseVisualStyleBackColor = false;
			this->AnimationDatAdd_button->Click += gcnew System::EventHandler(this, &cPIEditor::AnimationDatAdd_button_Click);
			// 
			// AnimationDataName_textBox
			// 
			this->AnimationDataName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationDataName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDataName_textBox->Location = System::Drawing::Point(1025, 26);
			this->AnimationDataName_textBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AnimationDataName_textBox->Name = L"AnimationDataName_textBox";
			this->AnimationDataName_textBox->Size = System::Drawing::Size(76, 22);
			this->AnimationDataName_textBox->TabIndex = 57;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label2->Location = System::Drawing::Point(1102, 8);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(88, 12);
			this->label2->TabIndex = 56;
			this->label2->Text = L"AnimationData:ist";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label1->Location = System::Drawing::Point(886, 8);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(75, 12);
			this->label1->TabIndex = 55;
			this->label1->Text = L"AnimationData";
			// 
			// AnimationDataList_listBox
			// 
			this->AnimationDataList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AnimationDataList_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationDataList_listBox->FormattingEnabled = true;
			this->AnimationDataList_listBox->ItemHeight = 12;
			this->AnimationDataList_listBox->Location = System::Drawing::Point(1104, 23);
			this->AnimationDataList_listBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AnimationDataList_listBox->Name = L"AnimationDataList_listBox";
			this->AnimationDataList_listBox->Size = System::Drawing::Size(152, 124);
			this->AnimationDataList_listBox->TabIndex = 54;
			this->AnimationDataList_listBox->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &cPIEditor::AnimationData_listBox_MouseDoubleClick);
			this->AnimationDataList_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &cPIEditor::AnimationData_listBox_KeyUp);
			// 
			// AnimationImageDown_button
			// 
			this->AnimationImageDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationImageDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationImageDown_button->Location = System::Drawing::Point(848, 108);
			this->AnimationImageDown_button->Name = L"AnimationImageDown_button";
			this->AnimationImageDown_button->Size = System::Drawing::Size(32, 34);
			this->AnimationImageDown_button->TabIndex = 53;
			this->AnimationImageDown_button->Text = L"v\r\nv";
			this->AnimationImageDown_button->UseVisualStyleBackColor = false;
			// 
			// AnimationImageUp_button
			// 
			this->AnimationImageUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationImageUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationImageUp_button->Location = System::Drawing::Point(848, 66);
			this->AnimationImageUp_button->Name = L"AnimationImageUp_button";
			this->AnimationImageUp_button->Size = System::Drawing::Size(32, 36);
			this->AnimationImageUp_button->TabIndex = 52;
			this->AnimationImageUp_button->Text = L"^\r\n^";
			this->AnimationImageUp_button->UseVisualStyleBackColor = false;
			// 
			// DelAnimationImage_button
			// 
			this->DelAnimationImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelAnimationImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelAnimationImage_button->Location = System::Drawing::Point(848, 38);
			this->DelAnimationImage_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->DelAnimationImage_button->Name = L"DelAnimationImage_button";
			this->DelAnimationImage_button->Size = System::Drawing::Size(33, 20);
			this->DelAnimationImage_button->TabIndex = 51;
			this->DelAnimationImage_button->Text = L"Del";
			this->DelAnimationImage_button->UseVisualStyleBackColor = false;
			this->DelAnimationImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddAnimationImage_button_Click);
			// 
			// AddAnimationImage_button
			// 
			this->AddAnimationImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddAnimationImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAnimationImage_button->Location = System::Drawing::Point(848, 5);
			this->AddAnimationImage_button->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AddAnimationImage_button->Name = L"AddAnimationImage_button";
			this->AddAnimationImage_button->Size = System::Drawing::Size(33, 25);
			this->AddAnimationImage_button->TabIndex = 50;
			this->AddAnimationImage_button->Text = L"Add";
			this->AddAnimationImage_button->UseVisualStyleBackColor = false;
			this->AddAnimationImage_button->Click += gcnew System::EventHandler(this, &cPIEditor::AddAnimationImage_button_Click);
			// 
			// AnimationData_listBox
			// 
			this->AnimationData_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AnimationData_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationData_listBox->FormattingEnabled = true;
			this->AnimationData_listBox->ItemHeight = 12;
			this->AnimationData_listBox->Location = System::Drawing::Point(888, 23);
			this->AnimationData_listBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->AnimationData_listBox->Name = L"AnimationData_listBox";
			this->AnimationData_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AnimationData_listBox->Size = System::Drawing::Size(134, 124);
			this->AnimationData_listBox->TabIndex = 49;
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Resize += gcnew System::EventHandler(this, &cPIEditor::splitContainer2_Panel1_Resize);
			this->splitContainer2->Panel2Collapsed = true;
			this->splitContainer2->Size = System::Drawing::Size(1276, 743);
			this->splitContainer2->SplitterDistance = 356;
			this->splitContainer2->SplitterWidth = 3;
			this->splitContainer2->TabIndex = 0;
			// 
			// dockToolStripMenuItem
			// 
			this->dockToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->floatToolStripMenuItem, 
				this->childToolStripMenuItem});
			this->dockToolStripMenuItem->Name = L"dockToolStripMenuItem";
			this->dockToolStripMenuItem->Size = System::Drawing::Size(46, 20);
			this->dockToolStripMenuItem->Text = L"Dock";
			// 
			// floatToolStripMenuItem
			// 
			this->floatToolStripMenuItem->Name = L"floatToolStripMenuItem";
			this->floatToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->floatToolStripMenuItem->Text = L"Float";
			this->floatToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// childToolStripMenuItem
			// 
			this->childToolStripMenuItem->Name = L"childToolStripMenuItem";
			this->childToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->childToolStripMenuItem->Text = L"Child";
			this->childToolStripMenuItem->Click += gcnew System::EventHandler(this, &cPIEditor::floatToolStripMenuItem_Click);
			// 
			// cPIEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->Name = L"cPIEditor";
			this->Size = System::Drawing::Size(1280, 1024);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageHeight_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageWidth_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitStartX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitEndY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitEndX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewPIUnitStartY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageDistanceY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImageDistanceX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImagePosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImagePosY_numericUpDown))->EndInit();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void AllImage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				 if( AllImage_listBox->SelectedIndex != -1 )
				 {
					 if(e->KeyCode == Keys::Delete)
					 {
						if( m_pImageomposerIRM->Count() )
						{
							m_pImageomposerIRM->RemoveObject(AllImage_listBox->SelectedIndex);
							m_ImageTale->Remove(AllImage_listBox->SelectedItem->ToString());
							AllImage_listBox->Items->RemoveAt(AllImage_listBox->SelectedIndex);
							ImageDetail_textBox->Text = GetTotalPixelExisted();
						}
					 }
				 }
			 }
	private: System::Void DeleteImage_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( sender == DeleteImage_button )
				 {
					 this->timer1->Enabled = false;
					 std::list<int>	l_iErseIndexList;
					 System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
					 l_MyList->AddRange(AllImage_listBox->SelectedItems);
					 for each (System::Object^l_pObject in l_MyList)
						l_iErseIndexList.push_back(AllImage_listBox->Items->IndexOf(l_pObject));
					 if( l_MyList->Count )
						m_iCurrentSelectedObjectIndex = -1;
					 for each (System::Object^l_pObject in l_MyList)
					 {
						int	l_iIndex = AllImage_listBox->Items->IndexOf(l_pObject);
						AllImage_listBox->Items->Remove(l_pObject);
						NamedTypedObject*l_pNamedTypedObject = m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_pObject->ToString()));
						if( l_pNamedTypedObject->GetOwner() )
							m_ImageTale->Remove(l_pObject->ToString());

						for(int i = 0;i<m_pImageomposerIRM->Count();++i)
						{
							cUIImage*l_pUIImage2 = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
							if( l_pUIImage2->m_pEditorAttachParent == l_pNamedTypedObject )
							{
								int	l_iIndex = AllImage_listBox->Items->IndexOf(DNCT::WcharToGcstring(l_pUIImage2->GetName()));
								AllImage_listBox->Items->RemoveAt(l_iIndex);
								m_pImageomposerIRM->RemoveObject(l_pUIImage2->GetName());
								--i;
							}
						}
						m_pImageomposerIRM->RemoveObject(DNCT::GcStringToWchar(l_pObject->ToString()));
					 }
					 if( m_pImageomposerIRM->Count() != AllImage_listBox->Items->Count )
						 UT::ErrorMsg(L"delete image error occur!!!",L"call fatming");
					 this->timer1->Enabled = true;
				 }
				 else
				 {
					array<String^>^l_pImageNameList = DNCT::OpenFileAndGetNames("image Files(*.png;*.bmp;*.jpg;*.dds)|*.png;*.bmp;*.jpg;*.dds|All files (*.*)|*.*");
					if( !l_pImageNameList )
						return;
					this->timer1->Enabled = false;
					for each(String^l_strFileName in l_pImageNameList)
					{
						String^l_strFileNameWithoutDirectory = DNCT::GetFileNameWithoutFullPath(l_strFileName,true);
						bool	l_bAlpha = false;
						if( l_strFileName->Contains(".png")||l_strFileName->Contains(".PNG") )
							l_bAlpha = true;					
						int l_iOriginalIndex = -1;
                        if(DNCT::CheckListContainStringAndAdd(AllImage_listBox,l_strFileNameWithoutDirectory))
                        {
					        WARING_YES_NO_TO_NO(l_strFileNameWithoutDirectory+"object name already exists,would u like to replace?"+DNCT::GetChanglineString()+"該物件名稱已存在,要覆蓋嗎?")
					            continue;
					        //remove old data
					        m_ImageTale->Remove(l_strFileNameWithoutDirectory);
					        l_iOriginalIndex = m_pImageomposerIRM->GetObjectIndexByName(DNCT::GcStringToWchar(l_strFileNameWithoutDirectory));
					        cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_strFileNameWithoutDirectory)));
					        SAFE_DELETE(l_pUIImage);
					    }
						try
						{
							//System::Diagnostics::Stopwatch^ sw = gcnew System::Diagnostics::Stopwatch();
							//sw->Start();
							System::Drawing::Bitmap^l_pImage;
							cUIImage*l_pUIImage = 0;
							bool	l_bUsingDDS = false;
							if( l_bUsingDDS )
							{
								l_pImage = DNCT::OpenImageFile(l_strFileName,&l_pUIImage);
							}
							else
							{
								l_pImage = DNCT::OpenImageFile(l_strFileName);
							}



							m_ImageTale[l_strFileNameWithoutDirectory] = l_pImage;
							if( !l_pUIImage )
								l_pUIImage = GetNewUIImageByBitMap(l_pImage,UT::GetFileNameWithoutFullPath(DNCT::GcStringToWchar(l_strFileName)));
							//POINT	l_WidthHeight = { *l_pUIImage->GetImageShowWidth()-1,*l_pUIImage->GetImageShowHeight()-1 };
							//l_pUIImage->SetRightDownStripOffPos(l_WidthHeight);
							//sw->Stop();
							if( l_iOriginalIndex != -1 )
						        (*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
						    else
							    m_pImageomposerIRM->AddObject(l_pUIImage);
						}
						catch(System::Exception^l_pException)
						{
							WARNING_MSG("doesn't support such file"+l_strFileName+DNCT::GetChanglineString()+l_pException->ToString());
							AllImage_listBox->Items->Remove(l_strFileNameWithoutDirectory);
						}
					}
					this->timer1->Enabled = true;
				 }
				 ImageDetail_textBox->Text = GetTotalPixelExisted();
			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( !this->Visible )
				 {
					 ::Threading::Thread::Sleep(100);
				 }
				//wglMakeCurrent(NULL,NULL);
				wglMakeCurrent( m_HdcMV,m_HGLRCMV );
				static	float	l_sfTime = 0;
				l_sfTime += 0.01f;
				if( l_sfTime >1.f )
					l_sfTime = 0.f;
				DEFAULT_SHADER = L"ICPathShader";
				NO_TEXTURE_SHADER = L"IC_NoTextureShader";
				UseShaderProgram();
				m_pTimeAndFPS->Update();
				float	l_fElpaseTime = m_pTimeAndFPS->fElpaseTime;
				glViewport(0,0,splitContainer2->Panel1->Width,splitContainer2->Panel1->Height);
				glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
				glClearColor( g_vBackgroundColor.x,g_vBackgroundColor.y,g_vBackgroundColor.z,g_vBackgroundColor.w );
				glClearDepth(1.0f);									// Depth Buffer Setup
				POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
				m_pOrthogonalCamera->Render();
				m_pOrthogonalCamera->DrawGrid(0,0,Vector4(0.3f,0.7f,0.3f,0.7f));
				glEnable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_TEXTURE_2D);
				for( int i=0;i<m_pImageomposerIRM->Count();++i )
				{
					cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					if( !l_p->m_pEditorAttachParent )
						l_p->Render();
				}
				glDisable(GL_TEXTURE_2D);
				int	l_iRectWidth = 0;
				int	l_iRectHeight = 0;
				if( AllShowBoundry_checkBox->Checked )
				{
					int	l_iNum =m_pImageomposerIRM->Count();
					for( int i=0;i<m_pImageomposerIRM->Count();++i )
					{
						cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
						Vector4	l_vColor(0,0,0,1);
						l_vColor.x = 1.f/((i%3)+1);
						l_vColor.y = 0.5f;
						l_vColor.z = 1.f/((l_iNum%(i+1))+1);
						l_vColor.w = 1;
						Vector3	l_vPos = *l_p->GetPos();
						//+1 for offset start at 0,0
						l_iRectWidth = l_p->GetRightDownStripOffPos().x-l_p->GetOffsetPos()->x+1;
						l_iRectHeight = l_p->GetRightDownStripOffPos().y-l_p->GetOffsetPos()->y+1;
						l_vPos.x+=l_p->GetOffsetPos()->x;
						l_vPos.y+=l_p->GetOffsetPos()->y;
						POINT	l_Pos = { (int)l_vPos.x,(int)l_vPos.y };
						//+1 for 0,0 start offset
						RenderRectangle(l_Pos,l_iRectWidth,l_iRectHeight,l_vColor,0,1);
					}
				}
				if( AllImage_listBox->Items->Count&&m_iCurrentSelectedObjectIndex!=-1 )
				{
					if( ShowBoundary_checkBox->Checked )
					{
						static Vector4 l_vColor(0,0,0,1);
						l_vColor.x = l_sfTime;
						l_vColor.y -= l_sfTime;
						l_vColor.z = l_sfTime;
						l_vColor.w = l_sfTime;
						if( l_vColor.x >= 1.f )
							l_vColor.x = 1.f;
						if( l_vColor.y<= 0 )
							l_vColor.y = 0;
						if( m_iCurrentSelectedObjectIndex>=m_pImageomposerIRM->Count() )
							m_iCurrentSelectedObjectIndex = m_pImageomposerIRM->Count()-1;
						if(  m_pImageomposerIRM->Count()>-1 )
						{
							cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
							Vector4	l_vOriginalColor = *l_p->GetColor();
							l_p->SetColor(l_vColor);
							l_p->Render();
							l_p->SetColor(l_vOriginalColor);
							Vector3	l_vPos = *l_p->GetPos();
							//+1 for offset start at 0,0
							l_iRectWidth = l_p->GetRightDownStripOffPos().x-l_p->GetOffsetPos()->x+1;
							l_iRectHeight = l_p->GetRightDownStripOffPos().y-l_p->GetOffsetPos()->y+1;
							l_vPos.x+=l_p->GetOffsetPos()->x;
							l_vPos.y+=l_p->GetOffsetPos()->y;
							POINT	l_Pos = {(int)l_vPos.x,(int)l_vPos.y};
							//+1 for 0,0 start offset
							RenderRectangle(l_Pos,l_iRectWidth,l_iRectHeight,l_vColor,0,1);
						}
					}
				}
				//for final image size
				POINT	l_Pos = {0,0};
				RenderRectangle(l_Pos,(int)ImageWidth_numericUpDown->Value,(int)ImageHeight_numericUpDown->Value,Vector4(1.f,0.3f,0.3f,0.8f));
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
				//GLRender::glDisable2D();
				glFlush();
				SwapBuffers (m_HdcMV);
			 }
	private: System::Void AutoOrderPosition_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				int	l_iCurrentX = 0;
				int	l_iCurrentY = 0;
				int	l_iMaxX = (int)ImageWidth_numericUpDown->Value;
				//the max height in current row
				int	l_iYLastBigPos = 0;
				if( !m_pImageomposerIRM->Count() )
					return;
				//so to fix UV problem,1 pixle problem
				int	l_iPixelXGap = (int)ImageDistanceX_numericUpDown->Value;
				int	l_iPixelYGap = (int)ImageDistanceY_numericUpDown->Value;
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(0));
				POINT	l_Pos = {-l_pUIImage->GetOffsetPos()->x,-l_pUIImage->GetOffsetPos()->y};
				l_pUIImage->SetPos(l_Pos);
				//+1 for start offset(0,0)
				l_iCurrentX = l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1;
				l_iYLastBigPos = l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1;
				//if(OnePixelDistance_checkBox->Checked)
				{
					l_iYLastBigPos += l_iPixelYGap;
					l_iCurrentX += l_iPixelXGap;
				}
				for( int i=1;i<m_pImageomposerIRM->Count();++i )
				{
					l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					if( l_pUIImage->m_pEditorAttachParent )
						continue;
					//this length has pixels,offset+width-rightdown pos,add 1 for avoid same image at same pixel
					//+1 for offset start at 0,0
					int	l_iXLengthForPixels = l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1;
					//if(OnePixelDistance_checkBox->Checked)
						l_iXLengthForPixels += l_iPixelXGap;
					l_iCurrentX += (l_iXLengthForPixels);
					//the y pixel we do not need
					//+1 for offset start at 0,0
					int	l_iGarbagePixelY = l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1;
					l_iGarbagePixelY += l_iPixelXGap;
					if( l_iCurrentX>l_iMaxX)
					{
						//for next start not object position
						l_iCurrentX = l_iXLengthForPixels;//-l_pUIImage->GetOffsetPos()->x;
						l_iCurrentY += l_iYLastBigPos;
						//+1 for offset start at 0,0
						l_iYLastBigPos = l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1;
						//if(OnePixelDistance_checkBox->Checked)
						    l_iYLastBigPos += l_iPixelYGap;
						//set object position x to offset pos,because it's changing to next line
						//
						l_Pos.x = -l_pUIImage->GetOffsetPos()->x;
						l_Pos.y = l_iCurrentY-l_pUIImage->GetOffsetPos()->y;
						l_pUIImage->SetPos(l_Pos);
						continue;
					}
					if( l_iYLastBigPos<l_iGarbagePixelY )
						l_iYLastBigPos = l_iGarbagePixelY;
					//although we know the position we wanted,we still have to minus the offset position to position
					l_Pos.x = l_iCurrentX-(l_iXLengthForPixels)-l_pUIImage->GetOffsetPos()->x;
					l_Pos.y = l_iCurrentY-l_pUIImage->GetOffsetPos()->y;
					l_pUIImage->SetPos(l_Pos);
				}
				AttachObjectPosAdjust();
			 }
	private: System::Void AllImage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 if( AllImage_listBox->SelectedIndex != -1 )
				 {
					m_iCurrentSelectedObjectIndex = AllImage_listBox->SelectedIndex;
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
					//+1 for offset start at 0,0
					POINT	l_ImageSize = l_pUIImage->GetImageRealSize();
					ImageDetail_textBox->Text = "X:"+l_pUIImage->GetPos()->x.ToString()+"Y:"+l_pUIImage->GetPos()->y.ToString()+DNCT::GetChanglineString()+
						"Size:"+l_ImageSize.x.ToString()+","+l_ImageSize.y.ToString()+DNCT::GetChanglineString()+
						"OffsetPos:"+l_pUIImage->GetOffsetPos()->x.ToString()+","+l_pUIImage->GetOffsetPos()->y.ToString()+
						GetTotalPixelExisted();
					if( l_pUIImage->m_pEditorAttachParent )
					{
						ImageDetail_textBox->Text += DNCT::GetChanglineString()+"AttachedPIUnit!!!";
						m_bAvoidDataAssignForPIUintChild = true;
						NewPIUnitStartX_numericUpDown->Value = (System::Decimal)l_pUIImage->GetPos()->x;
						NewPIUnitStartY_numericUpDown->Value = (System::Decimal)l_pUIImage->GetPos()->y;
						NewPIUnitEndX_numericUpDown->Value = (System::Decimal)(l_pUIImage->GetPos()->x+l_pUIImage->GetRightDownStripOffPos().x);
						NewPIUnitEndY_numericUpDown->Value = (System::Decimal)(l_pUIImage->GetPos()->y+l_pUIImage->GetRightDownStripOffPos().y);
						m_bAvoidDataAssignForPIUintChild = false;
					}
				 }
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 int	l_iSelectIndex = AllImage_listBox->SelectedIndex;
				 if( l_iSelectIndex == -1 )
					return;
				 std::list<int>	l_iAllSelectedIndex;
				 bool	l_bContainTop = false;
				 bool	l_bContainDown = false;
				 for each(int l_iIndex in AllImage_listBox->SelectedIndices)
					l_iAllSelectedIndex.push_back(l_iIndex);
				 l_iAllSelectedIndex.sort();
				 AllImage_listBox->SelectedIndices->Clear();
				 if( sender == ImageObjectDown_button )
				 {
					while( l_iAllSelectedIndex.size() )
					{
						int	l_iIndex = l_iAllSelectedIndex.back();
						l_iAllSelectedIndex.pop_back();
						 //it's bottom
						if( l_iIndex >= AllImage_listBox->Items->Count-1 )
						{
							WARNING_MSG("buttom one can't be down any more");
							goto ADJUST_IMAG_POS;
						}
						cUIImage*	l_pUIImage = m_pImageomposerIRM->GetUIImage(l_iIndex);
						m_pImageomposerIRM->RemoveObjectWithoutDelete( l_iIndex );
						m_pImageomposerIRM->GetList()->insert(m_pImageomposerIRM->GetList()->begin()+(l_iIndex+1),l_pUIImage);
						DNCT::SwapListBoxValue(l_iIndex,l_iIndex+1,AllImage_listBox);
						AllImage_listBox->SelectedIndices->Add(l_iIndex+1);
					}
				 }
				 else
				 {
					//it's top
					while( l_iAllSelectedIndex.size() )
					{
						int	l_iIndex = l_iAllSelectedIndex.front();
						l_iAllSelectedIndex.pop_front();
						 //it's top
						if( l_iIndex == 0 )
						{
							WARNING_MSG("top one can't be up any more");
							goto ADJUST_IMAG_POS;
						}
						cUIImage*	l_pUIImage = m_pImageomposerIRM->GetUIImage(l_iIndex);
						m_pImageomposerIRM->RemoveObjectWithoutDelete( l_iIndex );
						m_pImageomposerIRM->GetList()->insert(m_pImageomposerIRM->GetList()->begin()+(l_iIndex-1),l_pUIImage);
						DNCT::SwapListBoxValue(l_iIndex,l_iIndex-1,AllImage_listBox);
						AllImage_listBox->SelectedIndices->Add(l_iIndex-1);
					}
				 }
				 ADJUST_IMAG_POS:
				 AutoOrderPosition_button_Click(sender,e);
				 if( AllImage_listBox->SelectedIndex != -1 )
				 {
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(AllImage_listBox->SelectedIndex));
					POINT	l_ImageSize = {l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1,
						l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1};
					Vector3	l_vImagePos = *l_pUIImage->GetPos();
					POINT	l_ImagePos = {(int)l_vImagePos.x,(int)l_vImagePos.y};
					ImageDetail_textBox->Text = "X:"+l_ImagePos.x.ToString()+"Y:"+l_ImagePos.y.ToString()+DNCT::GetChanglineString()+
						"Size:"+l_ImageSize.x.ToString()+","+l_ImageSize.y.ToString()+DNCT::GetChanglineString()+
						"OffsetPos:"+l_pUIImage->GetOffsetPos()->x.ToString()+","+l_pUIImage->GetOffsetPos()->y.ToString()+
						GetTotalPixelExisted();
				 }
			 }
	//unform
	private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				if(this->Visible)
				{
					timer1->Enabled = true;
					wglMakeCurrent( m_HdcMV,m_HGLRCMV );
					glViewport(0,0,splitContainer2->Panel1->Width,splitContainer2->Panel1->Height);
				}
				else
				{
					timer1->Enabled = false;

				}
			 }
	private: System::Void MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer2->Panel1->Size.Width,(float)splitContainer2->Panel1->Size.Height));
				if(!timer1->Enabled)
					return;
				MouseCollideForPickUpObject(e,splitContainer2->Panel1);
			 }
	private: System::Void MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				splitContainer2->Panel1->Focus();
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer2->Panel1->Size.Width,(float)splitContainer2->Panel1->Size.Height));
				if(!timer1->Enabled)
					return;
				if( this->Visible )
					MouseCollideForPickUpObject(e,splitContainer2->Panel1);
			 }
	private: System::Void MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
			 {
				GCFORM::MouseButtons l_MouseButton = e->Button;
				m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton==System::Windows::Forms::MouseButtons::Left?true:false
					,l_MouseButton == System::Windows::Forms::MouseButtons::Right?true:false,e->Delta,e->X,e->Y,Vector2((float)splitContainer2->Panel1->Size.Width,(float)splitContainer2->Panel1->Size.Height));
				if(!timer1->Enabled)
					return;
				if( this->Visible )
					MouseCollideForPickUpObject(e,splitContainer2->Panel1);
			 }
	private: System::Void MyMouseHover(System::Object^  sender, System::EventArgs^  e) {
				if(!timer1->Enabled)
					return;
			 }
	private: System::Void MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
				if(!timer1->Enabled)
					return;
			 }
	private: System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				if( e->KeyCode == Keys::Add  )
				{
					m_pOrthogonalCamera->SetScale(m_pOrthogonalCamera->GetScale()-0.1f);
				}
				else
				if( e->KeyCode == Keys::Subtract )
				{
					m_pOrthogonalCamera->SetScale(m_pOrthogonalCamera->GetScale()+0.1f);
				}
				if( m_iCurrentSelectedObjectIndex!=-1 )
				{
					cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
					Vector3	l_Pos = *l_p->GetPos();
					if(e->KeyCode == Keys::Right)
						l_Pos.x+=1;
					if(e->KeyCode == Keys::Left)
						l_Pos.x-=1;
					if(e->KeyCode == Keys::Up)
						l_Pos.y-=1; 
					if(e->KeyCode == Keys::Down)
						l_Pos.y+=1;
					l_p->SetPos(l_Pos);
					AttachObjectPosAdjust();
				}
				//if(!timer1->Enabled)
				//	return;
			 }
	private: System::Void MyKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
				if(e->KeyCode == Keys::R)
					m_pOrthogonalCamera->Reset();
				//if(e->KeyCode == Keys::C)
				//{
				//	char l_strFileName[2048];
				//   SYSTEMTIME l_SYSTEMTIME;
				//   GetSystemTime(&l_SYSTEMTIME);
				//   sprintf(l_strFileName,"%d%d%d%d%d%d.bmp",l_SYSTEMTIME.wYear,l_SYSTEMTIME.wMonth,l_SYSTEMTIME.wDay,l_SYSTEMTIME.wHour,l_SYSTEMTIME.wMinute,l_SYSTEMTIME.wSecond);
				//   SaveCurrentBufferToImage(l_strFileName,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT);
				//}
				if(!timer1->Enabled)
					return;
				if( m_iCurrentSelectedObjectIndex!=-1 )
				{
					cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
					Vector3	l_Pos = *l_p->GetPos();
					if(e->KeyCode == Keys::Right)
						l_Pos.x+=1;
					if(e->KeyCode == Keys::Left)
						l_Pos.x-=1;
					if(e->KeyCode == Keys::Up)
						l_Pos.y-=1; 
					if(e->KeyCode == Keys::Down)
						l_Pos.y+=1;
					l_p->SetPos(l_Pos);
				}
			 }
private: System::Void Test_button_Click(System::Object^  sender, System::EventArgs^  e) {
			SavePuzzleFile();
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 GeneratePowOf2Image(sender==generateAllImageWithPowerOfTwoToolStripMenuItem?true:false);
		 }
private: System::Void ImageWidth_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) {

			 TotalPixel_label->Text = (ImageHeight_numericUpDown->Value*ImageWidth_numericUpDown->Value).ToString();
		 }
private: System::Void InvertPuzzleImage_button_Click(System::Object^  sender, System::EventArgs^  e) {
			 array< String^ >^l_pNameList = DNCT::OpenFileAndGetNames("pi files (*.pi)|*.pi|All files (*.*)|*.*");
			 if( !l_pNameList )
				 return;
			 String^l_strDirectory = DNCT::SelectDirectory();
			 if( !l_strDirectory )
				 return;
			 if( l_pNameList )
			 for each (String ^l_fileName in l_pNameList)
			 {
				 cNodeISAX	l_cNodeISAX;
				 TiXmlElement*l_pRootTiXmlElement = 0;
				 bool	l_b = l_cNodeISAX.ParseDataIntoXMLNode(DNCT::GcStringToChar(l_fileName));
				 if( l_b )
				 {
					const WCHAR*	l_strFileName = 0;
					 //find the tag we needed
					l_pRootTiXmlElement	= l_cNodeISAX.GetXmlElementByNameFromRoot(L"PuzzleImage");
					while( l_pRootTiXmlElement )
					{
						l_strFileName = l_pRootTiXmlElement->Attribute(L"ImageName");
						System::Drawing::Bitmap^l_pBitmapSource;
						String^l_strPGileName = DNCT::GetDirectoryWithoutFileName(l_fileName)+String(l_strFileName).ToString();
						try
						{
							l_pBitmapSource = gcnew System::Drawing::Bitmap(l_strPGileName);
						}
						catch(System::Exception^l_pExecption)
						{
							l_pExecption->ToString();
							WARNING_MSG(l_strPGileName+" not existed");
							continue;
						}
						int	l_iCount = _wtoi(l_pRootTiXmlElement->Attribute(L"Count"));
						TiXmlElement*l_pPuzzleImageUintTiXmlElement = l_pRootTiXmlElement->FirstChildElement();
						while(l_pPuzzleImageUintTiXmlElement)
						{
							TiXmlElement*l_pCurrentElement  = l_pPuzzleImageUintTiXmlElement;
							TiXmlAttribute*l_pTiXmlAttribute = l_pCurrentElement->FirstAttribute();
							float	l_fUV[4];
							POINT	l_Size;
							POINT	l_OffsetPos;
							POINT	l_ShowPosInPI;
							POINT	l_OriginalSize;
							//new attribute so make sure the data has this
							bool	l_bContainShowPosInPI = false;
							const WCHAR*	l_strPuzzleImageName = 0;
							while( l_pTiXmlAttribute )
							{
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"UV") )
								{
									GetUV(l_pTiXmlAttribute->Value(),l_fUV);
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"Size") )
								{
									l_Size = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"OffsetPos") )
								{
									l_OffsetPos = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"Name") )
								{
									l_strPuzzleImageName = l_pTiXmlAttribute->Value();
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"ShowPosInPI") )
								{
									l_bContainShowPosInPI = true;
									l_ShowPosInPI = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								if( !wcscmp(l_pTiXmlAttribute->Name(),L"OriginalSize") )
								{
									l_OriginalSize = GetPoint(l_pTiXmlAttribute->Value());
								}
								else
								{
									assert(0);
								}
								l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
							}
							//fill all alpha
							System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap( l_OriginalSize.x,l_OriginalSize.y );
							System::Drawing::Color l_NewColor = System::Drawing::Color::Transparent;
							System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( l_NewColor );
							System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
							l_pGr->FillRectangle(l_pDrawBrush,0,0,l_pBitMap->Width,l_pBitMap->Height);
							System::Drawing::Rectangle	l_rect = System::Drawing::Rectangle((int)(l_fUV[0]*l_pBitmapSource->Width),(int)(l_fUV[1]*l_pBitmapSource->Height),l_Size.x,l_Size.y);
							if( l_bContainShowPosInPI )
							{
								l_rect = System::Drawing::Rectangle(l_ShowPosInPI.x,l_ShowPosInPI.y,l_Size.x,l_Size.y);
							}
							if( 0 )
							{
								////copy Src
								//int	bufferSizeInPixels = l_pBitMap->Width*l_pBitMap->Height;
								//char*l_pbuff = new char[bufferSizeInPixels*l_iSrcChannel];
								//BitmapData^l_pData =
								//l_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, l_pBitMap->Width, l_pBitMap->Height),
								//ImageLockMode::WriteOnly,l_pBitmapForSave->PixelFormat);
								//memcpy(l_pbuff,l_pData->Scan0.ToPointer(),bufferSizeInPixels*l_iSrcChannel);
								//l_pBitmapForSave->UnlockBits(l_pData);
								////past to dest
								//l_pData = l_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, l_pBitMap->Width, l_pBitMap->Height),ImageLockMode::WriteOnly,l_pBitmapForSave->PixelFormat);
								//char*l_strScrData = (char*)l_pData->Scan0.ToPointer();

								//int	l_iSrcRenderStartPosX = l_pUIImage->GetOffsetPos()->x;
								//int	l_iSrcRenderStartPosY = l_pUIImage->GetOffsetPos()->y;
								//+1 for offset start at 0,0
								//int	l_iSrcRenderEndPosX = l_pUIImage->GetRightDownStripOffPos().x+1;
								//int	l_iSrcRenderEndPosY = l_pUIImage->GetRightDownStripOffPos().y+1;
								//int	l_iDestRenderPosX = StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos()->x+l_pUIImage->GetOffsetPos()->x);
								//int	l_iDestRenderPosY = StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos()->y+l_pUIImage->GetOffsetPos()->y);
								//int	l_iWorkPixelX = l_iSrcRenderEndPosX-l_iSrcRenderStartPosX;
								//int	l_iWorkPixelY = l_iSrcRenderEndPosY-l_iSrcRenderStartPosY;
								//int	l_iIndex = 0;
								//for( int l_iStartPixelY=l_iSrcRenderStartPosY;l_iStartPixelY<l_iSrcRenderEndPosY;++l_iStartPixelY )
								//{
								//	int	l_iYIndex = ((l_iDestRenderPosY+l_iIndex)*l_iSrcChannel*l_pBitMap->Width);
								//	int	l_iXIndex = l_iSrcChannel*l_iDestRenderPosX;
								//	int	l_iStartCopyIndex = l_iXIndex+l_iYIndex;
								//	int	l_iCopyIntoIndex = (l_iStartPixelY*l_iSrcChannel*l_pBitmapForSave->Width)+(l_iSrcChannel*l_iSrcRenderStartPosX);
								//	memcpy(&l_strScrData[l_iStartCopyIndex],&l_pbuff[l_iCopyIntoIndex],l_iWorkPixelX*l_iSrcChannel);
								//	++l_iIndex;
								//}
								//l_pBitMap->UnlockBits(l_pData);
								//delete l_pbuff;
							}
							else
								l_pGr->DrawImage(l_pBitmapSource,l_OffsetPos.x,l_OffsetPos.y,l_rect,System::Drawing::GraphicsUnit::Pixel);
							l_pBitMap->Save(l_strDirectory+String(l_strPuzzleImageName).ToString()+".png");
							l_pPuzzleImageUintTiXmlElement = l_pPuzzleImageUintTiXmlElement->NextSiblingElement();
						}
						if( l_pRootTiXmlElement->NextSibling() )
							l_pRootTiXmlElement	= l_cNodeISAX.GetXmlElementByNameFromElement(L"PuzzleImage",l_pRootTiXmlElement->NextSiblingElement());
						else
							l_pRootTiXmlElement = 0;
					}
				 }
			 }
		 }
private: System::Void TO96PdiPngFile_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			String^l_strDirectory = DNCT::SelectDirectory();
			if( !l_strDirectory )
				return;
			array<String^>^l_AllFileName = DNCT::OpenFileAndGetNames();
			for each(String^l_strName in l_AllFileName)
			{
				System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap(l_strName);
				l_pBitMap->Save(l_strDirectory+DNCT::GetFileNameWithoutFullPath(l_strName));
			}
			//System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( System::Drawing::Color::Transparent );
			//for( int i=0;i<m_pImageomposerIRM->Count();++i )
			//{
			//	cUIImage*l_pUIImage = (cUIImage*)((*m_pImageomposerIRM)[i]);
			//	System::Drawing::Bitmap^l_pBitmapForSave = (System::Drawing::Bitmap^)m_ImageTale[DNCT::WcharToGcstring((*m_pImageomposerIRM)[i]->GetName())];
			//	l_pBitMap->Save(l_strDirectory+DNCT::WcharToGcstring(l_pUIImage->GetName())+".png");
			//}
		}
private: System::Void openXMLToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^l_strFileName = DNCT::OpenFileAndGetName("PuzzleImage(*.pi)|*.pi");
			 //String^l_strFileName = DNCT::OpenFileAndGetName("txt files (*.txt)|*.txt|All files (*.*)|*.*");
			 if( l_strFileName )
			 {
				 OpenPIFile(l_strFileName);
			 }
		 }
private: System::Void AddAnimationImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == AddAnimationImage_button )
			 {
				 if( AllImage_listBox->SelectedIndices->Count == 0)
					 return;
				 while(AllImage_listBox->SelectedIndices->Count != 0 )
				 {
					int	l_iIndex = AllImage_listBox->SelectedIndices[0];
					m_pImageIndexOfAnimation->AddNameObject(DNCT::GcStringToWchar(AllImage_listBox->SelectedItems[0]->ToString()),AllImage_listBox->SelectedIndices[0],0.1f);
					AnimationData_listBox->Items->Add(AllImage_listBox->SelectedItems[0]->ToString());
					AllImage_listBox->SelectedItems->Remove(AllImage_listBox->SelectedItems[0]);
				 }
			 }
			 else
			 if( sender == DelAnimationImage_button )
			 {
				 if( AnimationData_listBox->SelectedIndices->Count == 0 )
					 return;
				 while(this->AnimationData_listBox->SelectedIndices->Count)
				 {
					int	l_iIndex = AnimationData_listBox->SelectedIndices[0];
					m_pImageIndexOfAnimation->RemoveNameObject(l_iIndex);
					AnimationData_listBox->Items->RemoveAt(l_iIndex);
				  }
			 }
		 }
private: System::Void AnimationDatAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( sender == AnimationDatAdd_button )
			 {
				if( AnimationDataName_textBox->Text->Length )
				{
				    if( m_pImageIndexOfAnimation->Count() )
				    {
					    if( !AnimationDataList_listBox->Items->Contains(AnimationDataName_textBox->Text) )
					    {
						    AnimationDataList_listBox->Items->Add(AnimationDataName_textBox->Text);
						    AnimationData_listBox->Items->Clear();
						    cImageIndexOfAnimation*l_pImageIndexOfAnimation = new cImageIndexOfAnimation(m_pImageIndexOfAnimation);
						    m_pImageIndexOfAnimation->Clear();
						    l_pImageIndexOfAnimation->SetName(DNCT::GcStringToWchar(AnimationDataName_textBox->Text));
						    m_pImageIndexOfAnimationList->AddObject(l_pImageIndexOfAnimation);
					    }
					}
				}
			 }
			 else
			 if( sender == AnimationDatDel_button )
			 {
				 while( AnimationDataList_listBox->SelectedItems->Count )
				 {
					 m_pImageIndexOfAnimationList->RemoveObject(DNCT::GcStringToWchar(AnimationDataList_listBox->SelectedItems[0]->ToString()));
					 AnimationDataList_listBox->Items->Remove(AnimationDataList_listBox->SelectedItems[0]);
				 }
			 }
		 }

private: System::Void ImagePosX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			int	l_iSelectedIndex = AllImage_listBox->SelectedIndex;
			if( l_iSelectedIndex != -1 )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[l_iSelectedIndex]);
				l_pUIImage->SetPos(Vector3((float)ImagePosX_numericUpDown->Value,(float)ImagePosY_numericUpDown->Value,0.f));
			}
		 }
private: System::Void AllImage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 if( AllImage_listBox->SelectedIndex != -1)
			 {
				 label3->Text = "Pos:"+AllImage_listBox->Text;
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[AllImage_listBox->SelectedIndex]);
				Vector3	l_vPos = *l_pUIImage->GetPos();
				ImagePosX_numericUpDown->Value = (int)l_vPos.x;
				ImagePosY_numericUpDown->Value = (int)l_vPos.y;
			 }
		 }
private: System::Void AllImage_listBox_KeyUp_1(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
		 {
			 if(e->KeyCode == Keys::Enter)
			 {
				 System::Windows::Forms::MouseEventArgs^  e2;
				AllImage_listBox_MouseDoubleClick(sender,e2);
			 }
		 }
private: System::Void AnimationData_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
        {
            AssignAnimationData();
        }
private: System::Void AnimationData_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
        {
            if( e->KeyCode == GCFORM::Keys::Enter )
                AssignAnimationData();
        }
private: System::Void ProperStripPixel_button_Click(System::Object^  sender, System::EventArgs^  e)
        {
			POINT	l_MinOffset = {79979,79979};
			POINT	l_MaxRightDown = {0,0};
			POINT	l_OffsetPos;
			POINT	l_RightDown;
			for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
				l_OffsetPos = *l_pUIImage->GetOffsetPos();
				l_RightDown = l_pUIImage->GetRightDownStripOffPos();
				//+1 for offset start at 0,0
				l_RightDown.x +=1;
				l_RightDown.y +=1;

				l_MinOffset.x = min(l_MinOffset.x,l_OffsetPos.x);
				l_MinOffset.y = min(l_MinOffset.y,l_OffsetPos.y);
				l_MaxRightDown.x = max(l_MaxRightDown.x,l_RightDown.x);
				l_MaxRightDown.y = max(l_MaxRightDown.y,l_RightDown.y);
			}
			for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
				l_pUIImage->SetOffsetPos(l_MinOffset);
				l_pUIImage->SetRightDownStripOffPos(l_MaxRightDown);
			}
        }
private: System::Void StripAlpha_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( ToOriginalImage_button == sender )
			 {
				 for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
				 {
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
					POINT	l_WidthHeight = { *l_pUIImage->GetImageShowWidth()-1,*l_pUIImage->GetImageShowHeight()-1 };
					POINT	l_Offset = {0,0};
					l_pUIImage->SetRightDownStripOffPos(l_WidthHeight);
					l_pUIImage->SetOffsetPos(l_Offset);
				 }
				 AllImage_listBox_SelectedIndexChanged(sender,e);
			 }
			 else
			 if( StripAlpha_button == sender )
			 {
				 for each( int l_iSelectedIndex in AllImage_listBox->SelectedIndices )
				 {
					System::Drawing::Bitmap^l_pImage = (System::Drawing::Bitmap^)m_ImageTale[AllImage_listBox->Items[l_iSelectedIndex]->ToString()];
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
					l_pUIImage->SetOffsetPos(DNCT::ImageGetAlphaOffsetPosLeftTop(l_pImage));
					l_pUIImage->SetRightDownStripOffPos(DNCT::ImageGetAlphaOffsetPosRightDown(l_pImage));
					POINT	l_ImageRealSize = l_pUIImage->GetImageRealSize();
					POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
					POINT	l_OffsetPos = *l_pUIImage->GetOffsetPos();
					//because MPDI is setposition by center
					//if( l_ImageRealSize.x%2 )
					//{
					//	if( l_RightDownStripOffPos.x+1 < l_pUIImage->GetOriginalImageSize().x )
					//		l_RightDownStripOffPos.x += 1;
					//	else
					//	if( l_OffsetPos.x != 0 )
					//		l_OffsetPos.x -= 1;
					//}
					//if( l_ImageRealSize.y%2)
					//{
					//	if( l_RightDownStripOffPos.y+1 < l_pUIImage->GetOriginalImageSize().y )
					//		l_RightDownStripOffPos.y += 1;
					//	else
					//	if( l_OffsetPos.y != 0 )
					//		l_OffsetPos.y -= 1;
					//}
					l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
					l_pUIImage->SetOffsetPos(l_OffsetPos);
				 }
				 AllImage_listBox_SelectedIndexChanged(sender,e);
			 }
		 }

private: System::Void splitContainer2_Panel1_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
			Vector2	l_vOriginalResolution = m_pOrthogonalCamera->GetResolution();
			Vector2	l_vOriginalLeftUpPos = m_pOrthogonalCamera->GetCameraPos()-(l_vOriginalResolution/2.f);
			m_pOrthogonalCamera->SetResolution(Vector2((float)l_ViewportSize.x,(float)l_ViewportSize.y));
			m_pOrthogonalCamera->SetCameraPos(l_vOriginalLeftUpPos+m_pOrthogonalCamera->GetResolution()/2.f);
			m_pOrthogonalCamera->CameraUpdateByMouse(false,false,0,0,0,Vector2((float)splitContainer2->Panel1->Size.Width,(float)splitContainer2->Panel1->Size.Height));
		 }

private: System::Void AddNewPIUnitImage_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( NewPIUnitStartX_numericUpDown->Value != NewPIUnitEndX_numericUpDown->Value &&
				 NewPIUnitStartY_numericUpDown->Value != NewPIUnitEndY_numericUpDown->Value &&
				   NewPIUnitEndX_numericUpDown->Value >  NewPIUnitStartX_numericUpDown->Value && 
				   NewPIUnitEndY_numericUpDown->Value >  NewPIUnitStartY_numericUpDown->Value)
			 {
				 if( NewPIUnitName_textBox->Text->Length )
				 {
					 this->timer1->Enabled = false;
					 int	l_iOriginalIndex = -1;
					 POINT	l_OffsetPos = {0,0};
					 POINT	l_RightDownStripOffPos = {(int)NewPIUnitEndX_numericUpDown->Value-(int)NewPIUnitStartX_numericUpDown->Value,(int)NewPIUnitEndY_numericUpDown->Value-(int)NewPIUnitStartY_numericUpDown->Value};
					 Vector3	l_vPos((float)NewPIUnitStartX_numericUpDown->Value,(float)NewPIUnitStartY_numericUpDown->Value,0.f);
					 RECT	l_rc = {(int)l_vPos.x,(int)l_vPos.y,(int)l_vPos.x+l_RightDownStripOffPos.x,(int)l_vPos.y+l_RightDownStripOffPos.y};
					 int	l_iCollideIndex = -1;
					 for(int i = 0;i<m_pImageomposerIRM->Count();++i)
					 {
						cUIImage*l_pUIImage2 = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
						Vector3	l_vPos2 = *l_pUIImage2->GetPos();
						l_vPos2.x += l_pUIImage2->GetOffsetPos()->x;
						l_vPos2.y += l_pUIImage2->GetOffsetPos()->y;
						POINT	l_RightDownStripOffPos2 = l_pUIImage2->GetRightDownStripOffPos();
						RECT	l_rc2 = {(int)l_vPos2.x,(int)l_vPos2.y,(int)l_vPos2.x+l_RightDownStripOffPos2.x,(int)l_vPos2.y+l_RightDownStripOffPos2.y};
						if( UT::RectCollideRect(l_rc2,l_rc) )
						{
							if( l_iCollideIndex != -1 )
							{
								if(!l_pUIImage2->m_pEditorAttachParent)
								{
									WARING_YES_NO_TO_NO("over 2 image would u like to do this anyway?")
									{
										goto LEAVE;
									}
									break;
								}
							}
							//ensure it is not a piunit child
							if(!l_pUIImage2->m_pEditorAttachParent)
								l_iCollideIndex = i;
						}
					 }
					 if( l_iCollideIndex == -1 )
					 {
						WARNING_MSG("no parent,please reset pos");
						goto LEAVE;
					 }
					 if(AllImage_listBox->Items->Contains(NewPIUnitName_textBox->Text))
					 {
						WARING_YES_NO_TO_NO("replace exist object?")
						{
							goto LEAVE;
						}
						l_iOriginalIndex = AllImage_listBox->Items->IndexOf(NewPIUnitName_textBox->Text);
						AllImage_listBox->SelectedIndex = l_iOriginalIndex;
						DeleteImage_button_Click(DeleteImage_button,e);
					 }
					 cUIImage*l_pAttachUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iCollideIndex));
					 cUIImage*l_pUIImage = new cUIImage(l_pAttachUIImage);
					 l_pUIImage->m_vEditorAttachParentRelativePos = l_vPos-*l_pAttachUIImage->GetPos();
					 l_pUIImage->SetPos(l_vPos);
					 l_pUIImage->SetName(DNCT::GcStringToWchar(NewPIUnitName_textBox->Text));
					 l_pUIImage->SetOffsetPos(l_OffsetPos);
					 l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
					 l_RightDownStripOffPos.x += 1;
					 l_RightDownStripOffPos.y += 1;
					 l_pUIImage->SetOriginalImageSize(l_RightDownStripOffPos);
					 if( l_iOriginalIndex != -1 )
					 {
				        (*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
						AllImage_listBox->Items[l_iOriginalIndex] = NewPIUnitName_textBox->Text;
					 }
				     else
					 {
					    m_pImageomposerIRM->AddObject(l_pUIImage);
						AllImage_listBox->Items->Add(NewPIUnitName_textBox->Text);
					 }
					 LEAVE:
					 this->timer1->Enabled = true;
				 }
				 else
				 {
					WARNING_MSG("please input new PI unit name");
				 }
				 
			 }
			 else
			 {
				WARNING_MSG("data error");
			 }
		 }
private: System::Void NewPIUnitStartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( AllImage_listBox->SelectedIndex == -1 || m_bAvoidDataAssignForPIUintChild )
			 {
				return;
			 }
			 cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(AllImage_listBox->SelectedIndex));
			 if( !l_pUIImage->m_pEditorAttachParent )
				 return;
			 POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
			 Vector3	l_vPos = *l_pUIImage->GetPos();
			 l_RightDownStripOffPos.x = (int)(NewPIUnitEndX_numericUpDown->Value-NewPIUnitStartX_numericUpDown->Value);	
			 l_RightDownStripOffPos.y = (int)(NewPIUnitEndY_numericUpDown->Value-NewPIUnitStartY_numericUpDown->Value);
			 l_vPos.x = (float)NewPIUnitStartX_numericUpDown->Value;
			 l_vPos.y = (float)NewPIUnitStartY_numericUpDown->Value;
			 l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
			 l_pUIImage->SetPos(l_vPos);
			 l_RightDownStripOffPos.x += 1;
			 l_RightDownStripOffPos.y += 1;
			 l_pUIImage->SetOriginalImageSize(l_RightDownStripOffPos);
		 }
private: System::Void floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if( m_pForm )
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
};
//end namespace
}