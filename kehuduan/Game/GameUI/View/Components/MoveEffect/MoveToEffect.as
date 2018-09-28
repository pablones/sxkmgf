package GameUI.View.Components.MoveEffect
{
	import flash.display.DisplayObject;
	
	public class MoveToEffect extends MovePoint
	{
		/** 目标点 */
		protected var _targetPoint:Vector2D;
		/** 操作的显示对象 */
		protected var _moveObject:DisplayObject;
		/** 距离小于该值时减速 */
		protected var _range:Number;
		/** 唯一标识 */
		private var _identifying:String;
		/** 周期 */
		private var _cycle:int;
		/** 运动结束后执行的方法 */
		private var _finishFunc:Function;
		
		/**
		 * 
		 * @param displayObject 	操作对象
		 * @param targetPoint 		目标位置
		 * @param maxSpeed 			最大速度
		 * @param range 			减速距离
		 * @cycle range 			移动周期
		 */		
		public function MoveToEffect( displayObject:DisplayObject, targetPoint:Vector2D , maxSpeed:Number = 10,range:Number = 20 ,cycle:int = 15 ,finishFunc:Function = null)
		{
			super();
			moveObject = displayObject;
			_targetPoint = targetPoint;
			_maxSpeed = maxSpeed;
			_range = range;
			_cycle = cycle;
			_finishFunc = finishFunc;
			
			_identifying = "update_" + _cycle + "_" + displayObject.x + "_" + displayObject.y + "_" + targetPoint.toString();
			CycleComponent.getInstance( _cycle ).addFun( _identifying ,update );
		}
		
		override public function update():void
		{
			/** 运动方向上的单位速度 */
			var dist:Vector2D = _targetPoint.subtract( _position );
			
			if( dist.length < 2 )
			{
				_moveObject.x = _targetPoint.x;
				_moveObject.y = _targetPoint.y;
				CycleComponent.getInstance( _cycle ).removeFun( _identifying );
				
				if(_finishFunc != null)
				{
					_finishFunc();
					_finishFunc = null;
				}
				return;
			}
			
			var OneVel:Vector2D = dist.clone().normalize();
			var distance:Number = _targetPoint.clone().subtract( position ).length;
			if( distance > _range )
			{
				_velocity = OneVel.multiply( _maxSpeed );
			}else
			{
				_velocity = OneVel.multiply( _maxSpeed * ( dist.length / _range ) );
			}
			super.update();
			_moveObject.x = int(position.x);
			_moveObject.y = int(position.y);
		}
		
		public function set moveObject(displayObject:DisplayObject):void
		{
			_moveObject = displayObject;
			position =  new Vector2D(_moveObject.x,_moveObject.y)
		}
	
		public function get moveObject():DisplayObject
		{
			return _moveObject;
		}
		
		public function set finishFunc( finishFunc:Function ):void
		{
			_finishFunc = finishFunc;
		}
	}
}