package GameUI.Modules.Chat.Command
{
//	import Controller.TalkController;
	
	import Controller.TalkController;
	
	import GameUI.Modules.Chat.Data.ChatData;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.display.Bitmap;
	import flash.display.BlendMode;
	import flash.display.DisplayObjectContainer;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class HeadTalkCommand extends SimpleCommand
	{
		public static const NAME:String = "HeadTalkCommand";
		private var regJian:RegExp = /(<\d_.*?>)/g;
		
		public function HeadTalkCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var infoObj:Object = notification.getBody();
			if ( !infoObj.talkObj ) return;
			
			var talkerName:String = infoObj.talkObj[0];
			// 获取人物
			var play:GameElementAnimal = TalkController.talk(talkerName); 
			if(play != null)
			{
				var container:Sprite = new Sprite();
				container.mouseEnabled = false;
				container.mouseChildren = false;
				container.blendMode = BlendMode.LAYER;
				var talkerSay:String = infoObj.talkObj[3];
				dealInfo(talkerSay, infoObj.nColor,container,play);
//				TalkController.createMC(play,container);
			}
		}
		
		private function dealInfo( talkerSay:String,defaultColor:uint,container:Sprite,play:GameElementAnimal ):void
		{
			var realSay:String = "";
			var totalArr:Array = talkerSay.split( regJian );
			for each ( var singleStr:String in totalArr )
			{
//				if ( singleStr.length == 0 ) continue;
				if ( regJian.test( singleStr ) )
				{
					var infoArr:Array = strToArr( singleStr ); 
					var t:Array = ChatData.CHAT_COLORS;
					var l:int = infoArr.length;
					realSay += "<font color = '#" + ChatData.CHAT_COLORS[ infoArr[ infoArr.length - 1 ]  ].toString( 16 ) +"'>" + infoArr[ 1 ] + "</font>";
				}
				else 
				{
					if ( singleStr.length > 0  )
					{
						realSay += "<font color = '#" + defaultColor.toString( 16 ) +"'>" + singleStr + "</font>";
					}
				}
			}
			
			var talk_txt:TextField = new TextField();
			var format2:TextFormat = new TextFormat();
			format2.align 		   = TextFormatAlign.LEFT;
			format2.size		   = 12;
			format2.font		   = "宋体";
			format2.leading        = 4;
			talk_txt.defaultTextFormat = format2;
		    talk_txt.cacheAsBitmap     = true;
			talk_txt.mouseEnabled      = false;
			talk_txt.width             = 120;
			talk_txt.multiline         = true;
			talk_txt.wordWrap          = true;
			talk_txt.selectable 	   = false;
			talk_txt.textColor         = 0xffffff;		
			talk_txt.autoSize          = TextFieldAutoSize.LEFT;
//			trace ( " realSay  是个啥玩意：  "+realSay );
			
			talk_txt.htmlText = realSay;
			talk_txt.filters = Font.Stroke(0x000000);	
			
			container.graphics.clear();
			container.graphics.beginFill( 0xff0000,0 );
			container.graphics.drawRect( 0,0,talk_txt.textWidth,talk_txt.textHeight );
			container.graphics.endFill();
			talk_txt.height = talk_txt.textHeight;
			container.addChild( talk_txt );	
			setFace( talk_txt,container );
			
			
			TalkController.createMC(play,container);
//			trace ( talk_txt.htmlText );
		}
		
		private function strToArr(str:String):Array
		{
			var arr:Array=[];
			if(str.length>2)
			{
				str=str.substring(1,str.length-1);
				arr=str.split("_");
			}
			return arr;
		}
		
		private function setFace(textFeild:TextField,container:Sprite):void 
		{
			var regExp:RegExp=/(\\\d{3})/g;
			var chatArr:Array=textFeild.text.split(regExp);
			if (! chatArr||chatArr.length==0) {
				return;
			}
			var index:int=0;
			var pos:int=0;
			var count:int = 0;
			while (index<chatArr.length) {
				if (regExp.test(chatArr[index])&&int(int(chatArr[index].slice(1,4)))<=32) { 
					if(count==5) {
						break;
					}
					setBmpMask(textFeild.getCharBoundaries(pos),container);
//					setBmpMask(textFeild.getCharBoundaries(0),container);
					addImg(chatArr[index].slice(1,4), textFeild.getCharBoundaries(pos),container);
					count++;
					pos = pos + chatArr[index].length;
				} else {
					pos = pos + chatArr[index].length;
				}
				index++;
			}			
		}
		
		private function addImg(faceName:String, rect:Rectangle,container:Sprite):void
		 {
			var face:Bitmap = new Bitmap();
			face.bitmapData = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("F"+faceName);
			face.x = rect.x;
			face.y = rect.y - 2; 
			container.addChild(face);
		}
		
		private function setBmpMask(rect:Rectangle,container:DisplayObjectContainer):void
		{ 
			if(rect) 
			{
				var s:Shape = new Shape();
				s.graphics.beginFill(0xffffff, 1);
				s.graphics.drawRect( rect.x-1, rect.y, rect.width+19, rect.height );  //rect.y+1
				s.graphics.endFill(); 
				s.blendMode = BlendMode.ERASE;
				container.addChild(s);
			}
		}
		
	}
}