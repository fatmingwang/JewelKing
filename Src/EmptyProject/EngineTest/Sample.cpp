#include "stdafx.h"
#include "GameApp.h"

cFrameCamera	*g_pFrameCamera;
cCameraBehaveByMouseBehave*g_pCameraBehaveByMouseBehave = 0;


cParticleEmitterGroup*g_pParticleEmitterGroup = 0;
cPrtEmitter*g_pPrtEmitter;


cPrtEmitter*g_pPrtEmiter = 0;
cCurveManager*g_p2DCurveLoader = 0;
cCurveWithTime*g_p2DCurvesWithTime = 0;

cMPDIList*g_pMPDIList = 0;
cMultiPathDynamicImage*g_pMultiPathDynamicImage = 0;
cMultiPathDynamicImage*g_pMultiPathDynamicImageClone = 0;

cColladaParser*g_pColladaParser = 0;

void	LoadSample();
void	DestoryObject();
void	SampleUpdate(float e_fElpaseTime);
void	SampleRender();

void	LoadSample()
{
	g_pMPDIList = cGameApp::GetMPDIListByFileName(L"Fish/Image/BigWinShow3.mpdi");
	if( g_pMPDIList )
	{
		g_pMultiPathDynamicImage = g_pMPDIList->GetObject(L"BigWinShow3_U");
		//ensure u have call start
		g_pMultiPathDynamicImage->Init();
		g_pMultiPathDynamicImageClone = new cMultiPathDynamicImage(g_pMultiPathDynamicImage);
		//g_pMultiPathDynamicImage->SetLoop(true);
		g_pMultiPathDynamicImage->SetStopAtLastFrame(true);
	}
	g_pParticleEmitterGroup = cGameApp::GetPRTG("ParticleData/Stage1.prtg",L"Fire");
	if( g_pParticleEmitterGroup )
	{
		g_pParticleEmitterGroup->Init();
		g_pParticleEmitterGroup->SetLoop(true);
	}
	g_pPrtEmitter = cGameApp::GetPrtEmitter("ParticleData/Smoke.prt",L"Fire");
	if( g_pPrtEmitter )
	{
		g_pPrtEmitter->Emit(Vector3(200,200,0));
		g_pPrtEmitter->SetLoop(true);
	}

	//g_pColladaParser = new cColladaParser();
	if( g_pColladaParser )
	{
		if(g_pColladaParser->ParseDataIntoXMLNode("C:/Users/Fatming/Desktop/123.dae"))
		{
			//g_pColladaParser->
		}
	}
}

void	DestorySampleObject()
{
	//do not delete g_pMultiPathDynamicImage,it come from g_pMPDIList
	//g_pMultiPathDynamicImageClone is a clone object so delete it.
	SAFE_DELETE(g_pMultiPathDynamicImageClone);
	//not necessary,it will auto release while m_pAnimationParser is delete
	//cGameApp::m_pAnimationParser->RemoveObject(L"fmbook_mpdi");
}

void	SampleUpdate(float e_fElpaseTime)
{
	if( g_pMultiPathDynamicImage )
		g_pMultiPathDynamicImage->Update(e_fElpaseTime);
	if( g_pPrtEmitter )
		g_pPrtEmitter->Update(e_fElpaseTime);
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Update(e_fElpaseTime);
}

void	SampleRender()
{
	if( g_pMultiPathDynamicImage )
		g_pMultiPathDynamicImage->Render();
	if( g_pPrtEmitter )
		g_pPrtEmitter->Render();
	if( g_pParticleEmitterGroup )
		g_pParticleEmitterGroup->Render();
}