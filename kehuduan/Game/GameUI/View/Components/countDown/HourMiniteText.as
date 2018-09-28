package GameUI.View.Components.countDown
{
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.setTimeout;

	public class HourMiniteText extends Sprite implements IUpdateable
	{
		public var timeDoneHandler:Function;
		
		private var totalTime:uint;
		private var main_txt:TextField;
		private var format:TextFormat;
		private var timer:Timer = new Timer();
		
		private var isShowMinte:Boolean;				//是否显示分钟
		
		private var textColor:uint;
		
		public function HourMiniteText()
		{
			initTxt();
		}
		
		//开始计时
		public function start( _totalTime:uint,_color:uint = 0x00ff00 ):void
		{
			totalTime = _totalTime;
//			textColor = _color;
			main_txt.textColor = _color;
			format.color = _color;
			showTime();
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
			format.font = "宋体";
			
			main_txt = new TextField();
			main_txt.mouseEnabled = false;
			main_txt.autoSize = TextFieldAutoSize.LEFT;
			this.addChild(main_txt);
		}
		
		private function initTime():void
		{
			timer.DistanceTime = 1000;
			if ( this.totalTime>0 )
			{
				if ( GameCommonData.GameInstance.GameUI.Elements.IndexOf( this ) == -1 )
				{
					GameCommonData.GameInstance.GameUI.Elements.Add(this);
				}
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
			showTime();
			if ( totalTime <= 0 )
			{
				if ( GameCommonData.GameInstance.GameUI.Elements.IndexOf( this ) > -1 )
				{
					GameCommonData.GameInstance.GameUI.Elements.Remove(this);
				}
				if ( timeDoneHandler != null )
				{
					setTimeout( timeDoneHandler,1000 );
				}
			}
		}
		
		private function showTime():void
		{
			var hour:int = int(totalTime/3600);
			var minite:int = int( (totalTime%3600)/60 ) ;
			var second:int = int(totalTime) % 60;
			var miniteStr:String = getStringTime(minite);
			var hourStr:String = getStringTime(hour);
			var secondStr:String = getStringTime(second);
			if ( hour > 0 )
			{
				main_txt.text = hour.toString() + GameCommonData.wordDic[ "mod_mas_view_gra_get_2" ] + minite.toString() + GameCommonData.wordDic[ "mod_newerPS_med_new_show_1" ];
			}
			else
			{
				if ( minite > 0 )
				{
					main_txt.text = minite.toString() + GameCommonData.wordDic[ "mod_newerPS_med_new_show_1" ] + second.toString() + GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_6" ];
				}
				else
				{
					main_txt.text = second.toString() + GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_6" ];
				}
			}
			
			main_txt.setTextFormat(format);
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
			GameCommonData.GameInstance.GameUI.Elements.Add(this);
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