// MsgName.h: interface for the CMsgName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
#define AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "NetStringPacker.h"

enum {	NAMEACT_NONE				= 0, 
		NAMEACT_FIREWORKS			= 1,
		NAMEACT_CREATE_SYN			= 2, 				// 改到TALK中
		NAMEACT_SYNDICATE			= 3, 				// 无法实现修改帮派名，仅下传
		NAMEACT_CHANGE_SYNTITLE		= 4, 
		NAMEACT_DELROLE				= 5,				// 无意义
		NAMEACT_MATE				= 6,
		NAMEACT_QUERY_NPC			= 7,				// to server，to client
		NAMEACT_WANTED				= 8,				// to client
		NAMEACT_MAPEFFECT			= 9,				// to client
		NAMEACT_ROLEEFFECT			= 10,
		NAMEACT_MEMBERLIST			= 11,				// to server/client, dwData is index
		MANEACT_KICKOUT_SYNMEM		= 12,
		NAMEACT_QUERY_WANTED		= 13,
		NAMEACT_QUERY_POLICEWANTED	= 14,
		NAMEACT_POLICEWANTED		= 15,
		NAMEACT_QUERY_MATE			= 16,
		NAMEACT_ADDDICE_PLAYER		= 17,			//BcastClient(INCLUDE_SELF) 增加骰子玩家// dwData为骰子摊ID // To Server 加入 需要原消息返回
		NAMEACT_DELDICE_PLAYER		= 18,			//BcastClient(INCLUDE_SELF) 删除骰子玩家// dwData为骰子摊ID // To Server 离开 需要原消息返回
		NAMEACT_DICE_BONUS			= 19,			//BcastClient 报骰子奖金 玩家// dwData为Money
		NAMEACT_PLAYER_WAVE			= 20,			// 播音效 // 如果usPosX = usPosY = 0; 非地图音效 // 压入的字符串为音效的相对路径 
		NAMEACT_MEMBERLIST_SPECIFYSYN = 21,			//查询指定帮派的成员列表
		NAMEACT_PLAYERTASKLIST		= 23,			// 佣兵任务查询返回
		NAMEACT_CHANGE_EUDEMON_NAME	= 24,			// 幻兽改名。idTarget=幻兽物品ID，第一个字符串为幻兽新名字
		NAMEACT_CHANGE_DESC			= 25,			// 改自我介绍..
		NAMEACT_REBORN_MSG			= 26,			//发送某人对你使用了复活术
		NAMEACT_REBORN_ACCEPT		= 27,			//客户端发送过来同意复活.
		NAMEACT_CHANGE_EXPSCALE		= 28,			//客户端发送过来更改武器获得经验比例
		NAMEACT_CHANGE_EXPSCALEOK	= 29,			//更改成功，发到客户端.
		NAMEACT_LOOK_DESC			= 30,			//查看别人的介绍
		NAMEACT_REBORN_MSG_SR		= 31,	//2007828修罗:struggle relive死地后生.自我复活
		NAMEACT_CHANGE_WPSLEXPSCALE		= 32,			//客户端发送过来更改武器泪灵获得经验比例
		NAMEACT_CHANGE_WPSLEXPSCALEOK	= 33,			//更改成功，发到客户端.

		NAMEACT_CHANGE_QQ               = 34,            //更改qq
		NAMEACT_CHANGE_PROV             = 35,            //省
		NAMEACT_CHANGE_CITY             = 36,            //市
		NAMEACT_CHANGE_OLD              = 37,             //年龄
		NAMEACT_CHANGE_STAR             = 38,            //星座
		NAMEACT_CHANGE_BTSEX            = 39,            //性变态

		NAMEACT_CHANGE_TITLE			= 40,			//[游途道标 2008/10/13]客户端发送过来更改称号
		NAMEACT_CHANGE_TITLEOK			= 41,			//[游途道标 2008/10/13]称号更改成功，发到客户端.

		NAMEACT_CHANGE_HOLDTITLEOK		= 42,			//[游途道标 2008/10/14]拥有称号更改成功，发到客户端.
		NAMEACT_CHANGE_RECORDKEY        = 43,           //保存快捷键和技能栏
		NAMEACT_HANDUP_BUG              = 44,           //提交问题
		NAMEACT_SETLINE					= 45,//[2009.07.06]
		NAMEACT_COPY_SYNNAME            = 46,           //复制帮派名
};

class CMsgName : public CNetMsg   
{
public:
	CMsgName();
	virtual ~CMsgName();

	BOOL	Create(int nType, const char* pszName, __int64 dwData=0);
	BOOL	Create(int nType, const char* pszName, USHORT usPosX, USHORT usPosY);
	BOOL	AppendStr(const char* pszName);

public:	
	BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	void			Process		(void* pInfo);
	
private:
	typedef struct{
		MSGHEAD_DEFINE

		DWORD	dwData;
		DWORD	dwHData;
		union {
			OBJID	idTarget;
			struct{
				USHORT  usPosX, usPosY;
			};
		};
		UCHAR	ucType;
		DWORD	dwSynData;	
		char	szBuf[1];
	}MSG_Info;
	
	MSG_Info*	m_pInfo;

private:
	CNetStringPacker	m_StrPacker;
};

#endif // !defined(AFX_MSGNAME_H__62DB03E2_7185_4BDF_B9B0_5AE64B03DFDB__INCLUDED_)
