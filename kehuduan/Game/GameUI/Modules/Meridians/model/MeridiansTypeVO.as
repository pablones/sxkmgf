package GameUI.Modules.Meridians.model
{
	public class MeridiansTypeVO
	{
		public var nType:int; 	 			// 经脉类型
		public var nLev:int; 	 			// 经脉等级
		public var nLeaveTime:int; 			// 经脉当前等级剩余修炼时间
		public var nStrengthLev:int; 		// 经脉强化等级
		public var nState:int; 	   			// 经脉状态

////经脉修炼状态
//enum 
//{
//	LEARNSTATU_STOP = 0,
//	LEARNSTATU_WAIT,
//	LEARNSTATU_PROCESS,
//	LEARNSTATU_COMPELETE,
//};
		
		public var nOrderTimer:int;			// 加入队列时间
			
	}
}