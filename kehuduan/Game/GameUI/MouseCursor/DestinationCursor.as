package GameUI.MouseCursor
{
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObjectContainer;
	import flash.display.Sprite;
	
	public class DestinationCursor
	{
		public static var Instance:DestinationCursor;
		private var sceneLayer:DisplayObjectContainer
		public var desIcon:Sprite=new Sprite();
		
		public function DestinationCursor(container:DisplayObjectContainer)
		{
			this.sceneLayer=container;
			var bitMapData:BitmapData=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("MoveTo");
			var bitMap:Bitmap=new Bitmap();
			bitMap.bitmapData=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("MoveTo");
			this.desIcon.addChild(bitMap);
			this.desIcon.mouseChildren=false;
			this.desIcon.mouseEnabled=false;
			this.sceneLayer.addChild(this.desIcon);
		}
		
		public static function getInstance(container:DisplayObjectContainer):DestinationCursor{
			Instance=new DestinationCursor(container);
			return Instance;
		}
		
		public function show():void{
			this.desIcon.visible=true;
			this.desIcon.x=sceneLayer.mouseX;
			this.desIcon.y=sceneLayer.mouseY;
		}
		
		public function hide():void{
			this.desIcon.visible=false;
		} 

	}
}