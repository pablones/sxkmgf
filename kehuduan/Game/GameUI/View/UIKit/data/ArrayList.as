package GameUI.View.UIKit.data
{
	import GameUI.View.UIKit.events.DataEvent;
	
	import flash.events.EventDispatcher;
	
	public class ArrayList extends EventDispatcher
	{
		protected var array:Array;
		
		public function ArrayList(source:Array = null):void
		{
			super();
			
			if (source) array = source;
			else array = [];
		}
		
		public function get length():int
		{
			return array.length;
		}
		
		public function get source():Array
		{
			return array;
		}
		
		public function set source(value:Array):void
		{
			if (value == null) return;
			
			if (array != value)
			{
				array = value;
				internalDispatch(new DataEvent(DataEvent.REFRESH));
			}
		}
		
		public function toArray():Array
		{
			return array.concat(); 
		}
		
		public function clone():ArrayList
		{
			return new ArrayList(array.concat());
		}
		
		
		// ----------------------------------------
		//
		//		元素操作：CRUD
		//
		// ----------------------------------------
		
		public function addItem(item:Object):void
		{
			addItemAt(item, length);
		}
		
		public function addItemAt(item:Object, index:int):void
		{
			if (index < 0 || index > length)
			{
				throw new RangeError("位置 " + index + " 越界。");
			}
			if (getItemIndex(item) > -1)
			{
				throw new Error("要添加的项目 " + item.toString() + " 与 ArrayList 中现有项目重复，拒绝添加。");
			}
			
			// 排序处理
			var actualPos:int = index;
			if (sortEnabled) actualPos = getInsertPosFromSort(item);
			
			array.splice(index, 0, item);
			if (sortEnabled) doSort();
			
			internalDispatch(new DataEvent(DataEvent.ADD, actualPos));
		}
		
		public function getItemAt(index:int):Object
		{
			if (index < 0 || index >= length)
			{
				throw new RangeError("位置 " + index + " 越界。");
			}
			
			if (sortEnabled)
			{
				return sortedData[index];
			}
			else
			{
				return array[index];
			}
		}
		
		public function getItemIndex(item:Object):int
		{
			if (sortEnabled)
			{
				return sortedData.indexOf(item);
			}
			else
			{
				return array.indexOf(item);
			}
		}
		
		public function contains(item:Object):Boolean
		{
			return getItemIndex(item) > -1;
		}
		
		public function query(property:String, value:*):Array
		{
			if (property == null || property == "" || value == null) return [];
			
			var l:int = length;
			var result:Array = [];
			
			for (var i:int = 0; i < l; i ++)
			{
				if (getItemAt(i)[property] == value)
					result.push(i);
			}
			
			return result;
		}
		
		public function itemUpdated(index:int):void
		{
			if (index < 0 || index >= length)
			{
				throw new RangeError("位置 " + index + " 越界。");
			}
			
			internalDispatch(new DataEvent(DataEvent.UPDATE, index));
		}
		
		public function removeItem(item:Object):void
		{
			var index:int = getItemIndex(item);
			
			if (index > -1)
			{
				removeItemAt(index);
			}
		}
		
		public function removeItemAt(index:int):void
		{
			if (index < 0 || index >= length)
			{
				throw new RangeError("位置 " + index + " 越界。"); 
			}
			
			if (sortEnabled)
			{
				array.splice(array.indexOf(sortedData[index]), 1);
				sortedData.splice(index, 1);
			}
			else
			{
				if (sortedData) sortedData.splice(sortedData.indexOf(array[index]), 1);
				array.splice(index, 1);
			}
			
			internalDispatch(new DataEvent(DataEvent.REMOVE, index));
		}
		
		public function removeAll():void
		{
			array.splice(0, length);
			if (sortedData) sortedData.splice(0, length);
			
			internalDispatch(new DataEvent(DataEvent.CLEAR));
		}
		
		
		
		// ----------------------------------------
		//
		//		排序
		//
		// ----------------------------------------
		
		public static const CASEINSENSITIVE:int = Array.CASEINSENSITIVE;
		
		public static const DESCENDING:int = Array.DESCENDING;
		
		public static const NUMERIC:int = Array.NUMERIC;
		
		
		protected var sortEnabled:Boolean = false;
		
		protected var sortedData:Array;
		
		protected var sortOptions:SortOptions;
		
		public function startSort(options:SortOptions):void
		{
			sortEnabled = true;
			
			if (sortOptions && sortOptions.equal(options)) return;
			
			sortOptions = options;
			
			doSort();
			internalDispatch(new DataEvent(DataEvent.REFRESH));
		}
		
		protected function doSort():void
		{
			if (sortOptions.field != null)
			{
				sortedData = array.concat().sortOn(sortOptions.field, sortOptions.param);
			}
			else if (sortOptions.compareFunction != null)
			{
				sortedData = array.concat().sort(sortOptions.compareFunction, sortOptions.param);
			}
			else
			{
				sortedData = array.concat().sortOn(sortOptions.param);
			}
		}
		
		public function stopSort():void
		{
			sortEnabled = false;
			sortOptions = null;
			internalDispatch(new DataEvent(DataEvent.REFRESH));
		}
		
		protected function getInsertPosFromSort(item:Object):int
		{
			var temp:Array = array.concat(item);
			var indices:Array;
			
			if (sortOptions == null)
			{
				indices = temp.sortOn(sortOptions.field, Array.RETURNINDEXEDARRAY);
			}
			else if (sortOptions.compareFunction != null)
			{
				indices = temp.sort(sortOptions.compareFunction, sortOptions.param | Array.RETURNINDEXEDARRAY);
			}
			else
			{
				indices = temp.sortOn(sortOptions.field, sortOptions.param | Array.RETURNINDEXEDARRAY);
			}
			
			return indices.indexOf(array.length);
		}
		
		
		
		// ----------------------------------------
		//
		//		事件预处理
		//
		// ----------------------------------------
		
		protected function internalDispatch(event:DataEvent):void
		{
			dispatchEvent(event);
		}
		
	}
}