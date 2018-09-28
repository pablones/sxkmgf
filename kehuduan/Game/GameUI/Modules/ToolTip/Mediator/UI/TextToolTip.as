package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	public class TextToolTip implements IToolTip
	{
		private var toolTip:Sprite = null;
		private var data:String = "";
		private var back:MovieClip;
		private var content:Sprite;
		
		private var max:uint = 150;
		
		public var fixedWidth:Number = NaN;
		
		public function TextToolTip(view:Sprite, data:String)
		{
			this.toolTip = view;
			this.data = data;
		}
		
		public function GetType():String
		{
			return "TextToolTip";
		}

		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackSmall");
			back.width = (isNaN(fixedWidth) ? max : fixedWidth);
			toolTip.addChild(back);
			setContent();
			upDatePos();
		}
		
		private function setContent():void
		{
			content = new Sprite();
			var contents:Array = [{text:data, color:IntroConst.COLOR}];
			showContent(contents);
			toolTip.addChild(content);
		}
		
		private function showContent(contents:Array):void
		{
			for(var i:int = 0; i<contents.length; i++)
			{
				var TF:TextFormat = new TextFormat();
					TF.leading = 3;
//				if(contents[i]==null ||contents[i]==undefined)continue;
				var tf:TextField = new TextField();
				tf.defaultTextFormat = setFormat();
				tf.autoSize=TextFieldAutoSize.LEFT;
//				tf.setTextFormat(setFormat());
				tf.width = toolTip.width;
				tf.wordWrap = true;
				tf.x = 2;
				tf.y = 2;
				tf.filters = Font.Stroke(0);
//				tf.autoSize = TextFieldAutoSize.LEFT;
				tf.textColor = contents[i].color;
				tf.htmlText = contents[i].text ? contents[i].text:"";				//杨龙在11.4号改,原text改为htmlText
				if(max > tf.textWidth)
				{
					max = tf.textWidth+10;
				}
				tf.setTextFormat( TF );
//				max=Math.max(tf.textWidth+10,max);
				content.addChild(tf);
			}
		}
		
		private function setFormat():TextFormat
		{
			var format:TextFormat = new TextFormat();
			format.font = "宋体";          //宋体
			format.align = TextFormatAlign.LEFT;
			return format;
		}
		
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:Number = 0;
			back.width = (isNaN(fixedWidth) ? max : fixedWidth);
//			back.width = content.width;
			back.height = content.height+4;
		}
		
		public function BackWidth():Number
		{
			return back.width;
		}
		
		public function Update(obj:Object):void
		{
		}
		
		public function Remove():void
		{
		}
		
	}
}