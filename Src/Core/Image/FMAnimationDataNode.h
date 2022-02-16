#ifndef	_FAT_ANIMATION_DATA_NODE_RULE
#define	_FAT_ANIMATION_DATA_NODE_RULE

namespace FATMING_CORE
{
	//class	cFrameWithAnimationRuleData:public Frame
	//{
	//protected:
	//	float			m_fUpdatedTime;
	//    float			m_fStartTime;
	//	float			m_fTargetTime;
	//    bool			m_bLoop;
	//    bool			m_bDone;
	//public:
	//	//local
	//	void			SetAngle(Vector3 e_vAngle);
	//	void			SetAngle(float e_fAngle);
	//	void			SetPos(Vector3 e_vPos);
	//	void			SetScale(float e_fScale);
	//	void			SetScale(Vector3 e_vScale);
	//	//
	//	float*			GetStartTimePointer(){return &m_fStartTime;}
	//    virtual void	Init();
	//    virtual void	Update(float e_fElpaseTime);
	//    virtual void	Render();
	//    bool			IsAnimationLoop();
	//    virtual bool	IsAnimationDone();
	//    //the time has been past
	//    float			GetPastTime();
	//    virtual	void	SetAnimationLoop(bool e_bLoop);
	//    virtual	void	SetAnimationDone(bool e_bAnimationDone);
	//	//
	//    virtual	void	RearrangeTime(float e_fPercenttage) = 0;
	//    virtual	void	RenderByGlobalTime(float e_fTime,bool e_bRotateWithAngle) = 0;
	//    virtual	void	InvertOrder() = 0;
	//    //start and last time.
	//    virtual	float	GetEndTime() = 0;
	//	virtual	void	Destroy(){}
	//	Vector3	GetPos(){ return this->GetWorldPosition(); }
	//	void	SetPos(Vector3 e_vPos){ this->SetLocalPosition(e_vPos); }
	//};
}

#endif