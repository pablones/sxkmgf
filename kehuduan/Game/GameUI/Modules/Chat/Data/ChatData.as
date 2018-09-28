package GameUI.Modules.Chat.Data
{
	import flash.geom.Point;
	import flash.text.StyleSheet;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	
	public class ChatData
	{
		public function ChatData()
		{
			
		}
		
		/** 聊天是否在显示 */
		public static var chat_view_is_show:Boolean = true;
		
		/** 玩家身份 */
		public static const USER_INDENTITY:Array = [
													"",
													"<3_["+GameCommonData.wordDic[ "mod_chat_data_USER_INDENTITY_1" ]+"]_15>",//"新手指导员"
													"<3_["+GameCommonData.wordDic[ "mod_chat_data_USER_INDENTITY_2" ]+"]_8>",//"游戏管理员"
													"",
													"",
													""
													];
		/** 玩家性别 */
		public static const USER_SEX:Array = ["<3_♂_3>", "<3_♀_6>"];
		
		/** 玩家线路 */
		public static const USER_LINE_NAME:Array = ["", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_1" ]+"]", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_2" ]+"]", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_3" ]+"]", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_4" ]+"]", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_5" ]+"]", "[" + GameCommonData.wordDic[ "mod_cha_dat_chg_get_6" ] + "]", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_7" ]+"]", "["+GameCommonData.wordDic[ "mod_cha_dat_chg_get_8" ]+"]"]; 
		//"一线"  "二线" "三线" "四线" "五线" "专线" "七线" "八线"
		/** 活动公告计时器 */
		public static var noticeTimer:Timer = new Timer(80000, 1);
		
		/** 聊天颜色 */
		public static var CHAT_COLORS:Array = null;
//		[
//													0xFFFFFF,	//0-白  (所有的表示自己的 “你” 字都是白色)
//													0xFFFFFF,	//1-白
//													0x00FF00,	//2-绿
//													0x0098FF,	//3-蓝
//													0x9727FF,	//4-紫
//													0xFF6532,	//5-橙
//													0xCC66FF,	//6-粉
//													0xFFFF00,	//7-黄  (附近频道 [附近]是白色， 人名字是黄色)
//													0xFF0000,	//8-红 
//													0x03CDDA,	//9-浅蓝（世界聊天头）
//													0xFF6633,	//10-浅黄（帮派聊天头） 	原 0xFFF54F
//													0xFCB68B,	//11-米色（组队聊天头）
//													0xFF4A79,	//12-浅粉（私聊聊天头）
//													0x00A651,	//13-提示绿（用于每5分钟发一次的小提示）
//													0xFF3232,	//14-提示高亮（用于每5分钟发一次的小提示）
//													0xFF00FF	//15-淡紫 （新手指导员文字颜色）
//													0xFF32CC  //16-阵营颜色：贪狼
//													0x00CBFF  //17-阵营颜色：破军
//													0xE08E1F  //18-阵营颜色：七杀
//													];
		/** 最大聊天数量 */
		public static const MAX_AMOUNT_MSG:int = 30;
		
		/** 临时聊天内容(显示) */
		public static var tmpChatInfo:Array = [];
		
//		/** 临时发送字符串 */
//		public static var tmpChatSend:String = "";
		
//		/** 临时聊天内容(发送) */
//		public static var tmpChatSend:Array = [];
		
		/** 物品快捷字符串 */
		public static var tempItemStr:String = "";
		
		/** 小喇叭物品快捷字符串 */
		public static var tempItemStrLeo:String = "";
		
		/**   */
		public static var chatItemIds:Array = [];		//聊天中物品编号数组   点击物品链接时 进行检验如果物品ID已经存在则不去查询服务器
		
		/** FaceText  SetHtmlText */
		public static var HtmlStyle:StyleSheet = new StyleSheet();
		
		/** FaceText  setNameLink */
		public static var NameStyle:StyleSheet = new StyleSheet();
		
		public static const ITEM_COLORS:Array = ["#ffffff", "#00ff00", "#0098FF", "#9727ff", "#FF6532"];

		public static var curSelectModel:int = 0;
		
		public static var channelModel:Array = [
												{label:"<font color='#ffffff'>["+GameCommonData.wordDic[ "mod_chat_data_channelModel_1" ]+"]</font>:"+GameCommonData.wordDic[ "mod_chat_data_channelModel_2" ],name:GameCommonData.wordDic[ "mod_chat_data_channelModel_1" ], channel:2013, rece:"ALLUSER", color:"#ffffff"},
												//"附"	 "附近信息"
												/* {label:"[队]:组队信息",name:"队", channel:2003, rece:"ALLUSER"},  */
												undefined,
												{label:"<font color='#03CDDA'>["+GameCommonData.wordDic[ "mod_chat_data_channelModel_3" ]+"]</font>:"+GameCommonData.wordDic[ "mod_chat_data_channelModel_4" ], name:GameCommonData.wordDic[ "mod_chat_data_channelModel_3" ], channel:2016, rece:"ALLUSER", color:"#03CDDA"},
												//"世"	"世界信息"
												/* {label:"[帮]:帮派信息",name:"帮", channel:2004, rece:"ALLUSER"},  */
												undefined,
												undefined,	//主职业频道
												undefined	//副职业频道
		/* 										{label:"<font color='#ff00ff'>[密]</font>:王康 ",name:"密", channel:2001, rece:"王康",color:0xff00ff}; */
												];					
		
		/** 自定义频道位 */
		public static var channelSign:uint = 0;
		
		//自定义频道列表1										
		public static var Set1ChannelList:Array = [	{channel:2016, value:false},		//false 世界信息0
													{channel:2004, value:false},		//false 帮会信息1
													{channel:2003, value:true},			//true  组队信息2
													{channel:2013, value:false},		//false 附近信息3
													{channel:2041, value:false},		//false 门派信息4	主职业
													{channel:2037, value:false},		//false 帮助信息5        后来改的帮助频道
													{channel:2005, value:false}, 		//false 这个是后来改的，也是发的系统信息10
													{channel:1007, value:true},			//true  个人信息7																															
													{channel:2001, value:true},			//true  私聊8
													{channel:2035, value:false},		//false 系统公告9
													{channel:2036, value:false},		//false 滚屏公告9
													{channel:4001, value:true},			//true  提示 频道
													{channel:2046, value:true},			//true  竞技场阵营频道
													
//													{channel:1006, value:false},		//帮助信息5       这个是以前的
//													{channel:2014, value:false}, 		//系统信息      这个本来是排在第六位滴
												];
		//自定义频道列表2
		public static var Set2ChannelList:Array	= [	{channel:2016, value:false},		//false 世界信息0
													{channel:2004, value:false},		//false 帮会信息1
													{channel:2003, value:true},			//true  组队信息2
													{channel:2013, value:false},		//false 附近信息3
													{channel:2041, value:true},			//true  门派信息4  主职业
													{channel:2037, value:false},		//false 帮助信息5        后来改的帮助频道
													{channel:2005, value:false}, 		//false 这个是后来改的，也是发的系统信息	现在排6	
													{channel:1007, value:false},		//false 个人信息7											
													{channel:2001, value:true},			//true  私聊	8
													{channel:2035, value:false},		//false 系统公告	 9
													{channel:2036, value:false},		//false 滚屏公告	 9
													{channel:4001, value:true},			//true  提示 频道
													{channel:2046, value:true},			//true  竞技场阵营频道
													
//													{channel:1006, value:true},			//帮助信息5       这个是以前的   原本排在第五位
//													{channel:2014, value:false} 		//系统信息6     原本是排在第六位
												]; 										
		
		/** 帮助小提示 */
		public static var NOTICE_ARR:Array = null;
		
//		[
//									/* 0 */		"如果您感觉当前线路比较繁忙，可以尝试切换到其他线路。",
//									/* 1 */		"当同一屏幕上在线玩家较多时，您可以选择按下<3_F12_14>键屏蔽其他玩家。",
//									/* 2 */		"打到自己不需要的装备时，可以选择卖给NPC或者与其他玩家进行交易。",
//									/* 3 */		"当你的级别到了<3_45级_14>，你可以选择加入第二门派继续您的江湖之旅。",
//									/* 4 */		"在御剑江湖中，每天都有各种丰富多彩的活动任务等待着您去参与，尝试与他人组队一起参加吧！",
//									/* 5 */		"去开封找<3_阿九_9>传送到宠物林就可以捉属于自己的宝宝了。",
//									/* 6 */		"将您的装备升星、强化、镶嵌宝石，能使您变得更加强大。",
//									/* 7 */		"双倍经验可以在江陵、开封的双倍经验NPC处冻结和解除冻结。",
//									/* 8 */		"当您的级别每提升到相应的级数，别忘了打开<3_新手小礼包_14>和<3_新手大礼包_14>，里面有为您精心配制的物品和装备。",
//									/* 9 */		"在提升人物等级的同时，别忘了提升您的职业等级。",
//									/* 10 */	"宠物出战和死亡会降低快乐值，当快乐值低于60时，您的宠物将无法出战。",
//									/* 11 */	"在江陵、开封的固定区域内，您可以进行开店摆摊。",
//									/* 12 */	"使用VIP中的<3_小飞鞋_14>功能，可以减少您在场景移动的时间，让您的行动更加便捷！",
//									/* 13 */	"做日常任务时，您可以选择<3_挂机_14>来完成任务中所需的打怪和道具获取。",
//									/* 14 */	"装备有5种颜色，从差到好分别是白色、绿色、蓝色、紫色、橙色。",
//									/* 15 */	"装备有5种前缀，从差到好分别是优秀、杰出、卓越、完美、神圣。",
//									/* 16 */	"当职业等级达到25级后，每提升一级，就会有潜力值供您进行分配。",
//									/* 17 */	"当您的血气减少时，可以回城找<3_郎中_9>进行血气补充。",
//									/* 18 */	"加入一个帮派，可以增加您的人脉关系，也可以更方便寻求帮助！",
//									/* 19 */	"按下<3_空格键_14>，可以自动捡取宝箱内的掉落物品。",
//									/* 20 */	"轻轻松松老板键，只要你连续按下<3_两次Z键_14>，就可以隐藏游戏窗口。",
//									/* 21 */	"<3_橙色_5>名字的怪不会主动攻击你，<3_红色_8>的会主动攻击，千万小心！",
//									/* 22 */	"行侠江湖，切记不要中了狡诈小人的圈套。《御剑江湖》提醒您，不要相信任何非官方通道的充值手段，防人之心不可无！"
//												]; 
		
		/** 欢迎公告 */
		public static var WELCOME_ARR:Array = null;
		
		/** 上一次随机数，5分钟小提示 */
		public static var lastPlayIndex:int = -1;
		
		/** 帮助小提示的播放间隔 */
		public static var NOTICE_HELP_INTERVAL:Number = 0;
		
		/** 欢迎公告的播放间隔 */
		public static var WELCOME_INTERVAL:Number = 0;
		
//		[
//												"<3_欢迎来到4399《御剑江湖》游戏世界！官方游戏交流QQ群：85766792，124971536 诚邀您的加入！_8>",
//												"<3_《御剑江湖》双线二服火热开启，各种精彩活动等着您参加，各种游戏道具让您拿到手软。_8><4_请点击查看_0x00FF00_0_http://web.4399.com/yjjh/xinwenzhongxin/huodong/201010/30-34048.html>",
//												"<3_限时送大礼！限定时间内完成首次充值，即可100%获得绝版橙色戒指、绝版超萌小白虎、绝版人形BOSS宠！_8><4_请点击查看_0x00FF00_0_http://web.4399.com/yjjh/xinwenzhongxin/huodong/201010/30-34048.html#5>"
//												];  
//		

//		"<3_天上掉馅饼，《御剑江湖》终极删档内测10万Q币大赠送！_8><4_点此了解详情_0x00FF00_0_http://bbs.youjia.cn/thread-261929-1-1.html>"
//		天上掉馅饼，《御剑江湖》终极删档内测10万Q币大赠送！点此了解详情
//
//能不能在本次测试里加个这个公告？刷频繁一些
//项目经理-杨路(4125654)  10:33:22
//和Q群一起刷频繁些
//项目经理-杨路(4125654)  10:33:49
//Q币链接是： http://bbs.youjia.cn/thread-261929-1-1.html
//		public static var welcomeQB:String = "<3_天上掉馅饼，《御剑江湖》终极删档内测10万Q币大赠送！_8><4_点此了解详情_0x00FF00_0_http://bbs.youjia.cn/thread-261929-1-1.html>";
		
		
		public static var CurAreaPos:int 	= 0;
												
		public static var MsgPosArea:Array	= [{width:328,height:120,x:0,y:413},
											   {width:328,height:162,x:0,y:371},
											   {width:328,height:200,x:0,y:333}
												];		
		
		//当前显示的内容
		public static var CurShowContent:int = 0;
		
		public static var SelectedLeoColor:uint = 0xffffff;
		public static var SelectedMsgColor:uint = 0xffffff;
		
		//										
		public static var ColorIsOpen:Boolean= false;	
		public static var CreateChannelIsOpen:Boolean = false;
		public static var FilterListIsOpen:Boolean = false;
		/** 小喇叭是否打开 */	
		public static var SetLeoIsOpen:Boolean = false;	
		/** 大喇叭是否打开 */	
		public static var SetBigLeoIsOpen:Boolean = false;	
		public static var SelectChannelOpen:Boolean = false;	
		public static var QuickChatIsOpen:Boolean = false;	
		//记录创建频道面板的位置
		public static var tmpCreatePoint:Point = new Point(50, 100);		
		//记录屏蔽列表的位置
		public static var tmpFilterPoint:Point = new Point(50, 100);
		//记录小喇叭的位置
		public static var tmpLeoPoint:Point = new Point(50, 100);	
		
		//信息内容
		public static var AllMsg:Array = new Array(); //综合信息
		public static var SystemMsg:Array = new Array();//系统
		public static var Set1Msg:Array = new Array();//自定义2	
		public static var Set2Msg:Array = new Array();//自定义1
		public static var ScrollNotice:Array = new Array(); //滚屏公告
		//屏蔽列表
		public static var FilterList:Array = new Array();	
		
		//颜色
//		public static var WorldColor:String = "#03cdda";		
//		public static var UnitColor:String = "#fff54f";
//		public static var TeamColor:String = "#fcb68b";
//		public static var MapColor:String = "#ffffff";	
//		public static var PrivateColor:String = "#ff4a79";	
//		public static var NoticeColor:String = "#ff0000";
		//世界消息：30秒，其他消息：3秒
		public static var worldBool:Boolean = true;
		public static var otherBool:Boolean = true;
		//输入框处在焦点中
		public static var txtIsFoucs:Boolean = false;
		//打开滚屏公告
		public static const OPENSCROLLNOTICE:String = "OPENSCROLLNOTICE";	
		//GM道具 大喇叭
		public static const USE_BIG_LEO:String = "USE_BIG_LEO";
		//滚屏是否还在运动
		public static var isGun:Boolean = false;
		//滚屏更新帧频
		public static var UPDATAGUN:String = "UPDATAGUN";
		
		/** 防沉迷地址 */
		public static var FAT_URL:String = "";		//"http://web.4399.com/user/userinfo.php";
		/** 防沉迷说明 */
		public static var FATIGUE_STR:String = "";	//"<font color='#00ff00'>    如果您已满18周岁，请及时登录4399通行证管理中心</font><font color='#ff0000'><a href='"+FAT_URL+"' target='_blank'>（"+FAT_URL+"）</a></font><font color='#00ff00'>完善您的身份信息即可不被纳入防沉迷系统保护。</font>";
		/** 官网地址 */
		public static var OFFICIAL_WEBSITE_ADDR:String = "";
		/** 官网论坛 */
		public static var FORUM_WEBSITE_ADDR:String = "";
		/** 运营商编号 */
		public static var SERVICE_BUSINESS_ID:uint = 0; 
		/** 充值地址 */
		public static var DEPOSIT_WEBSITE_ADDR:String = "";
		/** GM工具接口地址 */
		public static var GM_INTERFACE_ADDR:String = "";
		/** 领取新手卡接口地址 */
		public static var NEWER_CARD_INTERFACE_ADDR:String = "";
		/** 防沉迷平台验证库地址 */
        public static var FAT_TEST_URL:String = "";
        /** 防沉迷密钥 */
        public static var FAT_CODE:String = "";
		
		/** 游戏滚屏活动公告字典 */
		public static var GAME_SCROLL_NOTICE_DIC:Dictionary = null;
		
		/** 防骗过滤字典(外部加载) */
		public static var CHEAT_CHAT_FILTER:Array = [];  
		
//													{rep:/(元.*?宝)/ig, point:0.5},
//													{rep:/(出.*?售)/ig, point:0.5},
//													{rep:/(销.*?售)/ig, point:1.0},
//													{rep:/(经.*?销)/ig, point:1.0},
//													{rep:/(诚.*?信)/ig, point:0.8},
//													{rep:/(商.*?人)/ig, point:0.5},
//													{rep:/(代.*?理)/ig, point:0.8},
//													{rep:/(信.*?誉)/ig, point:0.5},
//													{rep:/(购.*?买)/ig, point:0.5},
//													{rep:/(咨.*?询)/ig, point:0.5},
//													{rep:/(联.*?系)/ig, point:0.5},
//													{rep:/(货.*?到)/ig, point:0.8},
//													{rep:/(付.*?款)/ig, point:0.8},
//													{rep:/(热.*?线)/ig, point:0.5},
//													{rep:/(特.*?价)/ig, point:0.5},
//													{rep:/(认.*?准)/ig, point:0.5},
//													{rep:/(=.*?100.*?RMB)/ig, point:0.8},
//													{rep:/(=.*?100.*?元)/ig, point:0.8},
//													{rep:/(=.*?200.*?RMB)/ig, point:0.8},
//													{rep:/(=.*?200.*?元)/ig, point:0.8}
		
		
	}
}


