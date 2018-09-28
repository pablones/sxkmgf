package OopsEngine.Graphics
{
	import flash.display.BitmapData;
	import flash.display.Shape;
	import flash.geom.Matrix;
	import flash.geom.Point;
	
	public class Draw
	{
		/** 
		 * 切图 
		 * @param bitmapData    源图
         * @param x          	切图起点X
         * @param y          	切图起点Y
         * @frameWidth		 	切图后宽
         * @frameHeight			切图后高
		 **/
		public static function Cut(bitmapData:BitmapData,x:int,y:int,width:Number,height:Number):BitmapData
		{
			var frame:BitmapData = new BitmapData(width, height, true, 0x00000000);
            var mx:Matrix		 = new Matrix();
            mx.tx		 		 = x;
            mx.ty 		 		 = y;
            frame.draw(bitmapData, mx);
            return frame;
		}
		
		/** 图片水平翻转 */
		public static function HorizontalTurn(bitmapData:BitmapData):BitmapData
		{
			 var frame:BitmapData = new BitmapData(bitmapData.width, bitmapData.height, true, 0x00000000);
			 frame.draw(bitmapData, new Matrix(-1,0,0,1,frame.width,0));
			 return frame;
		}
		
		/** 图片垂直翻转 */
		public static function VerticalTurn(bitmapData:BitmapData):BitmapData
		{
			 var frame:BitmapData = new BitmapData(bitmapData.width, bitmapData.height, true, 0x00000000);
			 frame.draw(bitmapData, new Matrix(1,0,0,-1,0,frame.height));
			 return frame;
		}
		
		/** 绘制一个菱形格 */
//		public static function DrawSingleTile(tileWidth:uint, tileHeight:uint, point:Point):Shape
//		{
//			var tile:Shape = new Shape();
//			tile.graphics.beginFill(0xFF0000, 0.5);
//			tile.graphics.moveTo(0, tileHeight / 2);
//            tile.graphics.lineTo(tileHeight, 0);
//            tile.graphics.lineTo(tileWidth , tileHeight / 2);
//            tile.graphics.lineTo(tileHeight, tileHeight);
//            tile.graphics.lineTo(0, tileHeight / 2); 
//			tile.graphics.endFill();
//			
//			if(point.y % 2 == 0)
//			{
//				point.y = point.y / 2;
//				tile.x  = point.x * tileWidth;
//				tile.y  = point.y * tileHeight;
//			}
//			else
//			{
//				point.y = (point.y - 1) / 2;
//				tile.x  = point.x * tileWidth  + tileWidth  / 2;
//				tile.y  = point.y * tileHeight + tileHeight / 2;
//			}
//			
//			return tile;
//		}
		
		/** 绘制45度菱形格 */
//		public static function DrawTile(tileWidth:uint, tileHeight:uint, mapWidth:Number, mapHeight:Number):Shape
//		{
//			var alpha:Number = 0.3;
//			var tile:Shape   = new Shape();
//			
//			var w:int= tileWidth  / 2;
//			var h:int= tileHeight / 2;
//			
//			var col:uint = mapWidth  % tileWidth  ==0 ? mapWidth  / tileWidth  : mapWidth  / tileWidth  + 1;
//			var row:uint = mapHeight % tileHeight ==0 ? mapHeight / tileHeight : mapHeight / tileHeight + 1;
//			
//			for(var i:uint = 0 ; i < col ; i++)
//			{
//				for(var j:uint = 0 ; j < row ; j++)											// 一次绘两个格
//				{
//					// ---偶数格---
//					tile.graphics.lineStyle(1, 0x000000, alpha);
//					tile.graphics.moveTo(i * tileWidth      , j * tileHeight + h);			// 左边
//					tile.graphics.lineTo(i * tileWidth + w  , j * tileHeight);				// 上边
//					tile.graphics.lineTo((i + 1) * tileWidth, j * tileHeight + h);			// 右边
//					tile.graphics.lineTo(i * tileWidth + w  , (j + 1) * tileHeight);		// 下边
//					tile.graphics.lineTo(i * tileWidth      ,j * tileHeight+h);				// 回到左边
//					tile.graphics.endFill();
//					
//					// ---奇数格---
//					tile.graphics.lineStyle(1, 0x000000, alpha);
//					tile.graphics.moveTo(i * tileWidth + w	    ,j * tileHeight + h + h);	// 左边
//					tile.graphics.lineTo(i * tileWidth + w + w  ,j * tileHeight + h);		// 上边
//					tile.graphics.lineTo((i + 1) * tileWidth + w,j * tileHeight + h + h);	// 右边
//					tile.graphics.lineTo(i * tileWidth + w + w  ,(j + 1) * tileHeight + h);	// 下边
//					tile.graphics.lineTo(i * tileWidth + w	    ,j * tileHeight + h + h);	// 回到左边
//					tile.graphics.endFill();
//				}
//			}
//			return tile;
//		}
	}
}