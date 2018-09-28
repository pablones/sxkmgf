package GameUI.Modules.IdentifyTreasure.UI
{
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.BaseUI.ItemBase;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class TreaGridUnit extends Sprite
	{
		public var grid:MovieClip;
		public var index:int = -1;
		
		public var item:ItemBase;
		public var isUsed:Boolean = false;
		
		//外框
		private var redShape:Shape;
		private var yellowShape:Shape;
		private var _isSelect:Boolean;												//是否被选中
		
		public function TreaGridUnit( _grid:MovieClip )
		{
			grid = _grid;
			grid.doubleClickEnabled = true;
			initialize();
		}
		
		public function initialize():void
		{
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			
			this.addChild( grid );
			redShape      = createShape( 0xff0000 );
			yellowShape = createShape( 0xffff00 );
		}
		
		//是否被选中
		public function set isSelect( value:Boolean ):void
		{
			this._isSelect = value;
			if ( grid.contains( redShape ) )
			{
				grid.removeChild( redShape );
			}
			if ( value && item )
			{
				if ( !grid.contains( yellowShape ) )
				{
					grid.addChild( yellowShape );
				}
			}
			else
			{
				if ( grid.contains( yellowShape ) )
				{
					grid.removeChild( yellowShape );
				}
			}
		}
		
		public function get isSelect():Boolean
		{
			return this._isSelect;
		}
		
		private function addStageHandler( evt:Event ):void
		{
			grid.addEventListener( MouseEvent.MOUSE_OVER,overGridHandler );
			grid.addEventListener( MouseEvent.MOUSE_OUT,outGridHandler );
			grid.addEventListener( MouseEvent.CLICK,clickGridHandler );
			grid.addEventListener( MouseEvent.DOUBLE_CLICK,doubleClickHandler );
			
			this.addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		//添加物品
		public function addItem( _item:ItemBase ):void
		{
			clearGrid();
			this.item = _item;
			grid.addChild( _item );
		}
		
		//移除item
		public function removeItem():void
		{
			clearGrid();
			this.item = null;
			if ( TreasureData.selectGrid == this )
			{
				TreasureData.selectGrid = null;
			}
		}
		
		private function clickGridHandler( evt:MouseEvent ):void
		{
			if ( this._isSelect ) return;
			if ( item )
			{
				if ( TreasureData.selectGrid && TreasureData.selectGrid != this )
				{
					TreasureData.selectGrid.isSelect = false;
				}
				isSelect = true;
				TreasureData.selectGrid = this;
			}
			else
			{
				isSelect = false;
				if ( TreasureData.selectGrid )
				{
					TreasureData.selectGrid.isSelect = false;
				}
				TreasureData.selectGrid = null;
			}
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( TreasureData.CLICK_SINGLE_GRID,{ gridUnit:this } );
		}
		
		private function doubleClickHandler( evt:MouseEvent ):void
		{			
			if ( this.item )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( TreasureData.DOUBLE_CLICK_GRID,{ gridUnit:this } );
			}
		}
		
		private function overGridHandler( evt:MouseEvent ):void
		{
			if ( this.grid.contains( yellowShape ) ) return;
			if ( !this.grid.contains( redShape ) )
			{
				grid.addChild( redShape );
			}
		}
		
		private function outGridHandler( evt:MouseEvent ):void
		{
			if ( this.grid.contains( redShape ) )
			{
				grid.removeChild( redShape );
			}
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			grid.removeEventListener( MouseEvent.MOUSE_OVER,overGridHandler );
			grid.removeEventListener( MouseEvent.MOUSE_OUT,outGridHandler );
			grid.removeEventListener( MouseEvent.CLICK,clickGridHandler );
			grid.removeEventListener( MouseEvent.DOUBLE_CLICK,doubleClickHandler );
			
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function createShape( color:uint ):Shape
		{
			var shape:Shape = new Shape();
			shape.graphics.clear();
			shape.graphics.lineStyle( 1,color );
			shape.graphics.lineTo( 0,0 );
			shape.graphics.lineTo( 35,0 );
			shape.graphics.lineTo( 35,35 );
			shape.graphics.lineTo( 0,35 );
			shape.graphics.lineTo( 0,0 );
			return shape;
		}
		
		private function clearGrid():void
		{
			var aDes:Array = [];
			for ( var i:uint=0; i<grid.numChildren; i++ )
			{
				if ( grid.getChildAt( i ) is ItemBase )
				{
					aDes.push( grid.getChildAt( i ) );		
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