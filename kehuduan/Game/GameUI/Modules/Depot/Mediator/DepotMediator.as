package GameUI.Modules.Depot.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Depot.Proxy.DepotNetAction;
	import GameUI.Modules.Depot.Proxy.ItemGridManager;
	import GameUI.Modules.Depot.UI.DepotUIManager;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.DepotAction;
	
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	/**
	 * 仓库视图管理
	 * @
	 * @
	 */ 
	public class DepotMediator extends Mediator
	{
		public static const NAME:String  = "DepotMediator";
		private static const START_POS:Point = new Point(0, 17);
		private static const MAXPAGE:uint = 2;
		
		private var dataProxy:DataProxy = null;
		private var depotUIManager:DepotUIManager = null;
		private var itemGridManager:ItemGridManager = null;
		
		private var depotPanel:PanelBase = null;
		private var moneyPanel:PanelBase = null;
		private var gridSprite:MovieClip = null;
		private var petSprite:MovieClip  = null;
		private var yellowFilter:GlowFilter = null;
		///////仓库宠物栏
		private var listView_0:ListComponent = null;
		private var iScrollPane_0:UIScrollPane = null;
		//////背包宠物栏
		private var listView_1:ListComponent = null;
		private var iScrollPane_1:UIScrollPane = null;
		
		private var selectType:int = 0;		/** 选中的宠物所在位置 0-仓库、1-宠物背包 */
		//////
		
		public function DepotMediator()
		{
			super(NAME);
		}
		
		private function get depot():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOWDEPOTVIEW,		//打开仓库
				EventList.CLOSEDEPOTVIEW,		//关闭仓库
				EventList.CLOSE_NPC_ALL_PANEL,	//关闭由NPC打开的面板
				DepotEvent.INITMONEY,			//显示金钱和物品
				DepotEvent.BAGTODEPOT,			//背包物品拖向仓库
				DepotEvent.DEPOTTOBAG,			//仓库物品拖向背包
				DepotEvent.ADDITEM,				//增加物品
				DepotEvent.DELITEM,				//删除物品
				DepotEvent.UPDATEMONEY,			//更新金钱
				DepotEvent.EXTITEMDEPOT,		//仓库扩容
				DepotEvent.IN_OUT_PET_UPDATE_DEPOT	//存取宠物更新仓库画面
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOWDEPOTVIEW:									//打开仓库
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(dataProxy.DepotIsOpen) {//仓库已打开了（此情况一般不会发生，这里是做容错处理）
						dataProxy.DepotIsOpen = false;
						gcAll();
						return;
					}
					if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_DEAD) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_hand_1" ], color:0xffff00});//"死亡状态不能打开仓库"
						return;
					} 
					if(dataProxy.TradeIsOpen) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_hand_2" ], color:0xffff00});//"交易中不能打开仓库"
						dataProxy = null;
						return;
					} else if(StallConstData.stallSelfId != 0) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_hand_3" ], color:0xffff00});//"摆摊中不能打开仓库"
						dataProxy = null;
						return;
					}
					if(dataProxy.NPCShopIsOpen) {//关掉NPC商店
						sendNotification(EventList.CLOSENPCSHOPVIEW);
					}
					if(dataProxy.equipPanelIsOpen) {	//关掉强化界面
						sendNotification(EquipCommandList.CLOSE_EQUIP_PANEL);
					}
					yellowFilter = UIUtils.getGlowFilter(0xffff00, 1, 3, 3, 6);
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.DEPOT});
					depotUIManager = DepotUIManager.getInstance();
					initView();
					addLis();
					dataProxy.DepotIsOpen = true;
					if(!dataProxy.BagIsOpen){
						facade.sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
					}
//					GameCommonData.Player.Role.State = GameRole.STATE_DEPOT;
					break;
				case EventList.CLOSEDEPOTVIEW:									//关闭仓库
					if(dataProxy.DepotIsOpen) gcAll();
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:								//关闭由NPC打开的面板
					if(dataProxy.DepotIsOpen) gcAll();
					break;
				case DepotEvent.INITMONEY:										//收到金钱，开始显示
					depotUIManager.refreshMoney();
					depotUIManager.lockItemDptBtn(true);//解锁仓库按钮
					itemGridManager.lockGrids(true);	//解锁格子
					lockPageBtn(true);					//解锁页签
					initItemShow();
					initPetView_0();
					break;
				case DepotEvent.BAGTODEPOT:										//背包物品拖向仓库
					var notiData:Object = notification.getBody();
					var indexToAdd:int = notiData.index;
					if(indexToAdd == -1) {
						indexToAdd = 60000;
						DepotNetAction.sendDepotOrder(notiData.id, DepotConstData.curDepotIndex+1, indexToAdd, DepotAction.ITEM_IN);
					} else {
						indexToAdd = notiData.index+1+DepotConstData.curDepotIndex*36;
						DepotNetAction.sendDepotOrder(notiData.id, 0, indexToAdd, DepotAction.ITEM_IN);
					}
					break;
				case DepotEvent.DEPOTTOBAG:										//仓库物品拖向背包
					var notiData1:Object = notification.getBody();
					var indexToDel:int = notiData1.index + 1;
					DepotNetAction.sendDepotOrder(notiData1.id, 0, indexToDel, DepotAction.ITEM_OUT);
					break;
				case DepotEvent.ADDITEM:										//增加物品
					var index:int = int(notification.getBody());
					itemGridManager.addItem(index);
					break;
				case DepotEvent.DELITEM:										//删除物品
					var id:int = int(notification.getBody());
					itemGridManager.removeItem(id);
					break;
				case DepotEvent.UPDATEMONEY:									//更新金钱
					depotUIManager.refreshMoney();
					break;
				case DepotEvent.EXTITEMDEPOT:									//仓库扩容
					//作废，打开仓库时使用物品=将该物品存入仓库，所以真正使用物品时仓库是没有打开的，所有这里收不到消息
//					if(dataProxy.DepotIsOpen) {
//						petSprite.txtPetNumDepot.text = count + "/" + DepotConstData.petDepotNum;
//					}
					break;
				case DepotEvent.IN_OUT_PET_UPDATE_DEPOT:						//存取宠物更新仓库画面
					initPetView_0();
					initPetView_1();
					break;
			}
		}
		
		private function initView():void
		{
			var moneyInput:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MoneyInput");
			(moneyInput.txtJin as TextField).restrict = "0-9";
			(moneyInput.txtYin as TextField).restrict = "0-9";
			(moneyInput.txtTong as TextField).restrict = "0-9";
			gridSprite = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ItemDepot");
			petSprite = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetDepot");
			
			gridSprite.name = "depotView"; 
			gridSprite.x = START_POS.x;
			gridSprite.y = START_POS.y;
			petSprite.x = START_POS.x;
			petSprite.y = START_POS.y;
			
			depotUIManager.moneyInput = moneyInput;
			depotUIManager.itemDepot  = gridSprite;
			depotUIManager.petDepot   = petSprite;
			
			depotPanel = new PanelBase(depot, depot.width+8, depot.height+12);
			depotPanel.name = "depotPanel";
			depotPanel.addEventListener(Event.CLOSE, depotCloseHandler);
			depotPanel.x = DepotConstData.DEPOT_DEFAULT_POS.x;
			depotPanel.y = DepotConstData.DEPOT_DEFAULT_POS.y;
			depotPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_dep_med_dep_ini_1" ]);//"仓 库"	
			moneyPanel = new PanelBase(moneyInput, moneyInput.width+8, moneyInput.height+12);
			moneyPanel.name = "moneyDepotPanel";
			moneyPanel.addEventListener(Event.CLOSE, moneyCloseHandler);
			moneyPanel.x = DepotConstData.MONEY_DEFAULT_POS.x;
			moneyPanel.y = DepotConstData.MONEY_DEFAULT_POS.y;
			moneyPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_dep_med_dep_ini_2" ]);//"存入金额"
//			SoundManager.PlaySound(SoundList.PANEOPEN);		
			GameCommonData.GameInstance.GameUI.addChild(depotPanel);
			
			for(var i:int = 0; i < MAXPAGE; i++)
			{
				depot["mcPage_"+i].buttonMode = true;
				depot["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
				if(i == DepotConstData.SelectIndex) 
				{		
					depot["mcPage_"+i].gotoAndStop(1);	
					depot["mcPage_"+i].removeEventListener(MouseEvent.CLICK, choicePageHandler);
					continue;
				}
				depot["mcPage_"+i].gotoAndStop(2);
			}
			
			var itemExtMediator:ItemExtendsMediator = new ItemExtendsMediator();
			facade.registerMediator(itemExtMediator);
			var petExtMediator:PetExtendsMediator = new PetExtendsMediator();
			facade.registerMediator(petExtMediator);
			
			depotUIManager.initItemDepot();
			depotUIManager.initPetDepot();
			depotUIManager.setPageNum();
			initGrid();
			
			itemGridManager.showItems(DepotConstData.goodList);
			depotUIManager.lockItemDptBtn(false);//加锁仓库按钮
			itemGridManager.lockGrids(false);	 //加锁格子
			lockPageBtn(false);					 //加锁页签
			DepotNetAction.sendDepotOrder(GameCommonData.Player.Role.Id, 0, 4, DepotAction.QUERY_LIST);
			DepotNetAction.sendDepotOrder(GameCommonData.Player.Role.Id, 0, DepotConstData.curDepotIndex+1, DepotAction.QUERY_LIST);
			
//			//宠物仓库
			
			initPetView_1();
		}
		
		private function initGrid():void
		{
			for(var i:int = 0; i < 36; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6) + 5;
				gridUnit.y = (gridUnit.height) * int(i/6) + 5;
				gridUnit.name = "depot_" + i.toString();
				gridSprite.addChild(gridUnit);	//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = gridSprite;								//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				DepotConstData.GridUnitList.push(gridUint);
			}
			depot.addChildAt(gridSprite, 0);
			itemGridManager = new ItemGridManager(DepotConstData.GridUnitList, gridSprite);
			facade.registerProxy(itemGridManager);
		}
		
		/** 开始显示物品 */
		private function initItemShow():void
		{
			itemGridManager.removeAllItem();
			itemGridManager.showItems(DepotConstData.goodList);
		}
		
		/** 选择不同页签 物品、宠物 */
		private function choicePageHandler(event:MouseEvent):void
		{
			for(var i:int = 0; i < MAXPAGE; i++) {
				depot["mcPage_"+i].gotoAndStop(2);
				depot["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			var index:uint = uint(event.target.name.split("_")[1]);
			depot["mcPage_"+index].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			DepotConstData.SelectIndex = index;
			depot["mcPage_"+index].gotoAndStop(1);
			if(depot.getChildByName("yellowFrame")) {
				depot.removeChild(gridSprite.getChildByName("yellowFrame"));
			}
			if(DepotConstData.SelectIndex == 0) {					//物品
				//请求 物品数据
				if(depot.contains(petSprite)) depot.removeChild(petSprite);
				depot.addChildAt(gridSprite, 0);
				if(!dataProxy.PetCanOperate) {
					sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
					dataProxy.PetCanOperate = true;
				}
				if(DepotConstData.petExtIsOpen) sendNotification(DepotEvent.REMOVEPETEXT);
			} else if(DepotConstData.SelectIndex == 1) {			//宠物
				//请求 宠物数据
				if(depot.contains(gridSprite)) depot.removeChild(gridSprite);
				depot.addChildAt(petSprite, 0);
				if(dataProxy.PetIsOpen) {
					sendNotification(EventList.CLOSEPETVIEW);
				}
				dataProxy.PetCanOperate = false;
				if(DepotConstData.itemExtIsOpen) sendNotification(DepotEvent.REMOVEITEMEXT);
				gcMoneyPanel();
			}
		}
		
		/** 初始化仓库宠物栏 */
		private function initPetView_0():void
		{
			initPetChoiceListData_0();
		}
		
		/** 初始化宠物背包栏 */
		private function initPetView_1():void
		{
			initPetChoiceListData_1();
		}
		
		/** 初始化宠物选择列表数据 */
		private function initPetChoiceListData_0():void
		{
			if(iScrollPane_0 && petSprite.contains(iScrollPane_0)) {
				petSprite.removeChild(iScrollPane_0);
				iScrollPane_0 = null;
				listView_0 = null;
			}
			listView_0 = new ListComponent(false);
			showFilterList_0();
			iScrollPane_0 = new UIScrollPane(listView_0);
			iScrollPane_0.x = 10;
			iScrollPane_0.y = 24;
			iScrollPane_0.width  = 207;
			iScrollPane_0.height = 117;
			iScrollPane_0.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane_0.refresh();
			petSprite.addChild(iScrollPane_0);
		}
		
		private function showFilterList_0():void
		{
			var count:uint = 0;
			for(var id:Object in DepotConstData.petListDepot)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemDepot");
				item.name = "petDepotList0_"+id;
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnabled = false;
				item.width = 206;
				item.mcSelected.width = 206;
				item.txtName.width = 206;
				item.txtName.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = false;
				item.txtName.text = DepotConstData.petListDepot[id].PetName;
				item.addEventListener(MouseEvent.CLICK, selectItem_0);
				listView_0.addChild(item);
				count++;
			}
			listView_0.width = 206;
			listView_0.upDataPos();
			if(count == 0) DepotConstData.curPetType = 0;
			petSprite.txtPetNumDepot.text = count + "/" + DepotConstData.petDepotNum;
		}
		
		private function selectItem_0(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			
			if(!DepotConstData.petListDepot[id]) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_sle_1" ], color:0xffff00});//"此宠物不存在"
				gcAll(); 
				return;
			}
			
			for(var i:int = 0; i < listView_0.numChildren; i++)
			{
				(listView_0.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			for(var j:int = 0; j < listView_1.numChildren; j++)
			{
				(listView_1.getChildAt(j) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			petSprite.txtInOrOut.text = GameCommonData.wordDic[ "mod_dep_med_dep_sle_2" ];//"取出"
			//更换宠物数据
			DepotConstData.petSelected = DepotConstData.petListDepot[id];
			DepotConstData.curPetType = 1;
			
			if(event.ctrlKey) {
				var type:uint = DepotConstData.petSelected.ClassId;
				var name:String = DepotConstData.petSelected.PetName;
				if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
					facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<2_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_0_4>");
				} else {
					facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<2_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_0_4>");
				}
			}
		}
//		
		/** 初始化宠物选择列表数据 */
		private function initPetChoiceListData_1():void
		{
			if(iScrollPane_1 && petSprite.contains(iScrollPane_1)) {
				petSprite.removeChild(iScrollPane_1);
				iScrollPane_1 = null;
				listView_1 = null;
			}
			listView_1 = new ListComponent(false);
			showFilterList_1();
			iScrollPane_1 = new UIScrollPane(listView_1);
			iScrollPane_1.x = 10;
			iScrollPane_1.y = 167;
			iScrollPane_1.width  = 207;
			iScrollPane_1.height = 117;
			iScrollPane_1.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane_1.refresh();
			petSprite.addChild(iScrollPane_1);
		}
		
		private function showFilterList_1():void
		{
			var count:uint = 0;
			for(var id:Object in GameCommonData.Player.Role.PetSnapList)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemDepot");
				item.name = "petDepotList1_"+id;
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnabled = false;
				item.width = 206;
				item.btnChosePet.width = 206;
				item.mcSelected.width = 206;
				item.txtName.width = 206;
				item.txtName.mouseEnabled = false;
				item.txtName.text = GameCommonData.Player.Role.PetSnapList[id].PetName;
				item.addEventListener(MouseEvent.CLICK, selectItem_1);
				listView_1.addChild(item);
				count++;
			}
			listView_1.width = 206;
			listView_1.upDataPos();
			if(count == 0) DepotConstData.curPetType = 0;
			petSprite.txtPetNumSelf.text = count + "/" + PetPropConstData.petBagNum;
		}
		
		private function selectItem_1(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			
			if(!GameCommonData.Player.Role.PetSnapList[id]) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_sle_1" ], color:0xffff00});//"此宠物不存在"
				gcAll();
				return;
			}
			for(var j:int = 0; j < listView_0.numChildren; j++)
			{
				(listView_0.getChildAt(j) as MovieClip).mcSelected.visible = false;
			}
			for(var i:int = 0; i < listView_1.numChildren; i++)
			{
				(listView_1.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			petSprite.txtInOrOut.text = GameCommonData.wordDic[ "mod_dep_med_dep_sle2" ];//"存入"
			//更换宠物数据
			DepotConstData.petSelected = GameCommonData.Player.Role.PetSnapList[id];
			DepotConstData.curPetType = 2;
			if(event.ctrlKey) {
				var type:uint = DepotConstData.petSelected.ClassId;
				var name:String = DepotConstData.petSelected.PetName;
				if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
					facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<2_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_0_4>");
				} else {
					facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<2_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_0_4>");
				}
			}
		}
//		
		
		private function addLis():void 
		{
			gridSprite.mcItemPage_0.addEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcItemPage_1.addEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcItemPage_2.addEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcItemPage_0.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcItemPage_1.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcItemPage_2.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcItemPage_0.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcItemPage_1.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcItemPage_2.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcNotOpen_1.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcNotOpen_1.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcNotOpen_2.addEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcNotOpen_2.addEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcNotOpen_1.addEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcNotOpen_2.addEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.btnMoneyIn.addEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.btnMoneyOut.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSprite.btnExtPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSprite.btnLookPetInfo.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSprite.btnInOrOut.addEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.btnInputSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.btnInputCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.txtJin.addEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtYin.addEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtTong.addEventListener(Event.CHANGE, moneyInputHandler);
			UIUtils.addFocusLis(moneyPanel.content.txtJin);
			UIUtils.addFocusLis(moneyPanel.content.txtYin);
			UIUtils.addFocusLis(moneyPanel.content.txtTong);
		}
		
		private function removeLis():void 
		{
			gridSprite.mcItemPage_0.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcItemPage_1.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcItemPage_2.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcItemPage_0.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcItemPage_1.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcItemPage_2.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcItemPage_0.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcItemPage_1.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcItemPage_2.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcNotOpen_1.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcNotOpen_1.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcNotOpen_2.removeEventListener(MouseEvent.MOUSE_OVER, mouseOverHandler);
			gridSprite.mcNotOpen_2.removeEventListener(MouseEvent.MOUSE_OUT, mouseOutHandler);
			gridSprite.mcNotOpen_1.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.mcNotOpen_2.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.btnMoneyIn.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			gridSprite.btnMoneyOut.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSprite.btnExtPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSprite.btnLookPetInfo.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSprite.btnInOrOut.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.btnInputSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.btnInputCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			for(var i:int = 0; i < MAXPAGE; i++) {
				if(depot["mcPage_"+i].hasEventListener(MouseEvent.CLICK)) depot["mcPage_"+i].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			}
			moneyPanel.content.txtJin.removeEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtYin.removeEventListener(Event.CHANGE, moneyInputHandler);
			moneyPanel.content.txtTong.removeEventListener(Event.CHANGE, moneyInputHandler);
			UIUtils.removeFocusLis(moneyPanel.content.txtJin);
			UIUtils.removeFocusLis(moneyPanel.content.txtYin);
			UIUtils.removeFocusLis(moneyPanel.content.txtTong);
		}
		
		private function mouseOverHandler(e:MouseEvent):void
		{
			(e.target as MovieClip).filters = [yellowFilter];
		}
		private function mouseOutHandler(e:MouseEvent):void
		{
			(e.target as MovieClip).filters = null;
		}
		
		private function moneyInputHandler(e:Event):void
		{
			var moneyType:int = DepotConstData.curMoneyType;
			var jin:uint   = uint(moneyPanel.content.txtJin.text);
			var yin:uint   = uint(moneyPanel.content.txtYin.text);
			var tong:uint  = uint(moneyPanel.content.txtTong.text);
			var money:uint = formatMoney(jin,yin,tong);
			switch(moneyType) {
				case 1:		//存钱
					if(money > GameCommonData.Player.Role.BindMoney) {
						var moneyArr:Array = UIUtils.getMoney(GameCommonData.Player.Role.BindMoney);
						moneyPanel.content.txtJin.text  = moneyArr[0].toString();
						moneyPanel.content.txtYin.text  = moneyArr[1].toString();
						moneyPanel.content.txtTong.text = moneyArr[2].toString();
					}
					break;
				case 2:		//取钱
					if(money > DepotConstData.moneyDepot) {
						var moneyArrDepot:Array = UIUtils.getMoney(DepotConstData.moneyDepot);
						moneyPanel.content.txtJin.text  = moneyArrDepot[0].toString();
						moneyPanel.content.txtYin.text  = moneyArrDepot[1].toString();
						moneyPanel.content.txtTong.text = moneyArrDepot[2].toString();
					}
					break;
			}
		}
		
		/** 点击按钮 第一仓库、第二仓库、第三仓库、金钱确定、金钱取消 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "mcItemPage_0":												//第一仓库
					if(DepotConstData.curDepotIndex == 0) return;
					DepotConstData.curDepotIndex = 0;
					depotUIManager.setPageBtnModel(0);	 //设置按钮
					depotUIManager.lockItemDptBtn(false);//加锁仓库按钮
					itemGridManager.lockGrids(false);	 //加锁格子
					lockPageBtn(false);					 //加锁页签
					DepotConstData.goodList = new Array(36);
					DepotConstData.curDepotIndex = 0;
					itemGridManager.removeAllItem();
					itemGridManager.showItems(DepotConstData.goodList);
					//请求0页数据
					DepotNetAction.sendDepotOrder(GameCommonData.Player.Role.Id, 0, 1, DepotAction.QUERY_LIST);
					break;
				case "mcItemPage_1":												//第二仓库
					if(DepotConstData.curDepotIndex == 1) return;
					DepotConstData.curDepotIndex = 0;
					depotUIManager.setPageBtnModel(1);	 //设置按钮
					depotUIManager.lockItemDptBtn(false);//加锁仓库按钮
					itemGridManager.lockGrids(false);	 //加锁格子
					lockPageBtn(false);					 //加锁页签
					DepotConstData.goodList = new Array(36);
					DepotConstData.curDepotIndex = 1;
					itemGridManager.removeAllItem();
					itemGridManager.showItems(DepotConstData.goodList);
					//请求1页数据
					DepotNetAction.sendDepotOrder(GameCommonData.Player.Role.Id, 0, 2, DepotAction.QUERY_LIST);
					break;
				case "mcItemPage_2":												//第三仓库
					if(DepotConstData.curDepotIndex == 2) return;
					DepotConstData.curDepotIndex = 0;
					depotUIManager.setPageBtnModel(2);	 //设置按钮
					depotUIManager.lockItemDptBtn(false);//加锁仓库按钮
					itemGridManager.lockGrids(false);	 //加锁格子
					lockPageBtn(false);					 //加锁页签
					DepotConstData.goodList = new Array(36);
					DepotConstData.curDepotIndex = 2;
					itemGridManager.removeAllItem();
					itemGridManager.showItems(DepotConstData.goodList);
					//请求2页数据
					DepotNetAction.sendDepotOrder(GameCommonData.Player.Role.Id, 0, 3, DepotAction.QUERY_LIST);
					break;
				case "btnMoneyIn":													//存钱
					if(DepotConstData.curMoneyType == 1) return;
					DepotConstData.curMoneyType = 1;
					showMoneyPanel(0);
					break;
				case "btnMoneyOut":													//取钱
					if(DepotConstData.curMoneyType == 2) return;
					DepotConstData.curMoneyType = 2;
					showMoneyPanel(1);
					break;
				case "btnInputSure":												//金钱输入 确定
					var moneyType:int = DepotConstData.curMoneyType;
					var jin:uint   = uint(moneyPanel.content.txtJin.text);
					var yin:uint   = uint(moneyPanel.content.txtYin.text);
					var tong:uint  = uint(moneyPanel.content.txtTong.text);
					var money:uint = formatMoney(jin,yin,tong);
					if(money <= 0) return;
					switch(moneyType) {
						case 0:
							return;
						case 1:
							//发送存钱命令
							DepotNetAction.sendDepotOrder(money, 0, 0, DepotAction.MONEY_IN);
							gcMoneyPanel();
							break;
						case 2:
							//发送取钱命令
							DepotNetAction.sendDepotOrder(money, 0, 0, DepotAction.MONEY_OUT);
							gcMoneyPanel();
							break;
					}
					break;
				case "btnInputCancel":												//金钱输入 取消
					gcMoneyPanel();
					break;
				case "btnExtPet":													//扩充宠物栏
					if(!DepotConstData.petExtIsOpen) {
						var petExtMediator:PetExtendsMediator = new PetExtendsMediator();
						facade.registerMediator(petExtMediator);
						facade.sendNotification(DepotEvent.SHOWPETEXT);
					} else {
						sendNotification(DepotEvent.REMOVEPETEXT);
					}
					break;
				case "btnLookPetInfo":												//查看宠物资料
					if(DepotConstData.petSelected) {
						dataProxy.PetEudemonTmp = DepotConstData.petSelected;
						PetPropConstData.isSearchOtherPetInto = true; 
						sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:DepotConstData.petSelected.Id, ownerId:GameCommonData.Player.Role.Id});
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_btn_1" ], color:0xffff00});//"请先选择宠物"
					}
					break;
				case "btnInOrOut":													//取出/存入宠物
					switch(DepotConstData.curPetType) {
						case 0:
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_btn_1" ], color:0xffff00});//"请先选择宠物"
							return;
						case 1:
							//发送取出宠物命令
							if(DepotConstData.petSelected) {
								DepotNetAction.sendDepotOrder(DepotConstData.petSelected.Id, 0, 0, DepotAction.PET_OUT_DEPOT);
								DepotConstData.curPetType = 0;
								DepotConstData.petSelected = null;
								petSprite.btnInOrOut.mouseEnabled = false;
								if(listView_0) {
									for(var i:int = 0; i < listView_0.numChildren; i++)
									{
										(listView_0.getChildAt(i) as MovieClip).mcSelected.visible = false;
									}
								}
								if(listView_1) {
									for(var j:int = 0; j < listView_1.numChildren; j++)
									{
										(listView_1.getChildAt(j) as MovieClip).mcSelected.visible = false;
									}
								}
								setTimeout(unLockBtnInOut, 1500);
							}
							break;
						case 2:
							//发送存入宠物命令
							if(DepotConstData.petSelected) {
								if(GameCommonData.Player.Role.PetSnapList[DepotConstData.petSelected.Id].State == 1) {
									facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_btn_2" ], color:0xffff00});//"出战状态的宠物不能存入仓库"
									return;
								}
								if(GameCommonData.Player.Role.PetSnapList[DepotConstData.petSelected.Id].IsLock == true) {
									facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_med_dep_btn_3" ], color:0xffff00});//"此宠物已锁定"
									return;
								}
								DepotNetAction.sendDepotOrder(DepotConstData.petSelected.Id, 0, 0, DepotAction.PET_IN_DEPOT);
								DepotConstData.curPetType = 0;
								DepotConstData.petSelected = null;
								petSprite.btnInOrOut.mouseEnabled = false;
								if(listView_0) {
									for(var i1:int = 0; i1 < listView_0.numChildren; i1++)
									{
										(listView_0.getChildAt(i1) as MovieClip).mcSelected.visible = false;
									}
								}
								if(listView_1) {
									for(var j1:int = 0; j1 < listView_1.numChildren; j1++)
									{
										(listView_1.getChildAt(j1) as MovieClip).mcSelected.visible = false;
									}
								}
								setInterval(unLockBtnInOut, 1500);
							}
							break;
					}
					break;
				case "mcNotOpen_1":
					var a:* = depot.mcItemPage_1;
//					if(a) {
//						return;
//					}
					if(!DepotConstData.itemExtIsOpen)
					{
						var extendsMediator:ItemExtendsMediator = new ItemExtendsMediator();
						facade.registerMediator(extendsMediator);
						facade.sendNotification(DepotEvent.SHOWITEMEXT);
					} else {
						sendNotification(DepotEvent.REMOVEITEMEXT);
					}
					break;
				case "mcNotOpen_2":
//					if(depot.mcItemPage_2.visible) {
//						return;
//					}
					if(!DepotConstData.itemExtIsOpen)
					{
						var extendsMediator1:ItemExtendsMediator = new ItemExtendsMediator();
						facade.registerMediator(extendsMediator1);
						facade.sendNotification(DepotEvent.SHOWITEMEXT);
					} else {
						sendNotification(DepotEvent.REMOVEITEMEXT);
					}
					break;
			}
				
		}
		
		private function unLockBtnInOut():void
		{
			petSprite.btnInOrOut.mouseEnabled = true;
		}
		
		/** 显示金钱面板 0=存钱，1=取钱 */
		public function showMoneyPanel(type:int):void
		{
			if(type == 0) {
				moneyPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_dep_med_dep_show_1" ]);//"存入金额"
				moneyPanel.content.txtJin.text = "";
				moneyPanel.content.txtYin.text = "";
				moneyPanel.content.txtTong.text = "";
			} else {
				moneyPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_dep_med_dep_show_2" ]);//"取出金额"
				var moneyArr:Array = UIUtils.getMoney(DepotConstData.moneyDepot);
				moneyArr[0] == 0 ? moneyPanel.content.txtJin.text = "" : moneyPanel.content.txtJin.text = moneyArr[0].toString();
				moneyArr[1] == 0 ? moneyPanel.content.txtYin.text = "" : moneyPanel.content.txtYin.text = moneyArr[1].toString();
				moneyArr[2] == 0 ? moneyPanel.content.txtTong.text = "" : moneyPanel.content.txtTong.text = moneyArr[2].toString();
			}
			if(GameCommonData.GameInstance.GameUI.contains(moneyPanel)) GameCommonData.GameInstance.GameUI.removeChild(moneyPanel);
			moneyPanel.x = DepotConstData.MONEY_DEFAULT_POS.x;
			moneyPanel.y = DepotConstData.MONEY_DEFAULT_POS.y;
			GameCommonData.GameInstance.GameUI.addChild(moneyPanel);
		}
		
		/** 加锁解锁 物品/宠物按钮 false=加锁 ，true=解锁 */
		private function lockPageBtn(val:Boolean):void
		{
			depot.mcPage_0.mouseEnabled = val;
			depot.mcPage_1.mouseEnabled = val;
		}
		
		private function formatMoney(jin:uint, yin:uint, tong:uint):uint
		{
			return (10000 * jin + 100 * yin + tong);	//1金=100银，1银=100铜
		}
		
		private function gcAll():void
		{
			removeLis();
			itemGridManager.removeAllItem();
			gcMoneyPanel();
//			SoundManager.PlaySound(SoundList.PANECLOSE);
			GameCommonData.GameInstance.GameUI.removeChild(depotPanel);
			depotUIManager.moneyInput = null;
			depotUIManager.itemDepot  = null;
			depotUIManager.petDepot   = null;
			depotUIManager = null;
			depotPanel = null;
			moneyPanel = null;
			if(DepotConstData.SelectIndex == 1) {
				dataProxy.PetCanOperate = true;
			}
			DepotConstData.petSelected = null;
			DepotConstData.SelectIndex 	 = 0;
			DepotConstData.curDepotIndex = 0;
			DepotConstData.curMoneyType	 = 0;
			DepotConstData.curPetType	 = 0;
			DepotConstData.SelectedItem = null;
			DepotConstData.TmpIndex = 0;
			DepotConstData.GridUnitList = new Array();
			DepotConstData.goodList = new Array(36);
			DepotConstData.petListDepot = new Dictionary();
			if(DepotConstData.itemExtIsOpen) sendNotification(DepotEvent.REMOVEITEMEXT);
			if(DepotConstData.petExtIsOpen) sendNotification(DepotEvent.REMOVEPETEXT);
			dataProxy.DepotIsOpen = false;
//			GameCommonData.Player.Role.State = GameRole.STATE_NULL;
			dataProxy = null;
			yellowFilter = null;
			viewComponent = null;
			facade.removeMediator(DepotMediator.NAME);
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLOSE_DEPOT_NOTICE_NEWER_HELP);
		}
		private function depotCloseHandler(e:Event):void
		{
			gcAll();
		}
		private function moneyCloseHandler(e:Event):void
		{
			gcMoneyPanel();
		}
		private function gcMoneyPanel():void
		{
			moneyPanel.content.txtJin.text = "";
			moneyPanel.content.txtYin.text = "";
			moneyPanel.content.txtTong.text = "";
			DepotConstData.curMoneyType = 0;
			if(moneyPanel && GameCommonData.GameInstance.GameUI.contains(moneyPanel)) GameCommonData.GameInstance.GameUI.removeChild(moneyPanel);
		}
		
	}
}




