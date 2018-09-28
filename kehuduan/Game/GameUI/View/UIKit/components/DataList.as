package GameUI.View.UIKit.components
{
	import GameUI.View.UIKit.core.UIComponent;
	import GameUI.View.UIKit.data.ArrayList;
	import GameUI.View.UIKit.data.DataTracker;
	import GameUI.View.UIKit.data.IDataTrackerClient;
	import GameUI.View.UIKit.events.DataEvent;
	import GameUI.View.UIKit.layout.VerticalLayout;
	
	import flash.display.DisplayObject;
	import flash.errors.IllegalOperationError;
	
	public class DataList extends UIComponent implements IDataTrackerClient
	{
		protected var _data:ArrayList;
		
		public function DataList()
		{
			_data = new ArrayList();
			
			var layout:VerticalLayout = new VerticalLayout(this);
			layout.gap = 1;
			this.layout = layout;
			
			itemRenderer = ItemRendererBase;
			
			initTracker();
		}
		
		public function get data():ArrayList
		{
			return _data;
		}
		
		public function set data(value:ArrayList):void
		{
			if (value == _data) return;
			
			_data = value;
			tracker.dataSource = value;
			
			if (data != null)
			{
				dataRefreshHandler(null);
			}
		}
		
		
		// ----------------------------------------
		//
		//		渲染器
		//
		// ----------------------------------------
		
		protected var _itemRenderer:Class = ItemRendererBase;
		
		public function get itemRenderer():Class
		{
			return _itemRenderer;
		}
		
		public function set itemRenderer(value:Class):void
		{
			if (value == _itemRenderer) return;
			
			_itemRenderer = value;
			
			$removeAllChildren();
			dataRefreshHandler(null);
		}
		
		
		
		// ----------------------------------------
		//
		//		数据跟踪器
		//
		// ----------------------------------------
		
		protected var tracker:DataTracker;
		public function get dataTracker():DataTracker
		{
			return tracker;
		}
		
		protected function initTracker():void
		{
			tracker = new DataTracker();
			
			tracker.dataSource = _data;
			tracker.addHandler = dataAddHandler;
			tracker.removeHandler = dataRemoveHandler;
			tracker.updateHandler = dataUpdateHandler;
			tracker.clearHandler = dataClearHandler;
			tracker.refreshHandler = dataRefreshHandler;
		}
		
		
		
		// ----------------------------------------
		//
		//		数据更新动作
		//
		// ----------------------------------------
		
		protected function dataAddHandler(event:DataEvent):void
		{
			var ir:ItemRendererBase = new _itemRenderer();
			ir.data = _data.getItemAt(event.index);
			ir.index = event.index;
			this.$addChildAt(ir, event.index);
			
			invalidateLayout();
		}
		
		protected function dataRemoveHandler(event:DataEvent):void
		{
			this.$removeChildAt(event.index);
			
			var l:int = this.numChildren;
			
			for (var i:int = event.index; i < l; i ++)
			{
				var ir:ItemRendererBase = this.getChildAt(i) as ItemRendererBase;
				ir.index = i;
			}
			
			invalidateLayout();
		}
		
		protected function dataUpdateHandler(event:DataEvent):void
		{
			(this.getChildAt(event.index) as ItemRendererBase).data = _data.getItemAt(event.index);
			(this.getChildAt(event.index) as ItemRendererBase).index = event.index;
		}
		
		protected function dataClearHandler(event:DataEvent):void
		{
			$removeAllChildren();
				
			invalidateLayout();
		}
		
		protected function dataRefreshHandler(event:DataEvent):void
		{
			var n:int = this.numChildren;
			var dn:int = _data.length;
			var i:int;
			var needLayout:Boolean = true;
			
			if (n > dn)
			{
				for (i = dn; i < n; i++)
				{
					this.$removeChildAt(i);
				}
			}
			else if (dn > n)
			{
				for (i = n; i < dn; i++)
				{
					var ir:ItemRendererBase = new _itemRenderer();
					//ir.data = _data.getItemAt(i);
					//ir.index = i;
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
		}
		
		
		
		// ----------------------------------------
		//
		//		布局
		//
		// ----------------------------------------
		
		override public function invalidateLayout():void
		{
			// zhao: 参数 startPosition 最初的用意是指定从某个位置开始重排，
			// 比如当向 1, 2, 4, 5 中插入元素 3 时，只需要重排 3, 4, 5 号元素。
			// 但由于泛化的 LayoutBase.doLayout() 函数不接收参数，
			// 所以 startPosition 目前暂不使用。
			
			layout.invalidateLayout(true);
		}
		
		
		
		// ----------------------------------------
		//
		//		子项操作：关闭默认函数
		//
		// ----------------------------------------
		
		override public function addChild(child:DisplayObject):DisplayObject
		{
			throw new IllegalOperationError("此类不支持 addChild() 函数，请使用 data.addItem() 修改数据。");
		}
		
		override public function addChildAt(child:DisplayObject, index:int):DisplayObject
		{
			throw new IllegalOperationError("此类不支持 addChildAt() 函数，请使用 data.addItemAt() 修改数据。");
		}
		
		override public function removeChild(child:DisplayObject):DisplayObject
		{
			throw new IllegalOperationError("此类不支持 removeChild() 函数，请使用 data.removeItem() 修改数据。");
		}
		
		override public function removeChildAt(index:int):DisplayObject
		{
			throw new IllegalOperationError("此类不支持 removeChildAt() 函数，请使用 data.removeItemAt() 修改数据。");
		}
		
		
		
		// ----------------------------------------
		//
		//		子项操作：添加自定义函数（$开头）
		//
		// ----------------------------------------
		
		protected function $addChild(child:DisplayObject):DisplayObject
		{
			return super.addChild(child);
		}
		
		protected function $addChildAt(child:DisplayObject, index:int):DisplayObject
		{
			return super.addChildAt(child, index);
		}
		
		protected function $removeChild(child:DisplayObject):DisplayObject
		{
			return super.removeChild(child);
		}
		
		protected function $removeChildAt(index:int):DisplayObject
		{
			return super.removeChildAt(index);
		}
		
		protected function $removeAllChildren():void
		{
			var n:int = this.numChildren;
			
			for (var i:int = 0; i < n; i ++)
			{
				this.$removeChildAt(0);
			}
		}
	}
}