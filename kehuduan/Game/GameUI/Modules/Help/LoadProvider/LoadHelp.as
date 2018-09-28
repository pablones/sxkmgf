package GameUI.Modules.Help.LoadProvider
{	
	import flash.display.Loader;
	public class LoadHelp
	{
		public function LoadHelp(path:String)
		{
			var loader:Loader = new Loader();
			loader.load(new URLRequest(path);
		}
		
		/** 下载项完成事件 */
		protected override function onBulkCompleteAll():void 
		{
			UIConstData.relDic		   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.Help).GetDisplayObject()["relDic"];	 			// 江湖指南关系数据
			UIConstData.proDic		   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.Help).GetDisplayObject()["proDic"];	 			// 江湖指南数据
		}

	}
}