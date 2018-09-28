package GameUI.Modules.Depot.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	/**
	 * 仓库数据
	 * @
	 * @
	 */ 
	public class DepotConstData
	{
		public function DepotConstData()
		{
		}
	
		public static const DEPOT_DEFAULT_POS:Point   = new Point(200, 60);		/** 仓库面板默认位置 */
		public static const PANEL_DEFAULT_POS:Point   = new Point(0, 17);		/** （物品、宠物面板）默认位置 */
		public static const EXTENDS_DEFAULT_POS:Point = new Point(540, 120);	/** 扩充空间面板默认位置 */
		public static const MONEY_DEFAULT_POS:Point   = new Point(420, 150);	/** 金钱输入面板位置 */
		
		public static var moneySelf:int = 0;									/** 携带银两 */
		
		public static var moneyDepot:int = 0;									/** 仓库中钱 */
		
		/** 格子数据 */
		public static var GridUnitList:Array = new Array();
		
		/** 物品数据 */
		public static var goodList:Array = new Array(36);
		
		/** 仓库锁定  */
		public static var depot_0_lock:Array = [
													false, false, false, false, false, false,
													false, false, false, false, false, false,
													false, false, false, false, false, false,
													false, false, false, false, false, false,
													false, false, false, false, false, false,
													false, false, false, false, false, false,
												];
		
		/** 当前仓库下标 （0，1，2） */
		public static var curDepotIndex:int = 0;
		
		/** 仓库容量（目前一共多少个格子，以后会存储在UserInfo中，去UserInfo中取即可） */
		public static var gridCount:int;
		
		/** 当前金钱模式 1=存入，2=取出 */
		public static var curMoneyType:int = 0;
		
		/** 拖动对象的临时位置 */
		public static var TmpIndex:int  = 0;
		
		/** 选择的物品 */
		public static var SelectedItem:GridUnit = null;		
		
		/** 物品仓库扩展面板已经打开 */
		public static var itemExtIsOpen:Boolean = false;
		
		/** 宠物仓库扩展面板已经打开 */
		public static var petExtIsOpen:Boolean = false;
		
		/** 当前宠物模式 0-return，1=取出(选中的是仓库中的)，2=存入(选中的是宠物背包中的) */
		public static var curPetType:int = 0;
		
		/** 仓库中 宠物列表数据 */
		public static var petListDepot:Dictionary = new Dictionary();
		
		/** 当前宠选择的宠物 */
		public static var petSelected:GamePetRole = null;
		
//		/** 自己宠物栏中 宠物列表数据 */
//		public static var petListSF:Array = new Array();
//		
//		/** 选择的宠物(仓库中的) */
//		public static var SelectedPetDepot:Object = null;
//		
//		/** 选择的宠物(自己宠物栏中的) */
//		public static var SelectedPetSF:Object = null;
//		
//		/** 选择的宠物MC(仓库中的) */
//		public static var SelectedPetMCDepot:MovieClip = null;
//		
//		/** 选择的宠物MC(自己宠物栏中的) */
//		public static var SelectedPetMCSF:MovieClip = null;
		
		/** 当前显示的页签，0=物品页，1=宠物页 */
		public static var SelectIndex:int = 0;
		
		/** 玩家当前仓库宠物容量 */
		public static var petDepotNum:uint = 0;
	}
}