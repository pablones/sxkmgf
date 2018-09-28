package GameUI.View.Components
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.setTimeout;

	public class TimeoutComponent extends Sprite implements IUpdateable
	{
		public var timeDoneHandler:Function;
		
		private var totalTime:uint;
		private var main_txt:TextField;
		private var format:TextFormat;
//		private var isTimeOut:Boolean;
		private var timer:Timer = new Timer();
//		private var colorId:int;
		
		private var isShowMinte:Boolean;				//是否显示分钟
		
		public function TimeoutComponent()
		{
			initTxt();
		}
		
		//开始计时
		public function start( _totalTime:uint,_isShowMinte:Boolean = false ):void
		{
			totalTime = _totalTime;
			if ( !_isShowMinte )
			{
				main_txt.text = "剩余时间：" + totalTime + "秒";
				main_txt.setTextFormat(format);
			}
			initTime();
		}
		
		public function stop():void
		{
			if ( GameCommonData.GameInstance.GameUI.Elements.IndexOf( this ) > -1 )
			{
				GameCommonData.GameInstance.GameUI.Elements.Remove( this );
			}
		}
		
		private function initTxt():void
		{
			format = new TextFormat();
			format.size = 12;
			format.color = 0xe2cca5;
//			format.color = 0xffffff;
			format.font = "宋体";
			
			main_txt = new TextField();
			main_txt.mouseEnabled = false;
			main_txt.autoSize = TextFieldAutoSize.LEFT;
			main_txt.textColor = 0xe2cca5;
			this.addChild(main_txt);
		}
		
		private function initTime():void
		{
			timer.DistanceTime = 1000;
//			timer.DistanceTime = 80;
			if ( this.totalTime>0 )
			{
//				isTimeOut = false;
				if ( GameCommonData.GameInstance.GameUI.Elements.IndexOf( this ) == -1 )
				{
					GameCommonData.GameInstance.GameUI.Elements.Add(this);
				}
			}
			else
			{
				if ( isShowMinte )
				{
					main_txt.text = "剩余时间：00:00秒";
				}
				else
				{
					main_txt.text = "剩余时间：0秒";
				}
				main_txt.setTextFormat(format);
//				colorId = setInterval( changeColor,300 );
			}
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				if ( totalTime >= 0 )
				{
					updateTime();
				}
				else
				{
					
				}
			}
		}
		
		private function updateTime():void
		{
			totalTime --;
			var minite:int = int(int(totalTime) % 3600) / 60;
			var second:int = int(totalTime) % 60;
			var miniteStr:String = getStringTime(minite);
			var secondStr:String = getStringTime(second);
			if ( isShowMinte )
			{
				main_txt.text = miniteStr + ":" + secondStr;
			}
			else
			{
				main_txt.text = "剩余时间：" + secondStr + "秒";
			}
			main_txt.setTextFormat(format);
			
			if ( totalTime <= 0 )
			{
				if ( GameCommonData.GameInstance.GameUI.Elements.IndexOf( this ) > -1 )
				{
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
				}
				if ( timeDoneHandler != null )
				{
					setTimeout( timeDoneHandler,1000 );
//					timeDoneHandler();
				}
			}
		}
	
		private function getStringTime(num:int):String
		{
			var str:String;
			if ( num<10 )
			{
				str = "0"+num.toString();
			}
			else
			{
				str = num.toString();
			}
			return str;
		}
		
		public function reStart(newTime:uint):void
		{
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);
			this.totalTime = newTime;
//			clearInterval( colorId );
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
//		public function clone():TimeOutTxt
//		{
//			return new TimeOutTxt(this.totalTime);
//		}
		
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