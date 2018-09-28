package GameUI.Modules.OnlineGetReward.Data
{
	import flash.utils.Dictionary;
	
	public class OnLineAwardData
	{
		public static const MOVE_AWARD:String = "move_award";
		public static const GET_AWARD_POINT:String = "get_award_point";
		public static const ONLINE_TIMEOUT_PANEL:String = "OnLineTimeOut";
		public static const ONLINE_GAINWARD_PANENL:String = "OnLineAwardPanel";
		public static const NEXT_ONLINE_GIFT:String = "NEXT_ONLINE_GIFT";
		public static const SHOW_ONLINE_TIMEOUT:String = "SHOW_ONLINE_TIMEOUT";
		public static const CLOSE_GAINAWARD_PAN:String = "CLOSE_GAINAWARD_PAN";
		public static var canGain:Boolean = false;
		
		public static var items:Array;
		public static var giftIndex:uint = 0;
		public static var testTimeArr:Array = [10,11,9,8,7];
		public static var awardTimeArr:Array = [30,120,180,300,600,900,1200,1800];				//奖励时间
//		public static var awardTimeArr:Array = [2,2,2,3,3,3,3,3];				//奖励时间
		public static var giftArr:Array = [  [ {type:300001,num:5},{type:310001,num:5},{type:150301,num:1} ],
															 [ {type:501015,num:1},{type:210301,num:1},{type:220301,num:1} ],
															 [ {type:501016,num:1},{type:310001,num:5},{type:160301,num:1} ],
															 [ {type:501017,num:1},{type:300002,num:10},{type:180301,num:1} ],
															 [ {type:501018,num:1},{type:301011,num:1},{type:311011,num:1} ],
															 [ {type:501019,num:1},{type:501003,num:1},{type:321002,num:1} ],
															 [ {type:501020,num:1},{type:501000,num:1},{type:630006,num:10} ],
															 [ {type:501021,num:1},{type:630000,num:5},{type:630014,num:2} ]
															];
															
		public static function getTimeStr(time:uint):String
		{
			var hour:int = int(time) / 3600;
			var minite:int = int(int(time) % 3600) / 60;
			var second:int = int(time) % 60;
			var hourStr:String;
			var miniteStr:String;
			var secondStr:String;
			if ( hour<10 )
			{
				hourStr="0"+hour.toString() 
			}
			else
			{
				 hourStr=hour.toString();
			}
			if ( minite<10 )
			{
				miniteStr="0"+minite.toString()
			}
			else
			{
				miniteStr=minite.toString();
			}
			if ( second<10 )
			{
				secondStr="0"+second.toString()
			}
			else
			{
				secondStr=second.toString();
			}
			return hourStr + ":" + miniteStr + ":" + secondStr;
		}
	}
}