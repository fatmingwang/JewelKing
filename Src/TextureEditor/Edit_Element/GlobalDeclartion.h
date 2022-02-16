#ifndef Global_Declartion_H
#define Global_Declartion_H

namespace FMCEditor 
{
	extern float			DRAW_PANEL_RESOLUTION_WIDTH;
	extern float			DRAW_PANEL_RESOLUTION_HEIGHT;
	extern Vector4			g_vBackgroundColor;
	extern bool				g_bResolutionChanged;
	extern void				StartAllPages(GCFORM::SplitContainer^e_pPanel,GCFORM::Form^e_pForm);
}

//for quickly to add data while list and node is not same count
template<class TYPE>
void	AssignListData(GCFORM::TreeNode^e_pNode,cObjectListByName<TYPE>*e_pNamedTypedObjectList)
{
	if(e_pNode->Nodes->Count != e_pNamedTypedObjectList->Count() )
	{
		e_pNode->Nodes->Clear();
		for( int i=0;i<e_pNamedTypedObjectList->Count();++i )
		{
			e_pNode->Nodes->Add(String(UT::WcharToChar((*e_pNamedTypedObjectList)[i]->GetName())).ToString());
		}
	}
}

template<class TYPE>
void	AssignListData(GCFORM::ListBox^e_pLisBox,cObjectListByName<TYPE>*e_pNamedTypedObjectList)
{
	if(e_pLisBox->Items->Count != e_pNamedTypedObjectList->Count() )
	{
		e_pLisBox->Items->Clear();
		for( int i=0;i<e_pNamedTypedObjectList->Count();++i )
		{
			e_pLisBox->Items->Add(String(UT::WcharToChar((*e_pNamedTypedObjectList)[i]->GetName())).ToString());
		}
	}
}

#endif