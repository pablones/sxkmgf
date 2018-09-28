// action list
// paled, 2003.1.25

////////////////////////////////////////////////////////////////////////
// "%%" 表示可显示的 "%" 符号
// "%nnXXX" 表示显示宽度，一律左对齐。超长会按实际长度显示。(例id为1234的NPC的"%6id"显示为"1234  ")

// 系统部分 //////////////////
const char PARAM_DATE_STAMP[]			= "date_stamp";					// 仅用于计算
const char PARAM_TIME[]					= "time";						// 仅用于计算
const char ACCEPT_STR_[]				= "accept";						// 客户端上传的一个子串 // %accept0 ~ %accept3

// ITEM部分 //////////////////
const char	PARA_ITEM_TYPE[]			= "item_type";
const char	PARA_ITEM_DATA[]			= "item_data";

// NPC部分 //////////////////
const char	PARA_DATASTR[]				= "datastr";
const char	PARA_DATA_[]				= "data";							// %data0 ~ %data3
const char	PARA_NAME[]					= "name";							// npc name
const char	PARA_NPC_ID[]				= "id";								// npc id in database
const char	PARA_NPC_X[]				= "npc_x";
const char	PARA_NPC_Y[]				= "npc_y";
const char	PARA_NPC_OWNERID[]			= "npc_ownerid";

// USER部分 ////////////////
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
const char	PARA_TUTOREXP_[]			= "tutor_exp";						// 导师经验
const char	PARA_STUDENT_EXP_[]			= "student_exp";					// 徒弟贡献的经验
const char	PARA_EXPLOIT_[]				= "exploit";						// 功勋值
//帮派部分
const char	PARA_AVAILABLE_FUND[]		= "available_fund";					// 提取帮派的可以分配基金
// user 的 iter 部分 ////////////////
const char	PARA_ITER_VALUE[]			= "iter_value";
const char	PARA_ITER_SYN_NAME[]		= "iter_syn_name";
const char	PARA_ITER_SYN_LEADER[]		= "iter_syn_leader";
const char	PARA_ITER_SYN_MONEY[]		= "iter_syn_money";
const char	PARA_ITER_SYN_AMOUNT[]		= "iter_syn_amount";
const char	PARA_ITER_SYN_FEALTY[]		= "iter_syn_fealty";
const char	PARA_ITER_MEMBER_NAME[]		= "iter_member_name";				// %iter为玩家ID，返回玩家名字
const char	PARA_ITER_MEMBER_RANK[]		= "iter_member_rank";				// %iter为玩家ID，返回玩家等级称号
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

//---任务系统的iter---begin
const char	PARA_ITER_TASK_USERNAME[]		= "iter_task_username";		//返回玩家的cq_user表中的name字段
const char	PARA_ITER_TASK_COMPLETENUM[]	= "iter_task_completenum";  //对任务完成次数取值，返回任务完成次数
const char	PARA_ITER_TASK_BEGINTIME[]		= "iter_task_begintime";    //对任务开始时间取值，返回任务开始时间
//---任务系统的iter---end

// 资源战部分 [8/28/2007 %PENGFENG%]
const char PARA_DRAGON_STONE_NUM[]		    = "dragon_stone_num";		//返回当前龙族获得的时空碎片个数
const char PARA_HUMAN_STONE_NUM[]			= "human_stone_num";		//返回当前人族获得的时空碎片个数
const char PARA_GENE_STONE_NUM[]			= "gene_stone_num";			//返回当前基因获得的时空碎片个数
// 资源战部分END [8/28/2007 %PENGFENG%]

// 下线泡点部分 [9/21/2007 Administrator]
const char PARA_SAVEPOINT_THISTIME_NUM[]	= "savepoint_thistime_num";		//返回本次下线修练积累经验
const char PARA_SAVEPOINT_ALLNUM[]			= "savepoint_allnum";			//返回积累经验总值
const char PARA_SAVEPOINT_RESIDUAL_TIME[]	= "savepoint_residual_time";	//返回剩余可下线修炼时间
// end [9/21/2007 Administrator]

////////////////////////////////////////////////////////////////////////
enum {
		// system part, need user ptr --------------------------------------------------------------------------
		ACTION_SYS_FIRST			= 100,
		ACTION_MENUTEXT				= 101,			// 菜单文本。data:显示行数(缺省为0), param="text"(可选): 显示文本，此类型可包含空格，也可为空行。
		ACTION_MENULINK				= 102,			// 菜单超链接。"text task_id align": align(可选): 对齐模式(缺省/0:左对齐; 5:居中; 9:右对齐; a(1-9)b(2-8):不换行,从a/b行宽处开始显示,9为右对齐)
		ACTION_MENUSHOW				= 103,			// 菜单输入框。"len task_id text": len:可输入的长度；text(可选)：显示的文字，align：对齐模式(缺省：文字在左，正整数：输入框嵌入在文字的第几字符处)。★说明：如有多个输入框，由界面用圆按钮区分，每次只上传一个。
		ACTION_RAND					= 121,			// 检测随机率。"data1 data2"。"10 100"表示有1/10的机会是true。
		ACTION_RANDACTION			= 122,			// 随机Action "action0 action1... action7"共8个，随机挑一个执行
		ACTION_CHKTIME				= 123,			// data为时间类型 0-检查当前服务器详细时间 "%d-%d-%d %d:%d %d-%d-%d %d:%d"; 1-检查年某天时间"%d-%d %d:%d %d-%d %d:%d", 2-检查月某天时间"%d %d:%d %d %d:%d", 3-检查周某天时间"%d %d:%d %d %d:%d", 4-检查日时间"%d:%d %d:%d", 5-检查小时时间"%d %d"(每个小时的第几分到底几分)
		ACTION_ERRORMESSAGEBOX		= 127,			// 让客户端弹出错误操作提示对话框20090410haocq添加
		ACTION_CHATSYSBOX			= 128,          // 系统1:1聊天框
		ACTION_SYS_LIMIT			= 199,

		// npc part, need npc ptr --------------------------------------------------------------------------
		ACTION_NPC_FIRST			= 200,
		ACTION_NPC_ERASE			= 205,			// 删除当前NPC。限动态NPC。注意：删除后，不要再对此NPC进行操作。dwData不为0，表示删除本地图的所有type为dwData的NPC。param="idMap type": 删除指定地图的某些NPC。
		ACTION_NPC_ISTTAG			= 241,
		ACTION_NPC_LIMIT			= 299,		

		// map part, need curr map --------------------------------------------------------------------------
		ACTION_MAP_FIRST			= 300,
		ACTION_MAP_MOVENPC			= 301,			// 把npc移动到指定的地图、位置(仅用于固定NPC)，data 为指定的npc的ID, param为 "idMap nPosX nPosY"。注意：可移动到该地图的(0,0)坐标，以便隐藏NPC。
		ACTION_MAP_MAPUSER			= 302,			// 判断指定地图中的用户人数, data 为指定地图ID, param为 "cmd opt data",
													//  cmd支持"map_user"和"alive_user", opt可以为"==, <=, >="，data为数量
		ACTION_MAP_BROCASTMSG		= 303,			// 广播消息, data 为地图id, szParam为广播的消息
		ACTION_MAP_LIMIT			= 399,

		// item action only part, need pUser+pItem ptr --------------------------------------------------------------------------
		ACTION_ITEMONLY_FIRST		= 400,
		ACTION_ITEM_DELTHIS			= 498,			// 删除当前任务物品。注意：必须为最后一个ACTION。
		ACTION_ITEMONLY_LIMIT		= 499,

		// user item part, need user ptr --------------------------------------------------------------------------
		ACTION_ITEM_FIRST			= 500,		ACTION_ITEM_CHECK			= 503,			// 检测物品。data=itemtype_id, param不为0时，同时检查物品的数量(或耐久度)，此物品必须达到要求的数量(或耐久度)才行。或者data为0，param表示要找的物品名。
		ACTION_ITEM_HOLE			= 504,			// 武器打洞。param 支持 "ChkHole HoleNum" 或者"MakeHole HoleNum"，Num为1或者2
		ACTION_ITEM_LEAVESPACE		= 508,			// 检查背包剩余空间。param="space weight packtype"
													// 其中packtype的取值范围为50 ~ 53
													// 50：普通物品背包
													// 51：魔魂宝石背包
													// 52：幻兽蛋背包
													// 53：幻兽背包
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
		// 帮派Action整理
		ACTION_SYN_CREATE			= 701,			// 创帮，玩家要输入帮派名字。param="level money leave_money", 三个参数为需要的玩家等级，需要的现金数，帮派成立后所余现金数。
		ACTION_SYN_DESTROY			= 702,			// 解散。执行Action的玩家为团长、分团长、分队长的时候，分别表示解散帮派、分团、分队
		ACTION_SYN_DONATE			= 703,			// 捐款，需要一个输入框。
		ACTION_SYN_ATTR				= 717,			// 检查和修改帮派属性，参数不小于3个，缺省帮派ID是当前玩家的帮派ID。
													// param="szField szOpt data syn_id", szField可选：
													// 基金："money"	(opt可选"+=", "<"), 
													// 声望："repute"	(opt可选"+=", "<"),
													// 人数："membernum"(opt为"<"),
													// 父帮派："fealty"	(opt为"=="),
													// 等级："level"	(opt可选"=", "+=", "<", "==")
													// 20070413修罗:军团贡献度 "synproffer" (opt可选"=", "+=", "<",">", "==")
													// 20070416修罗:个人贡献度 "userproffer" (opt可选"=", "+=", "<",">", "==")
													// 20070421修罗:个人贡献等级 "userprofferlev" (opt可选"<",">", "==")
													// 20070416修罗:个人军团任务掩码.周 "taskMaskWeekBit" (opt可选"==", "=")
													//								比特位.对应[判断为真].[赋为真]操作
													//								data:1~31.32表示非法或无效
													//	个人军团任务掩码.天..."taskMaskDayBit".同上

		ACTION_SYN_CHK_SYN			= 732,			// 20070409修罗:当前玩家是否有军团

		//----------------------师徒系统--------------------------------------------
		ACTION_TUTOR_GRADUATE		        =746,       // 师父带徒弟申请出师 [陈剑飞10/26/2007]
		ACTION_TUTOR_TEACHER                =748,      // 判断是否为师傅 [陈剑飞10/26/2007]
		ACTION_TUTOR_STUDENT                =749,      // 判断是否为徒弟 [陈剑飞10/26/2007]
		
		ACTION_TUTOR_TEAM_ISSTU				=750,		// goto 队友是不是徒弟
		ACTION_TUTOR_TEAM_STULEV			=751,		// goto 判断队伍中徒弟等级
		ACTION_TUTOR_DIVORCE				=752,		// goto 协议解除师徒关系,给徒弟发送确认框
		ACTION_TUTOR_HASTUTOR				=753,		// goto 判断是否有师父
		ACTION_TUTOR_HASSTUDENT				=754,		// goto 判断是否有徒弟
		ACTION_TUTOR_STUREBEL				=755,		// goto 徒弟叛师
		ACTION_TUTOR_TEAM_HASTUTOR			=756,		// goto 师父踢出徒弟
		
		ACTION_TUTOR_ITEMWITHNAME			=757,		// goto 给予玩家刻有名字的特殊道具
		ACTION_TUTOR_KICKOUTMSG				=758,		// 师父要踢出徒弟，给客户端徒弟数据
		ACTION_TUTOR_REBELWIN				=759,		// 徒弟叛师前的确认框
		ACTION_TUTOR_TUTORNAME_ITEM			=760,		// 生成刻有师父名字的物品
		ACTION_TUTOR_STUNAME_ITEM			=761,		// 生成刻有队伍中徒弟名字的物品
		ACTION_TUTOR_ADDSTUBUF				=762,		// 给队伍中的徒弟加Buf
		ACTION_TUTOR_STUAROUND				=763,		// 队伍中徒弟是否都在附近
		ACTION_TUTOR_ITEMWITHPOS			=764,		// 生成带坐标的物品 坐标记录在itemAddition1,itemAddition2,itemAddition3里
		ACTION_TUTOR_CREATEMONSTER			=765,		// 在指定的地点刷怪
		ACTION_TUTOR_GRADUATEWIN			=766,		// 告诉徒弟确认出师
		ACTION_TUTOR_TEAMADD				=767,		// 组队拜师,给徒弟弹确认框
		ACTION_SYN_LIMIT					= 799,

		// monster part --------------------------------------------------------------------------
		ACTION_MST_FIRST			= 800,
		ACTION_MST_LIMIT			= 899,		

		// user attr part --------------------------------------------------------------------------
		ACTION_USER_FIRST			= 1000,
		ACTION_USER_ATTR			= 1001,			// 玩家属性的修改和检查。"attr opt data"。attr可选择
													//		"life"(+=,==,<)、
													//		"mana"(+=,==,<)、
													//		"money"(+=,==,<)、
													//		"exp"(+=,==,<)、
													//		"pk"(+=,==,<)、
													//		"profession"(==, set, >=, <=)、
													//		"level"、(+=,==,<)、
													//		"force"、(+=,==,<)、
													//		"dexterity"、(+=,==,<)
													//		"speed"、(+=,==,<)、
													//		"health"、(+=,==,<)、
													//		"soul"、(+=,==,<)、
													//		"rank"、(==,<)、//20070616修罗:syn_rank
													//		"rankshow"、(==,<)、
													//		"iterator"、(=, <=, +=, ==)、
													//		"crime" (==, set)
													//		"gamecard"(==, >=, <=)
													//		"gamecard2"(==, >=, <=)
													//		"xp"(+=)
													//		"metempsychosis"(==, <)
//													//      "nobility_rank"(==, <, +=, =)	// 
													//		"mercenary_rank"(==, <, +=)		// 佣兵等级
													//		"mercenary_exp"(==, <, +=)		// 佣兵经验
													//		"exploit"(==, <, +=)			// 功勋值
													//		"maxlifepercent"(+=,==,<)		// 最大生命千分比 
													//		"maxlifevalue"(+=,==,<)			// 最大生命千分比——转换为具体数值计算
													//		"turor_exp"(==,<,+=,=)
													//		"tutor_level"(==,<,+=,=)
													//		"syn_proffer"(<,+=,=)	//帮派贡献度
													//		"maxeudemon"(<,+=,=)	//最大可召唤幻兽数
													//		"packcount"(<,+=,=)     //用户背包数
													//		"packrow_count"(<,+=,=) //开放的背包总排数08.9.27独孤求败
													//20070616修罗:
													//		"honor"(+=,-=,==,<,>,>=,<=) 荣誉值
													//		"rank" (==,<,>,>=,<=) 军阶点
													//		"rankLev" (==,<,>,>=) 军阶级别
													//		"present" (+=,==,>=) 地推礼品装备
		ACTION_USER_FULL			= 1002,			// 将玩家的属性加满。"attr"。attr可选"life","mana"
		ACTION_USER_CHGMAP			= 1003,			// 切地图 param "idMap nPosX nPosY bPrisonChk", bPrisonChk为可选参数，缺省不可以出监狱，设置为1可以出
		ACTION_USER_RECORDPOINT		= 1004,			// 存记录点 param "idMap nMapX nMapY"
		ACTION_USER_RESETPOINT		= 1005,			// "color num"
													// "style num"
		ACTION_USER_CHECKWEAPENSOUL	= 1007,			//是否有魂魄
	    ACTION_USER_INSTANCETIMES_OPERATE   = 1013,  //用户副本计数操作
	    ACTION_USER_RANDSHOPFULL     = 1014,
		ACTION_USER_MAGIC			= 1020,			// 检查法术。param可以为：
													// "check type"(玩家学过type类型的法术),
													// "check type level"(玩家学过type类型的法术，且等级为level级),
													// "learn type"(玩家学会type类法术，等级为0)，
													// "uplevel type"(玩家的type类法术升一级)
													// "addexp type exp"(玩家的type类法术增加exp点经验)
		ACTION_USER_CHECKINSTANCE	= 1021,			// "check type level", 检查武器技能的类型和等级，是否>=等级
													// "learn type level", 学习指定类型和等级的技能
		ACTION_USER_LOG				= 1022,			// 保存指定信息到gm log并带入触发者信息（名字和id）,信息指定在param中
													// 比如 "%s 完成天剑任务并获得宝石", param中%s就是保存触发者信息的位置
		ACTION_USER_SEX				= 1026,			// 性别检查, 男返回1，女返回0
		ACTION_USER_OPEN_DIALOG		= 1046,			// 通知客户端打开一个界面。data=idDialog。param="task_id0 task_id1 task_id2 task_id3 ..."，可以无param，最多20个task_id, task_id不能为0, 允许客户端选择下一个TASK。无param时，客户端只能上传“客户端可触发的TASK”。有param时，cq_task.client_active必须为0。
		ACTION_USER_CHGMAP_REBORN	= 1047,			// 切屏到复活点。
		ACTION_USER_CHK_ITEMAMOUNT	= 1050,
		ACTION_USER_TAKESTUDENTEXP	= 1051,			// 提取徒弟贡献的经验。玩家返回pszAccept指定需要提取的经验值，自动扣除导师经验。
		ACTION_USER_CHGTO_MAINMAP	= 1052,			// 到主地图的复活点复活
		ACTION_USER_CHGTO_RANDOMPOS	= 1053,			// 人物随机飞到当前地图上任意一个坐标（该点不能有掩码）
		ACTION_USER_ADD_TIME_STATUS = 1062,		//  [12/28/2007 朱斌]	此action的功能为给玩家添加一个magictype（魔法、BUFF）,该buff会在"清除时刻"消失。
		ACTION_USER_CHK_IN_RANGE	= 1065,		//  [1/27/2008 朱斌]	新增action，可以判断玩家所处的地图坐标
		ACTION_SEND_NOTICE			= 1069,			// 全服公告
		//---任务系统，记录任务的细节---begin
		ACTION_USER_DEPOT_CODE      = 1070,
		ACTION_USER_ADDEUDEMON			= 1075, 
		ACTION_USER_EUDEMON_DEAD		= 1077,
		ACTION_USER_EUDEMON			= 1079,
		ACTION_USER_TASK_MANAGER	= 1080,
		ACTION_USER_CHECKINSFINISHTIME	= 1081,
		ACTION_USER_SETJOB				= 1096,
		ACTION_USER_DELCURNPC			= 1097,
		ACTION_TASKSELECTITEM			= 1098,
		ACTION_TEAM_BROADCAST		= 1101,			// 向在队伍频道中广播一条消息。param=消息。
		ACTION_TEAM_ATTR			= 1102,			// 检查或操作队员属性。
													// param="field opt data", 
													// field="money"(+=、<、>、==), 
													// field="level"(<、>、==), 
													// field="count"(含队长的队员数量，<、==), 
													// field="count_near"(含队长的队员数量，本地图中，必须活着，<、==), 
													// field="mate"(只需要field，必须活着), 
													// field="friend"(只需要field，必须活着), 
		ACTION_TEAM_CHK_ISINTEAM	= 1112,	
		ACTION_TEAM_ISTUTOR			   = 1114,		//是否存在师徒关系// pengfeng [10/26/2007] 
		ACTION_TEAM_ACTION_DO		   = 1116,
		ACTION_TEAM_ACTIONFORMR_DO	   = 1118,
		ACTION_TEAM_SAME_IP			   = 1119,		//判断队伍中是否有N个人IP相同
		ACTION_TEAM_SAMEIP_SELF		   = 1120,		//判断队伍中有是否有N个人IP与自己相同
		ACTION_TEAM_DIFFENT_IP		   = 1121,		//判断队伍中是否有N个人IP不同	
		ACTION_TEAM_CHK_ISLEADER	= 1501,			// 检查是否为队长，不需要参数
		ACTION_USER_LIMIT			= 1999,
		
		// ghost gem magic part  ----------------------------------------------------------------------------------
		ACTION_MAGIC_FIRST			= 4000,
		ACTION_MAGIC_ATTACHSTATUS	= 4001,			// 追加状态，szParam="status power secs times"
		ACTION_CHECK_BUF            = 4004,         // 检查身上符合的BUF [1/29/2008 陈剑飞]
		ACTION_DETACH_BUF           = 4005,          // 清除某个固定的BUF [1/29/2008 陈剑飞]
		ACTION_DUBLEEXP_FREE        = 4006,
		ACTION_DUBLEEXP_LOCK        = 4007,
		ACTION_DUBLEEXP_CHECK       = 4008,
		ACTION_CHECK_MAGICANDLEV	= 4009,
		ACTION_MAGIC_LIMIT			= 4099,

		ACTION_SCRIPT_FIRST		= 6000,			//(向帐号服务器发消息)
		ACTION_SCRIPT_PRESENT	= 6001,			//脚本服务
		ACTION_SCRIPT_LIMIT		= 6999,			//
};


