package GameUI.View.UIKit.components 
{
	import GameUI.View.UIKit.events.DataEvent;
	
	import flash.events.Event;
	import flash.events.MouseEvent;
	/**
	 * ...
	 * @author statm
	 */
	public class PagingDataList extends InteractiveDataList
	{
		
		public function PagingDataList() 
		{
			
		}
		
		
		
		// ----------------------------------------
		//
		//		页码控制属性
		//
		// ----------------------------------------
		
		protected var _pageSize:int = 10;
		
		public function get pageSize():int
		{
			return _pageSize;
		}
		
		public function set pageSize(value:int):void
		{
			_pageSize = value;
			if (_pageSize < 1) _pageSize = 1;
			
			refreshContent();
		}
		
		protected var _currentPage:int = 0;
		
		public function get currentPage():int
		{
			return _currentPage;
		}
		
		public function set currentPage(value:int):void
		{
			_currentPage = value;
			
			refreshContent();
		}
		
		protected var _pageTotal:int = 0;
		
		public function get pageTotal():int
		{
			return _pageTotal;
		}
		
		
		// ----------------------------------------
		//
		//		数据更新动作
		//
		// ----------------------------------------
		
		override protected function dataAddHandler(event:DataEvent):void
		{
			if (event.index < (_currentPage + 1) * _pageSize)
			{
				refreshContent();
			}
		}
		
		override protected function dataRemoveHandler(event:DataEvent):void 
		{
			if (event.index < (_currentPage + 1) * _pageSize)
			{
				refreshContent();
			}
		}
		
		override protected function dataUpdateHandler(event:DataEvent):void 
		{
			if (event.index >= _currentPage * _pageSize
				&& event.index < (_currentPage + 1) * _pageSize)
			{
				(this.getChildAt(event.index % _pageSize) as ItemRendererBase).data = _data.getItemAt(event.index);
			}
		}
		
		override protected function dataClearHandler(event:DataEvent):void 
		{
			refreshContent();
		}
		
		override protected function dataRefreshHandler(event:DataEvent):void 
		{
			refreshContent();
		}
		
		
		// ----------------------------------------
		//
		//		分页刷新动作
		//
		// ----------------------------------------
		
		protected function refreshContent():void
		{
			if (_data == null) return;
			
			// 数据有效性检查
			var maxPage:Number = Math.ceil(_data.length / _pageSize) - 1;
			if (maxPage < 0) maxPage = 0;
			if (_currentPage < 0) _currentPage = 0;
			if (_currentPage > maxPage)	_currentPage = maxPage;
			
			var actualSize:int = _pageSize;
			
			if (_currentPage == maxPage)
			{
				actualSize = _data.length % _pageSize;
				if (actualSize == 0 && _data.length > 0) actualSize = _pageSize;
			}
			
			var i:int;
			
			if (this.numChildren < actualSize)
			{
				for (i = this.numChildren; i < actualSize; i ++)
				{
					var ir:ItemRendererBase = new _itemRenderer();
					if (ir is ItemRenderer)
					{
						ir.addEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
					}
					this.$addChildAt(ir, i);
				}
				invalidateLayout();
			}
			else if (this.numChildren > actualSize)
			{
				for (i = this.numChildren; i > actualSize; i --)
				{
					ir = this.getChildAt(0) as ItemRendererBase;
					ir.removeEventListener(MouseEvent.MOUSE_DOWN, itemRenderer_mouseDownHandler);
					this.$removeChildAt(0);
				}
				invalidateLayout();
			}
			
			for (i = 0; i < actualSize; i ++)
			{
				var index:int = _currentPage * _pageSize + i;
				
				var item:ItemRendererBase = this.getChildAt(i) as ItemRendererBase;
				item.index = index;
				item.data = _data.getItemAt(index);
			}
			
			if (Math.ceil(_data.length / _pageSize) != _pageTotal)
			{
				_pageTotal = Math.ceil(_data.length / _pageSize);
				this.dispatchEvent(new Event("pageTotalChanged"));
			}
		}
	}

}