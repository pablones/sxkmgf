package GameUI.Modules.PetPlayRule.PetRuleController.UI
{
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;

	public class PetRuleDataGrid extends Sprite
	{
		protected var _dataRro:Array;
		protected var _cells:Array;
		protected var _cacheCells:Array;
		protected var _scrollPane:UIScrollPane;
		protected var _container:UISprite;
		protected var _w:uint;
		protected var _h:uint;
		
		/** 渲染器类*/
		public var rendererClass:Class;
		/** 水平 */
		public var hPadding:uint=0;
		/** 垂直 */
		public var vPadding:uint=0;		
		
		public function PetRuleDataGrid(w:uint,h:uint)
		{
			super();
			this._w=w;
			this._h=h;
			this.createChildren();
		}
		
		/**
		 * 创建子类 
		 * 
		 */		
		protected function createChildren():void{
			this._cacheCells=[];
			this._cells=[[]];
			this._container=new UISprite();
			this._container.width=this._w-16;
			this._container.height=100;
			this._scrollPane=new UIScrollPane(this._container);
			this._scrollPane.width=this._w;
			this._scrollPane.height=this._h;
			this._scrollPane.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.addChild(this._scrollPane);
			this.mouseEnabled = false;
			this._scrollPane.mouseEnabled = false;
		}
		
		/**
		 * 重绘 
		 * 
		 */		 
		protected function toRepaint():void{
			this.removeAllCells();
			this.createCells();
			this.doLayout();
		}
		
		/**
		 *  创建渲染
		 * 
		 */		
		protected function createCells():void{
			var len1:uint=this._dataRro.length;
			for(var i:uint=0;i<len1;i++){
				var arr:Array=this._dataRro[i] as Array;
				var len2:uint=arr.length;
				for(var j:uint=0;j<len2;j++){
					var cell:PetRuleIDataGridCell=this.getCell();
					cell.cellData=this._dataRro[i][j];
					(cell as DisplayObject).addEventListener(GridCellEvent.GRIDCELL_CLICK,onGridCellClick);
				    this._container.addChild(cell as DisplayObject);
				    if(this._cells[i]==null)this._cells[i]=[];
					this._cells[i].push(cell);
				}
			}	
		}
		
		protected function onGridCellClick(e:GridCellEvent):void{
			var e1:GridCellEvent=new GridCellEvent(e.type);
			e1.data=e.data;
			this.dispatchEvent(e1);
		}
		
		/**
		 * 获得渲染器 
		 * @return 
		 * 
		 */		
		protected function getCell():PetRuleIDataGridCell{
			var cell:PetRuleIDataGridCell=this._cacheCells.shift();
			if(cell==null)cell=new rendererClass();
			return cell;
		}
		
		
		/**
		 * 布局 
		 * 
		 */		
		protected function doLayout():void{
			var len1:uint=this._cells.length;
			var y:Number=this.vPadding;
			for(var i:uint=0;i<len1;i++){
				var arr:Array=this._cells[i];
				var len2:uint=arr.length;
				var x:Number=this.hPadding;
				for(var j:uint=0;j<len2;j++){
					var cell:PetRuleIDataGridCell=this._cells[i][j];
					cell["x"]=x;
					cell["y"]=y;
					x+=cell.cellWidth+hPadding;
				}
				y+=cell.cellHeight+this.vPadding;
			}
			this._container.height=y;
			this._scrollPane.refresh();
		}
		
		/**
		 * 清除所有 
		 * 
		 */		
		protected function removeAllCells():void{
			var len1:uint=this._cells.length;
			for(var i:uint=0;i<len1;i++){
				var arr:Array=this._cells[i];
				var len2:uint=arr.length;
				for(var j:uint=0;j<len2;j++){
					var cell:PetRuleIDataGridCell=this._cells[i][j];
					if(this._container.contains(cell as DisplayObject)){
						this._container.removeChild(cell as DisplayObject);
					}
					cell.dispose();
					this._cacheCells.push(cell);
				}
			}
			this._cells=[];
		}
		
		/**
		 *  设置数据 
		 * @param value
		 * 
		 */		
		public function set dataPro(value:Array):void{
			this._dataRro=value;
			this.toRepaint();
		}
		
		/**
		 * 刷新 
		 * 
		 */		
		public function refresh():void{
			this.toRepaint();
		}

	}
}