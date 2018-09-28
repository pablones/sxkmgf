package GameUI.Modules.PetPlayRule.PetBreedDouble.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Data.PetBreedDoubleConstData;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Data.PetBreedDoubleEvent;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.UI.PetBreedDoubleUIManager;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetBreedDoubleMediator extends Mediator
	{
		public static const NAME:String = "petBreedDoubleMediator";
		private var dataProxy:DataProxy = null;
		private var panelBase:PanelBase = null;
		private var uiManager:PetBreedDoubleUIManager = null;
		
		/** 是否需要发送取消繁殖 */
		private var needSendCancel:Boolean = true;
		
		//宠物选择
//		private var petChoiceView:MovieClip = null;
//		private var petChoicePanelBase:PanelBase = null;
//		private const PETCHOICEPANEL_POS:Point = new Point(450, 150);
//		private var petChoiceOpen:Boolean = false;
//		private var listView:ListComponent;
//		private var iScrollPane:UIScrollPane;
		//
		
		
		public function PetBreedDoubleMediator()
		{
			super(NAME);
		}
		
		private function get petBreedDoubleView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
//				PetBreedDoubleEvent.SHOW_PETBREEDDOUBLE_VIEW,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
//				EventList.CLOSE_NPC_ALL_PANEL,
				PetBreedDoubleEvent.LOCKED_OP_BREEDDOUBLE,
				PetBreedDoubleEvent.ADDPET_OP_BREEDDOUBLE,
				PetBreedDoubleEvent.BEGIN_BREEDDOUBLE,
				PetBreedDoubleEvent.FAIL_BREEDDOUBLE,
				PetBreedDoubleEvent.UNLOCK_OP_BREEDDOUBLE,
				PetBreedDoubleEvent.UNLOCK_SELF_BREEDDOUBLE,
				PetBreedDoubleEvent.ADDPET_SELF_BREEDDOUBLE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:		//PetBreedDoubleEvent.SHOW_PETBREEDDOUBLE_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetBreedDoubleIsOpen) {
						initView();
						addLis();
						dataProxy.PetBreedDoubleIsOpen = true;
						setModel();
					}
					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:		//EventList.CLOSE_PET_PLAYRULE_VIEW:
					if(needSendCancel) {
						PetNetAction.opPet(PlayerAction.PET_CANCEL_BREED);
					}						
					closeHandler(null);		//关闭面板
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetBreedDoubleIsOpen) closeHandler(null);	
//					break;
				case PetBreedDoubleEvent.LOCKED_OP_BREEDDOUBLE:							//对方锁定
					PetBreedDoubleConstData.isLockOp = true;
					petBreedDoubleView.btnLockOp.visible = false;
					setModel();
					break;
				case PetBreedDoubleEvent.UNLOCK_OP_BREEDDOUBLE:							//对方解锁
					PetBreedDoubleConstData.isLockOp = false;
					petBreedDoubleView.btnLockOp.visible = true;
					setModel();
					break;
				case PetBreedDoubleEvent.UNLOCK_SELF_BREEDDOUBLE:						//自己解锁
					PetBreedDoubleConstData.isLockSelf = false;
					petBreedDoubleView.btnLockSelf.visible = true;
//					petBreedDoubleView.btnSelectPet.visible = true;    //  对方宠物信息改变,锁定自动解除
					facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_han_1" ], color:0xffff00 } );
					setModel();
					break;
				case PetBreedDoubleEvent.ADDPET_SELF_BREEDDOUBLE:						//自己加宠物
					var petIdSelf:uint = uint(notification.getBody());
					if(GameCommonData.Player.Role.PetSnapList[petIdSelf]) {
						PetBreedDoubleConstData.petShowSelf = GameCommonData.Player.Role.PetSnapList[petIdSelf];
						uiManager.addPet(0);
					}
					break;
				case PetBreedDoubleEvent.ADDPET_OP_BREEDDOUBLE:							//对方加宠物
					var petOp:GamePetRole = notification.getBody() as GamePetRole;
					PetBreedDoubleConstData.petShowOp = petOp;
					uiManager.addPet(1);
					break;
				case PetBreedDoubleEvent.BEGIN_BREEDDOUBLE:								//成功开始繁殖
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_han_2" ], color:0xffff00});    //  开始繁殖
					needSendCancel = false;
					sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
//					gcAll();
					break;
				case PetBreedDoubleEvent.FAIL_BREEDDOUBLE:								//繁殖失败
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_han_3" ], color:0xffff00});     //  繁殖失败
					needSendCancel = false;
					sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
					break;
				case PetRuleEvent.SELECT_PET_PET_RULE:
					addPetSelf(notification.getBody() as uint);
					break; 
			}
		}
		
		private function initView():void 
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetBreedDoubleView");

			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petBreedDoubleView);
			
			petCon.subView = petBreedDoubleView;
			
			uiManager = new PetBreedDoubleUIManager(petBreedDoubleView);
			
		}
		
		private function addLis():void
		{
			petBreedDoubleView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedDoubleView.btnLockSelf.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedDoubleView.btnLookOp.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedDoubleView.btnLookSelf.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private  function removeLis():void
		{
			petBreedDoubleView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedDoubleView.btnLockSelf.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedDoubleView.btnLookOp.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedDoubleView.btnLookSelf.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function closeHandler(e:Event):void
		{
//			PetNetAction.opPet(PlayerAction.PET_CANCEL_BREED);
			gcAll();	
		}
		
		private function gcAll():void
		{
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
//			dataProxy.PetCanOperate = true;
			viewComponent = null;
			panelBase = null;
			PetBreedDoubleConstData.petSelected = null;
			PetBreedDoubleConstData.petShowOp = null;
			PetBreedDoubleConstData.petShowSelf = null;
			PetBreedDoubleConstData.isLockOp = false;
			PetBreedDoubleConstData.isLockSelf = false;
			needSendCancel = true;
			uiManager = null;
			dataProxy.PetBreedDoubleIsOpen = false;
			dataProxy = null;
			facade.removeMediator(PetBreedDoubleMediator.NAME);
		}
		
		/** 点击按钮 查看对方宠物、查看自己宠物、锁定、确定、选择宠物、取消 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnLockSelf":									//自己锁定
					lockSelf();
					break;
				case "btnLookSelf":									//查看己方宠物
					lookPet(0);
					break;
				case "btnLookOp":									//查看对方宠物
					lookPet(1);
					break;
				case "btnSure":										//确定繁殖
					sureToBreed();
					break;
			}
		}
		
		/** 锁定己方宠物 */
		private function lockSelf():void
		{
			if(PetBreedDoubleConstData.petShowSelf) {
				petBreedDoubleView.btnLockSelf.visible = false;
				PetBreedDoubleConstData.isLockSelf = true;
//				choiceCloseHandler(null);
//				petBreedDoubleView.btnSelectPet.visible = false;
				PetNetAction.opPet(PlayerAction.PET_LOCKSELF_BREEDDOUBLE);
				setModel();
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});    //  请先选择宠物
			}
		}
		
		/** 查看宠物详细属性 0-己方，1-对方 */
		private function lookPet(type:int):void
		{
			if(type == 0) {
				PetPropConstData.isSearchOtherPetInto = true;
				if(PetBreedDoubleConstData.petShowSelf) {
					dataProxy.PetEudemonTmp = PetBreedDoubleConstData.petShowSelf;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetBreedDoubleConstData.petShowSelf.Id, ownerId:GameCommonData.Player.Role.Id});
				}
			} else {
				if(PetBreedDoubleConstData.petShowOp) {
					dataProxy.PetEudemonTmp = PetBreedDoubleConstData.petShowOp;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetBreedDoubleConstData.petShowOp.Id, ownerId:PetBreedDoubleConstData.petShowOp.OwnerId});
				}
			}
		}
		
		private function addPetSelf(id:uint):void
		{
			if(PetBreedDoubleConstData.isLockSelf) return;
			if(GameCommonData.Player.Role.PetSnapList[id]) {
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
				if(PetBreedDoubleConstData.petShowSelf && PetBreedDoubleConstData.petShowSelf.Id == id) {
					return;
				}
				if(pet.isFantasy)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petBreedDouble_1"], color:0xffff00});  //  "已幻化的宠物不能繁殖"
					return;
				}
				if(pet.State == 4)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  "附体状态的宠物无法进行此操作"
					return;
				}
//				if(!GameCommonData.Player.Role.PetSnapList[PetBreedDoubleConstData.petSelected.Id]) {
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该宠物不存在", color:0xffff00});
//					gcAll();
//					return;
//				}
				if(pet.Type != 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_1" ], color:0xffff00});   //  只有宝宝可以繁殖
					return;
				}
				/////////
				if(pet.BreedNow == pet.BreedMax) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_2" ], color:0xffff00});   //  该宠物已达最大繁殖
					return;
				}
				if(pet.Level < (30 * (pet.BreedNow+1)) ) {
					//  该宠物需  级以上才能继续繁殖
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_3" ]+(30 * (pet.BreedNow+1))+GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_4" ], color:0xffff00});
					return;
				}
				if(PetBreedDoubleConstData.petShowOp && PetBreedDoubleConstData.petShowOp.FaceType != pet.FaceType) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_5" ], color:0xffff00});   //  同种生物才能繁殖
					return;  
				}
				if(PetBreedDoubleConstData.petShowOp && PetBreedDoubleConstData.petShowOp.Sex == pet.Sex) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_6" ], color:0xffff00});   //  两只异性宠物才能繁殖
					return;
				}
				if(GameCommonData.Player.Role.PetSnapList[id].State == 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_7" ], color:0xffff00});   //  出战状态的宠物不能繁殖
					return;
				}
				
				/////////
				//发送自己添加宠物命令  带宠物ID
				PetNetAction.opPet(PlayerAction.PET_ADDEUM_BREEDDOUBLE, id);
				PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
				sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);	//点击成功
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   //  该宠物不存在
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
			}
		}
		
		/** 确定繁殖 */
		private function sureToBreed():void
		{
			if(!PetBreedDoubleConstData.petShowSelf || !PetBreedDoubleConstData.petShowOp) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_1" ], color:0xffff00});   // 两只宠物才能繁殖
				return;
			}
			if(!dataProxy.isTeamMember) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_2" ], color:0xffff00});    ///    需组队才能繁殖
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
				return;
			}
			if(!dataProxy.isTeamLeader) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_3" ], color:0xffff00});    //  你不是队长
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
				return;
			}
			if(PetBreedDoubleConstData.petShowSelf.FaceType != PetBreedDoubleConstData.petShowOp.FaceType) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_4" ], color:0xffff00});   //  同种类宠物才能繁殖
				return;
			}
			if(PetBreedDoubleConstData.petShowSelf.Sex == PetBreedDoubleConstData.petShowOp.Sex) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_5" ], color:0xffff00});   // 两只异性宠物才能繁殖
				return;
			}
			if(PetBreedDoubleConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});  // 没有足够的银两
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetBreedDoubleConstData.petShowSelf.Id]) {
				//发送繁殖命令，传。。。
				PetNetAction.petBreed(PlayerAction.PET_SURE_BREED, PetBreedDoubleConstData.petShowSelf.Id, PetBreedDoubleConstData.petShowOp.Id, PetBreedDoubleConstData.petShowOp.OwnerId);
				uiManager.setModel(1);
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_7" ], color:0xffff00});   // 宠物不存在
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
				return;
			}
		}
		
		/** 锁定检查，检查是否可以确定 */
		private function setModel(isSelf:Boolean = false):void
		{
			if(!dataProxy.isTeamMember) {
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_2" ], color:0xffff00 } );   // 需组队才能繁殖
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
				return;
			}
			if(dataProxy.isTeamLeader) {	//队长
				if(PetBreedDoubleConstData.isLockSelf && PetBreedDoubleConstData.isLockOp) {
					uiManager.setModel(0);
				} else {
					uiManager.setModel(1);
				}
			} else {						//队员
				uiManager.setModel(2);
			}
		}
	}
}
