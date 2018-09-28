////////////////////////////////////////////
// T_CompactObjSet.h
// paled, 2003.2.17
// ˵������SETֻ�ܷš��Ƕѷ��䡱�����ҹ��캯���������������ᱻ���á�
//		 ��SET�ڲ��Ķ���ֻ�ܵ��ó�Ա����(��Create(...))����ʽ��ʼ����������
///////////////////////////////////////////

#ifndef T_COMPACTOBJSET_H
#define T_COMPACTOBJSET_H

#include "common.h"

#include <map>
#include <list>
using namespace std;

#define MAX_MEMALLOC_NUM		50

template<TYPENAME T>
class CompactObjSet
{
public:
	CompactObjSet()	{ m_buf = NULL; }					
	~CompactObjSet() { if(m_buf) delete []m_buf;}
// 	{
// 		if(m_buf)
// 		{
// 			FreeMem(m_nSize,m_buf);
// 			m_buf = NULL;
// 		}
// 	}
public:
	bool	Create(int nSize)						{ CHECKF(!m_buf); m_nSize = nSize; m_buf = new char[sizeof(T)*m_nSize]; clear(); return m_buf != NULL; }
// 			{
// 				CHECKF(!m_buf); 
// 				m_nSize = nSize; 
// 				m_buf = AllocMem(sizeof(T),nSize); 
// 				return m_buf != NULL;
// 			}
public:
	int		size()									{ return m_nSize; }
	T*		operator [](int idx)					{ return (T*)(m_buf + sizeof(T)*idx); }
	void	clear()									{ memset(m_buf, 0, sizeof(T)*m_nSize); }
	//static void	Release();
protected:
	char*	m_buf;
	int		m_nSize;

private:
// 	static map<UINT,list<char*> >					m_MemPool;
// 
// 	static char* AllocMem(UINT nObjSize,UINT nObjCnt);
// 	static void	 FreeMem(UINT nObjCnt,char* pFree);
};

/*
template<TYPENAME T>
map<UINT,list<char*> > CompactObjSet<T>::m_MemPool;


template<TYPENAME T>
char* CompactObjSet<T>::AllocMem(UINT nObjSize,UINT nObjCnt)
{
	char* pMem = NULL;

	map<UINT,list<char*> >::iterator it = m_MemPool.find(nObjCnt);
	if(it!=m_MemPool.end())
	{
		//��list��ȡ�����е��ڴ��
		if(!(it->second.empty()))
		{
			pMem = it->second.front();
			it->second.pop_front();
			if(pMem)
				memset(pMem,0,nObjSize*nObjCnt);
			return pMem;
		}
		else
		{
			//���·����ڴ�
			pMem = new char[nObjSize*nObjCnt];
			if(!pMem)
				return pMem;
			memset(pMem,0,nObjSize*nObjCnt);
			return pMem;
		}
	}
	else
	{
		//���·����ڴ�
		pMem = new char[nObjSize*nObjCnt];
		if(!pMem)
			return pMem;
		memset(pMem,0,nObjSize*nObjCnt);
			return pMem;
	}
}



template<TYPENAME T>
void CompactObjSet<T>::FreeMem(UINT nObjCnt,char* pFree)
{
	map<UINT,list<char*> >::iterator it = m_MemPool.find(nObjCnt);
	if(it!=m_MemPool.end())
	{
		//�жϸ����Ƿ񳬳��˷�ֵ
		if(it->second.size()>=MAX_MEMALLOC_NUM)
			delete[] pFree;
		else
			it->second.push_back(pFree);
	}
	else
	{
		list<char*> tmplist;
		tmplist.push_back(pFree);
		m_MemPool.insert(make_pair(nObjCnt,tmplist));
	}
}

template<TYPENAME T>
void CompactObjSet<T>::Release()
{
	map<UINT,list<char*> >::iterator it;

	for (it = m_MemPool.begin();it!=m_MemPool.end();++it)
	{
		list<char*>::iterator itlist;
		for (itlist = it->second.begin();itlist!=it->second.end();++itlist)
		{
			delete[] (*itlist);
		}
		it->second.clear();
	}

	m_MemPool.clear();
}
*/
#endif
