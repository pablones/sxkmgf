// MsgTaskDetail.h: interface for the CMsgTaskDetail class.
//	20061106修罗
//	发送与taskDetail表相关信息时用
//	消息长度,可变长
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGTASKDETAIL_H__30338FBD_B76C_466E_97A1_A5CC9ACC7968__INCLUDED_)
#define AFX_MSGTASKDETAIL_H__30338FBD_B76C_466E_97A1_A5CC9ACC7968__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NETWORK\NetMsg.h"


const int _MAX_TASK_DETAIL_STRUCT_NUM = 20;
enum
{
	_TASTHINT_NULL								=0,
	_TASTHINT_REQUEST_RECEIVED_ALL				=1,
	_TASTHINT_REQUEST_RECEIVED					=2,
	_TASTHINT_APPLY_RECEIVED					=3,
	_TASTHINT_REQUEST_UNSOLVED_ALL				=4,
	_TASKHINT_REQUEST_UNSOLVED					=5,	
	_TASKHINT_APPLY_UNSOLVED					=6,
	_TASKHINT_REQUEST_SOLVED_ALL				=7,
	_TASKHINT_REQUEST_SOLVED					=8,
	_TASKHINT_APPLY_SOLVED						=9,
	_TASKHINT_SET_TASKMASK_ONE					=10,
	_TASKHINT_SET_TASKMASK						=11,
	_TASKHINT_UPDATE							=12,
	_TASKHINT_DELETE							=13,
	_TASKHINT_ADD								=14,
	_TASKHINT_GIVEUP							=15,
	_TASKHINT_SET_DAYTASKMASK_ONE				=16,
	_TASKHINT_SET_DAYTASKMASK					=17,
    _TASKHINT_SET_TASKSHARE                     =18,
	_TASKHINT_SET_WAIT                          =19,
	_TASKHINT_SET_TASKTIME                      =20,
};

//#pragma pack(4)
typedef struct
{
	unsigned int		taskID;
	unsigned int		taskPhase;
	unsigned int		lifeTime;
	unsigned short		data1;
	unsigned short		data2;
	unsigned short		data3;
	unsigned short		data4;
	unsigned short		data5;
	unsigned short		data6;
	unsigned short		isRec;
	unsigned short		isLink;
	unsigned int		randStep;
}TaskDetailInfoForMsg;
//#pragma pack()

class CMsgTaskDetail : public CNetMsg  
{
public:
	CMsgTaskDetail();
	virtual ~CMsgTaskDetail();
	
	BOOL Create(char* pbufMsg, DWORD dwMsgSize);
	BOOL Create(unsigned short usTaskDetailMsgType,TaskDetailInfoForMsg *taskDetailStruct,unsigned short usTaskDetailStructNum);
	void Process (void *pInfo);
protected:
	typedef struct
	{
		MSGHEAD_DEFINE
		unsigned short			usTaskDetailMsgType;
		unsigned short			usTaskDetailStructNum;
		TaskDetailInfoForMsg	taskDetailStruct[1];//变长
	}MSG_Info;
	
	MSG_Info* m_pInfo;

};

#endif // !defined(AFX_MSGTASKDETAIL_H__30338FBD_B76C_466E_97A1_A5CC9ACC7968__INCLUDED_)
