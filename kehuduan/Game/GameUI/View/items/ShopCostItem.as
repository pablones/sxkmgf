package GameUI.View.items
{
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	import flash.display.Sprite;
	
	/** 物品换物品-使用的小图标加载 */
	public class ShopCostItem extends Sprite
	{
		protected var mkDir:String = "icon";
		
		public var icon:Bitmap;
		public var iconName:String = "";
		
		public function ShopCostItem(icon:String, mkDir:String = "icon")
		{
			this.cacheAsBitmap=true;
			this.iconName = icon;
			this.mouseChildren = false;
			this.mouseEnabled = false;
			this.mkDir = mkDir;
			loadIcon();
		}
		
		private function loadIcon():void
		{
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + iconName + ".png",onLoabdComplete);
		}
		
		protected function onLoabdComplete():void
		{
			icon = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/"+mkDir+"/" + iconName + ".png");
			if(icon) {
				icon.scaleX = 0.5;
				icon.scaleY = 0.5;
				this.addChild(icon);
			}
		}
		
	}
}