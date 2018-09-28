package GameUI.Modules.Login.SoundUntil
{
	import Controller.AudioController;
	
	import GameUI.Modules.MusicPlayer.Command.MusicPlayerCommandList;
	
	import flash.display.Bitmap;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	public class SoundController
	{
		private var soundSwitchOn:Bitmap;
		private var soundSwitchOff:Bitmap;
		private var mcVolume:MovieClip;
		private var spriteOn:Sprite;
		private var spriteOff:Sprite;
		private var isLogin:Boolean;
		private var _point:Point;
		private var isClose:Boolean = false;
		private var _soundType:int = 0;				/** 控制的声音类型 1是音乐 2是音效*/
		private var _soundControllName:String;
		private var _uiLayer:Sprite = new Sprite();	/** 层 */
		
		public var clickFunction:Function;			/** 点击小喇叭事件 */
		/** 如果isLogin = true声音就是一键关闭，否则有调控器 */
		public function SoundController(path:String , point:Point , isLogin:Boolean = true , soundOnBitmap:Bitmap = null , soundOffBitmap:Bitmap = null , isSwc:Boolean = false , soundType:int = 1 , uiLayer:Sprite = null,soundControllName:String = null) //1是音乐    2是音效
		{
			if(!uiLayer) this._uiLayer = GameCommonData.GameInstance.GameUI;
			else  this._uiLayer = uiLayer;
			if(!soundControllName) _soundControllName = "SoundControll";
			else this._soundControllName = soundControllName;
			this.isLogin = isLogin;
			this._soundType = soundType;
			_point = point;
			if(isSwc == false)
			{
				soundSwitchOn    = GameCommonData.GameInstance.Content.Load(path).GetClassByBitmap("BitmapSoundOn") as Bitmap;
				soundSwitchOff   = GameCommonData.GameInstance.Content.Load(path).GetClassByBitmap("BitmapSoundOff") as Bitmap;  
				mcVolume   = GameCommonData.GameInstance.Content.Load(path).Content.applicationDomain.hasDefinition(_soundControllName) == false ? null : GameCommonData.GameInstance.Content.Load(path).GetClassByMovieClip(_soundControllName) as MovieClip;
			}
			else
			{
				if(soundOnBitmap == null || soundOffBitmap == null)
				{
					return;
				}
				soundSwitchOn = soundOnBitmap;
				soundSwitchOff = soundOffBitmap;
			}
	        spriteOn = new Sprite();
	        spriteOff = new Sprite(); 
	        spriteOn.name = "btn_SoundOn";
	        spriteOff.name = "btn_SoundOff";
	        spriteOn.addChild(soundSwitchOn);
	        spriteOff.addChild(soundSwitchOff);
			_uiLayer.addChild(spriteOn);
			_uiLayer.addChild(spriteOff);
			spriteOn.buttonMode = true;
			spriteOff.buttonMode = true;
			spriteOn.x = point.x;
			spriteOn.y = point.y;
			spriteOff.x = point.x;
			spriteOff.y = point.y;
			showSwitch();
			spriteOn.addEventListener(MouseEvent.CLICK,soundOnHandler);										  //点击声音开启状态
			spriteOff.addEventListener(MouseEvent.CLICK,soundOnHandler);									  //点击声音关闭状态
			if(mcVolume != null)
			{
				mcVolume.x = point.x;
				mcVolume.y = point.y + soundSwitchOn.height;
			}
			if(isLogin == false)				//这个判断是需要只出现控制条不需要喇叭的情况
			{
				_uiLayer.removeChild(spriteOn);
				_uiLayer.removeChild(spriteOff);
				volumeController();
			}
		}
		/** 点击声音开启状态，弹出音量管理 或 关闭音乐*/     
		private function soundOnHandler(e:MouseEvent):void
		{
			if(mcVolume == null || this.isLogin == true)
			{
				if(clickFunction != null) clickFunction();
				if( GameCommonData.isOpenFightSoundSwitch == false && GameCommonData.isOpenSoundSwitch == false)
				{
					GameCommonData.isOpenFightSoundSwitch  = true;	//控制音效
					openSound();									//控制音乐
				} 
				else if(GameCommonData.isOpenFightSoundSwitch == true || GameCommonData.isOpenSoundSwitch == true)
				{
					GameCommonData.isOpenFightSoundSwitch  = false;	//控制音效
					closeSound();
				}
			}
			else
			{
				if(_uiLayer.contains(mcVolume))
				{
					gcAll();
					return;
				}
				volumeController();
				blockPlace();
				e.stopPropagation();
			}
		}   
		
		/** 点击舞台 */
		private function stageClickHandler(e:MouseEvent):void
		{
			gcAll();
		}
		/** 点击减小音量按钮 */
		private function reduceClickHandler(e:MouseEvent):void
		{
			mcVolume.mcBlock.x -= mcVolume.mcBar.width / 10;
			if(mcVolume.mcBlock.x <= mcVolume.mcBar.x) mcVolume.mcBlock.x = mcVolume.mcBar.x;
			updateVolume();	
		}
		/** 点击增加音量按钮 */
		private function addClickHandler(e:MouseEvent):void
		{
			mcVolume.mcBlock.x += mcVolume.mcBar.width / 10;
			if(mcVolume.mcBlock.x >= (mcVolume.mcBar.x + mcVolume.mcBar.width)) mcVolume.mcBlock.x = mcVolume.mcBar.x + mcVolume.mcBar.width;
			updateVolume();	
		}
		
		/** 点击音量条 */
		private function barClickHandler(e:MouseEvent):void
		{
			mcVolume.mcBlock.x = mcVolume.mouseX;
			updateVolume();
			e.stopPropagation();	
		}
		
		/** 点击音量控制器 */
		private function mcVolumeClickHandler(e:MouseEvent):void
		{
			e.stopPropagation();
		}
		
		/** 控制音量 */
		private function volumeController():void
		{
			useEnabled = true;
			_uiLayer.addChild(mcVolume);
		}
		/** 拖动音量块 */
		private function mouseDownHandler(e:MouseEvent):void
		{
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_UP , mouseUpHandler);//_uiLayer.stage
			(mcVolume.mcBlock as MovieClip).startDrag(false , new Rectangle(mcVolume.mcBar.x , mcVolume.mcBlock.y ,mcVolume.mcBar.width ,0));
			mcVolume.mcBlock.addEventListener(Event.ENTER_FRAME , enterFrameHandler);
			e.stopPropagation();
		}
		/** 滑块滚动 */
		private function enterFrameHandler(e:Event):void
		{
			updateVolume();
		}
		/** 释放音量块 */
		private function mouseUpHandler(e:MouseEvent):void
		{
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_UP , mouseUpHandler);
			mcVolume.mcBlock.removeEventListener(Event.ENTER_FRAME , enterFrameHandler);
			(mcVolume.mcBlock as MovieClip).stopDrag();
			updateVolume();
			e.stopPropagation();
		}
		/** 改变音量 */
		private function updateVolume():void
		{
			soundVolume = (Number(mcVolume.mcBlock.x - mcVolume.mcBar.x) / mcVolume.mcBar.width) * 100
			if(soundVolume  <= 0)
			{
				soundVolume = 0;
				spriteOn.visible = false;
				spriteOff.visible = true;
			} 
			else																		//音量不为0
			{
				spriteOn.visible = true;
				spriteOff.visible = false;
			} 
			if(isLogin)
			{
				if(this._soundType == 1) AudioController.loginBgSound.Volume = GameCommonData.soundVolume ;
			} 
			else GameCommonData.GameInstance.GameScene.GetGameScene.SetVolume(GameCommonData.soundVolume );
			
			if (this._soundType == 1)
			{
				// 播放器音量同步
				GameCommonData.UIFacadeIntance.sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: GameCommonData.soundVolume, updateUI: true, from: this});
			}
		}
		/** 显示当前滑块的位置 */
		private function blockPlace():void
		{
			mcVolume.mcBlock.x = soundVolume / 100 * mcVolume.mcBar.width + mcVolume.mcBar.x;  
			if(mcVolume.mcBlock.x > (mcVolume.mcBar.x + mcVolume.mcBar.width)) mcVolume.mcBlock.x = mcVolume.mcBar + mcVolume.mcBar.width;
			if(soundSwitch == false)
			{
				mcVolume.mcBlock.x = mcVolume.mcBar.x;
			}
		}
		private function closeSound():void
		{
//			GameCommonData.isOpenFightSoundSwitch 	= false;
			GameCommonData.isOpenSoundSwitch		= false;
			spriteOn.visible = false;
			spriteOff.visible = true;
			if(mcVolume == null)  
			{
				AudioController.CloseLoginSwitch(soundSwitchOff , soundSwitchOn);
			}
			else
			{
//				GameCommonData.GameInstance.GameScene.GetGameScene.MusicStop();
				GameCommonData.UIFacadeIntance.sendNotification(MusicPlayerCommandList.MUSICPLAYER_STOP);
			}
		}
		private function openSound():void
		{
//			GameCommonData.isOpenFightSoundSwitch 	= true;
			GameCommonData.isOpenSoundSwitch		= true;
			spriteOn.visible = true;
			spriteOff.visible = false;
			if(mcVolume == null) AudioController.OpenLoginSwitch(soundSwitchOff , soundSwitchOn);
			else 
			{
//				GameCommonData.GameInstance.GameScene.GetGameScene.MusicLoad(soundVolume);
				GameCommonData.UIFacadeIntance.sendNotification(MusicPlayerCommandList.MUSICPLAYER_RESUME);
			}
		}
		/** 清除 */
		public function clearSoundSwitch():void
		{
			if(soundSwitchOn == null || soundSwitchOn == null)
			{
				return;
			}
			spriteOn.removeEventListener(MouseEvent.CLICK,soundOnHandler);										
			spriteOff.removeEventListener(MouseEvent.CLICK,soundOnHandler);	
			_uiLayer.removeChild(spriteOn);
			_uiLayer.removeChild(spriteOff);	
			spriteOn = null;	
			spriteOff = null;	
		}
		/** 是否可视 */
		public function isableSee(isSee:Boolean):void
		{
			if(isSee == false)
			{
				spriteOn.visible = false;
				spriteOff.visible = false;
			}
			else
			{
				showSwitch();
			}
		}
		private function showSwitch():void
		{
			if(soundSwitch == false)
			{
				spriteOn.visible = false;
				spriteOff.visible = true;
			}
			else
			{
				spriteOn.visible = true;
				spriteOff.visible = false;
			}
		}
		private function gcAll():void
		{
			mcVolume.mcBlock.removeEventListener(MouseEvent.MOUSE_DOWN , mouseDownHandler);
			_uiLayer.stage.removeEventListener(MouseEvent.MOUSE_UP , mouseUpHandler);
			mcVolume.mcBar.removeEventListener(MouseEvent.CLICK, barClickHandler);
			spriteOn.stage.removeEventListener(MouseEvent.CLICK , stageClickHandler);							  //点击舞台
			_uiLayer.removeChild(mcVolume);
			mcVolume.btnReduce.removeEventListener(MouseEvent.CLICK , reduceClickHandler);					    	//减小音量
			mcVolume.btnAdd.removeEventListener(MouseEvent.CLICK , addClickHandler);						    	//增加音量
		}
//		public static function loaderSound(isClear:Boolean = false):void
//		{
//			if(isClear == false)
//			{
//				var soundSwitch:Function = 
//				var isSoundOn:Boolean = soundSwitch();
//				if(isSoundOn) openSound();
//				else closeSound();	
//			}
//			else soundSwitch = null;
//		}
		private function get soundVolume():int
		{
			var volume:int = 0;
			if(_soundType == 1)  volume = GameCommonData.soundVolume; 										//控制的是音效还是音乐
			else if(_soundType == 2) volume = GameCommonData.fightSoundVolume; 
			return volume;
		}
		private function set soundVolume(value:int):void
		{
			if(_soundType == 1) //控制的是音效还是音乐
				GameCommonData.soundVolume = value;
			else if(_soundType == 2) GameCommonData.fightSoundVolume = value; 
		}
		
		public function setVolume(value:int):void
		{
			GameCommonData.soundVolume = value;
			mcVolume.mcBlock.x = value / 100 * mcVolume.mcBar.width + mcVolume.mcBar.x;
		}
		
		public function getVolume():int
		{
			return soundVolume;
		}
		
		/** 控制的声音開關*/
		private function get soundSwitch():Boolean
		{
			var _soundSwitch:Boolean;
			if(_soundType == 1) _soundSwitch = GameCommonData.isOpenSoundSwitch;
			else if(_soundType == 2) _soundSwitch = GameCommonData.isOpenFightSoundSwitch;
			return _soundSwitch;
		}
		private function set soundSwitch(_switch:Boolean):void
		{
			if(_soundType == 1)
			{
				GameCommonData.isOpenSoundSwitch = _switch;
				
				// 播放器同步
//				GameCommonData.UIFacadeIntance.sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: GameCommonData.soundVolume, updateUI: true});
			}
			else if(_soundType == 2) GameCommonData.isOpenFightSoundSwitch = _switch;
		}
		/** 设置调控器是否可点*/
		public function set useEnabled(isUse:Boolean):void
		{
			if(isUse == true)
			{
				mcVolume.addEventListener(MouseEvent.CLICK , mcVolumeClickHandler);
				mcVolume.mcBlock.addEventListener(MouseEvent.MOUSE_DOWN , mouseDownHandler);
				mcVolume.mcBar.addEventListener(MouseEvent.CLICK, barClickHandler);
				if(spriteOn.stage)spriteOn.stage.addEventListener(MouseEvent.CLICK , stageClickHandler);							  	//点击舞台
				mcVolume.btnReduce.addEventListener(MouseEvent.CLICK , reduceClickHandler);					    	//减小音量
				mcVolume.btnAdd.addEventListener(MouseEvent.CLICK , addClickHandler);						    	//增加音量
			}
			else
			{
				if(mcVolume.hasEventListener(MouseEvent.CLICK))  	mcVolume.removeEventListener(MouseEvent.CLICK , mcVolumeClickHandler);
				if(mcVolume.mcBlock.hasEventListener(MouseEvent.MOUSE_DOWN ))		mcVolume.mcBlock.removeEventListener(MouseEvent.MOUSE_DOWN , mouseDownHandler);
				if(mcVolume.mcBar.hasEventListener(MouseEvent.CLICK))		mcVolume.mcBar.removeEventListener(MouseEvent.CLICK, barClickHandler);
				if(spriteOn.stage && spriteOn.stage.hasEventListener(MouseEvent.CLICK))spriteOn.stage.removeEventListener(MouseEvent.CLICK , stageClickHandler);							  	//点击舞台
				if(mcVolume.btnReduce.hasEventListener(MouseEvent.CLICK))		mcVolume.btnReduce.removeEventListener(MouseEvent.CLICK , reduceClickHandler);					    	//减小音量
				if(mcVolume.btnAdd.hasEventListener(MouseEvent.CLICK))		mcVolume.btnAdd.removeEventListener(MouseEvent.CLICK , addClickHandler);						    	//增加音量
			}
		}
		public function get mcVolumeInstance():MovieClip
		{
			return this.mcVolume;
		}
		/** 设置声音关闭，开启 */
		public function soundOnOrOff(onSwitch:Boolean):void
		{
			if(onSwitch)
			{
				if(_soundType == 1)
				{
					openSound();
				}
				else GameCommonData.isOpenFightSoundSwitch 	= true;
			}
			else
			{
				if(_soundType == 1)
				{
					closeSound();
				}
				else GameCommonData.isOpenFightSoundSwitch 	= false;
			}
		}
		/** 喇叭禁用图标的显示 */
		public function soundPicSet():void
		{
			if( GameCommonData.isOpenFightSoundSwitch == false && GameCommonData.isOpenSoundSwitch == false)
			{
				spriteOn.visible = false;
				spriteOff.visible = true;						
			} 
			else if(GameCommonData.isOpenFightSoundSwitch == true || GameCommonData.isOpenSoundSwitch == true)
			{
				spriteOn.visible = true;
				spriteOff.visible = false;
			} 
		}
		/**点击设置面板上默认设置时将音效调回50%*/
		public function setMcBlockPlace():void
		{
			mcVolume.mcBlock.x = 80;
			updateVolume();
		}
	}
}