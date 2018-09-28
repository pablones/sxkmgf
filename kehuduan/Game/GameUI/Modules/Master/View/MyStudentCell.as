package GameUI.Modules.Master.View
{
	import Controller.TargetController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Master.Proxy.YoungStudent;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.countDown.CountDownEvent;
	import GameUI.View.Components.countDown.CountDownText;
	import GameUI.View.ResourcesFactory;
	
	import Net.ActionSend.FriendSend;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	public class MyStudentCell extends Sprite
	{
		public var talkStudentFun:Function;				//给徒弟发送消息的事件
		
		public var youngStudent:YoungStudent;
		private var main_mc:MovieClip;
		private var qMask:Bitmap;
		private var faceBmp:Bitmap;
		private var refreshDownTxt:CountDownText;
		
		public function MyStudentCell( _youngStudent:YoungStudent )
		{
			youngStudent = _youngStudent;
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function initUI():void
		{
			main_mc = new ( MasterData.masResPro.myStudentClass ) as MovieClip;
			addChild( main_mc );
			
			var bmpData:BitmapData = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByBitmapData("qMark");
			qMask = new Bitmap( bmpData );
			qMask.x = 37;
			qMask.y = 22;
			main_mc.addChild( qMask );
			
			( main_mc.name_txt as TextField ).text = youngStudent.name;
			( main_mc.line_txt as TextField ).htmlText = youngStudent.lineStr;
			( main_mc.level_txt as TextField ).text = youngStudent.roleLevel+GameCommonData.wordDic[ "often_used_level" ];      //级
			
			if ( youngStudent.mainJob == 0 )
			{
				( main_mc.mainJob_txt as TextField ).text = GameCommonData.wordDic[ "often_used_none" ];         //无
			}
			else
			{
				( main_mc.mainJob_txt as TextField ).text = GameCommonData.RolesListDic[ youngStudent.mainJob ] + " "+youngStudent.mainJobLevel + GameCommonData.wordDic[ "often_used_level" ];       //级
			}
			if ( youngStudent.viceJob == 0 )
			{
				( main_mc.viceJob_txt as TextField ).text = GameCommonData.wordDic[ "often_used_none" ];         //无
			}
			else
			{
				( main_mc.viceJob_txt as TextField ).text = GameCommonData.RolesListDic[ youngStudent.viceJob ] + " "+youngStudent.viceJobLevel + GameCommonData.wordDic[ "often_used_level" ];	        //级
			}
			
			( main_mc.lookInfo_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,lookInfo );
			( main_mc.sendMsg_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,sendMsg );
			( main_mc.iviteTeam_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,iviteTeam );
			( main_mc.callStudent_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,callStudent );
			( main_mc.gainAward_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,gainAward );
			
			( main_mc.team_txt as TextField ).mouseEnabled = false;
			
			if ( youngStudent.hasTeam == 0 )
			{
				( main_mc.team_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_view_mys_ini_1" ];          //邀请组队
			}
			else 
			{
				( main_mc.team_txt as TextField ).text = GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ];          //申请入队
			}
			
			main_mc.callStudent_txt.mouseEnabled = false;
			main_mc.gainAward_txt.mouseEnabled = false;
			
			if ( youngStudent.roleLevel<15  )
			{
				setGainBtnVis( false );
			}
			else
			{
				var realLevel:int = youngStudent.roleLevel;
				if ( youngStudent.roleLevel > 45 )
				{
					realLevel = 45;
				}
				var nIndex:int = Math.floor( ( realLevel - 15 ) / 5 );
				//越界
				if ( nIndex >= 27 )
				{
					setGainBtnVis( false );
				}
				if ( ( youngStudent.impart & (1<<nIndex) ) == 0 )
				{
					setGainBtnVis( true );
				}
				else
				{
					setGainBtnVis( false );
				}
			}
		
			loadSource();
		}
		
		public function loadSource():void
		{
			try
			{
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Face/" + youngStudent.face + ".png",onLoabdComplete);	
			}
			catch(e:Error)
			{
			}
		}
		
		private function onLoabdComplete():void
		{
			faceBmp = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Face/" + youngStudent.face + ".png");
			if ( faceBmp )
			{
				faceBmp.x = 27;
				faceBmp.y = 14;
				main_mc.addChild( faceBmp );
				if ( qMask && main_mc.contains( qMask ) )
				{
					main_mc.removeChild( qMask );
					qMask = null;
				}
 			}
		}
		
		private function lookInfo( evt:MouseEvent ):void
		{
			FriendSend.getInstance().getFriendInfo( youngStudent.id, youngStudent.name );
		}
		
		private function sendMsg( evt:MouseEvent ):void
		{
			if ( talkStudentFun != null )
			{
				talkStudentFun( youngStudent.name );
			}
		}
		
		private function iviteTeam( evt:MouseEvent ):void
		{
			if ( youngStudent.hasTeam == 0 )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.INVITETEAM, { id:youngStudent.id } );
			}
			else
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( EventList.APPLYTEAM,{ id:youngStudent.id } );
			}
		}
		
		private function callStudent( evt:MouseEvent ):void
		{
			if(TargetController.IsPKTeam())
			{
				UIFacade.UIFacadeInstance.showPrompt("该场景不能召唤徒弟",0xffff00);  
				return;
			} 
			if ( GameConfigData.GameSocketName == GameConfigData.specialLineName )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( GameCommonData.wordDic[ "mod_mas_view_mys_cal_4" ] );                //帮派里不能召唤徒弟
				return;
			}
//			trace ( "召唤徒弟" );
			if ( GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1026" )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( GameCommonData.wordDic[ "mod_mas_view_mys_cal_1" ] );                //地府中不能召唤徒弟
				return;
			}
			else if ( GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1027" )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( GameCommonData.wordDic[ "mod_mas_view_mys_cal_2" ] );                //监狱中不能召唤徒弟
				return;
			}
			if ( this.youngStudent.line == 0 )
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( GameCommonData.wordDic[ "mod_mas_view_mys_cal_3" ] );                    //你的徒弟不在线
				return;
			}
			RequestTutor.requestData( 27,youngStudent.id );
			isSeeCallBtn( false );
			
			refreshDownTxt = new CountDownText( 10 ); 
			refreshDownTxt.x = 287;
			refreshDownTxt.y = 35;
			main_mc.addChild( refreshDownTxt );
			refreshDownTxt.addEventListener( CountDownEvent.TIME_OVER,refresTimeOver );
			refreshDownTxt.start();
		}
		
		private function gainAward( evt:MouseEvent ):void
		{
//			trace ( "领取奖励" );
			RequestTutor.requestData( 29,youngStudent.id );
//			UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( "此功能暂未开放" );
		}
		
		private function refresTimeOver( evt:CountDownEvent ):void
		{
			isSeeCallBtn( true );
			if ( refreshDownTxt && main_mc.contains( refreshDownTxt ) )
			{
				refreshDownTxt.removeEventListener( CountDownEvent.TIME_OVER,refresTimeOver );
				main_mc.removeChild( refreshDownTxt );
				refreshDownTxt.dispose();
				refreshDownTxt = null;
			}
		}
		
		private function addStageHandler( evt:Event ):void
		{
			initUI();
			addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			refresTimeOver( null );
			
			( main_mc.lookInfo_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,lookInfo );
			( main_mc.sendMsg_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,sendMsg );
			( main_mc.iviteTeam_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,iviteTeam );
			( main_mc.callStudent_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,callStudent );
			( main_mc.gainAward_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,gainAward );
		}
		
		public function gc():void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			var des:*;
			while ( main_mc && main_mc.numChildren > 0 )
			{
				des = main_mc.removeChildAt( 0 );
				des = null;
			}
		}
		
		private function isSeeCallBtn( isSee:Boolean ):void
		{
			if ( main_mc )
			{
				main_mc.callStudent_btn.visible = isSee;
				main_mc.callStudent_txt.visible = isSee;
			}
		}
		
		private function setGainBtnVis( isSee:Boolean ):void
		{
			if ( main_mc )
			{
				main_mc.gainAward_btn.visible = isSee;
				main_mc.gainAward_txt.visible = isSee;
			}
		}
		
	}
}