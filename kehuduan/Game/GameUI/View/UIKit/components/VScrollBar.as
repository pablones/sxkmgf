package GameUI.View.UIKit.components 
{
	import GameUI.View.UIKit.core.UIComponent;
	import GameUI.View.UIKit.events.UIEvent;
	import GameUI.View.UIKit.layout.BasicLayout;
	
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Rectangle;
	import flash.utils.Timer;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	
	[Event(name="valueCommit", type="statm.dev.events.UIEvent")]
	
	/**
	 * ...
	 * @author statm
	 */
	public class VScrollBar extends UIComponent
	{
		
		public function VScrollBar() 
		{
			super();
			
			this.layout = new BasicLayout(this);
			longActionTimer.addEventListener(TimerEvent.TIMER, longActionTimer_timerHandler);
		}
		
		protected var initialized:Boolean = false;
		
		
		// ----------------------------------------
		//
		//		部件拉取 & 事件注册
		//
		// ----------------------------------------
		
		public var upButton:SimpleButton;
		
		public var downButton:SimpleButton;
		
		public var track:Sprite;
		
		public var thumb:Sprite;
		
		public var grip:Sprite;
		
		override public function loadSkinParts():void 
		{
			// TODO(zhao): 测试完了要删掉
//			upButton = new UpArrow();
//			downButton = new DownArrow();
//			track = new BarBackground();
//			thumb = new Thumb();
//			grip = new Grip();
//			
//			this.addChild(track);
//			this.addChild(upButton);
//			this.addChild(downButton);
//			this.addChild(thumb);
//			this.addChild(grip);
//			
//			grip.mouseEnabled = false;
//			
//			registerEventListeners();
		}
		
		protected function registerEventListeners():void
		{
			if (upButton)
			{
				upButton.addEventListener(MouseEvent.MOUSE_DOWN, arrowButton_mouseDownHandler);
			}
			
			if (downButton)
			{
				downButton.addEventListener(MouseEvent.MOUSE_DOWN, arrowButton_mouseDownHandler);
			}
			
			thumb.addEventListener(MouseEvent.MOUSE_DOWN, thumb_mouseDownHandler);
			
			track.addEventListener(MouseEvent.MOUSE_DOWN, track_mouseDownHandler);
		}
		
		
		// ----------------------------------------
		//
		//		布局
		//
		// ----------------------------------------
		
		protected var layoutInvalidated:Boolean = false;
		
		override public function invalidateLayout():void 
		{
			if (layoutInvalidated) return;
			
			layoutInvalidated = true;
			
			this.addEventListener(Event.ENTER_FRAME, _doLayout);
			
			layout.invalidateLayout(true);
		}
		
		protected function _doLayout(event:Event):void
		{
			doLayout();
			
			this.removeEventListener(Event.ENTER_FRAME, _doLayout);
			layoutInvalidated = false;
		}
		
		protected function doLayout():void
		{
			if (!initialized)
			{
				var maxWidth:Number = Math.max((track ? track.width : 0),
										   (thumb ? thumb.width : 0),
										   (upButton ? upButton.width : 0),
										   (downButton ? downButton.width : 0),
										   (grip ? grip.width : 0));
				
				if (track)
				{
					track.x = (maxWidth - track.width) / 2;
					track.height = this.height;
				}
				
				if (upButton)
				{
					upButton.x = (maxWidth - upButton.width) / 2;
					upButton.y = 0;
				}
				
				if (downButton)
				{
					downButton.x = (maxWidth - downButton.width) / 2;
					downButton.y = this.height - downButton.height /*- 1*/;
				}
				
				if (thumb)
				{
					thumb.x = (maxWidth - thumb.width) / 2;
					thumb.y = (upButton ? upButton.height : 0);
					
					if (_maximum - _minimum > 0)
					{
						thumb.height = Math.max(35,
												pageSize / (_maximum - _minimum + pageSize) * (this.height - (upButton ? upButton.height : 0) - (downButton ? downButton.height : 0)));
					}
					
					if (grip)
					{
						grip.x = (maxWidth - grip.width) / 2;
						grip.y = (thumb.height - grip.height) / 2 + thumb.y;
					}
				}
				
				initialized = true;
			}
			else
			{
				// 只处理 height 和 value 的变化
				if (heightChanged)
				{	
					if (track)
					{
						track.height = this.height;
					}
					
					if (downButton)
					{
						downButton.y = this.height - downButton.height /*- 1*/;
					}
					
					thumb.height = Math.max(35,
											pageSize / (_maximum - _minimum + pageSize) * (this.height - (upButton ? upButton.height : 0) - (downButton ? downButton.height : 0)));
					
					
					adjustThumbPositionByValue();
					heightChanged = false;
				}
				if (valueChanged)
				{
					adjustThumbPositionByValue();
					valueChanged = false;
				}
			}
		}
		
		
		// ----------------------------------------
		//
		//		尺寸
		//
		// ----------------------------------------
		
		override public function get width():Number 
		{
			return 16;
		}
		
		override public function set width(value:Number):void 
		{
		}
		
		protected var heightChanged:Boolean = false;
		
		override public function set height(value:Number):void 
		{
			super.height = value;
			
			heightChanged = true;
			invalidateLayout();
		}
		
		
		
		
		// ----------------------------------------
		//
		//		数值
		//
		// ----------------------------------------
		
		protected var _minimum:Number = 0;
		
		public function get minimum():Number 
		{
			return _minimum;
		}
		
		public function set minimum(value:Number):void 
		{
			_minimum = value;
		}
		
		protected var _maximum:Number = 0;
		
		public function get maximum():Number 
		{
			return _maximum;
		}
		
		public function set maximum(value:Number):void 
		{
			if (_maximum == value) return;
			
			_maximum = value;
			heightChanged = true;
			invalidateLayout();
		}
		
		protected var _value:Number = 0;
		
		public function get value():Number
		{
			return _value;
		}
		
		protected var valueChanged:Boolean = false;
		
		// zhao: 注意 _value 没有 setter。使用 setValue() 函数修改滚动条的值。
		public function setValue(value:Number, dispatchEvent:Boolean, redraw:Boolean = true):void
		{
			if (_value == value) return;
			
			_value = value;
			
			if (dispatchEvent)
			{
				this.dispatchEvent(new UIEvent(UIEvent.VALUE_COMMIT));
			}
			if (redraw)
			{
				valueChanged = true;
				invalidateLayout();
			}
		}
		
		protected var _pageSize:Number = 0;
		
		public function get pageSize():Number 
		{
			return _pageSize;
		}
		
		public function set pageSize(value:Number):void 
		{
			// pageSize 影响到 thumb 的高度，所以要特殊处理
			if (_pageSize == value) return;
			
			_pageSize = value;
			heightChanged = true;
			invalidateLayout();
		}
		
		protected var _stepSize:Number = 0;
		
		public function get stepSize():Number 
		{
			return _stepSize;
		}
		
		public function set stepSize(value:Number):void 
		{
			_stepSize = value;
		}
		
		
		
		// ----------------------------------------
		//
		//		UI 动作
		//
		// ----------------------------------------
		
		protected var longActionToken:uint = 0;
		
		protected var longActionTimer:Timer = new Timer(100);
		
		protected var longActionFunction:Function;
		protected var longActionFunctionParam:Array = [];
		
		protected function longActionTimer_timerHandler(event:TimerEvent):void
		{
			if (longActionFunction != null)
			{
				longActionFunction.apply(this, longActionFunctionParam);
			}
		}
		
		protected function arrowButton_mouseDownHandler(event:MouseEvent):void
		{
			var down:Boolean = true;
			
			if (event.currentTarget == upButton) down = false;
			
			changeValueByStep(down);
			
			stage.addEventListener(MouseEvent.MOUSE_UP, stage_longActionStopHandler);
			
			longActionToken = setTimeout(
				function():void
				{
					longActionFunction = changeValueByStep;
					longActionFunctionParam = [down];
					longActionTimer.start();
				}, 500);
		}
		
		protected function track_mouseDownHandler(event:MouseEvent):void
		{
			var down:Boolean = (this.mouseY > thumb.y);
			
			changeValueByPage(down);
			
			stage.addEventListener(MouseEvent.MOUSE_UP, stage_longActionStopHandler);
			
			longActionToken = setTimeout(
				function():void
				{
					longActionFunction = changeValueByPage;
					longActionFunctionParam = [down];
					longActionTimer.start();
				}, 500);
		}
		
		protected function stage_longActionStopHandler(event:MouseEvent):void
		{
			clearTimeout(longActionToken);
			longActionTimer.stop();
			stage.removeEventListener(MouseEvent.MOUSE_UP, stage_longActionStopHandler);
		}
		
		protected function adjustThumbPositionByValue():void
		{
			var minY:Number = (upButton ? upButton.height : 0);
			var maxY:Number = this.height - (downButton ? downButton.height : 0) - thumb.height;
			thumb.y = (_value - _minimum) / (_maximum - _minimum) * (maxY - minY) + minY;
			grip.y = thumb.y + (thumb.height - grip.height) / 2;
		}
		
		protected function thumb_mouseDownHandler(event:MouseEvent):void
		{
			stage.addEventListener(MouseEvent.MOUSE_MOVE, stage_thumbDragHandler);
			stage.addEventListener(MouseEvent.MOUSE_UP, stage_thumbDragStopHandler);
			thumb.startDrag(false, new Rectangle(thumb.x, (upButton ? upButton.height : 0), 0, this.height - thumb.height - (upButton ? upButton.height : 0) - (downButton ? downButton.height : 0)));
		}
		
		protected function stage_thumbDragHandler(event:MouseEvent):void
		{
			grip.y = thumb.y + (thumb.height - grip.height) / 2;
			
			var maxLength:Number = this.height - thumb.height - (upButton ? upButton.height : 0) - (downButton ? downButton.height : 0) - 1;
			var currPos:Number = thumb.y - (upButton ? upButton.height : 0);
			
//			trace(">>> " + _minimum, Math.round(currPos / maxLength * 100) / 100 * (_maximum - _minimum), _maximum);
			setValue(Math.round(currPos / maxLength * 100) / 100 * (_maximum - _minimum), true, false);
		}
		
		protected function stage_thumbDragStopHandler(event:MouseEvent):void
		{
			stage.removeEventListener(MouseEvent.MOUSE_MOVE, stage_thumbDragHandler);
			stage.removeEventListener(MouseEvent.MOUSE_UP, stage_thumbDragStopHandler);
			thumb.stopDrag();
		}
		
		
		
		// ----------------------------------------
		//
		//		数值动作
		//
		// ----------------------------------------
		
		public function changeValueByStep(increase:Boolean = true):void
		{
			var actualValue:Number = _value;
			
			actualValue += (increase ? _stepSize: -_stepSize);
			
			if (actualValue < minimum) actualValue = minimum;
			if (actualValue > maximum) actualValue = maximum;
			
			setValue(actualValue, true);
		}
		
		public function changeValueByPage(increase:Boolean = true):void
		{
			var actualValue:Number = _value;
			
			actualValue += (increase ? _pageSize: -_pageSize);
			
			if (actualValue < minimum) actualValue = minimum;
			if (actualValue > maximum) actualValue = maximum;
			
			setValue(actualValue, true);
		}
	}

}