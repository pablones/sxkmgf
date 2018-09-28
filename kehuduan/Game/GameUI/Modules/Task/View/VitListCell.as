package GameUI.Modules.Task.View
{
	import GameUI.Modules.Equipment.ui.IListCell;
	import GameUI.View.items.MoneyItem;
	
	import flash.text.TextFieldAutoSize;

	public class VitListCell extends MoneyItem implements IListCell
	{
		public function VitListCell(w:Number=176, h:Number=18)
		{
			super(w, h);
			this.mouseChildren=false;
			this.mouseEnabled=false;
		}
		
		public function set cellData(value:Object):void
		{
			this.tf.autoSize=TextFieldAutoSize.LEFT;
			this.update(String(value));
		}
		
		public function dispose():void
		{
			
		}
		
		
		protected override function doLayout():void{
			this.tf.x=0;
		}
		
		public function set cellWidth(value:uint):void
		{
			
		}
		
		public function set cellHeight(value:uint):void
		{
			
		}
		
		public function get cellWidth():uint
		{
			return this.tf.width;
		}
		
		public function get cellHeight():uint
		{
			return this.tf.height;
		}
		
	}
}