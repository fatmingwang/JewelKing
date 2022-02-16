#ifndef TemplateClass_H
#define TemplateClass_H
#include <vector>
#include <assert.h>
#include "CommonDefine.h"
#include "NamedTypedObject.h"
using namespace std;
////====================
////if the object is not belong to the other,itself must to be delete object or release object
////====================
//template <class T>class ObjectAndName
//{
//public:
//	T	*m_pObject;
//	const char*	m_sName;
//	ObjectAndName(const char*e_pString,T* e_p){ m_sName = e_pString; m_pObject = e_p; };
//	~ObjectAndName(){};
//	const char*	GetName(){ return m_sName; }
//	T*		GetObject(){ return m_pObject; }
//	void	SetObject(T*e_p){ m_pObject = e_p; }
//};

//the T must inherit NamedTypedObject
//T must be a pointer
//void    MyAssert2(bool e_b,char*e_str = "Error")
//{
  //      assert(e_b&&e_str);
//}

//only work for the class has new self
#define	CLONE_VECTOR_TO_NEW_VECTOR(TYPE,SRC_VECTOR,DEST_VECTOR)	\
int	l_iCount = SRC_VECTOR->Count();								\
for(int i=0;i<l_iCount;++i)										\
{																\
	TYPE* l_pData = new TYPE((*SRC_VECTOR)[i]);					\
	DEST_VECTOR->AddObjectNeglectExist(l_pData);				\
}


#define	ADD_CLONE_DATA_TO_NEW_VECTOR(TYPE,SRC_VECTOR,DEST_VECTOR)	\
int	l_iCount = SRC_VECTOR->Count();									\
for(int i=0;i<l_iCount;++i)											\
{																	\
	TYPE* l_pData = dynamic_cast<TYPE*>((*SRC_VECTOR)[i]->Clone());	\
	DEST_VECTOR->AddObjectNeglectExist(l_pData);					\
}


#define LIST_DO_FOR_ALL_CHILDREN(List,FUNCTION) \
{												\
	int l_iCount = List->Count();               \
	for( int i=0;i<l_iCount;++i )               \
	{                                           \
	    (*List)[i]->FUNCTION;                   \
	}											\
}

#define	NEW_TEMPLATE_LIST(TYPE,CHILDTYPE,DESTINATION,SOURCE,FROMRESOURCE)	\
	TYPE* DESTINATION = new TYPE();											\
	DESTINATION->SetName(SOURCE->GetName());								\
	if( FROMRESOURCE )														\
	{																		\
		DESTINATION->CopyListPointer(SOURCE);								\
	}																		\
	else																	\
	{																		\
		int	l_iCount = SOURCE->Count();										\
		for( int i=0;i<l_iCount;++i  )										\
		{																	\
			CHILDTYPE* l_p = new CHILDTYPE( (*SOURCE)[i] );					\
			l_p->SetName((*SOURCE)[i]->GetName());							\
			DESTINATION->AddObjectNeglectExist(l_p);						\
		}																	\
	}

template<class TYPE,class CHILDTYPE>
TYPE*	NewTemplateList(TYPE*e_pSource,bool e_bFromREsource)
{
	TYPE* l_pData = new TYPE();
	l_pData->SetName(e_pSource->GetName());
	if( e_bFromREsource )
	{
		l_pData->CopyListPointer(e_pSource);
	}
	else
	{
		int	l_iCount = e_pSource->Count();
		for( int i=0;i<l_iCount;++i  )
		{
			CHILDTYPE* l_p = new CHILDTYPE( (*e_pSource)[i] );
			l_p->SetName((*e_pSource)[i]->GetName());
			l_pData->AddObjectNeglectExist(l_p);
		}
	}
	return l_pData;
}
//template < class LIST,class CHILDREN_DATA_TYPE > class cObjectListTree;
//key function RemoveResourceObject.
template <class T> class cObjectListByName:virtual public NamedTypedObject
{
	//friend class	cObjectListTree<cObjectListByName<T*>,T*>;
	//this one to remove the resource if object has something must to delete
	//ex:AnimationParser or particle or else.....
	virtual	void	RemoveResourceObject(NamedTypedObject*e_pObject){}
protected:
	vector < T* > m_ObjectList;
	bool	m_bFromResource;
public:
	cObjectListByName(bool e_bFromResource = false){ m_bFromResource = e_bFromResource; }
	cObjectListByName(cObjectListByName<T>*e_pObjectListByName){CloneFromList(e_pObjectListByName);}
	virtual	NamedTypedObject*	Clone(){ return new cObjectListByName<T>(this); }
	~cObjectListByName(){ Destroy(); }
	//ensure what are u doing while old data is not from resource
    inline  void    CloneFromList(cObjectListByName<T>*e_pObjectListByName)
    {
        int l_iCount = e_pObjectListByName->Count();
        for( int i=0;i<l_iCount;++i )
        {
            T*l_p = dynamic_cast<T*>(e_pObjectListByName->GetObject(i)->Clone());
            l_p->SetName((*e_pObjectListByName)[i]->GetName());
            this->AddObjectNeglectExist(l_p);
        }
		m_bFromResource = false;
    }
	bool	IsFromResource(){ return m_bFromResource; }
	void	SetFromResource(bool e_bFromResource){ m_bFromResource = e_bFromResource; }
	virtual	void	Destroy()
	{
		if(!m_bFromResource)
		{
			while( m_ObjectList.size() )
			{
				T* l_p = m_ObjectList[0];
				if( l_p )
				{
					RemoveResourceObject(dynamic_cast<NamedTypedObject*>(l_p));
					SAFE_DELETE(l_p);
				}
				m_ObjectList.erase(m_ObjectList.begin());
				//m_ObjectList[i].~T();
			}
		}
		m_ObjectList.clear();
	}

	T*	GetObjectByUniqueID(uint64 e_uiID)
	{
		for(UINT i=0;i<m_ObjectList.size();++i)
		{
			T* l_t = m_ObjectList[i];
			if(l_t->GetUniqueID() == e_uiID)
				return l_t;
		}
		return 0;	
	}
	inline T* operator [](const WCHAR*e_pString){return GetObject(e_pString);}

	inline T* operator [](int e_iIndex){return GetObject(e_iIndex);}

	inline T* GetObject(int e_i){if( e_i<this->Count() && e_i>-1 )return m_ObjectList[e_i];	return 0;}

	inline T* GetObject(WCHAR*e_pString){return this->GetObject(this->GetObjectIndexByName(e_pString));	}

	inline T* GetObject(const WCHAR*e_pString){return this->GetObject(this->GetObjectIndexByName(e_pString));}

	inline T* GetLastObject(){	return this->GetObject(m_ObjectList.size()-1);}
	//however data is existed or not remove data and add it
	inline void	ReplaceObject(T* e_T){if( e_T ){RemoveObject(e_T);AddObject(e_T);}}

	inline T* GetObjectByType(const WCHAR*e_pString)
	{
		for( UINT i=0;i<m_ObjectList.size();++i  )
		{
			if( m_ObjectList[i]->Type() == e_pString )
				return m_ObjectList[i];
		}
		return 0;
	}
	
	inline T* GetObjectByTypePointer(T*e_pObject)
	{
		for( UINT i=0;i<m_ObjectList.size();++i  )
		{
			if( m_ObjectList[i]->Type() == e_pObject->Type() )
				return m_ObjectList[i];
		}
		return 0;
	}	
	//only need to overwrite RemoveObject(int e_iIndex).
	inline	bool	RemoveObject(const WCHAR*e_pString){return RemoveObject(GetObjectIndexByName(e_pString));}
	inline	bool	RemoveObject(WCHAR*e_pString){return RemoveObject(GetObjectIndexByName(e_pString));}
	inline	bool	RemoveObject(NamedTypedObject* e_t){return RemoveObject(GetObjectIndexByName(e_t->GetName()));}
	inline	virtual	bool	RemoveObject(int e_iIndex)
	{
		T*l_p = this->GetObject(e_iIndex);
		if( l_p )
		{
			RemoveResourceObject(l_p);
			m_ObjectList.erase(m_ObjectList.begin()+e_iIndex);
			delete l_p;
			return true;
		}
		return false;
	}
	void	RemoveObjectWithoutDelete(int e_iIndex)
	{
		T*l_p = this->GetObject(e_iIndex);
		if( l_p )
		{
			l_p->SetOwner(0);
			m_ObjectList.erase(m_ObjectList.begin()+e_iIndex);
		}
	}
	inline	void	RemoveObjectWithoutDelete(const WCHAR*e_strName){ RemoveObjectWithoutDelete((WCHAR*)e_strName); }
	inline	void	RemoveObjectWithoutDelete(WCHAR*e_strName){int	l_iIndex = GetObjectIndexByName(e_strName);if( l_iIndex != -1 )	RemoveObjectWithoutDelete(l_iIndex);}
	inline	void	RemoveObjectWithoutDelete(T*e_pObject){int	l_iIndex = GetObjectIndexByPointer(e_pObject);if( l_iIndex != -1 )	RemoveObjectWithoutDelete(l_iIndex);}
	inline	bool	AddObject(T* e_t){if( !e_t )return false;if( GetObject(e_t->GetName()) )return false;AddObjectNeglectExist(e_t);	return true;}
	inline	virtual	bool	AddObjectNeglectExist(T* e_t){m_ObjectList.push_back(e_t);e_t->SetOwner(this);return true;}

	int	Count(){ return (int)m_ObjectList.size(); }

	inline	int	GetObjectIndexByName(const WCHAR* e_pString){return GetObjectIndexByName((WCHAR*)e_pString);}
	inline	int	GetObjectIndexByName(WCHAR* e_pString)
	{
		if( e_pString )
		{
			for( UINT i=0;i<m_ObjectList.size();++i  )
			{
				if( !wcscmp(m_ObjectList[i]->GetName(),e_pString) )
				{
					return i;
				}
			}
		}
		return -1;
	}

	inline	int	GetObjectIndexByPointer(T* e_T)
	{
		for( UINT i=0;i<m_ObjectList.size();++i  )
		{
			if( m_ObjectList[i] == e_T )
			{
				return i;
			}
		}
		return -1;
	}

	std::vector<T*>*	GetList(){return &m_ObjectList;}
	//=========================
	//set from resource true and assign pointer to dest
	//get data from dest
	//=========================
	void	CopyListPointer(cObjectListByName<T>*e_pList)
	{
		Destroy();
		this->m_bFromResource = true;
		for( int i=0;i<e_pList->Count();++i )
		{
			m_ObjectList.push_back((*e_pList)[i]);
		}
	}
	//=========================
	//dump data to dest list and clear self
	//beware while has any data in the dest list it could be occor some problem
	//=========================
	virtual	void	DumpListPointer(cObjectListByName<T>*e_pList)
	{
		this->m_bFromResource = false;
		for( int i=0;i<Count();++i )
		{
			bool	l_b = e_pList->AddObject(m_ObjectList[i]);
			assert(l_b&&"such file has already exist");
		}
		e_pList->SetFromResource(false);
		m_ObjectList.clear();
	}
	//=========================
	//ensure m_bFromResource set as well
	//
	//=========================
	virtual	void	AddListToMe(cObjectListByName<T>*e_pList,bool e_bIgnoreSameName = false,bool e_bIgnoreSameData = false)
	{
		this->m_bFromResource = true;
		for( int i=0;i<e_pList->Count();++i )
		{
		    if( !e_bIgnoreSameName )
		    {
		        if(this->GetObject(e_pList->m_ObjectList[i]->GetName()))
		            continue;
		    }
			if( !e_bIgnoreSameData )
			{
				if(GetObjectIndexByPointer(e_pList->m_ObjectList[i]) != -1)
				{
					continue;
				}
			}
			m_ObjectList.push_back(e_pList->m_ObjectList[i]);
	    }
	}
	//==================================
	//swap data index
	//2 objects change index
	//==================================
	void	SwapIndex(int e_iIndex1,int e_iIndex2)
	{
		T* l_1 = this->m_ObjectList[e_iIndex1];
		T* l_2 = this->m_ObjectList[e_iIndex2];
		this->m_ObjectList[e_iIndex1] = l_2;
		this->m_ObjectList[e_iIndex2] = l_1;
	}
	//a vector contain a bunch data,now input a index it is possible bigger than last index or smaller 0,
	//so we have to find the proper one if we want it be loop
	int		GetLoopIndex(int e_iCurrentIndex)
	{
		int	l_iTotalCount = (int)m_ObjectList.size();
		if( e_iCurrentIndex<0 )
			return l_iTotalCount-1;
		if( e_iCurrentIndex+1>=l_iTotalCount )
			return l_iTotalCount-1;
		return e_iCurrentIndex;
	}
};
//T must inhirent NamedTypedObject
template<class  T>
class   cFMWorkingObjectChanger:public cObjectListByName<T>
{
protected:
    T*  m_pCurrentWorkingObject;
public:
    cFMWorkingObjectChanger(){ m_pCurrentWorkingObject = 0;}
    ~cFMWorkingObjectChanger(){}
	cFMWorkingObjectChanger(cFMWorkingObjectChanger<T>*e_pFMWorkingObjectChanger):cObjectListByName<T>(e_pFMWorkingObjectChanger){ m_pCurrentWorkingObject = 0; }

	inline	int		GetCurrentWorkingObjectIndex(){ if( !m_pCurrentWorkingObject )return -1;return this->GetObjectIndexByPointer(m_pCurrentWorkingObject); }
    inline  T*      GetCurrentWorkingObject(){return m_pCurrentWorkingObject; }
    inline  virtual	bool      SetCurrentWorkingObject(int e_iIndex,bool e_bRestart = true)
	{
		m_pCurrentWorkingObject = this->GetObject(e_iIndex);
		if( m_pCurrentWorkingObject )
		{
			if( e_bRestart ) 
			{
				m_pCurrentWorkingObject->Init();
				m_pCurrentWorkingObject->Update(0.f);
			}
			return true;
		}
		return false;
	}
    inline  bool      SetCurrentWorkingObject(const WCHAR*e_strName,bool e_bRestart = true)
	{
		return SetCurrentWorkingObject(this->GetObjectIndexByName(e_strName),e_bRestart);
	}
    inline  void    InitAllObject()
    {
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            this->m_ObjectList[i]->Init();
        }
    }
	virtual	void	Init(){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Init(); }
	//virtual	void	Destroy(){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Destroy(); }
	virtual	void	Update(float e_fElpaseTime){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Update(e_fElpaseTime); }
	virtual	void	Render(){ if( m_pCurrentWorkingObject )m_pCurrentWorkingObject->Render(); }
};

//template <class T>const wchar_t*cObjectListByName<T>::TypeID( L"cObjectListByName" );

//sample
// 
//	class MyListManager:public cObjectListByName<cObjectListByName<NamedTypedObject>,NamedTypedObject*>
//	{
//		this one just like tree nodes.
//		a list contain a list and the list which is use to be contained,may contain a list!
//	};
//
//  LIST must inherit cObjectListByName or cObjectListByName
//
//LIST as parent
//CHILDREN_DATA_TYPE as its child
template < class LIST,class CHILDREN_DATA_TYPE > class cObjectListTree:public cObjectListByName< LIST >
{
protected:
	LIST*	m_pCurrentList;
public:
	cObjectListTree()
	{
		m_pCurrentList = 0;
	}
	~cObjectListTree(){}

	bool	AddChildObject(WCHAR*e_strListName,CHILDREN_DATA_TYPE e_pObject)
	{
		return this->GetObject(e_strListName)->AddObject(e_pObject);
	}

	CHILDREN_DATA_TYPE*	GetChildObject(WCHAR*e_strListName,WCHAR*ChildName){ return this->GetObject(e_strListName)->GetObject(ChildName); }
	CHILDREN_DATA_TYPE*	GetChildObject(int e_iListIndex,int e_iChildIndex){ return this->GetObject(e_iListIndex)->GetObject(e_iChildIndex); }
	//from current list
	CHILDREN_DATA_TYPE*	GetChildObject(WCHAR*e_strChildName){ return m_pCurrentList->GetObject(e_strChildName); }
	CHILDREN_DATA_TYPE*	GetChildObject(int e_iChildIndex){ return m_pCurrentList->GetObject(e_iChildIndex); }

	void	SetCurrentList(WCHAR*e_strName){m_pCurrentList = this->GetObject(e_strName);}
	void	SetCurrentList(LIST* e_pList){ m_pCurrentList = e_pList; }
	void	SetCurrentList(int e_iIndex){ m_pCurrentList = this->m_ObjectList[e_iIndex]; }

	LIST*	GetCurrentList(){ return m_pCurrentList; }
	//this one is a sample and it sux....it should considate recursive sitiation(list of list of list...listtree type)
	CHILDREN_DATA_TYPE*	GetListChildByUniqueID(uint64 e_uiID)
	{
		int	l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			LIST*	l_pList = this->m_ObjectList[i];
			int	l_iCount2 = l_pList->Count();
			for( int j=0;j<l_iCount2;++j )
			{
				if( (*l_pList)[j]->GetUniqueID() == e_uiID )
				{
					return (*l_pList)[j];
				}
			}
		}
		return 0;
	}
};

//=================
//the object T must has update and render
//=================
template <class T> class    cBehaviorObjectList:public cObjectListByName<T>
{
public:
    virtual void    Init()
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Init();
        }
    }
    virtual void    Update(float e_fElpaseTime)
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Update(e_fElpaseTime);
        }
    }
    inline	virtual void    Render()
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Render();
        }    
    }
    //inline	virtual void    Start(float e_fElpaseTime = EPSIONAL)
    //{
    //    int l_iCount = this->Count();
    //    for(int i=0;i<l_iCount;++i)
    //    {
    //        this->m_ObjectList[i]->Start(e_fElpaseTime);
    //    }    
    //}
    inline	virtual void    Destroy()
    {
        int l_iCount = this->Count();
        for(int i=0;i<l_iCount;++i)
        {
            this->m_ObjectList[i]->Destroy();
        }
    }
};
//for level editor cMapLayer
template <class T> class cObjectListByNameWithData:public cObjectListByName<T>
{
	GET_SET_DEC(void*,m_pData,GetData,SetData);
public:
	cObjectListByNameWithData(bool e_bFromResource = false):cObjectListByName<T>(e_bFromResource)
	{
		m_pData = 0;
	}
	~cObjectListByNameWithData()
	{
		SAFE_DELETE(m_pData);
	}
};

//T must inhirent NamedTypedObject
template<class T>class	cObjectAndName:public NamedTypedObject
{
	T	*m_pObject;
public:
	//must set up object
	cObjectAndName(const WCHAR*e_strName){SetName(e_strName);}
	cObjectAndName(const WCHAR*e_strName,T*e_pObject)
	{
		SetName(e_strName);
		m_pObject = dynamic_cast<T*>(e_pObject->Clone());
	}
	cObjectAndName(cObjectAndName*e_pObjectAndName)
	{
		SetName(e_pObjectAndName->GetName());
		m_pObject = dynamic_cast<T*>(e_pObjectAndName->m_pObject->Clone());
	}
	CLONE_MYSELF(cObjectAndName);
	~cObjectAndName()
	{
		SAFE_DELETE(m_pObject);
	}
	//virtual	void	Start(float e_fElpaseTime = 0.f){ m_pObject->Start(e_fElpaseTime); }
	virtual	void	Init(){ m_pObject->Init(); }
	virtual	void	Destroy(){ m_pObject->Destroy(); }
	virtual	void	Update(float e_fElpaseTime){ m_pObject->Update(e_fElpaseTime); }
	virtual	void	Render(){ m_pObject->Render(); }
	T*				GetObject(){ return m_pObject; }
	//ensure memory control
	void			SetObject(T*e_pData){ m_pObject = e_pData; }
};

#define	cStatusObjectChanger(T)	cFMWorkingObjectChanger<cObjectAndName<T > >
//typedef cFMWorkingObjectChanger<cObjectAndName<T>>	cStatusObjectChanger;

#endif