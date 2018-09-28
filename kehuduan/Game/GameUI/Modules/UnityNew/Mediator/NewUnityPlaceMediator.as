package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Modules.UnityNew.View.DefendAttackPanel;
	import GameUI.UIUtils;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnityPlaceMediator extends Mediator
	{
		public static const NAME:String = "NewUnityPlaceMediator";
		
		private var parentContainer:Sprite;
		private var main_mc:MovieClip;
		private var openState:Boolean = false;
		private var getOutTimer:Timer = new Timer(300, 1);	//物品取出计时器
		private var currentIndex:int;
		private var isCanPromt:Boolean;				//分堂是否能升级
		private var isCanCallBoss:Boolean;
		private var defendAttackPanel:DefendAttackPanel;
		
		public function NewUnityPlaceMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CHANG_NEW_UNITY_PAGE,
							NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL,
							NewUnityCommonData.UPDATE_SYN_PLACE_INFO
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
					if ( NewUnityCommonData.currentPage == 4 )
					{
						openMe();
					}
				break;
				case NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL:
					if ( notification.getBody() == 4 )
					{
						clearMe();
					}
				break;
				case NewUnityCommonData.UPDATE_SYN_PLACE_INFO:
					if ( openState )
					{
						updateMe();
					}
				break;
			}
		}
		
		private function initView():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.UnityFenTangRes ) as MovieClip;
			
			for ( var i:uint=0; i<4; i++ )
			{
				main_mc[ "effectTxt_"+i ].mouseEnabled = false;
				main_mc[ "levelTxt_"+i ].mouseEnabled = false;
				main_mc[ "levelTxt_"+i ].text = "";
				
//				if ( i != 0 )
//				{
					main_mc[ "effBtn_"+i ].visible = false;
					main_mc[ "effectBtnTxt_"+i ].visible = false;
					main_mc[ "effectBtnTxt_"+i ].mouseEnabled = false;
//				}
			}
		}
		
		private function openMe():void
		{
			openState = true;
			if ( !main_mc )
			{
				initView();
			}
			
			initBtns();
			checkRequest();
			
			parentContainer.addChildAt( main_mc,0 );
		}
		
		private function checkRequest():void
		{
			RequestUnity.send( 306,0 );
		}
		
		private function initTxts():void
		{
			for ( var i:uint=0; i<4; i++ )
			{
				if ( NewUnityCommonData.unityPlaceLevelArr[i]>0 )
				{
					main_mc[ "effectTxt_"+i ].visible = false;
					main_mc[ "effBtn_"+i ].visible = true;
					main_mc[ "effectBtnTxt_"+i ].visible = true;
				}
				else
				{
					main_mc[ "effectTxt_"+i ].visible = true;
					main_mc[ "effBtn_"+i ].visible = false;
					main_mc[ "effectBtnTxt_"+i ].visible = false;
				}
			}
//			if ( NewUnityCommonData.unityPlaceLevelArr[0]>0 )
//			{
//				main_mc[ "effectTxt_0" ].text = GameCommonData.wordDic[ "mod_unityN_med_newui_init_1" ];    //已开启
//			}
//			else
//			{
				main_mc[ "effectTxt_0" ].text = GameCommonData.wordDic[ "mod_unityN_med_newui_init_2" ];    //效果1级开启
//			}
		}
		
		private function updateMe():void
		{
			for ( var i:uint=0; i<4; i++ )
			{
				main_mc[ "levelTxt_"+i ].text = NewUnityCommonData.unityPlaceLevelArr[i].toString();
				if ( NewUnityCommonData.unityPlaceLevelArr[i] >= 10 )
				{
					( main_mc[ "levelUpBtn_"+i ] as SimpleButton ).visible = false;
				}
			}
			initTxts();
		}
		
		private function initBtns():void
		{
			for( var i:uint=0; i<4; i++ )
			{
				( main_mc[ "effBtn_"+i ] as SimpleButton ).addEventListener( MouseEvent.CLICK,makeEffect );
			}
			for ( var j:uint=0; j<4; j++ )
			{
				( main_mc[ "levelUpBtn_"+j ] as SimpleButton ).addEventListener( MouseEvent.CLICK,levelUpHandler );
			}
		}
		
		//升级
		private function levelUpHandler( evt:MouseEvent ):void
		{
			if ( !NewUnityCommonData.myUnityInfo.isStop )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_lev_1" ] );    //帮派暂停维护中，不能升级
				return;
			}
			var index:int = evt.target.name.split( "_" )[1];
			currentIndex = index;
			isCanPromt = true;
//			trace ( "分堂升级："+index ); 
			var needJianSheArr:Array = [ 1000,2000,4000,8000,16000,25000,35000,50000,70000,100000 ];
			var needMoneyArr:Array = [ 100,200,400,800,1600,2400,3200,4000,4800,5600 ];
			
			var needJianShe:int = needJianSheArr[ NewUnityCommonData.unityPlaceLevelArr[index] ];
			var needMoney:int = 10000*needMoneyArr[ NewUnityCommonData.unityPlaceLevelArr[index] ];
			var jianSheColor:String;
			var moneyColor:String;
			
			if ( NewUnityCommonData.myUnityInfo.jianShe < needJianShe )
			{
				jianSheColor = '#ff0000';
				isCanPromt = false;
			}
			else
			{
				jianSheColor = '#00ff00';
			}
			
			if ( NewUnityCommonData.myUnityInfo.money < needMoney )
			{
				moneyColor = '#ff0000';
				isCanPromt = false;
			}
			else
			{
				moneyColor = '#00ff00';
			}
			
			
			var info:String = "<font color = '#ffff00' size = '13'>"+GameCommonData.wordDic[ "mod_unityN_med_newup_lev_1" ]+NewUnityCommonData.aPlaceName[ currentIndex ]+GameCommonData.wordDic[ "mod_unityN_med_newup_lev_1" ]+"</font>\n<font color = '#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_7" ]+"</font><font color='" + jianSheColor +   //升级      ，需要消耗    建 设 度：
										"'>"+needJianShe.toString() + "</font>\n<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_8" ]+"</font><font color='" + moneyColor + "'>"+ UIUtils.getMoneyStandInfo( needMoney, ["\\ce","\\cs","\\cc"] ) + "</font>";//帮派资金：
			sendNotification(EventList.SHOWALERT, { comfrim:sureLevelUp, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],autoSize:1,height:77,leading:6 } );   //提 示
		}
		
		private function sureLevelUp():void
		{
			if ( GameCommonData.Player.Role.unityJob<90 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );    //你的权限不足
				return;
			}
			if ( !isCanPromt )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_sur_1" ] );    //升级条件不满足
				return;
			}
			RequestUnity.send( 310,currentIndex+1 );
		}
		
		private function cancelClose():void{}
		
		private function makeEffect( evt:MouseEvent ):void
		{
			if ( !NewUnityCommonData.myUnityInfo.isStop )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newup_mak_1" ] );    //帮派暂停维护中，不能使用效果
				return;
			}
			var btnIndex:int = int( evt.target.name.split( "_" )[1] );
			if ( btnIndex == 1 )
			{
				if ( GameConfigData.GameSocketName != GameConfigData.specialLineName )
				{
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_unityN_med_newup_mak_2" ], color:0xffff00});//请先回到帮派
				}
				else
				{
					if ( NewUnityCommonData.myUnityInfo.unityJob < 90 )
					{
						GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );    //你的权限不足
						return;
					}
//					var needMoney:int = 800000 + NewUnityCommonData.unityPlaceLevelArr[1] * 200000;
//					var moneyColor:String;
//					
//					if ( NewUnityCommonData.myUnityInfo.money < needMoney )
//					{
//						moneyColor = '#ff0000';
//						isCanCallBoss = false;
//					}
//					else
//					{
//						moneyColor = '#00ff00';
//						isCanCallBoss = true;
//					}
//					
//					var info:String = "<font color = '#ffff00' size = '13'>"+GameCommonData.wordDic[ "mod_unityN_med_newup_mak_3" ]+"</font>\n" +     //抵抗帮敌将消耗
//											 "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_8" ]+"</font><font color='" + moneyColor + "'>"+ UIUtils.getMoneyStandInfo( needMoney, ["\\ce","\\cs","\\cc"] ) + "</font>";//帮派资金：
//					sendNotification(EventList.SHOWALERT, { comfrim:sureDefended, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],autoSize:1,height:77,leading:6 } );   //提 示
					if ( !defendAttackPanel )
					{
						defendAttackPanel = new DefendAttackPanel();
					}
					defendAttackPanel.showMe();
				}
				return;
			}
			if ( btnIndex == 3 )
			{
				if ( NewUnityCommonData.myUnityInfo.leftBangGong < 100 )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_gai_2" ] );    //领取帮派福利需要100帮贡，你的帮贡不够
					return;
				}
			}
			
			if ( btnIndex == 2 )
			{
				if ( NewUnityCommonData.myUnityInfo.leftBangGong < 50 )
				{
					GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_gai_3" ] );    //领取聚义酒需要50帮贡，你的帮贡不够
					return;
				}
			}
			
			if ( !startTimer() ) return;
			var singleIndex:int = btnIndex + 321;
			RequestUnity.send( singleIndex,0 );
		}
		
		private function sureDefended():void
		{
			if ( isCanCallBoss )
			{
				RequestUnity.send( 322,0 );
			}
			else
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_uni_uds_bui_3" ] );    //帮派资金不足
			}
		}
		
		private function clearMe():void
		{
			if ( main_mc && parentContainer.contains( main_mc ) )
			{				
				for( var i:uint=1; i<4; i++ )
				{
					( main_mc[ "effBtn_"+i ] as SimpleButton ).removeEventListener( MouseEvent.CLICK,makeEffect );
				}
				for ( var j:uint=0; j<4; j++ )
				{
					( main_mc[ "levelUpBtn_"+j ] as SimpleButton ).removeEventListener( MouseEvent.CLICK,levelUpHandler );
				}
				
				parentContainer.removeChild( main_mc );
				openState = false;
			}
		}
		
		//操作延时器
		private function startTimer():Boolean
		{
			if(getOutTimer.running) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_pro_ite_sta" ], color:0xffff00});//"请稍后"
				return false;
			} 
			else 
			{
				getOutTimer.reset();
				getOutTimer.start();
				return true;
			}
		}
		
	}
}