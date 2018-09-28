package GameUI.Modules.Meridians.model
{
	public class MeridiansVO
	{
		
		public  var roleID:int;  								// 角色ID
		public  var nAmount:int; 	 							// 修炼的经脉数量
		public  var nAllLevGrade:int;							//所有经脉修炼到达的等级（到达一定等级后会有4个效果参数，配置去写)		0~90
		public  var nAllStrengthLevAdd:int; 					//所有经脉强化到达的等级(到达一定等级会有1个加成效果参数，配置去写）	0~5
		
		public  var meridiansArray:Array = new Array();	    	//经脉详细信息列表
		

	}
}