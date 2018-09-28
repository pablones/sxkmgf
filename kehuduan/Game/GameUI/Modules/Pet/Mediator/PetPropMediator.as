package GameUI.Modules.Pet.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.Pet.Proxy.PetSkillGridManager;
	import GameUI.Modules.Pet.UI.NewPetLoadComponent;
	import GameUI.Modules.Pet.UI.PetUIManager;
	import GameUI.Modules.Pet.UI.ShowPetModuleComponent;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	import flash.utils.clearTimeout;
	import flash.utils.getTimer;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetPropMediator extends Mediator
	{
		public static const NAME:String = "PetPropMediator";
		
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var petUIManager:PetUIManager = null;
		private var petSkillGridManager:PetSkillGridManager = null;
		private var gridSprite:MovieClip = null;
		private var listView:ListComponent = null;
		private var iScrollPane:UIScrollPane = null;
//		private var itemCount:uint = 0;
		private var timer:Timer;
		private var timerOut:Timer;
		private var isQueryOtherOne:Boolean = false;
		private var intervalId:uint;			//点击加减属性的计时器
		
		private var tempMC:MovieClip; //获取更多信息面板上的mc
		private var petMoreInfoMediator:PetMoreInfoMediator;
		private var _showPetModule:ShowPetModuleComponent;
		private var backView:Sprite;	//宠物模型显示背景
		private var isSendDependence:Boolean;	//是否发送了附体（分离）请求
		private var dependenceCdTime:int;	//分离/附体 5秒 cd时间
		public function PetPropMediator()
		{
			super(NAME);
		}
		
		private function get petView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWPETVIEW,					//显示宠物面板
				EventList.CLOSEPETVIEW,					//关闭宠物面板
				PetEvent.LOOKPETINFO_BYID,				//通过ID去服务器查询，查看宠物属性
				PetEvent.LOOKPETINFO_OBJ,				//直接接收到GamePetRole查看宠物数据
				PetEvent.RETURN_TO_SHOW_PET_INFO,		//服务器返回详细宠物数据，显示到界面
				PetEvent.PET_RENAME_FAIL,				//宠物改名失败
				PetEvent.PET_UPDATE_SHOW_INFO,			//更新宠物信息   新增、更新
				PetEvent.PET_DELETE_SUCCESS,			//成功删除了宠物
				PetEvent.GET_PETINFO_OF_PLAYER,			//查询某人的宠物信息，对外接口
				PetEvent.SHOW_PET_LIST_OF_PLAYER,		//服务器返回的别人的宠物列表，显示。
				PetEvent.PET_FEED_OUTSIDE_INTERFACE,	//喂食，对外接口
				PetEvent.PET_TRAIN_OUTSIDE_INTERFACE,	//驯养，对外接口
				PetEvent.PET_REST_OUTSIDE_INTERFACE,	//休息，对外接口
				PetEvent.PET_COMEOUT_FIGHT_OUTSIDE_INTERFACE,	//出战，对外接口
				PetEvent.PET_AUTO_REST_OUTSIDE_INTERFACE, //休息，对外接口，直接休息，不受10秒时间限制
				PetEvent.PET_LOOK_OUTSIDE_INTERFACE,	//查看某个宠物资料(别人的宠物)，对外接口
				PetEvent.PET_ATT_UPDATE_UI,				//宠物更新指定属性
				BagEvents.EXTENDBAG,					//宠物背包扩充
				PetEvent.PETPROP_PANEL_STOP_DROG,		//禁止宠物面板拖动
				PetEvent.PETPROP_INIT_POS				//宠物面板位置还原
			];
		}
		
		public override function onRegister():void
		{
			if(PetPropConstData.isNewPetVersion)
			{
				var loadComponent:NewPetLoadComponent = new NewPetLoadComponent();
				loadComponent.callBackFun = loadCallBack;
			}
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:									//初始化
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					timer = new Timer(1000, 5);
					timerOut = new Timer(1000, 5);
					if(!PetPropConstData.isNewPetVersion)
					{
						facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.PETVIEW});
						initView();
						initPetUiManager();
					}
					break;
				case EventList.SHOWPETVIEW:									//显示宠物面板
					if(!dataProxy.PetCanOperate) {	//不可操作状态，正在进行宠物操作
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_1" ], color:0xffff00});  // 正在操作宠物中
						return;
					}
					if(!dataProxy.PetIsOpen) {
						addLis();
						showPetView();
						dataProxy.PetIsOpen = true;
						if(NewerHelpData.newerHelpIsOpen)	//通知新手指导系统
							sendNotification(NewerHelpEvent.OPEN_PETPROP_NOTICE_NEWER_HELP);
					}
					break;
				case EventList.CLOSEPETVIEW:								//关闭宠物面板
					gcAll();
					break;
				case PetEvent.LOOKPETINFO_BYID:								//通过ID去服务器查询，查看宠物属性
					var ids:Object = notification.getBody(); 
					if(ids.petId > 0) {
						petSkillGridManager.lockAllGrid(false);
						sendData(OperateItem.GET_PET_INFO, ids);
					}
					break;
				case PetEvent.LOOKPETINFO_OBJ:								//直接接收到GamePetRole查看宠物数据
					var petToShow:GamePetRole = notification.getBody() as GamePetRole;
					if(dataProxy.PetCanOperate) {		// 可操作
						if(petToShow.Potential > 0) {	//有点
							petToShow.State == 0 ? petUIManager.setModel(1) : petUIManager.setModel(0);
						} else {						//有点
							petToShow.State == 0 ? petUIManager.setModel(3) : petUIManager.setModel(2);
						}
						petUIManager.showPetData(petToShow, 0);
						petSkillGridManager.lockAllGrid(true);
					} else {							//不可操作
						petUIManager.setModel(4);
						petUIManager.showPetData(petToShow, 1);
						petSkillGridManager.lockAllGrid(false);
					}
					break;
				case PetEvent.PET_ATT_UPDATE_UI:			//更新PetAtt
					var petAttInfo:Object = notification.getBody();
					var attId:uint = petAttInfo.id;
					if(GameCommonData.Player.Role.UsingPet && GameCommonData.Player.Role.UsingPet.Id == attId) {
						GameCommonData.Player.Role.UsingPet = GameCommonData.Player.Role.PetSnapList[attId];
					}
					if(dataProxy.PetIsOpen)
						updatePetAtt(petAttInfo);
					break;
				case PetEvent.RETURN_TO_SHOW_PET_INFO:						//服务器返回详细宠物数据，显示到界面
					var petReturn:GamePetRole = notification.getBody() as GamePetRole;
					if(dataProxy.PetQuerySkill) {	//宠物学习(升级)技能时查询技能
						PetPropConstData.selectedPet = null;
						return;
					}
					if(petUIManager.countPetOthers() > 0) {		//查询其他玩家的宠物列表中的宠物数据
						if(!dataProxy.PetIsOpen) {
							GameCommonData.GameInstance.GameUI.addChild(panelBase);
//							panelBase.x = UIConstData.DefaultPos1.x;
//							panelBase.y = UIConstData.DefaultPos1.y;
							dataProxy.PetIsOpen = true;
						}
						PetPropConstData.selectedPet = null;
						dealOther(petReturn);
						
						if(PetPropConstData.isNewPetVersion)
						{
							PetPropConstData.isSearchOtherPetInto = true;
							PetPropConstData.newCurrentPet = petReturn;
							setViewState(true);
						}
						return;
					}
					if(!dataProxy.PetCanOperate || isQueryOtherOne) {
						PetPropConstData.selectedPet = null;
						if(iScrollPane && petView.contains(iScrollPane)) {
							petView.removeChild(iScrollPane);
							iScrollPane = null;
							listView = null;
						}
					}
					
					if(dataProxy.PetEudemonTmp && (petReturn.Id == dataProxy.PetEudemonTmp.Id)) {
						if(!dataProxy.PetIsOpen && dataProxy.PetCanOperate && GameCommonData.Player.Role.PetSnapList[petReturn.Id]) {
							return;
						}
						if(!dataProxy.PetIsOpen) {
							GameCommonData.GameInstance.GameUI.addChild(panelBase);
							if(PetPropConstData.isNewPetVersion)
							{
								sendNotification(PetEvent.LOOK_FOR_PET_DETAIL_INFO);
							}
							dataProxy.PetIsOpen = true;
						}
						if(dataProxy.PetEudemonTmp.Type) {
							petReturn.Type = dataProxy.PetEudemonTmp.Type;
							petReturn.PetName = dataProxy.PetEudemonTmp.PetName;
							petReturn.FaceType = dataProxy.PetEudemonTmp.FaceType;
							petReturn.Sex = dataProxy.PetEudemonTmp.Sex;
							petReturn.Character = dataProxy.PetEudemonTmp.Character;
						}
						if(dataProxy.PetCanOperate && GameCommonData.Player.Role.PetSnapList[petReturn.Id] && GameCommonData.Player.Role.PetSnapList[petReturn.Id].IsLock == false) {		// 可操作
							if(petReturn.Potential > 0) {	//有点
								petReturn.State == 0 ? petUIManager.setModel(1) : petUIManager.setModel(0);
							} else {						//有点
								petReturn.State == 0 ? petUIManager.setModel(3) : petUIManager.setModel(2);
							}
							if(GameCommonData.Player.Role.PetSnapList[petReturn.Id].State == 1) {
								petSkillGridManager.lockAllGrid(true);
							} else {
								petSkillGridManager.lockAllGrid(false);
							}
							petUIManager.showPetData(petReturn, 0);
						} else {							//不可操作
							var isSelfPet:Boolean = false;
							for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
								if(petReturn.Id == key) {
									isSelfPet = true;
									break;
								}
							}
							(isSelfPet == true) ? petUIManager.setModel(5) : petUIManager.setModel(4);
							petSkillGridManager.lockAllGrid(false);
							petUIManager.showPetData(petReturn, 1);
							PetPropConstData.selectedPet = null;
						}
						petSkillGridManager.removeAllItem();
						showPetSkill(petReturn);
						dataProxy.PetEudemonTmp = null;
						
					} else {
//						if(dataProxy.PetIsOpen) gcAll();
//						PetPropConstData.selectedPet = null;
					}
					if(PetPropConstData.isNewPetVersion)
					{
						PetPropConstData.newCurrentPet = petReturn;
						setViewState(true);
						if(PetPrivityMediator.isAddSend)
						{
							sendNotification(PetEvent.PET_PRIVITY_FEEDBACK);
						}
						else
						{
							sendNotification(PetEvent.CLOSE_PET_DETAIL_INFO);
						}
					}
					break;
				case PetEvent.PET_RENAME_FAIL:								//宠物改名失败
					var idFail:uint = uint(notification.getBody());
					if(PetPropConstData.selectedPet && PetPropConstData.selectedPet.Id == idFail) {
						if(GameCommonData.Player.Role.PetSnapList[idFail]) {
							petView.txtPetName.text = GameCommonData.Player.Role.PetSnapList[idFail].PetName;
						}
					} 
					break;
				case PetEvent.PET_UPDATE_SHOW_INFO:							//更新宠物信息   新增、更新
					if(dataProxy.PetIsOpen) {
						updatePetInfo();
					}
					break;
				case PetEvent.PET_DELETE_SUCCESS:							//删除宠物成功
					var idDel:uint = uint(notification.getBody());
					if(dataProxy.PetIsOpen && idDel > 0) {	//界面打开
						delPetSuccess();
					}
					break;
				case PetEvent.GET_PETINFO_OF_PLAYER:						//查询某人的宠物信息, 对外接口
					var idPlayer:uint = uint(notification.getBody());
					if(idPlayer > 0) {
						sendData(OperateItem.GETPETLIST_OFPLAYER, idPlayer);
					}
					break;
				case PetEvent.SHOW_PET_LIST_OF_PLAYER:						//服务器返回别人的宠物列表，显示
					if(petUIManager.countPetOthers() > 0) {
						initOtherView();
					}
					break;
				case PetEvent.PET_FEED_OUTSIDE_INTERFACE:					//喂食，对外接口
					if(GameCommonData.Player.Role.UsingPet) {
						sendData(PlayerAction.PET_EATHP, {petId:GameCommonData.Player.Role.UsingPet.Id});
					}
					break;
				case PetEvent.PET_TRAIN_OUTSIDE_INTERFACE:					//驯养，对外接口
					if(GameCommonData.Player.Role.UsingPet) {
						sendData(PlayerAction.PET_TRAIN, {petId:GameCommonData.Player.Role.UsingPet.Id});
					}
					break;
				case PetEvent.PET_COMEOUT_FIGHT_OUTSIDE_INTERFACE:			//出战，对外接口
					if(!GameCommonData.Player.Role.UsingPet) {
						var petIdToFight:uint = uint(notification.getBody());
						if(StallConstData.stallSelfId > 0) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_2" ], color:0xffff00});  // 摆摊中不能出战宠物
							return;
						}
						if(timer.running) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_3" ], color:0xffff00});  // 5秒后才可以再让宠物出战
							return;
						}
						//发送出战命令
						GameCommonData.Player.Role.UsingPet = GameCommonData.Player.Role.PetSnapList[petIdToFight];	//先存
						PetNetAction.opPet(PlayerAction.PET_GOTO_FIGHT, petIdToFight);
						GameCommonData.PetID = petIdToFight;
						timerOut.reset();
						timerOut.start();
					}
					break;
				case PetEvent.PET_REST_OUTSIDE_INTERFACE:					//休息，对外接口
					if(GameCommonData.Player.Role.UsingPet) {
						if(timerOut.running) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_4" ], color:0xffff00});  // 5秒后才可以再让宠物休息
							return;
						}
						PetNetAction.opPet(PlayerAction.PET_GOTO_REST, GameCommonData.Player.Role.UsingPet.Id);
						GameCommonData.PetID = 0;
						timer.reset();
						timer.start();		//10秒后才可以再招宠物
					}
					break;
				case PetEvent.PET_AUTO_REST_OUTSIDE_INTERFACE:				//休息，对外接口，直接休息，不受10秒时间限制
					if(GameCommonData.Player.Role.UsingPet) {
						PetNetAction.opPet(PlayerAction.PET_GOTO_REST, GameCommonData.Player.Role.UsingPet.Id);
						GameCommonData.PetID = 0;
						timer.reset();
						timer.start();		//10秒后才可以再招宠物
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_5" ], color:0xffff00});   // 宠物自动收回
					}
					break;
				case PetEvent.PET_LOOK_OUTSIDE_INTERFACE:					//查看某个宠物资料(别人的宠物)，对外接口:
					var queryInfo:Object = notification.getBody();
					if(queryInfo) {
						queryPet(queryInfo);
					}
					break;
				case BagEvents.EXTENDBAG:									//扩充宠物背包
					if(dataProxy && dataProxy.PetIsOpen && dataProxy.PetCanOperate) {
						petUIManager.updatePetNum();
					}
					break;
				case PetEvent.PETPROP_PANEL_STOP_DROG:						//禁止宠物面板拖动
					panelBase.IsDrag = false;
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}else{
						panelBase.x = UIConstData.DefaultPos1.x;
						panelBase.y = UIConstData.DefaultPos1.y;
					}
					break;
				case PetEvent.PETPROP_INIT_POS:								//宠物面板位置还原
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}else{
						panelBase.x = UIConstData.DefaultPos1.x;
						panelBase.y = UIConstData.DefaultPos1.y;
					}
					break;
			}
		}
		
		private function loadCallBack(mainView:DisplayObject,moreInfoView:DisplayObject,fantasyView:DisplayObject,privityview:DisplayObject,changeSexView:DisplayObject):void
		{
			this.setViewComponent(mainView);
			initView();
			petMoreInfoMediator = new PetMoreInfoMediator(moreInfoView);
			facade.registerMediator(petMoreInfoMediator);
			facade.registerMediator(new PetFantasyMediator(fantasyView));
			facade.registerMediator(new PetPrivityMediator(privityview));
			facade.registerMediator(new PetChangeSexMediator(changeSexView));
			_showPetModule = new ShowPetModuleComponent(backView);
			initPetUiManager();
		}
		
		private function initBackView():void
		{
			backView = new Sprite();
			backView.name = "backView";
			backView.graphics.lineStyle(0,0,0);
			backView.graphics.drawRect(0,0,300,300);
			backView.graphics.endFill();
			backView.x = 5;
			backView.y = 4;
			this.petView.addChildAt(backView,2);  
		}
		private function initPetUiManager():void
		{
			if(PetPropConstData.isNewPetVersion)
			{
				tempMC = petMoreInfoMediator.mainView;
			}
			else
			{
				tempMC = this.petView;
			}
			petUIManager = new PetUIManager(viewComponent as MovieClip, petSkillGridManager,tempMC,_showPetModule);
			petUIManager.setModel(0);
		}

		/** 查询宠物资料，对外接口 */
		private function queryPet(info:Object):void
		{
			var petId:uint = info.petId;
			var ownerId:uint = info.ownerId;
			if(petId > 0 && ownerId > 0) {
				dataProxy.PetEudemonTmp = new Object();
				dataProxy.PetEudemonTmp.Id = petId;
				PetPropConstData.isSearchOtherPetInto = true;	//查询他人宠物信息
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:petId, ownerId:ownerId});
				isQueryOtherOne = true;
			}
		}
		
		/** 显示其他玩家宠物列表的宠物数据 */
		private function dealOther(petReturn:GamePetRole):void
		{
			if(dataProxy.PetEudemonTmp && (petReturn.Id == dataProxy.PetEudemonTmp.Id)) {
				petReturn.Type = dataProxy.PetEudemonTmp.Type;
				petReturn.PetName = dataProxy.PetEudemonTmp.PetName;
				petReturn.FaceType = dataProxy.PetEudemonTmp.FaceType;
				petReturn.Sex = dataProxy.PetEudemonTmp.Sex;
				petReturn.Character = dataProxy.PetEudemonTmp.Character;
				petReturn.TakeLevel = dataProxy.PetEudemonTmp.TakeLevel;
				
				petUIManager.setModel(4);
				petUIManager.showPetData(petReturn, 1);
				petSkillGridManager.lockAllGrid(false);
				
				petSkillGridManager.removeAllItem();
				showPetSkill(petReturn);
			} else {
				if(dataProxy.PetIsOpen) gcAll();
			}
		}
			
		/** 其他玩家面板宠物列表 */
		private function initOtherView():void
		{
			petUIManager.setModel(4);
			petUIManager.clearAllData();
			initPetChoiceListOther();
		}
		
		/** 初始化其他玩家的宠物列表数据 */
		private function initPetChoiceListOther():void
		{
			if(iScrollPane && petView.contains(iScrollPane)) {
				petView.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			listView = new ListComponent(false);
			showFilterListOther();
			iScrollPane = new UIScrollPane(listView);
			if(PetPropConstData.isNewPetVersion)
			{
				iScrollPane.x = 114;
				iScrollPane.y = 263;
				iScrollPane.width = 110;
				iScrollPane.height = 115;
			}
			else
			{
				iScrollPane.x = 115;
				iScrollPane.y = 18;
				iScrollPane.width = 110;
				iScrollPane.height = 93;
			}
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_ALWAYS;
			iScrollPane.refresh();
			petView.addChild(iScrollPane);
			//////////////////
			var idQuery:uint = 0;
			for(var id:Object in PetPropConstData.petListOthers) {
				idQuery = uint(id);
				break;
			}
			if(idQuery > 0) {
				if(listView) {
					for(var i:int = 0; i < listView.numChildren; i++) {
						if(listView.getChildAt(i).name == "petPropList_"+idQuery) {
							(listView.getChildByName("petPropList_"+idQuery) as MovieClip).mcSelected.visible = true;
							break;
						}
					}
				}
				PetPropConstData.selectedPet = PetPropConstData.petListOthers[idQuery];
				dataProxy.PetEudemonTmp = PetPropConstData.petListOthers[idQuery];
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:idQuery, ownerId:PetPropConstData.petListOthers[idQuery].OwnerId});
				
				if(!dataProxy.PetIsOpen) {
					GameCommonData.GameInstance.GameUI.addChild(panelBase);
//					panelBase.x = UIConstData.DefaultPos1.x;
//					panelBase.y = UIConstData.DefaultPos1.y;
					dataProxy.PetIsOpen = true;
				}
			}
		}
		
		private function showFilterListOther():void
		{
			for(var id:Object in PetPropConstData.petListOthers)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petPropList_"+id;
				item.mcSelected.visible = false;
				item.txtName.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = false;
				item.txtName.text = PetPropConstData.petListOthers[id].PetName;
				item.addEventListener(MouseEvent.CLICK, selectItemOther);
				listView.addChild(item);
			}
			listView.width = 105;
			listView.upDataPos();
		}
		
		private function selectItemOther(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			
			if(PetPropConstData.selectedPet && id == PetPropConstData.selectedPet.Id) return;
			for(var i:int = 0; i < listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
//			PetPropConstData.selectedPet = PetPropConstData.petListOthers[id];
			dataProxy.PetEudemonTmp = PetPropConstData.petListOthers[id]	//PetPropConstData.selectedPet;
			//发送查询宠物信息命令  带id, 主人ID
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:PetPropConstData.petListOthers[id].OwnerId});
		}
		
		/** 初始化面板 */
		private function initView():void
		{
			panelBase = new PanelBase(petView, petView.width+8, petView.height+12);
			panelBase.name = "PetBag";
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_med_petp_ini_1" ] );   // 宠物背包
			gridSprite = new MovieClip();
			petView.addChild(gridSprite);
			if(PetPropConstData.isNewPetVersion)
			{
				gridSprite.x = 6
				gridSprite.y = 167;
			}
			else
			{
				gridSprite.x = 5;
				gridSprite.y = 282;
			}
			
			initGrid();
			initBackView();
		}
		
		/** 初始化宠物选择列表数据 */
		private function initPetChoiceListData():void
		{
			if(iScrollPane && petView.contains(iScrollPane)) {
				petView.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			if(dataProxy.PetCanOperate) {
				listView = new ListComponent(false);
				showFilterList();
				iScrollPane = new UIScrollPane(listView);
				if(PetPropConstData.isNewPetVersion)
				{
					iScrollPane.x = 114;
					iScrollPane.y = 263;
					iScrollPane.width = 110;
					iScrollPane.height = 115;
				}
				else
				{
					iScrollPane.x = 115;
					iScrollPane.y = 18;
					iScrollPane.width = 110;
					iScrollPane.height = 93;
				}
				iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_ALWAYS;
				iScrollPane.refresh();
				petView.addChild(iScrollPane);
			}
		}
		
		private function showFilterList():void
		{
			var count:uint = 0;
			for(var id:Object in GameCommonData.Player.Role.PetSnapList)
			{
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
				if(pet.IsLock == true) continue;		//摆摊中锁定的宠物不显示
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
				item.name = "petPropList_"+id;
				item.mcSelected.visible = false;
				item.txtName.mouseEnabled = false;
				item.btnChosePet.mouseEnabled = false;
				/*var nameStr:String;
				 if(pet.State == 4)
				{
					nameStr = "<font color='#ff0000'>" + pet.PetName + "</font>";
				}
				else
				{
					nameStr = pet.PetName;;
				}
				item.txtName.htmlText = nameStr; */
				item.txtName.htmlText = pet.PetName;
				item.addEventListener(MouseEvent.CLICK, selectItem);
//				item.addEventListener(MouseEvent.ROLL_OVER, itemOverHandler);
//				item.addEventListener(MouseEvent.ROLL_OUT, itemOutHandler);
				listView.addChild(item);
				count++;
			}
			if(count == 0) petUIManager.updatePetNum();
			listView.width = 105;
			listView.upDataPos();
		}
		
		private function selectItem(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(event.ctrlKey) {  
				var type:uint = GameCommonData.Player.Role.PetSnapList[id].ClassId;
				var name:String = GameCommonData.Player.Role.PetSnapList[id].PetName;
				if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
					facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<2_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_0_4>");
				} else {
					facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<2_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_0_4>");
				}
			}
			if(PetPropConstData.selectedPet && id == PetPropConstData.selectedPet.Id) return;
			for(var i:int = 0; i < listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			PetPropConstData.selectedPet = GameCommonData.Player.Role.PetSnapList[id];
			dataProxy.PetEudemonTmp = PetPropConstData.selectedPet;
			//更换宠物数据
//			if(GameCommonData.Player.Role.PetList[id]) {	//本地有缓存数据
//				PetPropConstData.selectedPet = GameCommonData.Player.Role.PetList[id];
//				sendNotification(PetEvent.LOOKPETINFO_OBJ, PetPropConstData.selectedPet);
//			} else {
			petUIManager.setModel(4);
			petUIManager.clearAllData();
			//发送查询宠物信息命令  带id
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
//			}
		}
		//		private function itemOverHandler(event:MouseEvent):void
//		{
//			var item:MovieClip = event.currentTarget as MovieClip;
//			var id:uint = uint(item.name.split("_")[1]);
//			if(!item.mcSelected.visible && PetPropConstData.selectedPet && PetPropConstData.selectedPet.Id != id) {
//				item.mcSelected.visible = true;
//			}
//		}
//		
//		private function itemOutHandler(event:MouseEvent):void
//		{
//			var item:MovieClip = event.currentTarget as MovieClip;
//			var id:uint = uint(item.name.split("_")[1]);
//			if(item.mcSelected.visible && PetPropConstData.selectedPet && PetPropConstData.selectedPet.Id != id) {
//				item.mcSelected.visible = false;
//			}
//		}
		
		/** 初始化格子 */
		private function initGrid():void
		{
			for(var i:int = 0; i < 12; i++) {
				var gridUnit:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
				gridUnit.x = (gridUnit.width) * (i%6);
				gridUnit.y = (gridUnit.height) * int(i/6);
				gridUnit.name = "petPropSkillShow_" + i.toString();
				gridSprite.addChild(gridUnit);								//添加到画面
				var gridUint:GridUnit = new GridUnit(gridUnit, true);
				gridUint.parent = gridSprite;								//设置父级
				gridUint.Index = i;											//格子的位置
				gridUint.HasBag = true;										//是否是可用的背包
				gridUint.IsUsed	= false;									//是否已经使用
				gridUint.Item	= null;										//格子的物品
				PetPropConstData.GridUnitList.push(gridUint);
			}
			petSkillGridManager = new PetSkillGridManager(PetPropConstData.GridUnitList, gridSprite);
			facade.registerProxy(petSkillGridManager);
		}
		
		/** 显示宠物技能 */
		private function showPetSkill(pet:GamePetRole):void
		{
			PetPropConstData.gridSkillList = pet.SkillLevel;
			petSkillGridManager.showItems(PetPropConstData.gridSkillList);
		}
		
		/** 显示面板 */
		private function showPetView():void
		{
			petUIManager.setModel(4);
			petUIManager.clearAllData();
			initPetChoiceListData();
			//////////////////////////////
			//自己查看自己的宠物属性，可看
			if(!GameCommonData.GameInstance.GameUI.contains(panelBase)) { 
				var idQuery:uint = 0;
				for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
					if(GameCommonData.Player.Role.PetSnapList[key].State == 1 && GameCommonData.Player.Role.PetSnapList[key].IsLock == false) { 
						idQuery = uint(key);
						break;
					}
				}
				if(idQuery == 0) {
					for(var id:Object in GameCommonData.Player.Role.PetSnapList) {
						if(GameCommonData.Player.Role.PetSnapList[id].IsLock == false) {
							idQuery = uint(id);
							break;
						}
					}
				}
				if(idQuery > 0) {
					if(listView) {
						for(var i:int = 0; i < listView.numChildren; i++) {
							if(listView.getChildAt(i).name == "petPropList_"+idQuery) {
								(listView.getChildByName("petPropList_"+idQuery) as MovieClip).mcSelected.visible = true;
								break;
							}
						}
					}
					PetPropConstData.selectedPet = GameCommonData.Player.Role.PetSnapList[idQuery];
					dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[idQuery];
					sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:idQuery, ownerId:GameCommonData.Player.Role.Id});
				}
				if( GameCommonData.fullScreen == 2 )
				{
					panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}else{
					panelBase.x = UIConstData.DefaultPos1.x;
					panelBase.y = UIConstData.DefaultPos1.y;
				}
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
			}
			if(PetPropConstData.isNewPetVersion)
			{
				setViewState(false);
			}
		}
		
		/** 点击按钮 修改、喂食、驯养、放生、出战、确定，合体，查看更多信息*/
		private function btnClickHandler(e:MouseEvent):void
		{
			var boo:Boolean;
			if(!PetPropConstData.selectedPet)
			{
				if(PetPropConstData.isNewPetVersion && PetPropConstData.newCurrentPet)
				{
					boo = true;
				}
			}
			else
			{
				boo = true;
			}
			if(!boo) return;
			handlPotential(e.target.name);
		}
		/** 点击加点 */
		private function addPotential(e:MouseEvent):void
		{
			if(!PetPropConstData.selectedPet) return;
			(e.currentTarget as DisplayObject).addEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			intervalId = setTimeout(addAll , 1000 * 2 , e.target.name);
		}
		/** 点击减点 */
		private function subPotential(e:MouseEvent):void
		{
			if(!PetPropConstData.selectedPet) return;
			(e.currentTarget as DisplayObject).addEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			intervalId = setTimeout(subAll , 1000 * 2 , e.target.name);
		}
		/** 鼠标弹起 */
		private function stopAddPotential(e:MouseEvent):void
		{
			clearTimeout(intervalId);
			handlPotential(e.target.name);
		}
		/** 点数全加满 */
		private function addAll(name:String):void
		{
			var total:int = PetPropConstData.potentials;
			for(var i:int = 0; i < total; i++)
			{
				handlPotential(name);
			}
		}
		/** 点数全减完 */
		private function subAll(name:String):void
		{
			var total:int;
			switch(name)
			{
				case "btnSubStrong":															//力量 减点
					total = PetPropConstData.points[0];
				break;
				case "btnSubSprite":															//灵力 减点
					total = PetPropConstData.points[1];
				break;
				case "btnSubPhysical":															//体力 减点
					total = PetPropConstData.points[2];
				break;
				case "btnSubConstant":															//定力 减点
					total = PetPropConstData.points[3];
				break;
				case "btnSubMagic":																//身法 减点
					total = PetPropConstData.points[4];
				break;
			}
			for(var i:int = 0; i < total; i++)
			{
				handlPotential(name);
			}
		}
		/** 给宠物加点 */
		private function handlPotential(name:String):void
		{
			switch(name) {
				case "btnModifyName":															//修改名字
					var name:String = petView.txtPetName.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
					if(name) {
						if(name != PetPropConstData.selectedPet.PetName) {
							if(!UIUtils.isPermitedPetName(name)) {
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_1" ], color:0xffff00});   //  宠物名字不合法
								petView.txtPetName.text = PetPropConstData.selectedPet.PetName;
								return;
							}
							sendData(PlayerAction.PET_RENAME, {petId:PetPropConstData.selectedPet.Id, petName:name});
						}
					} else {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_2" ], color:0xffff00});    //  宠物名字不能为空
						petView.txtPetName.text = PetPropConstData.selectedPet.PetName;
					}
					break;
				case "btnFeed":																	//喂食
					if(GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id]) {
						if(GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id].HpNow == GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id].HpMax) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_3" ], color:0xffff00});   //  宠物生命是满的
						} else {
							sendData(PlayerAction.PET_EATHP, {petId:PetPropConstData.selectedPet.Id});
						}
					}
					break;
				case "btnTraining":																//驯养
					if(GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id]) {
						if(GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id].HappyNow == GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id].HappyMax) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_4" ], color:0xffff00});    //  宠物快乐值是满的
						} else {
							sendData(PlayerAction.PET_TRAIN, {petId:PetPropConstData.selectedPet.Id});
						}
					}
					break;
				case "btnAlive":																//放生
					if(PetPropConstData.selectedPet) {
						if(PetPropConstData.isNewPetVersion)
						{
							if(PetPropConstData.newCurrentPet.State == 4)		
							{
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_PetPro_word_1" ], color:0xffff00});    //  附体状态的宠物，不能放生
								return;
							}
						}
						if(GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id].State == 1) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_5" ], color:0xffff00});    //  出战状态下无法放生
						} else {
							facade.sendNotification(EventList.SHOWALERT, { comfrim:applyAlive, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_6" ] });   //  你确定要放生此宠物吗？
						}
					}
					break;
				case "btnState":																//出战/休息
					if(petView.txtState.text ==  GameCommonData.wordDic[ "mod_pet_med_petp_upd_1" ] ) {    //  出战
						if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.PET_FIGHT_NOTICE_NEWER_HELP);
						if(StallConstData.stallSelfId > 0) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_2" ], color:0xffff00});    //  摆摊中不能出战宠物
							return;
						}
						for(var id:Object in GameCommonData.Player.Role.PetSnapList) {
							var pet1:Object = GameCommonData.Player.Role.PetSnapList[id];
							var pet:Object = PetPropConstData.selectedPet;
							if(id != PetPropConstData.selectedPet.Id && GameCommonData.Player.Role.PetSnapList[id].State == 1) {
								facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_hand_7" ], color:0xffff00});   //  请先让出战中的宠物休息
								return;
							}
						}
						if(timer.running) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_3" ], color:0xffff00});    //   5秒后才可以再让宠物出战
							return;
						}
						//发送出战命令
						GameCommonData.Player.Role.UsingPet = GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id];	//先存
						PetNetAction.opPet(PlayerAction.PET_GOTO_FIGHT, PetPropConstData.selectedPet.Id);
						GameCommonData.PetID = PetPropConstData.selectedPet.Id;
						timerOut.reset();
						timerOut.start();
//						petView.txtState.text = "休息";
					} else {
						//发送休息命令
						if(timerOut.running) {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petp_han_4" ], color:0xffff00});    //  5秒后才可以再让宠物休息
							return;
						}
						PetNetAction.opPet(PlayerAction.PET_GOTO_REST, PetPropConstData.selectedPet.Id);
						GameCommonData.PetID = 0;
						timer.reset();
						timer.start();		//10秒后才可以再招宠物
//						petView.txtState.text = "出战";
					}
					break;
				case "btnSure":																	//确定加点
					var canSendSure:uint;
					for(var i:int = 0; i < 5; i++) {
						if(PetPropConstData.points[i] == 0) {
							canSendSure++;
						}
					}
					if(canSendSure < 5) {
						sendData(PlayerAction.PET_ADDPOINTS, {petId:PetPropConstData.selectedPet.Id, points:PetPropConstData.points});
						if(NewerHelpData.newerHelpIsOpen) {
							sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 6);
							sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 34);
							sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 35);
						}
					}
					break;
				case "btnExtLife":																//延寿
					if(PetPropConstData.selectedPet) {
						if(GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id]) {
							sendData(PlayerAction.PET_EXT_LIFE, {petId:PetPropConstData.selectedPet.Id});
						}
					}
					break;
				case "btnAddStrong":															//力量 加点
					addPoint(0);
					break;
				case "btnSubStrong":															//力量 减点
					subPoint(0);
					break;
				case "btnAddSprite":															//灵力 加点
					addPoint(1);
					break;
				case "btnSubSprite":															//灵力 减点
					subPoint(1);
					break;
				case "btnAddPhysical":															//体力 加点
					addPoint(2);
					break;
				case "btnSubPhysical":															//体力 减点
					subPoint(2);
					break;
				case "btnAddConstant":															//定力 加点
					addPoint(3);
					break;
				case "btnSubConstant":															//定力 减点
					subPoint(3);
					break;
				case "btnAddMagic":																//身法 加点
					addPoint(4);
					break;
				case "btnSubMagic":																//身法 减点
					subPoint(4);
					break;
				case "btnMorePetInfo":
					sendNotification(PetEvent.LOOK_FOR_PET_DETAIL_INFO);
					break;
				case "btnDesignation":
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_4" ], color:0xffff00});//"此功能暂未开放"
				break;
				case "btnTurn_left":	//左转
					_showPetModule.turnLeft();
				break;
				case "btnTurn_right":	//右转
					_showPetModule.turnRight();
				break;
				case "btnDependence":	//附体	
					sendDependence();
				break;
				case "btnFantasy":	//幻化
					sendNotification(PetEvent.SHOW_PET_FANTASY_VIEW);
				break;	
				case "btn_addPrivity":	//增加默契
					sendNotification(PetEvent.SHOW_PET_PRIVITY_VIEW);
				break;
				case "btn_changeSex":	//变性
					sendNotification(PetEvent.SHOW_PET_CHANGE_SEX_VIEW);
				break;
			}
		}
		
		/**
		 *宠物附体（分离） 
		 * 
		 */		
		private function sendDependence():void
		{
			var pet:GamePetRole = PetPropConstData.selectedPet;
			var currentPet:GamePetRole = PetPropConstData.newCurrentPet;
			if(GameCommonData.Player.Role.Level < 18)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPro_sendDen_1"], color:0xffff00});//"您的等级未满18级，还不能让宠物附体"
				return;
			}
			if(currentPet.State == 1)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPro_sendDen_2"], color:0xffff00});	//"出战状态的宠物不能附体"
				return;
			}
			if(currentPet.LifeNow <= 0)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_med_petPro_sendDen_3"], color:0xffff00});	//"宠物寿命为0时不能附体，请补充它的寿命"
				return;
			}
			var tag:int;
			if(currentPet.State == 4)
			{
				tag = PlayerAction.NEWPET_CANCEL_DEPENDENCE_TAG;
			}
			else
			{
				tag = PlayerAction.NEWPET_DEPENDENCE_TAG;
			}
			if(getTimer() - dependenceCdTime < 5000)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_PetPro_word_2" ], color:0xffff00});	//请于5秒后做此操作
				return;
			}
			dependenceCdTime = getTimer();
			isSendDependence = true;
			PetNetAction.newPetOperate(tag, currentPet.Id, 0,0);
		}
		
		
		/** 确定放生此宠物 */
		private function applyAlive():void
		{
			if(PetPropConstData.selectedPet) {
				sendData(PlayerAction.PET_DEOP, {petId:PetPropConstData.selectedPet.Id});
			}
		}
		
		/** 取消放生 */
		private function cancelClose():void
		{
			
		}
		
		/** 关闭按钮 */
		private function panelCloseHandler(event:Event):void
		{
			gcAll();
		}
		
		private function addLis():void
		{
			petView.btnModifyName.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnFeed.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnTraining.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petView.btnAlive.addEventListener(MouseEvent.CLICK, btnClickHandler);
			tempMC.btnAlive.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnState.addEventListener(MouseEvent.CLICK, btnClickHandler);
//			petView.btnExtLife.addEventListener(MouseEvent.CLICK, btnClickHandler);
			tempMC.btnExtLife.addEventListener(MouseEvent.CLICK, btnClickHandler);
			
			petView.btnSure.addEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnAddStrong.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubStrong.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddSprite.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubSprite.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddPhysical.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubPhysical.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddConstant.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubConstant.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddMagic.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubMagic.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.txtPetName.addEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			petView.txtPetName.addEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			petView.txtPetName.addEventListener(Event.CHANGE, nameInputHandler);
			if(PetPropConstData.isNewPetVersion)
			{
				petView.btnMorePetInfo.addEventListener(MouseEvent.CLICK, btnClickHandler);	//查看更多信息
				petView.btnTurn_left.addEventListener(MouseEvent.CLICK, btnClickHandler);	//左转
				petView.btnTurn_right.addEventListener(MouseEvent.CLICK, btnClickHandler);	//右转
				petView.btnDependence.addEventListener(MouseEvent.CLICK, btnClickHandler);	//合成
				petView.btnFantasy.addEventListener(MouseEvent.CLICK, btnClickHandler);	//合成
				petView.btn_addPrivity.addEventListener(MouseEvent.CLICK, btnClickHandler);	//合成
				petView.btn_changeSex.addEventListener(MouseEvent.CLICK, btnClickHandler);	//变性
				tempMC.btnDesignation.addEventListener(MouseEvent.CLICK, btnClickHandler);
			}
		}
		
		private function removeLis():void
		{
			petView.btnModifyName.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnFeed.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnTraining.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petView.btnAlive.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			tempMC.btnAlive.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnState.removeEventListener(MouseEvent.CLICK, btnClickHandler);
//			petView.btnExtLife.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			tempMC.btnExtLife.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			
			petView.btnSure.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnAddStrong.removeEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubStrong.removeEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddSprite.removeEventListener(MouseEvent.CLICK, addPotential);
			petView.btnSubSprite.removeEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddPhysical.removeEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubPhysical.removeEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			petView.btnAddConstant.removeEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			petView.btnSubConstant.removeEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			
			petView.btnAddStrong.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnSubStrong.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnAddSprite.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnSubSprite.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnAddPhysical.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnSubPhysical.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnAddConstant.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			petView.btnSubConstant.removeEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			
			petView.btnAddMagic.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			petView.btnSubMagic.removeEventListener(MouseEvent.CLICK, btnClickHandler);
			
			petView.txtPetName.removeEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			petView.txtPetName.removeEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
			petView.txtPetName.removeEventListener(Event.CHANGE, nameInputHandler);
			if(PetPropConstData.isNewPetVersion)
			{
				petView.btnTurn_left.removeEventListener(MouseEvent.CLICK, btnClickHandler);	//左转
				petView.btnTurn_right.removeEventListener(MouseEvent.CLICK, btnClickHandler);	//右转
				petView.btnDependence.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				petView.btnMorePetInfo.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				petView.btnFantasy.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				petView.btn_addPrivity.removeEventListener(MouseEvent.CLICK, btnClickHandler);
				petView.btn_changeSex.removeEventListener(MouseEvent.CLICK, btnClickHandler);	//变性
				tempMC.btnDesignation.removeEventListener(MouseEvent.CLICK, btnClickHandler);
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
		
		/** 检测输入的宠物名 */
		private function nameInputHandler(e:Event):void
		{
			var name:String = petView.txtPetName.text.replace(/^\s*|\s*$/g,"").split(" ").join("");
			if(name) {
				petView.txtPetName.text = UIUtils.getTextByCharLength(name, 12);
			}
		}
		
		/** gc */
		private function gcAll():void
		{
			removeLis();
			panelBase.IsDrag = true;
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			petSkillGridManager.removeAllItem();
			PetPropConstData.gridSkillList = [];
			isQueryOtherOne = false;
			PetPropConstData.isSearchOtherPetInto = false;
			PetPropConstData.SelectedItem = null;
			PetPropConstData.selectedPet = null;
			PetPropConstData.petListOthers = new Dictionary();
			PetPropConstData.potentials = 0;
			PetPropConstData.points = [0,0,0,0,0];
			dataProxy.PetIsOpen = false;
			if(NewerHelpData.newerHelpIsOpen)	//通知新手指导系统
				sendNotification(NewerHelpEvent.CLOSE_PETPROP_NOTICE_NEWER_HELP);
			sendNotification(PetEvent.CLOSE_PET_DETAIL_INFO);
			if(_showPetModule)
			{
				_showPetModule.deleteView();
			} 
		}
		
		/** 加点  0-力量、1-灵力、2-体力、3-定力、4-身法 */
		private function addPoint(type:int):void
		{
			if(PetPropConstData.potentials > 0) {	//有潜力点
				switch(type) {
					case 0:
						PetPropConstData.points[0]++;
						petView.txtForce.text = (uint(petView.txtForce.text)+1).toString();
						break;
					case 1:
						PetPropConstData.points[1]++;
						petView.txtSpiritPower.text = (uint(petView.txtSpiritPower.text)+1).toString();
						break;
					case 2:
						PetPropConstData.points[2]++;
						petView.txtPhysical.text = (uint(petView.txtPhysical.text)+1).toString();
						break;
					case 3:
						PetPropConstData.points[3]++;
						petView.txtConstant.text = (uint(petView.txtConstant.text)+1).toString();
						break;
					case 4:
						PetPropConstData.points[4]++;
						petView.txtMagic.text = (uint(petView.txtMagic.text)+1).toString();
						break;
				}
				PetPropConstData.potentials--;
				petView.txtPotential.text = (PetPropConstData.potentials <=0) ? "" : PetPropConstData.potentials.toString();
			}
		}
		
		/**
		 *幻化后将幻化按钮消失 
		 * 
		 */		
		private function setViewState(isShowPet:Boolean):void
		{
			if(!isShowPet)
			{
				petView.txtFantasy.visible = false;
				petView.btnFantasy.visible = false;
				petView.txtSavvyName.x = 226;
				petView.txtSavvyName.y = 7;
				petView.txtSavvy.x = 264;
				petView.txtSavvy.y = 7;
				petView.btn_addPrivity.visible = false;
				petView.btnMorePetInfo.mouseEnabled = false;
				MasterData.setGrayFilter(petView.btnMorePetInfo);
				petView.btnTurn_right.mouseEnabled = false;
				MasterData.setGrayFilter(petView.btnTurn_right);
				petView.btnTurn_left.mouseEnabled = false;
				MasterData.setGrayFilter(petView.btnTurn_left);
				petView.btn_changeSex.mouseEnabled = false;
				MasterData.setGrayFilter(petView.btn_changeSex);
			}
			else
			{
				var currentPet:GamePetRole = PetPropConstData.newCurrentPet;
				if(!currentPet) return;
				var isFantasy:Boolean = currentPet.isFantasy;
				petView.txtFantasy.visible = !isFantasy;
				petView.btnFantasy.visible = !isFantasy;
				setNewPropertyVisible(isFantasy);
				petView.btn_addPrivity.visible = isFantasy;
				petView.txtWinning.text = currentPet.winning;
				petView.txtPrivity.text = currentPet.privity;
				petView.btn_changeSex.visible = !isFantasy;
				petView.btnMorePetInfo.mouseEnabled = true;
				petView.btnMorePetInfo.filters = null;
				petView.btnTurn_right.mouseEnabled = true;
				petView.btnTurn_right.filters = null;
				petView.btnTurn_left.mouseEnabled = true;
				petView.btnTurn_left.filters = null;
				petView.btn_changeSex.mouseEnabled = true;
				petView.btn_changeSex.filters = null;
				if(currentPet.Type < 2)
				{
//					petView.txtCanBreed.text = 3;
					currentPet.BreedMax = 3;
				}
				else	//二代 或 幻化宠物 不可再繁殖
				{ 
//					petView.txtCanBreed.text = 0;
					currentPet.BreedMax = 0;
				}
				if(isFantasy)
				{
					petView.txtSavvyName.x = 226;
					petView.txtSavvy.x = 259;
					petView.txtSex.htmlText = '<font color="'+IntroConst.itemColors[5]+'">'+GameCommonData.wordDic[ "mod_pet_med_petPro_setView_1"]+'</font>';//幻兽
//					petView.txtCanBreed.text = 0;
					currentPet.BreedMax = 0;
				}
				else
				{
					petView.txtSavvyName.x = 251;
					petView.txtSavvy.x = 284;
					var sexStr:String = (currentPet.Sex == 0) ? GameCommonData.wordDic[ "mod_pet_med_petPro_setView_2"]:GameCommonData.wordDic[ "mod_pet_med_petPro_setView_3"];//"雄":"雌";    
					petView.txtSex.htmlText = '<font color="#00FFFF">'+sexStr+'</font>'; 
				}
				petView.txtHadBreed.text = currentPet.BreedNow + "/" + currentPet.BreedMax;
				petView.txtPlayNum.text = currentPet.playNumber;
				if(currentPet.State == 4)	//4 附体状态
				{
					(petView.btnDependence as SimpleButton).mouseEnabled = true;
					(petView.btnDependence as DisplayObject).filters = null;
					petView.txtDependence.text = GameCommonData.wordDic[ "mod_pet_med_petPro_setView_4"];//"分离";
					
					seBtnDependenceState(true);
					petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_1" ];//"出战"; 
					petView.btn_addPrivity.visible = false;
					
				}
				else if(currentPet.State == 1)
				{
					MasterData.setGrayFilter(petView.btnDependence);
					(petView.btnDependence as SimpleButton).mouseEnabled = false;
					petView.txtDependence.text = GameCommonData.wordDic[ "mod_pet_med_petPro_setView_5"];//"附体";
					seBtnDependenceState(false);
					setBtnState(true); 
					petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_2" ];//"休息"; 
					
				}
				else if(currentPet.State == 0 || currentPet.State == 5)
				{
					(petView.btnDependence as DisplayObject).filters = null;
					(petView.btnDependence as SimpleButton).mouseEnabled = true;
					petView.txtDependence.text = GameCommonData.wordDic[ "mod_pet_med_petPro_setView_5"];//"附体";
					seBtnDependenceState(false);
					setBtnState(false);
					petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_1" ];//"出战"; 
					
				}
			}
			setLookOtherPetViewState();//是否查询其他玩家（或从其他面板查看宠物）的宠物，设置面板状态
			petView.txtPetIntro_8.x = petView.txtSavvyName.x;
			if(!listView) return;
			var childNum:int = listView.numChildren;
			var tagId:int;
			var pet:GamePetRole;
			for each(pet in GameCommonData.Player.Role.PetSnapList)
			{
				if(pet.IsLock == true) continue;		//摆摊中锁定的宠物不显示
				if(pet.State == 4)
				{
					tagId = pet.Id;
					break;
				}
			}
			for(var i:int = 0; i < childNum; i ++)
			{
				var item:MovieClip = listView.getChildAt(i) as MovieClip;
				if(item.name.split("_")[1] == tagId)
				{
					item.txtName.htmlText = "<font color='#ff0000'>" + pet.PetName + "</font>";
				}
				else
				{
					item.txtName.text = item.txtName.text;
				}
			}
		}
		
		private function setLookOtherPetViewState():void
		{
			var _w:int;
			var _x:int;
			if(PetPropConstData.isSearchOtherPetInto)
			{
				_w = 223;
				_x = 84;
				petView.txtPetName.x = _x;
				petView.nameContainer.width = 181;
				petView.txtSavvyName.x = 226;
				petView.txtSavvy.x = 259;
				PetPropConstData.isSearchOtherPetInto = false;
				petView.btn_changeSex.visible = false;
				petView.btnMorePetInfo.visible = true;
				petView.txtMorePetInfo.visible = true;
				petView.btnFantasy.visible = false;
				petView.txtFantasy.visible = false;
				petView.btn_addPrivity.visible = false;
				petView.btnTurn_left.addEventListener(MouseEvent.CLICK, btnClickHandler);	//合成
				petView.btnTurn_right.addEventListener(MouseEvent.CLICK, btnClickHandler);	//变性
				petView.btnMorePetInfo.addEventListener(MouseEvent.CLICK, btnClickHandler);	
				setNewPropertyVisible(true);
			}
			else
			{
				_w = 181; 
				_x = 54;
				petView.txtPetName.x = 36;
				petView.nameContainer.width = 97;
			}
			petView.nameContainerAdd.x = petView.nameContainer.x + petView.nameContainer.width;
			petView.txtExp.x = _x;
			petView.txtHappy.x = _x;
			petView.txtHp.x = _x;
			petView.container1.width = _w;
			petView.container2.width = _w;
			petView.container3.width = _w;
		}
		
		private function setNewPropertyVisible(boo:Boolean):void
		{
			petView.txtWinningName.visible = boo;
			petView.txtWinning.visible = boo;
			petView.txtPetIntro_11.visible = boo;
			petView.txtPrivityName.visible = boo;
			petView.txtPrivity.visible = boo;
			petView.txtPetIntro_12.visible = boo;
		}
		
		private function seBtnDependenceState(isDependence:Boolean):void
		{
			if(isDependence)
			{
				MasterData.setGrayFilter(petView.btnFantasy);
				petView.btnFantasy.mouseEnabled = false;
				
				MasterData.setGrayFilter(petView.btnState);
				(petView.btnState as SimpleButton).mouseEnabled = false;
				
				MasterData.setGrayFilter(petView.btnTraining);
				(petView.btnTraining as SimpleButton).mouseEnabled = false;
				MasterData.setGrayFilter(petView.btnFeed);
				(petView.btnFeed as SimpleButton).mouseEnabled = false;
				MasterData.setGrayFilter(petView.btnModifyName);
				(petView.btnModifyName as SimpleButton).mouseEnabled = false;
				MasterData.setGrayFilter(petView.btn_changeSex);
				petView.btn_changeSex.mouseEnabled = false;
			}
			else
			{
				petView.btnFantasy.filters = null;
				petView.btnFantasy.mouseEnabled = true;
				
				(petView.btnState as SimpleButton).filters = null;
				(petView.btnState as SimpleButton).mouseEnabled = true;
				
				petView.btnTraining.filters = null;
				(petView.btnTraining as SimpleButton).mouseEnabled = true;
				petView.btnFeed.filters = null;
				(petView.btnFeed as SimpleButton).mouseEnabled = true;
				petView.btnModifyName.filters = null;
				(petView.btnModifyName as SimpleButton).mouseEnabled = true;
				petView.btn_changeSex.filters = null;
				(petView.btn_changeSex as SimpleButton).mouseEnabled = true;
			}
			
		}
		private function setBtnState(isFightingState:Boolean):void
		{
			if(isFightingState)
			{
				MasterData.setGrayFilter(petView.btnFantasy);
				petView.btnFantasy.mouseEnabled = false;
				MasterData.setGrayFilter(petView.btn_changeSex);
				petView.btn_changeSex.mouseEnabled = false;
			}
			else
			{
				petView.btnFantasy.filters = null;
				petView.btnFantasy.mouseEnabled = true;
				petView.btn_changeSex.filters = null;
				(petView.btn_changeSex as SimpleButton).mouseEnabled = true;
			}
		}
		/** 减点  0-力量、1-灵力、2-体力、3-定力、4-身法 */
		private function subPoint(type:int):void
		{
			switch(type) {
				case 0: 
					if(PetPropConstData.points[0] > 0) {
						PetPropConstData.points[0]--;
						petView.txtForce.text =  (uint(petView.txtForce.text)-1).toString();
					} else {
						return;
					}
					break;
				case 1:
					if(PetPropConstData.points[1] > 0) {
						PetPropConstData.points[1]--;
						petView.txtSpiritPower.text =  (uint(petView.txtSpiritPower.text)-1).toString();
					} else {
						return;
					}
					break;
				case 2:
					if(PetPropConstData.points[2] > 0) {
						PetPropConstData.points[2]--;
						petView.txtPhysical.text =  (uint(petView.txtPhysical.text)-1).toString();
					} else {
						return;
					}
					break;
				case 3:
					if(PetPropConstData.points[3] > 0) {
						PetPropConstData.points[3]--;
						petView.txtConstant.text = (uint(petView.txtConstant.text)-1).toString();
					} else {
						return;
					}
					break;
				case 4:
					if(PetPropConstData.points[4] > 0) {
						PetPropConstData.points[4]--;
						petView.txtMagic.text = (uint(petView.txtMagic.text)-1).toString();
					} else {
						return;
					}
					break;
			}
			PetPropConstData.potentials++;
			petView.txtPotential.text = PetPropConstData.potentials.toString();
		}
		
		/** 更新画面信息 */
		private function updatePetInfo():void
		{
			initPetChoiceListData();
			var idQuery:uint = 0;
			if(PetPropConstData.selectedPet && PetPropConstData.selectedPet.Id && GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id] && GameCommonData.Player.Role.PetSnapList[PetPropConstData.selectedPet.Id].IsLock == false) {	//优先显示操作中的宠物
				idQuery = PetPropConstData.selectedPet.Id;
			}
			if(idQuery == 0) {
				for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
					if(GameCommonData.Player.Role.PetSnapList[key].State == 1 && GameCommonData.Player.Role.PetSnapList[key].IsLock == false) {	//第二优先显示出战的宠物
						idQuery = uint(key);
						break;
					}
				}
			}
			if(idQuery == 0) {
				for(var id:Object in GameCommonData.Player.Role.PetSnapList) {		//最后显示列表中第一个宠物
					if(GameCommonData.Player.Role.PetSnapList[id].IsLock == false) {
						idQuery = uint(id);
						break;
					}
				}
			}
			if(idQuery == 0) {
				return;
			}
			PetPropConstData.selectedPet = GameCommonData.Player.Role.PetSnapList[idQuery];
			dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[idQuery];
			if(listView) {
				for(var i:int = 0; i < listView.numChildren; i++) {
					if(listView.getChildAt(i).name == "petPropList_"+idQuery) {
						(listView.getChildByName("petPropList_"+idQuery) as MovieClip).mcSelected.visible = true;
						break;
					}
				}
			}
			sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:idQuery, ownerId:GameCommonData.Player.Role.Id});
			var count:int;
			petUIManager.updatePetNum();
		}
		
		/** 删除宠物成功 */
		private function delPetSuccess():void
		{
			////////////////////////////
			petUIManager.setModel(4);
			petSkillGridManager.removeAllItem();
			petUIManager.clearAllData();
			initPetChoiceListData();
			var idQuery:uint = 0;
			for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
				if(GameCommonData.Player.Role.PetSnapList[key].State == 1 && GameCommonData.Player.Role.PetSnapList[key].IsLock == false) { 
					idQuery = uint(key);
					break;
				}
			}
			if(idQuery == 0) {
				for(var id:Object in GameCommonData.Player.Role.PetSnapList) {
					if(GameCommonData.Player.Role.PetSnapList[key].IsLock == false) {
						idQuery = uint(id);
						break;
					}
				}
			}
			if(idQuery > 0) {
				if(listView) {
					for(var i:int = 0; i < listView.numChildren; i++) {
						if(listView.getChildAt(i).name == "petPropList_"+idQuery) {
							(listView.getChildByName("petPropList_"+idQuery) as MovieClip).mcSelected.visible = true;
							break;
						}
					}
				}
				PetPropConstData.selectedPet = GameCommonData.Player.Role.PetSnapList[idQuery];
				dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[idQuery];
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:idQuery, ownerId:GameCommonData.Player.Role.Id});
			}
			////////////////////////////
			var count:int;
			for(var keyNum:Object in GameCommonData.Player.Role.PetSnapList) {
				count++;
			}
			petView.txtPetCount.text = count + "/" + PetPropConstData.petBagNum;
		}
		
		/** 发送数据 */
		private function sendData(action:uint, data:Object=null):void
		{
			switch(action) {
				case OperateItem.GET_PET_INFO:											//查询宠物详细信息
					PetNetAction.operatePet(action, data.petId, data.ownerId);
					break;
				case OperateItem.GETPETLIST_OFPLAYER:									//查询别人的宠物列表，对外接口
					PetNetAction.operatePet(action, 0, uint(data));
					break;
				case PlayerAction.PET_EATHP:											//宠物吃血药
					PetNetAction.opPet(action, data.petId);
					break;
				case PlayerAction.PET_TRAIN:											//宠物驯养
					PetNetAction.opPet(action, data.petId);
					break;
				case PlayerAction.PET_DEOP:												//放生宠物
					PetNetAction.opPet(action, data.petId);
					break;
				case PlayerAction.PET_RENAME:											//改名
					PetNetAction.opPet(action, data.petId, data.petName);
					break;
				case PlayerAction.PET_ADDPOINTS:										//确定加点
					PetNetAction.addPointPet(action, data.petId, data.points); 
					break;
				case PlayerAction.PET_EXT_LIFE:											//宠物延寿
					PetNetAction.opPet(action, data.petId);
					break;
			}
		}
		
		/** 更新PetAtt */
		private function updatePetAtt(attInfo:Object):void
		{
			if(isSendDependence)
			{
				isSendDependence = false;
				return;
			}
			var petId:uint = attInfo.id;
			var potUpdate:Boolean = attInfo.potUpdate;
			if(PetPropConstData.selectedPet && PetPropConstData.selectedPet.Id == petId && PetPropConstData.selectedPet.OwnerId == GameCommonData.Player.Role.Id) {
				var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[petId];
				PetPropConstData.newCurrentPet = pet;
				 if(PetPropConstData.isNewPetVersion)
				{
					setViewState(true);
				} 
				if(pet.State == 0 || pet.State == 4) {
					petSkillGridManager.lockAllGrid(false);
					petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_1" ];      // 出战
				} 
				else if(pet.State == 1)
				{
					petSkillGridManager.lockAllGrid(true);
					petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_2" ];    //  休息
				}
				petView.txtHp.text = pet.HpNow + "/" + pet.HpMax;
				petView.txtHappy.text = pet.HappyNow + "/" + pet.HappyMax;
				if(PetPropConstData.isNewPetVersion)
				{
					tempMC.txtLife.text = pet.LifeNow + "/" + pet.LifeMax;
				}
				else
				{
					petView.txtLife.text = pet.LifeNow + "/" + pet.LifeMax;
				}
				petView.txtExp.text = pet.ExpNow + "/" + UIConstData.ExpDic[3000+pet.Level];
				petView.txtAppraise.text = UIUtils.GetScoreStr(pet.Appraise, true);
				petView.txtLevel.text = pet.Level.toString();
				
				if(potUpdate) {
					petView.txtForce.text = pet.Force.toString();
					petView.txtSpiritPower.text = pet.SpiritPower.toString();
					petView.txtPhysical.text = pet.Physical.toString();
					petView.txtConstant.text = pet.Constant.toString();
					petView.txtMagic.text = pet.Magic.toString(); 
					if(pet.Potential > 0) {
						PetPropConstData.potentials = pet.Potential;
						PetPropConstData.points = [0,0,0,0,0];
						petView.txtPotential.text = pet.Potential.toString();
						if(dataProxy.PetCanOperate) petUIManager.setModel(6);
					} else {
						petView.txtPotential.text = "";
						petUIManager.setModel(7);
					}
				}
				petView.txtPhyAttack.text = pet.PhyAttack.toString();
				petView.txtMagicAttack.text = pet.MagicAttack.toString();
				petView.txtPhyDef.text = pet.PhyDef.toString();
				petView.txtMagicDef.text = pet.MagicDef.toString();
				petView.txtHit.text = pet.Hit.toString();
				petView.txtHide.text = pet.Hide.toString();
				petView.txtCrit.text = pet.Crit.toString();
				petView.txtToughness.text = pet.Toughness.toString();
			}
		}
		
	}
}