package GameUI.Modules.Stall.UI
{
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	
	public class StallUIManager
	{
		private var stall:MovieClip;
		
		public function StallUIManager(stall:MovieClip)
		{
			this.stall = stall;
			initView();
		}
		
		/** 初始化面板 */
		private function initView():void
		{
			stall.txtModifyName.mouseEnabled = false;
			stall.txtTax.mouseEnabled = false;
			stall.txtOwerName.mouseEnabled = false;
			stall.txtBuyNum.mouseEnabled = false;
			stall.txtModifyPrice.mouseEnabled = false;
			stall.txtDelGood.mouseEnabled = false;
			stall.txtCloseStall.mouseEnabled = false;
			stall.txtBuy.mouseEnabled = false;
			stall.mcMoneyAll.mouseEnabled = false;
			stall.mcMoneySelf.mouseEnabled = false;
			stall.txtSeePet.mouseEnabled = false;
			stall.txtSaleInfo.mouseEnabled = false;
			stall.txtOwerName.text = "";
			stall.txtInputNum.visible = false;
			stall.txtStallName.text = "";
			stall.txtTax.text = "5";
			stall.txtSaleInfo.visible = true;
//			stall.txtInputNum.restrict = "0-9";
//			stall.txtInputNum.text = "";
//			(stall.txtInputNum as TextField).background = true;
//			(stall.txtInputNum as TextField).backgroundColor = 0x000000;
//			(stall.txtInputNum as TextField).border = true;
//			(stall.txtInputNum as TextField).borderColor = 0x4D3A12;
			
			stall.mcMoneyAll.txtMoney.text   = "0\\cc";
			stall.mcMoneySelf.txtMoney.text = "0\\cc";
			ShowMoney.ShowIcon(stall.mcMoneyAll, stall.mcMoneyAll.txtMoney, true);
			ShowMoney.ShowIcon(stall.mcMoneySelf, stall.mcMoneySelf.txtMoney, true);
		}
		
		/** 设置摆摊面板模式 0=摊主查看物品，1=摊主查看宠物，2=买家查看物品，3=买家查看宠物 */
		public function setModel(type:uint):void
		{
			if(type == 0) {
//				trace("摊主-物品-模式");
				stall.txtStallName.mouseEnabled = true;
				stall.txtModifyName.visible = true;
				stall.txtModifyPrice.visible = true;
				stall.txtCloseStall.visible = true;
				stall.txtDelGood.visible = true;
				stall.btnModifyName.visible = true;
				stall.btnModifyPrice.visible = true;
				stall.btnDelGood.visible = true;
				stall.btnCloseStall.visible = true;
				
				stall.txtBuyNum.visible = false;
				stall.txtInputNum.visible = false;
				stall.txtBuy.visible = false;
				stall.btnBuy.visible = false;
				stall.txtSeePet.visible = false;
				stall.btnSeePet.visible = false;
				stall.txtSaleInfo.visible = true;
			} else if(type == 1){
//				trace("摊主-宠物-模式");
				stall.txtStallName.mouseEnabled = true;
				stall.txtModifyName.visible = true;
				stall.txtModifyPrice.visible = true;
				stall.txtCloseStall.visible = true;
				stall.txtDelGood.visible = true;
				stall.btnModifyName.visible = true;
				stall.btnModifyPrice.visible = true;
				stall.btnDelGood.visible = true;
				stall.btnCloseStall.visible = true;
				stall.txtSeePet.visible = true;
				stall.btnSeePet.visible = true;
				
				stall.txtBuyNum.visible = false;
				stall.txtInputNum.visible = false;
				stall.txtBuy.visible = false;
				stall.btnBuy.visible = false;
				stall.txtSaleInfo.visible = false;
			} else if(type == 2){
//				trace("买家-物品-模式");
				stall.txtStallName.mouseEnabled = false;
				stall.txtModifyName.visible = false;
				stall.txtModifyPrice.visible = false;
				stall.txtCloseStall.visible = false;
				stall.txtDelGood.visible = false;
				stall.btnModifyName.visible = false;
				stall.btnModifyPrice.visible = false;
				stall.btnDelGood.visible = false;
				stall.btnCloseStall.visible = false;
				stall.txtSeePet.visible = false;
				stall.btnSeePet.visible = false;
				
				stall.txtBuyNum.visible = false;
				stall.txtInputNum.visible = false;
				stall.txtBuy.visible = true;
				stall.btnBuy.visible = true;
				stall.txtSaleInfo.visible = false;
			} else {
//				trace("买家-宠物-模式");
				stall.txtStallName.mouseEnabled = false;
				stall.txtModifyName.visible = false;
				stall.txtModifyPrice.visible = false;
				stall.txtCloseStall.visible = false;
				stall.txtDelGood.visible = false;
				stall.btnModifyName.visible = false;
				stall.btnModifyPrice.visible = false;
				stall.btnDelGood.visible = false;
				stall.btnCloseStall.visible = false;
				stall.txtBuyNum.visible = false;
				stall.txtInputNum.visible = false;
				
				stall.txtBuy.visible = true;
				stall.btnBuy.visible = true;
				stall.txtSeePet.visible = true;
				stall.btnSeePet.visible = true;
				stall.txtSaleInfo.visible = false;
			}
		}
		
		/** 重置面板 */
		public function resetPanel():void
		{
			stall.txtStallName.text = "";
			stall.txtOwerName.text = "";
			stall.txtTax.text = "";
//			stall.txtInputNum.text = "1";
			stall.mcMoneyAll.txtMoney.text   = "0\\cc";
			stall.mcMoneySelf.txtMoney.text = "0\\cc";
			ShowMoney.ShowIcon(stall.mcMoneyAll, stall.mcMoneyAll.txtMoney, true);
			ShowMoney.ShowIcon(stall.mcMoneySelf, stall.mcMoneySelf.txtMoney, true);
			
			stall.txtStallName.mouseEnabled = false;
			stall.txtModifyName.visible = false;
			stall.txtModifyPrice.visible = false;
			stall.txtCloseStall.visible = false;
			stall.txtDelGood.visible = false;
			stall.btnModifyName.visible = false;
			stall.btnModifyPrice.visible = false;
			stall.btnDelGood.visible = false;
			stall.btnCloseStall.visible = false;
			stall.txtBuyNum.visible = false;
			stall.txtInputNum.visible = false;
			stall.txtBuy.visible = false;
			stall.btnBuy.visible = false;
			stall.txtSeePet.visible = false;
			stall.btnSeePet.visible = false;
			stall.btnStallInfo.visible = false;
			stall.txtSaleInfo.visible = true;
			
			stall.btnModifyName.mouseEnabled = true;
			stall.btnModifyPrice.mouseEnabled = true;
			stall.btnDelGood.mouseEnabled = true;
			stall.btnCloseStall.mouseEnabled = true;
			stall.btnBuy.mouseEnabled = true;
			stall.btnSeePet.mouseEnabled = true;
			stall.btnStallInfo.mouseEnabled = true;
		}
		
		/** 锁住所有按钮 金钱面板弹出时 */
		public function lockBtns():void
		{
			stall.btnModifyName.mouseEnabled = false;
			stall.btnModifyPrice.mouseEnabled = false;
			stall.btnDelGood.mouseEnabled = false;
			stall.btnCloseStall.mouseEnabled = false;
			stall.btnBuy.mouseEnabled = false;
			stall.btnSeePet.mouseEnabled = false;
			stall.btnStallInfo.mouseEnabled = false;
			stall.mcPage_0.mouseEnabled = false;
			stall.mcPage_1.mouseEnabled = false;
		}
		
		/** 解锁所有按钮 金钱面板关闭时 */
		public function unLockBtns():void
		{
			stall.btnModifyName.mouseEnabled = true;
			stall.btnModifyPrice.mouseEnabled = true;
			stall.btnDelGood.mouseEnabled = true;
			stall.btnCloseStall.mouseEnabled = true;
			stall.btnBuy.mouseEnabled = true;
			stall.btnSeePet.mouseEnabled = true;
			stall.btnStallInfo.mouseEnabled = true;
			stall.mcPage_0.mouseEnabled = true;
			stall.mcPage_1.mouseEnabled = true;
		}
		
		/** 刷新显示钱数 */
		public function refreshMoney():void
		{
			if(StallConstData.SelectIndex == 0) {		//物品
				if(StallConstData.SelectedItem && StallConstData.SelectedItem.Item) {
					var pos:int = StallConstData.SelectedItem.Item.Pos;
					StallConstData.moneyAll = StallConstData.goodList[pos].price * StallConstData.goodList[pos].amount;
				} else {
					StallConstData.moneyAll = countMoneyAll();
				}
				showMoney(0);
			} else {									//宠物
				if(StallConstData.SelectedPet && StallConstData.SelectedPet.Id) {
					StallConstData.moneyAll = StallConstData.SelectedPet.Price;
				} else {
					StallConstData.moneyAll = countMoneyAll();
				}
				showMoney(0);
			}
		}
		
		/** 计算多个物品总价格 */
		private function countMoneyAll():Number
		{
			var moneyCount:Number = 0;
			for(var i:int = 0; i < StallConstData.goodList.length; i++) {
				if(StallConstData.goodList[i] && StallConstData.goodList[i].id) {
					var money:int = StallConstData.goodList[i].price * StallConstData.goodList[i].amount;
					moneyCount += money;
				}
			}
			for(var key:Object in StallConstData.petListSale) {
				moneyCount += StallConstData.petListSale[key].Price;
			}
			return moneyCount;
		}
		
		/** 界面上显示钱 0=总售价 1=携带银两*/
		public function showMoney(type:uint):void 
		{
			if(type == 0) {
				var moneyArr:Array = getMoney(StallConstData.moneyAll);
				var moneyStr:String = ""; 
				if(moneyArr[0] != 0) {
					moneyStr = moneyArr[0] + "\\ce" + moneyArr[1] + "\\cs" + moneyArr[2] + "\\cc";
				} else if(moneyArr[1] != 0) {
					moneyStr = moneyArr[1] + "\\cs" + moneyArr[2] + "\\cc";
				} else {
					moneyStr = moneyArr[2] + "\\cc";
				}
				stall.mcMoneyAll.txtMoney.text = moneyStr;
				ShowMoney.ShowIcon(stall.mcMoneyAll, stall.mcMoneyAll.txtMoney, true);
			} else {
				var moneyArrS:Array = getMoney(GameCommonData.Player.Role.BindMoney);
				var moneyStrS:String = ""; 
				if(moneyArrS[0] != 0) {
					moneyStrS = moneyArrS[0] + "\\ce" + moneyArrS[1] + "\\cs" + moneyArrS[2] + "\\cc";
				} else if(moneyArrS[1] != 0) {
					moneyStrS = moneyArrS[1] + "\\cs" + moneyArrS[2] + "\\cc";
				} else {
					moneyStrS = moneyArrS[2] + "\\cc";
				}
				stall.mcMoneySelf.txtMoney.text = moneyStrS;
				ShowMoney.ShowIcon(stall.mcMoneySelf, stall.mcMoneySelf.txtMoney, true);
			}
		}
	
		/** 将钱分解为 金、银、铜 数组 */
		public function getMoney(money:uint):Array
		{	
			var arr:Array = [];
			var jin:uint  = 0;
			var yin:uint  = 0;
			var tong:uint = 0;
			jin = money / 10000;
			yin = (money - jin * 10000) / 100;
			tong = money %100;
			arr.push(jin);
			arr.push(yin);
			arr.push(tong);
			return arr;
		}
		
		
		
	}
}

