package GameUI.Modules.TimeCountDown
{
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.UICore.UIFacade;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.text.TextField;
	
	public class TimeUIUtils implements IUpdateable
	{
		public static var timeUIUtils:TimeUIUtils;
		private var _txtTime:TextField;
		private var timer:Timer = new Timer();		/** 引擎的定时器 */
		private var enabled:Boolean = true;
		
		public function TimeUIUtils(txtTime:TextField)
		{
			_txtTime 	= txtTime;
			timer.DistanceTime = 1000 * 10;		//循环时间为1分钟
			GameCommonData.GameInstance.GameUI.Elements.Add(this);			//第一次才会添加心跳
		}
		public static function getInstance(txtTime:TextField):TimeUIUtils
		{
			if(timeUIUtils == null) timeUIUtils = new TimeUIUtils(txtTime);
			return timeUIUtils;
		}
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		
		public function get Enabled():Boolean
		{
			return enabled;
		}
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				GameCommonData.gameSecond += 10;
				showText([GameCommonData.gameYear , GameCommonData.gameMonth , GameCommonData.gameDay , 
							GameCommonData.gameHour , GameCommonData.gameMinute , GameCommonData.gameSecond]);
			}
		}
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
		/** 显示时间文本(小时，分钟) */
		public function showText(timeList:Array):void
		{
			GameCommonData.gameYear     = timeList[0];
		 	GameCommonData.gameMonth	= timeList[1];
		 	GameCommonData.gameDay		= timeList[2];
			GameCommonData.gameHour 	= timeList[3];
			GameCommonData.gameMinute 	= timeList[4];
			GameCommonData.gameSecond 	= timeList[5];
			if(GameCommonData.gameSecond >= 60)										//分钟过渡
			{
				GameCommonData.gameSecond = int(GameCommonData.gameSecond % 60);
				GameCommonData.gameMinute ++;
			}
			if(GameCommonData.gameMinute >= 60)										//小时过渡
			{
				GameCommonData.gameMinute = int(GameCommonData.gameMinute % 60);
				GameCommonData.gameHour ++;
			} 
			if(GameCommonData.gameHour >= 24)										//一天过渡
			{
				GameCommonData.gameHour = int(GameCommonData.gameHour % 24);
			}
			var gameHour:String = GameCommonData.gameHour >= 10 ? String(GameCommonData.gameHour):"0" + String(GameCommonData.gameHour);
			var timeTxt:String = (GameCommonData.gameMinute >= 10) ? String(gameHour + ":" + GameCommonData.gameMinute) : String(gameHour + ":0" + GameCommonData.gameMinute); 
			_txtTime.text = timeTxt;
			checkToNotice(timeTxt);	//检测游戏公告
		}
		/** 删除心跳 */
		public function deleteTimer():void
		{
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);		//全部倒计时删除才会删除心跳
		}
		
		/** 检测游戏公告 */
		private function checkToNotice(time:String):void
		{
			if(ChatData.noticeTimer.running) return;
			if(ChatData.GAME_SCROLL_NOTICE_DIC[time]) {
				ChatData.noticeTimer.reset();
				ChatData.noticeTimer.start();
				UIFacade.GetInstance(UIFacade.FACADEKEY).playGameNotice(time);
			}
		}
		
	}
}
