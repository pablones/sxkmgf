package OopsEngine.Graphics.Animation
{
	import OopsFramework.IDisposable;
	
	public class AnimationEventArgs implements IDisposable
	{
		/** 当前影片剪辑名  */
		public var CurrentClipName:String;
		/** 当前影片剪辑总帧数  */
		public var CurrentClipTotalFrameCount:int;
		/** 当前影片剪辑当前帧数  */
		public var CurrentClipFrameIndex:int;
		/** 事件发送者  */
		public var Sender:Object
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.Sender = null;
		}
		/** IDisposable End */
	}
}