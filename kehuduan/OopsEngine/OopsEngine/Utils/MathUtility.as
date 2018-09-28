package OopsEngine.Utils
{
	import flash.geom.Point;
	
	public class MathUtility
	{
		//计算两点间的弧度
		public static function Radian(p1:Point,p2:Point):Number
		{
			var dx:Number        = p1.x - p2.x;
			var dy:Number        = p1.y - p2.y;
			
			var radians:Number   = Math.atan2(dy, dx);
			return radians * 180 / Math.PI;
		}	
		
		public static function RadianCutLeft(p1:Point,p2:Point):Number
		{
			var dx:Number        = p1.x - p2.x;		// x
			var dy:Number        = p1.y - p2.y;		// y 
			var radians:Number   = Math.atan2(dy, dx);
			return radians * 180 / Math.PI;
		}	
		
		public static function RadianCutRight(p1:Point,p2:Point):Number
		{
			var dx:Number        = p1.x - p2.x;		// x
			var dy:Number        = p2.y - p1.y;		// y
			var radians:Number   = Math.atan2(dy, dx);
			return radians * 180 / Math.PI;
		}
		
		public static function Floor(nNumber:Number,nRoundToInterval:Number = 1):Number 
		{
			return Math.floor(nNumber / nRoundToInterval) * nRoundToInterval;
		}
	}
}