#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "MPDIEditor.h"

namespace FMCEditor 
{
	/// <summary>
	/// MPDI ªººK­n
	/// </summary>
	public ref class ProjectManager
	{
		bool			CloseOldMPDI();
		MPDIEditor^		GetOldMPDI(bool*e_pbExist);
		bool			IsContainMPDI(int*e_pIndex);
		bool			IsContainPI(int*e_pIndex);
	public:
		ProjectManager(GCFORM::SplitContainer^e_pSplitPanel);

	protected:
		~ProjectManager();
		//my
		TextureEditorWPF::MPDI::ProjectUI^m_pProjectUI;
		GCFORM::SplitContainer^m_pSplitPanel;
		//contain all PI,MPDI,Path....form
		System::Collections::ArrayList^m_pAllForm;
		System::Collections::ArrayList^m_pEditorInterfaceList;
		private: GCFORM::Form^GetForm();
		public:	 System::Void NewFolder(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void NewMPDI(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void NewPI(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void NewPath(System::Object^  sender, System::EventArgs^  e);
		public:  System::Void NewPhysical(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Copy(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Paste(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Delete(System::Object^  sender, System::EventArgs^  e);
		private: System::Void Drop(System::Object^  sender, System::EventArgs^  e);
		private: System::Void MouseDoubleClick(System::Object^  sender, System::EventArgs^  e);
		private: System::Void SetWorkingDirectory(System::Object^  sender, System::EventArgs^  e);
		private: System::Void CheckWorkingDirectory();
		//private: System::Void KeyUp(System::Object^  sender, System::EventArgs^  e);
		private: System::Void ProjectManager_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	};
}
