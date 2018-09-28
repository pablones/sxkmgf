package GameUI.Modules.PlayerIcon.UI
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;

	public class CursorIcon extends Sprite
	{
		protected var cursorName:String;
		
		public function CursorIcon(cursorName:String)
		{
			super();
			this.mouseEnabled 		  = false;
			this.mouseChildren 		  = false;
			this.cursorName    		  = cursorName;
			if(GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary)==null){
				return;
			}
			var bitMapData:BitmapData = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData(cursorName);	
			var bitMap:Bitmap 		  = new Bitmap();
			bitMap.bitmapData 		  = bitMapData;
			this.addChild(bitMap);
		}
	}
}