package OopsEngine.Scene
{
	import OopsEngine.Scene.StrategyElement.GameElementData;
	
	import OopsFramework.Audio.AudioEngine;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.events.TimerEvent;
	import flash.utils.Dictionary;
	
	public class CommonData
	{
		public static var shadow:BitmapData; //影子	
		public static var playerStall:BitmapData; //猫咪 
		public static var golem:BitmapData;  //小蓝人
		public static var playerBanner:BitmapData;//决斗旗
		public static var selectShadow:BitmapData;//选中的影子 
		public static var taskfinish:BitmapData;
		public static var taskUnAccpet:BitmapData;
		public static var taskunfinish:BitmapData;
		public static var lifeSeekState:BitmapData;
		public static var teamLeaderSign:BitmapData; //队长
		public static var teamMemberSign:BitmapData; //队员
		public static var bigShadow:BitmapData; 
		public static var bigSelect:BitmapData;
			
		public static var DataAnalyze:Array = new Array();                               // 解析动画数组
		public static var DataLoad:Array    = new Array();                               // 下载队列
		public static var IsAnalyze:Boolean = false;                                     // 是否开始解析数据
		public static var IsLoad:Boolean    = false;                                     // 人物加载流程
		
	    public static var MusicUrl:String;                                 //音乐地址
		public static var IsPlayThemeSong:Boolean = false;                 //是否播放主题音乐
		public static var IsPlayThemeSongComplete:Boolean = false;         //是否播放完主题曲
		public static var IsMusicPlayerBusy: Boolean = false;    // 是否被音乐播放器中断
		public static var SceneID:String = "";

        public static var audioEngine    : AudioEngine;	 
        
        public static var owner    :Dictionary = new Dictionary();	 
         public static function BigShadow(bitmap:Bitmap):BitmapData
		{
			if(bigShadow == null)
			{
				bigShadow = bitmap.bitmapData;
            }
			return bigShadow;
		}    
		
		 public static function BigSelect(bitmap:Bitmap):BitmapData
		{
			if(bigSelect == null)
			{
				bigSelect = bitmap.bitmapData;
            }
			return bigSelect;
		}    

		public static function Shadow(bitmap:Bitmap):BitmapData
		{
			if(shadow == null)
			{
				shadow = bitmap.bitmapData;
            }
			return shadow;
		}

		public static function PlayerStall(bitmap:Bitmap):BitmapData
		{
			if(playerStall == null)
			{
				playerStall = bitmap.bitmapData;
            }
			return playerStall;
		}
		
		public static function Golem(bitmap:Bitmap):BitmapData
		{
			if(golem == null)
			{
				golem = bitmap.bitmapData;
            }
			return golem;
		}
		
	    public static function PlayerBanner(bitmap:Bitmap):BitmapData
		{
			if(playerBanner == null)
			{
				playerBanner = bitmap.bitmapData;
            }
			return playerBanner;
		}
		
	    public static function SelectShadow(bitmap:Bitmap):BitmapData
		{
			if(selectShadow == null)
			{
				selectShadow = bitmap.bitmapData;
            }
			return selectShadow;
		}
		
		public static function Taskfinish(bitmap:Bitmap):BitmapData
		{
			if(taskfinish == null)
			{
				taskfinish = bitmap.bitmapData;
            }
			return taskfinish;
		}
		
		public static function TaskUnAccpet(bitmap:Bitmap):BitmapData
		{
			if(taskUnAccpet == null)
			{
				taskUnAccpet = bitmap.bitmapData;
            }
			return taskUnAccpet;
		}
		
		public static function Taskunfinish(bitmap:Bitmap):BitmapData
		{
			if(taskunfinish == null)
			{
				taskunfinish = bitmap.bitmapData;
            }
			return taskunfinish;
		}

        public static function LifeSeekState(bitmap:Bitmap):BitmapData
		{
			if(lifeSeekState == null)
			{
				lifeSeekState = bitmap.bitmapData;
            }
			return lifeSeekState;
		}
		
		public static function TeamLeaderSign(bitmap:Bitmap):BitmapData
		{
			if(teamLeaderSign == null)
			{
				teamLeaderSign = bitmap.bitmapData;
            }
			return teamLeaderSign;
		}
		
		// 图片内存存放地址
		public static var AnimationClipList:Dictionary = new Dictionary();
		// 图片内存存放地址
		public static var AnimationFrameList:Dictionary = new Dictionary();
		
		public static function TeamMemberSign(bitmap:Bitmap):BitmapData
		{
			if(teamMemberSign == null)
			{
				teamMemberSign = bitmap.bitmapData;
            }
			return teamMemberSign;
		}
		
	   public static function StopLoad(isdo:Boolean):void
	   {
	   		IsLoad = isdo;
	   		IsAnalyze = isdo;	   		  		
	   }
	   
		
		/**解析动画**/
		public static function 	AnalyzeUpdate(e:TimerEvent):void
		{
			var geed:GameElementData;
			if(IsLoad == false)
			{			
				for(var n:int = 0;n < DataAnalyze.length;n++)
				{
					geed = DataAnalyze[n] as GameElementData;
					if(geed.IsLoad == false && geed.LoaderResource != null
					 && IsLoad == false)
					{
						 geed.LoaderResource.Load();
						 IsLoad = true;
					}
				}
			}
				
			if(IsAnalyze == false)
			{
				IsAnalyze = true;	
				for(n = 0;n < DataAnalyze.length;n++)
				{
					geed = DataAnalyze[n] as GameElementData;
					if(geed.IsLoad)
					{
						if(geed.LoaderResource.GetResource(geed.DataUrl) != null)
							 geed.Analyze(geed.LoaderResource.GetResource(geed.DataUrl).GetMovieClip());	
						DataAnalyze.splice(n,1);					 
						break;
					}
				}			
			 	IsAnalyze = false;	 
			}

		}		
	}
}