package GameUI.Modules.Meridians.Components
{
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.DisplayObject;
	import flash.filters.GlowFilter;
//	import flash.utils.Timer;
	
	public class DisplayObjectGlow implements IUpdateable
	{
		private var mc:DisplayObject;
		private var yellowFilter:GlowFilter;
		private var IsAdd:Boolean = true;
//		private var timer:Timer ;
		private var gameTimer:Timer;
		
		public var isRun:Boolean = false;
	
		public var blur:Number = 0;
		public var alpha:Number = 1;
		public var color:uint = 0;
		public var strength:Number = 1;
		
		public function DisplayObjectGlow( _mc:DisplayObject, color:uint = 0xffff00, alpha:Number = 1, blur: Number = 0, strength:Number = 5)
		{
			this.mc = _mc;
			this.gameTimer = new Timer();
			
			this.color = color ;
			this.blur = blur;
			this.alpha = alpha;
			this.strength = strength;
		}
		
		//开始闪
		public function lightOn():void
		{
			isRun = true;
			yellowFilter = null;
			if(yellowFilter == null)
			{			
//				dx = 0;
				yellowFilter = new GlowFilter( color, alpha, blur, blur, strength, 1, false, false );
				this.mc.filters = [ yellowFilter ]; 
			}
//			if(timer == null)
//			{
//				timer = new Timer(50);
//				timer.addEventListener(TimerEvent.TIMER,Update);
//				timer.start();
//			}
			this.gameTimer.DistanceTime = 50;
			if(GameCommonData.GameInstance.GameUI.Elements.IndexOf(this) == -1)
			{
				GameCommonData.GameInstance.GameUI.Elements.Add( this );
			}
		}
		
		//停止闪 
		public function lightOff():void
		{
			isRun = false;
			blur = 0;
			this.mc.filters = null; 
//			if(timer != null)
//			{
//				timer.stop();
//				timer.removeEventListener(Event.ACTIVATE,Update);
//				timer = null;
//			}

			if(GameCommonData.GameInstance.GameUI.Elements.IndexOf(this) > -1)
			{
				GameCommonData.GameInstance.GameUI.Elements.Remove( this );
			}
		}
		
		public function UpdateGo():void
		{
			if(isRun)
			{
				if( blur <= 2 )
				{
					IsAdd = true ;
				}
				if( blur > 16 )
				{
					IsAdd = false ;
				}
				if( IsAdd )
				{
					blur++;
				}else
				{
					blur--;
				}
				yellowFilter.blurX = yellowFilter.blurY = blur ;
				mc.filters = [ yellowFilter ] ;
			}
			else
			{
				mc.filters = null; 
			}
		}
		
		public function Update( gameTime:GameTime ):void
		{
			if ( gameTimer.IsNextTime( gameTime ) )
			{
				UpdateGo();
			}
		}
		
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
        
        public function get Enabled():Boolean
		{
			return true;
		}

	}
}