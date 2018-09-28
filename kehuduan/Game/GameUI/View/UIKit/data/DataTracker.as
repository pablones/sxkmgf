package GameUI.View.UIKit.data
{
	import flash.events.IEventDispatcher;
	
	import GameUI.View.UIKit.events.DataEvent;

	public final class DataTracker
	{
		private var dataObj:IEventDispatcher;
		private var _addHandler:Function;
		private var _removeHandler:Function;
		private var _updateHandler:Function;
		private var _clearHandler:Function;
		private var _refreshHandler:Function;
		
		public function DataListTracker(dataSource:IEventDispatcher=null):void
		{
			if (dataSource)
				this.dataObj = dataSource;
		}
		
		public function set dataSource(value:IEventDispatcher):void
		{
			removeAllHandlers();
			
			dataObj = value;
			
			addAllHandlers();
		}
		
		private function addAllHandlers():void
		{
			if (!dataObj) return;
			
			if (_addHandler != null)
				dataObj.addEventListener(DataEvent.ADD, _addHandler);
			if (_removeHandler != null)
				dataObj.addEventListener(DataEvent.REMOVE, _removeHandler);
			if (_clearHandler != null)
				dataObj.addEventListener(DataEvent.CLEAR, _clearHandler);
			if (_updateHandler != null)
				dataObj.addEventListener(DataEvent.UPDATE, _updateHandler);
			if (_refreshHandler != null)
				dataObj.addEventListener(DataEvent.REFRESH, _refreshHandler);
		}
		
		private function removeAllHandlers():void
		{
			if (!dataObj) return;
			
			if (_addHandler != null)
				dataObj.removeEventListener(DataEvent.ADD, _addHandler);
			if (_removeHandler != null)
				dataObj.removeEventListener(DataEvent.REMOVE, _removeHandler);
			if (_clearHandler != null)
				dataObj.removeEventListener(DataEvent.CLEAR, _clearHandler);
			if (_updateHandler != null)
				dataObj.removeEventListener(DataEvent.UPDATE, _updateHandler);
			if (_refreshHandler != null)
				dataObj.removeEventListener(DataEvent.REFRESH, _refreshHandler);
		}
		
		public function set addHandler(value:Function):void
		{
			if (dataObj)
			{
				if (_addHandler != null)
					dataObj.removeEventListener(DataEvent.ADD, _addHandler);
				_addHandler = value;
				if (_addHandler != null)
					dataObj.addEventListener(DataEvent.ADD, _addHandler);
			}
			else
			{
				_addHandler = value;
			}
		}
		
		public function set removeHandler(value:Function):void
		{
			if (dataObj)
			{
				if (_removeHandler != null)
					dataObj.removeEventListener(DataEvent.REMOVE, _removeHandler)
				_removeHandler = value;
				if (_removeHandler != null)
					dataObj.addEventListener(DataEvent.REMOVE, _removeHandler);
			}
			else
			{
				_removeHandler = value;
			}
		}
		
		public function set clearHandler(value:Function):void
		{
			if (dataObj)
			{
				if (_clearHandler != null)
					dataObj.removeEventListener(DataEvent.CLEAR, _clearHandler);
				_clearHandler = value;
				if (_clearHandler != null)
					dataObj.addEventListener(DataEvent.CLEAR, _clearHandler);
			}
			else
			{
				_clearHandler = value;
			}
		}
		
		public function set updateHandler(value:Function):void
		{
			if (dataObj)
			{
				if (_updateHandler != null)
					dataObj.removeEventListener(DataEvent.UPDATE, _updateHandler);
				_updateHandler = value;
				if (_updateHandler != null)
					dataObj.addEventListener(DataEvent.UPDATE, _updateHandler);
			}
			else
			{
				_updateHandler = value;
			}
		}
		
		public function set refreshHandler(value:Function):void
		{
			if (dataObj)
			{
				if (_refreshHandler != null)
					dataObj.removeEventListener(DataEvent.REFRESH, _refreshHandler);
				_refreshHandler = value;
				if (_refreshHandler != null)
					dataObj.addEventListener(DataEvent.REFRESH, _refreshHandler);
			}
			else
			{
				_refreshHandler = value;
			}
		}
		
		public function $destroy():void
		{
			removeAllHandlers();
			dataObj = null;
		}
	}
}