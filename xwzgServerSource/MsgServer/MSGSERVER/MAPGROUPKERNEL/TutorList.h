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

const int _TUTOR_LIST_NUM = 420;//ʦ���б���������
const int _TUTOR_CLEAR_HOURS = 24;//���NСʱû�Ϲ��ߵ�

enum
{
	TUTOR_INLIST		= 0x0001,	//�Ƿ���ʦ���б���
	TUTOR_ISFIRST		= 0x0002,	//�Ƿ��һ�ΰ�ʦ
	TUTOR_HASGRADUARED	= 0x0004,	//�Ƿ���������ʦ
};

typedef struct
{
	OBJID	idUser;				//�û�ID
	UCHAR	ucGender;			//�Ա�
	UCHAR	ucVipLev;			//VIP����
	USHORT	usLev;				//�ȼ�
	DWORD	dwProfession;		//ְҵ
	DWORD	dwBattleNum;		//ս����
	DWORD	dwImpart;			//���ڵȼ�
	//DWORD	dwMorality;			//ʦ��
	DWORD	dwLastLogoutTime;	//�ϴ�����ʱ��
	NAMESTR	szName;				//����
	ULONG	uIPAddress;			//ip��ַ
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
	int		GetInfo( TUTORLIST* pData,int nPageSize, USHORT nPage );//��ѯָ��ҳ��ʦ���б����ݣ����ص�ǰҳ�б�����
	bool	TutorLogin( TUTORLIST* info );//�б��е�ĳ�����������
	bool	TutorLogout( TUTORLIST* info );//�б��е�ĳ�����������
	int		GetTutorLeaveHours( DWORD dwExitTime );//�������ж��ٸ�Сʱû�е�½����
	void	DeleteTutorFromList(OBJID idUser);
	void	AddTutorToList(TUTORLIST* info);
	int		GetTutorAmount()	{ return m_vTutorList.size(); }
	int		GetTutorApplyAmount(OBJID idTutor, int nImpart);//��ѯ��ѯ��ҵ�ͽ���Լ�ͽ�������б��Ƿ�����
	bool	HasTutor(OBJID idUser);//��ѯĳ������Ƿ���ʦ��
	bool static	TimeFromExit(TUTORLIST tutor1, TUTORLIST tutor2);
	void	SetTutorListNameInfo(OBJID idUser,LPCTSTR szName);//����
private:
	vector<TUTORLIST>	m_vTutorList;
	IDatabase*	m_pDb;
	SYSTEMTIME	m_LastUpdateTime;				//�ϴθ���ʱ��
	HANDLE		m_TutorUpdateCompleteEvent;		//�¼�����,�������
	int			m_TutorListUpMin;				//�ڵڼ�����ˢ��
};

extern CTutorList	g_TutorListSystem;			//ʦ���б�

#endif // !defined(AFX_TUTORLIST_H__3E8E698B_D814_4A09_95CB_0B99D7F59BA1__INCLUDED_)
