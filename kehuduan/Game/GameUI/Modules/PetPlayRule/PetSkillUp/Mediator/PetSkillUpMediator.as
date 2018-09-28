package GameUI.Modules.PetPlayRule.PetSkillUp.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpConstData;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpEvent;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Proxy.PetSkillUpGridManager;
	import GameUI.Modules.PetPlayRule.PetSkillUp.UI.PetSkillUpUIManager;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetSkillUpMediator extends Mediator
	{
		public static const NAME:String = "petSkillUpMediator";
		private var dataProxy:DataProxy = null;
		private var gridManager:PetSkillUpGridManager = null;
		private var uiManager:PetSkillUpUIManager = null;
		
		private var panelBase:PanelBase = null;
		private var listView:ListComponent = null;
		private var iScrollPane:UIScrollPane = null;
		
		public function PetSkillUpMediator()
		{
			super(NAME);
		}
		
		private function get petSkillUpView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				PetSkillUpEvent.SHOW_PETSKILLLUP_VIEW,
//				PetSkillUpEvent.BAG_TO_PETSKILLUP,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetSkillUpEvent.SHOW_DRUG_PET_SKILLUP,
				PetSkillUpEvent.REMOVE_DRUG_PET_SKILLUP,
				PetSkillUpEvent.REFRESH_MONEY_COST_PET_SKILLUP,
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
//				EventList.CLOSE_NPC_ALL_PANEL,
				PetEvent.RETURN_TO_SHOW_PET_INFO,
				PetSkillUpEvent.UP_SKILL_SUCCESS_PET,
				PetRuleEvent.SELECT_PET_PET_RULE,
				PetRuleEvent.NOTICE_UP_SKILL_PET_RULE,
				PetRuleEvent.UPDATE_ITEMS_PET_RULE
//				EventList.UPDATEMONEY,
//				BagEvents.EXTENDBAG						//宠物背包扩充
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:	//PetSkillUpEvent.SHOW_PETSKILLLUP_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetSkillUpIsOpen) {
						initView();
						addLis();
						dataProxy.PetSkillUpIsOpen = true;
					}
					break;
//				case PetSkillUpEvent.BAG_TO_PETSKILLUP:
//					var itemToUp:Object = notification.getBody();
//					addToUpItem(itemToUp);
//					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:	//EventList.CLOSE_PET_PLAYRULE_VIEW:
					closeHandler(null);
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetSkillUpIsOpen) closeHandler(null);	
//					break;
				case PetEvent.RETURN_TO_SHOW_PET_INFO:
					var info:GamePetRole = notification.getBody() as GamePetRole;
					if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == info.Id && dataProxy.PetQuerySkill) {
						PetRuleCommonData.selectedPet = info;
						removeDrug();
						uiManager.showData();
						showSkillInfo(info);
					}
					break;
				case PetSkillUpEvent.REFRESH_MONEY_COST_PET_SKILLUP:				//刷新显示钱
					var moneyType:int = notification.getBody() as int;
					uiManager.showMoney(moneyType);
					break;
//				case BagEvents.EXTENDBAG:											//宠物背包扩充
//					if(dataProxy && dataProxy.PetSkillUpIsOpen) {
//						uiManager.updatePetNum();
//					}
//					break;
				case PetSkillUpEvent.UP_SKILL_SUCCESS_PET:
					var petInfo:Object = notification.getBody();
					updateCurPet(petInfo);
					break;
				case PetRuleEvent.SELECT_PET_PET_RULE:	
					selectItem(notification.getBody() as uint);
					break;
				case PetSkillUpEvent.SHOW_DRUG_PET_SKILLUP:
					showDrug(notification.getBody() as uint);
					break;
				case PetSkillUpEvent.REMOVE_DRUG_PET_SKILLUP:
					removeDrug();
					break;
				case PetRuleEvent.UPDATE_ITEMS_PET_RULE:
					updateItem();
					break;
				case PetRuleEvent.NOTICE_UP_SKILL_PET_RULE:
					var petId:uint = notification.getBody().id;
					if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petId) {
						sureToUpSkill();
					}
					break;
//				case EventList.UPDATEMONEY:
//					uiManager.showMoney(1);
//					uiManager.showMoney(2);
//					break;
			}
		}
		
		private function updateCurPet(petInfo:Object):void
		{
			var petId:uint = petInfo.id;
			var flag:uint  = petInfo.flag; 
			if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petId && flag > 0) {
//				PetSkillUpConstData.breedCost = 0;
				gridManager.removeAllItem();
//				PetSkillUpConstData.SelectedItem = null;
				PetSkillUpConstData.skillDataList = [];
//				gridManager.removeItem();
//				gridManager.removeSkillSelect();
	//			uiManager.clearData();
	//			uiManager.showData();
//				uiManager.showMoney(0);
				//更换界面数据 技能格子数据
				removeDrug();
				dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
				dataProxy.PetQuerySkill = true;
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:petId, ownerId:GameCommonData.Player.Role.Id});
			}
			petSkillUpView.btnSure.mouseEnabled = true;
		}
		
		private function initView():void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetSkillUpView");
			
//			panelBase = new PanelBase(petSkillUpView, petSkillUpView.width+8, petSkillUpView.height+11);
//			panelBase.name = "PetSkillUp";
//			panelBase.addEventListener(Event.CLOSE, closeHandler);
//			panelBase.x = UIConstData.DefaultPos1.x;
//			panelBase.y = UIConstData.DefaultPos1.y;
//			panelBase.SetTitleTxt("宠物技能提升");
			
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petSkillUpView);
			
			petCon.subView = petSkillUpView;
			
			uiManager = new PetSkillUpUIManager(petSkillUpView);
			
			initGrid();
			
//			initPetChoiceListData();
//			
//			GameCommonData.GameInstance.GameUI.addChild(panelBase);
		}
		
		private function selectItem(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id].State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psu_med_pets_sel_1" ], color:0xffff00});  // 出战状态的宠物不能提升技能
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[id].State == 4)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
				return;
			}
			//更换宠物数据
			PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
			
			sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
			sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSkillUpConstData.breedCost);
			
			uiManager.clearData();
			gridManager.removeAllItem();
			PetSkillUpConstData.skillDataList = [];
//			PetSkillUpConstData.SelectedItem = null;
//			uiManager.showData();
//			gridManager.removeSkillSelect();
//			gridManager.removeItem();
//			PetSkillUpConstData.breedCost = 0;
//			uiManager.showMoney(0);
			//更换界面数据 技能格子数据
			removeDrug();
			dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
			dataProxy.PetQuerySkill = true;
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
		}
		
		/** 显示技能信息 */
		private function showSkillInfo(pet:GamePetRole):void
		{
			PetSkillUpConstData.skillDataList = pet.SkillLevel;
			if(PetSkillUpConstData.skillDataList.length > 0) {
				gridManager.showItems(PetSkillUpConstData.skillDataList);
			}
		}
		
		private function initGrid():void
		{
			//快速购买
//			for(var j:int = 0; j < 3; j++) {
//				if(!(UIConstData.MarketGoodList[22] as Array)[j]) continue;
//				var good:Object = (UIConstData.MarketGoodList[22] as Array)[j];
//				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//				unit.x = 243;
//				unit.y = j * (70 + unit.height) + 12;
//				unit.name = "goodQuickBuy"+j+"_"+good.type;
//				petSkillUpView.addChild(unit);
//				
//				var useItem:UseItem = new UseItem(j, good.type, petSkillUpView);
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
//				petSkillUpView.addChild(useItem);
//				
//				petSkillUpView["txtGoodNamePet_"+j].text = good.Name;
//				petSkillUpView["mcMoney_"+j].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
//				ShowMoney.ShowIcon(petSkillUpView["mcMoney_"+j], petSkillUpView["mcMoney_"+j].txtMoney, true);
//				petSkillUpView["txtGoodNamePet_"+j].mouseEnabled = false;
//				petSkillUpView["mcMoney_"+j].mouseEnabled = false;
//				petSkillUpView["btnBuy_"+j].addEventListener(MouseEvent.CLICK, buyHandler);
//			}
			//
			///////////////////////技能格子
			for(var i:int = 0; i < 12; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6) + 28;
				gridUnit.y = (gridUnit.height) * int(i/6) + 182;
				gridUnit.name = "petSkillUpShow_" + i.toString();
				petSkillUpView.addChild(gridUnit);						//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = petSkillUpView;						//设置父级
				gridUint.Index = i;										//格子的位置
				gridUint.HasBag = true;									//是否是可用的背包
				gridUint.IsUsed	= false;								//是否已经使用
				gridUint.Item	= null;									//格子的物品
				PetSkillUpConstData.GridUnitList.push(gridUint);
			}
			
			
			
			////////////////////////选中技能格子
//			var gridSkillSelectedUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//			gridSkillSelectedUnit.x = 34;
//			gridSkillSelectedUnit.y = 233;
//			gridSkillSelectedUnit.name = "petSkillToUp";
//			petSkillUpView.addChild(gridSkillSelectedUnit);										//添加到画面
//			PetSkillUpConstData.GridSkillUnit = new GridUnit(gridSkillSelectedUnit, true);
//			PetSkillUpConstData.GridSkillUnit.parent = petSkillUpView;					//设置父级
//			PetSkillUpConstData.GridSkillUnit.Index = 0;									//格子的位置
//			PetSkillUpConstData.GridSkillUnit.HasBag  = true;								//是否是可用的背包
//			PetSkillUpConstData.GridSkillUnit.IsUsed	= false;							//是否已经使用
//			PetSkillUpConstData.GridSkillUnit.Item	= null;								//格子的物品
//			////////////////////////技能书道具格子	
//			var gridItemUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//			gridItemUnit.x = 34;
//			gridItemUnit.y = 273;
//			gridItemUnit.name = "petSkillUpItem";
//			petSkillUpView.addChild(gridItemUnit);								//添加到画面
//			PetSkillUpConstData.GridItemUnit = new GridUnit(gridItemUnit, true);
//			PetSkillUpConstData.GridItemUnit.parent = petSkillUpView;			//设置父级
//			PetSkillUpConstData.GridItemUnit.Index = 0;							//格子的位置
//			PetSkillUpConstData.GridItemUnit.HasBag  = true;					//是否是可用的背包
//			PetSkillUpConstData.GridItemUnit.IsUsed	= false;					//是否已经使用
//			PetSkillUpConstData.GridItemUnit.Item	= null;						//格子的物品
			///////////////////////
			gridManager = new PetSkillUpGridManager(PetSkillUpConstData.GridUnitList, petSkillUpView);
			facade.registerProxy(gridManager);
		}
		
		private function showDrug(type:uint):void
		{
			removeDrug();
			
			var itemData:Object = UIConstData.getItem(type);
			var useItem:UseItem = new UseItem(0, type.toString(), petSkillUpView);
			useItem.Num = 1;
			useItem.x = 269;
			useItem.y = 191;
			useItem.Id = 0;
			useItem.IsBind = 0;
			useItem.Type = type;
			
			var item:Object = BagData.getItemByType(type);
			if(item == null) {
				useItem.Enabled = false;
				// 背包中没有  请购买
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_psu_med_pets_sho_1" ] + itemData.Name+"，"+GameCommonData.wordDic[ "mod_pet_psu_med_pets_sho_2" ], color:0xffff00});
			}
			
			useItem.name = "goodQuickBuy_"+type;//petSkillItem_
			useItem.mouseEnabled = true;
			PetSkillUpConstData.drugItem = useItem;
			petSkillUpView.addChild(useItem);
			petSkillUpView.txtDragName.text = itemData.Name;
		}
		
		public function removeDrug():void
		{
			if(PetSkillUpConstData.drugItem && petSkillUpView.contains(PetSkillUpConstData.drugItem)) {
				petSkillUpView.removeChild(PetSkillUpConstData.drugItem);
				PetSkillUpConstData.drugItem = null;
			}
			petSkillUpView.txtDragName.text = "";
		}
		
		private function updateItem():void
		{
			if(PetSkillUpConstData.drugItem) {
				var type:int = (PetSkillUpConstData.drugItem as UseItem).Type;
				if(BagData.getItemByType(type) != null) {
					(PetSkillUpConstData.drugItem as UseItem).Enabled = true;
				} else {
					(PetSkillUpConstData.drugItem as UseItem).Enabled = false;
				}
			}
		}
		
//		private function buyHandler(e:MouseEvent):void
//		{
//			var index:uint = uint(String(e.target.name).split("_")[1]);
//			for(var i:int = 0; i < petSkillUpView.numChildren; i++) {
//				if(petSkillUpView.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
//					var type:uint = uint(petSkillUpView.getChildAt(i).name.split("_")[1]);
//					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
//				}
//			}
//		}
		
		private function addLis():void
		{
			petSkillUpView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillUpView.btnQuickBuy.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillUpView.mcPhotoPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillUpView.btnPanelCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillUpView.btnToMarket.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private  function removeLis():void
		{
			petSkillUpView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillUpView.btnQuickBuy.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillUpView.mcPhotoPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			
//			petSkillUpView.btnPanelCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillUpView.btnToMarket.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private function closeHandler(e:Event):void
		{
			gcAll();	
		}
		
		private  function gcAll():void
		{
			removeDrug();
			removeLis();
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
			if(PetSkillUpConstData.itemData) {
				sendNotification(EventList.BAGITEMUNLOCK, PetSkillUpConstData.itemData.id);
			}
//			dataProxy.PetCanOperate = true;
//			dataProxy.PetQuerySkill = false;
			viewComponent = null;
			panelBase = null;
			
			PetSkillUpConstData.GridItemUnit = null;
			PetSkillUpConstData.GridSkillUnit = null;
			PetSkillUpConstData.GridUnitList = [];
			PetSkillUpConstData.skillDataList = [];
			PetSkillUpConstData.itemData = null;
			PetSkillUpConstData.petSelected = null;
			PetSkillUpConstData.SelectedItem = null; 
			PetSkillUpConstData.skillDataSelect = null;
			PetSkillUpConstData.TmpIndex = 0;
//			PetSkillUpConstData.breedCost = 0;
			
			gridManager = null;
			uiManager = null;
			panelBase = null;
			listView = null;
			iScrollPane = null;
			
			dataProxy.PetSkillUpIsOpen = false;
			dataProxy = null;
			facade.removeProxy(PetSkillUpGridManager.NAME);
			facade.removeMediator(PetSkillUpMediator.NAME);
		}
		
		/** 点击按钮 确定、取消 */
		private  function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":
					sureToUpSkill();
					break;
				case "btnQuickBuy":
					buyItem();
					break;
				case "mcPhotoPet":
					removePetSelected();
					break;
			}
		}
		
		private function removePetSelected():void
		{
			if(PetRuleCommonData.selectedPet) {
				PetRuleCommonData.selectedPet = null;
				removeDrug();
				uiManager.clearData();
				gridManager.removeAllItem();
				PetSkillUpConstData.skillDataList = [];
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, 0);
				sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
			}
		}
		
		/** 快速买丹 */
		private function buyItem(learnAfterBuy:Boolean=false):void
		{
			if(PetSkillUpConstData.drugItem) {
				var type:uint = PetSkillUpConstData.drugItem.Type;
				
				var len:int = (UIConstData.MarketGoodList[4] as Array).length;
				var cost:Number = 0;
				
				for(var j:int = 0; j < len; j++) {	//商品总价
					var good:Object = (UIConstData.MarketGoodList[4] as Array)[j];
					if(type == good.type) {
						cost = good.PriceIn;
						break;
					}
				}
				if(learnAfterBuy) {
					sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:2, cost:cost, num:1, learn:0, up:1, id:PetRuleCommonData.selectedPet.Id});
				} else {
					sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:2, cost:cost, num:1, learn:0, up:0});
				}
//				sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
			}
		}
		
		/** 确定学习技能 */
		private function sureToUpSkill():void
		{
			if(!PetRuleCommonData.selectedPet) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});  // 请先选择宠物
				return;
			}
			if(!PetSkillUpConstData.SelectedItem) {
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_psu_med_pets_sur_1" ], color:0xffff00 } );   // 请选择要提升的技能
				return;
			}
			if(PetSkillUpConstData.drugItem) {
				var item:Object = BagData.getItemByType(PetSkillUpConstData.drugItem.Type);
				if(item == null) {
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"背包中没有相应灵兽丹，请购买", color:0xffff00});
					//询问是否购买，购买后直接提升
					buyItem(true);
					//
					return;
				}
			}
			if(PetSkillUpConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});   // 没有足够的银两
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id]) {
				//发送提升技能命令  带 宠物ID，灵兽丹道具ID
				PetNetAction.petBreed(PlayerAction.PET_SKILL_UP, PetRuleCommonData.selectedPet.Id, PetSkillUpConstData.SelectedItem.Item.Id, 0);//PetNetAction.petBreed(PlayerAction.PET_SKILL_UP, PetSkillUpConstData.petSelected.Id, PetSkillUpConstData.SelectedItem.Item.Id, 0, PetSkillUpConstData.itemData.id);
//				gridManager.removeItem();
				PetSkillUpConstData.SelectedItem = null;
				petSkillUpView.btnSure.mouseEnabled = false;
//				setTimeout(canOp, 1500);
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			}
		}
		
		private function canOp():void
		{
			if(petSkillUpView) petSkillUpView.btnSure.mouseEnabled = true;
		}
		
		/** 添加技能书道具到格子 */
		private function addToUpItem(o:Object):void {
			if(!o) return;
//			if(o.type != 630004) {	//灵兽丹ID
//				sendNotification(EventList.BAGITEMUNLOCK, o.id);
//				return;
//			}
			if(!PetSkillUpConstData.petSelected) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});  // 请先选择宠物
				sendNotification(EventList.BAGITEMUNLOCK, o.id);
				return;
			}
			if(!PetSkillUpConstData.SelectedItem) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psu_med_pets_sur_1" ], color:0xffff00});  // 请先选择要提升的技能
				sendNotification(EventList.BAGITEMUNLOCK, o.id);
				return;
			}
//////////////////////////////////
//这里判断灵兽丹等级
//			if(灵兽丹等级不符) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"灵兽丹等级不符", color:0xffff00});
//				sendNotification(EventList.BAGITEMUNLOCK, o.id);
//				return;
//			}
//////////////////////////////////
			if(PetSkillUpConstData.itemData) {
				gridManager.removeItem();
			}
			PetSkillUpConstData.itemData = o;
			gridManager.addItem();
		}
		
		
		
		
	}
}
