package GameUI.Modules.PetPlayRule.PetRuleController.UI
{
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	import flash.text.TextField;
	
	public class PetRuleUIManager
	{
		private var panelView:MovieClip;
		
		public function PetRuleUIManager(view:MovieClip)
		{
			this.panelView = view;
			init();
			showMoney(0, 0);
		}
		
		private function init():void
		{
			for(var i:int = 0; i < 3; i++) {
				panelView["txtGoodNamePet_"+i].mouseEnabled = false;
				panelView["mcMoney_"+i].mouseEnabled = false;
				panelView.mcCost.mouseEnabled = false;
				panelView.mcSuiYin.mouseEnabled = false;
				panelView.mcYinLiang.mouseEnabled = false;
				(panelView["txtInput_"+i] as TextField).restrict = "0-9";
			}
			for(var j:int = 0; j < PetRuleControlMediator.MAXPAGE; j++) {
				if(panelView.getChildByName("txtPage_"+j)) {
					panelView["txtPage_"+j].mouseEnabled = false;
				}
			}
			panelView.mcAlphaBack.mouseEnabled = false;
			clearData();
			showMoney(1);
			showMoney(2);
		}
		
		/** 显示钱 0-需要花费，1-携带碎银，2-携带银两 */
		public function showMoney(type:int, money:int = 0):void
		{
			switch(type) {
				case 0:
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(money, ["\\se","\\ss","\\sc"]);
					panelView.mcCost.txtMoney.text = moneyStrNeed;
					ShowMoney.ShowIcon(panelView.mcCost, panelView.mcCost.txtMoney, true);
					break;
				case 1:
					var moneyStrSuiYin:String = UIUtils.getMoneyStandInfo(Number(GameCommonData.Player.Role.UnBindMoney), ["\\se","\\ss","\\sc"]);
					panelView.mcSuiYin.txtMoney.text = moneyStrSuiYin;
					ShowMoney.ShowIcon(panelView.mcSuiYin, panelView.mcSuiYin.txtMoney, true);
					break;
				case 2:
					var moneyStrYinLiang:String = UIUtils.getMoneyStandInfo(Number(GameCommonData.Player.Role.BindMoney), ["\\ce","\\cs","\\cc"]);
					panelView.mcYinLiang.txtMoney.text = moneyStrYinLiang;
					ShowMoney.ShowIcon(panelView.mcYinLiang, panelView.mcYinLiang.txtMoney, true);
					break;
			}
		}
		
		/** 清除所有数据 */
		public function clearData():void
		{
			for(var i:int = 0; i < 3; i++) {
				panelView["txtGoodNamePet_"+i].text = "";
				panelView["mcMoney_"+i].text = "1";
				panelView["txtInput_"+i].text = "1";
			}
		}

	}
}