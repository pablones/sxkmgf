package Controller
{
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.utils.setTimeout;
	
	public class FlutterController
	{
		public static var roseCount:int = 0;                           //玫瑰花飘落次数
		public static var roseMC:DisplayObject;                        //玫瑰花MC
		public static var roseResource:BulkLoaderResourceProvider;     //飘落下载器
		
		
		/**设置玫瑰花**/
		public static function SetRose():void
		{
			//增加玫瑰花数量
			roseCount ++;	
		
			if(roseResource == null)
			{
				LoadRose();
			}						
		}
		
		/**读取玫瑰花**/
		public static function LoadRose():void
		{
			roseResource = new BulkLoaderResourceProvider();
			roseResource.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF +"Rose.swf");
			roseResource.LoadComplete  = RoseLoadComplete;
			roseResource.Load();
		}
		
		/**玫瑰花加载完成**/
		public static function RoseLoadComplete():void
	    {			
		    roseMC =  roseResource.GetResource(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.GameSkillListSWF +"Rose.swf").GetDisplayObject();
			roseMC.x = 	GameCommonData.GameInstance.GameUI.stage.stageWidth / 2 - roseMC.width/2;	
			GameCommonData.GameInstance.Weather.addChild(roseMC);
			setTimeout(RoseTimerComplete,23000);
		}
		
		
		/**播放完成**/
		public static function RoseTimerComplete():void
		{
			try
			{
				roseResource.Dispose();
				roseResource = null;		
				GameCommonData.GameInstance.Weather.removeChild(roseMC);
				roseMC = null;
			}
			catch(e:Error)
			{
				
			}	
			roseCount --;
			if(roseCount > 0)
			{
				LoadRose();
			}
		}
	}
}