package GameUI.Modules.PetPlayRule.PetSavvyJoinView.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.Data.PetSavvyJoinConstData;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.UI.PetSavvyJoinUIManager;
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

	public class PetSavvyJoinMediator extends Mediator
	{	
		public static const NAME:String = "petSavvyJoinMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		
		private var uiManager:PetSavvyJoinUIManager;
		
		//宠物选择
		//--------------------------------------------
		private var petChoiceView_0:MovieClip = null;
		private var petChoicePanelBase_0:PanelBase = null;
		private const PETCHOICEPANEL_POS_0:Point = new Point(361, 114);
		
		private var petChoiceOpen_0:Boolean = false;
		private var listView_0:ListComponent;
		private var iScrollPane_0:UIScrollPane;
		//--------------------------------------------
		private var petChoiceView_1:MovieClip = null;
		private var petChoicePanelBase_1:PanelBase = null;
		private const PETCHOICEPANEL_POS_1:Point = new Point(511, 114);
		
		private var petChoiceOpen_1:Boolean = false;
		private var listView_1:ListComponent;
		private var iScrollPane_1:UIScrollPane;
		//--------------------------------------------
		
		public function PetSavvyJoinMediator()
		{
			super(NAME);
		}
		
		private function get petSavvyJoinView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				PetSavvyJoinEvent.SHOW_PETSAVVYJOIN_VIEW,
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
//				EventList.CLOSE_NPC_ALL_PANEL,
//				EventList.UPDATEMONEY,
				PetEvent.PET_DELETE_SUCCESS
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:	//PetSavvyJoinEvent.SHOW_PETSAVVYJOIN_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetSavvyJoinIsOpen) {
						initView();
						addLis();
						dataProxy.PetSavvyJoinIsOpen = true;
					}
					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:	//EventList.CLOSE_PET_PLAYRULE_VIEW:
					closeHandler(null);
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetSavvyJoinIsOpen) closeHandler(null);	
//					break;
				case PetEvent.PET_DELETE_SUCCESS:			//反馈
					updateCurPet();
					break;
				case PetRuleEvent.SELECT_PET_PET_RULE:	
					selectPet(notification.getBody() as uint);
					break;
//				case EventList.UPDATEMONEY:
//					uiManager.showMoney(1);
//					uiManager.showMoney(2);
//					break;
			}
		}
		
		private function initView():void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetSavvyJoinView");
			
//			panelBase = new PanelBase(petSavvyJoinView, petSavvyJoinView.width-16, petSavvyJoinView.height+11);
//			panelBase.name = "PetSavvyJoin";
//			panelBase.addEventListener(Event.CLOSE, closeHandler);
//			panelBase.x = UIConstData.DefaultPos1.x;
//			panelBase.y = UIConstData.DefaultPos1.y;
//			panelBase.SetTitleTxt("宠物合成");
//			
//			petChoiceView_0 = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetList");
//			petChoicePanelBase_0 = new PanelBase(petChoiceView_0, petChoiceView_0.width-6, petChoiceView_0.height+11);
//			petChoicePanelBase_0.name = "PetSavvyJoinChoice0";
//			petChoicePanelBase_0.addEventListener(Event.CLOSE, choiceCloseHandler_0);
//			petChoicePanelBase_0.x = PETCHOICEPANEL_POS_0.x;
//			petChoicePanelBase_0.y = PETCHOICEPANEL_POS_0.y;
//			petChoicePanelBase_0.SetTitleTxt("主宠物选择");
//			petChoicePanelBase_0.disableClose();
//			petChoiceView_0.txtCancel.mouseEnabled = false;
//			
//			petChoiceView_1 = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetList");
//			petChoicePanelBase_1 = new PanelBase(petChoiceView_1, petChoiceView_1.width-6, petChoiceView_1.height+11);
//			petChoicePanelBase_1.name = "PetSavvyJoinChoice1";
//			petChoicePanelBase_1.addEventListener(Event.CLOSE, choiceCloseHandler_1);
//			petChoicePanelBase_1.x = PETCHOICEPANEL_POS_1.x;
//			petChoicePanelBase_1.y = PETCHOICEPANEL_POS_1.y;
//			petChoicePanelBase_1.SetTitleTxt("副宠物选择");
//			petChoicePanelBase_1.disableClose();
//			petChoiceView_1.txtCancel.mouseEnabled = false;
			
			petSavvyJoinView.txtSucPercent.text = "";
			petSavvyJoinView.txtLoseTo.text = "";
			
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petSavvyJoinView);
			
			petCon.subView = petSavvyJoinView;
			
			uiManager = new PetSavvyJoinUIManager(petSavvyJoinView);
			
			
//			initChoice_0();
//			initChoice_1();
//			
//			GameCommonData.GameInstance.GameUI.addChild(panelBase);
//			openChoicePanel_0();
//			openChoicePanel_1();
		}
		
//		private function initChoice_0():void
//		{
//			if(iScrollPane_0 && petChoiceView_0.contains(iScrollPane_0)) {
//				petChoiceView_0.removeChild(iScrollPane_0);
//				iScrollPane_0 = null;
//				listView_0 = null;
//			}
//			listView_0 = new ListComponent(false);
//			showFilterList_0();
//			iScrollPane_0 = new UIScrollPane(listView_0);
//			iScrollPane_0.x = 3;
//			iScrollPane_0.y = 3;
//			iScrollPane_0.width = 118;
//			iScrollPane_0.height = 135;
//			iScrollPane_0.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
//			iScrollPane_0.refresh();
//			petChoiceView_0.addChild(iScrollPane_0);
//		}
//		
//		private function initChoice_1():void
//		{
//			if(iScrollPane_1 && petChoiceView_1.contains(iScrollPane_1)) {
//				petChoiceView_1.removeChild(iScrollPane_1);
//				iScrollPane_1 = null;
//				listView_1 = null;
//			}
//			listView_1 = new ListComponent(false);
//			showFilterList_1();
//			iScrollPane_1 = new UIScrollPane(listView_1);
//			iScrollPane_1.x = 3;
//			iScrollPane_1.y = 3;
//			iScrollPane_1.width = 118;
//			iScrollPane_1.height = 135;
//			iScrollPane_1.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
//			iScrollPane_1.refresh();
//			petChoiceView_1.addChild(iScrollPane_1);
//		}
//		
//		//-----------------------------------------------------------
//		private function showFilterList_0():void
//		{
//			for(var id:Object in GameCommonData.Player.Role.PetSnapList)
//			{
//				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItem");
//				item.name = "petSavvyJoinChoice0_"+id;
//				item.mcSelected.visible = false;
////				item.width = 120;
////				item.btnChosePet.width = 120;
////				item.mcSelected.width = 120;
//				item.mcSelected.mouseEnabled = false;
//				item.doubleClickEnabled = true;
//				item.txtName.mouseEnabled = false;
//				item.btnChosePet.mouseEnabled = false;
//				item.txtName.text = GameCommonData.Player.Role.PetSnapList[id].PetName;
//				item.addEventListener(MouseEvent.CLICK, selectItem_0);
//				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
//				listView_0.addChild(item);
//			}
//			listView_0.width = 115;
//			listView_0.upDataPos();
//		}
		
//		/** 选择列表中某个宠物 */
//		private function selectItem_0(event:MouseEvent):void
//		{
//			var item:MovieClip = event.currentTarget as MovieClip;
//			var id:int = int(item.name.split("_")[1]);
//			if(PetSavvyJoinConstData.petSelected_0 && id == PetSavvyJoinConstData.petSelected_0.Id) {
//				return;
//			}
//			for(var i:int = 0; i < listView_0.numChildren; i++){
//				(listView_0.getChildAt(i) as MovieClip).mcSelected.visible = false;
//			}
//			item.mcSelected.visible = true;
//			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				PetSavvyJoinConstData.petSelected_0 = GameCommonData.Player.Role.PetSnapList[id];
//			} else {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该宠物不存在", color:0xffff00});
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
//		//-----------------------------------------------------------
//		
//		private function showFilterList_1():void
//		{
//			for(var id:Object in GameCommonData.Player.Role.PetSnapList)
//			{
//				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItem");
//				item.name = "petSavvyJoinChoice1_"+id;
//				item.mcSelected.visible = false;
////				item.width = 120;
////				item.btnChosePet.width = 120;
////				item.mcSelected.width = 120;
//				item.mcSelected.mouseEnabled = false;
//				item.doubleClickEnabled = true;
//				item.txtName.mouseEnabled = false;
//				item.btnChosePet.mouseEnabled = false;
//				item.txtName.text = GameCommonData.Player.Role.PetSnapList[id].PetName;
//				item.addEventListener(MouseEvent.CLICK, selectItem_1);
//				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
//				listView_1.addChild(item);
//			}
//			listView_1.width = 115;
//			listView_1.upDataPos();
//		}
//		
//		/** 选择列表中某个宠物 */
//		private function selectItem_1(event:MouseEvent):void
//		{
//			var item:MovieClip = event.currentTarget as MovieClip;
//			var id:int = int(item.name.split("_")[1]);
//			if(PetSavvyJoinConstData.petSelected_1 && id == PetSavvyJoinConstData.petSelected_1.Id) return;
//			for(var i:int = 0; i < listView_1.numChildren; i++) {
//				(listView_1.getChildAt(i) as MovieClip).mcSelected.visible = false;
//			}
//			item.mcSelected.visible = true;
//			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				PetSavvyJoinConstData.petSelected_1 = GameCommonData.Player.Role.PetSnapList[id];
//			} else {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该宠物不存在", color:0xffff00});
//				gcAll();
//			}
//		}
//		//----------------------------------------------------------------------
//		
//		private function openChoicePanel_0():void
//		{
//			if(!petChoiceOpen_0 && !GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase_0)) {
//				GameCommonData.GameInstance.GameUI.addChild(petChoicePanelBase_0);
//				petChoicePanelBase_0.x = PETCHOICEPANEL_POS_0.x;
//				petChoicePanelBase_0.y = PETCHOICEPANEL_POS_0.y;
//				petChoiceOpen_0 = true;
//			}
//		}
//		
//		private function openChoicePanel_1():void
//		{
//			if(!petChoiceOpen_1 && !GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase_1)) {
//				GameCommonData.GameInstance.GameUI.addChild(petChoicePanelBase_1);
//				petChoicePanelBase_1.x = PETCHOICEPANEL_POS_1.x;
//				petChoicePanelBase_1.y = PETCHOICEPANEL_POS_1.y;
//				petChoiceOpen_1 = true;
//			}
//		}
		
//		private function choiceCloseHandler_0(e:Event):void
//		{
//			if(petChoiceOpen_0 && GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase_0)) {
//				GameCommonData.GameInstance.GameUI.removeChild(petChoicePanelBase_0);
//				petChoiceOpen_0 = false;
//			}
//		}
//		private function choiceCloseHandler_1(e:Event):void
//		{
//			if(petChoiceOpen_1 && GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase_1)) {
//				GameCommonData.GameInstance.GameUI.removeChild(petChoicePanelBase_1);
//				petChoiceOpen_1 = false;
//			}
//		}
		
		private function addLis():void
		{
//			petSavvyJoinView.btnSelectPet_0.addEventListener(MouseEvent.CLICK, btnHandler);
//			petSavvyJoinView.btnSelectPet_1.addEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.btnSure.addEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.btnLookMain.addEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.btnLookSub.addEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.mcPhotoMale.addEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.mcPhotoFemale.addEventListener(MouseEvent.CLICK, btnHandler);
//			petSavvyJoinView.btnPanelCancel.addEventListener(MouseEvent.CLICK, btnHandler);
//			
//			petChoiceView_0.btnPetChose.addEventListener(MouseEvent.CLICK, btnHandler);
//			petChoiceView_0.btnCancel.addEventListener(MouseEvent.CLICK, btnHandler);
//			petChoiceView_1.btnPetChose.addEventListener(MouseEvent.CLICK, btnHandler_1);
//			petChoiceView_1.btnCancel.addEventListener(MouseEvent.CLICK, btnHandler_1);
		}
		
		private  function removeLis():void
		{
//			petSavvyJoinView.btnSelectPet_0.removeEventListener(MouseEvent.CLICK, btnHandler);
//			petSavvyJoinView.btnSelectPet_1.removeEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.btnSure.removeEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.btnLookMain.removeEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.btnLookSub.removeEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.mcPhotoMale.removeEventListener(MouseEvent.CLICK, btnHandler);
			petSavvyJoinView.mcPhotoFemale.removeEventListener(MouseEvent.CLICK, btnHandler);
//			petSavvyJoinView.btnPanelCancel.removeEventListener(MouseEvent.CLICK, btnHandler);
//			
//			petChoiceView_0.btnPetChose.removeEventListener(MouseEvent.CLICK, btnHandler);
//			petChoiceView_0.btnCancel.removeEventListener(MouseEvent.CLICK, btnHandler);
//			petChoiceView_1.btnPetChose.removeEventListener(MouseEvent.CLICK, btnHandler_1);
//			petChoiceView_1.btnCancel.removeEventListener(MouseEvent.CLICK, btnHandler_1);
		}
		
		private function closeHandler(e:Event):void
		{
			gcAll();	
		}
		
		private  function gcAll():void
		{
			removeLis();
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
//			choiceCloseHandler_0(null);
//			choiceCloseHandler_1(null);
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
//			dataProxy.PetCanOperate = true;
			
			viewComponent = null;
			panelBase = null;
			
			PetSavvyJoinConstData.breedCost = 0;
			PetSavvyJoinConstData.petSelected_0 = null;
			PetSavvyJoinConstData.petSelected_1 = null;
			PetSavvyJoinConstData.petShow_0 = null;
			PetSavvyJoinConstData.petShow_1 = null;
			
			uiManager = null;
			
			petChoiceView_0 = null;
			petChoicePanelBase_0 = null;
			petChoiceOpen_0 = false;
			listView_0 = null;;
			iScrollPane_0 = null;;
			petChoiceView_1 = null;
			petChoicePanelBase_1 = null;
			petChoiceOpen_1 = false;
			listView_1 = null;;
			iScrollPane_1 = null;;
			
			dataProxy.PetSavvyJoinIsOpen = false;
			dataProxy = null;
			facade.removeMediator(PetSavvyJoinMediator.NAME);
		}
		
		/** 点击按钮 选择宠物0、选择宠物1、确定合成、取消合成、选择宠物0、关闭宠物0面板 */
		private function btnHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":								
					sureToJoin();
					break;
				case "btnLookMain":
					lookPet(0);
					break;
				case "btnLookSub":
					lookPet(1);
					break;
				case "mcPhotoMale":
					removePetSelect(0);
					break;
				case "mcPhotoFemale":
					removePetSelect(1);
					break;
			}
		}
		
		private function removePetSelect(type:uint):void
		{
			if(type == 0) {
				if(PetSavvyJoinConstData.petShow_0) {
					PetSavvyJoinConstData.petShow_0 = null;
					uiManager.clearData(0);
				}
				if(PetSavvyJoinConstData.petShow_1) {
					PetSavvyJoinConstData.petShow_1 = null;
					uiManager.clearData(1);
				}
				sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
				petSavvyJoinView.txtSucPercent.text = "";
				petSavvyJoinView.txtLoseTo.text = "";
				PetSavvyJoinConstData.breedCost = 0;
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyJoinConstData.breedCost);
			} else {
				if(PetSavvyJoinConstData.petShow_1) {
					PetSavvyJoinConstData.petShow_1 = null;
					uiManager.clearData(1);
					sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
				}
			}
		}
		
		/** 查看宠物详细属性 0-主，1-副 */
		private function lookPet(type:int):void
		{
			if(type == 0) {
				if(PetSavvyJoinConstData.petShow_0) {
					PetPropConstData.isSearchOtherPetInto = true;
					dataProxy.PetEudemonTmp = PetSavvyJoinConstData.petShow_0;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetSavvyJoinConstData.petShow_0.Id, ownerId:GameCommonData.Player.Role.Id});
				}
			} else {
				if(PetSavvyJoinConstData.petShow_1) {
					PetPropConstData.isSearchOtherPetInto = true;
					dataProxy.PetEudemonTmp = PetSavvyJoinConstData.petShow_1;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetSavvyJoinConstData.petShow_1.Id, ownerId:GameCommonData.Player.Role.Id});
				}
			}
		}
		
		private function updateCurPet():void
		{
			if(PetSavvyJoinConstData.petShow_0) {
//				PetSavvyJoinConstData.petSelected_1 = null;
//				PetSavvyJoinConstData.petSelected_0 = GameCommonData.Player.Role.PetSnapList[PetSavvyJoinConstData.petShow_0.Id];
				PetSavvyJoinConstData.petShow_0 = GameCommonData.Player.Role.PetSnapList[PetSavvyJoinConstData.petShow_0.Id];
				if(PetSavvyJoinConstData.petShow_0.Savvy == 10) {
					petSavvyJoinView.txtSucPercent.text = "";
					petSavvyJoinView.txtLoseTo.text = "";
					PetSavvyJoinConstData.breedCost = 0;
				} else {
					PetSavvyJoinConstData.breedCost = PetSavvyJoinConstData.breedCostList[PetSavvyJoinConstData.petShow_0.Savvy];
					petSavvyJoinView.txtSucPercent.text = PetSavvyJoinConstData.successList[PetSavvyJoinConstData.petShow_0.Savvy];
					var loseTo:String = "";
					if(PetSavvyJoinConstData.petShow_0.Savvy >7) {
						loseTo = "<font color='#00ff00'>" + GameCommonData.wordDic[ "often_used_losedownto" ] + "</font><font color='#ff0000'>7</font>";  // 失败降为
					} else if(PetSavvyJoinConstData.petShow_0.Savvy == 7) {
						loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];   // 失败不降
					} else if(PetSavvyJoinConstData.petShow_0.Savvy > 4) {
						loseTo = "<font color='#00ff00'>" + GameCommonData.wordDic[ "often_used_losedownto" ] + "</font><font color='#ff0000'>4</font>";  // 失败降为
					} else {
						loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];   // 失败不降
					}
					petSavvyJoinView.txtLoseTo.htmlText = loseTo;
				}
//				petSavvyJoinView.txtCurSavvy.text = PetSavvyJoinConstData.petShow_0.Savvy.toString();
				uiManager.clearData(0);
				uiManager.showData(PetSavvyJoinConstData.petShow_0, 0);
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyJoinConstData.breedCost);
				petSavvyJoinView.btnSure.mouseEnabled = true;
			}
		}
		
		/** 确定合成 */
		private function sureToJoin():void
		{
			if(!PetSavvyJoinConstData.petShow_0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_1" ], color:0xffff00});  // 请选择要主宠物
				return;
			}
			if(!PetSavvyJoinConstData.petShow_1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_2" ], color:0xffff00});  // 请选择副宠物
				return;
			}
			if(PetSavvyJoinConstData.petShow_0.Id == PetSavvyJoinConstData.petShow_1.Id) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_3" ], color:0xffff00});   // 不同宠物才能合成
				return;
			}
//			if(PetSavvyJoinConstData.petShow_0.ClassId != PetSavvyJoinConstData.petShow_1.ClassId) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"同类宠物才能合成", color:0xffff00});
//				return;
//			}
			if(PetSavvyJoinConstData.petShow_1.TakeLevel < PetSavvyJoinConstData.petShow_0.TakeLevel) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_4" ], color:0xffff00});   // 副宠物的携带等级需比主宠物的高
				return;
			}
			if(PetSavvyJoinConstData.petShow_1.Genius <= PetSavvyJoinConstData.petShow_0.Savvy) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_5" ], color:0xffff00});   // 副宠物的天赋需比主宠物的悟性高
				return;
			}
			if(PetSavvyJoinConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_6" ], color:0xffff00});  // 没有足够的银两
				return;
			}
			//发送合成命令 带ID_0,  ID_1
			PetNetAction.petBreed(PlayerAction.PET_SAVVY_JOIN, PetSavvyJoinConstData.petShow_0.Id, PetSavvyJoinConstData.petShow_1.Id);
			petSavvyJoinView.btnSure.mouseEnabled = false;
			PetSavvyJoinConstData.petShow_1 = null;
			uiManager.clearData(1);
//			petSavvyJoinView.txtPetName_1.text = "";
//			petSavvyJoinView.txtCurGenius.text = "";
//			setTimeout(canOp, 1000);
//			gcAll();
		}
		
		private function canOp():void
		{
			if(petSavvyJoinView) petSavvyJoinView.btnSure.mouseEnabled = true;
		}
		
		private function selectPet(id:uint):void
		{
			if(PetSavvyJoinConstData.petShow_0) {
				petToShow_1(id);
			} else {
				petToShow_0(id);
			}
		}
		
		/** 选择宠物0进界面 */
		private function petToShow_0(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				if(GameCommonData.Player.Role.PetSnapList[PetSavvyJoinConstData.petSelected_0.Id]) {
//				if(PetSavvyJoinConstData.petShow_0 && PetSavvyJoinConstData.petShow_0.Id == PetSavvyJoinConstData.petSelected_0.Id) {
//					return;
//				}
				if(GameCommonData.Player.Role.PetSnapList[id].State == 4)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
					return;
				}
				if(PetSavvyJoinConstData.petShow_1 && PetSavvyJoinConstData.petShow_1.Id == id) {
					return;
				}
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
				if(pet.Savvy == 10) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_1" ], color:0xffff00});  // 该宠物悟性已达到上限
					return;
				}
				if(pet.State == 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_2" ], color:0xffff00});  // 出战状态的宠物不能提升悟性
					return;
				}
				PetSavvyJoinConstData.petShow_0 = GameCommonData.Player.Role.PetSnapList[id];
//				petSavvyJoinView.txtPetName_0.text = PetSavvyJoinConstData.petShow_0.PetName;
				PetSavvyJoinConstData.breedCost = PetSavvyJoinConstData.breedCostList[PetSavvyJoinConstData.petShow_0.Savvy];
//				uiManager.showMoney(0);
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSavvyJoinConstData.breedCost);
				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
				
				petSavvyJoinView.txtSucPercent.text = PetSavvyJoinConstData.successList[pet.Savvy];
				var loseTo:String = "";

				if(pet.Savvy >7) {
					loseTo = "<font color='#00ff00'>"+ GameCommonData.wordDic[ "often_used_losedownto" ]+"</font><font color='#ff0000'>7</font>";
				} else if(pet.Savvy == 7) {
					loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];
				} else if(pet.Savvy > 4) {
					loseTo = "<font color='#00ff00'>"+GameCommonData.wordDic[ "often_used_losedownto" ]+"</font><font color='#ff0000'>4</font>";
				} else {
					loseTo = GameCommonData.wordDic[ "often_used_losenotdown" ];
				}
				petSavvyJoinView.txtCurSavvy.text = PetSavvyJoinConstData.petShow_0.Savvy.toString();
				petSavvyJoinView.txtLoseTo.htmlText = loseTo;
				uiManager.showData(GameCommonData.Player.Role.PetSnapList[id], 0);
//					petSavvyJoinView.txtLostPercent.text = "失败加成："+(PetSavvyJoinConstData.petShow_0.LoseTime * 2 +"%");
//				}
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			}
		}
		
		/** 选择宠物1进界面 */
		private function petToShow_1(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				if(GameCommonData.Player.Role.PetSnapList[PetSavvyJoinConstData.petSelected_1.Id]) {
				if(GameCommonData.Player.Role.PetSnapList[id].State == 4)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"附体状态的宠物无法进行此操作", color:0xffff00});  //  附体状态的宠物无法进行此操作
					return;
				}
				if(PetSavvyJoinConstData.petShow_1 && PetSavvyJoinConstData.petShow_1.Id == id) {
					return;
				}
				if(PetSavvyJoinConstData.petShow_0 && PetSavvyJoinConstData.petShow_0.Id == id) {
					return;
				}
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
				if(PetSavvyJoinConstData.petShow_0 && pet.TakeLevel < PetSavvyJoinConstData.petShow_0.TakeLevel) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_4" ], color:0xffff00});  // 副宠物的携带等级需比主宠物的高
					return;
				}
				if(PetSavvyJoinConstData.petShow_0 && PetSavvyJoinConstData.petShow_0.Savvy >= pet.Genius) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_5" ], color:0xffff00});  //  副宠物的天赋需比主宠物的悟性高
					return;
				}
				if(pet.State == 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_pet_3" ], color:0xffff00});  // 出战状态的宠物不能参与合成
					return;
				}
				PetSavvyJoinConstData.petShow_1 = GameCommonData.Player.Role.PetSnapList[id];
				
				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
				uiManager.showData(GameCommonData.Player.Role.PetSnapList[id], 1);
//				petSavvyJoinView.txtPetName_1.text = PetSavvyJoinConstData.petShow_1.PetName;
//				petSavvyJoinView.txtCurGenius.text = PetSavvyJoinConstData.petShow_1.Genius;
//				} 
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});  // 该宠物不存在
				gcAll();
			}
		}
		
		
	}
}