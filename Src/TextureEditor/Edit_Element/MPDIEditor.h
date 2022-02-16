#pragma once
#include "../../Core/AVIPlayer/avi_utils.h"
#include "../../Core/Image/MPDIForEditor.h"
#include "MPDIThread.h"
//#pragma comment(lib, "Vfw32.lib")
#include "vfw.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace FMCEditor
{
	GLenum	IndexToBlendingIndex( int e_iIndex );
	int BlendingIndexToIndex( GLenum e_eBlendingIndex);
	/// <summary>
	/// MPDIEditor 的摘要
	/// </summary>
	public ref class MPDIEditor : public System::Windows::Forms::UserControl
	{
	public:
		MPDIEditor(GCFORM::TabControl^e_ptabControl,GCFORM::Form^e_pForm);

	protected:
		~MPDIEditor();
	private: System::Windows::Forms::CheckBox^  Loop_checkBox;
	private: System::Windows::Forms::Label^  ErrorMsg_label;
	private: System::Windows::Forms::ListBox^  PathNode_listBox;
	private: System::Windows::Forms::ListBox^  WholePath_listBox;
	private: System::Windows::Forms::ListBox^  WholePuzzleImageUnit_listBox;
	private: System::Windows::Forms::Button^  AssignImage_button;
	private: System::Windows::Forms::Button^  AddPath_button;
	private: System::Windows::Forms::Button^  DelPath_button;
	private: System::Windows::Forms::Button^  DelPoint_button;
	private: System::Windows::Forms::Button^  ClonePath_button;
	private: System::Windows::Forms::CheckBox^  AllPlay_checkBox;
	private: System::Windows::Forms::TrackBar^  CurrentProgress_trackBar;
	private: System::Windows::Forms::CheckBox^  ShowAllPath_checkBox;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  TotalPlayTime_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  PathLOD_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  PathNodeTime_numericUpDown;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  InsertPoint_button;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::NumericUpDown^  StartTime_numericUpDown;
	private: System::Windows::Forms::CheckBox^  Play_checkBox;
	private: System::Windows::Forms::Button^  AnimationInvert_button;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  AnimationName_textBox;
	private: System::Windows::Forms::ListBox^  WholeAnimationList_listBox;
	private: System::Windows::Forms::Button^  DeleteListObject_button;
	private: System::Windows::Forms::NumericUpDown^  NumPointsToInsert_numericUpDown;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  backgroundImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::Button^  AddAnimationToList_button;
	private: System::Windows::Forms::Button^  ApplyLODToPoints_button;
	private: System::Windows::Forms::ListBox^  ShowImageOnScreen_listBox;
	private: System::Windows::Forms::Button^  AddShowImageOnScreenObject_button;
	private: System::Windows::Forms::Button^  DelShowImageOnScreenObject_button;
	private: System::Windows::Forms::CheckBox^  AllCurveLoop_checkBox;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::ToolStripMenuItem^  editModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addPointModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  fixPointModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  moveWholePathToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openImageFileToolStripMenuItemToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::Button^  PathPiorityUp_button;
	private: System::Windows::Forms::Button^  PathPiorityDown_button;
	private: System::Windows::Forms::ToolStripMenuItem^  moveAllPathToolStripMenuItem;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::Button^  InvertPath_button;
	private: System::Windows::Forms::ToolStripMenuItem^  rotatePathToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  rotateAllPathToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  AllPath_listBox;
	private: System::Windows::Forms::ToolStripMenuItem^  openPathFileToolStripMenuItem;
	private: System::Windows::Forms::Button^  DelPathIntoAllPath_button;
	private: System::Windows::Forms::Button^  AddPathIntoAllPath_button;
	private: System::Windows::Forms::Button^  UsingIntoCurrentPath_button;
	private: System::Windows::Forms::ToolStripMenuItem^  savePathFileToolStripMenuItem;
	private: System::Windows::Forms::TextBox^  EventPointName_textBox;
	private: System::Windows::Forms::ComboBox^  HintPointType_comboBox;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Button^  DelEventPoint_button;
	private: System::Windows::Forms::Button^  AddEventPOint_button;
	private: System::Windows::Forms::NumericUpDown^  RearrangeWholePathTime_numericUpDown;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::ToolStripMenuItem^  assistantModeToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showImageBoderToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showSelectedImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveGamePathToolStripMenuItem;
	private: System::Windows::Forms::Button^  MergeMPDIData_button;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAviFileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showPath_ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  showErrorMsh_ToolStripMenuItem;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::CheckBox^  BehaviorUpdateActive_checkBox;
	private: System::Windows::Forms::NumericUpDown^  SizeScale_numericUpDown;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::NumericUpDown^  GreenColor_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  PathNodePosY_numericUpDown;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::NumericUpDown^  PathNodePosX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  RedColor_numericUpDown;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::NumericUpDown^  AlphaColor_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::NumericUpDown^  AngleZ_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  BlueColor_numericUpDown;
	private: System::Windows::Forms::CheckBox^  Mirror_checkBox;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  DestBlending_comboBox;
	private: System::Windows::Forms::CheckBox^  AverageAssign_checkBox;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;
	private: System::Windows::Forms::ComboBox^  SrcBlending_comboBox;
	private: System::Windows::Forms::CheckBox^  EnableColorBlending_checkBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::ComboBox^  GroupRotationType_comboBox;
	private: System::Windows::Forms::NumericUpDown^  GroupPosY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupPosZ_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupPosX_numericUpDown;
	private: System::Windows::Forms::ToolStripMenuItem^  showPanelToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  AllPI_listBox;
	private: System::Windows::Forms::ToolStripMenuItem^  showPIImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddPI_toolStripMenuItem;
	private: System::Windows::Forms::Button^  DelPI_button;
	private: System::Windows::Forms::NumericUpDown^  PathNodePosZ_numericUpDown;
	private: System::Windows::Forms::Button^  ApplyGroupDataChange_button;
	private: System::Windows::Forms::NumericUpDown^  GroupScale_numericUpDown;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::NumericUpDown^  GroupRotationZ_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupRotationY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  GroupRotationX_numericUpDown;
	private: System::Windows::Forms::ComboBox^  GroupPositionMoveType_comboBox;
	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::ToolStripMenuItem^  ScaleWithPIAlphaStripPixel_toolStripMenuItem;
	private: System::Windows::Forms::Button^  SelectFirstAndLast_button;
	private: System::Windows::Forms::CheckBox^  GroupImageRotate_checkBox;
	private: System::Windows::Forms::Button^  WholeAnimationListUp_button;
	private: System::Windows::Forms::Button^  WholeAnimationListDown_lbutton;
	private: System::Windows::Forms::ToolStripMenuItem^  saveImageToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  ShowSelectedImageBorder_checkBox;
	private: System::Windows::Forms::GroupBox^  Position_groupBox;
	private: System::Windows::Forms::GroupBox^  Color_groupBox;
	private: System::Windows::Forms::GroupBox^  Size_groupBox;
	private: System::Windows::Forms::GroupBox^  ColorBlending_groupBox;
	private: System::Windows::Forms::GroupBox^  Rotation_groupBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  AngleY_numericUpDown;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  AngleX_numericUpDown;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel2;
	private: System::Windows::Forms::Button^  SelectAllPoints_button;
	private: System::Windows::Forms::Button^  InvertAllPath_button;
	private: System::Windows::Forms::ToolStripMenuItem^  renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem;
	private: DotNetComponent::XmlNodeEditComponent^  MPDIExtra_xmlNodeEditComponent;
	private: System::Windows::Forms::ContextMenuStrip^  PathGroup_contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::ComboBox^  ImageAnchor_comboBox;
	private: System::Windows::Forms::ToolTip^  toolTip1;
	private: System::Windows::Forms::ContextMenuStrip^  GameUIData_menuStrip;
	private: System::Windows::Forms::CheckBox^  StayAtLastFrame_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  addPointDataWithImageByPathNameToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddSliderBarUI_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  AddMultiPageUI_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  addSliderBarWithTabPagesUIToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  hockToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  floatToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  childToolStripMenuItem;
	private: System::Windows::Forms::Panel^  GroupData_collpaseForm;
	private: System::Windows::Forms::Panel^  AnimationData_collpaseForm;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  Path_groupBox;
	private: System::Windows::Forms::GroupBox^  ImageHintPoint_groupBox;
	private: System::Windows::Forms::GroupBox^  HintPoint_groupBox;
	private: System::Windows::Forms::ListBox^  AllEventPointHint_listBox;
	private: System::Windows::Forms::GroupBox^  Video_groupBox;
	private: System::Windows::Forms::ToolStripMenuItem^  addMultipageWithTabPagesUIToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  yesNoDialogToolStripMenuItem;
	private: System::Windows::Forms::ContextMenuStrip^  PointDataUtility_contextMenuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  FadeIn_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  FadeOut_toolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  flashSelectedImageToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  mergeToolStripMenuItem;
	private: System::Windows::Forms::Button^  ApplyImageWithOriginalData_button;
	private: System::Windows::Forms::ToolStripMenuItem^  cameraFitToolStripMenuItem;
	private: DotNetComponent::XYZNumeriaclControl^  RotationPosOffset_xyzNumeriaclContro;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::ToolStripMenuItem^  showBGToolStripMenuItem;
	private: System::Windows::Forms::CheckBox^  UseViewPort_checkBox;
	private: System::Windows::Forms::ToolStripMenuItem^  showGridToolStripMenuItem;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationZ_numericUpDown;
	private: System::Windows::Forms::Label^  CameraRotation_label3;
	private: System::Windows::Forms::Label^  CameraRotationCenter_label;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationCenterY_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationCenterX_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  CameraRotationCenterZ_numericUpDown;
	private: DotNetComponent::XYZWNumericalControl^  ViewPort_xyzwNumericalControl;
	//my
	public: void	Active(){wglMakeCurrent( m_HdcMV,m_HGLRCMV ); this->timer1->Enabled = true; }
	//main form text
	public: GCFORM::Form^	m_pForm;
	public:HWND					m_pParentHandle;
	//for attach from.
	private:GCFORM::TabPage^m_pTabPage;
	//to determin is tabpage clicked.
	private:GCFORM::TabControl^m_pTabControl;
	//each mpdi data could define a in game data it will store as another extra file.
	//String,GCFORM::TreeNode.from MPDIExtra_xmlNodeEditComponent
	private:System::Collections::Hashtable^m_pMPDIExtraData;
	private:bool			m_bMutiSampleSupported;
	private:HGLRC			m_HGLRCMV;
	private:HDC				m_HdcMV;
	private:cPuzzleImage*	m_pCurrentWorkingPI;//for multi pi,get the current selected
	//if new pi has been add or delete call this
	private:void			AssignLatestPI();
	private:String^			m_strCurrentFileName;
	private:void			UIInitialize();
	private:void			Render();
	private:void			AssignValueChange(System::Object^  sender);
	private:void			ImageApply();
	private:void			AddIntoList();							//add new m_pMultiPathDynamicImageAWP->m_pCurrentCurve into m_pMPDIList
	private:float			m_fTimeForCurrentProgess;				//for m_pMultiPathDynamicImageAWP
	private:bool			m_bAnimationPauseByTrackValue;
	//sometimes we do not want to change value,so set this as flag to prevent
	private:bool			m_bPreventChangeValue;
	//while invert MPDI data botton is click,we do not expect it change the path data time
	private:bool			m_bPreventChangeAllTimeValueSetup;
	//
	private:bool			m_bMouseEnterPathNodeListbox;
	private:cGlyphFontRender*m_pDebugFont;
	private:System::Void	MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void	MyMouseHover(System::Object^  sender, System::EventArgs^  e);
	private:System::Void	MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:System::Void	MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
	private:void			PathModifyFunction(System::Object^  sender, System::EventArgs^  e);
	private:void			PathMoveModifyMode(System::Object^  sender, System::EventArgs^  e);
	private:void			PathSelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);//because the code is too long,so.....-_-
	private:void			DelPoint(System::Object^  sender, System::EventArgs^  e);
	private:void			PlayCheckedChanged(System::Object^  sender, System::EventArgs^  e);
	private:void			AddPath(System::Object^  sender, System::EventArgs^  e);
	private:void			AddPath(String^e_strPathName);
	private:void			SaveAndOpen_MPDI(System::Object^  sender, System::EventArgs^  e);
	private:void			ReferencePathModify(System::Object^  sender, System::EventArgs^  e);
	private:System::Void	EventPoint(System::Object^  sender, System::EventArgs^  e);
	private:System::Void	AddGameUI(System::Object^  sender);
	void					RenderHintPoint(cCueToStartCurveWithTime*e_pObject,bool e_bShowBorder);
	//how many time opened
	public:static	int						m_siOpenCount = 0;
	cAnimationParser*				m_pImagePlayManagerAWP;
	cCurveManager*					m_pReferenceCurveManagerAWP;
	cBaseImage*						m_pBKImageAWP;
	cMPDIList*						m_pMPDIList;
	cImageParser*					m_pImageResourceManagerAWP;
	cMPDIForEditor*					m_pMultiPathDynamicImageAWP;
	UT::sTimeCounter*				m_pTimeCounterForAutoSaveFile;
	cOrthogonalCamera				*m_pOrthogonalCamera;
	float							m_fAlpha;
	public:bool						m_bDataChanged;
	//just avoid the numerialBoxto reset data ecome loop
	//so stupid here but who care
	int								m_iCurrentSelectedPathListIndexAWP;
	int								m_iCurrentSelectedPathNodeIndexAWP;
	//just for image pos info let artist easier to adjust position
	cCueToStartCurveWithTime*		m_pInterlUseForShowImagePos;
	GLenum							m_sfactor;
	GLenum							m_dfactor;
	UT::sTimeAndFPS					*m_pTimeAndFPS;
	//
	cMPDIThread^	m_pMPDIThread;
	String^			m_strCurrentSaveFileName;
	public:void		OnlyRenderWindow();
	public:void		OpenMPDIFile(String^e_strFileName);
	public:void		Save(String^e_strFileName);
	public:void		Merge(String^e_strFileName);
	public:void		AddPIFile(String^e_strFileName);
	public:void		OpenPIFile(String^e_strFileName);
	public:System::Void MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
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
			this->DelPoint_button = (gcnew System::Windows::Forms::Button());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->TotalPlayTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Loop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ErrorMsg_label = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->PathLOD_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->PathNodeTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNode_listBox = (gcnew System::Windows::Forms::ListBox());
			this->PointDataUtility_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->FadeIn_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->FadeOut_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DelPath_button = (gcnew System::Windows::Forms::Button());
			this->WholePath_listBox = (gcnew System::Windows::Forms::ListBox());
			this->GameUIData_menuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->AddSliderBarUI_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddMultiPageUI_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addSliderBarWithTabPagesUIToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addMultipageWithTabPagesUIToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->yesNoDialogToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddPath_button = (gcnew System::Windows::Forms::Button());
			this->PathGroup_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addPointDataWithImageByPathNameToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AllPlay_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->CurrentProgress_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->ShowAllPath_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->ClonePath_button = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->InsertPoint_button = (gcnew System::Windows::Forms::Button());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->StartTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Play_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->NumPointsToInsert_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->backgroundImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openImageFileToolStripMenuItemToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->AddPI_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openPathFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->savePathFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveGamePathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAviFileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->mergeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->editModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addPointModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->fixPointModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moveWholePathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->moveAllPathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rotatePathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->rotateAllPathToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->assistantModeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showImageBoderToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showSelectedImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPath_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showErrorMsh_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPanelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showPIImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->flashSelectedImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cameraFitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showBGToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showGridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->hockToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->floatToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->childToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ApplyLODToPoints_button = (gcnew System::Windows::Forms::Button());
			this->AllCurveLoop_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->PathPiorityUp_button = (gcnew System::Windows::Forms::Button());
			this->PathPiorityDown_button = (gcnew System::Windows::Forms::Button());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->RearrangeWholePathTime_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->WholePuzzleImageUnit_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AssignImage_button = (gcnew System::Windows::Forms::Button());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->ShowSelectedImageBorder_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->DelPI_button = (gcnew System::Windows::Forms::Button());
			this->ImageAnchor_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->UseViewPort_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->AllPI_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ViewPort_xyzwNumericalControl = (gcnew DotNetComponent::XYZWNumericalControl());
			this->RotationPosOffset_xyzNumeriaclContro = (gcnew DotNetComponent::XYZNumeriaclControl());
			this->ApplyImageWithOriginalData_button = (gcnew System::Windows::Forms::Button());
			this->StayAtLastFrame_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->ColorBlending_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->EnableColorBlending_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SrcBlending_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->DestBlending_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->Rotation_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->AngleY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->AngleX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->AngleZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->BehaviorUpdateActive_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->InvertAllPath_button = (gcnew System::Windows::Forms::Button());
			this->SelectAllPoints_button = (gcnew System::Windows::Forms::Button());
			this->AverageAssign_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->SelectFirstAndLast_button = (gcnew System::Windows::Forms::Button());
			this->Size_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->SizeScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->Mirror_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Color_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->RedColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GreenColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->AlphaColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->BlueColor_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->Position_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->PathNodePosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNodePosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->PathNodePosZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->InvertPath_button = (gcnew System::Windows::Forms::Button());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->GroupData_collpaseForm = (gcnew System::Windows::Forms::Panel());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->GroupScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotation_label3 = (gcnew System::Windows::Forms::Label());
			this->CameraRotationY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupRotationX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupPosY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->GroupPosX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->GroupPosZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupImageRotate_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->ApplyGroupDataChange_button = (gcnew System::Windows::Forms::Button());
			this->GroupPositionMoveType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->GroupRotationZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->GroupRotationY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->GroupRotationType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->AnimationData_collpaseForm = (gcnew System::Windows::Forms::Panel());
			this->WholeAnimationList_listBox = (gcnew System::Windows::Forms::ListBox());
			this->WholeAnimationListUp_button = (gcnew System::Windows::Forms::Button());
			this->MergeMPDIData_button = (gcnew System::Windows::Forms::Button());
			this->WholeAnimationListDown_lbutton = (gcnew System::Windows::Forms::Button());
			this->AnimationName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DeleteListObject_button = (gcnew System::Windows::Forms::Button());
			this->AnimationInvert_button = (gcnew System::Windows::Forms::Button());
			this->AddAnimationToList_button = (gcnew System::Windows::Forms::Button());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->MPDIExtra_xmlNodeEditComponent = (gcnew DotNetComponent::XmlNodeEditComponent());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->flowLayoutPanel2 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			this->Path_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->AddPathIntoAllPath_button = (gcnew System::Windows::Forms::Button());
			this->AllPath_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelPathIntoAllPath_button = (gcnew System::Windows::Forms::Button());
			this->UsingIntoCurrentPath_button = (gcnew System::Windows::Forms::Button());
			this->ImageHintPoint_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->ShowImageOnScreen_listBox = (gcnew System::Windows::Forms::ListBox());
			this->DelShowImageOnScreenObject_button = (gcnew System::Windows::Forms::Button());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->AddShowImageOnScreenObject_button = (gcnew System::Windows::Forms::Button());
			this->HintPoint_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->AllEventPointHint_listBox = (gcnew System::Windows::Forms::ListBox());
			this->HintPointType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->AddEventPOint_button = (gcnew System::Windows::Forms::Button());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->EventPointName_textBox = (gcnew System::Windows::Forms::TextBox());
			this->DelEventPoint_button = (gcnew System::Windows::Forms::Button());
			this->Video_groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
			this->CameraRotationCenter_label = (gcnew System::Windows::Forms::Label());
			this->CameraRotationCenterY_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationCenterX_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->CameraRotationCenterZ_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TotalPlayTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathLOD_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodeTime_numericUpDown))->BeginInit();
			this->PointDataUtility_contextMenuStrip->SuspendLayout();
			this->GameUIData_menuStrip->SuspendLayout();
			this->PathGroup_contextMenuStrip->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentProgress_trackBar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartTime_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumPointsToInsert_numericUpDown))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->RearrangeWholePathTime_numericUpDown))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->ColorBlending_groupBox->SuspendLayout();
			this->Rotation_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AngleY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AngleX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AngleZ_numericUpDown))->BeginInit();
			this->Size_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SizeScale_numericUpDown))->BeginInit();
			this->Color_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->RedColor_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GreenColor_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AlphaColor_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BlueColor_numericUpDown))->BeginInit();
			this->Position_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosZ_numericUpDown))->BeginInit();
			this->splitContainer3->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->flowLayoutPanel1->SuspendLayout();
			this->GroupData_collpaseForm->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupScale_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupRotationX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupPosY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupPosX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupPosZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupRotationZ_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupRotationY_numericUpDown))->BeginInit();
			this->AnimationData_collpaseForm->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->flowLayoutPanel2->SuspendLayout();
			this->Path_groupBox->SuspendLayout();
			this->ImageHintPoint_groupBox->SuspendLayout();
			this->HintPoint_groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationCenterY_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationCenterX_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationCenterZ_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// DelPoint_button
			// 
			this->DelPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPoint_button->Location = System::Drawing::Point(1297, 180);
			this->DelPoint_button->Margin = System::Windows::Forms::Padding(4);
			this->DelPoint_button->Name = L"DelPoint_button";
			this->DelPoint_button->Size = System::Drawing::Size(81, 25);
			this->DelPoint_button->TabIndex = 78;
			this->DelPoint_button->Text = L"DelPoint/刪除";
			this->DelPoint_button->UseVisualStyleBackColor = false;
			this->DelPoint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::DelPoint_button_Click);
			// 
			// label8
			// 
			this->label8->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label8->Location = System::Drawing::Point(111, 29);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(141, 15);
			this->label8->TabIndex = 76;
			this->label8->Text = L"TotalPlayTime/總時間";
			// 
			// TotalPlayTime_numericUpDown
			// 
			this->TotalPlayTime_numericUpDown->Location = System::Drawing::Point(8, 26);
			this->TotalPlayTime_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->TotalPlayTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999999, 0, 0, 0});
			this->TotalPlayTime_numericUpDown->Name = L"TotalPlayTime_numericUpDown";
			this->TotalPlayTime_numericUpDown->Size = System::Drawing::Size(93, 25);
			this->TotalPlayTime_numericUpDown->TabIndex = 75;
			this->TotalPlayTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::TotalPlayTime_numericUpDown_ValueChanged);
			// 
			// Loop_checkBox
			// 
			this->Loop_checkBox->AutoSize = true;
			this->Loop_checkBox->Enabled = false;
			this->Loop_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->Loop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Loop_checkBox->Location = System::Drawing::Point(216, 228);
			this->Loop_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->Loop_checkBox->Name = L"Loop_checkBox";
			this->Loop_checkBox->Size = System::Drawing::Size(208, 18);
			this->Loop_checkBox->TabIndex = 73;
			this->Loop_checkBox->Text = L"SingleCurveLoop/單一路徑loop";
			this->Loop_checkBox->UseVisualStyleBackColor = true;
			this->Loop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
			// 
			// ErrorMsg_label
			// 
			this->ErrorMsg_label->AutoSize = true;
			this->ErrorMsg_label->Location = System::Drawing::Point(1752, 174);
			this->ErrorMsg_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->ErrorMsg_label->Name = L"ErrorMsg_label";
			this->ErrorMsg_label->Size = System::Drawing::Size(32, 15);
			this->ErrorMsg_label->TabIndex = 72;
			this->ErrorMsg_label->Text = L"Msg";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::White;
			this->label4->Location = System::Drawing::Point(109, 159);
			this->label4->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(110, 15);
			this->label4->TabIndex = 71;
			this->label4->Text = L"PathLOD/精細度";
			// 
			// PathLOD_numericUpDown
			// 
			this->PathLOD_numericUpDown->Location = System::Drawing::Point(8, 150);
			this->PathLOD_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->PathLOD_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {7, 0, 0, 0});
			this->PathLOD_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->PathLOD_numericUpDown->Name = L"PathLOD_numericUpDown";
			this->PathLOD_numericUpDown->Size = System::Drawing::Size(92, 25);
			this->PathLOD_numericUpDown->TabIndex = 70;
			this->PathLOD_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->PathLOD_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::PathLOD_numericUpDown_ValueChanged);
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label1->Location = System::Drawing::Point(111, 89);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(134, 15);
			this->label1->TabIndex = 67;
			this->label1->Text = L"perTime/單獨點時間";
			// 
			// PathNodeTime_numericUpDown
			// 
			this->PathNodeTime_numericUpDown->Location = System::Drawing::Point(8, 89);
			this->PathNodeTime_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->PathNodeTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000000, 0, 0, 0});
			this->PathNodeTime_numericUpDown->Name = L"PathNodeTime_numericUpDown";
			this->PathNodeTime_numericUpDown->Size = System::Drawing::Size(91, 25);
			this->PathNodeTime_numericUpDown->TabIndex = 64;
			this->PathNodeTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			// 
			// PathNode_listBox
			// 
			this->PathNode_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNode_listBox->ContextMenuStrip = this->PointDataUtility_contextMenuStrip;
			this->PathNode_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNode_listBox->FormattingEnabled = true;
			this->PathNode_listBox->HorizontalScrollbar = true;
			this->PathNode_listBox->ItemHeight = 15;
			this->PathNode_listBox->Location = System::Drawing::Point(1156, 9);
			this->PathNode_listBox->Margin = System::Windows::Forms::Padding(4);
			this->PathNode_listBox->MinimumSize = System::Drawing::Size(4, 115);
			this->PathNode_listBox->Name = L"PathNode_listBox";
			this->PathNode_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->PathNode_listBox->Size = System::Drawing::Size(319, 169);
			this->PathNode_listBox->TabIndex = 16;
			this->PathNode_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholePath_listBox_SelectedIndexChanged);
			this->PathNode_listBox->MouseEnter += gcnew System::EventHandler(this, &MPDIEditor::PathNode_listBox_MouseEnter);
			this->PathNode_listBox->MouseLeave += gcnew System::EventHandler(this, &MPDIEditor::PathNode_listBox_MouseLeave);
			this->PathNode_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// PointDataUtility_contextMenuStrip
			// 
			this->PointDataUtility_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->FadeIn_toolStripMenuItem, 
				this->FadeOut_toolStripMenuItem});
			this->PointDataUtility_contextMenuStrip->Name = L"PointDataUtility_contextMenuStrip";
			this->PointDataUtility_contextMenuStrip->Size = System::Drawing::Size(123, 48);
			// 
			// FadeIn_toolStripMenuItem
			// 
			this->FadeIn_toolStripMenuItem->Name = L"FadeIn_toolStripMenuItem";
			this->FadeIn_toolStripMenuItem->Size = System::Drawing::Size(122, 22);
			this->FadeIn_toolStripMenuItem->Text = L"FadeIn";
			this->FadeIn_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::FadeIn_toolStripMenuItem_Click);
			// 
			// FadeOut_toolStripMenuItem
			// 
			this->FadeOut_toolStripMenuItem->Name = L"FadeOut_toolStripMenuItem";
			this->FadeOut_toolStripMenuItem->Size = System::Drawing::Size(122, 22);
			this->FadeOut_toolStripMenuItem->Text = L"FadeOut";
			this->FadeOut_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::FadeIn_toolStripMenuItem_Click);
			// 
			// DelPath_button
			// 
			this->DelPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->DelPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPath_button->Location = System::Drawing::Point(356, 10);
			this->DelPath_button->Margin = System::Windows::Forms::Padding(4);
			this->DelPath_button->Name = L"DelPath_button";
			this->DelPath_button->Size = System::Drawing::Size(100, 24);
			this->DelPath_button->TabIndex = 62;
			this->DelPath_button->Text = L"DelPath/刪除路徑";
			this->DelPath_button->UseVisualStyleBackColor = false;
			this->DelPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
			// 
			// WholePath_listBox
			// 
			this->WholePath_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholePath_listBox->ContextMenuStrip = this->GameUIData_menuStrip;
			this->WholePath_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholePath_listBox->FormattingEnabled = true;
			this->WholePath_listBox->HorizontalScrollbar = true;
			this->WholePath_listBox->ItemHeight = 15;
			this->WholePath_listBox->Location = System::Drawing::Point(0, 0);
			this->WholePath_listBox->Margin = System::Windows::Forms::Padding(4);
			this->WholePath_listBox->MinimumSize = System::Drawing::Size(4, 90);
			this->WholePath_listBox->Name = L"WholePath_listBox";
			this->WholePath_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->WholePath_listBox->Size = System::Drawing::Size(207, 304);
			this->WholePath_listBox->TabIndex = 60;
			this->WholePath_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholePath_listBox_SelectedIndexChanged);
			this->WholePath_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// GameUIData_menuStrip
			// 
			this->GameUIData_menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {this->AddSliderBarUI_toolStripMenuItem, 
				this->AddMultiPageUI_toolStripMenuItem, this->addSliderBarWithTabPagesUIToolStripMenuItem, this->addMultipageWithTabPagesUIToolStripMenuItem, 
				this->yesNoDialogToolStripMenuItem});
			this->GameUIData_menuStrip->Name = L"GameUIData_menuStrip";
			this->GameUIData_menuStrip->Size = System::Drawing::Size(270, 114);
			// 
			// AddSliderBarUI_toolStripMenuItem
			// 
			this->AddSliderBarUI_toolStripMenuItem->Name = L"AddSliderBarUI_toolStripMenuItem";
			this->AddSliderBarUI_toolStripMenuItem->Size = System::Drawing::Size(269, 22);
			this->AddSliderBarUI_toolStripMenuItem->Text = L"Add Slider Bar UI";
			this->AddSliderBarUI_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
			// 
			// AddMultiPageUI_toolStripMenuItem
			// 
			this->AddMultiPageUI_toolStripMenuItem->Name = L"AddMultiPageUI_toolStripMenuItem";
			this->AddMultiPageUI_toolStripMenuItem->Size = System::Drawing::Size(269, 22);
			this->AddMultiPageUI_toolStripMenuItem->Text = L"Add Multi Page UI";
			this->AddMultiPageUI_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
			// 
			// addSliderBarWithTabPagesUIToolStripMenuItem
			// 
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Name = L"addSliderBarWithTabPagesUIToolStripMenuItem";
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Size = System::Drawing::Size(269, 22);
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Text = L"Add Slider Bar With TabPages UI";
			this->addSliderBarWithTabPagesUIToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
			// 
			// addMultipageWithTabPagesUIToolStripMenuItem
			// 
			this->addMultipageWithTabPagesUIToolStripMenuItem->Name = L"addMultipageWithTabPagesUIToolStripMenuItem";
			this->addMultipageWithTabPagesUIToolStripMenuItem->Size = System::Drawing::Size(269, 22);
			this->addMultipageWithTabPagesUIToolStripMenuItem->Text = L"Add Multipage With TabPages UI";
			this->addMultipageWithTabPagesUIToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
			// 
			// yesNoDialogToolStripMenuItem
			// 
			this->yesNoDialogToolStripMenuItem->Name = L"yesNoDialogToolStripMenuItem";
			this->yesNoDialogToolStripMenuItem->Size = System::Drawing::Size(269, 22);
			this->yesNoDialogToolStripMenuItem->Text = L"YesNoDialog";
			this->yesNoDialogToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::AddSliderBarUI_toolStripMenuItem_Click);
			// 
			// AddPath_button
			// 
			this->AddPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddPath_button->Enabled = false;
			this->AddPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->AddPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddPath_button->Location = System::Drawing::Point(249, 10);
			this->AddPath_button->Margin = System::Windows::Forms::Padding(4);
			this->AddPath_button->Name = L"AddPath_button";
			this->AddPath_button->Size = System::Drawing::Size(75, 25);
			this->AddPath_button->TabIndex = 61;
			this->AddPath_button->Text = L"AddPath/增加路徑";
			this->AddPath_button->UseVisualStyleBackColor = false;
			this->AddPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
			// 
			// PathGroup_contextMenuStrip
			// 
			this->PathGroup_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripMenuItem1, 
				this->addPointDataWithImageByPathNameToolStripMenuItem});
			this->PathGroup_contextMenuStrip->Name = L"PathGroup_contextMenuStrip";
			this->PathGroup_contextMenuStrip->Size = System::Drawing::Size(292, 48);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(291, 22);
			this->toolStripMenuItem1->Text = L"Add Group Path";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &MPDIEditor::toolStripMenuItem1_Click);
			// 
			// addPointDataWithImageByPathNameToolStripMenuItem
			// 
			this->addPointDataWithImageByPathNameToolStripMenuItem->Name = L"addPointDataWithImageByPathNameToolStripMenuItem";
			this->addPointDataWithImageByPathNameToolStripMenuItem->Size = System::Drawing::Size(291, 22);
			this->addPointDataWithImageByPathNameToolStripMenuItem->Text = L"AddPointDataWithImageByPathName";
			this->addPointDataWithImageByPathNameToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointDataWithImageByPathNameToolStripMenuItem_Click);
			// 
			// AllPlay_checkBox
			// 
			this->AllPlay_checkBox->AutoSize = true;
			this->AllPlay_checkBox->Enabled = false;
			this->AllPlay_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->AllPlay_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPlay_checkBox->Location = System::Drawing::Point(380, 228);
			this->AllPlay_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->AllPlay_checkBox->Name = L"AllPlay_checkBox";
			this->AllPlay_checkBox->Size = System::Drawing::Size(127, 18);
			this->AllPlay_checkBox->TabIndex = 93;
			this->AllPlay_checkBox->Text = L"AllPlay/全部撥放";
			this->AllPlay_checkBox->UseVisualStyleBackColor = true;
			this->AllPlay_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
			// 
			// CurrentProgress_trackBar
			// 
			this->CurrentProgress_trackBar->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CurrentProgress_trackBar->Location = System::Drawing::Point(1151, 246);
			this->CurrentProgress_trackBar->Margin = System::Windows::Forms::Padding(0);
			this->CurrentProgress_trackBar->Name = L"CurrentProgress_trackBar";
			this->CurrentProgress_trackBar->Size = System::Drawing::Size(325, 53);
			this->CurrentProgress_trackBar->TabIndex = 94;
			this->CurrentProgress_trackBar->Scroll += gcnew System::EventHandler(this, &MPDIEditor::CurrentProgress_trackBar_Scroll);
			this->CurrentProgress_trackBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MPDIEditor::CurrentProgress_trackBar_MouseDown);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->ForeColor = System::Drawing::Color::White;
			this->label9->Location = System::Drawing::Point(1275, 235);
			this->label9->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(114, 15);
			this->label9->TabIndex = 95;
			this->label9->Text = L"Progess/目前進度";
			// 
			// ShowAllPath_checkBox
			// 
			this->ShowAllPath_checkBox->AutoSize = true;
			this->ShowAllPath_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->ShowAllPath_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ShowAllPath_checkBox->Location = System::Drawing::Point(216, 250);
			this->ShowAllPath_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->ShowAllPath_checkBox->Name = L"ShowAllPath_checkBox";
			this->ShowAllPath_checkBox->Size = System::Drawing::Size(185, 18);
			this->ShowAllPath_checkBox->TabIndex = 96;
			this->ShowAllPath_checkBox->Text = L"ShowAllPath/顯示所有路徑";
			this->ShowAllPath_checkBox->UseVisualStyleBackColor = true;
			// 
			// ClonePath_button
			// 
			this->ClonePath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ClonePath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->ClonePath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ClonePath_button->Location = System::Drawing::Point(249, 50);
			this->ClonePath_button->Margin = System::Windows::Forms::Padding(4);
			this->ClonePath_button->Name = L"ClonePath_button";
			this->ClonePath_button->Size = System::Drawing::Size(75, 25);
			this->ClonePath_button->TabIndex = 97;
			this->ClonePath_button->Text = L"ClonePath/拷貝路徑";
			this->ClonePath_button->UseVisualStyleBackColor = false;
			this->ClonePath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &MPDIEditor::timer1_Tick);
			// 
			// InsertPoint_button
			// 
			this->InsertPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InsertPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InsertPoint_button->Location = System::Drawing::Point(1384, 180);
			this->InsertPoint_button->Margin = System::Windows::Forms::Padding(4);
			this->InsertPoint_button->Name = L"InsertPoint_button";
			this->InsertPoint_button->Size = System::Drawing::Size(92, 25);
			this->InsertPoint_button->TabIndex = 107;
			this->InsertPoint_button->Text = L"InsertPoint/插入";
			this->InsertPoint_button->UseVisualStyleBackColor = false;
			this->InsertPoint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::DelPoint_button_Click);
			// 
			// label14
			// 
			this->label14->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label14->AutoSize = true;
			this->label14->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label14->Location = System::Drawing::Point(112, 61);
			this->label14->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(153, 15);
			this->label14->TabIndex = 110;
			this->label14->Text = L"PathStartTime/開始時間";
			// 
			// StartTime_numericUpDown
			// 
			this->StartTime_numericUpDown->Location = System::Drawing::Point(8, 58);
			this->StartTime_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->StartTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999999, 0, 0, 0});
			this->StartTime_numericUpDown->Name = L"StartTime_numericUpDown";
			this->StartTime_numericUpDown->Size = System::Drawing::Size(93, 25);
			this->StartTime_numericUpDown->TabIndex = 109;
			this->StartTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			// 
			// Play_checkBox
			// 
			this->Play_checkBox->AutoSize = true;
			this->Play_checkBox->Enabled = false;
			this->Play_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->Play_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Play_checkBox->Location = System::Drawing::Point(216, 202);
			this->Play_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->Play_checkBox->Name = L"Play_checkBox";
			this->Play_checkBox->Size = System::Drawing::Size(145, 18);
			this->Play_checkBox->TabIndex = 111;
			this->Play_checkBox->Text = L"SinglePlay/單一撥放";
			this->Play_checkBox->UseVisualStyleBackColor = true;
			this->Play_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
			// 
			// NumPointsToInsert_numericUpDown
			// 
			this->NumPointsToInsert_numericUpDown->Location = System::Drawing::Point(1391, 208);
			this->NumPointsToInsert_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->NumPointsToInsert_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000, 0, 0, 0});
			this->NumPointsToInsert_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->NumPointsToInsert_numericUpDown->Name = L"NumPointsToInsert_numericUpDown";
			this->NumPointsToInsert_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->NumPointsToInsert_numericUpDown->TabIndex = 118;
			this->NumPointsToInsert_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->backgroundImageToolStripMenuItem, 
				this->editModeToolStripMenuItem, this->assistantModeToolStripMenuItem, this->hockToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(2560, 24);
			this->menuStrip1->TabIndex = 120;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// backgroundImageToolStripMenuItem
			// 
			this->backgroundImageToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(11) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem, this->openImageFileToolStripMenuItemToolStripMenuItem, this->AddPI_toolStripMenuItem, this->toolStripMenuItem2, 
				this->openPathFileToolStripMenuItem, this->savePathFileToolStripMenuItem, this->saveGamePathToolStripMenuItem, this->saveAviFileToolStripMenuItem, 
				this->saveImageToolStripMenuItem, this->mergeToolStripMenuItem});
			this->backgroundImageToolStripMenuItem->Name = L"backgroundImageToolStripMenuItem";
			this->backgroundImageToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->backgroundImageToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openToolStripMenuItem_Click);
			// 
			// openImageFileToolStripMenuItemToolStripMenuItem
			// 
			this->openImageFileToolStripMenuItemToolStripMenuItem->Name = L"openImageFileToolStripMenuItemToolStripMenuItem";
			this->openImageFileToolStripMenuItemToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->openImageFileToolStripMenuItemToolStripMenuItem->Text = L"Open PI/打開PI檔";
			this->openImageFileToolStripMenuItemToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::OpenImageFile_button_Click);
			// 
			// AddPI_toolStripMenuItem
			// 
			this->AddPI_toolStripMenuItem->Name = L"AddPI_toolStripMenuItem";
			this->AddPI_toolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->AddPI_toolStripMenuItem->Text = L"AddPI/增加PI檔";
			this->AddPI_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::OpenImageFile_button_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(253, 22);
			this->toolStripMenuItem2->Text = L"BackgroundImage/背景圖片";
			this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &MPDIEditor::BKImage_button_Click);
			// 
			// openPathFileToolStripMenuItem
			// 
			this->openPathFileToolStripMenuItem->Name = L"openPathFileToolStripMenuItem";
			this->openPathFileToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->openPathFileToolStripMenuItem->Text = L"OpenPathFile";
			this->openPathFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openPathFileToolStripMenuItem_Click);
			// 
			// savePathFileToolStripMenuItem
			// 
			this->savePathFileToolStripMenuItem->Name = L"savePathFileToolStripMenuItem";
			this->savePathFileToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->savePathFileToolStripMenuItem->Text = L"SavePathFile";
			this->savePathFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openPathFileToolStripMenuItem_Click);
			// 
			// saveGamePathToolStripMenuItem
			// 
			this->saveGamePathToolStripMenuItem->Name = L"saveGamePathToolStripMenuItem";
			this->saveGamePathToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->saveGamePathToolStripMenuItem->Text = L"SaveGamePath/儲存遊戲路徑";
			this->saveGamePathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::openPathFileToolStripMenuItem_Click);
			// 
			// saveAviFileToolStripMenuItem
			// 
			this->saveAviFileToolStripMenuItem->Name = L"saveAviFileToolStripMenuItem";
			this->saveAviFileToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->saveAviFileToolStripMenuItem->Text = L"SaveAviFile";
			this->saveAviFileToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::saveAviFileToolStripMenuItem_Click);
			// 
			// saveImageToolStripMenuItem
			// 
			this->saveImageToolStripMenuItem->Name = L"saveImageToolStripMenuItem";
			this->saveImageToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->saveImageToolStripMenuItem->Text = L"SaveImage";
			this->saveImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::saveImageToolStripMenuItem_Click);
			// 
			// mergeToolStripMenuItem
			// 
			this->mergeToolStripMenuItem->Name = L"mergeToolStripMenuItem";
			this->mergeToolStripMenuItem->Size = System::Drawing::Size(253, 22);
			this->mergeToolStripMenuItem->Text = L"Merge";
			this->mergeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::mergeToolStripMenuItem_Click);
			// 
			// editModeToolStripMenuItem
			// 
			this->editModeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->addPointModeToolStripMenuItem, 
				this->fixPointModeToolStripMenuItem, this->moveWholePathToolStripMenuItem, this->moveAllPathToolStripMenuItem, this->rotatePathToolStripMenuItem, 
				this->rotateAllPathToolStripMenuItem});
			this->editModeToolStripMenuItem->Name = L"editModeToolStripMenuItem";
			this->editModeToolStripMenuItem->Size = System::Drawing::Size(140, 20);
			this->editModeToolStripMenuItem->Text = L"EditMode/編輯模式";
			// 
			// addPointModeToolStripMenuItem
			// 
			this->addPointModeToolStripMenuItem->Checked = true;
			this->addPointModeToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->addPointModeToolStripMenuItem->Name = L"addPointModeToolStripMenuItem";
			this->addPointModeToolStripMenuItem->Size = System::Drawing::Size(263, 22);
			this->addPointModeToolStripMenuItem->Text = L"AddPointMode/增加";
			this->addPointModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// fixPointModeToolStripMenuItem
			// 
			this->fixPointModeToolStripMenuItem->Name = L"fixPointModeToolStripMenuItem";
			this->fixPointModeToolStripMenuItem->Size = System::Drawing::Size(263, 22);
			this->fixPointModeToolStripMenuItem->Text = L"FixPointMode/修改";
			this->fixPointModeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// moveWholePathToolStripMenuItem
			// 
			this->moveWholePathToolStripMenuItem->Name = L"moveWholePathToolStripMenuItem";
			this->moveWholePathToolStripMenuItem->Size = System::Drawing::Size(263, 22);
			this->moveWholePathToolStripMenuItem->Text = L"MoveWholePath/移動整條路徑";
			this->moveWholePathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// moveAllPathToolStripMenuItem
			// 
			this->moveAllPathToolStripMenuItem->Name = L"moveAllPathToolStripMenuItem";
			this->moveAllPathToolStripMenuItem->Size = System::Drawing::Size(263, 22);
			this->moveAllPathToolStripMenuItem->Text = L"MoveAllPath/移動所有路徑";
			this->moveAllPathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// rotatePathToolStripMenuItem
			// 
			this->rotatePathToolStripMenuItem->Name = L"rotatePathToolStripMenuItem";
			this->rotatePathToolStripMenuItem->Size = System::Drawing::Size(263, 22);
			this->rotatePathToolStripMenuItem->Text = L"RotatePath/旋轉路徑";
			this->rotatePathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// rotateAllPathToolStripMenuItem
			// 
			this->rotateAllPathToolStripMenuItem->Name = L"rotateAllPathToolStripMenuItem";
			this->rotateAllPathToolStripMenuItem->Size = System::Drawing::Size(263, 22);
			this->rotateAllPathToolStripMenuItem->Text = L"RotateAllPath/旋轉所有路徑";
			this->rotateAllPathToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::addPointModeToolStripMenuItem_Click);
			// 
			// assistantModeToolStripMenuItem
			// 
			this->assistantModeToolStripMenuItem->Checked = true;
			this->assistantModeToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->assistantModeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(12) {this->showImageBoderToolStripMenuItem, 
				this->showSelectedImageToolStripMenuItem, this->showPath_ToolStripMenuItem, this->showErrorMsh_ToolStripMenuItem, this->showPanelToolStripMenuItem, 
				this->showPIImageToolStripMenuItem, this->ScaleWithPIAlphaStripPixel_toolStripMenuItem, this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem, 
				this->flashSelectedImageToolStripMenuItem, this->cameraFitToolStripMenuItem, this->showBGToolStripMenuItem, this->showGridToolStripMenuItem});
			this->assistantModeToolStripMenuItem->Name = L"assistantModeToolStripMenuItem";
			this->assistantModeToolStripMenuItem->Size = System::Drawing::Size(166, 20);
			this->assistantModeToolStripMenuItem->Text = L"AssistantMode/輔助模式";
			// 
			// showImageBoderToolStripMenuItem
			// 
			this->showImageBoderToolStripMenuItem->Name = L"showImageBoderToolStripMenuItem";
			this->showImageBoderToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showImageBoderToolStripMenuItem->Text = L"ShowImageBorder/顯示圖片邊框";
			this->showImageBoderToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// showSelectedImageToolStripMenuItem
			// 
			this->showSelectedImageToolStripMenuItem->Name = L"showSelectedImageToolStripMenuItem";
			this->showSelectedImageToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showSelectedImageToolStripMenuItem->Text = L"ShowSelectedImage/顯示選擇圖片";
			this->showSelectedImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// showPath_ToolStripMenuItem
			// 
			this->showPath_ToolStripMenuItem->Checked = true;
			this->showPath_ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showPath_ToolStripMenuItem->Name = L"showPath_ToolStripMenuItem";
			this->showPath_ToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showPath_ToolStripMenuItem->Text = L"ShowPath/顯示線段";
			this->showPath_ToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// showErrorMsh_ToolStripMenuItem
			// 
			this->showErrorMsh_ToolStripMenuItem->Checked = true;
			this->showErrorMsh_ToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showErrorMsh_ToolStripMenuItem->Name = L"showErrorMsh_ToolStripMenuItem";
			this->showErrorMsh_ToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showErrorMsh_ToolStripMenuItem->Text = L"ShowErrorMsh/顯示錯誤訊息";
			// 
			// showPanelToolStripMenuItem
			// 
			this->showPanelToolStripMenuItem->Name = L"showPanelToolStripMenuItem";
			this->showPanelToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showPanelToolStripMenuItem->Text = L"ShowPanel/顯示面板";
			this->showPanelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// showPIImageToolStripMenuItem
			// 
			this->showPIImageToolStripMenuItem->Checked = true;
			this->showPIImageToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showPIImageToolStripMenuItem->Name = L"showPIImageToolStripMenuItem";
			this->showPIImageToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showPIImageToolStripMenuItem->Text = L"Show PI Image/顯示pi圖片";
			this->showPIImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// ScaleWithPIAlphaStripPixel_toolStripMenuItem
			// 
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Name = L"ScaleWithPIAlphaStripPixel_toolStripMenuItem";
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Text = L"ScaleWithPIAlphaStripPixelProble/縮放pi檔案中去除的pizel問題";
			this->ScaleWithPIAlphaStripPixel_toolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::ScaleWithPIAlphaStripPixel_toolStripMenuItem_Click);
			// 
			// renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem
			// 
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Name = L"renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem";
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Text = L"RenewImageSize(ignore all animation with size)";
			this->renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem_Click);
			// 
			// flashSelectedImageToolStripMenuItem
			// 
			this->flashSelectedImageToolStripMenuItem->Name = L"flashSelectedImageToolStripMenuItem";
			this->flashSelectedImageToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->flashSelectedImageToolStripMenuItem->Text = L"FlashSelectedImage";
			this->flashSelectedImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// cameraFitToolStripMenuItem
			// 
			this->cameraFitToolStripMenuItem->Checked = true;
			this->cameraFitToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cameraFitToolStripMenuItem->Name = L"cameraFitToolStripMenuItem";
			this->cameraFitToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->cameraFitToolStripMenuItem->Text = L"CameraFit";
			this->cameraFitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showImageBoderToolStripMenuItem_Click);
			// 
			// showBGToolStripMenuItem
			// 
			this->showBGToolStripMenuItem->Checked = true;
			this->showBGToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showBGToolStripMenuItem->Name = L"showBGToolStripMenuItem";
			this->showBGToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showBGToolStripMenuItem->Text = L"ShowBG/顯示背景圖片";
			this->showBGToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// showGridToolStripMenuItem
			// 
			this->showGridToolStripMenuItem->Checked = true;
			this->showGridToolStripMenuItem->CheckState = System::Windows::Forms::CheckState::Checked;
			this->showGridToolStripMenuItem->Name = L"showGridToolStripMenuItem";
			this->showGridToolStripMenuItem->Size = System::Drawing::Size(465, 22);
			this->showGridToolStripMenuItem->Text = L"ShowGrid";
			this->showGridToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::showPath_ToolStripMenuItem_Click);
			// 
			// hockToolStripMenuItem
			// 
			this->hockToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->floatToolStripMenuItem, 
				this->childToolStripMenuItem});
			this->hockToolStripMenuItem->Name = L"hockToolStripMenuItem";
			this->hockToolStripMenuItem->Size = System::Drawing::Size(49, 20);
			this->hockToolStripMenuItem->Text = L"Dock";
			// 
			// floatToolStripMenuItem
			// 
			this->floatToolStripMenuItem->Name = L"floatToolStripMenuItem";
			this->floatToolStripMenuItem->Size = System::Drawing::Size(105, 22);
			this->floatToolStripMenuItem->Text = L"Float";
			this->floatToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::floatToolStripMenuItem_Click);
			// 
			// childToolStripMenuItem
			// 
			this->childToolStripMenuItem->Name = L"childToolStripMenuItem";
			this->childToolStripMenuItem->Size = System::Drawing::Size(105, 22);
			this->childToolStripMenuItem->Text = L"Child";
			this->childToolStripMenuItem->Click += gcnew System::EventHandler(this, &MPDIEditor::floatToolStripMenuItem_Click);
			// 
			// ApplyLODToPoints_button
			// 
			this->ApplyLODToPoints_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ApplyLODToPoints_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ApplyLODToPoints_button->Location = System::Drawing::Point(216, 171);
			this->ApplyLODToPoints_button->Margin = System::Windows::Forms::Padding(4);
			this->ApplyLODToPoints_button->Name = L"ApplyLODToPoints_button";
			this->ApplyLODToPoints_button->Size = System::Drawing::Size(245, 24);
			this->ApplyLODToPoints_button->TabIndex = 128;
			this->ApplyLODToPoints_button->Text = L"ApplyLODToPoints/LOD轉成可視點";
			this->ApplyLODToPoints_button->UseVisualStyleBackColor = false;
			this->ApplyLODToPoints_button->Click += gcnew System::EventHandler(this, &MPDIEditor::ApplyLODToPoints_button_Click);
			// 
			// AllCurveLoop_checkBox
			// 
			this->AllCurveLoop_checkBox->AutoSize = true;
			this->AllCurveLoop_checkBox->Enabled = false;
			this->AllCurveLoop_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->AllCurveLoop_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllCurveLoop_checkBox->Location = System::Drawing::Point(380, 202);
			this->AllCurveLoop_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->AllCurveLoop_checkBox->Name = L"AllCurveLoop_checkBox";
			this->AllCurveLoop_checkBox->Size = System::Drawing::Size(190, 18);
			this->AllCurveLoop_checkBox->TabIndex = 134;
			this->AllCurveLoop_checkBox->Text = L"AllCurveLoop/全部路徑loop";
			this->AllCurveLoop_checkBox->UseVisualStyleBackColor = true;
			this->AllCurveLoop_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
			// 
			// PathPiorityUp_button
			// 
			this->PathPiorityUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathPiorityUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathPiorityUp_button->Location = System::Drawing::Point(212, 9);
			this->PathPiorityUp_button->Margin = System::Windows::Forms::Padding(4);
			this->PathPiorityUp_button->Name = L"PathPiorityUp_button";
			this->PathPiorityUp_button->Size = System::Drawing::Size(29, 49);
			this->PathPiorityUp_button->TabIndex = 137;
			this->PathPiorityUp_button->Text = L"^\r\n^";
			this->PathPiorityUp_button->UseVisualStyleBackColor = false;
			this->PathPiorityUp_button->Click += gcnew System::EventHandler(this, &MPDIEditor::PathPiorityUp_button_Click);
			// 
			// PathPiorityDown_button
			// 
			this->PathPiorityDown_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathPiorityDown_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathPiorityDown_button->Location = System::Drawing::Point(212, 90);
			this->PathPiorityDown_button->Margin = System::Windows::Forms::Padding(4);
			this->PathPiorityDown_button->Name = L"PathPiorityDown_button";
			this->PathPiorityDown_button->Size = System::Drawing::Size(29, 40);
			this->PathPiorityDown_button->TabIndex = 136;
			this->PathPiorityDown_button->Text = L"v\r\nv";
			this->PathPiorityDown_button->UseVisualStyleBackColor = false;
			this->PathPiorityDown_button->Click += gcnew System::EventHandler(this, &MPDIEditor::PathPiorityUp_button_Click);
			// 
			// label20
			// 
			this->label20->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label20->AutoSize = true;
			this->label20->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label20->Location = System::Drawing::Point(109, 121);
			this->label20->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(248, 15);
			this->label20->TabIndex = 136;
			this->label20->Text = L"NewTimeForWholPath/設定整體新時間";
			this->label20->TextAlign = System::Drawing::ContentAlignment::BottomRight;
			// 
			// RearrangeWholePathTime_numericUpDown
			// 
			this->RearrangeWholePathTime_numericUpDown->Location = System::Drawing::Point(8, 119);
			this->RearrangeWholePathTime_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->RearrangeWholePathTime_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {999999999, 0, 
				0, 0});
			this->RearrangeWholePathTime_numericUpDown->Name = L"RearrangeWholePathTime_numericUpDown";
			this->RearrangeWholePathTime_numericUpDown->Size = System::Drawing::Size(93, 25);
			this->RearrangeWholePathTime_numericUpDown->TabIndex = 135;
			this->RearrangeWholePathTime_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::RearrangeWholePathTime_numericUpDown_ValueChanged);
			// 
			// splitContainer1
			// 
			this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Margin = System::Windows::Forms::Padding(0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->AutoScroll = true;
			this->splitContainer1->Panel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->splitContainer1->Panel1->Controls->Add(this->groupBox1);
			this->splitContainer1->Panel1->Controls->Add(this->WholePuzzleImageUnit_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->AssignImage_button);
			this->splitContainer1->Panel1->Controls->Add(this->label25);
			this->splitContainer1->Panel1->Controls->Add(this->ShowSelectedImageBorder_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->DelPI_button);
			this->splitContainer1->Panel1->Controls->Add(this->ImageAnchor_comboBox);
			this->splitContainer1->Panel1->Controls->Add(this->UseViewPort_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->AllPI_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->ViewPort_xyzwNumericalControl);
			this->splitContainer1->Panel1->Controls->Add(this->RotationPosOffset_xyzNumeriaclContro);
			this->splitContainer1->Panel1->Controls->Add(this->ApplyImageWithOriginalData_button);
			this->splitContainer1->Panel1->Controls->Add(this->StayAtLastFrame_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label22);
			this->splitContainer1->Panel1->Controls->Add(this->Play_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label9);
			this->splitContainer1->Panel1->Controls->Add(this->AllCurveLoop_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->CurrentProgress_trackBar);
			this->splitContainer1->Panel1->Controls->Add(this->AllPlay_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->ErrorMsg_label);
			this->splitContainer1->Panel1->Controls->Add(this->ColorBlending_groupBox);
			this->splitContainer1->Panel1->Controls->Add(this->Loop_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->Rotation_groupBox);
			this->splitContainer1->Panel1->Controls->Add(this->ApplyLODToPoints_button);
			this->splitContainer1->Panel1->Controls->Add(this->BehaviorUpdateActive_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->InvertAllPath_button);
			this->splitContainer1->Panel1->Controls->Add(this->SelectAllPoints_button);
			this->splitContainer1->Panel1->Controls->Add(this->ShowAllPath_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->AverageAssign_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->SelectFirstAndLast_button);
			this->splitContainer1->Panel1->Controls->Add(this->Size_groupBox);
			this->splitContainer1->Panel1->Controls->Add(this->Color_groupBox);
			this->splitContainer1->Panel1->Controls->Add(this->Position_groupBox);
			this->splitContainer1->Panel1->Controls->Add(this->InvertPath_button);
			this->splitContainer1->Panel1->Controls->Add(this->WholePath_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->ClonePath_button);
			this->splitContainer1->Panel1->Controls->Add(this->PathNode_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->DelPath_button);
			this->splitContainer1->Panel1->Controls->Add(this->AddPath_button);
			this->splitContainer1->Panel1->Controls->Add(this->DelPoint_button);
			this->splitContainer1->Panel1->Controls->Add(this->PathPiorityUp_button);
			this->splitContainer1->Panel1->Controls->Add(this->PathPiorityDown_button);
			this->splitContainer1->Panel1->Controls->Add(this->InsertPoint_button);
			this->splitContainer1->Panel1->Controls->Add(this->NumPointsToInsert_numericUpDown);
			this->splitContainer1->Panel1->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer3);
			this->splitContainer1->Size = System::Drawing::Size(2171, 1308);
			this->splitContainer1->SplitterDistance = 350;
			this->splitContainer1->TabIndex = 145;
			// 
			// groupBox1
			// 
			this->groupBox1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->groupBox1->Controls->Add(this->TotalPlayTime_numericUpDown);
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->label20);
			this->groupBox1->Controls->Add(this->RearrangeWholePathTime_numericUpDown);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->PathNodeTime_numericUpDown);
			this->groupBox1->Controls->Add(this->StartTime_numericUpDown);
			this->groupBox1->Controls->Add(this->PathLOD_numericUpDown);
			this->groupBox1->Controls->Add(this->label14);
			this->groupBox1->Location = System::Drawing::Point(469, 2);
			this->groupBox1->Margin = System::Windows::Forms::Padding(4);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Padding = System::Windows::Forms::Padding(4);
			this->groupBox1->Size = System::Drawing::Size(245, 192);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Path";
			// 
			// WholePuzzleImageUnit_listBox
			// 
			this->WholePuzzleImageUnit_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholePuzzleImageUnit_listBox->ContextMenuStrip = this->PathGroup_contextMenuStrip;
			this->WholePuzzleImageUnit_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholePuzzleImageUnit_listBox->FormattingEnabled = true;
			this->WholePuzzleImageUnit_listBox->HorizontalScrollbar = true;
			this->WholePuzzleImageUnit_listBox->ItemHeight = 15;
			this->WholePuzzleImageUnit_listBox->Location = System::Drawing::Point(908, 10);
			this->WholePuzzleImageUnit_listBox->Margin = System::Windows::Forms::Padding(4);
			this->WholePuzzleImageUnit_listBox->MinimumSize = System::Drawing::Size(4, 92);
			this->WholePuzzleImageUnit_listBox->Name = L"WholePuzzleImageUnit_listBox";
			this->WholePuzzleImageUnit_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->WholePuzzleImageUnit_listBox->Size = System::Drawing::Size(229, 274);
			this->WholePuzzleImageUnit_listBox->TabIndex = 81;
			this->WholePuzzleImageUnit_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholePuzzleImageUnit_listBox_SelectedIndexChanged);
			this->WholePuzzleImageUnit_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// AssignImage_button
			// 
			this->AssignImage_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AssignImage_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AssignImage_button->Location = System::Drawing::Point(908, 284);
			this->AssignImage_button->Margin = System::Windows::Forms::Padding(4);
			this->AssignImage_button->Name = L"AssignImage_button";
			this->AssignImage_button->Size = System::Drawing::Size(229, 28);
			this->AssignImage_button->TabIndex = 83;
			this->AssignImage_button->Text = L"ApplyImageToPoint/將所選圖片應用到點上";
			this->AssignImage_button->UseVisualStyleBackColor = false;
			this->AssignImage_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AssignImage_button_Click);
			// 
			// label25
			// 
			this->label25->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->label25->AutoSize = true;
			this->label25->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label25->Location = System::Drawing::Point(213, 269);
			this->label25->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(84, 15);
			this->label25->TabIndex = 154;
			this->label25->Text = L"ImageAnchor";
			this->label25->TextAlign = System::Drawing::ContentAlignment::BottomRight;
			// 
			// ShowSelectedImageBorder_checkBox
			// 
			this->ShowSelectedImageBorder_checkBox->AutoSize = true;
			this->ShowSelectedImageBorder_checkBox->Location = System::Drawing::Point(1151, 291);
			this->ShowSelectedImageBorder_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->ShowSelectedImageBorder_checkBox->Name = L"ShowSelectedImageBorder_checkBox";
			this->ShowSelectedImageBorder_checkBox->Size = System::Drawing::Size(179, 19);
			this->ShowSelectedImageBorder_checkBox->TabIndex = 144;
			this->ShowSelectedImageBorder_checkBox->Text = L"ShowSelectedImageBorder";
			this->ShowSelectedImageBorder_checkBox->UseVisualStyleBackColor = true;
			// 
			// DelPI_button
			// 
			this->DelPI_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPI_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPI_button->Location = System::Drawing::Point(723, 284);
			this->DelPI_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DelPI_button->Name = L"DelPI_button";
			this->DelPI_button->Size = System::Drawing::Size(165, 28);
			this->DelPI_button->TabIndex = 142;
			this->DelPI_button->Text = L"DelPI/砍PI";
			this->DelPI_button->UseVisualStyleBackColor = false;
			this->DelPI_button->Click += gcnew System::EventHandler(this, &MPDIEditor::DelPI_button_Click);
			// 
			// ImageAnchor_comboBox
			// 
			this->ImageAnchor_comboBox->FormattingEnabled = true;
			this->ImageAnchor_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {L"LeftUp", L"CenterUp", L"RightUp", 
				L"LeftCenter", L"CenterCenter", L"RightCenter", L"LeftDown", L"CenterDown", L"RightDown"});
			this->ImageAnchor_comboBox->Location = System::Drawing::Point(303, 272);
			this->ImageAnchor_comboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->ImageAnchor_comboBox->Name = L"ImageAnchor_comboBox";
			this->ImageAnchor_comboBox->Size = System::Drawing::Size(121, 23);
			this->ImageAnchor_comboBox->TabIndex = 153;
			this->ImageAnchor_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::ImageAnchor_comboBox_SelectedIndexChanged);
			// 
			// UseViewPort_checkBox
			// 
			this->UseViewPort_checkBox->AutoSize = true;
			this->UseViewPort_checkBox->Location = System::Drawing::Point(1993, 168);
			this->UseViewPort_checkBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->UseViewPort_checkBox->Name = L"UseViewPort_checkBox";
			this->UseViewPort_checkBox->Size = System::Drawing::Size(195, 19);
			this->UseViewPort_checkBox->TabIndex = 152;
			this->UseViewPort_checkBox->Text = L"UseViewPort/使用可視範圍";
			this->UseViewPort_checkBox->UseVisualStyleBackColor = true;
			this->UseViewPort_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::UseViewPort_checkBox_CheckedChanged);
			// 
			// AllPI_listBox
			// 
			this->AllPI_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AllPI_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPI_listBox->FormattingEnabled = true;
			this->AllPI_listBox->HorizontalScrollbar = true;
			this->AllPI_listBox->ItemHeight = 15;
			this->AllPI_listBox->Location = System::Drawing::Point(723, 12);
			this->AllPI_listBox->Margin = System::Windows::Forms::Padding(4);
			this->AllPI_listBox->MinimumSize = System::Drawing::Size(4, 20);
			this->AllPI_listBox->Name = L"AllPI_listBox";
			this->AllPI_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->AllPI_listBox->Size = System::Drawing::Size(167, 259);
			this->AllPI_listBox->TabIndex = 141;
			this->AllPI_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::AllPI_listBox_SelectedIndexChanged);
			// 
			// ViewPort_xyzwNumericalControl
			// 
			this->ViewPort_xyzwNumericalControl->Location = System::Drawing::Point(1980, 184);
			this->ViewPort_xyzwNumericalControl->Margin = System::Windows::Forms::Padding(5);
			this->ViewPort_xyzwNumericalControl->Name = L"ViewPort_xyzwNumericalControl";
			this->ViewPort_xyzwNumericalControl->Size = System::Drawing::Size(149, 132);
			this->ViewPort_xyzwNumericalControl->TabIndex = 0;
			// 
			// RotationPosOffset_xyzNumeriaclContro
			// 
			this->RotationPosOffset_xyzNumeriaclContro->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->RotationPosOffset_xyzNumeriaclContro->Location = System::Drawing::Point(1824, 191);
			this->RotationPosOffset_xyzNumeriaclContro->Margin = System::Windows::Forms::Padding(0);
			this->RotationPosOffset_xyzNumeriaclContro->Name = L"RotationPosOffset_xyzNumeriaclContro";
			this->RotationPosOffset_xyzNumeriaclContro->Size = System::Drawing::Size(101, 108);
			this->RotationPosOffset_xyzNumeriaclContro->TabIndex = 150;
			// 
			// ApplyImageWithOriginalData_button
			// 
			this->ApplyImageWithOriginalData_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ApplyImageWithOriginalData_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ApplyImageWithOriginalData_button->Location = System::Drawing::Point(248, 125);
			this->ApplyImageWithOriginalData_button->Margin = System::Windows::Forms::Padding(4);
			this->ApplyImageWithOriginalData_button->Name = L"ApplyImageWithOriginalData_button";
			this->ApplyImageWithOriginalData_button->Size = System::Drawing::Size(204, 28);
			this->ApplyImageWithOriginalData_button->TabIndex = 145;
			this->ApplyImageWithOriginalData_button->Text = L"ApplyImageWithOriginalData/應用圖片但是保持原本資料";
			this->ApplyImageWithOriginalData_button->UseVisualStyleBackColor = false;
			// 
			// StayAtLastFrame_checkBox
			// 
			this->StayAtLastFrame_checkBox->AutoSize = true;
			this->StayAtLastFrame_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 8));
			this->StayAtLastFrame_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->StayAtLastFrame_checkBox->Location = System::Drawing::Point(380, 250);
			this->StayAtLastFrame_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->StayAtLastFrame_checkBox->Name = L"StayAtLastFrame_checkBox";
			this->StayAtLastFrame_checkBox->Size = System::Drawing::Size(207, 18);
			this->StayAtLastFrame_checkBox->TabIndex = 149;
			this->StayAtLastFrame_checkBox->Text = L"StayAtLastFrame/停在最後一格";
			this->StayAtLastFrame_checkBox->UseVisualStyleBackColor = true;
			this->StayAtLastFrame_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Play_checkBox_CheckedChanged);
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(1824, 171);
			this->label22->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(204, 15);
			this->label22->TabIndex = 151;
			this->label22->Text = L"RotationPosOffset/旋轉中心調整";
			// 
			// ColorBlending_groupBox
			// 
			this->ColorBlending_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ColorBlending_groupBox->Controls->Add(this->EnableColorBlending_checkBox);
			this->ColorBlending_groupBox->Controls->Add(this->SrcBlending_comboBox);
			this->ColorBlending_groupBox->Controls->Add(this->DestBlending_comboBox);
			this->ColorBlending_groupBox->Controls->Add(this->label18);
			this->ColorBlending_groupBox->Controls->Add(this->label19);
			this->ColorBlending_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ColorBlending_groupBox->Location = System::Drawing::Point(1860, 8);
			this->ColorBlending_groupBox->Margin = System::Windows::Forms::Padding(4);
			this->ColorBlending_groupBox->Name = L"ColorBlending_groupBox";
			this->ColorBlending_groupBox->Padding = System::Windows::Forms::Padding(4);
			this->ColorBlending_groupBox->Size = System::Drawing::Size(187, 141);
			this->ColorBlending_groupBox->TabIndex = 0;
			this->ColorBlending_groupBox->TabStop = false;
			this->ColorBlending_groupBox->Text = L"Color Blending";
			// 
			// EnableColorBlending_checkBox
			// 
			this->EnableColorBlending_checkBox->AutoSize = true;
			this->EnableColorBlending_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->EnableColorBlending_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EnableColorBlending_checkBox->Location = System::Drawing::Point(8, 18);
			this->EnableColorBlending_checkBox->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->EnableColorBlending_checkBox->Name = L"EnableColorBlending_checkBox";
			this->EnableColorBlending_checkBox->Size = System::Drawing::Size(212, 19);
			this->EnableColorBlending_checkBox->TabIndex = 141;
			this->EnableColorBlending_checkBox->Text = L"EnableColorBlending/顏色混合";
			this->EnableColorBlending_checkBox->UseVisualStyleBackColor = false;
			this->EnableColorBlending_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::EnableColorBlending_checkBox_CheckedChanged);
			// 
			// SrcBlending_comboBox
			// 
			this->SrcBlending_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SrcBlending_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SrcBlending_comboBox->FormattingEnabled = true;
			this->SrcBlending_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {L"GL_ZERO", L"GL_ONE", L"GL_ONE_MINUS_SRC_COLOR", 
				L"GL_DST_COLOR", L"GL_ONE_MINUS_DST_COLOR", L"GL_SRC_ALPHA", L"GL_ONE_MINUS_SRC_ALPHA", L"GL_DST_ALPHA", L"GL_ONE_MINUS_DST_ALPHA"});
			this->SrcBlending_comboBox->Location = System::Drawing::Point(8, 58);
			this->SrcBlending_comboBox->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->SrcBlending_comboBox->Name = L"SrcBlending_comboBox";
			this->SrcBlending_comboBox->Size = System::Drawing::Size(169, 23);
			this->SrcBlending_comboBox->TabIndex = 14;
			this->SrcBlending_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::SrcBlending_comboBox_SelectedIndexChanged);
			// 
			// DestBlending_comboBox
			// 
			this->DestBlending_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DestBlending_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DestBlending_comboBox->FormattingEnabled = true;
			this->DestBlending_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(9) {L"GL_ZERO", L"GL_ONE", L"GL_ONE_MINUS_SRC_COLOR", 
				L"GL_DST_COLOR", L"GL_ONE_MINUS_DST_COLOR", L"GL_SRC_ALPHA", L"GL_ONE_MINUS_SRC_ALPHA", L"GL_DST_ALPHA", L"GL_ONE_MINUS_DST_ALPHA"});
			this->DestBlending_comboBox->Location = System::Drawing::Point(9, 106);
			this->DestBlending_comboBox->Margin = System::Windows::Forms::Padding(4, 2, 4, 2);
			this->DestBlending_comboBox->Name = L"DestBlending_comboBox";
			this->DestBlending_comboBox->Size = System::Drawing::Size(168, 23);
			this->DestBlending_comboBox->TabIndex = 15;
			this->DestBlending_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::DestBlending_comboBox_SelectedIndexChanged);
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label18->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label18->Location = System::Drawing::Point(9, 41);
			this->label18->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(60, 15);
			this->label18->TabIndex = 144;
			this->label18->Text = L"Src/圖檔";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label19->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label19->Location = System::Drawing::Point(9, 89);
			this->label19->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(66, 15);
			this->label19->TabIndex = 145;
			this->label19->Text = L"Dest/目的";
			// 
			// Rotation_groupBox
			// 
			this->Rotation_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Rotation_groupBox->Controls->Add(this->label3);
			this->Rotation_groupBox->Controls->Add(this->AngleY_numericUpDown);
			this->Rotation_groupBox->Controls->Add(this->label2);
			this->Rotation_groupBox->Controls->Add(this->AngleX_numericUpDown);
			this->Rotation_groupBox->Controls->Add(this->label5);
			this->Rotation_groupBox->Controls->Add(this->AngleZ_numericUpDown);
			this->Rotation_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Rotation_groupBox->Location = System::Drawing::Point(1492, 158);
			this->Rotation_groupBox->Margin = System::Windows::Forms::Padding(4);
			this->Rotation_groupBox->Name = L"Rotation_groupBox";
			this->Rotation_groupBox->Padding = System::Windows::Forms::Padding(4);
			this->Rotation_groupBox->Size = System::Drawing::Size(115, 120);
			this->Rotation_groupBox->TabIndex = 0;
			this->Rotation_groupBox->TabStop = false;
			this->Rotation_groupBox->Text = L"Rotation";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label3->Location = System::Drawing::Point(4, 55);
			this->label3->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(17, 15);
			this->label3->TabIndex = 93;
			this->label3->Text = L"Y";
			// 
			// AngleY_numericUpDown
			// 
			this->AngleY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AngleY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AngleY_numericUpDown->Location = System::Drawing::Point(33, 52);
			this->AngleY_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->AngleY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->AngleY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->AngleY_numericUpDown->Name = L"AngleY_numericUpDown";
			this->AngleY_numericUpDown->Size = System::Drawing::Size(64, 25);
			this->AngleY_numericUpDown->TabIndex = 4;
			this->AngleY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->AngleY_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label2->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label2->Location = System::Drawing::Point(4, 24);
			this->label2->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(17, 15);
			this->label2->TabIndex = 91;
			this->label2->Text = L"X";
			// 
			// AngleX_numericUpDown
			// 
			this->AngleX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AngleX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AngleX_numericUpDown->Location = System::Drawing::Point(32, 16);
			this->AngleX_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->AngleX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->AngleX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->AngleX_numericUpDown->Name = L"AngleX_numericUpDown";
			this->AngleX_numericUpDown->Size = System::Drawing::Size(64, 25);
			this->AngleX_numericUpDown->TabIndex = 3;
			this->AngleX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->AngleX_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label5->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label5->Location = System::Drawing::Point(4, 92);
			this->label5->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(16, 15);
			this->label5->TabIndex = 89;
			this->label5->Text = L"Z";
			// 
			// AngleZ_numericUpDown
			// 
			this->AngleZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AngleZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AngleZ_numericUpDown->Location = System::Drawing::Point(32, 86);
			this->AngleZ_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->AngleZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->AngleZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->AngleZ_numericUpDown->Name = L"AngleZ_numericUpDown";
			this->AngleZ_numericUpDown->Size = System::Drawing::Size(64, 25);
			this->AngleZ_numericUpDown->TabIndex = 5;
			this->AngleZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->AngleZ_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// BehaviorUpdateActive_checkBox
			// 
			this->BehaviorUpdateActive_checkBox->AutoSize = true;
			this->BehaviorUpdateActive_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->BehaviorUpdateActive_checkBox->ForeColor = System::Drawing::Color::White;
			this->BehaviorUpdateActive_checkBox->Location = System::Drawing::Point(1616, 246);
			this->BehaviorUpdateActive_checkBox->Margin = System::Windows::Forms::Padding(5);
			this->BehaviorUpdateActive_checkBox->Name = L"BehaviorUpdateActive_checkBox";
			this->BehaviorUpdateActive_checkBox->Size = System::Drawing::Size(211, 19);
			this->BehaviorUpdateActive_checkBox->TabIndex = 130;
			this->BehaviorUpdateActive_checkBox->Text = L"BehaviorUpdate/動態更新資料";
			this->BehaviorUpdateActive_checkBox->UseVisualStyleBackColor = false;
			this->BehaviorUpdateActive_checkBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			this->BehaviorUpdateActive_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::BehaviorUpdateActive_checkBox_CheckedChanged);
			// 
			// InvertAllPath_button
			// 
			this->InvertAllPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InvertAllPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->InvertAllPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InvertAllPath_button->Location = System::Drawing::Point(249, 88);
			this->InvertAllPath_button->Margin = System::Windows::Forms::Padding(4);
			this->InvertAllPath_button->Name = L"InvertAllPath_button";
			this->InvertAllPath_button->Size = System::Drawing::Size(112, 25);
			this->InvertAllPath_button->TabIndex = 146;
			this->InvertAllPath_button->Text = L"CloneBatchPath/拷貝一群路徑";
			this->InvertAllPath_button->UseVisualStyleBackColor = false;
			this->InvertAllPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
			// 
			// SelectAllPoints_button
			// 
			this->SelectAllPoints_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SelectAllPoints_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SelectAllPoints_button->Location = System::Drawing::Point(1151, 209);
			this->SelectAllPoints_button->Margin = System::Windows::Forms::Padding(4);
			this->SelectAllPoints_button->Name = L"SelectAllPoints_button";
			this->SelectAllPoints_button->Size = System::Drawing::Size(136, 25);
			this->SelectAllPoints_button->TabIndex = 145;
			this->SelectAllPoints_button->Text = L"SelectAllPoints/全選";
			this->SelectAllPoints_button->UseVisualStyleBackColor = false;
			this->SelectAllPoints_button->Click += gcnew System::EventHandler(this, &MPDIEditor::SelectAllPoints_button_Click);
			// 
			// AverageAssign_checkBox
			// 
			this->AverageAssign_checkBox->AutoSize = true;
			this->AverageAssign_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AverageAssign_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AverageAssign_checkBox->Location = System::Drawing::Point(1616, 218);
			this->AverageAssign_checkBox->Margin = System::Windows::Forms::Padding(5);
			this->AverageAssign_checkBox->Name = L"AverageAssign_checkBox";
			this->AverageAssign_checkBox->Size = System::Drawing::Size(205, 19);
			this->AverageAssign_checkBox->TabIndex = 124;
			this->AverageAssign_checkBox->Text = L"AverageAssign/自動平均給值";
			this->AverageAssign_checkBox->UseVisualStyleBackColor = false;
			// 
			// SelectFirstAndLast_button
			// 
			this->SelectFirstAndLast_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SelectFirstAndLast_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SelectFirstAndLast_button->Location = System::Drawing::Point(1153, 180);
			this->SelectFirstAndLast_button->Margin = System::Windows::Forms::Padding(4);
			this->SelectFirstAndLast_button->Name = L"SelectFirstAndLast_button";
			this->SelectFirstAndLast_button->Size = System::Drawing::Size(136, 25);
			this->SelectFirstAndLast_button->TabIndex = 143;
			this->SelectFirstAndLast_button->Text = L"SelectFirstAndLast/選頭尾";
			this->SelectFirstAndLast_button->UseVisualStyleBackColor = false;
			this->SelectFirstAndLast_button->Click += gcnew System::EventHandler(this, &MPDIEditor::SelectFirstAndLast_button_Click);
			// 
			// Size_groupBox
			// 
			this->Size_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Size_groupBox->Controls->Add(this->label6);
			this->Size_groupBox->Controls->Add(this->label21);
			this->Size_groupBox->Controls->Add(this->Width_numericUpDown);
			this->Size_groupBox->Controls->Add(this->Height_numericUpDown);
			this->Size_groupBox->Controls->Add(this->SizeScale_numericUpDown);
			this->Size_groupBox->Controls->Add(this->label7);
			this->Size_groupBox->Controls->Add(this->Mirror_checkBox);
			this->Size_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Size_groupBox->Location = System::Drawing::Point(1616, 6);
			this->Size_groupBox->Margin = System::Windows::Forms::Padding(4);
			this->Size_groupBox->Name = L"Size_groupBox";
			this->Size_groupBox->Padding = System::Windows::Forms::Padding(4);
			this->Size_groupBox->Size = System::Drawing::Size(124, 196);
			this->Size_groupBox->TabIndex = 148;
			this->Size_groupBox->TabStop = false;
			this->Size_groupBox->Text = L"Size";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label6->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label6->Location = System::Drawing::Point(9, 16);
			this->label6->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(61, 15);
			this->label6->TabIndex = 88;
			this->label6->Text = L"Width/寬";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label21->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label21->Location = System::Drawing::Point(8, 111);
			this->label21->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(170, 15);
			this->label21->TabIndex = 147;
			this->label21->Text = L"ScaleSyncly/尺寸等比縮放";
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Width_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Width_numericUpDown->Location = System::Drawing::Point(9, 32);
			this->Width_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000000, 0, 0, 0});
			this->Width_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000000, 0, 0, System::Int32::MinValue});
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(92, 25);
			this->Width_numericUpDown->TabIndex = 6;
			this->Width_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->Width_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Height_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Height_numericUpDown->Location = System::Drawing::Point(12, 80);
			this->Height_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000000, 0, 0, 0});
			this->Height_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1410065408, 2, 0, System::Int32::MinValue});
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(89, 25);
			this->Height_numericUpDown->TabIndex = 7;
			this->Height_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->Height_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// SizeScale_numericUpDown
			// 
			this->SizeScale_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->SizeScale_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->SizeScale_numericUpDown->Location = System::Drawing::Point(7, 131);
			this->SizeScale_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->SizeScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->SizeScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->SizeScale_numericUpDown->Name = L"SizeScale_numericUpDown";
			this->SizeScale_numericUpDown->Size = System::Drawing::Size(95, 25);
			this->SizeScale_numericUpDown->TabIndex = 8;
			this->SizeScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->SizeScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->SizeScale_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label7->Location = System::Drawing::Point(9, 62);
			this->label7->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(64, 15);
			this->label7->TabIndex = 91;
			this->label7->Text = L"Height/高";
			// 
			// Mirror_checkBox
			// 
			this->Mirror_checkBox->AutoSize = true;
			this->Mirror_checkBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Mirror_checkBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Mirror_checkBox->Location = System::Drawing::Point(7, 169);
			this->Mirror_checkBox->Margin = System::Windows::Forms::Padding(5);
			this->Mirror_checkBox->Name = L"Mirror_checkBox";
			this->Mirror_checkBox->Size = System::Drawing::Size(65, 19);
			this->Mirror_checkBox->TabIndex = 9;
			this->Mirror_checkBox->Text = L"Mirror";
			this->Mirror_checkBox->UseVisualStyleBackColor = false;
			this->Mirror_checkBox->CheckedChanged += gcnew System::EventHandler(this, &MPDIEditor::Mirror_checkBox_CheckedChanged);
			// 
			// Color_groupBox
			// 
			this->Color_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(74)), static_cast<System::Int32>(static_cast<System::Byte>(74)), 
				static_cast<System::Int32>(static_cast<System::Byte>(74)));
			this->Color_groupBox->Controls->Add(this->RedColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->GreenColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->AlphaColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->label12);
			this->Color_groupBox->Controls->Add(this->BlueColor_numericUpDown);
			this->Color_groupBox->Controls->Add(this->label11);
			this->Color_groupBox->Controls->Add(this->label13);
			this->Color_groupBox->Controls->Add(this->label10);
			this->Color_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Color_groupBox->Location = System::Drawing::Point(1748, 9);
			this->Color_groupBox->Margin = System::Windows::Forms::Padding(4);
			this->Color_groupBox->Name = L"Color_groupBox";
			this->Color_groupBox->Padding = System::Windows::Forms::Padding(4);
			this->Color_groupBox->Size = System::Drawing::Size(104, 149);
			this->Color_groupBox->TabIndex = 1;
			this->Color_groupBox->TabStop = false;
			this->Color_groupBox->Text = L"Color";
			// 
			// RedColor_numericUpDown
			// 
			this->RedColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->RedColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->RedColor_numericUpDown->Location = System::Drawing::Point(28, 18);
			this->RedColor_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->RedColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->RedColor_numericUpDown->Name = L"RedColor_numericUpDown";
			this->RedColor_numericUpDown->Size = System::Drawing::Size(57, 25);
			this->RedColor_numericUpDown->TabIndex = 10;
			this->RedColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->RedColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->RedColor_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// GreenColor_numericUpDown
			// 
			this->GreenColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GreenColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GreenColor_numericUpDown->Location = System::Drawing::Point(28, 51);
			this->GreenColor_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->GreenColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->GreenColor_numericUpDown->Name = L"GreenColor_numericUpDown";
			this->GreenColor_numericUpDown->Size = System::Drawing::Size(57, 25);
			this->GreenColor_numericUpDown->TabIndex = 11;
			this->GreenColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->GreenColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->GreenColor_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// AlphaColor_numericUpDown
			// 
			this->AlphaColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AlphaColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AlphaColor_numericUpDown->Location = System::Drawing::Point(28, 111);
			this->AlphaColor_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->AlphaColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->AlphaColor_numericUpDown->Name = L"AlphaColor_numericUpDown";
			this->AlphaColor_numericUpDown->Size = System::Drawing::Size(57, 25);
			this->AlphaColor_numericUpDown->TabIndex = 13;
			this->AlphaColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->AlphaColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->AlphaColor_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label12->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label12->Location = System::Drawing::Point(4, 116);
			this->label12->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(17, 15);
			this->label12->TabIndex = 101;
			this->label12->Text = L"A";
			// 
			// BlueColor_numericUpDown
			// 
			this->BlueColor_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->BlueColor_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->BlueColor_numericUpDown->Location = System::Drawing::Point(28, 84);
			this->BlueColor_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->BlueColor_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->BlueColor_numericUpDown->Name = L"BlueColor_numericUpDown";
			this->BlueColor_numericUpDown->Size = System::Drawing::Size(57, 25);
			this->BlueColor_numericUpDown->TabIndex = 12;
			this->BlueColor_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {255, 0, 0, 0});
			this->BlueColor_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->BlueColor_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label11->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label11->Location = System::Drawing::Point(3, 24);
			this->label11->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(16, 15);
			this->label11->TabIndex = 103;
			this->label11->Text = L"R";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label13->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label13->Location = System::Drawing::Point(3, 88);
			this->label13->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(16, 15);
			this->label13->TabIndex = 99;
			this->label13->Text = L"B";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label10->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label10->Location = System::Drawing::Point(3, 56);
			this->label10->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(17, 15);
			this->label10->TabIndex = 105;
			this->label10->Text = L"G";
			// 
			// Position_groupBox
			// 
			this->Position_groupBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->Position_groupBox->Controls->Add(this->PathNodePosX_numericUpDown);
			this->Position_groupBox->Controls->Add(this->PathNodePosY_numericUpDown);
			this->Position_groupBox->Controls->Add(this->PathNodePosZ_numericUpDown);
			this->Position_groupBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->Position_groupBox->Location = System::Drawing::Point(1493, 6);
			this->Position_groupBox->Margin = System::Windows::Forms::Padding(4);
			this->Position_groupBox->Name = L"Position_groupBox";
			this->Position_groupBox->Padding = System::Windows::Forms::Padding(4);
			this->Position_groupBox->Size = System::Drawing::Size(115, 121);
			this->Position_groupBox->TabIndex = 0;
			this->Position_groupBox->TabStop = false;
			this->Position_groupBox->Text = L"Position";
			// 
			// PathNodePosX_numericUpDown
			// 
			this->PathNodePosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosX_numericUpDown->DecimalPlaces = 1;
			this->PathNodePosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosX_numericUpDown->Location = System::Drawing::Point(9, 19);
			this->PathNodePosX_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->PathNodePosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->PathNodePosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->PathNodePosX_numericUpDown->Name = L"PathNodePosX_numericUpDown";
			this->PathNodePosX_numericUpDown->Size = System::Drawing::Size(95, 25);
			this->PathNodePosX_numericUpDown->TabIndex = 0;
			this->PathNodePosX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->PathNodePosX_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// PathNodePosY_numericUpDown
			// 
			this->PathNodePosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosY_numericUpDown->DecimalPlaces = 1;
			this->PathNodePosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosY_numericUpDown->Location = System::Drawing::Point(9, 54);
			this->PathNodePosY_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->PathNodePosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->PathNodePosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->PathNodePosY_numericUpDown->Name = L"PathNodePosY_numericUpDown";
			this->PathNodePosY_numericUpDown->Size = System::Drawing::Size(95, 25);
			this->PathNodePosY_numericUpDown->TabIndex = 1;
			this->PathNodePosY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->PathNodePosY_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// PathNodePosZ_numericUpDown
			// 
			this->PathNodePosZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->PathNodePosZ_numericUpDown->DecimalPlaces = 1;
			this->PathNodePosZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->PathNodePosZ_numericUpDown->Location = System::Drawing::Point(8, 86);
			this->PathNodePosZ_numericUpDown->Margin = System::Windows::Forms::Padding(5);
			this->PathNodePosZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, 0});
			this->PathNodePosZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {100000, 0, 0, System::Int32::MinValue});
			this->PathNodePosZ_numericUpDown->Name = L"PathNodePosZ_numericUpDown";
			this->PathNodePosZ_numericUpDown->Size = System::Drawing::Size(95, 25);
			this->PathNodePosZ_numericUpDown->TabIndex = 2;
			this->PathNodePosZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::AlphaColor_numericUpDown_ValueChanged);
			this->PathNodePosZ_numericUpDown->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::PathNode_listBox_KeyUp);
			// 
			// InvertPath_button
			// 
			this->InvertPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->InvertPath_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->InvertPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->InvertPath_button->Location = System::Drawing::Point(356, 51);
			this->InvertPath_button->Margin = System::Windows::Forms::Padding(4);
			this->InvertPath_button->Name = L"InvertPath_button";
			this->InvertPath_button->Size = System::Drawing::Size(100, 24);
			this->InvertPath_button->TabIndex = 138;
			this->InvertPath_button->Text = L"InvertPath/顛倒路勁";
			this->InvertPath_button->UseVisualStyleBackColor = false;
			this->InvertPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPath_button_Click);
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->splitContainer3->Name = L"splitContainer3";
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Resize += gcnew System::EventHandler(this, &MPDIEditor::splitContainer3_Panel1_Resize);
			this->splitContainer3->Panel2Collapsed = true;
			this->splitContainer3->Size = System::Drawing::Size(2167, 950);
			this->splitContainer3->SplitterDistance = 363;
			this->splitContainer3->TabIndex = 0;
			// 
			// splitContainer2
			// 
			this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 24);
			this->splitContainer2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->splitContainer2->Name = L"splitContainer2";
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->tabControl1);
			this->splitContainer2->Panel1MinSize = 385;
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->splitContainer1);
			this->splitContainer2->Size = System::Drawing::Size(2560, 1308);
			this->splitContainer2->SplitterDistance = 385;
			this->splitContainer2->TabIndex = 146;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Margin = System::Windows::Forms::Padding(4);
			this->tabControl1->Multiline = true;
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(381, 1304);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->AutoScroll = true;
			this->tabPage1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->tabPage1->Controls->Add(this->flowLayoutPanel1);
			this->tabPage1->Location = System::Drawing::Point(4, 25);
			this->tabPage1->Margin = System::Windows::Forms::Padding(4);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(4);
			this->tabPage1->Size = System::Drawing::Size(373, 1275);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"EditAnimationData/編輯動畫資料";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->AutoScroll = true;
			this->flowLayoutPanel1->Controls->Add(this->GroupData_collpaseForm);
			this->flowLayoutPanel1->Controls->Add(this->AnimationData_collpaseForm);
			this->flowLayoutPanel1->Controls->Add(this->MPDIExtra_xmlNodeEditComponent);
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->Location = System::Drawing::Point(4, 4);
			this->flowLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(365, 1267);
			this->flowLayoutPanel1->TabIndex = 0;
			// 
			// GroupData_collpaseForm
			// 
			this->GroupData_collpaseForm->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenter_label);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenterY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenterX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationCenterZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label26);
			this->GroupData_collpaseForm->Controls->Add(this->GroupScale_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotation_label3);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->CameraRotationZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPosY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label29);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPosX_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label28);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPosZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupImageRotate_checkBox);
			this->GroupData_collpaseForm->Controls->Add(this->label24);
			this->GroupData_collpaseForm->Controls->Add(this->label27);
			this->GroupData_collpaseForm->Controls->Add(this->ApplyGroupDataChange_button);
			this->GroupData_collpaseForm->Controls->Add(this->GroupPositionMoveType_comboBox);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationZ_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->label23);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationY_numericUpDown);
			this->GroupData_collpaseForm->Controls->Add(this->GroupRotationType_comboBox);
			this->GroupData_collpaseForm->ForeColor = System::Drawing::SystemColors::ControlDark;
			this->GroupData_collpaseForm->Location = System::Drawing::Point(4, 4);
			this->GroupData_collpaseForm->Margin = System::Windows::Forms::Padding(4);
			this->GroupData_collpaseForm->Name = L"GroupData_collpaseForm";
			this->GroupData_collpaseForm->Size = System::Drawing::Size(347, 249);
			this->GroupData_collpaseForm->TabIndex = 0;
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label26->Location = System::Drawing::Point(11, 15);
			this->label26->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(175, 15);
			this->label26->TabIndex = 21;
			this->label26->Text = L"Perentage Scale/百分比大小";
			// 
			// GroupScale_numericUpDown
			// 
			this->GroupScale_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupScale_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupScale_numericUpDown->Location = System::Drawing::Point(190, 11);
			this->GroupScale_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupScale_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->GroupScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->GroupScale_numericUpDown->Name = L"GroupScale_numericUpDown";
			this->GroupScale_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->GroupScale_numericUpDown->TabIndex = 25;
			this->GroupScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// CameraRotation_label3
			// 
			this->CameraRotation_label3->AutoSize = true;
			this->CameraRotation_label3->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotation_label3->Location = System::Drawing::Point(188, 62);
			this->CameraRotation_label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->CameraRotation_label3->Name = L"CameraRotation_label3";
			this->CameraRotation_label3->Size = System::Drawing::Size(98, 15);
			this->CameraRotation_label3->TabIndex = 33;
			this->CameraRotation_label3->Text = L"CameraRotation";
			// 
			// CameraRotationY_numericUpDown
			// 
			this->CameraRotationY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationY_numericUpDown->Location = System::Drawing::Point(191, 120);
			this->CameraRotationY_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CameraRotationY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->CameraRotationY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->CameraRotationY_numericUpDown->Name = L"CameraRotationY_numericUpDown";
			this->CameraRotationY_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->CameraRotationY_numericUpDown->TabIndex = 32;
			this->CameraRotationY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationX_numericUpDown
			// 
			this->CameraRotationX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationX_numericUpDown->Location = System::Drawing::Point(192, 85);
			this->CameraRotationX_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CameraRotationX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->CameraRotationX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->CameraRotationX_numericUpDown->Name = L"CameraRotationX_numericUpDown";
			this->CameraRotationX_numericUpDown->Size = System::Drawing::Size(76, 25);
			this->CameraRotationX_numericUpDown->TabIndex = 30;
			this->CameraRotationX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationZ_numericUpDown
			// 
			this->CameraRotationZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationZ_numericUpDown->Location = System::Drawing::Point(191, 155);
			this->CameraRotationZ_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CameraRotationZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->CameraRotationZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->CameraRotationZ_numericUpDown->Name = L"CameraRotationZ_numericUpDown";
			this->CameraRotationZ_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->CameraRotationZ_numericUpDown->TabIndex = 31;
			this->CameraRotationZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// GroupRotationX_numericUpDown
			// 
			this->GroupRotationX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationX_numericUpDown->Location = System::Drawing::Point(14, 84);
			this->GroupRotationX_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupRotationX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, 0});
			this->GroupRotationX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, System::Int32::MinValue});
			this->GroupRotationX_numericUpDown->Name = L"GroupRotationX_numericUpDown";
			this->GroupRotationX_numericUpDown->Size = System::Drawing::Size(76, 25);
			this->GroupRotationX_numericUpDown->TabIndex = 18;
			// 
			// GroupPosY_numericUpDown
			// 
			this->GroupPosY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPosY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPosY_numericUpDown->Location = System::Drawing::Point(98, 119);
			this->GroupPosY_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupPosY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->GroupPosY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->GroupPosY_numericUpDown->Name = L"GroupPosY_numericUpDown";
			this->GroupPosY_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->GroupPosY_numericUpDown->TabIndex = 7;
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Location = System::Drawing::Point(-1, 154);
			this->label29->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(13, 15);
			this->label29->TabIndex = 28;
			this->label29->Text = L"z";
			// 
			// GroupPosX_numericUpDown
			// 
			this->GroupPosX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPosX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPosX_numericUpDown->Location = System::Drawing::Point(99, 84);
			this->GroupPosX_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupPosX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->GroupPosX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->GroupPosX_numericUpDown->Name = L"GroupPosX_numericUpDown";
			this->GroupPosX_numericUpDown->Size = System::Drawing::Size(76, 25);
			this->GroupPosX_numericUpDown->TabIndex = 5;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Location = System::Drawing::Point(-1, 119);
			this->label28->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(14, 15);
			this->label28->TabIndex = 27;
			this->label28->Text = L"y";
			// 
			// GroupPosZ_numericUpDown
			// 
			this->GroupPosZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPosZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPosZ_numericUpDown->Location = System::Drawing::Point(98, 154);
			this->GroupPosZ_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupPosZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->GroupPosZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->GroupPosZ_numericUpDown->Name = L"GroupPosZ_numericUpDown";
			this->GroupPosZ_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->GroupPosZ_numericUpDown->TabIndex = 6;
			// 
			// GroupImageRotate_checkBox
			// 
			this->GroupImageRotate_checkBox->AutoSize = true;
			this->GroupImageRotate_checkBox->Checked = true;
			this->GroupImageRotate_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->GroupImageRotate_checkBox->Location = System::Drawing::Point(13, 185);
			this->GroupImageRotate_checkBox->Margin = System::Windows::Forms::Padding(4);
			this->GroupImageRotate_checkBox->Name = L"GroupImageRotate_checkBox";
			this->GroupImageRotate_checkBox->Size = System::Drawing::Size(133, 19);
			this->GroupImageRotate_checkBox->TabIndex = 29;
			this->GroupImageRotate_checkBox->Text = L"GroupImageRotate";
			this->GroupImageRotate_checkBox->UseVisualStyleBackColor = true;
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label24->Location = System::Drawing::Point(94, 38);
			this->label24->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(87, 15);
			this->label24->TabIndex = 11;
			this->label24->Text = L"Position/位置";
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Location = System::Drawing::Point(-1, 84);
			this->label27->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(14, 15);
			this->label27->TabIndex = 26;
			this->label27->Text = L"x";
			// 
			// ApplyGroupDataChange_button
			// 
			this->ApplyGroupDataChange_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ApplyGroupDataChange_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ApplyGroupDataChange_button->Location = System::Drawing::Point(-5, 206);
			this->ApplyGroupDataChange_button->Margin = System::Windows::Forms::Padding(4);
			this->ApplyGroupDataChange_button->Name = L"ApplyGroupDataChange_button";
			this->ApplyGroupDataChange_button->Size = System::Drawing::Size(320, 29);
			this->ApplyGroupDataChange_button->TabIndex = 13;
			this->ApplyGroupDataChange_button->Text = L"ApplyGroupDataChange/群組資料變更應用";
			this->ApplyGroupDataChange_button->UseVisualStyleBackColor = false;
			this->ApplyGroupDataChange_button->Click += gcnew System::EventHandler(this, &MPDIEditor::ApplyGroupDataChange_button_Click);
			// 
			// GroupPositionMoveType_comboBox
			// 
			this->GroupPositionMoveType_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupPositionMoveType_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupPositionMoveType_comboBox->FormattingEnabled = true;
			this->GroupPositionMoveType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"None", L"Relative", L"World"});
			this->GroupPositionMoveType_comboBox->Location = System::Drawing::Point(98, 54);
			this->GroupPositionMoveType_comboBox->Margin = System::Windows::Forms::Padding(4);
			this->GroupPositionMoveType_comboBox->Name = L"GroupPositionMoveType_comboBox";
			this->GroupPositionMoveType_comboBox->Size = System::Drawing::Size(81, 23);
			this->GroupPositionMoveType_comboBox->TabIndex = 17;
			// 
			// GroupRotationZ_numericUpDown
			// 
			this->GroupRotationZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationZ_numericUpDown->Location = System::Drawing::Point(13, 154);
			this->GroupRotationZ_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupRotationZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, 0});
			this->GroupRotationZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, System::Int32::MinValue});
			this->GroupRotationZ_numericUpDown->Name = L"GroupRotationZ_numericUpDown";
			this->GroupRotationZ_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->GroupRotationZ_numericUpDown->TabIndex = 19;
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label23->Location = System::Drawing::Point(10, 37);
			this->label23->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(89, 15);
			this->label23->TabIndex = 9;
			this->label23->Text = L"Rotation/旋轉";
			// 
			// GroupRotationY_numericUpDown
			// 
			this->GroupRotationY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationY_numericUpDown->Location = System::Drawing::Point(13, 119);
			this->GroupRotationY_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->GroupRotationY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, 0});
			this->GroupRotationY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {360, 0, 0, System::Int32::MinValue});
			this->GroupRotationY_numericUpDown->Name = L"GroupRotationY_numericUpDown";
			this->GroupRotationY_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->GroupRotationY_numericUpDown->TabIndex = 20;
			// 
			// GroupRotationType_comboBox
			// 
			this->GroupRotationType_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->GroupRotationType_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->GroupRotationType_comboBox->FormattingEnabled = true;
			this->GroupRotationType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"None", L"Self", L"World"});
			this->GroupRotationType_comboBox->Location = System::Drawing::Point(11, 54);
			this->GroupRotationType_comboBox->Margin = System::Windows::Forms::Padding(4);
			this->GroupRotationType_comboBox->Name = L"GroupRotationType_comboBox";
			this->GroupRotationType_comboBox->Size = System::Drawing::Size(81, 23);
			this->GroupRotationType_comboBox->TabIndex = 8;
			// 
			// AnimationData_collpaseForm
			// 
			this->AnimationData_collpaseForm->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->AnimationData_collpaseForm->Controls->Add(this->WholeAnimationList_listBox);
			this->AnimationData_collpaseForm->Controls->Add(this->WholeAnimationListUp_button);
			this->AnimationData_collpaseForm->Controls->Add(this->MergeMPDIData_button);
			this->AnimationData_collpaseForm->Controls->Add(this->WholeAnimationListDown_lbutton);
			this->AnimationData_collpaseForm->Controls->Add(this->AnimationName_textBox);
			this->AnimationData_collpaseForm->Controls->Add(this->DeleteListObject_button);
			this->AnimationData_collpaseForm->Controls->Add(this->AnimationInvert_button);
			this->AnimationData_collpaseForm->Controls->Add(this->AddAnimationToList_button);
			this->AnimationData_collpaseForm->Controls->Add(this->label15);
			this->AnimationData_collpaseForm->Location = System::Drawing::Point(4, 261);
			this->AnimationData_collpaseForm->Margin = System::Windows::Forms::Padding(4);
			this->AnimationData_collpaseForm->Name = L"AnimationData_collpaseForm";
			this->AnimationData_collpaseForm->Size = System::Drawing::Size(347, 532);
			this->AnimationData_collpaseForm->TabIndex = 1;
			// 
			// WholeAnimationList_listBox
			// 
			this->WholeAnimationList_listBox->AllowDrop = true;
			this->WholeAnimationList_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->WholeAnimationList_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationList_listBox->FormattingEnabled = true;
			this->WholeAnimationList_listBox->ItemHeight = 15;
			this->WholeAnimationList_listBox->Location = System::Drawing::Point(5, 80);
			this->WholeAnimationList_listBox->Margin = System::Windows::Forms::Padding(5);
			this->WholeAnimationList_listBox->MinimumSize = System::Drawing::Size(4, 105);
			this->WholeAnimationList_listBox->Name = L"WholeAnimationList_listBox";
			this->WholeAnimationList_listBox->ScrollAlwaysVisible = true;
			this->WholeAnimationList_listBox->Size = System::Drawing::Size(295, 439);
			this->WholeAnimationList_listBox->TabIndex = 114;
			this->WholeAnimationList_listBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationList_listBox_SelectedIndexChanged);
			this->WholeAnimationList_listBox->DoubleClick += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationList_listBox_DoubleClick);
			this->WholeAnimationList_listBox->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &MPDIEditor::WholeAnimationList_listBox_KeyUp);
			// 
			// WholeAnimationListUp_button
			// 
			this->WholeAnimationListUp_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeAnimationListUp_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationListUp_button->Location = System::Drawing::Point(309, 132);
			this->WholeAnimationListUp_button->Margin = System::Windows::Forms::Padding(4);
			this->WholeAnimationListUp_button->Name = L"WholeAnimationListUp_button";
			this->WholeAnimationListUp_button->Size = System::Drawing::Size(29, 49);
			this->WholeAnimationListUp_button->TabIndex = 139;
			this->WholeAnimationListUp_button->Text = L"^\r\n^";
			this->WholeAnimationListUp_button->UseVisualStyleBackColor = false;
			this->WholeAnimationListUp_button->Click += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationListUp_button_Click);
			// 
			// MergeMPDIData_button
			// 
			this->MergeMPDIData_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->MergeMPDIData_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MergeMPDIData_button->Location = System::Drawing::Point(256, 2);
			this->MergeMPDIData_button->Margin = System::Windows::Forms::Padding(5);
			this->MergeMPDIData_button->Name = L"MergeMPDIData_button";
			this->MergeMPDIData_button->Size = System::Drawing::Size(89, 31);
			this->MergeMPDIData_button->TabIndex = 118;
			this->MergeMPDIData_button->Text = L"Merge/混合";
			this->MergeMPDIData_button->UseVisualStyleBackColor = false;
			this->MergeMPDIData_button->Click += gcnew System::EventHandler(this, &MPDIEditor::MergeMPDIData_button_Click);
			// 
			// WholeAnimationListDown_lbutton
			// 
			this->WholeAnimationListDown_lbutton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->WholeAnimationListDown_lbutton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->WholeAnimationListDown_lbutton->Location = System::Drawing::Point(311, 276);
			this->WholeAnimationListDown_lbutton->Margin = System::Windows::Forms::Padding(4);
			this->WholeAnimationListDown_lbutton->Name = L"WholeAnimationListDown_lbutton";
			this->WholeAnimationListDown_lbutton->Size = System::Drawing::Size(29, 62);
			this->WholeAnimationListDown_lbutton->TabIndex = 138;
			this->WholeAnimationListDown_lbutton->Text = L"v\r\nv";
			this->WholeAnimationListDown_lbutton->UseVisualStyleBackColor = false;
			this->WholeAnimationListDown_lbutton->Click += gcnew System::EventHandler(this, &MPDIEditor::WholeAnimationListUp_button_Click);
			// 
			// AnimationName_textBox
			// 
			this->AnimationName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationName_textBox->Location = System::Drawing::Point(112, 5);
			this->AnimationName_textBox->Margin = System::Windows::Forms::Padding(5);
			this->AnimationName_textBox->Name = L"AnimationName_textBox";
			this->AnimationName_textBox->Size = System::Drawing::Size(139, 25);
			this->AnimationName_textBox->TabIndex = 115;
			// 
			// DeleteListObject_button
			// 
			this->DeleteListObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DeleteListObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DeleteListObject_button->Location = System::Drawing::Point(108, 40);
			this->DeleteListObject_button->Margin = System::Windows::Forms::Padding(5);
			this->DeleteListObject_button->Name = L"DeleteListObject_button";
			this->DeleteListObject_button->Size = System::Drawing::Size(145, 30);
			this->DeleteListObject_button->TabIndex = 113;
			this->DeleteListObject_button->Text = L"DeleteListObject/刪除物件";
			this->DeleteListObject_button->UseVisualStyleBackColor = false;
			this->DeleteListObject_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddAnimationToList_button_Click);
			// 
			// AnimationInvert_button
			// 
			this->AnimationInvert_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AnimationInvert_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AnimationInvert_button->Location = System::Drawing::Point(261, 39);
			this->AnimationInvert_button->Margin = System::Windows::Forms::Padding(5);
			this->AnimationInvert_button->Name = L"AnimationInvert_button";
			this->AnimationInvert_button->Size = System::Drawing::Size(92, 31);
			this->AnimationInvert_button->TabIndex = 117;
			this->AnimationInvert_button->Text = L"ReEdit/重新編輯";
			this->AnimationInvert_button->UseVisualStyleBackColor = false;
			this->AnimationInvert_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddAnimationToList_button_Click);
			// 
			// AddAnimationToList_button
			// 
			this->AddAnimationToList_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddAnimationToList_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddAnimationToList_button->Location = System::Drawing::Point(4, 40);
			this->AddAnimationToList_button->Margin = System::Windows::Forms::Padding(5);
			this->AddAnimationToList_button->Name = L"AddAnimationToList_button";
			this->AddAnimationToList_button->Size = System::Drawing::Size(100, 30);
			this->AddAnimationToList_button->TabIndex = 112;
			this->AddAnimationToList_button->Text = L"AddToList/增加到輸出檔案列表";
			this->AddAnimationToList_button->UseVisualStyleBackColor = false;
			this->AddAnimationToList_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddAnimationToList_button_Click);
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label15->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label15->Location = System::Drawing::Point(3, 8);
			this->label15->Margin = System::Windows::Forms::Padding(5, 0, 5, 0);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(149, 15);
			this->label15->TabIndex = 116;
			this->label15->Text = L"AnimationName/動畫名";
			// 
			// MPDIExtra_xmlNodeEditComponent
			// 
			this->MPDIExtra_xmlNodeEditComponent->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->MPDIExtra_xmlNodeEditComponent->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->MPDIExtra_xmlNodeEditComponent->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->MPDIExtra_xmlNodeEditComponent->Location = System::Drawing::Point(3, 799);
			this->MPDIExtra_xmlNodeEditComponent->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->MPDIExtra_xmlNodeEditComponent->Name = L"MPDIExtra_xmlNodeEditComponent";
			this->MPDIExtra_xmlNodeEditComponent->Size = System::Drawing::Size(345, 359);
			this->MPDIExtra_xmlNodeEditComponent->TabIndex = 5;
			// 
			// tabPage2
			// 
			this->tabPage2->AutoScroll = true;
			this->tabPage2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(104)), static_cast<System::Int32>(static_cast<System::Byte>(104)), 
				static_cast<System::Int32>(static_cast<System::Byte>(104)));
			this->tabPage2->Controls->Add(this->flowLayoutPanel2);
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Margin = System::Windows::Forms::Padding(4);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(4);
			this->tabPage2->Size = System::Drawing::Size(373, 1275);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Utility/雜項功能";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// flowLayoutPanel2
			// 
			this->flowLayoutPanel2->AutoScroll = true;
			this->flowLayoutPanel2->Controls->Add(this->Path_groupBox);
			this->flowLayoutPanel2->Controls->Add(this->ImageHintPoint_groupBox);
			this->flowLayoutPanel2->Controls->Add(this->HintPoint_groupBox);
			this->flowLayoutPanel2->Controls->Add(this->Video_groupBox);
			this->flowLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel2->Location = System::Drawing::Point(4, 4);
			this->flowLayoutPanel2->Margin = System::Windows::Forms::Padding(4);
			this->flowLayoutPanel2->Name = L"flowLayoutPanel2";
			this->flowLayoutPanel2->Size = System::Drawing::Size(365, 1267);
			this->flowLayoutPanel2->TabIndex = 0;
			// 
			// Path_groupBox
			// 
			this->Path_groupBox->Controls->Add(this->AddPathIntoAllPath_button);
			this->Path_groupBox->Controls->Add(this->AllPath_listBox);
			this->Path_groupBox->Controls->Add(this->DelPathIntoAllPath_button);
			this->Path_groupBox->Controls->Add(this->UsingIntoCurrentPath_button);
			this->Path_groupBox->Location = System::Drawing::Point(3, 2);
			this->Path_groupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Path_groupBox->Name = L"Path_groupBox";
			this->Path_groupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Path_groupBox->Size = System::Drawing::Size(357, 196);
			this->Path_groupBox->TabIndex = 0;
			this->Path_groupBox->TabStop = false;
			this->Path_groupBox->Text = L"Path";
			// 
			// AddPathIntoAllPath_button
			// 
			this->AddPathIntoAllPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddPathIntoAllPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddPathIntoAllPath_button->Location = System::Drawing::Point(19, 20);
			this->AddPathIntoAllPath_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->AddPathIntoAllPath_button->Name = L"AddPathIntoAllPath_button";
			this->AddPathIntoAllPath_button->Size = System::Drawing::Size(76, 26);
			this->AddPathIntoAllPath_button->TabIndex = 1;
			this->AddPathIntoAllPath_button->Text = L"Add/增加";
			this->AddPathIntoAllPath_button->UseVisualStyleBackColor = false;
			this->AddPathIntoAllPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPathIntoAllPath_button_Click);
			// 
			// AllPath_listBox
			// 
			this->AllPath_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(85)), static_cast<System::Int32>(static_cast<System::Byte>(85)), 
				static_cast<System::Int32>(static_cast<System::Byte>(85)));
			this->AllPath_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AllPath_listBox->FormattingEnabled = true;
			this->AllPath_listBox->ItemHeight = 15;
			this->AllPath_listBox->Location = System::Drawing::Point(16, 54);
			this->AllPath_listBox->Margin = System::Windows::Forms::Padding(0);
			this->AllPath_listBox->Name = L"AllPath_listBox";
			this->AllPath_listBox->Size = System::Drawing::Size(320, 124);
			this->AllPath_listBox->TabIndex = 0;
			// 
			// DelPathIntoAllPath_button
			// 
			this->DelPathIntoAllPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelPathIntoAllPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelPathIntoAllPath_button->Location = System::Drawing::Point(143, 20);
			this->DelPathIntoAllPath_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DelPathIntoAllPath_button->Name = L"DelPathIntoAllPath_button";
			this->DelPathIntoAllPath_button->Size = System::Drawing::Size(76, 25);
			this->DelPathIntoAllPath_button->TabIndex = 2;
			this->DelPathIntoAllPath_button->Text = L"Del/刪除";
			this->DelPathIntoAllPath_button->UseVisualStyleBackColor = false;
			this->DelPathIntoAllPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPathIntoAllPath_button_Click);
			// 
			// UsingIntoCurrentPath_button
			// 
			this->UsingIntoCurrentPath_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->UsingIntoCurrentPath_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->UsingIntoCurrentPath_button->Location = System::Drawing::Point(244, 20);
			this->UsingIntoCurrentPath_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->UsingIntoCurrentPath_button->Name = L"UsingIntoCurrentPath_button";
			this->UsingIntoCurrentPath_button->Size = System::Drawing::Size(71, 26);
			this->UsingIntoCurrentPath_button->TabIndex = 3;
			this->UsingIntoCurrentPath_button->Text = L"Using/使用";
			this->UsingIntoCurrentPath_button->UseVisualStyleBackColor = false;
			this->UsingIntoCurrentPath_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddPathIntoAllPath_button_Click);
			// 
			// ImageHintPoint_groupBox
			// 
			this->ImageHintPoint_groupBox->Controls->Add(this->ShowImageOnScreen_listBox);
			this->ImageHintPoint_groupBox->Controls->Add(this->DelShowImageOnScreenObject_button);
			this->ImageHintPoint_groupBox->Controls->Add(this->label16);
			this->ImageHintPoint_groupBox->Controls->Add(this->AddShowImageOnScreenObject_button);
			this->ImageHintPoint_groupBox->Location = System::Drawing::Point(3, 202);
			this->ImageHintPoint_groupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->ImageHintPoint_groupBox->Name = L"ImageHintPoint_groupBox";
			this->ImageHintPoint_groupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->ImageHintPoint_groupBox->Size = System::Drawing::Size(357, 160);
			this->ImageHintPoint_groupBox->TabIndex = 1;
			this->ImageHintPoint_groupBox->TabStop = false;
			this->ImageHintPoint_groupBox->Text = L"ImageHintPoint";
			// 
			// ShowImageOnScreen_listBox
			// 
			this->ShowImageOnScreen_listBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->ShowImageOnScreen_listBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->ShowImageOnScreen_listBox->FormattingEnabled = true;
			this->ShowImageOnScreen_listBox->ItemHeight = 15;
			this->ShowImageOnScreen_listBox->Location = System::Drawing::Point(128, 21);
			this->ShowImageOnScreen_listBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->ShowImageOnScreen_listBox->MinimumSize = System::Drawing::Size(4, 93);
			this->ShowImageOnScreen_listBox->Name = L"ShowImageOnScreen_listBox";
			this->ShowImageOnScreen_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->ShowImageOnScreen_listBox->Size = System::Drawing::Size(187, 124);
			this->ShowImageOnScreen_listBox->TabIndex = 131;
			// 
			// DelShowImageOnScreenObject_button
			// 
			this->DelShowImageOnScreenObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelShowImageOnScreenObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelShowImageOnScreenObject_button->Location = System::Drawing::Point(33, 95);
			this->DelShowImageOnScreenObject_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DelShowImageOnScreenObject_button->Name = L"DelShowImageOnScreenObject_button";
			this->DelShowImageOnScreenObject_button->Size = System::Drawing::Size(51, 21);
			this->DelShowImageOnScreenObject_button->TabIndex = 132;
			this->DelShowImageOnScreenObject_button->Text = L"Del/刪除";
			this->DelShowImageOnScreenObject_button->UseVisualStyleBackColor = false;
			this->DelShowImageOnScreenObject_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddShowImageOnScreenObject_button_Click);
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"PMingLiU", 7.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(136)));
			this->label16->ForeColor = System::Drawing::Color::White;
			this->label16->Location = System::Drawing::Point(29, 32);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(77, 14);
			this->label16->TabIndex = 135;
			this->label16->Text = L"Hint/提示點";
			// 
			// AddShowImageOnScreenObject_button
			// 
			this->AddShowImageOnScreenObject_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddShowImageOnScreenObject_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddShowImageOnScreenObject_button->Location = System::Drawing::Point(33, 52);
			this->AddShowImageOnScreenObject_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->AddShowImageOnScreenObject_button->Name = L"AddShowImageOnScreenObject_button";
			this->AddShowImageOnScreenObject_button->Size = System::Drawing::Size(51, 22);
			this->AddShowImageOnScreenObject_button->TabIndex = 133;
			this->AddShowImageOnScreenObject_button->Text = L"Add/增加";
			this->AddShowImageOnScreenObject_button->UseVisualStyleBackColor = false;
			this->AddShowImageOnScreenObject_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddShowImageOnScreenObject_button_Click);
			// 
			// HintPoint_groupBox
			// 
			this->HintPoint_groupBox->Controls->Add(this->AllEventPointHint_listBox);
			this->HintPoint_groupBox->Controls->Add(this->HintPointType_comboBox);
			this->HintPoint_groupBox->Controls->Add(this->AddEventPOint_button);
			this->HintPoint_groupBox->Controls->Add(this->label17);
			this->HintPoint_groupBox->Controls->Add(this->EventPointName_textBox);
			this->HintPoint_groupBox->Controls->Add(this->DelEventPoint_button);
			this->HintPoint_groupBox->Location = System::Drawing::Point(3, 366);
			this->HintPoint_groupBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->HintPoint_groupBox->Name = L"HintPoint_groupBox";
			this->HintPoint_groupBox->Padding = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->HintPoint_groupBox->Size = System::Drawing::Size(357, 255);
			this->HintPoint_groupBox->TabIndex = 6;
			this->HintPoint_groupBox->TabStop = false;
			this->HintPoint_groupBox->Text = L"HintPoint";
			// 
			// AllEventPointHint_listBox
			// 
			this->AllEventPointHint_listBox->FormattingEnabled = true;
			this->AllEventPointHint_listBox->ItemHeight = 15;
			this->AllEventPointHint_listBox->Location = System::Drawing::Point(28, 112);
			this->AllEventPointHint_listBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->AllEventPointHint_listBox->Name = L"AllEventPointHint_listBox";
			this->AllEventPointHint_listBox->Size = System::Drawing::Size(287, 124);
			this->AllEventPointHint_listBox->TabIndex = 6;
			// 
			// HintPointType_comboBox
			// 
			this->HintPointType_comboBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->HintPointType_comboBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->HintPointType_comboBox->FormattingEnabled = true;
			this->HintPointType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"None", L"Sound", L"Particle"});
			this->HintPointType_comboBox->Location = System::Drawing::Point(79, 35);
			this->HintPointType_comboBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->HintPointType_comboBox->Name = L"HintPointType_comboBox";
			this->HintPointType_comboBox->Size = System::Drawing::Size(113, 23);
			this->HintPointType_comboBox->TabIndex = 0;
			// 
			// AddEventPOint_button
			// 
			this->AddEventPOint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->AddEventPOint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->AddEventPOint_button->Location = System::Drawing::Point(55, 65);
			this->AddEventPOint_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->AddEventPOint_button->Name = L"AddEventPOint_button";
			this->AddEventPOint_button->Size = System::Drawing::Size(75, 24);
			this->AddEventPOint_button->TabIndex = 4;
			this->AddEventPOint_button->Text = L"Add/增加";
			this->AddEventPOint_button->UseVisualStyleBackColor = false;
			this->AddEventPOint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddEventPOint_button_Click);
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->label17->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->label17->Location = System::Drawing::Point(13, 38);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(70, 15);
			this->label17->TabIndex = 2;
			this->label17->Text = L"Type/類別";
			// 
			// EventPointName_textBox
			// 
			this->EventPointName_textBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->EventPointName_textBox->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->EventPointName_textBox->Location = System::Drawing::Point(199, 35);
			this->EventPointName_textBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->EventPointName_textBox->Name = L"EventPointName_textBox";
			this->EventPointName_textBox->Size = System::Drawing::Size(109, 25);
			this->EventPointName_textBox->TabIndex = 1;
			// 
			// DelEventPoint_button
			// 
			this->DelEventPoint_button->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->DelEventPoint_button->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->DelEventPoint_button->Location = System::Drawing::Point(187, 65);
			this->DelEventPoint_button->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->DelEventPoint_button->Name = L"DelEventPoint_button";
			this->DelEventPoint_button->Size = System::Drawing::Size(75, 24);
			this->DelEventPoint_button->TabIndex = 5;
			this->DelEventPoint_button->Text = L"Del/刪除";
			this->DelEventPoint_button->UseVisualStyleBackColor = false;
			this->DelEventPoint_button->Click += gcnew System::EventHandler(this, &MPDIEditor::AddEventPOint_button_Click);
			// 
			// Video_groupBox
			// 
			this->Video_groupBox->Location = System::Drawing::Point(4, 627);
			this->Video_groupBox->Margin = System::Windows::Forms::Padding(4);
			this->Video_groupBox->Name = L"Video_groupBox";
			this->Video_groupBox->Padding = System::Windows::Forms::Padding(4);
			this->Video_groupBox->Size = System::Drawing::Size(361, 379);
			this->Video_groupBox->TabIndex = 7;
			this->Video_groupBox->TabStop = false;
			this->Video_groupBox->Text = L"Video";
			// 
			// CameraRotationCenter_label
			// 
			this->CameraRotationCenter_label->AutoSize = true;
			this->CameraRotationCenter_label->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenter_label->Location = System::Drawing::Point(272, 64);
			this->CameraRotationCenter_label->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->CameraRotationCenter_label->Name = L"CameraRotationCenter_label";
			this->CameraRotationCenter_label->Size = System::Drawing::Size(129, 15);
			this->CameraRotationCenter_label->TabIndex = 37;
			this->CameraRotationCenter_label->Text = L"CamraRotationCenter";
			// 
			// CameraRotationCenterY_numericUpDown
			// 
			this->CameraRotationCenterY_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationCenterY_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenterY_numericUpDown->Location = System::Drawing::Point(275, 122);
			this->CameraRotationCenterY_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CameraRotationCenterY_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->CameraRotationCenterY_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->CameraRotationCenterY_numericUpDown->Name = L"CameraRotationCenterY_numericUpDown";
			this->CameraRotationCenterY_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->CameraRotationCenterY_numericUpDown->TabIndex = 36;
			this->CameraRotationCenterY_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationCenterX_numericUpDown
			// 
			this->CameraRotationCenterX_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationCenterX_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenterX_numericUpDown->Location = System::Drawing::Point(276, 87);
			this->CameraRotationCenterX_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CameraRotationCenterX_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->CameraRotationCenterX_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->CameraRotationCenterX_numericUpDown->Name = L"CameraRotationCenterX_numericUpDown";
			this->CameraRotationCenterX_numericUpDown->Size = System::Drawing::Size(76, 25);
			this->CameraRotationCenterX_numericUpDown->TabIndex = 34;
			this->CameraRotationCenterX_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// CameraRotationCenterZ_numericUpDown
			// 
			this->CameraRotationCenterZ_numericUpDown->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), 
				static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->CameraRotationCenterZ_numericUpDown->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(244)), 
				static_cast<System::Int32>(static_cast<System::Byte>(244)), static_cast<System::Int32>(static_cast<System::Byte>(244)));
			this->CameraRotationCenterZ_numericUpDown->Location = System::Drawing::Point(275, 157);
			this->CameraRotationCenterZ_numericUpDown->Margin = System::Windows::Forms::Padding(4);
			this->CameraRotationCenterZ_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, 0});
			this->CameraRotationCenterZ_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {79979, 0, 0, System::Int32::MinValue});
			this->CameraRotationCenterZ_numericUpDown->Name = L"CameraRotationCenterZ_numericUpDown";
			this->CameraRotationCenterZ_numericUpDown->Size = System::Drawing::Size(77, 25);
			this->CameraRotationCenterZ_numericUpDown->TabIndex = 35;
			this->CameraRotationCenterZ_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &MPDIEditor::CameraRotationX_numericUpDown_ValueChanged);
			// 
			// MPDIEditor
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->splitContainer2);
			this->Controls->Add(this->menuStrip1);
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"MPDIEditor";
			this->Size = System::Drawing::Size(2560, 1332);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TotalPlayTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathLOD_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodeTime_numericUpDown))->EndInit();
			this->PointDataUtility_contextMenuStrip->ResumeLayout(false);
			this->GameUIData_menuStrip->ResumeLayout(false);
			this->PathGroup_contextMenuStrip->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurrentProgress_trackBar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->StartTime_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NumPointsToInsert_numericUpDown))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->RearrangeWholePathTime_numericUpDown))->EndInit();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ColorBlending_groupBox->ResumeLayout(false);
			this->ColorBlending_groupBox->PerformLayout();
			this->Rotation_groupBox->ResumeLayout(false);
			this->Rotation_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AngleY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AngleX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AngleZ_numericUpDown))->EndInit();
			this->Size_groupBox->ResumeLayout(false);
			this->Size_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SizeScale_numericUpDown))->EndInit();
			this->Color_groupBox->ResumeLayout(false);
			this->Color_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->RedColor_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GreenColor_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->AlphaColor_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->BlueColor_numericUpDown))->EndInit();
			this->Position_groupBox->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PathNodePosZ_numericUpDown))->EndInit();
			this->splitContainer3->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->flowLayoutPanel1->ResumeLayout(false);
			this->GroupData_collpaseForm->ResumeLayout(false);
			this->GroupData_collpaseForm->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupScale_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupRotationX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupPosY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupPosX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupPosZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupRotationZ_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->GroupRotationY_numericUpDown))->EndInit();
			this->AnimationData_collpaseForm->ResumeLayout(false);
			this->AnimationData_collpaseForm->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->flowLayoutPanel2->ResumeLayout(false);
			this->Path_groupBox->ResumeLayout(false);
			this->ImageHintPoint_groupBox->ResumeLayout(false);
			this->ImageHintPoint_groupBox->PerformLayout();
			this->HintPoint_groupBox->ResumeLayout(false);
			this->HintPoint_groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationCenterY_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationCenterX_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CameraRotationCenterZ_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void OpenImageFile_button_Click(System::Object^  sender, System::EventArgs^  e);

private: System::Void BKImage_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddPath_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholePath_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void DelPoint_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AssignImage_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AlphaColor_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void Play_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void TotalPlayTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddAnimationToList_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CurrentProgress_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e);
private: System::Void ApplyLODToPoints_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void BehaviorUpdateActive_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddShowImageOnScreenObject_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholePuzzleImageUnit_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void addPointModeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathPiorityUp_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void Mirror_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationList_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationList_listBox_DoubleClick(System::Object^  sender, System::EventArgs^  e);
private: System::Void EnableColorBlending_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationList_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void SrcBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void DestBlending_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathNode_listBox_MouseEnter(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathNode_listBox_MouseLeave(System::Object^  sender, System::EventArgs^  e);
private: System::Void openPathFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddPathIntoAllPath_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AddEventPOint_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void RearrangeWholePathTime_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void showImageBoderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void MergeMPDIData_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CurrentProgress_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void saveAviFileToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void showPath_ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void DelPI_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void AllPI_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void ApplyGroupDataChange_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void ScaleWithPIAlphaStripPixel_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void SelectFirstAndLast_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholeAnimationListUp_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void saveImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void WholePath_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void SelectAllPoints_button_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void renewImageSizeignoreAllAnimationWithSizeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void splitContainer3_Panel1_Resize(System::Object^  sender, System::EventArgs^  e);
private: System::Void PathNode_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
private: System::Void AddSliderBarUI_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void addPointDataWithImageByPathNameToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void FadeIn_toolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void mergeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void UseViewPort_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void ImageAnchor_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void CameraRotationX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e);
};
}
