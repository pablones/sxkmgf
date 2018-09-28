package GameUI.Modules.Equipment.ui
{
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;

	public class UIStiletto extends Sprite
	{
		protected var tf:TextField;
		protected var shape:Shape;
		protected var bitmap:MovieClip;
		protected var _isShow:Boolean;
		protected var type:uint;
		protected var img:NumberItem;
		
		public function UIStiletto()
		{
			super();
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.tf=new TextField();
			this.tf.mouseEnabled=false;
			this.tf.width=100;
			this.tf.autoSize=TextFieldAutoSize.LEFT;
			this.tf.text=GameCommonData.wordDic[ "mod_equ_ui_sti1_cel_2" ];//"可以\n打孔";
			this.tf.textColor=0xffffff;
			this.tf.filters=Font.Stroke();
			this.tf.x=5;
			this.tf.y=5;
			
			this.shape=new Shape();
			this.shape.graphics.clear();
			this.shape.graphics.beginFill(0x999999,0.5);
			this.shape.graphics.drawRect(-28,-28,56,56);
			this.shape.graphics.endFill();
			
			this.bitmap=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Stiletto");
			this.bitmap.gotoAndStop(2);
			this.addChild(this.bitmap);
			
		}
		
		public function setEnable(value:Boolean):void{
			this._isShow=value;
			this.toRepaint();
		}
		 
		public function setStone(type:uint):void{
			if(this.type==type)return;
			this.type=type;
			this.bitmap.gotoAndStop(1);
			if(this.img && this.contains(this.img))this.removeChild(this.img);
			if(this.type==0)return;
			this.img=new NumberItem(String(type),"Icon");
			this.img.x=-23+7;
			this.img.y=-23+7;
			this.addChildAt(this.img,0);
		} 
		
		protected function toRepaint():void{
			if(this.img!=null && this.contains(this.img))this.removeChild(this.img);
			this.bitmap.gotoAndStop(2);
			if(this._isShow){
				if(this.contains(this.tf)){
					this.removeChild(this.tf);
				}
				if(this.contains(this.shape)){
					this.removeChild(this.shape);
				}
			}else{
				this.addChild(this.shape);
				this.addChild(this.tf);
			}	
		}
		
	}
}