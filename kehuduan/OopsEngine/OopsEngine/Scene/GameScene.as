package OopsEngine.Scene
{
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Engine;
	import OopsEngine.Exception.ExceptionResources;
	
	import OopsFramework.*;
	import OopsFramework.Audio.AudioEngine;
	import OopsFramework.Collections.DictionaryCollection;
	import OopsFramework.Content.Loading.BulkProgressEvent;
	
	import flash.display.*;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.*;
	
	/** 游戏场景实体类 */
	public class GameScene extends DrawableGameComponent implements IDisposable
	{
		public var SceneUrl:String;						// 场景资源路径
		public var ConfigXml:XML;						// 场景配制文件数据
		
		public var Scale:Number;
		public var MapId:String;						// 地图编号
		public var MapName:String;						// 地图名
		public var Description:String;					// 地图说明
		public var MapWidth:int;						// 地图宽
		public var MapHeight:int;						// 地图高
		public var OffsetX:int;							// 地图偏移 X
		public var OffsetY:int;							// 地图偏移 Y
		public var Map:MapTileModel;					// 地A*地型
		
		public var SmallMap:Bitmap;						// 小地图
		public var RealSmallMap:Bitmap;                 // 真正的小地图，上面的是马赛克
			
//		public var LoadingCurrentCount:int = 0;						// 当前下载对列数
//		public var LoadingQueue:Array;								// 场景上资料加载并发控制对列
//		public var LoadingCurrentCount:int = 0;						// 当前下载对列数
//		public var LoadingQueue:Array;								// 场景上资料加载并发控制对列
		
		public var ResourceLoadingQueue:Dictionary;					// 场景上资源加载对列
		public var CacheResource:DictionaryCollection;				// 场景上的资源缓存
		public var ConnectScene:DictionaryCollection;				// 场景接点数据存 GameElement 对象（转场点数据）
		
		public var CacheEffectResource:Dictionary;                  // 场景上特效缓存
		public var EffectResourceLoadingQueue:Dictionary;           // 场景上特效加载队列
		
		
		/** 游戏场景加载完成事件  */
		public var GameSceneLoadComplete : Function;
		/** 游戏场景加载数据进度事件  */
		public var GameSceneDataProgress : Function;
		/** 鼠标左键按下事件 */
		public var MouseDown : Function;
		/** 鼠标左键弹起事件 */
		public var MouseUp   : Function;	
		/** 鼠标移动事件 */
		public var MouseMove : Function;
         /**校对更新函数**/
        public var UpdateNicety:Function;

		private var topLayer    : GameSceneLayer;
		private var middleLayer : GameSceneLayer;
		private var bottomLayer : GameSceneLayer;
		private var background	: GameSceneBackground;

		private var configProvider : GameSceneConfig;
		
		public function set audioEngine(value:AudioEngine):void
		{
			CommonData.audioEngine = value;
		}
		
		public function get audioEngine():AudioEngine
		{
			return CommonData.audioEngine;
		}		
		
//		public var audioEngine    : AudioEngine;		
		
		/** IDisposable Start */
		public override function Dispose():void
		{
			this.Enabled       = false;
			this.MouseEnabled  = false;
			this.mouseChildren = false;
			
			this.background.Dispose();
			this.background			  = null;
			this.Map.Dispose();
			this.Map				  = null;
			this.bottomLayer.Dispose();
			this.bottomLayer		  = null;
			this.middleLayer.Dispose();
			this.middleLayer		  = null;
			this.topLayer.Dispose();
			this.topLayer		  	  = null;
			this.ConnectScene.Dispose();
			this.ConnectScene		  = null;
			this.CacheResource.Dispose();
			this.CacheResource  	  = null;
			
			this.ConfigXml			  = null;
			this.SmallMap			  = null;
			this.RealSmallMap		  = null;
			this.ResourceLoadingQueue = null;
			
			this.GameSceneLoadComplete = null;
			this.GameSceneDataProgress = null;
			this.MouseDown 			   = null;
			this.MouseUp 			   = null;
			this.MouseMove 			   = null;
			this.UpdateNicety 		   = null;
			
//			if(CommonData.IsPlayThemeSongComplete) 
//				this.MusicStop();
			
			// 父级对象移除
			if(this.parent != null)
			{
				this.parent.removeChild(this);
			}
			
			super.Dispose();
		}
		/** IDisposable End */
		
		
		public function GameScene(game:Game)
		{		
			super(game);
		}
		
		/** 加载对列  */
//		public function Loading():void
//		{
//			if(this.LoadingQueue!=null && this.LoadingQueue.length > 0 && LoadingCurrentCount==0)
//			{
//				LoadingCurrentCount++;
//				this.LoadingQueue.shift().Load();
//			}
//		}
		
		/** 加载对列完成  */
//		public function Loaded():void
//		{
//			LoadingCurrentCount--;
//			this.Loading();
//		}
		
		public override function Initialize():void
		{
//			this.CacheResource 		  = new Dictionary();
//			this.ResourceLoadingQueue = new Dictionary();
//			this.LoadingQueue		  = new Array();
			
			this.SceneUrl = this.Games.Content.RootDirectory + "Scene/" + this.name + "/";
			
			CommonData.SceneID = this.SceneUrl + "/" + "Sound.mp3";
			// 加载地图数据
			this.configProvider   		 = new GameSceneConfig(this);
			this.configProvider.Progress = onConfigLoadProgress;
			this.ResourceProviderEntity  = this.configProvider;

			super.Initialize();
		}
		
		protected override function LoadContent():void
		{
			if(this.ConfigXml == null || this.ConfigXml == "")
			{
				throw new Error(ExceptionResources.ErrorGameSceneConfig);
			}
			
        	this.AnalyseFloor();
        	this.AnalyseTopLayer();
        	this.AnalyseMiddleLayer();
        	this.AnalyseBottomLayer();
			
			// 背景图
			this.background 	 = new GameSceneBackground(this);
			this.background.name = "BackgroundLayer";
			this.addChildAt(this.background,0);
	
			this.addChildAt(this.bottomLayer, 1);
			this.addChildAt(this.middleLayer, 2);
			this.addChildAt(this.topLayer   , 3);
			
			// 地图加载完成事件
			if(GameSceneLoadComplete!=null)GameSceneLoadComplete();
			
			super.LoadContent();
		}
		
		public override function Update(gameTime:GameTime):void
		{
			this.bottomLayer.Update(gameTime);
			this.middleLayer.Update(gameTime);
			this.topLayer.Update(gameTime);
//			for (var i:int = 0; i < this.gameSceneLayers.length; i++)
//			{
//				this.gameSceneLayers[i].Update(gameTime);
//			}
        	super.Update(gameTime);
		}
		
		protected virtual function onMouseMove(e:MouseEvent) : void {if(MouseMove!=null) MouseDown(e);}
		protected virtual function onMouseUp(e:MouseEvent)   : void {if(MouseUp!=null)   MouseUp(e)  ;}
		protected virtual function onMouseDown(e:MouseEvent) : void {if(MouseDown!=null) MouseDown(e);}
		
		/** Event Modal Start */
		private function onConfigLoadProgress(e:BulkProgressEvent):void
		{
			if(GameSceneDataProgress!=null) GameSceneDataProgress(e);
		}
		/** Event Modal End */
		
		/** 清除场景上的元件  */
		public function ClearElement():void
		{
			this.bottomLayer.ClearElement();
			this.middleLayer.ClearElement();
			this.topLayer.ClearElement();
		}
		
        /** 地图信息 */
        private function AnalyseFloor():void
        {
        	this.MapName	 = this.ConfigXml.@Name;
        	this.MapWidth    = this.ConfigXml.@MapWidth;
        	this.MapHeight   = this.ConfigXml.@MapHeight
        	this.OffsetX	 = this.ConfigXml.@OffsetX;
        	this.OffsetY	 = this.ConfigXml.@OffsetY;
        	this.Scale       = this.ConfigXml.@Scale;
        	this.Description = this.ConfigXml.@Description;
        	
//        	floor 			 = new GameSceneFloor();
//        	floor.TileWidth  = ConfigXml.Floor.@TileWidth;
//        	floor.TileHeight = ConfigXml.Floor.@TileHeight;
//        	floor.Row		 = ConfigXml.Floor.@Row;
//        	floor.Col 		 = ConfigXml.Floor.@Col;
//        	floor.OffsetX 	 = ConfigXml.Floor.@OffsetX;
//        	floor.OffsetY 	 = ConfigXml.Floor.@OffsetY;
//        	MapTileModel.OFFSET_TAB_X = floor.OffsetX;
//        	MapTileModel.OFFSET_TAB_Y = floor.OffsetY;
//        	MapTileModel.TILE_WIDTH   = floor.TileWidth;
//        	MapTileModel.TILE_HEIGHT  = floor.TileHeight;
//        	MapTileModel.TITE_HREF_WIDTH  = floor.TileWidth / 2;
//        	MapTileModel.TITE_HREF_HEIGHT = floor.TileHeight / 2;
			MapTileModel.OFFSET_TAB_X	  = ConfigXml.Floor.@OffsetX;
        	MapTileModel.OFFSET_TAB_Y 	  = ConfigXml.Floor.@OffsetY;
        	MapTileModel.TILE_WIDTH   	  = ConfigXml.Floor.@TileWidth;
        	MapTileModel.TILE_HEIGHT 	  = ConfigXml.Floor.@TileHeight;
        	MapTileModel.TITE_HREF_WIDTH  = MapTileModel.TILE_WIDTH  / 2;
        	MapTileModel.TITE_HREF_HEIGHT = MapTileModel.TILE_HEIGHT / 2;
        	
        	// 生成A*地型数组
        	var area:String     = ConfigXml.Floor.text();
			var areaArray:Array = area.split(",");
			var w:int  			= MapTileModel.TILE_WIDTH  / 2;
			var h:int  			= MapTileModel.TILE_HEIGHT / 2;
			var mapData:Array = [];
			for(var i:uint = 0 ; i < ConfigXml.Floor.@Row ; i++)
			{
				mapData[i] = new Array();
				for(var j:uint = 0 ; j < ConfigXml.Floor.@Col ; j++)		// 每一次存两格数据
				{
					mapData[i][j] = areaArray[i * ConfigXml.Floor.@Col + j];
				}
			}
			this.Map 	 = new MapTileModel();
			this.Map.Map = mapData;
//        	var area:String     = ConfigXml.Floor.text();
//        	var areaArray:Array = area.split(",");
//        	var w:int  			= floor.TileWidth  / 2;
//			var h:int  			= floor.TileHeight / 2;
//        	floor.Area 			= new Array();
//        	for(var i:uint = 0 ; i < floor.Row ; i++)
//        	{
//        		floor.Area[i] = new Array();
//        		for(var j:uint = 0 ; j < floor.Col ; j++)		// 每一次存两格数据
//	        	{
//					floor.Area[i][j] = areaArray[i * floor.Col + j];
//	        	}
//        	}
//			this.Map 	 = new MapTileModel();
//			this.Map.Map = floor.Area;
        }
        
        /** 上层信息 */
        private function AnalyseTopLayer():void
        {
        	topLayer      = new GameSceneLayer();
			topLayer.name = "GameSceneLayer_Top";
        }
        
        /** 中间层信息 */
        private function AnalyseMiddleLayer():void
        {
        	middleLayer      = new GameSceneLayer();
        	middleLayer.name = "GameSceneLayer_Mmiddle";
        }
        
        /** 下层信息 */
        private function AnalyseBottomLayer():void
        {		
			this.ConnectScene = new DictionaryCollection();
        	bottomLayer       = new GameSceneLayer();
        	bottomLayer.name  = "GameSceneLayer_Bottom";
        	if(ConfigXml.Element != null)
        	{
	        	for each(var object:XML in ConfigXml.Element)
	        	{
	        		var element:GameElement = new GameElement(this.Games);
	        		element.Prams		    = object.@To;
	        		element.X 				= object.@X;
	        		element.Y 				= object.@Y;
	        		this.bottomLayer.Elements.Add(element);
	        		
	        		// 显示转场点
	        		var ts:MovieClip   = this.Games.Content.Load(Engine.UILibrary).GetClassByMovieClip("TransferScene_1");	// 转场点玩件
					ts.mouseChildren   = false;
					ts.mouseEnabled    = false;
					var p:Point		   = MapTileModel.GetTilePointToStage(element.X,element.Y);
					ts.x 			   = p.x;
					ts.y 			   = p.y;
	        		this.BottomLayer.addChild(ts);
	        		
	        		this.ConnectScene.Add(element.Prams, element);
	        	}
        	}
        }
        /** ISerializable End */
        
        /** Property Start */
        /** 场景地形 */
//		public function get Floor():GameSceneFloor
//		{
//			return this.floor;
//		}
		/** 场景顶层 */
		public function get TopLayer():GameSceneLayer
		{
			return this.topLayer;
		}
		/** 场景中层 */
		public function get MiddleLayer():GameSceneLayer
		{
			return this.middleLayer;
		}
		/** 场景下层 */
		public function get BottomLayer():GameSceneLayer
		{
			return this.bottomLayer;
		}
		/** 背景层管理对象  */
		public function get Background():GameSceneBackground
		{
			return this.background;
		}
		
		/** 鼠标事件是否启用 */
		public function get MouseEnabled():Boolean
		{
			return this.mouseEnabled;
		}
		public function set MouseEnabled(value:Boolean):void
		{
			this.mouseEnabled = value;
			if(this.mouseEnabled)
			{
				this.addEventListener(MouseEvent.MOUSE_DOWN  , onMouseDown);
				this.addEventListener(MouseEvent.MOUSE_UP    , onMouseUp);
				this.addEventListener(MouseEvent.MOUSE_MOVE  , onMouseMove);
			}
			else
			{
				this.removeEventListener(MouseEvent.MOUSE_DOWN  , onMouseDown);
				this.removeEventListener(MouseEvent.MOUSE_UP    , onMouseUp);
				this.removeEventListener(MouseEvent.MOUSE_MOVE  , onMouseMove);
			}
		}
		/** Property End */
		
		private var musicIntervalId:int;				// 场景背景音乐延时器编号
		private var musicDelay:int = 5000;				// 场景背景音乐延时时候（5000毫秒）
		
		/** 开始加载背景音乐  */
		public function MusicLoad(value:int = 100):void
		{
			this.musicIntervalId = setTimeout(PlayMusic,this.musicDelay,value);
		}
		
		/** 关闭背景音乐  */
		public function MusicStop():void
		{
			if(this.audioEngine!=null && this.audioEngine.IsPlaying)
			{
				this.audioEngine.Stop();
			}
			clearTimeout(this.musicIntervalId);
			this.audioEngine = null;
		}
		
		/** 声音打开  */
		public function VolumeOpen():void
		{
			if(this.audioEngine!=null)
			{
				this.audioEngine.Volume = 100;
			}
		}
		
		/** 声音关闭  */
		public function VolumeClose():void
		{
			if(this.audioEngine!=null)
			{
				this.audioEngine.Volume = 0;
			}
		}
		
		/** 改变音量 */
		public function SetVolume(value:int):void
		{
			if(this.audioEngine!=null)
			{
				this.audioEngine.Volume = value;
			}
		}
		
		/** 得到音量 */
		public function GetVolume():int
		{
			var value:int;
			if(this.audioEngine!=null)
			{
				value = this.audioEngine.Volume;
			}
			return value;
		}
		
		
		
		private function PlayMusic(value:int):void
		{
			var MusicValue:int = value;
			
			//读取音乐声音
			if(this.audioEngine != null)
			{
				MusicValue = GetVolume();
			}
	
			//还没有播放主题曲 并且 没有播放完成
//			if(IsPlayThemeSong == false && IsPlayThemeSongComplete == false)
//			{
//				if(this.audioEngine == null || MusicUrl != this.Games.Content.RootDirectory + "/" + "LoginSound.mp3")
//				{
//					this.audioEngine 			      = new AudioEngine(this.Games.Content.RootDirectory + "/" + "LoginSound.mp3");
//				}
//				MusicUrl = this.Games.Content.RootDirectory + "/" + "LoginSound.mp3";
//				this.audioEngine.Loop             = 1;
//				this.audioEngine.Volume 	      = MusicValue;
//				this.audioEngine.PlayComplete     = onPlayComplete;
//				this.audioEngine.Play();
//				IsPlayThemeSong = true;
//				IsPlayThemeSongComplete = false;
//			}
//			else
//			{
				if(!CommonData.IsMusicPlayerBusy && CommonData.IsPlayThemeSongComplete)
				{				
					if(CommonData.SceneID == this.SceneUrl + "/" + "Sound.mp3")
					{			
						if(this.audioEngine != null)
						{
							this.MusicStop();
						}
						
						if(this.audioEngine == null || CommonData.MusicUrl != this.Games.Content.RootDirectory + "/" + "Sound.mp3")
						{
							this.audioEngine = new AudioEngine(this.SceneUrl + "/" + "Sound.mp3");
						}
						CommonData.MusicUrl = this.SceneUrl + "/" + "Sound.mp3";
						this.audioEngine.Loop             = 1;
						this.audioEngine.Volume 	      = MusicValue;
						this.audioEngine.Play();
					}
				}
 				if(this.audioEngine != null && CommonData.IsPlayThemeSongComplete)
			    	this.audioEngine.PlayComplete     = onPlayComplete;
//				if(this.audioEngine==null)
//				{
//					this.audioEngine 			  = new AudioEngine(this.SceneUrl + "Sound.mp3");
//					this.audioEngine.Loop 		  = 1;
//					this.audioEngine.Volume 	  = value;
//					this.audioEngine.PlayComplete = onPlayComplete;
//					this.audioEngine.Play();
//				}
//				else
//				{
//					this.audioEngine.Volume = GetVolume();		//记忆上次的音量
//					this.audioEngine.Play();
//				}
//			}
			clearTimeout(this.musicIntervalId);
		}
		
		public function onPlayComplete():void
		{
			//已经播放完成		
			CommonData.IsPlayThemeSongComplete = true;		
			
			if (!CommonData.IsMusicPlayerBusy)
				this.MusicLoad();
		}
	}
}