// TutorList.h: interface for the CTutorList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUTORLIST_H__3E8E698B_D814_4A09_95CB_0B99D7F59BA1__INCLUDED_)
#define AFX_TUTORLIST_H__3E8E698B_D814_4A09_95CB_0B99D7F59BA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)
#include "../typedef.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include <vector>
using namespace std;

const int _TUTOR_LIST_NUM = 420;//师父列表人数上限
const int _TUTOR_CLEAR_HOURS = 24;//清除N小时没上过线的

enum
{
	TUTOR_INLIST		= 0x0001,	//是否在师父列表中
	TUTOR_ISFIRST		= 0x0002,	//是否第一次拜师
	TUTOR_HASGRADUARED	= 0x0004,	//是否曾经出过师
};

typedef struct
{
	OBJID	idUser;				//用户ID
	UCHAR	ucGender;			//性别
	UCHAR	ucVipLev;			//VIP级别
	USHORT	usLev;				//等级
	DWORD	dwProfession;		//职业
	DWORD	dwBattleNum;		//战斗力
	DWORD	dwImpart;			//传授等级
	//DWORD	dwMorality;			//师德
	DWORD	dwLastLogoutTime;	//上次下线时间
	NAMESTR	szName;				//姓名
	ULONG	uIPAddress;			//ip地址
}TUTORLIST;

class CTutorList  
{
public:
	CTutorList();
	virtual ~CTutorList();
public:
	bool	CreateData( IDatabase* pDb );
	void	Init();
	void	LoadInfo(IRecordset* pRes, TUTORLIST* listInfo, bool bCreate);
	TUTORLIST* FindTutorByID(OBJID idUser);
	TUTORLIST* FindTutorByName(NAMESTR szName);
	void	UpdateListData();
	void	OnTimer( time_t tCurr );
	int		GetInfo( TUTORLIST* pData,int nPageSize, USHORT nPage );//查询指定页的师父列表数据，返回当前页列表数量
	bool	TutorLogin( TUTORLIST* info );//列表中的某个玩家上线了
	bool	TutorLogout( TUTORLIST* info );//列表中的某个玩家下线了
	int		GetTutorLeaveHours( DWORD dwExitTime );//获得玩家有多少个小时没有登陆过了
	void	DeleteTutorFromList(OBJID idUser);
	void	AddTutorToList(TUTORLIST* info);
	int		GetTutorAmount()	{ return m_vTutorList.size(); }
	int		GetTutorApplyAmount(OBJID idTutor, int nImpart);//查询查询玩家的徒弟以及徒弟申请列表是否已满
	bool	HasTutor(OBJID idUser);//查询某个玩家是否有师父
	bool static	TimeFromExit(TUTORLIST tutor1, TUTORLIST tutor2);
	void	SetTutorListNameInfo(OBJID idUser,LPCTSTR szName);//改名
private:
	vector<TUTORLIST>	m_vTutorList;
	IDatabase*	m_pDb;
	SYSTEMTIME	m_LastUpdateTime;				//上次更新时间
	HANDLE		m_TutorUpdateCompleteEvent;		//事件对象,更新完成
	int			m_TutorListUpMin;				//在第几分钟刷新
};

extern CTutorList	g_TutorListSystem;			//师父列表

#endif // !defined(AFX_TUTORLIST_H__3E8E698B_D814_4A09_95CB_0B99D7F59BA1__INCLUDED_)
