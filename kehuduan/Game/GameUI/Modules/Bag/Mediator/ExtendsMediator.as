package GameUI.Modules.Bag.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ExtendsMediator extends Mediator
	{
		public static const NAME:String = "ExtendsMediator";
		private var panelBase:PanelBase = null;
		
		public function ExtendsMediator()
		{
			super(NAME);
		}
		
		private function get extendsBag():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				BagEvents.SHOWEXTENDS,
				BagEvents.REMOVE_BAG_EXTENDS
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case BagEvents.SHOWEXTENDS:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.EXTENDBAG});
					panelBase = new PanelBase(extendsBag, extendsBag.width-13, extendsBag.height+12);
					panelBase.name = "ExtendsBag";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					var point:Point = UIUtils.getMiddlePos(panelBase); 
					panelBase.x =  point.x - 70;//(GameCommonData.GameInstance.GameUI.width - panelBase.width)/2;
					panelBase.y = point.y;//(GameCommonData.GameInstance.GameUI.height - panelBase.height)/2;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_bag_med_ext_han_1" ] );   // 背包扩充
					initGrid();
					GameCommonData.GameInstance.GameUI.addChild(panelBase); 
					BagData.ExtendIsOpen = true;
				break;
				case BagEvents.REMOVE_BAG_EXTENDS:
					if(BagData.ExtendIsOpen) {
						panelCloseHandler(null);
					}
				break;
			}
		}
		
		private function initGrid():void
		{
			//快速购物
			var dicIndex:uint = 0;
			if(BagData.SelectIndex == 0) {
				dicIndex = 23;
			} else if(BagData.SelectIndex == 1) {
				dicIndex = 27;
			} else {
				dicIndex = 28;
			}
			if(dicIndex == 0) return;
			for(var i:int = 0; i < 4; i++) {
				if(!(UIConstData.MarketGoodList[dicIndex] as Array)[i]) continue;
				var good:Object = (UIConstData.MarketGoodList[dicIndex] as Array)[i];
				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				if(i < 4) {
					unit.x = i * (37+unit.width) + 23;
					unit.y = 49;
				} else {
					unit.x = (i-4) * (37+unit.width) + 23;
					unit.y = 157;
				}
				unit.name = "goodQuickBuy"+i+"_"+good.type;
				extendsBag.addChild(unit);
				
				var useItem:UseItem = new UseItem(i, good.type, extendsBag);
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
				
				extendsBag.addChild(useItem);
				
				extendsBag["txtGoodNamePet_"+i].text = good.Name;
				extendsBag["mcMoney_"+i].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
				ShowMoney.ShowIcon(extendsBag["mcMoney_"+i], extendsBag["mcMoney_"+i].txtMoney, true);
				extendsBag["txtGoodNamePet_"+i].mouseEnabled = false;
				extendsBag["mcMoney_"+i].mouseEnabled = false;
				extendsBag["btnBuy_"+i].addEventListener(MouseEvent.CLICK, buyHandler);
			}
		}
		
		private function buyHandler(e:MouseEvent):void
		{
			var index:uint = uint(String(e.target.name).split("_")[1]);
			for(var i:int = 0; i < extendsBag.numChildren; i++) {
				if(extendsBag.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
					var type:uint = uint(extendsBag.getChildAt(i).name.split("_")[1]);
					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
				}
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			BagData.ExtendIsOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			panelBase = null;
			this.viewComponent = null;
			facade.removeMediator(NAME);
		}
	}
}