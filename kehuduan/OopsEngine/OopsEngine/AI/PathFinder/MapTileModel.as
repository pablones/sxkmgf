package OopsEngine.AI.PathFinder
{
	import OopsEngine.Utils.Vector2;
	
	import OopsFramework.IDisposable;
	
	import flash.geom.Point;
	
	/** 地图模型类 （考虑把A*多余不可以走的格子删除）*/	
	public class MapTileModel implements IMapTileModel,IDisposable
	{
		public static const PATH_PASS         : int = 0;	// 路径中 0 为可以通过
		public static const PATH_BARRIER      : int = 1;	// 路径中 1 为障碍
		public static const PATH_TRANSLUCENCE : int = 2;	// 路径中 2 为半透明
		public static const PATH_BOOTH		  : int = 3;	// 路径中 3 为摆摊位
		
		public static var OFFSET_TAB_X : int;				// A*数据偏移X值
		public static var OFFSET_TAB_Y : int;				// A*数据偏移Y值
		public static var TILE_WIDTH   : int;				// A*格子宽
		public static var TILE_HEIGHT  : int;				// A*格子高
		public static var TITE_HREF_WIDTH  : int;			// A*格子一半宽
		public static var TITE_HREF_HEIGHT : int;			// A*格子一半高
		
	    private static var DELTA_X:Array =[0, 0, 1, 1, -1, 0, 1, -1, -1 , 0 , 0];
		private static var DELTA_Y:Array =[0, -1, -1, 0, -1, 0, 1, 0, 1 , 1 , 0];
		
		private var map : Array;							// 地图数据
		
		public function get Map() : Array
		{
			return this.map;
		}
		public function set Map(value : Array) : void
		{
			this.map = value;
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.map = null;
		}
		/** IDisposable End */
		
		/**
		 * 是否为障碍
		 * @param startX	始点X坐标
		 * @param startY	始点Y坐标
		 * @param endX	终点X坐标
		 * @param endY	终点Y坐标
		 * @return 0为通路 1为障碍 2 为半透明 3 为摆摊位
		 */
		public function IsBlock(startX : int, startY : int, endX : int, endY : int) : int
		{
			var mapWidth : int = this.map.length;
			var mapHeight : int = this.map[0].length;
			
			if (endX < 0 || endX == mapWidth || endY < 0 || endY == mapHeight)
			{
				return 0;
			}
			if(this.map[endX] != null &&  this.map[endX][endY] != null)
			{
				return this.map[endX][endY];		
			}
			else
			{
				return 1;
			}				
		}
		
		/** 获取目标格数据  */
		public function GetTargetTile(x:int,y:int):int
		{
			return this.IsBlock(0,0,x,y);
		}
		
		/**是否可以通过的点**/
		public function IsPassPoint(p:Point):Boolean
		{
			return IsPass(p.x,p.y);
		}
		
		/** 判断A*地图从标是否可以通过的路 true 为可以通过 */
		public function IsPass(checkX:int, checkY:int):Boolean
		{
			var mapWidth : int = this.map.length;
			var mapHeight : int = this.map[0].length;
			
			if (checkX < 0 || checkX == mapWidth || checkY < 0 || checkY == mapHeight)
			{
				return false;
			}
			return this.map[checkX][checkY] != PATH_BARRIER ? true : false;
		}
		
		/** 
		 * 判断a点到b点 2点之间的线上是否有障碍物
		 * @param startX		始点X坐标
		 * @param startY		始点Y坐标
		 * @param endX			终点X坐标
		 * @param endY			终点Y坐标
		 * @param checkDistance 检查点距离
		 * @return true为通过 false为不可通过
		 * */
		public function IsPassAToB(startX:int, startY:int, endX:int, endY:int, checkDistance:Number):Boolean
		{
			var a:Point = new Point(startX,startY);
			var b:Point = new Point(endX,endY);
			var distanceAToB:Number = Point.distance(a,b);
	    	var moveIncrement:Point = Vector2.MoveIncrement(a,b,checkDistance);
	    	var moveDistance:Number = 0;
	    	while(moveDistance < distanceAToB)
	    	{
	    		a = a.add(moveIncrement);
	    		moveDistance += checkDistance;
	    		var checkP:Point = GetTileStageToPoint(a.x, a.y);
	    		if(this.IsPass(checkP.x, checkP.y) == false)
	    		{
	    			return false;
	    		}
	    	}
	    	
			return true;
		}
		
		/** 获取指定点方法下一点A*格坐标  */
		public static function GetNextPos(x:int,y:int,dir:int):Point
		{
			var point:Point = new Point();		
			point.x = x + DELTA_X[dir];
			point.y = y + DELTA_Y[dir];
			return point;
		}
		
		/**
         * 获取45度A*单元格矩阵坐标
         * @param px    		目标点X坐标
         * @param py    		目标点Y坐标
         * @param tileWidth     单元格宽
         * @param tileHeight    单元格高
         * @return              矩阵点坐标
         */
		public static function GetTileStageToPoint(stageX:int, stageY:int):Point
        {
        	//界面坐标 计算以屏幕左上为原点的世界坐标
			var dataTempy:int = stageX  - stageY * 2;
			if(dataTempy < 0)
			{
				dataTempy -= TILE_WIDTH;
			}
			var dataTempx:int = stageY * 2 + stageX;
			
			var dataTempx1:int = (dataTempx + TITE_HREF_WIDTH) / TILE_WIDTH;
			var dataTempy1:int = (dataTempy + TITE_HREF_WIDTH) / TILE_WIDTH;
			
			//加上偏移
			return new Point(OFFSET_TAB_X + dataTempx1, OFFSET_TAB_Y + dataTempy1);
        }
        
        /**
         * 获取45度A*单元格矩阵坐标转舞台从标（获得的是格子的中心点坐标）
         * @param stageX    		舞台X坐标
         * @param stageY    		舞台Y坐标
         * @param tileWidth     单元格宽
         * @param tileHeight    单元格高
         * @return              矩阵点坐标
         */
        public static function GetTilePointToStage(px:int, py:int):Point
        {
        	var viewMouse:Point = new Point();
			
			var nOffX:int = px - OFFSET_TAB_X;		
			var nOffY:int = py - OFFSET_TAB_Y;
			viewMouse.x   = nOffX * TITE_HREF_WIDTH  + nOffY * TITE_HREF_WIDTH/*  - TITE_HREF_WIDTH*/;  	//斜坐标 x每加1   竖坐标x+1/2  y+1/2
			viewMouse.y   = nOffX * TITE_HREF_HEIGHT - nOffY * TITE_HREF_HEIGHT/* - TITE_HREF_HEIGHT*/;   //斜坐标 y每加1   竖坐标x+1/2  y-1/2
			return viewMouse;
        }
        
        /**
         * 生成地图数据
         * @param mapWidth    	地图宽
         * @param mapHeight    	地图高
         * @param tileWidth     单元格宽
         * @param tileHeight    单元格高
         * @return              地图二维数组
         */
        public static function CreateMapData(mapWidth:Number, mapHeight:Number, tileWidth:int, tileHeight:int):Array
        {		
			var arr:Array = new Array();
            var w:int     = tileWidth  / 2;
			var h:int     = tileHeight / 2;
			var col:int = mapWidth  % tileWidth  == 0 ? mapWidth  / tileWidth  : mapWidth  / tileWidth  + 1;
			var row:int = mapHeight % tileHeight == 0 ? mapHeight / tileHeight : mapHeight / tileHeight + 1;
			
			for(var i:uint = 0 ; i < col ; i++)
			{
				arr[i] = new Array();	
				for(var j:uint = 0;j < row ; j++)
				{
					arr[i][j*2]         = 0;
					arr[i][(j * 2) + 1] = 0;
				}
			}
			return arr;
        }
        
        /** 获取A*格两点的格子数距离  */
        public static function Distance(startX:int, startY:int, endX:int, endY:int):int
        {
        	var dx:int = Math.abs(startX - endX);
        	var dy:int = Math.abs(startY - endY);
        	return Math.max(dx,dy);
        }
        
        /** 通过A*格获取移动方法  */
		public static function Direction(startX:int, startY:int, endX:int, endY:int):int
		{
			if(startX < endX)
			{
				if(startY < endY)
				{
					return 6;
				}
				else if(startY > endY)
				{
					return 2;
				}
				else
				{
					return 3;
				}
			}
			else if(startX > endX)
			{
				if(startY < endY)
				{
					return 8;
				}
				else if(startY > endY)
				{
					return 4;
				}
				else
				{
					return 7;
				}
			}
			else 
			{
				if(startY < endY)
				{
					return 9;
				}
				else if(startY > endY)
				{
					return 1;
				}
			}
			return 0;
		}
	}
}