#pragma once
#include "../../DotNetCommon/DotNetCommonTools.h"
#include <d3d9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <D3dx9tex.h>
#include "dds.h"


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9         g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9   g_pd3dDevice = NULL; // Our rendering device




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

namespace TextureScale {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 的摘要
	///
	/// 警告: 如果您變更這個類別的名稱，就必須變更與這個類別所依據之所有 .resx 檔案關聯的
	///          Managed 資源編譯器工具的 'Resource File Name' 屬性。
	///          否則，這些設計工具
	///          將無法與這個表單關聯的當地語系化資源
	///          正確互動。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: 在此加入建構函式程式碼
			//
			InitD3D((HWND)panel1->Handle.ToPointer());
			Format_comboBox->SelectedIndex = 4;
			TransformType_comboBox->SelectedIndex = 0;
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			Cleanup();
		}
	private: System::Windows::Forms::ListBox^  AllFile_listBox;
	protected: 


	private: System::Windows::Forms::Button^  AddFile_button;
	private: System::Windows::Forms::Button^  DeleteFile_button;
	private: System::Windows::Forms::Button^  Export_button;
	private: System::Windows::Forms::NumericUpDown^  Width_numericUpDown;
	private: System::Windows::Forms::NumericUpDown^  Height_numericUpDown;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  RecursivelyAddFile_button;
	private: System::Windows::Forms::ComboBox^  Format_comboBox;




	private: System::Windows::Forms::ComboBox^  TransformType_comboBox;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::CheckBox^  ReplaceOriginalFile_checkBox;
	private: System::Windows::Forms::CheckBox^  CreateFolderIfNotReplaceOriginalFile_checkBox;
//my
	String^m_strDelectedDirectory;
	private: System::Windows::Forms::Label^  Progress_label;
	private: System::Windows::Forms::TextBox^  ErrorMsg_textBox;


	protected:
	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
		///
		/// </summary>
		void InitializeComponent(void)
		{
			this->AllFile_listBox = (gcnew System::Windows::Forms::ListBox());
			this->AddFile_button = (gcnew System::Windows::Forms::Button());
			this->DeleteFile_button = (gcnew System::Windows::Forms::Button());
			this->Export_button = (gcnew System::Windows::Forms::Button());
			this->Width_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->Height_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->RecursivelyAddFile_button = (gcnew System::Windows::Forms::Button());
			this->Format_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->TransformType_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->ReplaceOriginalFile_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->CreateFolderIfNotReplaceOriginalFile_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->Progress_label = (gcnew System::Windows::Forms::Label());
			this->ErrorMsg_textBox = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// AllFile_listBox
			// 
			this->AllFile_listBox->FormattingEnabled = true;
			this->AllFile_listBox->HorizontalScrollbar = true;
			this->AllFile_listBox->ItemHeight = 15;
			this->AllFile_listBox->Location = System::Drawing::Point(34, 12);
			this->AllFile_listBox->Name = L"AllFile_listBox";
			this->AllFile_listBox->Size = System::Drawing::Size(412, 334);
			this->AllFile_listBox->TabIndex = 0;
			// 
			// AddFile_button
			// 
			this->AddFile_button->Location = System::Drawing::Point(61, 440);
			this->AddFile_button->Name = L"AddFile_button";
			this->AddFile_button->Size = System::Drawing::Size(65, 25);
			this->AddFile_button->TabIndex = 2;
			this->AddFile_button->Text = L"AddFile";
			this->AddFile_button->UseVisualStyleBackColor = true;
			// 
			// DeleteFile_button
			// 
			this->DeleteFile_button->Location = System::Drawing::Point(141, 440);
			this->DeleteFile_button->Name = L"DeleteFile_button";
			this->DeleteFile_button->Size = System::Drawing::Size(65, 25);
			this->DeleteFile_button->TabIndex = 3;
			this->DeleteFile_button->Text = L"Delete";
			this->DeleteFile_button->UseVisualStyleBackColor = true;
			// 
			// Export_button
			// 
			this->Export_button->Location = System::Drawing::Point(234, 440);
			this->Export_button->Name = L"Export_button";
			this->Export_button->Size = System::Drawing::Size(65, 25);
			this->Export_button->TabIndex = 4;
			this->Export_button->Text = L"Export";
			this->Export_button->UseVisualStyleBackColor = true;
			this->Export_button->Click += gcnew System::EventHandler(this, &Form1::Export_button_Click);
			// 
			// Width_numericUpDown
			// 
			this->Width_numericUpDown->Location = System::Drawing::Point(424, 364);
			this->Width_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {8096, 0, 0, 0});
			this->Width_numericUpDown->Name = L"Width_numericUpDown";
			this->Width_numericUpDown->Size = System::Drawing::Size(108, 25);
			this->Width_numericUpDown->TabIndex = 5;
			this->Width_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// Height_numericUpDown
			// 
			this->Height_numericUpDown->Location = System::Drawing::Point(424, 415);
			this->Height_numericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {8096, 0, 0, 0});
			this->Height_numericUpDown->Name = L"Height_numericUpDown";
			this->Height_numericUpDown->Size = System::Drawing::Size(108, 25);
			this->Height_numericUpDown->TabIndex = 6;
			this->Height_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(347, 364);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 15);
			this->label1->TabIndex = 7;
			this->label1->Text = L"Width";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(344, 415);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 15);
			this->label2->TabIndex = 8;
			this->label2->Text = L"Height";
			// 
			// RecursivelyAddFile_button
			// 
			this->RecursivelyAddFile_button->Location = System::Drawing::Point(61, 364);
			this->RecursivelyAddFile_button->Name = L"RecursivelyAddFile_button";
			this->RecursivelyAddFile_button->Size = System::Drawing::Size(238, 39);
			this->RecursivelyAddFile_button->TabIndex = 10;
			this->RecursivelyAddFile_button->Text = L"AddFileRecursively";
			this->RecursivelyAddFile_button->UseVisualStyleBackColor = true;
			this->RecursivelyAddFile_button->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Format_comboBox
			// 
			this->Format_comboBox->FormattingEnabled = true;
			this->Format_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(10) {L"BMP", L"JPG", L"TGA", L"PNG", L"DDS", L"PPM", 
				L"DIB", L"HDR", L"PFM", L"All Image Support"});
			this->Format_comboBox->Location = System::Drawing::Point(424, 468);
			this->Format_comboBox->Name = L"Format_comboBox";
			this->Format_comboBox->Size = System::Drawing::Size(108, 23);
			this->Format_comboBox->TabIndex = 11;
			// 
			// TransformType_comboBox
			// 
			this->TransformType_comboBox->FormattingEnabled = true;
			this->TransformType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"PercentScale", L"FixedSize"});
			this->TransformType_comboBox->Location = System::Drawing::Point(424, 516);
			this->TransformType_comboBox->Name = L"TransformType_comboBox";
			this->TransformType_comboBox->Size = System::Drawing::Size(108, 23);
			this->TransformType_comboBox->TabIndex = 12;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(293, 524);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(96, 15);
			this->label3->TabIndex = 13;
			this->label3->Text = L"TransformType";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(341, 476);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(48, 15);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Format";
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(93, 477);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(51, 62);
			this->panel1->TabIndex = 15;
			// 
			// ReplaceOriginalFile_checkBox
			// 
			this->ReplaceOriginalFile_checkBox->AutoSize = true;
			this->ReplaceOriginalFile_checkBox->Location = System::Drawing::Point(551, 516);
			this->ReplaceOriginalFile_checkBox->Name = L"ReplaceOriginalFile_checkBox";
			this->ReplaceOriginalFile_checkBox->Size = System::Drawing::Size(139, 19);
			this->ReplaceOriginalFile_checkBox->TabIndex = 16;
			this->ReplaceOriginalFile_checkBox->Text = L"ReplaceOriginalFile";
			this->ReplaceOriginalFile_checkBox->UseVisualStyleBackColor = true;
			// 
			// CreateFolderIfNotReplaceOriginalFile_checkBox
			// 
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->AutoSize = true;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Checked = true;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Location = System::Drawing::Point(716, 516);
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Name = L"CreateFolderIfNotReplaceOriginalFile_checkBox";
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Size = System::Drawing::Size(243, 19);
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->TabIndex = 17;
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->Text = L"CreateFolderIfNotReplaceOriginalFile";
			this->CreateFolderIfNotReplaceOriginalFile_checkBox->UseVisualStyleBackColor = true;
			// 
			// Progress_label
			// 
			this->Progress_label->AutoSize = true;
			this->Progress_label->Location = System::Drawing::Point(694, 468);
			this->Progress_label->Name = L"Progress_label";
			this->Progress_label->Size = System::Drawing::Size(55, 15);
			this->Progress_label->TabIndex = 18;
			this->Progress_label->Text = L"Progress";
			// 
			// ErrorMsg_textBox
			// 
			this->ErrorMsg_textBox->Location = System::Drawing::Point(477, 24);
			this->ErrorMsg_textBox->Multiline = true;
			this->ErrorMsg_textBox->Name = L"ErrorMsg_textBox";
			this->ErrorMsg_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->ErrorMsg_textBox->Size = System::Drawing::Size(481, 321);
			this->ErrorMsg_textBox->TabIndex = 19;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(985, 560);
			this->Controls->Add(this->ErrorMsg_textBox);
			this->Controls->Add(this->Progress_label);
			this->Controls->Add(this->CreateFolderIfNotReplaceOriginalFile_checkBox);
			this->Controls->Add(this->ReplaceOriginalFile_checkBox);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->TransformType_comboBox);
			this->Controls->Add(this->Format_comboBox);
			this->Controls->Add(this->RecursivelyAddFile_button);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Height_numericUpDown);
			this->Controls->Add(this->Width_numericUpDown);
			this->Controls->Add(this->Export_button);
			this->Controls->Add(this->DeleteFile_button);
			this->Controls->Add(this->AddFile_button);
			this->Controls->Add(this->AllFile_listBox);
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Width_numericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->Height_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 AllFile_listBox->Items->Clear();
				 String^l_strDirectoryName = DNCT::SelectDirectory();
				 m_strDelectedDirectory = l_strDirectoryName;
				 if( l_strDirectoryName )
				 {
					 System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList();
					 GetFilesNameByRecursivelyDirectory(l_strDirectoryName,l_pArrayList);
					 char*l_strAllSupportFormat[] = {
						".BMP",".JPG",
						".TGA",".PNG",".DDS",".PPM",
						".DIB",".HDR",".PFM",
						"All Image Support"
					 };
					 for each(String^l_strName in l_pArrayList)
					 {
						String^l_strExtensionName = System::IO::Path::GetExtension(l_strName);
						char*l_strFileExtensionName = DNCT::GcStringToChar(l_strExtensionName);
						if( Format_comboBox->SelectedIndex!=9 )
						{
							if(!_stricmp(l_strAllSupportFormat[Format_comboBox->SelectedIndex],l_strFileExtensionName))
							//if( l_strExtensionName->Equals(l_strAllSupportFormat[Format_comboBox->SelectedIndex]) )
							{
								AllFile_listBox->Items->Add(l_strName);
							}
						}
						else
						{
							for(int i=0;i<9;++i)
							{
								if(!_stricmp(l_strAllSupportFormat[i],l_strFileExtensionName))
								//if( l_strExtensionName->Equals(l_strAllSupportFormat[i]) )
								{
									AllFile_listBox->Items->Add(l_strName);
								}
							}
						}
					 }
				 }
		 }

 int	ToMaxMipMap(int e_iWidth,int e_iHeight)
 {
	if( e_iWidth < 4||e_iHeight<4 )
		return 0;
	int	l_iMinmap = 0;
	int	l_i = 1;
	while( l_i<e_iWidth )
	{
		l_i = l_i<<1;
		l_iMinmap++;
	}
	int	l_iMinmap2 = 0;
	int	l_i2 = 1;
	while( l_i2<e_iHeight )
	{
		l_i2 = l_i2<<1;
		l_iMinmap2++;
	}
	return min(l_iMinmap2,l_iMinmap);
 }

LPDIRECT3DBASETEXTURE9 Resize(DWORD dwWidthNew, DWORD dwHeightNew,LPDIRECT3DBASETEXTURE9 pSrctex,DWORD e_dwMipMap,bool e_bIsVolumeMap,bool e_bIsCueMap)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pmiptexNew;
	if( e_dwMipMap >1 )
	{
		int	l_iMaxMipMap = ToMaxMipMap(dwWidthNew,dwHeightNew);
		if( (DWORD)l_iMaxMipMap<e_dwMipMap )
			e_dwMipMap = l_iMaxMipMap;
	}
    hr = g_pd3dDevice->CreateTexture(dwWidthNew, dwHeightNew, e_dwMipMap,
         0, GetFormat(pSrctex,e_bIsVolumeMap,e_bIsCueMap), D3DPOOL_MANAGED, &pmiptexNew, NULL);
    if (FAILED(hr))
	{
		e_dwMipMap--;
	}
    hr = g_pd3dDevice->CreateTexture(dwWidthNew, dwHeightNew, e_dwMipMap,
         0, GetFormat(pSrctex,e_bIsVolumeMap,e_bIsCueMap), D3DPOOL_MANAGED, &pmiptexNew, NULL);
    if (FAILED(hr))
	{
		return 0;
	}
    if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, pSrctex, pmiptexNew,e_dwMipMap,e_bIsVolumeMap,e_bIsCueMap)))
        return 0;
	
    return pmiptexNew;

    //if( m_ptexNew != NULL )
    //{
    //    hr = g_pd3dDevice->CreateTexture(dwWidthNew, dwHeightNew, m_numMips, 
    //         0, GetFormat(m_ptexOrig), D3DPOOL_MANAGED, &pmiptexNew, NULL);
    //    if (FAILED(hr))
    //        return hr;
    //    if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, m_ptexNew, pmiptexNew)))
    //        return hr;
    //    ReleasePpo(&m_ptexNew);
    //    m_ptexNew = pmiptexNew;
    //}
}
D3DFORMAT GetFormat(LPDIRECT3DBASETEXTURE9 ptex,bool e_bIsVolumeMap,bool e_bIsCueMap)
{
    LPDIRECT3DTEXTURE9 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE9 pcubetex = NULL;
    LPDIRECT3DVOLUMETEXTURE9 pvoltex = NULL;
    D3DFORMAT fmt = D3DFMT_UNKNOWN;

    //if (IsVolumeMap())
	if( e_bIsVolumeMap )
        pvoltex = (LPDIRECT3DVOLUMETEXTURE9)ptex;
    //else if (IsCubeMap())
	else if( e_bIsCueMap )
        pcubetex = (LPDIRECT3DCUBETEXTURE9)ptex;
    else
        pmiptex = (LPDIRECT3DTEXTURE9)ptex;

    if (pvoltex != NULL)
    {
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmt = vd.Format;
    }
    else if (pcubetex != NULL)
    {
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    else if( pmiptex != NULL )
    {
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    return fmt;
}

HRESULT BltAllLevels(D3DCUBEMAP_FACES FaceType, 
    LPDIRECT3DBASETEXTURE9 ptexSrc, LPDIRECT3DBASETEXTURE9 ptexDest,DWORD e_dwNumMips,bool e_bIsVolumeMap,bool e_bIsCueMap)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pmiptexSrc;
    LPDIRECT3DTEXTURE9 pmiptexDest;
    LPDIRECT3DCUBETEXTURE9 pcubetexSrc;
    LPDIRECT3DCUBETEXTURE9 pcubetexDest;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexSrc;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexDest;
    DWORD iLevel;

    if (e_bIsVolumeMap)
    {
        pvoltexSrc = (LPDIRECT3DVOLUMETEXTURE9)ptexSrc;
        pvoltexDest = (LPDIRECT3DVOLUMETEXTURE9)ptexDest;
    }
    else if (e_bIsCueMap)
    {
        pcubetexSrc = (LPDIRECT3DCUBETEXTURE9)ptexSrc;
        pcubetexDest = (LPDIRECT3DCUBETEXTURE9)ptexDest;
    }
    else
    {
        pmiptexSrc = (LPDIRECT3DTEXTURE9)ptexSrc;
        pmiptexDest = (LPDIRECT3DTEXTURE9)ptexDest;
    }

    for (iLevel = 0; iLevel < e_dwNumMips; iLevel++)
    {
		if( e_bIsVolumeMap )
        //if (IsVolumeMap())
        {
            LPDIRECT3DVOLUME9 pvolSrc = NULL;
            LPDIRECT3DVOLUME9 pvolDest = NULL;
            hr = pvoltexSrc->GetVolumeLevel(iLevel, &pvolSrc);
            hr = pvoltexDest->GetVolumeLevel(iLevel, &pvolDest);
            hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, 
                pvolSrc, NULL, NULL, D3DX_DEFAULT, 0);
			if(pvolSrc)
				pvolSrc->Release();
            if(pvolDest)
				pvolDest->Release();
        }
		if( e_bIsCueMap )
        //else if (IsCubeMap())
        {
            LPDIRECT3DSURFACE9 psurfSrc = NULL;
            LPDIRECT3DSURFACE9 psurfDest = NULL;
            hr = pcubetexSrc->GetCubeMapSurface(FaceType, iLevel, &psurfSrc);
            hr = pcubetexDest->GetCubeMapSurface(FaceType, iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
			if(psurfSrc)
				psurfSrc->Release();
            if(psurfDest)
				psurfDest->Release();
        }
        else
        {
            LPDIRECT3DSURFACE9 psurfSrc = NULL;
            LPDIRECT3DSURFACE9 psurfDest = NULL;
            hr = pmiptexSrc->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = pmiptexDest->GetSurfaceLevel(iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
            if(psurfSrc)
				psurfSrc->Release();
            if(psurfDest)
				psurfDest->Release();
        }
    }

    return S_OK;
}
private: System::Void Export_button_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 ErrorMsg_textBox->Text = "";
			 Progress_label->Text = "0";
			 String^l_strExtensionName;
			 String^l_strDirectoryName = this->m_strDelectedDirectory;
			 if( !ReplaceOriginalFile_checkBox->Checked )
				l_strDirectoryName = DNCT::SelectDirectory();
			 if( !l_strDirectoryName )
				 return;
			 D3DXIMAGE_FILEFORMAT	l_D3DXIMAGE_FILEFORMAT = D3DXIFF_DDS;
			 switch(Format_comboBox->SelectedIndex)
			 {
				case 0:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_BMP;
					l_strExtensionName = "bmp";
					break;
				case 1:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_JPG;
					l_strExtensionName = "jpg";
				break;
				case 2:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_TGA;
					l_strExtensionName = "tga";
				break;
				case 3:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_PNG;
					l_strExtensionName = "png";
				break;
				case 4:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_DDS;
					l_strExtensionName = "dds";
				break;
				case 5:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_PPM;
					l_strExtensionName = "ppm";
				break;
				case 6:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_DIB;
					l_strExtensionName = "dib";
				break;
				case 7:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_HDR;
					l_strExtensionName = "hdr";
				break;
				case 8:
					l_D3DXIMAGE_FILEFORMAT = D3DXIFF_PFM;
					l_strExtensionName = "pfm";
				break;
			 }
			 for(int i=0;i<AllFile_listBox->Items->Count;++i)
			 {
				WCHAR*l_strName = DNCT::GcStringToWchar(AllFile_listBox->Items[i]->ToString());
				D3DXIMAGE_INFO imageinfo;
				D3DXIMAGE_INFO imageinfo2;
				bool l_bIsVolumeMap = false;
				bool l_bIsCueMap = false;
				DWORD	l_dwDepth = 0;
				DWORD	l_dwCubeMapFlags = 0;
				if( FAILED( D3DXGetImageInfoFromFile( l_strName, &imageinfo ) ) )
				{
					WARNING_MSG(DNCT::WcharToGcstring(l_strName)+"open failed");
					continue;
				}
				IDirect3DTexture9* l_pTexture;
				switch( imageinfo.ResourceType )
				{
					case D3DRTYPE_TEXTURE:
						if( D3DXCreateTextureFromFileEx( g_pd3dDevice, l_strName, 
							imageinfo.Width, imageinfo.Height, imageinfo.MipLevels, 0,
							imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 
							&imageinfo2, NULL, (LPDIRECT3DTEXTURE9*)&l_pTexture )  != D3D_OK )
						{
							WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" create failed");
							continue;
						}
						break;

					case D3DRTYPE_VOLUMETEXTURE:
						if( FAILED( D3DXCreateVolumeTextureFromFileEx( g_pd3dDevice, l_strName, 
							imageinfo.Width, imageinfo.Height, imageinfo.Depth, imageinfo.MipLevels,
							0, imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
							0, &imageinfo2, NULL, (LPDIRECT3DVOLUMETEXTURE9*)&l_pTexture ) ) )
						{
							WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" create failed");
							continue;
						}
						l_bIsVolumeMap = true;
						l_dwDepth = imageinfo2.Depth;
						break;

					case D3DRTYPE_CUBETEXTURE:
						if( FAILED( D3DXCreateCubeTextureFromFileEx( g_pd3dDevice, l_strName, 
							imageinfo.Width, imageinfo.MipLevels, 0, imageinfo.Format, 
							D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
							0, &imageinfo2, NULL, (LPDIRECT3DCUBETEXTURE9*)&l_pTexture ) ) )
						{
							WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" create failed");
							continue;
						}
						l_bIsCueMap = true;
						l_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
						break;
					default:
							WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" type error");
						continue;
				}
				UINT	l_uiWidth = (UINT)Width_numericUpDown->Value;
				UINT	l_uiHeight = (UINT)Height_numericUpDown->Value;
				if( TransformType_comboBox->SelectedIndex == 0)//percentage
				{
					float	l_fOrigianlWidth = (float)imageinfo.Width;
					float	l_fOrigianlHeight = (float)imageinfo.Height;
					l_uiWidth = (UINT)(l_fOrigianlWidth*l_uiWidth/100.f);
					l_uiHeight = (UINT)(l_fOrigianlHeight*l_uiHeight/100.f);
				}
				WCHAR	*l_strNewFileName = DNCT::GcStringToWchar(l_strDirectoryName+GetFileNameWithoutFullPath(AllFile_listBox->Items[i]->ToString()));
				LPDIRECT3DBASETEXTURE9 l_pIDirect3DTexture9 = Resize(l_uiWidth,l_uiHeight,l_pTexture,imageinfo.MipLevels,l_bIsVolumeMap,l_bIsCueMap);
				if( l_pIDirect3DTexture9 )
				{
					if( CreateFolderIfNotReplaceOriginalFile_checkBox->Checked )
					{
						//original folder and file name length = original file name length - original directory length 
						int	l_iOriginalInternalFileNameLength = AllFile_listBox->Items[i]->ToString()->Length-m_strDelectedDirectory->Length;
						//file name without full path directory length
						int	l_iFileNameLength = System::IO::Path::GetFileName(AllFile_listBox->Items[i]->ToString())->Length;
						//internal folder length = l_iOriginalInternalFileNameLength-l_iFileNameLength
						int	l_iInternalFolderLength = l_iOriginalInternalFileNameLength-l_iFileNameLength;
						//new folder name = new export folder path + internal folder name
						String^l_strNewDirectoryFolderPath = l_strDirectoryName+
							AllFile_listBox->Items[i]->ToString()->Substring(m_strDelectedDirectory->Length,l_iInternalFolderLength);
						String^l_strDirectoryName = l_strNewDirectoryFolderPath+System::IO::Path::GetFileName(AllFile_listBox->Items[i]->ToString());
						l_strNewFileName = DNCT::GcStringToWchar(l_strDirectoryName);
						CreateDirectory(l_strNewDirectoryFolderPath);
					}
					if( l_strExtensionName )
					{
						l_strNewFileName = DNCT::GcStringToWchar(DNCT::ChangeFileExtensionName(DNCT::WcharToGcstring(l_strNewFileName),l_strExtensionName));
					}
					if( FAILED( D3DXSaveTextureToFile( l_strNewFileName, l_D3DXIMAGE_FILEFORMAT, l_pIDirect3DTexture9, NULL ) ) )
					{
						ErrorMsg_textBox->Text += "Error:"+AllFile_listBox->Items[i]->ToString()+" write file failed "+DNCT::GetChanglineString();
					}
					l_pIDirect3DTexture9->Release();
				}
				else
				{
					ErrorMsg_textBox->Text += "Error:"+AllFile_listBox->Items[i]->ToString()+" write file failed "+"NewSize:"+l_uiWidth.ToString()+","+
						l_uiHeight.ToString()+DNCT::GetChanglineString();
				}
				l_pTexture->Release();
				float	l_fProgress = i/(float)AllFile_listBox->Items->Count;
				Progress_label->Text = l_fProgress.ToString();
				this->Update();
				this->Refresh();
			 }
			 Progress_label->Text = "Done";
		 }
};
}

