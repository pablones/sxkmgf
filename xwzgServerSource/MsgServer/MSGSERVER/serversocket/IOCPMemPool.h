// IOCPMemPool.h: interface for the CIOCPMemPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCPMEMPOOL_H__AE3C8742_72FC_4B45_971B_126060AAB67C__INCLUDED_)
#define AFX_IOCPMEMPOOL_H__AE3C8742_72FC_4B45_971B_126060AAB67C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*#include <vld.h>*/
#include <vector>
#include <list>
//#include <deque>
using namespace std;

#include <windows.h>

#include "../share/define.h"
#include "protocol.h"
//#include "EncryptServer.h"
//#include "SSocket.h"
#include "I_Shell.h"
//#include "ServerSocket.h"
#include "I_MessagePort.h"
#include "basefunc.h"
/*
该模板类只支持单线程分配内存,多线程释放内存
暂时不支持多线分配内存
*/

template<class T>
class CIOCPMemPool  
{
public:

	CIOCPMemPool(int MaxNum=1024)
	{
		m_MaxNum = MaxNum;
		m_nCurNum = 0;

		InitializeCriticalSection(&m_ListaLock);

		//test
		newcount = 0;
	}

	~CIOCPMemPool()
	{
		
		list<T*>::iterator iter;
		for(iter=m_Lista.begin();iter!=m_Lista.end();++iter)
		{
			delete (*iter);
		}
		m_Lista.clear();
		DeleteCriticalSection(&m_ListaLock);
		
	}

public:
	T* GetObject();
	bool ReleaseObject(T* pBuff);

protected:


	int					m_MaxNum;
	int					m_nCurNum;
	
	list<T*>			m_Lista;		//回收列表a
	
	CRITICAL_SECTION	m_ListaLock;

	
	
public:
	//test
	int newcount;


	
};



template<class T>
T* CIOCPMemPool<T>:: GetObject()
{
	T* p = NULL;

	EnterCriticalSection(&m_ListaLock);
	if(!m_Lista.empty())
	{
		p = m_Lista.front();
		m_Lista.pop_front();
		m_nCurNum--;
	}
	else
	{
		p = new T;

		//test
		newcount++;
	}
	LeaveCriticalSection(&m_ListaLock);
	return p;
}

template<class T>
bool CIOCPMemPool<T>:: ReleaseObject(T* pBuff)
{
	EnterCriticalSection(&m_ListaLock);
	if(m_MaxNum==-1||m_nCurNum<m_MaxNum)
	{
		m_Lista.push_back(pBuff);
		m_nCurNum++;
	}
	else
	{
		delete pBuff;
	}
	LeaveCriticalSection(&m_ListaLock);
	return true;
}

#endif // !defined(AFX_IOCPMEMPOOL_H__AE3C8742_72FC_4B45_971B_126060AAB67C__INCLUDED_)
