package GameUI.Modules.NPCBusiness.UI
{
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	public class NPCBusinessUIManager
	{
		private var shopView:MovieClip = null;
		
		public function NPCBusinessUIManager(shopView:MovieClip)
		{
			this.shopView = shopView;
			init();
		}
		
		private function init():void
		{
			shopView.txtInputCount.restrict = "0-9";
			shopView.txtInputCount.text = "1";
			shopView.mcMoneyBuy.txtMoney.defaultTextFormat = textFormat();
			shopView.mcMoneySale.txtMoney.defaultTextFormat = textFormat();
			shopView.mcMoneyLast.txtMoney.defaultTextFormat = textFormat();
			
			for(var i:int = 0; i < 10; i++) {
				shopView["mcNPCGood_"+i].mcRed.visible = false;
				shopView["mcNPCGood_"+i].mcYellow.visible = false;
				shopView["mcNPCGood_"+i].txtGoodName.filters = Font.Stroke(0x000000);
			}
		}
		
		/** 获取左对齐文本样式 */
		private function textFormat():TextFormat
		{
			var tf:TextFormat = new TextFormat();
			tf.align = TextFormatAlign.LEFT;
			return tf;
		}
		
		/** 加锁解锁按钮 */
		public function lockBtns(value:Boolean):void
		{
			shopView.btnFrontPage.mouseEnabled = value;
			shopView.btnBackPage.mouseEnabled = value;
			shopView.btnBuy.mouseEnabled = value;
			shopView.btnSale.mouseEnabled = value;
		}
		
	}
}
