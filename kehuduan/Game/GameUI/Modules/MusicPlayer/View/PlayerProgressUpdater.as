package GameUI.Modules.MusicPlayer.View
{
	import OopsEngine.Scene.CommonData;
	
	import OopsFramework.Audio.AudioEngine;
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	
	import flash.display.MovieClip;
	import flash.text.TextField;

	public class PlayerProgressUpdater implements IUpdateable
	{
		protected var player:MovieClip;
		
		protected var lastValue:int;
		
		public var updatePlayBar:Boolean = true;
		
		public function PlayerProgressUpdater(player:MovieClip)
		{
			this.player = player;
			
			if (GameCommonData.GameInstance.GameUI.Elements.IndexOf(this) == -1)
			{
				GameCommonData.GameInstance.GameUI.Elements.Add(this);
			}
		}

		public function Update(gameTime:GameTime):void
		{
			if (CommonData.audioEngine)
			{
				if (CommonData.audioEngine.IsPlaying)
				{
					(player.mcPlayhead.mcPlayheadBlinking as MovieClip).visible = true;
				}
				else
				{
					(player.mcPlayhead.mcPlayheadBlinking as MovieClip).visible = false;
				}
				
				if (updatePlayBar)
				{
					(player.mcPlayBar as MovieClip).width = 212 * CommonData.audioEngine.GetCurrentPosition() / length / 1000;
					(player.mcPlayhead as MovieClip).x = Math.round((player.mcPlayBar as MovieClip).x + (player.mcPlayBar as MovieClip).width);
				}

				var time:int = int(CommonData.audioEngine.GetCurrentPosition() / 1000);
				if (time == lastValue) return;
				
				var m:String;
				var s:String;
				
				if (time != -1)
				{
					m = int(time / 60).toString();
					s = int(time % 60).toString();
					
					if (s.length == 1) s = "0" + s;
					if (m.length == 1) m = "0" + m;
				}
				else
				{
					m = "00";
					s = "00";
				}
				
				(player.txtPlayTime as TextField).text = m + ":" + s;
				lastValue = time;
			}
			else
			{
				(player.mcPlayBar as MovieClip).width = 1;
				(player.mcPlayhead as MovieClip).x = (player.mcPlayBar as MovieClip).x;
				(player.mcPlayhead.mcPlayheadBlinking as MovieClip).visible = false;
				(player.txtPlayTime as TextField).text = "00:00";
			}
		}
		
		protected var length:int = 0;
		
		public function set songLength(value:int):void
		{
			length = value;
		}
		
		public function get Enabled():Boolean
		{
			return true;
		}
		
		// 接口空实现
		public function get UpdateOrder():int
		{
			return 0;
		}
		
		public function get EnabledChanged():Function
		{
			return null;
		}
		
		public function set EnabledChanged(value:Function):void
		{
		}
		
		public function get UpdateOrderChanged():Function
		{
			return null;
		}
		
		public function set UpdateOrderChanged(value:Function):void
		{
		}
		
	}
}