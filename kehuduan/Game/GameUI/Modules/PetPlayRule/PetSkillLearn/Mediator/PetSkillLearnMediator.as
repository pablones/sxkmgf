package GameUI.Modules.PetPlayRule.PetSkillLearn.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
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
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnEvent;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Proxy.PetSkillLearnGridManager;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.UI.PetSkillLearnUIManager;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.items.SkillItem;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.OperatorItemSend;
	import Net.Protocol;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetSkillLearnMediator extends Mediator
	{
		public static const NAME:String = "petSkillLearnMediator";
		private var dataProxy:DataProxy = null;
		private var gridManager:PetSkillLearnGridManager = null;
		private var uiManager:PetSkillLearnUIManager = null;
		
		private var panelBase:PanelBase = null;
		private var listView:ListComponent = null;
		private var iScrollPane:UIScrollPane = null;
		private var listBask:MovieClip = null;
		
		
		public function PetSkillLearnMediator()
		{
			super(NAME);
		}
		
		private function get petSkillLearnView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				PetSkillLearnEvent.SHOW_PETSKILLLEARN_VIEW,
//				PetSkillLearnEvent.BAG_TO_PETSKILLLEARN,
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
				PetEvent.RETURN_TO_SHOW_PET_INFO,
//				EventList.CLOSE_NPC_ALL_PANEL,
				PetSkillLearnEvent.FORGET_SKILL_PET,
				PetRuleEvent.UPDATE_ITEMS_PET_RULE,
				PetRuleEvent.SELECT_ITEM_TO_SKILL_LEARN_PET_RULE,
				PetRuleEvent.NOTICE_LEARN_SKILL_PET_RULE,
				PetSkillLearnEvent.LEARN_SKILL_SUCCESS_PET
				
//				EventList.UPDATEMONEY,
//				BagEvents.EXTENDBAG						//宠物背包扩充
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:	//PetSkillLearnEvent.SHOW_PETSKILLLEARN_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetSkillLearnIsOpen) {
						initView();
						addLis();
//						dataProxy.PetQuerySkill = true;
						dataProxy.PetSkillLearnIsOpen = true;
					}
					break;
//				case PetSkillLearnEvent.BAG_TO_PETSKILLLEARN:
//					var itemToLearn:Object = notification.getBody();
//					addToLearnItem(itemToLearn);
//					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:		//EventList.CLOSE_PET_PLAYRULE_VIEW:
					closeHandler(null);
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetSkillLearnIsOpen) closeHandler(null);	
//					break;
				case PetEvent.RETURN_TO_SHOW_PET_INFO:
					var info:GamePetRole = notification.getBody() as GamePetRole;
					if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == info.Id && dataProxy.PetQuerySkill) {
						dataProxy.PetQuerySkill = false;
						PetRuleCommonData.selectedPet = info;
						uiManager.showData();
						showSkillInfo(info);
					}
					break;
//				case BagEvents.EXTENDBAG:						//宠物背包扩充
//					if(dataProxy && dataProxy.PetSkillLearnIsOpen) {
//						uiManager.updatePetNum();
//					}
//					break;
				case PetSkillLearnEvent.FORGET_SKILL_PET:		//遗忘技能
					forgetSkill(notification.getBody());
					break;
				case PetSkillLearnEvent.LEARN_SKILL_SUCCESS_PET:	//学习技能成功
					var petInfo:Object = notification.getBody();
					updateCurPet(petInfo);
					break;
				case PetRuleEvent.SELECT_PET_PET_RULE:
					selectItem(notification.getBody() as uint);
					break;
				case PetRuleEvent.UPDATE_ITEMS_PET_RULE:
					updateItem();
					break;
				case PetRuleEvent.SELECT_ITEM_TO_SKILL_LEARN_PET_RULE:
					choseItem(notification.getBody());
					break;
				case PetRuleEvent.NOTICE_LEARN_SKILL_PET_RULE:
					var petId:uint = notification.getBody().id;
					if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petId) {
						sureToLearnSkill();
					}
					break;
//				case EventList.UPDATEMONEY:
//					uiManager.showMoney(1);
//					uiManager.showMoney(2);
//					break;
			}
		}
		
		private function forgetSkill(info:Object):void
		{
			var petId:uint   = info.petId;
			var skillId:uint = info.skillId;
			if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petId && skillId > 0) {
				removeItem();
				var gridItemUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				
				var type:uint = PetSkillLearnConstData.getSkillData(skillId).itemType;
				var useItem:UseItem = new UseItem(0, type.toString(), gridItemUnit);
				useItem.Num = 1; 
				useItem.x = 2;
				useItem.y = 2;
				
				useItem.Id = skillId;
				useItem.Type = type;
				
				if(BagData.getItemByType(type) == null) {
					useItem.Enabled = false;
				}
				
				useItem.IsLock = false;
				
				gridItemUnit.x = 266;
				gridItemUnit.y = 185;
				gridItemUnit.name = "goodQuickBuy_"+type;
				
				gridItemUnit.addChild(useItem);
				petSkillLearnView.addChild(gridItemUnit);
				var color:String = IntroConst.itemColors[UIConstData.getItem(type).Color];
				var name:String  = PetSkillLearnConstData.getSkillData(skillId).name; 
				petSkillLearnView.txtSkillName.htmlText = "<font color='"+color+"'>"+name+"</font>";
				
				PetSkillLearnConstData.GridItemUnit = gridItemUnit;
			}
		}
		
		private function updateCurPet(petInfo:Object):void
		{
			var petId:uint = petInfo.id;
			var flag:uint  = petInfo.flag;
			if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petId && flag > 0) {
				gridManager.removeAllItem();
//				gridManager.removeItem();
				PetSkillLearnConstData.skillDataList = [];
				//更换宠物数据（界面数据，技能格子数据）
//				uiManager.clearData();
//				uiManager.showData();
				//更换界面数据 技能格子数据
				dataProxy.PetEudemonTmp = PetSkillLearnConstData.petSelected;
				dataProxy.PetQuerySkill = true;
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:petId, ownerId:GameCommonData.Player.Role.Id});
			} else {
				PetSkillLearnConstData.skillForgeted = 0;
			}
			PetSkillLearnConstData.NewSelectGrid = null;
			petSkillLearnView.btnSure.mouseEnabled = true;
		}
		
		private function initView():void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetSkillLearn");
			listBask = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ListBack");
			listBask.width  = 77;
			listBask.height = 157;
			listBask.x = 245;
			listBask.y = 251;
//			panelBase = new PanelBase(petSkillLearnView, petSkillLearnView.width+8, petSkillLearnView.height+11);
//			panelBase.name = "PetSkillLearn";
//			panelBase.addEventListener(Event.CLOSE, closeHandler);
//			panelBase.x = UIConstData.DefaultPos1.x;
//			panelBase.y = UIConstData.DefaultPos1.y;
//			panelBase.SetTitleTxt("宠物技能学习");
			
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petSkillLearnView);
			
			petCon.subView = petSkillLearnView;
			
			uiManager = new PetSkillLearnUIManager(petSkillLearnView);
			
			initGrid();
			
//			initPetChoiceListData();
//			initPetChoiceListData();
//			
//			GameCommonData.GameInstance.GameUI.addChild(panelBase);
		}
		
		/** 初始化技能选择列表数据 */
		private function initPetChoiceListData():void
		{
			if(iScrollPane && petSkillLearnView.contains(iScrollPane)) {
				petSkillLearnView.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			listView = new ListComponent(false);
			showFilterList();
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = 246;
			iScrollPane.y = 252;
			iScrollPane.width = 76;
			iScrollPane.height = 154;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			petSkillLearnView.addChild(listBask);
			petSkillLearnView.addChild(iScrollPane);
		}
//		
		private function showFilterList():void
		{
			var len:int = PetSkillLearnConstData.SKILL_DATA_PET.length
			for(var i:int = 0; i < len; i++) {
//				if(PetSkillLearnConstData.SKILL_DATA_PET[id].getType == 1) continue;   //只能通过野外掉落
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petSkillDataList_"+PetSkillLearnConstData.SKILL_DATA_PET[i].id;
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnabled = false;
//				item.width = 104;
//				item.btnChosePet.width = 104;
//				item.mcSelected.width = 104;
				item.txtName.mouseEnabled = false;
//				item.doubleClickEnabled = true;
//				item.btnChosePet.mouseEnabled = false;
				var color:String = IntroConst.itemColors[UIConstData.getItem(PetSkillLearnConstData.SKILL_DATA_PET[i].itemType).Color];
				item.txtName.htmlText = "<font color='"+color+"'>"+PetSkillLearnConstData.SKILL_DATA_PET[i].name+"</font>";
				item.addEventListener(MouseEvent.MOUSE_DOWN, selectSkillData);
//				item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
				listView.addChild(item);
			}
			
//			
//			for(var id:Object in PetSkillLearnConstData.SKILL_DATA_PET)
//			{
//			}
			listView.width = 110;
			listView.upDataPos();
		}
		
		private function selectSkillData(e:MouseEvent):void
		{
			var item:MovieClip = e.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			addItem(id);
		}
		
		private function choseItem(data:Object):void
		{
			var id:uint = data.id;
			var detailData:Object = data.detailData;
			
			var type:uint   = detailData.type;
			var skillData:Object = PetSkillLearnConstData.getSkillDataByType(type);
			var skillId:int = (skillData == null) ? 0 : skillData.id;
			
			removeItem();
			var gridItemUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			var useItem:UseItem = new UseItem(0, type.toString(), gridItemUnit);
			useItem.Num = 1; 
			useItem.x = 2;
			useItem.y = 2;
			useItem.Id = skillId;
			useItem.Type = type;
			
			if(BagData.getItemByType(type) == null) {
				useItem.Enabled = false;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_cho_1" ], color:0xffff00}); // 背包中没有该技能书
			} else if(!checkHasSkill(skillId)) {
				useItem.Enabled = false;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_cho_2" ], color:0xffff00});  // 宠物已有该技能
			}
			
			useItem.IsLock = false;
			
			gridItemUnit.x = 266;
			gridItemUnit.y = 185;
			gridItemUnit.name = "goodQuickBuy_"+type;
			
			gridItemUnit.addChild(useItem);
			petSkillLearnView.addChild(gridItemUnit);
			var color:String = IntroConst.itemColors[UIConstData.getItem(type).Color];
			var name:String  = PetSkillLearnConstData.getSkillData(skillId).name; 
			petSkillLearnView.txtSkillName.htmlText = "<font color='"+color+"'>"+name+"</font>";
			
			PetSkillLearnConstData.GridItemUnit = gridItemUnit;
		}
		
		private function addItem(skillId:uint):void
		{
			removeItem();
			var gridItemUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			
			var type:uint = PetSkillLearnConstData.getSkillData(skillId).itemType;
			var useItem:UseItem = new UseItem(0, type.toString(), gridItemUnit);
			useItem.Num = 1; 
			useItem.x = 2;
			useItem.y = 2;
			
			useItem.Id = skillId;
			useItem.Type = type;
			
			if(BagData.getItemByType(type) == null) {
				useItem.Enabled = false;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_cho_1" ], color:0xffff00});  // 背包中没有该技能书
			} else if(!checkHasSkill(skillId)) {
				useItem.Enabled = false;
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_cho_2" ], color:0xffff00});   // 宠物已有该技能
			}
			
			useItem.IsLock = false;
			
			gridItemUnit.x = 266;
			gridItemUnit.y = 185;
			gridItemUnit.name = "goodQuickBuy_"+type;
			
			gridItemUnit.addChild(useItem);
			petSkillLearnView.addChild(gridItemUnit);
			var color:String = IntroConst.itemColors[UIConstData.getItem(type).Color];
			var name:String  = PetSkillLearnConstData.getSkillData(skillId).name; 
			petSkillLearnView.txtSkillName.htmlText = "<font color='"+color+"'>"+name+"</font>";
			
			PetSkillLearnConstData.GridItemUnit = gridItemUnit;
		}
		
		private function removeItem():void
		{
			if(PetSkillLearnConstData.GridItemUnit && petSkillLearnView.contains(PetSkillLearnConstData.GridItemUnit)) {
				petSkillLearnView.removeChild(PetSkillLearnConstData.GridItemUnit);
				PetSkillLearnConstData.GridItemUnit = null;
			}
			petSkillLearnView.txtSkillName.text = GameCommonData.wordDic[ "mod_pet_psl_med_pets_rem_1" ];    // 选择技能
		}
		
		private function updateItem():void
		{
			if(PetSkillLearnConstData.GridItemUnit) {
				var type:int = int(PetSkillLearnConstData.GridItemUnit.name.split("_")[1]);
				var len:int = PetSkillLearnConstData.GridItemUnit.numChildren;
				for(var i:int = 0; i < len; i++) {
					if(PetSkillLearnConstData.GridItemUnit.getChildAt(i) is UseItem) {
						if(BagData.getItemByType(type) != null) {
							(PetSkillLearnConstData.GridItemUnit.getChildAt(i) as UseItem).Enabled = true;
						} else {
							(PetSkillLearnConstData.GridItemUnit.getChildAt(i) as UseItem).Enabled = false;
						}
						break;
					}
				}
			}
		}
		
		private function checkHasSkill(skillId:int):Boolean
		{
			var res:Boolean = true;
			for(var i:int = 0; i < PetSkillLearnConstData.skillDataList.length; i++) {
				if(!PetSkillLearnConstData.skillDataList[i]) continue;
				if(skillId == PetSkillLearnConstData.skillDataList[i].gameSkill.SkillID) {
					res = false;
					break;
				}
			}
			return res;
		}
		
		private function selectItem(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id].State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_sel_1" ], color:0xffff00});   // 出战状态的宠物不能学习技能
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[id].State == 4)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
				return;
			}
			PetRuleCommonData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
			sendNotification(PetRuleEvent.SELECT_PET_RETURN, id);
			sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetSkillLearnConstData.breedCost);
			uiManager.clearData();
			gridManager.removeAllItem();
			removeItem();
//			gridManager.removeItem();
			PetSkillLearnConstData.skillDataList = [];
			PetSkillLearnConstData.skillForgeted = 0;
			//更换宠物数据（界面数据，技能格子数据）
//			uiManager.showData();
			//更换界面数据 技能格子数据
			dataProxy.PetEudemonTmp = PetRuleCommonData.selectedPet;
			dataProxy.PetQuerySkill = true;
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
		}
		
		/** 显示技能信息 */
		private function showSkillInfo(pet:GamePetRole):void
		{
			PetSkillLearnConstData.skillDataList = pet.SkillLevel;
			if(PetSkillLearnConstData.skillDataList.length > 0) {
				gridManager.showItems(PetSkillLearnConstData.skillDataList);
			}
		}
		
//		/** 双击查看宠物属性 */
//		private function lookPetInfoHandler(e:MouseEvent):void
//		{
//			var item:MovieClip = e.currentTarget as MovieClip;
//			var id:uint = uint(item.name.split("_")[1]);
//			if(id > 0) {
//				dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
//				dataProxy.PetQuerySkill = false;
//				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
//			}
//		}
		
		private function initGrid():void
		{
			//快速购买
//			for(var j:int = 0; j < 3; j++) {
//				if(!(UIConstData.MarketGoodList[21] as Array)[j]) continue;
//				var good:Object = (UIConstData.MarketGoodList[21] as Array)[j];
//				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//				unit.x = 243;
//				unit.y = j * (70 + unit.height) + 12;
//				unit.name = "goodQuickBuy"+j+"_"+good.type;
//				petSkillLearnView.addChild(unit);
//				
//				var useItem:UseItem = new UseItem(j, good.type, petSkillLearnView);
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
//				petSkillLearnView.addChild(useItem);
//				
//				petSkillLearnView["txtGoodNamePet_"+j].text = good.Name;
//				petSkillLearnView["mcMoney_"+j].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
//				ShowMoney.ShowIcon(petSkillLearnView["mcMoney_"+j], petSkillLearnView["mcMoney_"+j].txtMoney, true);
//				petSkillLearnView["txtGoodNamePet_"+j].mouseEnabled = false;
//				petSkillLearnView["mcMoney_"+j].mouseEnabled = false;
//				petSkillLearnView["btnBuy_"+j].addEventListener(MouseEvent.CLICK, buyHandler);
//			}
			//
			///////////////////////技能格子
			for(var i:int = 0; i < 12; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6) + 22;
				gridUnit.y = (gridUnit.height) * int(i/6) + 193;
				gridUnit.name = "petSkillLearnShow_" + i.toString();
				petSkillLearnView.addChild(gridUnit);						//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = petSkillLearnView;						//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				PetSkillLearnConstData.GridUnitList.push(gridUint);
			}
//			////////////////////////道具格子
//			var gridItemUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//			gridItemUnit.x = 269;
//			gridItemUnit.y = 188;
//			gridItemUnit.name = "petSkillLearnItem";
//			petSkillLearnView.addChild(gridItemUnit);								//添加到画面
//			PetSkillLearnConstData.GridItemUnit = new GridUnit(gridItemUnit, true);
//			PetSkillLearnConstData.GridItemUnit.parent = petSkillLearnView;			//设置父级
//			PetSkillLearnConstData.GridItemUnit.Index = 0;							//格子的位置
//			PetSkillLearnConstData.GridItemUnit.HasBag  = true;						//是否是可用的背包
//			PetSkillLearnConstData.GridItemUnit.IsUsed	= false;					//是否已经使用
//			PetSkillLearnConstData.GridItemUnit.Item	= null;						//格子的物品
//			///////////////////////
			gridManager = new PetSkillLearnGridManager(PetSkillLearnConstData.GridUnitList, petSkillLearnView);
			facade.registerProxy(gridManager);
		}
		
//		private function buyHandler(e:MouseEvent):void
//		{
//			var index:uint = uint(String(e.target.name).split("_")[1]);
//			for(var i:int = 0; i < petSkillLearnView.numChildren; i++) {
//				if(petSkillLearnView.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
//					var type:uint = uint(petSkillLearnView.getChildAt(i).name.split("_")[1]);
//					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});
//				}
//			}
//		}
		
		private function addLis():void
		{
			petSkillLearnView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillLearnView.btnShowList.addEventListener(MouseEvent.MOUSE_DOWN, btnClickHandler);
			petSkillLearnView.btnQuickBuy.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillLearnView.stage.addEventListener(MouseEvent.MOUSE_DOWN, removeSkillList);
			petSkillLearnView.mcPhotoPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillLearnView.btnPanelCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillLearnView.btnToMarket.addEventListener(MouseEvent.CLICK, btnClickHandler); 
		}
		
		private  function removeLis():void
		{
			petSkillLearnView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillLearnView.btnShowList.removeEventListener(MouseEvent.MOUSE_DOWN, btnClickHandler);
			petSkillLearnView.btnQuickBuy.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petSkillLearnView.stage.removeEventListener(MouseEvent.MOUSE_DOWN, removeSkillList);
			petSkillLearnView.mcPhotoPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillLearnView.btnPanelCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petSkillLearnView.btnToMarket.removeEventListener(MouseEvent.CLICK, btnClickHandler);
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
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
//			if(PetSkillLearnConstData.itemData) {
//				sendNotification(EventList.BAGITEMUNLOCK, PetSkillLearnConstData.itemData.id);
//			}
			dataProxy.PetQuerySkill = false;
//			dataProxy.PetCanOperate = true;
			viewComponent = null;
			panelBase = null;
			PetSkillLearnConstData.GridItemUnit = null;
			PetSkillLearnConstData.itemData = null;
			PetSkillLearnConstData.GridUnitList = [];
			PetSkillLearnConstData.skillDataList = [];
			PetSkillLearnConstData.skillForgeted = 0;
			PetSkillLearnConstData.petSelected = null;
			PetSkillLearnConstData.SelectedItem = null;
			PetSkillLearnConstData.TmpIndex = 0;
			
			gridManager = null;
			uiManager = null;
			panelBase = null;
			listView = null;
			iScrollPane = null;
			
			dataProxy.PetSkillLearnIsOpen = false;
			dataProxy = null;
			facade.removeProxy(PetSkillLearnGridManager.NAME);
			facade.removeMediator(PetSkillLearnMediator.NAME);
		}
		
		/** 点击按钮 确定、取消 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":
					sureToLearnSkill();
					break;
				case "btnShowList":
					showSkillList();
					e.stopPropagation();
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
				removeItem();
				uiManager.clearData();
				gridManager.removeAllItem();
				PetSkillLearnConstData.skillDataList = [];
				sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, 0);
				sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
			}
		}
		
		private function buyItem(learnAfterBuy:Boolean=false):void
		{
			if(PetSkillLearnConstData.GridItemUnit) {
				for(var i:int = 0; i < PetSkillLearnConstData.GridItemUnit.numChildren; i++) {
					if(PetSkillLearnConstData.GridItemUnit.getChildAt(i) is UseItem) {
						var skillId:int = (PetSkillLearnConstData.GridItemUnit.getChildAt(i) as UseItem).Id;
						if(PetSkillLearnConstData.getSkillData(skillId).getType == 1) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_buy_1" ], color:0xffff00});  // 该技能书只能通过师门除叛获得
							return;
						} else {
							var type:int = (PetSkillLearnConstData.GridItemUnit.getChildAt(i) as UseItem).Type;
							var buyType:int = PetSkillLearnConstData.getSkillData(skillId).buyType;
							if(buyType == 0) {			//元宝商品
//								sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type});

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
									sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:2, cost:cost, num:1, learn:1, up:0, id:PetRuleCommonData.selectedPet.Id});
								} else {
									sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:2, cost:cost, num:1, learn:0, up:0});
								}
							} else if(buyType == 1) {	//银两商品
								//buyUseUnbindMoney(type);
								var costYL:Number = UIConstData.getItem(type).PriceIn;
								if(learnAfterBuy) {
									sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:1, cost:costYL, num:1, learn:1, up:0, id:PetRuleCommonData.selectedPet.Id});
								} else {
									sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:1, cost:costYL, num:1, learn:0, up:0});
								}
							}
							break;
						}
					}
				}
			}
		}
		
		private function buyUseUnbindMoney(type:uint):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.BUYNPCITEM);
			obj.data.push(1);
			obj.data.push(1); 			//支付方式
			obj.data.push(610);		//NPC id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");			//归属人 name
			
			obj.data.push(0);			//物品id
			obj.data.push(type);
			obj.data.push(1);		//购买数量
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		/** 显示技能选项 */
		private function showSkillList():void
		{
			if(iScrollPane) {	
				if(petSkillLearnView.contains(iScrollPane)) {	//非空 移除
					petSkillLearnView.removeChild(iScrollPane);
					petSkillLearnView.removeChild(listBask);
				} else {										//非空  添加
					petSkillLearnView.addChild(listBask);
					petSkillLearnView.addChild(iScrollPane);
				}
			} else {											//空  初始化添加
				initPetChoiceListData();
			}
		}
		
		private function removeSkillList(e:MouseEvent):void
		{
			if(iScrollPane && petSkillLearnView.contains(iScrollPane)) {
				petSkillLearnView.removeChild(iScrollPane);
				petSkillLearnView.removeChild(listBask);
			}
		}
		
		/** 确定学习技能 */
		private function sureToLearnSkill():void
		{
			if(!PetRuleCommonData.selectedPet) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});  // 请先选择宠物
				return;
			}
//			if(!PetSkillLearnConstData.itemData) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入技能书", color:0xffff00});
//				return;
//			}
//////////////////////////////////
//这里判断宠物是否已有该技能   客户端无法判断（无法知道技能书物品对应的技能编号），只能有服务器判断
			if(!PetSkillLearnConstData.GridItemUnit ) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_sur_1" ], color:0xffff00});  // 请先选择要学习的技能
				return;
			}
			var skillId:int = 0;
			var itemType:int = 0;
			for(var i:int = 0; i < PetSkillLearnConstData.GridItemUnit.numChildren; i++) {
				if(PetSkillLearnConstData.GridItemUnit.getChildAt(i) is UseItem) {
					skillId  = (PetSkillLearnConstData.GridItemUnit.getChildAt(i) as UseItem).Id;
					itemType = (PetSkillLearnConstData.GridItemUnit.getChildAt(i) as UseItem).Type;
					break;
				}
			}
			if(!PetSkillLearnConstData.getSkillData(skillId)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_sur_2" ], color:0xffff00});  // 该技能不存在
				return;
			}
			if(!checkHasSkill(skillId)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_psl_med_pets_sur_3" ], color:0xffff00});  // 宠物已有该技能
				return;
			}
			var item:Object = BagData.getItemByType(itemType);
			if(item == null) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"背包中没有该技能书", color:0xffff00});
				//询问是否购买，购买后直接学习
				buyItem(true);
				//
				return;
			}
//////////////////////////////////
			if(PetSkillLearnConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});   // 没有足够的银两
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetRuleCommonData.selectedPet.Id]) {
				//发送学习技能命令  带宠物ID，技能书道具ID
				if(PetPropConstData.isNewPetVersion)
				{
					var OverrideSkillId:int;
					if(PetSkillLearnConstData.NewSelectGrid)
					{
						var OverideItem:Object = PetSkillLearnConstData.NewSelectGrid.Item;
						var gridNum:int = PetSkillLearnConstData.NewSelectGrid.Grid.name.split("_")[1];
						OverrideSkillId = OverideItem is SkillItem ? (OverideItem as SkillItem).Id:0;
						if(gridNum > 0)
						{
							var bookItem:Object = PetSkillLearnConstData.getSkillData(skillId);
							if(bookItem && bookItem.showType)
							{
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_petRule_word_1" ], color:0xffff00});//"该技能只能在宠物栏第一格学习"   
								return;
							}
						}
					}
					PetNetAction.petBreed(PlayerAction.PET_SKILL_LEARN, PetRuleCommonData.selectedPet.Id, OverrideSkillId,0, item.id);
				}
				else
				{
					PetNetAction.petBreed(PlayerAction.PET_SKILL_LEARN, PetRuleCommonData.selectedPet.Id, 0,0, item.id);
				}
//				gridManager.removeItem();
				petSkillLearnView.btnSure.mouseEnabled = false;
				var timeId:uint = setTimeout(canOp,500,timeId);
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gcAll();
			}
		}
		
		private function canOp(timeId:uint):void
		{
			clearTimeout(timeId);
			if(petSkillLearnView) petSkillLearnView.btnSure.mouseEnabled = true;
		}
		
		/** 添加技能书道具到格子 */
		private function addToLearnItem(o:Object):void {
			if(!o) return;
			if(!PetSkillLearnConstData.petSelected) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_loc_1" ], color:0xffff00});   // 请先选择宠物
				sendNotification(EventList.BAGITEMUNLOCK, o.id);
				return;
			}
			if(String(o.type).indexOf("640") != 0) {	//技能书ID
				sendNotification(EventList.BAGITEMUNLOCK, o.id);
				return;
			}
			if(PetSkillLearnConstData.itemData) {
				gridManager.removeItem();
			}
			PetSkillLearnConstData.itemData = o;
			gridManager.addItem();
		}
		
	}
}
