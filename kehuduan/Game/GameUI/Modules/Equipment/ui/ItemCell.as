package GameUI.Modules.Equipment.ui
{
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
	import GameUI.UIUtils;
	import GameUI.View.Components.UISprite;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	
	public class ItemCell extends UISprite implements IDataGridCell
	{
		protected var _view:MovieClip;
		protected var _imgType:String="";
		protected var _useItem:UseItem;
		protected var data:*;
		
		
		public function ItemCell()
		{
			this._view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			this.mouseEnabled=false;
			this._view.mouseEnabled=false;
			this.addChild(this._view);		
		}

		public function set cellData(value:Object):void
		{	
			this.data=value;
			if(value==1){
				if(this._useItem!=null)this._view.removeChild(this._useItem);
				this._useItem=null;
			}else{
				var detailData:Object=value.detailData;
				if(this._useItem!=null)this._view.removeChild(this._useItem);
				this._useItem=null;
				this._imgType=detailData.type;
				this._useItem=new UseItem(0,detailData.type,null);
				this._useItem.x=this._useItem.y=2;
				_useItem.Id = detailData.id;
				_useItem.Type = detailData.type;
				_useItem.name = "bagQuickKeyItem_"+detailData.id;
				_useItem.addEventListener(MouseEvent.CLICK,onMouseClick);
				_useItem.mouseEnabled=true;
				_useItem.IsLock= value.isLock;
				if(value.usedNum){
					if(detailData.amount==value.usedNum){
						_useItem.IsLock=true;
					}
					_useItem.Num=uint(detailData.amount-value.usedNum);
				}else{
					_useItem.Num=detailData.amount;
				}				
				this._view.addChild(this._useItem);
			}
		}
		
		
		protected function onMouseClick(e:MouseEvent):void{
			if(this._useItem.IsLock)return;
			var e1:GridCellEvent=new GridCellEvent(GridCellEvent.GRIDCELL_CLICK);
			e1.data=this.data;
			this.dispatchEvent(e1);			
		}
		
		public function dispose():void
		{
			
		}
		
		public function get cellWidth():uint
		{
			return this._view.width;
		}
		
		public function get cellHeight():uint
		{
			return this._view.height;
		}
		
	}
}