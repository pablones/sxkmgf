package GameUI.Modules.Master.Command
{
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.MentorRelation;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.countDown.CountDownEvent;
	import GameUI.View.Components.countDown.CountDownText;
	
	import Net.ActionSend.FriendSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	//底部的5个按钮提取
	public class BottomButtonController extends Sprite
	{
		public var refreshFun:Function;
		
		private var mentor:MentorRelation;
		private var main_mc:MovieClip;
		private var type:int;									//1为师傅列表  2为徒弟申请列表
		private var refreshDownTxt:CountDownText;
		
		public function BottomButtonController( _type:int )
		{
			type = _type;
			main_mc = new ( MasterData.masResPro.fiveButtonClass ) as MovieClip;
			addChild( main_mc );
		}
		
		//开始侦听
		public function listen():void
		{
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		public function set _mentor( value:MentorRelation ):void
		{
			mentor = value;
		}
		
		public function set fourBtnVisble( isSee:Boolean ):void
		{
			( main_mc.notSure_btn as SimpleButton ).mouseEnabled = isSee;
			( main_mc.lookInfo_btn as SimpleButton ).mouseEnabled = isSee;
			( main_mc.addFriend_btn as SimpleButton ).mouseEnabled = isSee;
			( main_mc.privateTalk_btn as SimpleButton ).mouseEnabled = isSee;
			if ( !isSee )
			{
				MasterData.setGrayFilter( main_mc.notSure_btn );
				MasterData.setGrayFilter( main_mc.lookInfo_btn );
				MasterData.setGrayFilter( main_mc.addFriend_btn );
				MasterData.setGrayFilter( main_mc.privateTalk_btn );
				
				for ( var i:uint=0; i<3; i++ )
				{
					MasterData.setGrayFilter( main_mc[ "noUseTxt_"+i ] );
				}
				MasterData.setGrayFilter( main_mc.notSure_txt );
//				main_mc.notSure_txt.textColor = 0x999999;
			}
			else
			{
				( main_mc.notSure_btn as SimpleButton ).filters = null;
				( main_mc.lookInfo_btn as SimpleButton ).filters = null;
				( main_mc.addFriend_btn as SimpleButton ).filters = null;
				( main_mc.privateTalk_btn as SimpleButton ).filters = null;
				
				for ( var j:uint=0; j<3; j++ )
				{
					MasterData.addGlowFilter( main_mc[ "noUseTxt_"+j ] );
				}
				MasterData.addGlowFilter( main_mc.notSure_txt );
//				( main_mc.notSure_txt as TextField ).filters = null;
//				MasterData.delGrayFilter( main_mc.notSure_txt );
//				MasterData.delGrayFilter( main_mc[ "noUseTxt_"+1 ] );
//				main_mc.notSure_txt.textColor = 0xfffe65;
			}
		}
		
		public function set oneBtnVisble( isSee:Boolean ):void
		{
			( main_mc.refresh_btn as SimpleButton ).visible = isSee;
			( main_mc.refresh_txt as TextField ).visible = isSee;
		}
		
		private function initUI():void
		{
			( main_mc.refresh_txt as TextField ).visible = true;
			( main_mc.refresh_txt as TextField ).mouseEnabled = false;
			( main_mc.notSure_txt as TextField ).mouseEnabled = false;
			
			for ( var i:uint=0; i<3; i++ )
			{
				main_mc[ "noUseTxt_"+i ].mouseEnabled = false;
			}
			
			( main_mc.refresh_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onRefresh );
			( main_mc.notSure_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onNotSure );
			( main_mc.lookInfo_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onLookInfo );
			( main_mc.addFriend_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onAddFriend );
			( main_mc.privateTalk_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,onPrivateTalk );
			
			if ( type == 1 )
			{
				( main_mc.notSure_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_com_bot_exe_1" ];   //拜 师
			}
			else if ( type == 2 )
			{
				( main_mc.notSure_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_com_bot_exe_2" ];   //删 除
			}
		}
		
		//第二个按钮，自适应事件
		private function onNotSure( evt:MouseEvent ):void
		{
			if ( isMyself() ) return;
			if ( !mentor ) return;
			if ( type == 1 )
			{
				if ( GameCommonData.Player.Role.Level < 10 )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bot_exe_3" ] );     //人物等级不能小于10级
					return;
				}
				if ( GameCommonData.Player.Role.Level + 10 > mentor.roleLevel )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bot_exe_4" ] );     //只能拜等级超过你10级的玩家为师
					return;
				}
				RequestTutor.requestData( 13,mentor.id );
			}
			else if ( type == 2 )
			{
				//删除徒弟申请列表
				RequestTutor.requestData( 25,mentor.id );
			}
		}
		
		private function onLookInfo( evt:MouseEvent ):void
		{
			if ( !mentor ) return;
			if ( isMyself() ) return;
			FriendSend.getInstance().getFriendInfo( mentor.id, mentor.name );
		}
		
		private function onAddFriend( evt:MouseEvent ):void
		{
			if ( !mentor ) return;
			if ( isMyself() ) return;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( FriendCommandList.ADD_TO_FRIEND,{ id:mentor.id,name:mentor.name } );
		}
		
		private function onPrivateTalk( evt:MouseEvent ):void
		{
			if ( !mentor ) return;
			if ( isMyself() ) return;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( ChatEvents.QUICKCHAT,mentor.name ); 
		}
		
		private function isMyself():Boolean
		{
			if ( mentor && mentor.id == GameCommonData.Player.Role.Id )
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
			
			( main_mc.refresh_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onRefresh );
			( main_mc.notSure_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onNotSure );
			( main_mc.lookInfo_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onLookInfo );
			( main_mc.addFriend_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onAddFriend );
			( main_mc.privateTalk_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,onPrivateTalk );
		}
		
		
		public function gc():void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			if ( refreshDownTxt )
			{
				refresTimeOver( null );
			}
		}
		
		private function onRefresh( evt:MouseEvent ):void
		{
			if ( refreshFun != null ) refreshFun();
			oneBtnVisble = false;
			refreshDownTxt = new CountDownText( 5 );
			refreshDownTxt.x = 18;
			refreshDownTxt.y = 2;
			main_mc.addChild( refreshDownTxt );
			refreshDownTxt.addEventListener( CountDownEvent.TIME_OVER,refresTimeOver );
			refreshDownTxt.start();
		}
		
		private function refresTimeOver( evt:CountDownEvent ):void
		{
			oneBtnVisble = true;
			refreshDownTxt.removeEventListener( CountDownEvent.TIME_OVER,refresTimeOver );
			if ( main_mc.contains( refreshDownTxt ) )
			{
				main_mc.removeChild( refreshDownTxt );
				refreshDownTxt.dispose();
				refreshDownTxt = null;
			}
		}
		
	}
}