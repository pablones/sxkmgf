package GameUI.Modules.Chat.UI
{
	import GameUI.Modules.Chat.Data.ChatData;
	
	import flash.display.Bitmap;
	import flash.display.BlendMode;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.TextEvent;
	import flash.filters.GlowFilter;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.ByteArray;


	/**
	 *  聊天框渲染器
	 * @author felix
	 * 
	 */	
	public class ChatCellText extends Sprite
	{
		
		/**
		 * 聊天ID 
		 */		
		protected var id:uint;
		/** 描述文字框 */
		protected var tf:TextField;
		/**{pos:链接文字从哪个字符的位置开始,des:要显示的描述的文字内容,info:链接所携带的信息量} */
		protected var infoArr:Array=[];
		/** {startIndex:  ,endIndex: ,color:   }*/
		protected var colorArr:Array=[];
		/** 真正要显示的字符串信息(包括表情的掩码信息)*/
		protected var realStr:String="";
		/** 源字符串 */
		protected var sourceInfo:String;
		/** 默认字体显示颜色*/
		protected var defaultColor:uint;
		/** 缓存数组  */
		private var cacheCellArr:Array=[];
		/** 宽度 */
		private var _width:Number = 320;
		
		
		public function ChatCellText(sourceInfo:String,defaultColor:uint,preferredWidth:Number=NaN,id:uint=0)
		{
			super();
			this.id=id;
			this.mouseEnabled = false;
			this.sourceInfo=sourceInfo;
			this.defaultColor=defaultColor;
			this.blendMode=BlendMode.LAYER;
			if (!isNaN(preferredWidth)) _width = preferredWidth;
			this.createChildren();
		}
		
		
		protected function createChildren():void{
			var reg:RegExp=/(<\d_.*?>)/g;
			var arr:Array=sourceInfo.split(reg);
			
			var linePos:uint=0;      //行中的位置指针（行中最大54个字符）;
			
			for each(var s:String in arr){
				if(reg.test(s)){   //信息部分
					var tempArr:Array=strToArr(s);
					if(uint(tempArr[0])==3){     //只是显示颜色的信息
						colorArr.push({startIndex:realStr.length,endIndex:realStr.length+tempArr[1].length,color:tempArr[2]});
						realStr+=tempArr[1];
						linePos+=getStrLength(tempArr[1]);    //增加指针的长度()
						linePos=linePos%52;
					}else{                       //有链接的信息  type=0,1,2,4
						var tempLinePos:uint=linePos;
						if(tempLinePos+getStrLength(tempArr[1])>52){ 
							realStr+="\n"+tempArr[1];
							linePos=getStrLength(tempArr[1]);
						}else{
							var tempChangeStr:String=tempArr[1];
							tempChangeStr=tempChangeStr.replace("]"," ");
							realStr+=tempChangeStr;
							linePos+=getStrLength(tempArr[1]);
						}
						var tempS:String=s.substring(1,s.length-1);
						if(tempArr[0]==0 || tempArr[0]==1 || tempArr[0]==2){
							infoArr.push({pos:realStr.length-(tempArr[1].length),info:tempS,des:tempArr[1],color:ChatData.CHAT_COLORS[tempArr[tempArr.length-1]],type:tempArr[0]});				
						}else if(tempArr[0]==4){
							infoArr.push({pos:realStr.length-(tempArr[1].length),info:tempS,des:tempArr[1],color:tempArr[2],type:tempArr[0],url:tempArr[4],underLine:tempArr[3]});
						}
					}
				}else{             //显示部分
					realStr+=s;
					linePos+=getStrLength(s);
					linePos=linePos%52;
				}		
			}
			
			tf=new TextField();
			tf.textColor=defaultColor;
			tf.autoSize=TextFieldAutoSize.LEFT;
			tf.mouseEnabled=false;
			tf.wordWrap=true;
			tf.defaultTextFormat=textFormat();
			tf.text=realStr;
			tf.width=_width;
			tf.filters=this.stroke();
			this.addChild(tf);
			
			//设字体颜色
			for each(var obj:Object in colorArr){
				tf.setTextFormat(getColorFontFormat(obj.color),uint(obj.startIndex),uint(obj.endIndex));
			}
			//设置链接
			for each(var linkObj:Object in infoArr){
				var showTf:TextField=new TextField();
				showTf.selectable=false;
				showTf.autoSize=TextFieldAutoSize.LEFT;
				showTf.defaultTextFormat = textFormat();
				showTf.addEventListener(TextEvent.LINK,onClickLink);
				
				var html:String;
				if(linkObj.type==0){
					html='<font color="#'+Number(linkObj.color).toString(16)+'"><a href="event:'+linkObj.info+'">'+linkObj.des+'</a></font>';
					showTf.styleSheet=ChatData.NameStyle;
				}else if(linkObj.type==1 || linkObj.type==2 ){
					html='<font color="#'+Number(linkObj.color).toString(16)+'"><a href="event:'+linkObj.info+'_'+this.id+'">'+linkObj.des+'</a></font>';
					showTf.styleSheet=ChatData.HtmlStyle;
				}else if(linkObj.type==4){
					showTf.styleSheet=ChatData.HtmlStyle;
					html='<u><font color="#'+Number(linkObj.color).toString(16)+'"><a href="'+linkObj.url+'" target="_blank">'+linkObj.des+'</a></font></u>';
				}
				showTf.htmlText=html;
				var rect:Rectangle=tf.getCharBoundaries(linkObj.pos);
				showTf.x=rect.x-2;
				showTf.y=rect.y-2;
				showTf.filters=this.stroke();
				this.cacheCellArr.push(showTf);
				this.addChild(showTf);	
			}
			//设置表情
			this.setFace(tf);
		}
		
		/**
		 * 发送自定义事件   
		 * @param e
		 * 
		 */		
		private function onClickLink(e:TextEvent):void{
			this.dispatchEvent(new ChatCellEvent(ChatCellEvent.CHAT_CELLLINK_CLICK,e.text));
		}
		
		/**
		 * 显示表情 
		 * @param textFeild
		 * 
		 */		
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
		}
		
		/**
		 * 设置表情
		 * @param faceName
		 * @param rect
		 * 
		 */		
		private function addImg(faceName:String, rect:Rectangle):void {
			var face:Bitmap = new Bitmap();
			face.bitmapData = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("F"+faceName);
			face.x = rect.x;
			face.y = rect.y - 2; 
			this.addChild(face);
		}
		
		/**
		 * 设置遮罩将表情下面的掩码符遮住 
		 * @param rect
		 * 
		 */		
		private function setBmpMask(rect:Rectangle):void
		{ 
			if(rect) {
				var s:Shape = new Shape();
				s.graphics.beginFill(0xff0000, 1);
				s.graphics.drawRect(rect.x-1, rect.y, rect.width+19, rect.height);  //rect.y+1
				s.graphics.endFill(); 
				s.blendMode = BlendMode.ERASE;
				this.addChild(s);
			}
		}
		
		/**
		 * 获取颜色字体样式
		 * @param color ：字体颜色
		 * 
		 */		
		private function getColorFontFormat(colorIndex:uint):TextFormat{	
			var format:TextFormat=new TextFormat();
			format.color=ChatData.CHAT_COLORS[colorIndex];
			format.size = 12;
			format.leading = 4;
			format.font = "宋体";//"宋体"
			return format;
		}
			
			
		/**
		 * 求字符的字节长度 
		 * @param str
		 * @return 
		 * 
		 */		
		protected function getStrLength(str:String):uint{
			var lenByteArr:ByteArray=new ByteArray();
			lenByteArr.writeMultiByte(str,"ASNI");
			return lenByteArr.length;
		}
			
		/**
		 *  将字符串信息切分成数组(首先去掉两边的尖括号) 
		 * @param str
		 * @return 
		 * 
		 */		
		protected function strToArr(str:String):Array{
			var arr:Array=[];
			if(str.length>2){
				str=str.substring(1,str.length-1);
				arr=str.split("_");
			}
			return arr;
		}
		
		/**
		 * 字体描边滤境 
		 * @param fontStrokeColor
		 * @return 
		 * 
		 */		
		private function stroke(fontStrokeColor:uint=0):Array
		{
			var txtGF:GlowFilter = new GlowFilter(fontStrokeColor,1,2,2,12);
			var filters:Array = new Array();
			filters.push(txtGF);
			return filters;
		}
		
		
		/**
		 * 设置字体的格式
		 * @return 
		 * 
		 */		
		private function textFormat():TextFormat
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.leading = 3;
			tf.font = "宋体";//
			return tf;
		}
		
		/**
		 * 释放 
		 * 
		 */		
		public function dispose():void{
			for each(var tf:TextField in this.cacheCellArr){
				tf.removeEventListener(TextEvent.LINK,onClickLink);
			}
		}
			
		
	}

}