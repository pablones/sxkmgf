// GameAutoPtr.h: declare for the CAutoPtr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEAUTOPTR_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
#define AFX_GAMEAUTOPTR_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_

#include "common.h"
#include "basetype.h"

//////////////////////////////////////////////////////////////////////
// 说明：简单智能指针，不支持COPY和=。该模板需要Release()函数支持
// 注意：包含该对象的类必须是个不能COYP和=的类。

template<TYPENAME T>
class CAutoPtr
{
public:
	CAutoPtr() : m_ptr(NULL)					{}
	CAutoPtr(T* ptr) : m_ptr(ptr)				{}		// may be null
	~CAutoPtr()									{ if(m_ptr) m_ptr->Release(); }
	CAutoPtr& operator=(T* ptr)					{ ASSERT(!m_ptr && ptr); m_ptr=ptr; return *this; }
	T*	pop()									{ T* p=m_ptr; m_ptr=NULL; return p; }
private: // can't use these
	CAutoPtr& operator=(const CAutoPtr& ptr);
	CAutoPtr(const CAutoPtr&);

public:
	operator T*()									{ return m_ptr; }
	POINT_NO_RELEASE<T>*	operator->()								{ ASSERT(m_ptr); return static_cast<POINT_NO_RELEASE<T>* >(m_ptr); }
	operator const T*()	 	 const					{ return m_ptr; }
	const POINT_NO_RELEASE<T>*	operator->() const					{ ASSERT(m_ptr); return static_cast<POINT_NO_RELEASE<T>* >(m_ptr); }

protected:
	T*			m_ptr;
};

#define  STACK_DUMP_ENABLE

#ifdef STACK_DUMP_ENABLE
	#define STACK_DUMP() static MyStackWalker sw;sw.ShowCallstack();
#else
	#define STACK_DUMP()
#endif


#endif // !defined(AFX_GAMEAUTOPTR_H__5708545C_E8F2_4155_AC66_4191BE0F150E__INCLUDED_)
