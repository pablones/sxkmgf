/**
 * 实体元件抽象类 不继承 Sprite 而继承 DrawableGameComponent是因为有可能有的元件有
 * 实时更新重绘操作、加载资源操作的可能。
 */
package OopsEngine.Entity
{
	import OopsFramework.DrawableGameComponent;
	import OopsFramework.Game;

	public class EntityElement extends DrawableGameComponent
	{		
		/** 是否触发父对象鼠标事件 (默认不触发) */
//		public var ParentMouseEnabled:Boolean = false;
		
		public function EntityElement(game:Game)
		{
			super(game);
			
			this.mouseEnabled  = false;
			this.mouseChildren = false;
		}
		   
//		protected virtual function onMouseDown(e:MouseEvent):void
//		{
//			
//		}
//		
//		protected virtual function onMouseUp(e:MouseEvent):void
//		{
//			
//		}
//		
//		protected virtual function onMouseOver(e:MouseEvent):void
//		{
//			if(!ParentMouseEnabled) this.Games.MouseEnabled = false;
//		}
//		
//		protected virtual function onMouseOut(e:MouseEvent):void
//		{
//			if(!ParentMouseEnabled) this.Games.MouseEnabled = true;
//		}
		
		/** Property Start */
		public virtual function get X():Number
		{
			return this.x;
		}
		public virtual function set X(value:Number):void
		{
			this.x = value;
		}
		
		public virtual function get Y():Number
		{
			return this.y;
		}
		public virtual function set Y(value:Number):void
		{
			this.y = value;
		}
		
		public function get Rotation():Number
		{
			return this.rotation;
		}
		public function set Rotation(value:Number):void
		{
			this.rotation = value;
		}
		
//		public function get MouseEnabled():Boolean
//		{
//			return this.mouseEnabled;
//		}
//		public function set MouseEnabled(value:Boolean):void
//		{
//			this.mouseEnabled = value;
//			if(this.mouseEnabled)
//			{
//				this.addEventListener(MouseEvent.MOUSE_DOWN,onMouseDown)
//				this.addEventListener(MouseEvent.MOUSE_UP  ,onMouseUp)
//				this.addEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
//				this.addEventListener(MouseEvent.MOUSE_OUT ,onMouseOut);
//			}
//			else
//			{
//				this.removeEventListener(MouseEvent.MOUSE_DOWN,onMouseDown)
//				this.removeEventListener(MouseEvent.MOUSE_UP  ,onMouseUp)
//				this.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
//				this.removeEventListener(MouseEvent.MOUSE_OUT ,onMouseOut);
//			}
//		}
		/** Property End */
	}
}