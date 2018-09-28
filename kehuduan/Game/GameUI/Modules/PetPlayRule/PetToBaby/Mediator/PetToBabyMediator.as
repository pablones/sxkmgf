package GameUI.Modules.PetPlayRule.PetToBaby.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnConstData;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyConstData;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyEvent;
	import GameUI.Modules.PetPlayRule.PetToBaby.Proxy.PetToBabyGridManager;
	import GameUI.Modules.PetPlayRule.PetToBaby.UI.PetToBabyUIManager;
	import GameUI.Proxy.DataProxy;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetToBabyMediator extends Mediator
	{
		public static const NAME:String = "petToBabyMediator";
		private var dataProxy:DataProxy = null;
		private var gridManager:PetToBabyGridManager = null;
		private var uiManager:PetToBabyUIManager = null;
		
//		private var panelBase:PanelBase = null;
//		private var listView:ListComponent = null;
//		private var iScrollPane:UIScrollPane = null;
		
		public function PetToBabyMediator()
		{
			super(NAME);
		}
		
		private function get petToBabyView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				PetToBabyEvent.SHOW_PETTOBABY_VIEW,
				
//				PetToBabyEvent.BAG_TO_PETTOBABY,
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
//				EventList.CLOSE_NPC_ALL_PANEL,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
				PetEvent.RETURN_TO_SHOW_PET_INFO,
				PetToBabyEvent.PET_TO_BABY_RETURN
//				EventList.UPDATEMONEY,
//				BagEvents.EXTENDBAG						//宠物背包扩充
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:			//PetToBabyEvent.SHOW_PETTOBABY_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetToBabyIsOpen) {
						initView();
						addLis();
						dataProxy.PetToBabyIsOpen = true;
					}
					break; 
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:			//EventList.CLOSE_PET_PLAYRULE_VIEW:
					closeHandler(null);
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetToBabyIsOpen) closeHandler(null);	
//					break;
//				case PetToBabyEvent.BAG_TO_PETTOBABY:
//					var itemToBaby:Object = notification.getBody();
//					addToBabyItem(itemToBaby);
//					break;
//				case BagEvents.EXTENDBAG:											//宠物背包扩充
//					if(dataProxy && dataProxy.PetToBabyIsOpen) {
//						uiManager.updatePetNum();
//					}
//					break;
//				case EventList.UPDATEMONEY:			//刷新钱
//					uiManager.showMoney(1);
//					uiManager.showMoney(2);
//					break;
				case PetRuleEvent.SELECT_PET_PET_RULE:
					selectItem(notification.getBody() as uint);
					break;
				case PetToBabyEvent.PET_TO_BABY_RETURN:
					var petBabyId:uint = uint(notification.getBody());
					if(petBabyId > 0) updateCurPet(petBabyId);
					break;
				case PetEvent.RETURN_TO_SHOW_PET_INFO:
					var info:GamePetRole = notification.getBody() as GamePetRole;
					if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == info.Id && dataProxy.PetQuerySkill) {
						dataProxy.PetQuerySkill = false;
						PetRuleCommonData.selectedPet = info;
						uiManager.showData();
						showSkillInfo(info);
					}
					break;
			}
		}
		
		private function updateCurPet(petBabyId:uint):void
		{
			if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petBabyId) {
				PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id];
				if(gridManager) gridManager.removeAllItem();
				uiManager.clearData();
				uiManager.showData();
				
				dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
				dataProxy.PetQuerySkill = true; 
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:petBabyId, ownerId:GameCommonData.Player.Role.Id});
			}
			petToBabyView.btnSure.mouseEnabled = true;
			sendNotification(PetRuleEvent.UPDATE_PETNAME_LISTVIEW_PET_RULE, petBabyId);
		}
		
		private function initView():void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetToBabyView");
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petToBabyView);
			
			petCon.subView = petToBabyView;
			
			uiManager = new PetToBabyUIManager(petToBabyView);
			if(PetPropConstData.isNewPetVersion)
			{
				this.petToBabyView.removeChild(petToBabyView.skillContainer);
				this.petToBabyView.mianView.y = this.petToBabyView.mianView.y + 20;
			}
			else
			{
				initGrid();
			}
		}
		
		private function selectItem(id:uint):void
		{
			if(!GameCommonData.Player.Role.PetSnapList[id]) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
				return;
			} 
			if((GameCommonData.Player.Role.PetSnapList[id] as GamePetRole).State == 4)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
				return;
			}
			if((GameCommonData.Player.Role.PetSnapList[id] as GamePetRole).Sex == 2)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petToBaby_1"], color:0xffff00});  //  "幻化的宠物不能还童"
				return;
			}
			if(PetToBabyConstData.toBabyPet && id == PetToBabyConstData.toBabyPet.Id) return;
			if(GameCommonData.Player.Role.PetSnapList[id].Type > 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sel_1" ], color:0xffff00});  //  二代宠物无法还童
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[id].State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sel_2" ], color:0xffff00});  // 出战状态的宠物不能还童
				return;
			}
//			
			uiManager.clearData();
			if(gridManager) gridManager.removeAllItem();
			PetToBabyConstData.skillDataList = [];
			PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
			sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);	//点击成功
			if(PetRuleCommonData.selectedPet.LifeMax > 0) {	//有详细信息
				uiManager.showData();
				showSkillInfo(PetRuleCommonData.selectedPet);
			} else {										//没有，去服务器查询
				dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
				dataProxy.PetQuerySkill = true;
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
			}
			changeMoney();
		}
		
		private function removePetSelected():void
		{
			if(PetRuleCommonData.selectedPet) {
				PetRuleCommonData.selectedPet = null;
				uiManager.clearData();
				if(gridManager) gridManager.removeAllItem();
				PetToBabyConstData.skillDataList = [];
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, 0);
				sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
			}
		}
		
		private function changeMoney():void
		{
			var lev:int = PetRuleCommonData.selectedPet.TakeLevel;
			var money:int = 0;
			if(lev >= 75) {
				money = 400;
			} else if(lev >= 45) {
				money = 300;
			} else {
				money = 100;
			}
//			uiManager.showMoney(0);
			PetToBabyConstData.breedCost = money;
			sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, money); 
		}
		
		/** 初始化道具格子 */
		private function initGrid():void
		{
			for(var i:int = 0; i < 6; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6) + 60;
				gridUnit.y = (gridUnit.height) * int(i/6) + 241;
				gridUnit.name = "petSkillLearnShow_" + i.toString();
				petToBabyView.addChild(gridUnit);							//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = petToBabyView;							//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				PetToBabyConstData.GridUnitList.push(gridUint);
			}
			gridManager = new PetToBabyGridManager(PetToBabyConstData.GridUnitList, petToBabyView);
			facade.registerProxy(gridManager);
		}
		
		/** 显示技能信息 */
		private function showSkillInfo(pet:GamePetRole):void
		{
			if(!gridManager) return;
//			PetToBabyConstData.skillDataList = pet.SkillLevel;
			PetSkillLearnConstData.skillDataList = pet.SkillLevel;
			if(PetSkillLearnConstData.skillDataList.length > 0) {
				gridManager.showItems(PetSkillLearnConstData.skillDataList);
			}
		}
		
		private function addLis():void
		{
			petToBabyView.mianView.btnLook.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petToBabyView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petToBabyView.mianView.mcPhotoPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private  function removeLis():void
		{
			petToBabyView.mianView.btnLook.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petToBabyView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petToBabyView.mianView.mcPhotoPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function closeHandler(e:Event):void
		{
			gcAll();	
		}
		
		private  function gcAll():void 
		{
			removeLis();
//			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
//				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
//			}
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
			if(PetToBabyConstData.itemData) {
				sendNotification(EventList.BAGITEMUNLOCK, PetToBabyConstData.itemData.id);
			}
//			dataProxy.PetCanOperate = true;
			viewComponent = null;
//			panelBase = null;
			PetToBabyConstData.GridItemUnit = null;
			PetToBabyConstData.itemData = null;
			PetToBabyConstData.toBabyPet = null;
			PetToBabyConstData.breedCost = 0;
			PetToBabyConstData.GridUnitList = [];
//			PetToBabyConstData.skillDataList = [];
			PetSkillLearnConstData.skillDataList = [];
//			gridManager = null;
			uiManager = null;
//			panelBase = null;
//			listView = null;
//			iScrollPane = null;
			facade.removeProxy(PetToBabyGridManager.NAME); 
			gridManager = null;
			dataProxy.PetToBabyIsOpen = false;
			dataProxy = null;
//			facade.removeProxy(PetToBabyGridManager.NAME);
			facade.removeMediator(PetToBabyMediator.NAME);
		}
		
		/** 点击按钮 确定、取消 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":
					sureToBaby();
					break;
				case "btnLook":
					lookPetInfo();
					break;
				case "btnPanelCancel":
					closeHandler(null);
					break;
				case "mcPhotoPet":
					removePetSelected();
					break;
			}
		}
		
		/** 查看宠物属性 */
		private function lookPetInfo():void
		{
			if(!PetRuleCommonData.selectedPet) {
				return;
			}
			var id:int = PetRuleCommonData.selectedPet.Id;
//			dataProxy.PetQuerySkill = true;
			PetPropConstData.isSearchOtherPetInto = true;
			dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
		}
		
		/** 确定还童 */
		private function sureToBaby():void
		{
			if(!PetRuleCommonData.selectedPet) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});  // 请先选择宠物
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id]) {
				if(judgeCanToBaby()) {
					if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].Grade >= 81) {
						// 该宠物已是  完美  成长，要继续还童吗？
						facade.sendNotification(EventList.SHOWALERT, {comfrim:comitToBaby, cancel:cancelToBaby, info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sur_1" ]+"<font color='#FFFF00'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ]+"</font>"+GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sur_2" ], 
																										title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_yes" ], cancelTxt:GameCommonData.wordDic[ "often_used_no" ] } );
					} else if (GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].Grade >= 61){
						//  该宠物已是  完美  成长，要继续还童吗？
						facade.sendNotification(EventList.SHOWALERT, {comfrim:comitToBaby, cancel:cancelToBaby, info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sur_1" ]+"<font color='#FFFF00'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ]+"</font>"+GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sur_2" ], 
																										title:GameCommonData.wordDic[ "often_used_warning" ], comfirmTxt:GameCommonData.wordDic[ "often_used_yes" ], cancelTxt:GameCommonData.wordDic[ "often_used_no" ] } );
					} else {
						PetNetAction.petBreed(PlayerAction.PET_TO_BABY, PetRuleCommonData.selectedPet.Id, 0,0);  //PetNetAction.petBreed(PlayerAction.PET_TO_BABY, PetToBabyConstData.toBabyPet.Id, 0,0, PetToBabyConstData.itemData.id);
						petToBabyView.btnSure.mouseEnabled = false;
					}
				}
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
			}
//			gcAll();
		}
		
		/** 确定宠物还童 */
		private function comitToBaby():void
		{
			if(judgeCanToBaby()) {
				PetNetAction.petBreed(PlayerAction.PET_TO_BABY, PetRuleCommonData.selectedPet.Id, 0,0);  //PetNetAction.petBreed(PlayerAction.PET_TO_BABY, PetToBabyConstData.toBabyPet.Id, 0,0, PetToBabyConstData.itemData.id);
				petToBabyView.btnSure.mouseEnabled = false;
			}
//				gridManager.removeItem();
//				uiManager.clearData();
//				PetToBabyConstData.toBabyPet = null;
//				setTimeout(canOp, 1000);
		}
		
		/** 取消宠物还童 */
		private function cancelToBaby():void
		{
			
		}
		
		/** 校验合法 */
		private function judgeCanToBaby():Boolean
		{
			var ret:Boolean = true;
			if(!PetRuleCommonData.selectedPet) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});   // 请先选择宠物
				ret = false;
			} else if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_sel_2" ], color:0xffff00});  // 出战状态的宠物不能还童
				ret = false;
			} else if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id].BreedNow > 0) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_jud_1" ], color:0xffff00});  // 繁殖过的宠物不能还童
				ret = false;
			} else if(!judgeLevItem()) {
				ret = false;
			} else if(PetToBabyConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psj_med_pets_sur_6" ], color:0xffff00});  //  没有足够的银两
				ret = false;
			}
//			if(!PetToBabyConstData.itemData) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入还童书", color:0xffff00});
//				return;
//			}
			return ret;
		}
		
		private function canOp():void
		{
			if(petToBabyView) petToBabyView.btnSure.mouseEnabled = true;
		}
		
		private function judgeItem(o:Object):Boolean
		{
			var result:Boolean = true;
			if(PetToBabyConstData.itemData) {
				result = false;
			} else if(o.type != 630008 && o.type != 630009 && o.type != 630010) {
				result = false;
			} else if(PetToBabyConstData.toBabyPet.TakeLevel >= 75) {
				if(o.type != 630010) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_jud_2" ], color:0xffff00});     // 请放入高级还童书
					result = false;
				}
			} else if(PetToBabyConstData.toBabyPet.TakeLevel >= 55) {
				if(o.type != 630009 && o.type != 630010) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_jud_3" ], color:0xffff00});  // 请放入中级还童书
					result = false;
				}
			} else {
				if(o.type != 630008 && o.type != 630009 && o.type != 630010) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_ptb_med_pett_jud_4" ], color:0xffff00});  // 请放入低级还童书
					result = false;
				}
			}
			return result;
		}
		
		private function judgeLevItem():Boolean
		{
			var lev:uint = PetRuleCommonData.selectedPet.TakeLevel;
			var needItemType:uint = 0;
			var needBook:String = "";
			var result:Boolean = true;
			if(lev > 70) {
				needItemType = 630010;
				needBook = GameCommonData.wordDic[ "often_used_highlevel" ];  // 高级
			} else if(lev > 40) {
				needItemType = 630009;
				needBook = GameCommonData.wordDic[ "often_used_midlevel" ];  // 中级
			} else {
				needItemType = 630008;
				needBook = GameCommonData.wordDic[ "often_used_lowerlevel" ];  // 低级
			}
			if(!BagData.isHasItem(needItemType)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:needBook + GameCommonData.wordDic[ "mod_pet_ptb_med_pett_jud_5" ], color:0xffff00});  // 还童书不足，请补充
				result = false;
			}
			return result;
		}
		
		
	}
}