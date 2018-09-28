package GameUI.View.UIKit.components 
{
	import GameUI.View.UIKit.data.ArrayList;
	import GameUI.View.UIKit.events.DataEvent;
	import GameUI.View.UIKit.events.IndexChangeEvent;
	import GameUI.View.UIKit.layout.VerticalLayout;
	
	import flash.events.MouseEvent;
	
	[Event(name="changed", type="GameUI.View.UIKit.events.IndexChangeEvent")]
	
	/**
	 * ...
	 * @author statm
	 */
	public class InteractiveDataList extends DataList 
	{
		
		public function InteractiveDataList() 
		{
			_data = new ArrayList();
			
			var layout:VerticalLayout = new VerticalLayout(this);
			this.layout = layout;
			
			itemRenderer = ItemRenderer;
			
			initTracker();
		}
		
		
		
		// ----------------------------------------
		//
		//		数据更新动作
		//
		// ----------------------------------------
		
		override protected function dataAddHandler(event:DataEvent):void 
		{
			super.dataAddHandler(event);
			
			var ir:ItemRendererBase = this.getChildAt(event.index) as ItemRendererBase;
			
			if (ir is ItemRenderer)
			{
				ir.addEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
			}
			
			if (_selectedIndex >= event.index)
			{
				_selectedIndex ++;
			}
		}
		
		override protected function dataRemoveHandler(event:DataEvent):void 
		{
			var ir:ItemRendererBase = this.getChildAt(event.index) as ItemRendererBase;
			
			if (ir is ItemRenderer)
			{
				ir.removeEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
			}
			
			if (event.index == _selectedIndex)
			{
				setSelectedIndex( -1);
			}
			else if (event.index < _selectedIndex)
			{
				_selectedIndex --;
			}
			
			super.dataRemoveHandler(event);
		}
		
		override protected function dataClearHandler(event:DataEvent):void 
		{
			var n:int = this.numChildren;
			
			for (var i:int = 0; i < n; i ++)
			{
				var ir:ItemRendererBase = this.getChildAt(i) as ItemRendererBase;
				
				if (ir is ItemRenderer)
				{
					ir.removeEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
				}
			}
			
			setSelectedIndex( -1);
			
			super.dataClearHandler(event);
		}
		
		override protected function dataRefreshHandler(event:DataEvent):void 
		{
			var n:int = this.numChildren;
			var dn:int = _data.length;
			var i:int;
			var needLayout:Boolean = true;
			var ir:ItemRendererBase;
			
			if (n > dn)
			{
				for (i = dn; i < n; i++)
				{
					ir = this.getChildAt(event.index) as ItemRendererBase;
			
					if (ir is ItemRenderer)
					{
						ir.removeEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
					}
					
					this.$removeChild(ir);
				}
			}
			else if (dn > n)
			{
				for (i = n; i < dn; i++)
				{
					ir = new _itemRenderer();
					
					if (ir is ItemRenderer)
					{
						ir.addEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
					}
					
					this.$addChild(ir);
				}
				needLayout = true;
			}
			
			for (i = 0; i < this.numChildren; i ++)
			{
				(this.getChildAt(i) as ItemRendererBase).data = _data.getItemAt(i);
				(this.getChildAt(i) as ItemRendererBase).index = i;
			}
			
			if (needLayout)
			{
				invalidateLayout();
			}
			
			// TODO(zhao): 捕捉选择项目，重定位
			setSelectedIndex( -1);
		}
		
		
		
		// ----------------------------------------
		//
		//		选择控制
		//
		// ----------------------------------------
		
		protected var _selectedIndex:int = -1;
		
		public function get selectedIndex():int
		{
			return _selectedIndex;
		}
		
		public function set selectedIndex(value:int):void
		{
			setSelectedIndex(value);
		}
		
		protected function setSelectedIndex(index:int, dispatchChangeEvent:Boolean = true):void
		{
			var ir:ItemRendererBase;
			
			if (index >= this.numChildren || index < -2)
			{
				throw new RangeError("位置 " + index + " 越界。");
			}
			
			if (index == -1)
			{
				if (_selectedIndex == -1)
				{
					if (_selectionRepeat && dispatchChangeEvent)
					{
						dispatchEvent(new IndexChangeEvent(IndexChangeEvent.CHANGED, -1, -1));
					}
				}
				else
				{
					ir = this.getChildAt(_selectedIndex) as ItemRendererBase;
					if (ir is ItemRenderer)
					{
						(ir as ItemRenderer).selected = false;
					}
					if (dispatchChangeEvent)
					{
						dispatchEvent(new IndexChangeEvent(IndexChangeEvent.CHANGED, _selectedIndex, index));
					}
				}
				
				_selectedIndex = index;
				
				return;
			}
			
			if (index == _selectedIndex)
			{
				if (_selectionRepeat && dispatchChangeEvent)
				{
					dispatchEvent(new IndexChangeEvent(IndexChangeEvent.CHANGED, _selectedIndex, index));
				}
				else return;
			}
			
			if (_selectedIndex > -1)
			{
				ir = this.getChildAt(_selectedIndex) as ItemRendererBase;
				if (ir is ItemRenderer)
				{
					(ir as ItemRenderer).selected = false;
				}
			}
			if (index > -1)
			{
				ir = this.getChildAt(index) as ItemRendererBase;
				if (ir is ItemRenderer)
				{
					(ir as ItemRenderer).selected = true;
				}
			}
			
			
			var oldIndex:int = _selectedIndex;
			_selectedIndex = index;
			
			if (dispatchChangeEvent)
			{
				dispatchEvent(new IndexChangeEvent(IndexChangeEvent.CHANGED, oldIndex, index));
			}
		}
		
		protected var _selectionRepeat:Boolean = false;
		
		public function get selectionRepeat():Boolean 
		{
			return _selectionRepeat;
		}
		
		public function set selectionRepeat(value:Boolean):void 
		{
			_selectionRepeat = value;
		}
		
		// TODO(zhao): requireSelection 属性。
		
		
		// ----------------------------------------
		//
		//		渲染器鼠标动作
		//
		// ----------------------------------------
		
		protected function itemRenderer_mouseDownHandler(event:MouseEvent):void
		{
			setSelectedIndex((event.currentTarget as ItemRenderer).index);
		}
	}

}