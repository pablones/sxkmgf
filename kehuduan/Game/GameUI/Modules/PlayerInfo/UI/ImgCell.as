package GameUI.Modules.PlayerInfo.UI
{
	import GameUI.View.Components.UISprite;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.Bitmap;
	
	/**
	 * 小头像CellRenderer 
	 * @author felix
	 * 
	 */	
	public class ImgCell extends UISprite
	{
		private var icon:Bitmap;
		public var iconUrl:String;
		public var cellData:Object={};
		public function ImgCell(icon:String,tip:String,isDeBuff:Boolean=false)
		{
			this.width=16;
			this.height=16;
			this.cacheAsBitmap=true;
			if(isDeBuff){
				this.graphics.clear();
				this.graphics.lineStyle(1,0xff0000,1);
				this.graphics.moveTo(-1,-1);
				this.graphics.lineTo(16,-1);
				this.graphics.lineTo(16,16);
				this.graphics.lineTo(-1,16);
				this.graphics.lineTo(-1,-1);
				this.graphics.endFill();	
			}
			this.iconUrl=icon;
			this.name="buffIcon_"+tip;
			this.mouseEnabled=true;
			init();
		}
		
		private function init():void{
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/icon/" + iconUrl + ".png",loadComplete);
		}
		
		private function loadComplete():void{
			
			this.icon=ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/icon/" + iconUrl + ".png");
			if(this.icon==null){
				this.icon=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
			}
			this.icon.scaleX=0.5;
			this.icon.scaleY=0.5;
			this.addChild(this.icon);
		}
		
	}
}