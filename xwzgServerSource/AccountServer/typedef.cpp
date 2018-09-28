// 宏定义
// 仙剑修，2001.11.19

#include "AllHeads.h"
#include "typedef.h"

#define DUMP


#define	GETINTROUND(x,y,z)	x = ini.GetInt(#x);\
	if(x < (y))\
		x = (y);\
	else if(x > (z))\
		x = (z);\
	LOGMSG("config.ini->" #x " = %d", x);

#define	GETINTERROR(x,y,z)	x = ini.GetInt(#x);\
	if(x < (y) || x > (z))\
	{\
		LOGERROR("illegal " #x " in config.ini");\
		return false;\
	}\
	LOGMSG("config.ini->" #x " = %d", x);

#define	GETSTRING(x,y)	if(!ini.GetString(#x, x))\
	{\
		LOGERROR("illegal " #x " in config.ini");\
		return false;\
	}\
	if(y && x[0] == 0)\
	{\
		LOGERROR(#x "can't be NULL in config.ini");\
		return false;\
	}\
	LOGMSG("config.ini->" #x " = \"%s\"", x);

//#define	INIDEFINE(x)	int x;
////////////////////////////////////////////////////////////////////////////////
char	SERVERKEY[INIWORDSIZE];
int		LOGINLOOPDELAY;
int		POINTLOOPDELAY;
int		REBUILDLISTENDELAYSEC;
int		LOGINLISTENPORT;
int		POINTLISTENPORT;
int		POINTSOCKETSNDBUF;
int		MAXBANIPS;
int		BANERRORS;
int		BANERRORSECS;
int		BANSECS;
int		ONLINETABLESIZE;
int		HEARTBEATINTERVALSECS;
int		HEARTBEATKICKSECS;
int		ONLINEHEARTBEATSECS;
int		POINTFEEINTERVALSECS;
int		ENABLELOGINDELAY;
int		OUTPUTONLINESECS;
int		REJOINTIMEWAITSECS;
int		COUNTFILESECS;
int		ACCOUNT_AHEAD_DAYS;
int		SERVER_BUSY_DELAY_SECS;
int		SERVER_FULL_DELAY_SECS;
int		KICKOUT_USER_WHEN_NO_POINT;
int		ENABLE_LOGIN_NO_POINT;
int		MONTH_CARD_TO_POINT_CARD;
int		MAX_GM_ID;
int		STAT_SERIAL;
char	SERVER_TITLE[INIWORDSIZE];
char	SERVER_SPECIL[INIWORDSIZE];
char	DBHOSTNAME[INIWORDSIZE];
char	DBUSER[INIWORDSIZE];
char	DBPASSWORD[INIWORDSIZE];
char	DATABASENAME[INIWORDSIZE];
BYTE	g_cbEncryptKey=0;	//加密键值
////////////////////////////////////////////////////////////////////////////////
bool GetConfig()
{
////////////////////////////////////////////////////////////////////////////////
	{
		CIniFile	ini("accountserver.ini", "System");	//06.10.17王玉波修改
		GETSTRING(SERVERKEY, true);
		g_cbEncryptKey=121;//ini.GetInt("ENCRYPT_KEY");//方便运维密钥写死
		GETINTROUND(LOGINLOOPDELAY,			10, 5000);
		GETINTROUND(POINTLOOPDELAY,			0, 500);
		GETINTROUND(REBUILDLISTENDELAYSEC,	0, 60);
		GETINTERROR(LOGINLISTENPORT,		1024, 32767);
		GETINTERROR(POINTLISTENPORT,		1024, 32767);
		GETINTROUND(POINTSOCKETSNDBUF,		0, 1048576);
		GETINTROUND(MAXBANIPS,				1, 1000);
		GETINTROUND(BANERRORS,				1, 100);
		GETINTROUND(BANERRORSECS,			0, 86400);
		GETINTROUND(BANSECS,				0, 86400);
		GETINTROUND(ONLINETABLESIZE,		1000, 1000000);
		GETINTROUND(HEARTBEATINTERVALSECS,	10, 86400);
		GETINTROUND(HEARTBEATKICKSECS,		HEARTBEATINTERVALSECS, 86400);
		GETINTROUND(POINTFEEINTERVALSECS,	20, 86400);
		GETINTROUND(ONLINEHEARTBEATSECS,	POINTFEEINTERVALSECS*2, 86400);	//? 必须大于计点时间片的两倍
		GETINTROUND(ENABLELOGINDELAY,		0, 86400);
		if(ENABLELOGINDELAY <= POINTFEEINTERVALSECS)
			LOGWARNING("登录延迟时间ENABLELOGINDELAY应大于计点间隔POINTFEEINTERVALSECS，否则在线表不能同步。");
		GETINTROUND(OUTPUTONLINESECS,		1, 86400);
		GETINTROUND(REJOINTIMEWAITSECS,		0, 3600);
		GETINTROUND(COUNTFILESECS,			1, 86400);
		GETINTROUND(ACCOUNT_AHEAD_DAYS,		0, 365);
		GETINTROUND(SERVER_BUSY_DELAY_SECS,	0, 86400);
		GETINTROUND(SERVER_FULL_DELAY_SECS,	0, 86400);
		GETINTROUND(KICKOUT_USER_WHEN_NO_POINT,	0, 1);
		GETINTROUND(ENABLE_LOGIN_NO_POINT,	0, 1);
		GETINTROUND(MONTH_CARD_TO_POINT_CARD,	0, 1);
		GETINTROUND(MAX_GM_ID,	0, 1000000);
		GETINTROUND(STAT_SERIAL,	0, 1);
		ini.GetString("SERVER_TITLE", SERVER_TITLE);
		ini.GetString("SERVER_SPECIL",SERVER_SPECIL);
		ini.SetSection("Database");
		GETSTRING(DBHOSTNAME, true);
		GETSTRING(DBUSER, true);
		GETSTRING(DBPASSWORD, false);	// 可以为空串
		GETSTRING(DATABASENAME, true);
	}
////////////////////////////////////////////////////////////////////////////////
	{
		CIniFile	ini("isp.ini", "MAIN");
		int nMaxPart = ini.GetInt("MaxPart");
		if(nMaxPart > 100)
			nMaxPart = 100;
		for(int i = 0; i <= nMaxPart; i++)
		{
			char buf[INIWORDSIZE];
			sprintf(buf, "PART%d", i);
			ini.SetSection(buf);

			ISP_ST		st;
			if(!ini.GetString("Name",		st.szName))
				continue;
			ini.GetString("FirstIP",	st.szFirstIP);
			ini.GetString("LastIP",		st.szLastIP);
			ini.GetString("Account",	st.szAccount);
			ini.GetString("Password",	st.szPassword);
			if(strlen(st.szName) > 0)
				 g_setISPList.push_back(st);
		}
	}
////////////////////////////////////////////////////////////////////////////////

	return true;
}

bool	GetServerAccount()
{
	CIniFile	ini("Account.ini", "Account");
//	g_nServerAccount = ini.GetInt("ACCOUNTNUMS");

	// 修正帐号数
//	if(g_nServerAccount > MAXGAMESERVERS)
//		g_nServerAccount = MAXGAMESERVERS;
//	LOGACCOUNT("Account.ini -> ACCOUNTNUMS = %d", g_nServerAccount);

	for(int i = 1; i < MAXGAMESERVERS; i++)	//?? 1: 从1开始，保留0。
	{
		char bufName[250];
		char bufString[SERVERNAMESIZE];
		sprintf(bufName, "Account%d", i);
		ini.SetSection(bufName);

		ini.GetString("SERVERNAME", bufString);
#ifdef	DUMP
		LOGSERVER("Account.ini -> SERVERNAME = \"%s\"", bufString);
#endif
		strcpy(g_aServerAccount[i].m_szServerName, bufString);

		ini.GetString("LOGINNAME", bufString);
#ifdef	DUMP
		LOGSERVER("Account.ini -> LOGINNAME = \"%s\"", bufString);
#endif
		strcpy(g_aServerAccount[i].m_szLoginName, bufString);

		ini.GetString("PASSWORD", bufString);
#ifdef	DUMP
		LOGSERVER("Account.ini -> PASSWORD = \"%s\"", bufString);
#endif
		strcpy(g_aServerAccount[i].m_szPassword, bufString);

		ini.GetString("SERVERURL", bufString);
#ifdef	DUMP
		LOGSERVER("Account.ini -> SERVERURL = \"%s\"", bufString);
#endif
		strcpy(g_aServerAccount[i].m_szServerURL, bufString);

	
		g_aServerAccount[i].m_nServerLoginMode = ini.GetInt("LOGINTYPE");

		g_aServerAccount[i].m_bFree		= ini.GetInt("FREE_DEBUG");
#ifdef	DUMP
//		LOGSERVER("Account.ini -> FREE_DEBUG = \"%d\"", m_bFree);
#endif

		if(g_aServerAccount[i].m_szServerName[0] == 0 || g_aServerAccount[i].m_szLoginName[0] == 0)
			break;
		if(strlen(g_aServerAccount[i].m_szServerName) >= SERVERNAMESIZE
							|| strlen(g_aServerAccount[i].m_szLoginName) >= SERVERNAMESIZE
							|| strlen(g_aServerAccount[i].m_szPassword) >= SERVERNAMESIZE
							|| strlen(g_aServerAccount[i].m_szServerURL) >= SERVERNAMESIZE)
			break;
	}

	g_nServerAccount = i;
	LOGMSG("共读入[%d]个游戏服务器帐号", g_nServerAccount - 1);
	PrintText("读入%d个帐号", g_nServerAccount - 1);

	// 生成g_szCountHeadLine
	sprintf(g_szCountHeadLine, "时间                | %12s", "      合计");
	for( i = 1; i < g_nServerAccount; i++)	// 1: 从1开始。
	{
		char	buf[1024];
		sprintf(buf, " | %12s", g_aServerAccount[i].m_szServerName);
		strcat(g_szCountHeadLine, buf);
	}
	strcat(g_szCountHeadLine, "\n");

	return true;
}






