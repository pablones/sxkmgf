package GameUI.Modules.Maket.Proxy
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Maket.Mediator.MarketMediator;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.items.UseItem;
	
	import Net.ActionSend.MarketSend;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class MarketGridManager extends Proxy
	{
		public static const NAME:String = "MarketGridManager";
		
		private var redFrame:MovieClip = null;
		private var useItemList:Array ;
//		private var yellowFrame:MovieClip = null;
		private var curGood:Object;		//购买的物品
		private var curPayType:int;		//购买方式
		private var curCount:int;		//购买数量
		
		public function MarketGridManager()
		{
			super(NAME);
			init();
		}
		
		private function init():void
		{
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			redFrame.name = "redFrame";
			redFrame.mouseEnabled = false;
//			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
//			yellowFrame.name = "yellowFrame";
		
			for( var i:int = 0; i < MarketConstData.GridUnitList.length; i++ )
			{
				var gridUint:GridUnit = MarketConstData.GridUnitList[i] as GridUnit;
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
				gridUint.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				gridUint.Grid.addEventListener(MouseEvent.CLICK, onMouseClick);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			event.currentTarget.parent.addChild(redFrame);
			redFrame.x = event.currentTarget.x;
			redFrame.y = event.currentTarget.y; 		
		}
		
		private function onMouseOut(event:MouseEvent):void
		{
    		if(event.currentTarget.parent.getChildByName("redFrame")) 
    		{
    			event.currentTarget.parent.removeChild(event.currentTarget.parent.getChildByName("redFrame"));
    		}
		}
		
		private function onMouseClick(event:MouseEvent):void
		{
			curCount = 1;
			if(event.target.parent)
			{
				curCount = event.target.parent.count;
			}
			curCount = Math.max(curCount , 1);
			// 是否使用珠宝
			var isUseJewelry:Boolean = true;
			// 是否需要提示
			var isNeedPrompt:Boolean = true;
			var ybSelf:Number = GameCommonData.Player.Role.UnBindRMB;	//自己的元宝
			var zbSelf:Number = GameCommonData.Player.Role.BindRMB;		//自己的珠宝
			var dqSelf:Number = GameCommonData.Player.Role.GiveAway;	//自己的点券
			var name0:String = event.target.name.split("_")[0];
			var index:int = int(event.target.name.split("_")[1]);
			switch(name0)
			{
				case "MarketSaleItem":
					curGood = MarketConstData.curPageData[index];
					break;
				case "MarketPrivilegeItem":
					curGood = MarketConstData.specialDiscount[index];
					if(curGood.leftNum < 1)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketGridManager_onMouseClick_1" ]/**"此物品已售完"*/, color:0xffff00});
						return ;
					}
					break;
			}
			
			var marketMediator:MarketMediator = facade.retrieveMediator(MarketMediator.NAME) as MarketMediator;
			isUseJewelry = marketMediator.isUseJewelry;
			isNeedPrompt = marketMediator.isNeedPrompt;
			
			if(curGood)
			{
				var PayTypeArr:Array = curGood.PayType;
				curPayType = PayTypeArr[0];
				if(isUseJewelry && PayTypeArr.indexOf(1) > -1)
				{
					curPayType = 1;
				}
				
				var buyable:Boolean = true;
				if(1 == curPayType && zbSelf < curGood.PriceIn * curCount)
				{
					curPayType = 0;
				}
				if(0 == curPayType && ybSelf < curGood.PriceIn * curCount)
				{
					buyable = false;
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_1" ], color:0xffff00});
				}
				else if(2 == curPayType && dqSelf < curGood.PriceIn * curCount)
				{
					buyable = false;
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_3" ], color:0xffff00});
				}
				
//				switch(curPayType)
//				{
//					case 0:
//						if( ybSelf < curGood.PriceIn)
//						{
//							buyable = false;
//							sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_1" ], color:0xffff00});         //元宝不足
//						}
//						break;
//					case 1:
//						if( zbSelf < curGood.PriceIn)
//						{
//							buyable = false;
//							sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_2" ], color:0xffff00});         //珠宝不足
//						}
//						break;
//					case 2:
//						if( dqSelf < curGood.PriceIn)
//						{
//							buyable = false;
//							sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_3" ], color:0xffff00});         //点券不足
//						}
//						break;
//				}
				
				if(buyable)
				{
					if(isNeedPrompt)
					{
						var color:uint = UIConstData.getItem(curGood.type).Color; 
						var str:String = '<font color="#E2CCA5">'+ GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketGridManager_onMouseClick_2" ] /**'是否消费'*/ + '<font color="#00ff00">' + curGood.PriceIn * curCount + MarketConstData.payWayStrList[curPayType] +'</font>，'+ GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketGridManager_onMouseClick_3" ]/**'购买'*/ + '<font color="#00ff00">' + curCount + '</font>' + GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketGridManager_onMouseClick_4" ]/**'个'*/ + '<font color="' + IntroConst.itemColors[color] + '">' + curGood.Name + '</font>' + '。</font>';
						
						facade.sendNotification(EventList.SHOWALERT, {comfrim:sureToBuy, cancel:cancelFun, info:str, title:GameCommonData.wordDic[ "mod_team_med_teamm_isc_2" ], worldMap:1});    //      提 示
						var alert:DisplayObject = GameCommonData.GameInstance.TooltipLayer.getChildByName("Alert");
						alert.x = event.stageX;
						alert.y = event.stageY;
					}
					else
					{
						sureToBuy();
					}
				}
			}
			
			if(curGood.type == 610046 && NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_WDQK_MARKET_NOTICE_NEWER_HELP);
		}
		
		private function sureToBuy():void
		{
			var obj:Object = {type: curGood.type,buyNum:curCount,payWay:curPayType,leftNum:curGood.leftNum};
			MarketSend.buyDiscoutProduct(obj);
		}
		
		private function cancelFun():void
		{
			
		}
		
		/** 把物品放到购物篮，index为物品排序编号 */
		public function buyGoods(type:int):void
		{
			var l:Array = MarketConstData.curPageData;			
			var good:Object = null;
			var i:int = 0;
			for( i = 0; i < l.length ; i++ )
			{
				if( type == (l[i] as Object).type )
				{
					good = l[i] ;
				}
			}
			if( good != null ) {
				var car:Array = MarketConstData.shopCarData;
				for(i = 0; i < MarketConstData.shopCarData.length; i++) {
					if(good.type == MarketConstData.shopCarData[i].type && good.PriceIn == MarketConstData.shopCarData[i].PriceIn ) {
						sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_pro_mar_onm_1" ], color:0xffff00});        //购物车中已有此商品
						return;
					}
				}
				if(MarketConstData.shopCarData.length >= 10) {
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_pro_mar_onm_2" ], color:0xffff00});             //购物车已满
					return;
				}
				good.buyNum = 1;
				good.payWay = (good.PayType as Array)[0];
				
				var upperLimit:uint = UIConstData.getItem(good.type).UpperLimit; 
				if(good.type < 300000) {		//如果是装备则叠加上限=1
					upperLimit = 1;
				}
				good.UpperLimit = upperLimit	//物品叠加上限，用于限制输入数量
				MarketConstData.shopCarData.push(good);
				sendNotification(MarketEvent.ADDTO_SHOPCAR_MARKET);
				if(good.type == 610046 && NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_WDQK_MARKET_NOTICE_NEWER_HELP);
			}
		}
		
		public function showItems(list:Array):void
		{
			if(MarketConstData.GridUnitList.length == 0) return;
			removeAllItem();
			useItemList = new Array();
			for(var j:int = 0; j < MarketConstData.curMaxGoodsNum; j++) {
				var good:Object = list[j];
				if(good) {
					MarketConstData.GridUnitList[j].HasBag = true;
					var useItem:UseItem = new UseItem(good.index, good.type, MarketConstData.GridUnitList[j].parent);
					good.type < 300000 ? useItem.Num = 1 : useItem.Num = good.amount;
					useItem.x = 8;
					useItem.y = 10;
					useItem.Id = good.id;
					useItem.IsBind = good.isBind;
					useItem.Type = good.type;
					useItem.IsLock = false;
					MarketConstData.GridUnitList[j].Item = useItem;
					MarketConstData.GridUnitList[j].IsUsed = true;
					MarketConstData.GridUnitList[j].parent.addChild(useItem);
					MarketConstData.GridUnitList[j].Grid.mouseEnabled = true;
					useItemList.push(useItem);
				} else {
					MarketConstData.GridUnitList[j].Grid.mouseEnabled = false;
				}
			}
		}
		
		public function removeAllItem():void
		{
			var i:int;
			for( i = 0; i < MarketConstData.curMaxGoodsNum; i++ ) 
			{
				if(MarketConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame")) 
	    		{
	    			MarketConstData.GridUnitList[i].Grid.parent.removeChild(MarketConstData.GridUnitList[i].Grid.parent.getChildByName("redFrame"));
	    		}
				MarketConstData.GridUnitList[i].Item = null;
				MarketConstData.GridUnitList[i].IsUsed = false;
			}
			
			for( i = 0; useItemList && i < useItemList.length ; i++)
			{
				if( useItemList[i])
				{
					MarketConstData.GridUnitList[i].Item = null;
					useItemList[i].parent.removeChild(useItemList[i]);
				}
			}
			
			while(useItemList && useItemList.length) useItemList.pop();
			useItemList = null;
		}
		
	}
}