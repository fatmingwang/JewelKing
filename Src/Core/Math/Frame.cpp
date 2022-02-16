#include "../stdafx.h"
#include "Frame.h"


const wchar_t*         Frame::TypeID( L"Frame" );

const FLOAT    FRAME_DIRTY_WORLD_CACHE = 1e10f;


//-----------------------------------------------------------------------------
// Name: Frame::Frame()
//-----------------------------------------------------------------------------
Frame::Frame()
{
    m_pParent = NULL;
    m_pNextSibling = NULL;
    m_pFirstChild = NULL;
    
    m_CachedWorldTransform._11 = FRAME_DIRTY_WORLD_CACHE;
	m_LocalTransform = cMatrix44::Identity;
	m_bDestroyConnectionWhileDestroy =  true;
}



//-----------------------------------------------------------------------------
// Name: Frame::~Frame()
//-----------------------------------------------------------------------------
Frame::~Frame()
{
	if( m_bDestroyConnectionWhileDestroy )
		SetParent( NULL );
}


Frame*	Frame::FinFrameByName(WCHAR*e_strName)
{
	if( !wcscmp(e_strName,GetName()) )
		return this;
	if( this->GetNextSibling() != NULL)
	{		
		return GetNextSibling()->FinFrameByName(e_strName);
	}

	if (GetFirstChild() != NULL)
	{
		return GetFirstChild()->FinFrameByName(e_strName);
	}
	return NULL;
}
//-----------------------------------------------------------------------------
// Name: Frame::AddChild()
//-----------------------------------------------------------------------------
void Frame::AddChild( Frame* pChild )
{
    assert( pChild );    
    pChild->SetParent( this );    
}


//-----------------------------------------------------------------------------
// Name: Frame::SetParent()
//-----------------------------------------------------------------------------
void Frame::SetParent( Frame* pParent )
{
    Frame *pSrch, *pLast;

    UpdateCachedWorldTransformIfNeeded();

    if( m_pParent )
    {
        pLast = NULL;
        for( pSrch = m_pParent->m_pFirstChild; pSrch; pSrch = pSrch->m_pNextSibling )
        {
            if ( pSrch == this )
                break;
            pLast = pSrch;
        }

        // If we can't find this frame in the old parent's list, assert
        assert( pSrch );

        // Remove us from the list
        if ( pLast ) 
        {
            pLast->m_pNextSibling = m_pNextSibling;
        }
        else // at the beginning of the list
        {
            m_pParent->m_pFirstChild = m_pNextSibling;
        }

        m_pNextSibling = NULL;
        m_pParent = NULL;
    }

    // Add us to the new parent's list               
    if ( pParent )
    {
        m_pParent = pParent;   
        m_pNextSibling = pParent->m_pFirstChild;
        pParent->m_pFirstChild = this;
    }

    // now we update our local transform to match the old world transform
    // (i.e. move under our new parent's frame with no detectable change)
    
    cMatrix44 worldMatrix = m_CachedWorldTransform;
    SetWorldTransform( worldMatrix );
}


void	Frame::SetNextibling(Frame*e_pNextSibling)
{
	m_pNextSibling = e_pNextSibling;
	if( m_pNextSibling )
		m_pNextSibling->m_pParent = this->m_pParent;
}

//-----------------------------------------------------------------------------
// Name: Frame::IsAncestor()
// Desc: Returns TRUE if the specified frame is an ancestor of this frame
//       ancestor = parent, parent->parent, etc.
//       Also returns TRUE if specified frame is NULL.
//-----------------------------------------------------------------------------
bool Frame::IsAncestor( Frame* pOtherFrame )
{
    if( pOtherFrame == NULL )
        return true;
    if( pOtherFrame == this )
        return false;
    Frame* pFrame = GetParent();
    while( pFrame != NULL )
    {
        if( pFrame == pOtherFrame )
            return true;
        pFrame = pFrame->GetParent();
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: Frame::IsChild()
// Desc: Returns TRUE if the specified frame is a direct child of this frame
//-----------------------------------------------------------------------------
bool Frame::IsChild( Frame* pOtherFrame )
{
    if( pOtherFrame == NULL )
        return false;
    Frame* pChild = GetFirstChild();
    while( pChild != NULL )
    {
        if( pChild == pOtherFrame )
            return true;
        pChild = pChild->GetNextSibling();
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: Frame::GetWorldPosition()
//-----------------------------------------------------------------------------
Vector3 Frame::GetWorldPosition()
{
    UpdateCachedWorldTransformIfNeeded();
    return *(Vector3*)(&m_CachedWorldTransform.r[3]);
}


//-----------------------------------------------------------------------------
// Name: Frame::SetWorldPosition()
//-----------------------------------------------------------------------------
void Frame::SetWorldPosition( const VECTOR4 &NewPosition )
{
    UpdateCachedWorldTransformIfNeeded();
    cMatrix44 ModifiedWorldTransform = m_CachedWorldTransform;

    ModifiedWorldTransform.r[3] = NewPosition;
    
    SetWorldTransform( ModifiedWorldTransform );
}


//-----------------------------------------------------------------------------
// Name: Frame::GetWorldTransform()
//-----------------------------------------------------------------------------
cMatrix44 Frame::GetWorldTransform() 
{
    UpdateCachedWorldTransformIfNeeded();
    return m_CachedWorldTransform;
}

cMatrix44*	Frame::GetWorldTransformPointer()
{
    UpdateCachedWorldTransformIfNeeded();
    return &m_CachedWorldTransform;
}

//-----------------------------------------------------------------------------
// Name: Frame::GetWorldRight()
//-----------------------------------------------------------------------------
Vector3 Frame::GetWorldRight() 
{ 
    UpdateCachedWorldTransformIfNeeded();
    return *(Vector3*)(&m_CachedWorldTransform.r[0]);
}


//-----------------------------------------------------------------------------
// Name: Frame::GetWorldUp()
//-----------------------------------------------------------------------------
Vector3 Frame::GetWorldUp() 
{ 
    UpdateCachedWorldTransformIfNeeded();
    return *(Vector3*)(&m_CachedWorldTransform.r[1]);
}


//-----------------------------------------------------------------------------
// Name: Frame::GetWorldDirection()
//-----------------------------------------------------------------------------
Vector3 Frame::GetWorldDirection()
{ 
    UpdateCachedWorldTransformIfNeeded(); 
    return *(Vector3*)(&m_CachedWorldTransform.r[2]);
}


//-----------------------------------------------------------------------------
// Name: Frame::SetWorldTransform()
//-----------------------------------------------------------------------------
void Frame::SetWorldTransform( const cMatrix44& WorldTransform )
{
    cMatrix44 ParentInverse;
    //Vector4 vDeterminant;

    if ( m_pParent )
    {    
        m_pParent->UpdateCachedWorldTransformIfNeeded();
		ParentInverse = m_pParent->m_CachedWorldTransform.Inverted();

		//if the transform is not correct check this function,because I am lazy to check it's correction
        //ParentInverse = cMatrix44Inverse( &vDeterminant, m_pParent->m_CachedWorldTransform );
        // Parent's matrix should be invertible
        //assert( vDeterminant.x != 0.0f );
        
        m_LocalTransform = ParentInverse*WorldTransform;
    }
    else
    {
        m_LocalTransform = WorldTransform;    
    }

    SetCachedWorldTransformDirty();
}


//-----------------------------------------------------------------------------
// Name: Frame::UpdateCachedWorldTransformIfNeeded()
//-----------------------------------------------------------------------------
void Frame::UpdateCachedWorldTransformIfNeeded()
{
    if( m_CachedWorldTransform._11 == FRAME_DIRTY_WORLD_CACHE )
    {
        if( m_pParent )
        {
            m_pParent->UpdateCachedWorldTransformIfNeeded();        
            m_CachedWorldTransform = m_pParent->m_CachedWorldTransform * m_LocalTransform;
            //m_CachedWorldBound = m_LocalBound * m_CachedWorldTransform;
        }
        else
        {   
            m_CachedWorldTransform = m_LocalTransform;
			//if( m_LocalBound.GetType() != 0 )
			//	m_CachedWorldBound = m_LocalBound * m_CachedWorldTransform;;

        }
    }
}


//-----------------------------------------------------------------------------
// Name: Frame::SetCachedWorldTransformDirty()
//-----------------------------------------------------------------------------
void Frame::SetCachedWorldTransformDirty()
{
    Frame *pChild;
    m_CachedWorldTransform._11 = FRAME_DIRTY_WORLD_CACHE;
    
    for( pChild = m_pFirstChild; pChild; pChild = pChild->m_pNextSibling )
    {
        pChild->SetCachedWorldTransformDirty();
    }
}


//-----------------------------------------------------------------------------
// Name: Frame::DisconnectFromParent
// Desc: Removes parent link from the object, and converts world transform
//       to local transform.  This method is only to be used on clones of
//       scene objects, since the parent is not notified of the disconnection.
//-----------------------------------------------------------------------------
void Frame::DisconnectFromParent()
{
    // Debug check to make sure the parent really isn't the parent of this clone
#ifdef _DEBUG
    if( m_pParent != NULL )
    {
        assert( !m_pParent->IsChild( this ) );
    }
#endif

    UpdateCachedWorldTransformIfNeeded();
    m_pParent = NULL;
    SetWorldTransform( m_CachedWorldTransform );
}

void	Frame::SetDestroyConnectionWhileDestoruction(bool e_bDestroyConnectionWhileDestroy)
{
	m_bDestroyConnectionWhileDestroy = e_bDestroyConnectionWhileDestroy;
}

void    Frame::Forward(float e_fDistance)
{
    Vector3 l_vDirection = this->GetLocalDirection();
    this->SetWorldPosition(this->GetWorldPosition()+l_vDirection*e_fDistance);
}

int		Frame::GetNextLevelChildrenCount()
{
	int	l_iCount = 0;
	Frame*l_pFrame = m_pFirstChild;
	while(l_pFrame)
	{
		++l_iCount;
		l_pFrame = l_pFrame->m_pNextSibling;
	}
	return l_iCount;
}