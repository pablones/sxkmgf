package GameUI.Modules.Trade.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Trade.Data.TradeConstData;
	import GameUI.Modules.Trade.Data.TradeDataProxy;
	import GameUI.Modules.Trade.Data.TradeEvent;
	import GameUI.Modules.Trade.Proxy.TradeNetAction;
	import GameUI.Modules.Trade.UI.TradeUIManager;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.TradeAction;
	import Net.ActionSend.TradeSend;
	import Net.Protocol;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TradeMediator extends Mediator
	{
		public static const NAME:String = "TradeMediator";
		public static const TRADE_PANEL_POS:Point = new Point(80, 58);	//交易面板位置
		public static const MONEY_PANEL_POS:Point = new Point(435, 94); //金钱输入面板位置
		public static const PET_PANEL_POS:Point   = new Point(440, 278);//宠物列表面板位置
		public static const GRID_POS:Point        = new Point(12, 55);	//格子位置
	
		private var dataProxy:DataProxy           = null;
		private var tradeDataProxy:TradeDataProxy = null;
		private var tradeUIManager:TradeUIManager = null;
		
		private var tradePanel:PanelBase		  = null;				//交易面板
		private var moneyPanel:PanelBase 		  = null;				//金钱输入面板
		
		private var notiData:Object				  = new Object();		//消息数据
		private var applyPersonIds:Array 		  = new Array();		//请求与我交易人的ID数组
		
		private var petListPanel:PanelBase		  = null;				//宠物列表面板
		private var listView:ListComponent		  = null;				//宠物列表
		private var iScrollPane:UIScrollPane	  = null;				//Scroll Bar
		
		private var listViewPetSf:ListComponent	  = null;				//自己交易栏中的宠物						
		
		private var listViewPetOp:ListComponent	  = null;				//对方交易栏中的宠物
		
		
		public function TradeMediator()
		{
			super(NAME);
		}
		
		private function get trade():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,					//初始化
				EventList.SHOWTRADE,				//显示交易窗
				EventList.UPDATETRADE,				//更新交易信息
				EventList.REMOVETRADE,				//关闭交易，主动发取消交易
				EventList.APPLYTRADE,				//申请交易，对外接口
				EventList.GOTRADEVIEW,				//添加交易物品
				EventList.GOBAGVIEW,				//减少物品
				EventList.TRADEUNLOCK,				//自己交易解锁
				EventList.TRADEUNLOCK_OP,			//对方交易解锁
				TradeEvent.SOMEONETRADEME,			//某人要与我交易
				TradeEvent.SOMEONEREFUSEME,			//某人拒绝了我的交易请求
				TradeEvent.SHOWTRADEINFORMATION, 	//显示交易相关的提示信息
				TradeEvent.PET_ADD_SELF_TRADE,		//自己添加宠物
				TradeEvent.PET_DEL_SELF_TRADE,		//自己删除宠物
				TradeEvent.PET_DEL_OP_TRADE,		//对方删除宠物
				TradeEvent.PET_ADD_OP_TRADE			//对方加了宠物
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.TRADE});
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					initView();
					break;
				case EventList.SHOWTRADE:																			//显示交易窗
					if(StallConstData.stallSelfId > 0) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_han_1" ], color:0xffff00});        //摆摊中不能交易
						//发送取消交易消息
						sendData(TradeAction.QUIT);
						gcAll();
						return;
					}
					if(dataProxy.DepotIsOpen) {			//先关掉仓库
						sendNotification(EventList.CLOSEDEPOTVIEW);
					} 
					if(dataProxy.NPCShopIsOpen) {		//关掉NPC商店
						sendNotification(EventList.CLOSENPCSHOPVIEW);
					}
					if(dataProxy.NPCBusinessIsOpen) {	//关掉跑商商店
						sendNotification(EventList.CLOSE_NPC_BUSINESS_SHOP_VIEW);
					}
					if(dataProxy.PetIsOpen) {			//关掉宠物面板
						facade.sendNotification(EventList.CLOSEPETVIEW);
					}
					if(dataProxy.equipPanelIsOpen) {
						sendNotification(EquipCommandList.CLOSE_EQUIP_PANEL);
					}
					sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);		//关闭宠物操作系列面板
					GameCommonData.Player.Role.State = GameRole.STATE_TRADE;
					var traderId:int = notification.getBody().id;
					var trader:GameElementAnimal = GameCommonData.SameSecnePlayerList[traderId];
					var traderName:String = trader.Role.Name;
					TradeConstData.opName = traderName;
					trade.txtRoleName_op.text = traderName.toString();
					//宠物列表副本
					for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
						TradeConstData.petListSelfDic[key] = UIUtils.DeeplyCopy(GameCommonData.Player.Role.PetSnapList[key]);
					}
					showView();
					dataProxy.PetCanOperate = false;	//禁止操作宠物
					dataProxy.TradeIsOpen = true;
					UIConstData.IsTrading = true;
					UIConstData.KeyBoardCanUse = false;		//禁用快捷键
					if(!dataProxy.BagIsOpen){
						facade.sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
					}
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_han_2" ]+"<font color='#ff0000'>["+traderName+"]</font>"+GameCommonData.wordDic[ "often_used_trade" ], color:0xffff00});    //你正在与      交易   
					break;
				case EventList.UPDATETRADE:																			//更新交易信息
					if(dataProxy.TradeIsOpen) {
						notiData = new Object();
						notiData = notification.getBody();
						updateView();
					}
					break;
				case EventList.REMOVETRADE:																			//关闭交易，主动发取消交易
					if(dataProxy.TradeIsOpen) {
						tradePanelCloseHandler(null);
					}
					break;
				case EventList.APPLYTRADE:																			//申请别人交易，对外接口
					if(StallConstData.stallSelfId > 0) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_han_1" ], color:0xffff00});     //摆摊中不能交易
						return;
					}
//					else if(dataProxy.DepotIsOpen) {
//						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请关闭仓库再交易", color:0xffff00});
//						sendNotification(EventList.CLOSEDEPOTVIEW);
//						return;
//					}
					if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_DEAD) {
						sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:15});
						return;
					}
					if(dataProxy.TradeIsOpen) {
						sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:2});
						return;
					}
					notiData = notification.getBody();
					
					if(!GameCommonData.SameSecnePlayerList[notiData.id]) {
						sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:1});
						return;
					}
					
					var toPerson:GameElementAnimal = GameCommonData.SameSecnePlayerList[notiData.id];
					var pName:String = toPerson.Role.Name;
					sendData(TradeAction.APPLY);
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_han_3" ]+"<font color='#ff0000'>["+pName+"]</font>"+GameCommonData.wordDic[ "mod_tra_med_tra_han_4" ], color:0xffff00});   //交易请求已发送，请耐心等待        回复
					break;
				case EventList.GOTRADEVIEW:																			//增加物品
					if(!tradeDataProxy.sfLocked) {
						var itemId:int = notification.getBody() as int;
						notiData.id = notification.getBody();
						sendData(TradeAction.ADDITEM);
						TradeConstData.idItemSelfArr.push(itemId);
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_dat_tra_dra_1" ], color:0xffff00});         //交易已锁定，无法操作
						sendNotification(EventList.BAGITEMUNLOCK, notification.getBody());
					}
					break;
				case EventList.GOBAGVIEW:																			//减少物品
					tradeDataProxy.goodPetOperating.id = notification.getBody();
					sendData(TradeAction.BACK_WU);
					break;
				case EventList.TRADEUNLOCK:																			//自己解锁
					trade.btnLock.visible 		= true;
					trade.btnInputMoney.visible = true;
					trade.btnRemovePet.visible  = true;
					trade.mcGreenRect.visible 	= false;
					trade.txtState_self.text = GameCommonData.wordDic[ "mod_tra_med_tra_han_5" ];         //尚未锁定
					petListPanel.content.btnPetChose.visible = true;
					tradeDataProxy.sfLocked = false;
					if(tradeDataProxy.opLocked && tradeDataProxy.sfLocked) trade.btnSure.visible = true;
					refreshPetBtn();
					sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:13});
					break;
				case EventList.TRADEUNLOCK_OP:																		//对方解锁
					tradeDataProxy.opLocked = false;
					trade.txtState_op.htmlText = GameCommonData.wordDic[ "mod_tra_med_tra_han_5" ];         //尚未锁定
					trade.mcGreenRectOp.visible = false;
					if(tradeDataProxy.opLocked && tradeDataProxy.sfLocked) trade.btnSure.visible = true;
					sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:14});
					break;
				case TradeEvent.SOMEONETRADEME:																		//某人想要与我交易
					var personId:int = int(notification.getBody());
					if(applyPersonIds.indexOf(personId) == -1) {	//同一个人请求过，则不予处理
						applyPersonIds.push(personId);				//请求交易是一个请求队列，可能多人同时请求一个人交易
						if(applyPersonIds.length == 1) showTradeAsk();
					} else {
						return;
					}
					break;
				case TradeEvent.SOMEONEREFUSEME:																	//某人拒绝了与我交易请求
					var refuserId:int = int(notification.getBody());
					showRefuseAlert(refuserId);
					break;
				case TradeEvent.SHOWTRADEINFORMATION:			//显示交易相关的提示信息
					var infoObj:Object = notification.getBody();
					showTradeInformation(infoObj);
					break;
				case TradeEvent.PET_ADD_SELF_TRADE:				//自己添加宠物
					if(dataProxy.TradeIsOpen) {
						var addResult:Object = notification.getBody();
						if(addResult.result == true) {	//添加成功
							var dic:Dictionary = GameCommonData.Player.Role.PetSnapList;
							var dic1:Dictionary = TradeConstData.petListSelfDic;
							delete TradeConstData.petListSelfDic[addResult.petId];
							TradeConstData.petSelfDic[addResult.petId] = GameCommonData.Player.Role.PetSnapList[addResult.petId];
						} else {						//添加失败
							
						}
						TradeConstData.petSelectOfList = null;
						TradeConstData.petSelectOfTrade = null;
						initPetChoiceView();
						initPetListTradeSelf();
						refreshPetBtn();
						lockPetOp(true);
					}
					break;
				case TradeEvent.PET_DEL_SELF_TRADE:				//自己删除宠物
					if(dataProxy.TradeIsOpen) {
						var delResult:Object = notification.getBody();
						if(delResult.result == true) {	//删除成功
							delete TradeConstData.petSelfDic[delResult.petId];
							TradeConstData.petListSelfDic[delResult.petId] = GameCommonData.Player.Role.PetSnapList[delResult.petId];
						} else {						//删除失败
							
						}
						TradeConstData.petSelectOfList = null;
						TradeConstData.petSelectOfTrade = null;
						initPetChoiceView();
						initPetListTradeSelf();
						refreshPetBtn();
						lockPetOp(true);
					}
					break;
				case TradeEvent.PET_DEL_OP_TRADE:				//对方删除宠物
					if(dataProxy.TradeIsOpen) {
						var petIdOp:uint = uint(notification.getBody().petId);
						if(petIdOp > 0) {
							delete TradeConstData.petOpDic[petIdOp];
							initPetListTradeOp();
						}
					}
					break;
				case TradeEvent.PET_ADD_OP_TRADE:				//对方加了宠物
					if(dataProxy.TradeIsOpen) {
						initPetListTradeOp();
					}
					break;
				default:
					
					break;
			}
		}
		
		private function initView():void
		{
			var moneyInput:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MoneyInput");
			(moneyInput.txtJin as TextField).restrict = "0-9";
			(moneyInput.txtYin as TextField).restrict = "0-9";
			(moneyInput.txtTong as TextField).restrict = "0-9";
			
			var petList:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetList");
			tradePanel 		= new PanelBase(trade, trade.width+6, trade.height+7);
			moneyPanel 		= new PanelBase(moneyInput, moneyInput.width+8, moneyInput.height+11);
			petListPanel    = new PanelBase(petList, petList.width-6, petList.height+11);
			
			tradePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_tra_med_tra_ini_1" ]);        //交 易
			moneyPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_tra_med_tra_ini_2" ]);        //金钱输入
			petListPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_tra_med_tra_ini_3" ]);       //宠物列表
			petListPanel.disableClose();
			
			tradePanel.addEventListener(Event.CLOSE, tradePanelCloseHandler);
			moneyPanel.addEventListener(Event.CLOSE, moneyPanelCloseHandler);
			
			tradePanel.x 	= TRADE_PANEL_POS.x;
			tradePanel.y 	= TRADE_PANEL_POS.y;
			moneyPanel.x 	= MONEY_PANEL_POS.x;
			moneyPanel.y 	= MONEY_PANEL_POS.y;
			petListPanel.x 	= PET_PANEL_POS.x;
			petListPanel.y 	= PET_PANEL_POS.y;
			
			UIUtils.addFocusLis(moneyInput.txtJin);
			UIUtils.addFocusLis(moneyInput.txtYin);
			UIUtils.addFocusLis(moneyInput.txtTong);
			
			initGrid();
			tradeDataProxy = new TradeDataProxy(TradeConstData.GridUnitList, TradeConstData.GridUnitListOp);
			facade.registerProxy(tradeDataProxy);
			
			tradeUIManager = new TradeUIManager(trade, petList, moneyInput, tradeDataProxy);
			
		}
		
		/** 初始化格子 */
		private function initGrid():void
		{
			for(var i:int = 0; i < 5; i++) {
				var grid:MovieClip = trade["mcPhoto_"+i];
				var gridUint:GridUnit = new GridUnit(grid, false);
				gridUint.parent = trade;									//设置父级
				gridUint.Index = i;											//格子的位置		
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				TradeConstData.GridUnitList.push(gridUint);
			}
			showItems(TradeConstData.goodSelfList);
			
			for(var j:int = 0; j < 5; j++) {
				var gridOp:MovieClip = trade["mcOpPhoto_"+j];
				var gridUintOp:GridUnit = new GridUnit(gridOp, false);
				gridUintOp.parent = trade;										//设置父级
				gridUintOp.Index = i;											//格子的位置		
				gridUintOp.HasBag = true;										//是否是可用的背包
				gridUintOp.IsUsed	= false;									//是否已经使用
				gridUintOp.Item	= null;											//格子的物品
				TradeConstData.GridUnitListOp.push(gridUintOp);
			}
			showItems(TradeConstData.goodOpList);
		}
		
		/**  初始化物品 */
		private function showItems(list:Array):void
		{
			removeAllItem();
			for(var i:int = 0; i<list.length; i++)
			{
				if(list[i] == undefined) 
				{
					continue;
				}; 
				var useItem:UseItem = new UseItem(list[i].index, list[i].type, trade);
				useItem.Num = list[i].amount;
				useItem.x = 2;
				useItem.y = 2;
				useItem.Id = list[i].id;
				useItem.IsBind = list[i].isBind;
				useItem.Type = list[i].type;
				TradeConstData.GridUnitList[list[i].index].Item = useItem;
				TradeConstData.GridUnitList[list[i].index].IsUsed = true;
				TradeConstData.GridUnitList[list[i].index].Grid.addChild(useItem);
			}
			
		}
		
		/**
		 * 做全刷新
		 * 移除所有的物品
		 * 将所有的格子都初始化
		 * */
		private function removeAllItem():void
		{
			var count:int = trade.numChildren - 1;
			while(count)
			{
				if(trade.getChildAt(count) is ItemBase)
				{
					var item:ItemBase = trade.getChildAt(count) as ItemBase;
					trade.removeChild(item);
					item = null;
				}
				count--;
			}
			for( var i:int = 0; i < 5; i++ ) 
			{
				TradeConstData.GridUnitList[i].Item = null;
				TradeConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		private function addLis():void
		{
			trade.btnInputMoney.addEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnRemovePet.addEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnLock.addEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnSure.addEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnCancel.addEventListener(MouseEvent.CLICK, btnHandler);
			moneyPanel.content.btnInputSure.addEventListener(MouseEvent.CLICK, btnHandler);
			moneyPanel.content.btnInputCancel.addEventListener(MouseEvent.CLICK, btnHandler);
			moneyPanel.content.txtJin.addEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtYin.addEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtTong.addEventListener(Event.CHANGE, moneyInputHandler);
			petListPanel.content.btnPetChose.addEventListener(MouseEvent.CLICK, btnHandler);
		}
		
		private function removeLis():void
		{
			trade.btnInputMoney.removeEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnRemovePet.removeEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnLock.removeEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnSure.removeEventListener(MouseEvent.CLICK, btnHandler);
			trade.btnCancel.removeEventListener(MouseEvent.CLICK, btnHandler);
			moneyPanel.content.btnInputSure.removeEventListener(MouseEvent.CLICK, btnHandler);
			moneyPanel.content.btnInputCancel.removeEventListener(MouseEvent.CLICK, btnHandler);
			moneyPanel.content.txtJin.removeEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtYin.removeEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtTong.removeEventListener(Event.CHANGE, moneyInputHandler);
			petListPanel.content.btnPetChose.removeEventListener(MouseEvent.CLICK, btnHandler);
		}
		
		private function moneyInputHandler(e:Event):void
		{
			var jin:uint   = uint(moneyPanel.content.txtJin.text);
			var yin:uint   = uint(moneyPanel.content.txtYin.text);
			var tong:uint  = uint(moneyPanel.content.txtTong.text);
			var money:uint = formatMoney(jin,yin,tong);
			if(money > GameCommonData.Player.Role.BindMoney) {
				var moneyArr:Array = UIUtils.getMoney(GameCommonData.Player.Role.BindMoney);
				moneyPanel.content.txtJin.text  = moneyArr[0].toString();
				moneyPanel.content.txtYin.text  = moneyArr[1].toString();
				moneyPanel.content.txtTong.text = moneyArr[2].toString();
			}
		}
		
		/** 显示交易窗 */
		private function showView():void
		{
			tradeUIManager.initPanel();
			addLis();
			tradeDataProxy.opLocked = false;
			tradeDataProxy.sfLocked = false;
			GameCommonData.GameInstance.GameUI.addChild(tradePanel);
			GameCommonData.GameInstance.GameUI.addChild(petListPanel);
			tradePanel.x 	= TRADE_PANEL_POS.x;
			tradePanel.y 	= TRADE_PANEL_POS.y;
			moneyPanel.x 	= MONEY_PANEL_POS.x;
			moneyPanel.y 	= MONEY_PANEL_POS.y;
			petListPanel.x 	= PET_PANEL_POS.x;
			petListPanel.y 	= PET_PANEL_POS.y;
			
			initPetChoiceView();
			initPetListTradeOp();
			initPetListTradeSelf();
			refreshPetBtn();
		}
		
		/** 初始化宠物选择列表 */
		private function initPetChoiceView():void
		{
			if(iScrollPane && petListPanel.content.contains(iScrollPane)) {
				petListPanel.content.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			listView = new ListComponent(false);
//			listView.Offset = 0;
			showFilterList();
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = 3;
			iScrollPane.y = 3;
			iScrollPane.width = 118;
			iScrollPane.height = 135;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			petListPanel.content.addChild(iScrollPane);
		}
		
		private function showFilterList():void
		{
			for(var id:Object in TradeConstData.petListSelfDic)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petTradeChoice_"+id;
				item.doubleClickEnabled = true;
				item.mcSelected.visible = false;
				item.txtName.mouseEnabled = false;
				item.mcSelected.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = false;
				item.txtName.text = TradeConstData.petListSelfDic[id].PetName;
				item.addEventListener(MouseEvent.CLICK, selectItem);
				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
				listView.addChild(item);
			}
			listView.width = 115;
			listView.upDataPos();
		}
		
		private function selectItem(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(TradeConstData.petSelectOfList && id == TradeConstData.petSelectOfList.Id) return;
			for(var i:int = 0; i < listView.numChildren; i++){
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			
			item.mcSelected.visible = true;
			if(GameCommonData.Player.Role.PetSnapList[id]) {
				TradeConstData.petSelectOfList = GameCommonData.Player.Role.PetSnapList[id];
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_sel_1" ], color:0xffff00});         //该宠物不存在
//				sendNotification(EventList.TRADEFAULT);//通知背包交易失败
				sendData(TradeAction.QUIT);
				gcAll();
			}
		}
		
		/** 双击查看宠物属性 */
		private function lookPetInfoHandler(e:MouseEvent):void
		{
			var item:MovieClip = e.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(id > 0) {
				PetPropConstData.isSearchOtherPetInto = true;
				if(GameCommonData.Player.Role.PetSnapList[id]) {
					dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
				} else {
					if(TradeConstData.petSelectOp) {
						dataProxy.PetEudemonTmp = TradeConstData.petSelectOp;
						sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:TradeConstData.petSelectOp.OwnerId});
					}
				}
			}
		}
		
		/** 初始化自己交易栏中宠物列表 */
		private function initPetListTradeSelf():void
		{
			if(listViewPetSf && tradePanel.content.contains(listViewPetSf)) {
				tradePanel.content.removeChild(listViewPetSf);
				listViewPetSf = null;
			}
			listViewPetSf = new ListComponent(false);
			listViewPetSf.x = 217;
			listViewPetSf.y = 270;
			showFilterListSf();
			tradePanel.content.addChild(listViewPetSf);
		}
		
		private function showFilterListSf():void
		{
			for(var id:Object in TradeConstData.petSelfDic)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petTradeSelf_"+id;
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnabled = false;
				item.doubleClickEnabled = true;
				item.txtName.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = false;
				item.txtName.text = GameCommonData.Player.Role.PetSnapList[id].PetName;
				item.addEventListener(MouseEvent.CLICK, selectItemSf);
				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
				listViewPetSf.addChild(item);
			}
			listViewPetSf.width = 110;
			listViewPetSf.upDataPos();
		}
		
		private function selectItemSf(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(TradeConstData.petSelectOfTrade && id == TradeConstData.petSelectOfTrade.Id) return;
			for(var i:int = 0; i < listViewPetSf.numChildren; i++) {
				(listViewPetSf.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			
			item.mcSelected.visible = true;
			if(GameCommonData.Player.Role.PetSnapList[id]) {
				TradeConstData.petSelectOfTrade = GameCommonData.Player.Role.PetSnapList[id];
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_sel_1" ], color:0xffff00});         //该宠物不存在
//				sendNotification(EventList.TRADEFAULT);//通知背包交易失败
				sendData(TradeAction.QUIT);
				gcAll();
			}
		}
		
		/** 初始化自己交易栏中宠物列表 */
		private function initPetListTradeOp():void
		{
			if(listViewPetOp && tradePanel.content.contains(listViewPetOp)) {
				tradePanel.content.removeChild(listViewPetOp);
				listViewPetOp = null;
			}
			listViewPetOp = new ListComponent(false);
			listViewPetOp.x = 48;
			listViewPetOp.y = 270;
			showFilterListOp();
			tradePanel.content.addChild(listViewPetOp);
		}
		
		private function showFilterListOp():void
		{
			for(var id:Object in TradeConstData.petOpDic)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petTradeOp_"+id;
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnabled = false;
				item.doubleClickEnabled = true;
				item.txtName.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = false;
				item.txtName.text = TradeConstData.petOpDic[id].PetName;
				item.addEventListener(MouseEvent.CLICK, selectItemOp);
				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
				listViewPetOp.addChild(item);
			}
			listViewPetOp.width = 110;
			listViewPetOp.upDataPos();
		}
		
		private function selectItemOp(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(TradeConstData.petSelectOp && TradeConstData.petSelectOp.Id == id) return;
			for(var i:int = 0; i < listViewPetOp.numChildren; i++) {
				(listViewPetOp.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			TradeConstData.petSelectOp = TradeConstData.petOpDic[id];
		}
		
		/** 显示交易询问窗 */
		private function showTradeAsk():void
		{
			var pId:int = applyPersonIds[0];
			if(pId && GameCommonData.SameSecnePlayerList[pId]) {
				var person:GameElementAnimal = GameCommonData.SameSecnePlayerList[pId];
				var name:String = person.Role.Name;
				facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade, cancel:cancelClose, isShowClose:false, info: "["+name+"]"+GameCommonData.wordDic[ "mod_tra_med_tra_showt_1" ], title:GameCommonData.wordDic[ "mod_team_med_teamm_isc_2" ], comfirmTxt:GameCommonData.wordDic[ "mod_tra_med_tra_showt_2" ], cancelTxt:GameCommonData.wordDic[ "mod_team_med_teamm_mem_4" ]});    //希望与你交易     提 示    同 意     拒 绝  
			} else {
				applyPersonIds.shift();
				if(applyPersonIds.length > 0) showTradeAsk();
			}
		}
		
		/** 同意交易 */
		private function applyTrade():void
		{
			if(StallConstData.stallSelfId > 0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_han_1" ], color:0xffff00});       //摆摊中不能交易
				cancelClose();
				return;
			}
			var pId:int = applyPersonIds.shift();
			if(pId && GameCommonData.SameSecnePlayerList[pId]) {
				var oAPP:Object = new Object();
				var paramAPP:Array = new Array();
				oAPP.type = Protocol.TRADE_INFO;
				oAPP.data = paramAPP;
				
				paramAPP.push(pId);	//id
				
				paramAPP.push(0);
				paramAPP.push(0);
				paramAPP.push(0);
				paramAPP.push(TradeAction.AGREE_TRADE);//paramAPP.push(TradeAction.APPLY);//action
				TradeSend.createMsgTeam(oAPP);
				
				while(applyPersonIds.length > 0) {				//拒绝其他人的交易请求
					var otherId:int = applyPersonIds.shift();
					if(otherId && GameCommonData.SameSecnePlayerList[otherId]) {
						var oRE:Object = new Object();
						var paramRE:Array = new Array();
						oRE.type = Protocol.TRADE_INFO;
						oRE.data = paramRE;
						
						paramRE.push(otherId);	 //id
						
						paramRE.push(0);
						paramRE.push(0);
						paramRE.push(0);
						paramRE.push(TradeAction.REFUSE);//action
						
						TradeSend.createMsgTeam(oRE);
					}
				}
			} else {
				sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:1});
			}
		}
		
		/** 拒绝交易 */
		private function cancelClose():void
		{
			var pId:int = applyPersonIds.shift();
			if(pId && GameCommonData.SameSecnePlayerList[pId]) {
				var oAPP:Object = new Object();
				var paramAPP:Array = new Array();
				oAPP.type = Protocol.TRADE_INFO;
				oAPP.data = paramAPP;
				
				paramAPP.push(pId);	 //id
				
				paramAPP.push(0);
				paramAPP.push(0);
				paramAPP.push(0);
				paramAPP.push(TradeAction.REFUSE);//action
				
				TradeSend.createMsgTeam(oAPP);
				
				var pName:String = (GameCommonData.SameSecnePlayerList[pId] as  GameElementAnimal).Role.Name;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_team_med_teamm_sho_22" ]+"<font color='#ff0000'>["+pName+"]</font>"+GameCommonData.wordDic[ "mod_tra_med_tra_can_1" ], color:0xffff00});   //你拒绝了        的交易请求
			}
			if(applyPersonIds.length > 0) showTradeAsk();
		}
		
		/** 显示某人拒绝了我的交易请求 提示窗 */
		private function showRefuseAlert(refuserId:int):void
		{
			if(refuserId && GameCommonData.SameSecnePlayerList[refuserId]) {
				var person:GameElementAnimal = GameCommonData.SameSecnePlayerList[refuserId];
				var name:String = person.Role.Name;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+name+"]</font>"+GameCommonData.wordDic[ "mod_tra_med_tra_showr_1" ], color:0xffff00});     //拒绝了你的交易请求
			} else {
				sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:1});
			}
		}
		
		private function alertBack():void
		{
			return;
		}
		
		/** 关闭交易面板 */
		private function tradePanelCloseHandler(e:Event):void
		{
			sendData(TradeAction.QUIT);					//发送取消交易的命令
//			sendNotification(EventList.TRADEFAULT);		//通知背包交易失败
			gcAll();
		}
		
		/** 关闭金钱输入面板 */
		private function moneyPanelCloseHandler(e:Event):void
		{
			gcMoneyPanel();
		}
		
		/** 回收全部面板 */
		private function gcAll():void
		{
			removeLis();
			tradeUIManager.removeAllItem();
			tradeUIManager.removeAllItemOp();
			tradeDataProxy.moneyOp 	 = 0;
			tradeDataProxy.moneySelf = 0;
			TradeConstData.opName = "";
			TradeConstData.goodSelfList = new Array(5);
			TradeConstData.goodOpList = new Array(5);
			TradeConstData.petOpDic = new Dictionary();
			TradeConstData.petSelfDic = new Dictionary();
			TradeConstData.petListSelfDic = new Dictionary();
			TradeConstData.petSelectOfList = null;
			TradeConstData.petSelectOfTrade = null;
			TradeConstData.petSelectOp = null;
			
			if(GameCommonData.GameInstance.GameUI.contains(tradePanel))GameCommonData.GameInstance.GameUI.removeChild(tradePanel);
			if(GameCommonData.GameInstance.GameUI.contains(petListPanel))GameCommonData.GameInstance.GameUI.removeChild(petListPanel);
			gcMoneyPanel();
			UIConstData.IsTrading = false;
			dataProxy.TradeIsOpen = false;
			dataProxy.PetCanOperate = true;
			GameCommonData.Player.Role.State = GameRole.STATE_NULL;
			UIConstData.KeyBoardCanUse = true;		//可用快捷键
			
			for(var i:int = 0; i < TradeConstData.idItemSelfArr.length; i++) {		//解锁
				sendNotification(EventList.BAGITEMUNLOCK, TradeConstData.idItemSelfArr[i]);
			}
			TradeConstData.idItemSelfArr = [];
//			SysCursor.GetInstance().revert();
		}
		
		/** 回收金钱输入面板 */
		private function gcMoneyPanel():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(moneyPanel)) {
				GameCommonData.GameInstance.GameUI.removeChild(moneyPanel);
			}
		}
		
		/** 收到交易数据 更新界面 */
		private function updateView():void
		{
			switch(notiData.action) {
				case TradeAction.LOCK:								//对方锁定
					tradeDataProxy.opLocked = true;
					trade.txtState_op.htmlText = "<font color='#5df029'>" + GameCommonData.wordDic[ "mod_tra_med_tra_upd_1" ] + "</font>";         //已锁定
					trade.mcGreenRectOp.visible = true;
					if(tradeDataProxy.opLocked && tradeDataProxy.sfLocked) {
						trade.btnSure.visible = true;
						sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:7});
					} 
					break;
				case TradeAction.OK:								//对方确认
					trade.txtState_op.htmlText = "<font color='#5df029'>" + GameCommonData.wordDic[ "mod_tra_med_tra_upd_2" ] + "</font>";          //已确认
					if(dataProxy.TradeIsOpen) sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:10});
					break;
				case TradeAction.MONEYALL:							//对方加钱
					tradeDataProxy.moneyOp = notiData.id;
					tradeUIManager.showMoney(0);
					break;
				case TradeAction.SELFMONEYALL:						//自己加钱
					tradeDataProxy.moneySelf = notiData.id;
					tradeUIManager.showMoney(1);
					break;
				case TradeAction.SUCCESS:							//交易成功
					sendNotification(EventList.TRADECOMPLETE);		//通知背包交易成功
					sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:8});
					gcAll();
					break;
				case TradeAction.FALSE:								//交易失败
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_upd_3" ], color:0xffff00});            //交易失败
//					sendNotification(EventList.TRADEFAULT);			//通知背包交易失败
					gcAll();
					break;
				case TradeAction.ADDITEM:							//自己加物品
					tradeUIManager.addItem(1, notiData);
					break;
				case TradeAction.ADDITEM_OP:						//别人加物品
					tradeUIManager.addItem(0, notiData);
					break;
				case TradeAction.BACK_WU:							//自己减物品
					tradeUIManager.delItem(1, notiData);
					sendNotification(EventList.BAGITEMUNLOCK, notiData.id);	//告诉背包解锁物品
					break;
				case TradeAction.BACK_WU_OP:						//对方减物品
					tradeUIManager.delItem(0, notiData);
					break;
				case TradeAction.ADDITEMFAIL:						//自己加物品失败
					break;
				case TradeAction.DELITEMFAIL:						//自己减物品失败
					break;
				
			}
		}
		/** 输入银两，移除宠物，锁定交易，确认交易，取消交易 */
		private function btnHandler(e:MouseEvent):void
		{
			var btnName:String = e.target.name;
			switch(btnName) {
				case "btnInputMoney":													//输入银两
					if(!GameCommonData.GameInstance.GameUI.contains(moneyPanel)) {
						GameCommonData.GameInstance.GameUI.addChild(moneyPanel);
						var moneyArr:Array = tradeUIManager.getMoney(tradeDataProxy.moneySelf);
						moneyPanel.content.txtJin.text  = moneyArr[0].toString();
						moneyPanel.content.txtYin.text  = moneyArr[1].toString();
						moneyPanel.content.txtTong.text = moneyArr[2].toString();
					} else {
						gcMoneyPanel();
					}
					break;
				case "btnInputSure":													//输入金钱确认
					sendData(TradeAction.ADDMONEY);
					gcMoneyPanel();
					break;
				case "btnInputCancel":													//输入金钱取消
					gcMoneyPanel();
					break;
				case "btnRemovePet":													//移除宠物
					if(TradeConstData.petSelectOfTrade && tradeUIManager.getPetCountSelf() > 0) {
						if(GameCommonData.Player.Role.PetSnapList[TradeConstData.petSelectOfTrade.Id]) {
							sendData(TradeAction.PET_DEL_SELF);
							lockPetOp(false);
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_sel_1" ], color:0xffff00});        //该宠物不存在
							sendNotification(EventList.TRADEFAULT);//通知背包交易失败
							sendData(TradeAction.QUIT);
							gcAll();
						}
					}
					break;
				case "btnPetChose":														//选择宠物   点击后宠物列表中的宠物 进交易栏
					if(TradeConstData.petSelectOfList && tradeUIManager.getPetCountSelf() < 3) {
						if(GameCommonData.Player.Role.PetSnapList[TradeConstData.petSelectOfList.Id]) {
							if(GameCommonData.Player.Role.PetSnapList[TradeConstData.petSelectOfList.Id].State == 1) {
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_btn_1" ], color:0xffff00});       //出战状态的宠物不能交易
								return;
							}
							sendData(TradeAction.PET_ADD_SELF);
							lockPetOp(false);
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_sel_1" ], color:0xffff00});            //该宠物不存在
							sendNotification(EventList.TRADEFAULT);//通知背包交易失败
							sendData(TradeAction.QUIT);
							gcAll();
						}
					}
					break;
				case "btnLock":															//锁定交易
					trade.txtState_self.htmlText = "<font color='#5df029'>" + GameCommonData.wordDic[ "mod_tra_med_tra_upd_1" ] + "</font>";         //已锁定
					trade.btnLock.visible 		= false; 
					trade.btnInputMoney.visible = false;
					trade.btnRemovePet.visible  = false;
					trade.mcGreenRect.visible 	= true;
					petListPanel.content.btnPetChose.visible = false; //宠物选择按钮
					tradeDataProxy.sfLocked = true;
					if(tradeDataProxy.opLocked && tradeDataProxy.sfLocked) {
						trade.btnSure.visible = true;
						sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:7});
					}
					gcMoneyPanel();	//禁用物品拖拽
					sendData(TradeAction.LOCK);
					break;
				case "btnSure":															//确认交易
					trade.btnSure.visible = false;
					trade.txtState_self.htmlText = "<font color='#5df029'>" + GameCommonData.wordDic[ "mod_tra_med_tra_upd_2" ] + "</font>";            //已确认
					if(trade.txtState_op.text != GameCommonData.wordDic[ "mod_tra_med_tra_upd_2" ]) sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:9});            //已确认
					sendData(TradeAction.OK);
					break;
				case "btnCancel":														//取消交易
					sendNotification(EventList.TRADEFAULT);//通知背包交易失败
					sendData(TradeAction.QUIT);
					gcAll();
					break;
				default:
					
					break;
			}
		}
				
		private function formatMoney(jin:Number, yin:Number, tong:Number):Number
		{
			return (10000 * jin + 100 * yin + tong);	//1金=100银，1银=100铜
		}
		
		/** 更新按钮状态 移除宠物、选择 */
		private function refreshPetBtn():void
		{
			trade.btnRemovePet.visible = (tradeUIManager.getPetCountSelf() > 0) ? true : false; 
			petListPanel.content.btnPetChose.visible = (tradeUIManager.getPetCountSelf() >= 3 || tradeUIManager.getPetCountList() <= 0) ? false : true;
		}

		/** 发送数据 */
		private function sendData(action:int):void 
		{
			switch(action) {
				case TradeAction.ADDMONEY:											//增加金钱
					var jin:Number   = Number(moneyPanel.content.txtJin.text);
					var yin:Number   = Number(moneyPanel.content.txtYin.text);
					var tong:Number  = Number(moneyPanel.content.txtTong.text);
					var money:Number = formatMoney(jin,yin,tong);
					if(money != tradeDataProxy.moneySelf) { //钱数有变化
						TradeNetAction.tradeOperate(action, money);
					}
					break;
				case TradeAction.LOCK:												//锁定交易			
					TradeNetAction.tradeOperate(action, GameCommonData.Player.Role.Id);
					break;
				case TradeAction.OK:												//确认交易
					TradeNetAction.tradeOperate(action, GameCommonData.Player.Role.Id);
					break;
				case TradeAction.QUIT:												//取消交易
					TradeNetAction.tradeOperate(action, GameCommonData.Player.Role.Id);
					break;
				case TradeAction.BACK_WU:											//移除物品、宠物
					TradeNetAction.tradeOperate(action, tradeDataProxy.goodPetOperating.id);
					break;
				case TradeAction.ADDITEM:											//添加物品
					TradeNetAction.tradeOperate(action, notiData.id);
					break;
				case TradeAction.APPLY:												//申请与别人交易
					TradeNetAction.tradeOperate(action, notiData.id);
					break;
				case TradeAction.PET_ADD_SELF:										//自己加宠物
					TradeNetAction.tradeOperate(action, TradeConstData.petSelectOfList.Id);
					break;
				case TradeAction.PET_DEL_SELF:
					TradeNetAction.tradeOperate(action, TradeConstData.petSelectOfTrade.Id);
					break
			}
		}
		
		/** 显示交易相关的信息提示文字 */
		private function showTradeInformation(infoObj:Object):void
		{
			switch(infoObj.type) {
				case 1:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_1" ], color:0xffff00});           //交易请求超时或者距离太远，交易失败
					break;
				case 2:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_2" ], color:0xffff00});            //你已经正在交易
					break;
				case 3:																											//
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"交易请求已发送，请耐心等待<font color='#ff0000'>["+notiData.name+"]</font>回复", color:0xffff00});
					break;
				case 4:																											//
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你拒绝了<font color='#ff0000'>["+tradePerson.name+"]</font>的交易请求", color:0xffff00});
					break;
				case 5:																											//
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+tradePerson.name+"]</font>拒绝了你的交易请求", color:0xffff00});
					break;
				case 6:																											//
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你正在与<font color='#ff0000'>["+tradePerson.name+"]</font>交易", color:0xffff00});
					break;
				case 7:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_3" ], color:0xffff00});         //交易成功锁定，请确认交易
					break;
				case 8:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_4" ], color:0xffff00});           //交易成功
					break;
				case 9:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_5" ], color:0xffff00});         //对方还没有确认交易，请耐心等候
					break;
				case 10:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_6" ], color:0xffff00});            //对方已确认
					break;
				case 11:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_7" ], color:0xffff00});            //交易已取消
					break;
				case 12:				
					var opId:int = int(infoObj.data);
					if(!GameCommonData.SameSecnePlayerList[opId]) {
						sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:1});
						return;
					}
					var opPerson:GameElementAnimal = GameCommonData.SameSecnePlayerList[opId];
					var opName:String = opPerson.Role.Name;
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"<font color='#ff0000'>["+opName+"]</font>"+GameCommonData.wordDic[ "mod_tra_med_tra_showtr_8" ], color:0xffff00});        //正在交易，请稍候
					break;
				case 13:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_9" ], color:0xffff00});        //对方交易金钱或物品有变动，锁定自动解除
					break;
				case 14:																											//
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_10" ], color:0xffff00});         //你的交易金钱或物品有变动，锁定自动解除
					break;
				case 15:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_tra_med_tra_showtr_11" ], color:0xffff00});           //死亡状态不能交易
					break;
					
			}
		}
		
		/** 是否锁住宠物操作按钮 */
		private function lockPetOp(mouseEnabled:Boolean=false):void
		{
			trade.btnRemovePet.mouseEnabled = mouseEnabled;
			petListPanel.content.btnPetChose.mouseEnabled = mouseEnabled;
		}
		
		
	}

}
