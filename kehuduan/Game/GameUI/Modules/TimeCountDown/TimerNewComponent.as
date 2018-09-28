package GameUI.Modules.TimeCountDown
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.utils.getTimer;

	public class TimerNewComponent implements IUpdateable
	{
		private static var _instance:TimerNewComponent;
		public static var isInstanceInited:Boolean;
		private static var distanceTime:int;
		public static var updateTag:int;
		private var differTime:int;
		
		public function TimerNewComponent()
		{
			isInstanceInited = true;
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		public static function getInstance(date:Date = null):TimerNewComponent
		{
			if(!_instance)
			{
				_instance = new TimerNewComponent();
				distanceTime = getTimer();
				GameCommonData.nowDate.time = date.time - 1500;
			}
			return _instance;
		}
		
		public function Update(gameTime:GameTime):void
		{
			differTime = getTimer() - distanceTime;
			distanceTime = getTimer();
			GameCommonData.nowDate.time += differTime;
			/* if( differTime>= 100)
			{
				distanceTime = getTimer();
				GameCommonData.nowDate.time += differTime;
			} */
		
		}
 		/**
 		 *每过半个小时，将GameCommonData.nowDate置为服务器时间
 		 * @param de 服务器时间
 		 * 
 		 */		
 		public function modifyDate(de:Date):void
		{
			GameCommonData.nowDate.time = de.time - 1500;	
		} 

		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
        public function get Enabled():Boolean
		{
			return true;
		}
		
	}
}