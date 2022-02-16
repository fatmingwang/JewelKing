#ifndef	_FATMING_ANIMATION_RULW
#define	_FATMING_ANIMATION_RULW

namespace FATMING_CORE
{
	extern void		SetupShaderViewProjectionMatrix(float*e_pfVPMatrix);
	extern float*	GetCurrentViewProjectionMatrix();
    //=====================
    //a base class for animtion control(LERP)
    //a start time for animation control and a position(matrix is better!?)
    //base render and update interface to be call for any inherit calss
    //three major funciton are internal start,render,update,u have to implement them.
    //some common function have to implement with timing base.
    //=====================
    class	cFatmingAnimationRule:virtual public cBaseBehavior//,virtual public NamedTypedObject
    {
    protected:
	    //how long we have past
	    float	m_fPastTime;
	    //the position will effect to this animation role,it could be set by parent or itself.
	    GET_SET_DEC(Vector3,m_vPos,GetPos,SetPos);
	    //cue time to start
	    GET_SET_DEC(float,m_fStartTime,GetStartTime,SetStartTime);
	    GET_SET_DEC(bool,m_bStart,IsStart,SetStart);						//while the time is over start time the real start is start,not just call start mean it start
	    bool		m_bAnimationLoop;
	    bool		m_bAnimationDone;
	    //===================================
    private:
	    //the start condition for inherit class to implement
	    virtual	void	InternalInit() = 0;
	    //if m_bAnimationDone is false and start condition is actived,it will be actived.
	    virtual	void	InternalUpdate(float e_fElpaseTime) = 0;
	    //if m_bAnimationDone is false and start condition is actived,it will be actived.
	    virtual	void	InternalRender() = 0;
    public:
	    cFatmingAnimationRule();
	    cFatmingAnimationRule(cFatmingAnimationRule*e_pFatmingAnimationRule);
	    virtual	~cFatmingAnimationRule();
		float*			GetStartTimePointer(){return &m_fStartTime;}
	    virtual void	Init();
	    virtual void	Update(float e_fElpaseTime);
	    virtual void	Render();
	    bool			IsAnimationLoop();
	    virtual bool	IsAnimationDone();
	    //the time has been past
	    float			GetPastTime();
	    virtual	void	SetAnimationLoop(bool e_bLoop);
	    virtual	void	SetAnimationDone(bool e_bAnimationDone);
		//
	    virtual	void	RearrangeTime(float e_fPercenttage) = 0;
	    virtual	void	RenderByGlobalTime(float e_fTime,bool e_bRotateWithAngle) = 0;
	    virtual	void	InvertOrder() = 0;
	    //start and last time.
	    virtual	float	GetEndTime() = 0;
		virtual	void	Destroy(){}
    };
    //======================================
    //basicly the TYPE is a inherit class base on cFatmingAnimationRule
    //this is a list to update and render all animation rule objects
    //it also have timing base function for the objects.
    //======================================
    template<class	TYPE>
    class	cFatmingGroupBehaviorList:public cObjectListByName<TYPE>,virtual public cBaseBehavior
    {
    protected:
		//helpful for object different camera view
		GET_SET_DEC(cMatrix44*,m_pCameraView,GetCameraView,SetCameraView);
	    //sometimes the time is up,but the performance is still going,we expected it finished,and stop update
	    //depend what kind performance is it going
	    virtual bool	WaitUntilAllAnimationIsDone(){ return false; }
	    //all path's center
	    Vector3	m_vRotationCenter;
	    //time as far as we are
	    float	m_fCurrentTime;
	    //counting number buffer
	    GET_SET_DEC(int,m_iCurrentPlayCount,GetCurrentPlayCount,SetGetCurrentPlayCount);
	    //how many time we want to play,if the value is -1 it will be loop
	    GET_SET_DEC(int,m_iPlayCount,GetPlayCount,SetPlayCount);
	    //total time we are going to play
	    GET_SET_DEC(float,m_fTotalPlayTime,GetTotalPlayTime,SetTotalPlayTime);
	    //0 to 1,while progress is i it's end.
	    GET_SET_DEC(float,m_fCurrentProgress,GetCurrentProgress,SetCurrentProgress);
    public:

	    cFatmingGroupBehaviorList();
	    //for clone
	    cFatmingGroupBehaviorList(cFatmingGroupBehaviorList*e_pFatmingGroupBehaviorList);
	    virtual	~cFatmingGroupBehaviorList();
		virtual	NamedTypedObject*	Clone(){ return NamedTypedObject::Clone(); }
		virtual	void Destroy(){ cObjectListByName<TYPE>::Destroy(); }
	    virtual	void	Init();
	    virtual	void	Update(float e_fElpaseTime);
	    //using global time to update,true while done
	    virtual	void	UpdateByGlobalTime(float e_fGlobalTime);
	    //if u like to change position call SetPosition
	    virtual	void	Render();
	    virtual	void	RenderByGlobalTime();
	    //we should call it after AddObject or merge
	    virtual	void	RefreshTotalPlayTime();
	    //if u wanna set done is true,is it done(play done)?
	    virtual	bool	IsDone(){ return m_fCurrentProgress>=1?true:false; }
	    virtual	void	Stop(){ m_fCurrentTime = m_fTotalPlayTime+1.f; m_iCurrentPlayCount = m_iPlayCount; m_fCurrentProgress = 1.f; }
	    virtual	void	SetLoop(bool e_b){ if(e_b)m_iPlayCount = -1;else m_iPlayCount = 1; }
	    virtual	bool	IsLoop(){if(m_iPlayCount==-1)return true;return false;}
	    virtual	void	RearrangeTime(float e_fNewTime);
	    virtual bool    IsStart(){ return m_fCurrentProgress>0.f?true:false; }
	    //2 group combine together
	    virtual	void	Merge(cFatmingGroupBehaviorList<TYPE>*e_pTYPEVector);
	    //set point between start time  and last time invert.
	    virtual	void	InvertOrder();
	    //follow group behavior's angle or direction or matrix to rotate,default is false
	    virtual	bool	IsRotation(){ return false; }
	    //
	    Vector3	GetRotationCenter(){return m_vRotationCenter;}
    };

    //======================================
    //
    //======================================
    template<class	TYPE>
    cFatmingGroupBehaviorList<TYPE>::cFatmingGroupBehaviorList()
    {
		m_pCameraView = 0;
	    m_fCurrentTime = 0.f;
	    m_iPlayCount = 1;
	    m_fTotalPlayTime = 0.f;
	    m_fCurrentProgress = 0.f;
	    m_iCurrentPlayCount = 0;
	    m_vRotationCenter = Vector3::Zero;
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    cFatmingGroupBehaviorList<TYPE>::cFatmingGroupBehaviorList(cFatmingGroupBehaviorList<TYPE>*e_pFatmingGroupBehaviorList)
    {
		assert(e_pFatmingGroupBehaviorList);
		this->SetName(e_pFatmingGroupBehaviorList->GetName());
		m_pCameraView = 0;
		if( e_pFatmingGroupBehaviorList->m_pCameraView )
		{
			m_pCameraView = new cMatrix44(*e_pFatmingGroupBehaviorList->m_pCameraView);
		}
	    m_vRotationCenter = e_pFatmingGroupBehaviorList->m_vRotationCenter;
	    m_fCurrentTime = 0.f;
	    m_iPlayCount = e_pFatmingGroupBehaviorList->GetPlayCount();
	    m_fTotalPlayTime = e_pFatmingGroupBehaviorList->GetTotalPlayTime();
	    m_fCurrentProgress = 0.f;
	    m_iCurrentPlayCount = 0;
	    //the type may not as we expect so do this by all implements class!
	    //int	l_iNum = e_pFatmingGroupBehaviorList->Count();
	    //for( int i=0;i<l_iNum;++i )
	    //{
	    //	TYPE* l_pTYPE = new TYPE((*e_pFatmingGroupBehaviorList)[i]);
	    //	bool	l_b = this->AddObject(l_pTYPE);
	    //	assert(l_b&&"same name");
	    //}
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    cFatmingGroupBehaviorList<TYPE>::~cFatmingGroupBehaviorList()
    {
		SAFE_DELETE(m_pCameraView);
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::Init()
    {
	    m_iCurrentPlayCount = 0;
	    m_fCurrentProgress = 0.f;
	    m_fCurrentTime = EPSIONAL;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
		    ((cFatmingAnimationRule*)this->m_ObjectList[i])->Init();
	    //fuck here is weired,but I am LAZY to find out why it is now,so I call update instead  send a elpase time into start.
		    //((cFatmingAnimationRule*)this->m_ObjectList[i])->Start(e_fElpaseTime);
	    //if( e_fElpaseTime !=0.f )
	    //for( UINT i=0;i<this->m_ObjectList.size();++i )
		  //  ((cFatmingAnimationRule*)this->m_ObjectList[i])->Update(e_fElpaseTime);
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::Update(float e_fElpaseTime)
    {
	    if( m_iPlayCount != m_iCurrentPlayCount )
	    {
		    this->m_fCurrentTime+=e_fElpaseTime;
		    m_fCurrentProgress = this->m_fCurrentTime/this->m_fTotalPlayTime;
			size_t	l_iCount = this->m_ObjectList.size();
		    for( UINT i=0;i<l_iCount;++i )
			    ((cFatmingAnimationRule*)this->m_ObjectList[i])->Update(e_fElpaseTime);
		    if( m_fCurrentProgress >= 1.f )
		    {
			    ++m_iCurrentPlayCount;
			    if( m_iPlayCount == -1 ||//if loop
				    m_iPlayCount > m_iCurrentPlayCount )
			    {
				    assert(m_fTotalPlayTime>0.f);
				    if(m_fCurrentTime>=m_fTotalPlayTime)
				    {
					    //m_fCurrentTime -= this->m_fTotalPlayTime;
					    //m_fCurrentTime -= ((int)(m_fCurrentTime/this->m_fTotalPlayTime)*m_fTotalPlayTime);
					    m_fCurrentTime = GetFloatModulus(m_fCurrentTime,m_fTotalPlayTime);
				    }
				    for( UINT i=0;i<l_iCount;++i )
				    {
				        //if(this->m_ObjectList[i]->IsAnimationLoop())
					    //if(!this->m_ObjectList[i]->IsAnimationDone())//here is quit weired to force the loop event to restart so here we skip to start if it's loop.
						//compile do not like me....fuck it must filled a  time or it filled 1.0f autoly
						    ((cFatmingAnimationRule*)this->m_ObjectList[i])->Init();
				    }
				    //update to the time has elpase
				    if( m_fCurrentTime>0.f )
					    this->Update(m_fCurrentTime);
			    }
		    }
	    }
	    else//peformance is still going,but the count is reached.
	    if(WaitUntilAllAnimationIsDone())
	    {
		    for( UINT i=0;i<this->m_ObjectList.size();++i )
			    ((cFatmingAnimationRule*)this->m_ObjectList[i])->Update(e_fElpaseTime);
	    }
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::UpdateByGlobalTime(float e_fGlobalTime)
    {
	    m_fCurrentTime = e_fGlobalTime;
	    m_fCurrentProgress = this->m_fCurrentTime/this->m_fTotalPlayTime;
	    if(this->m_iPlayCount == -1)
	    {
		    if( m_fCurrentProgress>=1.f )
		    {
			    m_fCurrentTime = e_fGlobalTime-m_fCurrentTime;
			    m_fCurrentProgress = this->m_fCurrentTime/this->m_fTotalPlayTime;
		    }
	    }
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::Render()
    {
	    if( m_fCurrentProgress >= 1.f && !WaitUntilAllAnimationIsDone() )
		    return;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    TYPE* l_pTYPE = this->m_ObjectList[i];
		    if( l_pTYPE->IsAnimationDone() )
			    continue;
		    ((cFatmingAnimationRule*)l_pTYPE)->Render();
	    }
    }

    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RenderByGlobalTime()
    {
	    if( m_fCurrentProgress >= 1.f )
		    return;
	    //int	l_iLastImageIndex = -1;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    if( IsRotation() )
			    this->m_ObjectList[i]->RenderByGlobalTime(this->m_fCurrentTime,true);
		    else
			    this->m_ObjectList[i]->RenderByGlobalTime(this->m_fCurrentTime,false);
	    }
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RefreshTotalPlayTime()
    {
	    this->m_fTotalPlayTime = 0.f;
	    for( UINT i=0;i<this->m_ObjectList.size();++i )
	    {
		    TYPE* l_pType = this->m_ObjectList[i];
		    float	l_fTime = l_pType->GetEndTime();
		    if(m_fTotalPlayTime<l_fTime)
			    m_fTotalPlayTime = l_fTime;
	    }
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::InvertOrder()
    {
	    int	l_iNum = this->Count();
	    for( int i=0;i<l_iNum;++i )
		    (*this)[i]->InvertOrder();
    }
    //======================================
    //
    //======================================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::RearrangeTime(float e_fNewTime)
    {
	    if( m_fTotalPlayTime!=0.f )
	    {
		    float	l_fPercenttage = e_fNewTime/this->m_fTotalPlayTime;
		    int	l_iNum = this->Count();
		    for( int i=0;i<l_iNum;++i )
		    {
			    TYPE*	l_pType = (*this)[i];
			    l_pType->RearrangeTime(l_fPercenttage);
		    }
		    m_fTotalPlayTime = e_fNewTime;
	    }
    }
    //===============================
    //
    //===============================
    template<class	TYPE>
    void	cFatmingGroupBehaviorList<TYPE>::Merge(cFatmingGroupBehaviorList<TYPE>* e_pTYPEVector)
    {
		if( e_pTYPEVector && this == e_pTYPEVector )
			return;
	    int	l_iNum = e_pTYPEVector->Count();
	    WCHAR	l_strName[MAX_PATH];
	    for(int i =0;i<l_iNum;++i)
	    {
		    TYPE* l_pOriginal = (*e_pTYPEVector)[i];
		    TYPE* l_pNewOne = dynamic_cast<TYPE*>(l_pOriginal->Clone());
		    while(!this->AddObject(l_pNewOne))
		    {
			    static int	l_isSeriesIndex = 0;
			    ++l_isSeriesIndex;
			    swprintf(l_strName,MAX_PATH,L"%ls%d\0",l_pOriginal->GetName(),l_isSeriesIndex);
			    l_pNewOne->SetName(l_strName);
		    }
	    }
    }
}
#endif