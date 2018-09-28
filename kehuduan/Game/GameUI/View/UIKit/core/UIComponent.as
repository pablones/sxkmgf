package GameUI.View.UIKit.core 
{
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import GameUI.View.UIKit.layout.LayoutBase;
	
	[Event(name="resize", type="statm.dev.events.ResizeEvent")]
	
	/**
	 * ...
	 * @author statm
	 */
	public class UIComponent extends Sprite 
	{
		
		public function UIComponent() 
		{
			super();
			
			loadSkinParts();
		}
		
		
		
		// ----------------------------------------
		//
		//		尺寸
		//
		// ----------------------------------------
		
		protected var _width:Number = 0; 
		
		override public function get width():Number
		{
			return _width;
		}
		
		override public function set width(value:Number):void
		{
			if (_explicitWidth != value)
			{
				_explicitWidth = value;
			}
			if (_width != value)
			{
				_width = value;
			}
			
			invalidateLayout();
		}
		
		protected var _height:Number = 0;
		
		override public function get height():Number
		{
			return _height;
		}
		
		override public function set height(value:Number):void
		{
			if (_explicitHeight != value)
			{
				_explicitHeight = value;
			}
			if (_height != value)
			{
				_height = value;
			}
			
			invalidateLayout();
		}
		
		protected var _explicitWidth:Number = NaN;
		
		public function get explicitWidth():Number
		{
			return _explicitWidth;
		}
		
		protected var _explicitHeight:Number = NaN;
		
		public function get explicitHeight():Number
		{
			return _explicitHeight;
		}
		
		
		
		// ----------------------------------------
		//
		//		布局 & 尺寸更新
		//
		// ----------------------------------------
		
		protected var _layout:LayoutBase;
		
		public function get layout():LayoutBase
		{
			return _layout;
		}
		
		public function set layout(value:LayoutBase):void
		{
			if (value == _layout) return;
			var needLayout:Boolean = (_layout != null);
			
			_layout = value;
			
			if (needLayout) _layout.invalidateLayout();
		}
		
		public function invalidateLayout():void
		{
			// zhao: 失效机制的不足和 TODOs：
			// 1. 现在只有 invalidateLayout，没有 invalidateSize, invalidateProperties 等；
			// 2. 超类中没有定义 invalidateFlags，需要写在这里吗？
			// 3. 只有失效函数，没有生效函数（commitProperties, validateSize...）；
			//	  现在的生效机制是组件自行延迟到下一个 RENDER 中实现的。
		}
		
		public function setActualSize(width:Number, height:Number):void
		{
			_width = width;
			_height = height;
		}
		
		
		
		// ----------------------------------------
		//
		//		子项
		//
		// ----------------------------------------
		
		override public function addChildAt(child:DisplayObject, index:int):DisplayObject 
		{
			var result:DisplayObject = super.addChildAt(child, index);
			
			_layout.invalidateLayout();
			return result;
		}
		
		override public function removeChildAt(index:int):DisplayObject 
		{
			var result:DisplayObject = super.removeChildAt(index);
			
			_layout.invalidateLayout();
			return result;
		}
		
		
		
		
		// ----------------------------------------
		//
		//		视口
		//
		// ----------------------------------------
		
		protected var _viewport:ViewportController;
		
		public function get viewport():ViewportController
		{
			return _viewport;
		}
		
		protected var _viewportEnabled:Boolean = false;
		
		public function get clipAndEnableScrolling():Boolean
		{
			return _viewportEnabled;
		}
		
		public function set clipAndEnableScrolling(value:Boolean):void
		{
			if (value == _viewportEnabled) return;
			
			_viewportEnabled = value;
			
			if (_viewportEnabled)
			{
				if (!_viewport) _viewport = new ViewportController(this);
				
				_viewport.activate();
			}
			else
			{
				_viewport.deactivate();
			}
		}
		
		
		
		// ----------------------------------------
		//
		//		部件
		//
		// ----------------------------------------
		
		public function loadSkinParts():void
		{
			// zhao: 这个函数究竟应该是 protected 还是 public？
			// 能想到的一种情况是，如果以后出现资源通过某个管理器
			// 延迟加载的时候，需要在外部调用所有相关组件的这个函数。
			// 除此之外，其他情况下应该没有 public 的必要。
		}
		
		protected function layoutSkinParts():void
		{
		}
		
		
		
		// ----------------------------------------
		//
		//		测试代码：销毁
		//
		// ----------------------------------------
		
		// zhao: 什么地方会用到呢？
		
		public function finalize():void
		{
			if (_layout)
			{
				_layout.finalize();
			}
			if (_viewport)
			{
				_viewport.finalize();
			}
		}
	}

}