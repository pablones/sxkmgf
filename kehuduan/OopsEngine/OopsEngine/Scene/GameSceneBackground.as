package OopsEngine.Scene
{
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	import OopsFramework.Game;
	import OopsFramework.IDisposable;
	
	import flash.display.Shape;
	import flash.events.Event;
	import flash.geom.Matrix;
	import flash.utils.Dictionary;
	
	/** 游戏场景背景分块加载管理  */
	public class GameSceneBackground extends Shape implements IDisposable                                                         
	{
		/** 地图块宽  */
		public var MapTileWidth :int = 300;
		/** 地图块高  */
		public var MapTileHeight:int = 300;
		
		private var gameScene:GameScene;
		private var picMaxX:int;							// 图片最大 X	
		private var picMaxY:int;							// 图片最大 Y	
		private var maps:Dictionary;						// 图片块缓存
		private var mapMaxCount:int;
		private var mapCount:int;
		
		public function GameSceneBackground(gameScene:GameScene)
		{
			this.maps 		  = new Dictionary();
			this.gameScene    = gameScene;
			this.picMaxX 	  = this.gameScene.MapWidth  / MapTileWidth  - 1;
			this.picMaxY 	  = this.gameScene.MapHeight / MapTileHeight - 1;
			this.mapMaxCount  = (this.picMaxX + 1) * (this.picMaxY + 1);
			
			// 画马塞克图背景图
			var gameSceneRealWidth:int  = this.gameScene.MapWidth  + this.gameScene.OffsetX;
			var gameSceneRealHeight:int = this.gameScene.MapHeight + this.gameScene.OffsetY;
			var matrix:Matrix 			= new Matrix();
			matrix.a 					= gameSceneRealWidth  / this.gameScene.SmallMap.width;
			matrix.d 					= gameSceneRealHeight / this.gameScene.SmallMap.height;
			this.graphics.beginBitmapFill(this.gameScene.SmallMap.bitmapData, matrix, false, false);
			this.graphics.drawRect(0, 0, gameSceneRealWidth, gameSceneRealHeight);
			this.graphics.endFill();
			
			matrix = null;
		}
		
		/** 地图块加载（默认四个方向都多加载一格图片）  */
		public function LoadMap():void
		{
			if(this.maps!=null && this.mapMaxCount != this.mapCount)
			{
				var CurrentPicX:int    = int(Math.abs(this.gameScene.x) / MapTileWidth);
				var CurrentPicY:int    = int(Math.abs(this.gameScene.y) / MapTileHeight);
				var CurrentPicMaxX:int = int(Math.abs(this.gameScene.Games.ScreenWidth  - this.gameScene.x) / this.MapTileWidth);
				var CurrentPicMaxY:int = int(Math.abs(this.gameScene.Games.ScreenHeight - this.gameScene.y) / this.MapTileHeight);
				
				CurrentPicX    = (CurrentPicX <= 0 ? 0 : CurrentPicX);
				CurrentPicY    = (CurrentPicY <= 0 ? 0 : CurrentPicY);
				CurrentPicMaxX = (CurrentPicMaxX >= picMaxX ? picMaxX : CurrentPicMaxX);
				CurrentPicMaxY = (CurrentPicMaxY >= picMaxY ? picMaxY : CurrentPicMaxY);
				
				// 多加载一圈小地图
//				CurrentPicX    = (CurrentPicX <= 0 ? 0 : CurrentPicX - 1);
//				CurrentPicY    = (CurrentPicY <= 0 ? 0 : CurrentPicY - 1);
//				CurrentPicMaxX = (CurrentPicMaxX >= picMaxX ? picMaxX : CurrentPicMaxX + 1);
//				CurrentPicMaxY = (CurrentPicMaxY >= picMaxY ? picMaxY : CurrentPicMaxY + 1);
				
				for (var i:int = CurrentPicX; i<= CurrentPicMaxX; i++) 
				{
					for (var j:int = CurrentPicY; j <= CurrentPicMaxY; j++) 
					{
						var picName:String = i + "_" + j;
						if(this.maps[picName]==null)
						{
							this.maps[picName]	    = true;
							var picLoader:ImageItem = new ImageItem(this.gameScene.Games.Content.RootDirectory + "Scene/" + this.gameScene.name + "/" + picName + ".jpg", 
																	BulkLoader.TYPE_IMAGE, 
																	picName);
							picLoader.Version		= BulkLoader.version;
							picLoader.addEventListener(Event.COMPLETE, onPicComplete);
							picLoader.load();
							this.mapCount++;
						}
					}
				}
			}
		}
		
		private function onPicComplete(e:Event):void
		{
			var picLoader:ImageItem   = e.target as ImageItem;
			var pos:Array 			  = picLoader.name.split("_");
			var i:uint	  			  = pos[0];
			var j:uint 	  		      = pos[1];
			
			this.graphics.beginBitmapFill(picLoader.content.content.bitmapData);
			this.graphics.drawRect(i * this.MapTileWidth, j * this.MapTileHeight, this.MapTileWidth, this.MapTileHeight);
			this.graphics.endFill();
			
			picLoader.removeEventListener(Event.COMPLETE, onPicComplete);
			picLoader.destroy();
			picLoader = null;
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.maps      = null;
			this.gameScene = null;
			
			// 父级对象移除
			if(this.parent != null)
			{
				this.parent.removeChild(this);
			}
		}
		/** IDisposable End */
	}
}