package GameUI.View.items
{
	import GameUI.View.Components.UISprite;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Bitmap;
	import flash.display.BlendMode;
	import flash.display.Shape;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.utils.Dictionary;
	import flash.utils.setTimeout;

	public class MoneyItem extends UISprite
	{
		protected var tf:TextField;
		protected const iconDic:Dictionary=new Dictionary();
		protected var shape:Shape;
		
		public function MoneyItem(w:Number=176,h:Number=18)
		{
			super();
			
			iconDic["fx"]=true;
			iconDic["se"]=true;
			iconDic["ss"]=true;
			iconDic["sc"]=true;
			iconDic["ce"]=true;
			iconDic["cs"]=true;
			iconDic["cc"]=true;
			iconDic["ab"]=true;
			iconDic["zb"]=true;
			iconDic["dq"]=true;
			iconDic["aa"]=true;
			
			this.width=w;
			this.height=h;
			this.blendMode = BlendMode.LAYER;
			this.tf=new TextField();
			this.tf.textColor=0xe2cca5;
			this.tf.wordWrap=false;
			this.tf.condenseWhite=false;
			this.tf.filters =Font.Stroke(0);
			this.tf.multiline=false;
			this.tf.type=TextFieldType.DYNAMIC;
			this.tf.selectable=false;
			this.tf.autoSize=TextFieldAutoSize.LEFT;
			this.tf.mouseWheelEnabled=false;
			this.tf.defaultTextFormat = textFormat();
			this.addChildAt(tf,1);
		}
		
		public function update(des:String):void{
			this.clear();
			this.tf.text=des;
			this.showIcon();
		}
		
		public function updateHtml(des:String):void{
			this.clear();
			this.tf.htmlText=des;
			this.showIcon();
			
			this.shape = new Shape();
			shape.graphics.clear();
//			shape.graphics.lineStyle( 1.4,0xff0000 );
			shape.graphics.lineStyle( 1.5,0xff0000 );
			
//			shape.graphics.moveTo( this.width,12 );
//			shape.graphics.lineTo( this.width - 36,2 );
			shape.graphics.moveTo( tf.x+2,tf.y+1 );
			shape.graphics.lineTo( tf.x+tf.width,tf.y + 12 );
			shape.alpha = .5;
			this.addChild( shape );
		} 
		
		public function set textColor( value:uint ):void
		{
			this.tf.textColor = value;
		}
		
		protected function showIcon():void{
			this.doLayout();
			var regExp:RegExp = /(\\[a-z]{2})/g;
			var chatArr:Array=tf.text.split(regExp);
			var chatHtmlArr:Array=tf.htmlText.split(regExp);
			
			if (!chatArr||chatArr.length==0) 
			{
				return;
			}
			var index:int = 0;
			var pos:int = 0;
			var posHtml:int=0;
			while (index<chatArr.length)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
			{
				if (regExp.test(chatArr[index])) 
				{	
					if(!isIcon(chatArr[index].slice(1,3))) return;
					var bitmap:Bitmap = new Bitmap();
					bitmap.bitmapData = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData(chatArr[index].slice(1,3));
					if(tf.getCharBoundaries(pos)==null){
						setTimeout(showIcon,50);
						return;
					}
					bitmap.x = int(tf.getCharBoundaries(pos).x+this.tf.x);
					bitmap.y = int(tf.getCharBoundaries(pos).y - 1);
					setMcMask(bitmap);
					this.addChild(bitmap);
					posHtml+=chatHtmlArr[index].length;
					pos = pos + chatArr[index].length;
					
				} else {
					posHtml+=chatHtmlArr[index].length;
					pos = pos + chatArr[index].length;
				}
				index++;
			}
		}
		
		protected function doLayout():void{
			this.tf.x=this.width-this.tf.width;
		}
		
		/**
		 * 检测这种特殊的字符串是否为图标 
		 * @param str
		 * @return 
		 * 
		 */		
		protected function isIcon(str:String):Boolean{
			if(iconDic[str]){
				return true;
			}
			return false;
		}
		
		protected function setMcMask(mc:Bitmap):void{
			var s:Shape = new Shape();
			s.graphics.beginFill(0xffffff, 1);
			s.graphics.drawRect(mc.x, mc.y, mc.width+2, mc.height);
			s.graphics.endFill();
			s.blendMode = BlendMode.ERASE;
			this.addChild(s);
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.leading=5;
			tf.size = 12;
			tf.font = "宋体";//"宋体";
			return tf;
		}
		
		/**
		 * 清除重置所以的图标
		 * 
		 */		
		public  function clear():void{
			while(this.numChildren>2){
				this.removeChildAt(2);
			}
		}
		
	}
}