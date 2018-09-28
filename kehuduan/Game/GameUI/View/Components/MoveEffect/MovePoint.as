package GameUI.View.Components.MoveEffect
{
	public class MovePoint
	{
		/** 最大速度 */
		protected var _maxSpeed:int = 10;
		/** 当前位置 */
		protected var _position:Vector2D;
		/** 速度 */
		protected var _velocity:Vector2D;
		
		public function MovePoint()
		{
			_position = new Vector2D();
			_velocity = new Vector2D();
		}
		
		public function update():void
		{
			_velocity.truncate(_maxSpeed);
			
			_position = _position.add(_velocity);
		}
		
		public function set maxSpeed(value:Number):void
		{
			_maxSpeed = value;
		}
		
		public function get maxSpeed():Number
		{
			return _maxSpeed;
		}
		
		public function set position(value:Vector2D):void
		{
			_position = value;
			x = _position.x;
			y = _position.y;
		}
		
		public function get position():Vector2D
		{
			return _position;
		}
		
		public function set velocity(value:Vector2D):void
		{
			_velocity = value;
		}
		
		public function get velocity():Vector2D
		{
			return _velocity;
		}
		
		public function set x(value:Number):void
		{
			_position.x = value;
		}
		
		public function set y(value:Number):void
		{
			_position.y = value;
		}
	}
}