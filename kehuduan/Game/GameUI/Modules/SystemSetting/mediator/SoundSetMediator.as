package GameUI.Modules.SystemSetting.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Encrypt.des.DESKey;
	import GameUI.Encrypt.des.Hex;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Login.SoundUntil.SoundController;
	import GameUI.Modules.Map.SmallMap.Mediator.SmallMapMediator;
	import GameUI.Modules.MusicPlayer.Command.MusicPlayerCommandList;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionProcessor.EncyptAction;
	import Net.ActionSend.EncryptSend;
	
	import OopsEngine.Scene.CommonData;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.ByteArray;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SoundSetMediator extends Mediator
	{
		public static const NAME:String = "mcSystemMediator";
		private var btn_gameMusic:MovieClip;//游戏声音按钮
		private var btn_backMusic:MovieClip;//背景声音按钮
		private var mcIsEnabledG:MovieClip;//游戏音效组件遮蔽层
		private var mcIsEnabledB:MovieClip;//背景音效组件遮蔽层
		private var gameSoundController:SoundController;//游戏声音组件
		private var backSoundController:SoundController;//背景声音按钮
		private var isFirstInit:Boolean = true;//是否是第一次开启此面板
		private var isGameSoundOpen:Boolean = false;//游戏音效是否开启
		private var isBackSoundOpen:Boolean = false;//背景音乐是否开启
		private var isSoundSwitchOpen:Boolean = false;//小喇叭的状态
		private var dataArr:Array;//存储状态的临时数组
		
		private var pwdInputPanelBase:PanelBase  = null;
		private var pwdModifyPanelBase:PanelBase = null;
		private var backMask:Sprite = null;
		private var timerSave:Timer = new Timer(10000, 1);		//保存计时器  	5秒一次
		private var timerModify:Timer = new Timer(120000, 1); 	//修改密码计时器	2分钟一次	
		private var isLocked:Boolean = false;					//是否已开启账号保护
		
		public function SoundSetMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		public function get soundSetUI():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
	
		public override function listNotificationInterests():Array
		{
			return [
				SystemSettingData.INIT_SOUND_VIEW,
				SystemSettingData.OPEN_SETTING_UI,
				SystemSettingData.CLOSE_SETTING_VIEW,
				SystemSettingData.INIT_SYSTEM_SETTING_DATA,
				SystemSettingData.SYS_SET_PWD_OP_RETURN,
				MusicPlayerCommandList.MUSICPLAYER_VOLUME,
			];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case SystemSettingData.INIT_SOUND_VIEW:
					initMC();
					break;
				case SystemSettingData.CLOSE_SETTING_VIEW:
					gc();
					break;
				case SystemSettingData.INIT_SYSTEM_SETTING_DATA:		//上线初始化系统设置
					initSetting();
					break;
//				case SystemSettingData.OPEN_SETTING_UI:
//					initSetting();
//					break;
				case SystemSettingData.SYS_SET_PWD_OP_RETURN:			//密码操作的返回
					pwdOpRes(notification.getBody());
					break;
				case MusicPlayerCommandList.MUSICPLAYER_VOLUME:
					if (notification.getBody().from != backSoundController)
						backSoundController.setVolume(Number(notification.getBody().vol));
					break;
			}
		}
		private function initMC():void
		{
			if(isFirstInit)
			{
				isFirstInit = false;
				gameSoundController = new SoundController(GameConfigData.UILibrary,new Point(290,90),false ,null , null ,false , 2,soundSetUI,"SoundControll2"); 
				backSoundController = new SoundController(GameConfigData.UILibrary,new Point(290,195),false ,null , null ,false , 1,soundSetUI,"SoundControll2"); 
				
				btn_gameMusic = getMCChild(soundSetUI,"mc_gameMusic");
				btn_backMusic = getMCChild(soundSetUI,"mc_backMusic");
				btn_gameMusic.buttonMode = true;
				btn_backMusic.buttonMode = true;
				mcIsEnabledG = getMCChild(gameSoundController.mcVolumeInstance,"mcUseEnabled");
				mcIsEnabledB = getMCChild(backSoundController.mcVolumeInstance,"mcUseEnabled");
				
				if(GameCommonData.isOpenSoundSwitch)
				{
					isGameSoundOpen = true;
					isSoundSwitchOpen = true;
					mcIsEnabledG.visible = false;
					btn_backMusic.gotoAndStop(2);
				}
				else
				{
					btn_backMusic.gotoAndStop(1);
				}	
				if(GameCommonData.isOpenFightSoundSwitch)
				{
					isBackSoundOpen = true;
					isSoundSwitchOpen = true;
					mcIsEnabledB.visible = false;
					btn_gameMusic.gotoAndStop(2);
				}
				else	
				{
					btn_gameMusic.gotoAndStop(1);
				}
				(facade.retrieveMediator(SmallMapMediator.NAME) as SmallMapMediator).soundController.clickFunction = clickFunction;
			}
			openSetting();
			initPwdPanel();	//初始化密码输入、修改面板
			addEventListenters(); 
		}
		private function getMCChild(parentMC:MovieClip,childName:String):MovieClip
		{
			return parentMC.getChildByName(childName) as MovieClip;
		}

		private function addEventListenters():void
		{
			btn_gameMusic.addEventListener(MouseEvent.CLICK,isOpenMusicHandler);
			btn_backMusic.addEventListener(MouseEvent.CLICK,isOpenMusicHandler);
			soundSetUI.mc_team.addEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_trade.addEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_beat.addEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_friend.addEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_save.addEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.btn_save.addEventListener(MouseEvent.CLICK,settingBtnHandler);
			soundSetUI.btn_default.addEventListener(MouseEvent.CLICK,settingBtnHandler);
			soundSetUI.btn_modiPwd.addEventListener(MouseEvent.CLICK,settingBtnHandler);
		}
		
		
		private function settingHandler(me:MouseEvent):void
		{
			var tempMc:MovieClip = me.currentTarget as MovieClip;
			switch(tempMc)
			{
				case soundSetUI.mc_team:
					changeTempArr(tempMc,2);
					break;
				case soundSetUI.mc_trade:
					changeTempArr(tempMc,3);
					break;
				case soundSetUI.mc_beat:
					changeTempArr(tempMc,4);
					break;
				case soundSetUI.mc_friend:
					changeTempArr(tempMc,5);
					break;
				case soundSetUI.mc_save:
					changeTempArr(tempMc,6);
					break;
			}
		}
		private function changeTempArr(tempMc:MovieClip,num:int):void
		{
			
			if(2 == tempMc.currentFrame ) 
			{
				tempMc.gotoAndStop(1);
				dataArr[num] = 0;
			}
			else 
			{
				tempMc.gotoAndStop(2);
				dataArr[num] = 1;
			}
		}
		private function settingBtnHandler(me:MouseEvent):void
		{
			switch(me.target.name) {
				case "btn_default":																//默认选择
					GameCommonData.soundVolume = 50;
					GameCommonData.fightSoundVolume = 50;
					dataArr[0] = 1;
					btn_gameMusic.gotoAndStop(2);
					isGameSoundOpen = true;
					mcIsEnabledG.visible = false;
					isSoundSwitchOpen = true;
					gameSoundController.soundOnOrOff(true);
					
					dataArr[1] = 1;
					btn_backMusic.gotoAndStop(2);
					isBackSoundOpen = true;
					mcIsEnabledB.visible = false;
					
					backSoundController.soundOnOrOff(true);
					gameSoundController.setMcBlockPlace();
					backSoundController.setMcBlockPlace();
					(facade.retrieveMediator(SmallMapMediator.NAME) as SmallMapMediator).soundController.soundPicSet();
					
					for(var i:int = 2; i < 6; i++) {		//取消其他设置
						dataArr[i] = 0;
					}
					dataArr[6] = 1;							//打开保护锁
					
					soundSetUI.mc_team.gotoAndStop(1);
					soundSetUI.mc_trade.gotoAndStop(1);
					soundSetUI.mc_beat.gotoAndStop(1);
					soundSetUI.mc_friend.gotoAndStop(1);
					
					soundSetUI.mc_save.gotoAndStop(2);
					break;
				case "btn_save":																//保存设置
					if(timerSave.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_1" ], color:0xffff00});   //"10秒后才能再次保存设置"
						return;
					}
					sureSaveSet();
					break;
				case "btn_modiPwd":																//修改密码
					if(timerModify.running) {
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_2" ], color:0xffff00});   //"2分钟后才能再次修改密码"
						return;
					}
					if(SystemSettingData.pwdModifyIsOpen) {
						closePwdModify(null);
					} else {
						openPwdModify();
						SystemSettingData.pwdModifyIsOpen = true;
					}
					break;
			}
		}
		
		/**
		 * 初始化设置
		 **/ 
		private function initSetting():void
		{
			/**判断创建角色时的小喇叭状态，并保存到服务端*/
			 if(!GameCommonData.isOpenSoundSwitch)
			{
				SystemSettingData._dataArr[0] = SystemSettingData._dataArr[1] = 0;
				isSoundSwitchOpen = false;
			} 
			
			dataArr = UIUtils.DeeplyCopy(SystemSettingData._dataArr) as Array;
			if(1 == dataArr[0])
			{
				btn_gameMusic.gotoAndStop(2);
				isGameSoundOpen = true;
				mcIsEnabledG.visible = false;
				gameSoundController.soundOnOrOff(true);
			} else {
				btn_gameMusic.gotoAndStop(1);
				isGameSoundOpen = false;
				mcIsEnabledG.visible = true;
				gameSoundController.soundOnOrOff(false);
			}
			if(1 == dataArr[1])
			{
				btn_backMusic.gotoAndStop(2);
				isBackSoundOpen = true;
				mcIsEnabledB.visible = false;
				backSoundController.soundOnOrOff(true);
			} else {
				btn_backMusic.gotoAndStop(1);
				isBackSoundOpen = false;
				mcIsEnabledB.visible = true;
				backSoundController.soundOnOrOff(false);
			}
			judgeSoundSwitch();
			(facade.retrieveMediator(SmallMapMediator.NAME) as SmallMapMediator).soundController.soundPicSet();
			if(1 == dataArr[2]) soundSetUI.mc_team.gotoAndStop(2);
			if(1 == dataArr[3]) soundSetUI.mc_trade.gotoAndStop(2);
			if(1 == dataArr[4]) soundSetUI.mc_beat.gotoAndStop(2);
			if(1 == dataArr[5]) soundSetUI.mc_friend.gotoAndStop(2);
			
			if(1 == dataArr[6]) {
				soundSetUI.mc_save.gotoAndStop(2);
				isLocked = true; 
			}
		}
		/**
		 *每次打开面板时设置 
		 * @param me
		 * 
		 */
		private function openSetting():void
		{
			dataArr = UIUtils.DeeplyCopy(SystemSettingData._dataArr) as Array;
			if(1 == dataArr[2])
			{
				soundSetUI.mc_team.gotoAndStop(2);
			} 
			else{
				soundSetUI.mc_team.gotoAndStop(1);
			}
			if(1 == dataArr[3]) 
			{
				soundSetUI.mc_trade.gotoAndStop(2);
			}
			else{
				soundSetUI.mc_trade.gotoAndStop(1);
			}
			if(1 == dataArr[4])
			{
				soundSetUI.mc_beat.gotoAndStop(2);
			} 
			else{
				soundSetUI.mc_beat.gotoAndStop(1);
			}
			if(1 == dataArr[5]) 
			{
				soundSetUI.mc_friend.gotoAndStop(2);
			}
			else{
				soundSetUI.mc_friend.gotoAndStop(1);
			}
			
			if(1 == dataArr[6]) {
				soundSetUI.mc_save.gotoAndStop(2);
				isLocked = true; 
			}
			else{
				soundSetUI.mc_save.gotoAndStop(1);
			}
		}		
		
		private var lastVolume:int = 100;
	
		private function isOpenMusicHandler(me:MouseEvent):void
		{
			var tempMc:MovieClip = me.target as MovieClip;
			if(tempMc.name == btn_gameMusic.name && btn_gameMusic.currentFrame == 1)
			{
				isGameSoundOpen = true;
				btn_gameMusic.gotoAndStop(2);
				mcIsEnabledG.visible = false;
				isSoundSwitchOpen = true;
				gameSoundController.soundOnOrOff(true);
				dataArr[0] = 1;
			}
			else if(tempMc.name == btn_gameMusic.name && btn_gameMusic.currentFrame == 2)
			{
				isGameSoundOpen = false;
				btn_gameMusic.gotoAndStop(1);
				mcIsEnabledG.visible = true;
				gameSoundController.soundOnOrOff(false);
				judgeSoundSwitch();
				dataArr[0] = 0;
			}
			else if(tempMc.name == btn_backMusic.name && btn_backMusic.currentFrame == 1)
			{
				isBackSoundOpen = true;
				btn_backMusic.gotoAndStop(2);
				mcIsEnabledB.visible = false;
				isSoundSwitchOpen = true;
				backSoundController.soundOnOrOff(true);
				dataArr[1] = 1;
				sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: (GameCommonData.soundVolume == 0 ? lastVolume : GameCommonData.soundVolume), updateUI: true, from: this});
			}
			else if(tempMc.name == btn_backMusic.name && btn_backMusic.currentFrame == 2)
			{
				isBackSoundOpen = false;
				btn_backMusic.gotoAndStop(1);
				mcIsEnabledB.visible = true;
				backSoundController.soundOnOrOff(false);
				judgeSoundSwitch();
				dataArr[1] = 0;
				lastVolume = GameCommonData.soundVolume;
				sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: 0, updateUI: true, from: this});
			}
			(facade.retrieveMediator(SmallMapMediator.NAME) as SmallMapMediator).soundController.soundPicSet();
		}
		
		/**
		 * 判断场景中小喇叭的状态
		 **/ 
		private function judgeSoundSwitch():void
		{
			if(!isGameSoundOpen && !isBackSoundOpen)
			{
				isSoundSwitchOpen = false;
			}
			else{
				isSoundSwitchOpen = true;
			}
		}
		private function gc():void
		{
			btn_gameMusic.removeEventListener(MouseEvent.CLICK,isOpenMusicHandler);
			btn_backMusic.removeEventListener(MouseEvent.CLICK,isOpenMusicHandler);
			soundSetUI.mc_team.removeEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_trade.removeEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_beat.removeEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_friend.removeEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.mc_save.removeEventListener(MouseEvent.CLICK,settingHandler);
			soundSetUI.btn_save.removeEventListener(MouseEvent.CLICK,settingBtnHandler);
			soundSetUI.btn_default.removeEventListener(MouseEvent.CLICK,settingBtnHandler);
			soundSetUI.btn_modiPwd.removeEventListener(MouseEvent.CLICK,settingBtnHandler);
			if(SystemSettingData.pwdInputIsOpen) {
				closePwdInput(null);
			}
			if(SystemSettingData.pwdModifyIsOpen) {
				closePwdModify(null);
			}
		}
		public function clickFunction():void//点击小喇叭时候面板的状态
		{
			if(isSoundSwitchOpen)
			{
				isGameSoundOpen = false;
				btn_gameMusic.gotoAndStop(1);
				mcIsEnabledG.visible = true;
				
				isBackSoundOpen = false;
				btn_backMusic.gotoAndStop(1);
				mcIsEnabledB.visible = true;
				
				isSoundSwitchOpen = false;
				
				SystemSettingData._dataArr[0] = SystemSettingData._dataArr[1] = 0;
				
				CommonData.IsPlayThemeSongComplete = true; //孔亮 主题音乐已经播放完成
			}
			else
			{
				isGameSoundOpen = true;
				btn_gameMusic.gotoAndStop(2);
				mcIsEnabledG.visible = false;
				
				isBackSoundOpen = true;
				btn_backMusic.gotoAndStop(2);
				mcIsEnabledB.visible = false;
				
				isSoundSwitchOpen = true;
				
				SystemSettingData._dataArr[0] = SystemSettingData._dataArr[1] = 1;
				
				CommonData.IsPlayThemeSongComplete = true; //孔亮 主题音乐已经播放完成
			} 
		}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------		
		
		/** 初始化密码输入、修改界面 */
		private function initPwdPanel():void
		{
			var pwdInput:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SysSetPwdInput");
			var pwdModify:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SysSetPwdModify");
			var pos:Point = UIUtils.getMiddlePos(pwdInput);
			pwdInputPanelBase  = new PanelBase(pwdInput, pwdInput.width-10, pwdInput.height+13);
			pwdModifyPanelBase = new PanelBase(pwdModify, pwdModify.width+9, pwdModify.height+13);
			pwdInputPanelBase.addEventListener(Event.CLOSE, closePwdInput);
			pwdModifyPanelBase.addEventListener(Event.CLOSE, closePwdModify);
			
			pwdInputPanelBase.name = "sysSetPwdInputPanel";
			pwdInputPanelBase.x = pos.x;
			pwdInputPanelBase.y = pos.y; 
			pwdInputPanelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_sysset_med_ssm_3" ]);   //"关闭账号保护锁"
			
			pwdModifyPanelBase.name = "sysSetPwdModiPanel";
			pwdModifyPanelBase.x = pos.x;
			pwdModifyPanelBase.y = pos.y;
			pwdModifyPanelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_sysset_med_ssm_4" ]);   //"修改账号保护密码"
			
			(pwdInputPanelBase.content.txt_inputPwd as TextField).displayAsPassword = true;
			(pwdInputPanelBase.content.txt_inputPwd as TextField).maxChars = 6;
			(pwdInputPanelBase.content.txt_inputPwd as TextField).restrict = "0-9";
			
			(pwdModifyPanelBase.content.txt_oldPwd as TextField).displayAsPassword = true;
			(pwdModifyPanelBase.content.txt_oldPwd as TextField).maxChars = 6;
			(pwdModifyPanelBase.content.txt_oldPwd as TextField).restrict = "0-9";
			(pwdModifyPanelBase.content.txt_newPwd as TextField).displayAsPassword = true;
			(pwdModifyPanelBase.content.txt_newPwd as TextField).maxChars = 6;
			(pwdModifyPanelBase.content.txt_newPwd as TextField).restrict = "0-9";
			(pwdModifyPanelBase.content.txt_rePwd as TextField).displayAsPassword = true;
			(pwdModifyPanelBase.content.txt_rePwd as TextField).maxChars = 6;
			(pwdModifyPanelBase.content.txt_rePwd as TextField).restrict = "0-9";
			
			pwdInputPanelBase.content.txt_infoShow.text = "";
			pwdModifyPanelBase.content.txt_infoShow.text = "";
			pwdInputPanelBase.content.txt_infoShow.mouseEnabled = false;
			pwdModifyPanelBase.content.txt_infoShow.mouseEnabled = false;
			
			initMask();
		}
		
		/** 初始化遮罩 */
		private function initMask():void
		{
			backMask = new Sprite();
			backMask.name = "sysSetMask";
			backMask.graphics.beginFill(0xffffff, 0);
			backMask.graphics.drawRect(0,0,GameCommonData.GameInstance.GameUI.width, GameCommonData.GameInstance.GameUI.height);
			backMask.graphics.endFill();
		}
		
		/** 打开密码输入界面 */
		private function openPwdInput():void
		{
			if(!GameCommonData.GameInstance.GameUI.contains(pwdInputPanelBase)) {
				UIUtils.addFocusLis(pwdInputPanelBase.content.txt_inputPwd);
				UIConstData.FocusIsUsing = true;
				GameCommonData.GameInstance.GameUI.addChild(backMask);
				GameCommonData.GameInstance.GameUI.addChild(pwdInputPanelBase);
				pwdInputPanelBase.stage.focus = pwdInputPanelBase.content.txt_inputPwd;
				addInputLis();
			}
		}
		
		/** 关闭密码输入界面 */
		private function closePwdInput(e:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(pwdInputPanelBase)) {
				removeInputLis();
				UIUtils.removeFocusLis(pwdInputPanelBase.content.txt_inputPwd);
				pwdInputPanelBase.stage.focus = null;
				UIConstData.FocusIsUsing = false;
				pwdInputPanelBase.content.btn_commit.visible = true;
				pwdInputPanelBase.content.btn_cancel.visible = true;
				pwdInputPanelBase.content.txt_inputPwd.text = "";
				pwdInputPanelBase.content.txt_infoShow.text = "";
				GameCommonData.GameInstance.GameUI.removeChild(pwdInputPanelBase);
				GameCommonData.GameInstance.GameUI.removeChild(backMask);
				SystemSettingData.pwdInputIsOpen = false;
			}
		}
		
		/** 打开密码修改界面 */
		private function openPwdModify():void
		{
			if(!GameCommonData.GameInstance.GameUI.contains(pwdModifyPanelBase)) {
				UIUtils.addFocusLis(pwdModifyPanelBase.content.txt_oldPwd);
				UIUtils.addFocusLis(pwdModifyPanelBase.content.txt_newPwd);
				UIUtils.addFocusLis(pwdModifyPanelBase.content.txt_rePwd);
				UIConstData.FocusIsUsing = true;
				GameCommonData.GameInstance.GameUI.addChild(backMask);
				GameCommonData.GameInstance.GameUI.addChild(pwdModifyPanelBase);
				pwdModifyPanelBase.stage.focus = pwdModifyPanelBase.content.txt_oldPwd;
				addModifyLis();
			}
		}
		
		/** 关闭密码修改界面 */
		private function closePwdModify(e:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(pwdModifyPanelBase)) {
				removeModifyLis();
				UIUtils.removeFocusLis(pwdModifyPanelBase.content.txt_oldPwd);
				UIUtils.removeFocusLis(pwdModifyPanelBase.content.txt_newPwd);
				UIUtils.removeFocusLis(pwdModifyPanelBase.content.txt_rePwd);
				pwdModifyPanelBase.stage.focus = null;
				UIConstData.FocusIsUsing = false;
				pwdModifyPanelBase.content.btn_commit.visible = true;
				pwdModifyPanelBase.content.btn_cancel.visible = true;
				pwdModifyPanelBase.content.txt_infoShow.text = "";
				pwdModifyPanelBase.content.txt_oldPwd.text = "";
				pwdModifyPanelBase.content.txt_newPwd.text = "";
				pwdModifyPanelBase.content.txt_rePwd.text = "";
				GameCommonData.GameInstance.GameUI.removeChild(pwdModifyPanelBase);
				GameCommonData.GameInstance.GameUI.removeChild(backMask);
				SystemSettingData.pwdModifyIsOpen = false;
			}
		}
		
		/** 添加监听-密码输入 */
		private function addInputLis():void
		{
			pwdInputPanelBase.content.btn_commit.addEventListener(MouseEvent.CLICK, btnInputClickHandler);
			pwdInputPanelBase.content.btn_cancel.addEventListener(MouseEvent.CLICK, btnInputClickHandler);
		}
		
		/** 移除监听-密码输入 */
		private function removeInputLis():void
		{
			pwdInputPanelBase.content.btn_commit.removeEventListener(MouseEvent.CLICK, btnInputClickHandler);
			pwdInputPanelBase.content.btn_cancel.removeEventListener(MouseEvent.CLICK, btnInputClickHandler);
		}
		
		/** 添加监听-密码修改 */
		private function addModifyLis():void
		{
			pwdModifyPanelBase.content.btn_commit.addEventListener(MouseEvent.CLICK, btnModifyClickHandler);
			pwdModifyPanelBase.content.btn_cancel.addEventListener(MouseEvent.CLICK, btnModifyClickHandler);
		}
		
		/** 移除监听-密码修改 */
		private function removeModifyLis():void
		{
			pwdModifyPanelBase.content.btn_commit.removeEventListener(MouseEvent.CLICK, btnModifyClickHandler);
			pwdModifyPanelBase.content.btn_cancel.removeEventListener(MouseEvent.CLICK, btnModifyClickHandler);
		}
		
		/** 点击按钮-密码输入 */
		private function btnInputClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btn_commit":					//确定
					commitPwdInput();
					break;
				case "btn_cancel":					//取消
					closePwdInput(null);
					break;
			}
		}
		
		/** 点击按钮-密码修改 */
		private function btnModifyClickHandler(e:MouseEvent):void
		{
			switch(e.target.name)
			{
				case "btn_commit":					//确定
					commitPwdModify();
					break;
				case "btn_cancel":					//取消
					closePwdModify(null);
					break;
			}
		}
		
		/** 确定输入密码 */
		private function commitPwdInput():void
		{
			var pwd:String = pwdInputPanelBase.content.txt_inputPwd.text;
			if(!pwd || pwd.length < 6) {
				pwdInputPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_5" ];    //"<font color='#FF0000'>密码为6位数字</font>"
				pwdInputPanelBase.stage.focus = pwdInputPanelBase.content.txt_inputPwd;
				return;
			}
			pwdInputPanelBase.content.btn_commit.visible = false;
			pwdInputPanelBase.content.btn_cancel.visible = false;
			pwdInputPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_6" ];  //"<font color='#00FF00'>请稍后...</font>"
			var pwdEncrypted:String = encrypt(pwd);
			sendNetAction(EncyptAction.SYSSET_COLSE_PWD, "", pwdEncrypted);
		}
		
		/** 确定修改密码 */
		private function commitPwdModify():void
		{
			var pwdOld:String = pwdModifyPanelBase.content.txt_oldPwd.text;
			var pwdNew:String = pwdModifyPanelBase.content.txt_newPwd.text;
			var pwdRet:String = pwdModifyPanelBase.content.txt_rePwd.text;
			if(!pwdOld || pwdOld.length < 6) {
				pwdModifyPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_7" ];             //"<font color='#FF0000'>原密码长度不够</font>"
				pwdModifyPanelBase.stage.focus = pwdModifyPanelBase.content.txt_oldPwd;
				return;
			}
			if(!pwdNew || pwdNew.length < 6) {
				pwdModifyPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_8" ];         //"<font color='#FF0000'>新密码长度不够</font>"
				pwdModifyPanelBase.stage.focus = pwdModifyPanelBase.content.txt_newPwd;
				return;
			}
			if(!pwdRet || pwdRet != pwdNew) {
				pwdModifyPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_9" ];           //"<font color='#FF0000'>两次输入不一致</font>"
				pwdModifyPanelBase.stage.focus = pwdModifyPanelBase.content.txt_rePwd;
				return;
			}
			if(pwdNew == pwdOld) {
				pwdModifyPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_10" ];              //"<font color='#FF0000'>新旧密码需不同</font>"
				pwdModifyPanelBase.stage.focus = pwdModifyPanelBase.content.txt_rePwd;
				return;
			}
			pwdModifyPanelBase.content.btn_commit.visible = false;
			pwdModifyPanelBase.content.btn_cancel.visible = false;
			pwdModifyPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_11" ];                        //"<font color='#00FF00'>请稍后...</font>"
			
			var pwdOldEncrypted:String = encrypt(pwdOld);
			var pwdNewEncrypted:String = encrypt(pwdNew);
			sendNetAction(EncyptAction.SYSSET_MODIFY_PWD, pwdOldEncrypted, pwdNewEncrypted);
		}
		
		/** DES加密 */
		private function encrypt(pwd:String):String
		{
			var ba:ByteArray = Hex.toArray(pwd);
			UIConstData.DES.encrypt(ba);
			var res:String = Hex.fromArray(ba).toUpperCase();
			return res;
		}
		
		/** 保存设置 */
		private function sureSaveSet():void
		{
			timerSave.reset();
			timerSave.start();
			if(dataArr[6] == 0 && isLocked) {		//上次已锁定 且 本次要解锁
				openPwdInput();
				SystemSettingData.pwdInputIsOpen = true;
			} else {
				if(SystemSettingData._dataArr[6] == 0 && dataArr[6] == 1) {  //上次没锁，本次加锁
					sendNetAction(EncyptAction.SYSSET_OPEN_PWD);
					isLocked = true;
				}
				SystemSettingData._dataArr = UIUtils.DeeplyCopy(dataArr) as Array;
				sendNotification(EventList.SEND_QUICKBAR_MSG);
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_12" ], color:0xffff00});      //"保存成功"
			}
		}
		
		/** 发送密码命令 */
		private function sendNetAction(actionType:uint, pwd:String="", newPwd:String=""):void
		{
			var obj:Object = new Object();
			obj.type = actionType;
			obj.oldPwd = pwd;
			obj.newPwd = newPwd;
			EncryptSend.send(obj);
		}
		
		/** 密码操作返回 */
		private function pwdOpRes(data:Object):void
		{
			var type:uint = data.type;
			var res:uint  = data.res;
			switch(type) {
				case 1:							//解锁
					if(res == 1) {	//fail
						if(SystemSettingData.pwdInputIsOpen) {
							pwdInputPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_13" ];    //"<font color='#FF0000'>密码错误</font>"
							pwdInputPanelBase.content.txt_inputPwd.text = "";
							pwdInputPanelBase.content.btn_commit.visible = true;
							pwdInputPanelBase.content.btn_cancel.visible = true;
							pwdInputPanelBase.stage.focus = pwdInputPanelBase.content.txt_inputPwd;
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_14" ], color:0xffff00});     //"密码错误 保存失败"
						}
					} else {		//sucess
						isLocked = false;
						SystemSettingData._dataArr = UIUtils.DeeplyCopy(dataArr) as Array;
						sendNotification(EventList.SEND_QUICKBAR_MSG);
						if(SystemSettingData.pwdInputIsOpen) {
							closePwdInput(null);
						}
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_12" ], color:0xffff00});      //"保存成功"
					}
					break;
				case 2:							//修改密码
					if(res == 1) {	//fail
						if(SystemSettingData.pwdModifyIsOpen) {
							pwdModifyPanelBase.content.txt_infoShow.htmlText = GameCommonData.wordDic[ "mod_sysset_med_ssm_15" ];            //"<font color='#FF0000'>原密码错误</font>"
							pwdModifyPanelBase.content.btn_commit.visible = true;
							pwdModifyPanelBase.content.btn_cancel.visible = true;
							pwdModifyPanelBase.stage.focus = pwdModifyPanelBase.content.txt_oldPwd;
						} else {
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_16" ], color:0xffff00});            //"原密码错误 修改失败"
						}
					} else {		//sucess
						closePwdModify(null);
						timerModify.reset();
						timerModify.start();
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_sysset_med_ssm_17" ], color:0xffff00});                                       //"密码修改成功"
					}
					break;
				case 3:							//接到密钥
					if(!UIConstData.DES) {
						var key:String = data.key;
						UIConstData.DES = new DESKey(Hex.toArray(key));
					}
			}
			 
		}
	}
}