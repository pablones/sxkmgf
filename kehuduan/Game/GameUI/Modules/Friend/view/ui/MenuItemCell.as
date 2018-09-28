package GameUI.Modules.Friend.view.ui
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;

	public class MenuItemCell extends Sprite
	{
		
		/** 内容MC */
		public var content:MovieClip;
		/** 数据绑定 */
		public var data:Object={};
		
		public function MenuItemCell()
		{
			super();
			this.buttonMode=true;
			this.content=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MenuItemRenderer");
			
			this.addChild(this.content);
			(this.content.txt_name as TextField).mouseEnabled=false;
			this.addEventListener(MouseEvent.ROLL_OVER,onMouseRollOverHandler);
			this.addEventListener(MouseEvent.ROLL_OUT,onMouseRollOutHandler);			
		}
		
		
		/**
		 * 设置渲染器文本值 
		 * @param value
		 * 
		 */		
		public function setText(value:String):void{
			(this.content.txt_name as TextField).htmlText=value;
		}
		
		protected function onMouseRollOverHandler(e:MouseEvent):void{
//			(this.content.bgMc as MovieClip).alpha=1;
		}
		
		protected function onMouseRollOutHandler(e:MouseEvent):void{
//			(this.content.bgMc as MovieClip).alpha=0;
		}
		
	}
}