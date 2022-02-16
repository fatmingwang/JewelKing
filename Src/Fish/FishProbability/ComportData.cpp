#include "stdafx.h"
#include "ComportData.h"
#include "../EngineTest/GameApp.h"
#include "../EngineTest/MonsterBase.h"
#include "../EngineTest/Bullet.h"
#include "../EngineTest/PlayerBehaviorBase.h"
#include "../EngineTest/FishProbability.h"

void	sSchoolMasterLiSendDataWithMonster::Clear()
{
	CompareMonster.clear();ComparePlayer.clear();CompareBullet.clear();
	memset(&SchoolMasterLiSendData,0,sizeof(sSchoolMasterLiSendData));
	SchoolMasterLiSendData.cHeader = 0x55;
}
void	sSchoolMasterLiSendDataWithMonster::Copy(sSchoolMasterLiSendDataWithMonster*e_pSchoolMasterLiSendDataWithMonster)
{
	Clear();
	SchoolMasterLiSendData = e_pSchoolMasterLiSendDataWithMonster->SchoolMasterLiSendData;
	for( size_t i=0;i<e_pSchoolMasterLiSendDataWithMonster->ComparePlayer.size();++i )
	{
		ComparePlayer.push_back(e_pSchoolMasterLiSendDataWithMonster->ComparePlayer[i]);
		CompareMonster.push_back(e_pSchoolMasterLiSendDataWithMonster->CompareMonster[i]);
		CompareBullet.push_back(e_pSchoolMasterLiSendDataWithMonster->CompareBullet[i]);
	}
}

cLiSchoolMasterSerialIO::cLiSchoolMasterSerialIO()
{

}

cLiSchoolMasterSerialIO::~cLiSchoolMasterSerialIO()
{

}

void cLiSchoolMasterSerialIO::OnEventOpen(BOOL bSuccess)
{
	std::string	l_str = UT::ComposeMsgByFormat("comport open %s",bSuccess?L"ok":L"false");
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
}

void cLiSchoolMasterSerialIO::OnEventClose(BOOL bSuccess)
{
	//UT::ErrorMsg(L"Comport cloase",bSuccess?L"okay":L"failed");
}

void cLiSchoolMasterSerialIO::OnEventRead(char *inPacket,int inLength)
{
	std::string	l_str = UT::ComposeMsgByFormat("OnEventRead:size%d",inLength);
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
	sSchoolMasterLiReceiveData*l_pSchoolMasterLiReceiveData = cFishApp::m_spFishProbability->GetSchoolMasterLiReceiveData();
	unsigned char	l_cData[MAX_SEND_BUFFER];
	memcpy(l_cData,inPacket,inLength);
	std::string	l_Value;
	for( int i=0;i<10;++i )
	{
		l_Value += ValueToString((int)l_cData[i]);
		l_Value += ",";
	}
	cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_Value.c_str());
	if( l_cData[0] == 0xaa )
	{
		int	l_iNum = l_cData[1];
		l_pSchoolMasterLiReceiveData->cNumFishHitted = l_cData[1];
		l_str = UT::ComposeMsgByFormat("FishCount:%d",l_iNum);
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());
		int	l_iNextFishSize = 7;
		int	l_iStartIndex = 2;
		char	l_cCheckSum = l_cData[0]+l_cData[1];
		for( int i=0;i<l_iNum;++i )
		{
			unsigned char*l_pData = &l_cData[l_iStartIndex+(l_iNextFishSize*i)];
			memcpy(&l_pSchoolMasterLiReceiveData->HittedFish[i].usID,l_pData,sizeof(unsigned short));
			l_pSchoolMasterLiReceiveData->HittedFish[i].ucPlayerID = l_pData[2];
			memcpy(&l_pSchoolMasterLiReceiveData->HittedFish[i].uiFishOutMoney,&l_pData[3],sizeof(int));
			//l_cCheckSum += l_pSchoolMasterLiReceiveData->HittedFish[i].usID;
			for( int j=0;j<l_iNextFishSize;++j )
				l_cCheckSum += l_pData[j];
		}
		if( l_cCheckSum != l_cData[l_iStartIndex+(l_iNextFishSize*l_iNum)] )
		{
			cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("received check sum error");
		}
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine("received data done!");
		cFishApp::m_spFishProbability->SetReceivedSingnal(true);
	}
	else
	{
		l_str = UT::ComposeMsgByFormat("HeaderValue:%d",l_cData[0]);
		cGameApp::m_spLogFile->WriteToFileImmediatelyWithLine(l_str.c_str());	
	}
}

void cLiSchoolMasterSerialIO::OnEventWrite(int nWritten)
{

}