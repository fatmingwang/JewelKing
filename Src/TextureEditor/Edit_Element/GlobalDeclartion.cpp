#include "StdAfx.h"
#include "GlobalDeclartion.h"
#include "MPDIEditor.h"
#include "EditPath.h"
#include "PIEditor.h"
#include "SimpleAnimation.h"
#include "SoundObject.h"
#include "ImageConvexHullEditor.h"
#include "ProjectManager.h"

namespace FMCEditor 
{
	float			DRAW_PANEL_RESOLUTION_WIDTH = 1920.f;
	float			DRAW_PANEL_RESOLUTION_HEIGHT = 1080.f;
	Vector4			g_vBackgroundColor = Vector4::Zero;
	UT::sTimeAndFPS m_pTimeAndFPS;
	bool			g_bResolutionChanged = false;
	void			StartAllPages(GCFORM::SplitContainer^e_pPanel,GCFORM::Form^e_pForm)
	{
		FMCEditor::ProjectManager^l_pMPDI = gcnew FMCEditor::ProjectManager(e_pPanel);
		//l_pMPDI->Dock = GCFORM::DockStyle::Fill;
		//e_pForm->Controls->Add(l_pMPDI);
		//GCFORM::TabPage^l_pTabPage = gcnew GCFORM::TabPage();
		//l_pTabPage->Text = "MPDI/¸ô®|¹Ï¤ù½s¿è";
		//l_pTabPage->AutoScroll = true;
		//l_pTabPage->Controls->Add(l_pMPDI);
		//e_ptabControl->TabPages->Add(l_pTabPage);

		//FMCEditor::MPDIEditor^l_pMPDIEditor = gcnew MPDIEditor(e_ptabControl,e_pForm);
		//FMCEditor::cPIEditor^l_pcPIEditor = gcnew cPIEditor(e_ptabControl);
		//FMCEditor::SoundObject^l_pSoundObject = gcnew SoundObject(e_ptabControl);
		//FMCEditor::ImageConvexHullEditor^l_pImageConvexHullEditor = gcnew ImageConvexHullEditor(e_ptabControl);
		//FMCEditor::SimpleAnimation^l_pSimpleAnimation = gcnew SimpleAnimation(e_ptabControl);
		//FMCEditor::EditPath^l_pTest = gcnew FMCEditor::EditPath( e_ptabControl);
		//l_pMPDIEditor->Active();
	}
}