package GameUI.Modules.Depot.UI
{
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	
	import flash.display.MovieClip;
	
	/**
	 * 仓库UI基本管理
	 * @
	 * @
	 */ 
	public class DepotUIManager
	{
		private static var _instance:DepotUIManager;
		
		public var itemDepot:MovieClip;		/** 物品仓库面板 */
		public var itemExt:MovieClip;		/** 物品仓库扩充面板 */
		public var petDepot:MovieClip; 		/** 宠物仓库面板 */
		public var petExt:MovieClip;		/** 宠物仓库扩充面板 */
		public var moneyInput:MovieClip		/** 金钱输入面板 */
		
		public function DepotUIManager(s:Singleton)
		{
			
		}
		
		public static function getInstance():DepotUIManager
		{
			if(!_instance) _instance = new DepotUIManager(new Singleton());
			return _instance;
		}
		
		/** 初始化物品仓库 */
		public function initItemDepot():void
		{
			itemDepot.mcItemPage_0.gotoAndStop(1);	//关闭状态
			itemDepot.mcItemPage_1.gotoAndStop(2);
			itemDepot.mcItemPage_2.gotoAndStop(2);
			itemDepot.mcMoneyDepot.mouseEnabled = false;
			itemDepot.mcItemPage_0.visible = true;	//不可见
			itemDepot.mcItemPage_1.visible = false;
			itemDepot.mcItemPage_2.visible = false;
			itemDepot.mcItemPage_0.buttonMode    = true;
			itemDepot.mcItemPage_0.useHandCursor = true;
			itemDepot.mcItemPage_1.buttonMode 	 = true;
			itemDepot.mcItemPage_1.useHandCursor = true;
			itemDepot.mcItemPage_2.buttonMode 	 = true;
			itemDepot.mcItemPage_2.useHandCursor = true;
			itemDepot.mcNotOpen_1.buttonMode 	 = true;
			itemDepot.mcNotOpen_1.useHandCursor  = true;
			itemDepot.mcNotOpen_2.buttonMode 	 = true;
			itemDepot.mcNotOpen_2.useHandCursor  = true;
			itemDepot.mcMoneyDepot.txtMoney.text = "0\\cc";
			ShowMoney.ShowIcon(itemDepot.mcMoneyDepot, itemDepot.mcMoneyDepot.txtMoney, true);
		}
		
		/** 点击仓库页按钮 */
		public function setPageBtnModel(btnIndex:int):void
		{
			switch(btnIndex) {
				case 0:
					itemDepot.mcItemPage_0.gotoAndStop(1);
					itemDepot.mcItemPage_1.gotoAndStop(2);
					itemDepot.mcItemPage_2.gotoAndStop(2);
					break;
				case 1:
					itemDepot.mcItemPage_0.gotoAndStop(2);
					itemDepot.mcItemPage_1.gotoAndStop(1);
					itemDepot.mcItemPage_2.gotoAndStop(2);
					break;
				case 2:
					itemDepot.mcItemPage_0.gotoAndStop(2);
					itemDepot.mcItemPage_1.gotoAndStop(2);
					itemDepot.mcItemPage_2.gotoAndStop(1);
					break;
			}
		}
		
		/** 设置仓库页数 */
		public function setPageNum():void
		{
			var gridCount:int = DepotConstData.gridCount;
			if(gridCount > 72) {
				itemDepot.mcItemPage_0.visible = true;
				itemDepot.mcItemPage_1.visible = true;
				itemDepot.mcItemPage_2.visible = true;
			} else if(gridCount > 36) {
				itemDepot.mcItemPage_0.visible = true;
				itemDepot.mcItemPage_1.visible = true;
				itemDepot.mcItemPage_2.visible = false;
			} else {
				itemDepot.mcItemPage_0.visible = true;
				itemDepot.mcItemPage_1.visible = false;
				itemDepot.mcItemPage_2.visible = false;
			}
		}
		
		/** 初始化宠物仓库 */
		public function initPetDepot():void
		{
			petDepot.txtPetNumDepot.mouseEnabled = false;
			petDepot.txtPetNumSelf.mouseEnabled = false;
			petDepot.txtInOrOut.mouseEnabled = false;
		}
		
		/** 初始化物品扩充 */
		public function initItemExt():void
		{
//			itemExt.txtExtInfo.mouseEnabled = false;
//			itemExt.txtCost_0.mouseEnabled = false;
		}
		
		/** 初始化宠物扩充 */
		public function initPetExt():void
		{
//			petExt.txtExtInfo.mouseEnabled = false;
//			petExt.txtCost_0.mouseEnabled = false;
//			petExt.txtCost_1.mouseEnabled = false;
		}
		
		/** 显示仓库钱数 */
		public function refreshMoney():void
		{
			var moneyArr:Array = UIUtils.getMoney(DepotConstData.moneyDepot);
			var moneyStr:String = ""; 
			if(moneyArr[0] != 0) {
				moneyStr = moneyArr[0] + "\\ce" + moneyArr[1] + "\\cs" + moneyArr[2] + "\\cc";
			} else if(moneyArr[1] != 0) {
				moneyStr = moneyArr[1] + "\\cs" + moneyArr[2] + "\\cc";
			} else {
				moneyStr = moneyArr[2] + "\\cc";
			}
			itemDepot.mcMoneyDepot.txtMoney.text = moneyStr;
			ShowMoney.ShowIcon(itemDepot.mcMoneyDepot, itemDepot.mcMoneyDepot.txtMoney, true);
		}
		
		/** 加锁/解锁 物品仓库按钮 false=加锁 ，true=解锁 */
		public function lockItemDptBtn(val:Boolean):void
		{
			itemDepot.mcItemPage_0.mouseEnabled = val;
			itemDepot.mcItemPage_1.mouseEnabled = val;
			itemDepot.mcItemPage_2.mouseEnabled = val;
			itemDepot.btnMoneyIn.mouseEnabled 	= val;
			itemDepot.btnMoneyOut.mouseEnabled  = val;
			itemDepot.mcNotOpen_1.mouseEnabled  = val;
			itemDepot.mcNotOpen_2.mouseEnabled  = val;
		}
		
		public function gc():void
		{
			itemDepot  = null;
			itemExt    = null;
			petDepot   = null;
			petExt     = null;
			moneyInput = null;
		}
		
		
	}
}
class Singleton {}
