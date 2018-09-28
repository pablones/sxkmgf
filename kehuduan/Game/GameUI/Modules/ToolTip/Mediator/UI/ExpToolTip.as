package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	public class ExpToolTip implements IToolTip
	{
		private var toolTip:Sprite;
		private var back:MovieClip;
		private var content:Sprite;
		
		public function ExpToolTip(view:Sprite)
		{
			toolTip = view;
		}
		
		public function GetType():String
		{
			return "ExpToolTip";
		}
		
		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackSmall");
			//back.width = 150;
			toolTip.addChild(back);
			setContent();
			upDatePos();
		}
		
		private function setContent():void
		{
			content = new Sprite();
			var contents:Array = [{text:" " + GameCommonData.wordDic[ "mod_too_med_ui_exp_set_1" ]+GameCommonData.Player.Role.Exp+"/"+UIConstData.ExpDic[GameCommonData.Player.Role.Level] + "", color:IntroConst.COLOR}];    //经验:
			showContent(contents);
			toolTip.addChild(content);
		}
		
		private function showContent(contents:Array):void
		{
			for(var i:int = 0; i<contents.length; i++)
			{
				var tf:TextField = new TextField();
				tf.width = toolTip.width;
				tf.wordWrap = false;
				tf.x = 2;
				tf.y = 1;
				tf.filters = Font.Stroke();
				tf.textColor = contents[i].color;
				tf.htmlText = contents[i].text;
				tf.autoSize = TextFieldAutoSize.LEFT;
				tf.defaultTextFormat = setFormat();
				tf.setTextFormat(setFormat());
				content.addChild(tf);
			}
		}
		
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:Number = 0;
			for(; i < toolTip.numChildren; i++)
			{
				var child:DisplayObject = toolTip.getChildAt(i);
				child.y = ypos;
				ypos += child.height;
			}
			back.width = toolTip.width+4;
			back.height = toolTip.height-8;
		}
		
		private function setFormat():TextFormat
		{
			var format:TextFormat = new TextFormat();
			format.align = TextFormatAlign.CENTER;
			format.font = "宋体";       //宋体
			return format;
		}
		
		public function Update(obj:Object):void
		{
			
		}
		
		public function Remove():void
		{
			var count:int = toolTip.numChildren-1;
			while(count>=0)
			{
				toolTip.removeChildAt(count);
				count--;	
			}
			toolTip = null;
			back = null;
			content = null;
		}
		
		public function BackWidth():Number
		{
			return back.width;
		}
	}
}