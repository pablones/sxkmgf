package GameUI.Modules.Designation.view.ui
{
	import GameUI.Modules.Designation.Data.DesignationEvent;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class ScrollContainer extends Sprite
	{
		private var cellContainer:Sprite;//装条目的容器
		private var scroll:UIScrollPane;
		private var _dataArr:Array;
		private var _txtArr:Array;
		private var tempTxt:TextContent;
		public function ScrollContainer()
		{
			
		}
		/**添加每个条目的小项*/
		public function createCells(dataArr:Array,_width:Number = 0,_height:Number = 0):void
		{
			_dataArr = dataArr;
			_txtArr = [];
			cellContainer = new Sprite();
			var tag:int = 0;
			for each(var obj:Object in _dataArr)
			{
				var textContent:TextContent = new TextContent(obj);
				textContent.addEventListener(MouseEvent.CLICK,txtClickHandler);
				textContent.x = 1;
				textContent.y = 17*tag;
				cellContainer.addChild(textContent);
				tag ++;
			}

			cellContainer.graphics.clear();
			cellContainer.graphics.beginFill( 0xff0000,0 );
			cellContainer.graphics.drawRect( 0,0,_width,_height );
			cellContainer.graphics.endFill(); 

			scroll = new UIScrollPane(cellContainer);
			scroll.width = _width;
			scroll.height = _height;
			scroll.refresh();
			this.addChild(scroll);
		}
		private function txtClickHandler(e:MouseEvent):void
		{
			if((e.target as TextContent).text == "" || (e.target as TextContent).text == null)
			{
				return;
			}
			if(tempTxt)	tempTxt.background = false;
			(e.currentTarget as TextContent).background = true;
			(e.currentTarget as TextContent).backgroundColor = 0x685848;
			tempTxt = e.currentTarget as TextContent;
			GameCommonData.GameInstance.GameScene.dispatchEvent(new DesignationEvent(DesignationEvent.DESIGNATION_EVENT,false,false,(e.currentTarget as TextContent).dataObj));
		}
		public function getTxtArr():Array
		{
			return _txtArr;
		}
		public function gc():void
		{
			if(_txtArr)
			{
				for each(var tContent:Object in _txtArr)
				{
					(tContent as TextContent).removeEventListener(MouseEvent.CLICK,txtClickHandler);
					tContent = null;
				}
				this.removeChild(scroll);
			}
			cellContainer = null;
			scroll = null;
			_dataArr = null;
			_txtArr = null;
		}
		public override function set width(value:Number):void
		{
			scroll.width = value;
		}
		public override function set height(value:Number):void
		{
			scroll.height = value;
		}
		public override function get height():Number
		{
			return scroll.height;
		}
	}
}