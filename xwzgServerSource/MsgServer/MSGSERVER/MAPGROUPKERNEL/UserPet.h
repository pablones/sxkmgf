// UserPet.h: interface for the CUserPet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERPET_H__FAC81625_499E_439E_B22E_74E10A63A50A__INCLUDED_)
#define AFX_USERPET_H__FAC81625_499E_439E_B22E_74E10A63A50A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../share/define.h"
#include <vector>
#include "TaskDetailData.h"
#include "TaskSetRandData.h"
class IDatabase;
class IRole;

//�������״̬
enum
{
	PETSTATE_REST = 0,//��Ϣ
	PETSTATE_INWAR = 1,//��ս
	PETSTATE_BORN  = 3,//��ֳ
	PETSTATE_ADDIN = 4,//����
	PETSTATE_DELIN = 5,//����
	PETSTATE_PLAY  = 6,//��ˣ��
	PETSTATE_INSTORAGE = 10,//�ֿ���
};

class CUserPet  
{
public:
	CUserPet();
	virtual ~CUserPet();

	static CUserPet* CreateNew(){return new CUserPet;}
	//bool	CreateNewTaskDetailData(ST_TASKDETAILDATA * pInfo);
public:
	bool	Create(PROCESS_ID idProcess,CUser *pUser,bool bLogin);
	bool	Release()	{delete this; return true;}
	void    SaveData();
public:
 	CUserPetData* QueryData(OBJID idData,bool bDel = false);
	bool AddData(CUserPetData* pData);
	int  sendInfo(CUser* pUser = NULL);
	bool  sendStorageInfo();//���Ͳֿ��еĳ���
	bool  fillType(EudemonData& data,int nType);
	bool  fillAtt(EudemonData& data,int nType);
	bool  DeleteData(OBJID idPet);
	bool  CreateNewPetData(OBJID idEudemonType,EudemonData *pData,bool bSave, bool born = false,int nBornGrow = 0,bool bCompensation = false);
	int   GetEudmonAmount() { return m_setData.size();}
	int   GetShowAmountByType(OBJID idType);
	int   GetShowAmount();
	int	  GetStorageAmount();//�ֿ��г��������
//	CUserPetData* PopEudemon(OBJID idEudemon);
	bool  AddEudemon(CUserPetData* pEudemon, bool bUpdate);
	CUserPetData* QueryDataByIndex(int index) { if(index >=0 && index < m_setData.size() )return m_setData[index]; else return NULL;}

	CUserPetData*  CreateNewPetByEgg(OBJID idEudemonType,EudemonData *pData);
// 	CTaskDetailData* QueryData(OBJID idUser,OBJID idTask);
// 	CTaskDetailData* QueryDataByChain(OBJID idUser,OBJID idChain);
// 	int              QueryDataNumByChain(OBJID idUser,OBJID idChain);
// 	bool	FindNextData(int &nIter);
// 	bool	FindData(OBJID  idTask,int nPhase,int nCompleteNum, int &nIter);
// 	bool	FindNextData(OBJID idUser,int& nIter);
 	
// 	ULONG    DeleteDataByChain(OBJID idUser,OBJID idChain);
//     CTaskSetRandData*   GetRandTask(int nChain,OBJID idNpc);//�Ǹ�������,�������ͣ������Ĺֻ���Ʒ�����ֵ(���ڻ�������)
	void  SendAllObjInfo(PROCESS_ID idProcess);
	bool  AddPeTBySend(EudemonData info);

	bool EudemonTransform(OBJID idEudemon);//����û�
	bool EudemonIntelUp(OBJID idEudemon);//��������
	bool EudemonFusionUp(OBJID idEudemon);//����Ĭ��
	bool EudemonChangeSex(OBJID idEudemon);//�������
	//bool EudemonSkill(OBJID eduemonItemId,OBJID idItem,int skill,bool bLean);
	bool LearnSkill(OBJID idEudemon, OBJID idItem, int nIgnoreSkill);
	bool UplevSkill(OBJID idEudemon, int nUplevSkill);
	bool EudemonPlay(OBJID idEudemon1,OBJID idEudemon2,CUser* pOther);
protected:
	typedef std::vector<CUserPetData*>	USERPETDATA_SET;
	USERPETDATA_SET	m_setData;//20061109����.��Ϊpublic.�������.
protected:
	PROCESS_ID		m_idProcess;
	CUser *m_pOwner;	
	MYHEAP_DECLARATION(s_heap)
};


class CUserPeifang  
{
public:
	CUserPeifang();
	virtual ~CUserPeifang();
	
	static CUserPeifang* CreateNew(){return new CUserPeifang;}
public:
	bool	Create(PROCESS_ID idProcess,CUser *pUser,bool bLogin = true);
	bool	Release()	{delete this; return true;}
public:
	CUserPeifangData* QueryData(OBJID idPeifang,bool bDel = false);
	bool AddData(CUserPeifangData* pData);
	int  sendInfo(CUser* pUser = NULL);
	bool  DeleteData(OBJID idPeifang);
	bool  CreateNewPeifangData(OBJID idUser,OBJID idPeifangType);
	void  SendAllObjInfo(PROCESS_ID idProcess);
	bool AddPeifangBySend(ST_PEIFANGDATA info);

protected:
	typedef std::vector<CUserPeifangData*>	USERPEIFANGDATA_SET;
	USERPEIFANGDATA_SET	m_setData;//20061109����.��Ϊpublic.�������.
protected:
	PROCESS_ID		m_idProcess;
	CUser*			m_pOwner;	
	MYHEAP_DECLARATION(s_heap)
};
#endif // !defined(AFX_USERPET_H__FAC81625_499E_439E_B22E_74E10A63A50A__INCLUDED_)
