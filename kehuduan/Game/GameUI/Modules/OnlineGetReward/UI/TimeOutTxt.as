package GameUI.Modules.OnlineGetReward.UI
{
	import GameUI.Modules.OnlineGetReward.Data.OnLineAwardData;
	import GameUI.Modules.OnlineGetReward.Event.TimeOutEvent;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;

	public class TimeOutTxt extends Sprite implements IUpdateable
	{
		private var totalTime:uint;
		private var main_txt:TextField;
		private var format:TextFormat;
		private var isTimeOut:Boolean;
		private var timer:Timer = new Timer();
		private var colorId:int;
		
		public function TimeOutTxt(_totalTime:uint)
		{
			totalTime = _totalTime;
			initTxt();
			initTime();
		}
		
		private function initTxt():void
		{
			format = new TextFormat();
			format.size = 25;
//			format.color = 0x00ffcb;
			format.font = "宋体";
			
			main_txt = new TextField();
			main_txt.mouseEnabled = false;
			main_txt.autoSize = TextFieldAutoSize.LEFT;
			main_txt.textColor = 0x00ffcb;
			this.addChild(main_txt);
		}
		
		private function initTime():void
		{
			timer.DistanceTime = 1000;
//			timer.DistanceTime = 80;
			if ( this.totalTime>0 )
			{
				isTimeOut = false;
				GameCommonData.GameInstance.GameUI.Elements.Add(this);
			}
			else
			{
				OnLineAwardData.canGain = true;
				main_txt.text = "00:00";
				main_txt.setTextFormat(format);
				colorId = setInterval( changeColor,300 );
			}
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				if ( totalTime >= 0 )
				{
					clearInterval( colorId );
					main_txt.textColor = 0x00ffcb;
					OnLineAwardData.canGain = false;
					updateTime();
				}
				else
				{
					
				}
			}
		}
		
		private function updateTime():void
		{
			var minite:int = int(int(totalTime) % 3600) / 60;
			var second:int = int(totalTime) % 60;
			var miniteStr:String = getStringTime(minite);
			var secondStr:String = getStringTime(second);
			main_txt.text = miniteStr + ":" + secondStr;
			main_txt.setTextFormat(format);
			
			if ( totalTime <= 0 )
			{
				OnLineAwardData.canGain = true;
				this.dispatchEvent( new TimeOutEvent(TimeOutEvent.TIME_IS_ZERO));
				GameCommonData.GameInstance.GameUI.Elements.Remove(this);
				//变色
				colorId = setInterval( changeColor,300 );
			}
			totalTime --;
		}
		
		private function changeColor():void
		{
			if ( main_txt.textColor == 0x00ffcb )
			{
				main_txt.textColor = 0xff3232;
				return;
			}
			if ( main_txt.textColor == 0xff3232 )
			{
				main_txt.textColor = 0x00ffcb;
				return;
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
			clearInterval( colorId );
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
		}
		
		public function clone():TimeOutTxt
		{
			return new TimeOutTxt(this.totalTime);
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