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
const unsigned short _MSG_NONE			=0;
const unsigned short _MSG_GENERAL		=1000;
const unsigned short _MSG_REGISTER		=_MSG_GENERAL+1;
const unsigned short _MSG_LOGIN			=_MSG_GENERAL+2;
const unsigned short _MSG_LOGOUT		=_MSG_GENERAL+3;
const unsigned short _MSG_TALK			=_MSG_GENERAL+4;
const unsigned short _MSG_WALK			=_MSG_GENERAL+5;
const unsigned short _MSG_USERINFO		=_MSG_GENERAL+6;
const unsigned short _MSG_ATTACK		=_MSG_GENERAL+7;
const unsigned short _MSG_ITEMINFO		=_MSG_GENERAL+8;
const unsigned short _MSG_ITEM			=_MSG_GENERAL+9;
const unsigned short _MSG_ACTION		=_MSG_GENERAL+10;
const unsigned short _MSG_ACCIDENT		=_MSG_GENERAL+11;
const unsigned short _MSG_TICK			=_MSG_GENERAL+12;
const unsigned short _MSG_ROOM			=_MSG_GENERAL+13;
const unsigned short _MSG_PLAYER		=_MSG_GENERAL+14;
const unsigned short _MSG_NAME			=_MSG_GENERAL+15;
const unsigned short _MSG_WEATHER		=_MSG_GENERAL+16;
const unsigned short _MSG_USERATTRIB	=_MSG_GENERAL+17;
const unsigned short _MSG_ROLE			=_MSG_GENERAL+18;
const unsigned short _MSG_FRIEND		=_MSG_GENERAL+19;
const unsigned short _MSG_EFFECT		=_MSG_GENERAL+20;
const unsigned short _MSG_QUIZGAME		=_MSG_GENERAL+21;
const unsigned short _MSG_INTERACT		=_MSG_GENERAL+22;
const unsigned short _MSG_TEAM			=_MSG_GENERAL+23;
const unsigned short _MSG_ALLOT			= _MSG_GENERAL+24;
const unsigned short _MSG_WEAPONSKILL	= _MSG_GENERAL+25;
const unsigned short _MSG_TEAMMEMBER	= _MSG_GENERAL+26;
const unsigned short _MSG_GEMEMBED		=_MSG_GENERAL+27;
const unsigned short _MSG_CHANGEGEM		=_MSG_GENERAL+28;
const unsigned short _MSG_TEAMPOS		=_MSG_GENERAL+29;
const unsigned short _MSG_TITLE			=_MSG_GENERAL+30;
const unsigned short _MSG_LOTTERY		=_MSG_GENERAL+31;
const unsigned short _MSG_CONNECT		=_MSG_GENERAL+52;
const unsigned short _MSG_TRADE			=_MSG_GENERAL+56;
const unsigned short _MSG_BATTLESYSTEM	=_MSG_GENERAL+57;
const unsigned short _MSG_COMBINATION	=_MSG_GENERAL+58;
const unsigned short _MSG_ANTIPLUG	    =_MSG_GENERAL+59;
const unsigned short _MSG_CITYWAR	    =_MSG_GENERAL+60;
const unsigned short _MSG_HOUSE  	    =_MSG_GENERAL+61;
const unsigned short _MSG_STOCKTRADE	=_MSG_GENERAL+62;
const unsigned short _MSG_WEEKACTIVE  	=_MSG_GENERAL+63;
const unsigned short _MSG_USERKEY       =_MSG_GENERAL+66;
const unsigned short _MSG_INFONPCUSEREXIT = _MSG_GENERAL+64;
const unsigned short _MSG_USERPOS = _MSG_GENERAL+65;
const unsigned short _MSG_NEWCARD = _MSG_GENERAL+67;
const unsigned short _MSG_ACCLOCK = _MSG_GENERAL+68;
const unsigned short _MSG_VIPLIST = _MSG_GENERAL+69;
// new info in cq
const unsigned short _MSG_MAPITEM		=_MSG_GENERAL+101;
const unsigned short _MSG_PACKAGE		=_MSG_GENERAL+102;
const unsigned short _MSG_MAGICINFO		=_MSG_GENERAL+103;
const unsigned short _MSG_FLUSHEXP		=_MSG_GENERAL+104;
const unsigned short _MSG_MAGICEFFECT	=_MSG_GENERAL+105;
const unsigned short _MSG_SYNATTRINFO	=_MSG_GENERAL+106;
const unsigned short _MSG_SYNDICATE		=_MSG_GENERAL+107;
const unsigned short _MSG_ITEMINFOEX	=_MSG_GENERAL+108;
const unsigned short _MSG_NPCINFOEX		=_MSG_GENERAL+109;
const unsigned short _MSG_MAPINFO		=_MSG_GENERAL+110;
const unsigned short _MSG_MESSAGEBOARD	=_MSG_GENERAL+111;
const unsigned short _MSG_SYNMEMBERINFO	=_MSG_GENERAL+112;
const unsigned short _MSG_DICE			=_MSG_GENERAL+113;
const unsigned short _MSG_SYNINFO		=_MSG_GENERAL+114;
const unsigned short _MSG_MAGICINTONE	=_MSG_GENERAL+115;
const unsigned short _MSG_EUDPACKAGE	=_MSG_GENERAL+117;
const unsigned short _MSG_EUDEMON		=_MSG_GENERAL+118;
const unsigned short _MSG_EUDEMONINFO	=_MSG_GENERAL+119;
const unsigned short _MSG_SUBINFO		=_MSG_GENERAL+120;
const unsigned short _MSG_INSMEMINFO	=_MSG_GENERAL+121;
const unsigned short _MSG_RELAY			=_MSG_GENERAL+202;///softworms relay msg

const unsigned short _MSG_JINMEISYS     =_MSG_GENERAL+205;
const unsigned short _MSG_PETEGG	    =_MSG_GENERAL+206;
//modify code by �ֵ�Ԩ begin 2011.5.30
const unsigned short _MSG_VASLV			=_MSG_GENERAL+300;
//modify code by �ֵ�Ԩ end
const unsigned short _MSG_NPCINFO		=_MSG_GENERAL+1030;
const unsigned short _MSG_NPC			=_MSG_GENERAL+1031;
const unsigned short _MSG_DIALOG		=_MSG_GENERAL+1032;
const unsigned short _MSG_FRIENDINFO	=_MSG_GENERAL+1033;
const unsigned short _MSG_TIME			=_MSG_GENERAL+1034;
const unsigned short _MSG_PETINFO		=_MSG_GENERAL+1035;
const unsigned short _MSG_DATAARRAY		=_MSG_GENERAL+1036;
const unsigned short _MSG_EUDEMONATTRIB	=_MSG_GENERAL+1037;
const unsigned short _MSG_SCHOOLMEMBER	=_MSG_GENERAL+1038;
const unsigned short _MSG_PLAYERTASK	=_MSG_GENERAL+1039;
const unsigned short _MSG_TASK_LIST		=_MSG_GENERAL+1040;
const unsigned short _MSG_ANNOUNCE_LIST =_MSG_GENERAL+1041;
const unsigned short _MSG_ANNOUNCE_INFO =_MSG_GENERAL+1042;
const unsigned short _MSG_AUCTION       =_MSG_GENERAL+1043;
const unsigned short _MSG_CHATROOM      =_MSG_GENERAL+1044;	//ʥս��������ϵͳ��Ϣ
const unsigned short _MSG_ITEMATTRIB	=_MSG_GENERAL+1045;
const unsigned short _MSG_TRAFFIC		=_MSG_GENERAL + 1046;	//
const unsigned short _MSG_COOLDOWN		=_MSG_GENERAL + 1047;	//������ȴʱ��,06.9.4����..
const unsigned short _MSG_TOPLIST		=_MSG_GENERAL + 1048;	//���а���Ϣ��,06.9.9����..
const unsigned short _MSG_BUFF			=_MSG_GENERAL + 1049;	//״̬��Ϣ
const unsigned short _MSG_TASKDETAIL	=_MSG_GENERAL + 1050;	//20061107����.������Ϣ
const unsigned short _MSG_COMPOUND  	=_MSG_GENERAL + 1051;	//20061107����.������Ϣ
const unsigned short _MSG_VAS	        =_MSG_GENERAL + 1052;	///07.01.09�����ֵ������Ϣ
const unsigned short _MSG_MAIL	        =_MSG_GENERAL + 1053;	///07.01.09�����ֵ������Ϣ
const unsigned short _MSG_WAITQUEUE		=_MSG_GENERAL + 1054;	//  [2/12/2007 Halei] ����Ŷ���Ϣ
const unsigned short _MSG_MARRY			=_MSG_GENERAL + 1055;	//  [4/14/2007 softworms] ������Ϣ
const unsigned short _MSG_GMACTION		=_MSG_GENERAL + 1056;	//  [4/25/2007] HALEI GM������Ϣ
const unsigned short _MSG_GMADVACTION	=_MSG_GENERAL + 1057;	//  [4/27/2007] HALEI GM���߸߼�Ȩ����Ϣ
const unsigned short _MSG_GMACTIONLOG	=_MSG_GENERAL + 1058;	//  [4/28/2007] HALEI GM������־
const unsigned short _MSG_GMMAC			=_MSG_GENERAL + 1059;	//  [6/19/2007] halei GM mac��ַ��
const unsigned short _MSG_GMCHGPSW		= _MSG_GENERAL + 1060;	//  [6/27/2007 Add By HaLei] GM �޸�GM���߸߼�Ȩ������
const unsigned short _MSG_GMSUPERCREATE = _MSG_GENERAL + 1061;	//  [8/4/2007 Add By HaLei] ����������Ʒ
const unsigned short _MSG_GMSTOPACCOUNT	= _MSG_GENERAL + 1062;	//  [8/15/2007 Add By HaLei] GM���
const unsigned short _MSG_CHANGE_NAME   = _MSG_GENERAL + 1063;  //������Ϣ//  [8/21/2007 %PENGFENG%]
//const unsigned short _MSG_GENERALIZE    = _MSG_GENERAL + 1070;	// ����������ϢID ��� [7/10/2007]
const unsigned short _MSG_WARGAME		= _MSG_GENERAL + 1064;	//  [2007-8-29] ��� ս���
const unsigned short _MSG_ADJUSTRANGE	= _MSG_GENERAL + 1065;// �޸�adjust�������Ի�ñ���,����,������ʵ��� [9/12/2007 Add By Halei ]
const unsigned short _MSG_EXAM			= _MSG_GENERAL + 1066;// ���ִ��� [9/22/2007 Administrator]
const unsigned short _MSG_WEAPONSOUL			= _MSG_GENERAL + 1070;//���
const unsigned short _MSG_PKGAMEINFO			= _MSG_GENERAL + 1071;	//  [1/8/2008 ���]	PK��������������Ϣ
const unsigned short _MSG_WEAPONSOULSHOW		= _MSG_GENERAL + 1072; //������
const unsigned short _MSG_INSTANCE			= _MSG_GENERAL + 1073;//20071214������:����
const unsigned short _MSG_GIFT			= _MSG_GENERAL + 1067;//20071203����:�츳
const unsigned short _MSG_HELPACTION			= _MSG_GENERAL + 1068;
const unsigned short _MSG_SHOP			= _MSG_GENERAL + 1074;//[��;���� 2009.01.05]����ϵͳ
const unsigned short _MSG_USERPROPERTY	= _MSG_GENERAL + 1075;//20090218�������
const unsigned short _MSG_VASITEM	= _MSG_GENERAL + 1080;//20090218�������
const unsigned short _MSG_ISSUE	= _MSG_GENERAL + 1076;//�ڿ�ʼ��Ϸʱ�õ�..
const unsigned short _MSG_BROADCAST		= _MSG_GENERAL + 1077;///[2009.3.2--�Ŷ�]ϵͳ�㲥��Ϣ
const unsigned short _MSG_TASKACTION    = _MSG_GENERAL + 1078;//����Action
const unsigned short _MSG_AUTOFIGHT     = _MSG_GENERAL + 1079;//�Զ��һ�
const unsigned short _MSG_VIRTUALITEM	= _MSG_GENERAL + 1081; //������Ʒ��Ϣ
const unsigned short _MSG_SYNATTACKWAR  = _MSG_GENERAL + 1082; //���ս��

const unsigned short _MSG_FIGHTTEAMINFO  = _MSG_GENERAL + 1083;   //ս����Ϣ
const unsigned short _MSG_FIGHTTEAMACTION = _MSG_GENERAL + 1084;  //ս�Ӳ���
const unsigned short _MSG_FIGHTTEAMRESULTINFO = _MSG_GENERAL + 1085; //ս�ӷ������
const unsigned short _MSG_SAVEITEMVIEWINFO    = _MSG_GENERAL + 1086;  //��ѯ���������Ϣ
const unsigned short _MSG_GAINAWARDS			= _MSG_GENERAL + 1090;       //�콱
const unsigned short _MSG_PLAYERDETIL			= _MSG_GENERAL + 1091; 
const unsigned short _MSG_NPCSHOP				= _MSG_GENERAL + 1092; 
const unsigned short _MSG_SYNLIST				= _MSG_GENERAL + 1093; 
const unsigned short _MSG_RANDNPCSHOP			= _MSG_GENERAL + 1094; 
const unsigned short _MSG_COMPSITION			= _MSG_GENERAL + 1095;
const unsigned short _MSG_COMPSITIONMSG			= _MSG_GENERAL + 1096;


// new msg in qq
const unsigned short _MSG_WALKEX		=_MSG_GENERAL+2005;

// msg npc server
const unsigned short _MSG_NPCSERVER		=20000;
const unsigned short _MSG_AINPCINFO		=_MSG_NPCSERVER+1;

//	monitor server
const unsigned short _MSG_MONITORSERVER	= 30000;
const unsigned short _MSG_MONITORSERVER_MONITOR = _MSG_MONITORSERVER + 1;

// msg account server
const unsigned short _MSG_ACCOUNTSERVER	=1000;
const unsigned short _MSG_LOGIN_A	=_MSG_ACCOUNTSERVER+2;
const unsigned short _MSG_ACCOUNT_A	=_MSG_ACCOUNTSERVER+51;
const unsigned short _MSG_MAINSERVER=_MSG_ACCOUNTSERVER+55;
//const unsigned short _MSG_CONNECT	=_MSG_ACCOUNTSERVER+52;
const unsigned short _MSG_FEE_A		=_MSG_ACCOUNTSERVER+200;		// paled
const unsigned short _MSG_QUERYFEE_A=_MSG_ACCOUNTSERVER+201;		// paled

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

const unsigned short _TXTATR_MAYBEMAIL			=_TXTATR_NORMAL+40;//ϵͳ�ʼ�����ʱ��
const unsigned short _TXTATR_PRO				=_TXTATR_NORMAL+41;//������˵��
const unsigned short _TXTATR_SECPRO				=_TXTATR_NORMAL+42;//������˵��

const unsigned short _TXTATR_PRIVATE_OTHERLINE	=_TXTATR_NORMAL+43;//�����ߴ�������˽����Ϣ
const unsigned short _TXTATR_FORGMTOOL			=_TXTATR_NORMAL+44;//GM������
const unsigned short _TXTATR_TREASURE			=_TXTATR_NORMAL+45;//�����ӹ㲥Ƶ��
const unsigned short _TXTATR_WARGAME			=_TXTATR_NORMAL+46;//��������ӪƵ��


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

const unsigned short _TXTATR_SYNUPSKILL		=_TXTATR_NORMAL+117;	//���ɼ�������
const unsigned short _TXTATR_SYNWORK		=_TXTATR_NORMAL+118;	//���ɴ�
const unsigned short _TXTATR_SYNAWARDMONEY		=_TXTATR_NORMAL+119;	//���ɼ�Ǯ���߼ӷ��ٽ����


const unsigned short _TXTATR_MSG_TRADE	=_TXTATR_NORMAL+201;	// �������԰�
const unsigned short _TXTATR_MSG_FRIEND	=_TXTATR_NORMAL+202;	// �������԰�
const unsigned short _TXTATR_MSG_TEAM	=_TXTATR_NORMAL+203;	// ������԰�
const unsigned short _TXTATR_MSG_SYN	=_TXTATR_NORMAL+204;	// �������԰� 
const unsigned short _TXTATR_MSG_OTHER	=_TXTATR_NORMAL+205;	// �������԰�
const unsigned short _TXTATR_MSG_SYSTEM	=_TXTATR_NORMAL+206;	// �������԰�
const unsigned short _TXTATR_MSG_ABOUTLOGIN	= _TXTATR_NORMAL + 207;//�ڿ�ʼ��Ϸʱ�õ�..
const unsigned short _TXTATR_MSG_SERVERLIST	= _TXTATR_NORMAL + 208;//�ڿ�ʼ��Ϸʱ�õ�..
// ....... �뱣������ID

const unsigned short _TXTATR_DIALOG_BEGIN	=_TXTATR_NORMAL+500;	// �Ի����һ��
// ... �м�ı�������֪ͨ�ͻ��˴򿪽���
const unsigned short _TXTATR_DIALOG_END		=_TXTATR_NORMAL+600;	// �Ի������һ�� + 1

const unsigned short _MSG_Fatigue			=_TXTATR_NORMAL+700;		// �����԰�

const unsigned short _TXTATR_SPECILLINE		=_TXTATR_NORMAL+1000;		//�����ڸ���NPC������ר��
#endif
