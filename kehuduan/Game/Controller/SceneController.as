package Controller
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Arena.Command.ArenaPanelCommandList;
	import GameUI.Modules.Arena.Mediator.ArenaPanelMediator;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.ArenaMsgMediator;
	import GameUI.Modules.IdentifyingCode.Data.IdentifyingCodeData;
	import GameUI.Modules.MusicPlayer.Command.MusicPlayerCommandList;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Modules.Relive.Data.ReliveEvent;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.MouseCursor.DestinationCursor;
	import GameUI.MouseCursor.RepeatRequest;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionProcessor.MapItem;
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.PlayerActionSend;
	import Net.ActionSend.Zippo;
	import Net.Protocol;
	
	import OopsEngine.AI.MapPathFinder.MapFinder;
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Role.SkinNameController;
	import OopsEngine.Scene.CommonData;
	import OopsEngine.Scene.GameScene;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	import OopsEngine.Scene.StrategyScene.GameScenePlay;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	import OopsEngine.Skill.GameSkillResource;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	import flash.utils.setTimeout;
	
	/** 游戏场景控制 */	
	public class SceneController
	{
		/** 场景是否加载完成 */
		public var IsSceneLoaded:Boolean = false;			// 用于Net.ActionProcessor.PlayerInfo中判断是否加载上线玩家到游戏场景中
		
		public var IsFirstLoad:Boolean = false;             // 是否第1次加载
		public var IsCanController:Boolean = false;         // 是否可以操作
		public var playerdistance:int = 0;                  // 玩家攻击距离
		public var gameScenePlay:GameScenePlay;			    // 游戏场景对象
		private var moveStepCount:int = 0;					// 每开始一步的记数
		private var BeforeTargetPoint:Point;			    // 上次移动目标点
		private var NextPoint:Point;                        // 没执行的点
		
		private var PetmoveStepCount:int    = 0;			// 宠物每开始一步的记数
		private var combat:CombatController = new CombatController();
		public var SceneVerse:MovieClip;	
        public  var loadCircle:MovieClip;                   //加载图标
        
        public var ScenePlayerCount:int = 0;                //同场景人数
        public var begin:Boolean = true;
        public var hp:Boolean = true;
        
        						
		public function SceneController(name:String,mapId:String)
		{
			GameCommonData.GameInstance.GameScene.GameSceneTransferOpen = onGameSceneTransferOpen;
			GameCommonData.GameInstance.GameScene.GameSceneLoadComplete = onGameSceneLoadComplete;
			GameCommonData.GameInstance.GameScene.StartScene(GameCommonData.SCENE_GAME);
			GameCommonData.GameInstance.GameScene.TransferScene(name,mapId);
		}
		
		/** 开始转场事件  */
		private function onGameSceneTransferOpen():void
		{
			if(this.SceneVerse!=null && this.SceneVerse.parent!=null)
			{
				GameCommonData.GameInstance.GameUI.removeChild(this.SceneVerse);
			}
		}
		
		/**清空技能*/
		public function ClearSkill():void
		{
			// 添加其它同显示区的玩家
			for (var key:Object in GameCommonData.SkillOnLoadEffectList)
			{               
				var skillResource:GameSkillResource = GameCommonData.SkillOnLoadEffectList[key] as GameSkillResource;			
				var skill:GameSkill = GameCommonData.SkillList[skillResource.SkillID] as GameSkill;
				
			    if(!(skill.Job == GameCommonData.Player.Role.MainJob.Job
			    || skill.Job == GameCommonData.Player.Role.ViceJob.Job
			    || skill.SkillMode == 1) &&  ! GameSkillMode.IsPetSkill(skill.SkillMode)
			    ) 
			    {
					skillResource.EffectBR = null;
					skillResource.geed     = null;
					skillResource          = null;
					GameCommonData.SkillLoadEffectList[key] = null;
					delete GameCommonData.SkillLoadEffectList[key];
					GameCommonData.SkillOnLoadEffectList[key] = null;
					delete GameCommonData.SkillOnLoadEffectList[key];
			    }
			}
		}
		
		/** 转移场景  */
		public function TransferScene(name:String,mapId:String):void
		{		
			CommonData.StopLoad(true);
		    this.gameScenePlay.IsUpdateNicety = false;
			this.IsSceneLoaded    = false;
			this.IsCanController = false;
			setTimeout(SetCanController,5000);	
				
			if(mapId != GameCommonData.GameInstance.GameScene.GetGameScene.MapId)
			{
				if(GameCommonData.Scene.loadCircle == null)
				{		
					GameCommonData.Scene.loadCircle   = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("LoadCircle") as MovieClip;	
					GameCommonData.Scene.loadCircle.x = GameCommonData.GameInstance.ScreenWidth  / 2;	
					GameCommonData.Scene.loadCircle.y = GameCommonData.GameInstance.ScreenHeight / 2;		
					GameCommonData.GameInstance.GameUI.addChild(GameCommonData.Scene.loadCircle);
				}
				GameCommonData.GameInstance.GameScene.TransferScene(name,mapId);
				GameCommonData.UIFacadeIntance.sendNotification(EventList.CLEARCOPYTIME);
			}
			else
			{
				CommonData.StopLoad(false);
				/**清空元件**/
				GameCommonData.GameInstance.GameScene.GetGameScene.ClearElement();
	       
			    //发送236
				SendOnLoadComplete();	
								
				// 初始化鼠标点
				DestinationCursor.getInstance(GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer);
				this.IsSceneLoaded = true;		
				
				/**添加自己**/
				AddOwner();
										
				/**飞行**/
				GameCommonData.Scene.StopPlayerMove(GameCommonData.Player);
                GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
                
                /**核对小地图坐标**/
                UIFacade.UIFacadeInstance.updateSmallMap({type:1,id:0});
			    UIFacade.UIFacadeInstance.updateSmallMap({type:5,id:0});
			   	UIFacade.UIFacadeInstance.updateSmallMap({type:6,id:0});
			   	
			    this.gameScenePlay.Background.LoadMap();					   		   		
			}
			
			//取消挂机
            if(GameCommonData.Player.IsAutomatism)
            {
				PlayerController.EndAutomatism();	
            }
			
			//转移场景的时候，关闭自动寻路
			UIFacade.UIFacadeInstance.closeOpenPanel();
		}
	
	    /**清除数据信息**/
	    public function ClearElementData():void
	    {
	    	var a:FlutterController = new FlutterController();
	    	/**清楚地效**/
	    	FloorEffectController.ClearFloorEffect();
	    	/**清除人物选中目标**/
			GameCommonData.TargetAnimal 	   = null;
			/**清楚人物移动点**/
			GameCommonData.Player.MustMovePoint = null;	
		    /**清除人物击打的目标**/
			GameCommonData.AttackAnimal  	   = null;
			/**清除是否向人物移动**/
			GameCommonData.IsMoveTargetAnimal  = false;
			/**清除宠物攻击目标**/
			GameCommonData.PetTargetAnimal 	   = null;		
			/** 创建场景上的包集合 */
			GameCommonData.PackageList 		   = new Dictionary();
			this.begin = true;
			/** 创建其它在线玩家集合  */
			GameCommonData.SameSecnePlayerList = new Dictionary();
			GameCommonData.TargetCommon        = 0;	
			ScenePlayerCount                   = 0;	
			ClearSkill();		
            /**结束挂机*/
		    PlayerController.EndAutomatism();			
			//清除 移动和攻击信息
			GameCommonData.Player.prepPoint         = null;
			GameCommonData.Player.handler           = null;
			BeforeTargetPoint                       = null;
			NextPoint                               = null;			
			this.ResetMoveState();
		    this.PetResetMoveState();			
		    GameCommonData.UIFacadeIntance.clearTargetPhoto();	//GetKeyCode(Keyboard.ESCAPE);  by Ginoo 2011.1.5
		    TransferSceneController.PKShowTime      = 0;
	    }
	    
	    /**发送236信息 **/
	    public function SendOnLoadComplete():void
	    {
	    	//清空信息
			ClearElementData();
			
	    	// 发了此信息后，服务器才会发送用户上线，用户移动信息。
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(236);							//进入地图
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);	 
	    }
	    
//	    /**加载元件**/
//	    public function AddElement():void
//	    {	
//			// 添加其它同显示区的玩家
//			for (var key:Object in GameCommonData.SameSecnePlayerList)
//			{
//				this.AddPlayer(GameCommonData.SameSecnePlayerList[key]);
//			}
//			
//			// 加载地图上的包
//			for (var keyPackage:Object in GameCommonData.PackageList)
//			{
//				var p:Point = MapTileModel.GetTilePointToStage(GameCommonData.PackageList[keyPackage].TileX, GameCommonData.PackageList[keyPackage].TileY);
//				GameCommonData.PackageList[keyPackage].x = p.x;
//				GameCommonData.PackageList[keyPackage].y = p.y;
//				GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.addChild(GameCommonData.PackageList[keyPackage]);
//			}
//	    }
	    
	    /**添加自己**/
        public function AddOwner():void
        {		
            this.AddPlayer(GameCommonData.Player);
                  
            GameCommonData.Player.AddTernal();
//            PlayerSkinsController.SetSkinWeapenData(GameCommonData.Player);
			// 加载地图块
			this.gameScenePlay.Background.LoadMap();			
			
			//判断是否是PK地图
			if(!TargetController.IsPKTeam() && GameCommonData.Player.Role.PKteam != 0)
			{
				GameCommonData.Player.Role.NameColor       = PKController.GetFontColor(GameCommonData.Player.Role.PkValue);
				GameCommonData.Player.Role.NameBorderColor = PKController.GetBorderColor(GameCommonData.Player.Role.PkValue);
				GameCommonData.Player.UpdatePersonName();
			}
			
			if(GameCommonData.AnalyzeTime == null)
			{
				GameCommonData.AnalyzeTime = new Timer(500,0);
				GameCommonData.AnalyzeTime.start();
				GameCommonData.AnalyzeTime.addEventListener(TimerEvent.TIMER,CommonData.AnalyzeUpdate);
			}		
        }
	
		/** 场景加载完成  */
		private function onGameSceneLoadComplete(curretnScene:GameScene):void
		{
			CommonData.StopLoad(false);
			// 删除生活技能提示
			GameCommonData.Player.SetMissionPrompt(0);  
			
			if(loadCircle != null)
			{
				GameCommonData.GameInstance.GameUI.removeChild(GameCommonData.Scene.loadCircle);
				GameCommonData.Scene.loadCircle = null;
			}

			if(curretnScene is GameScenePlay)
			{
				// 初始化鼠标点
				DestinationCursor.getInstance(curretnScene.BottomLayer);
				
				this.gameScenePlay 		       = curretnScene as GameScenePlay;
				this.gameScenePlay.MouseDown   = onMouseDown;
				
				GameCommonData.Player.MoveComplete                              = onPlayMoveComplete;
				GameCommonData.Player.MoveStep 	                                = onPlayMoveStep;
				GameCommonData.Player.MoveNode 		                            = onMoveNode;
				GameCommonData.Player.UpdateSkillEffect                         = PlayerController.onUpdateSkillEffect;
				GameCommonData.Player.CheckPoint                                = PlayerController.CheckPoint;
				GameCommonData.Player.MustMove                                  = PlayerController.MustMove;
				GameCommonData.Player.Attack                                    = PlayerController.UpdateAttack;
				GameCommonData.GameInstance.GameScene.GetGameScene.UpdateNicety = updateSmallMap;
				
				// 地图初始居中
				var p:Point = MapTileModel.GetTilePointToStage(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY);
				var targetPoint:Point = this.gameScenePlay.SceneMove(p.x,p.y,true);
				this.gameScenePlay.x  = int(targetPoint.x);
				this.gameScenePlay.y  = int(targetPoint.y);
				
				if(GameCommonData.wordVersion == 2)	//台服
				{
					if(SelfInfoMediator.isFBScene)  //从副本出来
					{
						SelfInfoMediator.isFBScene = false;
						GameCommonData.UIFacadeIntance.sendNotification(TaskCommandList.SEND_FB_AWARD,11);	//成就11
					}
				}
				//如果是FB则要下坐骑
				if(GameCommonData.GameInstance.GameScene.GetGameScene.name != GameCommonData.GameInstance.GameScene.GetGameScene.MapId)
				{
					if(GameCommonData.wordVersion == 2) //台服
					{
						SelfInfoMediator.isFBScene = true;	
					}
					 GameCommonData.Player.Role.MountSkinID = 0;
				     PlayerSkinsController.SetSkinMountData(GameCommonData.Player);		
			    }	
				
				if(!GameCommonData.IsLoadUserInfo)	// 人物坐标配置数据，只加载一次，要不会数据乱，报错。
				{						
					//设置自己信息		
                    SetUser();   
                    var dataProxy:DataProxy = UIFacade.GetInstance(UIFacade.FACADEKEY).retrieveProxy(DataProxy.NAME) as DataProxy;
                    
                    if(AudioController.Islogin)
                    {
                    	if(AudioController.loginBgSound == null)
                    	{
                    		CommonData.IsPlayThemeSongComplete   = true;
                    	}
                    	else
                    	{
	                        GameCommonData.GameInstance.GameScene.GetGameScene.audioEngine               = AudioController.loginBgSound;
	                    	CommonData.MusicUrl                  = GameCommonData.GameInstance.Content.RootDirectory + "/" + "LoginSound.mp3";
	                    	CommonData.IsPlayThemeSong           = true;
//	                    	CommonData.IsPlayThemeSongComplete   = false;
	                    	if(GameCommonData.GameInstance.GameScene.GetGameScene.audioEngine.soundChannel != null)
	                    	{
	                    		GameCommonData.GameInstance.GameScene.GetGameScene.audioEngine.soundChannel.addEventListener(Event.SOUND_COMPLETE,GameCommonData.GameInstance.GameScene.GetGameScene.audioEngine.onComplete);	
					    		GameCommonData.GameInstance.GameScene.GetGameScene.audioEngine.PlayComplete  = GameCommonData.GameInstance.GameScene.GetGameScene.onPlayComplete;		
	                    	}
                    	}
                    }
                    else
                    {
                    	CommonData.IsPlayThemeSong           = false;
                    	CommonData.IsPlayThemeSongComplete    = true;
                    }
                    
					GameCommonData.IsLoadUserInfo = true;
					// 开始加载背包
					//todo 加定时器（进行第一次的背包物品的请求）
					RepeatRequest.getInstance().bagItemCount = 0;
					RepeatRequest.getInstance().addDelayProcessFun("bag",NetAction.RequestItems,2000);
					
					// 结束加载背包					
					NetAction.RequestItems();		
					
					if(GameCommonData.wordVersion == 2)	//台服
					{
						GameCommonData.UIFacadeIntance.sendNotification(TaskCommandList.SEND_FB_AWARD,1);	//成就1
					}	
				}
				else
				{	
					//是否切线
					if(GameCommonData.IsChangeOnline)
					{
						//设置自己信息		
                    	SetUser();
                    	SendOnLoadComplete();
                    	UIFacade.UIFacadeInstance.chgLineSuc();
                    	GameCommonData.IsChangeOnline    = false;
                    	GameCommonData.Scene.IsFirstLoad = true; 
                    	PlayerActionSend.PlayerAction({type:1010,data:[0,0,0,0,0,0,PlayerAction.CLIENT_INIT_ALL_COMP,0,0]});	//发送 客户端完全初始化成功命令
					}			
					else
					{
						//发送236
						SendOnLoadComplete();
					}
				}


				//添加自己
				AddOwner();
				this.IsSceneLoaded = true;
				
				// 场景诗句
				SceneVerse				 = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SceneVerse");
				SceneVerse.mouseChildren = false;
				SceneVerse.mouseEnabled  = false;
				SceneVerse.x 			 = (GameCommonData.GameInstance.ScreenWidth  - SceneVerse.width)  / 2;
				SceneVerse.y 			 = (GameCommonData.GameInstance.ScreenHeight - SceneVerse.height) / 2 - 170;
				SceneVerse.Container.txtMapName.text 	 = this.gameScenePlay.MapName;
				SceneVerse.Container.txtDescription.text = this.gameScenePlay.Description;
				GameCommonData.GameInstance.GameUI.addChild(SceneVerse);
				UIFacade.UIFacadeInstance.updateSmallMap({type:7,id:0});
						   
				// 场景音乐
				if(GameCommonData.isOpenSoundSwitch == true)
				{
					CommonData.IsPlayThemeSong = false;
//	           		GameCommonData.GameInstance.GameScene.GetGameScene.MusicLoad(GameCommonData.soundVolume);
	           		GameCommonData.UIFacadeIntance.sendNotification(MusicPlayerCommandList.MUSICPLAYER_SCENECHANGE, GameCommonData.GameInstance.GameScene.GetGameScene.name);
	 			}
	 			
		 		if(GameCommonData.TargetScene.length > 0)
				{	
					if(GameCommonData.TargetScene != GameCommonData.GameInstance.GameScene.GetGameScene.name)
					{	
						GameCommonData.Scene.MoveNextScenePoint();
					}
					else
					{
						GameCommonData.Scene.playerdistance = GameCommonData.TargetDistance;
						GameCommonData.Scene.PlayerMove(MapTileModel.GetTilePointToStage(GameCommonData.TargetPoint.x,GameCommonData.TargetPoint.y));
						GameCommonData.TargetScene = "";
					}
				}
				
				if (GameCommonData.GameInstance.GameScene.GetGameScene.name == "1049" ||
					GameCommonData.GameInstance.GameScene.GetGameScene.name == "1050" ||
					GameCommonData.GameInstance.GameScene.GetGameScene.name == "1051") // 竞技场
				{
					GameCommonData.UIFacadeIntance.registerMediator(new ArenaPanelMediator());
					GameCommonData.UIFacadeIntance.registerMediator(new ArenaMsgMediator());
					GameCommonData.UIFacadeIntance.sendNotification(ArenaPanelCommandList.ARENASMALLPANEL_SHOW);
					GameCommonData.UIFacadeIntance.sendNotification(ChatEvents.ARENA_MSG_PANEL_OPEN);
				}
				else
				{
					GameCommonData.UIFacadeIntance.sendNotification(ArenaPanelCommandList.ARENASMALLPANEL_HIDE);
					GameCommonData.UIFacadeIntance.sendNotification(ChatEvents.ARENA_MSG_PANEL_CLOSE);
					(GameCommonData.UIFacadeIntance.retrieveProxy(DataProxy.NAME) as DataProxy).lastUsedMsgPanel = "chat";
					GameCommonData.UIFacadeIntance.removeMediator(ArenaPanelMediator.NAME);
					GameCommonData.UIFacadeIntance.removeMediator(ArenaMsgMediator.NAME);
				}
				
				if (GameCommonData.GameInstance.GameScene.GetGameScene.name == "1026")
				{
					GameCommonData.UIFacadeIntance.sendNotification(ReliveEvent.REMOVERELIVE);
				}							
			}
			
		}
		
		/**设置当前玩家信息**/
		public function SetUser():void
		{
			var mountData:XML = GameCommonData.ModelOffsetMount[GameCommonData.Player.Role.MountSkinName + "_" + GameCommonData.Player.Role.Sex];
			if(mountData!=null)
			{
				GameCommonData.Player.MountOffset		  = new Point(mountData.@X,mountData.@Y);
				GameCommonData.Player.MountHeight		  = mountData.@H;
			}
			
			var playerData:XML;
			if(GameCommonData.Player.Role.IsShowDress)
				playerData	= GameCommonData.ModelOffsetPlayer[PlayerSkinsController.GetOffsetPlayer(GameCommonData.Player.Role.PersonSkinID,GameCommonData.Player.Role.Sex,GameCommonData.Player.Role.CurrentJobID)];
			else
				playerData	= GameCommonData.ModelOffsetPlayer[PlayerSkinsController.GetOffsetPlayer(0,GameCommonData.Player.Role.Sex,GameCommonData.Player.Role.CurrentJobID)];
			
			GameCommonData.Player.Offset 		        = new Point(playerData.@X,playerData.@Y);					// 时装偏移值
			GameCommonData.Player.OffsetHeight			= playerData.@H;
			
			if(!PlayerSkinsController.IsShowWeaponEffect(GameCommonData.Player.Role.WeaponSkinID))
			{
				GameCommonData.Player.Role.WeaponEffectModel = 0;
			}		
			var skinNameController:SkinNameController = PlayerSkinsController.GetSkinName(GameCommonData.Player);
     		GameCommonData.Player.Role.MountSkinName     = PlayerSkinsController.GetMount(GameCommonData.Player.Role.MountSkinID);
			GameCommonData.Player.Role.PersonSkinName    = skinNameController.PersonSkinName;	
			GameCommonData.Player.Role.WeaponSkinName    = skinNameController.WeaponSkinName;	
			GameCommonData.Player.Role.WeaponEffectName  = skinNameController.WeaponEffectSkinName;
			GameCommonData.Player.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
			GameCommonData.Player.Role.WeaponDiaphaneity = skinNameController.WeaponDiaphaneity;
		}
		
		/** 玩家KickBack停止移动打回服务器 */
		public function StopPlayerMove(player:GameElementPlayer):void
		{
			trace("服务器打回: X:"+player.Role.TileX+" Y:"+player.Role.TileY);
			this.ResetMoveState();
			this.PetResetMoveState();
			this.SetPlayerPos(player);
			player.AddTernal();
			player.MustMovePoint = null;
			this.SetScenePos(true);
		}
		
		/**取消点**/
		public function ClearAutoPath():void 
		{	
		    if(GameCommonData.isAutoPath)
		    {
				GameCommonData.isAutoPath = false;
				UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(2);			
		    }
		}
		
		/**关闭NPC对话框**/
		public function CloseNPCDialog():void
		{
			if(GameCommonData.NPCDialogIsOpen)
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(3);			
			}		
		}
		
		/** 停止人物移动不关NPC商店 */
		public function PlayerStopAtIC(isClearTargetScene:Boolean = true):void
		{	
			GameCommonData.Player.Stop();
			
			if(GameCommonData.Player.Role.HP > 0)
				GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
			GameCommonData.IsFollow = false; 
			this.ResetMoveState();	
			this.NextPoint = null;	
			GameCommonData.Player.MustMovePoint = null;		
		    ClearAutoPath();	
		    if(isClearTargetScene)
		    	GameCommonData.TargetScene ="";
		    
		    if(GameCommonData.Player.Role.UsingPetAnimal != null)
		    {
		    	GameCommonData.Player.Role.UsingPetAnimal.Stop();
				GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);
		    }
		}
		
		public function PlayerStop(isClearTargetScene:Boolean = true):void
		{	
			DestinationCursor.Instance.hide();
			GameCommonData.Player.Stop();
			
			if(GameCommonData.Player.Role.HP > 0)
				GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
			GameCommonData.IsFollow = false; 
			this.ResetMoveState();	
			this.NextPoint = null;	
			GameCommonData.Player.MustMovePoint = null;		
			CloseNPCDialog();
		    ClearAutoPath();	
		    if(isClearTargetScene)
		    	GameCommonData.TargetScene ="";
		    
		    if(GameCommonData.Player.Role.UsingPetAnimal != null)
		    {
		    	GameCommonData.Player.Role.UsingPetAnimal.Stop();
				GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);
		    }
		}
		
		/**攻击
		 * 攻击对象
		 * 技能编号 默认
		 * **/
		public function Attack(targetAnimal:GameElementAnimal,skillID:int = 0):void
		{
			//判断是否可以移动 在攻击距离不够的时候要进行移动
			var IsMove:Boolean = false;
			var nextPoint:Point = new Point();
				
		    var skill:GameSkill;	    
		    
		    if(skillID != 0)
		  	   skill = GameCommonData.SkillList[skillID] as GameSkill;
			
		    //点击人物判断
			if(targetAnimal !=null && GameCommonData.Player.Role.HP > 0 &&  GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_DEAD)       //点击了人物 
			{
							
			    if(PlayerController.IsUseSkill(GameCommonData.Player,targetAnimal,skill))                              //对象是否可攻击
			    {
			    	GameCommonData.AttackAnimal = targetAnimal;
			    	
			    	GameCommonData.IsMoveTargetAnimal=true;
			    	CloseNPCDialog();
			        if(skillID == 0)
			        { 
			        	skillID = AutomatismController.GetCanUseSkill(); //判断是否有可以使用的技能	
			        	
			        	if(skillID == 0)
                    		skill = PlayerController.GetDefaultSkill(GameCommonData.Player);  //获取普通攻击信息
                    	else
                    	    skill =  GameCommonData.SkillList[skillID] as GameSkill; //获取使用技能的信息
	                }
	                else
	                {
//	                	if(skillID != 0)
//	                		skill =  GameCommonData.SkillList[skillID] as GameSkill;
//	                	else
//	                	{
							skill =  GameCommonData.SkillList[skillID] as GameSkill;
//	                		skill = PlayerController.GetDefaultSkill(GameCommonData.Player);	
//	                	}
	                }
	                
	                GameCommonData.Player.Role.DefaultSkill = skillID;	
	                		            
			        this.playerdistance =  skill.Distance;
			        
		       		// 判断是否在范围内
		       		if(DistanceController.PlayerTargetAnimalDistance(GameCommonData.TargetAnimal,playerdistance))
		       		{     			
			            GameCommonData.Player.Stop();
			            if(GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK && GameCommonData.Player.Role.HP > 0)
			            	GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
			            
			            GameCommonData.Player.MustMovePoint = null;	 
			            
			            if(!GameCommonData.Player.IsAutomatism)
			            	GameCommonData.Player.IsAddAttack = true;
			            				                       	
			        	UseSkill(targetAnimal);		            
			            IsMove=false; 
			            this.ResetMoveState();
			            this.PetResetMoveState();
			  		 }
			  		 else
			  		 {		
			  		 	//不是可以走的点
						if(!this.gameScenePlay.Map.IsPass(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY))
						{
							var CanMovePoint:Point;
							CanMovePoint = AStarController.GetNearPoint(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),    //当前的点
						    new Point(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY),true);  
												     
							//判断是否有路可以走
							if(CanMovePoint != null)
							{
								playerdistance = 0;
								nextPoint      = MapTileModel.GetTilePointToStage(CanMovePoint.x, CanMovePoint.y);
							    ClearAutoPath();
							    CloseNPCDialog();
							    IsMove=true;
							}
						}
						else
						{			  		 		  		 	
			  		 		nextPoint =   MapTileModel.GetTilePointToStage(targetAnimal.Role.TileX,targetAnimal.Role.TileY);  		 
			  		 		IsMove=true;
			  			} 		 			  		 	
			  		 }
				 }
			}
			else
			{
				CloseNPCDialog();
			}
		    
		    if(IsPlayerWalk() == false)
		    {
		    	IsMove = false;
		    }
		
			if(IsMove)
			{	
			    //不跨场景走路了
			    GameCommonData.TargetScene ="";  
				UIFacade.GetInstance(UIFacade.FACADEKEY).changePath();
				PlayerMove(nextPoint);
			}		
		}	
		
		
		public function DistanceUseSkill(playerdistance:int):void
		{
			//玩家没死亡
			if(GameCommonData.Player.Role.HP > 0 &&  GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				if(GameCommonData.AttackAnimal != null)
				{
				   var skill:GameSkill = GameCommonData.SkillList[GameCommonData.Player.Role.DefaultSkill] as GameSkill;
					
					// 判断是否在范围内
		       		if(DistanceController.PlayerTargetAnimalDistance(GameCommonData.AttackAnimal, playerdistance) || (playerdistance == 0 && GameSkillMode.GetAutomatismState(skill.SkillMode) == 3))
		       		{
			            GameCommonData.Player.Stop();
			            if(GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK && GameCommonData.Player.Role.HP > 0)
			            	GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
			            
			            GameCommonData.Player.MustMovePoint = null;		            	            	
			        	UseSkill(GameCommonData.AttackAnimal);
                           
			            this.ResetMoveState();
			            this.PetResetMoveState();
			            
			            GameCommonData.IsMoveTargetAnimal = false;
			  		 }
		        }
	       }
		}
		
		
		public function DistanceTargetUseSkill(playerdistance:int,target:GameElementAnimal):void
		{
			//玩家没死亡
			if(GameCommonData.Player.Role.HP > 0 &&  GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_DEAD)
			{
				if(target != null)
				{
				   var skill:GameSkill = GameCommonData.SkillList[GameCommonData.Player.Role.DefaultSkill] as GameSkill;
					
					// 判断是否在范围内
		       		if(DistanceController.PlayerTargetAnimalDistance(target, playerdistance) || (playerdistance == 0 && GameSkillMode.GetAutomatismState(skill.SkillMode) == 3))
		       		{
			            GameCommonData.Player.Stop();
			            if(GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK && GameCommonData.Player.Role.HP > 0)
			            	GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
			            
			            GameCommonData.Player.MustMovePoint = null;
			            
			            var playerSkill:GameSkillLevel =  GameCommonData.Player.Role.SkillList[skill.SkillID] as GameSkillLevel;		            	            		            
			        	PlayerController.UsePlayerSkill(playerSkill,target,null);                           
			            this.ResetMoveState();
			            this.PetResetMoveState();
			            
			            GameCommonData.IsMoveTargetAnimal = false;
			  		 }
		        }
	       }
		}
		
		
		public function SetCanController():void
		{
			this.IsCanController = true;
		} 
		
		/** 控制玩家角色移动 */
		public function onMouseDown(e:MouseEvent):void
		{    		
			//是否挂机 
            if(GameCommonData.Player.IsAutomatism)
            {
            	//取消挂机
				PlayerController.EndAutomatism();	
            }
            
            if(IdentifyingCodeData.isShowView == true)
            {
            	trace("YanZhengMaData.isShowView");
				return;
            }
            
			//切线状态不能移动	
			if(ChgLineData.isChgLine)
			{
				trace("ChgLineData.isChgLine");
				return;
			}
			
			//档前第一次没加载 完成 保证必须的包
			if(this.IsFirstLoad == false)
			{
				trace("this.IsFirstLoad");
				return;
			}
			
			//是否加载完成
			if(this.IsSceneLoaded == false)
			{
				trace("this.IsSceneLoaded");
				return;
			}
			
            //取消攻击对象
            GameCommonData.AttackAnimal = null;

		    //跟随取消	
			GameCommonData.IsFollow = false; 								
			
			//转换成A*的点	
			var ClickPoint:Point = MapTileModel.GetTileStageToPoint(this.gameScenePlay.mouseX, this.gameScenePlay.mouseY);	
			
			//地效 群攻技能的释放
		    if(GameCommonData.Rect != null)
		    {
		    	FloorEffectController.ClearFloorEffect();
		    	
		    	var gamskill:GameSkill = GameCommonData.SkillList[GameCommonData.RectSkillID];
		    	//判断宠物的技能 还是人的技能
			    if(GameSkillMode.IsPetSkill(gamskill.SkillMode))
			    {
		    		PetController.PetUseSkill(GameCommonData.RectSkillID,null,ClickPoint);
		    	}
		    	else
		    	{
		    		PlayerController.PlayerUseSkill(GameCommonData.RectSkillID,ClickPoint);
		    	}
		    	CloseNPCDialog();
		    	ClearAutoPath();
		    	GameCommonData.TargetScene ="";
		    	return;
		    }
				
			var IsMove:Boolean       = true;                                                                  // 是否可以移动
			var IsShowCursor:Boolean = true;                                                           		  // 是否显示鼠标
			var nextPoint :Point     = new Point(this.gameScenePlay.mouseX, this.gameScenePlay.mouseY);       // 下个移动目标
			this.playerdistance 	 = 0;	                                                              	  // 重置距离
			
			
			
			// 判断是否点的地图
			if(e.target.name == this.gameScenePlay.name)												// 在选中人物的坐骑后，在点坐骑下空白区不可以移动，是因为坐骑的层把
			{					
				SysCursor.GetInstance().isLock    = false;
				GameCommonData.IsMoveTargetAnimal = false;                                              // 取消向对象移动
				GameCommonData.TargetCommon       = 0;                                               	// 普通攻击目标
			}	
			else
			{
				nextPoint = new Point(this.gameScenePlay.mouseX, this.gameScenePlay.mouseY);                  
			}					 	 		 
					 	 		 
			//判断点击的点是否是有效点
			if((!this.gameScenePlay.Map.IsPass(ClickPoint.x,ClickPoint.y) &&                                            
			    !(GameCommonData.TargetAnimal!=null && GameCommonData.IsMoveTargetAnimal==true)) ||                        	 //判断不是地图的点 并且在不是地图点的时候 点击的不是人物   
			     (GameCommonData.Player.Role.TileX == ClickPoint.x && GameCommonData.Player.Role.TileY == ClickPoint.y) &&      //是否点击的是脚下的点
			    !(GameCommonData.TargetAnimal!=null && GameCommonData.IsMoveTargetAnimal==true))                          		 //判断是否与怪重合了
			{
				//不是可以走的点
				if(!this.gameScenePlay.Map.IsPass(ClickPoint.x,ClickPoint.y))
				{
					var CanMovePoint:Point = AStarController.GetNearPoint(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),    //当前的点
				    MapTileModel.GetTileStageToPoint(this.gameScenePlay.mouseX, this.gameScenePlay.mouseY));  
										     
					//判断是否有路可以走
					if(CanMovePoint != null)
					{
						IsMove       = true; 
						IsShowCursor = true;
						nextPoint    = MapTileModel.GetTilePointToStage(CanMovePoint.x, CanMovePoint.y);
					    ClearAutoPath();
					    CloseNPCDialog();
					}
					else
					{
						IsMove       = false; 
						IsShowCursor = false;
					}
				}
				else
				{
					IsMove       = false; 
					IsShowCursor = false;
				}
			}
			else
			{
				ClearAutoPath();
	
				DestinationCursor.Instance.hide();                                                     //取消鼠标的点击效果                   
							
				//点击其他物体判断
				if(e.target.name.indexOf(MapItem.PackageTypeName)>-1)                                  //捡包
				{
					var obj:Object = new Object();
        			obj.items      = e.target.Data;
          			obj.id         = e.target.ID;
			        GameCommonData.UIFacadeIntance.PickItem(obj);       
					IsMove       = false; 
					IsShowCursor = false;
				}
				
                //点击人物判断
				if(GameCommonData.TargetAnimal!=null && GameCommonData.IsMoveTargetAnimal==true)       //点击了人物 
				{	
			 		GameCommonData.Player.Role.DefaultSkill = 0;
					GameCommonData.TargetScene ="";
				    UIFacade.UIFacadeInstance.selectPlayer();
				    
				    //判断是否点中了摆摊角色上的文字
				    if(GameCommonData.TargetAnimal.Role.Face == 30000)
				    {
				    	IsMove       = false; 
				    	IsShowCursor = false; 
				    	CloseNPCDialog();
				    }
				    else
				    {
				    	if(PlayerController.IsUseSkill(GameCommonData.Player,GameCommonData.TargetAnimal))                              //对象是否可攻击
				    	{
				    					    		
				    		CloseNPCDialog();
				    		
				    		if(TargetController.IsAttack(GameCommonData.TargetAnimal))
				    	    {
				    	    	//赋值攻击对象
				    	    	GameCommonData.AttackAnimal = GameCommonData.TargetAnimal;
				    	    	
					    		//技能距离计算
					            var skill:GameSkill;
					
					            if(GameCommonData.Player.Role.DefaultSkill == 0)
				                {
				                	var skillID:int = AutomatismController.GetCanUseSkill();
				                	GameCommonData.Player.Role.DefaultSkill = skillID;
				                	
				                	if(skillID == 0)
			                    		skill = PlayerController.GetDefaultSkill(GameCommonData.Player);	
			                    	else
			                    	    skill =  GameCommonData.SkillList[skillID] as GameSkill;
					       		}
					       		else
					       		{
						        	skill =  GameCommonData.SkillList[GameCommonData.Player.Role.DefaultSkill] as GameSkill ;
					       		}
					            
//					            if(skillID == 0)
//						        { 
//						        	skillID = AutomatismController.GetCanUseSkill();
//						        	
//						        	if(skillID == 0)
//			                    		skill = PlayerController.GetDefaultSkill(GameCommonData.Player);	
//			                    	else
//			                    	    skill =  GameCommonData.SkillList[skillID] as GameSkill;
//				                }
//				                else
//				                {
//				                	skill =  GameCommonData.SkillList[skillID] as GameSkill;
//				                }
					            
					            				            
					            this.playerdistance =  skill.Distance;
					             
					       		// 判断是否在范围内
					       		if(DistanceController.PlayerTargetAnimalDistance(GameCommonData.TargetAnimal,playerdistance))
					       		{
						            GameCommonData.Player.Stop();
						            if(GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK && GameCommonData.Player.Role.HP >0)
						            	GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);	            
						            
						            if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.Role.ActionState == GameElementSkins.ACTION_RUN)
						            {	
						            	GameCommonData.Player.Role.UsingPetAnimal.Stop();
						            	GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);
						            }
						            GameCommonData.Player.MustMovePoint = null;	
						            GameCommonData.Player.IsAddAttack = true;		            	
						        	UseSkill(GameCommonData.AttackAnimal);
						            			            
						            IsMove       = false; 
						            IsShowCursor = false;
						            this.ResetMoveState();
						            this.PetResetMoveState();
						  		 }
						  		 else
						  		 {
						  		 	IsMove       = true; 
						  		 	IsShowCursor = false;
						  		 	
									//不是可以走的点
									if(!this.gameScenePlay.Map.IsPass(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY))
									{
										CanMovePoint = AStarController.GetNearPoint(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),    //当前的点
									    new Point(GameCommonData.TargetAnimal.Role.TileX,GameCommonData.TargetAnimal.Role.TileY),true);  
															     
										//判断是否有路可以走
										if(CanMovePoint != null)
										{
											playerdistance = 0;
											nextPoint     = MapTileModel.GetTilePointToStage(CanMovePoint.x, CanMovePoint.y);
										    ClearAutoPath();
										    CloseNPCDialog();
										}
									}
									else
									{
						  		 		nextPoint    = new Point(GameCommonData.TargetAnimal.GameX, GameCommonData.TargetAnimal.GameY);
						  			}
						  		 }
				    	     }
				    	     else
				    	     {
				    	     		IsMove       = true; 
						            IsShowCursor = true;
						            GameCommonData.IsMoveTargetAnimal = false;
						            nextPoint = new Point(this.gameScenePlay.mouseX, this.gameScenePlay.mouseY);   
				    	     }
					   }
				       else
				       {    
				       	    //与NPC对话 距离设置为2   	    
				       		playerdistance = 1;
				       		if( DistanceController.PlayerTargetAnimalDistance(GameCommonData.TargetAnimal,playerdistance))
				       		{		     
				       			GameCommonData.Player.Stop();  
				       			if(GameCommonData.Player.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK  && GameCommonData.Player.Role.HP > 0)
				       				GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
				       			{
				       				if(GameCommonData.TargetAnimal.Role.Type == GameRole.TYPE_NPC)
				       				{
				       					UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(1,GameCommonData.TargetAnimal.Role.Id,autoPathData);	
				       				}				       				
				       				IsMove       = false; 
				       				IsShowCursor = false;	
				       			}	
				       			
				       			if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.Role.ActionState == GameElementSkins.ACTION_RUN)
					            {	
					            	GameCommonData.Player.Role.UsingPetAnimal.Stop();
					            	GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);
					            }	
					            
					            this.ResetMoveState();
					            this.PetResetMoveState();
             				}
             				else
             				{
                                CloseNPCDialog();
                                IsMove	     = true;
                                IsShowCursor = false;
             					nextPoint    = new Point(GameCommonData.TargetAnimal.GameX, GameCommonData.TargetAnimal.GameY);
             				}
				    	}
				    }
				}
				else
				{
					CloseNPCDialog();
				}	    
			}
			
			if(IsPlayerWalk() == false)
			{
		    	IsMove 		 = false;
		    	IsShowCursor = false;	
		    }
			
//			if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_NEAR_ATTACK)
//			{
//				
//				Zippo.PlayerAttackStop();
//				GameCommonData.Player.MustMovePoint = nextPoint;
//				GameCommonData.Player.Dis           = playerdistance;
//			}	
//			else
//			{
			    //显示鼠标的点
				if(IsShowCursor)
		        {        				
			 		this.SetCursor(nextPoint);
		        }	
			
				if(IsMove)
				{	
					ClearTaskAttack();
				    GameCommonData.TargetScene = "";  							// 不跨场景走路了
					UIFacade.GetInstance(UIFacade.FACADEKEY).changePath();
					CombatController.attackTime = 0;
					PlayerMove(nextPoint);
				}	
//			}			
		}
		
		/**判断自身是否可以行走**/
	    public function IsPlayerWalk():Boolean
	    {
	    	var IsWalk:Boolean = true;
	    	
    		//自身状态判断
			if(GameCommonData.Player.Role.State == GameRole.STATE_TRADE)		                    //交易中
			{
			    UIFacade.GetInstance(UIFacade.FACADEKEY).showNoMoveInfo(1);
                IsWalk = false;
             }
			else if(GameCommonData.Player.Role.State == GameRole.STATE_STALL)	                     //摆摊中 
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).showNoMoveInfo(2);  
			    IsWalk = false;
			}
			else if(GameCommonData.Player.Role.State == GameRole.STATE_LOOKINGNPCSHOP)               //打开NPC商店中	
			{
			    IsWalk = false; 
			}
			else if(GameCommonData.Player.Role.State != GameRole.STATE_NULL) 		                 //其他不可移动状态
			{
			    IsWalk = false;
			}		    
		
			if(GameCommonData.Player.Role.HP == 0 ||                                                //人物死亡或战斗
//		   	   GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_NEAR_ATTACK || 
		  	   GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_FAR_ATTACK)
		  	{
		  	   	IsWalk = false;
			}
			
			return IsWalk;
	    }	
		
		private var autoPathData:Object;
		
		public function MapPlayerTitleMove(nextPoint:Point,distance:int = 0,SceneName:String ="",data:Object=null):void
		{
			autoPathData = data;
			var stage:Point = MapTileModel.GetTilePointToStage(nextPoint.x,nextPoint.y);
			MapPlayerMove(stage,distance,SceneName);
		}
		
		/** 夸场景寻路  */
		public function MoveNextScenePoint():void
		{
			 CloseNPCDialog();
			 try
			 {
			     var finder:MapFinder  = new MapFinder(GameCommonData.MapTree);
			     var MapList:Array     = finder.Find(GameCommonData.GameInstance.GameScene.GetGameScene.name,GameCommonData.TargetScene); 
			     var TargetPoint:Point = new Point(GameCommonData.GameInstance.GameScene.GetGameScene.ConnectScene[MapList[0]].X,
			     								   GameCommonData.GameInstance.GameScene.GetGameScene.ConnectScene[MapList[0]].Y);     
			     playerdistance = 0;
			     PlayerMove(MapTileModel.GetTilePointToStage(TargetPoint.x,TargetPoint.y));
		     }
		     catch(e:Error)
		     {
//		     	  GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:"你已经是队长", color:0xffff00});
		     	  GameCommonData.UIFacadeIntance.showPrompt(GameCommonData.wordDic["con_sce_move_1"], 0xffff00);  // "当前地图找不到路径，无法自动寻路"
		     }
		}
				
				
		/**点击小地图的方法**/
		public function MapPlayerMove(nextPoint:Point,distance:int = 0,SceneName:String =""):void
		{		
			//切线状态不能移动
	       	if(ChgLineData.isChgLine)
			{
				return;
			}      
			
			if(IdentifyingCodeData.isShowView == true)
            {
            	trace("YanZhengMaData.isShowView");
				return;
            }
	
			GameCommonData.IsMoveTargetAnimal = false;
			
		    CloseNPCDialog();
			
			if(IsPlayerWalk())
			{	
	     		if((distance != 0 && SceneName =="") ||
	     		 GameCommonData.GameInstance.GameScene.GetGameScene.name == SceneName)
				{
					var p:Point = MapTileModel.GetTileStageToPoint(nextPoint.x,nextPoint.y);
					var targetDistance:int = MapTileModel.Distance(GameCommonData.Player.Role.TileX, 
																   GameCommonData.Player.Role.TileY,
											  					   p.x, 
											  					   p.y);
					
					//判断是怪物还是NPC						  					   
	                if(GameCommonData.autoPlayAnimalType != 0)
	                {           
	                	CombatController.attackTime = 0;		                	
	                	/** 判断类型动态改与目标判断的距离 */
						if(targetDistance <= 10)			// 动态参数据判断与目标的距离
						{
							var isattackTarget:Boolean = false;
							if(GameCommonData.isAutoPath)
							{
								if(GameCommonData.TargetScene == "" && GameCommonData.autoPlayAnimalType != 0)
								{
									if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_STATIC)
									{
										isattackTarget = TargetController.GetTaskTarget(GameCommonData.autoPlayAnimalType,nextPoint);
										GameCommonData.autoPlayAnimalType = 0;
									}
									else
									{
									    isattackTarget = TargetController.GetTaskTarget(GameCommonData.autoPlayAnimalType,nextPoint,false);
										GameCommonData.autoPlayAnimalType = 0;
									}
								}
								
								UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(1,0,autoPathData);						
								GameCommonData.isAutoPath = false;
							}		
							if(isattackTarget)		
							{					
								return;
							}
						}
	                }
	                else
	                {
	                	/** 判断类型动态改与目标判断的距离 */
						if(targetDistance <= distance)			// 动态参数据判断与目标的距离
						{
		                	if(GameCommonData.isAutoPath)
							{				
								if(GameCommonData.targetID == 2147483647) {	//皇陵副本中自动寻路到张泽端 需要对NPCID进行特殊处理，taskInfo中填2147483647，这里进行转换 2011.1.14 PM 冯
									for(var id:Object in GameCommonData.SameSecnePlayerList) {
										if(id >= 1 && id <= 299999) {
											GameCommonData.targetID = int(id);
											break;
										}
									}
								}
								UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(1,0,autoPathData);						
								GameCommonData.isAutoPath = false;
								return;
							}		
						}
	                }                
				}
				
				GameCommonData.TargetPoint = MapTileModel.GetTileStageToPoint(nextPoint.x,nextPoint.y); //保存A*的点
				GameCommonData.TargetDistance  = distance;
				GameCommonData.TargetScene = "";
				
				//判断是否点击的是NPC 是否要截取2格
			    playerdistance = distance;		
				
				if(SceneName.length > 0 && SceneName != GameCommonData.GameInstance.GameScene.GetGameScene.name)
				{
					 GameCommonData.TargetScene = SceneName;
	                 MoveNextScenePoint();
				}
				else
				{					
					//转换成A*的点	
					var ClickPoint:Point = MapTileModel.GetTileStageToPoint(nextPoint.x, nextPoint.y);		
					
					if(this.gameScenePlay.Map.IsPass(ClickPoint.x,ClickPoint.y))
					{
						//同一个点不显示地标
						if(!(GameCommonData.Player.Role.TileX == ClickPoint.x  && GameCommonData.Player.Role.TileY == ClickPoint.y))
						{
							this.SetCursor(nextPoint);
						}
						PlayerMove(nextPoint);
					}
					else
					{
						var CanMovePoint:Point = AStarController.GetNearPoint(new Point(GameCommonData.Player.Role.TileX,GameCommonData.Player.Role.TileY),    //当前的点
					    MapTileModel.GetTileStageToPoint(nextPoint.x, nextPoint.y));  
											     
						//判断是否有路可以走
						if(CanMovePoint != null)
						{
							nextPoint    = MapTileModel.GetTilePointToStage(CanMovePoint.x, CanMovePoint.y);
							this.SetCursor(nextPoint);
						    PlayerMove(nextPoint);
						}
					}
				}
			}
			else
			{
				GameCommonData.isAutoPath = false;
			}
		}

        public function ClearTaskAttack():void
        {
        	GameCommonData.autoPlayAnimalType = 0;
        	GameCommonData.AttackTargetID = 0;
        	GameCommonData.AttackTargetTime = 0;
        	GameCommonData.Player.IsAttack  = false;
        }  

		/**玩家移动**/
		public function PlayerMove(nextPoint:Point,isCheckPKScene:Boolean = true):void
		{	
			//判断是否需要提示
			if(isCheckPKScene)
				TransferSceneController.IsCheckPKScene = true;
			
			if(!GameCommonData.Player.IsAutomatism)
				//追加攻击取消
	            GameCommonData.Player.IsAddAttack = false;
	            
			//走路禁止缓冲
			this.gameScenePlay.IsUpdateNicety = false;
//			//取消攻击
//			Zippo.PlayerAttackStop();
			
			
			var ClickPoint:Point = MapTileModel.GetTileStageToPoint(nextPoint.x,nextPoint.y);	
			if(!this.gameScenePlay.Map.IsPass(ClickPoint.x,ClickPoint.y))
			{
				this.ResetMoveState();
				this.PetResetMoveState();
				if(GameCommonData.Player.Role.HP > 0)
					GameCommonData.Player.SetAction(GameElementSkins.ACTION_STATIC);
				return;
			}
			
			GameCommonData.TargetCommon = 0;
						
		    //判断是否点一样的点
			var targetPoint:Point = nextPoint.clone();	
			//必须要移动到的点
//			GameCommonData.MustPoint = nextPoint.clone();					
					
							
			//判断是否移动完成   或着 是静止状态
			if(GameCommonData.Player.PathDirection == null || GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_STATIC ||
			GameCommonData.Player.smoothMove.IsMoving  == false)
			{				
				this.ResetMoveState();
				this.PetResetMoveState();
				GameCommonData.Player.Move(targetPoint,playerdistance);
			}	
			else
			{   
				//如果是移动2步 或是完成                               
				if(GameCommonData.Player.Role.ActionState == GameElementSkins.ACTION_STATIC)					// 开始移动
	   		 	{	   		 		
					this.ResetMoveState();
					this.PetResetMoveState();
					GameCommonData.Player.Move(targetPoint,playerdistance);			
	   		 	}
	   		 	else
	   		 	{
	   		 		NextPoint = nextPoint;
	   		 	}
	    	}
		}

		/** 人物行走完后，触发地图平移 */
		public function onPlayMoveComplete():void
		{
			//看是否有预备的点 有预备的点必须走完
			if(this.NextPoint != null)
			{	
				//判断预备的点是否是同样的点
				var tag:Point = this.NextPoint.clone();
			    this.ResetMoveState();
			    this.PetResetMoveState();
				GameCommonData.Player.Move(tag,playerdistance);			
				return;
			}
			
			GameCommonData.AttackTargetTime = 0;
			
			//如果是同场景则取消自动寻路
			if(GameCommonData.TargetScene == GameCommonData.GameInstance.GameScene.GetGameScene.name)
			{
				GameCommonData.TargetScene = "";
			}
			
			//跨场景寻路结束 则打开NPC
			if(GameCommonData.isAutoPath && GameCommonData.TargetScene == "")
			{
				if(GameCommonData.targetID == 2147483647) {	//皇陵副本中自动寻路到张泽端 需要对NPCID进行特殊处理，taskInfo中填2147483647，这里进行转换 2011.1.14 PM 冯
					for(var id:Object in GameCommonData.SameSecnePlayerList) {
						if(id >= 1 && id <= 299999) {
							GameCommonData.targetID = int(id);
							break;
						}
					}
				}
				if(GameCommonData.TargetScene == "" && GameCommonData.autoPlayAnimalType != 0)
				{
					TargetController.GetTaskTarget(GameCommonData.autoPlayAnimalType,new Point(GameCommonData.Player.GameX,GameCommonData.Player.GameY));
					GameCommonData.autoPlayAnimalType = 0;
				}
				UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(1,0,autoPathData);			
				GameCommonData.isAutoPath = false;
			}
			
			DestinationCursor.Instance.hide();
			
			if(GameCommonData.AttackAnimal != null && PlayerController.IsUseSkill(GameCommonData.Player,GameCommonData.AttackAnimal)
			&& GameCommonData.IsMoveTargetAnimal == true)
			{
				//追击
				Attack(GameCommonData.AttackAnimal,GameCommonData.Player.Role.DefaultSkill);
			}
			else
			{
				this.ResetMoveState();
			}
			
			if(GameCommonData.TargetAnimal !=null &&  GameCommonData.TargetAnimal.Role.Type == GameRole.TYPE_NPC && GameCommonData.IsMoveTargetAnimal == true)
			{
				UIFacade.GetInstance(UIFacade.FACADEKEY).changeNpcWin(1,GameCommonData.TargetAnimal.Role.Id,autoPathData);
			}
				

			UIFacade.GetInstance(UIFacade.FACADEKEY).changePath();	
			//走完校对地图                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
			this.gameScenePlay.IsUpdateNicety = true;
			
			//挂机切换目标
			if(PlayerController.IsChangeAutomatism)
			{
				PlayerController.IsChangeAutomatism = false;
				this.Attack(GameCommonData.TargetAnimal);
			}
			
			//如果是挂机状态 走回攻击
			if(GameCommonData.Player.IsAutomatism)
			{
				if(GameCommonData.AutomatismPoint.x == GameCommonData.Player.Role.TileX 
				&& GameCommonData.AutomatismPoint.y == GameCommonData.Player.Role.TileY)				
				{
					PlayerController.Automatism();
				}
			}
		}
		
		public function onPetPlayMoveComplete():void
		{
			this.PetResetMoveState();
		}
		
		/**使用技能*/
		public function UseSkill(targetAnimal:GameElementAnimal):void
		{ 	
			//赋值 攻击目标
			GameCommonData.AttackAnimal = targetAnimal;
			
			//人物攻击 判断是否使用普通攻击
			if(GameCommonData.Player.Role.DefaultSkill == 0)
			{
				this.combat.ReserveAttack(targetAnimal);	   								
     		}
     		else
     		{
     		    var playerSkill:GameSkillLevel =  GameCommonData.Player.Role.SkillList[GameCommonData.Player.Role.DefaultSkill] as GameSkillLevel;
     		    if(playerSkill != null && targetAnimal != null &&  targetAnimal.Role.HP > 0)
     		    {
     				PlayerController.UsePlayerSkill(playerSkill,targetAnimal,null);   //使用技能
     		    }
     			GameCommonData.Player.Role.DefaultSkill  = 0;
     		}	 
				
			//宠物攻击
     		if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.Role.HP > 0
     		&& targetAnimal != null && targetAnimal.Role.HP > 0)
     		{ 
                //在9格之内打 9格之外飞   
                if(DistanceController.PetChangeTargetDistance(16))
                {    	
                	//宠物停止移动
                	PetResetMoveState();
                	if(GameCommonData.Player.Role.UsingPetAnimal.Role.ActionState != GameElementSkins.ACTION_STATIC
                     && GameCommonData.Player.Role.UsingPetAnimal.Role.ActionState != GameElementSkins.ACTION_NEAR_ATTACK)
                	{
						GameCommonData.Player.Role.UsingPetAnimal.SetAction(GameElementSkins.ACTION_STATIC);  	
                	}
					if(GameCommonData.Player.Role.UsingPetAnimal.Role.Id !=  targetAnimal.Role.Id)
					{
						//设置宠物攻击目标                  	
		     			GameCommonData.PetTargetAnimal = targetAnimal;
		     			this.combat.PetReserveAttack(targetAnimal);
	                }
                }
                else
                {
                	GameCommonData.Player.Role.UsingPetAnimal.MoveSeek();
                }
     		} 
			  				 
		}
				  
	    /** 重置移动状态参数  */
	    public function ResetMoveState():void
	    {
			GameCommonData.Player.Stop();
			this.NextPoint     = null;
	    	this.moveStepCount = 0;
	    }
	   
	    /** 重置宠物移动状态参数  */
	    public  function PetResetMoveState():void
	    {
	    	if(GameCommonData.Player.Role.UsingPetAnimal != null)
	    	{
				GameCommonData.Player.Role.UsingPetAnimal.Stop();
	    	    this.PetmoveStepCount = 0;
	    	} 	
	    }
	   
	    /**宠物飞行**/
		public function onMomentMove():void
		{
			var obj:Object = new Object();
			var parm:Array = new Array;
			parm.push(0);
			parm.push(GameCommonData.Player.Role.UsingPetAnimal.Role.Id);       //宠物编号
			parm.push(GameCommonData.Player.Role.TileX);                                           //目标点
			parm.push(GameCommonData.Player.Role.TileY);
			parm.push(GameCommonData.Player.Role.Direction);                                               //宠物的方向
			parm.push(0);                    
			parm.push(PlayerActionSend.PetMomentMove);							//飞行 19
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;                                                 
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
	    
	    /**人物移动**/ 
		public function onMoveNode(direction:int):Boolean
	    {   	   	
	    	var jumpTime:Date = new Date();
	    	
			// 每移动两格发一个通信信息
			if(this.moveStepCount % 2==0)
			{			
				//每走2步判断宝宝在不在身边
				if(GameCommonData.Player.Role.UsingPetAnimal != null)
				{				
					//判断宝宝是否超过了10格
					if(!DistanceController.PlayerPetDistance(7) &&  (jumpTime.time - GameCommonData.PetJump > 2000))
					{
						GameCommonData.Player.Role.UsingPetAnimal.DistanceMaster(GameCommonData.Player);
						GameCommonData.PetJump = jumpTime.time;
					}
				}
				
				//看是否有预备的点
				if(this.NextPoint != null)
				{	
					//判断预备的点是否是同样的点
//					if(!IsSamePoint(this.NextPoint))
//					{
						var tag:Point = this.NextPoint.clone();
					    this.ResetMoveState();
					    this.PetResetMoveState();
//						SetBeforeTargetPoint(tag);
						GameCommonData.Player.Stop();
						GameCommonData.Player.Move(tag,playerdistance);			
						return false;	
//					}
				}
				
				if(GameCommonData.Player.PathDirection != null                      //是否有要走的路
				&& this.IsSceneLoaded)                                              //是否加载完成
				{
					var a:int = GameCommonData.Player.PathDirection.shift();		// 第一步方向
					var b:int = 0;
					if(GameCommonData.Player.PathDirection.length != 0)
					{
						b = GameCommonData.Player.PathDirection.shift();			// 第二步方向
					}
					
					var point:Point = MapTileModel.GetNextPos(GameCommonData.Player.Role.TileX,		// 计算走一步后的坐标
															  GameCommonData.Player.Role.TileY,
															  a);
					var Fpoint:Point = point.clone();
					
					if(b > 0)// 计算走二步后的坐标
					{
						point	  = MapTileModel.GetNextPos(point.x, point.y, b);
					}
					
					var Spoint:Point = point.clone();
					
					if(!TransferSceneController.TransferSceneCheckPoint(Fpoint,Spoint))
					{
						return false;	
					}
//					trace("第一步:"+point.x,point.y);
//					trace("第二步:"+point.x,point.y);
//					
//					if(GameCommonData. ConfigXml)
					
					Zippo.PlayerWalk(GameCommonData.Player.Role.Id,
									 GameCommonData.Player.Role.TileX,
									 GameCommonData.Player.Role.TileY,
									 a,
									 b > 0?(b + 20):0                        // 数据没有第二步就发第二方向为"0"，否则把方向标记加"20"
									 );					 													
								 					
					GameCommonData.Player.Role.TileX = point.x;										// 预计移动一格或二格后的A*格坐标
					GameCommonData.Player.Role.TileY = point.y;

					
				}
			}
			
			this.moveStepCount++;
			
		    if(GameCommonData.Player.PathMap.length > 0)
			{
				GameCommonData.Player.PathMap.shift();
				UIFacade.GetInstance(UIFacade.FACADEKEY).changePath(2);
			}
			
			// 加载地图块
			this.gameScenePlay.Background.LoadMap();							
			return true;	
	   }
	    	    
		/** 每移动一个A*格事件  */	
		public function onPetMoveNode(direction:int):Boolean
		{	
			var jumpTime:Date = new Date();
					
			//判断宝宝是否超过了10格
			if(!DistanceController.PlayerPetDistance(7) && jumpTime.time - GameCommonData.PetJump > 2000)
			{
				GameCommonData.Player.Role.UsingPetAnimal.DistanceMaster(GameCommonData.Player);
				GameCommonData.PetJump = jumpTime.time;
				return false;
			}
				  			
			// 每移动两格发一个通信信息
			if(this.PetmoveStepCount % 2==0)
			{			
				if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.Player.Role.UsingPetAnimal.PathDirection != null)
				{
					var a:int = GameCommonData.Player.Role.UsingPetAnimal.PathDirection.shift();		// 第一步方向
					var b:int = 0;
					if(GameCommonData.Player.Role.UsingPetAnimal.PathDirection.length != 0)
					{						
						b = GameCommonData.Player.Role.UsingPetAnimal.PathDirection.shift();			// 第二步方向
					}
									
					var point:Point = MapTileModel.GetNextPos(GameCommonData.Player.Role.UsingPetAnimal.Role.TileX,		// 计算走一步后的坐标
															  GameCommonData.Player.Role.UsingPetAnimal.Role.TileY,
															  a);
					if(b > 0)												// 计算走二步后的坐标
					{
						point	  = MapTileModel.GetNextPos(point.x, point.y, b);
					}					
										
					Zippo.PlayerWalk(GameCommonData.Player.Role.UsingPetAnimal.Role.Id,
									 GameCommonData.Player.Role.UsingPetAnimal.Role.TileX,
									 GameCommonData.Player.Role.UsingPetAnimal.Role.TileY,
									 a,
									 b > 0?(b + 20):0                        // 数据没有第二步就发第二方向为"0"，否则把方向标记加"20"
									 );											
													
					GameCommonData.Player.Role.UsingPetAnimal.Role.TileX = point.x;										// 预计移动一格或二格后的A*格坐标
					GameCommonData.Player.Role.UsingPetAnimal.Role.TileY = point.y;
				}
			}
			this.PetmoveStepCount++;	
			
			return true;
		}
	
		/** 人物行走完一步后，触发地图平移 */
		public function onPlayMoveStep(e:GameElementAnimal):void
		{		
			this.SetScenePos();
			
			// 大地图X、Y坐标
			UIFacade.GetInstance(UIFacade.FACADEKEY).updateSmallMap({type:1,id:0});
			UIFacade.GetInstance(UIFacade.FACADEKEY).updateSmallMap({type:5,id:0});					
		}
			
		/** 设置人物在场景中心 */
		public function SetScenePos(IsNicety:Boolean = false):void
		{
            if(this.IsSceneLoaded)
            {
				var targetPoint:Point = this.gameScenePlay.SceneMove(GameCommonData.Player.GameX,GameCommonData.Player.GameY,IsNicety);
	
				// 技能释放范围特效
				if(GameCommonData.Rect != null)
				{
					GameCommonData.Rect.x = GameCommonData.Rect.x + this.gameScenePlay.x - int(targetPoint.x);
					GameCommonData.Rect.y = GameCommonData.Rect.y + this.gameScenePlay.y - int(targetPoint.y);
				}
				
				this.gameScenePlay.x  = int(targetPoint.x);
				this.gameScenePlay.y  = int(targetPoint.y);				
	
				UIFacade.UIFacadeInstance.updateSmallMap({type:6,id:0});
            }
		}
		
		//更新小地图
		public function updateSmallMap():void
		{
			UIFacade.UIFacadeInstance.updateSmallMap({type:1,id:0});
		}
				
		/** 设置玩家角色坐标  */
		public function SetPlayerPos(player:GameElementPlayer):void
		{
			var p:Point = MapTileModel.GetTilePointToStage(player.Role.TileX, player.Role.TileY);
			player.X 	= p.x;
			player.Y    = p.y;
		}
		
		/** 添加玩家到场景中 */
		public function AddPlayer(player:GameElementAnimal):void
		{				
			player.SetParentScene(this.gameScenePlay);						// 设置玩家所属场景，方便玩家A*寻路。
			if(player.Role.Face == 30000)									// 摊位
			{
				this.gameScenePlay.TopLayer.Elements.Add(player);
			}		
			else															// 玩家
			{
				UIFacade.UIFacadeInstance.updateSmallMap({type:3,id:player.Role.Id});
				if(player == GameCommonData.Player)							// 玩家自己不要选中特效
				{
					player.Skins.IsEffect(false);							
				}
				if(player.Role.HP == 0)										// 死亡状态
				{
                    if(player.Role.Type == GameRole.TYPE_OWNER || player.Role.Type == GameRole.TYPE_PLAYER)
                    {
                       // 上线死亡
				        if(player.Role.HP == 0)  
				        {
				        	player.Skins.ChangeAction(GameElementSkins.ACTION_DEAD,true);
				        	if(player.Role.Type == GameRole.TYPE_OWNER)
				        	{	
				        		player.Skins.InitActionDead(7);		
				        	}
				        	else if(player.Role.Type == GameRole.TYPE_PLAYER)
				        	{
				       			player.Skins.InitActionDead(7);				// 玩家死亡为8帧
				       		}
				       		else if(player.Role.Type == GameRole.TYPE_ENEMY)
				       		{
				       			player.Skins.InitActionDead(3);				// 敌人死亡为4帧
				       		}
				        }	
                    }
                    if(player.Role.Id == GameCommonData.Player.Role.Id)
                    {
                    	UIFacade.UIFacadeInstance.showRelive();					
                    }
//					this.gameScenePlay.BottomLayer.Elements.Add(player);
					player.Initialize();
					this.gameScenePlay.BottomLayer.addChild(player);
				}
				else
				{
					this.gameScenePlay.MiddleLayer.Elements.Add(player);
					if(player.Role.Type == GameRole.TYPE_PLAYER
					|| player.Role.Type == GameRole.TYPE_OWNER)
					{
						(player as GameElementPlayer).AddTernal();
					}
				}
				
				switch(player.Role.Type)
				{
					//如果是宠物
					case GameRole.TYPE_PET:
					  if(!(GameCommonData.Player.Role.UsingPetAnimal != null &&                           //判断是不是自己的宠物
						GameCommonData.Player.Role.UsingPetAnimal.Role.Id == player.Role.Id))		
					    {
					    	//判断场景中的人数是否超标
					    	if(this.ScenePlayerCount >= GameCommonData.SameSecnePlayerMaxCount)
					    	{
					    		player.Visible = false;
					    		player.Enabled = false;
					    	}		    	
					    }
					    else
					    {
					  	   UIFacade.UIFacadeInstance.sendNotification(EventList.ALLROLEINFO_UPDATE,{id:player.Role.Id,type:1001});
					    }
					    break;
				   case GameRole.TYPE_PLAYER:
				       if(this.ScenePlayerCount >= GameCommonData.SameSecnePlayerMaxCount)
					   {
					    	player.Visible = false;
					    	player.Enabled = false;
					    	if(player.Role.gameElementTernal != null)
							{
								player.Role.gameElementTernal.Visible = false;
							}
					   }
					   
					   //如果自己的宠物被先丢进去 那么人必须进去
					   if(player.Role.UsingPetAnimal != null && player.Role.UsingPetAnimal.Enabled)
					   {
					   		player.Visible = true;
			     	    	player.Enabled = true;
			     	    	if(player.Role.gameElementTernal != null)
							{
								player.Role.gameElementTernal.Visible = true;
							}
				       }
					   break;  
				}
							
				switch(player.Role.Type)
				{
					//如果是宠物
					case GameRole.TYPE_PET:
					if(GameCommonData.Screen == 0 || GameCommonData.Screen == 1)                            //屏宠物 屏所有
					    if(!(GameCommonData.Player.Role.UsingPetAnimal != null &&                           //判断是不是自己的宠物
						GameCommonData.Player.Role.UsingPetAnimal.Role.Id == player.Role.Id))			
						player.Visible = false;
					break;
					case GameRole.TYPE_PLAYER:
					   if(GameCommonData.Screen == 0)  
					   {
							player.Visible = false;
						    if(player.Role.gameElementTernal != null)
							{
								player.Role.gameElementTernal.Visible = false;
							}
					   }
					break;							
				}
						
				if(player.Role.isHidden)
			    {
					player.Visible = false;
				}
				
				if(player.Role.MountSkinName != null && player.Role.MountSkinName.length > 0)
				{
					PlayerController.SetPlayerSpeed(player,6);
				}
				
				//判断是否是宠物
				if(player.Role.Type == GameRole.TYPE_PET)
				{
					//判断宠物的主人是否有坐骑
					if(player.Role.MasterPlayer != null)
					{
					   if(player.Role.MasterPlayer.Role.MountSkinName != null)
					   {
					  	 player.SetMoveSpend(6);
					   }
					   player.Role.PKteam = player.Role.MasterPlayer.Role.PKteam ;
					   if(player.Role.PKteam != 0 && TargetController.IsPKTeam())
					   {
						   player.Role.NameColor  = PKController.GetPKTeamFontColor(player.Role.PKteam);
						   player.Role.NameBorderColor =  PKController.GetPKTeamBorderColor(player.Role.PKteam);
						   player.UpdatePersonName();
					  }
					  if(TargetController.IsPKTeam() == false)
					  {
					  	player.Role.PKteam = 0;
					  }
					}
					
					if(!(GameCommonData.Player.Role.UsingPetAnimal != null &&                           //判断是不是自己的宠物
			           GameCommonData.Player.Role.UsingPetAnimal.Role.Id == player.Role.Id))
			        {
			        	//如果不是自己的宠物 则不计算阻挡
			            var pet:GameElementPet = player as GameElementPet;
			            pet.PathFinder.Isbalk  = true;
			        }
				}
			}
			
			if(GameCommonData.Player.Role.UsingPetAnimal != null &&                           //判断是不是自己的宠物
			GameCommonData.Player.Role.UsingPetAnimal.Role.Id == player.Role.Id )
			{
			 	player.MoveComplete         = onPetPlayMoveComplete;                         
			 	GameCommonData.Player.Role.UsingPetAnimal.MomentMove = onMomentMove;          
				player.MoveNode 		    = onPetMoveNode;
				player.PetDistance          = PetController.PetDistance;
				player.CheckPoint           = PetController.CheckPetPoint;
				player.smoothMove.CheckPoint = PetController.CheckPetPoint;
				PetResetMoveState();				
				PetController.BuffUseSkill();
							
				//宠物瞬移则需要继续移动
				GameCommonData.Player.Role.UsingPetAnimal.MoveSeek();
			}
			
			//增加 玩家和宠物
			if((player.Role.Type == GameRole.TYPE_PET ||  player.Role.Type == GameRole.TYPE_PLAYER)
			&& player.Enabled) //是否显示                                   
			{
				this.ScenePlayerCount += 1;	
			}		
					
			//添加的是怪物 并且是否在挂机 用于周围怪都没了 有多出1个怪 立即开始打
			if(player.Role.Type == GameRole.TYPE_ENEMY && GameCommonData.Player.IsAutomatism && GameCommonData.TargetAnimal == null
			&& TargetController.DeadAnimal[player.Role.Id] == null )
			{
				 PlayerController.Automatism();
			}		 
		}
			
		/** 从场景中删除玩家 宠物 怪物 */
		public function DeletePlayer(key:Object,enemy:Boolean = false):void
		{
			if(GameCommonData.SameSecnePlayerList != null && GameCommonData.SameSecnePlayerList[key]!=null)
			{
				var deleteplayer:GameElementAnimal = GameCommonData.SameSecnePlayerList[key];		
				
				if(deleteplayer.Role.Type == GameRole.TYPE_PLAYER)	
				{
					(deleteplayer as GameElementPlayer).RemoveTernal();
				}
				
				if(deleteplayer.Role.Type != GameRole.TYPE_ENEMY  || enemy || deleteplayer.Role.HP > 0)
				{
					//减数值 玩家和宠物
					if((deleteplayer.Role.Type == GameRole.TYPE_PET || deleteplayer.Role.Type == GameRole.TYPE_PLAYER)
					&& deleteplayer.Enabled) //是否显示
					{
						this.ScenePlayerCount -= 1;	
					}
					
					//是否增加了人
					var IsAddplayer:Boolean = false;
					
					//判断场景中显示的人数是否超过了最大数 减少1个增加1个 
					if(this.ScenePlayerCount <= GameCommonData.SameSecnePlayerMaxCount)
					{
						//首先找人
						 for(var playerName:String in GameCommonData.SameSecnePlayerList)
						{
						    var Addplayer:GameElementAnimal= GameCommonData.SameSecnePlayerList[playerName];
							if(Addplayer.Role.Type == GameRole.TYPE_PLAYER && Addplayer.Enabled == false)
							{	
								 Addplayer.Enabled = true;  			 
								 Addplayer.Visible = true;
								 IsAddplayer       = true; 
								  						 
						         if(GameCommonData.Screen == 0)  
							        Addplayer.Visible = false;			
							        
							     this.ScenePlayerCount += 1;	            
						         break;									 
							}
						}			
						
						//人没有找宠物
						if(IsAddplayer == false)
						{
							 for(var petName:String in GameCommonData.SameSecnePlayerList)
							{
							    var Addpet:GameElementAnimal= GameCommonData.SameSecnePlayerList[petName];
							    //他的主人一定要显示出来才可以
								if(Addpet.Role.Type == GameRole.TYPE_PET
								 && Addpet.Role.MasterPlayer != null && Addpet.Role.MasterPlayer.Enabled
								&&  Addpet.Enabled == false)
								{	
									 Addpet.Enabled = true;  			 
									 Addpet.Visible = true; 
	
						             if(GameCommonData.Screen == 0 || GameCommonData.Screen == 1)                            //屏宠物 屏所有
						    		 if(!(GameCommonData.Player.Role.UsingPetAnimal != null &&                           //判断是不是自己的宠物
							 		 GameCommonData.Player.Role.UsingPetAnimal.Role.Id == Addpet.Role.Id))			
											Addpet.Visible = false;		
											
									 this.ScenePlayerCount += 1;									
									 break;								 
								}
							}		
						}
					}
					
								
					if(GameCommonData.SameSecnePlayerList[key] == GameCommonData.PetTargetAnimal)
					{
						if(GameCommonData.Player.Role.UsingPetAnimal != null)
						{
							GameCommonData.PetTargetAnimal  = null;
							GameCommonData.Player.Role.UsingPetAnimal.MoveSeek();
						}			
					}
					
					if(GameCommonData.SameSecnePlayerList[key] == GameCommonData.TargetAnimal)
					{
						GameCommonData.TargetAnimal = null;
						UIFacade.GetInstance(UIFacade.FACADEKEY).upDateInfo(31);			
					}
					
					if(GameCommonData.SameSecnePlayerList[key] == GameCommonData.AttackAnimal)
					{
						GameCommonData.AttackAnimal = null;			
					}
					
					
					//删除宠物
					if(GameCommonData.Player.Role.UsingPetAnimal != null && GameCommonData.SameSecnePlayerList[key] == GameCommonData.Player.Role.UsingPetAnimal)
					{
						GameCommonData.PetTargetAnimal = null;
						GameCommonData.Player.Role.UsingPetAnimal = null;
						UIFacade.GetInstance(UIFacade.FACADEKEY).upDateInfo(41,uint(key));
					}			
					
					// 小地图上删除人物 开始
					UIFacade.UIFacadeInstance.updateSmallMap({type:4,id:key});
					// 小地图上删除人物 结果
					
					if( GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer != null &&
					GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.contains(GameCommonData.SameSecnePlayerList[key]))			// 玩家死亡状态删除
					{
						GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.removeChild(GameCommonData.SameSecnePlayerList[key]);
					}
					else if( GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer != null &&
					GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.contains(GameCommonData.SameSecnePlayerList[key]))			// 玩家摆滩功能图标删除
					{
						GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.Elements.Remove(GameCommonData.SameSecnePlayerList[key]);
					}
					else																															// 玩家其它删除
					{
						if(GameCommonData.GameInstance.GameScene.GetGameScene.MiddleLayer != null)
						{
							GameCommonData.GameInstance.GameScene.GetGameScene.MiddleLayer.Elements.Remove(GameCommonData.SameSecnePlayerList[key]);
					    }
					}
//					GameCommonData.SameSecnePlayerList[key].Dispose();
					delete GameCommonData.SameSecnePlayerList[key];
				}
				else
				{
					DeferDeletePlayer(key,true);
//					//延迟删除
//                	setTimeout(DeferDeletePlayer,1000,key,true);
				}
			}
		}
		
		public function DeferDeletePlayer(key:Object,enemy:Boolean = false):void
		{
			if(GameCommonData.SameSecnePlayerList != null && GameCommonData.SameSecnePlayerList[key]!=null)
			{
				var deleteplayer:GameElementAnimal = GameCommonData.SameSecnePlayerList[key];
				
				if(deleteplayer.Role.Type == GameRole.TYPE_ENEMY  && deleteplayer.Role.HP == 0)
				{
					if(deleteplayer.Role.ActionState != GameElementSkins.ACTION_DEAD)
					{
						deleteplayer.IsMustDel = true;
						deleteplayer.SetAction(GameElementSkins.ACTION_DEAD);
					}
					else
					{
					    DeletePlayer(key,true);	
					}
				}
			}
		}

		/** 删除场景上的包 */
		public function DeletePackage(key:Object):void
		{
			if(GameCommonData.PackageList[key]!=null)
			{
				if(GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.contains(GameCommonData.PackageList[key])) {
					GameCommonData.GameInstance.GameScene.GetGameScene.BottomLayer.removeChild(GameCommonData.PackageList[key]);
					UIFacade.GetInstance(UIFacade.FACADEKEY).DeletePackage(key as uint);
				}
				delete GameCommonData.PackageList[key];
			}
		}

		/** 鼠标点击目标点图标 */
		public function SetCursor(cursorPoint:Point):void
		{
//			this.gameScenePlay.mouseX, this.gameScenePlay.mouseY
			if(GameCommonData.IsMoveTargetAnimal == false)
			{
				// TODO : 显示目标提示点 开始
				DestinationCursor.Instance.hide();
				var p:Point = MapTileModel.GetTileStageToPoint(cursorPoint.x,cursorPoint.y);
				var p1:Point = MapTileModel.GetTilePointToStage(p.x,p.y);
								
//				var p:Point = MapTileModel.GetTileStageToPoint(cursorPoint.x,cursorPoint.y);
////				if(this.gameScenePlay.Map.IsPass(p.x,p.y))3
////				{
//				p = MapTileModel.GetTilePointToStage(p.x, p.y);
				DestinationCursor.Instance.show();
				DestinationCursor.Instance.desIcon.x = p1.x - 24;
				DestinationCursor.Instance.desIcon.y = p1.y - 17;
//				}
			}
		}
	}
}