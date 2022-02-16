// ControlePanelVariableChanger.cpp : main project file.

#include "stdafx.h"
#include "windows.h"
#include "shellapi.h"

#include "shellapi.h"
#include "../../Core/AllCoreInclude.h"
#include "../../Core/GamePlayUT/Event/AllEventInclude.h"
#include "../../AllLibInclude.h"
using namespace System;

//VendorAskMachineStartStatusData.xml
void	ProcessVendorAskMachineStartStatusData(char* e_strFileName)
{
	cNodeISAX	l_VendorAskMachineStartStatusDataNodeISAX;
	if(	l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(e_strFileName) )
	{
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int	l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					l_strValue = l_pElement->Attribute(L"Value");
					//cEventVariable*l_pEventVariable = m_pEventManager->m_pEventVariableManager->GetObject(L"l_strName");
					//l_pEventVariable->SetDataByString(l_strValue);
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}	
	}
}

//============================================
//this project for vendor ask,
//while game is starting to sell,vendor might need to force setup some control panel data.
//so wollow the format as velow
//<Root>
//    <Date Day="29" />
//    <TargetMachineChangeVariableTime Value="10" />
//    <MachineChangeVariableTime Value="0" />
//    <Data Name="Probability" Value="100" />
//</Root>
//it will compare Date's Day if day is not same MachineChangeVariableTime will add 1
//until MachineChangeVariableTime is bigger than TargetMachineChangeVariableTime,this behavior will end.
//============================================
int main(array<System::String ^> ^args)
{
	char*	l_strVendorAskMachineStartStatusData = "VendorAskMachineStartStatusData.xml";
	WCHAR*	l_strOutputFileName = L"VariableChanger.xml";
	SYSTEMTIME l_st;
	GetLocalTime(&l_st);
	bool	l_bSameDay = false;
	int		l_iDay = l_st.wDay;

	cNodeISAX	l_VendorAskMachineStartStatusDataNodeISAX;


	TiXmlDocument doc;
	TiXmlElement * root = new TiXmlElement( L"Root" );  
	doc.LinkEndChild( root );  


	if(	l_VendorAskMachineStartStatusDataNodeISAX.ParseDataIntoXMLNode(l_strVendorAskMachineStartStatusData) )
	{//create file
		TiXmlElement*l_pElement = l_VendorAskMachineStartStatusDataNodeISAX.GetRootElement();
		int	l_iTargetTime = 0;
		if( l_pElement )
		{
			l_pElement = l_pElement->FirstChildElement();
			while( l_pElement )
			{
				const WCHAR*l_strValue = l_pElement->Value();
				if( !wcscmp(l_strValue,L"MachineChangeVariableTime") )
				{
					l_strValue = l_pElement->Attribute(L"Value");
					int	l_iValue = _wtoi(l_strValue);
					if( l_iValue >= l_iTargetTime )
					{
						DeleteFile(l_strOutputFileName);
						goto EXIT;
					}
					if( !l_bSameDay )
					{
						l_pElement->SetAttribute(L"Value",ValueToStringW(l_iValue+1));
					}
				}
				else
				if( !wcscmp(l_strValue,L"TargetMachineChangeVariableTime") )
				{
					l_strValue = l_pElement->Attribute(L"Value");
					l_iTargetTime = _wtoi(l_strValue);
				}
				else
				if( !wcscmp(l_strValue,L"Date") )
				{
					l_strValue = l_pElement->Attribute(L"Day");
					int	l_iLastTimeLoginDay = _wtoi(l_strValue);
					if( l_iDay != l_iLastTimeLoginDay )
					{
						l_pElement->SetAttribute(L"Day",ValueToStringW(l_iDay));
					}
					else
					{
						l_bSameDay = true;
					}
				}
				else
				if( !wcscmp(l_strValue,L"Data") )
				{
					const WCHAR*l_strName = l_pElement->Attribute(L"Name");
					l_strValue = l_pElement->Attribute(L"Value");

					TiXmlElement *l_pDataElement = new TiXmlElement( L"Data" );
					root->LinkEndChild( l_pDataElement );
					l_pDataElement->SetAttribute(L"Name", l_strName);
					l_pDataElement->SetAttribute(L"Value", l_strValue);
				}
				l_pElement = l_pElement->NextSiblingElement();
			}
		}
	}
	else
	{//delete file
		DeleteFile(l_strOutputFileName);
	}
	doc.SaveFile( UT::WcharToChar(l_strOutputFileName) );
	l_VendorAskMachineStartStatusDataNodeISAX.Export(l_strVendorAskMachineStartStatusData);
EXIT:
	ShellExecute(0, L"open", L"Fish_Win32.exe", 
                           0, 
                           0, SW_SHOWNORMAL);
    return 0;
}
