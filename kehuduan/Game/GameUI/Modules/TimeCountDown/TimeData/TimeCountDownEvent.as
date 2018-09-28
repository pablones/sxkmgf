package GameUI.Modules.TimeCountDown.TimeData
{
	public class TimeCountDownEvent
	{
		public function TimeCountDownEvent()
		{
		}
		public static const SHOWTIMECOUNTDOWN:String = "SHOWTIMECOUNTDOWN";					/** 打开倒计时面板 */
		public static const CLOSETIMECOUNTDOWN:String = "CLOSETIMECOUNTDOWN";				/** 手动删除倒计时面板 */
		public static const CLOSEWORKCOUNTDOWN:String = "CLOSEWORKCOUNTDOWN";				/** 手动删除打工倒计时面板 */
	}
}