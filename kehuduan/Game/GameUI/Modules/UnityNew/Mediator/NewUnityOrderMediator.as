package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityMemberVo;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnityOrderMediator extends Mediator
	{
		public static const NAME:String = "NewUnityOrderMediator";
		
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		private var currentMemberVo:UnityMemberVo;
		private var selectIndex:int;
		private var maxCheckBox:int;
		
		public function NewUnityOrderMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [ 
							NewUnityCommonData.OPEN_NEW_UNITY_ORDER_PANEL,
							NewUnityCommonData.CLOSE_NEW_UNITY_ORDER_PANEL 
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.OPEN_NEW_UNITY_ORDER_PANEL:
					currentMemberVo = notification.getBody() as UnityMemberVo;
					openMe();
				break;
				case NewUnityCommonData.CLOSE_NEW_UNITY_ORDER_PANEL:
					
				break;
			}
		}
		
		private function openMe():void
		{
			if ( !main_mc )
			{
				///////////////////////////////////////////////////////////这些代码是任命副帮主为帮主专用，暂时屏蔽，不能删
				if ( GameCommonData.Player.Role.unityJob == 100 )
				{
					main_mc = new ( NewUnityCommonData.newUnityResProvider.BossUnityOrderRes ) as MovieClip;
					maxCheckBox = 5;
				}
				else
				{
					main_mc = new ( NewUnityCommonData.newUnityResProvider.NewUnityOrderRes ) as MovieClip;
					maxCheckBox = 4;
				}
				////////////////////////////////////////////////////////////
				/**
				main_mc = new ( NewUnityCommonData.newUnityResProvider.NewUnityOrderRes ) as MovieClip;
				maxCheckBox = 4;*/
				
				main_mc.hint_txt.mouseEnabled = false;
				panelBase = new PanelBase( main_mc, main_mc.width+8, main_mc.height+12 );
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
				
				panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_ord_han_1" ] ); // 任命
				panelBase.addEventListener(Event.CLOSE, closeMe);
				
				for ( var i:uint=1; i<maxCheckBox; i++ )
				{
					main_mc[ "checkBox_"+i ].buttonMode = true;
					main_mc[ "checkBox_"+i ].addEventListener( MouseEvent.CLICK,clickCheckBox );
				}
				main_mc.commit_btn.addEventListener( MouseEvent.CLICK,onCommit );
				main_mc.cancel_btn.addEventListener( MouseEvent.CLICK,onCancel );
			}
			                                                       //任命                                                                                                                   //为：
			main_mc.hint_txt.htmlText = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_uni_med_ord_han_1" ]+" <font color='#00ff00'>" + currentMemberVo.name + "</font> "+GameCommonData.wordDic[ "mod_unityN_med_newuo_ope_1" ]+"</font>";
			selectIndex = 0;
			initCheckBox();
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
		}
		
		private function initCheckBox():void
		{
			for ( var i:uint=1; i<maxCheckBox; i++ )
			{
				if ( i==selectIndex )
				{
					main_mc[ "checkBox_"+i ].gotoAndStop(2);
				}
				else
				{
					main_mc[ "checkBox_"+i ].gotoAndStop(1);
				}
			}
		}
		
		private function clickCheckBox( evt:MouseEvent ):void
		{
			if ( GameCommonData.Player.Role.unityJob < 90 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );		//你的权限不足
				return;
			}
			
			if ( GameCommonData.Player.Role.unityJob <= currentMemberVo.unityJob )			//不能任命跟自己同级的人
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );		//你的权限不足
				return;
			}
			
			var targetIndex:int = evt.target.name.split( "_" )[1];
			if ( targetIndex == 1 ) 
			{
				if ( GameCommonData.Player.Role.unityJob != 100 )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );
					return;
				}
			}
			else if ( targetIndex == 4 )
			{
				if ( currentMemberVo.unityJob != 90 )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newuo_cli_1" ] );   //只能任命副帮主为帮主
					return;
				}
			}
			selectIndex = targetIndex;

			initCheckBox();
		}
		
		private function onCommit( evt:MouseEvent ):void
		{
			if ( selectIndex == 0 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newuo_cli_2" ] );     //请选择职位
				return;
			}
			else if ( selectIndex == 4 && GameCommonData.Player.Role.unityJob == 100 )
			{
				var sendBossObj:Object = new Object();
				sendBossObj.type = 1107;														//协议号
				sendBossObj.data = [ 0, 0, 338, 100, currentMemberVo.id ];
				UnityActionSend.SendSynAction( sendBossObj );										//发送任命请求
				closeMe( null );
				return;
			}
			
			var newJob:int;
			switch ( selectIndex )
			{
				case 1:								//副帮主
				
					if ( viceBossNum>=2 )
					{
						GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newuo_cli_3" ] );     //副帮主名额已满
						return;
					}
					if ( currentMemberVo.unityJob == 90 )
					{
						closeMe( null );
						return;
					}
					newJob = 90;
				break;
				case 2:							//精英
					if ( currentMemberVo.unityJob == 85 )
					{
						closeMe( null );
						return;
					}
					newJob = 85;
				break;
				case 3:							//帮众
					if ( currentMemberVo.unityJob == 10 )
					{
						closeMe( null );
						return;
					}
					newJob = 10;
				break;
			}

			var sendObj:Object = new Object();
			sendObj.type = 1107;														//协议号
			sendObj.data = [ 0, 0, 215, newJob, currentMemberVo.id ];
			UnityActionSend.SendSynAction( sendObj );										//发送任命请求
			closeMe( null );
		}
		
		private function onCancel( evt:MouseEvent ):void
		{
			closeMe( null );
		}
		
		private function closeMe( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				for ( var i:uint=1; i<maxCheckBox; i++ )
				{
					main_mc[ "checkBox_"+i ].removeEventListener( MouseEvent.CLICK,clickCheckBox );
				}
				
				main_mc.commit_btn.removeEventListener( MouseEvent.CLICK,onCommit );
				main_mc.cancel_btn.removeEventListener( MouseEvent.CLICK,onCancel );
				
				panelBase.removeEventListener(Event.CLOSE, closeMe);
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				main_mc = null;
				panelBase = null;
			}
			facade.removeMediator( NAME );
		}
		
		public function get viceBossNum():int
		{
			var num:int;
			for ( var i:uint=0; i<NewUnityCommonData.allUnityMemberArr.length; i++ )
			{
				if ( NewUnityCommonData.allUnityMemberArr[i].unityJob == 90 )
				{
					num++;
				}
			}
			return num;
		}
		
	}
}