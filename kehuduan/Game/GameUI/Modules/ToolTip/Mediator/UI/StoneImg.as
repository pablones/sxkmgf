package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.View.Components.UISprite;
	
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.display.Bitmap;

	public class StoneImg extends UISprite
	{
		protected var imgUrl:String;
		protected var icon:Bitmap;
		protected var scale:Number;
		protected var dir:String;0
		protected var loader:BulkLoaderResourceProvider;
		public function StoneImg(imgUrl:String,dir:String="icon",scale:Number=1.0)
		{
			
			
			if(uint(imgUrl) > 100000) {	   //410101
				if(UIConstData.getItem(uint(imgUrl)).img != null) 
				{
					this.imgUrl = String(UIConstData.getItem(uint(imgUrl)).img);
				}
			} 
			this.dir=dir;
			this.scale=scale;
			init();
		}
		
		private function init():void{
			loader=new BulkLoaderResourceProvider();
			loader.Download.Add(GameCommonData.GameInstance.Content.RootDirectory +dir + imgUrl + ".png");
			loader.LoadComplete=loadComplete;
			loader.Load();
		}
		
		private function loadComplete():void{
			if(this.loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory  +dir + imgUrl + ".png")==null){
				this.icon=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("NoResource");
			}else{
				this.icon=this.loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory +dir + imgUrl + ".png").GetBitmap();
			}
			this.icon.scaleX=this.icon.scaleY=scale;
			this.addChild(this.icon);
		}
		
	}
}