/**
动画数据格式
使用例子：
var ap:AnimationPlayer = new AnimationPlayer(Bitmap,IAnimationStrategy);
ap.StartClip("动画剪辑名");
addChild(ap);

ap.Update(gameTime);		// 循环更新帧速
**/

package OopsEngine.Graphics.Animation
{
	import OopsEngine.Scene.CommonData;
	
	import OopsFramework.*;
	import OopsFramework.Collections.DictionaryCollection;
	
	import flash.display.*;
	import flash.geom.*;
	
	public class AnimationPlayer extends Bitmap implements IDisposable
	{		
		public static const LOOP   : int = -1;						// 无限循环
		public static const SINGLE : int = 1;						// 播放一次
		
		public var PlayComplete : Function;							// 播放完成一件事件
		public var PlayFrame    : Function;							// 帧事件
		
//		public var Frames:DictionaryCollection;						// 动画帧集合（Bitmap）
		public var Clips:DictionaryCollection;						// 动画剪辑集合（Array → int）
		public var MaxWidth:int;									// 动画最大宽
		public var MaxHeight:int;									// 动画最大高
		public var DataName:String = "";          
		
		protected var playAnimationClip:AnimationClip;				// 正在播放动画剪辑名
		protected var playCount:int;								// 已播放次数
		protected var playMaxCount:int  = LOOP;						// 最大已播放次数
		protected var isPlaying:Boolean = false;					// 是否正在播放
		protected var clipName:String;
		
		public var variation:int = 0;                       // 变异
		
		public function get Frames():DictionaryCollection
		{
			return	CommonData.AnimationFrameList[this.DataName];
		}
		
		// 默认美术出640*480大小的画布
		public var offsetX:Number = 370;	
		public var offsetY:Number = 200;	
		
		private var eventArgs:AnimationEventArgs = new AnimationEventArgs();
		
		/** 每秒多少帧（默认24帧）*/
		
		public function AnimationPlayer(playMaxCount:int = LOOP)
		{
        	this.playMaxCount = playMaxCount;
		}
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.PlayComplete	   = null;
			this.PlayFrame   	   = null;
			
			this.playAnimationClip.Dispose();		// 可能会让玩家自己动做停止
			this.playAnimationClip = null;
			
			this.eventArgs.Dispose();              
			this.eventArgs = null;
			
//			this.Frames.Dispose();
//			this.Frames = null;
			
			this.Clips.Dispose();
			this.Clips = null;
		}
		/** IDisposable End */
		
		/** 播放指定名称的动画剪辑 */
		public virtual function StartClip(clipName:String, frameIndex:int = 0):void
		{
			if(this.Clips[clipName]!=null)
			{
				this.playAnimationClip    			      = this.Clips[clipName];
				this.eventArgs.CurrentClipName 			  = clipName;
				this.eventArgs.CurrentClipTotalFrameCount = this.playAnimationClip.Frame.length;
				this.eventArgs.CurrentClipFrameIndex      = frameIndex;
				this.Play();
			}
		}
		
		/** 播放动画 */
		public function Play():void
		{	
           //如果是刚转场的时候有可能动画正好加载完 并播放 							
		   if(this.Frames != null)
		   {
				this.playCount = 0;
				this.CalculateOffset(this.Frames[int(this.playAnimationClip.Frame[this.eventArgs.CurrentClipFrameIndex])]);
			
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
		
		/** 停止动画 */
		public function Stop():void
		{
			this.isPlaying = false;
		}
		
		/** 动画帧更新新 */
        public function Update(gameTime:GameTime):void
		{
            if(this.isPlaying)
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
	            		if(PlayComplete!=null) PlayComplete(this.eventArgs); 
	            	}

					if(this.playMaxCount == LOOP || this.playCount < this.playMaxCount)
	            	{	                     
						this.CalculateOffset(this.Frames[int(this.playAnimationClip.Frame[this.eventArgs.CurrentClipFrameIndex])]);
						
		            	if(PlayFrame!=null) PlayFrame(this.eventArgs);
	            	}
	            	else
	            	{
						this.isPlaying = false;
	            	}
	            }
            }
		}
		
		/** 设置翻转后图片坐标 */ 
		private function CalculateOffset(frame:AnimationFrame):void
		{
			if(this.playAnimationClip.TurnType)
			{
				this.scaleX = -1;
//				this.x 		= -1 * frame.X + offsetX + offsetX / 4;		// 有的模型要在水平翻转后脚下点对不上，所以要补差值(例：孔雀模型补差 + 40）
				this.x 		= -1 * frame.X + 463;
			}
			else
			{
				this.scaleX = 1;
				this.x 		= frame.X - this.offsetX;
			}
			this.y		    = frame.Y - this.offsetY;

            if(this.variation == 0)
            {
            	this.bitmapData = frame.FrameBitmapData;
            }
            else
            {
	            var changeData:BitmapData = frame.FrameBitmapData.clone();
				switch(this.variation)
				{
					case 1:
					    var bmd:BitmapData = new BitmapData(500, 500, false,0x006600);
					    changeData.copyChannel(bmd,new Rectangle(0,0,500,500),new Point(0,0),BitmapDataChannel.BLUE,BitmapDataChannel.BLUE);
					    changeData.copyChannel(bmd,new Rectangle(0,0,500,500),new Point(0,0),BitmapDataChannel.RED,BitmapDataChannel.RED);
					    break; 
					case 2:
					    bmd = new BitmapData(MaxWidth, MaxHeight, false, 0xFF0000);
					    changeData.copyChannel(bmd,new Rectangle(0,0,MaxWidth,MaxHeight),new Point(0,0),BitmapDataChannel.BLUE,BitmapDataChannel.BLUE);
					    changeData.copyChannel(bmd,new Rectangle(0,0,MaxWidth,MaxHeight),new Point(0,0),BitmapDataChannel.GREEN,BitmapDataChannel.GREEN);
					    break;
					case 3: //0x0033FF   0x1b03ff 0x3333FF #0000FF
						bmd = new BitmapData(500, 500, false,0x0000FF);
					    changeData.copyChannel(bmd,new Rectangle(0,0,500,500),new Point(0,0),BitmapDataChannel.RED,BitmapDataChannel.RED);
					    changeData.copyChannel(bmd,new Rectangle(0,0,500,500),new Point(0,0),BitmapDataChannel.GREEN,BitmapDataChannel.GREEN);				   
					    break;
				}
				this.bitmapData = changeData;
			}
		}
	}
}