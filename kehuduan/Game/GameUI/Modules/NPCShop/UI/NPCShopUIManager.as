package GameUI.Modules.NPCShop.UI
{
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	public class NPCShopUIManager
	{
		private var shopView:MovieClip = null;
		
		public function NPCShopUIManager(shopView:MovieClip)
		{
			this.shopView = shopView;
			init();
		}
		
		private function init():void
		{
			shopView.txtInputCount.restrict = "0-9";
			shopView.txtInputCount.text = "1";
			shopView.txtRepare.mouseEnabled = false;
			shopView.txtRepareAll.mouseEnabled = false;
			shopView.mcMoneyBuy.txtMoney.defaultTextFormat = textFormat();
			shopView.mcMoneySale.txtMoney.defaultTextFormat = textFormat();
			
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
			shopView.btnRepare.mouseEnabled = value;
			shopView.btnBuy.mouseEnabled = value;
			shopView.btnRepareAll.mouseEnabled = value;
			shopView.btnSale.mouseEnabled = value;
		}
		
		/** 设置商店模式 0-可购物可修理商店，1-只能购物不能修理的商店*/
		public function setModel(type:int):void
		{
			switch(type) {
				case 0:
					shopView.btnRepare.visible = true;
					shopView.btnRepareAll.visible = true;
					shopView.txtRepare.visible = true;
					shopView.txtRepareAll.visible = true;
					break;
				case 1:
					shopView.btnRepare.visible = false;
					shopView.btnRepareAll.visible = false;
					shopView.txtRepare.visible = false;
					shopView.txtRepareAll.visible = false;
					break;
			}
		}
			
	}
}
