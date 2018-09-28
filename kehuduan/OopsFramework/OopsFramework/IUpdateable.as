package OopsFramework
{
	public interface IUpdateable
	{
        /**
         * 更新动作。
         */		
        function Update(gameTime:GameTime):void;
        
		/**
		 * 是否启动更新。
		 */        
		function get Enabled():Boolean;
		
		/**
		 * 更新优先级（数值小的优先更新）。
		 */		
		function get UpdateOrder():int;
		
		/**
		 * 更新开关值修改事件。
		 */		
		function get EnabledChanged():Function;
		
		function set EnabledChanged(value:Function):void;
		
        /**
         * 更新优先级修改事件。
         */				
        function get UpdateOrderChanged():Function;
         
        function set UpdateOrderChanged(value:Function):void;
	}
}