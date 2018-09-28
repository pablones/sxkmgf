package GameUI.Modules.Rank.Proxy
{
	import flash.utils.ByteArray;
	public class RankNetAction
	{
		public static var rankObj:Object;                                           //最外层得对象，包括了所有的数据
		private static var obj:Object;                                              //内层对象，包括每一页得所有数据
		private static const COUNT:int = 20;                                        //每一页得行数
		private static var bytes:ByteArray;                                         //数据数组
		private static var list:Array;                                              //装有一页数据的数组
	
		/** 发送查询排行榜请求 */
		public static function sendRankOrder(rankIndex:int,currentpage:int):void    //参数1，某个排行。参数2，某一页。
		{
			rankObj        = new Object();
			obj            = new Object();                                          //内层对象，包括每一页得所有数据
			//bytes          = new ByteArray();                                     //创建数据数组
			obj.data         =new Array();
			obj.data.push(rankIndex);                                               //具体参数根据服务器的协议来确定
			obj.data.push(currentpage);
			Ranksend.createMsRank(rankObj);                                         //这个类未建立
			                                            
			
			
		   
			/** if(RankObj.rankindex[index] == undefined)                          //如果第rankIndex排行榜得第index页为定义
			 {
			 	//去服务器取数据
			 }
			 else
			 {
			 	//在本地缓存中调用
			 }
			 
			 
			 obj.type = 5;
			 var data:Array = new Array();
			 obj.param = data;
			 data.push(0);
			 data.push("");
			 data.push("");
			 data.push(0);
			 */
			 
		} 
		
	}
}