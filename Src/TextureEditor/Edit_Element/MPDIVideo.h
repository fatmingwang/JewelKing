#pragma once

#include "EditorInterface.h"

class	cFUSynchronized;

public ref class	cMPDIVideo_Editor:public cEditorInterface
{
	System::Windows::Controls::Label^m_TimeLabel;
	cFUSynchronized*m_pLocker;
	cAVIPlayer*	  m_pAVIPlayer;
	cVideoSubMPDI*m_pVideoSubMPDI;
	CoreWPF::VideoPlayer::VideoPlayer^m_pVideoPlayer;
	System::Windows::Controls::Slider^m_pSliderTime;
	void	FileOpen(char*e_strFileName);
	private: System::Void FileOpen(System::Object^  sender, System::EventArgs^  e);
	private: System::Void StartTimeValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void EndTimeValueChanged(System::Object^  sender, System::EventArgs^  e);
public:
	cMPDIVideo_Editor(CoreWPF::VideoPlayer::VideoPlayer^e_pVideoPlayer);
	~cMPDIVideo_Editor();
	void			UpdateTrackBar();
	virtual	void	Update(float e_fElpaseTime) override ;
	virtual	void	Render() override ;
	virtual	void	Destroy() override ;
	public:  System::Void SetCurrentVideoTime(float e_fTime);
	public:  System::Void Play(System::Object^  sender, System::EventArgs^  e);
	public:  System::Void Pause(System::Object^  sender, System::EventArgs^  e);
	public:  System::Void Stop(System::Object^  sender, System::EventArgs^  e);
	public:  System::Void SliderToTime(System::Object^  sender, System::EventArgs^  e);
};