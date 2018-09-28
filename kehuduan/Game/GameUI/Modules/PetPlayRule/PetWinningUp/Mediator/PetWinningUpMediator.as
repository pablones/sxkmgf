package GameUI.Modules.PetPlayRule.PetWinningUp.Mediator
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
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Proxy.PetSavvyUseMoneyGridManager;
	import GameUI.Modules.PetPlayRule.PetWinningUp.Data.PetWinningConstData;
	import GameUI.Modules.PetPlayRule.PetWinningUp.Data.PetWinningEvent;
	import GameUI.Modules.PetPlayRule.PetWinningUp.UI.PetWinningUIManager;
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
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class PetWinningUpMediator extends Mediator
	{
		public static const NAME:String = "PetWinningUpMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var gridManager:PetSavvyUseMoneyGridManager;
		private var uiManager:PetWinningUIManager;
		
		//宠物选择
		private var petChoiceView:MovieClip = null;
		private var petChoicePanelBase:PanelBase = null;
		private const PETCHOICEPANEL_POS:Point = new Point(450, 150);
		
		private var petChoiceOpen:Boolean = false;
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		private var sendDistanceTime:int;
		//
		
		public function PetWinningUpMediator()
		{
			super(NAME);
		}
		
		private function get petWinningView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
				PetEvent.RETURN_TO_SHOW_PET_INFO,
				PetWinningEvent.PET_WINNING_FEEDBACK
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:	
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetBreedDoubleIsOpen) {
						initView();
						addLis();
					}
					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:	//EventList.CLOSE_PET_PLAYRULE_VIEW:
					closeHandler(null);
					break;
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
				case PetWinningEvent.PET_WINNING_FEEDBACK:
					var petIdSavvy:uint = notification.getBody() as uint;
					if(petIdSavvy > 0) updateCurPet(petIdSavvy);
					break;
			}
		}
		
		private function initView():void 
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetWinningView");

			
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petWinningView);
			
			petCon.subView = petWinningView;
			
			uiManager = new PetWinningUIManager(petWinningView);
			
		}
		
		
		private function addLis():void
		{
			petWinningView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petWinningView.btnLook.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petWinningView.mcPhotoPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private  function removeLis():void
		{
			petWinningView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petWinningView.btnLook.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petWinningView.mcPhotoPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
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
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				if(iScrollPane && petWinningView.contains(iScrollPane)) {
					iScrollPane.removeChild(listView);
					petWinningView.removeChild(iScrollPane);
				}
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			if(PetWinningConstData.itemData) {
				sendNotification(EventList.BAGITEMUNLOCK, PetWinningConstData.itemData.id);
			}
			uiManager = null;
			panelBase = null;
			petChoiceView = null;
			petChoicePanelBase = null;
			listView = null;
			iScrollPane = null;
			
			PetWinningConstData.petShow = null;
			PetWinningConstData.itemTypeNeed = "";
			PetWinningConstData.GridItemUnit = null;
			PetWinningConstData.itemData = null;
			
			dataProxy = null;
//			facade.removeProxy(PetSavvyUseMoneyGridManager.NAME);
			facade.removeMediator(NAME);
		}
		
		/** 点击按钮 选择宠物、确定、取消 */
		private  function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":								//确定提升灵性
					sureAddWinning();
					break;
				case "btnLook":						//取消提升灵性
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
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetWinningConstData.breedCost);
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
		
		/** 灵性提升回馈 */
		private function updateCurPet(petId:uint):void
		{
			var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[petId];
			PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[petId];
			
			dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet; 
			dataProxy.PetQuerySkill = true;
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:petId, ownerId:GameCommonData.Player.Role.Id});
		}
		
		/** 确定提升灵性 */
		private function sureAddWinning():void
		{
			if(!PetRuleCommonData.selectedPet) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});  // 请先选择宠物
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWiningUp_1"], color:0xffff00});  // "出战状态的宠物不能提升灵性"
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].winning == 10) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWiningUp_2"], color:0xffff00});  // "该宠物灵性已达到上限"
				return;
			}
			if(PetRuleCommonData.selectedPet.Savvy <= PetRuleCommonData.selectedPet.winning)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWiningUp_3"], color:0xffff00});  // "宠物的灵性值不能大于悟性值"
				return;
			}
			if(PetWinningConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});   // 没有足够的银两
				return;
			}
			if(!checkItem()) return;
			if(!GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id]) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			} else {	//灵性提升
				if(getTimer() - sendDistanceTime > 80)
				{
					PetNetAction.petBreed(PlayerAction.NEWPET_WINNING_TAG, PetRuleCommonData.selectedPet.Id, 0,0);//PetNetAction.petBreed(PlayerAction.PET_SAVVY_USEMONEY, PetWinningConstData.petShow.Id, 0,0, PetWinningConstData.itemData.id);
					sendDistanceTime = getTimer();
				}
				else
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWiningUp_4"] , color:0xffff00});  // 点击过快
				}
			}
		}
		
		private function canOp():void
		{
			if(petWinningView) petWinningView.btnSure.mouseEnabled = true;
		}
		
		/** 选择宠物进显示栏 */
		private function petToShow(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				if(!GameCommonData.Player.Role.PetSnapList[PetWinningConstData.petSelected.Id]) {
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该宠物不存在", color:0xffff00});
//					gcAll();
//					return;
//				}
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
				if(pet.State == 4)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
					return;
				}
				
				if(pet.State == 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWiningUp_5"], color:0xffff00});  //"出战状态的宠物不能提升灵性"
					return;
				}
				if(pet.winning == 10)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWinning_petToShow_1" ], color:0xffff00});  //"出战状态的宠物不能提升灵性"
					return;
				}
				PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
				
//				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
				/////////////
				uiManager.clearData();
				
				if(PetRuleCommonData.selectedPet.LifeMax > 0) {	//有详细信息
					uiManager.showData(PetRuleCommonData.selectedPet);
					//---
					

					uiManager.showData(PetRuleCommonData.selectedPet);
					var cost:int = PetWinningConstData.breedCost;
					sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR,cost);
					//---
				} else {										//没有，去服务器查询
					dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
					dataProxy.PetQuerySkill = true;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
				}
				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
				/////////
//				petSavvyUseMoneyView.txtPetName.text = PetWinningConstData.petShow.PetName;
//				if(PetWinningConstData.petShow.Savvy >= 9) {
//					loseTo = "不降";
//				} else 
//				? "成功率：100%" : "成功率："+PetWinningConstData.successList[PetWinningConstData.petShow.Savvy]+"<font color='"+GameCommonData.Player.Role.VIPColor+"'>+"+PetWinningConstData.VIP_SUCCESS_LEV[GameCommonData.Player.Role.VIP]+"</font>";
//					petSavvyUseMoneyView.txtCurSavvy.text = PetWinningConstData.petShow.Savvy.toString();
//				uiManager.showMoney(0);
//				petSavvyUseMoneyView.txtSucPercent.text = "成功率："+PetWinningConstData.successList[PetWinningConstData.petShow.Savvy];
//				petSavvyUseMoneyView.txtLostPercent.text = "失败加成："+(PetWinningConstData.petShow.LoseTime * 2 + "%");
//				gridManager.removeItem();	//移除道具，须重新拖进灵性丹
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			}
		}
		
		private function checkItem():Boolean
		{
			var result:Boolean = true;
			var type:uint = 630034;	//灵犀丹
			if(!BagData.isHasItem(type)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petWiningUp_6"], color:0xffff00});   // "灵犀丹不足，请补充"
				result = false;
			}
			return result;
		}
		
		
	}
}