package GameUI.Modules.Maket.Mediator
{
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.SoundList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Maket.Proxy.MarketGridManager;
	import GameUI.Modules.Maket.Proxy.MarketNetAction;
	import GameUI.Modules.Maket.UI.MarketPrivilegeManager;
	import GameUI.Modules.Maket.UI.MarketUIManager;
	import GameUI.Modules.Maket.UI.MarketUtils;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.Sound.SoundManager;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionProcessor.MarketAction;
	import Net.ActionSend.MarketSend;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.utils.Timer;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MarketMediator extends Mediator
	{
		public static const NAME:String = "MarketMediator";
		private static const MAXPAGE:uint = 9;
		
		private var panelBase:PanelBase;
		private var dataProxy:DataProxy;
		private var gridManager:MarketGridManager;
		private var uiManager:MarketUIManager;
		private var privilegeManager:MarketPrivilegeManager;
		
		private var curPage:uint = 0;
//		private var curPage:uint = 8;
		private var pageCount:uint = 0; 
		
		private var firstReqTime:uint;
		private var newReqTime:uint;
		
		private var aTodayDisGoods:Array;
		private var allMarketGoodsType:Array = [];						//商城里所有出卖的物品
		private var allMarketGoodsObj:Array = [];
		
		/** 是否使用珠宝 */
		private var _isUseJewelry:Boolean = true;

		/** 是否需要提示 */
		private var _isNeedPrompt:Boolean = true;
		
		/** 搜索文本中的内容 */
		private var searchText:String = "";
		
		/** 商城美女 */
		private var belle:Bitmap;
		private var sexLoader:ImageItem;
		
		/** 外部接口CD */
		private var buyCDTimerOutSide:Timer;
		
		public function MarketMediator()
		{
			super(NAME);
		}
		
		private function get marketView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWMARKETVIEW,
				EventList.CLOSEMARKETVIEW,
				MarketEvent.ADDTO_SHOPCAR_MARKET,
				MarketEvent.BUY_ITEM_MARKET,
				MarketEvent.UPDATE_MONEY_MARKET,
				EventList.UPDATEMONEY,
				MarketEvent.MARKET_STOP_DROG,
				MarketEvent.REC_MARKET_DIS_GOOD,
				MarketEvent.UPDATE_MARKET_GOODS_NUM,
				MarketEvent.UPDATE_COUPONS_BUTTON
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:											//初始化
					initView();
					buyCDTimerOutSide = new Timer(1000, 1);
					for ( var j:uint=0; j<8; j++ )
					{
						if ( !UIConstData.MarketGoodList[j] ) continue; 
						for ( var k:uint=0; k<UIConstData.MarketGoodList[j].length; k++ )
						{
							allMarketGoodsType.push( UIConstData.MarketGoodList[j][k].type );
							allMarketGoodsObj.push( UIConstData.MarketGoodList[j][k] );
						}
					}
					break;
				case EventList.SHOWMARKETVIEW:										//打开商店(可以直接打开到指定商品页签)
					MarketNetAction.queryYB();
					requestCountData();
					var pageIndex:Object = notification.getBody();
					showView(pageIndex); 
//					showView( 8 );
					addLis();
					if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.OPEN_MARKET_NOTICE_NEWER_HELP);	//通知新手引导
//					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					SoundManager.playSoundCanInterrupt(SoundList.OPEN_MARKET_SOUND);
					break; 
				case EventList.CLOSEMARKETVIEW:										//关闭
					panelCloseHandler(null);
					break;
//				case MarketEvent.ADDTO_SHOPCAR_MARKET:								//添加商品到购物车
//					uiManager.addItem();
//					uiManager.showMoney(0);
//					break;
				case MarketEvent.BUY_ITEM_MARKET:									//购买某物品，外部接口
					if(buyCDTimerOutSide.running) {
//						sendNotification(HintEvents.RECEIVEINFO, {info:"请稍后", color:0xffff00});
						return;
					}
					var goodToBuy:Object = notification.getBody();
					if(goodToBuy) {
						buyItem(goodToBuy);
					}
					break;
				case EventList.UPDATEMONEY:
					if(dataProxy.MarketIsOpen)
						uiManager.showMoney(1);
					break;
				case MarketEvent.UPDATE_MONEY_MARKET:
					if(dataProxy.MarketIsOpen)
						uiManager.showMoney(1);
					break;
				case MarketEvent.MARKET_STOP_DROG:									//背包禁止拖动是还原位置	
					if(panelBase) {
						panelBase.x = UIConstData.DefaultPos1.x;
						panelBase.y = UIConstData.DefaultPos1.y;
						panelBase.IsDrag = false;
					}
					break;
				case MarketEvent.REC_MARKET_DIS_GOOD:
					var aDiscountGoods:Array = notification.getBody() as Array; 
					recDisGood( aDiscountGoods );
					break;
				case MarketEvent.UPDATE_MARKET_GOODS_NUM:
					var aNumGoods:Array = notification.getBody() as Array; 
					updateGood( aNumGoods );
					break;
				case MarketEvent.UPDATE_COUPONS_BUTTON:
					uiManager.setCouponsOpen( notification.getBody() as Boolean );
					break;
			}
		}
		
		//更新限量商品
		private function updateGood( arr:Array ):void
		{
			var i:int;
			var j:uint;
			var k:uint;
			var m:uint;
			var goodObj:Object;
			for(i = 0; i < MarketConstData.specialGoods.length; i++)
			{
				goodObj = MarketConstData.specialGoods[i];
				for(m = 0; m < arr.length; m++)
				{
					if(goodObj.type == arr[m].type)
					{
						goodObj.leftNum = arr[m].amount;
						arr.splice(m,1);
						break;
					}
				}
			}
			for ( m=0; m < arr.length; m++ )
			{
				for ( j = 0; j<8; j++ )
				{
					if ( !UIConstData.MarketGoodList[j] ) continue; 
					for ( k = 0; k<UIConstData.MarketGoodList[j].length; k++ )
					{
						goodObj = UIConstData.MarketGoodList[j][k];
						if ( goodObj.type == arr[m].type )
						{
							goodObj.leftNum = arr[m].amount;
							goodObj.oldPrice = int( goodObj.PriceIn / goodObj.SalePercent );
							MarketConstData.specialGoods.unshift(UIConstData.MarketGoodList[j].splice(k,1)[0]);
						}
					}
				}
			}
			showCurData();
		}
		
		private function initView():void
		{
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.MARKETVIEW});
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			panelBase = new PanelBase(marketView, marketView.width + 8, marketView.height + 12);
			panelBase.name = "MarketPanel";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketMediator_initView" ]/**"商   城"*/);
			facade.registerMediator(new MarketPreviewMediator());
			
			MarketConstData.view = marketView;
			
			uiManager = new MarketUIManager(marketView);

			privilegeManager = new MarketPrivilegeManager(marketView["marketPrivilegeView"]);

			gridManager = new MarketGridManager();
			
			facade.registerProxy(gridManager);
			
			GameCommonData.GameInstance.TooltipLayer.addEventListener(MouseEvent.MOUSE_DOWN, removeCombox);
//			GameCommonData.GameInstance.GameUI.addEventListener(MouseEvent.MOUSE_DOWN, removeCombox);

			this.isUseJewelry = true;
			this.isNeedPrompt = true;
			
			showMoney1();
		} 
		
		//请求打折商品信息
		private function requestCountData():void
		{
			if ( firstReqTime == 0 )
			{
				sendDataToServer();
				firstReqTime = getTimer();
			}
			else
			{
				newReqTime = getTimer();
				if ( newReqTime - firstReqTime > 900 )
				{
					sendDataToServer();
					firstReqTime = newReqTime;
				}
			}
		}
		
		private function sendDataToServer():void
		{
			MarketSend.inquiresDiscount();
		}
		
		//处理收到的打折物品数据
		private function recDisGood( aGoods:Array ):void
		{
			aTodayDisGoods = [];
			MarketConstData.aTotalDisGoods = [];
			aTodayDisGoods = aGoods.concat();
			var obj:Object;
			for ( var i:uint=0; i<aTodayDisGoods.length; i++ )
			{
				var index:int = allMarketGoodsType.indexOf( aTodayDisGoods[i].type );
				if ( index > -1 )
				{
					obj = new Object();
//					obj = allMarketGoodsObj[ index ];
					
					obj.type = allMarketGoodsObj[ index ].type;
					obj.Name = allMarketGoodsObj[ index ].Name;
					obj.PayType = allMarketGoodsObj[ index ].PayType;
					obj.SalePercent = allMarketGoodsObj[ index ].SalePercent;
					obj.Hot = allMarketGoodsObj[ index ].Hot;
					obj.New = allMarketGoodsObj[ index ].New;
					obj.LimitTime = allMarketGoodsObj[ index ].LimitTime;
					obj.oldPrice = allMarketGoodsObj[ index ].PriceIn;    //原价
					
					obj.PriceIn = aTodayDisGoods[ i ].price;					//打折后的价格
					obj.leftNum = aTodayDisGoods[ i ].amount;
					MarketConstData.aTotalDisGoods.push( obj );
				}
			}
			/** 更新打折物品 */
			MarketConstData.specialDiscount = MarketUtils.addArray(MarketConstData.specialGoods, MarketConstData.aTotalDisGoods);
			privilegeManager.goodsList = MarketConstData.specialDiscount;
			
			if ( MarketConstData.curPageIndex == 8 )
			{
				MarketConstData.curPageData = MarketConstData.aTotalDisGoods;
				uiManager.showCountData();
				gridManager.showItems(MarketConstData.curPageData); 
			}
		}
		
		private function removeCombox(e:MouseEvent):void
		{
			MarketUtils.hideAllCombox();
		}
		
		private function showView(pageIndex:Object):void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			if(!dataProxy.MarketIsOpen)
			{
				if( GameCommonData.fullScreen == 2 )
				{
					panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth-GameConfigData.GameWidth)/2 + 160;
					panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight-GameConfigData.GameHeight)/2 + 20;
				}else{
					panelBase.x = UIConstData.DefaultPos1.x + 160;
					panelBase.y = UIConstData.DefaultPos1.y + 25;
				}
			}
			
			if(2 == GameCommonData.wordVersion && 7 == MarketConstData.curPageIndex)
			{
				MarketConstData.curPageIndex = 0;
			}
			
			uiManager.showMoney(1);
			
			MarketConstData.curPageIndex = (pageIndex == null) ? 0 : uint(pageIndex);
			
			setCurGoodsNum(MarketConstData.curPageIndex);
			
			for(var i:int = 0; i < MAXPAGE; i++) {
				marketView["mcPage_"+i].buttonMode = true;
				marketView["mcPage_"+i].gotoAndStop(2);
				marketView["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			marketView["mcPage_"+MarketConstData.curPageIndex].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			marketView["mcPage_"+MarketConstData.curPageIndex].gotoAndStop(1);
			
			marketView.mcPage_8.visible = false;
			marketView.txtPage_8.visible = false;
//			if ( MarketConstData.curPageIndex == 8 ) return;
			
			initPage();
			showCurData();
			
			if(7 == MarketConstData.curPageIndex)
			{
				showMoney2();
			}
			else
			{
				showMoney1();
			}
			
			//加载商城美女
			if(!MarketConstData.isLoadBelle)
			{
				loadBelle();
			}
			
			dataProxy.MarketIsOpen = true;
		}
		
		/** 选择不同商品页签 */
		private function choicePageHandler(event:MouseEvent):void
		{
			var index:uint = uint(event.target.name.split("_")[1]);
			if(index == MarketConstData.curPageIndex) return;
			
			setCurGoodsNum(index);
			
			for(var i:int = 0; i < MAXPAGE; i++) {
				marketView["mcPage_"+i].gotoAndStop(2);
				marketView["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			marketView["mcPage_"+index].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			MarketConstData.curPageIndex = index;
			marketView["mcPage_"+index].gotoAndStop(1);
			if(index == 7 && NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_DQ_MARKET_NOTICE_NEWER_HELP);
			initPage();
			showCurData();
			
			if(7 == MarketConstData.curPageIndex)
			{
				showMoney2();
			}
			else
			{
				showMoney1();
			}
		}
		
		/** 设置当前页最多可呈现的物品数 */
		private function setCurGoodsNum(index:int):void
		{
			if(index == 0)
			{
				MarketConstData.curMaxGoodsNum = 10;
				marketView["marketPrivilegeView"].visible = true;
			}	
			else
			{
				MarketConstData.curMaxGoodsNum = 15;
				marketView["marketPrivilegeView"].visible = false;
			}
		}
		
		private function initPage():void
		{
			curPage = 0;
			if(8 == MarketConstData.curPageIndex)
			{
				pageCount = Math.ceil( MarketConstData.searchGoods.length / MarketConstData.curMaxGoodsNum );
			}
			else
			{
				pageCount = Math.ceil( (UIConstData.MarketGoodList[MarketConstData.curPageIndex] as Array).length / MarketConstData.curMaxGoodsNum );
			}
			if(pageCount > 0) {	
				uiManager.clearData();
				curPage++;
				gridManager.removeAllItem();
				showCurData();
				marketView.txtPageInfo.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + curPage+"/"+pageCount + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第" "页"
			} else {
				uiManager.clearData();
				gridManager.removeAllItem();
				marketView.txtPageInfo.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + "1/1" + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第" "页"
			}
		}
		
		private function showCurData():void
		{
			MarketConstData.curPageData = [];
			var fromIndex:int = (curPage-1) * MarketConstData.curMaxGoodsNum;
			//打折商品特殊处理
			if ( MarketConstData.curPageIndex == 8 )
			{
				MarketConstData.curPageData = MarketConstData.searchGoods.slice(fromIndex,fromIndex + MarketConstData.curMaxGoodsNum);
			}else
			{
				for(var i:int = 0; i < MarketConstData.curMaxGoodsNum; i++) {
					var index:uint = fromIndex + i;
					if( UIConstData.MarketGoodList[MarketConstData.curPageIndex] && (UIConstData.MarketGoodList[MarketConstData.curPageIndex] as Array)[index] ) {
						MarketConstData.curPageData[i] = (UIConstData.MarketGoodList[MarketConstData.curPageIndex] as Array)[index];
					}
				}
			}
			uiManager.showData();
			gridManager.showItems(MarketConstData.curPageData); 
			MarketConstData.specialDiscount = MarketUtils.addArray(MarketConstData.specialGoods, MarketConstData.aTotalDisGoods);
			privilegeManager.goodsList = MarketConstData.specialDiscount;
		}
		
		private function panelCloseHandler(e:Event):void 
		{
			gcAll();	
		}
		
		private function gcAll():void
		{
			
			sendNotification(MarketEvent.CLOSE_MARKET_PREVIEW);
			removeLis();
			
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				panelBase.IsDrag = true;
			}
			curPage = 0;
			pageCount = 0;
			MarketConstData.curPageIndex = 0;
			MarketConstData.curPageData = [];
			MarketConstData.shopCarData = [];
			MarketConstData.comBoxList = [];
			uiManager.clearData();
//			uiManager.clearShopCar();
			gridManager.removeAllItem();
			dataProxy.MarketIsOpen = false;
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLOSE_MARKET_NOTICE_NEWER_HELP);		//通知新手引导
			SoundManager.interruptSound(SoundList.OPEN_MARKET_SOUND);
			sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
		}
		
		private function addLis():void
		{
			UIUtils.addFocusLis(marketView.txt_market_search);
			UIConstData.FocusIsUsing = true;
			
			marketView.txt_market_search.addEventListener(KeyboardEvent.KEY_UP,onKeyUp);
			
			marketView.mc_market_useJewelry.addEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.mc_market_needPrompt.addEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btnFront.addEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btnNext.addEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btnDeposit.addEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btn_market_search.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function removeLis():void
		{
						
			UIUtils.removeFocusLis(marketView.txt_market_search);
			UIConstData.FocusIsUsing = false;
			
			marketView.txt_market_search.removeEventListener(KeyboardEvent.KEY_UP,onKeyUp);
			
			marketView.mc_market_useJewelry.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.mc_market_needPrompt.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btnFront.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btnNext.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btnDeposit.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			marketView.btn_market_search.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function onKeyUp(event:KeyboardEvent):void
		{
			if(13 == event.keyCode)
			{
				searchByName();
			}
		}
		
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnFront":
					frontPage();
					break;
				case "btnNext":
					backPage();
					break;
				case "btnDeposit":	//充值
					facade.sendNotification(TerraceController.NAME , "pay");
					break; 
				case "btn_market_search":
					searchByName();
					break;
				case "mc_market_useJewelry":
					this.isUseJewelry = !this.isUseJewelry;
					break;
				case "mc_market_needPrompt":
					this.isNeedPrompt = !this.isNeedPrompt;
					break;
			}
		}
		
		
		private function searchByName():void
		{
			searchText = marketView.txt_market_search.text;
			if(searchText && searchText.length > 0)
			{
				MarketConstData.searchGoods = allMarketGoodsObj.filter(filterFunction);
				MarketConstData.searchGoods = MarketConstData.searchGoods.sortOn("type");
				/** 去除相同的物品 */
				var arr:Array = new Array();
				var i:int;
				if(MarketConstData.searchGoods.length > 0)
				{
					arr.push(MarketConstData.searchGoods[0]);
				}
				for(i = 1 ; i < MarketConstData.searchGoods.length; i++)
				{
					if(MarketConstData.searchGoods[i].type != MarketConstData.searchGoods[i-1].type)
					{
						arr.push(MarketConstData.searchGoods[i]);
					}
				}
				MarketConstData.searchGoods = arr;
				/***/
				if(MarketConstData.searchGoods.length > 0)
				{
					showView(8);
					marketView.mcPage_8.visible = true;
					marketView.txtPage_8.visible = true;
				}
				else
				{
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketMediator_searchByName" ]/**"不存在类似物品"*/, color:0xffff00});        
				}
			}
		}
		
		private function filterFunction(element:*, index:int, arr:Array):Boolean
		{
			var str:String = element["Name"];
			if(str && str.indexOf(searchText) != -1)
			{
				return true;
			}
			return  false;
		}
		
		
		private function cancelFun():void
		{
		}
		
		/** 前一页 */
		private function frontPage():void
		{
			if(pageCount > 0 && curPage > 1) {
				curPage--;
				uiManager.clearData();
				gridManager.removeAllItem();
				showCurData();
				marketView.txtPageInfo.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + curPage+"/"+pageCount + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第" "页"
			}
		}
		/** 后一页 */
		private function backPage():void
		{
			if(pageCount > 0 && curPage < pageCount) {
				curPage++;
				uiManager.clearData();
				gridManager.removeAllItem();
				showCurData();
				marketView.txtPageInfo.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + curPage+"/"+pageCount + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第" "页"
			}
		}
		
		/** 外部接口，购买商品 */
		private function buyItem(good:Object):void
		{
			var type:uint	 = good.type;		//商品typeId
			var count:uint 	 = good.count;		//购买数量
			var payType:uint = good.payType;	//支付方式
			if(type == 0) return;
			if(count == 0) count = 1;
//			var typeStr:String = String(type);
//			if(typeStr.indexOf("62") == 0) {	//任务物品
//				var taskUsed:uint = 0;
//				for(var n:int = 0; n < BagData.AllItems[2].length; n++) {
//					if(BagData.AllItems[0][n] != undefined) taskUsed++;
//				}
//				if(count > BagData.BagNum[2] - taskUsed) {
//					sendNotification(HintEvents.RECEIVEINFO, {info:"任务物品背包容量不足", color:0xffff00});
//					return;
//				}
//			} else if(typeStr.indexOf("40") == 0 || typeStr.indexOf("63") == 0 || typeStr.indexOf("61") == 0) {//材料物品
//				var datumUsed:uint = 0;
//				for(var m:int = 0; m < BagData.AllItems[1].length; m++) {
//					if(BagData.AllItems[0][m] != undefined) datumUsed++;
//				}
//				if(count > BagData.BagNum[1] - datumUsed) {
//					sendNotification(HintEvents.RECEIVEINFO, {info:"材料物品背包容量不足", color:0xffff00});
//					return;
//				}
//			} else {							//普通物品
//				var commonUsed:uint = 0;
//				for(var l:int = 0; l < BagData.AllItems[0].length; l++) {
//					if(BagData.AllItems[0][l] != undefined) commonUsed++;
//				}
//				if(count > BagData.BagNum[0] - commonUsed) {
//					sendNotification(HintEvents.RECEIVEINFO, {info:"普通物品背包容量不足", color:0xffff00});
//					return;
//				}
//			}
			MarketNetAction.buyItem(MarketAction.MARKET_BUY, type, count, payType);		//发送购买命令
			buyCDTimerOutSide.reset();
			buyCDTimerOutSide.start();
		}
		
		/** 确定购买商品 */
		private function sureToBuy():void
		{
			if(MarketConstData.shopCarData && MarketConstData.shopCarData.length > 0) {
				var ybSelf:Number = GameCommonData.Player.Role.UnBindRMB;	//自己的元宝
				var zbSelf:Number = GameCommonData.Player.Role.BindRMB;		//自己的珠宝
				var dqSelf:Number = GameCommonData.Player.Role.GiveAway;	//自己的点券
				var costArr:Array = MarketUtils.getCostMoney();
				if(ybSelf < costArr[0]) {
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_1" ], color:0xffff00});         //元宝不足
					return;
				}
				if(zbSelf < costArr[1]) {
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_2" ], color:0xffff00});         //珠宝不足
					return;
				}
				if(dqSelf < costArr[2]) {
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_sur_3" ], color:0xffff00});         //点券不足
					return;
				}
				//判断背包格子数  (购物车中商品先归类，普通、材料、任务、按类别判断背包相应格子是否足够)
//				var commonNeed:uint = 0;
//				var datumNeed:uint  = 0;
//				var taskNeed:uint	= 0;
//				for(var j:int = 0; j < MarketConstData.shopCarData.length; j++) {
//					var good:Object = MarketConstData.shopCarData[j];
//					if(String(good.type).indexOf("62") == 0) {	//任务物品
//						taskNeed++;
//					} else if(String(good.type).indexOf("40") == 0 || String(good.type).indexOf("63") == 0 || String(good.type).indexOf("61") == 0) {//材料物品
//						datumNeed++;
//					} else {	//普通物品
//						commonNeed++;
//					}
//				}
//				if(commonNeed > 0) {
//					var commonUsed:uint = 0;
//					for(var l:int = 0; l < BagData.AllItems[0].length; l++) {
//						if(BagData.AllItems[0][l] != undefined) commonUsed++;
//					}
//					if(commonNeed > BagData.BagNum[0] - commonUsed) {
//						sendNotification(HintEvents.RECEIVEINFO, {info:"普通物品背包容量不足", color:0xffff00});
//						return;
//					}
//				}
//				if(datumNeed > 0) {
//					var datumUsed:uint = 0;
//					for(var m:int = 0; m < BagData.AllItems[1].length; m++) {
//						if(BagData.AllItems[0][m] != undefined) datumUsed++;
//					}
//					if(datumNeed > BagData.BagNum[1] - datumUsed) {
//						sendNotification(HintEvents.RECEIVEINFO, {info:"材料物品背包容量不足", color:0xffff00});
//						return;
//					}
//				}
//				if(taskNeed > 0) {
//					var taskUsed:uint = 0;
//					for(var n:int = 0; n < BagData.AllItems[2].length; n++) {
//						if(BagData.AllItems[0][n] != undefined) taskUsed++;
//					}
//					if(taskNeed > BagData.BagNum[2] - taskUsed) {
//						sendNotification(HintEvents.RECEIVEINFO, {info:"任务物品背包容量不足", color:0xffff00});
//						return;
//					}
//				}
				//
				MarketNetAction.opMarket(null);		//发送购买命令
				
//				panelCloseHandler(null);//关闭商城

//--------------------------------------------------不关闭商城，只清空购物车和数据，更新显示钱
				MarketConstData.shopCarData = [];
				MarketConstData.comBoxList = [];
//				uiManager.clearShopCar();
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_BUY_MARKET_NOTICE_NEWER_HELP);
//-----------------------------------------------------------
			} else {
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mak_med_mar_btn_2" ], color:0xffff00});             //购物车中尚无商品
			}
		}
		
		public function set isUseJewelry(isUseJewelry:Boolean):void
		{
			if(isUseJewelry)
			{
				(marketView.mc_market_useJewelry as MovieClip).gotoAndStop(2);
			}
			else
			{
				(marketView.mc_market_useJewelry as MovieClip).gotoAndStop(1);
			}
		}
		
		public function get isUseJewelry():Boolean
		{
			return 2 == (marketView.mc_market_useJewelry as MovieClip).currentFrame;
		}
		
		public function set isNeedPrompt(isNeedPrompt:Boolean):void
		{
			if(isNeedPrompt)
			{
				(marketView.mc_market_needPrompt as MovieClip).gotoAndStop(2);
			}
			else
			{
				(marketView.mc_market_needPrompt as MovieClip).gotoAndStop(1);
			}
		}
		
		public function get isNeedPrompt():Boolean
		{
			return 2 == (marketView.mc_market_needPrompt as MovieClip).currentFrame;
		}
		
		/** 显示珠宝 元宝 */
		private function showMoney1():void
		{
			marketView.txtMarketIntro1_1.visible = true;
			marketView.mcHave_1.visible = true;
			marketView.txtMarketIntro1_0.visible = true;
			marketView.mcHave_0.visible = true;
			marketView.txtMarketIntro1_2.visible = false;
			marketView.mcHave_2.visible = false;
		}
		
		/** 显示点券 */
		private function showMoney2():void
		{
			marketView.txtMarketIntro1_1.visible = false;
			marketView.mcHave_1.visible = false;
			marketView.txtMarketIntro1_0.visible = false;
			marketView.mcHave_0.visible = false;
			marketView.txtMarketIntro1_2.visible = true;
			marketView.mcHave_2.visible = true;
		}
		
		/** 加载商城美女 */
		private function loadBelle():void
		{
			var sexUrl:String = GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/marketBelle.swf";
			this.sexLoader = new ImageItem( sexUrl,BulkLoader.TYPE_IMAGE,"" );
			sexLoader.addEventListener( Event.COMPLETE,sexLoadCom );
			sexLoader.load();
		}
		
		//加载完成
		private function sexLoadCom( evt:Event ):void
		{
			sexLoader.removeEventListener( Event.COMPLETE,sexLoadCom );
			
			var sexBmp:Bitmap = new Bitmap();
			var bgClass:Class;
			bgClass = sexLoader.GetDefinitionByName( "MarketBelle" );
			
			if ( bgClass )
			{
				sexBmp.bitmapData = new bgClass(192,365) as BitmapData;
			}
			sexBmp.x = -298;
			sexBmp.y = -105;
			panelBase.addChildAt( sexBmp,0 );
			
			MarketConstData.isLoadBelle = true;
		}
	}
}
