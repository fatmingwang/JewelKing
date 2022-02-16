#ifndef _MOUSE_TOUCH_
#define _MOUSE_TOUCH_
//===========================
//main function is 
//MouseDown
//MouseMove(MouseHorver)    ,m_bCollided is possible be false,because it mouse down at orther place and move to this object!.
//MouseUp                   ,it's possible not mouse down at the object we expect,if it's not first time we into it will call InternalMouseLeave
//MouseLeave                ,it still possible to mouse Leave again and again...
//they all call InternalCollide to detect is it in the range we expect.
//
//main overwrite funciton are InternalMouseXXXXX
//you should overwrite their content 
//
//ex there is a button
//while mouse is click on it or not
//MouseClick is called!!
//if it's in the range InternalMouseDown will be called!.
//then the state will change to eOMB_HORVER
//if mouse up or mouse leave they also call InternalMouseXXXX to implement their goal



//
//cClickMouseBehaviorList will override write the function
//MouseDown
//MouseMove(MouseHorver)
//MouseUp
//MouseLeave
//because it inherient cObjectListByName
//

//===========================
namespace FATMING_CORE
{
	//===========================================
	//the reason why we do want a global sMouseMoveData
	//because the button event just spend a bit calculate works,
	//and we might need to work at multi touch so just keep every mouse has a sMouseMoveData
	//so we could get all touch's data
	//===========================================
    struct  sMouseMoveData
    {
		//the pos for mouse down
        POINT   DownPos;
		//the pos for mouse up
        POINT   UpPos;
		//the pos for mouse move
        POINT   MovePos;
		//the position for how mouse move in each mouse move call.
		POINT	FrameMovePos;
        UINT	MouseDownTime;
        float   fDownToUpElpaseTime;
		//for mouse double click time
		UINT	uiLastMouseUpClickElpaseTime;

        sMouseMoveData(){ MouseDownTime = 0; fDownToUpElpaseTime = 0.f; FrameMovePos.x = FrameMovePos.y = DownPos.x = DownPos.y = UpPos.x = UpPos.y = MovePos.x = MovePos.y = 0; uiLastMouseUpClickElpaseTime = 0; }
        ~sMouseMoveData(){}
        float   DownUpDistance(Vector2 *e_pvDirection = 0)
        {
            Vector2 l_vDirection((float)(UpPos.x-DownPos.x),(float)(UpPos.y-DownPos.y));
            if( e_pvDirection )
                *e_pvDirection = l_vDirection;
            return l_vDirection.Length();
        }
        float   DownMoveDistance(Vector2 *e_pvDirection = 0)
        {
            Vector2 l_vDirection((float)(MovePos.x-DownPos.x),(float)(MovePos.y-DownPos.y));
            if( e_pvDirection )
                *e_pvDirection = l_vDirection;
            return l_vDirection.Length();
        }
        Vector2   DownUp(){ return Vector2((float)(DownPos.x-UpPos.x),(float)(DownPos.y-UpPos.y)); }
        Vector2   DownMove(){ return Vector2((float)(DownPos.x-MovePos.x),(float)(DownPos.y-MovePos.y)); }
		//
		void	MouseDown(int e_iPosX,int e_iPosY);
		void	MouseMove(int e_iPosX,int e_iPosY);
		//if true double click is true or not
		bool	MouseUp(int e_iPosX,int e_iPosY);
    };
    //mouse behavior,Mouse Down,Mouse Horver MouseLeave MouseUp
    //basicly it will has a parent cClickMouseBehaviorList.
    class   cClickMouseBehavior:virtual public NamedTypedObject
    {
    protected:
		bool		m_bKeyDown;
		//for button event.
		GET_SET_DEC(unsigned char,m_ucKeyData,GetKeyData,SetKeyData);
		GET_SET_DEC(bool,m_bPressedRepeatMouseUp,IsPressedRepeatMouseUp,SetPressedRepeatMouseUp);
		UT::sTimeCounter	m_TCForRepeat;
		static	float m_sfRepeatTime;
		//here whoy not use a global share mouse data the reason is:the object could not same coordinate,it's possible use different resolution.
        GET_SET_DEC(sMouseMoveData,m_MouseMoveData,GetMouseMoveData,SetMouseMoveData);
        eObjectMouseBehavior    m_eObjectMouseBehavior;
        //if mouse down is at this object,this object will be true,or false,a member work for the class implement it
		//it's also mean it's in touched
		//please set this at InternalCllided or mouse down or the plac u expaected.
        bool    m_bCollided;
        //the range for vaild collide,simply use for button
        GET_SET_DEC(Vector4,m_vCollisionRange,GetCollisionRange,SetCollisionRange);
        //if mouse up and the position is not similar,it's mean the mouse move from the other,so ignore it
        virtual void			InternalMouseMove(int e_iPosX,int e_iPosY){}
        //first time mouse into
        virtual void			InternalMouseDown(int e_iPosX,int e_iPosY){}
        //while mouse clicked is up
        virtual void    		InternalMouseUp(int e_iPosX,int e_iPosY){}
        //while mouse click and its in the collided,not necessary,most time it call InternalMouseUp
        virtual void    		InternalMouseLeave(int e_iPosX,int e_iPosY){}
        //if mouse double click,if u dud's expect this it will call InternalMouseUp again
		//make sure cGameApp::m_sTimeAndFPS has benn active.
        virtual void    		InternalMouseDoubleClickUp(int e_iPosX,int e_iPosY){ InternalMouseUp(e_iPosX,e_iPosY); }
        //it only call by MouseMove,usually it will set state back to idle...
        inline  virtual void    MouseLeave(int e_iPosX,int e_iPosY)
        {
            if( m_eObjectMouseBehavior == eOMB_HORVER || m_eObjectMouseBehavior == eOMB_FIRST_TIME_INTO )
            {
                InternalMouseLeave(e_iPosX,e_iPosY);
            }
			m_eObjectMouseBehavior = eOMB_LEAVE;
        }
        //if touch point is collided return true or false.
		virtual bool    		InternalCollide(int e_iPosX,int e_iPosY){ return m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY); }
		//whild mouse up and satisfied
		GET_SET_DEC(DoButtonGoal_Callback,m_pDoButtonGoal_Callback,GetDoButtonGoal,SetDoButtonGoal);
    public:
        //cClickMouseBehavior(RECT e_RECT){ m_CollisionRange = e_RECT; m_bCollided = false; m_eObjectMouseBehavior = eOMB_NONE; }
        cClickMouseBehavior();
		cClickMouseBehavior(cClickMouseBehavior*e_pClickMouseBehavior);
		//CLONE_MYSELF(cClickMouseBehavior);
        ~cClickMouseBehavior();
        eObjectMouseBehavior    GetMouseBehavior(){ return m_eObjectMouseBehavior; }
        //one of option is mouse up and m_eObjectMouseBehavior is eOMB_UP return true
        virtual bool    		IsSatisfiedCondition();
		virtual	void			SetSatisfiedCondition(bool e_b){ if( e_b )m_eObjectMouseBehavior = eOMB_UP;else m_eObjectMouseBehavior = eOMB_NONE; }
        //set state as start.
		//this->m_bCollided = false; m_bKeyDown = false;
		virtual void    		Init();
        //horver and move
        virtual void    		MouseMove(int e_iPosX,int e_iPosY);
        //first time into
        virtual void    		MouseDown(int e_iPosX,int e_iPosY);
        //
        virtual void    		MouseUp(int e_iPosX,int e_iPosY);
		//for keyboard
		virtual void			ButtonClick();
		//for keyboard
		virtual void			ButtonPressed();
		//for keyboard
		virtual void			ButtonUp();
		//for IO
		virtual	void			SingnalProcess(unsigned char e_usKey);
		virtual	void			SingnalProcess();
        //it's possible we want to move the object
        virtual void    		SetRelativePosition( Vector3 e_vPos ){}
        //
        virtual void    		Update(float e_fElpaseTime);
        //
        virtual void    		Render(){}
		//
		virtual void    		DebugRender();
		//
		bool					IsCollided(){ return m_bCollided; }
		//
		bool					IsCollided(int e_iPosX,int e_iPosY)
		{
			return InternalCollide(e_iPosX,e_iPosY);		
		}

		virtual bool    Collide(int e_iPosX,int e_iPosY){ return InternalCollide(e_iPosX,e_iPosY); }
		virtual	void	SetPos(Vector3 e_vPos);
		void			SetRepeatTime(float e_fRepeatTime);
    };
    //=============================================
    //ensure T has inherient cClickMouseBehavior!
    //a lots object all need to do mouse behavior
    //ex: a bunch of images,we have to decide which one image we selected and if mouse move we have to show the mouse is on
    //the image.
	//if one of children has been collided,mouse behavior will skip rest in the vector.
    //=============================================
    template<class T>   class cClickMouseBehaviorList:virtual public cObjectListByName<T>,virtual public cClickMouseBehavior
    {
    protected:
		//while its children has thing want to lock all mouse or render set working aas true or false
		//
		//GET_SET_DEC(bool,m_bWorking,IsWorking,SetWorking);
		//while mouse is doen and the object is collided,if this is not NULL,mouse behavior only work for it,if mouse is worked but mouse move is leaved,it won't work again
		T*						m_pCurrentMouseDownCollidedObject;
		//the top mouse behavior,if multi object works and now switch one of those or set it to a special one,only works for this top object
		//this one only works for mouse behavior
		//we could simple do not add any child into this object,but set m_pTopClickMouseBehavior as we want
		//then while object is satisfied change to next behavior
		T*						m_pTopClickMouseBehavior;
		//the working object which is satisfied.
        T*  					m_pCurrentWorkingObject;
        //one of option condition is satisfied,so all mouse behavior is done
        bool 					m_bClickMouseBehaviorSatisfied;
        virtual bool    		RenderViewportCollide(cClickMouseBehavior*e_pClickMouseBehavior){ return true; }
    public:
        cClickMouseBehaviorList(){ this->m_pCurrentMouseDownCollidedObject = 0;this->m_pCurrentWorkingObject = 0; this->m_bClickMouseBehaviorSatisfied = false;	 m_pTopClickMouseBehavior = 0; }
		cClickMouseBehaviorList(cClickMouseBehaviorList<T>*e_pClickMouseBehaviorList):cClickMouseBehavior(e_pClickMouseBehaviorList)
		{
			ADD_CLONE_DATA_TO_NEW_VECTOR(T,e_pClickMouseBehaviorList,this);
			this->m_pCurrentMouseDownCollidedObject = 0;
			this->m_pCurrentWorkingObject = 0;
			this->m_bClickMouseBehaviorSatisfied = false;
			this->m_pTopClickMouseBehavior = 0;
		}
		CLONE_MYSELF(cClickMouseBehaviorList);
        ~cClickMouseBehaviorList(){}
		T*						GetCurrentMouseDownCollidedObject(){ return m_pCurrentMouseDownCollidedObject; }
		void					SetCurrentMouseDownCollidedObject(T*e_pData){ m_pCurrentMouseDownCollidedObject = e_pData; }
		T*						GetTopClickMouseBehavior(){return m_pTopClickMouseBehavior;}
		void					SetTopClickMouseBehavior(T*e_pObject)
		{
			m_pTopClickMouseBehavior = e_pObject;	
			//if( m_pTopClickMouseBehavior )
				//m_pTopClickMouseBehavior->Init();
		}
		//inline  virtual void    Start(float e_fElpaseTime)
		//{
  //          int l_iCount = this->Count();
  //          for(int i=0;i<l_iCount;++i  )
  //          {
  //              this->m_ObjectList[i]->Start(e_fElpaseTime);
  //          }		
		//}

		//inline  virtual void    Destroy()
		//{
  //          int l_iCount = this->Count();
  //          for(int i=0;i<l_iCount;++i  )
  //          {
  //              this->m_ObjectList[i]->Destroy();
  //          }		
		//}
        inline  virtual void    Init()
        {
            cClickMouseBehavior::Init();
            this->m_bClickMouseBehaviorSatisfied = false;
			this->m_pCurrentMouseDownCollidedObject = 0;
            this->m_pCurrentWorkingObject = 0;
            int l_iCount = this->Count();
            for(int i=0;i<l_iCount;++i  )
            {
                this->m_ObjectList[i]->Init();
            }
        }
        //first time into
        inline  virtual void    MouseDown(int e_iPosX,int e_iPosY)
        {
			m_bCollided = false;
			//cClickMouseBehavior::MouseDown(e_iPosX,e_iPosY);
			if( m_pTopClickMouseBehavior )
			{
				m_pTopClickMouseBehavior->MouseDown(e_iPosX,e_iPosY);
				if( m_pTopClickMouseBehavior->IsCollided() )
				{
					m_bCollided = true;
				}
				return;
			}
            if( this->m_bClickMouseBehaviorSatisfied )
			{
				//m_pCurrentWorkingObject->MouseDown(e_iPosX,e_iPosY);
                return;
			}
			//we also could set m_pCurrentMouseDownCollidedObject
			if( !this->m_pCurrentMouseDownCollidedObject )
			{
				int l_iCount = this->Count();
				for(int i=0;i<l_iCount;++i  )
				{
					this->m_ObjectList[i]->MouseDown(e_iPosX,e_iPosY);
					if(this->m_ObjectList[i]->IsCollided())
					{
						this->m_pCurrentMouseDownCollidedObject = this->m_ObjectList[i];
						m_bCollided = true;
						return;
					}
				}
				this->m_pCurrentMouseDownCollidedObject = 0;
			}
			else
				m_bCollided = true;
        }
        inline  virtual void    MouseMove(int e_iPosX,int e_iPosY)
        {
            //cClickMouseBehavior::MouseMove(e_iPosX,e_iPosY);
			if( m_pTopClickMouseBehavior )
			{
				m_pTopClickMouseBehavior->MouseMove(e_iPosX,e_iPosY);
				return;
			}
            if( this->m_bClickMouseBehaviorSatisfied )
			{
				//m_pCurrentWorkingObject->MouseMove(e_iPosX,e_iPosY);
                return;
			}
			if( this->m_pCurrentMouseDownCollidedObject )
			{
				this->m_pCurrentMouseDownCollidedObject->MouseMove(e_iPosX,e_iPosY);
				return;
			}
        }
        //
        inline  virtual void    MouseUp(int e_iPosX,int e_iPosY)
        {
            //cClickMouseBehavior::MouseUp(e_iPosX,e_iPosY);
			if( m_pTopClickMouseBehavior )
				m_pTopClickMouseBehavior->MouseUp(e_iPosX,e_iPosY);
            if( this->m_bClickMouseBehaviorSatisfied )
			{
				//m_pCurrentWorkingObject->MouseUp(e_iPosX,e_iPosY);
                return;
			}
			m_bCollided = false;
			if( this->m_pCurrentMouseDownCollidedObject )
			{
				this->m_pCurrentMouseDownCollidedObject->MouseUp(e_iPosX,e_iPosY);
				if( this->m_pCurrentMouseDownCollidedObject->IsSatisfiedCondition() )
					m_pCurrentWorkingObject = this->m_pCurrentMouseDownCollidedObject;
				this->m_pCurrentMouseDownCollidedObject = 0;
				return;
			}
        }
        //if m_pCurrentWorkingObject is not NULL m_bClickMouseBehaviorSatisfied is true
        T*  					GetCurrentWorkingObject(){return this->m_pCurrentWorkingObject;}
		int  					GetCurrentWorkingObjectIndex(){if( !m_pCurrentWorkingObject )return -1;return this->GetObjectIndexByPointer(m_pCurrentWorkingObject);}
        virtual bool    		IsSatisfiedCondition(){ return this->m_bClickMouseBehaviorSatisfied;  }
		void					SetSatisfiedCondition(bool e_b){ this->m_bClickMouseBehaviorSatisfied = e_b;}
        inline  virtual void    SetRelativePosition( Vector3 e_vPos )
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->SetRelativePosition(e_vPos);
            }
        }
        virtual void    		Update(float e_fElpaseTime)
        {
            //all mouse event condition is filled so ignore all child update.
            //only update current workingobject
            //for further use please overwrite this function!.
            if( this->m_bClickMouseBehaviorSatisfied )
            {
				if( m_pCurrentWorkingObject )
					this->m_pCurrentWorkingObject->Update(e_fElpaseTime);
                return;
            }
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Update(e_fElpaseTime);
                if( this->m_ObjectList[i]->IsSatisfiedCondition() )
                {
                    this->m_bClickMouseBehaviorSatisfied = true;
                    this->m_pCurrentWorkingObject = this->m_ObjectList[i];
                    return;
                }
            }
        }
        virtual     void		Render()
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                if( RenderViewportCollide(this->m_ObjectList[i]) )
                    this->m_ObjectList[i]->Render();
            }
        }
        virtual     void		DebugRender()
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				this->m_ObjectList[i]->DebugRender();
            }
        }
		virtual	int				IsChildrenCollided()
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				if(this->m_ObjectList[i]->IsCollided())
					return i;
            }
			return -1;
		}
		virtual	int				IsChildrenCollided(int e_iPosX,int e_iPosY)
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				if(this->m_ObjectList[i]->IsCollided(e_iPosX,e_iPosY))
					return i;
            }
			return -1;
		}
		virtual	int				IsChildrenSatisfied()
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				if(this->m_ObjectList[i]->IsSatisfiedCondition())
					return i;
            }
			return -1;		
		}
		virtual	const WCHAR*			GetNextPhaseName()
		{
			if( m_pCurrentWorkingObject )
				return m_pCurrentWorkingObject->GetName();
			else
			if( this->m_pTopClickMouseBehavior )	
				return m_pTopClickMouseBehavior->GetName();
			return 0;
		}

		virtual	void			SingnalProcess()
		{
			//if( m_pCurrentWorkingObject )
			//	return;
			//if( m_pCurrentMouseDownCollidedObject )
			//{
			//	m_pCurrentMouseDownCollidedObject->SingnalProcess();
			//	if( m_pCurrentMouseDownCollidedObject->IsSatisfiedCondition() )
			//	{
			//		m_pCurrentWorkingObject = m_pCurrentMouseDownCollidedObject;
			//		m_pCurrentMouseDownCollidedObject = 0;
			//	}
			//	return ;
			//}
			int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				this->m_ObjectList[i]->SingnalProcess();
				if(this->m_ObjectList[i]->IsCollided())
				{
					//m_pCurrentMouseDownCollidedObject = this->m_ObjectList[i];
					//return;
				}
            }		
		}
		virtual void			SingnalProcess(unsigned char e_usKey)
		{
			//if( m_pCurrentWorkingObject )
			//	return;
			//if( m_pCurrentMouseDownCollidedObject )
			//{
			//	m_pCurrentMouseDownCollidedObject->SingnalProcess();
			//	if( m_pCurrentMouseDownCollidedObject->IsSatisfiedCondition() )
			//	{
			//		m_pCurrentWorkingObject = m_pCurrentMouseDownCollidedObject;
			//		m_pCurrentMouseDownCollidedObject = 0;
			//	}
			//	return ;
			//}
			int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
				this->m_ObjectList[i]->SingnalProcess(e_usKey);
				if(this->m_ObjectList[i]->IsCollided())
				{
					//m_pCurrentMouseDownCollidedObject = this->m_ObjectList[i];
					//return;
				}
            }
		}
    };
    //=================
    //T must inhirent cClickMouseBehavior
    //if a simplephase has a lot object and all of them are inhirent cClickMouseBehavior,
    //we could push into cDoWorkingObjecInMouseBehaviorList,then we could just do the mouse behavior for Working Object.
    //while first internal collided is true,should't call internal collided anymore
    //override InternalCollide to find out which one is m_pCurrentWorkingObject.
    //=================
    //template<class T> class   cDoWorkingObjecInMouseBehaviorList:virtual public cFMWorkingObjectChanger<T>,virtual public cClickMouseBehavior
    //{
    //protected:
    //    inline  virtual void    InternalMouseMove(int e_iPosX,int e_iPosY)
    //    {
    //        if( this->m_pCurrentWorkingObject )
    //        {
    //            this->m_pCurrentWorkingObject->MouseMove(e_iPosX,e_iPosY);
    //        }
    //    }
    //    //first time into
    //    inline  virtual void    InternalMouseDown(int e_iPosX,int e_iPosY)
    //    {
    //        if( this->m_pCurrentWorkingObject )
    //        {
    //            this->m_pCurrentWorkingObject->MouseDown(e_iPosX,e_iPosY);
    //        }            
    //    }
    //    //
    //    inline  virtual void    InternalMouseUp(int e_iPosX,int e_iPosY)
    //    {
    //        if( this->m_pCurrentWorkingObject )
    //        {
    //            this->m_pCurrentWorkingObject->MouseUp(e_iPosX,e_iPosY);
    //        }
    //        this->m_pCurrentWorkingObject = 0;
    //    }        
    //public:
    //    cDoWorkingObjecInMouseBehaviorList(){}
    //    ~cDoWorkingObjecInMouseBehaviorList(){}
    //    inline  virtual void    Init()
    //    {
    //        this->m_pCurrentWorkingObject = 0;
    //    }
    //    //if m_pCurrentWorkingObject is not NULL m_bClickMouseBehaviorSatisfied is true
    //    T*  GetCurrentWorkingObject(){return this->m_pCurrentWorkingObject;}    
    //    void    Update(float e_fElpaseTime)
    //    {
    //        int l_iCount = this->Count();
    //        for( int i=0;i<l_iCount;++i )
    //        {
    //            this->m_ObjectList[i]->Update(e_fElpaseTime);
    //        }
    //    }
    //    void    Render()
    //    {
    //        int l_iCount = this->Count();
    //        for( int i=0;i<l_iCount;++i )
    //        {
    //            this->m_ObjectList[i]->Render();
    //        }
    //    }    
    //};
	//========================
	//just do all the thing
	//========================
    template<class T> class   cDoAllMouseBehaviorList:public cClickMouseBehaviorList<T>
    {
    public:
		cDoAllMouseBehaviorList(cDoAllMouseBehaviorList<T>*e_pcDoAllMouseBehaviorList):cClickMouseBehaviorList<T>(e_pcDoAllMouseBehaviorList){}
        cDoAllMouseBehaviorList(){}
        ~cDoAllMouseBehaviorList(){}
        inline  virtual   void    Init()
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Init();
            }
        }
        inline  virtual   void    Update(float e_fElpaseTime)
        {
			bool	l_bSatisfied  =true;
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Update(e_fElpaseTime);
				if(!this->m_ObjectList[i]->IsSatisfiedCondition())
				{
					l_bSatisfied = false;
				}
            }
			this->m_bClickMouseBehaviorSatisfied = l_bSatisfied;
        }
        inline  virtual   void    Render()
        {
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->Render();
            }
        }
		inline  virtual   void    MouseDown(int e_iPosX,int e_iPosY)
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->MouseDown(e_iPosX,e_iPosY);
            }
		}
		inline  virtual   void    MouseMove(int e_iPosX,int e_iPosY)
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->MouseMove(e_iPosX,e_iPosY);
            }
		}
		inline  virtual   void    MouseUp(int e_iPosX,int e_iPosY)
		{
            int l_iCount = this->Count();
            for( int i=0;i<l_iCount;++i )
            {
                this->m_ObjectList[i]->MouseUp(e_iPosX,e_iPosY);
            }
		}
    };
//end namespace FATMING_CORE
}

#endif