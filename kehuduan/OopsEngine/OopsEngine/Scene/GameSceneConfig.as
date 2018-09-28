package OopsEngine.Scene
{
	import OopsEngine.Exception.ExceptionResources;
	
	import OopsFramework.Content.Loading.BulkProgressEvent;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	import OopsFramework.Game;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	public class GameSceneConfig extends BulkLoaderResourceProvider
	{
		public var Progress:Function;
		
		private var gameScene:GameScene;
		private var configXmlUrl:String;
		private var smallUrl:String;
		
		public function GameSceneConfig(gameScene:GameScene)
		{
			this.gameScene 	  = gameScene;
			this.configXmlUrl = this.gameScene.SceneUrl + "Config.xml";
			this.smallUrl	  = this.gameScene.Games.Content.RootDirectory + "Scene/" + this.gameScene.name + "/Small.jpg";
		}
		
		public override function Load():void
		{
//			this.Download.Version = Game.Version;
			this.Download.Add(this.configXmlUrl);
			this.Download.Add(this.smallUrl);
			super.Load();
		}
		
		protected override function onBulkProgress(e:BulkProgressEvent):void
	    {
			super.onBulkProgress(e);
	    	if(Progress!=null) Progress(e);
	    }
		
		protected override function onBulkCompleteAll():void 
		{
			this.gameScene.ConfigXml  = this.GetResource(this.configXmlUrl).GetXML();
			this.gameScene.SmallMap   = this.GetResource(this.smallUrl).GetBitmap();
			
			var bitMapData:BitmapData = new BitmapData(this.gameScene.SmallMap.width,this.gameScene.SmallMap.height);
			bitMapData.copyPixels(this.gameScene.SmallMap.bitmapData,
								  new Rectangle(0, 0, this.gameScene.SmallMap.width, this.gameScene.SmallMap.height),
								  new Point(0,0));
			this.gameScene.RealSmallMap = new Bitmap(bitMapData);
			
			bitMapData 		= null;
			this.Progress   = null;
			this.gameScene  = null;
			
			super.onBulkCompleteAll();
		}
		
		protected override function onBulkError(e:BulkProgressEvent):void
		{
			throw new Error(ExceptionResources.ErrorGameSceneConfig + "：" + e.ErrorMessage);
		}
	}
}