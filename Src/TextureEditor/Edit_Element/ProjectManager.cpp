#include "StdAfx.h"
#include "ProjectManager.h"
#include "MPDIEditor.h"
#include "EditPath.h"
#include "PIEditor.h"
#include "SoundObject.h"
#include "EditPath.h"
#include "EditorInterface.h"
#include "ImageConvexHullEditor.h"
namespace FMCEditor 
{
	ProjectManager::ProjectManager(GCFORM::SplitContainer^e_pSplitPanel)
	{
		m_pSplitPanel = e_pSplitPanel;
		m_pAllForm = gcnew System::Collections::ArrayList();
		m_pEditorInterfaceList = gcnew System::Collections::ArrayList;
		m_pProjectUI = gcnew TextureEditorWPF::MPDI::ProjectUI();
		System::Windows::Forms::Integration::ElementHost^l_pElementHots = gcnew System::Windows::Forms::Integration::ElementHost;
		l_pElementHots->Dock = DockStyle::Fill;
		l_pElementHots->Child = m_pProjectUI;
		m_pSplitPanel->Panel1->Controls->Add(l_pElementHots);
		array<String^>^l_strArray = {".mpdi",".pi",".path",".xml",".collision"};

		m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->m_strExtensionName = l_strArray;
		CheckWorkingDirectory();

		m_pProjectUI->m_FMCFolderExplorerTree->f_NewFolder += gcnew EventHandler(this,&ProjectManager::NewFolder);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewMPDI += gcnew EventHandler(this,&ProjectManager::NewMPDI);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewPI += gcnew EventHandler(this,&ProjectManager::NewPI);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewPath += gcnew EventHandler(this,&ProjectManager::NewPath);
		m_pProjectUI->m_FMCFolderExplorerTree->f_NewPhysical  += gcnew EventHandler(this,&ProjectManager::NewPhysical);
		m_pProjectUI->m_FMCFolderExplorerTree->f_Copy += gcnew EventHandler(this,&ProjectManager::Copy);
		m_pProjectUI->m_FMCFolderExplorerTree->f_Paste += gcnew EventHandler(this,&ProjectManager::Paste);
		m_pProjectUI->m_FMCFolderExplorerTree->f_Delete += gcnew EventHandler(this,&ProjectManager::Delete);
		m_pProjectUI->m_FMCFolderExplorerTree->f_SetWorkingDirectory += gcnew EventHandler(this,&ProjectManager::SetWorkingDirectory);
		m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->f_Drop += gcnew EventHandler(this,&ProjectManager::Drop);
		m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->f_MouseDoubleClick += gcnew EventHandler(this,&ProjectManager::MouseDoubleClick);
		//m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->f_KeyUp += gcnew EventHandler(this,&ProjectManager::KeyUp);

	}
	ProjectManager::~ProjectManager()
	{
		delete m_pAllForm;
	for(int i=0;i<m_pEditorInterfaceList->Count;++i)
	{
		System::Object^l_pObject = m_pEditorInterfaceList[i];
		delete l_pObject;
	}
		delete m_pEditorInterfaceList;
	}

	MPDIEditor^	ProjectManager::GetOldMPDI(bool*e_pbExist)
	{
		MPDIEditor^l_pOlfMPDIEditor;
		int	l_iIndex = -1;
		if(IsContainMPDI(&l_iIndex))
		{
			*e_pbExist = true;
			MPDIEditor^l_pMPDIEditor = (MPDIEditor^)m_pEditorInterfaceList[l_iIndex];
			if(l_pMPDIEditor->m_bDataChanged)
			{
				WARING_YES_NO_TO_NO("file changed,ignore change to open new MPDI and close old one?")
					return l_pOlfMPDIEditor;
			}
			if( l_pMPDIEditor->m_siOpenCount == 0 )
			{
				CloseOldMPDI();
				*e_pbExist = false;
			}
			else
				l_pOlfMPDIEditor = l_pMPDIEditor;
		}
		else
			*e_pbExist = false;
		return l_pOlfMPDIEditor;
	}

	bool	ProjectManager::CloseOldMPDI()
	{
		int	l_iIndex = -1;
		while(IsContainMPDI(&l_iIndex))
		{
			MPDIEditor^l_pMPDIEditor = (MPDIEditor^)m_pEditorInterfaceList[l_iIndex];
			//if(l_pMPDIEditor->m_bDataChanged)
			//{
			//	WARING_YES_NO_TO_NO("file changed,ignore change to open new MPDI and close old one?")
			//		return false;
			//}
			l_pMPDIEditor->m_pForm->Close();
			m_pEditorInterfaceList->Remove(l_pMPDIEditor);
		}
		return true;
	}

	bool	ProjectManager::IsContainMPDI(int*e_pIndex)
	{
		if( !m_pEditorInterfaceList )
			return false;
		for(int i=0;i<m_pEditorInterfaceList->Count;++i)
		{
			String^l_str = m_pEditorInterfaceList[i]->ToString();
			if(l_str->CompareTo("FMCEditor.MPDIEditor") == 0)
			{
				if( e_pIndex )
					*e_pIndex = i;
				return true;
			}
		}
		return false;
	}

	bool	ProjectManager::IsContainPI(int*e_pIndex)
	{
		if( !m_pEditorInterfaceList )
			return false;
		for(int i=0;i<m_pEditorInterfaceList->Count;++i)
		{
			String^l_str = m_pEditorInterfaceList[i]->ToString();
			if(l_str->CompareTo("FMCEditor.cPIEditor") == 0)
			{
				if( e_pIndex )
					*e_pIndex = i;
				return true;
			}
		}
		return false;
	}
	//<Root	Directory="C:\Users\Fatming\Desktop\FM79979\Media">
	System::Void ProjectManager::CheckWorkingDirectory()
	{
		cNodeISAX	l_NodeISAX;
		bool	l_b = l_NodeISAX.ParseDataIntoXMLNode("AnimationEditorWorkingdirectory.xml");
		if(l_b)
		{
			TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
			const WCHAR*l_strDirectory = l_pTiXmlElement->Attribute(L"Directory");
			if( l_strDirectory )
			{
				m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->SetWorkingProject(DNCT::WcharToGcstring(l_strDirectory));
			}
		}
		else
		{
			WCHAR	l_str[MAX_PATH];
			GetCurrentDirectory(MAX_PATH,l_str);
			m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->SetWorkingProject(DNCT::WcharToGcstring(l_str));
		}
	}



	GCFORM::Form^ProjectManager::GetForm()
	{
		GCFORM::Form^l_pForm = gcnew GCFORM::Form();
		l_pForm->TopLevel = false;
		HWND l_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
		HWND l_pFormHandle = (HWND)l_pForm->Handle.ToPointer();
		//::Threading::Thread::Sleep(1000);
		SetParent(l_pFormHandle,l_pParentHandle);
		l_pForm->Show();
		l_pForm->WindowState = GCFORM::FormWindowState::Maximized;
		//l_pForm->Parent = m_pProjectUI->m_ContainerBorder->Child;
		l_pForm->KeyPreview = true;
		l_pForm->KeyUp += gcnew KeyEventHandler(this,&ProjectManager::ProjectManager_KeyUp);
		m_pAllForm->Add(l_pForm);
		return l_pForm;
	}
	System::Void ProjectManager::NewFolder(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void ProjectManager::NewMPDI(System::Object^  sender, System::EventArgs^  e)
	{
		if(CloseOldMPDI())
		{
			GCFORM::Form^l_pForm = GetForm();
			GCFORM::TabControl^l_ptabControl;
			MPDIEditor^l_pMPDIEditor = gcnew MPDIEditor(l_ptabControl,l_pForm);
			l_pMPDIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
			m_pEditorInterfaceList->Add(l_pMPDIEditor);
		}
		
	}

	System::Void ProjectManager::NewPI(System::Object^  sender, System::EventArgs^  e)
	{
		GCFORM::Form^l_pForm = GetForm();
		GCFORM::TabControl^l_ptabControl;
		FMCEditor::cPIEditor^l_pcPIEditor = gcnew cPIEditor(l_ptabControl,l_pForm);
		l_pcPIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
		m_pEditorInterfaceList->Add(l_pcPIEditor);
	//FMCEditor::SoundObject^l_pSoundObject = gcnew SoundObject(e_ptabControl);
	//FMCEditor::ImageConvexHullEditor^l_pImageConvexHullEditor = gcnew ImageConvexHullEditor(e_ptabControl);
	//FMCEditor::SimpleAnimation^l_pSimpleAnimation = gcnew SimpleAnimation(e_ptabControl);
	//FMCEditor::EditPath^l_pTest = gcnew FMCEditor::EditPath( e_ptabControl);
	}

	System::Void ProjectManager::NewPath(System::Object^  sender, System::EventArgs^  e)
	{
		GCFORM::Form^l_pForm = GetForm();
		GCFORM::TabControl^l_ptabControl;
		FMCEditor::EditPath^l_pTest = gcnew FMCEditor::EditPath( l_ptabControl, l_pForm);
		//l_pMPDIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
		m_pEditorInterfaceList->Add(l_pTest);

		
	}

	System::Void ProjectManager::NewPhysical(System::Object^  sender, System::EventArgs^  e)
	{
		GCFORM::Form^l_pForm = GetForm();
		GCFORM::TabControl^l_ptabControl;
		FMCEditor::ImageConvexHullEditor^l_pTest = gcnew FMCEditor::ImageConvexHullEditor( l_ptabControl,l_pForm);
		//l_pMPDIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
		m_pEditorInterfaceList->Add(l_pTest);
	}

	System::Void ProjectManager::Copy(System::Object^  sender, System::EventArgs^  e)
	{
		
	}

	System::Void ProjectManager::Paste(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void ProjectManager::Delete(System::Object^  sender, System::EventArgs^  e)
	{

	}

	System::Void ProjectManager::Drop(System::Object^  sender, System::EventArgs^  e)
	{
	
	}

	System::Void ProjectManager::MouseDoubleClick(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strFileName = m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->m_SelectedFileNameTextBlock->Text;
		String^l_strExtensionName = System::IO::Path::GetExtension(l_strFileName);
		MPDIEditor^l_pMPDIEditor;
		GCFORM::Form^l_pForm;
		bool	l_bMatched = false;
		int	l_iTargetIndex = -1;//0 for mpdi 1 for pi
		if( l_strExtensionName->Equals(".mpdi") )
		{
			l_iTargetIndex = 0;
			bool	l_bExist = false;
			l_pMPDIEditor = GetOldMPDI(&l_bExist);
			//check ignore edited object?
			if(l_bExist&&!l_pMPDIEditor)
				return;
		}
		else
		if( l_strExtensionName->Equals(".pi") )
		{
			l_iTargetIndex = 1;
		}
		else
		if( l_strExtensionName->Equals(".collision") )
		{
			l_iTargetIndex = 4;
		}
		if( l_iTargetIndex != -1 )
		{
			GCFORM::TabControl^l_ptabControl;
			//System::Windows::Forms::UserControl^l_pUserControl;
			if( l_iTargetIndex == 0 )
			{
				if( l_pMPDIEditor )
				{
					l_pMPDIEditor->OpenMPDIFile(l_strFileName);
				}
				else
				{
					l_pForm = GetForm();
					MPDIEditor^l_pNewMPDIEditor = gcnew MPDIEditor(l_ptabControl,l_pForm);
					l_pNewMPDIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
					l_pNewMPDIEditor->OpenMPDIFile(l_strFileName);
					m_pEditorInterfaceList->Add(l_pNewMPDIEditor);
				}

			}
			else
			if( l_iTargetIndex == 1 )
			{
				l_pForm = GetForm();
				FMCEditor::cPIEditor^l_pcPIEditor = gcnew cPIEditor(l_ptabControl,l_pForm);
				l_pcPIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
				l_pcPIEditor->OpenPIFile(l_strFileName);
				m_pEditorInterfaceList->Add(l_pcPIEditor);
			}
			else
			if( l_iTargetIndex == 4 )
			{
				l_pForm = GetForm();
				FMCEditor::ImageConvexHullEditor^l_pTest = gcnew FMCEditor::ImageConvexHullEditor(l_ptabControl,l_pForm);
				//l_pMPDIEditor->m_pParentHandle = (HWND)this->m_pSplitPanel->Panel2->Handle.ToPointer();
				l_pTest->OpenCollisionFile(l_strFileName);
				m_pEditorInterfaceList->Add(l_pTest);
			}
			if( l_pForm )
			{
				l_pForm->Text = l_strFileName;
				l_pForm->KeyPreview = true;
				l_pForm->KeyUp += gcnew KeyEventHandler(this,&ProjectManager::ProjectManager_KeyUp);
			}
		}
	}
	System::Void ProjectManager::SetWorkingDirectory(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strDirectory = DNCT::SelectDirectory();
		if(l_strDirectory)
		{
			TiXmlDocument doc;
			TiXmlElement * root = new TiXmlElement( L"Root" );  
			root->SetAttribute(L"Directory",DNCT::GcStringToWchar(l_strDirectory));
			doc.LinkEndChild( root );  	
			doc.SaveFile("AnimationEditorWorkingdirectory.xml");
			m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->SetWorkingProject(l_strDirectory);
		}
	}

	System::Void ProjectManager::ProjectManager_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
			 {
				for each(GCFORM::Form^l_pForm in m_pAllForm)
				{
					if( l_pForm == sender )
					{
						if( e->KeyCode == GCFORM::Keys::F11 )
						{
							 MPDIEditor^l_pMPDIEditor = dynamic_cast<MPDIEditor^>(l_pForm->Controls[0]);
							 if( l_pMPDIEditor )
								l_pMPDIEditor->OnlyRenderWindow();
							 if(l_pForm->FormBorderStyle == GCFORM::FormBorderStyle::None )
							 {
								 l_pForm->FormBorderStyle = GCFORM::FormBorderStyle::SizableToolWindow;
							 }
							 else
							 {
								 l_pForm->FormBorderStyle = GCFORM::FormBorderStyle::None;
							 }
							 if(l_pForm->WindowState != GCFORM::FormWindowState::Maximized )
							 {
								l_pForm->WindowState = GCFORM::FormWindowState::Maximized;
							 }
						}
					}
				}
			 }
	//System::Void ProjectManager::KeyUp(System::Object^  sender, System::EventArgs^  e)
	//{
	//	//String^l_strExtenStionName = System::IO::Path::GetExtension(m_pProjectUI->m_FMCFolderExplorerTree->m_FolderExplorerTree->m_SelectedFileNameTextBlock->Text)
	//}
}