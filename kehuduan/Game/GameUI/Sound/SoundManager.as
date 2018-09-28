package GameUI.Sound
{
	import flash.events.Event;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.media.SoundTransform;
	import flash.utils.Dictionary;
	
	public class SoundManager
	{
		public static var soundDic:Dictionary = new Dictionary();
		
		public function SoundManager()
		{
		}
		
		public static function PlaySound(name:String):void
		{
			if(GameCommonData.isOpenFightSoundSwitch == false || GameCommonData.fightSoundVolume == 0)
			{
				return;
			}
			
			var sound:Sound = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassBySound(name);
			var sf:SoundTransform = new SoundTransform(GameCommonData.fightSoundVolume / 100);
			sound.play(0,0,sf);
		}
		
		public static function playSoundCanInterrupt(name:String):void
		{
			if(GameCommonData.isOpenFightSoundSwitch == false || GameCommonData.fightSoundVolume == 0) return;
			
			interruptSound(name);
			var sound:Sound = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassBySound(name);
			var sf:SoundTransform = new SoundTransform(GameCommonData.fightSoundVolume * 0.01); 
			var soundChannel:SoundChannel = sound.play(0,0,sf);
			if (soundChannel)
			{
				soundChannel.addEventListener(Event.SOUND_COMPLETE, soundCompHandler);
				soundDic[name] = soundChannel;
			}
		}
		
		public static function interruptSound(name:String):void
		{
			if(soundDic[name]) {
				var soundCannel:SoundChannel = (soundDic[name] as SoundChannel);
				soundCannel.stop();
				soundCannel.removeEventListener(Event.SOUND_COMPLETE, soundCompHandler);
				delete soundDic[name];
			}
		}
		
		public static function soundCompHandler(e:Event):void
		{
			for(var key:* in soundDic) {
				if(soundDic[key] == e.target) {
					interruptSound(key as String);
					break;
				}
			}
		}
	}
}