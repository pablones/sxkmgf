package GameUI.View.Components.MoveEffect
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.system.System;
	import flash.utils.Dictionary;

	/** 周期组件 */
	public class CycleComponent implements IUpdateable
	{
		private var timer:Timer = new Timer();
		private static var CycleComponents:Dictionary = new Dictionary();
		private var funDic:Dictionary = new Dictionary();
		private var isStart:Boolean = false;		
		
		public static function getInstance( distanceTime:int ):CycleComponent
		{
			if(CycleComponents[distanceTime] == null)
			{
				CycleComponents[distanceTime] = new CycleComponent(distanceTime);
			}
			return CycleComponents[distanceTime];
		}
		
		public function CycleComponent( distanceTime:int )
		{
			timer.DistanceTime = distanceTime;
		}
		
		/**
		 * 添加执行函数
		 */		
		public function addFun(funcName:String,func:Function):void
		{
			funDic[funcName] = func;
			if(!isStart)
			{
				this.start();
				isStart = true;
			}
		}
		
		/**
		 * 移除执行函数
		 */	
		public function removeFun(funcName:String):void
		{
			funDic[funcName] = null;
			delete funDic[funcName];
			var num:int = 0;
			var name:Object;
			for(name in funDic)
			{
				num++;
			}
			if(num < 1)
			{
				this.stop();
				this.isStart = false;
			}
			
		}
		
		//开始执行
		private function start():void
		{
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		private function stop():void
		{
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);
		}
		
		public function Update( gameTime:GameTime ):void
		{
			if( timer.IsNextTime( gameTime ) )
			{
//				if ( doSomeThing != null ) doSomeThing();
				var funcName:Object;
				var func:Function;
				for( funcName in funDic)
				{
					func = (funDic[funcName] as Function);
					func();
				}
			}
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