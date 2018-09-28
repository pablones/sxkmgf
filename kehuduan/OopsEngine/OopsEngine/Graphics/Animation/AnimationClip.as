package OopsEngine.Graphics.Animation
{
	import OopsFramework.IDisposable;
	
	public class AnimationClip implements IDisposable
	{
		/** 动画剪辑名  */
		public var Name:String;		
		/** 动画剪辑总帧数  */
		public var FrameCount:int;
		/** 动画最画布大宽偏移 X轴值  */
		public var OffsetX:int;
		/** 动画最画布大宽偏移 Y轴值  */
		public var OffsetY:int;
		/** 是否翻转  */
		public var TurnType:Boolean;
		/** 是否清理 */
		public var isDelete:Boolean = false;
		/** 动画帧数据  */
		public var Frame:Array = [];
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.Frame = null;
		}
		/** IDisposable End */
	}
}