// PVPManager.h: interface for the CPVPManager class.
//
//////////////////////////////////////////////////////////////////////

#include "../share/define.h"
#include <list>
using namespace std;

#if !defined(AFX_PVPMANAGER_H__168232B4_C591_4AB7_959C_540941837F11__INCLUDED_)
#define AFX_PVPMANAGER_H__168232B4_C591_4AB7_959C_540941837F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct PVPData{
	DWORD nMode; //ģʽ
	DWORD nBegainTime; //��ʼʱ��
	DWORD nOutTime1;
	DWORD nOutTime2;

	OBJID userID1; //���1
	OBJID userID2; //���2
	OBJID flagID; //���ĵ�ID
};

typedef list<PVPData*> PVPDataSet;

class CPVPManager  
{
public:

	CPVPManager(PROCESS_ID id);
	virtual ~CPVPManager();
	void doPVP(); //��ͼ����� ����ʱ�����
	void EndPVP(OBJID id1,OBJID winner = 0, bool bKill = false);//����winner��ʤ���� Ϊ0ʱid1�Ķ���Ϊʤ����
	bool findPVP(OBJID id1);//�ҵ�����Ƿ��о���
	void AddPVP(OBJID id1,OBJID id2,int mode,OBJID flagID);//��Ӿ���
	void SetLong(OBJID id1); //���ó�����
	void ClrLong(OBJID id1); //���������
protected:
	PVPDataSet m_pAllPVP;
	DWORD thisChkTime;
	PROCESS_ID	m_idProcess;
};

#endif // !defined(AFX_PVPMANAGER_H__168232B4_C591_4AB7_959C_540941837F11__INCLUDED_)
