package GameUI.View.Components.ItemList
{
	import flash.display.DisplayObject;

	public class ItemListBase
	{
		private var _disOArr:Array;
		private var _row:int;
		private var _column:int;
		private var _distanceX:int;
		private var _distanceY:int;
		private var _layoutType:int;
		public static const LAYOUT_ROW:int = 1;			//先横着排
		public static const LAYOUT_COLUMN:int = 2;		//先列着排
		
		/**
		 * 
		 * @param disOArr					显示对象数组
		 * @param row						行数
		 * @param column					列数
		 * @param distanceX					行间距
		 * @param distanceY					列间距
		 * @param layoutType				布局类型
		 * 
		 */
		public function ItemListBase(disOArr:Array,row:int,column:int,distanceX:int,distanceY:int,layoutType:int = ItemListBase.LAYOUT_ROW)
		{
			_disOArr = disOArr;
			_row = row;
			_column = column;
			_distanceX = distanceX;
			_distanceY = distanceY;
			_layoutType = layoutType;
		}
		
		/** 布局静态方法
		 * 
		 * @param initX						
		 * @param initY
		 * @param disOArr					显示对象数组
		 * @param row						行数
		 * @param column					列数
		 * @param distanceX					行间距
		 * @param distanceY					列间距
		 * @param layoutType				布局类型
		 * 
		 */		
		public static function setLayout(disOArr:Array,row:int,column:int,distanceX:int,distanceY:int,initX:int = 0,initY:int = 0,layoutType:int = ItemListBase.LAYOUT_ROW):void
		{
			var i:int;
			var disO:DisplayObject;
			for( i = 0; i < disOArr.length && i < row * column ; i++ )
			{
				disO = disOArr[i];
				if( ItemListBase.LAYOUT_ROW == layoutType )
				{
					disO.x = distanceX * (i % column) + initX;
					disO.y = distanceY * int(i / column) + initY;
				}
				else if(ItemListBase.LAYOUT_COLUMN == layoutType)
				{
					disO.x = distanceX * int(i / row) + initX;
					disO.y = distanceY * (i % row) + initY;
				}
			}
		}
		
		/** 更新布局 */
		protected function updateLayout():void
		{
			var i:int;
			var disO:DisplayObject;
			for( i = 0; i < _disOArr.length && i < _row * _column ; i++ )
			{
				disO = _disOArr[i];
				if( ItemListBase.LAYOUT_ROW == _layoutType )
				{
					disO.x = _distanceX * (i % _column);
					disO.y = _distanceY * (i / _column);
				}
				else if(ItemListBase.LAYOUT_COLUMN == _layoutType)
				{
					disO.x = _distanceX * (i / _row);
					disO.y = _distanceY * (i % _row);
				}
			}
		}
		
		protected function set row(row:int):void
		{
			_row = row;
			updateLayout();
		}
		
		protected function get row():int
		{
			return _row;
		}
		
		protected function set column(column:int):void
		{
			_column = column;
			updateLayout();
		}
		
		protected function get column():int
		{
			return  _column ;
		}
		
		protected function set distanceX(distanceX:int):void
		{
			_distanceX = distanceX;
			updateLayout();
		}
		
		protected function get distanceX():int
		{
			return _distanceX;
		}
		
		protected function set distanceY(distanceY:int):void
		{
			_distanceY = distanceY;
			updateLayout();
		}
		
		protected function get distanceY():int
		{
			return _distanceY;
		}
		
		protected function set layoutType(_layoutType:int):void
		{
			_layoutType = layoutType;
			updateLayout();
		}
		
		protected function get layoutType():int
		{
			return _layoutType ;
		}
	}
}