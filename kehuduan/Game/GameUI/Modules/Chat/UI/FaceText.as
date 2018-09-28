package GameUI.Modules.Chat.UI{
	
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.View.ShowMoney;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.*;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.setTimeout;
	
	public class FaceText extends Sprite 
	{
		
		private var textFeild:TextField = null;
		
		public function FaceText():void 
		{
			this.cacheAsBitmap = true;
//			this.blendMode = BlendMode.LAYER;
			this.mouseEnabled = false;
		}
		
		public function set Width(v:Number):void
		{
			textFeild.width = v;
		}
		
		//设置文本
		public function SetHtmlText(s:String, name:String = "", enable:Boolean=false,width:Number=315):void 
		{
			textFeild = new TextField();
			textFeild.name = "faceTextFeild";
			textFeild.cacheAsBitmap = true;
			textFeild.width = width;
			textFeild.x = 0;
			textFeild.y = 2;
			textFeild.wordWrap = true;
			textFeild.selectable = false;
			textFeild.textColor= 0xffffff;
			textFeild.filters = Font.Stroke(0x000000);
			textFeild.multiline = true;
			textFeild.mouseEnabled = false;
			textFeild.autoSize = TextFieldAutoSize.LEFT;
			textFeild.setTextFormat(textFormat());
			textFeild.defaultTextFormat = textFormat();
			if(enable)
			{
				textFeild.mouseEnabled = true;
				textFeild.addEventListener(TextEvent.LINK, selectItemLink);
//				var sheet:StyleSheet = new StyleSheet();
//				sheet.parseCSS("a:hover{color:#00ffff;background-color:black;cursor:none;}");
				textFeild.styleSheet = ChatData.HtmlStyle;
				textFeild.mouseWheelEnabled = false;
				textFeild.addEventListener(MouseEvent.ROLL_OVER, onMouseRoll);
				textFeild.addEventListener(MouseEvent.ROLL_OUT, onMouseOut);
			} 
			//
			this.addChild(textFeild);
//			trace("textFeild.htmlText = s " +s);
			textFeild.htmlText = s;
			//
			if(name != "")
			{
//				var noticeReg:RegExp = /[公告]/g;
				var noticeReg:RegExp = new RegExp("["+GameCommonData.wordDic[ "mod_chat_com_rec_exe_3" ]+"]","g");////公告
				if(noticeReg.test(s))
				{
					setNoticeNameLink(textFeild,name,s);					//专门用来处理公告中的人物名字
				}else
				{
					setNameLink(name);                                          //之前版本
				}			
			}

			setFace(textFeild);			
			ShowMoney.ShowIcon(this, textFeild);
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.leading = 4;
			tf.font = "宋体";//"宋体"
			return tf;
		}
		
		private function selectItemLink(event:TextEvent):void
		{
			this.dispatchEvent(new ItemLinkEvent(event.text));
		}
		
		private function setNameLink(nameStr:String):void
		{
//			trace("nameStr +++++++++ "+nameStr);
			var chatArr:Array = textFeild.text.split(nameStr);
			var pos:int = chatArr[0].length ;
			
			var rect:Rectangle = textFeild.getCharBoundaries(pos);
			if(rect==null){
				setTimeout(setNameLink,50,nameStr);
				return;
			}
			var tf:TextField = new TextField();
			tf.textColor= 0xffffff;
			tf.filters = Font.Stroke(0x000000);
			tf.defaultTextFormat = textFormat();
//			var sheet:StyleSheet = new StyleSheet();
//			sheet.parseCSS("a:hover{color:#ff0000;background-color:black;cursor:none;}");
			tf.styleSheet = ChatData.NameStyle;
			tf.htmlText = '<font color="#ffff00"><a href="event:name_'+nameStr+'">['+nameStr+']</a></font>';
//			trace("tf.htmlText =   " +tf.htmlText);
			tf.wordWrap = false;												//之前为false?????
			tf.selectable = false;
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.addEventListener(MouseEvent.ROLL_OVER, onMouseRoll);
			tf.addEventListener(MouseEvent.ROLL_OUT, onMouseOut);
			if(rect)
			{
				tf.x = rect.x-8;
				tf.y = rect.y;
				setTextMask(rect.x-6, rect.y, tf.width-4, tf.height-4)
				tf.addEventListener(TextEvent.LINK, selectName);
				this.addChild(tf);
			}
		}
		
		//处理帮派公告
		private function setNoticeNameLink(tf:TextField,nameStr:String,s:String):void
		{
			tf.mouseEnabled = true;
			tf.styleSheet = ChatData.HtmlStyle;
			tf.mouseWheelEnabled = false;
			tf.addEventListener(MouseEvent.ROLL_OVER, onMouseRoll);
			tf.addEventListener(MouseEvent.ROLL_OUT, onMouseOut);
			tf.addEventListener(TextEvent.LINK, selectName);
		}
		//很久很久\001实施\002事实上\003事\004sss\011
		//世界]：007[PM] ：很久很久 实施 事实上 事 sss 
		
		private function setTextMask(xpos:int, ypos:int, width:Number, height:Number ):void
		{
			var s:Shape = new Shape();
			s.graphics.beginFill(0xffffff, 1);
			s.graphics.drawRect(xpos, ypos, width, height);
			s.graphics.endFill();
			s.blendMode = BlendMode.ERASE;
			this.addChild(s);
		}
		
		private function onMouseRoll(event:MouseEvent):void
		{
			SysCursor.GetInstance().showSystemCursor();
		}
		
		private function onMouseOut(event:MouseEvent):void
		{
			SysCursor.GetInstance().revert();
		}
		
		public function HideMouse():void
		{
			textFeild.selectable = false;
			textFeild.mouseEnabled = false;
			this.mouseEnabled = false;
			this.mouseChildren = false; 
		}
		
		//
		public function ShowMouse():void
		{
			textFeild.selectable = true;
			textFeild.mouseEnabled = true;
			this.mouseEnabled = true;
			this.mouseChildren = true; 
		}
		
		//设置表情
		private function setFace(textFeild:TextField):void 
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
					setBmpMask(textFeild.getCharBoundaries(pos));
					addImg(chatArr[index].slice(1,4), textFeild.getCharBoundaries(pos));
					count++;
					pos = pos + chatArr[index].length;
				} else {
					pos = pos + chatArr[index].length;
				}
				index++;
			}			
			return;
		}
		
		private function selectName(event:TextEvent):void
		{
			this.dispatchEvent(new TextLinkEvent(event.text));
		}
		
		//在Html上面加表情
		private function addImg(faceName:String, rect:Rectangle):void {
			var face:Bitmap = new Bitmap();
			face.bitmapData = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("F"+faceName);
			face.x = rect.x;
			face.y = rect.y - 2; 
			this.addChild(face);
		}
		
		private function setBmpMask(rect:Rectangle):void
		{ 
			if(rect) {
				var s:Shape = new Shape();
				s.graphics.beginFill(0xffffff, 1);
				s.graphics.drawRect(rect.x-1, rect.y, rect.width+19, rect.height); //rect.y+1
				s.graphics.endFill();
				s.blendMode = BlendMode.ERASE;
				this.addChild(s);
			}
		} 
		
		public function removeAllChild():void
		{
			var index:int = this.numChildren - 1;
			while(index>=0)
			{
				this.removeChildAt(index);
				index--;
			} 
		}
	}
}