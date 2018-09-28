package GameUI.Modules.MusicPlayer.View
{
	import GameUI.View.UIKit.components.ItemRenderer;
	
	import flash.display.Graphics;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;

	public class MusicPlayerPlaylistItemRenderer extends ItemRenderer
	{
		protected var txtNum:TextField;
		protected var txtTitle:TextField;
		protected var txtLength:TextField; 
		
		protected var _highlighted:Boolean = false;
		
		public function get highlighted():Boolean
		{
			return _highlighted;
		}
		
		public function set highlighted(value:Boolean):void
		{
			_highlighted = value;
			
			requestRedraw();
		}
		
		public function MusicPlayerPlaylistItemRenderer()
		{
			super();
			
			this.buttonMode = this.useHandCursor = true;
			
			txtNum = new TextField();
			txtTitle = new TextField();
			txtLength = new TextField();
			
			txtNum.y = txtTitle.y = txtLength.y = 1;
			txtNum.height = txtTitle.height = txtLength.height = 20;
			txtNum.x = 5;
			txtNum.width = 18;
			txtTitle.x = 23;
			txtTitle.width = 162;
			txtLength.x = 185;
			txtLength.width = 50;
			
			txtNum.mouseEnabled = txtTitle.mouseEnabled = txtLength.mouseEnabled = false;
			
			this.addChild(txtNum);
			this.addChild(txtTitle);
			this.addChild(txtLength);
			
			this.doubleClickEnabled = true;
			this.name = "musicPlayerPlaylistItemRenderer";
		}
		
		override protected function requestRedraw():void
		{
			super.requestRedraw();
				
			var bgColor:uint = 0x000000;
			var bgAlpha:Number = 1;
			
			if (this.selected)
			{
				bgColor = 0xCACAAB;
				bgAlpha = .2;
			}
			else if (this.index % 2 == 1)
			{
				bgColor = 0x333333;
				bgAlpha = .2;
			}
			
			var g:Graphics = this.graphics;
			g.clear();
			g.beginFill(bgColor, bgAlpha);
			g.drawRect(0, 0, 220, 20);
			g.endFill();
			
			var tf:TextFormat = new TextFormat(null, null, this.highlighted ? 0x00FF00 : 0xFFFFFF);
			var tf2:TextFormat = new TextFormat(null, null, this.highlighted ? 0x00FF00 : 0xFFFFFF, null, null, null, null, null, TextFormatAlign.RIGHT);
			
			txtNum.text = (this.index + 1).toString();
			txtTitle.text = data.nameWithScene;
			txtLength.text = getLengthString(data.length);
			
			txtNum.setTextFormat(tf2);
			txtTitle.setTextFormat(tf);
			txtLength.setTextFormat(tf);
		}
		
		protected function getLengthString(l:int):String
		{
			var m:String = int(l / 60).toString();
			var s:String = int(l % 60).toString();
			
			if (m.length == 1) m = "0" + m;
			if (s.length == 1) s = "0" + s;
			
			return m + ":" + s;
		}	
	}
}