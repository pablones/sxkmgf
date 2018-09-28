package OopsFramework
{
	public class GameTime
	{
		/** 上次Update被调用以后的时间 */
		public var ElapsedGameTime:int;
		
//        public var ElapsedRealTime:int;
        
        /** 自游戏开始时到现在的游戏总时间量 */
        public var TotalGameTime:int;
        
//        public var TotalRealTime:int;
        
        public function toString():String
        {
        	return "ElapsedGameTime:"    + ElapsedGameTime + 
//        		   "---ElapsedRealTime:" + ElapsedRealTime + 
        		   "---TotalGameTime:"   + TotalGameTime;
//        		   "---TotalRealTime:"   + TotalRealTime;      		           		   
        }
	}
}