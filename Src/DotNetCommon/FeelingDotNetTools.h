#ifndef _FEELING_DOTNET_TOOLS_H_
#define _FEELING_DOTNET_TOOLS_H_
#include "../FeelingLib/AllFeelingLibInclude.h"
//ensure u have include AllFeelingLihInclude.h
//and it under common runtime environment

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

	inline	System::Windows::Forms::TreeNode^	AddFCDSceneNodeChildren(FCDSceneNode*e_pFCDSceneNode,System::Windows::Forms::TreeNode^e_pTreeNode)
	{
		System::Windows::Forms::TreeNode^l_pTreeNode;
		if( e_pTreeNode )
			l_pTreeNode = e_pTreeNode->Nodes->Add(String(e_pFCDSceneNode->GetDaeId().c_str()).ToString());
		else
		{
			l_pTreeNode = gcnew System::Windows::Forms::TreeNode(String(e_pFCDSceneNode->GetDaeId().c_str()).ToString());
		}
		int	l_iChildrenCount = e_pFCDSceneNode->GetChildrenCount();
		for( int i=0;i<l_iChildrenCount;++i )
		{
			const FCDSceneNode*l_pFCDSceneNode = e_pFCDSceneNode->GetChildren()[i];
			l_pTreeNode->Nodes->Add(String(l_pFCDSceneNode->GetDaeId().c_str()).ToString());
			AddFCDSceneNodeChildren((FCDSceneNode*)l_pFCDSceneNode,l_pTreeNode);
		}
		return l_pTreeNode;
	}

	//System::Windows::Forms::TreeNode^FCDSceneNodeToTreeNode(FCDSceneNode*e_pFCDSceneNode)
	//{
	//	System::Windows::Forms::TreeNode^l_pTreeNode = gcnew 
	//	AddFCDSceneNodeChildren(e_pFCDSceneNode,)
	//}

#endif