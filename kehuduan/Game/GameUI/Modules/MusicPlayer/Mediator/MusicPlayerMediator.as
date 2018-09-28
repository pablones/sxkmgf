package GameUI.Modules.MusicPlayer.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.MusicPlayer.Command.MusicPlayerCommandList;
	import GameUI.Modules.MusicPlayer.Data.MusicPlaylist;
	import GameUI.Modules.MusicPlayer.View.MusicPlayerPlaylistItemRenderer;
	import GameUI.Modules.MusicPlayer.View.PlayerProgressUpdater;
	import GameUI.Modules.MusicPlayer.View.PlaylistUI;
	import GameUI.Modules.MusicPlayer.View.PlaylistVScrollBar;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.UIKit.components.Scroller;
	import GameUI.View.UIKit.layout.VerticalLayout;
	
	import OopsEngine.Scene.CommonData;
	
	import OopsFramework.Audio.AudioEngine;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MusicPlayerMediator extends Mediator
	{
		public static const NAME:String = "MusicPlayerMediator";
		public const DEFAULT_POS:Point = new Point(500, 60);
		
		protected var dataProxy:DataProxy = null;
		protected var basePanel:PanelBase;
		protected var playerView:MovieClip;
		protected var playerButton:SimpleButton;
		protected var minimizeButton:SimpleButton;
		protected var playlistUI:PlaylistUI;
		
		public function MusicPlayerMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				MusicPlayerCommandList.TOGGLE_MUSICPLAYER_UI,
				MusicPlayerCommandList.MUSICPLAYER_PLAY,
				MusicPlayerCommandList.MUSICPLAYER_PAUSE,
				MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO,
				MusicPlayerCommandList.MUSICPLAYER_SCENECHANGE,
				MusicPlayerCommandList.MUSICPLAYER_VOLUME,
				MusicPlayerCommandList.MUSICPLAYER_STOP,
				MusicPlayerCommandList.MUSICPLAYER_RESUME
			];
		}
		
		protected function showView():void
		{
			if (!initialized)
			{
				showViewPending = true;
			}
			if (dataProxy.musicPlayerIsOpen) return;
			
			dataProxy.musicPlayerIsOpen = true;
			UIConstData.FocusIsUsing = true;
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
			if (progressUpdater)
			{
				progressUpdater.updatePlayBar = true;
			}
		}
		
		protected var playlist:MusicPlaylist;
		protected var loader:BulkLoaderResourceProvider = new BulkLoaderResourceProvider();;
		
		protected function loadPlaylist():void
		{
			loader.Download.Add(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/MusicPlaylist.xml");
			loader.LoadComplete = loadPlaylistCallback;
			loader.Load();
		}
		
		protected function loadPlaylistCallback():void
		{
			var playlistXML:XML = loader.GetResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/MusicPlaylist.xml").GetXML();
			if (playlistXML)
			{
				playlist = new MusicPlaylist(playlistXML);
			}
			
			configUI();
			registerEventListeners();
			
			initialized = true;
		}
		
		protected var initialized:Boolean = false;
		protected var showViewPending:Boolean = false;
		
		protected function onPanelLoadComplete():void
		{
			var swf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/MusicPlayer.swf");
			playerView = new (swf.loaderInfo.applicationDomain.getDefinition("PlayerMain"));
			this.setViewComponent(playerView);
			
			this.basePanel = new PanelBase(playerView, playerView.width + 11, playerView.height + 12);
			playerView.x += 2;
			playerView.y -= 1;
			this.basePanel.x = DEFAULT_POS.x;
			this.basePanel.y = DEFAULT_POS.y;
			
			this.basePanel.SetTitleTxt(GameCommonData.wordDic["mod_mus_med_mus_onp_1"]); // 音乐播放器
			
			playerButton = new (swf.loaderInfo.applicationDomain.getDefinition("PlayerButton"));
			playerButton.x = 897 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;;
			playerButton.y = 511 + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
			playerButton.name = "musicPlayerBtn";
			playerButton.addEventListener(MouseEvent.CLICK,
				function(event:MouseEvent):void
				{
					sendNotification(MusicPlayerCommandList.TOGGLE_MUSICPLAYER_UI);
				});
			GameCommonData.GameInstance.GameUI.addChildAt(playerButton, 0);
			
			minimizeButton = new (swf.loaderInfo.applicationDomain.getDefinition("MinimizeButton"));
			minimizeButton.x = 215;
			minimizeButton.y = 2;
			this.basePanel.addChild(minimizeButton);
			
			loadPlaylist();
			
			if (showViewPending)
			{
				sendNotification(MusicPlayerCommandList.TOGGLE_MUSICPLAYER_UI);
			}
			
			sendNotification(EventList.STAGECHANGE);
		}
		
		protected var progressUpdater:PlayerProgressUpdater;
		
		protected function configUI():void
		{
			(playerView.btnPlay as SimpleButton).visible = false;
			(playerView.btnUnmute as SimpleButton).visible = false;
			(playerView.mcVolBarClickMask as MovieClip).buttonMode = (playerView.mcVolBarClickMask as MovieClip).useHandCursor = true;
			(playerView.mcPlayBarClickMask as MovieClip).buttonMode = (playerView.mcPlayBarClickMask as MovieClip).useHandCursor = true;
			(playerView.mcPlayhead as MovieClip).buttonMode = (playerView.mcPlayhead as MovieClip).useHandCursor = true;
			(playerView.mcMode.txtModeName as TextField).mouseEnabled = false;
			
			// 播放列表
			var listScroller:Scroller = new Scroller();
			playlistUI = new PlaylistUI();
			
			playlistUI.data = playlist;
			playlistUI.itemRenderer = MusicPlayerPlaylistItemRenderer;
			playlistUI.selectedIndex = 0;
			playlistUI.highlightedIndex = 0;
			(playlistUI.layout as VerticalLayout).gap = 2;
			
			listScroller.x = 1;
			listScroller.y = 1;
			listScroller.width = playerView.mcPlaylistContainer.width - 2;
			listScroller.height = playerView.mcPlaylistContainer.height - 2;
			listScroller.content = playlistUI;
			listScroller.verticalScrollBar = new PlaylistVScrollBar();
			
//			playlistUI.addEventListener(IndexChangeEvent.CHANGED, playlistUI_changeHandler);
			
			playerView.mcPlaylistContainer.addChild(listScroller);
			
			progressUpdater = new PlayerProgressUpdater(playerView);
			// TODO(zhao): 这里是个临时 fix，因为第一次主题曲不经过播放器流程，所以 progressUpdater 根本没法获取总时长
			progressUpdater.songLength = 304;
			
			(playerView.mcPlayBar as MovieClip).width = 1;
			(playerView.mcPlayhead as MovieClip).x = (playerView.mcPlayBar as MovieClip).x;
		}
		
		protected function registerEventListeners():void
		{
			this.basePanel.addEventListener(Event.CLOSE, panelCloseHandler);
			(playerView.btnPrev as SimpleButton).addEventListener(MouseEvent.CLICK, btnPrev_clickHandler);
			(playerView.btnNext as SimpleButton).addEventListener(MouseEvent.CLICK, btnNext_clickHandler);
			(playerView.btnPlay as SimpleButton).addEventListener(MouseEvent.CLICK, btnPlay_clickHandler);
			(playerView.btnPause as SimpleButton).addEventListener(MouseEvent.CLICK, btnPause_clickHandler);
			(playerView.btnMute as SimpleButton).addEventListener(MouseEvent.CLICK, btnMute_clickHandler);
			(playerView.btnUnmute as SimpleButton).addEventListener(MouseEvent.CLICK, btnUnmute_clickHandler);
			
			(playerView.mcMode as MovieClip).addEventListener(MouseEvent.CLICK, mcMode_clickHandler);
			
			(playerView.mcVolBarClickMask as MovieClip).addEventListener(MouseEvent.CLICK, mcVolBarClickMask_clickHandler);
			(playerView.btnVolDrag as SimpleButton).addEventListener(MouseEvent.MOUSE_DOWN, btnVolDrag_mouseDownHandler);
			(playerView.mcPlayBarClickMask as MovieClip).addEventListener(MouseEvent.CLICK, mcPlayBarClickMask_clickHandler);
			(playerView.mcPlayhead as MovieClip).gotoAndStop(1);
			(playerView.mcPlayhead as MovieClip).addEventListener(MouseEvent.ROLL_OVER, mcPlayhead_rollOverHandler);
			(playerView.mcPlayhead as MovieClip).addEventListener(MouseEvent.ROLL_OUT, mcPlayhead_rollOutHandler);
			(playerView.mcPlayhead as MovieClip).addEventListener(MouseEvent.MOUSE_DOWN, mcPlayhead_mouseDownHandler);
			
			minimizeButton.addEventListener(MouseEvent.CLICK, minimizeButton_clickHandler);
		}	
		
		protected function panelCloseHandler(event:Event):void
		{
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			dataProxy.musicPlayerIsOpen = false;
			UIConstData.FocusIsUsing = false;
			mode = PlayerMode.DEFAULT;
			(playerView.mcMode as MovieClip).gotoAndStop(1);
			(playerView.mcMode.txtModeName as TextField).text = GameCommonData.wordDic["mod_mus_med_mus_mod_1"]; // 默认
			
			if (CommonData.audioEngine && GameCommonData.isOpenSoundSwitch)
			{
				CommonData.audioEngine.Stop();
				next();
			}
		}
		
		protected function minimizeButton_clickHandler(event:MouseEvent):void
		{
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			dataProxy.musicPlayerIsOpen = false;
			UIConstData.FocusIsUsing = false;
		}
		
		protected function btnPrev_clickHandler(event:MouseEvent):void
		{
			if (!GameCommonData.isOpenSoundSwitch) 
			{
				sendNotification(EventList.SHOWALERT, {title: GameCommonData.wordDic["mod_mus_med_mus_1"], info: GameCommonData.wordDic["mod_mus_med_mus_2"], comfrim:function():void{}});
				return;
			}
			next(true, false);
		}
		
		protected function btnNext_clickHandler(event:MouseEvent):void
		{
			if (!GameCommonData.isOpenSoundSwitch) 
			{
				sendNotification(EventList.SHOWALERT, {title: GameCommonData.wordDic["mod_mus_med_mus_1"], info: GameCommonData.wordDic["mod_mus_med_mus_2"], comfrim:function():void{}});
				return;
			}
			next(true, true);
		}
		
		protected function btnPlay_clickHandler(event:MouseEvent):void
		{
			if (!GameCommonData.isOpenSoundSwitch) 
			{
				sendNotification(EventList.SHOWALERT, {title: GameCommonData.wordDic["mod_mus_med_mus_1"], info: GameCommonData.wordDic["mod_mus_med_mus_2"], comfrim:function():void{}});
				return;
			}
			sendNotification(MusicPlayerCommandList.MUSICPLAYER_PLAY);
		}	
		
		protected function btnPause_clickHandler(event:MouseEvent):void
		{
			sendNotification(MusicPlayerCommandList.MUSICPLAYER_PAUSE);
		}
		
		protected var _lastVol:Number;
		
		protected function btnMute_clickHandler(event:MouseEvent):void
		{
			_lastVol = GameCommonData.soundVolume;
			(playerView.btnMute as SimpleButton).visible = false;
			(playerView.btnUnmute as SimpleButton).visible = true;
			sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: 0, updateUI: true, from: this});
		}
		
		protected function btnUnmute_clickHandler(event:MouseEvent):void
		{	
			(playerView.btnMute as SimpleButton).visible = true;
			(playerView.btnUnmute as SimpleButton).visible = false;
			sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: _lastVol, updateUI: true, from: this});
		}
		
		protected function btnVolDrag_mouseDownHandler(event:MouseEvent):void
		{	
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_MOVE, stage_volDragHandler);
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_UP, stage_volDragStopHandler);
		}
		
		protected function stage_volDragHandler(event:MouseEvent):void
		{
			var btnVolDrag:SimpleButton = (playerView.btnVolDrag as SimpleButton);
			var v:Number = playerView.mouseX;
			if (v < 110) v = 110;
			if (v > 160) v = 160;
			btnVolDrag.x = v;
			
			(playerView.mcVolBar as MovieClip).width = v - 104;
			
			sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: ((v - 110) * 2), updateUI: false, from: this}); // (v - 110) / (160 - 110) * 100
		}
		
		protected function stage_volDragStopHandler(event:MouseEvent):void
		{
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_MOVE, stage_volDragHandler);
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_UP, stage_volDragStopHandler);
		}
		
		protected function mcVolBarClickMask_clickHandler(event:MouseEvent):void
		{
			sendNotification(MusicPlayerCommandList.MUSICPLAYER_VOLUME, {vol: (((playerView.mcVolBar as MovieClip).x + event.localX) - 110) * 2, updateUI: true, from: this}); 
		}
		
		protected function mcPlayBarClickMask_clickHandler(event:MouseEvent):void
		{
			// 这里需要发一个“播放跳转”通知吗？好像不大可能有其他模块用到这个。  --zhao
			if (CommonData.audioEngine)
			{
				if (CommonData.audioEngine.IsPlaying)
				{
					CommonData.audioEngine.Pause();
					CommonData.audioEngine.Position = event.localX * playerView.mcPlayBarClickMask.scaleX / (playerView.mcPlayBarClickMask.width) * playlist.getItemAt(_current).length * 1000;
					CommonData.audioEngine.Pause();
					CommonData.audioEngine.PlayComplete = next;
				}
				else
				{
					CommonData.audioEngine.Position = event.localX * playerView.mcPlayBarClickMask.scaleX / (playerView.mcPlayBarClickMask.width) * playlist.getItemAt(_current).length * 1000;
					CommonData.audioEngine.PlayComplete = next;
				}
			}
		}
		
		protected function mcPlayhead_rollOverHandler(event:MouseEvent):void
		{
			(playerView.mcPlayhead as MovieClip).gotoAndPlay(2);
		}
		
		protected function mcPlayhead_rollOutHandler(event:MouseEvent):void
		{
			(playerView.mcPlayhead as MovieClip).gotoAndPlay(8);
		}
		
		
		protected function mcMode_clickHandler(event:MouseEvent):void
		{
			popUpMenu(playerView.mcMode as MovieClip);
		}
		
		protected function mcPlayhead_mouseDownHandler(event:MouseEvent):void
		{
			if (!CommonData.audioEngine)
			{
				return;
			}
			
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_MOVE, stage_playheadDragHandler);
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_UP, stage_playheadDragStopHandler);
			
			// 快速拖动时，鼠标可能离开播放头
			(playerView.mcPlayhead as MovieClip).removeEventListener(MouseEvent.ROLL_OVER, mcPlayhead_rollOverHandler);
			(playerView.mcPlayhead as MovieClip).removeEventListener(MouseEvent.ROLL_OUT, mcPlayhead_rollOutHandler);
		}
		
		protected function stage_playheadDragHandler(event:MouseEvent):void
		{
			if (progressUpdater)
			{
				progressUpdater.updatePlayBar = false;
			}
			
			var min:Number = (playerView.mcPlayBarClickMask as MovieClip).x;
			var max:Number = min + (playerView.mcPlayBarClickMask as MovieClip).width;
			var v:Number = playerView.mouseX;
			if (v < min) v = min;
			if (v > max) v = max;
			
			var playhead:MovieClip = (playerView.mcPlayhead as MovieClip);
			playhead.x = v;
			(playerView.mcPlayBar as MovieClip).width = playhead.x - (playerView.mcPlayBar as MovieClip).x;
		}
		
		protected function stage_playheadDragStopHandler(event:MouseEvent):void
		{
			if (progressUpdater)
			{
				progressUpdater.updatePlayBar = true;
			}
			
			if (CommonData.audioEngine)
			{
				if (CommonData.audioEngine.IsPlaying)
				{
					CommonData.audioEngine.Pause();
					CommonData.audioEngine.Position = (playerView.mcPlayhead.x
																			- playerView.mcPlayBarClickMask.x)
																			/ (playerView.mcPlayBarClickMask.width)
																			* playlist.getItemAt(_current).length
																			* 1000;
					CommonData.audioEngine.Pause();
					CommonData.audioEngine.PlayComplete = next;
				}
				else
				{
					CommonData.audioEngine.Position = (playerView.mcPlayhead.x
																			- playerView.mcPlayBarClickMask.x)
																			/ (playerView.mcPlayBarClickMask.width)
																			* playlist.getItemAt(_current).length
																			* 1000;
					CommonData.audioEngine.PlayComplete = next;
				}
			}
			
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_MOVE, stage_playheadDragHandler);
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_UP, stage_playheadDragStopHandler);
			
			(playerView.mcPlayhead as MovieClip).addEventListener(MouseEvent.ROLL_OVER, mcPlayhead_rollOverHandler);
			(playerView.mcPlayhead as MovieClip).addEventListener(MouseEvent.ROLL_OUT, mcPlayhead_rollOutHandler);
		}
		
		protected var modeMenu:MenuItem;
		protected var menuShowing:Boolean = false;
		
		protected function popUpMenu(anchor:DisplayObject):void
		{
			if (!modeMenu)
			{
				buildMenu();
			}
			
			var m:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if (m)
			{
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			
			var pos:Point = anchor.localToGlobal(new Point(0, anchor.height + 1));
			modeMenu.x = pos.x;
			modeMenu.y = pos.y;
			modeMenu.width = 80;
			GameCommonData.GameInstance.GameUI.addChild(modeMenu);
			GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.MOUSE_DOWN, stage_mouseDownHandler);
		}
		
		protected function buildMenu():void
		{
			modeMenu = new MenuItem();
			
			var menuStr:Array = ["　 " + GameCommonData.wordDic["mod_mus_med_mus_bui_1"], // 默认模式
												 "　 " + GameCommonData.wordDic["mod_mus_med_mus_bui_2"], // 顺序播放
												 "　 " + GameCommonData.wordDic["mod_mus_med_mus_bui_3"], // 单曲循环
												 "　 " + GameCommonData.wordDic["mod_mus_med_mus_bui_4"]  // 随机播放
												];
												
			switch (mode)
			{
				case PlayerMode.DEFAULT:
					menuStr[0] = "√ " + GameCommonData.wordDic["mod_mus_med_mus_bui_1"];
					break;
				case PlayerMode.SEQUENTIAL:
					menuStr[1] = "√ " + GameCommonData.wordDic["mod_mus_med_mus_bui_2"];
					break;
				case PlayerMode.SINGLE:
					menuStr[2] = "√ " + GameCommonData.wordDic["mod_mus_med_mus_bui_3"];
					break;
				case PlayerMode.RANDOM:
					menuStr[3] = "√ " + GameCommonData.wordDic["mod_mus_med_mus_bui_4"];
					break; 
			}
												
			var menuData:Array = [{cellText: menuStr[0], data: {mode: PlayerMode.DEFAULT}},
												 {cellText: menuStr[1], data: {mode: PlayerMode.SEQUENTIAL}},
												 {cellText: menuStr[2], data: {mode: PlayerMode.SINGLE}},
												 {cellText: menuStr[3], data: {mode: PlayerMode.RANDOM}}
												];
			
			modeMenu.dataPro = menuData;
			modeMenu.addEventListener(MenuEvent.Cell_Click, modeMenu_cellClickHandler);
		}
		
		protected function destroyMenu():void
		{
			modeMenu.removeEventListener(MenuEvent.Cell_Click, modeMenu_cellClickHandler);
			modeMenu.parent.removeChild(modeMenu);
			modeMenu = null;
			GameCommonData.GameInstance.GameUI.stage.removeEventListener(MouseEvent.MOUSE_DOWN, stage_mouseDownHandler);
		}
		
		protected function modeMenu_cellClickHandler(event:MenuEvent):void
		{
//			trace(">>> mode: " + event.cell.data.mode);
			mode = event.cell.data.mode.toString();
			
			switch (mode)
			{
				case PlayerMode.DEFAULT:
					(playerView.mcMode.txtModeName as TextField).text = GameCommonData.wordDic["mod_mus_med_mus_mod_1"]; // 默认
					break;
				case PlayerMode.SEQUENTIAL:
					(playerView.mcMode.txtModeName as TextField).text = GameCommonData.wordDic["mod_mus_med_mus_mod_2"]; // 顺序
					break;
				case PlayerMode.SINGLE:
					(playerView.mcMode.txtModeName as TextField).text = GameCommonData.wordDic["mod_mus_med_mus_mod_3"]; // 单曲
					break;
				case PlayerMode.RANDOM:
					(playerView.mcMode.txtModeName as TextField).text = GameCommonData.wordDic["mod_mus_med_mus_mod_4"]; // 随机
					break;
			}
			
			CommonData.IsMusicPlayerBusy = true;
			CommonData.audioEngine.PlayComplete = next;
			
			var mcMode:MovieClip = (playerView.mcMode as MovieClip);
			
			switch (mode)
			{
				case PlayerMode.DEFAULT:
					mcMode.gotoAndStop(1);
					break;
				case PlayerMode.SEQUENTIAL:
					mcMode.gotoAndStop(2);
					break;
				case PlayerMode.SINGLE:
					mcMode.gotoAndStop(3);
					break;
				case PlayerMode.RANDOM:
					mcMode.gotoAndStop(4);
					break;
			}
			
			destroyMenu();
		}
		
		protected function stage_mouseDownHandler(event:MouseEvent):void
		{
			if (GameCommonData.GameInstance.GameUI.contains(modeMenu) && !modeMenu.contains(event.target as DisplayObject))
			{
				destroyMenu();
			}
		}
		
		
		// 播放细节
		
		protected var mode:String = PlayerMode.DEFAULT;
		
		protected var _current:int = 0;
		
		protected function get current():int
		{
			return _current;
		}
		
		protected function set current(value:int):void
		{
//			trace(">>> current: " + current + " --> " + value);
			_current = value;
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case EventList.INITVIEW:
					ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/MusicPlayer.swf", onPanelLoadComplete);
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					break;
				case MusicPlayerCommandList.TOGGLE_MUSICPLAYER_UI:
					if (!dataProxy.musicPlayerIsOpen)
					{
						this.showView();
					}
					else
					{
						this.minimizeButton_clickHandler(null);
					}
					break;
				case MusicPlayerCommandList.MUSICPLAYER_PLAY:
					if (CommonData.audioEngine && !CommonData.audioEngine.IsPlaying)
						CommonData.audioEngine.Pause();
//					if (mode == PlayerMode.DEFAULT) CommonData.IsMusicPlayerBusy = false;
					(playerView.btnPause as SimpleButton).visible = true;
					(playerView.btnPlay as SimpleButton).visible = false;
					break;
				case MusicPlayerCommandList.MUSICPLAYER_PAUSE:
					if (CommonData.audioEngine && CommonData.audioEngine.IsPlaying)
						CommonData.audioEngine.Pause();
					(playerView.btnPause as SimpleButton).visible = false;
					(playerView.btnPlay as SimpleButton).visible = true;
					break;
				case MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO:
					if (!GameCommonData.isOpenSoundSwitch && dataProxy.musicPlayerIsOpen) 
					{
						sendNotification(EventList.SHOWALERT, {title: GameCommonData.wordDic["mod_mus_med_mus_1"], info: GameCommonData.wordDic["mod_mus_med_mus_2"], comfrim:function():void{}});
						return;
					}
					current = notification.getBody() as int;
					playlistUI.selectedIndex = playlistUI.highlightedIndex = current;
					
					CommonData.IsPlayThemeSong = false;
					CommonData.IsPlayThemeSongComplete = true;
					CommonData.IsMusicPlayerBusy = true;
					
					if (CommonData.audioEngine)
					{
						CommonData.audioEngine.Stop();
					}
					
					if (current == -1) break;
					
					holdMusic = true;
					var musicURL:String = ((notification.getBody() == 0) ?
													(GameCommonData.GameInstance.Content.RootDirectory + "LoginSound.mp3")
													: (GameCommonData.GameInstance.Content.RootDirectory + "Scene/" + playlist.getItemAt(notification.getBody() as int).sceneID.split(",")[0] + "/Sound.mp3")); 
					CommonData.audioEngine = new AudioEngine(musicURL);
					CommonData.audioEngine.Volume = GameCommonData.soundVolume;
					CommonData.audioEngine.Loop = 1;
					CommonData.audioEngine.Play();
					CommonData.audioEngine.PlayComplete = next;
					
					(playerView.btnPause as SimpleButton).visible = true;
					(playerView.btnPlay as SimpleButton).visible = false;
					
					(playerView.txtSongName as TextField).text = playlist.getItemAt(notification.getBody() as int).nameWithScene;
					if (progressUpdater)
					{
						progressUpdater.songLength = playlist.getItemAt(notification.getBody() as int).length;
					}
					break;
				case MusicPlayerCommandList.MUSICPLAYER_SCENECHANGE:
					CommonData.audioEngine.PlayComplete = next;
					if (!CommonData.IsPlayThemeSongComplete) // 还没有接管控制
						return;

					if ((mode == PlayerMode.DEFAULT) && !holdMusic)
					{
						if (CommonData.audioEngine)
						{
							CommonData.audioEngine.Stop();
						}
						next();
					}
					break;
				case MusicPlayerCommandList.MUSICPLAYER_VOLUME:
					var v:Number = Number(notification.getBody().vol);
					if (v < 0) v = 0;
					if (v > 100) v = 100;
					GameCommonData.soundVolume = v;
					if (CommonData.audioEngine) CommonData.audioEngine.Volume = v;
					
					if (v > 0)
					{
						(playerView.btnMute as SimpleButton).visible = true;
						(playerView.btnUnmute as SimpleButton).visible = false;
					}
					
					if (notification.getBody().updateUI == true)
					{
						(playerView.mcVolBar as MovieClip).width = v / 2 + 6;
						(playerView.btnVolDrag as SimpleButton).x = (playerView.mcVolBar as MovieClip).x + (playerView.mcVolBar as MovieClip).width; 
					}
					break;
				case MusicPlayerCommandList.MUSICPLAYER_STOP:
					if (CommonData.audioEngine) CommonData.audioEngine.Stop();
					holdMusic = (mode != PlayerMode.DEFAULT);
					(playerView.btnPlay as SimpleButton).visible = true;
					(playerView.btnPause as SimpleButton).visible = false;
					break;
				case MusicPlayerCommandList.MUSICPLAYER_RESUME:
					if (CommonData.audioEngine && !CommonData.audioEngine.IsPlaying)
					{
						if (holdMusic)
						{
							CommonData.audioEngine.Play(0);
							CommonData.audioEngine.PlayComplete = next;
							(playerView.btnPlay as SimpleButton).visible = false;
							(playerView.btnPause as SimpleButton).visible = true;
						}
						else
						{
							next(false, false, true);
						}
					}
					else if (!CommonData.audioEngine)
					{
//						if (mode == PlayerMode.DEFAULT)
//						{
//							var sceneID:String = GameCommonData.GameInstance.GameScene.GetGameScene.name;
//							var items:Array = playlist.query("sceneID", sceneID);
//							_current = items[0]; 
//						}
						
						next(false, false, true);
						
//						var musicURL2:String = ((_current == 0) ?
//													(GameCommonData.GameInstance.Content.RootDirectory + "LoginSound.mp3")
//													: (GameCommonData.GameInstance.Content.RootDirectory + "Scene/" + playlist.getItemAt(_current).sceneID + "/Sound.mp3")); 
//						CommonData.audioEngine = new AudioEngine(musicURL2);
//						CommonData.audioEngine.Volume = GameCommonData.soundVolume;
//						CommonData.audioEngine.Loop = 1;
//						CommonData.audioEngine.Play();
//						CommonData.audioEngine.PlayComplete = next;
					}
					break;
			}
		}
		
		protected var holdMusic:Boolean = false;
		
		protected var nextTimeout:uint = 0;
		
		protected function next(manual:Boolean = false, inc:Boolean = true, immediate:Boolean = false):void
		{
			CommonData.IsPlayThemeSong = false;
			CommonData.IsPlayThemeSongComplete = true;
			
			if (nextTimeout > 0)
			{
				clearTimeout(nextTimeout);
			}
			
			(playerView.btnPlay as SimpleButton).mouseEnabled = false;
			(playerView.btnPause as SimpleButton).mouseEnabled = false;
			
//			if (!manual) playlistUI.selectedIndex = -1;
			
			nextTimeout = setTimeout(function():void{

			if (mode == PlayerMode.DEFAULT) // 默认模式
			{
				if (!manual) // 非用户干预
				{
					// 音乐结束以后，如果还是默认模式，那么释放当前 AE
					// 5.9 不释放 AE
//					CommonData.IsMusicPlayerBusy = false;
					
					var sceneID:String = GameCommonData.GameInstance.GameScene.GetGameScene.name;
					var items:Array = playlist.query("sceneID", sceneID);
					
					sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, items[0]);
					holdMusic = false;
					
//					GameCommonData.GameInstance.GameScene.GetGameScene.onPlayComplete();
				}
				else // 用户干预
				{
					// 同 SEQUENTIAL
					sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, (_current + (inc ? 1 : -1) + playlist.length) % playlist.length);
					holdMusic = true;
				}
			}
			else if (mode == PlayerMode.SEQUENTIAL) // 顺序模式
			{
				// 音乐结束以后，如果是顺序模式，那么播放下/上一首
				sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, (_current + (inc ? 1 : -1) + playlist.length) % playlist.length);
				holdMusic = true;
			}
			else if (mode == PlayerMode.SINGLE) // 单曲模式
			{
				if (!manual) // 非用户干预
				{
					sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, _current);
				}
				else
				{
					// 同 SEQUENTIAL
					sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, (_current + (inc ? 1 : -1) + playlist.length) % playlist.length);
				}
				holdMusic = true;
			}
			else if (mode == PlayerMode.RANDOM) // 随机模式
			{
				sendNotification(MusicPlayerCommandList.MUSICPLAYER_NAVIGATE_TO, shuffle());
				holdMusic = true;
			}
			
			(playerView.btnPlay as SimpleButton).mouseEnabled = true;
			(playerView.btnPause as SimpleButton).mouseEnabled = true;
			
			}, ((manual || immediate) ? 0 : 5000));
		}
		
		protected var randomList:Array = [];
		protected function shuffle():int
		{
			if (randomList.indexOf(current) == -1)
			{
				randomList.push(current);
			}
			
			if (randomList.length == playlist.length && current != -1) // TODO(zhao): 这里依然要处理主题曲...
			{
				randomList = [current];
			}
			
			var candidates:Array = [];
			var l:int = playlist.length;
			
			for (var i:int = 0; i < l; i++)
			{
				if (randomList.indexOf(i) == -1)
				{
					candidates.push(i);
				}
			}
			
//			trace(">>> candidates: " + candidates.join(", "));
			var result:int = candidates[Math.round(candidates.length * Math.random()) % candidates.length];
			randomList.push(result);
			
			return result;
		}
	}
}

class PlayerMode
{
	public static const DEFAULT:String = "default"; // 默认
	
	public static const SEQUENTIAL:String = "sequential"; // 顺序（列表循环）
	
	public static const SINGLE:String = "single"; // 单曲
	
	public static const RANDOM:String = "random"; // 随机
}