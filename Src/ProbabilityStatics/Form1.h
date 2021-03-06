#pragma once


namespace ProbabilityStatics 
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			m_pDataAndCurveListWithXMlSechma = 0;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			UT::InitOpenGL((HWND)Render_panel->Handle.ToPointer(),true,GetDC((HWND)Render_panel->Handle.ToPointer()));
			m_pGameApp = new cGameApp(0);
			m_pGameApp->Init();
			cGameApp::m_sHdc = GetDC((HWND)Render_panel->Handle.ToPointer());
			XMLSechma_textBox->Text = "Fish/Probability/ProbabilityXMLSechma.xml";
			StaticsFile_textBox->Text = "Log/FishProbability2012_12_18_17_37_7.txt";
			MaxMinValueType_listBox->SelectedIndex = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			SAFE_DELETE(m_pDataAndCurveListWithXMlSechma);
			SAFE_DELETE(m_pGameApp);
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Timer^				timer1;
	private: System::Windows::Forms::TextBox^			XMLSechma_textBox;
	private: System::Windows::Forms::TextBox^			StaticsFile_textBox;
	private: System::Windows::Forms::ListBox^			SechmaMember_listBox;
	private: System::Windows::Forms::Panel^				Render_panel;
	private: System::Windows::Forms::Button^			Apply_button;
	private: System::ComponentModel::IContainer^		components;
	private: System::Windows::Forms::SplitContainer^	splitContainer1;
	private: System::Windows::Forms::CheckBox^			ShowMaxForAll_checkBox;
	private: System::Windows::Forms::NumericUpDown^		numericUpDown1;
	private: System::Windows::Forms::ListBox^			MaxMinValueType_listBox;
	private: System::Windows::Forms::Label^				XMLSechma_label;
	private: System::Windows::Forms::Label^				StaticsFile_label;
	private: System::Windows::Forms::Label^				label1;
			cDataAndCurveListWithXMlSechma*				m_pDataAndCurveListWithXMlSechma;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  MaxMinScale_numericUpDown;
			 cGameApp*m_pGameApp;
	private:
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
			this->XMLSechma_textBox = (gcnew System::Windows::Forms::TextBox());
			this->StaticsFile_textBox = (gcnew System::Windows::Forms::TextBox());
			this->XMLSechma_label = (gcnew System::Windows::Forms::Label());
			this->StaticsFile_label = (gcnew System::Windows::Forms::Label());
			this->SechmaMember_listBox = (gcnew System::Windows::Forms::ListBox());
			this->Render_panel = (gcnew System::Windows::Forms::Panel());
			this->Apply_button = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->MaxMinValueType_listBox = (gcnew System::Windows::Forms::ListBox());
			this->ShowMaxForAll_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->MaxMinScale_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaxMinScale_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// XMLSechma_textBox
			// 
			this->XMLSechma_textBox->AllowDrop = true;
			this->XMLSechma_textBox->Location = System::Drawing::Point(13, 40);
			this->XMLSechma_textBox->Name = L"XMLSechma_textBox";
			this->XMLSechma_textBox->Size = System::Drawing::Size(141, 25);
			this->XMLSechma_textBox->TabIndex = 0;
			this->XMLSechma_textBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::XMLSechma_textBox_DragDrop);
			this->XMLSechma_textBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::XMLSechma_textBox_DragEnter);
			// 
			// StaticsFile_textBox
			// 
			this->StaticsFile_textBox->AllowDrop = true;
			this->StaticsFile_textBox->Location = System::Drawing::Point(12, 479);
			this->StaticsFile_textBox->Name = L"StaticsFile_textBox";
			this->StaticsFile_textBox->Size = System::Drawing::Size(154, 25);
			this->StaticsFile_textBox->TabIndex = 1;
			this->StaticsFile_textBox->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::StaticsFile_textBox_DragDrop);
			this->StaticsFile_textBox->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &Form1::XMLSechma_textBox_DragEnter);
			// 
			// XMLSechma_label
			// 
			this->XMLSechma_label->AutoSize = true;
			this->XMLSechma_label->Location = System::Drawing::Point(17, 18);
			this->XMLSechma_label->Name = L"XMLSechma_label";
			this->XMLSechma_label->Size = System::Drawing::Size(83, 15);
			this->XMLSechma_label->TabIndex = 2;
			this->XMLSechma_label->Text = L"XMLSechma";
			// 
			// StaticsFile_label
			// 
			this->StaticsFile_label->AutoSize = true;
			this->StaticsFile_label->Location = System::Drawing::Point(9, 461);
			this->StaticsFile_label->Name = L"StaticsFile_label";
			this->StaticsFile_label->Size = System::Drawing::Size(66, 15);
			this->StaticsFile_label->TabIndex = 3;
			this->StaticsFile_label->Text = L"StaticsFile";
			// 
			// SechmaMember_listBox
			// 
			this->SechmaMember_listBox->FormattingEnabled = true;
			this->SechmaMember_listBox->ItemHeight = 15;
			this->SechmaMember_listBox->Location = System::Drawing::Point(12, 82);
			this->SechmaMember_listBox->Name = L"SechmaMember_listBox";
			this->SechmaMember_listBox->SelectionMode = System::Windows::Forms::SelectionMode::MultiExtended;
			this->SechmaMember_listBox->Size = System::Drawing::Size(149, 364);
			this->SechmaMember_listBox->TabIndex = 4;
			// 
			// Render_panel
			// 
			this->Render_panel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->Render_panel->Location = System::Drawing::Point(0, 0);
			this->Render_panel->Name = L"Render_panel";
			this->Render_panel->Size = System::Drawing::Size(1245, 686);
			this->Render_panel->TabIndex = 6;
			// 
			// Apply_button
			// 
			this->Apply_button->Location = System::Drawing::Point(12, 517);
			this->Apply_button->Name = L"Apply_button";
			this->Apply_button->Size = System::Drawing::Size(149, 166);
			this->Apply_button->TabIndex = 7;
			this->Apply_button->Text = L"Apply";
			this->Apply_button->UseVisualStyleBackColor = true;
			this->Apply_button->Click += gcnew System::EventHandler(this, &Form1::Apply_button_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->MaxMinScale_numericUpDown);
			this->splitContainer1->Panel1->Controls->Add(this->MaxMinValueType_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->ShowMaxForAll_checkBox);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->numericUpDown1);
			this->splitContainer1->Panel1->Controls->Add(this->XMLSechma_label);
			this->splitContainer1->Panel1->Controls->Add(this->Apply_button);
			this->splitContainer1->Panel1->Controls->Add(this->XMLSechma_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->SechmaMember_listBox);
			this->splitContainer1->Panel1->Controls->Add(this->StaticsFile_textBox);
			this->splitContainer1->Panel1->Controls->Add(this->StaticsFile_label);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->Render_panel);
			this->splitContainer1->Size = System::Drawing::Size(1590, 686);
			this->splitContainer1->SplitterDistance = 341;
			this->splitContainer1->TabIndex = 8;
			// 
			// MaxMinValueType_listBox
			// 
			this->MaxMinValueType_listBox->FormattingEnabled = true;
			this->MaxMinValueType_listBox->ItemHeight = 15;
			this->MaxMinValueType_listBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"All", L"SelectedItems"});
			this->MaxMinValueType_listBox->Location = System::Drawing::Point(182, 138);
			this->MaxMinValueType_listBox->Name = L"MaxMinValueType_listBox";
			this->MaxMinValueType_listBox->Size = System::Drawing::Size(118, 49);
			this->MaxMinValueType_listBox->TabIndex = 11;
			// 
			// ShowMaxForAll_checkBox
			// 
			this->ShowMaxForAll_checkBox->AutoSize = true;
			this->ShowMaxForAll_checkBox->Checked = true;
			this->ShowMaxForAll_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->ShowMaxForAll_checkBox->Location = System::Drawing::Point(179, 100);
			this->ShowMaxForAll_checkBox->Name = L"ShowMaxForAll_checkBox";
			this->ShowMaxForAll_checkBox->Size = System::Drawing::Size(122, 19);
			this->ShowMaxForAll_checkBox->TabIndex = 10;
			this->ShowMaxForAll_checkBox->Text = L"ShowMaxForAll";
			this->ShowMaxForAll_checkBox->UseVisualStyleBackColor = true;
			this->ShowMaxForAll_checkBox->CheckedChanged += gcnew System::EventHandler(this, &Form1::ShowMaxForAll_checkBox_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(173, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(94, 15);
			this->label1->TabIndex = 9;
			this->label1->Text = L"ParameterScale";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(176, 41);
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(120, 25);
			this->numericUpDown1->TabIndex = 8;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(179, 205);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(87, 15);
			this->label2->TabIndex = 13;
			this->label2->Text = L"MaxMinScale";
			// 
			// MaxMinScale_numericUpDown
			// 
			this->MaxMinScale_numericUpDown->Location = System::Drawing::Point(182, 223);
			this->MaxMinScale_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->MaxMinScale_numericUpDown->Name = L"MaxMinScale_numericUpDown";
			this->MaxMinScale_numericUpDown->Size = System::Drawing::Size(120, 25);
			this->MaxMinScale_numericUpDown->TabIndex = 12;
			this->MaxMinScale_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			this->MaxMinScale_numericUpDown->ValueChanged += gcnew System::EventHandler(this, &Form1::MaxMinScale_numericUpDown_ValueChanged);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1590, 686);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->MaxMinScale_numericUpDown))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Apply_button_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 this->timer1->Enabled = false;
				System::Threading::Thread::Sleep(10);
				 if( XMLSechma_textBox->Text->Length && StaticsFile_textBox->Text->Length )
				 {
					 if( System::IO::File::Exists(XMLSechma_textBox->Text) && System::IO::File::Exists(StaticsFile_textBox->Text) )
					 {
						SechmaMember_listBox->Items->Clear();
						SAFE_DELETE(m_pDataAndCurveListWithXMlSechma);
						std::string	l_strBinary = DNCT::GcStringToChar(StaticsFile_textBox->Text);
						std::string	l_strXmlSechma = DNCT::GcStringToChar(XMLSechma_textBox->Text);
						m_pDataAndCurveListWithXMlSechma = new cDataAndCurveListWithXMlSechma(l_strXmlSechma.c_str(),l_strBinary.c_str());
						int	l_iCount = m_pDataAndCurveListWithXMlSechma->Count();
						std::string	l_str;
						cBinaryFile*l_pBinaryFile = new cBinaryFile();
						l_pBinaryFile->Writefile("StaticsFile.txt",false,false);
						for( int i=0;i<l_iCount;++i )
						{
							cDataAndCurve*l_pDataAndCurve = m_pDataAndCurveListWithXMlSechma->GetObject(i);
							cDataAndCurve::sDataAndCount*l_pDataAndCount = l_pDataAndCurve->GetDataAndCount();
							SechmaMember_listBox->Items->Add(DNCT::WcharToGcstring(l_pDataAndCurve->GetName()));
							l_pBinaryFile->WriteToFile(UT::WcharToChar(l_pDataAndCurve->GetName()));
							l_pBinaryFile->WriteToFile(":");
							for( int j=0;j<l_pDataAndCount->iOriginalCount;++j )
							{
								l_pBinaryFile->WriteToFile(ValueToString(l_pDataAndCount->pdbOriginalfData[j]));
								l_pBinaryFile->WriteToFile(",");
							}
							l_pBinaryFile->WriteToFile("\n");
							//l_pDataAndCurve->CalMinMaxValue();
						}
						delete l_pBinaryFile;
						m_pDataAndCurveListWithXMlSechma->CalMinMaxValue();
					 }
				 }
				 this->timer1->Enabled = true;
			 }

private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
		 {
			Vector2	l_vGameResolution(1900.f,1060.f);
			Vector4	l_vViewRect( 10,10,l_vGameResolution.x,l_vGameResolution.y );
			m_pGameApp->Run();
			glViewport(0,0,Render_panel->Width,Render_panel->Height);
			//glScissor(0,0,(int)m_svViewPortSize.x,(int)m_svViewPortSize.y);
			glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
			glClearColor( 0,0.0,0.0,1 );
			glClearDepth(1.0f);	
			glEnable(GL_TEXTURE_2D);
			glEnable2D(1920,1080);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			//glAlphaFunc(GL_GREATER,0.001f);
			glEnable2D(1920,1080);
			#define	ALL_COLOR_COUNT		25
			Vector4	l_vColor[ALL_COLOR_COUNT] = {
				Vector4::One,
				Vector4::Red,
				Vector4::Green,
				Vector4::Blue,

				Vector4(255/255.f,255/255.f,128/255.f,1.f),
				Vector4(255/255.f,128/255.f,255/255.f,1.f),
				Vector4(128/255.f,255/255.f,255/255.f,1.f),

				Vector4(255/255.f,255/255.f,200/255.f,1.f),
				Vector4(255/255.f,200/255.f,255/255.f,1.f),
				Vector4(200/255.f,255/255.f,255/255.f,1.f),

				Vector4(255/255.f,255/255.f, 64/255.f,1.f),
				Vector4(255/255.f, 64/255.f,255/255.f,1.f),
				Vector4( 64/255.f,255/255.f,255/255.f,1.f),


				Vector4(200/255.f,200/255.f,200/255.f,1.f),
				Vector4(128/255.f,128/255.f,128/255.f,1.f),
				Vector4(64/255.f,64/255.f,64/255.f,1.f),

				Vector4(255/255.f,128/255.f,128/255.f,1.f),
				Vector4(255/255.f,200/255.f,200/255.f,1.f),
				Vector4(255/255.f, 64/255.f, 64/255.f,1.f),

				Vector4(128/255.f,128/255.f,255/255.f,1.f),
				Vector4(200/255.f,200/255.f,255/255.f,1.f),
				Vector4( 64/255.f, 64/255.f,255/255.f,1.f),

				Vector4(128/255.f,255/255.f,128/255.f,1.f),
				Vector4(200/255.f,255/255.f,200/255.f,1.f),
				Vector4(64 /255.f,255/255.f, 64/255.f,1.f),};
			if( m_pDataAndCurveListWithXMlSechma )
			{
				if(m_pDataAndCurveListWithXMlSechma->Count())
				{
					int	l_iColorIndex = 0;
					for(int i=0;i<SechmaMember_listBox->SelectedIndices->Count;++i)
					{
						int	l_iTargetIndex = SechmaMember_listBox->SelectedIndices[i];
						m_pDataAndCurveListWithXMlSechma->Render((float)this->MaxMinScale_numericUpDown->Value/100.f,l_iTargetIndex,l_vViewRect,l_vColor[l_iTargetIndex],i==0?true:false);


						++l_iColorIndex;
						if( l_iColorIndex>=ALL_COLOR_COUNT )
							l_iColorIndex = 0;
					}
				}
			}
			//WCHAR   l_str[MAX_PATH];
			//swprintf(l_str,MAX_PATH,L"MousePos:%d,%d",cGameApp::m_sMousePosition.x,cGameApp::m_sMousePosition.y);
			//cGameApp::m_spGlyphFontRender->RenderFont(0,0,l_str);
			//cGameApp::m_spGlyphFontRender->RenderFont(0,20,UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()));
			glDisable(GL_TEXTURE_2D);
			SwapBuffers(cGameApp::m_sHdc);
		 }

private: System::Void ShowMaxForAll_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->timer1->Enabled = false;
			 if(ShowMaxForAll_checkBox->Checked)
			 {
				if( m_pDataAndCurveListWithXMlSechma )
				{
					if( MaxMinValueType_listBox->SelectedIndex == 0 )
					{
						m_pDataAndCurveListWithXMlSechma->CalMinMaxValue();
					}
					else
					if( MaxMinValueType_listBox->SelectedIndex == 1 )
					{
						std::vector<int>	l_iIndexVector;
						for(int i=0;i<SechmaMember_listBox->SelectedIndices->Count;++i)
						{
							int	l_iSelectedIndex = SechmaMember_listBox->SelectedIndices[i];
							l_iIndexVector.push_back(l_iSelectedIndex);
						}
						m_pDataAndCurveListWithXMlSechma->CalMinMaxValue(l_iIndexVector);
					}
				}
			 }
			 else
			 {
				for(int i=0;i<m_pDataAndCurveListWithXMlSechma->Count();++i)
				{
					m_pDataAndCurveListWithXMlSechma->GetObject(i)->CalMinMaxValue();
				}
			 }
			 this->timer1->Enabled = true;
		 }

void	Scale(int e_iValue)
		{
			 this->timer1->Enabled = false;
			 System::Threading::Thread::Sleep(100);
			if( m_pDataAndCurveListWithXMlSechma )
			{
				float	l_fValue = e_iValue/100.f;
				for(int i=0;i<m_pDataAndCurveListWithXMlSechma->Count();++i)
				{
					m_pDataAndCurveListWithXMlSechma->GetObject(i)->Compress(l_fValue);
				}
			}
			this->timer1->Enabled = true;
		}

private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			Scale((int)numericUpDown1->Value);
		 }

private: System::Void XMLSechma_textBox_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) 
		 {
			 if ( e->Data->GetDataPresent( DataFormats::FileDrop ) )
			 {
				e->Effect = DragDropEffects::Copy;
			 }
			 else
				 e->Effect = DragDropEffects::None;
		 }

private: System::Void StaticsFile_textBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
		 {
			array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				StaticsFile_textBox->Text = l_str;
			}
		 }

private: System::Void XMLSechma_textBox_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) 
		 {
			array<String^>^l_strFileNames = DNCT::DragDropEventWhileFileDrop(e);
			for each(String^l_str in l_strFileNames)
			{
				XMLSechma_textBox->Text = l_str;
			}
		 }
private: System::Void MaxMinScale_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {

		 }
};
}

