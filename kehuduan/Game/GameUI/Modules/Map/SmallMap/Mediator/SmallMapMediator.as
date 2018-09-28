package GameUI.Modules.Map.SmallMap.Mediator
{
	import Controller.PKController;
	import Controller.PlayerController;
	import Controller.TargetController;
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Help.Data.DataEvent;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Login.SoundUntil.SoundController;
	import GameUI.Modules.MainSence.Mediator.MainSenceMediator;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.TimeCountDown.TimeUIUtils;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.Components.UISprite;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SmallMapMediator extends Mediator
	{
		public static const NAME:String = "SmallMapMediator";
		
		private var showMap:Boolean = true;
		
		private var date:Date = null;
		private var map:Bitmap;
		private var mapContainer:Sprite;
	
		
		private var playerIcon:MovieClip;
		private var animalDic:Dictionary=new Dictionary();
		private var dataProxy:DataProxy;
		private var mapDic:Dictionary = new Dictionary();
		private var pathSprite:UISprite;
		private var flag:Boolean=false;
		
		public var soundController:SoundController;
		private var timeUtils:TimeUIUtils;  //显示服务器时间组件
		
		
		private const direction:Array=[0,-27,-90,-153,0,0,180,27,90,153];
		private var doFirst:Boolean=false;
		private var teamDic:Dictionary=new Dictionary();
		
		public static var isFullScreen:Boolean = true;  
		
		
		public function SmallMapMediator()
		{
			super(NAME, viewComponent);
		}
		
		public function get SmallMap():MovieClip 
		{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.ENTERMAPCOMPLETE,
				EventList.UPDATE_SMALLMAP_DATA,
				PlayerInfoComList.UPDATE_TEAM,
				EventList.UPDATE_SMALLMAP_PATH,
				EventList.SHOW_SMALL_MAP,
				EventList.SHOW_TIME
			];
		} 
	
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SMALLMAP});
					this.SmallMap.mouseEnabled=false;			
					if ( isFullScreen )
					{
						soundController = new SoundController(GameConfigData.UILibrary , new Point(830,167) , true);  //(830,167)	
					}		
					else
					{
						soundController = new SoundController(GameConfigData.UILibrary , new Point(860,167) , true);  //(830,167)					//创建声音控制
					}
					this.SmallMap.addEventListener(Event.ADDED_TO_STAGE, initResize);
				break;
				case EventList.ENTERMAPCOMPLETE:
					GameCommonData.GameInstance.GameUI.addChild(SmallMap);
					dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					addLis();
					initView();
				break;
				
				case PlayerInfoComList.UPDATE_TEAM:
					this.changeTeam();
				break;
				
				case EventList.UPDATE_SMALLMAP_DATA:
					var obj:Object=notification.getBody();
					var type:uint=obj.type;    
					var id:uint=obj.id;
					switch (type){
						case 1:
							this.changeMapPos();
							break;
						case 2:
							this.updateAnimalPos(id);
							break;
						case 3:
							this.addAnimal(id);
							break;
						case 4:
							this.removeAnimal(id);
							break;
						case 5:
							this.changePlayerPos();
							break;
						case 6:
							this.changePlayerDirection();
							break;
						case 7:
							this.changeSence();
							sendNotification(EventList.CHANGE_SCENEMAP_EVENT);
							
							break;	
					}
				break;
				
				case EventList.UPDATE_SMALLMAP_PATH:
					var pathType:uint=uint(notification.getBody());
					if(pathType==0 || pathType==1){
						this.flag=false;
						if(this.pathSprite!=null){
							pathSprite.graphics.clear();
						}
					}else if(pathType==2){
						if(this.flag){
							this.drawPath();
						}
					}
					break;
				case EventList.SHOW_SMALL_MAP:			//外部调用的 显示小地图
					if(!showMap) onChangeMap(null);
					break;
				case EventList.SHOW_TIME:				//显示服务器时间
					timeUtils = TimeUIUtils.getInstance(SmallMap.txtCurTime);
					timeShow(notification.getBody() as Array);
					break;
			}
		}
		
		private function initView():void
		{
			SmallMap.btnExtend.visible = !showMap;
			SmallMap.btnReduce.visible = showMap;
			
			(this.SmallMap.mcSmallElment.btn_rank as SimpleButton).addEventListener(MouseEvent.CLICK,onClickRanKHandler);
			(this.SmallMap.mcSmallElment.autoRoad as SimpleButton).addEventListener(MouseEvent.CLICK,onAutoRoadHandler);
			(this.SmallMap.mcSmallElment.btn_sceneMap as SimpleButton).addEventListener(MouseEvent.CLICK,onSceneMapClickHandler);
//			(this.SmallMap.mcSmallElment.btn_bigMap as SimpleButton).addEventListener(MouseEvent.CLICK,onBigMapClickHanler);
//			(this.SmallMap.mcSmallElment.btn_help as SimpleButton).addEventListener(MouseEvent.CLICK,onHelpHandler);
			(this.SmallMap.mcSmallElment.btn_autoPlay as SimpleButton).addEventListener(MouseEvent.CLICK,onAutoPalyClickHandler);
//			(this.SmallMap.mcSmallElment.btn_pk as SimpleButton).addEventListener(MouseEvent.CLICK,onPkClickHandler);
			(this.SmallMap.mcSmallElment.btn_gm as SimpleButton).addEventListener(MouseEvent.CLICK,onGmClickHandler);
			
			this.SmallMap.mcSmallElment.fullScreen.visible = false; 
//			this.SmallMap.mcSmallElment.fullScreen.addEventListener(MouseEvent.CLICK,onFullScreenHandler); //全屏按钮
//			(this.SmallMap.mcSmallElment.fullScreen as MovieClip).buttonMode = true;
//			(this.SmallMap.mcSmallElment.fullScreen as MovieClip).gotoAndStop( 1 );
			
			this.SmallMap.mcSmallElment.btnRight_5.addEventListener(MouseEvent.CLICK, clickBtnRightHandler);
			this.SmallMap.mcSmallElment.btnRight_4.addEventListener(MouseEvent.CLICK, clickBtnRightHandler);
			this.SmallMap.mcSmallElment.btnRight_2.addEventListener(MouseEvent.CLICK, clickBtnRightHandler);
			this.SmallMap.mcSmallElment.btnRight_0.addEventListener(MouseEvent.CLICK, clickBtnRightHandler);
			this.SmallMap.mcSmallElment.btnRight_8.addEventListener(MouseEvent.CLICK, clickBtnRightHandler);
			
			this.playerIcon=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SelfPos");
			
			//打开全屏
			if ( isFullScreen)
			{
				openFullScreen();
			}
			
			if (GameCommonData.isExe)
			{
				this.SmallMap.mcSmallElment.fullScreen.visible = false;
			}
		}
		
		private function openFullScreen():void
		{
			this.SmallMap.mcSmallElment.fullScreen.visible = true;
			this.SmallMap.mcSmallElment.fullScreen.addEventListener(MouseEvent.CLICK,onFullScreenHandler); //全屏按钮
			(this.SmallMap.mcSmallElment.fullScreen as MovieClip).buttonMode = true;
			(this.SmallMap.mcSmallElment.fullScreen as MovieClip).gotoAndStop( 1 );
		}
		
		/** 右边按钮更改  */
		private function clickBtnRightHandler(e:MouseEvent):void
		{
			var type:uint = (e.currentTarget.name as String).split("_")[1];
			var mainSenceMediator:MainSenceMediator = facade.retrieveMediator( MainSenceMediator.NAME ) as MainSenceMediator;
			mainSenceMediator.useRightQuickBtn(type);
		}
		
		/**
		 * 打开PK开关 
		 * @param e
		 * 
		 */		
		protected function onPkClickHandler(e:MouseEvent):void{
			sendNotification(EventList.SHOWPKVIEW);
			e.stopPropagation();				
		}
		
		/**
		 * 打开场景地图 
		 * @param e
		 * 
		 */		
		protected function onSceneMapClickHandler(e:MouseEvent):void{
			if(this.dataProxy.SenceMapIsOpen){
				this.sendNotification(EventList.CLOSESCENEMAP);
			}else{
				this.sendNotification(EventList.SHOWSENCEMAP);
			}
		}
		/**
		 * 打开世界地图 
		 * @param e
		 * 
		 */		
		protected function onBigMapClickHanler(e:MouseEvent):void{
			sendNotification(EventList.SHOWBIGMAP);
		}
		/**
		 * 打开帮助文档 
		 * @param e
		 * 
		 */		
		protected function onHelpHandler(e:MouseEvent):void{
			facade.sendNotification(DataEvent.OUTSHOWPK);
		}
		
		/**
		 * 自动挂机 
		 * @param e
		 * 
		 */		
		protected function onAutoPalyClickHandler(e:MouseEvent):void{
			if(this.dataProxy.autoPlayIsOpen){
				sendNotification(AutoPlayEventList.HIDE_AUTOPLAY_UI);
				sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
			}else{
				sendNotification(AutoPlayEventList.SHOW_AUTOPLAY_UI);
			}
		}
		/***
		 * 打开GM
		 * 
		 * 
		 */
		protected function onGmClickHandler(e:MouseEvent):void{
			facade.sendNotification(TerraceController.NAME , "GMTools");
		}
		
		/** 全屏 */
		protected function onFullScreenHandler(e:MouseEvent):void{
			
//			facade.sendNotification(HintEvents.RECEIVEINFO, {info:"暂未开放", color:0xffff00});	
			if( GameCommonData.fullScreen == 1 )
			{
				GameCommonData.fullScreen = 2;
				UIUtils.callJava( "intofullscreen" );
				(this.SmallMap.mcSmallElment.fullScreen as MovieClip).gotoAndStop( 2 );
			}else{
				GameCommonData.fullScreen = 1;
				UIUtils.callJava( "exitfullscreen" );
				(this.SmallMap.mcSmallElment.fullScreen as MovieClip).gotoAndStop( 1 );
			}
			if( GameCommonData.Scene.IsSceneLoaded )
			{
				GameCommonData.Scene.gameScenePlay.Background.LoadMap();
			}
		}
		
		protected function onClickRanKHandler(e:MouseEvent):void{
			if( GameCommonData.Player.Role.Level >= 20 )
			{
				if( this.dataProxy.RankIsOpen){
					sendNotification(EventList.CLOSERANKVIEW);
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
				}else{
					sendNotification(EventList.SHOWONLY_CENTER_FIVE_PANEL, "rank");
					sendNotification(EventList.SHOWRANKVIEW);
				}
			}
			else
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_Map_SmallMap_Mediator_SmallMapMediator_1" ]/** = "达到20级才能看到排行榜";*/, color:0xffff00});	
			}
		}
		
		private function onAutoRoadHandler(e:MouseEvent):void{
			if(!dataProxy.AutoRoadIsOpen){
				sendNotification(EventList.SHOW_AUTOPATH_UI);
			}else{
				sendNotification(EventList.HIDE_AUTOPATH_UI);
				sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
			}
		}
		
		/**
		 * 分组改变时，更新小地图的显示 
		 * 
		 */		
		private function changeTeam():void{

			//队伍列表
			for(var id:* in this.teamDic){
				//在同场景中
				if(GameCommonData.SameSecnePlayerList[id]!=null){
					var point:MovieClip=this.teamDic[id];
					if(point!=null && this.mapContainer.contains(point)){
						this.mapContainer.removeChild(point);
						delete this.teamDic[id];
					}
					this.addAnimal(id);
				}	
			}

			for(var teamId:* in GameCommonData.TeamPlayerList){
				if(GameCommonData.SameSecnePlayerList[teamId]!=null){
					if(this.teamDic[teamId]==null && this.animalDic[teamId]!=null){
						var p:MovieClip=this.teamDic[teamId];
						if(p!=null && this.mapContainer.contains(p)){
							this.mapContainer.removeChild(p);
						}
						this.addAnimal(teamId);
					}
				}
			}
		}
		
		// 1:
		private function changeMapPos():void{
			var bigMapX:Number=GameCommonData.GameInstance.GameScene.GetGameScene.x;
			var bigMapY:Number=GameCommonData.GameInstance.GameScene.GetGameScene.y;
			var scale:Number=GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
			var h:Number=GameCommonData.GameInstance.GameScene.GetGameScene.MapHeight;
			var w:Number=GameCommonData.GameInstance.GameScene.GetGameScene.MapWidth;
			var mul:Number=(1/scale);	
			mapContainer.x=bigMapX*scale+SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name][0];
			mapContainer.y=bigMapY*scale+SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name][1];
			if(mapContainer.x>0){
				mapContainer.x=0;
			}else if((mapContainer.x-135-SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name][2])*mul<-w){
				mapContainer.x=Math.floor(-w*scale+135+SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name][4]);
			}
			if(mapContainer.y>0){
				mapContainer.y=0;
			}else if((mapContainer.y-140-SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name][3])*mul<=-h){
				mapContainer.y=Math.floor(-h*scale+140+SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name][3]);
			}
			
			SmallMap.txtCurPoint.text=GameCommonData.Player.Role.TileX+","+GameCommonData.Player.Role.TileY;
		}
		
		 
		/**
		 * 改变场景玩家位置(包括改变他的队友信息)  
		 * 
		 */		
		 //2:
		private function updateAnimalPos(id:uint):void{
			if(GameCommonData.SameSecnePlayerList!=null)
			{
				var animal:GameElementAnimal=GameCommonData.SameSecnePlayerList[id] as GameElementAnimal;
				var point:MovieClip=this.animalDic[id] as MovieClip;
//				if(animal!=null && point==null && animal.Role.Type==GameRole.TYPE_PLAYER){
//					if(GameCommonData.Player.Role.idTeam!=0 && GameCommonData.Player.Role.idTeam==animal.Role.idTeam){
//						if(this.teamDic[id]!=null)return;
//						
//						point=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BulePoint");
//						this.mapContainer.addChild(point);
//						this.animalDic[id]=point;
//						this.teamDic[id]=point;
//						point.name="SMALLMAP_"+animal.Role.Name;
//					}
//				}
			}
			if(animal==null || point==null)return;
			if(animal.Role.isHidden)return;
			
			var p:Point = MapTileModel.GetTilePointToStage(animal.Role.TileX,animal.Role.TileY);
			point.x=p.x*GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
			point.y=p.y*GameCommonData.GameInstance.GameScene.GetGameScene.Scale;

		}
		
		/**
		 * 添加玩家  
		 * 
		 */
		private function addAnimal(id:uint):void{
			var obj:Object = new Object();
			obj.id = id;
			obj.delayID = setInterval(showAnimal, 20, obj);			
		}
		
		private function showAnimal(obj:Object):void
		{
			if(!this.mapContainer) return;
			clearInterval(obj.delayID);
			if(this.animalDic[obj.id]!=null){
				this.removeAnimal(obj.id);
			}
			
			// 2010-09-07 董刚修改
			if(GameCommonData.SameSecnePlayerList==null)return;
			
			var animal:GameElementAnimal=GameCommonData.SameSecnePlayerList[obj.id] as GameElementAnimal;
			if(animal==null)return;
			if(animal.Role.isHidden)return;
			var point:MovieClip;
			if(animal.Role.Type==GameRole.TYPE_PLAYER){
				//进入pk场景
				if(TargetController.IsPKTeam())	{
					point = new MovieClip();
					point.graphics.beginFill(uint(PKController.getPKPersonColor(animal.Role.PKteam)));
					point.graphics.drawCircle(3,3,3);
					point.graphics.endFill();
				} 
				else{
					if(animal.Role.idTeam==0){
						point=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GreenPoint");   //非队友
					}else if(GameCommonData.Player.Role.idTeam==animal.Role.idTeam){
						point=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BulePoint");   //队友
						this.teamDic[obj.id]=point;
					}else{
						point=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GreenPoint");   //非队友
					}
				}
			}else if(animal.Role.Type==GameRole.TYPE_ENEMY){
				point=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedPoint");
			}else if(animal.Role.Type==GameRole.TYPE_NPC){
				point=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowPoint");
			}
			if(point==null)return;
			this.animalDic[obj.id]=point;
			point.name="SMALLMAP_"+animal.Role.Name;
			this.mapContainer.addChild(point);
			
			var p:Point = MapTileModel.GetTilePointToStage(animal.Role.TileX,animal.Role.TileY);
			point.x=p.x*GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
			point.y=p.y*GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
		}
		/**
		 *删除玩家 
		 * 
		 */	
		 //4	
		private function removeAnimal(id:uint):void{
			var mc:MovieClip=this.animalDic[id] as MovieClip;
			if(mc!=null && this.mapContainer.contains(mc)){
				delete this.animalDic[id];
				delete this.teamDic[id];
				this.mapContainer.removeChild(mc);
			}
		}
		
		
		/**
		 * 改变玩家自己的坐标
		 * 
		 */	
		 //5	
		private function changePlayerPos():void{
			
			this.mapContainer.addChild(this.playerIcon);
			this.playerIcon.x=GameCommonData.Player.GameX*GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
			this.playerIcon.y=GameCommonData.Player.GameY*GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
			var arr:Array=SmallConstData.getInstance().smallMapDic[GameCommonData.GameInstance.GameScene.GetGameScene.name] as Array;
			if(arr[6]==true){
				if(this.playerIcon.x>arr[7]){
					this.playerIcon.x=arr[7];
				}
			}
			if(arr[8]==true){
				
				if(this.playerIcon.x>arr[9]){
					this.playerIcon.x=arr[9];
				}
			}
			
		}
		
		/**
		 *改变玩家自己的方向 
		 * 
		 */	
		 //6	
		private function changePlayerDirection():void{
			if(this.playerIcon==null)return;
			this.playerIcon.rotation=this.direction[GameCommonData.Player.Role.Direction];
		}
		
		
		
		private function changeSence():void{
			this.animalDic=new Dictionary();
			if(this.mapContainer){
				while(mapContainer.numChildren>0){
					mapContainer.removeChildAt(0);
				}
				mapContainer.removeEventListener(MouseEvent.CLICK,onSmallClickHandler);
				SmallMap.mcSmallElment.removeChild(mapContainer);
				mapContainer = null;
			}			
			this.playerIcon=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SelfPos");
			this.map=GameCommonData.GameInstance.GameScene.GetGameScene.RealSmallMap;
			this.setMapPos();
			
		}
		/** 显示服务器的时间 */
		private function timeShow(timeList:Array):void
		{
			timeUtils.showText(timeList);
		}
		
		//显示客户端时间
//		private function onTimer(event:TimerEvent):void
//		{
//			date = new Date();
//			SmallMap.txtCurTime.text = date.getHours() + ":" + (date.getMinutes()<10?"0"+date.getMinutes():date.getMinutes());
//		}
		
		//添加事件
		private function addLis():void
		{
			SmallMap.btnExtend.addEventListener(MouseEvent.CLICK, onChangeMap);
			SmallMap.btnReduce.addEventListener(MouseEvent.CLICK, onChangeMap);
			SmallMap.mcSmallElment.huodong.addEventListener(MouseEvent.CLICK,onHuoDongHandler);
//			timer.addEventListener(TimerEvent.TIMER, onTimer);
		}
		
		private function onHuoDongHandler(e:MouseEvent):void
		{
//			if(dataProxy.MarketIsOpen) {
//				sendNotification(EventList.CLOSEMARKETVIEW);
//				sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
//			} else {
//				sendNotification(EventList.SHOWMARKETVIEW);
//			}
			if( GameCommonData.Player.Role.Level >= 15 )
			{
				facade.sendNotification(CampaignData.INIT_CAMPAIGN);
			}
			else
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_Map_SmallMap_Mediator_SmallMapMediator_2" ]/** = "达到15级才能查看活动列表";*/, color:0xffff00});	
			}	
		}
		
		
		/**
		 * 隐藏与显示小地图 
		 * @param event
		 * 
		 */		
		private function onChangeMap(event:MouseEvent):void
		{
			
			showMap = !showMap;
			SmallMap.btnExtend.visible = !showMap;
			SmallMap.btnReduce.visible = showMap
			SmallMap.mcSmallElment.visible = showMap;
			SmallMap.txtCurPoint.visible=showMap;
			this.soundController.isableSee(showMap);			//隐藏声音开关
		}
		
		//点击小地图寻路
		protected function onSmallClickHandler(e:MouseEvent):void{
			if(!GameCommonData.Scene.IsFirstLoad)return;
			if(GameCommonData.Player.IsAutomatism)return;
			GameCommonData.isAutoPath=false;
			GameCommonData.IsMoveTargetAnimal = false;
			this.flag=true;
			GameCommonData.Scene.MapPlayerMove(new Point(this.mapContainer.mouseX/GameCommonData.GameInstance.GameScene.GetGameScene.Scale,this.mapContainer.mouseY/GameCommonData.GameInstance.GameScene.GetGameScene.Scale));	
			GameCommonData.Player.MapPathUpdate = drawPath;
			PlayerController.EndAutomatism();
		}
		
		protected function drawPath():void{
			if(flag)
			{
				var path:Array=GameCommonData.Player.PathMap;	
				this.mapContainer.addChild(this.pathSprite);
				if(path==null){
					return;
				}
				this.pathSprite.graphics.clear();
				this.pathSprite.graphics.beginFill(0xff0000,1);
				var scale:Number=GameCommonData.GameInstance.GameScene.GetGameScene.Scale;
				var len:uint=path.length;
				for(var i:uint=0;i<len;i++){
					pathSprite.graphics.drawCircle(path[i].x*scale,path[i].y*scale,1);
				}
				this.pathSprite.graphics.endFill();
			}
		}
		
		
		private function setMapPos():void
		{
			mapContainer=new Sprite();
			mapContainer.addChild(map);
			
			SmallMap.txtMapName.text=GameCommonData.GameInstance.GameScene.GetGameScene.MapName;
			MasterData.addGlowFilter(SmallMap.txtMapName);
			mapContainer.addEventListener(MouseEvent.CLICK,onSmallClickHandler);
			mapContainer.addChild(this.playerIcon);
			this.changePlayerPos();
			this.changeMapPos();
			this.changePlayerDirection();
			
			pathSprite=new UISprite();
			pathSprite.width=map.width;
			pathSprite.height=map.height;
			(SmallMap.mcSmallElment as MovieClip).mouseEnabled=false;
			mapContainer.mask = SmallMap.mcSmallElment.mcMapMask;
			SmallMap.mcSmallElment.addChild(mapContainer);
			var pos:uint=SmallConstData.getInstance().mapItemDic[GameCommonData.GameInstance.GameScene.GetGameScene.name].id
			
			/////第一次进入蝴蝶谷，通知新手指导系统
			if(NewerHelpData.newerHelpIsOpen) {
				var mask:uint=GameCommonData.BigMapMaskLow & Math.pow(2,0);
				var sceneName:String = GameCommonData.GameInstance.GameScene.GetGameScene.name;
				if(mask==0 && this.doFirst && (sceneName == "1002" || sceneName == "1035" || sceneName == "1037" || sceneName == "1039" || sceneName == "1041") && NewerHelpData.isFirst){
					sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 15);
					NewerHelpData.isFirst = false;
				}
			}
			/////
			GameCommonData.BigMapMaskLow=GameCommonData.BigMapMaskLow | Math.pow(2,(pos-1))
			//toDo 向服务器发一下
			
			if(!this.doFirst){
				this.doFirst=true;
				return;
			}
			
			sendNotification(EventList.SEND_QUICKBAR_MSG);
			
			//过场景关闭场景地图
			sendNotification(EventList.CLOSESCENEMAP);
			
			
		}
		
		protected function initResize(event:Event):void
		{
			sendNotification(EventList.STAGECHANGE);
		}
	}
}