package GameUI.View.Components.MoveEffect
{
	import flash.display.DisplayObject;
	import flash.display.Shape;
	
	/** 处理把几个组件绑在一起移动效果 */
	public class VirtualContainer extends Shape
	{
		private var _x:Number;
		private var _y:Number;
		private var displayObjects:Array;
		
		public function VirtualContainer(x:Number = 0, y:Number = 0)
		{
			_x = x;
			_y = y;
			displayObjects = new Array();
		}

		public function add(displayObject:DisplayObject):void
		{
			displayObjects.push( displayObject );
		}
		
		public function removeAll():void
		{
			var length:uint = displayObjects.length;
			while(length-- > 0)
			{
				displayObjects.pop();
			}
		}
		
		public function setInitx(value:Number):void
		{
			_x = value;
		}
		
		public function setInity(value:Number):void
		{
			_y = value;
		}
		
		override public function get y():Number
		{
			return _y;
		}

		override public function set y(value:Number):void
		{
			if(_y != value)
			{
				for each(var diso:DisplayObject in displayObjects)
				{
					diso.y += value - _y;
				}
				_y = value;
			}
		}

		override public function get x():Number
		{
			return _x;
		}

		override public function set x(value:Number):void
		{
			if(_x != value)
			{
				for each(var diso:DisplayObject in displayObjects)
				{
					diso.x += value - _x;
				}
				_x = value;
			}
		}
	}
}