package GameUI.Modules.Arena.Data
{
	import GameUI.View.UIKit.data.ArrayList;
	
	public class ArenaScore
	{
		// 初始化标记
		public static var initialized:Boolean = false;
		
		// 各阵营分数
		public static var camp1Score:uint;
		public static var camp2Score:uint;
		public static var camp3Score:uint;
		
		// 我的分数
		public static var myCamp:uint;
		public static var myScore:uint;
		public static var myAwardScore:uint;
		public static var myKill:uint;
		public static var myRank:uint;
		public static var myLevel:uint;
		public static var myVIPLevel:uint;
		public static var myPro:uint;
		public static var myName:String;
		
		// 总表
		public static var listFull:ArrayList = new ArrayList();
		
		public static function clear():void
		{
			camp1Score = camp2Score = camp3Score = 0;
			myCamp = myScore = myAwardScore = myKill = myRank = myLevel = myVIPLevel = myPro = 0;
			myName = "";
		}
	}
}