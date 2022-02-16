#ifndef FRAME_H
#define FRAME_H

#include "Matrix44.h"

//-----------------------------------------------------------------------------
// Name: Frame
// Desc: An named typed object with a frame of reference and a bound
//       Note: X is Right, Y is Up, and Z is back
//-----------------------------------------------------------------------------

class Frame :virtual  public NamedTypedObject
{
    DEFINE_TYPE_INFO();

public:    
    Frame();
    ~Frame();
	Frame*						FinFrameByName(WCHAR*e_strName);
    // frame hierarchy
    void                        AddChild( Frame* pChild );
    void                        SetParent( Frame* pParent );  // Set to NULL to unparent
	void	                    SetNextibling(Frame*e_pNextSibling);
    Frame*                      GetFirstChild() const { return m_pFirstChild; }
    Frame*                      GetNextSibling() const { return m_pNextSibling; }
    Frame*                      GetParent() const { return m_pParent; }
    bool                        IsAncestor( Frame* pOtherFrame );
    bool                        IsChild( Frame* pOtherFrame );

    // cloned object support
    void                        DisconnectFromParent();    

    // local position
    Vector3                     GetLocalPosition() const { return *(Vector3*)(&m_LocalTransform.r[3]); }
    void                        SetLocalPosition( const VECTOR4& NewPosition ) { SetCachedWorldTransformDirty(); m_LocalTransform.r[3] = NewPosition; }
	void                        SetLocalPosition( Vector3 e_vPos ){ SetCachedWorldTransformDirty(); m_LocalTransform.r[3] = VECTOR4Set(e_vPos.x,e_vPos.y,e_vPos.z,1.f); }
   
    // world position
    Vector3                     GetWorldPosition();
    void                        SetWorldPosition( const VECTOR4& NewPosition );
	void                        SetWorldPosition( Vector3 e_vPos ){ SetWorldPosition(VECTOR4Set(e_vPos.x,e_vPos.y,e_vPos.z,1.f)); }

    // local transform
    cMatrix44                   GetLocalTransform() const { return m_LocalTransform; }
    void                        SetLocalTransform( const cMatrix44& LocalTransform ) { SetCachedWorldTransformDirty(); m_LocalTransform = LocalTransform; }

    // world transform
    cMatrix44                   GetWorldTransform();
    void                        SetWorldTransform( const cMatrix44& WorldTransform );
	cMatrix44*                  GetWorldTransformPointer();

    // basis vectors
    Vector3                     GetLocalRight() const { return *(Vector3*)(&m_LocalTransform.r[0]); }
    Vector3                     GetLocalUp() const { return *(Vector3*)(&m_LocalTransform.r[1]); }
    Vector3                     GetLocalDirection() const { return *(Vector3*)(&m_LocalTransform.r[2]); }      

    Vector3                     GetWorldRight();
    Vector3                     GetWorldUp();
    Vector3                     GetWorldDirection();

    // bound    
    //void                      SetLocalBound( const Bound& bound ) { m_LocalBound = bound; SetCachedWorldTransformDirty(); }
    //const Bound&              GetLocalBound() const { return m_LocalBound; }
    //const Bound&              GetWorldBound() { UpdateCachedWorldTransformIfNeeded(); return m_CachedWorldBound; }

	void						SetDestroyConnectionWhileDestoruction(bool e_bDestroyConnectionWhileDestroy);
	
	void                        Forward(float e_fDistance);
	//only for first level not all children
	int							GetNextLevelChildrenCount();
protected:
    void                        UpdateCachedWorldTransformIfNeeded();
    void                        SetCachedWorldTransformDirty();

    cMatrix44                   m_LocalTransform;
    cMatrix44                   m_CachedWorldTransform;

    //Bound                     m_LocalBound;
    //Bound                     m_CachedWorldBound;
    
    Frame*                      m_pParent;
    Frame*                      m_pNextSibling;
    Frame*                      m_pFirstChild;
	bool						m_bDestroyConnectionWhileDestroy;//when destruction is called,SetParent may not call as ue expected
};

#endif // ATG_FRAME_H
