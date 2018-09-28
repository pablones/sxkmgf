package GameUI.Modules.ChangeLine.Data
{
	public class ChgLineData
	{
		public static var isChgLine:Boolean = false;										//是否换线
		public static var isChooseLine:Boolean = false;									//是否选择了服务器
		public static var isCanChgLine:Boolean = true;									//是否可以切线
		public static var chgLineInfo:String;													//换线服务器信息
		public static var flyServerInfo:String = "";											//玩家要切线去的服务器
		public static var maxServerPerson:uint = 800;										//服务器最大人数
		public static var gsNameArr:Array;														//所有服务器名字列表
		
		
		public static const CHG_LINE_SUC:String = "CHG_LINE_SUC";						//切线成功
		public static const SHOW_CHGLINE:String = "SHOW_CHGLINE";					//显示换线界面
		public static const UPDATA_SERVER:String = "UPDATA_SERVER";				//更新服务器信息
		public static const CHG_LINE_GO:String = "CHG_LINE_GO";							//去新服
		public static const SHOW_LINE_LIST:String = "SHOW_LINE_LIST";				//显示下拉列表
		public static const ONE_KEY_HIDE:String = "ONE_KEY_HIDE";						//一键屏蔽 
		public static const GO_TO_TARGET_LINE:String = "GO_TO_TARGET_LINE";	//去一条指定的线
		
		public static const REC_UNITY_MAP_ORDER:String = "REC_UNITY_MAP_ORDER";								//收到去帮派专线消息
		
		public function ChgLineData()
		{
		}
		
		public static function getLineId(str:String):int
		{
			var id:int;
			switch(str)
			{
				case "一线":
					id = 1;
					break;
				case "二线":
					id = 2;
					break;
				case "三线":
					id = 3;
					break;
				case "四线":
					id = 4;
					break;
				case "五线":
					id = 5;
					break;
				case "六线":
					id = 6;
					break;
				case "七线":
					id = 7;
					break;
				case "八线":
					id = 8;
					break;
				case "九线":
					id = 9;
					break;
				default:
					id = 100;
					break;
			}
			return id;
		}

	}
}