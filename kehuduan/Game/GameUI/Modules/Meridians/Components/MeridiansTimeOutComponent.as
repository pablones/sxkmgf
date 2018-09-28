package GameUI.Modules.Meridians.Components
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.utils.Dictionary;

	public class MeridiansTimeOutComponent implements IUpdateable
	{
		private var timer:Timer = new Timer();
		private static var meridiansTimeOutComponent:MeridiansTimeOutComponent;
		private var funDic1:Dictionary = new Dictionary();
		private var funDic2:Dictionary = new Dictionary();
		private var isStart:Boolean = false;		
		
		public static function getInstance():MeridiansTimeOutComponent
		{
			if(meridiansTimeOutComponent == null)
			{
				meridiansTimeOutComponent = new MeridiansTimeOutComponent();
			}
			return meridiansTimeOutComponent;
		}
		
		public function MeridiansTimeOutComponent()
		{
			timer.DistanceTime = 1000;
		}
		
		/**
		 * 添加执行函数
		 */		
		public function addFun1(funcName:String,func:Function):void
		{
			funDic1[funcName] = func;
			if(!isStart)
			{
				this.start();
				isStart = true;
			}
		}
		
		public function addFun2(funcName:String,func:Function):void
		{
			funDic2[funcName] = func;
			if(!isStart)
			{
				this.start();
				isStart = true;
			}
		}
				
		/**
		 * 移除执行函数
		 */	
		public function removeFun1(funcName:String):void
		{
			funDic1[funcName] = null;
			delete funDic1[funcName];
			var num:int = 0;
			var name:Object;
			for(name in funDic1)
			{
				num++;
			}
			for(name in funDic2)
			{
				num++;
			}
			if(num < 1)
			{
				this.stop();
				this.isStart = false;
			}
			
		}
		
		public function removeFun2(funcName:String):void
		{
			funDic2[funcName] = null;
			delete funDic2[funcName];
			var num:int = 0;
			var name:Object;
			for(name in funDic1)
			{
				num++;
			}
			for(name in funDic2)
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
			GameCommonData.GameInstance.GameUI.Elements.Add(meridiansTimeOutComponent);
		}
		
		private function stop():void
		{
			GameCommonData.GameInstance.GameUI.Elements.Remove(meridiansTimeOutComponent);
		}
		
		public function Update( gameTime:GameTime ):void
		{
			if( timer.IsNextTime( gameTime ) )
			{
//				if ( doSomeThing != null ) doSomeThing();
				var funcName:Object;
				var func:Function;
				for( funcName in funDic1)
				{
					func = (funDic1[funcName] as Function);
					func();
				}
				for( funcName in funDic2)
				{
					func = (funDic2[funcName] as Function);
					func();
				}
			}
		}
		
//		private function updateTime():void
//		{
//			
//		}
		
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