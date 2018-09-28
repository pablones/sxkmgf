package OopsFramework
{
	import flash.display.Sprite;
	
	/** 游戏逻辑组件模板 */
	public class GameComponent extends Sprite implements IGameComponent, IUpdateable, IDisposable
	{        
		private var game			   : Game;
		private var enabled			   : Boolean = false;
		private var updateOrder		   : int;
		private var enabledChanged     : Function;
		private var updateOrderChanged : Function;
		
		public var Disposed:Function;
		
		public function GameComponent(game:Game)
		{
			this.game = game;
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.enabled 		    = false;
//			this.game			    = null;
			this.enabledChanged     = null;
			this.updateOrderChanged = null;
			
			if(Disposed!=null)
			{
				Disposed();
				this.Disposed = null;
			}
		}
		/** IDisposable End */
		
		public function get Games():Game
		{
			return this.game;
		}
		
/** IGameComponent Start */
		public virtual function Initialize():void 
		{ 
			this.enabled = true;
		}
/** IGameComponent End */	
		
/** IUpdateable Start */
		public virtual function Update(gameTime:GameTime):void { }
		
		/** 是否启用游戏更新 */
		public function get Enabled():Boolean
		{
			return enabled;
		}
		public function set Enabled(value:Boolean):void
		{
			if (this.enabled != value)
			{
				this.enabled = value;
				if(enabledChanged!=null) enabledChanged(this);
			}
		}
		
		/** 更新优先级 */
		public function get UpdateOrder():int
		{
			return updateOrder;
		}
		public function set UpdateOrder(value:int):void
		{
			if (this.updateOrder != value)
			{
				this.updateOrder = value;
				if(updateOrderChanged!=null) updateOrderChanged(this);
			}
		}
		
		/** 更新开关值修改事件 */
		public function get EnabledChanged():Function
		{
			return this.enabledChanged;
		}
		public function set EnabledChanged(value:Function):void
		{
			this.enabledChanged = value
		}
		
		/** 更新优先级修改事件 */
		public function get UpdateOrderChanged():Function
		{
			return this.updateOrderChanged;
		}
		public function set UpdateOrderChanged(value:Function):void
		{
			this.updateOrderChanged = value
		}
/** IUpdateable End */
	}
}