package GameUI.Modules.Depot.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Depot.UI.DepotUIManager;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	/**
	 * 仓库物品扩充视图
	 * @
	 * @
	 */ 
	public class ItemExtendsMediator extends Mediator
	{
		public static const NAME:String = "ItemExtendstMediator";
		
		private var itemExtPanel:PanelBase = null;
		private var depotUIManager:DepotUIManager = null;
		
		public function ItemExtendsMediator()
		{
			super(NAME);
		}
		
		private function get itemExt():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				DepotEvent.SHOWITEMEXT,
				DepotEvent.REMOVEITEMEXT
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName()) {
				case DepotEvent.SHOWITEMEXT:
					if(!DepotConstData.itemExtIsOpen) {
						depotUIManager = DepotUIManager.getInstance();
						viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ItemExt");
						itemExtPanel = new PanelBase(itemExt, itemExt.width+7, itemExt.height+12);
						itemExtPanel.name = "itemExt";
						itemExtPanel.addEventListener(Event.CLOSE, itemExtCloseHandler);
						itemExtPanel.x = DepotConstData.EXTENDS_DEFAULT_POS.x;
						itemExtPanel.y = DepotConstData.EXTENDS_DEFAULT_POS.y;
						itemExtPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_dep_med_item_hand" ]);//"仓库扩充")
						depotUIManager.itemExt = itemExt;
						depotUIManager.initItemExt();
						GameCommonData.GameInstance.GameUI.addChild(itemExtPanel);
						initGrid();
						addLis();
						DepotConstData.itemExtIsOpen = true;
					}
					break;
				case DepotEvent.REMOVEITEMEXT:
					gc();
					break;
			}
		}
		
		private function initGrid():void
		{
			//快速购物
			for(var i:int = 0; i < 1; i++) {
				if(!(UIConstData.MarketGoodList[24] as Array)[i]) continue;
				var good:Object = (UIConstData.MarketGoodList[24] as Array)[i];
				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				unit.x = 37 * i + 20;
				unit.y = 47;
				unit.name = "goodQuickBuy"+i+"_"+good.type;
				itemExt.addChild(unit);
				
				var useItem:UseItem = new UseItem(i, good.type, itemExt);
				if(good.type < 300000) {
					useItem.Num = 1;
				}
				else if(good.type >= 300000) {
					useItem.Num = UIConstData.getItem(good.type).amount; 
				}
				useItem.x = unit.x + 2;
				useItem.y = unit.y + 2;
				useItem.Id = UIConstData.getItem(good.type).id;
				useItem.IsBind = 0;
				useItem.Type = good.type;
				useItem.IsLock = false;
				
				itemExt.addChild(useItem);
				
				itemExt["txtGoodNamePet_"+i].text = good.Name;
				itemExt["mcMoney_"+i].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
				ShowMoney.ShowIcon(itemExt["mcMoney_"+i], itemExt["mcMoney_"+i].txtMoney, true);
				itemExt["txtGoodNamePet_"+i].mouseEnabled = false;
				itemExt["mcMoney_"+i].mouseEnabled = false;
				itemExt["btnBuy_"+i].addEventListener(MouseEvent.CLICK, buyHandler);
			}
		}
		
		private function buyHandler(e:MouseEvent):void
		{
			var index:uint = uint(String(e.target.name).split("_")[1]);
			for(var i:int = 0; i < itemExt.numChildren; i++) {
				if(itemExt.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
					var type:uint = uint(itemExt.getChildAt(i).name.split("_")[1]);
					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
				}
			}
		}
		
		private function itemExtCloseHandler(e:Event):void
		{
			gc();
		}
		
		private function gc():void
		{
			removeLis();
			if(itemExtPanel && GameCommonData.GameInstance.GameUI.contains(itemExtPanel))GameCommonData.GameInstance.GameUI.removeChild(itemExtPanel);
			depotUIManager.itemExt = null;
			viewComponent = null;
			itemExtPanel = null;
			DepotConstData.itemExtIsOpen = false;
			facade.removeMediator(ItemExtendsMediator.NAME);
		}
		
		private function addLis():void
		{
//			itemExt.btnBuy_0.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		private function removeLis():void
		{
//			itemExt.btnBuy_0.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnBuy_0":
					facade.sendNotification(EventList.SHOWALERT, {comfrim:applyOpenItem, cancel:cancelAlert, info: GameCommonData.wordDic[ "mod_dep_med_item_btn" ], title:GameCommonData.wordDic[ "often_used_tip" ]});//"开启此6个仓库栏需要使用100元宝，是否开启？" 	"提 示"
					break;
			}
		}
		
		/** 确定开启仓库栏 */
		private function applyOpenItem():void
		{
			
		}
		/** 取消开启仓库栏 */
		private function cancelAlert():void
		{
			
		}
		
	}
}
