// PlayerLeaveWord.cpp: implementation of the CLeaveWord class.
// 仙剑修, 2002.10.11
//////////////////////////////////////////////////////////////////////

#include "LeaveWord.h"
#include "AllMsg.h"
#include "../../GameBaseCodeMT/I_mydb.h"
#include "MapGroup.h"
#include "TimeOut.h"

MYHEAP_IMPLEMENTATION(CLeaveWord,s_heap)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLeaveWord::CLeaveWord()
{
	m_idLastMsg		= 0;
}

//////////////////////////////////////////////////////////////////////
CLeaveWord::~CLeaveWord()
{
}

//////////////////////////////////////////////////////////////////////
// application
//////////////////////////////////////////////////////////////////////
// 本地函数，仅本文件使用
void GetTimeStamp(char* pBuf15, time_t tTime = 0)
{
	ASSERT(pBuf15);

	if(!tTime)
		tTime	= time(NULL);
	tm*	pTm		= localtime(&tTime);
	if(pTm)
	{
		sprintf(pBuf15, "%04d%02d%02d%02d%02d%02d", 
				pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
	}
}
//////////////////////////////////////////////////////////////////
bool CLeaveWord::CheckWords(char* word,int nMaxSize) 
{
	int nEnd  = strlen(word) + 1; //最后字符串长度
	if(nEnd == 1)
	{
		return false;
	}
	if(nEnd > nMaxSize)
	{
		return false;
	}
	
	
	char* pszRet = new char[nMaxSize];
	int nCurs = 0;
	int nCurt = 0;
	int nLen = 0;
	for(int i = 0 ; i < strlen(word)  ; i++)
	{
		if(word[i] == '\0')
		{
			break; //结束
		}
		else if(word[i] == '\\')
		{
			nLen = i - nCurs + 1;
			strncpy(pszRet + nCurt,word + nCurs,nLen);
			nCurs += nLen;
			nCurt += nLen;
			pszRet[nCurt++] = '\\';
			
			nEnd++;
			if(nEnd > nMaxSize)
			{
				delete[] pszRet;
				return false;
			}
		}
	}
	//末尾补上
	nLen = strlen(word) - nCurs;
	strncpy(pszRet + nCurt,word + nCurs,nLen);
	nCurt += nLen;
	pszRet[nCurt] = '\0';
	
	strcpy(word,pszRet);
	delete[] pszRet;
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CLeaveWord::Create(PROCESS_ID idProcess)
{
	m_idProcess = idProcess;

	char	szSQL[1024];

	// 清除过期留言
	time_t	tDelTime = time(NULL);
	tDelTime -= DELETE_LEAVEWORD_DAYS * 86400;
	char	szDelTime[_MAX_NAMESIZE];
	GetTimeStamp(szDelTime, tDelTime);
	sprintf(szSQL, "DELETE FROM %s WHERE time < %s", _TBL_LEAVEWORD, szDelTime);	// need not '%s'
	g_db.ExecuteSQL(szSQL);

	m_idLastMsg	= 0;
	sprintf(szSQL, "SELECT MAX(id) as max_id FROM %s", _TBL_LEAVEWORD);
	IRecordset *pRes = Database()->CreateNewRecordset(szSQL);		//? may be failed.
	if(pRes)
	{
		m_idLastMsg = pRes->GetInt(0);
		pRes->Release();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CLeaveWord::InsertWords(LPCTSTR szSender, LPCTSTR szRecvUser, LPCTSTR szWords, LPCTSTR szColor)//[2010-06-10 goto]增加文本颜色字段
{
	if(!szSender || strlen(szSender) == 0)
		return false;
	if(!szRecvUser || strlen(szRecvUser) == 0)
		return false;
	if(!szWords || strlen(szWords) == 0)
		return false;
	if(!szColor || strlen(szColor) == 0)
		szColor = "0";//return false;
	char szReWords[_MAX_WORDSSIZE];
	SafeCopy(szReWords,szWords);
	CheckWords(szReWords,_MAX_WORDSSIZE);
	CHECKF(!UserManager()->GetUser(szRecvUser));

	char	szTime[_MAX_NAMESIZE] = "00000000000000";		// 要填入EMOTION位置
	GetTimeStamp(szTime);

	char	szSQL[1024];
	sprintf(szSQL, "INSERT %s SET user_name='%s', send_name='%s', time='%s', words='%s', color='%s'",
			_TBL_LEAVEWORD,
			szRecvUser, szSender, szTime, szReWords, szColor);
	return g_db.ExecuteSQL(szSQL);
}

/////////////////////////////////////////////////////////////////////
int CLeaveWord::CountWords(LPCTSTR szSender, LPCTSTR szReceiver/*=NULL*/)
{
	if(!szSender || strlen(szSender) == 0)
		return false;

	char	szSQL[1024];
	if (szReceiver)//留言者给接收者一共发了多少留言
		sprintf(szSQL, "SELECT COUNT(*) as count_star FROM %s WHERE send_name='%s' AND user_name='%s'", _TBL_LEAVEWORD, szSender, szReceiver);
	else//留言者一共发了多少留言，包括所有存在的接收者
		sprintf(szSQL, "SELECT COUNT(*) as count_star FROM %s WHERE send_name='%s'", _TBL_LEAVEWORD, szSender);
	IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
	if(!pRes)
		return 0;

	int nCount = pRes->GetInt(0);
	pRes->Release();
	return nCount;
}

//////////////////////////////////////////////////////////////////////
bool CLeaveWord::ShowWords(CUser* pUser, int nMaxNum /*= PLAYER_WORDS_PER_TIME*/)
{
	if (!pUser->CanBeSee())
		return false;
	char	szSQL[1024];
// 	sprintf(szSQL, "SELECT a.*,b.lookface,b.Feel,b.id as userid FROM %s a,%s b WHERE a.user_name='%s' AND a.send_name=b.name ORDER BY id LIMIT %d",
// 			_TBL_LEAVEWORD,_TBL_USER, pUser->GetName(), nMaxNum);
	sprintf(szSQL, "SELECT * FROM %s WHERE user_name='%s' ORDER BY id", _TBL_LEAVEWORD, pUser->GetName());
	IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
	if(!pRes)
	{
		pUser->SetLeaveWord(false);
		return false;
	}

	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		char	szSender[_MAX_NAMESIZE];
		char	szRecvUser[_MAX_NAMESIZE];
		char	szTime[_MAX_NAMESIZE];
		char	szWords[_MAX_WORDSSIZE];
		char	szColor[_MAX_COLORSIZE];
		char	szFeel[_MAX_FEELSIZE];
		DWORD	dwID;
		UINT usLookFace = 0;
		SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
		SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);
		SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
		SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
		SafeCopy(szColor, pRes->GetStr(LEAVEWORDDATA_COLOR), _MAX_COLORSIZE);

		
		CUser* pUserSender = UserManager()->GetUser(szSender);
		if(pUserSender)
		{
			strcpy(szFeel,pUserSender->GetName());
			dwID	=	pUserSender->GetID();
			usLookFace = pUserSender->GetLookFace();
		}
		else
		{
			stTempUser* pUserSenderTemp = UserManager()->QueryTempUser(szSender);
		}

		DWORD dwUseGMToolPsw=0;
		CFriend* pFriend = pUser->GetFriendByName(szSender);
		if(!pFriend)
		{
			dwUseGMToolPsw=1;//临时好友发来的留言
		//	pUser->AddBeFriend(0,pUser->GetID(), TEMP_FRIEND_GROUP);
		}
		bool bOnline = false;
		if (UserManager()->GetUser(szSender))
			bOnline = true;
		CMsgTalk	msg;
		if(msg.Create(szSender, szRecvUser, szWords, szTime, dwID, _TXTATR_LEAVEWORD,_TXT_NORMAL,usLookFace,bOnline,szColor,dwUseGMToolPsw,szFeel))
			pUser->SendMsg(&msg);

		pRes->MoveNext();
	}
	pUser->SetSendLeaveWordCount(pRes->RecordCount());
	pRes->Release();
	return true;
}

//////////////////////////////////////////////////////////////////////
void CLeaveWord::OnTimer(DWORD tCurr)
{
	int nStep = 0;
	DEBUG_TRY//goto 加强日志
	if(!m_tAutoFlush.ToNextTime(SECS_PER_AUTOFLUSH))		// 每分钟一次
		return ;
	nStep = 1;
	char	szSQL[1024];
	sprintf(szSQL, "SELECT * FROM %s WHERE id>%u ORDER BY id LIMIT %d", _TBL_LEAVEWORD, m_idLastMsg, NEW_WORDS_PER_MINUTE);
	IRecordset *pRes = Database()->CreateNewRecordset(szSQL);
	if(pRes)
	{
		nStep = 2;
		for(int i = 0; i < pRes->RecordCount(); i++)
		{
			char	szRecvUser[_MAX_NAMESIZE];
			SafeCopy(szRecvUser, pRes->GetStr(LEAVEWORDDATA_USER), _MAX_NAMESIZE);

			CUserPtr pUser = g_UserManager.GetUser(szRecvUser);
			nStep = 3;
			if(pUser && !pUser->IsMoreLeaveWord() && pUser->CanBeSee())
			{
				char	szSender[_MAX_NAMESIZE];
				char	szTime[_MAX_NAMESIZE];
				char	szWords[_MAX_WORDSSIZE];
				char	szColor[_MAX_COLORSIZE];
				DWORD	dwID;
				SafeCopy(szSender, pRes->GetStr(LEAVEWORDDATA_SENDER), _MAX_NAMESIZE);
				SafeCopy(szTime, pRes->GetStr(LEAVEWORDDATA_TIME), _MAX_NAMESIZE);
				SafeCopy(szWords, pRes->GetStr(LEAVEWORDDATA_WORDS), _MAX_WORDSSIZE);
				SafeCopy(szColor, pRes->GetStr(LEAVEWORDDATA_COLOR), _MAX_COLORSIZE);
				dwID	=	pRes->LoadDWord("id");

				nStep = 4;
				DWORD dwUseGMToolPsw=0;
				CFriend* pFriend = pUser->GetFriendByName(szSender);
				if(!pFriend)
					dwUseGMToolPsw=1;//临时好友发来的留言

				CMsgTalk	msg;
// 				if(msg.Create(szSender, pUser->GetName(), szWords, szTime, 0, _TXTATR_LEAVEWORD))
// 					pUser->SendMsg(&msg);

				if(msg.Create(szSender, pUser->GetName(), szWords, szTime, dwID, _TXTATR_LEAVEWORD,_TXT_NORMAL,0,0,szColor,dwUseGMToolPsw))
					pUser->SendMsg(&msg);
				pUser->SetLeaveWord(true);
				pUser->SetSendLeaveWordCount(pUser->GetSendLeaveWordCount()+1);
				//[2010-06-09 goto]此时不删，等用户看了再删
// 				OBJID	idMsg = pRes->GetInt(LEAVEWORDDATA_ID);
// 				sprintf(szSQL, "DELETE FROM %s WHERE id=%u LIMIT 1", _TBL_LEAVEWORD, idMsg);
// 				g_db.ExecuteSQL(szSQL);
			}
			nStep = 5;
			pRes->MoveNext();
		}
		nStep = 6;
		pRes->Release();
	}
	DEBUG_CATCH2("CLeaveWord::OnTimer step=%d", nStep)
}






