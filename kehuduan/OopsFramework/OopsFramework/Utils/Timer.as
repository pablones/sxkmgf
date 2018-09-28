package OopsFramework.Utils
{
	import OopsFramework.Game;
	import OopsFramework.GameTime;
	
	/** 帧速控制组件 */
	public class Timer
	{
		private var frequency:uint = 12;						// 动画每秒帧速
		private var distanceTime:int;							// 每次触发间隔时间
		private var elapsedRealTime:int;
		private var previous : int;
		private var current  : int;
		
		/** 每秒多少次（默认12次）*/
		public function get Frequency():uint
		{
			return this.frequency;
		}
		public function set Frequency(value:uint):void
		{
			this.frequency 	  = value;
			this.distanceTime = 1 / frequency * 1000;
		}
		
		/** 每次触发间隔时间（毫秒） */
		public function get DistanceTime():int
		{
			return this.distanceTime;
		}
		public function set DistanceTime(value:int):void
		{
			this.distanceTime = value;
		}
		
		public function IsNextTime(gameTime:GameTime):Boolean
		{
//			previous = current;
//			current  = gameTime.TotalGameTime / this.distanceTime;
//			if(previous != current)
//			{
//				return true;
//			}
//			return false;
			
			if(this.elapsedRealTime >= this.distanceTime)
			{
				this.elapsedRealTime -= this.distanceTime;
				return true;
			}
			else
	        {
            	this.elapsedRealTime += gameTime.ElapsedGameTime;
            	return false;
	        }
		}
	}
}