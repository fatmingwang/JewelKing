#include "stdafx.h"
#include "NamedTypedObject.h"


uint64	g_ui64GlobalUniqueID = 0;
uint64	g_ui64TimeStamp = 0;

uint64	GetGlobalUniqueID()
{
	return ++g_ui64GlobalUniqueID;
}

uint64	GetTimeStampID()
{
	return ++g_ui64TimeStamp;
}


const wchar_t*         NamedTypedObject::TypeID( L"NamedTypedObject" );

#ifdef _DEBUG
bool	g_bDumpUsing = true;
//0,1,2 no use
int		g_iDumpUsing = 2;
std::vector<NamedTypedObject*>*g_pNamedTypedObjectList = 0;
NameIndexedCollection*g_pNameIndexedCollection = 0;
void	DebugResourceInfoAdd(NamedTypedObject*e_pNamedTypedObject)
{
	if( g_iDumpUsing == 0 )
	{
		if( !g_pNameIndexedCollection )
			g_pNameIndexedCollection = new NameIndexedCollection;
		g_pNameIndexedCollection->Add(e_pNamedTypedObject);
	}
	else
	if( g_iDumpUsing == 1 )
	{
		if(!g_pNamedTypedObjectList)
			g_pNamedTypedObjectList = new std::vector<NamedTypedObject*>;
		g_pNamedTypedObjectList->push_back(e_pNamedTypedObject);
	}
}

void	DebugResourceDelete(NamedTypedObject*e_pNamedTypedObject)
{
	if( g_iDumpUsing == 0 )
	{
		g_pNameIndexedCollection->Remove(e_pNamedTypedObject);
		if( g_pNameIndexedCollection->Size() == 0 )
		{
			SAFE_DELETE(g_pNameIndexedCollection);
		}
	}
	else
	if( g_iDumpUsing == 1 )
	{
		for( size_t i=0;i<g_pNamedTypedObjectList->size();++i )
		{
			if( (*g_pNamedTypedObjectList)[i] == e_pNamedTypedObject )
			{
				g_pNamedTypedObjectList->erase(g_pNamedTypedObjectList->begin()+i);
				break;
			}
		}
		if( g_pNamedTypedObjectList->size() == 0 )
		{
			SAFE_DELETE(g_pNamedTypedObjectList);
		}
	}
}

void	DumpUnReleaseInfo()
{
	if( g_iDumpUsing == 0 )
	{
		if( g_pNameIndexedCollection )
		{
			if( g_pNameIndexedCollection->Size() != 0 )
			{
				NameIndexedCollection::iterator	i = g_pNameIndexedCollection->begin();
				NameIndexedCollection::iterator	l_End = g_pNameIndexedCollection->end();
				for( ;i != l_End; i++  )
				{
#ifdef WIN32
					NamedTypedObject*l_pNamedTypedObject = dynamic_cast<NamedTypedObject*>(*i);
					WCHAR	l_strData[MAX_PATH];
					swprintf(l_strData,MAX_PATH,L"unreleaseData---Type:%ls,Name:%ls,UniqueID:%d\n",l_pNamedTypedObject->Type(),l_pNamedTypedObject->GetName(),l_pNamedTypedObject->GetUniqueID());
					OutputDebugString(l_strData);
#else
					printf("%s unrelease!!\n",(*g_pNamedTypedObjectList)[i]->GetCharName());
#endif
				}
			}
			SAFE_DELETE(g_pNameIndexedCollection);
		}
	}
	else
	if( g_iDumpUsing == 1 )
	{
		if( g_pNamedTypedObjectList )
		{
			assert(g_pNamedTypedObjectList->size() == 0&&"unrelease object exists!!");
			for( size_t i=0;i<g_pNamedTypedObjectList->size();++i )
			{
#ifdef WIN32
				WCHAR	l_strData[MAX_PATH];
				swprintf(l_strData,MAX_PATH,L"unreleaseData---Type:%ls,Name:%ls,UniqueID:%d\n",(*g_pNamedTypedObjectList)[i]->Type(),(*g_pNamedTypedObjectList)[i]->GetName(),(*g_pNamedTypedObjectList)[i]->GetUniqueID());
				OutputDebugString(l_strData);
#else
				printf("%s unrelease!!\n",(*g_pNamedTypedObjectList)[i]->GetCharName());
#endif
			}
		}
	}
}
const UINT64	g_ui64MAGICID = 123456789;
bool	NamedTypedObject::IsMagicIDMatch()
{
	return m_ui64PointerVaildMagicID == g_ui64MAGICID;;
}
#endif

NamedTypedObject::NamedTypedObject()
{
	this->m_uiUniqueID = GetGlobalUniqueID();
	m_pWoner = 0;
#ifdef _DEBUG
	m_ui64PointerVaildMagicID = g_ui64MAGICID;
	DebugResourceInfoAdd(this);
	if( m_uiUniqueID >= 551 && m_uiUniqueID <= 617 )
	{
		int a=0;
	}
	if( m_uiUniqueID>617 )
	{
		int a=0;
	}
#endif
}

NamedTypedObject::~NamedTypedObject()
{
#ifdef _DEBUG
	DebugResourceDelete(this);
#endif
	//Destroy();
}

char*NamedTypedObject::GetCharName()
{
	static char	l_sTemp[MAX_PATH];
	WCHAR_TO_CHAR(m_sObjectName,l_sTemp);
	return l_sTemp;
}

void NamedTypedObject::SetName( char*e_pString )
{
	WCHAR	l_str[2048];
	UT::CharToWchar(e_pString,l_str);
	this->m_sObjectName = l_str;
}

void NamedTypedObject::SetName( const char*e_pString )
{
	WCHAR	l_str[2048];
	UT::CharToWchar(e_pString,l_str);
	this->m_sObjectName = l_str;
}


void	NamedTypedObject::DumpUnReleaseInfo()
{
#ifdef _DEBUG
	::DumpUnReleaseInfo();
#endif
}


NamedTypedObject*	NamedTypedObject::Clone()
{
	UT::ErrorMsg(this->Type(),L"not implement clone");
	return 0;
}

void cBaseBehavior::Update(float e_fElpaseTime)
{
#ifdef	_DEBUG
	m_ui64TimeStamp = GetTimeStampID();
#endif
	InternalUpdate();
}

//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::iterator()
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator::iterator()
{
    m_pCollection = NULL;    
    m_iCurBucket = 0;
}

//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator*()
// Desc: iterator dereference
//-----------------------------------------------------------------------------
NamedTypedObject* NameIndexedCollection::iterator::operator*()
{
    return *m_iter;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator=()
// Desc: iterator assignment operator
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator& 
NameIndexedCollection::iterator::operator=( CONST NameIndexedCollection::iterator& iRHS)
{
    m_pCollection = iRHS.m_pCollection;
    m_iter = iRHS.m_iter;
    m_iCurBucket = iRHS.m_iCurBucket;
    return (*this);
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator==()
// Desc: iterator comparison operator
//-----------------------------------------------------------------------------
BOOL NameIndexedCollection::iterator::operator==( CONST NameIndexedCollection::iterator& iRHS)
{
    if ( ( m_pCollection == iRHS.m_pCollection ) &&
         ( m_iCurBucket == iRHS.m_iCurBucket ) && 
         ( m_iter == iRHS.m_iter ) )   
        return TRUE;
    return FALSE;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator!=()
// Desc: iterator comparison operator
//-----------------------------------------------------------------------------
BOOL NameIndexedCollection::iterator::operator!=( CONST NameIndexedCollection::iterator& iRHS)
{
    if ( ( m_pCollection == iRHS.m_pCollection ) &&
         ( m_iCurBucket == iRHS.m_iCurBucket ) && 
         ( m_iter == iRHS.m_iter ) )    
        return FALSE;
    return TRUE;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::iterator::operator++()
// Desc: iterator increment
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator& 
NameIndexedCollection::iterator::operator++( int )
{    
    assert( m_pCollection );

    // increment the iterator
    m_iter++;    

    // if we are at the end of a bucket, move to the next bucket
    while ( ( m_iter == m_pCollection->s_Lists[ m_iCurBucket ].end() ) &&
            ( m_iCurBucket < DEFAULT_COLLECTION_HASHSIZE - 1 ) )
    {        
        m_iCurBucket++;        
        m_iter = m_pCollection->s_Lists[ m_iCurBucket ].begin();
    }
        
    return (*this);
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::Add()
//-----------------------------------------------------------------------------
VOID NameIndexedCollection::Add( NamedTypedObject *pObjectToAdd )
{
    assert( pObjectToAdd );
	//int iBucket = pObjectToAdd->GetUniqueID()%DEFAULT_COLLECTION_HASHSIZE;
    //int iBucket = StringID::HashString( pObjectToAdd->GetName() ) % DEFAULT_COLLECTION_HASHSIZE;    
	//int iBucket = StringID::HashString( pObjectToAdd->Type() ) % DEFAULT_COLLECTION_HASHSIZE;    
	int iBucket = ((WCHAR)pObjectToAdd)%DEFAULT_COLLECTION_HASHSIZE;
    // $OPTIMIZE: use a fixed size allocator here
	if( pObjectToAdd->GetUniqueID() == 617 )
	{
		int a=0;
	}
    s_Lists[ iBucket ].push_back( pObjectToAdd );
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::Remove()
//-----------------------------------------------------------------------------
VOID NameIndexedCollection::Remove( NamedTypedObject *pObjectToRemove )
{
    assert( pObjectToRemove );
    
    //int iBucket = StringID::HashString( pObjectToRemove->GetName() ) % DEFAULT_COLLECTION_HASHSIZE;
	//int iBucket = pObjectToRemove->GetUniqueID()%DEFAULT_COLLECTION_HASHSIZE;
	//int iBucket = StringID::HashString( pObjectToRemove->Type() ) % DEFAULT_COLLECTION_HASHSIZE;    
	int iBucket = ((WCHAR)pObjectToRemove)%DEFAULT_COLLECTION_HASHSIZE;
	std::vector<NamedTypedObject *>*l_pList = &s_Lists[ iBucket ];
	int	l_iSize = l_pList->size();
	//uint64	l_ui46ID = pObjectToRemove->GetUniqueID();
	for( int i=l_iSize-1;i>-1;--i )
	{
#ifdef _DEBUG
		//if( (*l_pList)[i]->GetUniqueID() == 617 || (*l_pList)[i]->GetUniqueID() == 557 )
		//{
		//	int a=0;
		//}
		//if( pObjectToRemove->GetUniqueID() == 90574 || !pObjectToRemove->IsMagicIDMatch() )
		//{
		//	int a=0;
		//}
		//if(!pObjectToRemove->IsMagicIDMatch()||!(*l_pList)[i]->IsMagicIDMatch())
		//{
		//	int a=0;
		//}
#endif
		//if((*l_pList)[i]->GetUniqueID() == l_ui46ID)
		if( (*l_pList)[i] == pObjectToRemove )
		{
			s_Lists[ iBucket ].erase( l_pList->begin()+i );
			return;
		}
	}

	NamedTypedObject*l_pp = Find( pObjectToRemove );
	if( l_pp )
	{
		l_pp->GetName();
	}

}

NamedTypedObject* NameIndexedCollection::Find( NamedTypedObject*e_pNamedTypedObject )
{
    std::vector<NamedTypedObject *>::iterator i;
    
    //int iBucket = StringID::HashString( strName ) % DEFAULT_COLLECTION_HASHSIZE;
	int iBucket = ((WCHAR)e_pNamedTypedObject)%DEFAULT_COLLECTION_HASHSIZE;

    for ( i = s_Lists[ iBucket ].begin(); i != s_Lists[ iBucket ].end(); ++i )
    {
        if ( (*i) == e_pNamedTypedObject )
            break;
    }
    
    if ( i == s_Lists[ iBucket ].end() )
        return NULL;
    
    return (*i);
}
//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::Find()
//-----------------------------------------------------------------------------
//NamedTypedObject* NameIndexedCollection::Find( CONST WCHAR* strName )
//{
//    std::vector<NamedTypedObject *>::iterator i;
//    
//    int iBucket = StringID::HashString( strName ) % DEFAULT_COLLECTION_HASHSIZE;    
//
//    for ( i = s_Lists[ iBucket ].begin(); i != s_Lists[ iBucket ].end(); ++i )
//    {
//        if ( (*i)->GetName() == strName )
//            break;
//    }
//    
//    if ( i == s_Lists[ iBucket ].end() )
//        return NULL;
//    
//    return (*i);
//}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::FindTyped()
//-----------------------------------------------------------------------------
NamedTypedObject* NameIndexedCollection::FindTyped( CONST WCHAR* strName, const StringID TypeID )
{
    std::vector<NamedTypedObject *>::iterator i;

    int iBucket = StringID::HashString( strName ) % DEFAULT_COLLECTION_HASHSIZE;    

    for ( i = s_Lists[ iBucket ].begin(); i != s_Lists[ iBucket ].end(); ++i )
    {
        NamedTypedObject* pNTO = (*i);
        if ( pNTO->IsDerivedFrom( TypeID ) && pNTO->GetName() == strName )
            break;
    }

    if ( i == s_Lists[ iBucket ].end() )
        return NULL;

    return (*i);
}


DWORD NameIndexedCollection::Size()
{
    DWORD dwSize = 0;
    for( DWORD i = 0; i < DEFAULT_COLLECTION_HASHSIZE; i++ )
    {
        dwSize += s_Lists[i].size();
    }
    return dwSize;
}


//-----------------------------------------------------------------------------
// Name: NameIndexedCollection::begin()
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator NameIndexedCollection::begin()
{
    NameIndexedCollection::iterator rtn;

    rtn.m_pCollection = this;
    rtn.m_iCurBucket = 0;
    rtn.m_iter = s_Lists[ rtn.m_iCurBucket ].begin();
    
    // if we are at the end of a bucket, move to the next bucket
    while ( ( rtn.m_iter == s_Lists[ rtn.m_iCurBucket ].end() ) &&
            ( rtn.m_iCurBucket != DEFAULT_COLLECTION_HASHSIZE - 1 ) )
    {        
        rtn.m_iCurBucket++;
        rtn.m_iter = s_Lists[ rtn.m_iCurBucket ].begin();
    }
        
    return rtn;
}


//----------------------------------------------------------------------------
// Name: NameIndexedCollection::end()
//-----------------------------------------------------------------------------
NameIndexedCollection::iterator NameIndexedCollection::end()
{
    NameIndexedCollection::iterator rtn;

    rtn.m_pCollection = this;
    rtn.m_iCurBucket = DEFAULT_COLLECTION_HASHSIZE - 1;
    rtn.m_iter = s_Lists[ DEFAULT_COLLECTION_HASHSIZE - 1].end();    
    
    return rtn;
}
