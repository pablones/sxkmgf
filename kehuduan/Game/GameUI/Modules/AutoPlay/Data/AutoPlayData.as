package GameUI.Modules.AutoPlay.Data
{
	import flash.utils.Timer;
	
	public class AutoPlayData
	{
		public function AutoPlayData()
		{
		}
		public static const XIAOYAODANTYPE:int = 630014;							/** 逍遥丹的物品type*/
		
		public static var offLineExp:int = 100;										/** 离线经验 */	
//		public static var offLineNeedMoney:int = 1;									/** 离线挂机2倍经验所需的碎银*/
		public static var offLineCurrentType:int = 0;								/** 当前离线经验类型 */
		public static var offLineTime:int = 0;										/** 离线经验 */
		public static var offLineMoney:int = 0;										/** 离线所需碎银 */
		public static var offLineIsOpen:Boolean = false;							/** 离线挂机面板是否打开 */
		public static var dataIsSendOver:Boolean = false;							/** 离线数据是否传送成功 */
		
		public static var aSaveTick:Array = [1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0];											//默认是否打勾
		public static var aSaveNum:Array = [60,40,70,25,40,60,65,7,0,0,0];												//默认限制数
		public static var aSaveType:Array = [0,0,0,0,0,0,0,0,0,0,0];																	//物品type
		
		public static var autoTimer:Timer = new Timer( 4000 );																	//挂机定时器
	}
}