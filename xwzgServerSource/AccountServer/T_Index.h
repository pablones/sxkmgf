// 索引类，用于建立数据表之间的快速查询。加入的KEY不能重复
// 仙剑修，2001.12.18
//////////////////////////////////////////////////////////////////////

#ifndef	INDEX_H
#define INDEX_H

#include "Define.h"
//#include "basefunc.h"

#pragma warning(disable:4786)
#include <map>
using namespace std;

///////////////////////////////////////////////////////////////////////
//#define	MORECHECK		//??? 加强检测, 但降低效率。重复添加会出错
///////////////////////////////////////////////////////////////////////

//#define	LogSave	printf
#define	INDEX_PARA	class K, class D, D ERR
#define	INDEX_PARA2	K, D, ERR

template	< INDEX_PARA >
class CIndex
{
public:
	CIndex() {}
	virtual ~CIndex() { if(m_map.size()) LogSave("WARNING: CIndex destruction in not empty[%d].", m_map.size()); }
public:
	bool		Add(K key, D data);
	bool		Del(K key);
	bool		IsIn(K key);
	D			operator[](K key);
	void		ClearAll() { m_map.clear(); }
	typedef	map<K, D> MAPINDEX;
	typedef	typename MAPINDEX::iterator	iterator;
	K			Key(iterator iter)	{ return iter->first; }
	D			Data(iterator iter)	{ return iter->second; }
	iterator	Begin() { return m_map.begin(); }
	iterator	End()	{ return m_map.end(); }
	DWORD		Size()	{ return m_map.size();}
protected:
	MAPINDEX	m_map;
};

template	< INDEX_PARA >
bool	CIndex<INDEX_PARA2>::Add(K key, D data)
{
#ifdef	MORECHECK
	if(m_map.find(key) ==m_map.end())
#endif
	{
		m_map[key] = data;
		return true;
	}
	LogSave("ERROR: CIndex::Add() attempt add again. refused.");
	return false;
}

template	< INDEX_PARA >
bool	CIndex<INDEX_PARA2>::Del(K key)
{
#ifdef	MORECHECK
	if(m_map.find(key) !=m_map.end())
#endif
	{
		m_map.erase(key);
		return true;
	}
	LogSave("ERROR: CIndex::Del() attempt del again. refused.");
	return false;
}

template	< INDEX_PARA >
bool	CIndex<INDEX_PARA2>::IsIn(K key)
{
	if(m_map.find(key) !=m_map.end())
	{
		return true;
	}
	return false;
}

template	< INDEX_PARA >
D		CIndex<INDEX_PARA2>::operator[](K key)
{
	MAPINDEX::iterator iter;
	if((iter=m_map.find(key)) !=m_map.end())
	{
		return iter->second;
	}
	//? 因为有时会用不存在的ID为参数	LogSave("ERROR: CIndex operator[]() not found key.");
	return ERR;
}


#endif // INDEX_H
