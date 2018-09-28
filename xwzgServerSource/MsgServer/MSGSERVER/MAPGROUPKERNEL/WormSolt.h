// WormSolt.h: interface for the CWormSolt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORMSOLT_H__130AA9C1_3286_4266_A0DC_5BA01594E10F__INCLUDED_)
#define AFX_WORMSOLT_H__130AA9C1_3286_4266_A0DC_5BA01594E10F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "vector"
#include "list"
using namespace std;
#include "atlbase.h"

typedef vector<VARIANT> ParamList;///万能参数列表
//
//template <class CALLBACK_CLASS>
//class SlotMethod_v1
//{
//	typedef void (CALLBACK_CLASS::*SoltFunc)(const ParamList&);
//public:
//	SlotMethod_v1(CALLBACK_CLASS *cb_class, SoltFunc pfun)
//	{
//		m_cb_class=cb_class;
//		m_pfun=pfun;
//	}
////		: Slot_v1<const ParamList&>(SlotMethod), cb_class(cb_class), pfun(pfun) {}
//	virtual void Call(const ParamList& param1) { (cb_class->*pfun)(param1); }
//private:
//	CALLBACK_CLASS *m_cb_class;
//	SoltFunc m_pfun;
//};
//
//<class CALLBACK_CLASS>
//class CWormSolt  
//{
//public:
//	CWormSolt();
//	virtual ~CWormSolt();
//	template <class CALLBACK_CLASS>
//	BOOL Connect(CALLBACK_CLASS* pcb_class,SoltFunc fun)
//	{
//		m_funlist.push_back(fun);
//		return m_funlist.size();
//	}
//	BOOL Remove(SoltFunc fun)
//	{
//		m_funlist.remove(fun);
//		return m_funlist.size();
//	}
//	BOOL Clear()
//	{
//		m_funlist.clear();
//		return m_funlist.size();
//	}
//	BOOL operator()(const ParamList& paramlist)
//	{
//		for(list<SoltFunc>::iterator it=m_funlist.begin();it!=m_funlist.end();++it)
//			(*it)(paramlist);
//		return m_funlist.size();
//	}
//private:
//	list<SoltFunc> m_funlist;
//};

//template <class const ParamList&>
class Slot_v1 //: public RefCount
{
private:
//	SlotType	m_Type;

public:
//	Slot_v1(SlotType t)
//	{
//		m_Type = t;
//	}
//
//	SlotType GetType() const
//	{
//		return m_Type;
//	}

	virtual void Call(const ParamList& param1) = 0;
};

//template <class const ParamList&>
//class SlotFunctor_v1 : public Slot_v1//<const ParamList&>
//{
//private:
//	typedef void (*callback_fun)(const ParamList&);
//	callback_fun pfun;
//
//public:
//	SlotFunctor_v1(callback_fun pfun) : pfun(pfun) {}
//	virtual void Call(const ParamList& param1) { pfun(param1); }
//	bool IsEqual(callback_fun pfun) {return this->pfun == pfun; }
//};
struct Section//同步代理
{
	Section()
	{
		InitializeCriticalSection(&CriticalSection);//初始化线程同步对象
		::EnterCriticalSection(&CriticalSection);//lock		
	}
	~Section()
	{
		LeaveCriticalSection(&CriticalSection);//unlock
		DeleteCriticalSection(&CriticalSection);		
	}
	CRITICAL_SECTION CriticalSection;
};

template <class CALLBACK_CLASS>
class SlotMethod_v1 : public Slot_v1
{
private:
	typedef void (CALLBACK_CLASS::*callback_fun)(const ParamList&);
	CALLBACK_CLASS *cb_class;
	callback_fun pfun;

public:
	SlotMethod_v1(CALLBACK_CLASS *cb_class, callback_fun pfun) : cb_class(cb_class), pfun(pfun) {}
	virtual void Call(const ParamList& param1) { (cb_class->*pfun)(param1); }
	bool IsEqual(CALLBACK_CLASS *cb_class, callback_fun pfun) {return this->pfun == pfun && this->cb_class == cb_class; }
};

class Singnal_v1
{
private:
	typedef Slot_v1 SlotType;
	typedef list<Slot_v1*> slot_list;
	slot_list Slots;

public:

	typedef list<Slot_v1*>::iterator slot_iterator;

private:
	void Connect(SlotType *slot)
	{
		Section lock;
		Slots.push_back(slot);
	}

	void Remove(SlotType*pSlot)
	{
		Section lock;
		Slots.remove(pSlot);
	}

public:
	Singnal_v1()
	{
	}

	virtual ~Singnal_v1()
	{
		Clear();
	}

//	void Connect(void (*pfun)(const ParamList&))
//	{
//		Connect(new SlotFunctor_v1(pfun));
//	}

	template <class CALLBACK_CLASS>
		void Connect(CALLBACK_CLASS *cb_class, void(CALLBACK_CLASS::*pfun)(const ParamList&))
	{
		Remove(cb_class,pfun);
		Connect(new SlotMethod_v1<CALLBACK_CLASS>(cb_class, pfun));
	}

	void operator ()(const ParamList& param1)
	{
		slot_iterator it = Slots.begin();

		while(it != Slots.end())
		{
			slot_iterator itNext = it;
			itNext++;
			SlotType *pSlot = *it;
			pSlot->Call(param1);
			it = itNext;
		}
	}

	void Clear()
	{
		Section lock;
		slot_iterator it = Slots.begin();
		while(it != Slots.end())
		{
			delete *it;
			it++;
		}
		Slots.clear();
	}

//	void Remove(void (*pfun)(const ParamList&))
//	{
//		slot_iterator it = Slots.begin();
//		while(it != Slots.end())
//		{
//			if (((SlotFunctor_v1 *)(*it))->IsEqual(pfun))
//			{
//				delete (*it);
//				Slots.Remove(it);
//				return;
//			}
//			it++;
//		}
//	}

	template <class CALLBACK_CLASS>
	void Remove(CALLBACK_CLASS *cb_class, void (CALLBACK_CLASS::*pfun)(const ParamList&))
	{
		slot_iterator it = Slots.begin();
		while(it != Slots.end())
		{
			if(((SlotMethod_v1<CALLBACK_CLASS> *)(*it))->IsEqual(cb_class, pfun))
			{
				delete *it;
				Slots.remove(*it);
				break;
			}
			it++;
		}
	}

//	Singnal_v1 &operator += (const Singnal_v1 &s)
//	{
//		slot_list::Iterator it = Slots.Begin();
//
//		while(it != Slots.End())
//		{
//			(*it)->AddRef();
//			Connect(*it);
//			it++;
//		}
//
//		return *this;
//	}
};

#endif // !defined(AFX_WORMSOLT_H__130AA9C1_3286_4266_A0DC_5BA01594E10F__INCLUDED_)
