package OopsEngine.Graphics.Animation
{
	import OopsFramework.IDisposable;
	
	import flash.display.BitmapData;
	
	public class AnimationFrame implements IDisposable
	{
		public var X:int;
		public var Y:int;
		public var Width:int;
		public var Height:int;
		public var FrameBitmapData:BitmapData;
		public var isDelete:Boolean = true;
		
		/** IDisposable Start */
		public function Dispose():void
		{
			this.FrameBitmapData.dispose();
			this.FrameBitmapData = null;
		}
		/** IDisposable End */
	}
}