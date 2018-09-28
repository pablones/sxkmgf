package GameUI.Modules.Equipment.ui
{
	import flash.display.Shape;
	
	public class EnableItem extends NumberItem
	{
		protected var _enable:Boolean;
		protected var shape:Shape;
		
		public function EnableItem(type:String, dir:String="NumberIcon")
		{
			super(type, dir);
		}
		
		protected override function onLoabdComplete():void{
			super.onLoabdComplete();
			this.toRepaint();
		}
		
		public function setEnable(value:Boolean):void{
			this._enable=value;
			this.toRepaint();
		}
		
		protected function toRepaint():void{
			if(this._enable){
				if(this.shape==null){
					this.shape=new Shape();
					shape.name="grayShape";
					this.shape.graphics.beginFill(0x999999,0.5);
					this.shape.graphics.drawRect(0,0,32,32);
					this.shape.graphics.endFill();
				}
				this.addChild(this.shape);
			}else{
				if(this.shape==null)return;
				if(this.contains(this.shape))this.removeChild(this.shape);
			}
			
		}
		
	}
}