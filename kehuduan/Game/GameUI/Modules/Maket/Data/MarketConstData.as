package GameUI.Modules.Maket.Data
{
	import flash.display.MovieClip;
	
	public class MarketConstData
	{
		public function MarketConstData()
		{
		}
		
		/** 格子 */
		public static var GridUnitList:Array = [];
		
		/** 当前页商品数据 */
		public static var curPageData:Array = [];
		
		public static var aTotalDisGoods:Array = [];						//真正用到的打折物品详细信息
		
		/** 特殊物品 比如玩龙蛋 */
		public static var specialGoods:Array = [];
		
		/** 特殊物品和打折物品的集合 */
		public static var specialDiscount:Array = [];
		
		/** 购物车数据 */
		public static var shopCarData:Array = []; 
		
		/** 搜索出的商品 */
		public static var searchGoods:Array = [];
		
		/** 当前商品页签 */
//		public static var curPageIndex:uint = 0;
		
		/** 当前商品页签 */
		public static var curPageIndex:uint = 8;
		
		/** 当前页最多呈现商品数 */
		public static var curMaxGoodsNum:int = 10;
		
		/** 界面UI */
		public static var view:MovieClip; 
		
		/** 商城美女是否加载 */
		public static var isLoadBelle:Boolean = false;
		
		/** comBox列表 (用于从GameUI层移除) */
		public static var comBoxList:Array = [];
		
		/** 预览界面显示 */
		public static var PreviewIsOpen:Boolean = false;
		
		/** 预览方向 */
		public static var directions:Array = [2,3,6,9,8,7,4,1];
		
		/** 支付方式名称列表 */
		public static const payWayNameList:Array = [GameCommonData.wordDic[ "often_used_yb" ], GameCommonData.wordDic[ "often_used_zb" ], GameCommonData.wordDic[ "often_used_dj" ]];//"元宝"	"珠宝" "点券"
		
		/** 支付方式字符串 */
		public static const payWayStrList:Array = ["\\ab", "\\zb", "\\dq"];
		
		
	}
}
