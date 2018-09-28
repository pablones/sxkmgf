package GameUI.Modules.NPCShop.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	public class NPCShopConstData
	{
		public function NPCShopConstData()
		{
		}
		
		/** 格子数据 */
		public static var GridUnitList:Array = [];
		
		/** 出售的格子数据 */
		public static var goodSaleList:Array = [];
		
		/** 选择的物品 */
		public static var SelectedItem:GridUnit = null;
		
		/** 支付方式 df:0-碎银，1-银两 */
		public static var payWay:uint = 0;
		
		/** 商店物品数据 */
		public static var goodList:Array = [];
		
		/** 物品ID列表 */
		public static var goodTypeIdList:Array = [];
		
		/** 当前选择的商品下标 */
		public static var selectedIndex:int = -1;
		
		/** 拖动对象的临时位置 */
		public static var TmpIndex:int  = 0;
		
	}
}