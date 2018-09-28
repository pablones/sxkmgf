#ifndef _STATISTICAL_H_
#define _STATISTICAL_H_
#include <map>
#define _MAX_RECV_COUNT_ 450
#define _MAX_WALK_COUNT_ 200
#define _MAX_INTE_COUNT_ 260
class CStatistical
{
private:
	typedef struct countpackets
	{
		int npcrecvcount;
		int maxnpcrecvcount;
		int recvcount;
		int maxrecvcount;
		int walkcount;
		int maxwalkcount;
		int interact;
		int maxinteract;
	}COUNTPACKETS,*PCOUNTPACKETS;
	typedef std::map<unsigned long,COUNTPACKETS> COUPAC;
public:
	CStatistical()
	{
		::InitializeCriticalSection(&cs_section);
		maxnpcrecvcount = 0;
		maxrecvcount = 0;
		maxwalkcount = 0;
		maxinteract = 0;
		npcrecvkey = -1;
		recvkey = -1;
		walkkey = 0;
		intekey = 0;
	}
	virtual ~CStatistical(){ ::DeleteCriticalSection(&cs_section); }
	bool Addnpcrecvcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		COUNTPACKETS data = {0};
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
			data = ((COUNTPACKETS)(lpstatcount->second));
		data.npcrecvcount++;
		if( data.npcrecvcount > data.maxnpcrecvcount )
		{
			data.maxnpcrecvcount++;
			if( data.maxnpcrecvcount > maxnpcrecvcount )
			{
				maxnpcrecvcount = data.maxnpcrecvcount;
				npcrecvkey = key;
			}
		}
		m_statcount[key] = data;
		::LeaveCriticalSection(&cs_section);
		return false;
	}
	bool Addrecvcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		COUNTPACKETS data = {0};
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
			data = ((COUNTPACKETS)(lpstatcount->second));
		data.recvcount++;
		if( data.recvcount > data.maxrecvcount )
		{
			data.maxrecvcount++;
			if( data.maxrecvcount > maxrecvcount )
			{
				maxrecvcount = data.maxrecvcount;
				recvkey = key;
			}
		}
		m_statcount[key] = data;
		::LeaveCriticalSection(&cs_section);
		if( _MAX_RECV_COUNT_ < maxrecvcount )
			return true;
		return false;
	}
	bool Addwalkcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		COUNTPACKETS data = {0};
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
			data = ((COUNTPACKETS)(lpstatcount->second));
		data.walkcount++;
		if( data.walkcount > data.maxwalkcount )
		{
			data.maxwalkcount++;
			if( data.maxwalkcount > maxwalkcount )
			{
				maxwalkcount = data.maxwalkcount;
				walkkey = key;
			}
		}
		m_statcount[key] = data;
		::LeaveCriticalSection(&cs_section);
		if( _MAX_WALK_COUNT_ < maxwalkcount )
			return true;
		return false;
	}
	bool Addinteract(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		COUNTPACKETS data = {0};
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
			data = ((COUNTPACKETS)(lpstatcount->second));
		data.interact++;
		if( data.interact > data.maxinteract )
		{
			data.maxinteract++;
			if( data.maxinteract > maxinteract )
			{
				maxinteract = data.maxinteract;
				intekey = key;
			}
		}
		m_statcount[key] = data;
		::LeaveCriticalSection(&cs_section);
		if( _MAX_INTE_COUNT_ < maxinteract )
			return true;
		return false;
	}
	
	int Getrecvcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
		{
			int data = ((COUNTPACKETS)(lpstatcount->second)).recvcount;
			::LeaveCriticalSection(&cs_section);
			return data;
		}
		::LeaveCriticalSection(&cs_section);
		return 0;
	}
	int Getwalkcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
		{
			int data = ((COUNTPACKETS)(lpstatcount->second)).walkcount;
			::LeaveCriticalSection(&cs_section);
			return data;
		}
		::LeaveCriticalSection(&cs_section);
		return 0;
	}
	int Getinteract(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
		{
			int data = ((COUNTPACKETS)(lpstatcount->second)).interact;
			::LeaveCriticalSection(&cs_section);
			return data;
		}
		::LeaveCriticalSection(&cs_section);
		return 0;
	}
	
	int GetMaxrecvcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
		{
			int data = ((COUNTPACKETS)(lpstatcount->second)).maxrecvcount;
			::LeaveCriticalSection(&cs_section);
			return data;
		}
		::LeaveCriticalSection(&cs_section);
		return 0;
	}

	int GetMaxnpcrecvcount(void)
	{
		return maxnpcrecvcount;
	}

	int GetMaxrecvcount(void)
	{
		return maxrecvcount;
	}

	int GetMaxwalkcount(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
		{
			int data = ((COUNTPACKETS)(lpstatcount->second)).maxwalkcount;
			::LeaveCriticalSection(&cs_section);
			return data;
		}
		::LeaveCriticalSection(&cs_section);
		return 0;
	}

	int GetMaxwalkcount(void)
	{
		return maxwalkcount;
	}

	int GetMaxinteract(unsigned long key)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.find(key);
		if( lpstatcount != m_statcount.end() )
		{
			int data = ((COUNTPACKETS)(lpstatcount->second)).maxinteract;
			::LeaveCriticalSection(&cs_section);
			return data;
		}
		::LeaveCriticalSection(&cs_section);
		return 0;
	}

	int GetMaxinteract(void)
	{
		return maxinteract;
	}

	void ClearRecvcount(void)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			data.recvcount = 0;
			lpstatcount->second = data;
			lpstatcount++;
		}
		::LeaveCriticalSection(&cs_section);
	}
	void ClearWalkcount(void)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			data.walkcount = 0;
			lpstatcount->second = data;
			lpstatcount++;
		}
		::LeaveCriticalSection(&cs_section);
	}
	void ClearInteract(void)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			data.interact = 0;
			lpstatcount->second = data;
			lpstatcount++;
		}
		::LeaveCriticalSection(&cs_section);
	}
	void Clear(void)
	{
		::EnterCriticalSection(&cs_section);
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			data.npcrecvcount = 0;
			data.recvcount = 0;
			data.walkcount = 0;
			data.interact = 0;
			lpstatcount->second = data;
			lpstatcount++;
		}
		::LeaveCriticalSection(&cs_section);
	}

	unsigned long GetNpcRecvKey(void)
	{
		return npcrecvkey;
	}
	unsigned long GetRecvKey(void)
	{
		return recvkey;
	}
	unsigned long GetWalkKey(void)
	{
		return walkkey;
	}
	unsigned long GetInteKey(void)
	{
		return intekey;
	}

	unsigned long EraseMaxRecv(void)
	{
		::EnterCriticalSection(&cs_section);
		unsigned long l_key = recvkey;
		m_statcount.erase(recvkey);
		maxrecvcount = 0;
		recvkey = -1;
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			if( data.maxrecvcount > maxrecvcount )
			{
				maxrecvcount = data.maxrecvcount;
				recvkey = lpstatcount->first;
			}
			lpstatcount++;
		}
		::LeaveCriticalSection(&cs_section);
		return l_key;
	}
	unsigned long EraseMaxRecv(unsigned long recvkey)
	{
		::EnterCriticalSection(&cs_section);
		unsigned long l_key = recvkey;
		m_statcount.erase(recvkey);
		maxrecvcount = 0;
		recvkey = -1;
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			if( data.maxrecvcount > maxrecvcount )
			{
				maxrecvcount = data.maxrecvcount;
				recvkey = lpstatcount->first;
			}
			lpstatcount++;
		}
		::LeaveCriticalSection(&cs_section);
		return l_key;
	}
	unsigned long EraseMaxWalk(void)
	{
		::EnterCriticalSection(&cs_section);
		unsigned long l_key = walkkey;
		m_statcount.erase(walkkey);
		if( l_key == intekey )
			maxinteract = 0;
		maxwalkcount = 0;
		walkkey = 0;
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			if( data.maxwalkcount > maxwalkcount )
			{
				maxwalkcount = data.maxwalkcount;
				walkkey = lpstatcount->first;
			}
			lpstatcount++;
		}
		if( l_key == intekey )
		{
			intekey = 0;
			lpstatcount = m_statcount.begin();
			while( lpstatcount != m_statcount.end() )
			{
				COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
				if( data.maxinteract > maxinteract )
				{
					maxinteract = data.maxinteract;
					intekey = lpstatcount->first;
				}
				lpstatcount++;
			}
		}
		::LeaveCriticalSection(&cs_section);
		return l_key;
	}
	unsigned long EraseMaxInte(void)
	{
		::EnterCriticalSection(&cs_section);
		unsigned long l_key = intekey;
		m_statcount.erase(intekey);
		if( l_key == walkkey )
			maxwalkcount = 0;
		maxinteract = 0;
		intekey = 0;
		std::map<unsigned long,COUNTPACKETS>::iterator lpstatcount = m_statcount.begin();
		while( lpstatcount != m_statcount.end() )
		{
			COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
			if( data.maxinteract > maxinteract )
			{
				maxinteract = data.maxinteract;
				intekey = lpstatcount->first;
			}
			lpstatcount++;
		}
		if( l_key == walkkey )
		{
			walkkey = 0;
			lpstatcount = m_statcount.begin();
			while( lpstatcount != m_statcount.end() )
			{
				COUNTPACKETS data = ((COUNTPACKETS)(lpstatcount->second));
				if( data.maxwalkcount > maxwalkcount )
				{
					maxwalkcount = data.maxwalkcount;
					walkkey = lpstatcount->first;
				}
				lpstatcount++;
			}
		}
		::LeaveCriticalSection(&cs_section);
		return l_key;
	}
private:
	std::map<unsigned long,COUNTPACKETS> m_statcount;
	CRITICAL_SECTION cs_section;
	int maxnpcrecvcount;//统计NPC每分钟最大发包数
	int maxrecvcount;//统计每个玩家每分钟最大发包数
	int maxwalkcount;//统计每个玩家每分钟走路最大发包数
	int maxinteract;//统计每个玩家每分钟攻击最大发包数
	unsigned long npcrecvkey;//NPC每分钟最大发包数key
	unsigned long recvkey;//玩家每分钟最大发包数key
	unsigned long walkkey;//玩家每分钟走路最大发包数key
	unsigned long intekey;//玩家每分钟攻击最大发包数key
};

extern CStatistical g_StatisticalData;

#endif