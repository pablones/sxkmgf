package GameUI.Modules.MainSence.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.SoundList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Designation.Data.DesignationEvent;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.proxy.MessageWordProxy;
	import GameUI.Modules.Help.Data.DataEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Modules.MainSence.Command.OnsyncBagQuickBarCommand;
	import GameUI.Modules.MainSence.Command.SendQuickBarCommand;
	import GameUI.Modules.MainSence.Data.MainSenceData;
	import GameUI.Modules.MainSence.Proxy.QuickGridManager;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.PlayerInfo.Mediator.CounterWorkerInfoMediator;
	import GameUI.Modules.PlayerInfo.Mediator.PetInfoMediator;
	import GameUI.Modules.PlayerInfo.Mediator.PlayerDetailInfoMediator;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Modules.PlayerInfo.Mediator.TeamListInfoMediator;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.SystemMessage.Data.SysMessageData;
	import GameUI.Modules.SystemMessage.Data.SysMessageEvent;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Mediator.NewUnityMainMediator;
	import GameUI.Modules.VipShow.Data.VipShowData;
	import GameUI.Modules.VipShow.VipShowMediator.VipShowMediator;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.Proxy.DataProxy;
	import GameUI.Sound.SoundManager;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.ItemBase;
	
	import Net.ActionSend.VipListSend;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MainSenceMediator extends Mediator
	{
		public static const NAME:String = "MainSenceMediator";
		private var dataProxy:DataProxy = null;
		private var quickBarFlag:Boolean = false;
		private var quickBarFlagRight:Boolean = false;
		private var quickGridManager:QuickGridManager;
		protected var expSprite:Sprite=new Sprite();
		private var redFrame:MovieClip = null;
		private var oldTime:int ;
		
		protected const ColorArr:Array= [0xFF0000
										,0xE42200
										,0xCE3E00
										,0xB45F00
										,0xA37400
										,0x9F7A00
										,0x998100
										,0x8E8F00
										,0x809F00
										,0x77AB00
										,0x67BF00
										,0x60C800
										,0x4FDD00
										,0x45E900
										,0x33FF00];
										
										
		public function MainSenceMediator()
		{
			super(NAME);
		}
		
		public function get mainSence():MovieClip
		{
			return this.viewComponent as MovieClip;	
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.ENTERMAPCOMPLETE,
				EventList.DROPINQUICK,
				EventList.TEAMBTNRAY,
				FriendCommandList.FRIEND_MESSAGE,
				FriendCommandList.LEAVE_WORD,
				FriendCommandList.READED_MESSAGE,
				EventList.UPDATE_MAINSECEN_EXP,
				EventList.DROPSKILLINQUICK,
				EventList.USE_EXTENDSKILL_MSG,
				EventList.SHOW_MAINSENCE_BTN_FLASH,
				EventList.PET_RESTORDEAD_MSG,
				EventList.CHANGE_QUICKBAR_UI,
				EventList.RECEIVE_QUICKBAR_MSG,
				PlayerInfoComList.SHOW_EXPANDTEAM_ICON,
				PlayerInfoComList.HIDE_EXPANDTEAM_ICON,
				EventList.PLAY_SOUND_OPEN_PANEL,
				PetEvent.PET_TO_FIGHT_AFTER_GETINFO,
				EventList.ADDQUICKFLOW,
				SysMessageEvent.SYSMESSAGE_FLASH_MAIN_SENCE,
				EventList.REMOVEQUICKFLOW,
				OnLineAwardData.GET_AWARD_POINT,
				MainSenceData.USETEAMBUTTON
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					SysCursor.GetInstance().setMouseType();
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.MAINSENCE});
					redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
					redFrame.name = "redFrame";
					(this.mainSence["btn_expandTeam_up"] as SimpleButton).addEventListener(MouseEvent.CLICK,onExpandTeamClick);
					(this.mainSence["btn_expandTeam_down"] as SimpleButton).addEventListener(MouseEvent.CLICK,onExpandTeamClick);
					(this.mainSence["btn_expandTeam_up"] as SimpleButton).visible=false;
					(this.mainSence["btn_expandTeam_down"] as SimpleButton).visible=false;
					dataProxy = facade.retrieveProxy( DataProxy.NAME) as DataProxy;
					facade.registerMediator(new SelfInfoMediator());
					facade.registerMediator(new CounterWorkerInfoMediator());
					facade.registerMediator(new PetInfoMediator());
					facade.registerMediator(new TeamListInfoMediator());
					facade.registerMediator(new PlayerDetailInfoMediator());
					facade.sendNotification(PlayerInfoComList.INIT_PLAYERINFO_UI);
					facade.registerProxy(new MessageWordProxy());  //注册留言板 
					facade.registerCommand(EventList.SEND_QUICKBAR_MSG,SendQuickBarCommand);
					facade.registerCommand(EventList.ONSYNC_BAG_QUICKBAR,OnsyncBagQuickBarCommand);
//					facade.registerCommand(EventList.OPEN_PANEL_TOGETHER, OpenPanelTogetherCommand); 
					this.mainSence.addEventListener(Event.ADDED_TO_STAGE, initResize);
				break;
				case PetEvent.PET_TO_FIGHT_AFTER_GETINFO:
					sendNotification(PlayerInfoComList.UPDATE_PET_UI);
					this.quickGridManager.addPetInitiativeSkill();  
				break;
				
				case EventList.ENTERMAPCOMPLETE: 
					this.quickGridManager=new QuickGridManager(this.mainSence);
					initMainSence();
					this.updateExp();
				break;
				case EventList.DROPINQUICK:
					this.quickGridManager.addUseItem(notification.getBody());
				break;
				//宠物休息或死亡（清除宠物的技能）
				case EventList.PET_RESTORDEAD_MSG:
					this.quickGridManager.clearAllPetSkill();
					sendNotification(EventList.SEND_QUICKBAR_MSG); //同步快捷栏
				break;
				case EventList.USE_EXTENDSKILL_MSG:
					this.quickGridManager.clearAllPlayerSkillCd();
				break;
				case EventList.TEAMBTNRAY:
//					showTeamFlash();
				break;
				case EventList.SHOW_MAINSENCE_BTN_FLASH:
					var flashObj:uint = uint(notification.getBody());
					showBtnFlash(flashObj);
				break;
				//好友消息
				case FriendCommandList.FRIEND_MESSAGE:
					showFriendFlash();
					break;
				//好友留言	
				case FriendCommandList.LEAVE_WORD:
					showFriendFlash();
					break;
				//已经读完消息 停止提示 	
				case FriendCommandList.READED_MESSAGE:
					this.hideFriendFlash();
					 break;
				//更新界面中的经险值	 
				case EventList.UPDATE_MAINSECEN_EXP:
					this.updateExp();
					break;	 	
				//切换职业，改变快捷栏	
				case EventList.CHANGE_QUICKBAR_UI:
					this.quickGridManager.changeJob();
				 	break;	
				case EventList.RECEIVE_QUICKBAR_MSG:
					setPageBtn();
					break;
				//隐藏（隐藏或显示队伍图标	第一帧是向上的 二帧是向下的）	
				case PlayerInfoComList.SHOW_EXPANDTEAM_ICON:
					if((this.mainSence["btn_expandTeam_down"] as SimpleButton).visible){
						return;
					}
					(this.mainSence["btn_expandTeam_up"] as SimpleButton).visible=true;
					(this.mainSence["btn_expandTeam_down"] as SimpleButton).visible=true;
					break;
				//显示（隐藏或显示队伍图标	）
				case PlayerInfoComList.HIDE_EXPANDTEAM_ICON:
					(this.mainSence["btn_expandTeam_up"] as SimpleButton).visible=false;
					(this.mainSence["btn_expandTeam_down"] as SimpleButton).visible=false;
					break;	
				case EventList.PLAY_SOUND_OPEN_PANEL:
					playSoundOpenPanel();
					break;	 
				case EventList.ADDQUICKFLOW:
					if(quickGridManager)
					{
						quickGridManager.addQuickFlow(notification.getBody());
					}
					break;	
				case EventList.REMOVEQUICKFLOW:
					quickGridManager.removeQuickFlow(notification.getBody());
					break;
				case SysMessageEvent.SYSMESSAGE_FLASH_MAIN_SENCE:
					showFriendFlash()
//					mainSence.mcSysFlash.play();
//					mainSence.mcSysFlash.visible = true;
					break;
				case OnLineAwardData.GET_AWARD_POINT:
				    getPoint();
				    break;
		    	case MainSenceData.USETEAMBUTTON:
				    var btnIndex:uint = notification.getBody() as uint;
				    useQuickBtn( btnIndex );
				    break;
			}
		}
		
		private function getPoint():void
		{
			var p:Point = mainSence.localToGlobal(new Point(mainSence.btn_4.x, mainSence.btn_4.y));
			sendNotification( OnLineAwardData.MOVE_AWARD, p );
		}
		
		/** 打开面板时播放声音 */
		private function playSoundOpenPanel():void
		{
			SoundManager.PlaySound(SoundList.PANECLOSE);
		}
		
		/**
		 * 收缩与展开队伍按钮 
		 * @param e
		 * 
		 */		
		protected function onExpandTeamClick(e:MouseEvent):void{
			if((this.mainSence["btn_expandTeam_up"] as SimpleButton).visible){
				(this.mainSence["btn_expandTeam_up"] as SimpleButton).visible=false;
				
				sendNotification(PlayerInfoComList.HIDE_TEAM_UI);
			}else{
				(this.mainSence["btn_expandTeam_up"] as SimpleButton).visible=true;
				sendNotification(PlayerInfoComList.SHOW_TEAM_UI);
				
			}
		}
		
		/**
		 * 更新经验值 
		 * 
		 */		
		protected function updateExp():void{
			var maxExp:uint=UIConstData.ExpDic[GameCommonData.Player.Role.Level];
			var exp:uint=GameCommonData.Player.Role.Exp;
			if(exp>maxExp)exp=maxExp;
			var width:Number=(exp/maxExp)*293;
			var color:uint=this.ColorArr[Math.floor((exp/maxExp)*(this.ColorArr.length-1))];
			
			this.expSprite.mouseEnabled=false;
			this.expSprite.graphics.clear();
			this.expSprite.graphics.beginFill(color,0.9);
			this.expSprite.graphics.drawRect(0,0,width,3);
			this.expSprite.graphics.endFill();
			(this.viewComponent.mcExp as MovieClip).addChild(this.expSprite);
			this.expSprite.x=1;
			this.expSprite.y=1;
		}
		
		/**
		 * 停止好友消息提示 
		 * 
		 */		
		protected function hideFriendFlash():void{
			mainSence["btn_5"].mcFlash.stop()
			mainSence["btn_5"].mcFlash.visible = false;
		}
		
		/**
		 * 好友消息提示 
		 * 
		 */		
		protected function showFriendFlash():void{
			mainSence["btn_5"].mcFlash.play();
			mainSence["btn_5"].mcFlash.visible = true;
		}
		
		/** 
		 * 让按钮或箭头闪起
		 * @param btnIndex:要闪按钮的下标
		 */
		private function showBtnFlash(type:uint):void
		{
			for( var i:int = 0; i < 8; i++ ) {
				mainSence["btn_"+i].mcFlash.stop();
				mainSence["btn_"+i].mcFlash.visible = false;	
			}
			if(type > 7) return;
			mainSence["btn_"+type].mcFlash.play();
			mainSence["btn_"+type].mcFlash.visible = true;
			
			var point:Point = (mainSence["btn_"+type] as DisplayObject).localToGlobal(new Point(mainSence["btn_"+type].mcFlash.x, mainSence["btn_"+type].mcFlash.y));
			NewerHelpData.point = point;
			
			if(NewerHelpData.curType)
			{
				switch(NewerHelpData.curType) {
					case 3:
					    this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 3);
					    break;
					case 4:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 4);
					    break;
					case 5:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 5);
					    break;
					case 6:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 6);
					    break;
					case 7:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 7);
					    break;
					case 8:
                        this.sendNotification(NewerHelpEvent.RECALL_TASK_HELP);
					    break;
					case 11:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 11);
					    break;
					case 13:
                        this.sendNotification(NewerHelpEvent.RECALL_TASK_HELP);
					    break;
					case 34:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 34);
					    break;
					case 35:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 35);
					    break;
					case 41:
                        this.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 41);
					    break;
					default:
					    break;
				}
			}
		}
		
		private function initMainSence():void
		{
			
			GameCommonData.GameInstance.GameUI.addChild(mainSence);
			for( var i:int = 0; i<8; i++ )
			{
				mainSence["btn_"+i].addEventListener(MouseEvent.CLICK, onQuickBtn);
				mainSence["btn_"+i].mcFlash.mouseChildren = false;
				mainSence["btn_"+i].mcFlash.mouseEnabled = false;
				mainSence["btn_"+i].mcFlash.stop();
				mainSence["btn_"+i].mcFlash.visible = false;	
			}
			//注册场景右侧按钮点击事件
//			for(var n:int = 0; n<9; n++)
//			{
//				mainSence.mc_settingBtns["btnRight_"+n].addEventListener(MouseEvent.CLICK, clickBtnRightHandler);
//			}
			//红色边框
			for(var j:int=0;j<8;j++){
				(mainSence.mcQuickBar0["quick_"+j] as MovieClip).addEventListener(MouseEvent.MOUSE_MOVE,onMouseMoveHandler);
				(mainSence.mcQuickBar0["quick_"+j] as MovieClip).addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
				
				(mainSence.mcQuickBar1["quickf_"+j] as MovieClip).addEventListener(MouseEvent.MOUSE_MOVE,onMouseMoveHandler);
				(mainSence.mcQuickBar1["quickf_"+j] as MovieClip).addEventListener(MouseEvent.MOUSE_OUT,onMouseOutHandler);
			}
			facade.registerProxy(quickGridManager);	
			mainSence.mcQuickBar1.visible = quickBarFlag;
			mainSence.btnQuickLanUp.addEventListener(MouseEvent.CLICK, onPageBtn);	
			mainSence.btnQuickLanDown.addEventListener(MouseEvent.CLICK, onPageBtn);
//			mainSence.btnQuickLanRight.addEventListener(MouseEvent.CLICK, rightBtnHandler);	
//			mainSence.btnQuickLanLeft.addEventListener(MouseEvent.CLICK, rightBtnHandler);		
			mainSence.btnShopMain.addEventListener(MouseEvent.CLICK, onShopHandler);	
//			mainSence.btnQuickAutoPlay.addEventListener(MouseEvent.CLICK, onQuickHandler);	
			mainSence.btn_help.addEventListener(MouseEvent.CLICK,onHelpHandler); //帮助
//			mainSence.btnSysMessage.addEventListener(MouseEvent.CLICK, onQuickHandler);	
			mainSence.btnQuickSys.addEventListener(MouseEvent.CLICK, onQuickHandler);	
			mainSence.btnVIP.addEventListener(MouseEvent.CLICK, onQuickHandler);
			mainSence.BtnTeam.addEventListener(MouseEvent.CLICK, onQuickHandler);
//			mainSence.mcSysFlash.stop();
//			mainSence.mcSysFlash.visible = false;
//			mainSence.mcSysFlash.mouseEnabled = false;
			
//			mainSence.btnQuickLanLeft.visible = false;
//			mainSence.btnSysMessage.visible = false;
//			(mainSence.btnQuickLanRight as SimpleButton).dispatchEvent( new MouseEvent(MouseEvent.CLICK) );
		}
		
		private function onQuickHandler(e:MouseEvent):void
		{
			var name:String = e.currentTarget.name;
			switch(name) {
				case "btnQuickAutoPlay":	//快速挂机
					sendNotification( AutoPlayEventList.START_AUTO_PLAY );
					break;
				case "btnSysMessage":		//系统消息
//					mainSence.mcSysFlash.stop();
//					mainSence.mcSysFlash.visible = false;
					if(SysMessageData.messageListIsOpen){
						facade.sendNotification(SysMessageEvent.CLOSEMESSAGEVIEW);
					}else{
						facade.sendNotification(SysMessageEvent.SHOWMESSAGEVIEW );
					}
					break;
				case "btnQuickSys":			//系统设置
					facade.sendNotification(SystemSettingData.OPEN_SETTING_UI);
					break;
				case "btnVIP":				//VIP按钮
					if( VipShowData.IsVipShowOpen )
					{
						sendNotification( VipShowData.CLOSE_VIPSHOW_VIEW );
					}
					else
					{
						if(getTimer() - oldTime > 5000)
						{
							oldTime = getTimer();
							if(!VipShowData.IsVipShowOpen)
							{
								var obj:Object = {action: 1 ,pageIndex: 1 , amount: 13 ,memID: 0 };
								VipListSend.sendVipListAction(obj);
							}
						}
						else
						{
							var vipShowMediator:VipShowMediator = facade.retrieveMediator( VipShowMediator.NAME ) as VipShowMediator;
							vipShowMediator.initView();
						}
					}
					break;
				case "BtnTeam":				//组队设置
					useQuickBtn(7);
					break;
			}
		}
		
		/**
		 * 打开帮助文档 
		 * @param e
		 * 
		 */		
		protected function onHelpHandler(e:MouseEvent):void{
			facade.sendNotification(DataEvent.OUTSHOWPK);
		}
		
		private function onShopHandler(e:MouseEvent):void
		{
			if(dataProxy.MarketIsOpen) {
				sendNotification(EventList.CLOSEMARKETVIEW);
				sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
			} else {
				sendNotification(EventList.SHOWMARKETVIEW);
			}
		}
		
		protected function onMouseMoveHandler(e:MouseEvent):void{
			var mc:DisplayObject=e.currentTarget as DisplayObject;
			mc.parent.addChild(this.redFrame);
			redFrame.x=mc.x;
			redFrame.y=mc.y;
		}
		
		protected function onMouseOutHandler(e:MouseEvent):void{
			if(e.currentTarget.parent.contains(redFrame)){
				e.currentTarget.parent.removeChild(redFrame);
			}
		}
		
		public function getQuickfPoint():Point
		{
			var point:Point = mainSence.localToGlobal( new Point(mainSence.mcQuickBar1.x, mainSence.mcQuickBar1.y) );
			return point;
		}
		
		public function isVisible():Boolean
		{
			if( !quickBarFlag )
			{
				quickBarFlag = true;
				mainSence.mcQuickBar1.visible = quickBarFlag;
				mainSence.btnQuickLanUp.visible = false;
				mainSence.btnQuickLanDown.visible = true;
				return false;
			}
			return true;
		}
		
		private function onPageBtn(e:MouseEvent):void
		{
			
			quickBarFlag = !quickBarFlag;
			mainSence.mcQuickBar1.visible = quickBarFlag;
			switch(quickBarFlag)
			{
				case true:
					mainSence.btnQuickLanUp.visible = false;
					mainSence.btnQuickLanDown.visible = true;
				break;
				case false:
					mainSence.btnQuickLanUp.visible = true;
					mainSence.btnQuickLanDown.visible = false;
				break;
			}
			
			if(quickBarFlag){
				GameCommonData.dialogStatus=GameCommonData.dialogStatus | 2;
			}else{
				GameCommonData.dialogStatus=GameCommonData.dialogStatus & (uint.MAX_VALUE-2);
			}		
		}
		/** 右栏按钮触发事件 */
		private function rightBtnHandler(e:MouseEvent):void
		{
//			for(var i:int = 0; i < 8;i++){
//				mainSence["btnRight_" + i].visible = quickBarFlagRight;
//			}
//			mainSence.mc_settingBtns.visible = quickBarFlagRight;
				
			switch(quickBarFlagRight)
			{
				case true:
					if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null) {
						GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton").x = 965; //910
					}
					if(GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton") != null) {
						GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton").x = 965; //910
					}
//					mainSence.btnQuickLanRight.visible = true;
//					mainSence.btnQuickLanLeft.visible = false;
				break;
				case false:
					if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null) {
						GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton").x = 965;
					}
					if(GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton") != null) {
						GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton").x = 965;
					}
//					mainSence.btnQuickLanRight.visible = false;
//					mainSence.btnQuickLanLeft.visible = false;
				break;
			}
			quickBarFlagRight = !quickBarFlagRight;
		}
		/** 设置快捷栏的展开与收缩方式*/
		private function setPageBtn():void{
			var maskBit:uint=GameCommonData.dialogStatus & 2;
			quickBarFlag=Boolean(maskBit);
			mainSence.mcQuickBar1.visible = quickBarFlag;
			switch(quickBarFlag)
			{
				case true:
					mainSence.btnQuickLanUp.visible = false;
					mainSence.btnQuickLanDown.visible = true;
				break;
				case false:
					mainSence.btnQuickLanUp.visible = true;
					mainSence.btnQuickLanDown.visible = false;
				break;
			}
			
		}
		
		private function getAllNum(item:ItemBase):int
		{
			var num:int = 0;
			for( var i:int = 0; i<BagData.AllItems.length; i++ )
			{
				for( var j:int = 0; j<BagData.AllItems[i].length; j++ )
				{
					if(BagData.AllItems[i][j] == undefined) continue;
					if(item.Type ==  BagData.AllItems[i][j].type)
					{
						num += BagData.AllItems[i][j].amount;
					}
				}
			}
			return num;
		}
		
		private function showTeamFlash():void
		{
			mainSence["btn_7"].mcFlash.play();
			mainSence["btn_7"].mcFlash.visible = true;
		}
		
		private function onQuickBtn(e:MouseEvent):void
		{
			var type:uint = (e.currentTarget.name as String).split("_")[1];
			if( type == 7 )
			{
				useRightQuickBtn(3);    //挑战 又原组队修改
			}
			else
			{
				useQuickBtn(type);
			}
		}
		
		private function clickBtnRightHandler(e:MouseEvent):void
		{
			var type:uint = (e.currentTarget.name as String).split("_")[1];
			useRightQuickBtn(type);
		}
		
		public function useRightQuickBtn(type:uint):void
		{
			switch(type)
			{
				case 0:			//离线挂机
					if( GameCommonData.Player.Role.Level >= 10 )
					{
						if(AutoPlayData.dataIsSendOver == false) facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_1" ], color:0xffff00});//"正在传输数据，请稍后"
						else 
						{
//							PrepaidLevelNet.sendPrepaidDemand(1); 
//							PrepaidUIData.openFrom = "offline";
							facade.sendNotification( PrepaidUIData.SHOW_OFFLINE_VIEW );
						}
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_1" ]/** = "达到10级才能领取离线经验";*/, color:0xffff00});	
					}
				break;
				case 1:			//活动日程
					if( GameCommonData.Player.Role.Level >= 15 )
					{
						facade.sendNotification(CampaignData.INIT_CAMPAIGN);
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_2" ]/** = "达到15级才能查看活动列表";*/, color:0xffff00});	
					}	
				break;
				case 2:			//生产
//					var simulateObj:Object={
//						simulateDes:'<font face="宋体"  size="12" color="#ffffff">刀剑磨得快，才能锻造出好兵器。只要将武器装备强化升星后，再加上适合相应职业的宝石，就能号令武林、称霸天下！</font>',
//						simulateDataPro:[{iconUrl:"symbol_talk",showText:"装备强化",linkText:"装备强化"},
//						{iconUrl:"symbol_talk",showText:"装备升星",linkText:"装备升星"},
//						{iconUrl:"symbol_talk",showText:"装备打孔",linkText:"装备打孔"},
//						{iconUrl:"symbol_talk",showText:"宝石合成",linkText:"宝石合成"},
//						{iconUrl:"symbol_talk",showText:"宝石镶嵌",linkText:"宝石镶嵌"},
//						{iconUrl:"symbol_talk",showText:"宝石取出",linkText:"宝石取出"},	
//						{iconUrl:"symbol_ask",showText:"装备玩法说明",linkText:"装备玩法说明"}	
//						]		
//					};
//					facade.sendNotification(NPCChatComList.SHOW_SIMULATE_NPC_CHAT,simulateObj);
					if( GameCommonData.Player.Role.Level >= 25 )
					{
						if(dataProxy.TradeIsOpen) 
						{
							sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_2" ], color:0xffff00});//"交易时不能生产"
							return;
						}
						 else if(StallConstData.stallSelfId > 0) 
						 {
							sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_3" ], color:0xffff00});//"摆摊时不能生产"
							return;
						}
						if ( ManufactoryData.ResourseIsLoaded )
						{
							facade.sendNotification( ManufactoryData.SHOW_MANUFACTORY_UI,{view:false} );
						}
						else
						{
							facade.sendNotification( ManufactoryData.INIT_MANUFACTORY_UI );
						}
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_3" ]/** = "达到25级才能进行生产制造";*/, color:0xffff00});	
					}
				break;
				case 3:			//开箱子
					//屏蔽北京ip
					if ( GameCommonData.cztype == 1 )
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_4" ], color:0xffff00});//"此功能暂未开放"	
						return;
					}
					if ( GameCommonData.openTreasureStragety == 1 )
					{
						if ( !TreasureData.TreaResourceLoaded )
						{
							sendNotification( TreasureData.LOAD_TREASURE_RES );
						}
						else
						{
							if ( dataProxy.treasurePanelIsOpen )
							{
								sendNotification( TreasureData.CLOSE_TREASURE_UI );
							}	
							else
							{
								sendNotification( TreasureData.SHOW_TREASURE_UI );
							}
						}
					}
					else if ( GameCommonData.openTreasureStragety == 2 )
					{ 
						if ( TreasureData.packageDateArr.length == 0 )
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_5" ], color:0xffff00});//"宝物包裹无任何物品"	
							return;
						}
						if ( !TreasureData.TreaResourceLoaded )
						{
							sendNotification( TreasureData.LOAD_TREASURE_RES );
						}
						else
						{
							if ( dataProxy.treasurePackageIsOpen )
							{
								sendNotification( TreasureData.CLOSE_MY_TREA_PACKAGE );
							}	
							else
							{
								sendNotification( TreasureData.OPEN_MY_TREA_PACKAGE );
							}
						}
					}
					else if ( GameCommonData.openTreasureStragety == 3 )
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_med_mai_use_4" ], color:0xffff00});//"此功能暂未开放"	
					}
				break;
				case 4:			//强化
					if( GameCommonData.Player.Role.Level >= 10 )
					{
						facade.sendNotification(EquipCommandList.SHOW_EQUIPSTRENGEN_UI);
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_4" ]/** = "达到10级才能强化装备";*/, color:0xffff00});	
					}	
				break;
				case 5:			//万兽		
					if( GameCommonData.Player.Role.Level >= 15 )
					{
						if(dataProxy.petRuleIsOpen) {
							sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
						} else {
							sendNotification(EventList.SHOW_PET_PLAYRULE_VIEW, {type:UIConstData.PET_RULE_BASE, index:0});
						}
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_5" ]/** = "达到15级才能强化宠物";*/, color:0xffff00});	
					}		
				break;
				case 6:			//称号
					facade.sendNotification(DesignationEvent.OPEN_DESIGNATION_PANEL);
					break;
				case 7: 		//系统设置
					facade.sendNotification(SystemSettingData.OPEN_SETTING_UI);
					break;
				case 8:			//师徒 
					if ( GameCommonData.Player.Role.Level < 10 )
					{
						sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_6" ]/** = "10级才能拜师";*/, color:0xffff00});//"10级才能拜师"	
						return;
					}
					sendNotification( MasterData.CLICK_MASTER_NPC );
					break;
			}
		}
		
		public function useQuickBtn(type:uint):void
		{
			switch(type) {
				case 0:
					if(!dataProxy.HeroPropIsOpen)
					{
						//关掉其他面板
						facade.sendNotification(EventList.SHOWONLY, "hero");
						dataProxy.HeroPropIsOpen = true;
						facade.sendNotification(EventList.SHOWHEROPROP);
						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
					else
					{
						facade.sendNotification(EventList.CLOSEHEROPROP);
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					mainSence["btn_0"].mcFlash.stop()
					mainSence["btn_0"].mcFlash.visible = false;
					break;
				case 1:
					if(!dataProxy.SkillIsOpen)
					{
						//关掉其他面板
						facade.sendNotification(EventList.SHOWONLY, "skill");
						dataProxy.SkillIsOpen = true;
						facade.sendNotification(EventList.SHOWSKILLVIEW);
						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
					else
					{
						facade.sendNotification(EventList.CLOSESKILLVIEW);
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					mainSence["btn_1"].mcFlash.stop()
					mainSence["btn_1"].mcFlash.visible = false;
					break;
				case 2:
					if(!dataProxy.PetCanOperate) 
					{
						facade.sendNotification(EventList.SHOWPETVIEW);
						return;
					}
					if(!dataProxy.PetIsOpen)
					{
						//关掉其他面板
						facade.sendNotification(EventList.SHOWONLY, "pet");
						facade.sendNotification(EventList.SHOWPETVIEW);
						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
					else
					{
						facade.sendNotification(EventList.CLOSEPETVIEW);
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					mainSence["btn_2"].mcFlash.stop()
					mainSence["btn_2"].mcFlash.visible = false;
					break;
				case 3:
					if(!dataProxy.TaskIsOpen)
					{
						//关掉其他面板
						facade.sendNotification(EventList.SHOWONLY, "task");
						dataProxy.TaskIsOpen = true;
						facade.sendNotification(EventList.SHOWTASKVIEW);
						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
					else
					{
						facade.sendNotification(EventList.CLOSETASKVIEW);
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					mainSence["btn_3"].mcFlash.stop()
					mainSence["btn_3"].mcFlash.visible = false;
					break;
				case 4:
					if(!dataProxy.BagIsOpen)
					{
						sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "bag");
						facade.sendNotification(EventList.SHOWBAG);
						dataProxy.BagIsOpen = true;
						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
					else
					{
						facade.sendNotification(EventList.CLOSEBAG);
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					mainSence["btn_4"].mcFlash.stop()
					mainSence["btn_4"].mcFlash.visible = false;
					break;
				case 5:
					//有消息
					if(mainSence["btn_5"].mcFlash.visible){
						this.sendNotification(FriendCommandList.SHOW_RECEIVE_MSG);
					 }else{
						 if(!dataProxy.FriendsIsOpen){
//							sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "friend");
							facade.sendNotification(FriendCommandList.SHOWFRIEND);
							dataProxy.FriendsIsOpen=true;
							sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
						}else{
							facade.sendNotification(FriendCommandList.HIDEFRIEND);
						} 	 
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					break;
				case 6:
					if ( NewUnityCommonData.closeUnity )
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_7" ]/** = "帮派功能正在调试，暂时关闭";*/, color:0xffff00});	
						return;
					}
//					if(!dataProxy.UnityIsOpen)
//					{
//						if ( GameCommonData.wordVersion == 2 )
//						{
//							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_7" ]/** = "帮派功能正在调试，暂时关闭";*/, color:0xffff00});	
//						}
//						else
//						{
							if ( !facade.hasMediator( NewUnityMainMediator.NAME ) )
							{
								facade.registerMediator( new NewUnityMainMediator() );
							}
							sendNotification( NewUnityCommonData.CLICK_NEW_UNITY_BTN );
//						}
//						else
//						{
//							dataProxy.UnityIsOpen = true;
//							facade.sendNotification(EventList.SHOWUNITYVIEW);
//							sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "unity");
//							sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
//						}
//					}
//					else
//					{
//						facade.sendNotification(EventList.CLOSEUNITYVIEW);
//					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					mainSence["btn_6"].mcFlash.stop()
					mainSence["btn_6"].mcFlash.visible = false;
					break;
				case 7:
					if(!dataProxy.TeamIsOpen)
					{
						facade.sendNotification(EventList.SHOWTEAM);
						sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "team");
						sendNotification(EventList.OPEN_PANEL_TOGETHER);	//组合打开面板
					}
					else 
					{
						facade.sendNotification(EventList.REMOVETEAM);
					}
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
//					mainSence["btn_7"].mcFlash.stop();
//					mainSence["btn_7"].mcFlash.visible = false;
					break;
//				case 8:
//					if(dataProxy.buildPanelIsOpen){
//						sendNotification(BuildCommonList.CLOSE_BUILD_UI);
//					}else{
//						sendNotification(BuildCommonList.SHOW_BUILD_UI);
//					}
				
				
//					facade.sendNotification(HintEvents.RECEIVEINFO, {info:"此功能暂未开放", color:0xffff00});
 					
//					break;
//				case 9:
					
//					break;
			}
		}
		
		
		protected function initResize(event:Event):void
		{
			sendNotification(EventList.STAGECHANGE);
		}
	}
}