package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Mediator.Member.SingleApplyListMediator;
	import GameUI.Modules.UnityNew.Mediator.Member.SingleContributeMediator;
	import GameUI.Modules.UnityNew.Mediator.Member.SingleMemberListMediator;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnityMemberMediator extends Mediator
	{
		public static const NAME:String = "NewUnityMemberMediator";
		
		private var parentContainer:Sprite;
		private var main_mc:MovieClip;
		
		private var singleMemberListMediator:SingleMemberListMediator;
		private var singleContributeMediator:SingleContributeMediator;
		private var singleApplyListMediator:SingleApplyListMediator;
		
		public function NewUnityMemberMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CHANG_NEW_UNITY_PAGE,
							NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.CHANG_NEW_UNITY_PAGE:
					if ( !parentContainer )
					{
						parentContainer = notification.getBody() as Sprite;
					}
					if ( NewUnityCommonData.currentPage == 1 )
					{
						openMe();
					}
				break;
				case NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL:
					if ( notification.getBody() == 1 )
					{
						clearMe();
					}
				break;
			}
		}
		
		private function initView():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.MemberList ) as MovieClip;
			
			singleApplyListMediator = new SingleApplyListMediator();
			singleMemberListMediator = new SingleMemberListMediator();
			singleContributeMediator = new SingleContributeMediator();
			
			singleMemberListMediator.setViewComponent( this.main_mc );
			singleApplyListMediator.setViewComponent( this.main_mc );
			singleContributeMediator.setViewComponent( this.main_mc );
//			changPage();
		}
		
		private function openMe():void
		{
			if ( !main_mc )
			{
				initView();
			}
//			else
//			{
//				sendNotification( NewUnityCommonData.CLICK_MEMBER_PAGE_COME );
//			}
			
			facade.registerMediator( singleApplyListMediator );
			facade.registerMediator( singleMemberListMediator );
			facade.registerMediator( singleContributeMediator );
		
			changPage();
			parentContainer.addChildAt( main_mc,0 );
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "memPage_"+i ] as MovieClip ).buttonMode = true;
				( main_mc[ "memPage_"+i ] as MovieClip ).addEventListener( MouseEvent.CLICK,onClickPage );
			}
		}
		
		private function onClickPage( evt:MouseEvent ):void
		{
			var index:int = evt.target.name.split( "_" )[ 1 ];
			if ( NewUnityCommonData.currentMemPage == index ) return;
			sendNotification( NewUnityCommonData.CLEAR_MEMBER_PAGE_GO,NewUnityCommonData.currentMemPage );
			NewUnityCommonData.currentMemPage = index;
			changPage();
		}
		
		private function changPage():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "memPage_"+i ] as MovieClip ).gotoAndStop( 2 );
			}
			main_mc[ "memPage_" + NewUnityCommonData.currentMemPage ].gotoAndStop( 1 ); 
			main_mc.gotoAndStop( NewUnityCommonData.currentMemPage + 1 );
			
			sendNotification( NewUnityCommonData.CLICK_MEMBER_PAGE_COME );
		}
		
		private function clearMe():void
		{
			if ( main_mc && parentContainer.contains( main_mc ) )
			{
				for ( var i:uint=0; i<3; i++ )
				{
					( main_mc[ "memPage_"+i ] as MovieClip ).removeEventListener( MouseEvent.CLICK,onClickPage );
				}
				
				sendNotification( NewUnityCommonData.CLEAR_MEMBER_PAGE_GO,NewUnityCommonData.currentMemPage );		//清理当前面板
				parentContainer.removeChild( main_mc );
				
				facade.removeMediator( SingleApplyListMediator.NAME );
				facade.removeMediator( SingleMemberListMediator.NAME );
				facade.removeMediator( SingleContributeMediator.NAME );
			}
		}
		
	}
}