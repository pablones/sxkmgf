package GameUI.Modules.Equipment.ui
{
	import GameUI.Modules.Equipment.ui.event.ListCellEvent;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;

	/**
	 * 列表组件 
	 * @author felix
	 * 
	 */	
	public class UIList extends Sprite
	{
		/**
		 * -1:有多少显示多少，按最大化显示。  n:规定显示多少行。
		 */		
		protected var _proxy:int=-1;
		
		protected var _dataRro:Array;
		protected var _cells:Array;
		protected var _cacheCells:Array;
		protected var _scrollPane:UIScrollPane;
		protected var _container:UISprite;
		protected var _bgList:MovieClip;
		protected var _w:uint;
		protected var _h:uint;
		
		/** 渲染器类*/
		public var rendererClass:Class;
		/** 水平 */
		public var hPadding:uint=1;
		/** 垂直 */
		public var vPadding:uint=0;		
		
		
  		
		public function UIList(w:uint,h:int,proxy:int=-1)
		{
			super();
			this.name="LIST";
			this._w=w;
			if(proxy>0){
				this._h=proxy*h;
			}else{
				this._h=h;
				
			}
			this._proxy=proxy;
			this.createChildren();
		}
		
		protected function createChildren():void{
			this._cells=[];
			this._cacheCells=[];
			this._bgList=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ListBack");
			this._bgList.width=this._w;
			this.addChild(this._bgList);
			this._container=new UISprite;
			if(this._proxy==-2){
				this._bgList.visible=false;
			}
			if(this._proxy==-1){
				this._container.width=this._w;
				this.addChild(this._container);
			}else{
				
				this._container.width=this._w-16;
				this._scrollPane=new UIScrollPane(this._container);
				this._scrollPane.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
				this._scrollPane.width=this._w;
				this._scrollPane.height=this._h;
				this.addChild(this._scrollPane);
			}	
		}
		
		
		protected function toRepaint():void{
			this.removeAllCells();
			this.createCells();
			this.doLayout();
		}
		
		
		protected function doLayout():void{
			if(this._proxy==-1){
				var y:Number=this.vPadding;
				for each(var cell:IListCell in this._cells){
					cell.cellWidth=this._w-2*this.hPadding;
					cell.cellHeight=18;
					cell["x"]=this.hPadding;
					cell["y"]=y;
					y+=Math.ceil(cell.cellHeight+this.vPadding);
				}
				this._bgList.height=y+this.vPadding+Math.floor((y+this.vPadding)*0.01);
			}else{
				var y1:Number=this.vPadding;
				for each(var cell1:IListCell in this._cells){
					cell1.cellWidth=this._w-2*this.hPadding-16;
					cell1.cellHeight=18;
					cell1["x"]=this.hPadding;
					cell1["y"]=y1;
					y1+=Math.ceil(cell1.cellHeight+this.vPadding);
				}
				this._container.height=y1+this.vPadding+Math.floor((y1+this.vPadding)*0.01);
				this._bgList.height=this._h;
				this._scrollPane.refresh();
			}	
		}
		
		
		protected function getCell():IListCell{
			var cell:IListCell=this._cacheCells.shift();
			if(cell==null)cell=new rendererClass();
			return cell;			
		}
		
		
		protected function createCells():void{
			var len:uint=this._dataRro.length;
			for(var i:uint=0;i<len;i++){
				var cell:IListCell=this.getCell();
				cell.cellData=this._dataRro[i];
				this._cells.push(cell);
				(cell as DisplayObject).addEventListener(ListCellEvent.LISTCELL_CLICK,onListCellClick);
				//todo 监听事件
				this._container.addChild(cell as DisplayObject);	
			}
			
		}	
		
		protected function onListCellClick(e:ListCellEvent):void{
			var e1:ListCellEvent=new ListCellEvent(ListCellEvent.LISTCELL_CLICK);
			e1.data=e.data;
			this.dispatchEvent(ListCellEvent(e1));
		}
		
		protected function removeAllCells():void{
			var len:uint=this._cells.length;
			for(var i:uint=0;i<len;i++){
				var displayObj:IListCell=this._cells[i];
				if(this._container.contains(displayObj as DisplayObject)){
					this._container.removeChild(displayObj as DisplayObject);
				}
				displayObj.dispose();
				this._cacheCells.push(displayObj);
			}
			this._cells=[];
		}
		
		/**
		 * 设置改变数据类型 
		 * @param value
		 * 
		 */		
		public function set dataPro(value:Array):void{
			this._dataRro=value;
			this.toRepaint();
		}
		
		/**
		 *	设置选中的选项 
		 * @param value
		 * 
		 */		
		public function setSelectedIndex(value:uint):void{
			var e1:ListCellEvent=new ListCellEvent(ListCellEvent.LISTCELL_CLICK);
			e1.data=this._dataRro[value];
			this.dispatchEvent(ListCellEvent(e1)); 
		}

	}
}