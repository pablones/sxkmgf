package GameUI.ConstData
{
	import GameUI.Encrypt.des.DESKey;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	
	public class UIConstData
	{
		/** 快速栏上面的物品  */
		public static var QuickUnitList:Array = new Array();
		/** 正在交易  */
		public static var IsTrading:Boolean	= false;
		/** 表情栏已经打开  */
		public static var SelectFaceIsOpen:Boolean = false;	
		/** 前面四个默认的弹出位置  */
		public static var DefaultPos1:Point = new Point(80, 58); 
		/** 后面四个默认的弹出位置  */
		public static var DefaultPos2:Point = new Point(586, 58);
	
		/** 物品数据1  */
		public static var ItemDic_1:Dictionary = new Dictionary();
		
		/** 物品数据2  */
		public static var ItemDic_2:Dictionary = new Dictionary();
		
		/** 广告字典 */
		public static var Filter_ad:Array = new Array();
		
		/** 聊天字典 */
		public static var Filter_chat:Array = new Array();
		
		/** 角色名字典 */
		public static var Filter_role:Array = new Array();
		
		/** 合格字符字典 */
		public static var Filter_okName:Array = new Array();
		
		/** 商城商品列表 */
		public static var MarketGoodList:Array = [];
		
		/** 属性加成  */
		public static var AppendAttribute:Dictionary = new Dictionary();
		
		/** 物品数据  */
		public static var CoordinatesEquip:Dictionary = new Dictionary();
		
		/** 经验  */
		public static var ExpDic:Dictionary = new Dictionary();
		
		/** Control键被按下  */
		public static var ControlIsDown:Boolean = false;
		
		/** 键盘开关 */
		public static var KeyBoardCanUse:Boolean = true; 
		
		/** 字典加载开关 */
		public static var Filter_Switch:Boolean = false;
		
//		/** 换装备计时器 */
//		public static var EquipTimer:Timer = new Timer(2000, 1);  
		/** 临时任务字典*/
		public static var TaskTempInfo:Dictionary=new Dictionary();
		
		/** ToolTip开关 */
		public static var ToolTipShow:Boolean = true;
		
		/** 焦点占用开关 */
		public static var FocusIsUsing:Boolean = false;
		
		/** 物品使用计时器 */
		public static var useItemTimer:Timer = new Timer(1000, 1);
		
		/////////////////////////////////////////////////
		//宠物系列面板
		
		public static const PET_RULE_BASE:String = "pet_rule_base";			/** 宠物基本面板 */
		
		public static const PET_DOUBLE_BREED:String = "pet_double_breed";	/** 宠物双繁 */
		

		public static const PETBREEDSINGLE:String = "petBreedSingle";		/** 宠物单人繁殖 */
		public static const PETBREEDDOUBLE:String = "petBreedDouble";		/** 宠物双人繁殖 */
		public static const PETTOBABY:String = "petToBaby";					/** 宠物还童 */
		public static const PETSKILLLEARN:String = "petSkillLearn";			/** 宠物技能学习 */
		public static const PETSKILLUP:String = "petSkillUp";				/** 宠物技能升级 */
		public static const PETSAVVYUSEMONEY:String = "petSavvyUseMoney";	/** 宠物悟性提升 */
		public static const PETSAVVYJOIN:String = "petSavvyJoin";			/** 宠物二合一 */
		public static var PET_MODEL_DIC:String = GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.PetModelDic; /** 宠物模型目录 */
		//////////////////////////////////////////////////
		
		/** 装备绑定类型 */
//		public static const ITEM_BIND_DATA:Array = [
//													{data:0x0200, name:"装备后绑定"},        	//装备后绑定
//													{data:0x0400, name:"拾取后绑定"},			//拾取后绑定
//													{data:0x20,   name:"不可交易"},				//不可交易
//													{data:0x40,   name:"不可丢弃"}				//不可丢弃
//													];
		
		/** 是否在跑商 */
		public static var IS_BUSINESSING:Boolean = false;
		
		/** DES加密 */
		public static var DES:DESKey = null;
		
		/** 获取物品对象 */
		public static function getItem(type:uint):Object
		{
			var res:Object = ItemDic_1[type];
			if(!res) {
				res = ItemDic_2[type];
			}
			return res;
		}
		
		/** 自动挂机中是否自动接受组队邀请和申请 */
		public static var AUTO_ACCEPT_TEAM_INTITE_AND_APPLY:Boolean = false;
		
		/** 自动挂机中是否自动剔除离线队员 */
		public static var AUTO_KICKOUT_LEAVE_MEMBER:Boolean = false;
		
		/** 竞技场聊天面板的表情框是否打开 */
		public static var SelectFaceForArenaIsOpen:Boolean = false;
	}
}
