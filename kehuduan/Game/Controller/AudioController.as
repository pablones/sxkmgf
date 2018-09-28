package Controller
{
	import OopsFramework.Audio.AudioEngine;
	
	import flash.events.Event;
	import flash.media.Sound;
	import flash.media.SoundChannel;
	import flash.media.SoundTransform;
	
	/** 游戏音频控制  */
	public class AudioController
	{	
		/** 门派声音 */
		public static var homeSound:AudioEngine;
		/** 登录背景音乐 */
		public static var loginBgSound:AudioEngine;	
	
		/** 是否新登录**/
		public static var Islogin:Boolean = false;
		
		/**加载音乐**/	
		public static function LoadAudio(gameAudio:AudioEngine,audioName:String):void
		{
			GameCommonData.SkillOnLoadAudioEngine[audioName] = gameAudio;
		}
		
		/** 攻击音效  */
		public static function SoundAttack(audio:AudioEngine):void
		{
			if(GameCommonData.soundVolume > 0 && GameCommonData.isOpenFightSoundSwitch)
			{
				audio.Volume = GameCommonData.fightSoundVolume;
				audio.Loop = 1;
				audio.Play();
				
//				var ae:AudioEngine = gameAudioResource.GetAudio();
//				ae.Loop = 1;
//				ae.Play();
//				ae = null;
			}
//			var ae:AudioEngine = new AudioEngine(GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassBySound("Sound_Attack"));
		}
		
		
		
		/** 门派音效  */
		public static function SoundHomeOn(SoundName:*):void
		{
			if(GameCommonData.isOpenSoundSwitch == false)
			{
				return;
			}
			homeSound = new AudioEngine(GameCommonData.GameInstance.Content.Load(GameConfigData.UILibraryRole).GetClassBySound(SoundName));
			homeSound.Loop = 1;
			homeSound.Play();
			homeSound.Volume = 100;
			if(loginBgSound) loginBgSound.Volume = 30;
		}
		/** 门派音效关闭 */
		public static function SoundHomeOff():void
		{
			if(homeSound)
			{
				homeSound.Stop();
				homeSound = null;
				if(loginBgSound) loginBgSound.Volume = 60;
			}
		}
		/** 登录背景音乐  */
		public static function SoundLoginOn( loaderSound:Sound=null,loginSoundChannel:SoundChannel = null ,loginSoundTransform:SoundTransform = null):void
		{
			if(GameCommonData.isOpenSoundSwitch == false)
			{
				return;
			}
			//原版本音乐
//			loginBgSound = new AudioEngine(GameCommonData.GameInstance.Content.RootDirectory + "LoginSound.mp3");
//			loginBgSound.Volume = 100;
//			loginBgSound.Play(); 
			
			if ( loaderSound )
			{
				loginBgSound = new AudioEngine( loaderSound );
				loginBgSound.soundChannel   = loginSoundChannel;
				loginBgSound.soundTransform = loginSoundTransform;
				loginBgSound.Loop      = 1;
				loginBgSound.IsPlaying = true;			
				Islogin = true;			
			}
			else
			{
				loginBgSound = new AudioEngine(GameCommonData.GameInstance.Content.RootDirectory + "LoginSound.mp3");
				loginBgSound.Volume = 100;
				loginBgSound.Loop      = 1;
				Islogin = true;	
				loginBgSound.Play();
			}
			//////////////////////////////////
		}
		/** 关闭背景音乐 */
		public static function SoundLoginOff():void
		{
			if(loginBgSound )//&& Islogin == false)
			{
				loginBgSound.Stop();
				loginBgSound = null;
			}
		}
		/** 打开登录声音开关 */
		public static function OpenLoginSwitch(btnOff:Object , btnOn:Object):void
		{
			btnOn.visible = true;
			btnOff.visible = false;
			AudioController.SoundLoginOn();
		}
		/** 关闭登录声音开关 */
		public static function CloseLoginSwitch(btnOff:Object , btnOn:Object):void
		{
			btnOn.visible = false;
			btnOff.visible = true;
			AudioController.SoundLoginOff();
			AudioController.SoundHomeOff();
		}
	}
}