
#ifndef  _MYDB_
#define  _MYDB_

#include "mysql.h"
#include "I_mydb.h"

//------ for bcb use ------
#ifdef _BCB
typedef __int64 _int64;
#endif
//--------------------------

#include "mydatabase.h"
#include "myfield.h"
#include "myrecordset.h"
#include "myrecord.h"
#include "MyConstRecord.h"

const int			INT_NONE	= 0;			// �Ƿ�����������ֵ
const LPCTSTR		STR_NONE	= "";			// �Ƿ��Ĵ�����ֵ
const double		DOUBLE_NONE    = 0;			//-------���---2007/05/15-----
const __int64		INT64_NONE    = 0;			// ��� [7/23/2007]		

static const char	DEFALT_SQL_STMT[]			=	"SELECT * FROM ";
static const char	SQL_STMT_SHOWDATABASES[]	=	"SHOW  DATABASES";
static const char	SQL_STMT_SHOWTABLES[]		=	"SHOW  TABLES";

static const char	SQL_STMT_DELETE[]			=	"DELETE FROM %s %s WHERE %s";
static const char	SQL_STMT_UPDATE[]			=	"UPDATE %s SET %s WHERE %s LIMIT 1";	//paled add "LIMIT 1"
static const char	SQL_STMT_INSERT[]			=	"INSERT %s SET %s";		//paled - ������һ��������������

//------------07.5.24����ע�����²��޸�--------------------------
//static const DWORD	_MAX_DBACCESSTIME			=	10000;
//��10���Ϊ1/10��,Ŀ��Ϊ��������̻߳������䲢�޸���.�����Ļ���Ϊÿһ���߳�ʵ����һ�����ݿ�,���������ϲ�Ӧ�ô��ڻ���
static const DWORD	_MAX_DBACCESSTIME				=	10000;
//-------------end-------------------------------------------------

//extern CMyDatabase		g_db;06.11.16����ע��

#endif
