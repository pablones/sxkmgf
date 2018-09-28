package OopsEngine.Utils
{
	import flash.geom.Point;
	
	public class Vector2
	{
	    /**
		 * 移动增量计算
		 * @param startPoint 起点
		 * @param endPoint   终点
		 * @param stepLength 每步步长
		 */	
		public static function MoveIncrement(startPoint:Point,endPoint:Point,stepLength:Number):Point
		{
			var seDistance:Number	   = Point.distance(startPoint,endPoint);	// startPoint 到 endPoint 的距离 这里的距离在一定几率下可以算到小数 那么除以他的书就可能是无穷大了
			var scaleStepLength:Number = stepLength / seDistance;               
			var x:Number 			   = (endPoint.x - startPoint.x) * scaleStepLength;
			var y:Number			   = (endPoint.y - startPoint.y) * scaleStepLength;
			
			return new Point(x,y);
		}
		
		/**
		 * 移动A*格距离计算
		 * @param startPoint 起点
		 * @param endPoint   终点
		 * @param stepLength 移动长度
		 */	
		public static function MoveDistance(startPoint:Point,endPoint:Point,stepLength:int):Point
		{
			var seDistance:Number	   = Point.distance(startPoint,endPoint);	// startPoint 到 endPoint 的距离
			var scaleStepLength:Number = stepLength / seDistance;
			var x:int 			       = int(startPoint.x + (endPoint.x - startPoint.x) * scaleStepLength);
			var y:int			       = int(startPoint.y + (endPoint.y - startPoint.y) * scaleStepLength);		
			return new Point(x,y);
		}
		
		/**
		 * 通过正切值获取向量朝向代号（方向代码和小键盘的数字布局一样-8：上、４：左、６：右、２：下等）
		 * @param targetX  目标点的X值
		 * @param targetY  目标点的Y值
		 * @param currentX 当前点的X值
		 * @param currentY 当前点的Y值
		 */	
		public static function DirectionByTan(currentX:Number, currentY:Number, targetX:Number, targetY:Number):int
        {
            var tan:Number = (targetY - currentY) / (targetX - currentX);
            if (Math.abs(tan) >= Math.tan(Math.PI * 3 / 8) && targetY <= currentY)
            {
                return 8;
            }
            else if (Math.abs(tan) > Math.tan(Math.PI / 8) && Math.abs(tan) < Math.tan(Math.PI * 3 / 8) && targetX > currentX && targetY < currentY)
            {
                return 9;
            }
            else if (Math.abs(tan) <= Math.tan(Math.PI / 8) && targetX >= currentX)
            {
                return 6;
            }
            else if (Math.abs(tan) > Math.tan(Math.PI / 8) && Math.abs(tan) < Math.tan(Math.PI * 3 / 8) && targetX > currentX && targetY > currentY)
            {
                return 3;
            }
            else if (Math.abs(tan) >= Math.tan(Math.PI * 3 / 8) && targetY >= currentY)
            {
                return 2;
            }
            else if (Math.abs(tan) > Math.tan(Math.PI / 8) && Math.abs(tan) < Math.tan(Math.PI * 3 / 8) && targetX < currentX && targetY > currentY)
            {
                return 1;
            }
            else if (Math.abs(tan) <= Math.tan(Math.PI / 8) && targetX <= currentX)
            {
                return 4;
            }
            else if (Math.abs(tan) > Math.tan(Math.PI / 8) && Math.abs(tan) < Math.tan(Math.PI * 3 / 8) && targetX < currentX && targetY < currentY)
            {
                return 7;
            }
            else
            {
                return 4;
            }
        }
	}
}