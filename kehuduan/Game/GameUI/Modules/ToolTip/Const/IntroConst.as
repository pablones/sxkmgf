package GameUI.Modules.ToolTip.Const
{
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import flash.utils.Dictionary;
	
	public class IntroConst
	{
		public static const COLOR:uint = 0xe9ca9f; 
		
		public static const TIMER_COLOR_ARR:Array = [0x00ff00, 0xff9900, 0xff6600, 0xff0000];	//绿，浅橙，深橙，红
		
		public static const SHOWPARALLEL:String = "showparallel";		//显示装备对比信息
		
		public static var IntroDic:Array = [
				GameCommonData.wordDic[ "mod_too_con_int_int_1" ],		//0          //主要影响外攻
				GameCommonData.wordDic[ "mod_too_con_int_int_2" ],		//1		     //主要影响内攻																	
				GameCommonData.wordDic[ "mod_too_con_int_int_3" ],		//2              //主要影响血
				GameCommonData.wordDic[ "mod_too_con_int_int_4" ],	//3            //主要影响外防，内防，气
				GameCommonData.wordDic[ "mod_too_con_int_int_5" ],	//4         //主要影响命中，躲闪，暴击
				GameCommonData.wordDic[ "mod_too_con_int_int_6" ],	//5          //潜力值，可自由分配到5项基本属性中
				GameCommonData.wordDic[ "mod_too_con_int_int_7" ],	//6         //将根据你的人物等级，当前职业等级，装备等级以及技能等级情况综合评定得出，是实力的象征
				GameCommonData.wordDic[ "mod_too_con_int_int_8" ],	//7            //绑定且无法流通的碎银，主要用于学习技能和日常的消耗，可以通过任务等大量获得。
				GameCommonData.wordDic[ "mod_too_con_int_int_9" ],	//8            //正常流通并且可以进行交易的银两，可以通过特定的活动获得
				GameCommonData.wordDic[ "mod_too_con_int_int_10" ],	//9              //在商城中使用的元宝，可以通过充值获得
				GameCommonData.wordDic[ "mod_too_con_int_int_11" ],	//10                  //显示/隐藏时装显示
				GameCommonData.wordDic[ "mod_too_con_int_int_12" ],//11           //当前的人物等级，25级之后可以手动升级
				GameCommonData.wordDic[ "mod_too_con_int_int_13" ],	//12                //提升人物等级所需要的经验
				GameCommonData.wordDic[ "mod_too_con_int_int_14" ],	//13           //当前的职业等级，25级之后每提升一级，可获得潜力值
				GameCommonData.wordDic[ "mod_too_con_int_int_15" ],	//14              //提升职业等级所需要的经验
				GameCommonData.wordDic[ "mod_too_con_int_int_16" ],	//15          //当前拥有的经验/可拥有经验的上限经验可以用于提升人物等级，职业等级和技能等级
				GameCommonData.wordDic[ "mod_too_con_int_int_17" ],	//16             //扩展额外的背包栏
				GameCommonData.wordDic[ "mod_too_con_int_int_18" ],		//17          //发送小喇叭喊话消息，需要消耗小喇叭道具（商城购买）
				GameCommonData.wordDic[ "mod_too_con_int_int_19" ],	//18            //选择聊天颜色
				GameCommonData.wordDic[ "mod_too_con_int_int_20" ],	//19            //设置当前频道
				GameCommonData.wordDic[ "mod_too_con_int_int_21" ],	//20            //屏蔽玩家列表
				GameCommonData.wordDic[ "mod_too_con_int_int_22" ],		//21                //清屏
				GameCommonData.wordDic[ "mod_too_con_int_int_23" ],	//22            //调整聊天框高度
				GameCommonData.wordDic[ "mod_too_con_int_int_24" ],	//23          //调整聊天框透明度
				GameCommonData.wordDic[ "mod_too_con_int_int_25" ],	//24            //切换消息类型
				GameCommonData.wordDic[ "mod_too_con_int_int_26" ],	//25              //插入表情
				"10"+GameCommonData.wordDic[ "mod_too_con_int_int_27" ], //26          //级后可以在门派接引人处入职
				"45"+GameCommonData.wordDic[ "mod_too_con_int_int_28" ], //27            //级后可以加入第二门派
				GameCommonData.wordDic[ "mod_too_con_int_int_29" ], //28       //发言
				GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"50-60"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//29          //山洞          级\n怪物分布：未知 
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"50-55"+GameCommonData.wordDic[ "mod_too_con_int_int_33" ],//30          //野外          级\n怪物分布：紫貂、玉髓羊、回鹘兵、游牧强匪
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"65-70"+GameCommonData.wordDic[ "mod_too_con_int_int_34" ],//31        //野外          级\n怪物分布：骆驼、沙漠狐、沙漠花豹、楼兰士兵
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"45-50"+GameCommonData.wordDic[ "mod_too_con_int_int_35" ],//32          //野外          级\n怪物分布：毒蝎、野狗、幽州马贼、齐国幽魂
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"25-30"+GameCommonData.wordDic[ "mod_too_con_int_int_36" ],//33         //野外          级\n怪物分布：狸猫、金国步兵、金国步兵长、金国哨兵、北宋兵卒
				GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"85-100"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//34           //山洞              级\n怪物分布：未知
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"85-90"+GameCommonData.wordDic[ "mod_too_con_int_int_37" ],//35           //野外               级\n怪物分布：流寇、白玉婷、祭奠士兵
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"80-85"+GameCommonData.wordDic[ "mod_too_con_int_int_38" ],//36         //野外           级\n怪物分布：青眼白虎、辽国巡逻兵、流窜的小贩、皇城守卫
				GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//37          //门派
				GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"20-35"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//38             //山洞            级\n怪物分布：未知
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"20-25"+GameCommonData.wordDic[ "mod_too_con_int_int_41" ],//39		    //野外           	级\n怪物分布：野猫、青鹿、游荡的剑客、武林高手	
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"15-20"+GameCommonData.wordDic[ "mod_too_con_int_int_42" ], //40            //野外            级\n怪物分布：孔雀、京城使者、建筑工匠
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"30-35"+GameCommonData.wordDic[ "mod_too_con_int_int_43" ],//41           //野外            级\n怪物分布：灰熊、云雀、银狼、昆仑山贼
				GameCommonData.wordDic[ "mod_too_con_int_int_40" ],//42        //主城
				GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//43        //门派
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"10-15"+GameCommonData.wordDic[ "mod_too_con_int_int_44" ],//44          //野外           级\n怪物分布：山猪、松鼠、青蛙、迷茫的棋手
				GameCommonData.wordDic[ "mod_too_con_int_int_40" ],//45        //主城
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"1-10"+GameCommonData.wordDic[ "mod_too_con_int_int_45" ],//46           //野外             级\n怪物分布：兔子、斑嘴鸭、土匪
				GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//47        //门派
				GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//48        //门派
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"40-45"+GameCommonData.wordDic[ "mod_too_con_int_int_46" ],//49            //野外            级\n怪物分布：石狮、泼猴、钟灵龟、八卦树精
				GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//50        //门派
				GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"40-50"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//51              //山洞               级\n怪物分布：未知
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"70-75"+GameCommonData.wordDic[ "mod_too_con_int_int_47" ],//52             //野外           级\n怪物分布：山猪王、猛虎、雷州山贼
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"55-60"+GameCommonData.wordDic[ "mod_too_con_int_int_48" ],//53          //野外          级\n怪物分布：泼猴、白族少年、白族少女、朝珠花妖、龙宫天将
				GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//54        //门派
				GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"60-75"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//55            //山洞             级\n怪物分布：未知
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"75-80"+GameCommonData.wordDic[ "mod_too_con_int_int_49" ],//56            //野外          级\n怪物分布：琼州雄狮、野猴王、鳄鱼
				GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"75-85"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//57             //山洞             级\n怪物分布：未知
				GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"60-65"+GameCommonData.wordDic[ "mod_too_con_int_int_50" ],//58          //野外           级\n怪物分布：螳螂、毒蝎、苗疆蛊女、苗疆蛊术师
				GameCommonData.wordDic[ "mod_too_con_int_int_51" ],//59            //关闭/打开附加技能栏
				GameCommonData.wordDic[ "mod_too_con_int_int_52" ],//60            //世界地图 M
				GameCommonData.wordDic[ "mod_too_con_int_int_53" ],//61          //场景地图 Tab
				GameCommonData.wordDic[ "mod_too_con_int_int_54" ],//62              //排行榜 K
				GameCommonData.wordDic[ "mod_too_con_int_int_55" ],//63              //进入商城
				GameCommonData.wordDic[ "mod_too_con_int_int_56" ],//64      //显示/隐藏雷达地图
				GameCommonData.wordDic[ "mod_too_con_int_int_57" ],//65         //当前服务器时间
				GameCommonData.wordDic[ "mod_too_con_int_int_58" ],//66              //地图名称
				GameCommonData.wordDic[ "mod_too_con_int_int_59" ],//67            //江湖指南 H
				GameCommonData.wordDic[ "mod_too_con_int_int_60" ],//68            //自动挂机 V
				GameCommonData.wordDic[ "mod_too_con_int_int_61" ],//69               //求助GM
				GameCommonData.wordDic[ "mod_too_con_int_int_62" ],//70             //自动寻路
				GameCommonData.wordDic[ "mod_too_con_int_int_52" ],//71           //世界地图 M
				GameCommonData.wordDic[ "mod_too_con_int_int_63" ],//72             //声音开关
				GameCommonData.wordDic[ "mod_too_con_int_int_64" ],//73          //勾选可自动释放
				GameCommonData.wordDic[ "mod_too_con_int_int_65" ], //74            //展开/收缩队伍面板
				GameCommonData.wordDic[ "mod_too_con_int_int_66" ], //75              //左键点击查看详细，领取完全部礼物后，本窗口会自动关闭
				GameCommonData.wordDic[ "mod_too_con_int_int_67" ],//76                //离线经验
				GameCommonData.wordDic[ "mod_too_con_int_int_68" ],//77                //活动日程
				GameCommonData.wordDic[ "mod_too_con_int_int_69" ],//78                //装备强化
				GameCommonData.wordDic[ "mod_too_con_int_int_70" ],//79                //江湖挑战
				GameCommonData.wordDic[ "mod_too_con_int_int_71" ],//80                //系统设置
				GameCommonData.wordDic[ "mod_too_con_int_int_72" ],//81             //活力值在线每10分钟涨1点\n每日早晨5点时会自动补全
				GameCommonData.wordDic[ "mod_too_con_int_int_73" ],//82             //精力值在线每10分钟涨1点\n每日早晨5点时会自动补全
				GameCommonData.wordDic[ "mod_too_con_int_int_74" ],//83           //杀死白名玩家，杀气值会增长。杀气值在野外每30分钟降低1点
				GameCommonData.wordDic[ "mod_too_con_int_int_75" ],//84                //可以通过打工、捐献等方式增长
				GameCommonData.wordDic[ "mod_too_con_int_int_76" ],//85                //可以通过收徒、出师等方式增长
				GameCommonData.wordDic[ "mod_too_con_int_int_77" ],//86                  //完成主职业门派师门任务获得
				GameCommonData.wordDic[ "mod_too_con_int_int_78" ],//87                  //完成副职业门派师门任务获得
				GameCommonData.wordDic[ "mod_too_con_int_int_79" ],//88                 //不同的VIP等级享受的特权不同
				GameCommonData.wordDic[ "mod_too_con_int_int_80" ],//89                     //可以使用道具栏道具扩充
				GameCommonData.wordDic[ "mod_too_con_int_int_81" ],//90                       //当前所属武林同盟会
				GameCommonData.wordDic[ "mod_too_con_int_int_82" ],//91                      //可以在领双NPC处解冻
				GameCommonData.wordDic[ "mod_too_con_int_int_83" ],//92                    //可以使用材料栏道具扩充
				GameCommonData.wordDic[ "mod_too_con_int_int_84" ],//93                         //外攻属性伤害
				GameCommonData.wordDic[ "mod_too_con_int_int_85" ],//94                         //内攻属性伤害
				GameCommonData.wordDic[ "mod_too_con_int_int_86" ],//95                         //防御外攻伤害
				GameCommonData.wordDic[ "mod_too_con_int_int_87" ],//96                         //防御内攻伤害
				GameCommonData.wordDic[ "mod_too_con_int_int_88" ],//97                           //冰属性伤害
				GameCommonData.wordDic[ "mod_too_con_int_int_89" ],//98                    //提高攻击的命中几率
				GameCommonData.wordDic[ "mod_too_con_int_int_90" ],//99                    //提高躲避攻击的几率
				GameCommonData.wordDic[ "mod_too_con_int_int_91" ],//100               //提高攻击产生暴击的几率
				GameCommonData.wordDic[ "mod_too_con_int_int_92" ],//101               //降低被攻击时暴击的几率
				GameCommonData.wordDic[ "mod_too_con_int_int_93" ],//102               //施放一些特殊技能所必须
				GameCommonData.wordDic[ "mod_too_con_int_int_94" ],//103                   //称号设置
				GameCommonData.wordDic[ "mod_too_con_int_int_95" ],		//104            //杀死白名玩家，杀气值会增长。在野外场景（江陵、开封除外）每30分钟杀气值-1点，下线时间不保存
				GameCommonData.wordDic[ "mod_too_con_int_int_96" ],	//105           //显示聊天栏
				GameCommonData.wordDic[ "mod_too_con_int_int_97" ],		//106          //隐藏聊天栏
				"混沌之孔\n前3个孔全部开启后才被激活，开启混沌之孔需要虚空破碎针，在该孔内镶嵌的宝石，并不遵循现有的宝石互斥规则。"		//107          //隐藏聊天栏
		];
		
		public static const moneyIntro:Array = 
		[
			GameCommonData.wordDic[ "mod_too_con_int_mon_1" ],              //元宝通过充值获得，充值1元RMB可获得10元宝
			GameCommonData.wordDic[ "mod_too_con_int_mon_2" ],              //珠宝通过官方特殊活动获得，价值等同于元宝，但购买的道具会绑定
			GameCommonData.wordDic[ "mod_too_con_int_mon_3" ]               //每充值5元RMB即可获得1点券，用于购买点券商店中的道具，购买后会绑定
//			"元宝-用于商城购物，购买的道具不会绑定",//0
//			"珠宝-用于商城购物，购买的道具会绑定玩家",//1
//			"点券-只能购买点券商店中的道具，购买的道具会绑定玩家"//2
		]
		
		public static var mapInfoArr:Array=[
			GameCommonData.wordDic[ "mod_too_con_int_map_1" ]+"ToolTip"+GameCommonData.wordDic[ "mod_too_con_int_map_2" ],          //大地图             数据
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"1-10"+GameCommonData.wordDic[ "mod_too_con_int_int_45" ],//46           //野外             级\n怪物分布：兔子、斑嘴鸭、土匪
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"10-15"+GameCommonData.wordDic[ "mod_too_con_int_int_44" ],//44       //野外             级\n怪物分布：山猪、松鼠、青蛙、迷茫的棋手
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"15-20"+GameCommonData.wordDic[ "mod_too_con_int_int_42" ], //40           //野外               级\n怪物分布：孔雀、京城使者、建筑工匠
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"20-25"+GameCommonData.wordDic[ "mod_too_con_int_int_41" ],//39	        //野外           级\n怪物分布：野猫、青鹿、游荡的剑客、武林高手
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"25-30"+GameCommonData.wordDic[ "mod_too_con_int_int_36" ],//33         //野外           级\n怪物分布：狸猫、金国步兵、金国步兵长、金国哨兵、北宋兵卒
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"30-35"+GameCommonData.wordDic[ "mod_too_con_int_int_43" ],//41           //野外             级\n怪物分布：灰熊、云雀、银狼、昆仑山贼
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"35-40"+GameCommonData.wordDic[ "mod_too_con_int_map_3" ],//59          //野外            级\n怪物分布：乾坤使者、绿泽树精、雾花婢女、雾灵
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"40-45"+GameCommonData.wordDic[ "mod_too_con_int_int_46" ],//49             //野外              级\n怪物分布：石狮、泼猴、钟灵龟、八卦树精
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"45-50"+GameCommonData.wordDic[ "mod_too_con_int_int_35" ],//32          //野外                级\n怪物分布：毒蝎、野狗、幽州马贼、齐国幽魂
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"50-55"+GameCommonData.wordDic[ "mod_too_con_int_int_33" ],//30            //野外              级\n怪物分布：紫貂、玉髓羊、回鹘兵、游牧强匪
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"55-60"+GameCommonData.wordDic[ "mod_too_con_int_int_48" ],//53          //野外              级\n怪物分布：泼猴、白族少年、白族少女、朝珠花妖、龙宫天将
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"60-65"+GameCommonData.wordDic[ "mod_too_con_int_int_50" ],//58           //野外               级\n怪物分布：螳螂、毒蝎、苗疆蛊女、苗疆蛊术师
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"65-70"+GameCommonData.wordDic[ "mod_too_con_int_int_34" ],//31         //野外              级\n怪物分布：骆驼、沙漠狐、沙漠花豹、楼兰士兵
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"70-75"+GameCommonData.wordDic[ "mod_too_con_int_map_6" ],//52             //野外              级\n怪物分布：荒村幽魂、荒村鬼民、鳄鱼
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"75-80"+GameCommonData.wordDic[ "mod_too_con_int_map_4" ],//56            //野外             级\n怪物分布：琼州雄狮、野猴王、猛虎
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"80-85"+GameCommonData.wordDic[ "mod_too_con_int_int_38" ],//36            //野外            级\n怪物分布：青眼白虎、辽国巡逻兵、流窜的小贩、皇城守卫
			GameCommonData.wordDic[ "mod_too_con_int_int_32" ]+"85-90"+GameCommonData.wordDic[ "mod_too_con_int_map_5" ],//35         //野外              级\n怪物分布：流寇、女飞贼、祭奠士兵
			GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"85-100"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//34            //山洞           级\n怪物分布：未知
			GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"50-60"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//29            //山洞            级\n怪物分布：未知
			GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//50        //门派
			GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"20-35"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//38             //山洞           级\n怪物分布：未知 
			GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//43         //门派
			GameCommonData.wordDic[ "mod_too_con_int_int_40" ],//42         //主城
			GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//37         //门派
			GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//48         //门派
			GameCommonData.wordDic[ "mod_too_con_int_int_40" ],//45         //主城
			GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//47         //门派
			GameCommonData.wordDic[ "mod_too_con_int_int_39" ],//54         //门派
			GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"40-50"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//51            //山洞           级\n怪物分布：未知
			GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"60-75"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//55            //山洞           级\n怪物分布：未知
			GameCommonData.wordDic[ "mod_too_con_int_int_30" ]+"75-85"+GameCommonData.wordDic[ "mod_too_con_int_int_31" ],//57            //山洞           级\n怪物分布：未知
		];
		
		public static const AttDic:Array = [
				GameCommonData.wordDic[ "mod_too_con_int_con_1" ],             //外攻
				GameCommonData.wordDic[ "mod_too_con_int_con_2" ],             //内攻
				GameCommonData.wordDic[ "mod_too_con_int_con_3" ],	       //外攻防御
				GameCommonData.wordDic[ "mod_too_con_int_con_4" ],         //内攻防御
				GameCommonData.wordDic[ "mod_too_con_int_con_5" ],	           //命中
				GameCommonData.wordDic[ "mod_too_con_int_con_6" ],	           //躲闪
				GameCommonData.wordDic[ "mod_too_con_int_con_7" ],	           //暴击
				GameCommonData.wordDic[ "mod_too_con_int_con_8" ],	           //坚韧
				GameCommonData.wordDic[ "mod_too_con_int_con_9" ],	           //力量
				GameCommonData.wordDic[ "mod_too_con_int_con_10" ],	           //灵力
				GameCommonData.wordDic[ "mod_too_con_int_con_11" ],	           //体力
				GameCommonData.wordDic[ "mod_too_con_int_con_12" ],	           //定力
				GameCommonData.wordDic[ "mod_too_con_int_con_13" ],	           //身法
				GameCommonData.wordDic[ "mod_too_con_int_con_14" ],	       //所有属性
				GameCommonData.wordDic[ "mod_too_con_int_con_15" ],	       //血上限
				GameCommonData.wordDic[ "mod_too_con_int_con_16" ],           //气上限
				undefined,
				undefined,
				undefined,
				GameCommonData.wordDic[ "mod_too_con_int_con_17" ],	       //冰攻击
				GameCommonData.wordDic[ "mod_too_con_int_con_18" ],	       //火攻击
				GameCommonData.wordDic[ "mod_too_con_int_con_19" ],	       //玄攻击
				GameCommonData.wordDic[ "mod_too_con_int_con_20" ],	       //毒攻击
				GameCommonData.wordDic[ "mod_too_con_int_con_21" ],	       //冰防御
				GameCommonData.wordDic[ "mod_too_con_int_con_22" ],	       //火防御
				GameCommonData.wordDic[ "mod_too_con_int_con_23" ],	       //玄防御
				GameCommonData.wordDic[ "mod_too_con_int_con_24" ],           //毒防御
				GameCommonData.wordDic[ "mod_too_con_int_con_25" ]		//27      //速度
		];
		
		public static var PetInfoArr:Array = [
				GameCommonData.wordDic[ "mod_too_con_int_pet_1" ],		      //繁殖需要一只雄性宝宝和一只雌性宝宝						//性别
				GameCommonData.wordDic[ "mod_too_con_int_pet_2" ],				//宠物不同的性格影响宠物技能的冷却时间					//性格
				GameCommonData.wordDic[ "mod_too_con_int_pet_3" ],	     //宠物出战和死亡会降低快乐，快乐值低于60将无法出战					//快乐
				GameCommonData.wordDic[ "mod_too_con_int_pet_4" ],			//宠物寿命达到0时无法出战									//寿命
				GameCommonData.wordDic[ "mod_too_con_int_pet_5" ],			//宠物整体战斗力的评价				    				//评价
				GameCommonData.wordDic[ "mod_too_con_int_pet_6" ],		//副宠的天赋不低于主宠的悟性才可以参与合成				//天赋
				GameCommonData.wordDic[ "mod_too_con_int_pet_7" ],				//已繁殖次数/可繁殖次数						//繁殖
				GameCommonData.wordDic[ "mod_too_con_int_pet_8" ],   //成长从低到高一共有五个档次：普通、优秀、杰出、卓越、完美。影响宠物升级时自然获得的属性点      	//成长
				GameCommonData.wordDic[ "mod_too_con_int_pet_9" ],		//悟性直接影响宠物资质的优劣，资质越高的宠物，基本属性换算战斗属性的比例越高  		      //悟性
				GameCommonData.wordDic[ "mod_too_con_int_newPet_1" ],				//已繁殖次数/可繁殖次数
				GameCommonData.wordDic[ "mod_too_con_int_newPet_2" ],				//宠物达到50级可玩耍一次，70可增加一次，90可增加一次，一个宠物一共可玩耍3次，还痛可重置玩耍次数
				GameCommonData.wordDic[ "mod_too_con_int_newPet_3" ], 	//"宠物幻化后，可在宠物强化界面万兽谱内提升灵性。灵性可以进一步增加宠物资质，灵性不能大于当前的悟性。"
				GameCommonData.wordDic[ "mod_too_con_int_newPet_4" ],	//"宠物幻化后，可提升默契。默契越高宠物附体技能效果越明显，默契不能大于当前的灵性。",							//繁殖
				GameCommonData.wordDic[ "mod_too_con_int_newPet_5" ]	//"主要影响外防，内防"			
		];
		
		/** 系统设置ToolTip */
		public static const SYS_SET_INTRO_INFO:Array = [
														GameCommonData.wordDic[ "mod_too_con_int_sys_1" ],            //拒绝所有组队请求
														GameCommonData.wordDic[ "mod_too_con_int_sys_2" ],            //拒绝所有交易请求
														GameCommonData.wordDic[ "mod_too_con_int_sys_3" ],            //拒绝所有切磋请求
														GameCommonData.wordDic[ "mod_too_con_int_sys_4" ],          //拒绝所有加好友请求
														//开启账号保护锁时，不能交易、不能摆摊、不能商城购物、不能使用碎银和银两、宠物不能放生、贵重物品不能丢弃和卖NPC商店(蓝色及蓝色以上物品和装备、前缀是完美的装备、有强化、有升星、有打孔、镶嵌了宝石的装备)，解除账号保护锁时需要验证密码，建议您及时修改默认密码。
														GameCommonData.wordDic[ "mod_too_con_int_sys_5" ]
														];
		
		public static var wordDicForTaiWan:Array = [
					GameCommonData.wordDic[ "mod_too_con_wordDicForTaiWan_1" ]	,	        //facebook链接按钮(點擊實現與Facebook帳號綁定，在此提醒您未來遊戲中達成的成就，將自動發佈至Facebook中的資訊收聽，與您的好友一起分享喜悅。)
					GameCommonData.wordDic[ "mod_too_con_wordDicForTaiWan_2" ]	        //共享到facebook(點擊分享至facebook)
			];
		public function IntroConst()
		{
		}
		
		/** 物品详细信息  */
		public static var ItemInfo:Dictionary = new Dictionary();
		
		/**  铸灵升级所需的魔灵数   */
		public static const castSpiritLevelCount:Array = [10, 30, 90, 270, 810, 2430, 7290, 21870, 65610];
		
		/**  铸灵等级所对应的冰、火、玄、毒的攻击力   */
		public static const castSpiritAtt:Array = [19, 34, 66, 101, 216, 325, 518, 645, 766, 896];
		
		/**  铸灵等级所对应的力、灵、体、定、身的防御力   */
		public static const castSpiritDeffAtt:Array = [12, 22, 43, 67, 144, 216, 346, 430, 509, 640];
		
		/**  铸灵等级所对应的冰、火、玄、毒的装备评分   */  
		public static const castSpiritScore:Array = [21, 43, 71, 96, 125, 148, 175, 196, 233, 260];
		
		/**  铸灵等级所对应的力、灵、体、定、身的装备评分   */
		public static const castSpiritDeffScore:Array = [18, 37, 63, 87, 119, 137, 166, 185, 226, 255];
		
		/** 物品颜色表(白、白、绿、蓝、紫、橙) */
//		public static const itemColors:Array = ["#ffffff", "#ffffff", "#00ff00", "#0098FF", "#9727ff", "#FF6532"];
		public static const itemColors:Array = ["#ffffff", "#ffffff", "#00ff00", "#0098FF", "#7a3fe9", "#FF6532", "#730E54", "#dd00ac"];
		
//		public static const ITEM_COLORS_HEX:Array = [0xffffff, 0xffffff, 0x00cc00, 0x0098FF, 0x7a3fe9, 0xFF6532];
		public static const ITEM_COLORS_HEX:Array = [0xffffff, 0xffffff, 0x00cc00, 0x0066ff, 0x6600ff, 0xFF6600];  //6600ff
		 
		/** 装备强化悬浮框显示 */
		public static const STENS_INCREMENT:Array = [
													null, 
													{str:"(+10%)",  color:"#00FF00"},
													{str:"(+20%)",  color:"#00FF00"},
													{str:"(+30%)",  color:"#00FF00"},
													{str:"(+40%)",  color:"#00CBFF"},
													{str:"(+55%)",  color:"#00CBFF"},
													{str:"(+70%)",  color:"#00CBFF"},
													{str:"(+85%)",  color:"#00CBFF"},
													{str:"(+105%)", color:"#FF0000"},
													{str:"(+125%)", color:"#FF0000"},
													{str:"(+160%)", color:"#FF0000"}
													];
		
		/** 装备升星悬浮框显示 */
		public static const STARS_INCREMENT:Array = [
													null, 
													{str:1.1,  color:"#FFEE00"},
													{str:1.2,  color:"#FFEE00"},
													{str:1.3,  color:"#FFEE00"},
													{str:1.4,  color:"#FFEE00"},
													{str:1.55, color:"#FFEE00"},
													{str:1.7,  color:"#FFEE00"},
													{str:1.85, color:"#FFEE00"},
													{str:2.05, color:"#FFEE00"},
													{str:2.25, color:"#FFEE00"},
													{str:2.6,  color:"#FFEE00"}
													];
		
		/** 魂印颜色 */
		public static const HUN_YIN_COLOR:String = "#6532FF";
		
		/** 魂印加成属性 */
		public static const HUN_YIN_DATA:String = "(+10%)";
		
		/** vip名字颜色 */
		public static const VIP_COLORS:Array = ["", "3", "4", "5","2"];
		
		//标题文本格式
		public static function fontTf(size:uint=16,type:String="", font:String="STKaiti"):TextFormat
		{ 
			if(!type) type = TextFormatAlign.LEFT;
			var tf:TextFormat = new TextFormat();
			tf.size = size;
			tf.font = font;
			tf.align = type;
			return tf;
		}
		
		/** 职业数组 */
		public static const USER_JOB_ARR:Array = [4096, 1, 2, 4, 8, 16, 32];  //新手，唐门，全真，峨眉，丐帮，少林，点苍
		
		/** 属性对应职业评分 */
		public static const PROP_JOB_ARR:Array = [
													[10, 10, 10, 10, 10, 10, 10],					//0 外攻
													[10, 10, 10, 10, 10, 10, 10],					//1 内攻
													[3.33, 3.33, 3.33, 3.33, 3.33, 3.33, 3.33],		//2 外防
													[3.33, 3.33, 3.33, 3.33, 3.33, 3.33, 3.33],		//3 内防
													[33.3, 33.3, 33.3, 33.3, 33.3, 33.3, 33.3],		//4 命中
													[33.3, 33.3, 33.3, 33.3, 33.3, 33.3, 33.3],		//5 闪避
													[33.3, 33.3, 33.3, 33.3, 33.3, 33.3, 33.3],		//6 暴击
													[30, 30, 30, 30, 30, 30, 30],					//7 坚韧
													[100, 20, 20, 20, 100, 100, 100],						//8 力量
													[100, 100, 110, 100, 20, 20, 20],						//9 灵力
													[100, 100, 90, 90, 100, 120, 100],						//10 体力
													[101.6, 81.62, 88.45, 91.78, 94.94, 94.6, 84.95],		//11 定力
													[93.24, 100, 93.24, 93.24, 96.57, 89.91, 103.23],		//12 身法
													[494.84, 401.62, 401.9, 395.02, 411.51, 424.51, 408.18],//13 所有属性
													[2, 2, 2, 2, 2, 2, 2],				//14 血
													[1, 1, 1, 1, 1, 1, 1],				//15 气
													[0, 0, 0, 0, 0, 0, 0],		//16
													[0, 0, 0, 0, 0, 0, 0],		//17
													[0, 0, 0, 0, 0, 0, 0],		//18
													[50, 50, 50, 50, 50, 50, 50],		//19 冰攻击
													[50, 50, 50, 50, 50, 50, 50],		//20 火攻击
													[50, 50, 50, 50, 50, 50, 50],		//21 玄攻击
													[50, 50, 50, 50, 50, 50, 50],		//22 毒攻击
													[12, 12, 12, 12, 12, 12, 12],		//23 冰防御
													[12, 12, 12, 12, 12, 12, 12],		//24 火防御
													[12, 12, 12, 12, 12, 12, 12],		//25 玄防御
													[12, 12, 12, 12, 12, 12, 12],		//26毒防御
													[0, 0, 0, 0, 0, 0, 0]				//27 坐骑速度
													];
		
		public static const MUSICPLAYER_NAME:String = GameCommonData.wordDic["mod_mus_med_mus_onp_1"];
		public static const MUSICPLAYER_TOOLTIP_STRING:String = GameCommonData.wordDic["mod_too_con_int_mus_1"];
	}
}