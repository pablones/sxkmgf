package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityBaseInfo;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewLookUnityMediator extends Mediator
	{
		public static const NAME:String = "NewLookUnityMediator";
		public static var openState:Boolean = false;
		public static var isRequestLookInfo:Boolean = false;
		
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		
		private var unityId:int;				//要查看帮派的id
		private var recBaseInfo:UnityBaseInfo;
		
		public function NewLookUnityMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.SHOW_NEW_UNITY_LOOK_INFO,
							NewUnityCommonData.UPDATE_NEW_UNITY_LOOK_INFO
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.SHOW_NEW_UNITY_LOOK_INFO:
					unityId = notification.getBody() as int;
//					if ( unityId == GameCommonData.Player.Role.unityId )
//					{
//						return;
//					}
					showMe();
				break;
				case NewUnityCommonData.UPDATE_NEW_UNITY_LOOK_INFO:
					recBaseInfo = notification.getBody() as UnityBaseInfo;
					if ( recBaseInfo.id == unityId && openState )
					{
						isRequestLookInfo = false;
						updateMe();
					}
				break;
			}
		}
		
		private function showMe():void
		{
			if ( !panelBase )
			{
				main_mc = new ( NewUnityCommonData.newUnityResProvider.LookUnityInfoRes ) as MovieClip;
				panelBase = new PanelBase( main_mc,main_mc.width+8,main_mc.height+12 );
//				panelBase.x = UIConstData.DefaultPos2.x + 110;
//				panelBase.y = UIConstData.DefaultPos2.y;
				
				if( GameCommonData.fullScreen == 2 )
				{
					panelBase.x = UIConstData.DefaultPos2.x + 110 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					panelBase.y = UIConstData.DefaultPos2.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}else{
					panelBase.x = UIConstData.DefaultPos2.x + 110;
					panelBase.y = UIConstData.DefaultPos2.y;
				}
				
				panelBase.addEventListener( Event.CLOSE,closeMe );
				panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_unityN_med_newl_sho_1" ] );      //帮派信息
							
				main_mc.beFriend_txt.mouseEnabled = false;
				main_mc.beFriend_txt.visible = false;
				main_mc.beFriend_btn.visible = false;
				( main_mc.beFriend_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,toBeFriend );
				for ( var i:uint=0; i<7; i++ )
				{
					main_mc[ "txt_"+i ].mouseEnabled = false;
					main_mc[ "txt_"+i ].text = "";
				}
			}
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			openState = true;
			requestDate();
		}
		
		//请求数据
		private function requestDate():void
		{
			isRequestLookInfo = true;
			var obj:Object = new Object();
			obj.type = 1107;														
			obj.data = [ 0 , 0 , 208 , 0 , unityId ];
			UnityActionSend.SendSynAction( obj );
		}
		
		private function updateMe():void
		{
			main_mc[ "txt_"+0 ].text = recBaseInfo.name;
			main_mc[ "txt_"+1 ].text = recBaseInfo.boss;
			main_mc[ "txt_"+3 ].text = recBaseInfo.level;
			main_mc[ "txt_"+4 ].text = recBaseInfo.currentPeople + "/" + recBaseInfo.maxPeople;
			main_mc[ "txt_"+5 ].text = recBaseInfo.jianShe;
			main_mc[ "txt_"+6 ].text = recBaseInfo.notice;
			
			var createTimeStr:String = recBaseInfo.historyBangGong.toString();
//			main_mc[ "txt_"+2 ].text = createTimeStr.slice( 0,4 ) + "年" + createTimeStr.slice( 4,6 ) + "月" + createTimeStr.slice( 6,8 ) + "日";
			main_mc[ "txt_"+2 ].text = createTimeStr.slice( 0,4 ) + GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_2" ] + analyMonth( createTimeStr.slice( 4,6 ) ) + GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_3" ] + createTimeStr.slice( 6,8 ) + GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_4" ];   //年    月     日
			
			main_mc.beFriend_txt.visible = true;
			main_mc.beFriend_btn.visible = true;
			
			main_mc.money_mc.txtMoney.text = UIUtils.getMoneyStandInfo( recBaseInfo.money, ["\\ce","\\cs","\\cc"] );
			ShowMoney.ShowIcon( main_mc.money_mc, main_mc.money_mc.txtMoney, true );
		}
		
		private function toBeFriend( evt:MouseEvent ):void
		{
			sendNotification( FriendCommandList.ADD_TO_FRIEND,{ id:-1, name:recBaseInfo.boss } );
		}
		
		private function closeMe( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				( main_mc.beFriend_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,toBeFriend );
				panelBase.removeEventListener( Event.CLOSE,closeMe );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				main_mc = null;
				panelBase = null;
				openState = false;
			}
		}
		
		private function analyMonth( str:String ):String
		{
			if ( str.slice( 0,1 ) == "0" )
			{
				return str.slice( 1,2 );
			}
			else
			{
				return str;
			}
		}
		
	}
}