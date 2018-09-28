package GameUI.Modules.Maket.UI
{
	import GameUI.Modules.Maket.Data.MarketConstData;
	
	public class MarketUtils
	{
		public function MarketUtils()
		{
		}
		
		/** remove所有combox */
		public static function hideAllCombox():void
		{
			for(var i:int = 0; i < MarketConstData.comBoxList.length; i++) {
				MarketConstData.comBoxList[i].hide();
			}
		}
		
		/** 计算需花费的钱 0-元宝，1-珠宝，2-点券*/
		public static function getCostMoney():Array 
		{
			var yb:Number = 0;
			var zb:Number = 0;
			var dq:Number = 0;
			var costArr:Array = [];
			for(var i:int = 0; i < MarketConstData.shopCarData.length; i++) {
				var good:Object = MarketConstData.shopCarData[i];
				if(good.payWay == 0) {
					yb += good.PriceIn * good.buyNum;  //good.SalePercent *
					continue;
				}
				if(good.payWay == 1) {
					zb += good.PriceIn * good.buyNum;	//* good.SalePercent
					continue;
				}
				if(good.payWay == 2) {
					dq += good.PriceIn * good.buyNum;	//good.SalePercent *
					continue;
				}
			}
			costArr.push(yb);
			costArr.push(zb);
			costArr.push(dq);
			return costArr;
		}
		
		public static function addArray(a:Array,b:Array):Array
		{
			var arr:Array = new Array();
			var i:int;
			for(i = 0; i < a.length; i++)
			{
				arr.push(a[i]);
			}
			for(i = 0; i < b.length; i++)
			{
				arr.push(b[i]);
			}
			return arr;
		}
		
	}
}