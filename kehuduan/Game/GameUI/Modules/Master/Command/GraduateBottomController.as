package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.YoungStudent;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.FriendSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	public class GraduateBottomController extends Sprite
	{
		private var main_mc:MovieClip;
		private var _student:YoungStudent;
		
		public function GraduateBottomController()
		{
			main_mc = new ( MasterData.masResPro.graduateBottomUI ) as MovieClip;
			addChild( main_mc );
//			( main_mc.iviteTeam_btn as SimpleButton ).visible = false;
//			( main_mc.joinTeam_btn as SimpleButton ).visible = false;
		}
		
		//开始侦听
		public function listen():void
		{
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		public function set student( value:YoungStudent ):void
		{
			this._student = value;
		}
		
		public function set fourBtnVisble( isSee:Boolean ):void
		{
			( main_mc.lookInfo_btn as SimpleButton ).mouseEnabled = isSee;
			( main_mc.privateTalk_btn as SimpleButton ).mouseEnabled = isSee;
			if ( !isSee )
			{
				MasterData.setGrayFilter( main_mc.lookInfo_btn );
				MasterData.setGrayFilter( main_mc.privateTalk_btn );
				MasterData.setGrayFilter( main_mc.iviteTeam_btn );
				MasterData.setGrayFilter( main_mc.joinTeam_btn );
				for ( var i:uint=0; i<4; i++ )
				{
					MasterData.setGrayFilter( main_mc[ "noUseTxt_" + i ] );
				}
				
				( main_mc.iviteTeam_btn as SimpleButton ).mouseEnabled = isSee;
				( main_mc.joinTeam_btn as SimpleButton ).mouseEnabled = isSee;
			}
			else
			{
				main_mc.lookInfo_btn.filters = null;
				main_mc.privateTalk_btn.filters = null;
				MasterData.addGlowFilter( main_mc.noUseTxt_2 );
				MasterData.addGlowFilter( main_mc.noUseTxt_3 );
				if ( this._student.outLineTime == 0 )
				{
					main_mc.iviteTeam_btn.filters = null;
					main_mc.joinTeam_btn.filters = null;
					MasterData.addGlowFilter( main_mc.noUseTxt_0 );
					MasterData.addGlowFilter( main_mc.noUseTxt_1 );
					( main_mc.iviteTeam_btn as SimpleButton ).mouseEnabled = isSee;
					( main_mc.joinTeam_btn as SimpleButton ).mouseEnabled = isSee;
				}
				else
				{
					MasterData.setGrayFilter( main_mc.iviteTeam_btn );
					MasterData.setGrayFilter( main_mc.joinTeam_btn );
					MasterData.setGrayFilter( main_mc.noUseTxt_0 );
					MasterData.setGrayFilter( main_mc.noUseTxt_1 );
					( main_mc.iviteTeam_btn as SimpleButton ).mouseEnabled = false;
					( main_mc.joinTeam_btn as SimpleButton ).mouseEnabled = false;
				}
			}
//			if ( isSee && this._student.outLineTime == 0  )
//			{
//				( main_mc.iviteTeam_btn as SimpleButton ).visible = true;
//				( main_mc.joinTeam_btn as SimpleButton ).visible = true;
//			}
//			else
//			{
//				( main_mc.iviteTeam_btn as SimpleButton ).visible = false;
//				( main_mc.joinTeam_btn as SimpleButton ).visible = false;
//			}
		}
		
		private function initUI():void
		{
			( main_mc.iviteTeam_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,inviteTeam );
			( main_mc.joinTeam_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,joinTeam );
			( main_mc.lookInfo_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onLookInfo );
			( main_mc.privateTalk_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onPrivateTalk );
			
			for ( var i:uint=0; i<4; i++ )
			{
				( main_mc[ "noUseTxt_" + i ] as TextField ).mouseEnabled = false; 
			}
		}
		
		//邀请入队
		private function inviteTeam( evt:MouseEvent ):void
		{
			if ( !_student ) return;
			if ( isMyself() ) return;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.INVITETEAM, { id:_student.id } );
		}
		
		//申请入队
		private function joinTeam( evt:MouseEvent ):void
		{
			if ( !_student ) return;
			if ( isMyself() ) return;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.APPLYTEAM,{ id:_student.id } );
		}
		
		//查看资料
		private function onLookInfo( evt:MouseEvent ):void
		{
			if ( !_student ) return;
			if ( isMyself() ) return;
			FriendSend.getInstance().getFriendInfo( _student.id, _student.name );
		}
		
		//设为私聊
		private function onPrivateTalk( evt:MouseEvent ):void
		{
			if ( !_student ) return;
			if ( isMyself() ) return;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( ChatEvents.QUICKCHAT,_student.name ); 
		}
		
		private function isMyself():Boolean
		{
			if ( _student && _student.id == GameCommonData.Player.Role.Id )
			{
				return true;
			}
			return false;
		}
		
		private function addStageHandler( evt:Event ):void
		{
			initUI();
			addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			
			( main_mc.iviteTeam_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,inviteTeam );
			( main_mc.joinTeam_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,joinTeam );
			( main_mc.lookInfo_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onLookInfo );
			( main_mc.privateTalk_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onPrivateTalk );
		}
		
		
		public function gc():void
		{
			this._student = null;
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
	}
}