package GameUI.Modules.ReName.Mediator
{
	import Controller.PlayerController;
	
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.ReName.Data.ReNameData;
	import GameUI.Modules.ReName.Data.ReNameNetAction;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.VipShow.Data.VipShowData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ReNameMediator extends Mediator
	{
		public static const NAME:String = "reNameMediator";
		private var loader:Loader;
		private var reNameBtn:MovieClip;
		private var reNameView:MovieClip;
		private var panelBase:PanelBase;
		private var lastTime:uint;
		
		public function ReNameMediator( )
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					ReNameData.INIT_RENAME,
					ReNameData.SHOW_RENAME_VIEW,
					ReNameData.SHOW_RENAME_BUTTON,
					ReNameData.HIDE_RENAME_BUTTON,
					ReNameData.REPLY_RENAME
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case ReNameData.INIT_RENAME:
					loadView();
					break;
				case ReNameData.SHOW_RENAME_VIEW:
				    showView();
					break;
				case ReNameData.SHOW_RENAME_BUTTON:
					if( GameCommonData.canReName == 1 && !GameCommonData.GameInstance.GameUI.contains( reNameBtn )) 
					{
						showBtn();
						addListener();
					}
					break;
				case ReNameData.HIDE_RENAME_BUTTON:
					if( GameCommonData.GameInstance.GameUI.contains( reNameBtn ) )
					{
						GameCommonData.GameInstance.GameUI.removeChild( reNameBtn );
						reNameBtn.removeEventListener( MouseEvent.CLICK, onClick );
						if( ReNameData.RoleReNameIsOpen ) this.closeView( null );
					}
					break;
				case ReNameData.REPLY_RENAME:
				    var obj:Object = notification.getBody();
				    checkResult(obj);
					break;
			}
		}
		
		private function loadView():void
		{
			loader = new Loader();
			var request:URLRequest = new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/ReName.swf");
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onPicComplete);
			loader.load(request);
		}
		
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			var BgClass:Class;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "ReNameBtn" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "ReNameBtn" ) as Class;
				reNameBtn = new BgClass() as MovieClip;
			}
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( "ReNameView" ) )
			{
				BgClass = loader.contentLoaderInfo.applicationDomain.getDefinition( "ReNameView" ) as Class;
				reNameView = new BgClass() as MovieClip;
			}
			e.target.removeEventListener(Event.COMPLETE, onPicComplete);
			loader = null;
			
			if( GameCommonData.canReName == 1 && GameCommonData.GameInstance.GameUI.getChildByName("btnPreventWallow") == null )
			{
				showBtn();
				addListener();
			}
			
			initView();
		}
		
		private function initView():void
		{
			panelBase = new PanelBase( reNameView, reNameView.width+8, reNameView.height+14 );
//			reNameView.x += 1
			reNameView.txtInfo.maxChars = 7;
			reNameView.txtInfo.textColor = 0xffffff;
			reNameView.txtTitle.selectable = false;
		}
		
		private function showView():void
		{
			if( !ReNameData.UnityReNameIsOpen )
			{
				if( ReNameData.RoleReNameIsOpen )
				{
					closeView( null );
					ReNameData.RoleReNameIsOpen = false; 
				}
				panelBase.SetTitleTxt( "帮派改名" );
				panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
				panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				reNameView.txtTitle.text = "请输入帮派的新名字：";
				reNameView.txtInfo.text = "";
				reNameView.change_btn.addEventListener( MouseEvent.CLICK, clickFun );
				reNameView.txtInfo.addEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
				reNameView.txtInfo.addEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
	//			reNameView.txtInfo.addEventListener(Event.CHANGE,textInputHandler);
				panelBase.addEventListener( Event.CLOSE, closeView );
				reNameView.stage.focus = reNameView.txtInfo;
				ReNameData.UnityReNameIsOpen = true;	
			}else{
				closeView( null );
			}
		}
		
		private function showBtn():void
		{
			reNameBtn.x = 216;
			reNameBtn.y = 3;
			GameCommonData.GameInstance.GameUI.addChild( reNameBtn );
		}
		
		private function addListener():void
		{
			reNameBtn.addEventListener( MouseEvent.CLICK, onClick );
		}
		
		private function onClick( event:MouseEvent ):void
		{
			if( !ReNameData.RoleReNameIsOpen )
			{
				if( ReNameData.UnityReNameIsOpen )
				{
					closeView( null );
					ReNameData.UnityReNameIsOpen = false;
				}
				panelBase.SetTitleTxt( "角色改名" );          //角色改名
				panelBase.x = (GameCommonData.GameInstance.stage.stageWidth - panelBase.width)/2;
				panelBase.y = (GameCommonData.GameInstance.stage.stageHeight - panelBase.height)/2;;
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				reNameView.txtTitle.text = "请输入您的新名字：";         //请输入您的新名字：
				reNameView.txtInfo.text = "";
				reNameView.change_btn.addEventListener( MouseEvent.CLICK, clickFun );
				reNameView.txtInfo.addEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
				reNameView.txtInfo.addEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
//				reNameView.txtInfo.addEventListener(Event.CHANGE,textInputHandler);
				panelBase.addEventListener( Event.CLOSE, closeView );
				reNameView.stage.focus = reNameView.txtInfo;
				ReNameData.RoleReNameIsOpen = true;
			}else
			{
				closeView( null );
			}
		}
		
		private function closeView( e:Event ):void
		{
			GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			reNameView.change_btn.removeEventListener( MouseEvent.CLICK, clickFun );
			panelBase.removeEventListener( Event.CLOSE, closeView );
			reNameView.txtInfo.removeEventListener(FocusEvent.FOCUS_IN, onFoucsIn);
			reNameView.txtInfo.removeEventListener(FocusEvent.FOCUS_OUT, onFoucsOut);
//			reNameView.txtInfo.removeEventListener(Event.CHANGE,textInputHandler);
			GameCommonData.isFocusIn = false;
			ReNameData.RoleReNameIsOpen = false;
			ReNameData.UnityReNameIsOpen = false;
		}
		
		private function clickFun( e:MouseEvent ):void
		{
			if( ReNameData.RoleReNameIsOpen == true )
			{
				if( reNameView.txtInfo.text == "" )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"请输入角色名",color:0xffff00} );
					return;
				}
				if( reNameView.txtInfo.text == GameCommonData.Player.Role.Name )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"不能改为原来的名字",color:0xffff00} );
					return;
				}
				if( reNameView.txtInfo.text.length < 2 || reNameView.txtInfo.text.length > 7 )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"角色名必须是2~7字符",color:0xffff00} );
					return;
				} 
				if( reNameView.txtInfo.text.indexOf("御剑江湖") > -1 )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"角色名不合法",color:0xffff00} );
					return;
				}
				if(UIUtils.isPermitedRoleName(reNameView.txtInfo.text) == false || UIUtils.legalRoleName(reNameView.txtInfo.text) == false)				//有不合格字符
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"角色名不合法",color:0xffff00} );
					return;
				}
				if(getTimer() - lastTime < 5000)
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"5秒钟后方可再次修改",color:0xffff00} );
					return;
				}
			}else if( ReNameData.UnityReNameIsOpen == true )
			{
				if( reNameView.txtInfo.text == "" )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"请输入帮派名",color:0xffff00} );
					return;
				}
				if( reNameView.txtInfo.text == NewUnityCommonData.myUnityInfo.name )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"不能改为原来的名字",color:0xffff00} );
					return;
				}
				if( reNameView.txtInfo.text.length < 2 || reNameView.txtInfo.text.length > 7 )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"帮派名必须是2~7字符",color:0xffff00} );
					return;
				} 
				if( reNameView.txtInfo.text.indexOf("御剑江湖") > -1 )
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"帮派名不合法",color:0xffff00} );
					return;
				}
				if(UIUtils.isPermitedRoleName(reNameView.txtInfo.text) == false || UIUtils.legalRoleName(reNameView.txtInfo.text) == false)				//有不合格字符
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"帮派名不合法",color:0xffff00} );
					return;
				}
				if(getTimer() - lastTime < 5000)
				{
					sendNotification( HintEvents.RECEIVEINFO, {info:"5秒钟后方可再次修改",color:0xffff00} );
					return;
				}
			}
			
			lastTime = getTimer();
			reNameView.txtInfo.mouseEnabled = false;
			reNameView.change_btn.mouseEnabled = false;
			var _index:int;
			if( ReNameData.RoleReNameIsOpen == true )
			{
				_index = 0;
			}else if( ReNameData.UnityReNameIsOpen == true )
			{
				_index = 1;
			}
			ReNameNetAction.sendReName( reNameView.txtInfo.text, _index );
		}
		
		private function checkResult( object:Object ):void
		{
			var state:int = object.state as int;
			var index:int = object.index as int;
			var id:int = object.id as int;
			var name:String = object.name as String;
			
			switch( index )
			{
				case 0:      //角色改名
					if( state == 1 )
					{
						closeView( null );
						closeBtn();
						GameCommonData.Player.Role.Name = name;
						PlayerController.GetPlayer( id ).UpdatePersonName();
						sendNotification( HintEvents.RECEIVEINFO, {info:"角色名修改成功",color:0xffff00} );
						sendNotification( PlayerInfoComList.UPDATE_SELF_INFO );
						sendNotification( VipShowData.UPDATA_USERINFO );
						GameCommonData.canReName = 0;
					}else
					{
						sendNotification( HintEvents.RECEIVEINFO, {info:"该名字已经存在",color:0xffff00} );
						reNameView.txtInfo.mouseEnabled = true;
						reNameView.change_btn.mouseEnabled = true;
					}
					break;
				case 1:       //帮派改名
					if( state == 1 )
					{
						closeView( null );
						sendNotification( HintEvents.RECEIVEINFO, {info:"帮派名修改成功",color:0xffff00} );
						sendNotification( NewUnityCommonData.RE_UNITY_NAME_SUCCESS, name );
					}else
					{
						sendNotification( HintEvents.RECEIVEINFO, {info:"该名字已经存在",color:0xffff00} );
						reNameView.txtInfo.mouseEnabled = true;
						reNameView.change_btn.mouseEnabled = true;
					}
					break;
			}
			
		}
		
		private function closeBtn():void
		{
			GameCommonData.GameInstance.GameUI.removeChild( reNameBtn );
			reNameBtn = null;
		}
		
		private function onFoucsIn(event:FocusEvent):void
		{ 
			GameCommonData.isFocusIn = true; 
		}
		
		private function onFoucsOut(event:FocusEvent):void
		{
			GameCommonData.isFocusIn = false;
		}
	}
}