/**--------  系统消息对象      -------**/
package GameUI.Modules.SystemMessage.Memento
{
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;
	import GameUI.UIUtils;
	
	public class MessageMemento
	{
		public var timeStr	:String;
		public var timeNum	:int;
		public var content	:String;
		public var index	:int;
		public var title	:String;
		public var setSateFun:Function;			//设置状态时调用的函数
		
		private var _state	:String;
		public function MessageMemento(_title:String , _content:String)
		{
			if(_title == "0" || _title == "") title = "系统消息";
			else  title		= _title;
			timeNum 	= int(UnityUtils.timeToNum());
			if(GameCommonData.gameYear == 0)		timeStr 	= UIUtils.placeTimeToYear() + "   " + UIUtils.placeTimeToHour();
			else 	timeStr 	= UIUtils.timeToYear() + "   " + UIUtils.timeToHour();
			state 		= "未读";
			content		= _content;
		}
		public function get state():String
		{
			return _state;
		}
		public function set state(str:String):void
		{
			_state = str;
			switch(str)
			{
				case "未读":
					_state = "<font color = '#FF0000'>未读</font>";
				break;
				case "已读":
					_state = "<font color = '#00FF00'>已读</font>";
				break;
			}
			if(setSateFun != null) setSateFun();
		}

	}
}