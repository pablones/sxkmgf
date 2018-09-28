package GameUI.Modules.Trade.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.utils.Dictionary;
	
	public class TradeConstData
	{
		public function TradeConstData()
		{
			
		}
		
		/** 自己交易栏物品格子数据列表 */
		public static var GridUnitList:Array = new Array();
		
		public static var GridUnitListOp:Array = new Array();
		
		/** 选择的物品 */
		public static var SelectedItem:GridUnit = null;		
		
		public static var TmpIndex:int  = 0;
		
		public static var AllItems:Array = new Array(5);
		
		public static var SelectIndex:int = 0;
		
		/** 自己交易栏中的物品ID表，用于取消交易时解锁 */
		public static var idItemSelfArr:Array = [];
		
		public static var goodSelfList:Array = new Array(5);		/** 己方物品列表 */
		public static var goodOpList:Array = new Array(5);			/** 对方物品列表 */
		
		/** 当前交易人（对方）的名字 */
		public static var opName:String = "";
		
		/** 对方宠物快照列表 */
		public static var petOpDic:Dictionary = new Dictionary();
		
		/** 自己宠物快照列表 */
		public static var petSelfDic:Dictionary = new Dictionary();
		
		/** 自己背包中的宠物（宠物列表副本） */
		public static var petListSelfDic:Dictionary = new Dictionary();
		
		/** 宠物选择列表中，当前选择的宠物 */
		public static var petSelectOfList:GamePetRole = null;
		
		/** 自己交易宠物列表中，当前选择的宠物 */
		public static var  petSelectOfTrade:GamePetRole = null;
		
		/** 对方交易宠物列表中，当前选择的宠物 */
		public static var petSelectOp:GamePetRole = null;
		
		public static var traderId:int;
		
	}
}