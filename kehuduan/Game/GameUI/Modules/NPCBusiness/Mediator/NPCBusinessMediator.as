package GameUI.Modules.NPCBusiness.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessConstData;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessEvent;
	import GameUI.Modules.NPCBusiness.Proxy.NPCBusinessGridManager;
	import GameUI.Modules.NPCBusiness.Proxy.NPCBusinessNetAction;
	import GameUI.Modules.NPCBusiness.UI.NPCBusinessUIManager;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NPCBusinessMediator extends Mediator
	{
		public static const NAME:String = "NPCBusinessMediator";
		private static const START_POS:Point = new Point();
		private var panelBase:PanelBase = null;
		private var gridSprite:MovieClip = null;
		private var dataProxy:DataProxy = null;
		private var shopUIManager:NPCBusinessUIManager = null;
		private var shopGridManager:NPCBusinessGridManager = null;
		private var yellowFilter:GlowFilter = null;
		private var redFilter:GlowFilter = null;
		
		private var npcId:int = 0;
		private var shopType:int = 0;
		private var shopName:String = "";
		
		private var pageCount:int = 0;
		private var curPage:int = 0;
		
		private static const STR_ARR:String = "\\aa"; 
		
		public function NPCBusinessMediator()
		{
			super(NAME);
		}
		
		private function get shopView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOW_NPC_BUSINESS_SHOP_VIEW,				//打开跑商商店
				EventList.CLOSE_NPC_BUSINESS_SHOP_VIEW,				//关闭跑商商店
				EventList.CLOSE_NPC_ALL_PANEL,						//关闭由NPC打开的面板
				NPCBusinessEvent.BAG_TO_NPCBUSINESS,				//物品从背包过来
				NPCBusinessEvent.UPDATE_NPCBUSINESS_SALE_MONEY,		//更新出售钱数
				NPCBusinessEvent.UPDATE_MONEY_LAST_NPCBUSINESS		//更新银子余额显示
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOW_NPC_BUSINESS_SHOP_VIEW:												//打开商店
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!judge()) {
						gcAll();
						return;
					}
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.NPCBUSINESSVIEW});
					yellowFilter = UIUtils.getGlowFilter(0xffff00, 1, 2, 2, 4);
					redFilter = UIUtils.getGlowFilter(0xff0000, 1, 2, 2, 4);
					npcId = notification.getBody().npcId;
					shopName = notification.getBody().shopName;
					shopType = notification.getBody().shopType;
					initView();
					updateLastMoney();
					initData();
					addLis();
					dataProxy.NPCBusinessIsOpen = true;
					if(!dataProxy.BagIsOpen) {
						sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
					}
					break;
				case EventList.CLOSE_NPC_BUSINESS_SHOP_VIEW:											//关闭商店
					gcAll();
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:														//关闭由NPC打开的面板
					if(dataProxy && dataProxy.NPCBusinessIsOpen) gcAll();
					break;
				case NPCBusinessEvent.BAG_TO_NPCBUSINESS:
					var goodToSale:Object = notification.getBody();
//					var typeMul:uint = goodToSale.type / 1000;
//					if(typeMul != 626) {		//跑商物品
					if(!NPCBusinessConstData.goodSalePriceDic[goodToSale.type]) {	//不在出售价格列表中
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_1" ], color:0xffff00});              //"此物品无法在该商店出售"
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);
					} else if(NPCBusinessConstData.goodSaleList.length >= 8) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_2" ], color:0xffff00});       //"一次最多出售8件物品"
						sendNotification(EventList.BAGITEMUNLOCK, goodToSale.id);
					} else {
						NPCBusinessConstData.goodSaleList.push(goodToSale);
						updateSaleData();
					}
					break;
				case NPCBusinessEvent.UPDATE_NPCBUSINESS_SALE_MONEY:									//更新出售总钱数
					updateSaleMoney();
					break;
				case NPCBusinessEvent.UPDATE_MONEY_LAST_NPCBUSINESS:									//更新银子余额
					updateLastMoney();
					break;
			}
		}
		
		private function initView():void
		{
			panelBase = new PanelBase(shopView, shopView.width+8, shopView.height+12);
			panelBase.name = "NPCBusiness";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt(shopName);
			gridSprite = new MovieClip();
			shopView.addChild(gridSprite);
			gridSprite.x = 22;
			gridSprite.y = 328;
			
			shopUIManager = new NPCBusinessUIManager(shopView); 
			
			initGrid();
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
		}
		
		/** 初始化格子 */
		private function initGrid():void
		{
			for(var i:int = 0; i < 8; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%8);
				gridUnit.y = (gridUnit.height) * int(i/8);
				gridUnit.name = "NPCBusinessSale_" + i.toString();
				gridSprite.addChild(gridUnit);	//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = gridSprite;								//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				NPCBusinessConstData.GridUnitList.push(gridUint);
			}
			shopView.addChild(gridSprite);
			shopGridManager = new NPCBusinessGridManager(NPCBusinessConstData.GridUnitList, gridSprite);
			facade.registerProxy(shopGridManager);
		}
		
		/** 初始化数据 */
		private function initData():void
		{
			pageCount = Math.ceil( (NPCBusinessConstData.goodList.length / 10) );	//向上取整: 2.3 = 3
			if(pageCount > 0) {
				clearData();
				curPage++;
				updateData();
				shopView.txtPageInfo.text = curPage+"/"+pageCount;
			} else {
				clearData();
				shopView.txtPageInfo.text = "1/1";
			}
		}
		
		private function gcAll():void {
			removLis();
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) 
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			for(var i:int = 0; i < NPCBusinessConstData.goodSaleList.length; i++) {
				sendNotification(EventList.BAGITEMUNLOCK, NPCBusinessConstData.goodSaleList[i].id);
			}
			NPCBusinessConstData.SelectedItem = null;
			if(shopGridManager) shopGridManager.removeAllItem();
			viewComponent = null;
			yellowFilter = null;
			redFilter = null;
			panelBase = null;
			gridSprite = null;
			pageCount = 0;
			curPage = 0;
			npcId = 0;
			shopName = "";
			shopType = 0;
			NPCBusinessConstData.goodList = [];
			NPCBusinessConstData.goodSalePriceDic = new Dictionary();
			NPCBusinessConstData.goodSaleList = [];
			NPCBusinessConstData.GridUnitList = [];
			NPCBusinessConstData.selectedIndex - 1;
			NPCBusinessConstData.TmpIndex = 0;
			shopUIManager = null;
			shopGridManager = null;
			dataProxy.NPCBusinessIsOpen = false;
			dataProxy = null;
			facade.removeMediator(NPCBusinessMediator.NAME);
		}
		
		/** 点击按钮 上页、下页、修理、修理全部、购买、出售 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnFrontPage":												//上页
					frontPage();
					break;
				case "btnBackPage":													//下页
					backPage();
					break;
				case "btnBuy":														//购买
					buyGood();
					break;
				case "btnSale":														//出售
					saleGood();
					break;
			}
		}
		
		/** 前一页 */
		private function frontPage():void
		{
			if(pageCount > 0 && curPage > 1) {
				removeAllFrames();
				curPage--;
				clearData();
				updateData();
				shopView.txtPageInfo.text = curPage+"/"+pageCount;
			}
		}
		/** 后一页 */
		private function backPage():void
		{
			if(pageCount > 0 && curPage < pageCount) {
				removeAllFrames();
				curPage++;
				clearData();
				updateData();
				shopView.txtPageInfo.text = curPage+"/"+pageCount;
			}
		}
		
		/** 购买物品 */
		private function buyGood():void
		{
			if(NPCBusinessConstData.selectedIndex > -1) {
				var count:int = int(shopView.txtInputCount.text);
				if(count <= 0){
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_3" ] , color:0xffff00});              //"请输入购买数量"
				} else {
					var index:int = (curPage-1) * 10 + NPCBusinessConstData.selectedIndex;
					var good:Object = NPCBusinessConstData.goodList[index];
					var price:Number = good.price;
					var money:Number = price * count;
					if(judgeMoney(money)) {
						NPCBusinessConstData.goodList[index].amount -= count;		//客户端自行减除购买的数量
						if(count > NPCBusinessConstData.goodList[index].amount) {	//判断新数量
							shopView.txtInputCount.text = NPCBusinessConstData.goodList[index].amount.toString();
							money = price * NPCBusinessConstData.goodList[index].amount;
							var moneyStr:String = (money == 0) ? "" : money + STR_ARR;
							shopView.mcMoneyBuy.txtMoney.text = moneyStr;
							ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
						}
						//发送购买命令
						NPCBusinessNetAction.buyNPCItem(good.type, npcId, count, price);
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_4" ], color:0xffff00});         //"未接跑商任务或银票余额不足"
					}
				}
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_17" ], color:0xffff00});           //"请先选择物品"
			}
		}
		
		/** 出售物品 */
		private function saleGood():void
		{
			if(NPCBusinessConstData.goodSaleList.length <= 0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_5" ], color:0xffff00});                 //"当前没有要出售物品"
			} else {
				//发送出售物品命令  要出售的物品ID
				for(var i:int = 0; i < NPCBusinessConstData.goodSaleList.length; i++) {
					var price:Number =  NPCBusinessConstData.goodSalePriceDic[NPCBusinessConstData.goodSaleList[i].type];
					NPCBusinessNetAction.saleGood(NPCBusinessConstData.goodSaleList[i].id, npcId, price);
				}
				for(var j:int = 0; j < NPCBusinessConstData.goodSaleList.length; j++) {
					//解锁
					sendNotification(EventList.BAGITEMUNLOCK, NPCBusinessConstData.goodSaleList[j].id);
				}
				shopGridManager.removeAllItem();
				NPCBusinessConstData.goodSaleList = [];
				shopView.mcMoneySale.txtMoney.text = "";
				ShowMoney.ShowIcon(shopView.mcMoneySale, shopView.mcMoneySale.txtMoney, true);
			}
		}
		
		/** 更新出售栏物品数据 */
		private function updateSaleData():void
		{
			var index:int = NPCBusinessConstData.goodSaleList.length - 1;
			shopGridManager.addItem(index);
			//更新总售价
			updateSaleMoney();
		}
		
		private function panelCloseHandler(e:Event):void {
			gcAll();	
		}
		
		private function addLis():void {
			for(var i:int = 0; i < 10; i++) {
				shopView["mcNPCGood_"+i].addEventListener(MouseEvent.ROLL_OVER, mouseOverHandler);
				shopView["mcNPCGood_"+i].addEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
			}
			shopView.txtInputCount.addEventListener(Event.CHANGE, inputHandler);
			shopView.btnFrontPage.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnBackPage.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnBuy.addEventListener(MouseEvent.CLICK, btnClickHandler);
			shopView.btnSale.addEventListener(MouseEvent.CLICK, btnClickHandler);
			UIUtils.addFocusLis(shopView.txtInputCount);
		}
		
		private function removLis():void {
			if(shopView) {
				for(var i:int = 0; i < 10; i++) {
					shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.ROLL_OVER, mouseOverHandler);
					shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.ROLL_OUT, mouseOutHandler);
				}
				shopView.txtInputCount.removeEventListener(Event.CHANGE, inputHandler);
				shopView.btnFrontPage.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnBackPage.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnBuy.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				shopView.btnSale.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				UIUtils.removeFocusLis(shopView.txtInputCount);
			}
		}
		
		/** 鼠标移入 */
		private function mouseOverHandler(e:MouseEvent):void
		{
			e.currentTarget.mcRed.visible = true;
		}
		/** 鼠标移出 */
		private function mouseOutHandler(e:MouseEvent):void
		{
			e.currentTarget.mcRed.visible = false;
		}
		
		/** 点击选择商品 */
		private function goodSelectHandler(e:MouseEvent):void
		{
			removeAllFrames();
			e.currentTarget.mcYellow.visible = true;
			var index:int = e.currentTarget.name.split("_")[1];
			if(index != NPCBusinessConstData.selectedIndex) {
				var good:Object = NPCBusinessConstData.goodList[(curPage-1) * 10 + index];
				if(good) {
					NPCBusinessConstData.selectedIndex = index;
					if(good.amount > 0) {
						shopView.txtInputCount.text = "1";
	//					var money:String = UIUtils.getMoneyStandInfo(good.price, STR_ARR);
						shopView.mcMoneyBuy.txtMoney.text = good.price + STR_ARR;
						ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
					} else {
						shopView.txtInputCount.text = "0";
						shopView.mcMoneyBuy.txtMoney.text = "";
						ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_6" ], color:0xffff00});           //"该商品库存不足"
					}
				}
			}
		}
		
		/** 输入检测 购买数量 */
		private function inputHandler(e:Event):void
		{
			switch(e.target.name) 
			{
				case "txtInputCount":							//购买数量
					var num:int = int(shopView.txtInputCount.text);
					if(NPCBusinessConstData.selectedIndex > -1) {
						var good:Object = NPCBusinessConstData.goodList[ (curPage-1)*10 + NPCBusinessConstData.selectedIndex ];
						if(good.amount == 0) {
							shopView.txtInputCount.text = "0";
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_6" ], color:0xffff00});         //"该商品库存不足"
							return;
						}
						if(num <= 0) shopView.txtInputCount.text = "1";
						if(num > good.amount) {
							shopView.txtInputCount.text = good.amount.toString();
							num = good.amount;
						}
						var price:Number = good.price;
						var money:Number = price * num;
//						var moneyStr:String = UIUtils.getMoneyStandInfo(money, STR_ARR);
						shopView.mcMoneyBuy.txtMoney.text = money + STR_ARR;
						ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
					}
					break;
			}
		}
		
		/** 移除所有红黄边框 */
		private function removeAllFrames():void
		{
			for(var i:int = 0; i < 10; i++) {
				shopView["mcNPCGood_"+i].mcRed.visible = false;
				shopView["mcNPCGood_"+i].mcYellow.visible = false;
			}
		}
		
		/** 清除画面数据 */
		private function clearData():void
		{
			for(var i:int = 0; i < 10; i++) {
				var count:int = 0;
				while(count < (shopView["mcNPCGood_"+i] as MovieClip).numChildren) {
					if((shopView["mcNPCGood_"+i] as MovieClip).getChildAt(count) is ItemBase) {
						(shopView["mcNPCGood_"+i] as MovieClip).removeChild((shopView["mcNPCGood_"+i] as MovieClip).getChildAt(count));
						break;
					}
					count++;
				}
				shopView["mcNPCGood_"+i].txtGoodName.text = "";
				shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.htmlText = "";
				ShowMoney.ShowIcon(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney, true);
				shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.CLICK, goodSelectHandler);
			}
			NPCBusinessConstData.selectedIndex = -1;
			shopView.txtInputCount.text = "1";
			shopView.mcMoneyBuy.txtMoney.text = "";
			ShowMoney.ShowIcon(shopView.mcMoneyBuy, shopView.mcMoneyBuy.txtMoney, true);
		}
		
		/** 更新数据 */
		private function updateData():void
		{
			var fromIndex:int = (curPage-1) * 10;
			var goodList:Array = NPCBusinessConstData.goodList;
			for(var i:int = 0; i < 10; i++) {
				if(goodList[fromIndex + i]) {
					var faceItem:FaceItem = new FaceItem(goodList[fromIndex + i].type, shopView["mcNPCGood_"+i]);
					faceItem.x = 2;
					faceItem.y = 2;
					faceItem.setEnable(true);
					faceItem.name = "mcNPCGoodPhoto_"+goodList[fromIndex + i].type;
					shopView["mcNPCGood_"+i].addChild(faceItem);
					var color:uint = UIConstData.getItem(goodList[fromIndex + i].type).Color; 
					shopView["mcNPCGood_"+i].txtGoodName.htmlText =  '<font color="' + IntroConst.itemColors[color] + '">' + UIConstData.getItem(goodList[fromIndex + i].type).Name + '</font>'; 
					//////
//					shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.text = goodList[fromIndex + i].price + STR_ARR;
					shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney.htmlText = goodList[fromIndex + i].price + judgePriceLev(goodList[fromIndex + i]) + STR_ARR;
					ShowMoney.ShowIcon(shopView["mcNPCGood_"+i].mcMoneyPrice, shopView["mcNPCGood_"+i].mcMoneyPrice.txtMoney, true);
					shopView["mcNPCGood_"+i].addEventListener(MouseEvent.CLICK, goodSelectHandler);
				} else {
					shopView["mcNPCGood_"+i].removeEventListener(MouseEvent.CLICK, goodSelectHandler);
				}
			}
		}
		
		/** 更新出售栏总钱数 */
		private function updateSaleMoney():void
		{
			var money:Number = 0;
			var moneyStr:String = "";
//			var a:Array = NPCBusinessConstData.goodSaleList;
//			var aa:Dictionary = NPCBusinessConstData.goodSalePriceDic;
			for(var i:int = 0; i < NPCBusinessConstData.goodSaleList.length; i++) {
				var price:Number =  NPCBusinessConstData.goodSalePriceDic[NPCBusinessConstData.goodSaleList[i].type];
				price = price * NPCBusinessConstData.goodSaleList[i].amount;
				money += price;
			}
//			money == 0 ? moneyStr = "" : moneyStr = UIUtils.getMoneyStandInfo(money, STR_ARR);
			money == 0 ? moneyStr = "" : moneyStr = money + STR_ARR;
			shopView.mcMoneySale.txtMoney.text = moneyStr;
			ShowMoney.ShowIcon(shopView.mcMoneySale, shopView.mcMoneySale.txtMoney, true);
		}
		
		/** 更新银子余额 */
		private function updateLastMoney():void
		{
			var money:int = 0;
			var obj:Object = BagData.getItemByType(626100);
			if(obj != null) {
				money = obj.amountMoney;
			}
			shopView.mcMoneyLast.txtMoney.text = money + STR_ARR;
			ShowMoney.ShowIcon(shopView.mcMoneyLast, shopView.mcMoneyLast.txtMoney, true);
		}
		
		/** 判断当前打开商店条件是否合法 */
		private function judge():Boolean
		{
			var result:Boolean = true;
			if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_DEAD) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_7" ], color:0xffff00});   //"死亡状态不能打开跑商商店"
				result = false;
			} else if(dataProxy.TradeIsOpen) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_8" ], color:0xffff00});              //"交易中不能打开跑商商店"
				result = false;
			} else if(StallConstData.stallSelfId != 0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_npcb_med_npcbm_9" ], color:0xffff00});        //"摆摊中不能打开跑商商店"
				result = false;
			} else if(dataProxy.DepotIsOpen) {
				sendNotification(EventList.CLOSEDEPOTVIEW);
			}
			return result;
		}
		
		/** 判断身上银票（身上无银票或钱不足） */
		private function judgeMoney(money:Number):Boolean
		{
			var ret:Boolean = true;
			var obj:Object = BagData.getItemByType(626100);
			if(!obj) {
				ret = false;
			} else if(money > obj.amountMoney){
				ret = false;
			}
			return ret;
		}
		
		/** 判断价格范围 */
		private function judgePriceLev(obj:Object):String 
		{
			var ret:String = "";
			if(NPCBusinessConstData.GOOD_DIC[obj.type]) {
				var goodData:Object = NPCBusinessConstData.GOOD_DIC[obj.type];
				var priceNow:int = obj.price;
				var price:int = goodData.base;
				if( priceNow >= (price * goodData.middle + price) ) {
					ret = NPCBusinessConstData.PRICE_TYPE_STR[2];
				} else if( priceNow >= (price * goodData.low + price) ) {
					ret = NPCBusinessConstData.PRICE_TYPE_STR[1];
				} else {
					ret = NPCBusinessConstData.PRICE_TYPE_STR[0];
				}
			}
			return ret;
		}
		
	}
}
