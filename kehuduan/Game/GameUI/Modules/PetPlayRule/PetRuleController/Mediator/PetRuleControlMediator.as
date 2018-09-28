package GameUI.Modules.PetPlayRule.PetRuleController.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Data.PetBreedDoubleConstData;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Mediator.PetBreedDoubleMediator;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Mediator.PetBreedSingleMediator;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Proxy.PetRuleGridManager;
	import GameUI.Modules.PetPlayRule.PetRuleController.UI.PetRuleDataGrid;
	import GameUI.Modules.PetPlayRule.PetRuleController.UI.PetRuleItemCell;
	import GameUI.Modules.PetPlayRule.PetRuleController.UI.PetRuleUIManager;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.Mediator.PetSavvyJoinMediator;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Mediator.PetSavvyUseMoneyMediator;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Mediator.PetSkillLearnMediator;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Mediator.PetSkillUpMediator;
	import GameUI.Modules.PetPlayRule.PetToBaby.Mediator.PetToBabyMediator;
	import GameUI.Modules.PetPlayRule.PetWinningUp.Mediator.PetWinningUpMediator;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.OperatorItemSend;
	import Net.Protocol;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetRuleControlMediator extends Mediator
	{
		public static const NAME:String = "PetRuleControlMediator";
		public var subView:MovieClip = null;
		
		public static var MAXPAGE:uint = 7;
		private var dataProxy:DataProxy = null;
		
		private var panelBase:PanelBase = null;
		private var gridManager:PetRuleGridManager = null;
		private var uiManager:PetRuleUIManager = null;
		
		private var listView:ListComponent = null;
		private var iScrollPane:UIScrollPane = null;
		
		private var buyItemList:Array = [];
		
		private var isLock:Boolean = false;
		
		private var clickTimer:Timer = new Timer(250, 1); 
		private var tmpIdClick:uint = 0;
		
		/** 格子面板 */
		private var itemDataGrid:PetRuleDataGrid = null;
		
		/** 购买信息 */
		private var buyInfo:Object = null;
		
		public function PetRuleControlMediator()
		{
			super(NAME);
		}
		
		public function get ruleBase():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				PetRuleEvent.SHOW_PET_RULE_BASE,
				EventList.CLOSE_PET_PLAYRULE_VIEW,
				EventList.CLOSE_NPC_ALL_PANEL,
				PetRuleEvent.SELECT_PET_RETURN,
				PetRuleEvent.SHOW_MONEY_BASE_PET_RULR,
				PetRuleEvent.UPDATE_PETNAME_LISTVIEW_PET_RULE,
				PetRuleEvent.INIT_DATA_AFTER_OPERATE_PET_RULE,
				PetEvent.PET_DELETE_SUCCESS,
				PetRuleEvent.UPDATE_ITEMS_PET_RULE,
				PetRuleEvent.CLEAR_PET_SELECT_PET_RULE,
				PetRuleEvent.ASK_TO_BUY_PET_RULE,
				EventList.UPDATEMONEY
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetRuleEvent.SHOW_PET_RULE_BASE:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(!dataProxy.petRuleIsOpen) {
						var opData:Object = notification.getBody();
						var opType:String = opData.type;
						var opIndex:int   = opData.index; 
						if(opType == UIConstData.PET_RULE_BASE) {				//基本玩法
							initPanelBase(opIndex);
						} else if(opType == UIConstData.PET_DOUBLE_BREED) {		//双繁
							initPanelBaseDoubleBreed();
						}
						dataProxy.petRuleIsOpen = true;
					}
					break;
				case EventList.CLOSE_PET_PLAYRULE_VIEW:
					gc();
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					gc();
					break;
				case EventList.UPDATEMONEY:
					uiManager.showMoney(1);
					uiManager.showMoney(2);
					break;
				case PetRuleEvent.SHOW_MONEY_BASE_PET_RULR:
					uiManager.showMoney(0, notification.getBody() as int);
					break;
				case PetRuleEvent.SELECT_PET_RETURN:
					selectPetSucess(notification.getBody() as int);
					break;
				case PetRuleEvent.UPDATE_PETNAME_LISTVIEW_PET_RULE:
					updatePetNameListVew(notification.getBody() as uint);
					break;
				case PetRuleEvent.INIT_DATA_AFTER_OPERATE_PET_RULE:
					PetRuleCommonData.selectedPet = null;
					initPetChoiceListData();
				case PetRuleEvent.UPDATE_ITEMS_PET_RULE:	
					updateItems();
					checkLearnAndUpSkill();
					break;
				case PetEvent.PET_DELETE_SUCCESS:
					PetRuleCommonData.selectedPet = null;
					initPetChoiceListData();
					break;
				case PetRuleEvent.CLEAR_PET_SELECT_PET_RULE:
					clearPetSelect();
					break;
				case PetRuleEvent.ASK_TO_BUY_PET_RULE:
					buyInfo = notification.getBody();
					askToBuy();
					break;
			}
		}
		
		private function initPanelBase(pageIndex:int=0):void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetRuleBase");
			panelBase = new PanelBase(ruleBase, ruleBase.width+6, ruleBase.height+11);
			slectViewState();
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ini_1" ] );   // 万兽谱
			panelBase.name = "PetRuleController";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;;
			}else{
				var pos:Point = UIUtils.getMiddlePos(panelBase); 
				panelBase.x = pos.x - 20;
				panelBase.y = pos.y;
			}
			
			itemDataGrid = new PetRuleDataGrid(128, 252);
			itemDataGrid.rendererClass = PetRuleItemCell;
			itemDataGrid.x = 329;
			itemDataGrid.y = 220;
			
			ruleBase.addChild(itemDataGrid);
			
			uiManager = new PetRuleUIManager(ruleBase);
			
			PetRuleCommonData.curPageIndex = pageIndex;
			
			isLock = true;
			
			for(var i:int = 0; i < MAXPAGE; i++) {
				(ruleBase["mcPage_"+i] as MovieClip).buttonMode = true;
				(ruleBase["mcPage_"+i] as MovieClip).useHandCursor = true;
				ruleBase["mcPage_"+i].gotoAndStop(2);
				ruleBase["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			ruleBase["mcPage_"+PetRuleCommonData.curPageIndex].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			ruleBase["mcPage_"+PetRuleCommonData.curPageIndex].gotoAndStop(1);
			
			initGrid();
			initPetChoiceListData();
			
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			
			showCurView();
			showCurData();
			
			
			addLis();
			
			isLock = false;
			
			//----------测试
//			var PET_SKILL_DATA:Array =[];
//
//			var o:Object ={};
//			
//
//			o = {id:7001, name:"冰魂",     itemType:640010, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7002, name:"高级冰魂", itemType:640011, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7003, name:"火魂",     itemType:640021, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7004, name:"高级火魂", itemType:640022, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7005, name:"毒魂",     itemType:640031, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7006, name:"高级毒魂", itemType:640032, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7007, name:"玄魂",     itemType:640041, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7008, name:"高级玄魂", itemType:640042, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7009, name:"迟钝",     itemType:640051, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7010, name:"高级迟钝", itemType:640052, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//						
//			o = {id:7011, name:"狡猾",     itemType:640061, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7012, name:"高级狡猾", itemType:640062, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7013, name:"憨厚",     itemType:640071, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7014, name:"高级憨厚", itemType:640072, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7015, name:"拼命",     itemType:640081, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7016, name:"高级拼命", itemType:640082, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7017, name:"法魂",     itemType:640091, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7018, name:"高级法魂", itemType:640092, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7019, name:"蛮力",     itemType:640101, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7020, name:"高级蛮力", itemType:640102, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//						
//			o = {id:7021, name:"借力",     itemType:640111, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7022, name:"高级借力", itemType:640112, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7023, name:"移魂",     itemType:640121, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7024, name:"高级移魂", itemType:640122, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7025, name:"瞬影",     itemType:640131, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7026, name:"高级瞬影", itemType:640132, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7027, name:"强身",     itemType:640141, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7028, name:"高级强身", itemType:640142, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7029, name:"凝神",     itemType:640151, getType:0, buyType:1, drugType:630015};
//			PET_SKILL_DATA.push(o);
//			o = {id:7030, name:"高级凝神", itemType:640152, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//						
//			o = {id:7031, name:"冰天雪地", itemType:640161, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7032, name:"烈火燎原", itemType:640171, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7033, name:"血毒万里", itemType:640181, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7034, name:"五雷轰顶", itemType:640191, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7035, name:"血爆",     itemType:640201, getType:0, buyType:0, drugType:630018};
//			PET_SKILL_DATA.push(o);
//			o = {id:7036, name:"高级血爆", itemType:640202, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7037, name:"圣爆",     itemType:640211, getType:0, buyType:0, drugType:630018};
//			PET_SKILL_DATA.push(o);
//			o = {id:8001, name:"吸血",     itemType:640221, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:8002, name:"高级吸血", itemType:640222, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//						
//			o = {id:7039, name:"寒冰咒",   itemType:640231, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:7040, name:"极冰咒",   itemType:640232, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7041, name:"烈火咒",   itemType:640241, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:7042, name:"冥火咒",   itemType:640242, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7043, name:"血毒咒",   itemType:640251, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:7044, name:"嗜毒咒",   itemType:640252, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7045, name:"玄雷咒",   itemType:640261, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:7046, name:"天雷咒",   itemType:640262, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7047, name:"猛击",     itemType:640291, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:7048, name:"高级猛击", itemType:640292, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//						
//			o = {id:8007, name:"连击",     itemType:640301, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:8008, name:"高级连击", itemType:640302, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7048, name:"痛击",     itemType:640311, getType:0, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:7050, name:"高级痛击", itemType:640312, getType:0, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//						
//			//只能通过打怪获得
//			o = {id:8003, name:"吸气",     itemType:640271, getType:1, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:8004, name:"高级吸气", itemType:640272, getType:1, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:8005, name:"打怒",     itemType:640281, getType:1, buyType:0, drugType:630020};
//			PET_SKILL_DATA.push(o);
//			o = {id:8006, name:"高级打怒", itemType:640282, getType:1, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			o = {id:7038, name:"高级圣爆", itemType:640212, getType:1, buyType:0, drugType:0};
//			PET_SKILL_DATA.push(o);
//			
//			
//			PetSkillLearnConstData.SKILL_DATA_PET = PET_SKILL_DATA;
			// 商店/商城有售
//			PetSkillLearnConstData.SKILL_DATA_PET[7001] = {name:"冰魂",     itemType:640010, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7002] = {name:"高级冰魂", itemType:640011, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7003] = {name:"火魂",     itemType:640021, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7004] = {name:"高级火魂", itemType:640022, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7005] = {name:"毒魂",     itemType:640031, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7006] = {name:"高级毒魂", itemType:640032, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7007] = {name:"玄魂",     itemType:640041, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7008] = {name:"高级玄魂", itemType:640042, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7009] = {name:"迟钝",     itemType:640051, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7010] = {name:"高级迟钝", itemType:640052, getType:0, buyType:0, drugType:0};
//			
//			PetSkillLearnConstData.SKILL_DATA_PET[7011] = {name:"狡猾",     itemType:640061, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7012] = {name:"高级狡猾", itemType:640062, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7013] = {name:"憨厚",     itemType:640071, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7014] = {name:"高级憨厚", itemType:640072, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7015] = {name:"拼命",     itemType:640081, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7016] = {name:"高级拼命", itemType:640082, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7017] = {name:"法魂",     itemType:640091, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7018] = {name:"高级法魂", itemType:640092, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7019] = {name:"蛮力",     itemType:640101, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7020] = {name:"高级蛮力", itemType:640102, getType:0, buyType:0, drugType:0};
//			
//			PetSkillLearnConstData.SKILL_DATA_PET[7021] = {name:"借力",     itemType:640111, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7022] = {name:"高级借力", itemType:640112, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7023] = {name:"移魂",     itemType:640121, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7024] = {name:"高级移魂", itemType:640122, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7025] = {name:"瞬影",     itemType:640131, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7026] = {name:"高级瞬影", itemType:640132, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7027] = {name:"强身",     itemType:640141, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7028] = {name:"高级强身", itemType:640142, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7029] = {name:"凝神",     itemType:640151, getType:0, buyType:1, drugType:630015};
//			PetSkillLearnConstData.SKILL_DATA_PET[7030] = {name:"高级凝神", itemType:640152, getType:0, buyType:0, drugType:0};
//			
//			PetSkillLearnConstData.SKILL_DATA_PET[7031] = {name:"冰天雪地", itemType:640161, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7032] = {name:"烈火燎原", itemType:640171, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7033] = {name:"血毒万里", itemType:640181, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7034] = {name:"五雷轰顶", itemType:640191, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7035] = {name:"血爆",     itemType:640201, getType:0, buyType:0, drugType:630018};
//			PetSkillLearnConstData.SKILL_DATA_PET[7036] = {name:"高级血爆", itemType:640202, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7037] = {name:"圣爆",     itemType:640211, getType:0, buyType:0, drugType:630018};
//			PetSkillLearnConstData.SKILL_DATA_PET[8001] = {name:"吸血",     itemType:640221, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[8002] = {name:"高级吸血", itemType:640222, getType:0, buyType:0, drugType:0};
//			
//			PetSkillLearnConstData.SKILL_DATA_PET[7039] = {name:"寒冰咒",   itemType:640231, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[7040] = {name:"极冰咒",   itemType:640232, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7041] = {name:"烈火咒",   itemType:640241, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[7042] = {name:"冥火咒",   itemType:640242, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7043] = {name:"血毒咒",   itemType:640251, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[7044] = {name:"嗜毒咒",   itemType:640252, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7045] = {name:"玄雷咒",   itemType:640261, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[7046] = {name:"天雷咒",   itemType:640262, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7047] = {name:"猛击",     itemType:640291, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[7048] = {name:"高级猛击", itemType:640292, getType:0, buyType:0, drugType:0};
//			
//			PetSkillLearnConstData.SKILL_DATA_PET[8007] = {name:"连击",     itemType:640301, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[8008] = {name:"高级连击", itemType:640302, getType:0, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7049] = {name:"痛击",     itemType:640311, getType:0, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[7050] = {name:"高级痛击", itemType:640312, getType:0, buyType:0, drugType:0};
//			
//			//只能通过打怪获得
//			PetSkillLearnConstData.SKILL_DATA_PET[8003] = {name:"吸气",     itemType:640271, getType:1, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[8004] = {name:"高级吸气", itemType:640272, getType:1, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[8005] = {name:"打怒",     itemType:640281, getType:1, buyType:0, drugType:630020};
//			PetSkillLearnConstData.SKILL_DATA_PET[8006] = {name:"高级打怒", itemType:640282, getType:1, buyType:0, drugType:0};
//			PetSkillLearnConstData.SKILL_DATA_PET[7038] = {name:"高级圣爆", itemType:640212, getType:1, buyType:0, drugType:0};
		}
		/**
		 *是否是新版宠物 
		 * 
		 */		
		private function slectViewState():void
		{
			if(PetPropConstData.isNewPetVersion)
			{
				PetRuleControlMediator.MAXPAGE = 8;
			}
			else
			{
				ruleBase.removeChild(ruleBase.mcPage_7);
				ruleBase.removeChild(ruleBase.txtPage_7);
			}
		}
		
		private function initPanelBaseDoubleBreed():void
		{
			viewComponent = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetRuleBase");
			
			for(var i:int = 0; i < 8; i++) {
				ruleBase.removeChild(ruleBase["txtPage_"+i]);
				ruleBase.removeChild(ruleBase["mcPage_"+i]);
			}
			for(var j:int = 0; j < ruleBase.numChildren; j++) {
				ruleBase.getChildAt(j).y -= 17;
			}
			
			panelBase = new PanelBase(ruleBase, ruleBase.width+5, ruleBase.height+11);
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ini_2" ] );   // 双人繁殖
			panelBase.name = "PetRuleController";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;;
			}else{
				var pos:Point = UIUtils.getMiddlePos(panelBase); 
				panelBase.x = pos.x - 20;
				panelBase.y = pos.y;
			}
			
			itemDataGrid = new PetRuleDataGrid(128, 252);
			itemDataGrid.rendererClass = PetRuleItemCell;
			itemDataGrid.x = 330;
			itemDataGrid.y = 220;
			ruleBase.addChild(itemDataGrid);
			
			uiManager = new PetRuleUIManager(ruleBase);
			sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetBreedDoubleConstData.breedCost);
			PetRuleCommonData.curPageIndex = 6;
			
			isLock = true;
			
//			initGrid();
			initPetChoiceListData(1);
//			showCurView();
//			showCurData();
			
			facade.registerMediator(new PetBreedDoubleMediator());
			facade.sendNotification(PetRuleEvent.INIT_SUB_VIEW_PET_RULE);
			
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			
			addLis();
			
			isLock = false;
		}
		
		public function initPetChoiceListData(type:uint=0):void
		{
			if(iScrollPane && ruleBase.contains(iScrollPane)) {
				ruleBase.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			listView = new ListComponent(false);
			showFilterList();
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = 326;
			iScrollPane.y = 48;
			if(type == 1) iScrollPane.y -= 17;
			iScrollPane.width = 130;
			iScrollPane.height = 128;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			ruleBase.addChild(iScrollPane);
		}
		
		private function showFilterList():void
		{
			for(var id:Object in GameCommonData.Player.Role.PetSnapList)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petRuleList_"+id;
				item.mcSelected.visible = false; 
				item.mcSelected.mouseEnabled = false;
				item.txtName.mouseEnabled = false;
				var sexId:int = GameCommonData.Player.Role.PetSnapList[id].Sex;
				var sexStr:String = sexId == 0 ? "<font color='#0098FF'>♂</font> " : (sexId == 1 ? "<font color='#CC66FF'>♀ </font>" : "<font color='#FF9900'>§ </font>");
//				var sex:String = (GameCommonData.Player.Role.PetSnapList[id].Sex == 0) ? "<font color='#0098FF'>♂</font> " : "<font color='#CC66FF'>♀ </font>";
				item.txtName.htmlText = sexStr+"<font color='#ffff00'>"+GameCommonData.Player.Role.PetSnapList[id].PetName+"</font>";
				item.addEventListener(MouseEvent.CLICK, selectItem);
				listView.addChild(item);
				if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == id) item.mcSelected.visible = true;
			}
			listView.width = 115;
			listView.upDataPos();
		}
		
		private function selectPetSucess(id:uint):void
		{
			if(listView) {
				for(var j:int = 0; j < listView.numChildren; j++) {
					(listView.getChildAt(j) as MovieClip).mcSelected.visible = false;
				}
				var item:* = listView.getChildByName("petRuleList_"+id);
				if(item) {
					item.mcSelected.visible = true;
				}
			}
		}
		
		private function clearPetSelect():void
		{
			if(listView) {
				for(var j:int = 0; j < listView.numChildren; j++) {
					(listView.getChildAt(j) as MovieClip).mcSelected.visible = false;
				}
			}
		}
		
		private function updatePetNameListVew(id:uint):void
		{
			if(listView) {
				var item:* = listView.getChildByName("petRuleList_"+id);
				if(item) {
					var sexId:int = GameCommonData.Player.Role.PetSnapList[id].Sex;
					var sexStr:String = sexId == 0 ? "<font color='#0098FF'>♂</font> " : (sexId == 1 ? "<font color='#CC66FF'>♀ </font>" : "<font color='#FF9900'>§ </font>");
//					var sex:String = (GameCommonData.Player.Role.PetSnapList[id].Sex == 0) ? "<font color='#0098FF'>♂</font> " : "<font color='#CC66FF'>♀ </font>";
					item.txtName.htmlText = sexStr+"<font color='#ffff00'>"+GameCommonData.Player.Role.PetSnapList[id].PetName+"</font>";
				}
			}
		}
		
		private function selectItem(event:MouseEvent):void
		{
			if(isLock || PetRuleCommonData.selectLock) return;
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(!GameCommonData.Player.Role.PetSnapList[id]) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_add_8" ], color:0xffff00});   // 该宠物不存在
				gc();
				return;
			}
			if(clickTimer.running && id == tmpIdClick) {
				clickTimer.reset();
				dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
				PetPropConstData.isSearchOtherPetInto = true;
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
			} else {
				clickTimer.reset();
				clickTimer.start();
				tmpIdClick = id;
//				if(PetRuleCommonData.selectedPet && id == PetRuleCommonData.selectedPet.Id) return;
			}
		}
		
		private function clickTimerCompHandler(e:TimerEvent):void
		{
			PetRuleCommonData.selectLock = true;
			sendNotification(PetRuleEvent.SELECT_PET_PET_RULE, tmpIdClick);
			PetRuleCommonData.selectLock = false;
		}
		
		private function initGrid():void
		{
			for(var i:int = 0; i < 3; i++) {
				var unit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				unit.x = 475;
				unit.y = i * (85 + unit.height)  + 30;
				ruleBase.addChild(unit);
				buyItemList.push(unit);
			}
		}
		
		private function choicePageHandler(e:MouseEvent):void
		{
			var index:uint = uint(e.target.name.split("_")[1]);
			if(index == PetRuleCommonData.curPageIndex || isLock) return;
			isLock = true;
			for(var i:int = 0; i < MAXPAGE; i++) {
				ruleBase["mcPage_"+i].gotoAndStop(2);
				ruleBase["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			ruleBase["mcPage_"+index].removeEventListener(MouseEvent.CLICK, choicePageHandler);
			PetRuleCommonData.curPageIndex = index;
			ruleBase["mcPage_"+index].gotoAndStop(1);
			
			clearData();
			showCurView();
			showCurData();
			PetRuleCommonData.selectedPet = null;
			isLock = false;
		}
		
		private function clearData():void
		{
			if(subView && ruleBase.contains(subView)) {
				sendNotification(PetRuleEvent.CLEAR_SUB_VIEW_PET_RULE);
				ruleBase.removeChild(subView);
				subView = null;
			}
			for(var i:int = 0; i < 3; i++) {
				if(buyItemList[i]) {
					for(var k:int = 0; k < (buyItemList[i] as MovieClip).numChildren; k++) {
						if( (buyItemList[i] as MovieClip).getChildAt(k) is UseItem ) {
							(buyItemList[i] as MovieClip).removeChildAt(k);
							break;
						}
					}
					buyItemList[i].name = "petRuleConGrid_"+i;
				}
				ruleBase["btnBuy_"+i].removeEventListener(MouseEvent.CLICK, buyHandler);
				
				ruleBase["txtInput_"+i].text = "1";
				ruleBase["txtGoodNamePet_"+i].text = "";
				ruleBase["mcMoney_"+i].txtMoney.text = "";
				ShowMoney.ShowIcon(ruleBase["mcMoney_"+i], ruleBase["mcMoney_"+i].txtMoney, true);
			}
			for(var j:int = 0; j < listView.numChildren; j++) {
				(listView.getChildAt(j) as MovieClip).mcSelected.visible = false;
			}
			uiManager.showMoney(0, 0);
			buyInfo = null;
		}
		
		private function showCurView():void
		{
			switch(PetRuleCommonData.curPageIndex) {
				case 0:
					facade.registerMediator(new PetToBabyMediator());
					break;
				case 1:
					facade.registerMediator(new PetBreedSingleMediator());
					break;
				case 2:
					facade.registerMediator(new PetSavvyUseMoneyMediator());
					break;
				case 3:
					facade.registerMediator(new PetSavvyJoinMediator());
					break;
				case 4:
					facade.registerMediator(new PetSkillLearnMediator());
					break;
				case 5:
					facade.registerMediator(new PetSkillUpMediator());
					break;
				case 6:
//					facade.registerMediator(new PetBreedDoubleMediator());
					subView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetBreedDouble_phony");
					ruleBase.addChild(subView);
					sendNotification(PetRuleEvent.SHOW_MONEY_BASE_PET_RULR, PetBreedDoubleConstData.breedCost);
					itemDataGrid.dataPro = [[]];
					break;
				case 7:
				 	facade.registerMediator(new PetWinningUpMediator());
				break;
			}
			facade.sendNotification(PetRuleEvent.INIT_SUB_VIEW_PET_RULE);
		}
		
		private function showCurData():void
		{
			showBuy();
			showGrid();
		}
		
		private function showBuy():void
		{
			if(PetRuleCommonData.MARKET_INDEX[PetRuleCommonData.curPageIndex]) {
				var index:uint = PetRuleCommonData.MARKET_INDEX[PetRuleCommonData.curPageIndex];
				for(var i:int = 0; i < 3; i++) {
					if(!(UIConstData.MarketGoodList[index] as Array)[i]) continue;
					var good:Object = (UIConstData.MarketGoodList[index] as Array)[i];
					buyItemList[i].name = "goodQuickBuy"+i+"_"+good.type;
					var useItem:UseItem = new UseItem(i, good.type, ruleBase);
					if(good.type < 300000) {
						useItem.Num = 1;
					}
					else if(good.type >= 300000) {
						useItem.Num = UIConstData.getItem(good.type).amount; 
					}
					useItem.x = 2;
					useItem.y = 2;
					useItem.Id = UIConstData.getItem(good.type).id;
					useItem.IsBind = 0;
					useItem.Type = good.type;
					useItem.IsLock = false;
					buyItemList[i].addChild(useItem);
					
					var color:String = IntroConst.itemColors[UIConstData.getItem(good.type).Color];
					var name:String  = good.Name;
					
					ruleBase["txtGoodNamePet_"+i].htmlText = "<font color='"+color+"'>"+name+"</font>";
					ruleBase["mcMoney_"+i].txtMoney.text = good.PriceIn + MarketConstData.payWayStrList[good.PayType[0]];
					ShowMoney.ShowIcon(ruleBase["mcMoney_"+i], ruleBase["mcMoney_"+i].txtMoney, true);
					ruleBase["btnBuy_"+i].addEventListener(MouseEvent.CLICK, buyHandler);
				}
			}
		}
		
		/** 显示格子 */
		private function showGrid():void
		{
			var index:int = PetRuleCommonData.curPageIndex;
			
			if(PetRuleCommonData.PAGE_PANEL_DATA[index] == null) {
				itemDataGrid.dataPro = [[]];
				return;
			}
			
			var bagIndex:uint = PetRuleCommonData.PAGE_PANEL_DATA[index].index;
			
			var res:Array = [[]];
			
			var curIndex:uint = 0; 
			
			var arr:Array = BagData.AllItems[bagIndex];
			var len:int = BagData.BagNum[bagIndex];
			
			for(var i:int = 0; i < len; i++) {
				if(arr[i] == undefined) {
					if(res[Math.floor(curIndex/3)] == null) {
						res[Math.floor(curIndex/3)] = [];
					}
					res[Math.floor(curIndex/3)][curIndex%3] = 1;
					curIndex++;
				} else {
					var item:Object = arr[i];
					if( PetRuleCommonData.isPermitItem(index, item.type) ) {
						if(res[Math.floor(curIndex/3)] == null) {
							res[Math.floor(curIndex/3)] = [];
						}
//						if(EquipDataConst.getInstance().lockItems[arr[i].id]){
//							strengenItems[Math.floor(strengenCount/8)][strengenCount%8]={detailData:arr[i],id:arr[i].id,isLock:false,usedNum:EquipDataConst.getInstance().lockItems[arr[i].id].usedNum};
//						}else{
							res[Math.floor(curIndex/3)][curIndex%3] = {detailData:arr[i], id:arr[i].id, isLock:false};
//						}
						curIndex++;
					}
				}
			}
			itemDataGrid.dataPro = res;
//			ruleBase.addChildAt(itemDataGrid,0);
		}
		
		private function updateItems():void
		{
			showGrid();
		}
		
		private function buyHandler(e:MouseEvent):void
		{
			var index:uint = uint(String(e.target.name).split("_")[1]);
			for(var i:int = 0; i < ruleBase.numChildren; i++) {
				if(ruleBase.getChildAt(i).name.indexOf("goodQuickBuy"+index+"_") == 0) {
					var type:uint = uint(ruleBase.getChildAt(i).name.split("_")[1]);
					var num:uint = uint(ruleBase["txtInput_"+index].text);
					if(num == 0) num++;
//					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:type, count:num});

					var goodIndex:uint = PetRuleCommonData.MARKET_INDEX[PetRuleCommonData.curPageIndex];
					var len:int = (UIConstData.MarketGoodList[goodIndex] as Array).length;
					var cost:Number = 0;
					
					for(var j:int = 0; j < len; j++) {	//商品总价
						var good:Object = (UIConstData.MarketGoodList[goodIndex] as Array)[j];
						if(type == good.type) {
							cost = good.PriceIn * num;
							break;
						}
					}
					
					sendNotification(PetRuleEvent.ASK_TO_BUY_PET_RULE, {itemType:type, buyType:2, cost:cost, num:num, learn:0, up:0});
					break;
				}
			}
		}
		
		private function askToBuy():void
		{
			if(buyInfo) {
				var itemType:uint = buyInfo.itemType;
				var buyType:uint  = buyInfo.buyType;
				var cost:int      = buyInfo.cost;
				var num:uint      = buyInfo.num;
				var learn:uint    = buyInfo.learn;
				var up:uint		  = buyInfo.up;
				
				var resStr:String = "";
				var moneyStr:String = "";
				var itemName:String = UIConstData.getItem(itemType).Name; 
				if(buyType == 1) { 			//银两购买
					moneyStr = UIUtils.getMoneyInfo(cost);
					var tmpArr:Array = moneyStr.split("\\");
					for(var i:int = 0; i < tmpArr.length; i++) {
						if(int(tmpArr[i]) > 0) {
							var tmpInt:int = tmpArr[i];
							tmpArr[i] = "<font color='#00ff00'>"+tmpInt+"</font>\\";
						}
					}
					moneyStr = tmpArr.join("");
				} else if(buyType == 2) {	//元宝购买
					moneyStr = "<font color='#00ff00'>" + int(cost) + "</font>\\ab";
				}
				// 是否花费  购买   个
				resStr = GameCommonData.wordDic[ "often_used_iscost" ] + " "+moneyStr+" "+ GameCommonData.wordDic[ "often_used_buy" ] + "<font color='#00ff00'>"+num+"</font>"+GameCommonData.wordDic[ "mod_pet_pbc_med_petr_ask_1" ]+"<font color='#00ff00'>"+itemName+"</font>？";
				if(learn > 0 || up > 0) {
					resStr = GameCommonData.wordDic[ "mod_pet_psu_med_pets_sho_1" ]+"<font color='#00ff00'>"+itemName+"</font>，" + resStr;   // 背包中没有
				}
				facade.sendNotification(EventList.SHOWALERT, {comfrim:applyBuy, cancel:cancelClose, info:resStr, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ]});   // 提 示
			}
		}
		
		private function applyBuy():void
		{
			if(buyInfo) {
				var itemType:uint = buyInfo.itemType;
				var buyType:uint  = buyInfo.buyType;
				var num:uint      = buyInfo.num;
				var learn:uint	  = buyInfo.learn;
				var up:uint  	  = buyInfo.up;
				
				if(buyType == 1) {			//银两购买
					var obj:Object = new Object();
					obj.type = Protocol.OPERATE_ITEMS;
					obj.data = [];
					obj.data.push(OperateItem.BUYNPCITEM);
					obj.data.push(1);
					obj.data.push(1); 			//支付方式
					obj.data.push(610);			//NPC id
					obj.data.push(0);
					obj.data.push(0);
					obj.data.push("");			//归属人 name
					
					obj.data.push(0);			//物品id
					obj.data.push(itemType);
					obj.data.push(num);			//购买数量
					obj.data.push(0);
					obj.data.push(0);
					obj.data.push(0);
					obj.data.push(0);
					OperatorItemSend.PlayerAction(obj);
				} else if(buyType == 2) {	//元宝购买
					sendNotification(MarketEvent.BUY_ITEM_MARKET, {type:itemType, count:num});
				}
				if(learn == 0 && up == 0) {
					buyInfo = null;
				}
			}
		}
		
		private function checkLearnAndUpSkill():void
		{
			if(buyInfo) {
				var learn:uint = buyInfo.learn;
				var up:uint	   = buyInfo.up;
				if(learn > 0 || up > 0) {
					var id:int = buyInfo.id;
					if(learn > 0) {
						sendNotification(PetRuleEvent.NOTICE_LEARN_SKILL_PET_RULE, {id:id});
					} else {
						sendNotification(PetRuleEvent.NOTICE_UP_SKILL_PET_RULE, {id:id});
					}
					buyInfo = null;
				}
			}
		}
		
		private function cancelClose():void
		{
			buyInfo = null;
		}
		
		private function addLis():void
		{
			for(var i:int = 0; i < 3; i++) {
				ruleBase["btnSub_"+i].addEventListener(MouseEvent.CLICK, clickSubHandler);
				ruleBase["btnAdd_"+i].addEventListener(MouseEvent.CLICK, clickAddHandler);
				ruleBase["txtInput_"+i].addEventListener(Event.CHANGE, textChangeHandler);
				UIUtils.addFocusLis(ruleBase["txtInput_"+i]);
			}
			if(clickTimer != null) clickTimer.addEventListener(TimerEvent.TIMER_COMPLETE, clickTimerCompHandler);
			ruleBase.btnToMarket.addEventListener(MouseEvent.CLICK, btnClickHandler);
			itemDataGrid.addEventListener(GridCellEvent.GRIDCELL_CLICK, clickItemHandler);
		}
		
		private function removeLis():void
		{
			for(var i:int = 0; i < 3; i++) {
				ruleBase["btnSub_"+i].removeEventListener(MouseEvent.CLICK, clickSubHandler);
				ruleBase["btnAdd_"+i].removeEventListener(MouseEvent.CLICK, clickAddHandler);
				ruleBase["txtInput_"+i].removeEventListener(Event.CHANGE, textChangeHandler);
				UIUtils.removeFocusLis(ruleBase["txtInput_"+i]);
			}
			for(var j:int = 0; j < MAXPAGE; j++) {
				(ruleBase["mcPage_"+j] as MovieClip).stop();
				ruleBase["mcPage_"+j].removeEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			if(clickTimer != null) clickTimer.removeEventListener(TimerEvent.TIMER_COMPLETE, clickTimerCompHandler);
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
			ruleBase.btnToMarket.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			itemDataGrid.removeEventListener(GridCellEvent.GRIDCELL_CLICK, clickItemHandler);
		}
		
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name) {
				case "btnToMarket":
					if(dataProxy.MarketIsOpen) {
						sendNotification(EventList.CLOSEMARKETVIEW);
					}
					sendNotification(EventList.SHOWMARKETVIEW, 4);
					break;  
			}
		}
		
		private function clickItemHandler(e:GridCellEvent):void
		{
			if(PetRuleCommonData.curPageIndex == 4) {
				var data:Object = e.data;
				sendNotification(PetRuleEvent.SELECT_ITEM_TO_SKILL_LEARN_PET_RULE, data);
			}
		}
		
		private function clickSubHandler(e:MouseEvent):void
		{
			var index:int = int((e.target.name as String).split("_")[1]);
			var num:int = int(ruleBase["txtInput_"+index].text) - 1;
			if(num <= 0) {
				ruleBase["txtInput_"+index].text = "1";
			} else {
				ruleBase["txtInput_"+index].text = num.toString();
			}
		}
		
		private function clickAddHandler(e:MouseEvent):void
		{
			var index:int = int((e.target.name as String).split("_")[1]);
			var num:int = int(ruleBase["txtInput_"+index].text) + 1;
			if(num > 999) {
				ruleBase["txtInput_"+index].text = "999";
			} else {
				ruleBase["txtInput_"+index].text = num.toString();
			}
		}
		
		private function textChangeHandler(e:Event):void
		{
			var index:int = int((e.target.name as String).split("_")[1]);
			if(int(ruleBase["txtInput_"+index].text) <= 0) {
				ruleBase["txtInput_"+index].text = "1";
			} else if(int(ruleBase["txtInput_"+index].text) > 999) {
				ruleBase["txtInput_"+index].text = "999";
			}
		}
		
		private function panelCloseHandler(e:Event):void 
		{
			gc();
		}
		
		private function gc():void
		{
			clearData();
			removeLis();
			ruleBase.removeChild(itemDataGrid);
			itemDataGrid = null;
			buyInfo = null;
			PetRuleCommonData.curPageIndex = 0;
			PetRuleCommonData.selectedPet  = null;
			PetRuleCommonData.selectLock   = false;
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				panelBase = null;
			}
			clickTimer.stop();
			clickTimer = null;
			facade.removeProxy(PetRuleGridManager.NAME);
			gridManager = null;
			uiManager = null;
			listView = null;
			iScrollPane = null;
			buyItemList = null;
			dataProxy.PetQuerySkill = false;
			dataProxy.PetCanOperate = true;
			dataProxy.petRuleIsOpen = false;
			dataProxy = null;
			facade.removeMediator(PetRuleControlMediator.NAME);
		}
		
		
	}
}




