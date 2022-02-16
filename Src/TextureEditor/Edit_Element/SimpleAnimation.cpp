#include "StdAfx.h"
#include "SimpleAnimation.h"
namespace FMCEditor
{
	System::Void SimpleAnimation::SaveImagePlayerData()
	{
		String^l_strFileName = DNCT::SaveFileAndGetName("(*.sa)|*.sa");
		if(l_strFileName)
		{
			l_strFileName = ForceAddExtenName(l_strFileName,"sa");
			m_pImagePlayerSA->SetName(DNCT::GcStringToWchar(DNCT::GetFileNameWithoutFullPath(l_strFileName,true)));
			m_pImagePlayManager->Export(DNCT::GcStringToChar(l_strFileName));
			cObjectListByName<cImageIndexOfAnimation>*l_p = m_pImagePlayerSA->GetPuzzleImage()->GetImageIndexOfAnimationList();
			SAFE_DELETE(l_p);
			m_pImagePlayerSA->GetPuzzleImage()->SetImageIndexOfAnimationList(m_pImagePlayerSA->ToPIImageAnimationData());
			String^l_strPIFileName = DNCT::GetDirectoryWithoutFileName(l_strFileName)+DNCT::GetFileNameWithoutFullPath(l_strFileName,true)+String(m_pImagePlayManager->GetAllBaseImageList()->ExtensionName()).ToString();
			m_pImagePlayManager->GetAllBaseImageList()->Export(DNCT::GcStringToChar(l_strPIFileName));
//			FileToUnicode(l_strFileName);
		}
	}
//end namespace FMCEditor
}