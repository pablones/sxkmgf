package GameUI.Modules.Equipment.ui
{
	import GameUI.Modules.Equipment.ui.event.ListCellEvent;
	import GameUI.View.Components.UISprite;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;

	/**
	 *   列表组件渲染器
	 * @author felix
	 * 
	 */	
	public class ListCell extends Sprite implements IListCell
	{
		protected var _desTf:TextField;
		protected var _defaultSkin:MovieClip;
		protected var _mouseMoveSkin:MovieClip;
		protected var _mouseDownSkin:MovieClip;
		protected var _bgCell:UISprite;
		protected var _cellData:Object;
		
		
		public function ListCell()
		{
			super();
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.addEventListener(MouseEvent.ROLL_OVER,onMouseOver);
			this.addEventListener(MouseEvent.ROLL_OUT,onMouseOut);
			this.addEventListener(MouseEvent.MOUSE_DOWN,onMouseClick);
			
			this._bgCell=new UISprite();
			this.addChild(this._bgCell);
			
//			this._defaultSkin=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("defaultListSkin");
			this._defaultSkin=new MovieClip();
			this._mouseMoveSkin=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mouseMoveListSkin");
			this._mouseDownSkin=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mouseDownListSkin");
			this._defaultSkin.mouseEnabled=false;
			this._mouseDownSkin.mouseEnabled=false;
			this._mouseMoveSkin.mouseEnabled=false;
			this._bgCell.addChild(this._defaultSkin);
			
			this._defaultSkin.width=this._bgCell.width;
			this._defaultSkin.height=this._bgCell.height;
			this._mouseMoveSkin.width=this._bgCell.width;
			this._mouseMoveSkin.height=this._bgCell.height;
			this._mouseDownSkin.width=this._bgCell.width;
			this._mouseDownSkin.height=this._bgCell.height;
			
			this._desTf=new TextField();
			this._desTf.mouseEnabled=false;
			this._desTf.mouseWheelEnabled=false;
			this._desTf.textColor=0xe2cca5;
			this._desTf.autoSize=TextFieldAutoSize.LEFT;
			this._desTf.text=GameCommonData.wordDic[ "mod_equ_ui_lis_cre" ];//"3级强化符"
			this.doLayout();
			this.addChild(this._desTf);
					
		}
		
		protected function onMouseOver(e:MouseEvent):void{
			this.clearAllSkin();
			this._bgCell.addChild(this._mouseMoveSkin);
		}
		
		protected function onMouseOut(e:MouseEvent):void{
			this.clearAllSkin();
			this._bgCell.addChild(this._defaultSkin);
		}
		
		protected function onMouseClick(e:MouseEvent):void{
			var e1:ListCellEvent=new ListCellEvent(ListCellEvent.LISTCELL_CLICK);
			e1.data=this._cellData;
			this.dispatchEvent(e1);                                   
			this.clearAllSkin();
			this._bgCell.addChild(this._mouseDownSkin);
		}
		
		protected function clearAllSkin():void{
			if(this._bgCell.contains(this._defaultSkin))this._bgCell.removeChild(this._defaultSkin);
			if(this._bgCell.contains(this._mouseMoveSkin))this._bgCell.removeChild(this._mouseMoveSkin);
			if(this._bgCell.contains(this._mouseDownSkin))this._bgCell.removeChild(this._mouseDownSkin);
		}
		
		
		protected function toRepaint():void{
			this._desTf.text=this._cellData["Name"];	
			this.addEventListener(MouseEvent.ROLL_OVER,onMouseOver);
			this.addEventListener(MouseEvent.ROLL_OUT,onMouseOut);
			this.addEventListener(MouseEvent.CLICK,onMouseClick);
		}
		
		protected function doLayout():void{
			
			this._defaultSkin.width=this._bgCell.width;
			this._defaultSkin.height=this._bgCell.height;
			this._mouseMoveSkin.width=this._bgCell.width;
			this._mouseMoveSkin.height=this._bgCell.height;
			this._mouseDownSkin.width=this._bgCell.width;
			this._mouseDownSkin.height=this._bgCell.height;
			
			this._desTf.x=((this._bgCell.width-this._desTf.width)/2);
			this._desTf.y=((this._bgCell.height-this._desTf.height)/2);
		}
		

		////////////////////////////////////////////////////
		//interface
		public function set cellData(value:Object):void
		{
			this._cellData=value;
			this.toRepaint();
		}
		
		public function dispose():void
		{
			this.removeEventListener(MouseEvent.MOUSE_OVER,onMouseOver);
			this.removeEventListener(MouseEvent.MOUSE_OUT,onMouseOut);
			this.removeEventListener(MouseEvent.CLICK,onMouseClick);
		}
		
		public function set cellWidth(value:uint):void
		{
			this._bgCell.width=value;
			this.doLayout();
		}
		
		public function set cellHeight(value:uint):void
		{
			this._bgCell.height=value;
			this.doLayout();
		}
		public function get cellWidth():uint
		{
			return this._bgCell.width;
		}
		
		public function get cellHeight():uint
		{
			return this._bgCell.height;	
		}
		
	}
}