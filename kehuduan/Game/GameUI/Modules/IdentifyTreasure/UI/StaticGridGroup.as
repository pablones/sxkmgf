package GameUI.Modules.IdentifyTreasure.UI
{
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;

	//静态格子组件 创建一系列放东西的格子
	public class StaticGridGroup extends Sprite
	{
		private var grid_mc:DisplayObjectContainer;
		private var col:uint;				//列
		private var row:uint;			//行
		private var _rowDis:Number;			//行距
		private var _colDis:Number;			//列距
		private var aGrids:Array;
		
		public function StaticGridGroup( _GridClass:Class,_row:uint,_col:uint )
		{
			super();
			
			row = _row;
			col = _col;
			createGrids( _GridClass );
		}
		
		private function createGrids( _GridClass:Class ):void
		{
			aGrids = [];
			for ( var i:uint=0; i<col*row; i++ )
			{
				grid_mc = new _GridClass() as DisplayObjectContainer;
				aGrids.push( grid_mc );
			}
		}
		
		//排列  根据列数和数组长度即可进行排列
		public function placeGrids():void
		{
			var len:uint;
			var r:uint;					// 行
			var c:uint;				//列
			if ( aGrids && aGrids.length>0 )
			{
				len = aGrids.length;
				for ( var i:uint=0; i<len; i++ )
				{
					c = i % col;
					r = Math.floor( i/col );
					if ( c == 0 )
					{
						aGrids[i].x = 0;		
					}
					else
					{
						aGrids[i].x = c * ( colDis + aGrids[i].width );	
					}
					if ( r==0 )
					{
						aGrids[i].y = 0;		
					}
					else
					{
						aGrids[i].y = r * ( aGrids[i].height + rowDis );
					}
					this.addChild( aGrids[i] );
				}
				
			}
		}
		
		public function set dataGrid( arr:Array ):void
		{
			var item:UseItem;
			var container:DisplayObjectContainer;	
			for ( var j:uint=0; j<aGrids.length; j++ )
			{
				clearContainer( aGrids[j] );
			}
			
			if ( arr && arr.length>0 )
			{
				for ( var i:uint=0; i<arr.length; i++ )
				{
					if ( i >= aGrids.length ) return;
					container = aGrids[i] as DisplayObjectContainer;	
					if ( arr[i].type == 0 ) continue;
					item = new UseItem( 0,String( arr[i].type ),container );
					item.x = 3;
					item.y = 3;
					item.Type = int( arr[i].type );
					item.Num = arr[i].num;
					container.name = "TaskEqu_" + arr[i].type;
					container.addChild( item );
				}
			}
		}
		
		private function clearContainer( dis:DisplayObjectContainer ):void
		{
			var aDes:Array = [];
			for ( var i:uint=0; i<dis.numChildren; i++ )
			{
				if ( dis.getChildAt( i ) is ItemBase )
				{
					aDes.push( dis.getChildAt( i ) );
					dis.name = "";
				}
			}
			for ( var j:uint=0; j<aDes.length; j++ )
			{
				aDes[j].parent.removeChild( aDes[j] );
				aDes[j] = null;
			}
		}
		
//		//设置行
//		public function set row( value:uint )
//		{
//			this._row = value;
//		}
//		
//		public function get row():uint
//		{
//			return this._row;
//		}
//		
//		//设置列
//		public function set col( value:uint )
//		{
//			this._col = value;
//		}
//		
//		public function get col():uint
//		{
//			return this._col;
//		}
		
		//设置行距
		public function set rowDis( value:Number ):void
		{
			this._rowDis = value;
		}
		
		public function get rowDis():Number
		{
			return this._rowDis;
		}
		
		//设置列距
		public function set colDis( value:Number ):void
		{
			this._colDis = value;
		}
		
		public function get colDis():Number
		{
			return this._colDis;
		}
		
	}
}