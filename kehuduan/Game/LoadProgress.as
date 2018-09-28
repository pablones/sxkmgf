package
{
	import flash.text.TextField;
	
	/** 测试用进度条 */
	public class LoadProgress
	{
		private var load_mc:Loading = null;
		
		public function LoadProgress()
		{
			load_mc = new Loading();
			load_mc.x = -300;
			load_mc.y = -1000;
			load_mc.load_txt.text = "0%"; 
			GameCommonData.GameInstance.GameScene.addChild(load_mc);
			load_mc.mcLoadPercent.width = 1;
		}
		
		public  function startLoad(percent:Number,xPos:int,yPos:int):void
		{
//			trace("start percent " +percent);
			load_mc.y = yPos;
			load_mc.x = xPos;
			if(load_mc.mcLoadPercent.width < 1)
			{
				load_mc.mcLoadPercent.width = 1;
			}
			if(load_mc.mcLoadPercent.width > 132)
			{
				load_mc.mcLoadPercent.width = 132;
			}
			load_mc.load_txt.text = Math.ceil(percent*100)+"%";
			load_mc.mcLoadPercent.width = 132*percent;
//			trace(load_mc.mcLoadPercent.width);
//			trace("tiao 1 d scaleX  " +percent + "  tiao2 d scaleX is " +itemLoaded);
		}
		
		public function startLoadRed(percent:Number):void
		{
//			load_mc.tiao2.width = percent * 100;
		}
		
		public function endLoad():void
		{
			if(GameCommonData.GameInstance.GameScene.contains(load_mc))
			{
				GameCommonData.GameInstance.GameScene.removeChild(load_mc);
			}
//			load_mc = null;
//			trace("===================");
		}

	}
}