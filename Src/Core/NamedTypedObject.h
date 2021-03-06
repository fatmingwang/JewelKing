/**
	@file Quaternion.h
	The file containing the class for quaternions.
*/


#ifndef NamedTypedObject_H
#define NamedTypedObject_H
#include "CommonDefine.h"
// C4127: conditional expression is constant
// this shows up when using STL without exception handling

#include <list>
#include <string.h>
#include "StringID.h"
//-----------------------------------------------------------------------------
// Name: DEFINE_TYPE_INFO
// Desc: Creates a type based on the NamedTypedObject class.  Use this in any
//       classes derived from NamedTypedObject to get the IsDerived() functionality.
//-----------------------------------------------------------------------------
//#define Conn(x,y) x##y
//#define ToChar(x) #@x		@ = ''
//#define ToString(x) #x	# = 


#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif
//fuck only works for windows
//#define TYPDE_DEFINE_MARCO(TYPE) const wchar_t*         ##TYPE::TypeID( L#TYPE );


#ifndef DEFINE_TYPE_INFO
#define DEFINE_TYPE_INFO() \
    public: \
	virtual bool IsDerivedFrom( const WCHAR* e_strTypeID ) const  { if( e_strTypeID == TypeID ) return true;  return __super::IsDerivedFrom( e_strTypeID ); }	\
	virtual const wchar_t*        Type() const { return TypeID; }\
	static const  wchar_t*        TypeID;
#endif
	//virtual bool                IsDerivedFrom( const wchar_t* _TypeID ) const { return ( _TypeID == TypeID ); }\

#define	CLONE_MYSELF(TYPE)	virtual NamedTypedObject*	Clone(){ return new TYPE(this); }
//! class name="NamedTypedObject" Desc="Base class for exposed objects"
//!    property name="Name" Desc="Name of the Object" Get=GetName Set=SetName
/**
	A quaternion.
	Used to represent rotations: quaternions have the sole advantage of reduced
	floating-point error when concatenating rotations.
*/
class NamedTypedObject
{
// Because NamedTypedObject has no base class we explicitly define the type info here- most classes
// will us the DEFINE_TYPE_INFO macro, with an explicit callout in the .cpp file.
// TypeID is public so you can use NamedTypedObject::TypeID for comparisons without an instance.
protected:
	//not necessary but quit helpful to find out who contain this
	//if not calling cObjectListByName::AddObject...AddObjectNeglectExist...
	//the owner might be NULL
	//but cObjectListByName::CopyListPointer and cObjectListByName::DumpListPointer will occur multi parents problem...be carfful!
	//if the object is cloned!the parent may not real parent.....it might be a reference.
	GET_SET_DEC(NamedTypedObject*,m_pWoner,GetOwner,SetOwner);
	//name coudl be same but unique won't,but it still has a limit for 2^64
	uint64	m_uiUniqueID;
public:
	NamedTypedObject();
	virtual ~NamedTypedObject();
	virtual bool                IsDerivedFrom( const wchar_t* _TypeID ) const { return ( _TypeID == TypeID ); }
	virtual const wchar_t*      Type() const { return TypeID; }
	static const  wchar_t*      TypeID;

public:
    //const wchar_t*	            GetName() const { return m_sObjectName; };
	const wchar_t*				GetName(){ return m_sObjectName; };
	char*						GetCharName();
	bool						IsSameName(wchar_t*e_strName){ return e_strName == m_sObjectName?true:false; }
	bool						IsSameName(NamedTypedObject*e_pNamedTypedObject){ return e_pNamedTypedObject->GetName()==m_sObjectName?true:false; }
	bool						IsSameType( NamedTypedObject*e_pNamedTypedObject ){ if(e_pNamedTypedObject->Type() == this->Type())return true;return false; }
	bool						IsSameType( const WCHAR*e_strTypeID ){ if(e_strTypeID == this->Type())return true;return false; }
	
    void                        SetName( wchar_t*e_pString ){m_sObjectName = e_pString;}
	void                        SetName( const wchar_t*e_pString ){	m_sObjectName = e_pString;}
    void                        SetName( char*e_pString );
    void                        SetName( const char*e_pString );
	//dump all un release data
	static void					DumpUnReleaseInfo();
	//haha......I am lazy to implement this virtual function now,because too many class have to do...so it only work at cSimpleGLTexture for now!
	//virtual WCHAR*              DescriptionInfo(){return L"I am NamedTypedObject";}
	//while u are assign object is not pointer it's possible get same unique ID
	uint64						GetUniqueID(){ return m_uiUniqueID; }
	//
	//virtual	void				Init(){}
	//virtual	void				Destroy(){}
	//virtual	void				Update(float e_fElpaseTime){}
	virtual	void				Render(){}
	virtual	NamedTypedObject*	Clone();
private:
	StringID					m_sObjectName;
#ifdef _DEBUG
public:
	//please don not chang this value.
	UINT64						m_ui64PointerVaildMagicID;
	bool						IsMagicIDMatch();//if not match pointer might delete at somewhere
#endif
};

const int DEFAULT_COLLECTION_HASHSIZE = 61;
class NameIndexedCollection
{
public:
    // iterator for the collection - it is made so you can 
    // delete the object at the current position and STILL do a ++
    // safely afterwards
    class iterator
    {
    friend class NameIndexedCollection;
    public:
        iterator();      

        NamedTypedObject*   operator*();
        iterator&               operator++( int ); // only postfix defined 
        iterator&               operator=( CONST iterator& iRHS );
        BOOL                    operator==( CONST iterator& iRHS );
        BOOL                    operator!=( CONST iterator& iRHS );
    private:   
        NameIndexedCollection*                  m_pCollection;
        std::vector<NamedTypedObject*>::iterator  m_iter;        
        int                                         m_iCurBucket;
    };
    friend class NameIndexedCollection::iterator;
        
    VOID    Add( NamedTypedObject *pObjectToAdd );        // Add a NamedTypedObject to the collection        
    VOID    Remove( NamedTypedObject *pObjectToRemove );  // Remove a NamedTypedObject from the collection        
    //NamedTypedObject* Find( CONST WCHAR* strName );       // Find a NamedTypedObject in the collection
	NamedTypedObject* Find( NamedTypedObject*e_pNamedTypedObject );
    NamedTypedObject* FindTyped( CONST WCHAR* strName, const StringID TypeID );       // Find a NamedTypedObject of a certain type in the collection
    DWORD   Size();
       
    iterator        begin();
    iterator        end();
private:
    std::vector<NamedTypedObject *> s_Lists[ DEFAULT_COLLECTION_HASHSIZE ];     
};



/** get a unique ID.
	@return The values for identifiy the object. */
uint64	GetGlobalUniqueID();

class	cBaseBehavior:virtual public NamedTypedObject
{
	virtual	void InternalUpdate(){}
#ifdef	_DEBUG
	uint64		m_ui64TimeStamp;
#endif
public:
	virtual	void Init() = 0;
	virtual	void Update(float e_fElpaseTime);
	virtual	void Render() = 0;
	virtual	void Destroy() = 0;
};


#endif // NamedTypedObject_H

//#ifndef _X_H_
//#define _X_H_
//
//	class	cSample:public NamedTypedObject
//	{
//	public:
//		//DEFINE_TYPE_INFO();
//		cSample();
//		~cSample();
//		virtual	void	Update(float e_fElpaseTime);
//		virtual	void	Render();
//		virtual	void	DebugRender();
//	};
//
//#include "stdfax.h"
//#include "xx.h"
//	const wchar_t*         cSample::TypeID( L"cSample" );
//	cSample::cSample()
//	{
//	
//	}
//
//	cSample::~cSample()
//	{
//	
//	}
//	void	cSample::Update(float e_fElpaseTime)
//	{
//	
//	}
//	void	cSample::Render()
//	{
//	
//	}
//	void	cSample::DebugRender()
//{
//
//}
//
//#endif