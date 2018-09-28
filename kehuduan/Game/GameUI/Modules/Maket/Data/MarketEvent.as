package GameUI.Modules.Maket.Data
{
	public class MarketEvent
	{
		public function MarketEvent()
		{
		}
		
		public static var ADDTO_SHOPCAR_MARKET:String = "addTo_shopCar_market";		/** 添加商品到购物车 */ 
		
		public static var BUY_ITEM_MARKET:String = "buy_item_market";				/** 购买物品（外部购买接口） */
		
		public static var UPDATE_MONEY_MARKET:String = "update_money_market";		/** 收到userAtt更新商城钱 */
		
		public static var MARKET_STOP_DROG:String = "market_stop_drop";				/** 商城禁止拖动 */
		
		public static var REC_MARKET_DIS_GOOD:String = "REC_MARKET_DIS_GOOD";			/** 收到商城打折物品信息 */
		
		public static var UPDATE_MARKET_GOODS_NUM:String = "UPDATE_MARKET_GOODS_NUM";  /** 收到商城限量物品信息 */
		
		public static var INIT_MARKET_PREVIEW:String = "INIT_MARKET_PREVIEW";  			/** 初始化预览界面 */
		
		public static var SHOW_MARKET_PREVIEW:String = "SHOW_MARKET_PREVIEW";  			/** 显示预览界面 */
		
		public static var CLOSE_MARKET_PREVIEW:String = "CLOSE_MARKET_PREVIEW";  		/** 关闭预览界面 */
		
		public static var UPDATE_COUPONS_BUTTON:String = "update_coupons_view";         /** 更新点券按钮 */
	}
}