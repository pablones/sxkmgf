package GameUI.Modules.Friend.view.ui
{
	import GameUI.View.Components.UISprite;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Bitmap;
	import flash.display.BlendMode;
	import flash.display.Shape;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.setTimeout;

	public class FriendChatCell extends UISprite
	{
		
		/**
		 * 文本组件 
		 */		
		protected var desTf:TextField;
		/**
		 * 消息内容 
		 */		
		protected var _des:String;
		/**
		 * 消息文本显示宽度 
		 */		
		protected var _w:uint;
		
		
		public function FriendChatCell(w:uint)
		{
			super();
			this._w=w;
			this.createChildren();
		}
		
		
		/**
		 * 创建子元素 
		 * 
		 */		
		protected function createChildren():void{
			this.desTf=new TextField();
			this.addChild(this.desTf);
			this.desTf.textColor=0xffffff;
			this.desTf.defaultTextFormat=this.textFormat();
			this.desTf.mouseEnabled=false;
			this.desTf.autoSize=TextFieldAutoSize.LEFT;
			this.desTf.width=this._w;
			this.desTf.filters=	Font.Stroke(0x000000);
			this.desTf.wordWrap=true;
			this.desTf.multiline=true;	
			this.desTf.selectable=false;
			this.desTf.mouseWheelEnabled=false;
			this.cacheAsBitmap=true;
		}
		
		/**
		 * 设置格式 
		 * @return 
		 * 
		 */		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.leading = 8;
			tf.font = GameCommonData.wordDic["mod_cam_med_ui_UIC1_getI"];//"宋体"
			return tf;
		}
		
		/**
		 * 重绘 
		 * 
		 */		
		protected function toRepaint():void{
			this.removeAllFace();
			this.desTf.htmlText=this._des;
			this.setFace();
			this.doLayout();
		}
		
		/**
		 * 布局 
		 * 
		 */		 
		protected function doLayout():void{
			this.height=this.desTf.height+2;
		}
		
		/**
		 * 设置显示内容 
		 * @param value
		 * 
		 */		
		public function set des(value:String):void{
			if(value==this._des)return;
			this._des=value;
			this.toRepaint();
		}
		
		/**
		 *  删除文本上的表情
		 * 
		 */		
		protected function removeAllFace():void{
			while(this.numChildren>2){
				this.removeChildAt(2);
			}
		}
		/**
		 *  设置表情
		 * 
		 */		
		private function setFace():void 
		{
			var regExp:RegExp=/(\\\d{3})/g;
			var chatArr:Array=this.desTf.text.split(regExp);
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
					if(this.desTf.getCharBoundaries(pos)==null){
						setTimeout(setFace,50);
						return;
					}
					setBmpMask(this.desTf.getCharBoundaries(pos));
					addImg(chatArr[index].slice(1,4), this.desTf.getCharBoundaries(pos));
					count++;
					pos = pos + chatArr[index].length;
				} else {
					pos = pos + chatArr[index].length;
				}
				index++;
			}			
			return;
		}
		
		
		/**
		 * 添加表情图像   
		 * @param faceName  ：表情名
		 * @param rect      ：表情位置
		 * 
		 */		
		private function addImg(faceName:String, rect:Rectangle):void {
			var face:Bitmap = new Bitmap();
			face.bitmapData = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("F"+faceName);
			face.x = rect.x;
			face.y = rect.y-4; 
			this.addChild(face);
		}
		
		/**
		 * 设置擦除 
		 * @param rect ：擦除范围
		 * 
		 */		
		private function setBmpMask(rect:Rectangle):void
		{ 
			if(rect) {
				var s:Shape = new Shape();
				s.graphics.beginFill(0xffffff, 1);
				s.graphics.drawRect(rect.x-1, rect.y, rect.width+19, rect.height);
				s.graphics.endFill();
				s.blendMode = BlendMode.ERASE;
				this.addChild(s);
			}
		}
		
	}
}