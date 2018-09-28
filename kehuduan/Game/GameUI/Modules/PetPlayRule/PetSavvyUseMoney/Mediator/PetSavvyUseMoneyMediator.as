package GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyConstData;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyEvent;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Proxy.PetSavvyUseMoneyGridManager;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.UI.PetSavvyUseMoneyUIManager;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class PetSavvyUseMoneyMediator extends Mediator
	{
		public static const NAME:String = "petSavvyUseMoneyMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var gridManager:PetSavvyUseMoneyGridManager;
		private var uiManager:PetSavvyUseMoneyUIManager;
		
		//宠物选择
		private var petChoiceView:MovieClip = null;
		private var petChoicePanelBase:PanelBase = null;
		private const PETCHOICEPANEL_POS:Point = new Point(450, 150);
		
		private var petChoiceOpen:Boolean = false;
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		//
		
		public function PetSavvyUseMoneyMediator()
		{
			super(NAME);
		}
		
		private function get petSavvyUseMoneyView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				PetSavvyUseMoneyEvent.SHOW_PETSAVVYUSEMONEY_VIEW,
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
//				EventList.CLOSE_NPC_ALL_PANEL,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
//				PetSavvyUseMoneyEvent.BAG_TO_SAVVY_USEMONEY,
//				EventList.UPDATEMONEY,
//				PetEvent.PET_UPDATE_SHOW_INFO
				PetEvent.RETURN_TO_SHOW_PET_INFO,
				PetSavvyUseMoneyEvent.PET_SAVVY_USE_MONEY_RETURN
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:	//PetSavvyUseMoneyEvent.SHOW_PETSAVVYUSEMONEY_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetBreedDoubleIsOpen) {
						initView();
						addLis();
						dataProxy.PetSavvyUseMoneyIsOpen = true;
					}
					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:	//EventList.CLOSE_PET_PLAYRULE_VIEW:
					closeHandler(null);
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetSavvyUseMoneyIsOpen) closeHandler(null);	
//					break;
//				case PetSavvyUseMoneyEvent.BAG_TO_SAVVY_USEMONEY:
//					var itemUseMoney:Object = notification.getBody();
//					addUseMoneyItem(itemUseMoney);
//					break;
//				case PetEvent.PET_UPDATE_SHOW_INFO:			//更新宠物快照
////					updateCurPet();
//					break;	
				case PetRuleEvent.SELECT_PET_PET_RULE:
					petToShow(notification.getBody() as uint);
					break;
				case PetEvent.RETURN_TO_SHOW_PET_INFO:
					var info:GamePetRole = notification.getBody() as GamePetRole;
					if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == info.Id && dataProxy.PetQuerySkill) {
						dataProxy.PetQuerySkill = false;
						PetRuleCommonData.selectedPet = info;
						petToShow(info.Id);
//						uiManager.showData(info);
					}
					break;
				case PetSavvyUseMoneyEvent.PET_SAVVY_USE_MONEY_RETURN:
					var petIdSavvy:uint = notification.getBody() as uint;
					if(petIdSavvy > 0) updateCurPet(petIdSavvy);
					break;
				
//				case EventList.UPDATEMONEY:
//					uiManager.showMoney(1);
//					uiManager.showMoney(2);
//					break;
			}
		}
		
		private function initView():void 
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetSavvyUseMoneyView");
//			petChoiceView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetList");
//			
//			panelBase = new PanelBase(petSavvyUseMoneyView, petSavvyUseMoneyView.width+8, petSavvyUseMoneyView.height+12);
//			panelBase.name = "PetSavvyUseMoney";
//			panelBase.addEventListener(Event.CLOSE, closeHandler);
//			panelBase.x = UIConstData.DefaultPos1.x;
//			panelBase.y = UIConstData.DefaultPos1.y;
//			panelBase.SetTitleTxt("宠物悟性提升");
//			
//			//宠物选择面板
//			petChoicePanelBase = new PanelBase(petChoiceView, petChoiceView.width-6, petChoiceView.height+11);
//			petChoicePanelBase.name = "PetSavvyUseMoneyChoice";
//			petChoicePanelBase.addEventListener(Event.CLOSE, choiceCloseHandler);
//			petChoicePanelBase.x = UIConstData.DefaultPos1.x;
//			petChoicePanelBase.y = UIConstData.DefaultPos1.y;
//			petChoicePanelBase.SetTitleTxt("宠物选择");
//			
//			listView = new ListComponent(false);
//			showFilterList();
//			iScrollPane = new UIScrollPane(listView);
//			iScrollPane.x = 3;
//			iScrollPane.y = 3;
//			iScrollPane.width = 118;
//			iScrollPane.height = 135;
//			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
//			iScrollPane.refresh();
//			petChoiceView.addChild(iScrollPane);
//			petChoiceView.txtCancel.mouseEnabled = false;
			
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petSavvyUseMoneyView);
			
			petCon.subView = petSavvyUseMoneyView;
			
			uiManager = new PetSavvyUseMoneyUIManager(petSavvyUseMoneyView);
			
//			initGrid();
//			GameCommonData.GameInstance.GameUI.addChild(panelBase);
//			openChoicePanel();
		}
		
//		private function initGrid():void
//		{
//			//快速购买
//			for(var j:int = 0; j < 3; j++) {
//				if(!(UIConstData.MarketGoodList[20] as Array)[j]) continue;
//				var good:Object = (UIConstData.MarketGoodList[20] as Array)[j];
//				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//				unit.x = 243;
//				unit.y = j * (70 + unit.height) + 12;
//				unit.name = "goodQuickBuy"+j+"_"+good.type;
//				petSavvyUseMoneyView.addChild(unit);
//				
//				var useItem:UseItem = new UseItem(j, good.type, petSavvyUseMoneyView);
//				if(good.type < 300000) {
//					useItem.Num = 1;
//				}
//				else if(good.type >= 300000) {
//					useItem.Num = UIConstData.getItem(good.type).amount; 
//				}
//				useItem.x = unit.x + 2;
//				useItem.y = unit.y + 2;
//				useItem.Id = UIConstData.getItem(good.type).id;
//				useItem.IsBind = 0;
//				useItem.Type = good.type;
//				useItem.IsLock = false;
//				
//				petSavvyUseMoneyView.addChild(useItem);
//				
//				petSavvyUseMoneyView["txtGoodNamePet_"+j].text = good.Name;
//				petSavvyUseMoneyView["mcMoney_"+j].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
//				ShowMoney.ShowIcon(petSavvyUseMoneyView["mcMoney_"+j], petSavvyUseMoneyView["mcMoney_"+j].txtMoney, true);
//				petSavvyUseMoneyView["txtGoodNamePet_"+j].mouseEnabled = false;
//				petSavvyUseMoneyView["mcMoney_"+j].mouseEnabled = false;
//				petSavvyUseMoneyView["btnBuy_"+j].addEventListener(MouseEvent.CLICK, buyHandler);
//			}
			//
//			var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//			gridUnit.x = 34;
//			gridUnit.y = 181;
//			gridUnit.name = "petSavvyUseMoneyItem";
//			petSavvyUseMoneyView.addChild(gridUnit);	//添加到画面
//			PetSavvyUseMoneyConstData.GridItemUnit = new GridUnit(gridUnit, true);
//			PetSavvyUseMoneyConstData.GridItemUnit.parent = petSavvyUseMoneyView;	//设置父级
//			PetSavvyUseMoneyConstData.GridItemUnit.Index = 0;						//格子的位置
//			PetSavvyUseMoneyConstData.GridItemUnit.HasBag = true;					//是否是可用的背包
//			PetSavvyUseMoneyConstData.GridItemUnit.IsUsed = false;					//是否已经使用
//			PetSavvyUseMoneyConstData.GridItemUnit.Item	= null;						//格子的物品
//			
//			gridManager = new PetSavvyUseMoneyGridManager(petSavvyUseMoneyView);
//			facade.registerProxy(gridManager);
//		}
		
//		private function buyHandler(e:MouseEvent):void
//		{
//			var index:uint = uint(String(e.target.name).split("_")[1]);
//			for(var i:int = 0; i < petSavvyUseMoneyView.numChildren; i++) {
//				if(petSavvyUseMoneyView.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
//					var type:uint = uint(petSavvyUseMoneyView.getChildAt(i).name.split("_")[1]);
//					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
//				}
//			}
//		}
		
//		/** 初始化宠物选择面板数据 */
//		private function showFilterList():void
//		{
//			for(var id:Object in GameCommonData.Player.Role.PetSnapList)
//			{
//				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItem");
//				item.name = "petSavvyUseMoneyChoice_"+id;
//				item.mcSelected.visible = false;
////				item.width = 120;
////				item.btnChosePet.width = 120;
////				item.mcSelected.width = 120;
//				item.mcSelected.mouseEnabled = false;
//				item.txtName.mouseEnabled = false;
//				item.doubleClickEnabled = true;
//				item.btnChosePet.mouseEnabled = false;
//				item.txtName.text = GameCommonData.Player.Role.PetSnapList[id].PetName;
//				item.addEventListener(MouseEvent.CLICK, selectItem);
//				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
//				listView.addChild(item);
//			}
//			listView.width = 115;
//			listView.upDataPos();
//		}
		
//		/** 选择列表中某个宠物 */
//		private function selectItem(event:MouseEvent):void
//		{
//			var item:MovieClip = event.currentTarget as MovieClip;
//			var id:int = int(item.name.split("_")[1]);
//			if(PetSavvyUseMoneyConstData.petSelected && id == PetSavvyUseMoneyConstData.petSelected.Id) return;
//			for(var i:int = 0; i < listView.numChildren; i++){
//				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
//			}
//			item.mcSelected.visible = true;
//			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				PetSavvyUseMoneyConstData.petSelected = GameCommonData.Player.Role.PetSnapList[id];
//			} else {
//				gcAll();
//			}
//		}
		
//		/** 双击查看宠物属性 */
//		private function lookPetInfoHandler(e:MouseEvent):void
//		{
//			var item:MovieClip = e.currentTarget as MovieClip;
//			var id:uint = uint(item.name.split("_")[1]);
//			if(id > 0) {
//				dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
//				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
//			}
//		}
		
//		/** 显示当前选择的宠物的数据 */
//		private function showPetData():void
//		{
//			
//		}
//		
//		/** 打开宠物选择界面 */
//		private function openChoicePanel():void
//		{
//			if(!petChoiceOpen && !GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase)) {
//				GameCommonData.GameInstance.GameUI.addChild(petChoicePanelBase);
//				petChoicePanelBase.x = PETCHOICEPANEL_POS.x;
//				petChoicePanelBase.y = PETCHOICEPANEL_POS.y;
//				petChoiceOpen = true;
//			}
//		}
//		
//		/** 关闭面板宠物选择 */
//		private function choiceCloseHandler(e:Event):void
//		{
//			if(petChoiceOpen && GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase)) {
//				GameCommonData.GameInstance.GameUI.removeChild(petChoicePanelBase);
//				petChoiceOpen = false;
//			}
//		}
		
		private function addLis():void
		{
//			petSavvyUseMoneyView.btnPanelCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSavvyUseMoneyView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSavvyUseMoneyView.btnLook.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSavvyUseMoneyView.mcPhotoPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSavvyUseMoneyView.btnSelectPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
			
//			petChoiceView.btnPetChose.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petChoiceView.btnCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private  function removeLis():void
		{
//			petSavvyUseMoneyView.btnPanelCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSavvyUseMoneyView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSavvyUseMoneyView.btnLook.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSavvyUseMoneyView.mcPhotoPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSavvyUseMoneyView.btnSelectPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			
//			petChoiceView.btnPetChose.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petChoiceView.btnCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function closeHandler(e:Event):void
		{
			gcAll();	
		}
		
		private  function gcAll():void
		{
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
//			choiceCloseHandler(null);
//			dataProxy.PetCanOperate = true;
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				if(iScrollPane && petSavvyUseMoneyView.contains(iScrollPane)) {
					iScrollPane.removeChild(listView);
					petSavvyUseMoneyView.removeChild(iScrollPane);
				}
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			if(PetSavvyUseMoneyConstData.itemData) {
				sendNotification(EventList.BAGITEMUNLOCK, PetSavvyUseMoneyConstData.itemData.id);
			}
			uiManager = null;
			panelBase = null;
			petChoiceView = null;
			petChoicePanelBase = null;
			listView = null;
			iScrollPane = null;
			
			PetSavvyUseMoneyConstData.breedCost = 0;
			PetSavvyUseMoneyConstData.petSelected = null;
			PetSavvyUseMoneyConstData.petShow = null;
			PetSavvyUseMoneyConstData.itemTypeNeed = "";
			PetSavvyUseMoneyConstData.GridItemUnit = null;
			PetSavvyUseMoneyConstData.itemData = null;
			
			dataProxy.PetSavvyUseMoneyIsOpen = false;
			dataProxy = null;
//			facade.removeProxy(PetSavvyUseMoneyGridManager.NAME);
			facade.removeMediator(PetSavvyUseMoneyMediator.NAME);
		}
		
		/** 点击按钮 选择宠物、确定、取消 */
		private  function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":								//确定提升悟性
					sureToUseMoney();
					break;
				case "btnLook":						//取消提升悟性
					lookPetInfo();
					break;
				case "mcPhotoPet":
					removePetSelect();
					break;
			}
		}
		
		private function removePetSelect():void
		{
			if(PetRuleCommonData.selectedPet) {
				PetRuleCommonData.selectedPet = null;
				uiManager.clearData();
				sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
				PetSavvyUseMoneyConstData.breedCost = 0;
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyUseMoneyConstData.breedCost);
			}
		}
		
		/** 查看宠物属性 */
		private function lookPetInfo():void
		{
			if(!PetRuleCommonData.selectedPet) {
				return;
			}
			PetPropConstData.isSearchOtherPetInto = true;
			var id:int = PetRuleCommonData.selectedPet.Id;
			dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
		}
		
		/** 悟性提升回馈 */
		private function updateCurPet(petId:uint):void
		{
			if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petId) {
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[petId];
				PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[petId];
				
				dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
				dataProxy.PetQuerySkill = true;
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:petId, ownerId:GameCommonData.Player.Role.Id});
				
//				petSavvyUseMoneyView.txtPetName.text = PetSavvyUseMoneyConstData.petShow.PetName;
//				if(PetRuleCommonData.selectedPet.Savvy >= 6) {
//					PetSavvyUseMoneyConstData.breedCost = 300;
//					PetSavvyUseMoneyConstData.itemTypeNeed = "630013_高级";	//高级悟性丹
//				} else if(PetRuleCommonData.selectedPet.Savvy >= 3) {
//					PetSavvyUseMoneyConstData.breedCost = 200;
//					PetSavvyUseMoneyConstData.itemTypeNeed = "630012_中级";	//中级悟性丹
//				} else {
//					PetSavvyUseMoneyConstData.breedCost = 100; 
//					PetSavvyUseMoneyConstData.itemTypeNeed = "630011_低级";	//低级悟性丹    630011
//				}
////				uiManager.showMoney(0);
//				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyUseMoneyConstData.breedCost);
//				if(PetRuleCommonData.selectedPet.Savvy == 10) {
//					petSavvyUseMoneyView.txtSucPercent.text = "";
////					petSavvyUseMoneyView.txtLostPercent.text = "失败加成："; 
//					petSavvyUseMoneyView.txtLoseTo.text = "";
//					
//				} else {
//					var loseTo:String = "";
////					if(PetSavvyUseMoneyConstData.petShow.Savvy >= 9) {
////						loseTo = "7";
////					} else 
//					if(PetRuleCommonData.selectedPet.Savvy >7) {
//						loseTo = "<font color='#00ff00'>失败降为</font><font color='#ff0000'>7</font>";
//					} else if(PetRuleCommonData.selectedPet.Savvy == 7) {
//						loseTo = "失败不降";
//					} else if(PetRuleCommonData.selectedPet.Savvy > 4) {
//						loseTo = "<font color='#00ff00'>失败降为</font><font color='#ff0000'>4</font>";
//					} else {
//						loseTo = "失败不降";
//					}
//					
//					petSavvyUseMoneyView.txtSucPercent.htmlText = PetSavvyUseMoneyConstData.successList[PetRuleCommonData.selectedPet.Savvy];
//					petSavvyUseMoneyView.txtLoseTo.text = loseTo;
//					petSavvyUseMoneyView.txtLoseTo.x = 138;
//					petSavvyUseMoneyView.txtLoseTo.y = 76;
//					if(PetSavvyUseMoneyConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP]) {
//						petSavvyUseMoneyView.txtVipAdd.text = "VIP加成 +" + PetSavvyUseMoneyConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP];
//					} else {
//						petSavvyUseMoneyView.txtVipAdd.text = "";
//						petSavvyUseMoneyView.txtLoseTo.y = 81;
//					}
//					uiManager.showData(GameCommonData.Player.Role.PetSnapList[petId]);
////					petSavvyUseMoneyView.txtSucPercent.htmlText = (PetSavvyUseMoneyConstData.successList[PetSavvyUseMoneyConstData.petShow.Savvy] == "100%") ? "成功率：100%" : "成功率："+PetSavvyUseMoneyConstData.successList[PetSavvyUseMoneyConstData.petShow.Savvy]+"<font color='"+GameCommonData.Player.Role.VIPColor+"'>+"+PetSavvyUseMoneyConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP]+"</font>";
//////					petSavvyUseMoneyView.txtLostPercent.text = "失败加成："+(PetSavvyUseMoneyConstData.petShow.LoseTime * 2 + "%");
////					petSavvyUseMoneyView.txtLoseTo.text = loseTo;
//				}
////				petSavvyUseMoneyView.txtCurSavvy.text = PetSavvyUseMoneyConstData.petShow.Savvy.toString();
////				gridManager.removeItem();	//移除道具，须重新拖进悟性丹
			}
			petSavvyUseMoneyView.btnSure.mouseEnabled = true;
		}
		
		/** 确定提升悟性 */
		private function sureToUseMoney():void
		{
			if(!PetRuleCommonData.selectedPet) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});  // 请先选择宠物
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_2" ], color:0xffff00});  // 出战状态的宠物不能提升悟性
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].Savvy == 10) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_1" ], color:0xffff00});  // 该宠物悟性已达到上限
				return;
			}
			if(PetSavvyUseMoneyConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});   // 没有足够的银两
				return;
			}
//			if(!PetSavvyUseMoneyConstData.itemData) {
//				var itemLevel:String = PetSavvyUseMoneyConstData.itemTypeNeed.split("_")[1];
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入"+itemLevel+"悟性丹", color:0xffff00});
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入悟性丹", color:0xffff00});
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入"+PetSavvyUseMoneyConstData.itemTypeNeed.split("_")[1]+"悟性丹", color:0xffff00});
//				return;
//			}
			if(!checkItem()) return;
			if(!GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id]) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			} else {
				if(PetRuleCommonData.selectedPet.Savvy > 7) {	//8升9、9升10时提示
					// 该宠物当前悟性为  ，要继续提升吗？  警 告  是  否
					facade.sendNotification(EventList.SHOWALERT, {comfrim:comitToUp, cancel:cancelToUp, info:GameCommonData.wordDic[ "mod_pet_psu_med_pets_sureToUseMoney_1" ]+"<font color='#FFFF00'>"+PetRuleCommonData.selectedPet.Savvy+"</font>，"+ GameCommonData.wordDic[ "mod_pet_psu_med_pets_sur_2" ], title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_yes" ], cancelTxt:GameCommonData.wordDic[ "often_used_no" ] });
				} else {
					PetNetAction.petBreed(PlayerAction.PET_SAVVY_USEMONEY, PetRuleCommonData.selectedPet.Id, 0,0);//PetNetAction.petBreed(PlayerAction.PET_SAVVY_USEMONEY, PetSavvyUseMoneyConstData.petShow.Id, 0,0, PetSavvyUseMoneyConstData.itemData.id);
					petSavvyUseMoneyView.btnSure.mouseEnabled = false;
				}
				//发送提升悟性命令 带宠物ID
//				gridManager.removeItem();
//				setTimeout(canOp, 1000);	
			}
//			gcAll();
		}
		
		private function comitToUp():void
		{
			PetNetAction.petBreed(PlayerAction.PET_SAVVY_USEMONEY, PetRuleCommonData.selectedPet.Id, 0,0);//PetNetAction.petBreed(PlayerAction.PET_SAVVY_USEMONEY, PetSavvyUseMoneyConstData.petShow.Id, 0,0, PetSavvyUseMoneyConstData.itemData.id);
			petSavvyUseMoneyView.btnSure.mouseEnabled = false;
		}
		private function cancelToUp():void
		{
			
		}
		
		private function canOp():void
		{
			if(petSavvyUseMoneyView) petSavvyUseMoneyView.btnSure.mouseEnabled = true;
		}
		
		/** 选择宠物进显示栏 */
		private function petToShow(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				if(!GameCommonData.Player.Role.PetSnapList[PetSavvyUseMoneyConstData.petSelected.Id]) {
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该宠物不存在", color:0xffff00});
//					gcAll();
//					return;
//				}
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
				uiManager.clearData();
				if(pet.State == 4)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
					return;
				}
				 if(pet.Savvy == 10) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_1" ], color:0xffff00});  // 该宠物悟性已达到上限
					return;
				} 
				if(pet.State == 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_2" ], color:0xffff00});  // 出战状态的宠物不能提升悟性
					return;
				}
				PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
				
				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
				/////////////
				if(PetRuleCommonData.selectedPet.LifeMax > 0) {	//有详细信息
					uiManager.showData(PetRuleCommonData.selectedPet);
					//---
					if(PetRuleCommonData.selectedPet.Savvy >= 6) {
						PetSavvyUseMoneyConstData.breedCost = 300;
						PetSavvyUseMoneyConstData.itemTypeNeed = "630013_"+GameCommonData.wordDic[ "often_used_highlevel" ];	//高级  
					} else if(PetRuleCommonData.selectedPet.Savvy >= 3) {
						PetSavvyUseMoneyConstData.breedCost = 200;
						PetSavvyUseMoneyConstData.itemTypeNeed = "630012_"+GameCommonData.wordDic[ "often_used_midlevel" ];	//中级
					} else {
						PetSavvyUseMoneyConstData.breedCost = 100;
						PetSavvyUseMoneyConstData.itemTypeNeed = "630011_"+GameCommonData.wordDic[ "often_used_lowerlevel" ];	//低级    630011
					}
					sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyUseMoneyConstData.breedCost);
					var loseTo:String = "";
					if(PetRuleCommonData.selectedPet.Savvy >7) {
						loseTo = "<font color='#00ff00'>" + GameCommonData.wordDic[ "often_used_losedownto" ] + "</font><font color='#ff0000'>7</font>";   // 失败降为
					} else if(PetRuleCommonData.selectedPet.Savvy == 7) {
						loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];  // 失败不降
					} else if(PetRuleCommonData.selectedPet.Savvy > 4) {
						loseTo = "<font color='#00ff00'>" + GameCommonData.wordDic[ "often_used_losedownto" ] + "</font><font color='#ff0000'>4</font>";  // 失败降为
					} else {
						loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];  // 失败不降
					}
					petSavvyUseMoneyView.txtSucPercent.htmlText = PetSavvyUseMoneyConstData.successList[PetRuleCommonData.selectedPet.Savvy];
					//有悟性保护丹的时候（9升10），失败不降
					if(pet.Savvy == 9)
					{
						if(BagData.isHasItem(612002))
						{
							loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];  // 失败不降
						}
					}
					petSavvyUseMoneyView.txtLoseTo.htmlText = loseTo;
//					petSavvyUseMoneyView.txtLoseTo.x = 138;
//					petSavvyUseMoneyView.txtLoseTo.y = 76;
					if(PetSavvyUseMoneyConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP]) {
						// VIP加成 +   
						petSavvyUseMoneyView.txtVipAdd.text = GameCommonData.wordDic[ "mod_pet_psu_med_pets_pet_1" ]+" +" + PetSavvyUseMoneyConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP];
					} else {
						petSavvyUseMoneyView.txtVipAdd.text = GameCommonData.wordDic[ "mod_pet_psu_med_pets_pet_2" ];   // 非VIP +0%
//						petSavvyUseMoneyView.txtLoseTo.y = 81;
					}
					uiManager.showData(PetRuleCommonData.selectedPet);
					sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyUseMoneyConstData.breedCost);
					//---
				} else {										//没有，去服务器查询
					dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
					dataProxy.PetQuerySkill = true;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
				}
				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
				/////////
//				petSavvyUseMoneyView.txtPetName.text = PetSavvyUseMoneyConstData.petShow.PetName;
//				if(PetSavvyUseMoneyConstData.petShow.Savvy >= 9) {
//					loseTo = "不降";
//				} else 
//				? "成功率：100%" : "成功率："+PetSavvyUseMoneyConstData.successList[PetSavvyUseMoneyConstData.petShow.Savvy]+"<font color='"+GameCommonData.Player.Role.VIPColor+"'>+"+PetSavvyUseMoneyConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP]+"</font>";
//					petSavvyUseMoneyView.txtCurSavvy.text = PetSavvyUseMoneyConstData.petShow.Savvy.toString();
//				uiManager.showMoney(0);
//				petSavvyUseMoneyView.txtSucPercent.text = "成功率："+PetSavvyUseMoneyConstData.successList[PetSavvyUseMoneyConstData.petShow.Savvy];
//				petSavvyUseMoneyView.txtLostPercent.text = "失败加成："+(PetSavvyUseMoneyConstData.petShow.LoseTime * 2 + "%");
//				gridManager.removeItem();	//移除道具，须重新拖进悟性丹
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			}
		}
		
		//GameCommonData.Player.Role.State  GameCommonData.Player.Role.State = GameRole.STATE_STALL
		
//		/** 拖进悟性丹 */
//		private function addUseMoneyItem(o:Object):void
//		{
//			if(!o) return;
//			if(!PetSavvyUseMoneyConstData.petShow) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请先选择宠物", color:0xffff00});
//				sendNotification(EventList.BAGITEMUNLOCK, o.id);
//				return;
//			}
//			if(o.type != 630011 && o.type != 630012 && o.type != 630013) {
//				sendNotification(EventList.BAGITEMUNLOCK, o.id);
//				return;
//			}
//			var needType:uint = uint(PetSavvyUseMoneyConstData.itemTypeNeed.split("_")[0]);
//			if(o.type < needType) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入"+PetSavvyUseMoneyConstData.itemTypeNeed.split("_")[1]+"悟性丹", color:0xffff00});
//				sendNotification(EventList.BAGITEMUNLOCK, o.id);
//				return;
//			}
//			if(PetSavvyUseMoneyConstData.itemData) {
//				sendNotification(EventList.BAGITEMUNLOCK, o.id);
//				return;
//			}
//			PetSavvyUseMoneyConstData.itemData = o;
//			gridManager.addItem();
//		}
		
		private function checkItem():Boolean
		{
			var result:Boolean = true;
			var type:uint = uint(PetSavvyUseMoneyConstData.itemTypeNeed.split("_")[0]);
			if(!BagData.isHasItem(type)) {
				var itemLev:String = PetSavvyUseMoneyConstData.itemTypeNeed.split("_")[1];
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:itemLev+GameCommonData.wordDic[ "mod_pet_psu_med_pets_che_1" ], color:0xffff00});   // 悟性丹不足，请补充
				result = false;
			}
			return result;
		}
		
		
	}
}