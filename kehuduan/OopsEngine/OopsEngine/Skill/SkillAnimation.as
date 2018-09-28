package OopsEngine.Skill
{
	import OopsEngine.Graphics.Animation.AnimationClip;
	import OopsEngine.Graphics.Animation.AnimationEventArgs;
	import OopsEngine.Graphics.Animation.AnimationFrame;
	import OopsEngine.Scene.GameSceneLayer;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import OopsFramework.GameTime;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Bitmap;
	import flash.utils.Dictionary;
	
	
	public class SkillAnimation extends Bitmap
	{
		public static const LOOP   : int = -1;						// 无限循环
		public static const SINGLE : int = 1;						// 播放一次
		
		public var PlayComplete:Function;							// 播放完成一件事件
		public var PlayFrame:Function;								// 帧事件
		public var skillID:int = 0;
		
		public var Frames:Dictionary;								// 动画帧集合
		public var Clips:Dictionary;								// 动画剪辑集合
		public var MaxWidth:int;									// 动画最大宽
		public var MaxHeight:int;									// 动画最大高
		
		public var playAnimationClip:AnimationClip;				// 正在播放动画剪辑名
		protected var playCount:int;								// 已播放次数
		protected var playMaxCount:int  = LOOP;						// 最大已播放次数
		protected var isPlaying:Boolean = false;					// 是否正在播放
		
		protected var clipName:String;
		
		public  var IsPlayComplete:Boolean = false;                 //是否播放完成
		public var player:GameElementAnimal;                        //人物对象
		public var gameScene:GameSceneLayer;                        //场景对象     
		
		public var offsetX:Number = 0;	
		public var offsetY:Number = 0;	
		
		public var frameRate:Timer = new Timer();								// 动画帧速控制
		public var eventArgs:AnimationEventArgs = new AnimationEventArgs();
		
		/** 每秒多少帧（默认24帧）*/
		public function get FrameRate():uint
		{
			return this.frameRate.Frequency;
		}
		public function set FrameRate(value:uint):void
		{
			this.frameRate.Frequency = value; 
		}
		
		public function AnimationPlayer(playMaxCount:int = LOOP):void
		{
        	this.playMaxCount  = playMaxCount;
        	this.cacheAsBitmap = true;
	        //this.frameRate 	   = new Timer();
		}
		
		/** 播放指定名称的动画剪辑 */
		public virtual function StartClip(clipName:String, frameIndex:int = 0):void
		{
			if(this.Clips[clipName]!=null)
			{
				this.playAnimationClip    			      = this.Clips[clipName];
				this.eventArgs.CurrentClipName 			  = clipName;
				this.eventArgs.CurrentClipTotalFrameCount = this.playAnimationClip.Frame.length;
				this.eventArgs.CurrentClipFrameIndex      = frameIndex;
				if(this.skillID != 100000)
				{
					this.frameRate.Frequency                  = 24;
				}
				if(this.skillID == 4011)
				{
					this.frameRate.Frequency                  = 7;
				}
			    if(this.skillID == 4010)
				{
					this.frameRate.Frequency                  = 4;
				}
				this.Play();
			}		
		}
		
		/** 播放动画 */
		public function Play():void
		{
			this.playCount			 = 0;
		    var frame:AnimationFrame = this.Frames[int(this.playAnimationClip.Frame[this.eventArgs.CurrentClipFrameIndex])] as AnimationFrame;
		    
		    this.CalculateOffset(frame);
			this.bitmapData = frame.FrameBitmapData;
			
			// 剪辑只有一帧时，组件不在更新帧了（因为FALSH有封装了重绘过程，设置了第一帧就可以了）
			if(this.playAnimationClip.Frame.length > 1)
			{
				this.isPlaying = true;
			}
			else
			{
				if(PlayFrame!=null) PlayFrame(this.eventArgs);
				this.isPlaying = false;
			}
		}
		
		/** 暂停动画 */
		public function Pause():void
		{
			if(this.isPlaying == true)
			{
				this.isPlaying = false;
			}
			else
			{
				this.isPlaying = true;
			}
		}
		
		
		/** 动画帧更新新 */
        public function Update(gameTime:GameTime):void
		{
			if(frameRate.IsNextTime(gameTime))
			{
	            if(this.playAnimationClip != null && this.isPlaying)
	            {
	            	if((this.playMaxCount == LOOP || this.playCount < this.playMaxCount))
	            	{
		            	// 当前影片剪辑动画指针递增
		            	this.eventArgs.CurrentClipFrameIndex++;				
		        
		            	// 判断是否进去下一个动画循环
		            	if(this.eventArgs.CurrentClipFrameIndex >= this.playAnimationClip.Frame.length)
		            	{
		            		this.eventArgs.CurrentClipFrameIndex = 0;
		            		this.playCount++;
		            		if(PlayComplete!=null) PlayComplete(this); 
		            	}
	
						if(this.playMaxCount == LOOP || this.playCount < this.playMaxCount)
		            	{
			            	var frame:AnimationFrame = this.Frames[int(this.playAnimationClip.Frame[this.eventArgs.CurrentClipFrameIndex])] as AnimationFrame;
							this.bitmapData = frame.FrameBitmapData;
							this.CalculateOffset(frame);
			            	if(PlayFrame!=null) PlayFrame(this.eventArgs);
		            	}
		            	else
		            	{
							this.isPlaying = false;
		            	}
		            }
	            }
            }
		}
		
		
		/**  */
		private function CalculateOffset(frame:AnimationFrame):void
		{
//			if(this.playAnimationClip.TurnType)
//			{
//				this.scaleX = -1;
////				if(this.offsetX == 200)
////				{
////					this.x = -1 * frame.X + offsetX * 2 - offsetX / 2;	// 400 为2倍的脚下点(400 - this.offsetX / 2)	300
////				}
////				else
////				{
//					this.x = -1 * frame.X + offsetX + offsetX / 4;
////				}
//			}
//			else
//			{
//				this.scaleX = 1;
				this.x 		= frame.X + offsetX;
//			}
			this.y = frame.Y + offsetY;
		}
	}
}