package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.NewUnityResouce;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.AutoPanelBase;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnityMainMediator extends Mediator
	{
		public static const NAME:String = "NewUnityMainMediator";
		
		private var newUintyRes:NewUnityResouce;
		private var main_mc:MovieClip;
		private var dataProxy:DataProxy;
		
		private var panelBase:AutoPanelBase;
		private var parentContainer:Sprite;
		private var quickSelectMediator:QuickSelectMediator;
		private var newLookUnityMediator:NewLookUnityMediator;
		
		public function NewUnityMainMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CLICK_NEW_UNITY_BTN,
							NewUnityCommonData.CLOSE_NEW_UNITY_MAIN_PANEL
						]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.CLICK_NEW_UNITY_BTN:
					if ( !NewUnityCommonData.unityResIsDone )
					{
						if ( !newUintyRes )
						{
							newUintyRes = new NewUnityResouce();
							newUintyRes.resDoneHandler = resDoneHandler;
						}
					}
					else
					{
						checkIsJoin();
					}
				break;
				case NewUnityCommonData.CLOSE_NEW_UNITY_MAIN_PANEL:
					if ( dataProxy.UnityIsOpen )
					{
						panelClosed(null);
					}
				break;
			}
		}
		
		private function resDoneHandler():void
		{
			checkIsJoin();
		}
		
		private function checkIsJoin():void
		{
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			if ( GameCommonData.Player.Role.unityId != 0 )
			{
				hasUnity();
			}
			else
			{
				hasNotUnity();
			}
			if ( !newLookUnityMediator )
			{
				newLookUnityMediator = new NewLookUnityMediator();
				facade.registerMediator( newLookUnityMediator );
			}
		}
		
		//有帮
		private function hasUnity():void
		{
			if ( !main_mc )
			{
				main_mc = new ( NewUnityCommonData.newUnityResProvider.UnityMainRes ) as MovieClip;
				
				parentContainer = new Sprite();
				main_mc.x = 14;
				main_mc.y = 0;
				parentContainer.addChild( main_mc );
				
				facade.registerMediator( new NewUnityInfoMediator() );
				facade.registerMediator( new NewUnityMemberMediator() );
				facade.registerMediator( new NewUnityListMediator() );
				facade.registerMediator( new NewUnitySkillMediator() );
				facade.registerMediator( new NewUnityPlaceMediator() );
				facade.registerMediator( new NewUnityContributeMediator() );
//				facade.registerMediator( new NewLookUnityMediator() );
				
				panelBase = new AutoPanelBase( parentContainer,609,444 );
//				panelBase.x = UIConstData.DefaultPos1.x;
//				panelBase.y = UIConstData.DefaultPos1.y;
				
				if( GameCommonData.fullScreen == 2 ) 
				{
					panelBase.x = UIConstData.DefaultPos1.x + Math.floor(GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					panelBase.y = UIConstData.DefaultPos1.y + Math.floor(GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}else{
					panelBase.x = UIConstData.DefaultPos1.x;
					panelBase.y = UIConstData.DefaultPos1.y;
				}
				
				panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_unityN_med_joi_ini_1" ] );    //帮 派
			}
			
			if ( dataProxy.UnityIsOpen )
			{
				panelClosed( null );
			}
			else
			{
				openUnity();
			}
		}
		
		private function openUnity():void
		{
			panelBase.addEventListener( Event.CLOSE,panelClosed );
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			dataProxy.UnityIsOpen = true;
			
			changPage();
			for ( var i:uint=0; i<5; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).buttonMode = true;
				main_mc[ "mcPage_"+i ].addEventListener( MouseEvent.CLICK,onClickPage );
			}
		}
		
		//点击页签
		private function onClickPage( evt:MouseEvent ):void
		{
			var index:int = evt.target.name.split( "_" )[ 1 ];
			if ( NewUnityCommonData.currentPage == index ) return;
			sendNotification( NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL,NewUnityCommonData.currentPage );
			NewUnityCommonData.currentPage = index;
			changPage();
		}
		
		private function changPage():void
		{
			for ( var i:uint=0; i<5; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).gotoAndStop( 2 );
			}
			main_mc[ "mcPage_" + NewUnityCommonData.currentPage ].gotoAndStop( 1 ); 
			sendNotification( NewUnityCommonData.CHANG_NEW_UNITY_PAGE,this.parentContainer );
		}
		
		//没帮
		private function hasNotUnity():void
		{
			if ( !facade.hasMediator( JoinUnityMediator.NAME ) )
			{
				facade.registerMediator( new JoinUnityMediator() );
			}
			
			if ( dataProxy.UnitInfoIsOpen )
			{
//				dataProxy.UnitInfoIsOpen = false;
				sendNotification( NewUnityCommonData.CLOSE_JOIN_UNITY_NEW );
			}
			else
			{
//				dataProxy.UnitInfoIsOpen = true;
				sendNotification( NewUnityCommonData.SHOW_JOIN_UINTY_NEW );
			}
		}
		
		private function panelClosed( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				sendNotification( NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL,NewUnityCommonData.currentPage );
				for ( var i:uint=0; i<5; i++ )
				{
					main_mc[ "mcPage_"+i ].removeEventListener( MouseEvent.CLICK,onClickPage );
				}
				
				panelBase.removeEventListener( Event.CLOSE,panelClosed );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				dataProxy.UnityIsOpen = false;
			}
			if ( facade.hasMediator( QuickSelectMediator.NAME ) )
			{
				facade.removeMediator( QuickSelectMediator.NAME ) ;
			}
			if ( newLookUnityMediator )
			{
				facade.removeMediator( NewLookUnityMediator.NAME );
				newLookUnityMediator = null;
			}
		}
	
		
	}
}