package GameUI.View.Components.MoveEffect
{
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.utils.Dictionary;
	import flash.utils.Timer;

	public class CDUpdateComponent extends EventDispatcher
	{
		private static var instanceContainer:Dictionary = new Dictionary(); 
		private static var instanceNumber:int;
		public var _name:String; 
		private var _runFun:Function;
		private var isRunning:Boolean;
		private var timer:Timer; 
		public function CDUpdateComponent(cdSingle:CdSingleClass,name:String,cdDistanceTime:int)
		{
			if(!cdSingle)
			{
				throw new Error("请通过getInstance（）方法获取实例");
				return;
			}
			this._name = name;
			timer = new Timer(cdDistanceTime);
			timer.addEventListener(TimerEvent.TIMER,onTimer);
		}
		
		public static function getInstance(cdDistanceTime:int,cdType:int):CDUpdateComponent
		{
			instanceNumber ++;
			var name:String = cdType +"_"+ instanceNumber.toString();
			if(!instanceContainer[name])
			{
				instanceContainer[name] = new CDUpdateComponent(new CdSingleClass(),name,cdDistanceTime);
			} 
			return instanceContainer[name];
		}
		
		
		public function onTimer(te:TimerEvent):void
		{
			if(!isRunning)
			{
				this.dipoise();
				return;
			}
			if(_runFun != null)
			{
				_runFun(this._name);
			}
		}
		public function start(runFun:Function = null):void
		{
			if(runFun != null)
			{
				this._runFun = runFun;
			}
			this.isRunning = true;
			timer.start();
		}
		public function stop():void
		{
			if(timer)
			{
				timer.stop();
			}
		}
		
		public function get isCompRunning():Boolean
		{
			return this.isRunning;
		}
		public function set isCompRunning(boo:Boolean):void
		{
			this.isRunning = boo;
		}
		public function dipoise():void
		{
			isRunning = false;
			if(!timer)
				return;
			this.stop();
			timer.stop();
			_runFun = null;
			timer.removeEventListener(TimerEvent.TIMER,onTimer);
			timer = null;
			delete instanceContainer[_name];
			this.dispatchEvent(new Event("disposeCd"));
			var type:String = _name.split("_")[0];
			this.disposeSameTypeInstance(type);
		}
		private function disposeSameTypeInstance(type:String):void
		{
			for(var typeStr:String in instanceContainer)
			{
				if(type == typeStr.split("_")[0])
				{
//					var instance:CDUpdateComponent2 = (instanceContainer[typeStr] as CDUpdateComponent2);
//					if(!isRunning)
//					{
						(instanceContainer[typeStr] as CDUpdateComponent).dipoise();
//					}
				}
			}
		}
	}
}

class CdSingleClass{}