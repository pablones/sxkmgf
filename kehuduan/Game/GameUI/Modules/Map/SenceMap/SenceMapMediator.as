package GameUI.Modules.Map.SenceMap
{
	import Controller.PlayerController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Task.Commamd.MoveToCommon;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.LoadingView;
	import GameUI.View.Components.UISprite;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import OopsEngine.AI.PathFinder.MapTileModel;
	
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.display.Bitmap;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SenceMapMediator extends Mediator
	{
		public static const NAME:String = "SenceMapMediator";
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase = null;
		
		private var symbolLayer:Bitmap;                             //标记层
		private var mapLayer:Bitmap;                                //地图层
		private var loader:BulkLoaderResourceProvider;
		private var senceMapContainer:Sprite;
		private var mapContainer:Sprite  							//地图容器
		
		
		private var minWidth:Number =500;							//场景地图最小Width
		private var minHeight:Number = 336;							//场景地图最小Height
		private var mapControl:MovieClip;		                    //功能MC		
		private var closeBtn:SimpleButton;							//关闭按钮
		private var scenceName:String;                              //场景名称
		private var pathSprite:UISprite;                            //路径显示图
		private var isCurrentScene:Boolean=false;
		private var SceneMapBg:MovieClip;
		private var mapLayerContainer:Sprite;
		private var sName:String;
		
		private var playerIcon:MovieClip;                          //玩家标记图标
		private const direction:Array=[0,-27,-90,-153,0,0,180,27,90,153];       
		private var flag:Boolean=false;
		/** 是否正在进行预加载*/
		private var isProLoading:Boolean;
		private var isLoading:Boolean;
		         
		private var mapImgDic:Dictionary=new Dictionary();              
		
		public function SenceMapMediator()
		{
			super(NAME);
		}
		
		//边框层
		private function get SenceMap():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWSENCEMAP,
				EventList.CLOSESCENEMAP,
				EventList.UPDATE_SMALLMAP_DATA,
				EventList.UPDATE_SMALLMAP_PATH,
				EventList.CHANGE_SCENEMAP_EVENT,
				EventList.SHOW_SCENE_TEAMINFO
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SENCEMAP});
					(this.viewComponent as DisplayObjectContainer).mouseEnabled=false;
					//功能控制层
					mapControl=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SenceMapSymbol");
					mapControl.mouseEnabled=false;
					(mapControl.btn_forceChar as SimpleButton).addEventListener(MouseEvent.CLICK,onForceCharClick);
					(mapControl.btn_translation as SimpleButton).addEventListener(MouseEvent.CLICK,onTranslationClick);
					//背景层
					SceneMapBg=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SenceMapBg");
					SceneMapBg.mouseEnabled=false;
					
					(mapControl.btn_bigMap as SimpleButton).addEventListener(MouseEvent.CLICK,onBigMapChangeHandler);
					closeBtn=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("CloseBtn");
					closeBtn.addEventListener(MouseEvent.CLICK,panelCloseHandler);
					senceMapContainer = new Sprite();
					senceMapContainer.name = "senceMapContainer";
					mapContainer = new Sprite();
					mapLayerContainer=new Sprite();
					mapLayerContainer.mouseEnabled=false;
					mapContainer.mouseEnabled=false;
					this.playerIcon=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SelfPos");
					this.senceMapContainer.addChild(SceneMapBg);
					this.senceMapContainer.addChild(mapLayerContainer);
					this.senceMapContainer.addChild(SenceMap);
					this.senceMapContainer.addChild(this.mapContainer);
					this.senceMapContainer.addChild(mapControl);
					this.senceMapContainer.addChild(closeBtn);
				break;
				case EventList.SHOWSENCEMAP:
					if(this.isLoading){
						return;
					}
					this.isLoading=true;
					LoadingView.getInstance().showLoading();
					if(notification.getBody()==null){
						this.isCurrentScene=true;
						if(flag){
							GameCommonData.Player.SMapPathUpdate=drawPath;
						}
						this.scenceName=GameCommonData.GameInstance.GameScene.GetGameScene.name;
						this.pathSprite=new UISprite();
					}else{
						this.isCurrentScene=false;
						this.scenceName=notification.getBody().toString();
					}
					
					if(dataProxy.BigMapIsOpen){
						sendNotification(EventList.CLOSEBIGMAP);
					}
					
					dataProxy.SenceMapIsOpen=true;
					
					loadMap();
				break;
				case EventList.CLOSESCENEMAP:
					if(this.isLoading)return;
					panelCloseHandler(null);
				break;
				case EventList.UPDATE_SMALLMAP_DATA:
					if(dataProxy.SenceMapIsOpen==false || this.isCurrentScene==false)return;
					var obj:Object=notification.getBody();
					var type:uint=obj.type;    
					var id:uint=obj.id;
					if(type==5 && this.playerIcon!=null){
						this.changePlayerPos();
					}
					if(type==6 && this.playerIcon!=null){
						this.changePlayerDirection();
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
				//切换场景地图进行预加载	
				case EventList.CHANGE_SCENEMAP_EVENT:
					
					if(this.mapImgDic[sName]==null &&!this.isProLoading){
						this.isProLoading=true;
						sName=GameCommonData.GameInstance.GameScene.GetGameScene.name;
						loader=new BulkLoaderResourceProvider();
						loader.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + "Scene/"+sName+"/SceneMapBottom.jpg");
						loader.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + "Scene/"+sName+"/SceneMapTop.png");
						loader.LoadComplete=onPreLoadComplete
						loader.Download.Load()
					}
					break;	
				case EventList.SHOW_SCENE_TEAMINFO:
					this.showTeamerScale(notification.getBody() as Array);
					break;	
			}	
		}
		/**
		 * 势力分析图  
		 * @param e
		 * 
		 */		
		protected function onForceCharClick(e:MouseEvent):void{
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_map_sen_sen_onF" ], color:0xffff00});//"此功能还未开放" 
		}
		
		
		/**
		 * 传送 
		 * @param e
		 * 
		 */		
		protected function onTranslationClick(e:MouseEvent):void{
			if(this.scenceName==GameCommonData.GameInstance.GameScene.GetGameScene.name){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_map_sen_sen_onT" ], color:0xffff00});//"已经在当前场景中了，无需传送" 
			}else{
				var obj:Object=SmallConstData.getInstance().mapItemDic[this.scenceName];
				if(obj!=null){
					MoveToCommon.FlyTo(uint(this.scenceName),obj.tileX,obj.tileY,0,0);
				}
			}
		}
		
		
		protected function onPreLoadComplete():void{
			if(!this.isProLoading)return;
			var map:Bitmap = loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory  + "Scene/" + sName + "/SceneMapBottom.jpg").GetBitmap();
			var symbol:Bitmap =loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory + "Scene/" + sName + "/SceneMapTop.png").GetBitmap();
			this.mapImgDic[sName]={mapLayer:map,symbolLayer:symbol};
			this.isProLoading=false;
		}
		
		protected function onBigMapChangeHandler(e:MouseEvent):void{
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name=="1026"){
				return;
			}
			this.panelCloseHandler(null);
			sendNotification(EventList.SHOWBIGMAP);
		}
		
		protected function onSmallClickHandler(e:MouseEvent):void{
			GameCommonData.isAutoPath=false;
			GameCommonData.IsMoveTargetAnimal = false;
			this.flag=true;
			this.pathSprite.graphics.clear();
			GameCommonData.Scene.MapPlayerMove(new Point(this.pathSprite.mouseX/getRatio(),this.pathSprite.mouseY/getRatio()));	
			GameCommonData.Player.SMapPathUpdate = drawPath;
			PlayerController.EndAutomatism();
		}
		
		protected function drawPath():void{
			
			if(flag)
			{
				var path:Array=GameCommonData.Player.PathMap;	
				if(path==null || pathSprite==null)return;
				this.pathSprite.graphics.clear();
				this.pathSprite.graphics.beginFill(0xff0000,1);
				var len:uint=path.length;
				for(var i:uint=0;i<len;i++){
					pathSprite.graphics.drawCircle(path[i].x*getRatio(),path[i].y*getRatio(),1);
				}
				this.pathSprite.graphics.endFill();
			}
		}
			
		/** 改变位置*/
		private function changePlayerPos():void{
			this.playerIcon.x=uint(GameCommonData.Player.GameX*getRatio());
			this.playerIcon.y=uint(GameCommonData.Player.GameY*getRatio());
		}
		
		/**改变方向 */
		private function changePlayerDirection():void{
			this.playerIcon.rotation=this.direction[GameCommonData.Player.Role.Direction];
		}
		
		private function loadMap():void
		{
			if(this.mapImgDic[scenceName]!=null){
				this.processMap();
				return;
			}
			if(GameCommonData.GameInstance.GameScene.GetGameScene.name==scenceName && this.isProLoading){
				this.isProLoading=false;
			}
			loader=new BulkLoaderResourceProvider();
			loader.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + "Scene/"+scenceName+"/SceneMapBottom.jpg");
			loader.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + "Scene/"+scenceName+"/SceneMapTop.png");
			loader.Download.Load()
			loader.LoadComplete=onLoaderComplete;
		}
		
		private function onLoaderComplete():void
		{
			var map:Bitmap = loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory  + "Scene/" + scenceName + "/SceneMapBottom.jpg").GetBitmap();
			var symbol:Bitmap =loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory + "Scene/" + scenceName + "/SceneMapTop.png").GetBitmap();
			this.mapImgDic[scenceName]={mapLayer:map,symbolLayer:symbol};
			processMap();
		}
		
		private function processMap():void{
			
			mapLayer = this.mapImgDic[scenceName].mapLayer;
			symbolLayer=this.mapImgDic[scenceName].symbolLayer;
			initMap();
		}
		
		
		private function initMap():void
		{		
			while(this.mapLayerContainer.numChildren>0){
				this.mapLayerContainer.removeChildAt(0);
			}
			while(this.mapContainer.numChildren>0){
				this.mapContainer.removeChildAt(0);     
			}
			this.mapLayerContainer.addChild(mapLayer);
			this.mapContainer.addChild(symbolLayer);
			
			if(this.isCurrentScene){
				this.mapContainer.addChild(this.pathSprite);
				if(GameCommonData.Player.Role.idTeam>0){	
					this.getTeamPosAction();
				}
				var pos:Point=new Point(uint(GameCommonData.Player.GameX*getRatio()),uint(GameCommonData.Player.GameY*getRatio()));
				this.pathSprite.addChild(this.playerIcon);
				this.playerIcon.x=pos.x;
				this.playerIcon.y=pos.y;
				this.pathSprite.addEventListener(MouseEvent.MOUSE_MOVE,onPathSpriteMove);
				this.pathSprite.addEventListener(MouseEvent.ROLL_OUT,onRollOutPathSprite);
				this.pathSprite.addEventListener(MouseEvent.CLICK,onSmallClickHandler);
				this.pathSprite.mouseEnabled=true;
			}
			SenceMap.addEventListener(MouseEvent.MOUSE_DOWN, senceMapStart);
			this.doLayout();
		}
		
		
		/**
		 * 显示同场景的队友信息 
		 * @param list
		 * 
		 */		
		protected function showTeamerScale(list:Array):void{
			var len:uint=list.length;
			if(this.pathSprite==null)return;
			for(var i:uint=0;i<len;i++){
				var point:MovieClip=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BigBulePoint");   //队友
				var pos:Point=MapTileModel.GetTilePointToStage(list[i].nPosX,list[i].nPosY);
				this.pathSprite.addChild(point);
				point.x=Math.floor(pos.x*getRatio());
				point.y=Math.floor(pos.y*getRatio());
				point.name="SMALLMAP_"+list[i].memName;
			}
		}
		
		/**
		 *   获取队友位置信息action
		 * 
		 */		
		protected function getTeamPosAction():void{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(290);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		
		private function onRollOutPathSprite(e:MouseEvent):void{
			(mapControl.txt_posX as TextField).text=GameCommonData.Player.Role.TileX.toString();
			(mapControl.txt_posY as TextField).text=GameCommonData.Player.Role.TileY.toString();
		}
		
		private function onPathSpriteMove(e:MouseEvent):void{	
			var stagePoint:Point=new Point(e.currentTarget.mouseX/getRatio(),e.currentTarget.mouseY/getRatio())
			var point:Point=MapTileModel.GetTileStageToPoint(stagePoint.x,stagePoint.y);
			(mapControl.txt_posX as TextField).text=String(point.x);
			(mapControl.txt_posY as TextField).text=String(point.y);
		}
		
		private function doLayout():void
		{
			SenceMap.width = Math.max(minWidth,mapLayer.width)+20;
			SenceMap.height= Math.max(minHeight,mapLayer.height) + 50;
			this.SceneMapBg.width=SenceMap.width;
			this.SceneMapBg.height=SenceMap.height;
			
			this.mapContainer.x=Math.floor((SenceMap.width-this.mapContainer.width)/2);
			this.mapContainer.y=Math.floor((SenceMap.height-this.mapContainer.height)/2)+16;
			
			this.mapLayer.x=20;
			if(isCurrentScene){
				this.pathSprite.x=this.mapLayer.x;
				this.pathSprite.width=this.mapLayer.width;
				this.pathSprite.height=this.mapLayer.height;
			}
			
			this.mapControl.x=20;
			this.mapControl.y=17;
			this.closeBtn.x=SenceMap.width-40;
			this.closeBtn.y=17;
			
			senceMapContainer.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - senceMapContainer.width) / 2;
			senceMapContainer.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - senceMapContainer.height) / 2;
			
			mapLayer.x=this.mapContainer.x+20;
			mapLayer.y=this.mapContainer.y;
			
			//布局完后才出现
			LoadingView.getInstance().removeLoading();
			GameCommonData.GameInstance.GameUI.addChild(senceMapContainer);
			this.isLoading=false;
			
			if(NewerHelpData.newerHelpIsOpen) {
				sendNotification(NewerHelpEvent.OPEN_SENCE_MAP_NOTICE_NEWER_HELP);	//通知新手引导
			}
		}
		
		private function senceMapStart(event:MouseEvent):void
		{
			senceMapContainer.startDrag();
			SenceMap.addEventListener(MouseEvent.MOUSE_UP, senceMapStop);
		}
		
		private function senceMapStop(event:MouseEvent):void
		{
			senceMapContainer.stopDrag();
			SenceMap.removeEventListener(MouseEvent.MOUSE_UP, senceMapStop);
		}

		private function panelCloseHandler(event:Event = null):void
		{
			GameCommonData.Player.SMapPathUpdate=null;
			if(SenceMap)
			{
				if(GameCommonData.GameInstance.GameUI.contains(senceMapContainer))
				{
					GameCommonData.GameInstance.GameUI.removeChild(senceMapContainer);
					dataProxy.SenceMapIsOpen = false;					
				}
			}
			while(mapContainer.numChildren>0){
				mapContainer.removeChildAt(0);
			}
			this.mapLayer=null;
			this.symbolLayer=null;
			if(pathSprite){
				this.pathSprite.removeEventListener(MouseEvent.MOUSE_MOVE,onPathSpriteMove);
				this.pathSprite.removeEventListener(MouseEvent.ROLL_OUT,onRollOutPathSprite);
				this.pathSprite.removeEventListener(MouseEvent.CLICK,onSmallClickHandler);
			}
			this.pathSprite=null;
			
			if(NewerHelpData.newerHelpIsOpen) {	//通知新手引导
				sendNotification(NewerHelpEvent.CLOSE_SENCE_MAP_NOTICE_NEWER_HELP);
			}
		}
		
		private const RATIO_NORMAL:Number = 0.14;
		private const RATIO_2016:Number = 0.0765;
		
		private function getRatio():Number
		{
			if (scenceName == "2016")
			{
				return RATIO_2016;
			}
			else
			{
				return RATIO_NORMAL;
			}
		}
	}
}