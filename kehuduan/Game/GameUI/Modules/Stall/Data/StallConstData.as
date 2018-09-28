package GameUI.Modules.Stall.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	public class StallConstData
	{
		public function StallConstData()
		{
		}
		
		public static const STALL_DEFAULT_POS:Point = new Point(180, 95);	/** 摆摊面板位置 */
		public static const MONEY_DEFAULT_POS:Point = new Point(565, 180);	/** 金钱输入面板位置 */
		public static const PET_DEFAULT_POS:Point   = new Point(40, 230);	/** 宠物列表面板位置 */
		public static const BBS_DEFAULT_POS:Point   = new Point(432, 95);	/** 摊位留言面板位置 */
		
		public static var StallBBSisOpen:Boolean = false;					/** 摆摊留言板是否已经打开 */
		public static var PetListPanelIsOpen:Boolean = false;				/** 宠物列表是否已经打开 */
		public static var moneyAll:int = 0;									/** 总售价 */
		
		/** 查看过/关注的 摊位列表 1-查看过的， 2-关注的 */
		public static var stallLookDic:Dictionary = new Dictionary();
		/** 摊位名 */
		public static var stallName:String = "";
		/**摊主名*/
		public static var stallOwnerName:String = "";
		/** 摊位信息 */
		public static var stallInfo:String = ""; 
		/** 摊位留言 */
		public static var stallMsg:Array = []; 
		
		/** 当前在查询或打开的摊位ID */
		public static var stallIdToQuery:int = 0;
		
		/** 自己的摊位的ID，摆摊成功则为非零 */
		public static var stallSelfId:int = 0;								
		
		/** 自己交易栏物品格子数据列表 */
		public static var GridUnitList:Array = new Array();
		
		/** 摊位出售的 物品数据 */
		public static var goodList:Array = new Array(24);
		
		/** 接收服务器 临时物品数据 */
		public static var tempGoodList:Array = [];
		
		/** 选择的物品 */
		public static var SelectedItem:GridUnit = null;		
		
		public static var TmpIndex:int  = 0;


		/** 选择的宠物(摊位中的) */
		public static var SelectedPet:Object = null;
		
		/** 选择的宠物(自己宠物列表中的) */
		public static var SelectedPetSF:Object = null;

		/** 摊位出售的 宠物数据 */
		public static var petListSale:Dictionary = new Dictionary();
		
		/** 摊位出售的 自己宠物ID列表 */
		public static var petListSaleSelfIdArr:Array = [];
		
		/** 宠物选择列表中的宠物 */
		public static var petListChoice:Dictionary = new Dictionary();
		
		/** 当前显示的页签，0=物品页，1=宠物页 */
		public static var SelectIndex:int = 0;
		
		public static var stallOwnerIdDic:Dictionary = new Dictionary();
		
	}
}