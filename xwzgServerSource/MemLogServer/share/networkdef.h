#ifndef _NETWORKDEF_H
#define _NETWORKDEF_H

#include "define.h"

// max size define
const int _MAX_MSGSIZE			= 1024 ;
const int _MAX_WORDSSIZE		= 256  ;
const int _MAX_SPEAK_WORDSSIZE  = 512  ;
const int _MAX_IDLETIME			= 600  ;	// 600s idle time

typedef	char	WORDSSTR[_MAX_WORDSSIZE];

// msg type define
const unsigned short _MSG_NONE					=0;
//client
const unsigned short _MSG_CLIENT_NONE			=10000;
const unsigned short _MSG_CLIENT_LOGIN			=_MSG_CLIENT_NONE+1;
const unsigned short _MSG_CLIENT_RELOGIN		=_MSG_CLIENT_NONE+2;


const unsigned short _MSG_SERVER_NONE			=20000;
const unsigned short _MSG_SERVER_LOGIN			=_MSG_SERVER_NONE+1;
const unsigned short _MSG_SERVER_RELOGIN		=_MSG_SERVER_NONE+2;
const unsigned short _MSG_SERVER_STATUS			=_MSG_SERVER_NONE+3;
const unsigned short _MSG_SERVER_POST			=_MSG_SERVER_NONE+4;
const unsigned short _MSG_SERVER_ACTION			=_MSG_SERVER_NONE+5;

const unsigned short _MSG_SERVER_DATAUSER		=_MSG_SERVER_NONE+1005;
const unsigned short _MSG_SERVER_DATAACC		=_MSG_SERVER_NONE+1006;

// color define
const unsigned long	_COLOR_WHITE	=0x00ffffff;

// msg style define
const unsigned short _TXT_NORMAL	=0;
const unsigned short _TXT_SCROLL	=0x0001;
const unsigned short _TXT_FLASH		=0x0002;
const unsigned short _TXT_BLAST		=0x0008;

// msg attribute define
const unsigned short _TXTATR_NORMAL		=2000;
const unsigned short _TXTATR_PRIVATE	=_TXTATR_NORMAL+1;	// ˽��
const unsigned short _TXTATR_ACTION		=_TXTATR_NORMAL+2;	// ����
const unsigned short _TXTATR_TEAM		=_TXTATR_NORMAL+3;	// ����
const unsigned short _TXTATR_SYNDICATE	=_TXTATR_NORMAL+4;	// ����
const unsigned short _TXTATR_SYSTEM		=_TXTATR_NORMAL+5;	// ϵͳ
const unsigned short _TXTATR_FAMILY		=_TXTATR_NORMAL+6;	// ��ͥ
const unsigned short _TXTATR_TALK		=_TXTATR_NORMAL+7;	// ��̸
const unsigned short _TXTATR_YELP 		=_TXTATR_NORMAL+8;	// �к�
const unsigned short _TXTATR_FRIEND 	=_TXTATR_NORMAL+9;	// ����
const unsigned short _TXTATR_GLOBAL 	=_TXTATR_NORMAL+10;	// ǧ�ﴫ��
const unsigned short _TXTATR_IMPORTINFO =_TXTATR_NORMAL+11;	// ��Ҫ��Ϣ,�൱����ǰ��GMƵ��
const unsigned short _TXTATR_WHISPER 	=_TXTATR_NORMAL+12;	// ����
const unsigned short _TXTATR_GHOST	 	=_TXTATR_NORMAL+13;	// ����
const unsigned short _TXTATR_MAP        =_TXTATR_NORMAL+13;//========add by cz �����ͼƵ����������ͼ�ϵ��˶����Կ���

const unsigned short _TXTATR_SERVE		=_TXTATR_NORMAL+14; // ����
const unsigned short _TXTATR_WORLD		=_TXTATR_NORMAL+16;//  [6/21/2007 Add By HaLei]  С����
const unsigned short _TXTATR_PK			=_TXTATR_NORMAL+17;//  [8/27/2007 Add By HaLei] PK��Ϣ
const unsigned short _TXTATR_DEF		=_TXTATR_NORMAL+18;// ���ط��� halei [10/27/2007]
const unsigned short _TXTATR_CHATWITH	=_TXTATR_NORMAL+19;//�������[2008/12/23]1:1����
const unsigned short _TXTATR_SERVEEX	=_TXTATR_NORMAL+20; // [��;���� 2009.04.29]��չ����Ƶ��
const unsigned short _TXTATR_HOUSE		=_TXTATR_NORMAL+23;
const unsigned short _TXTATR_MYSYSTEM   =_TXTATR_NORMAL+24;

const unsigned short _TXTATR_WORLDSERVER		=_TXTATR_NORMAL+30;//С����
const unsigned short _TXTATR_BOOTHNAME			=_TXTATR_NORMAL+31;//̯λ����
const unsigned short _TXTATR_BOOTHDES			=_TXTATR_NORMAL+32;//̯λ��Ϣ
const unsigned short _TXTATR_BOOTHLEAVEWORD		=_TXTATR_NORMAL+33;//̯λ����
const unsigned short _TXTATR_TOPSHOW			=_TXTATR_NORMAL+34;//top show
const unsigned short _TXTATR_ANNOUNCE			=_TXTATR_NORMAL+35;//ϵͳ����
const unsigned short _TXTATR_TOPANNOUNCE		=_TXTATR_NORMAL+36;//��������
const unsigned short _TXTATR_HELP				=_TXTATR_NORMAL+37;//����
const unsigned short _TXTATR_FATI				=_TXTATR_NORMAL+38;//������
const unsigned short _TXTATR_BIGHORN			=_TXTATR_NORMAL+39;//������

const unsigned short _TXTATR_MAYBEMAIL			=_TXTATR_NORMAL+40;
const unsigned short _TXTATR_PRO				=_TXTATR_NORMAL+41; 
const unsigned short _TXTATR_SECPRO				=_TXTATR_NORMAL+42; 

const unsigned short _TXTATR_PRIVATE_OTHERLINE	=_TXTATR_NORMAL+43;//�����ߴ�������˽����Ϣ
const unsigned short _TXTATR_FORGMTOOL			=_TXTATR_NORMAL+44;//GM������




//------add by cz-----------
const char strLeoTalk[]="LeoTalk";//--------���_TXTATR_WORLD��Ϣ�Ľ�������strLeoTalk��ʾ����һ��ʨ�Ӻ������һ��������Ϣ

const unsigned short _TXTATR_REGISTER	=_TXTATR_NORMAL+100;
const unsigned short _TXTATR_ENTRANCE	=_TXTATR_NORMAL+101;
const unsigned short _TXTATR_SHOP		=_TXTATR_NORMAL+102;
const unsigned short _TXTATR_PETTALK	=_TXTATR_NORMAL+103;
const unsigned short _TXTATR_CRYOUT		=_TXTATR_NORMAL+104;
const unsigned short _TXTATR_WEBPAGE	=_TXTATR_NORMAL+105;

const unsigned short _TXTATR_NEWMESSAGE	=_TXTATR_NORMAL+106;
const unsigned short _TXTATR_TASK		=_TXTATR_NORMAL+107;
const unsigned short _TXTATR_SYNWAR_FIRST	=_TXTATR_NORMAL+108;	// ����ս��
const unsigned short _TXTATR_SYNWAR_NEXT	=_TXTATR_NORMAL+109;	// ����ս��

const unsigned short _TXTATR_LEAVEWORD		=_TXTATR_NORMAL+110;	// ����ϵͳ
const unsigned short _TXTATR_SYNANNOUNCE	=_TXTATR_NORMAL+111;	// ���ɹ���
const unsigned short _TXTATR_MESSAGEBOX		=_TXTATR_NORMAL+112;	// �Ի���
const unsigned short _TXTATR_REJECT			=_TXTATR_NORMAL+113;	// ����
const unsigned short _TXTATR_SYNTENET		=_TXTATR_NORMAL+114;	// ������ּ
const unsigned short _TXTATR_USERSNAP		=_TXTATR_NORMAL+115;	//���0307 ��ҵ�½ʱ�ļ���Ϣ
const unsigned short _TXTATR_ERRORMESSAGEBOX=_TXTATR_NORMAL+116;	// 20090410haocq���������ʾ�Ի���

const unsigned short _TXTATR_MSG_TRADE	=_TXTATR_NORMAL+201;	// �������԰�
const unsigned short _TXTATR_MSG_FRIEND	=_TXTATR_NORMAL+202;	// �������԰�
const unsigned short _TXTATR_MSG_TEAM	=_TXTATR_NORMAL+203;	// ������԰�
const unsigned short _TXTATR_MSG_SYN	=_TXTATR_NORMAL+204;	// �������԰� 
const unsigned short _TXTATR_MSG_OTHER	=_TXTATR_NORMAL+205;	// �������԰�
const unsigned short _TXTATR_MSG_SYSTEM	=_TXTATR_NORMAL+206;	// �������԰�
const unsigned short _TXTATR_MSG_ABOUTLOGIN	= _TXTATR_NORMAL + 207;//�ڿ�ʼ��Ϸʱ�õ�..
const unsigned short _TXTATR_MSG_SERVERLIST	= _TXTATR_NORMAL + 208;//�ڿ�ʼ��Ϸʱ�õ�..
const unsigned short _TXTATR_DIALOG_BEGIN	=_TXTATR_NORMAL+500;	// �Ի����һ��
// ... �м�ı�������֪ͨ�ͻ��˴򿪽���
const unsigned short _TXTATR_DIALOG_END		=_TXTATR_NORMAL+600;	// �Ի������һ�� + 1
#endif
