package OopsEngine.Scene.StrategyScene
{
	import OopsEngine.AI.PathFinder.MapTileModel;
	import OopsEngine.Scene.GameScene;
	
	import OopsFramework.Collections.DictionaryCollection;
	import OopsFramework.Game;
	
	import flash.display.BitmapData;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.ui.Keyboard;

	public class GameSceneEdit extends GameScene
	{
		private var isSpaceDown:Boolean = false;			// 空格键是否按下	
		private var currentGameScreenPoint:Point;			// 当前场景位置
		private var currentMouseDownPoint:Point;			// 当前鼠标点下位置
		
		private var tileLayer:Sprite;
		private var barrier:DictionaryCollection = new DictionaryCollection();
				
		public function GameSceneEdit(game:Game)
		{
			super(game);
//			this.MouseEnabled  = true;
//			this.mouseChildren = false;
//			this.Games.KeyDown = onKeyDown;
//			this.Games.KeyUp   = onKeyUp;
		}
		
		public override function Initialize():void
		{
			super.Initialize();
		}
		
		/** 是否显示网格 */
		public function get IsShowMesh():Boolean
		{
			if(this.tileLayer!=null)
			{
				return this.tileLayer.visible;
			}
			return false;
		}
		public function set IsShowMesh(value:Boolean):void
		{
			if(this.tileLayer!=null)
			{
				this.tileLayer.visible = value;
			}
		}
		
		protected override function LoadContent():void
		{
			super.LoadContent();
			
			// 绘制网格
			this.tileLayer = CreateMesh(MapTileModel.TILE_WIDTH,MapTileModel.TILE_HEIGHT,this.MapWidth,this.MapHeight);
			this.tileLayer.mouseChildren = false;
			this.tileLayer.mouseEnabled  = false;
			this.addChildAt(this.tileLayer,this.numChildren);
			
			// 显示地型数据
			for(var i:uint = 0 ; i < this.Map.Map.length ; i++)
			{
				for(var j:uint = 0 ; j < this.Map.Map[0].length ; j++)					// 一次绘两个格
				{
					var key:String = i + "_" + j;
					var tile:Shape;
					if(this.Map.Map[i][j] == MapTileModel.PATH_BARRIER)					// 障碍物
					{
						tile = this.CreateTile(MapTileModel.TILE_WIDTH,MapTileModel.TILE_HEIGHT,0xff0000,0x000000,0.5,0);
						this.SetTileLocation(tile, new Point(i,j));
						this.barrier.Add(key,tile);
						this.tileLayer.addChild(tile);
					}
					else if(this.Map.Map[i][j] == MapTileModel.PATH_TRANSLUCENCE)		// 半透区
					{
						tile = this.CreateTile(MapTileModel.TILE_WIDTH,MapTileModel.TILE_HEIGHT,0x00ff00,0x000000,0.5,0);
						this.SetTileLocation(tile, new Point(i,j));
						this.barrier.Add(key,tile);
						this.tileLayer.addChild(tile);
					}
					else if(this.Map.Map[i][j] == MapTileModel.PATH_BOOTH)				// 摆滩位
					{
						tile = this.CreateTile(MapTileModel.TILE_WIDTH,MapTileModel.TILE_HEIGHT,0x0000ff,0x000000,0.5,0);
						this.SetTileLocation(tile, new Point(i,j));
						this.barrier.Add(key,tile);
						this.tileLayer.addChild(tile);
					}
				}
			}
		}
		
		/** 鼠标按下事件-托动地图开关和选择障碍物点 */
//		protected override function onMouseDown(e:MouseEvent):void
//		{
//			if(isSpaceDown == true)
//			{
//				currentGameScreenPoint = new Point(this.x,this.y);
//				currentMouseDownPoint  = new Point(this.Games.mouseX,this.Games.mouseY);
//			}
//			else// A*地图编辑
//			{
//				// 鼠标选矩阵中点
//				var hitPoint:Point = MapTileModel.GetTileStageToPoint(this.mouseX, this.mouseY);
//				
//				// 障碍物菱形显示
//				if(hitPoint.x >= 0 && hitPoint.x <= this.Floor.Area.length && 
//				   hitPoint.y >= 0 && hitPoint.y <= this.Floor.Area[0].length)
//				{ 
//					var key:String = hitPoint.x + "_" + hitPoint.y;
//					if(this.Floor.Area[hitPoint.x][hitPoint.y] == MapTileModel.PATH_PASS)
//					{
//						// 设计地型不可通过
//						this.Floor.Area[hitPoint.x][hitPoint.y] = MapTileModel.PATH_BARRIER;
//						
//						var tile:Shape = this.CreateTile(this.Floor.TileWidth,this.Floor.TileHeight,0xff0000,0x000000,0.5,0);
//						this.SetTileLocation(tile, hitPoint);
//						this.barrier.Add(key,tile);
//						this.tileLayer.addChild(tile);
//					}
//					else
//					{
//						// 设计地型不可通过
//						this.Floor.Area[hitPoint.x][hitPoint.y] = MapTileModel.PATH_PASS;
//						
//						this.tileLayer.removeChild(this.barrier[key]);
//						this.barrier.Remove(key);
//					}
//				}
//			}
//		}
		
		/** 关闭地图托放开关 */
//		protected override function onMouseUp(e:MouseEvent):void
//		{
//			currentGameScreenPoint = null;
//			currentMouseDownPoint  = null;
//		}
		
		/** 鼠标移动事件 - 在按下空格+鼠标左按时可以按动地图 */
		protected override function onMouseMove(e:MouseEvent):void
		{
			if((isSpaceDown == true) && e.buttonDown == true 
				&& currentGameScreenPoint!=null && currentMouseDownPoint!=null)
			{
				var x:Number = currentGameScreenPoint.x - currentMouseDownPoint.x + this.Games.mouseX;
				var y:Number = currentGameScreenPoint.y - currentMouseDownPoint.y + this.Games.mouseY;
				
				var leftX:Number   = 0;
				var rightX:Number  = Math.abs(this.Games.ScreenWidth  - this.MapWidth);

				var topY:Number    = 0;
				var bottomY:Number = Math.abs(this.Games.ScreenHeight - this.MapHeight);
				
				// 地图范围限制
				if(this.Games.ScreenWidth > this.MapWidth)
				{  
					x = 0;
				}
				else
				{
					rightX            = rightX * -1;
					if(x > leftX)   x = leftX;
					if(x < rightX)  x = rightX
				}
				if(this.Games.ScreenHeight > this.MapHeight) 
				{  
					y = 0;
				}
				else
				{
					bottomY			  = bottomY * -1;
					if(y > topY)    y = topY;
					if(y < bottomY) y = bottomY;
				}
	
				this.x = x;
				this.y = y;
			}
		}
		
		private function onKeyDown(e:KeyboardEvent):void
		{
			if(e.keyCode==Keyboard.SPACE)
			{
				this.mouseChildren = false;
				this.isSpaceDown   = true;
			}
		}
		
		/** 地图托动开关 */
		private function onKeyUp(e:KeyboardEvent):void
		{
			if(e.keyCode==Keyboard.SPACE)
			{
				this.mouseChildren = true;
				this.isSpaceDown   = false;
			}
		}
		
		/** 设置障碍格子的位置 */
		private function SetTileLocation(tile:Shape,hitPoint:Point):void
		{
			var p:Point = MapTileModel.GetTilePointToStage(hitPoint.x,hitPoint.y);
			tile.x = p.x - MapTileModel.TITE_HREF_WIDTH;
			tile.y = p.y - MapTileModel.TITE_HREF_HEIGHT;
//			if(hitPoint.y % 2 == 0)
//			{
//				hitPoint.y = hitPoint.y / 2;
//				tile.x     = hitPoint.x * this.Floor.TileWidth;
//				tile.y     = hitPoint.y * this.Floor.TileHeight;
//			}
//			else
//			{
//				hitPoint.y = (hitPoint.y - 1) / 2;
//				tile.x     = hitPoint.x * this.Floor.TileWidth  + this.Floor.TileWidth  / 2;
//				tile.y     = hitPoint.y * this.Floor.TileHeight + this.Floor.TileHeight / 2;
//			}			
		}
		
		/** 地图生成时候 背景网格和 障碍物网格元件生成 */
		private function CreateMesh(tileWidth:uint, tileHeight:uint, mapWidth:Number, mapHeight:Number):Sprite
		{
			// 绘制底图小格
			var grid:Shape = CreateTile(tileWidth,tileHeight,0xffff00,0x000000,0,1);
			 
			// 将小格转为绘图格式
			var myBitmapData:BitmapData = new BitmapData(tileWidth , tileHeight ,true, 0x00000000);
			myBitmapData.draw(grid);
			
			// 在指定区域里面填充小格子 
			var gridSreenW:Number = (int( mapWidth  / tileWidth )  + 1) * tileWidth;
			var gridSreenH:Number = (int( mapHeight / tileHeight ) + 1) * tileHeight;
			
			var child:Sprite = new Sprite();
			child.graphics.clear();
			child.graphics.beginBitmapFill (myBitmapData, null , true , true);
            child.graphics.drawRect(0, 0, gridSreenW, gridSreenH);
            child.graphics.endFill();
            return child;
		}
		
		/** 格子生成方法 */
		private function CreateTile(tileWidth:uint , tileHeight:uint , 
								    bgcolor:Number , bgLinecolor:Number , 
								    bgalpha:Number , bgLinealpha:Number):Shape
		{
			var tile:Shape = new Shape();
			tile.graphics.lineStyle(1, bgLinecolor, bgLinealpha);
			tile.graphics.beginFill(bgcolor, bgalpha);
			tile.graphics.moveTo(0, tileHeight / 2);
            tile.graphics.lineTo(tileHeight, 0);
            tile.graphics.lineTo(tileWidth , tileHeight / 2);
            tile.graphics.lineTo(tileHeight, tileHeight);
            tile.graphics.lineTo(0, tileHeight / 2); 
			tile.graphics.endFill();
			return tile;
		}
	}
}