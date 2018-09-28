// action list
// paled, 2003.1.25

////////////////////////////////////////////////////////////////////////
// "%%" ��ʾ����ʾ�� "%" ����
// "%nnXXX" ��ʾ��ʾ��ȣ�һ������롣�����ᰴʵ�ʳ�����ʾ��(��idΪ1234��NPC��"%6id"��ʾΪ"1234  ")

// ϵͳ���� //////////////////
const char PARAM_DATE_STAMP[]			= "date_stamp";					// �����ڼ���
const char PARAM_TIME[]					= "time";						// �����ڼ���
const char ACCEPT_STR_[]				= "accept";						// �ͻ����ϴ���һ���Ӵ� // %accept0 ~ %accept3

// ITEM���� //////////////////
const char	PARA_ITEM_TYPE[]			= "item_type";
const char	PARA_ITEM_DATA[]			= "item_data";

// NPC���� //////////////////
const char	PARA_DATASTR[]				= "datastr";
const char	PARA_DATA_[]				= "data";							// %data0 ~ %data3
const char	PARA_NAME[]					= "name";							// npc name
const char	PARA_NPC_ID[]				= "id";								// npc id in database
const char	PARA_NPC_X[]				= "npc_x";
const char	PARA_NPC_Y[]				= "npc_y";
const char	PARA_NPC_OWNERID[]			= "npc_ownerid";

// USER���� ////////////////
const char	PARA_USER_ID[]				= "user_id";
const char	PARA_USER_MAPID[]			= "user_map_id";
const char	PARA_USER_MAPX[]			= "user_map_x";
const char	PARA_USER_MAPY[]			= "user_map_y";
const char	PARA_USER_HOME[]			= "user_home_id";
const char	PARA_SYN_ID[]				= "syn_id";
const char	PARA_SYN_NAME[]				= "syn_name";
const char	PARA_USER_NAME[]			= "user_name";
const char	PARA_MATE_NAME[]			= "mate_name";
const char	PARA_MAP_OWNER_ID[]			= "map_owner_id";
const char	PARA_MAP_OWNER_TYPE[]		= "map_owner_type";
const char	PARA_ALLY_SYN_[]			= "ally_syn";						// %ally_syn0 ~ %ally_syn4
const char	PARA_ENEMY_SYN_[]			= "enemy_syn";						// %enemy_syn0 ~ %enemy_syn4
const char	PARA_TUTOREXP_[]			= "tutor_exp";						// ��ʦ����
const char	PARA_STUDENT_EXP_[]			= "student_exp";					// ͽ�ܹ��׵ľ���
const char	PARA_EXPLOIT_[]				= "exploit";						// ��ѫֵ
//���ɲ���
const char	PARA_AVAILABLE_FUND[]		= "available_fund";					// ��ȡ���ɵĿ��Է������
// user �� iter ���� ////////////////
const char	PARA_ITER_VALUE[]			= "iter_value";
const char	PARA_ITER_SYN_NAME[]		= "iter_syn_name";
const char	PARA_ITER_SYN_LEADER[]		= "iter_syn_leader";
const char	PARA_ITER_SYN_MONEY[]		= "iter_syn_money";
const char	PARA_ITER_SYN_AMOUNT[]		= "iter_syn_amount";
const char	PARA_ITER_SYN_FEALTY[]		= "iter_syn_fealty";
const char	PARA_ITER_MEMBER_NAME[]		= "iter_member_name";				// %iterΪ���ID�������������
const char	PARA_ITER_MEMBER_RANK[]		= "iter_member_rank";				// %iterΪ���ID��������ҵȼ��ƺ�
const char	PARA_ITER_MEMBER_PROFFER[]	= "iter_member_proffer";
const char	PARA_ITER_WANTED[]			= "iter_wanted";
const char	PARA_ITER_POLICEWANTED[]	= "iter_police_wanted";
const char	PARA_ITER_GEMSUPQUALITY[]	= "iter_upquality_gem";
const char	PARA_ITER_GEMSUPLEVEL[]		= "iter_uplevel_gem";
const char	PARA_ITER_COST_DURRECOVER[]	= "iter_cost_durrecover";			// 
const char	PARA_ITER_GAME_CARD[]		= "iter_game_card";					// 
const char	PARA_ITER_GAME_CARD2[]		= "iter_game_card2";				// 

const char	PARA_ITER_TABLE_DATASTR[]	= "iter_table_datastr";
const char	PARA_ITER_TABLE_DATA_[]		= "iter_table_data";				// %iter_table_data0 ~ %iter_table_data3

const char	PARA_ITER_ITEM_DATA[]		= "iter_item_data";

//---����ϵͳ��iter---begin
const char	PARA_ITER_TASK_USERNAME[]		= "iter_task_username";		//������ҵ�cq_user���е�name�ֶ�
const char	PARA_ITER_TASK_COMPLETENUM[]	= "iter_task_completenum";  //��������ɴ���ȡֵ������������ɴ���
const char	PARA_ITER_TASK_BEGINTIME[]		= "iter_task_begintime";    //������ʼʱ��ȡֵ����������ʼʱ��
//---����ϵͳ��iter---end

// ��Դս���� [8/28/2007 %PENGFENG%]
const char PARA_DRAGON_STONE_NUM[]		    = "dragon_stone_num";		//���ص�ǰ�����õ�ʱ����Ƭ����
const char PARA_HUMAN_STONE_NUM[]			= "human_stone_num";		//���ص�ǰ�����õ�ʱ����Ƭ����
const char PARA_GENE_STONE_NUM[]			= "gene_stone_num";			//���ص�ǰ�����õ�ʱ����Ƭ����
// ��Դս����END [8/28/2007 %PENGFENG%]

// �����ݵ㲿�� [9/21/2007 Administrator]
const char PARA_SAVEPOINT_THISTIME_NUM[]	= "savepoint_thistime_num";		//���ر��������������۾���
const char PARA_SAVEPOINT_ALLNUM[]			= "savepoint_allnum";			//���ػ��۾�����ֵ
const char PARA_SAVEPOINT_RESIDUAL_TIME[]	= "savepoint_residual_time";	//����ʣ�����������ʱ��
// end [9/21/2007 Administrator]

////////////////////////////////////////////////////////////////////////
enum {
		// system part, need user ptr --------------------------------------------------------------------------
		ACTION_SYS_FIRST			= 100,
		ACTION_MENUTEXT				= 101,			// �˵��ı���data:��ʾ����(ȱʡΪ0), param="text"(��ѡ): ��ʾ�ı��������Ϳɰ����ո�Ҳ��Ϊ���С�
		ACTION_MENULINK				= 102,			// �˵������ӡ�"text task_id align": align(��ѡ): ����ģʽ(ȱʡ/0:�����; 5:����; 9:�Ҷ���; a(1-9)b(2-8):������,��a/b�п���ʼ��ʾ,9Ϊ�Ҷ���)
		ACTION_MENUSHOW				= 103,			// �˵������"len task_id text": len:������ĳ��ȣ�text(��ѡ)����ʾ�����֣�align������ģʽ(ȱʡ�����������������������Ƕ�������ֵĵڼ��ַ���)����˵�������ж��������ɽ�����Բ��ť���֣�ÿ��ֻ�ϴ�һ����
		ACTION_RAND					= 121,			// �������ʡ�"data1 data2"��"10 100"��ʾ��1/10�Ļ�����true��
		ACTION_RANDACTION			= 122,			// ���Action "action0 action1... action7"��8���������һ��ִ��
		ACTION_CHKTIME				= 123,			// dataΪʱ������ 0-��鵱ǰ��������ϸʱ�� "%d-%d-%d %d:%d %d-%d-%d %d:%d"; 1-�����ĳ��ʱ��"%d-%d %d:%d %d-%d %d:%d", 2-�����ĳ��ʱ��"%d %d:%d %d %d:%d", 3-�����ĳ��ʱ��"%d %d:%d %d %d:%d", 4-�����ʱ��"%d:%d %d:%d", 5-���Сʱʱ��"%d %d"(ÿ��Сʱ�ĵڼ��ֵ��׼���)
		ACTION_ERRORMESSAGEBOX		= 127,			// �ÿͻ��˵������������ʾ�Ի���20090410haocq���
		ACTION_CHATSYSBOX			= 128,          // ϵͳ1:1�����
		ACTION_SYS_LIMIT			= 199,

		// npc part, need npc ptr --------------------------------------------------------------------------
		ACTION_NPC_FIRST			= 200,
		ACTION_NPC_ERASE			= 205,			// ɾ����ǰNPC���޶�̬NPC��ע�⣺ɾ���󣬲�Ҫ�ٶԴ�NPC���в�����dwData��Ϊ0����ʾɾ������ͼ������typeΪdwData��NPC��param="idMap type": ɾ��ָ����ͼ��ĳЩNPC��
		ACTION_NPC_ISTTAG			= 241,
		ACTION_NPC_LIMIT			= 299,		

		// map part, need curr map --------------------------------------------------------------------------
		ACTION_MAP_FIRST			= 300,
		ACTION_MAP_MOVENPC			= 301,			// ��npc�ƶ���ָ���ĵ�ͼ��λ��(�����ڹ̶�NPC)��data Ϊָ����npc��ID, paramΪ "idMap nPosX nPosY"��ע�⣺���ƶ����õ�ͼ��(0,0)���꣬�Ա�����NPC��
		ACTION_MAP_MAPUSER			= 302,			// �ж�ָ����ͼ�е��û�����, data Ϊָ����ͼID, paramΪ "cmd opt data",
													//  cmd֧��"map_user"��"alive_user", opt����Ϊ"==, <=, >="��dataΪ����
		ACTION_MAP_BROCASTMSG		= 303,			// �㲥��Ϣ, data Ϊ��ͼid, szParamΪ�㲥����Ϣ
		ACTION_MAP_LIMIT			= 399,

		// item action only part, need pUser+pItem ptr --------------------------------------------------------------------------
		ACTION_ITEMONLY_FIRST		= 400,
		ACTION_ITEM_DELTHIS			= 498,			// ɾ����ǰ������Ʒ��ע�⣺����Ϊ���һ��ACTION��
		ACTION_ITEMONLY_LIMIT		= 499,

		// user item part, need user ptr --------------------------------------------------------------------------
		ACTION_ITEM_FIRST			= 500,		ACTION_ITEM_CHECK			= 503,			// �����Ʒ��data=itemtype_id, param��Ϊ0ʱ��ͬʱ�����Ʒ������(���;ö�)������Ʒ����ﵽҪ�������(���;ö�)���С�����dataΪ0��param��ʾҪ�ҵ���Ʒ����
		ACTION_ITEM_HOLE			= 504,			// �����򶴡�param ֧�� "ChkHole HoleNum" ����"MakeHole HoleNum"��NumΪ1����2
		ACTION_ITEM_LEAVESPACE		= 508,			// ��鱳��ʣ��ռ䡣param="space weight packtype"
													// ����packtype��ȡֵ��ΧΪ50 ~ 53
													// 50����ͨ��Ʒ����
													// 51��ħ�걦ʯ����
													// 52�����޵�����
													// 53�����ޱ���
		ACTION_ITEM_ADD_COUNT		= 519,
		ACTION_SOUL_ADD_TYPE		= 520,
		ACTION_ITEM_LOCK_CHECK		= 527,
		ACTION_ITEM_ADD_PEIFANG     = 529,
		ACTION_ITEM_DEL_PEIFANG     = 530,
		ACTION_ITEM_HAS_PEIFANG     = 531,
		ACTION_ITEM_LIMIT			= 599,

		// user syndicate part --------------------------------------------------------------------------
		ACTION_SYN_FIRST			= 700,
		////////////////////////////////////////////////////////
		// ����Action����
		ACTION_SYN_CREATE			= 701,			// ������Ҫ����������֡�param="level money leave_money", ��������Ϊ��Ҫ����ҵȼ�����Ҫ���ֽ��������ɳ����������ֽ�����
		ACTION_SYN_DESTROY			= 702,			// ��ɢ��ִ��Action�����Ϊ�ų������ų����ֶӳ���ʱ�򣬷ֱ��ʾ��ɢ���ɡ����š��ֶ�
		ACTION_SYN_DONATE			= 703,			// ����Ҫһ�������
		ACTION_SYN_ATTR				= 717,			// �����޸İ������ԣ�������С��3����ȱʡ����ID�ǵ�ǰ��ҵİ���ID��
													// param="szField szOpt data syn_id", szField��ѡ��
													// ����"money"	(opt��ѡ"+=", "<"), 
													// ������"repute"	(opt��ѡ"+=", "<"),
													// ������"membernum"(optΪ"<"),
													// �����ɣ�"fealty"	(optΪ"=="),
													// �ȼ���"level"	(opt��ѡ"=", "+=", "<", "==")
													// 20070413����:���Ź��׶� "synproffer" (opt��ѡ"=", "+=", "<",">", "==")
													// 20070416����:���˹��׶� "userproffer" (opt��ѡ"=", "+=", "<",">", "==")
													// 20070421����:���˹��׵ȼ� "userprofferlev" (opt��ѡ"<",">", "==")
													// 20070416����:���˾�����������.�� "taskMaskWeekBit" (opt��ѡ"==", "=")
													//								����λ.��Ӧ[�ж�Ϊ��].[��Ϊ��]����
													//								data:1~31.32��ʾ�Ƿ�����Ч
													//	���˾�����������.��..."taskMaskDayBit".ͬ��

		ACTION_SYN_CHK_SYN			= 732,			// 20070409����:��ǰ����Ƿ��о���

		//----------------------ʦͽϵͳ--------------------------------------------
		ACTION_TUTOR_GRADUATE		        =746,       // ʦ����ͽ�������ʦ [�½���10/26/2007]
		ACTION_TUTOR_TEACHER                =748,      // �ж��Ƿ�Ϊʦ�� [�½���10/26/2007]
		ACTION_TUTOR_STUDENT                =749,      // �ж��Ƿ�Ϊͽ�� [�½���10/26/2007]
		
		ACTION_TUTOR_TEAM_ISSTU				=750,		// goto �����ǲ���ͽ��
		ACTION_TUTOR_TEAM_STULEV			=751,		// goto �ж϶�����ͽ�ܵȼ�
		ACTION_TUTOR_DIVORCE				=752,		// goto Э����ʦͽ��ϵ,��ͽ�ܷ���ȷ�Ͽ�
		ACTION_TUTOR_HASTUTOR				=753,		// goto �ж��Ƿ���ʦ��
		ACTION_TUTOR_HASSTUDENT				=754,		// goto �ж��Ƿ���ͽ��
		ACTION_TUTOR_STUREBEL				=755,		// goto ͽ����ʦ
		ACTION_TUTOR_TEAM_HASTUTOR			=756,		// goto ʦ���߳�ͽ��
		
		ACTION_TUTOR_ITEMWITHNAME			=757,		// goto ������ҿ������ֵ��������
		ACTION_TUTOR_KICKOUTMSG				=758,		// ʦ��Ҫ�߳�ͽ�ܣ����ͻ���ͽ������
		ACTION_TUTOR_REBELWIN				=759,		// ͽ����ʦǰ��ȷ�Ͽ�
		ACTION_TUTOR_TUTORNAME_ITEM			=760,		// ���ɿ���ʦ�����ֵ���Ʒ
		ACTION_TUTOR_STUNAME_ITEM			=761,		// ���ɿ��ж�����ͽ�����ֵ���Ʒ
		ACTION_TUTOR_ADDSTUBUF				=762,		// �������е�ͽ�ܼ�Buf
		ACTION_TUTOR_STUAROUND				=763,		// ������ͽ���Ƿ��ڸ���
		ACTION_TUTOR_ITEMWITHPOS			=764,		// ���ɴ��������Ʒ �����¼��itemAddition1,itemAddition2,itemAddition3��
		ACTION_TUTOR_CREATEMONSTER			=765,		// ��ָ���ĵص�ˢ��
		ACTION_TUTOR_GRADUATEWIN			=766,		// ����ͽ��ȷ�ϳ�ʦ
		ACTION_TUTOR_TEAMADD				=767,		// ��Ӱ�ʦ,��ͽ�ܵ�ȷ�Ͽ�
		ACTION_SYN_LIMIT					= 799,

		// monster part --------------------------------------------------------------------------
		ACTION_MST_FIRST			= 800,
		ACTION_MST_LIMIT			= 899,		

		// user attr part --------------------------------------------------------------------------
		ACTION_USER_FIRST			= 1000,
		ACTION_USER_ATTR			= 1001,			// ������Ե��޸ĺͼ�顣"attr opt data"��attr��ѡ��
													//		"life"(+=,==,<)��
													//		"mana"(+=,==,<)��
													//		"money"(+=,==,<)��
													//		"exp"(+=,==,<)��
													//		"pk"(+=,==,<)��
													//		"profession"(==, set, >=, <=)��
													//		"level"��(+=,==,<)��
													//		"force"��(+=,==,<)��
													//		"dexterity"��(+=,==,<)
													//		"speed"��(+=,==,<)��
													//		"health"��(+=,==,<)��
													//		"soul"��(+=,==,<)��
													//		"rank"��(==,<)��//20070616����:syn_rank
													//		"rankshow"��(==,<)��
													//		"iterator"��(=, <=, +=, ==)��
													//		"crime" (==, set)
													//		"gamecard"(==, >=, <=)
													//		"gamecard2"(==, >=, <=)
													//		"xp"(+=)
													//		"metempsychosis"(==, <)
//													//      "nobility_rank"(==, <, +=, =)	// 
													//		"mercenary_rank"(==, <, +=)		// Ӷ���ȼ�
													//		"mercenary_exp"(==, <, +=)		// Ӷ������
													//		"exploit"(==, <, +=)			// ��ѫֵ
													//		"maxlifepercent"(+=,==,<)		// �������ǧ�ֱ� 
													//		"maxlifevalue"(+=,==,<)			// �������ǧ�ֱȡ���ת��Ϊ������ֵ����
													//		"turor_exp"(==,<,+=,=)
													//		"tutor_level"(==,<,+=,=)
													//		"syn_proffer"(<,+=,=)	//���ɹ��׶�
													//		"maxeudemon"(<,+=,=)	//�����ٻ�������
													//		"packcount"(<,+=,=)     //�û�������
													//		"packrow_count"(<,+=,=) //���ŵı���������08.9.27�������
													//20070616����:
													//		"honor"(+=,-=,==,<,>,>=,<=) ����ֵ
													//		"rank" (==,<,>,>=,<=) ���׵�
													//		"rankLev" (==,<,>,>=) ���׼���
													//		"present" (+=,==,>=) ������Ʒװ��
		ACTION_USER_FULL			= 1002,			// ����ҵ����Լ�����"attr"��attr��ѡ"life","mana"
		ACTION_USER_CHGMAP			= 1003,			// �е�ͼ param "idMap nPosX nPosY bPrisonChk", bPrisonChkΪ��ѡ������ȱʡ�����Գ�����������Ϊ1���Գ�
		ACTION_USER_RECORDPOINT		= 1004,			// ���¼�� param "idMap nMapX nMapY"
		ACTION_USER_RESETPOINT		= 1005,			// "color num"
													// "style num"
		ACTION_USER_CHECKWEAPENSOUL	= 1007,			//�Ƿ��л���
	    ACTION_USER_INSTANCETIMES_OPERATE   = 1013,  //�û�������������
	    ACTION_USER_RANDSHOPFULL     = 1014,
		ACTION_USER_MAGIC			= 1020,			// ��鷨����param����Ϊ��
													// "check type"(���ѧ��type���͵ķ���),
													// "check type level"(���ѧ��type���͵ķ������ҵȼ�Ϊlevel��),
													// "learn type"(���ѧ��type�෨�����ȼ�Ϊ0)��
													// "uplevel type"(��ҵ�type�෨����һ��)
													// "addexp type exp"(��ҵ�type�෨������exp�㾭��)
		ACTION_USER_CHECKINSTANCE	= 1021,			// "check type level", ����������ܵ����ͺ͵ȼ����Ƿ�>=�ȼ�
													// "learn type level", ѧϰָ�����ͺ͵ȼ��ļ���
		ACTION_USER_LOG				= 1022,			// ����ָ����Ϣ��gm log�����봥������Ϣ�����ֺ�id��,��Ϣָ����param��
													// ���� "%s ����콣���񲢻�ñ�ʯ", param��%s���Ǳ��津������Ϣ��λ��
		ACTION_USER_SEX				= 1026,			// �Ա���, �з���1��Ů����0
		ACTION_USER_OPEN_DIALOG		= 1046,			// ֪ͨ�ͻ��˴�һ�����档data=idDialog��param="task_id0 task_id1 task_id2 task_id3 ..."��������param�����20��task_id, task_id����Ϊ0, ����ͻ���ѡ����һ��TASK����paramʱ���ͻ���ֻ���ϴ����ͻ��˿ɴ�����TASK������paramʱ��cq_task.client_active����Ϊ0��
		ACTION_USER_CHGMAP_REBORN	= 1047,			// ����������㡣
		ACTION_USER_CHK_ITEMAMOUNT	= 1050,
		ACTION_USER_TAKESTUDENTEXP	= 1051,			// ��ȡͽ�ܹ��׵ľ��顣��ҷ���pszAcceptָ����Ҫ��ȡ�ľ���ֵ���Զ��۳���ʦ���顣
		ACTION_USER_CHGTO_MAINMAP	= 1052,			// ������ͼ�ĸ���㸴��
		ACTION_USER_CHGTO_RANDOMPOS	= 1053,			// ��������ɵ���ǰ��ͼ������һ�����꣨�õ㲻�������룩
		ACTION_USER_ADD_TIME_STATUS = 1062,		//  [12/28/2007 ���]	��action�Ĺ���Ϊ��������һ��magictype��ħ����BUFF��,��buff����"���ʱ��"��ʧ��
		ACTION_USER_CHK_IN_RANGE	= 1065,		//  [1/27/2008 ���]	����action�������ж���������ĵ�ͼ����
		ACTION_SEND_NOTICE			= 1069,			// ȫ������
		//---����ϵͳ����¼�����ϸ��---begin
		ACTION_USER_DEPOT_CODE      = 1070,
		ACTION_USER_ADDEUDEMON			= 1075, 
		ACTION_USER_EUDEMON_DEAD		= 1077,
		ACTION_USER_EUDEMON			= 1079,
		ACTION_USER_TASK_MANAGER	= 1080,
		ACTION_USER_CHECKINSFINISHTIME	= 1081,
		ACTION_USER_SETJOB				= 1096,
		ACTION_USER_DELCURNPC			= 1097,
		ACTION_TASKSELECTITEM			= 1098,
		ACTION_TEAM_BROADCAST		= 1101,			// ���ڶ���Ƶ���й㲥һ����Ϣ��param=��Ϣ��
		ACTION_TEAM_ATTR			= 1102,			// ���������Ա���ԡ�
													// param="field opt data", 
													// field="money"(+=��<��>��==), 
													// field="level"(<��>��==), 
													// field="count"(���ӳ��Ķ�Ա������<��==), 
													// field="count_near"(���ӳ��Ķ�Ա����������ͼ�У�������ţ�<��==), 
													// field="mate"(ֻ��Ҫfield���������), 
													// field="friend"(ֻ��Ҫfield���������), 
		ACTION_TEAM_CHK_ISINTEAM	= 1112,	
		ACTION_TEAM_ISTUTOR			   = 1114,		//�Ƿ����ʦͽ��ϵ// pengfeng [10/26/2007] 
		ACTION_TEAM_ACTION_DO		   = 1116,
		ACTION_TEAM_ACTIONFORMR_DO	   = 1118,
		ACTION_TEAM_SAME_IP			   = 1119,		//�ж϶������Ƿ���N����IP��ͬ
		ACTION_TEAM_SAMEIP_SELF		   = 1120,		//�ж϶��������Ƿ���N����IP���Լ���ͬ
		ACTION_TEAM_DIFFENT_IP		   = 1121,		//�ж϶������Ƿ���N����IP��ͬ	
		ACTION_TEAM_CHK_ISLEADER	= 1501,			// ����Ƿ�Ϊ�ӳ�������Ҫ����
		ACTION_USER_LIMIT			= 1999,
		
		// ghost gem magic part  ----------------------------------------------------------------------------------
		ACTION_MAGIC_FIRST			= 4000,
		ACTION_MAGIC_ATTACHSTATUS	= 4001,			// ׷��״̬��szParam="status power secs times"
		ACTION_CHECK_BUF            = 4004,         // ������Ϸ��ϵ�BUF [1/29/2008 �½���]
		ACTION_DETACH_BUF           = 4005,          // ���ĳ���̶���BUF [1/29/2008 �½���]
		ACTION_DUBLEEXP_FREE        = 4006,
		ACTION_DUBLEEXP_LOCK        = 4007,
		ACTION_DUBLEEXP_CHECK       = 4008,
		ACTION_CHECK_MAGICANDLEV	= 4009,
		ACTION_MAGIC_LIMIT			= 4099,

		ACTION_SCRIPT_FIRST		= 6000,			//(���ʺŷ���������Ϣ)
		ACTION_SCRIPT_PRESENT	= 6001,			//�ű�����
		ACTION_SCRIPT_LIMIT		= 6999,			//
};


