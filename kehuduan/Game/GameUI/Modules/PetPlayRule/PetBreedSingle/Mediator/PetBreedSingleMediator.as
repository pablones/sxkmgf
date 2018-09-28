package GameUI.Modules.PetPlayRule.PetBreedSingle.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleConstData;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleEvent;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Proxy.PetBreedSingleGirdManager;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.UI.PetBreedSingleUIManager;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
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

	public class PetBreedSingleMediator extends Mediator
	{
		public static const NAME:String = "petBreedSingleMediator";
		private var dataProxy:DataProxy = null;
		private var gridManager:PetBreedSingleGirdManager = null;
		private var uiManager:PetBreedSingleUIManager = null;
		
		private var panelBase:PanelBase = null;
		
		//宠物选择
		private var petChoiceView:MovieClip = null;
		private var petChoicePanelBase:PanelBase = null;
		private const PETCHOICEPANEL_POS:Point = new Point(450, 150);
		private var petChoiceOpen:Boolean = false;
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		//
		
		private function get petBreedSingleView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public function PetBreedSingleMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				PetBreedSingleEvent.SHOW_PETBREEDSINGLE_VIEW,
//				EventList.CLOSE_PET_PLAYRULE_VIEW,
				PetRuleEvent.INIT_SUB_VIEW_PET_RULE,
				PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE,
				PetRuleEvent.SELECT_PET_PET_RULE,
//				EventList.CLOSE_NPC_ALL_PANEL,
				PetBreedSingleEvent.PET_BREED_SINGLE_RETURN
//				PetBreedSingleEvent.BAG_TO_PETBREEDSINGLE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.INIT_SUB_VIEW_PET_RULE:		//SHOW_PETBREEDSINGLE_VIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.PetBreedSingleIsOpen) {
						initView();
						addLis();
						dataProxy.PetBreedSingleIsOpen = true;
					}
					break;
				case PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE:
					closeHandler(null);
					break;
//				case EventList.CLOSE_NPC_ALL_PANEL:
//					if(dataProxy && dataProxy.PetBreedSingleIsOpen) closeHandler(null);	
//					break;
//				case PetBreedSingleEvent.BAG_TO_PETBREEDSINGLE:
//					var item:Object = notification.getBody();
//					addBreedItem(item);
//					break;
				case PetRuleEvent.SELECT_PET_PET_RULE:
					selectPetToShow(notification.getBody() as uint);
					break;
				case PetBreedSingleEvent.PET_BREED_SINGLE_RETURN:
					var petSingleFlag:uint = uint(notification.getBody());
					updateCurPet(petSingleFlag);
					break;
			}
		}
		
		private function updateCurPet(flag:uint):void
		{
			if(flag == 0) {	//繁殖失败
				
			} else {		//繁殖成功
//				closeHandler(null);
				uiManager.clearData(0);
				uiManager.clearData(1);
				PetBreedSingleConstData.petMaleShow = null;
				PetBreedSingleConstData.petFemaleShow = null;
				sendNotification(PetRuleEvent.INIT_DATA_AFTER_OPERATE_PET_RULE);
			}
			petBreedSingleView.btnSure.mouseEnabled = true;
		}
		
		/** 显示界面 */
		private function initView():void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetBreedSingleView");
//			petChoiceView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetList");
			
//			panelBase = new PanelBase(petBreedSingleView, petBreedSingleView.width+6, petBreedSingleView.height+12);
//			panelBase.name = "PetBreedSingle";
//			panelBase.addEventListener(Event.CLOSE, closeHandler);
//			panelBase.x = UIConstData.DefaultPos1.x;
//			panelBase.y = UIConstData.DefaultPos1.y;
//			panelBase.SetTitleTxt("宠物单人繁殖");
			
			//宠物选择面板
//			petChoicePanelBase = new PanelBase(petChoiceView, petChoiceView.width-6, petChoiceView.height+11);
//			petChoicePanelBase.name = "PetBreedSingleChoice";
//			petChoicePanelBase.addEventListener(Event.CLOSE, choiceCloseHandler);
//			petChoicePanelBase.x = UIConstData.DefaultPos1.x;
//			petChoicePanelBase.y = UIConstData.DefaultPos1.y;
//			petChoicePanelBase.SetTitleTxt("宠物选择");
//			petChoiceView.txtCancel.mouseEnabled = false;
//			
//			listView = new ListComponent(false);
//			listView.Offset = 0;
//			showFilterList();
//			iScrollPane = new UIScrollPane(listView);
//			iScrollPane.x = 3;
//			iScrollPane.y = 3;
//			iScrollPane.width = 118;
//			iScrollPane.height = 135;
//			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
//			iScrollPane.refresh();
//			petChoiceView.addChild(iScrollPane);
			
			var petCon:PetRuleControlMediator = facade.retrieveMediator(PetRuleControlMediator.NAME) as PetRuleControlMediator;
			var parent:MovieClip = petCon.ruleBase;
			
			parent.addChild(petBreedSingleView);
			
			petCon.subView = petBreedSingleView;
			
			uiManager = new PetBreedSingleUIManager(petBreedSingleView);
			
			sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetBreedSingleConstData.breedCost);
			
//			initGrid();
			
//			GameCommonData.GameInstance.GameUI.addChild(panelBase);
//			
//			openChoicePanel();
		}
		
//		/** 初始化格子 */
//		private function initGrid():void
//		{
//			//快速购买
//			for(var j:int = 0; j < 1; j++) {
//				if(!(UIConstData.MarketGoodList[18] as Array)[j]) continue;
//				var good:Object = (UIConstData.MarketGoodList[18] as Array)[j];
//				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//				unit.x = 246;
//				unit.y = j * (70 + unit.height) + 13;
//				unit.name = "goodQuickBuy"+j+"_"+good.type;
//				petBreedSingleView.addChild(unit);
//				
//				var useItem:UseItem = new UseItem(j, good.type, petBreedSingleView);
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
//				petBreedSingleView.addChild(useItem);
//				
//				petBreedSingleView["txtGoodNamePet_"+j].text = good.Name;
//				petBreedSingleView["mcMoney_"+j].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
//				ShowMoney.ShowIcon(petBreedSingleView["mcMoney_"+j], petBreedSingleView["mcMoney_"+j].txtMoney, true);
//				petBreedSingleView["txtGoodNamePet_"+j].mouseEnabled = false;
//				petBreedSingleView["mcMoney_"+j].mouseEnabled = false;
//				petBreedSingleView["btnBuy_"+j].addEventListener(MouseEvent.CLICK, buyHandler);
//			}
			//
//			var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
//			gridUnit.x = 35;
//			gridUnit.y = 210;
//			gridUnit.name = "petBreedSingleItem";
//			petBreedSingleView.addChild(gridUnit);	//添加到画面
//			PetBreedSingleConstData.GridItemUnit = new GridUnit(gridUnit, true);
//			PetBreedSingleConstData.GridItemUnit.parent = petBreedSingleView;	//设置父级
//			PetBreedSingleConstData.GridItemUnit.Index = 0;						//格子的位置
//			PetBreedSingleConstData.GridItemUnit.HasBag = true;					//是否是可用的背包
//			PetBreedSingleConstData.GridItemUnit.IsUsed	= false;				//是否已经使用
//			PetBreedSingleConstData.GridItemUnit.Item	= null;					//格子的物品
//			
//			gridManager = new PetBreedSingleGirdManager(petBreedSingleView);
//			facade.registerProxy(gridManager);
//		}
		
//		private function buyHandler(e:MouseEvent):void
//		{
//			var index:uint = uint(String(e.target.name).split("_")[1]);
//			for(var i:int = 0; i < petBreedSingleView.numChildren; i++) {
//				if(petBreedSingleView.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
//					var type:uint = uint(petBreedSingleView.getChildAt(i).name.split("_")[1]);
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
//				item.name = "petBreedSingleChoice_"+id;
//				item.btnChosePet.mouseEnabled = true;
//				item.mcSelected.visible = false;
////				item.btnChosePet.width = 120;
////				item.mcSelected.width = 120;
//				item.mcSelected.mouseEnabled = false;
//				item.doubleClickEnabled = true;
//				item.txtName.mouseEnabled = false;
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
//			var id:uint = uint(item.name.split("_")[1]);
//			if(PetBreedSingleConstData.petSelected && id == PetBreedSingleConstData.petSelected.Id) return;
//			for(var i:int = 0; i < listView.numChildren; i++){
//				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
//			}
//			
//			item.mcSelected.visible = true;
//			if(GameCommonData.Player.Role.PetSnapList[id]) {
//				PetBreedSingleConstData.petSelected = GameCommonData.Player.Role.PetSnapList[id];
//			} else {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"该宠物不存在", color:0xffff00});
//				gcAll();
//			}
////			showPetData();
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
		
		/** 显示当前选择的宠物的数据 */
		private function showPetData(type:int):void
		{
			uiManager.addPetPhoto(type);
			uiManager.showData(type);
		}
		
//		/** 打开宠物选择界面 */
//		private function openChoicePanel():void
//		{
//			if(!petChoiceOpen && !GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase)) {
//				GameCommonData.GameInstance.GameUI.addChild(petChoicePanelBase);
//				petChoicePanelBase.x = PETCHOICEPANEL_POS.x;
//				petChoicePanelBase.y = PETCHOICEPANEL_POS.y;
//				petChoiceOpen = true;
//			} else if(petChoiceOpen && GameCommonData.GameInstance.GameUI.contains(petChoicePanelBase)) {
//				choiceCloseHandler(null);
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
			petBreedSingleView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.btnLookMale.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.btnLookFemale.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.mcPhotoMale.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.mcPhotoFemale.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petBreedSingleView.btnSelectPet.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petBreedSingleView.btnPanelCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petChoiceView.btnPetChose.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petChoiceView.btnCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		private  function removeLis():void
		{
			petBreedSingleView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.btnLookMale.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.btnLookFemale.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.mcPhotoMale.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petBreedSingleView.mcPhotoFemale.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petBreedSingleView.btnSelectPet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petBreedSingleView.btnPanelCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petChoiceView.btnPetChose.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petChoiceView.btnCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
		}
		
		
		private function closeHandler(e:Event):void
		{
			gcAll();
		}
		
		private function gcAll():void
		{
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
//			choiceCloseHandler(null);
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
//			dataProxy.PetCanOperate = true;
			if(PetBreedSingleConstData.itemData) {
				sendNotification(EventList.BAGITEMUNLOCK, PetBreedSingleConstData.itemData.id);
			}
			
//			gridManager = null;
			uiManager = null;
			panelBase = null;
			petChoiceView = null;
			petChoicePanelBase = null;
			listView = null;
			iScrollPane = null;
			
			
			PetBreedSingleConstData.itemData 	  = null;
			PetBreedSingleConstData.GridItemUnit  = null;
			PetBreedSingleConstData.petFemaleShow = null;
			PetBreedSingleConstData.petMaleShow   = null;
			PetBreedSingleConstData.petSelected   = null;
			
			dataProxy.PetBreedSingleIsOpen = false;
			dataProxy = null;
//			facade.removeProxy(PetBreedSingleGirdManager.NAME);
			facade.removeMediator(PetBreedSingleMediator.NAME);
		}
		
		/** 点击按钮 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnSure":
					sureToBreed();
					break;
				case "btnLookMale":
					lookPet(0);
					break;
				case "btnLookFemale":
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
				if(PetBreedSingleConstData.petMaleShow) {
					PetBreedSingleConstData.petMaleShow = null;
					uiManager.clearData(0);
					sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
				}
			} else {
				if(PetBreedSingleConstData.petFemaleShow) {
					PetBreedSingleConstData.petFemaleShow = null;
					uiManager.clearData(1);
					sendNotification(PetRuleEvent.CLEAR_PET_SELECT_PET_RULE);
				}
			}
		}
		
		/** 查看宠物详细属性 0-雄，1-雌 */
		private function lookPet(type:int):void
		{
			if(type == 0) {
				if(PetBreedSingleConstData.petMaleShow) {
					PetPropConstData.isSearchOtherPetInto = true;
					dataProxy.PetEudemonTmp = PetBreedSingleConstData.petMaleShow;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetBreedSingleConstData.petMaleShow.Id, ownerId:GameCommonData.Player.Role.Id});
				}
			} else {
				if(PetBreedSingleConstData.petFemaleShow) {
					PetPropConstData.isSearchOtherPetInto = true;
					dataProxy.PetEudemonTmp = PetBreedSingleConstData.petFemaleShow;
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:PetBreedSingleConstData.petFemaleShow.Id, ownerId:GameCommonData.Player.Role.Id});
				}
			}
		}
		
		/** 选择宠物显示到界面 */ 
		private function selectPetToShow(id:uint):void
		{
			if(GameCommonData.Player.Role.PetSnapList[id]) {
				if((GameCommonData.Player.Role.PetSnapList[id] as GamePetRole).isFantasy)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petBreedDouble_1"], color:0xffff00});  //  "已幻化的宠物不能繁殖"
					return;
				}
				if((GameCommonData.Player.Role.PetSnapList[id] as GamePetRole).State == 4)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_sho_1"], color:0xffff00});  //  附体状态的宠物无法进行此操作
					return;
				}
				if((PetBreedSingleConstData.petMaleShow && PetBreedSingleConstData.petMaleShow.Id == id) || (PetBreedSingleConstData.petFemaleShow && PetBreedSingleConstData.petFemaleShow.Id == id))
					return;
				if(GameCommonData.Player.Role.PetSnapList[id].Type != 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_1" ], color:0xffff00});    // 只有宝宝可以繁殖
					return;
				}
				if(GameCommonData.Player.Role.PetSnapList[id].BreedNow == GameCommonData.Player.Role.PetSnapList[id].BreedMax) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_2" ], color:0xffff00});   // 该宠物已达最大繁殖代
					return;
				}
				if(GameCommonData.Player.Role.PetSnapList[id].Level < (30 * (GameCommonData.Player.Role.PetSnapList[id].BreedNow+1))) {
					// 该宠物需   级以上才能继续繁殖
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_3" ]+(30 * (GameCommonData.Player.Role.PetSnapList[id].BreedNow+1))+GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_4" ], color:0xffff00});
					return;
				}
				if(GameCommonData.Player.Role.PetSnapList[id].State == 1) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_7" ], color:0xffff00});  // 出战状态的宠物不能繁殖
					return;
				}
				if(!GameCommonData.Player.Role.PetSnapList[id]) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
					gcAll();
					return;
				}
				if(GameCommonData.Player.Role.PetSnapList[id].Sex == 0) { //雄性
					PetBreedSingleConstData.petMaleShow = GameCommonData.Player.Role.PetSnapList[id];
					showPetData(0);
				} else {
					PetBreedSingleConstData.petFemaleShow = GameCommonData.Player.Role.PetSnapList[id];
					showPetData(1);
				}
			}
		}
		
		/** 确定繁殖 */
		private function sureToBreed():void
		{
			if(PetBreedSingleConstData.petMaleShow == null || PetBreedSingleConstData.petFemaleShow == null) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_petRule_petBreedDouble_2"], color:0xffff00});   // 繁殖需要两只宠物
				return;
			}
			if(PetBreedSingleConstData.petMaleShow.FaceType != PetBreedSingleConstData.petFemaleShow.FaceType) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_4" ], color:0xffff00});   // 同种类宠物才能繁殖
				return;
			}
			if(PetBreedSingleConstData.petMaleShow.State == 1 || PetBreedSingleConstData.petFemaleShow.State == 1) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_7" ], color:0xffff00});  // 出战状态的宠物不能繁殖
				return;
			}
			if(!checkItem()) return;
//			if(PetBreedSingleConstData.itemData == null) {
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请放入准生证", color:0xffff00});
//				return;
//			}
			if(PetBreedSingleConstData.breedCost > (GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_6" ], color:0xffff00});   // 没有足够的银两
				return;
			}
			if(GameCommonData.Player.Role.PetSnapList[PetBreedSingleConstData.petMaleShow.Id] && GameCommonData.Player.Role.PetSnapList[PetBreedSingleConstData.petFemaleShow.Id]) {
				//发送繁殖命令， 携带两个宠物ID，准生证道具ID
				PetNetAction.petBreed(PlayerAction.PET_SURE_BREED, PetBreedSingleConstData.petMaleShow.Id, PetBreedSingleConstData.petFemaleShow.Id, GameCommonData.Player.Role.Id);//PetNetAction.petBreed(PlayerAction.PET_SURE_BREED, PetBreedSingleConstData.petMaleShow.Id, PetBreedSingleConstData.petFemaleShow.Id, GameCommonData.Player.Role.Id, PetBreedSingleConstData.itemData.id.toString());
				petBreedSingleView.btnSure.mouseEnabled = false;
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_7" ], color:0xffff00});  // 宠物不存在
				gcAll();
			}
		}
		
		private function checkItem():Boolean
		{
			var result:Boolean = true;
			if(!BagData.isHasItem(630001)) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbs_med_petb_che_1" ], color:0xffff00});  // 准生证不足，请补充
				result = false;
			}
			return result;
		}
		
		/** 添加繁殖道具到格子 */
		private function addBreedItem(o:Object):void {
			if(!o) return;
			if(PetBreedSingleConstData.itemData || o.type != 630001) {//准生证type
				sendNotification(EventList.BAGITEMUNLOCK, o.id);
				return;
			}
			PetBreedSingleConstData.itemData = o;
			gridManager.addItem();
		}
		
		
		
	}
}