package GameUI.Modules.Unity.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	public class UnityConstData
	{
//		/** 帮会ID */
//		public static var unityID:int;										//0为没有加入帮派
//		/** 帮会职位*/
//		public static var unityJop:int;										//100帮主 90副帮主 81-84堂主 71-74副堂主 61-64精英 51-54分堂帮众 20果农 10普通帮众
		/** 帮派的操作的对象 */
		public static var unityObj:Object = new Object();
		/** 玩家已经响应的帮派 */
		public static var respondUnity:String ;
		/** 捐献界面是否打开 */
		public static var contributeIsOpen:Boolean = false;
		/** 任命界面是否打开 */
		public static var ordainIsOpen:Boolean = false;
		/** 修改公告界面是否打开 */
		public static var perfectNoticeIsOpen:Boolean = false;
		/** 申请列表界面是否打开*/
		public static var applyViewIsOpen:Boolean = false;
		/** 响应界面是否打开*/
		public static var respondViewIsOpen:Boolean = false;
		/** 雇佣界面是否打开*/
		public static var hireViewIsOpen:Boolean = false;
		/** 任命 堂主的数组*/
		//  青龙堂  白虎堂  玄武堂  朱雀堂  堂主  副堂主  精英  帮众
		public static var ordainArr:Array = [GameCommonData.wordDic[ "mod_uni_dat_uni_green_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_white_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_red_1" ],
																GameCommonData.wordDic[ "mod_uni_dat_uni_ord_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_ord_2" ],GameCommonData.wordDic[ "mod_uni_dat_uni_ord_3" ],GameCommonData.wordDic[ "mod_uni_dat_uni_ord_4" ]];
		/** 指定某一个帮派的ID*/
		public static var oneUnityId:int = 0;
		/** 指定某一个申请帮派的玩家的ID */
		public static var playerId:int = 0;
		/** 是否在建帮中*/
		public static var iscreating:int ;							//如果为0则还没建帮，否则正在建帮中		
		/** 是否选择了要响应的帮派*/
		public static var isSelectUnity:Boolean = false ;
		/** 是否可以响应帮派*/
		public static var isRespondUnity:Boolean = true ;
		/** 帮派主界面所有成员数据 */
		public static var allMenberList:Array = [];	
		/** 申请帮派界面所有帮派的数据 */
		public static var allUnityList:Array = [];	
		/** 排序方法数组 */
		public static const SORTLIST:Array = ["CASEINSENSITIVE","NUMERIC","NUMERIC","NUMERIC","NUMERIC","NUMERIC"]
		/** 申请帮派排序方法数组 */
		public static const ApplySORTLIST:Array = ["CASEINSENSITIVE","CASEINSENSITIVE","NUMERIC","NUMERIC"]
		/** 是否从帮派NPC那打开了面板 */
		public static var isOpenNpcView:Boolean = false;		
		/** 帮派更新数据 对象，操作号 数组 */
		public static var updataArr:Array = [];
		/** 帮派等级 */
		public static var unityLevel:int;
		/** 帮派主界面的当前分页 */
		public static var unityPage:int = 0;
		/** 帮派成员界面是否打开 */
		public static var unityMenberIsOpen:Boolean = false;		
		/** 帮派含有分堂的主界面是否打开 */		
		public static var unityMainIsOpen:Boolean = false;		
		/** 帮派选中哪个分堂 */
		public static var unityCurSelect:int = 0;
		/** 帮派分堂的总个数*/
		public static var unityOtherNum:int = 5;			
		/*-------- 主堂数据对象的属性 ----------------*/
		/**分堂名称，等级 ，帮派资金，帮派建设度，帮派繁荣度，建筑工人，商人，武学大师，帮派名，开创者，现任帮主，创建时间，帮派成员数，当前可雇用的总数，在线人数，行动力，帮派通告*/
		/** 主堂数据对象 */   //  主堂
		public static var mainUnityDataObj:Object = {name:"主堂" , level:1 , unityMoney:132324234 , unityBuilt:289999, unitybooming:10000000
													,craftsmanNum:10 , businessmanNum:10 , masterNum:10 ,unityName:"帮助很帅" 
													, oldBoss:"杨龙" , newBoss:"杨龙" , createTime:"2008060301" , unityMenber:"99"
													, hireNum:0 ,onlineMenber:0 , moving:0 ,unityNotice:""};
		/*-------- 分堂数据对象的属性 ----------------*/
		/**分堂名称，等级 ，建筑工人，商人，武学大师，技能名1，技能名2，技能名3*，正在学习的技能序列号，技能123升级所需的经验，主角的技能等级，当前研究到的技能等级，当前技能的总经验，该分堂是否停用
		   技能图标	*/
		/*----------------------------------------*/
		/** 青龙堂数据对象 */  //  青龙堂  青龙毒煞  青龙摆尾  青龙斩月
		public static var greenUnityDataObj:Object = {name:GameCommonData.wordDic[ "mod_uni_dat_uni_green_1" ] , level:0 , craftsmanNum:0 , businessmanNum:0 , masterNum:0
													 	,skillName1:GameCommonData.wordDic[ "mod_uni_dat_uni_green_2" ] , skillName2:GameCommonData.wordDic[ "mod_uni_dat_uni_green_3" ] , skillName3:GameCommonData.wordDic[ "mod_uni_dat_uni_green_4" ] , skillStuding:0
													 	,skillStudyNum1:0 , skillStudyNum2:0,skillStudyNum3:0			//技能所显示的经验
													 	,skillStudySelf1:0 , skillStudySelf2:0 , skillStudySelf3:0		//主角学到的技能等级
													 	,skillStudyCurr1:30 ,skillStudyCurr2:0 ,skillStudyCurr3:0 		//当前分堂研究到的技能等级
													 	,skillTolExp1:0 , skillTolExp2:0 , skillTolExp3:0				//技能的总经验
													 	,isStop:false , skillIconList:[2101 , 2102 , 2103]				//技能图标
													 };
		/** 白虎堂数据对象 */  // 白虎堂  白虎冰啸  白虎星降  猛虎一击
		public static var whiteUnityDataObj:Object = {name:GameCommonData.wordDic[ "mod_uni_dat_uni_white_1" ] , level:0, craftsmanNum:20 , businessmanNum:20 , masterNum:20 
													 	,skillName1:GameCommonData.wordDic[ "mod_uni_dat_uni_white_2" ], skillName2:GameCommonData.wordDic[ "mod_uni_dat_uni_white_3" ], skillName3:GameCommonData.wordDic[ "mod_uni_dat_uni_white_4" ] , skillStuding:0
													 	,skillStudyNum1:1 , skillStudyNum2:1,skillStudyNum3:0
													 	,skillStudySelf1:0 , skillStudySelf2:0, skillStudySelf3:0
													 	,skillStudyCurr1:30 ,skillStudyCurr2:29 ,skillStudyCurr3:29 
													 	,skillTolExp1:0 , skillTolExp2:0 , skillTolExp3:0
													 	,isStop:false , skillIconList:[2201 , 2202 , 2203]				//技能图标
													 };
		/** 玄武堂数据对象 */  //  玄武堂  玄影武袭  玄武战甲  玄武霸体
		public static var xuanUnityDataObj:Object = {name:GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_1" ] , level:1, craftsmanNum:1 , businessmanNum:1 , masterNum:50 
													 	,skillName1:GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_2" ] , skillName2:GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_3" ] , skillName3:GameCommonData.wordDic[ "mod_uni_dat_uni_xuan_4" ] , skillStuding:2
													 	,skillStudyNum1:1 , skillStudyNum2:1,skillStudyNum3:1
													 	,skillStudySelf1:0 , skillStudySelf2:0 , skillStudySelf3:0
													 	,skillStudyCurr1:30 ,skillStudyCurr2:28 ,skillStudyCurr3:29 
													 	,skillTolExp1:0 , skillTolExp2:0 , skillTolExp3:0
													 	,isStop:false , skillIconList:[2301 , 2302 , 2303]				//技能图标
													 };
		/** 朱雀堂数据对象 */  //  朱雀堂  朱雀焰舞  朱雀真火  朱雀灵羽
		public static var redUnityDataObj:Object = {name:GameCommonData.wordDic[ "mod_uni_dat_uni_red_1" ] , level:1, craftsmanNum:1 , businessmanNum:1 , masterNum:50
													 	,skillName1:GameCommonData.wordDic[ "mod_uni_dat_uni_red_2" ] , skillName2:GameCommonData.wordDic[ "mod_uni_dat_uni_red_3" ] , skillName3:GameCommonData.wordDic[ "mod_uni_dat_uni_red_4" ] , skillStuding:3
													 	,skillStudyNum1:1, skillStudyNum2:1,skillStudyNum3:1
													 	,skillStudySelf1:0 , skillStudySelf2:0 , skillStudySelf3:0
													 	,skillStudyCurr1:20 ,skillStudyCurr2:20 ,skillStudyCurr3:20 
													 	,skillTolExp1:0 , skillTolExp2:0 , skillTolExp3:0
													 	,isStop:true , skillIconList:[2401 , 2402 , 2403]				//技能图标
													 };
		/** 含有帮派所有分堂数据的总数组 */
		public static var otherUnityArray:Array = [mainUnityDataObj , greenUnityDataObj , whiteUnityDataObj , xuanUnityDataObj , redUnityDataObj];	
		/** 捐献的物品数组 */
		public static var contributeArray:Array = [GameCommonData.wordDic[ "mod_uni_dat_uni_con_1" ],GameCommonData.wordDic[ "mod_uni_dat_uni_con_2" ],GameCommonData.wordDic[ "mod_uni_dat_uni_con_3" ],GameCommonData.wordDic[ "mod_uni_dat_uni_con_4" ]];  //  木材"  矿石  皮毛  金币
		/** 格子数据 */
		public static var GridUnitList:Array = [];
		/** 出售的格子数据 */
		public static var goodSaleList:Array = [];
		/** 选择的物品 */
		public static var SelectedItem:GridUnit = null;
		/** 拖动对象的临时位置 */
		public static var TmpIndex:int  = 0;
		/** 上线后的第一次打开 */
		public static var firstOnline:Boolean = true;
		/** 前一次存储的技能数据时间数组 */
		public static var lastTimeList_Skill:Array = [];	
		/** 前一次存储的建设度数据时间数组 */
		public static var lastTimeList_Built:Array = [];	
		/** 帮派停止维护 , 帮派功能关闭(打工 ， 领工资 ，学技能。。。。。) */
		public static var UnityPerformanceClose:Boolean = false;
		/** 帮派分堂的职位 (堂主，副堂主，本堂精英，本堂帮众)*/
		public static var unityOtherJopList:Array = [[81,71,61,51] , [82,72,62,52] , [83,73,63,53] , [84,74,64,54]];
		/** 分堂关闭的匹配数组 */
		public static var otherCloseList:Array = [1 , 2 , 4 , 8];
		/** 分堂升级条件数组*/
		public static var levelNeedList:Array = [];		/** 0，1,2，3是升级要求  4,5,6，7是升级消耗，预留了几个空位*/
		/** 升级达到要求的颜色 */
		public static const LEVELCOLOR:String = "#00FF00";
		/** 打工类型 */
		public static var subWorkType:int;
		/** 是否正在打工 */
		public static var isWorking:Boolean = false;
		/** 技能研究切换时间 */
		public static var changeSkillList:Array = [{start:0 , over:0} , {start:0 , over:0} , {start:0 , over:0} , {start:0 , over:0}];	//每个堂的切换时间
		/** 帮派数据的传输状态 */
		public static var dataSendState:Boolean = true;		//进入场景时请求数据的状态
		/** 帮派等级上限 */
		public static var unityLevelTop:int = 5;
		///////悬浮框数据区域
		/** 分堂升级按钮悬浮框数据 */
		public static var levelUpData:String = "";
		/** 分堂繁荣度 */
		public static var infoBooming:String = "";
		/** 分堂建设度 */
		public static var infoBuilt:String = "";
		/** 帮派资金 */
		public static var infoMoney:String = "";
		/** 成员数量 */
		public static var infoMenber:String = "";
		/** 分堂建筑工匠 */
		public static var infoCraftsman:String = "";
		/** 分堂武学大师 */
		public static var infoMasterNum:String = "";
		/** 分堂贸易商人 */
		public static var infoBusinessman:String = "";
		/** 建设度进度条悬浮框 */
		public static var infoBuiltBar:String = "";
		/** 繁荣度进度条悬浮框 */
		public static var infoBoomingBar:String = "";
		/** 技能等级悬浮框1 */
		public static var infoSkill_1:String = "";
		/** 技能等级悬浮框2 */
		public static var infoSkill_2:String = "";
		/** 技能等级悬浮框3 */
		public static var infoSkill_3:String = "";
		/** 青龙堂悬浮框 */
		public static var infoSynQ:String = "";
		/** 白虎堂悬浮框 */
		public static var infoSynB:String = "";
		/** 玄武堂悬浮框 */
		public static var infoSynX:String = "";
		/** 朱雀堂悬浮框 */
		public static var infoSynZ:String = "";
		/** 停止维护的悬浮框 */
		public static var infoUnityState:String = "";
		/** 分堂关闭的悬浮框 */
		public static var infoSubState:String = "";
		////////悬浮框数据结束
		/** 区分技能面板请求 跟 帮派请求 */
		public static var unityIsSend:Boolean = false;
		
	}
}