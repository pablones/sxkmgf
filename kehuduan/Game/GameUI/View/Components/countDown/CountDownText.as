package GameUI.View.Components.countDown
{
	import OopsEngine.Graphics.Font;
	
	import flash.display.Sprite;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.utils.Timer;
	
	/**
	 * 倒计时文字组件
	 * @author:Ginoo
	 * @version:1.0
	 * @langVersion:3.0
	 * @playerVersion:10.0
	 * @since:1/22/2011
	 */
	public class CountDownText extends Sprite
	{
		/** 倒计时秒数 */
		private var _secNum:uint;
		
		/** 计时器 */
		private var _timer:Timer;
		 
		/** 文本 */
		private var _tf:TextField;
		
		/** 当前时间 */
		private var _count:int = 0;
 
		public function CountDownText(secNum:uint)
		{
			this._secNum = secNum;
			_count = 0;
			init();
			this.mouseEnabled  = false;
			this.mouseChildren = false;
		}
		
		private function init():void
		{
			_timer = new Timer(1000, _secNum);
			_timer.addEventListener(TimerEvent.TIMER, timerHandler);
			_timer.addEventListener(TimerEvent.TIMER_COMPLETE, compHandler);
			_tf = new TextField();
			_tf.filters = Font.Stroke(0x000000);
			_tf.width  = 23;
			_tf.height = 16;
			_tf.defaultTextFormat = new TextFormat("宋体", 12, 0xe2cca5, null,null,null,null,null,"center");//"宋体"
			this.addChild(_tf);
		}
		
		public function start():void
		{
			if(_timer.running) {
				return;
			}
			_tf.text = _secNum.toString();
			_timer.start();
		}
		
		private function timerHandler(e:TimerEvent):void
		{
//			var count:uint = _secNum - _timer.currentCount; 
			_count = _secNum - _timer.currentCount; 
			_tf.text = _count.toString();
		}
		
		private function compHandler(e:TimerEvent):void
		{
			dispose();
			this.dispatchEvent(new CountDownEvent(CountDownEvent.TIME_OVER, null));
		}
		
		public function dispose():void
		{
			if(_timer) {
				_timer.stop();
				_timer.removeEventListener(TimerEvent.TIMER, timerHandler);
				_timer.removeEventListener(TimerEvent.TIMER_COMPLETE, compHandler);
				_timer = null;
			}
			
			if(_tf && this.contains(_tf)) {
				this.removeChild(_tf);
				_tf = null;
			}
		}
		
		public function get count():int
		{
			return this._count;
		}
		
	}
}