package GameUI.Modules.Stall.Mediator
{
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.Stall.Command.StallBBSReceiveCommand;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.Stall.Proxy.StallGridManager;
	import GameUI.Modules.Stall.Proxy.StallNetAction;
	import GameUI.Modules.Stall.UI.StallUIManager;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StallMediator extends Mediator
	{
		public static const NAME:String = "StallMediator";
		public static const MAXPAGE:uint = 2;
		public static const STALLNAME_MAX_CHAR:uint = 14;
		public static const GRID_STARTPOS:Point = new Point(8, 73);
		
		private var dataProxy:DataProxy = null;
		private var stallUIManager:StallUIManager = null;
		private var stallGridManager:StallGridManager = null;
		
		private var stallPanel:PanelBase = null;
		private var moneyPanel:PanelBase = null;
		
		private var gridSprite:MovieClip = null;		/** 格子面板 */	
		private var petSprite:Sprite = null;			/** 宠物面板 */
		
		private var listView:ListComponent = null;
		private var iScrollPane:UIScrollPane = null;
		
		private var moneyOpType:int = 0;				/** 金钱输入用途 0=从背包拖进摆摊、1=修改价格，2=拖宠物进摆摊栏 */
		
		private var idToAdd:uint = 0;					/** 要添加的物品ID */
		
		//////////////////////////////////
		private var panelBaseChoice:PanelBase = null;
		private var listViewChoice:ListComponent = null;
		private var iScrollPaneChoice:UIScrollPane = null;
		//////////////////////////////////
		
		public function StallMediator()
		{
			super(NAME);
		}
		
		private function get stall():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWSTALL,				//点击背包中的摆摊
				EventList.BEGINSTALL,				//开始摆摊（收到服务器成功返回）
				EventList.GETSTALLITEMS,			//查询某摊位物品等信息
				StallEvents.SHOWSOMESTALL,			//收到某摊位物品等信息
				EventList.STALLITEM,				//服务器返回添加的物品
				EventList.BAGTOSTALL,				//背包物品拖进摆摊栏
				StallEvents.STALLTOBAG,				//拖回背包
				StallEvents.REFRESHMONEY,			//刷新显示钱数
				StallEvents.REFRESHMONEYSEFLSTALL,	//刷新自己携带金钱
				StallEvents.DELSTALLITEM,			//成功删除摆摊物品
				StallEvents.UPDATESTALLNAME,		//更新摊位名
				StallEvents.UPDATESTALLOWNERNAME,	//更新摊主名
				StallEvents.REMOVESTALL,			//收摊，对外接口
				StallEvents.REMOVESTALLNOW,         //强行收摊
				StallEvents.UPDATE_SALE_PET_STALL,	//更新出售中的宠物列表
				StallEvents.UPDATE_PET_LIST_STALL,	//更新宠物选择列表
				PlayerInfoComList.SELECT_ELEMENT	//选中玩家，
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.STALL});
					facade.registerCommand(CommandList.STALLBBSRECEIVECOMMAND, StallBBSReceiveCommand);
					initView();
					break;
				case EventList.SHOWSTALL:																//点击 背包的摆摊按钮
					if(StallConstData.stallSelfId == 0) {//尚未摆摊
						if(dataProxy.StallIsOpen) {
							StallConstData.goodList = new Array(24);
							gcAll();
						}
						if(dataProxy.TradeIsOpen) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_1" ], color:0xffff00});   //"交易中不能摆摊"
							return;
						} else if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_DEAD) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_2" ], color:0xffff00});   //"死亡状态不能摆摊"
							return;
						} else if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_RUN) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_3" ], color:0xffff00});   //"移动时不能摆摊"
							return;
						} 
						if(GameCommonData.GameInstance.GameScene.GetGameScene.Map.GetTargetTile(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY) != MapTileModel.PATH_BOOTH) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_4" ] , color:0xffff00});   //"此地不允许摆摊"
							return;
						}
						if(dataProxy.DepotIsOpen) {//关掉仓库
							sendNotification(EventList.CLOSEDEPOTVIEW);
						}
						if(dataProxy.NPCShopIsOpen) {//关掉NPC商店
							sendNotification(EventList.CLOSENPCSHOPVIEW);
						}
						StallNetAction.OperateStall(PlayerAction.BEGIN_STALL);		//请求开始摆摊
					} else {														//查看自己摊位信息
						if(dataProxy.StallIsOpen && StallConstData.stallIdToQuery == StallConstData.stallSelfId) return;
						if(dataProxy.TradeIsOpen) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_5" ], color:0xffff00});    // "交易中不能进行摊位操作"
							return;
						}
						StallConstData.goodList = new Array(24);
						if(dataProxy.StallIsOpen) gcAll();
						StallConstData.stallIdToQuery = StallConstData.stallSelfId;
						StallConstData.stallMsg  = [];
						StallNetAction.OperateItem(OperateItem.GETSTALLITEMS, 1, StallConstData.stallIdToQuery, 0, null);
					}
					break;
				case EventList.BEGINSTALL:																//收到服务器返回的   开始摆摊
					var npcId:int = int(notification.getBody());
					if(npcId != 0) {
						GameCommonData.Player.HideName();
						GameCommonData.Player.HideTitle();
						GameCommonData.Player.Role.StallId = npcId;
						GameCommonData.Player.PlayerStall();
						
						if(dataProxy.DepotIsOpen) {								//关闭仓库
							sendNotification(EventList.CLOSEDEPOTVIEW);
						}
						sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);	//关闭宠物操作系列面板
						if(dataProxy.equipPanelIsOpen) {						//关闭强化界面
							sendNotification(EquipCommandList.CLOSE_EQUIP_PANEL);
						}
						StallConstData.goodList = new Array(24);
						StallConstData.stallSelfId = npcId;						//自己的摊位ID
						StallConstData.stallIdToQuery = npcId;
						stallUIManager.setModel(0);
						initStall();
						stallGridManager.addMouseDown();						//允许格子拖动
						addLis();
						dataProxy.StallIsOpen = true;
						OperateItem.CompleteTrade = true;
						stallUIManager.refreshMoney();
						
						if(GameCommonData.SameSecnePlayerList[npcId] != null) {
							var playerMC:MovieClip = GameCommonData.SameSecnePlayerList[npcId].getChildAt(0);
							var stallNameStr:String = playerMC.txtStallName.text;
							stall.txtStallName.text = stallNameStr;
							StallConstData.stallName = stallNameStr;
						} else {
							stall.txtStallName.text  = "";
							StallConstData.stallName = "";
						}
						stall.txtOwerName.text = GameCommonData.Player.Role.Name.toString();
						if(!StallConstData.StallBBSisOpen) {
							var stallBBSMediator:StallBBSMediator = new StallBBSMediator();
							facade.registerMediator(stallBBSMediator);
							sendNotification(StallEvents.SHOWSTALLBBS, StallConstData.stallIdToQuery);
						}
						stallUIManager.showMoney(1);
						//提示摆摊成功
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_6" ], color:0xffff00});  //"摆摊成功"
						sendNotification(PetEvent.PET_AUTO_REST_OUTSIDE_INTERFACE);
						StallNetAction.OperateMsg(2032, GameCommonData.wordDic[ "mod_stall_med_sm_7" ], StallConstData.stallSelfId);    //"人傻，钱多，速来！"
						StallConstData.stallOwnerName = GameCommonData.Player.Role.Name;
//						if(!dataProxy.BagIsOpen) {
//							sendNotification(EventList.SHOWBAG);
//							dataProxy.BagIsOpen = true;
//						} 
						GameCommonData.Player.Role.State = GameRole.STATE_STALL;
						
						for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
							StallConstData.petListChoice[key] = UIUtils.DeeplyCopy(GameCommonData.Player.Role.PetSnapList[key]);
						}
						initPetChoiceList();
						initPetSaleList();
						//去掉组队小旗子
						GameCommonData.Player.SetTeam(false);
						GameCommonData.Player.SetTeamLeader(false);
						UIConstData.KeyBoardCanUse = false;		// 禁用快捷键
					}
					break;
				case EventList.GETSTALLITEMS:															//查询某摊位物品等信息-对外接口
					var idToQuery:int = int(notification.getBody());
					if(idToQuery > 0) {
						if(dataProxy.StallIsOpen) gcAll();
						StallConstData.goodList = new Array(24);
						StallConstData.stallIdToQuery = idToQuery;
						StallConstData.stallMsg  = [];
						StallNetAction.OperateItem(OperateItem.GETSTALLITEMS, 1, idToQuery, 0, null);
					}
					break;
				case StallEvents.SHOWSOMESTALL:															//收到某摊位物品等信息
					if(StallConstData.stallIdToQuery > 0 && StallConstData.stallIdToQuery == StallConstData.stallSelfId) {//自己的摊位
						stallUIManager.setModel(0);
						stallGridManager.addMouseDown();
						if(GameCommonData.SameSecnePlayerList[StallConstData.stallSelfId] != null)
						{
							var playerMCC:MovieClip = GameCommonData.SameSecnePlayerList[StallConstData.stallSelfId].getChildAt(0);
							var stallNameStrr:String = playerMCC.txtStallName.text;
							stall.txtStallName.text = stallNameStrr;
							StallConstData.stallName = stallNameStrr;
						} else {
							stall.txtStallName.text  = "";
							StallConstData.stallName = "";
						}
						StallConstData.stallOwnerName = GameCommonData.Player.Role.Name.toString();
						stall.txtOwerName.text = StallConstData.stallOwnerName;
						
						for(var id:Object in GameCommonData.Player.Role.PetSnapList) {
							StallConstData.petListChoice[id] = UIUtils.DeeplyCopy(GameCommonData.Player.Role.PetSnapList[id]);
						}
						initPetChoiceList();
					} else {	//别人的摊位
						stallUIManager.setModel(2);
						stallGridManager.removeMouseDown();
						
						if(GameCommonData.SameSecnePlayerList[StallConstData.stallIdToQuery] != null) {
							var playerMCCC:MovieClip = GameCommonData.SameSecnePlayerList[StallConstData.stallIdToQuery].getChildAt(0);
							var stallNameStrrr:String = playerMCCC.txtStallName.text;
							stall.txtStallName.text = stallNameStrrr;
							StallConstData.stallName = stallNameStrrr;
						} else {
							stall.txtStallName.text  = "";
							StallConstData.stallName = "";
						}
						stall.txtOwerName.text = StallConstData.stallOwnerName;
						if(!GameCommonData.Player.Role.State) {
							GameCommonData.Player.Role.State = GameRole.STATE_LOOKINGSTALL;
						}
					}
					stallUIManager.showMoney(1);
					initStall();
					addLis();
					OperateItem.CompleteTrade = true;
					stallUIManager.refreshMoney();
					if(!StallConstData.StallBBSisOpen) {
						var stallBBSMediatorR:StallBBSMediator = new StallBBSMediator();
						facade.registerMediator(stallBBSMediatorR);
						sendNotification(StallEvents.SHOWSTALLBBS, StallConstData.stallIdToQuery);
					}
					dataProxy.StallIsOpen = true;
//					if(!dataProxy.BagIsOpen) {
//						sendNotification(EventList.SHOWBAG);
//						dataProxy.BagIsOpen = true;
//					}
					initPetSaleList();
					break;
				case EventList.BAGTOSTALL:																//物品从背包拖进摆摊
					idToAdd = uint(notification.getBody());
					if(StallConstData.stallSelfId > 0 && StallConstData.stallOwnerName == GameCommonData.Player.Role.Name) {//是自己的摊
						moneyOpType = 0;
						showMoneyPanel();
						stallGridManager.lockGrids();	//锁住所有格子
						stallUIManager.lockBtns();		//锁住所有按钮
						stallPanel.canClose(false);		//锁住关闭按钮
					} else {
						sendNotification(EventList.BAGITEMUNLOCK, idToAdd);
					}
					break;
				case StallEvents.STALLTOBAG:															//物品从摆摊拖回背包
					var goodId:int = int(notification.getBody());
					var operateItem:Object = new Object();
					
					operateItem.id = goodId;
					operateItem.type = 0;
					operateItem.amount = 0;
					operateItem.maxAmount = 0;
					operateItem.position = 0;
					operateItem.isBind = 0;
					operateItem.index = getItemDataPos(goodId) + 1;
					operateItem.price = 0;
					//发送删除物品命令
					StallNetAction.OperateItem(OperateItem.DELSTALLITEM, 1, 0, goodId, operateItem);
					break;
				case EventList.STALLITEM:																//收到服务器返回的  添加(修改)物品
					stallGridManager.unLockGrids();	//解锁所有格子
					stallUIManager.unLockBtns();	//解锁所有按钮
					stallPanel.canClose(true);		//解锁关闭按钮
					stallGridManager.removeAllItem();
					StallConstData.SelectedItem = null;
					stallGridManager.removeAllFrames();
					stallGridManager.showItems(StallConstData.goodList);
					stallUIManager.refreshMoney();
					break;
				case StallEvents.DELSTALLITEM:															//收到服务器返回的  删除摆摊物品
					var delId:int = int(notification.getBody());
					stallGridManager.removeAllItem();
					StallConstData.SelectedItem = null;
					stallGridManager.removeAllFrames();
					stallGridManager.showItems(StallConstData.goodList);
					stallUIManager.refreshMoney();
					sendNotification(EventList.BAGITEMUNLOCK, delId);
					break;
				case StallEvents.REFRESHMONEY:															//刷新钱数显示
					stallUIManager.refreshMoney();
					break;
				case StallEvents.REFRESHMONEYSEFLSTALL:													//刷新自己携带金钱
					stallUIManager.showMoney(1);
					break;
				case StallEvents.UPDATESTALLNAME:														//更新摊位名字
					if(dataProxy.StallIsOpen) {
						stall.txtStallName.text = StallConstData.stallName.toString();
					}
					break;
				case StallEvents.UPDATESTALLOWNERNAME:													//更新摊主名字
					stall.txtOwerName.text = StallConstData.stallOwnerName;
					break;
				case StallEvents.REMOVESTALL:															//收摊，对外接口
					if(StallConstData.stallSelfId != 0) {
						facade.sendNotification(EventList.SHOWALERT, {comfrim:closeStall, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_stall_med_sm_8" ], title:GameCommonData.wordDic[ "often_used_tip" ]});    //"你真的确定要收摊吗？"  "提 示"
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_9" ], color:0xffff00});    //"你已不在摆摊状态"
					}
					break;
				case StallEvents.REMOVESTALLNOW:
					closeStall();
					break;
//				case PlayerInfoComList.SELECT_ELEMENT:
//					if(GameCommonData.TargetAnimal.Role.Type == "NPC" && GameCommonData.TargetAnimal.Role.Face == 30000) {
//						var idRole:int = GameCommonData.TargetAnimal.Role.Id;
//						sendNotification(EventList.GETSTALLITEMS, idRole);
//					}
//					break;
				case StallEvents.UPDATE_SALE_PET_STALL:													//更新出售中的宠物列表
					initPetSaleList();
					stallUIManager.refreshMoney();
					break;
				case StallEvents.UPDATE_PET_LIST_STALL:													//更新宠物选择列表
					initPetChoiceList();
					stallUIManager.refreshMoney();
					break;
			}
		}
		
		private function initView():void
		{
			var moneyInput:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MoneyInput");
			(moneyInput.txtJin as TextField).restrict = "0-9";
			(moneyInput.txtYin as TextField).restrict = "0-9";
			(moneyInput.txtTong as TextField).restrict = "0-9";
			var choiceList:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetList");
			gridSprite = new MovieClip();
			gridSprite.name = "stall";
			petSprite = new Sprite();
			
			stallUIManager = new StallUIManager(stall);
			
			stallPanel = new PanelBase(stall, stall.width+8, stall.height+12);
			stallPanel.name = "StallPanel";
			stallPanel.addEventListener(Event.CLOSE, stallCloseHandler);
			stallPanel.x = StallConstData.STALL_DEFAULT_POS.x;
			stallPanel.y = StallConstData.STALL_DEFAULT_POS.y;
			stallPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_stall_med_sm_10" ]);	     //"摆 摊"
			
			moneyPanel = new PanelBase(moneyInput, moneyInput.width+8, moneyInput.height+12);
			moneyPanel.name = "MoneyInpu";
			moneyPanel.addEventListener(Event.CLOSE, moneyCloseHandler);
			moneyPanel.x = StallConstData.MONEY_DEFAULT_POS.x;
			moneyPanel.y = StallConstData.MONEY_DEFAULT_POS.y;
			moneyPanel.SetTitleTxt(GameCommonData.wordDic[ "mod_stall_med_sm_11" ]);    //"单价输入"
			
			panelBaseChoice = new PanelBase(choiceList, choiceList.width-6, choiceList.height+11);
			panelBaseChoice.name = "PetPanelStallChoice";
			panelBaseChoice.x = StallConstData.PET_DEFAULT_POS.x;
			panelBaseChoice.y = StallConstData.PET_DEFAULT_POS.y;
			panelBaseChoice.SetTitleTxt(GameCommonData.wordDic[ "mod_stall_med_sm_12" ]);     //"宠物列表"
			panelBaseChoice.disableClose();
			choiceList.btnCancel.visible = false;
			choiceList.txtCancel.visible = false;
			
			UIUtils.addFocusLis(moneyInput.txtJin);
			UIUtils.addFocusLis(moneyInput.txtYin);
			UIUtils.addFocusLis(moneyInput.txtTong);
			
			initGrid();
		}
	
		/** 初始化摆摊 */
		private function initStall():void
		{
			for(var i:int = 0; i < MAXPAGE; i++)
			{
				stall["mcPage_"+i].buttonMode = true;
				stall["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
				if(i == StallConstData.SelectIndex) 
				{		
					stall["mcPage_"+i].gotoAndStop(1);	
					stall["mcPage_"+i].removeEventListener(MouseEvent.CLICK, choicePageHandler);			
					continue;
				} 
				stall["mcPage_"+i].gotoAndStop(2);
			}
			stallPanel.x = StallConstData.STALL_DEFAULT_POS.x;
			stallPanel.y = StallConstData.STALL_DEFAULT_POS.y;
			stall.addChild(gridSprite);
			
			GameCommonData.GameInstance.GameUI.addChild(stallPanel);
			
//			if(StallConstData.stallName) stall.txtStallName.text = GameCommonData.TargetAnimal.Role.Name.toString();
//			if(StallConstData.stallOwnerName) stall.txtOwerName.text = StallConstData.stallOwnerName.toString();
		}
		
		private function initGrid():void
		{
			for(var i:int = 0; i < 24; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6);
				gridUnit.y = (gridUnit.height) * int(i/6);
				gridUnit.name = "stall_" + i.toString();
				gridSprite.addChild(gridUnit);	//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = gridSprite;								//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				StallConstData.GridUnitList.push(gridUint);
			}
			gridSprite.x = GRID_STARTPOS.x;
			gridSprite.y = GRID_STARTPOS.y;
			stall.addChild(gridSprite);
			stallGridManager = new StallGridManager(StallConstData.GridUnitList, gridSprite);
			facade.registerProxy(stallGridManager);
		}
		
		private function initPetSaleList():void
		{
			StallConstData.SelectedPet = null;
			if(iScrollPane && petSprite.contains(iScrollPane)) {
				petSprite.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			listView = new ListComponent(false);
			showFilterList();
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = 15;
			iScrollPane.y = 77.5;
			iScrollPane.width = 204;
			iScrollPane.height = 135.5;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			
			gridSprite.x = 7;
			gridSprite.y = 73;
			petSprite.addChild(iScrollPane);
		}
		
		private function showFilterList():void
		{
			for(var id:Object in StallConstData.petListSale)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemBig");
				item.name = "petStallIsSaleChoice_"+id;
				item.txtName.text = StallConstData.petListSale[id].PetName;
				item.txtName.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = true;
				item.mcSelected.mouseEnabled = false;
				item.mcSelected.visible = false;
				item.doubleClickEnabled = true;
				item.btnChosePet.mouseEnabled = false;
				item.addEventListener(MouseEvent.CLICK, selectItem);
				listView.addChild(item);
			}
			listView.width = 195;
			listView.upDataPos();
		}
		
		private function selectItem(event:MouseEvent):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(moneyPanel)) return;
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			for(var i:int = 0; i < listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			StallConstData.SelectedPet = StallConstData.petListSale[id];
			stallUIManager.refreshMoney();
			if(event.ctrlKey) {
				var type:uint = StallConstData.SelectedPet.ClassId;
				var name:String = StallConstData.SelectedPet.PetName;
				var ownerId:int = 0;
				if(StallConstData.stallSelfId > 0 && StallConstData.stallOwnerName == GameCommonData.Player.Role.Name) {
					ownerId = GameCommonData.Player.Role.Id;
				} else {
					for(var key:* in GameCommonData.SameSecnePlayerList) {
						var person:GameElementAnimal = GameCommonData.SameSecnePlayerList[key];
						if(person.Role.Name == StallConstData.stallOwnerName) {
							ownerId = person.Role.Id;
							break;
						}
					}
				}
				if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
					facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<2_["+name+"]_"+id+"_"+type+"_"+ownerId+"_0_4>");
				} else {
					facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<2_["+name+"]_"+id+"_"+type+"_"+ownerId+"_0_4>");
				}
			}
		}
		
		private function initPetChoiceList():void
		{
			StallConstData.SelectedPetSF = null;
			
			if(iScrollPaneChoice && panelBaseChoice.content.contains(iScrollPaneChoice)) {
				panelBaseChoice.content.removeChild(iScrollPaneChoice);
				iScrollPaneChoice = null;
				listViewChoice = null;
			}
			listViewChoice = new ListComponent(false);
			showFilterListSf();
			iScrollPaneChoice = new UIScrollPane(listViewChoice);
			iScrollPaneChoice.x = 3;
			iScrollPaneChoice.y = 3;
			iScrollPaneChoice.width = 118;
			iScrollPaneChoice.height = 135;
			iScrollPaneChoice.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPaneChoice.refresh();
			panelBaseChoice.content.addChild(iScrollPaneChoice);
		}
		
		private function showFilterListSf():void
		{
			for(var i:Object in StallConstData.petListChoice)
			{
				if(GameCommonData.Player.Role.PetSnapList[i].IsLock == false) {
					var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
					item.name = "petStallChoice_"+i.toString();
					item.doubleClickEnabled = true;
					item.mcSelected.visible = false;
					item.txtName.mouseEnabled = false;
					item.mcSelected.mouseEnabled = false;
					item.btnChosePet.mouseEnabled = false;
					item.txtName.text = StallConstData.petListChoice[i].PetName;
					item.addEventListener(MouseEvent.CLICK, selectItemSf);
					item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
					listViewChoice.addChild(item);
				}
			}
			listViewChoice.width = 115;
			listViewChoice.upDataPos();
		}
		
		private function selectItemSf(event:MouseEvent):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(moneyPanel)) return;
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(StallConstData.SelectedPetSF && StallConstData.SelectedPetSF.Id == id) return;
			for(var i:int = 0; i < listViewChoice.numChildren; i++)
			{
				(listViewChoice.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			StallConstData.SelectedPetSF = StallConstData.petListChoice[id];
		}
		
		/** 双击查看宠物属性 */
		private function lookPetInfoHandler(e:MouseEvent):void
		{
			var item:MovieClip = e.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(id > 0) {
				PetPropConstData.isSearchOtherPetInto = true;
				if(GameCommonData.Player.Role.PetSnapList[id]) {
					dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
				} else {
					dataProxy.PetEudemonTmp = StallConstData.petListSale[id];
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:StallConstData.petListSale[id].OwnerId});
				}
			}
		}
		
		/** 选择不同页签 物品、宠物 */
		private function choicePageHandler(event:MouseEvent):void
		{
			for(var i:int = 0; i < MAXPAGE; i++) {
				stall["mcPage_"+i].gotoAndStop(2);
				stall["mcPage_"+i].addEventListener(MouseEvent.CLICK, choicePageHandler);
			}
			var index:uint = uint(event.target.name.split("_")[1]);
			stall["mcPage_"+index].removeEventListener(MouseEvent.CLICK, choicePageHandler);	
			StallConstData.SelectIndex = index;
			stall["mcPage_"+index].gotoAndStop(1);
			if(stall.getChildByName("yellowFrame")) {
				stall.removeChild(stall.getChildByName("yellowFrame"));
			}
			if(StallConstData.SelectIndex == 0) {					//物品
				StallConstData.stallSelfId == StallConstData.stallIdToQuery ? stallUIManager.setModel(0) : stallUIManager.setModel(2);
				//请求 物品数据
				if(stall.contains(petSprite)) stall.removeChild(petSprite);
				if(panelBaseChoice && GameCommonData.GameInstance.GameUI.contains(panelBaseChoice)) {
					GameCommonData.GameInstance.GameUI.removeChild(panelBaseChoice);
				}
				stall.addChild(gridSprite);
				if(!dataProxy.PetCanOperate) {
					sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
					dataProxy.PetCanOperate = true;
				}
			} else if(StallConstData.SelectIndex == 1) {			//宠物
				if(StallConstData.stallSelfId != 0 && StallConstData.stallSelfId == StallConstData.stallIdToQuery) {
					stallUIManager.setModel(1);
					if(dataProxy.PetIsOpen) {
						sendNotification(EventList.CLOSEPETVIEW);
					}
					dataProxy.PetCanOperate = false;
					GameCommonData.GameInstance.GameUI.addChild(panelBaseChoice);
				} else {
					stallUIManager.setModel(3);
				}
				//请求 宠物数据
				stall.removeChild(gridSprite);
				stall.addChild(petSprite);
			}
		}
		
		private function gcAll():void
		{
			stallGridManager.removeAllItem();
			if(StallConstData.SelectIndex == 1) {
				dataProxy.PetCanOperate = true;
			}
			if(panelBaseChoice && GameCommonData.GameInstance.GameUI.contains(panelBaseChoice)) {
				GameCommonData.GameInstance.GameUI.removeChild(panelBaseChoice);
			}
			StallConstData.SelectIndex = 0;
			StallConstData.SelectedItem = null;
			StallConstData.SelectedPet = null;
			StallConstData.SelectedPetSF = null;
			StallConstData.stallInfo = "";
			StallConstData.stallIdToQuery = 0;
			StallConstData.petListChoice = new Dictionary();
			StallConstData.petListSale = new Dictionary();
			
//			if(StallConstData.stallSelfId <= 0) {
//				StallConstData.petListSale = new Dictionary();
//				StallConstData.petListChoice = new Dictionary();
//			}
			
//			StallConstData.stallName = "";
//			StallConstData.stallOwnerName = "";
			StallConstData.stallMsg  = [];
			
			StallConstData.moneyAll = 0;
			removeLis();
			stallPanel.canClose(true);		//解锁关闭按钮
			if(stall.contains(gridSprite)) stall.removeChild(gridSprite);
			if(stall.contains(petSprite)) stall.removeChild(petSprite);
			dataProxy.StallIsOpen = false;
			gcMoneyPanel();
			if(StallConstData.StallBBSisOpen) sendNotification(StallEvents.REMOVESTALLBBS);
			if(stallPanel && GameCommonData.GameInstance.GameUI.contains(stallPanel)) GameCommonData.GameInstance.GameUI.removeChild(stallPanel);
			if(GameCommonData.Player.Role.State == GameRole.STATE_LOOKINGSTALL) {
				GameCommonData.Player.Role.State = null;
			}
		}
		
		/** 点击按钮  修改名称、摊位信息、改价、下架、收摊、购买、物品、查看宠物资料 */
		private function btnClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btnCloseStall":																					//收摊
					facade.sendNotification(EventList.SHOWALERT, {comfrim:closeStall, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_stall_med_sm_13" ], title:GameCommonData.wordDic[ "often_used_tip" ]});   //"你真的确定要收摊吗？"   "提 示"
					break;
				case "btnInputSure":																					//金钱输入 确定
					var jin:uint   = uint(moneyPanel.content.txtJin.text);
					var yin:uint   = uint(moneyPanel.content.txtYin.text);
					var tong:uint  = uint(moneyPanel.content.txtTong.text);
					var money:uint = formatMoney(jin,yin,tong);
					if(money > 0) {	
						if(moneyOpType == 0) {			//从背包拖进物品
							//发送 添加物品命令
							StallNetAction.OperateItem(OperateItem.ADDSTALLITEM, 1, money, idToAdd, null);
							BagData.lockBagGridUnit(true);
						} else if(moneyOpType == 2) {	//添加宠物
							StallNetAction.OperateItem(OperateItem.PET_ADD_STALL, 1, money, StallConstData.SelectedPetSF.Id, null);
							StallConstData.SelectedPetSF = null;
						} else {						//修改原有商品价格
							if(StallConstData.SelectIndex == 0) {  //物品
								var goodId:int = StallConstData.SelectedItem.Item.Id;
								StallNetAction.OperateItem(OperateItem.MODIFYPRICE, 1, money, goodId, null);
							} else {								//宠物
								var petId:uint = StallConstData.SelectedPet.Id;
								StallNetAction.OperateItem(OperateItem.PET_MODIFY_PRICE_STALL, 1, money, petId, null);
								StallConstData.SelectedPet = null;
							}
						}
						gcMoneyPanel();
					}
					break;
				case "btnInputCancel":																					//金钱输入 取消
					stallGridManager.unLockGrids();	//解锁所有格子
					stallUIManager.unLockBtns();	//解锁所有按钮
					stallPanel.canClose(true);		//解锁关闭按钮
					if(moneyOpType == 0) {	//从背包拖进物品
						BagData.lockBagGridUnit(true);
						sendNotification(EventList.BAGITEMUNLOCK, idToAdd);
					}
					gcMoneyPanel();
					break;
				case "btnStallInfo":																					//摊位信息
					if(!StallConstData.StallBBSisOpen) {
						var stallBBSMediator:StallBBSMediator = new StallBBSMediator();
						facade.registerMediator(stallBBSMediator);
						sendNotification(StallEvents.SHOWSTALLBBS, StallConstData.stallIdToQuery);
					} else {
						sendNotification(StallEvents.REMOVESTALLBBS);
					}
					break;
				case "btnModifyName":																					//修改摊位名称
					var stallName:String = stall.txtStallName.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(stallName) {
						if(stallName != StallConstData.stallName) {
							//发送 修改摊位名的命令
							if(!UIUtils.checkChat(stallName)) {
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_14" ], color:0xffff00});  //"含有非法字符"
								stall.txtStallName.text = StallConstData.stallName;
							} else {
								StallNetAction.OperateMsg(2031, stallName, StallConstData.stallSelfId);
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_15" ], color:0xffff00});   //"摊位名称修改成功"
								StallConstData.stallName = stallName;
							}
						}
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_16" ], color:0xffff00});     //"摊位名称不能为空"
						stall.txtStallName.text = StallConstData.stallName;
					} 
					break;
				case "btnModifyPrice":																					//修改价格
					if(StallConstData.SelectIndex == 0) {//物品
						if(StallConstData.SelectedItem && StallConstData.SelectedItem.Item) {
							moneyOpType = 1;
							showMoneyPanel();
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_17" ], color:0xffff00});    //"请先选择物品"
						}
					} else {							 //宠物
						if(StallConstData.SelectedPet && StallConstData.SelectedPet.Id) {
							moneyOpType = 1;
							showMoneyPanel();
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_18" ], color:0xffff00});    //"请先选择宠物"
						}
					}
					break;
				case "btnDelGood":																						//下架
					if(StallConstData.SelectIndex == 0) {//物品
						if(StallConstData.SelectedItem && StallConstData.SelectedItem.Item) {
							var goodDelId:int = StallConstData.SelectedItem.Item.Id;
							var itemDel:Object = new Object();
							itemDel.id = goodDelId;
							itemDel.type = 0;
							itemDel.amount = 0;
							itemDel.maxAmount = 0;
							itemDel.position = 0;
							itemDel.isBind = 0;
							itemDel.index = getItemDataPos(goodDelId) + 1;
							itemDel.price = 0;
							//发送删除物品命令
							StallNetAction.OperateItem(OperateItem.DELSTALLITEM, 1, 0, goodDelId, itemDel);
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_17" ], color:0xffff00});    //"请先选择物品"
						}
					} else {							 //宠物
						if(StallConstData.SelectedPet && StallConstData.SelectedPet.Id) {
							StallNetAction.OperateItem(OperateItem.PET_DEL_STALL, 1, 0, StallConstData.SelectedPet.Id, null);
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_18" ], color:0xffff00});  //"请先选择宠物"
						}
					}
					break;
				case "btnBuy":																							//购买
					if(StallConstData.SelectIndex == 0) {//物品
//						var num:int = int(stall.txtInputNum.text);
						if(StallConstData.SelectedItem && StallConstData.SelectedItem.Item) {	//选中物品不为空
//----------购买数量，暂时不用
//							if(num <= 0) {
//								facade.sendNotification(HintEvents.RECEIVEINFO, {info:"请输入购买数量", color:0xffff00});
//								return;
//							}
//----------购买数量，暂时不用
							var goodItem:Object = StallConstData.goodList[StallConstData.SelectedItem.Item.Pos];
							var num:int = StallConstData.SelectedItem.Item.Num;
							var moneyCost:Number = Number(goodItem.price * num);
							if(moneyCost < 100000) {	//10金以下的消费不用提示
								applyBuyGood();
							} else {
								var moneyCostStr:String = UIUtils.getMoneyInfo(moneyCost);
//								trace("花费字符串：",moneyCostStr);
								facade.sendNotification(EventList.SHOWALERT, {comfrim:applyBuyGood, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_stall_med_sm_19" ]+moneyCostStr+GameCommonData.wordDic[ "mod_stall_med_sm_20" ], title:GameCommonData.wordDic[ "often_used_tip" ]});  //"是否花费 "  " 购买该物品？"  "提 示"
							}
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_21" ], color:0xffff00});      //"请先选择要购买的物品"
						}
					} else {							 //宠物
						if(StallConstData.SelectedPet && StallConstData.SelectedPet.Id) {	//选中宠物不为空
							//发送 购买命令
							var pricePet:Number = StallConstData.SelectedPet.Price;
							if(pricePet < 100000) {		//10金以下的消费不用提示
								applyBuyGood();
							} else {
								var petCostStr:String = UIUtils.getMoneyInfo(pricePet);
								facade.sendNotification(EventList.SHOWALERT, {comfrim:applyBuyGood, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_stall_med_sm_19" ]+petCostStr+GameCommonData.wordDic[ "mod_stall_med_sm_22" ], title:GameCommonData.wordDic[ "often_used_tip" ]});   //"是否花费 "   " 购买该宠物？"   "提 示"
							}
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_23" ], color:0xffff00});    //"请先选择要购买的宠物"
						}
					}
					break;
				case "btnSeePet":																						//查看宠物资料
					if(StallConstData.SelectedPet && StallConstData.SelectedPet.Id) {
						dataProxy.PetEudemonTmp = StallConstData.SelectedPet;
						dataProxy.PetCanOperate = false;
						PetPropConstData.isSearchOtherPetInto = true;
						sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:StallConstData.SelectedPet.Id, ownerId:StallConstData.SelectedPet.OwnerId});
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_24" ], color:0xffff00});      //"请先选择宠物"
					}
					break;
			}
		}
		
		private function chosePetHandler(e:MouseEvent):void
		{
			if(StallConstData.stallSelfId > 0 && StallConstData.stallOwnerName == GameCommonData.Player.Role.Name) { //是自己的摊
				if(StallConstData.SelectedPetSF && StallConstData.SelectedPetSF.Id) {
					if(GameCommonData.Player.Role.PetSnapList[StallConstData.SelectedPetSF.Id].State == 1) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_25" ], color:0xffff00});   //"出战状态宠物不能出售"
						return;
					}
					moneyOpType = 2;
					showMoneyPanel();
					stallGridManager.lockGrids();	//锁住所有格子
					stallUIManager.lockBtns();		//锁住所有按钮
					stallPanel.canClose(false);		//锁住关闭按钮
					panelBaseChoice.content.btnPetChose.mouseEnabled = false;
				} else {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_24" ], color:0xffff00});     //"请先选择宠物"
				}
			}
		}
		
		/** 确定购买物品 */
		public function applyBuyGood():void
		{
			if(StallConstData.SelectIndex == 0) {	//物品页
				if(StallConstData.SelectedItem && StallConstData.SelectedItem.Item) {	//选中物品不为空
					var goodItem:Object = StallConstData.goodList[StallConstData.SelectedItem.Item.Pos];
					var numItem:int = StallConstData.SelectedItem.Item.Num;				//数量
					var objItem:Object = new Object();
					objItem.id = goodItem.id;
					objItem.type = 0;
					objItem.amount = numItem;
					objItem.maxAmount = 0;
					objItem.position = 0;
					objItem.isBind = 0;
					objItem.index = 0;
					objItem.price = goodItem.price;
					StallNetAction.OperateItem(OperateItem.BUYSTALLITEM, 1, StallConstData.stallIdToQuery, 0, objItem);
					StallConstData.SelectedItem = null;
					stallGridManager.removeAllFrames();
	//				stall.txtInputNum.text = "1";		购买数量，暂时不用
				} else {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_21" ], color:0xffff00});   //"请先选择要购买的物品"
				}
			} else {								//宠物页
				if(StallConstData.SelectedPet && StallConstData.SelectedPet.Id) {		//选中宠物不为空
					var objPet:Object = new Object();
					objPet.id = StallConstData.SelectedPet.Id;
					objPet.type = 0;
					objPet.amount = 1;
					objPet.maxAmount = 0;
					objPet.position = 0;
					objPet.isBind = 0;
					objPet.index = 0;
					objPet.price = StallConstData.SelectedPet.Price;
					StallNetAction.OperateItem(OperateItem.PET_BUY_STALL, 1, StallConstData.stallIdToQuery, 0, objPet);
					StallConstData.SelectedPet = null;
				} else {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_23" ], color:0xffff00});     //"请先选择要购买的宠物"
				}
			}
		}
		
		public function closeStall():void
		{
			StallNetAction.OperateStall(PlayerAction.DESTROY_STALL);
			StallConstData.stallSelfId = 0;
//			for(var o:Object in StallConstData.petListSale) {
//				if(GameCommonData.Player.Role.PetSnapList[o]) {
//					GameCommonData.Player.Role.PetSnapList[o].IsLock = false;
//				}
//			}
			for(var i:int = 0; i < StallConstData.petListSaleSelfIdArr.length; i++) {	//解锁自己的宠物
				if(StallConstData.petListSaleSelfIdArr[i] == undefined) continue;
				var idUnlock:uint = StallConstData.petListSaleSelfIdArr[i];
				if(GameCommonData.Player.Role.PetSnapList[idUnlock]) {
					GameCommonData.Player.Role.PetSnapList[idUnlock].IsLock = false;
				}
			}
			StallConstData.petListSaleSelfIdArr = [];
			for(var j:int = 0; j < StallConstData.goodList.length; j++) {				//解锁物品
				if(StallConstData.goodList[j] && StallConstData.goodList[j].id) {
					var unLockGoodId:int = StallConstData.goodList[j].id;
					sendNotification(EventList.BAGITEMUNLOCK, unLockGoodId);
				}
			}
			if(idToAdd > 0) {
				sendNotification(EventList.BAGITEMUNLOCK, idToAdd);
			}
			BagData.lockBagGridUnit(true);
			gcAll();
			GameCommonData.Player.Role.State = GameRole.STATE_NULL;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stall_med_sm_26" ], color:0xffff00});     //"摊位已收起"
			UIConstData.KeyBoardCanUse = true;		// 可用快捷键
			GameCommonData.Player.ShowName();
			GameCommonData.Player.ShowTitle();
			GameCommonData.Player.Role.StallId = 0;
			GameCommonData.Player.PlayerStall();
			
			//还原头上组队旗子
			var player:GameElementPlayer = GameCommonData.Player;
			if(player.Role.idTeam == 0)
			{
				player.SetTeam(false);
				player.SetTeamLeader(false);
			}
			else
			{
				if(player.Role.idTeamLeader == player.Role.Id)
				{
					player.SetTeamLeader(true);
				}
				else
				{
					player.SetTeam(true);
				}
			}
			//恢复心情显示
			if(player.Role.IsShowFeel==0)
			{
				player.ShowTitle();
			}
			else
			{
				player.HideTitle();
			}
			player.ShowName();
		}
		public function cancelClose():void
		{
			
		}

		/** 显示金钱面板 */
		public function showMoneyPanel():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(moneyPanel)) {
				moneyPanel.x = StallConstData.MONEY_DEFAULT_POS.x;
				moneyPanel.y = StallConstData.MONEY_DEFAULT_POS.y;
			} else {
				GameCommonData.GameInstance.GameUI.addChild(moneyPanel);
			}
			if(moneyOpType != 1) {	//添加物品 或 添加宠物
				moneyPanel.content.txtJin.text = "0";
				moneyPanel.content.txtYin.text = "0";
				moneyPanel.content.txtTong.text = "1";
			} else {				//修改价格
				if(StallConstData.SelectIndex == 0) {	//修改物品价格
					var pos:int = getItemDataPos(StallConstData.SelectedItem.Item.Id);
					var price:int = StallConstData.goodList[pos].price;
					var priceArr:Array = stallUIManager.getMoney(price);
					moneyPanel.content.txtJin.text = priceArr[0].toString();
					moneyPanel.content.txtYin.text = priceArr[1].toString();
					moneyPanel.content.txtTong.text = priceArr[2].toString();
				} else {								//修改宠物价格
					var pricePet:uint = StallConstData.petListSale[StallConstData.SelectedPet.Id].Price;
					var priceArrPet:Array = stallUIManager.getMoney(pricePet);
					moneyPanel.content.txtJin.text = priceArrPet[0].toString();
					moneyPanel.content.txtYin.text = priceArrPet[1].toString();
					moneyPanel.content.txtTong.text = priceArrPet[2].toString();
				}
			}
		}
		
		/** 输入检测 摊位名称、 购买数量 */
		private function inputHandler(e:Event):void
		{
			switch(e.target.name)
			{
				case "txtStallName":			//摊位名称
					var name:String = stall.txtStallName.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(name) {
						stall.txtStallName.text = UIUtils.getTextByCharLength(name, STALLNAME_MAX_CHAR);
					}
					break;
//-------------	购买数量 暂时不用				
//				case "txtInputNum":				//购买数量
//					var num:int = int(stall.txtInputNum.text);
//					if(num <= 0) {
//						stall.txtInputNum.text = "1";
//					} else 
//					if(StallConstData.SelectedItem) {
//						var maxNum:int = StallConstData.SelectedItem.Item.Num;
//						if(num > maxNum) stall.txtInputNum.text = maxNum.toString();
//					}
//					break;
//-------------	购买数量 暂时不用				
			}
		}
		
		/** 添加事件监听 */
		private function addLis():void
		{
			panelBaseChoice.content.btnPetChose.addEventListener(MouseEvent.CLICK, chosePetHandler);
			if(StallConstData.stallSelfId == StallConstData.stallIdToQuery) {	//摊主
				stall.btnModifyName.addEventListener(MouseEvent.CLICK, btnClickHandler);
				stall.btnModifyPrice.addEventListener(MouseEvent.CLICK, btnClickHandler);
				stall.btnDelGood.addEventListener(MouseEvent.CLICK, btnClickHandler);
				stall.btnCloseStall.addEventListener(MouseEvent.CLICK, btnClickHandler);
				stall.txtStallName.addEventListener(Event.CHANGE, inputHandler);
				stall.txtStallName.addEventListener(FocusEvent.FOCUS_IN, focusInHandler);
				stall.txtStallName.addEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			} else {						//买家
				stall.btnBuy.addEventListener(MouseEvent.CLICK, btnClickHandler);
//				stall.txtInputNum.addEventListener(Event.CHANGE, inputHandler);
			}
			stall.btnSeePet.addEventListener(MouseEvent.CLICK, btnClickHandler);
			stall.btnStallInfo.addEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.btnInputSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			moneyPanel.content.btnInputCancel.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			UIUtils.addFocusLis(
		}
		
		private function removeLis():void
		{
			panelBaseChoice.content.btnPetChose.removeEventListener(MouseEvent.CLICK, chosePetHandler);
			if(stall.btnModifyName.hasEventListener(MouseEvent.CLICK)) stall.btnModifyName.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.btnModifyPrice.hasEventListener(MouseEvent.CLICK)) stall.btnModifyPrice.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.btnDelGood.hasEventListener(MouseEvent.CLICK)) stall.btnDelGood.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.btnCloseStall.hasEventListener(MouseEvent.CLICK)) stall.btnCloseStall.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.btnBuy.hasEventListener(MouseEvent.CLICK)) stall.btnBuy.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.btnStallInfo.hasEventListener(MouseEvent.CLICK)) stall.btnStallInfo.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.btnSeePet.hasEventListener(MouseEvent.CLICK)) stall.btnSeePet.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(stall.txtStallName.hasEventListener(Event.CHANGE)) stall.txtStallName.removeEventListener(Event.CHANGE, inputHandler);
			if(stall.txtStallName.hasEventListener(FocusEvent.FOCUS_IN)) stall.txtStallName.removeEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			if(stall.txtStallName.hasEventListener(FocusEvent.FOCUS_OUT)) stall.txtStallName.removeEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
//			if(stall.txtInputNum.hasEventListener(Event.CHANGE)) stall.txtInputNum.removeEventListener(Event.CHANGE, inputHandler);
			if(moneyPanel.content.btnInputSure.hasEventListener(MouseEvent.CLICK)) moneyPanel.content.btnInputSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			if(moneyPanel.content.btnInputCancel.hasEventListener(MouseEvent.CLICK)) moneyPanel.content.btnInputCancel.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			for(var i:int = 0; i < MAXPAGE; i++) {
				if(stall["mcPage_"+i].hasEventListener(MouseEvent.CLICK)) stall["mcPage_"+i].removeEventListener(MouseEvent.CLICK, choicePageHandler)
			}
		}
		
		private function focusInHandler(e:FocusEvent):void
		{
			GameCommonData.isFocusIn = true;
		}
		private function focusOutHandler(e:FocusEvent):void
		{
			GameCommonData.isFocusIn = false;
		}
		
		private function stallCloseHandler(e:Event):void
		{
			gcAll();
		}
		private function moneyCloseHandler(e:Event):void
		{
			if(moneyOpType == 0) {	//从背包拖进物品
				BagData.lockBagGridUnit(true);
				sendNotification(EventList.BAGITEMUNLOCK, idToAdd);
			}
			gcMoneyPanel();
		}
		
		private function alertBack():void
		{
			
		}
		
		private function gcMoneyPanel():void
		{
			stallGridManager.unLockGrids();	//解锁所有格子
			stallUIManager.unLockBtns();	//解锁所有按钮
			stallPanel.canClose(true);		//解锁关闭按钮
			panelBaseChoice.content.btnPetChose.mouseEnabled = true;
			if(GameCommonData.GameInstance.GameUI.contains(moneyPanel)) {
				GameCommonData.GameInstance.GameUI.removeChild(moneyPanel);
			}
		}
		
		private function formatMoney(jin:uint, yin:uint, tong:uint):uint
		{
			return (10000 * jin + 100 * yin + tong);	//1金=100银，1银=100铜
		}
		
		private function getItemDataPos(goodId:int):int
		{
			var pos:int;
			for(var i:int = 0; i < StallConstData.goodList.length; i++){
				if(StallConstData.goodList[i] && StallConstData.goodList[i].id && StallConstData.goodList[i].id == goodId) {
					pos = i;
					break;
				}
			}
			return pos;
		}
		
		
	}
}



