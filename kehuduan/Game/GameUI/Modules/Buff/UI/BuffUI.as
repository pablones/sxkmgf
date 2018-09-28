package GameUI.Modules.Buff.UI
{
	public class BuffUI
	{
		public function BuffUI()
		{
		}
		public static function timeChange(time:int):String
		{
			var timeStr:String;
			var h:int = time / 60;  	//	分钟
			var f:int = time % 60;	//  秒
			var fStr:String;
			if(f < 10) 
			{
				if(f <= 0) f = 0;
				fStr = ("0" + f) as String;
			}
			else fStr = String(f);
			timeStr = String(h) + ":" + fStr;
			return timeStr;
		}
	}
}