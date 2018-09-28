package GameUI.Modules.NPCBusiness.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import flash.utils.Dictionary;
	
	public class NPCBusinessConstData
	{
		public function NPCBusinessConstData()
		{
		}
		
		/** 格子数据 */
		public static var GridUnitList:Array = [];
		
		/** 出售的格子数据 */
		public static var goodSaleList:Array = [];
		
		/** 选择的物品 */
		public static var SelectedItem:GridUnit = null;
		
		/** 商店物品数据 */
		public static var goodList:Array = [];
		
		/** 物品售价表 */
		public static var goodSalePriceDic:Dictionary = new Dictionary();
		
		/** 当前选择的商品下标 */
		public static var selectedIndex:int = -1;
		
		/** 拖动对象的临时位置 */
		public static var TmpIndex:int  = 0;
		
		/** 跑商商品 */
		public static var GOOD_DIC:Dictionary;
		
		/** 价格串 */
		public static const PRICE_TYPE_STR:Array = [
													GameCommonData.wordDic[ "mod_npcb_dat_npcbcd_1" ],  //"<font color='#00FF00'>(低价)</font>"
													GameCommonData.wordDic[ "mod_npcb_dat_npcbcd_2" ],  //"<font color='#FF6532'>(中价)</font>"
													GameCommonData.wordDic[ "mod_npcb_dat_npcbcd_3" ]   //"<font color='#FF0000'>(高价)</font>"
													]
		
	}
}