#ifndef _LI_COMPORT_H_
#define _LI_COMPORT_H_
#include "SerialCtrl.h"

	#define	MAX_FISH_DIED_AT_ONE_UPLOAD	80//ENSURE sSchoolMasterLiSendData size is smaller than 1k
	struct	sSchoolMasterLiSendData
	{
		char	cHeader;
		struct sHittedFish
		{
			unsigned short	usID;			//2
			unsigned char	ucPlayerID;		//1
			unsigned short	ucBulletValue;	//2
			unsigned int	uiFishOutMoney;	//4
		};
		unsigned char cNumFishHitted;//1
		char cChecksum;
		sHittedFish	HittedFish[MAX_FISH_DIED_AT_ONE_UPLOAD];
		sSchoolMasterLiSendData(){ memset(this,0,sizeof(sSchoolMasterLiSendData));cHeader = 0x55; }
	};
	class	cMonster;
	class	cPlayerBehaviorBase;
	class	cBullet;
	struct	sSchoolMasterLiSendDataWithMonster
	{
		sSchoolMasterLiSendData				SchoolMasterLiSendData;
		std::vector<cMonster*>				CompareMonster;
		std::vector<cPlayerBehaviorBase*>	ComparePlayer;
		std::vector<cBullet*>				CompareBullet;
		void								Clear();
		void								Copy(sSchoolMasterLiSendDataWithMonster*e_pSchoolMasterLiSendDataWithMonster);
	};

	struct	sSchoolMasterLiReceiveData
	{
		struct sHittedFish
		{
			unsigned short	usID;
			unsigned char	ucPlayerID;
			unsigned int	uiFishOutMoney;
		};
		char cHeader;
		unsigned char cNumFishHitted;
		sHittedFish	HittedFish[MAX_FISH_DIED_AT_ONE_UPLOAD];
		char cChecksum;
		sSchoolMasterLiReceiveData(){ memset(this,0,sizeof(sSchoolMasterLiReceiveData)); cHeader = (char)0xaa; }
	};

	class	cLiSchoolMasterSerialIO:public CSerialIO
	{

	public:
		cLiSchoolMasterSerialIO();
		~cLiSchoolMasterSerialIO();
		virtual void OnEventOpen(BOOL bSuccess);
		virtual void OnEventClose(BOOL bSuccess);
		virtual void OnEventRead(char *inPacket,int inLength);
		virtual void OnEventWrite(int nWritten);
	};

#endif