package GameUI.Modules.SmallWindow.Mediator
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Sprite;

	public class FixedTime implements IUpdateable
	{
		private var obj:*;
		private var timer:Timer;
		private var _distanceTime:int;
		private var _onTimeFun:Function;
		
		public function FixedTime( time:uint, onTimeFun:Function )
		{
			timer = new Timer();
			_distanceTime = time;
			timer.DistanceTime = _distanceTime;
			_onTimeFun = onTimeFun;
			GameCommonData.GameInstance.GameUI.Elements.Add( this );
		}

		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				_onTimeFun();
				GameCommonData.GameInstance.GameUI.Elements.Remove( this );
			}
		}
		
		public function reset():void
		{
			GameCommonData.GameInstance.GameUI.Elements.Remove( this );
			timer = new Timer();
			timer.DistanceTime = _distanceTime;
			GameCommonData.GameInstance.GameUI.Elements.Add( this );
		}
		
		public function stop():void
		{
			GameCommonData.GameInstance.GameUI.Elements.Remove( this );
		}
		
		public function get Enabled():Boolean
		{
			return true;
		}
		
		public function get UpdateOrder():int
		{
			return 0;
		}
		
		public function get EnabledChanged():Function
		{
			return null;
		}
		
		public function set EnabledChanged(value:Function):void
		{
		}
		
		public function get UpdateOrderChanged():Function
		{
			return null;
		}
		
		public function set UpdateOrderChanged(value:Function):void
		{
		}
		
	}
}