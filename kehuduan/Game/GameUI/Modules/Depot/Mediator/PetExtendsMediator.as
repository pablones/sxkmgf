package GameUI.Modules.Depot.Mediator
{
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
	 * 仓库宠物充视图
	 * @
	 * @
	 */ 
	public class PetExtendsMediator extends Mediator
	{
		public static const NAME:String = "PetExtendstMediator";
		
		private var petExtPanel:PanelBase = null;
		private var depotUIManager:DepotUIManager = null;
		
		public function PetExtendsMediator()
		{
			super(NAME);
		}
		
		private function get petExt():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				DepotEvent.SHOWPETEXT,
				DepotEvent.REMOVEPETEXT
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName()) {
				case DepotEvent.SHOWPETEXT:
					if(!DepotConstData.petExtIsOpen) {
						depotUIManager = DepotUIManager.getInstance();
						viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetExt");
						petExtPanel = new PanelBase(petExt, petExt.width-13, petExt.height+12);
						petExtPanel.name = "petExt";
						petExtPanel.addEventListener(Event.CLOSE, petExtCloseHandler);
						petExtPanel.x = DepotConstData.EXTENDS_DEFAULT_POS.x;
						petExtPanel.y = DepotConstData.EXTENDS_DEFAULT_POS.y;
						petExtPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_dep_med_pet_hand" ]);//"宠物栏扩充"
						depotUIManager.petExt = petExt;
						depotUIManager.initPetExt();
						initGrid();
						GameCommonData.GameInstance.GameUI.addChild(petExtPanel);
						DepotConstData.petExtIsOpen = true;
					}
					break;
				case DepotEvent.REMOVEPETEXT:
					gc();
					break;
			}
		}
		
		private function initGrid():void
		{
			//快速购物
			for(var i:int = 0; i < 8; i++) {
				if(!(UIConstData.MarketGoodList[25] as Array)[i]) continue;
				var good:Object = (UIConstData.MarketGoodList[25] as Array)[i];
				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				if(i < 4) {
					unit.x = i * (37+unit.width) + 23;
					unit.y = 49;
				} else {
					unit.x = (i-4) * (37+unit.width) + 23;
					unit.y = 157;
				}
				unit.name = "goodQuickBuy"+i+"_"+good.type;
				petExt.addChild(unit);
				
				var useItem:UseItem = new UseItem(i, good.type, petExt);
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
				
				petExt.addChild(useItem);
				
				petExt["txtGoodNamePet_"+i].text = good.Name;
				petExt["mcMoney_"+i].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
				ShowMoney.ShowIcon(petExt["mcMoney_"+i], petExt["mcMoney_"+i].txtMoney, true);
				petExt["txtGoodNamePet_"+i].mouseEnabled = false;
				petExt["mcMoney_"+i].mouseEnabled = false;
				petExt["btnBuy_"+i].addEventListener(MouseEvent.CLICK, buyHandler);
			}
		}
		
		private function buyHandler(e:MouseEvent):void
		{
			var index:uint = uint(String(e.target.name).split("_")[1]);
			for(var i:int = 0; i < petExt.numChildren; i++) {
				if(petExt.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
					var type:uint = uint(petExt.getChildAt(i).name.split("_")[1]);
					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
				}
			}
		}
		
		private function petExtCloseHandler(e:Event):void
		{
			gc();
		}
		
		private function gc():void
		{
			if(petExtPanel && GameCommonData.GameInstance.GameUI.contains(petExtPanel)) GameCommonData.GameInstance.GameUI.removeChild(petExtPanel);
			depotUIManager.petExt = null;
			viewComponent = null;
			petExtPanel = null;
			DepotConstData.petExtIsOpen = false;
			facade.removeMediator(PetExtendsMediator.NAME);
		}
		
		
	}
}