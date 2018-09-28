package GameUI.Modules.IdentifyTreasure.UI
{
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class TreaGridManager extends Sprite
	{
		private var row:uint = 5;							//行
		private var col:uint = 10;							//列	
		private var aGrids:Array;							//所有格子数组
		private var _typeDate:Array = new Array();
		private var yellowShape:Shape;
		
		public function TreaGridManager()
		{
			createGrids();
//			yellowShape = createShape( 0xffff00 );
		}
		
		//创建格子
		private function createGrids():void
		{
			var gridUnit:TreaGridUnit;
			aGrids = [];
						
			for ( var i:uint=0; i<row*col; i++ )
			{
				gridUnit = new TreaGridUnit( new TreasureData.BlackRectGrid() );
				gridUnit.index = i;
				aGrids.push( gridUnit );
			}
			placeGrids();
		}
		
		private function placeGrids():void
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
					aGrids[i].x = c * ( aGrids[i].width );	
					aGrids[i].y = r * ( aGrids[i].height );
					this.addChild( aGrids[i] );
				}
			}
		}
		
		public function set typeData( arr:Array ):void
		{
			_typeDate = arr;
			for ( var j:uint=0; j<aGrids.length; j++ )
			{
				clearItemContainer( aGrids[j].grid );
				aGrids[j].item = null;
				aGrids[j].isSelect = false;
			}
			var item:UseItem;
			var container:DisplayObjectContainer;
			if ( arr && arr.length>0 )
			{
				for ( var i:uint=0; i<arr.length; i++ )
				{
					if ( i >= aGrids.length ) return;
					container = aGrids[i].grid as DisplayObjectContainer;	
					if ( arr[i] == 0 ) continue;
					item = new UseItem( 0,arr[i].type,container );
					item.Num = arr[i].num;
					item.Id = arr[i].id;
					item.x = 2;
					item.y = 2;
					aGrids[i].item = item;
					container.name = "TaskEqu_" + arr[i].type;
					( aGrids[i] as TreaGridUnit ).addItem( item );
				}
			}
		}
		
		public function get typeData():Array
		{
			return _typeDate;
		}
		
		//移除item 
		public function removeItem( type:uint ):void
		{
			var index:int = typeData.indexOf( type );
			if ( index == -1 || index >= aGrids.length ) return;
			clearItemContainer( aGrids[ index ].grid );
			aGrids[ index ].isSelect = false;
			typeData.splice( index,1 );
		}
		
		private function clearItemContainer( dis:DisplayObjectContainer ):void
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

	}
}