package GameUI.Modules.UnityNew.Mediator
{
	import Controller.TargetController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.ReName.Data.ReNameData;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Modules.UnityNew.Proxy.UnityBaseInfo;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.YellowButton;
	import GameUI.View.ShowMoney;
	
	import Net.ActionSend.UnityActionSend;
	import Net.ActionSend.Zippo;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldType;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnityInfoMediator extends Mediator
	{
		public static const NAME:String = "NewUnityInfoMediator";
		public static var openState:Boolean = false;
		
		private var parentContainer:Sprite;
		private var main_mc:MovieClip;
		private var myUnityBaseInfo:UnityBaseInfo;
		private var oldNotice:String;
		private var isCanPromt:Boolean;			//是否能升级
		private var canComeBack:Boolean = false;
		private var canRename:Boolean = false;	///是否能改名
		private var renameBtn:YellowButton = null;
		private var helpPanelBase:PanelBase;
		private var help_mc:MovieClip;
		
		public function NewUnityInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CHANG_NEW_UNITY_PAGE,
							NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL,
							NewUnityCommonData.UPDATE_NEW_UNITY_BASE_INFO,
							NewUnityCommonData.UPDATE_SYN_PLACE_INFO,
							NewUnityCommonData.RE_UNITY_NAME_SUCCESS,
							NewUnityCommonData.UPDATE_ON_TIME_UNITY_BASEINFO
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
					if ( NewUnityCommonData.currentPage == 0 )
					{
						openMe();
					}
				break;
				case NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL:
					if ( notification.getBody() == 0 )
					{
						clearMe();
					}
				break;
				case NewUnityCommonData.UPDATE_NEW_UNITY_BASE_INFO:
					var recUnity:UnityBaseInfo = notification.getBody() as UnityBaseInfo;
					if ( openState )
					{
						if ( recUnity.id == GameCommonData.Player.Role.unityId )
						{
							myUnityBaseInfo = recUnity;
							updateInfo( myUnityBaseInfo );
						}
					}
				break;
				case NewUnityCommonData.UPDATE_SYN_PLACE_INFO:
					if ( openState )
					{
						updateLevelTxt();
						updateFuliTxt();
					}
				break;
				case NewUnityCommonData.RE_UNITY_NAME_SUCCESS:
					if ( openState )
					{
						updateName( notification.getBody() as String );
					}
				break;
				case NewUnityCommonData.UPDATE_ON_TIME_UNITY_BASEINFO:
					if ( openState )
					{
						updateSomeInfo();
					}
				break;
			}
		}
		
		private function initView():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.UnityInfoRes ) as MovieClip;
			initTxts();
			
			help_mc = new ( NewUnityCommonData.newUnityResProvider.NewUnityHelpRes ) as MovieClip;
//			helpPanelBase = new PanelBase( help_mc,help_mc.width-16,help_mc.height+12 );
			helpPanelBase = new PanelBase( help_mc,402,help_mc.height+12 );
			
			if( GameCommonData.fullScreen == 2 )
			{
				helpPanelBase.x = UIConstData.DefaultPos2.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				helpPanelBase.y = UIConstData.DefaultPos2.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				helpPanelBase.x = UIConstData.DefaultPos2.x;
				helpPanelBase.y = UIConstData.DefaultPos2.y;
			}
				
			helpPanelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_chat_com_rec_exe_2" ] );       //帮助
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
			RequestUnity.send( 305,0 );
			RequestUnity.send( 306,0 );			//请求分堂等级数据
		}
		
		private function initTxts():void
		{
			for ( var i:uint=0; i<14; i++ )
			{
				if ( i == 10 ) continue;
				main_mc[ "unityInfoTxt_"+i ].mouseEnabled = false;
				main_mc[ "unityInfoTxt_"+i ].text = "";
			}
			main_mc.unityHelp_txt.mouseEnabled = false;
			main_mc.unityHelp_txt.text = "";
			main_mc.ChgNotice_txt.mouseEnabled = false;
			main_mc.ChgNotice_txt.text = GameCommonData.wordDic[ "mod_unityN_med_newui_ini_1" ];     //修改公告
		}
		
		private function updateInfo( baseInfo:UnityBaseInfo ):void
		{
			oldNotice = baseInfo.notice;
			main_mc[ "unityInfoTxt_"+1 ].text = baseInfo.boss;
			baseInfo.viceBoss1 == "0" ? main_mc[ "unityInfoTxt_"+2 ].text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ] : main_mc[ "unityInfoTxt_"+2 ].text = baseInfo.viceBoss1;     //无
			baseInfo.viceBoss2 == "0" ? main_mc[ "unityInfoTxt_"+3 ].text = GameCommonData.wordDic[ "mod_rank_med_rm_hn_3" ] : main_mc[ "unityInfoTxt_"+3 ].text = baseInfo.viceBoss2;     //无
			main_mc[ "unityInfoTxt_"+4 ].text = baseInfo.level.toString();
			main_mc[ "unityInfoTxt_"+5 ].text = baseInfo.onLinePeople.toString();
			main_mc[ "unityInfoTxt_"+6 ].text = baseInfo.currentPeople.toString() + "/" + baseInfo.maxPeople.toString();
//			main_mc[ "unityInfoTxt_"+7 ].text = baseInfo.money.toString();
			main_mc[ "unityInfoTxt_"+7 ].text = baseInfo.jianShe.toString();
			main_mc[ "unityInfoTxt_"+8 ].text = baseInfo.notice.toString();
			main_mc[ "unityInfoTxt_"+9 ].text = baseInfo.unityJobStr;
			main_mc[ "unityInfoTxt_"+12 ].text = baseInfo.leftBangGong.toString();
			main_mc[ "unityInfoTxt_"+13 ].text = baseInfo.historyBangGong.toString();
			checkCanReName();
			
			main_mc.money_mc.txtMoney.text = UIUtils.getMoneyStandInfo( baseInfo.money, ["\\ce","\\cs","\\cc"] );
			ShowMoney.ShowIcon( main_mc.money_mc, main_mc.money_mc.txtMoney, true );
			if ( NewUnityCommonData.myUnityInfo.level >= 10 )
			{
				( main_mc.levelUp_btn as SimpleButton ).visible = false;
			}
		}
		
		//更新福利
		private function updateFuliTxt():void
		{
			//领取福利数目，根据朱雀堂等级来的。
			var redLevel:int = NewUnityCommonData.unityPlaceLevelArr[3];
			if ( redLevel <= 1 )
			{
				updateFuliMoney( 1 );
				main_mc[ "unityInfoTxt_"+11 ].text = GameCommonData.wordDic[ "mod_unityN_med_newui_upd_1" ] + "10";       //有几率获得
			}
			else
			{
//				main_mc[ "unityInfoTxt_"+10 ].text = redLevel.toString();
				updateFuliMoney( redLevel );
				main_mc[ "unityInfoTxt_"+11 ].text = GameCommonData.wordDic[ "mod_unityN_med_newui_upd_1" ] + ( ( redLevel+1)*5 ).toString();    //有几率获得
			}
		}
		
		private function updateFuliMoney( m:int ):void
		{
			main_mc.fuliMoney_mc.txtMoney.text = UIUtils.getMoneyStandInfo( m*10000, ["\\se","\\ss","\\sc"] );
			ShowMoney.ShowIcon( main_mc.fuliMoney_mc, main_mc.fuliMoney_mc.txtMoney, true );

		}
		
		private function updateSomeInfo():void
		{
			checkCanReName();
			main_mc.money_mc.txtMoney.text = UIUtils.getMoneyStandInfo( NewUnityCommonData.myUnityInfo.money, ["\\ce","\\cs","\\cc"] );
			ShowMoney.ShowIcon( main_mc.money_mc, main_mc.money_mc.txtMoney, true );
			main_mc[ "unityInfoTxt_"+12 ].text = NewUnityCommonData.myUnityInfo.leftBangGong.toString();
			main_mc[ "unityInfoTxt_"+13 ].text = NewUnityCommonData.myUnityInfo.historyBangGong.toString();
			main_mc[ "unityInfoTxt_"+7 ].text = NewUnityCommonData.myUnityInfo.jianShe.toString();
		}
		
		//检查是否可改名
		private function checkCanReName():void
		{
			if ( NewUnityCommonData.reUnityName == 0 )
			{
				main_mc[ "unityInfoTxt_"+0 ].htmlText = "<font color='#e2cca5'>"+NewUnityCommonData.myUnityInfo.name+"</font>";
				main_mc.unityHelp_txt.htmlText = NewUnityCommonData.myUnityInfo.weiHuStr;
				if ( renameBtn )
				{
					renameBtn.visible = false;
				}
			}
			else
			{
				main_mc[ "unityInfoTxt_"+0 ].htmlText = "<font color='#e2cca5'>"+NewUnityCommonData.myUnityInfo.name+"</font>";
				main_mc.unityHelp_txt.htmlText = "";
				if ( !renameBtn )
				{
					renameBtn = new YellowButton();
					renameBtn.init();
					renameBtn.x = 162;
					renameBtn.y = main_mc[ "unityInfoTxt_0" ].y - 1;
					renameBtn.text = GameCommonData.wordDic[ "mod_unityN_med_newui_che_1" ];          //帮派改名
					main_mc.addChild( renameBtn );
					renameBtn.addEventListener( MouseEvent.CLICK,renameHandler );
				}
			}
		}
		
		//改名成功
		private function updateName( newName:String ):void
		{
			NewUnityCommonData.reUnityName = 0;
			if ( renameBtn )
			{
				renameBtn.removeEventListener( MouseEvent.CLICK,renameHandler );
				renameBtn.gc()
				main_mc.removeChild( renameBtn );
				renameBtn = null;
			}
			NewUnityCommonData.myUnityInfo.name = newName;
			main_mc[ "unityInfoTxt_"+0 ].htmlText = "<font color='#e2cca5'>"+NewUnityCommonData.myUnityInfo.name+"</font>"+NewUnityCommonData.myUnityInfo.weiHuStr;
		}
		
		private function renameHandler( evt:MouseEvent ):void
		{
			sendNotification( ReNameData.SHOW_RENAME_VIEW );
		}
		
		private function updateLevelTxt():void
		{
			for ( var i:uint=0; i<4; i++ )
			{
				main_mc[ "tangTxt_" + i ].text = NewUnityCommonData.unityPlaceLevelArr[ i ].toString();
			}
		}
		
		private function initBtns():void
		{
			( main_mc.levelUp_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,levelUp );
			( main_mc.chgNotice_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,chgNotice );
			( main_mc.comeBack_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,comeBack );
			( main_mc.bangGongChg_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,bangGongChg );
			( main_mc.tanHe_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,tanHe );
			( main_mc.goAway_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goAway );
			( main_mc.gainAward_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,gainAward );
			( main_mc.contribute_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,contributeHandler );
			( main_mc.help_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,showHelp );
		}
		
		//升级
		private function levelUp( evt:MouseEvent ):void
		{
			if ( !NewUnityCommonData.myUnityInfo.isStop )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_lev_1" ] );        //帮派暂停维护中，不能升级
				return;
			}
			isCanPromt = true;
			var colorArr:Array = [];
			for ( var i:uint=0; i<NewUnityCommonData.unityPlaceLevelArr.length; i++ )
			{
				if ( NewUnityCommonData.unityPlaceLevelArr[i]<myUnityBaseInfo.level )
				{
					isCanPromt = false;
					colorArr.push( "#ff0000" );
				}
				else
				{
					colorArr.push( "#00ff00" );
				}
			}
			
			var tangStr:String = "\n<font color = '#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_2" ]+"</font><font color='" + colorArr[0] +"'>" + myUnityBaseInfo.level.toString() + GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>\n"      //青 龙 堂：    级
							+"<font color = '#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_3" ]+"</font><font color='" + colorArr[1] +"'>" + myUnityBaseInfo.level.toString()+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>\n"            //白 虎 堂：    级
							+"<font color = '#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_4" ]+"</font><font color='" + colorArr[3] +"'>" + myUnityBaseInfo.level.toString()+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>\n"            //朱 雀 堂：    级
							+"<font color = '#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_5" ]+"</font><font color='" + colorArr[2] +"'>" + myUnityBaseInfo.level.toString()+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>\n"            //玄 武 堂：    级
			var jiansheArr:Array = [ 0,5000,10000,25000,50000,100000,150000,200000,250000,300000 ]; 
			var maxMoneyArr:Array = [ 0,1000,2000,4000,8000,12000,16000,20000,24000,28000 ];
//			var needJianShe:int = 5000*Math.pow( 2,myUnityBaseInfo.level-1 );
			var needJianShe:int = jiansheArr[ myUnityBaseInfo.level ];
			var needMoney:int = 10000*maxMoneyArr[ myUnityBaseInfo.level ]; 
			var jianSheColor:String;
			var moneyColor:String;
			if ( myUnityBaseInfo.jianShe < needJianShe )
			{
				jianSheColor = '#ff0000';
				isCanPromt = false;
			}
			else
			{
				jianSheColor = '#00ff00';
			}
			
			if ( myUnityBaseInfo.money < needMoney )
			{
				moneyColor = '#ff0000';
				isCanPromt = false;
			}
			else
			{
				moneyColor = '#00ff00';
			}
			
			var info:String = "<font color = '#ffff00' size = '13'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_6" ]+"</font>" +         //帮派升级条件
					tangStr+
					"<font color = '#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_7" ]+"</font><font color='" + jianSheColor +       //建 设 度：
							"'>"+needJianShe.toString() + "</font>\n<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_lev_8" ]+"</font><font color='" + moneyColor + "'>"+ UIUtils.getMoneyStandInfo( needMoney, ["\\ce","\\cs","\\cc"] ) + "</font>";  //帮派资金：
			sendNotification(EventList.SHOWALERT, { comfrim:sureLevelUp, cancel:cancelClose, info:info, title:"提 示",autoSize:1,height:147,leading:6 } );   // 提 示
		}
		
		private function sureLevelUp():void
		{
			if ( GameCommonData.Player.Role.unityJob<90 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );      //你的权限不足
				return;
			}
			if ( !isCanPromt )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_sur_1" ] );     //升级条件不满足
				return;
			}
			RequestUnity.send( 310,0 );
		}
		
		//修改公告
		private function chgNotice( evt:MouseEvent ):void
		{
			if ( GameCommonData.Player.Role.unityJob<90 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ] );        //你的权限不足
				return;
			}
			var str:String=this.main_mc.ChgNotice_txt.text;
			if( str==GameCommonData.wordDic[ "mod_unityN_med_newui_ini_1" ] )        //修改公告
			{
//				this.main_mc.ChgNotice_txt.text=GameCommonData.wordDic[ "often_used_commit" ];//"确定"
				this.main_mc.ChgNotice_txt.text = GameCommonData.wordDic[ "mod_unityN_med_newui_sur_2" ];//"确  定"
				main_mc.unityInfoTxt_8.type=TextFieldType.INPUT;
				main_mc.unityInfoTxt_8.selectable=true;
				main_mc.unityInfoTxt_8.maxChars=70;
//				(main_mc.unityInfoTxt_9 as TextField).scrollH=0;
				(main_mc.unityInfoTxt_8 as TextField).multiline=true;
				main_mc.unityInfoTxt_8.mouseEnabled=true;
				main_mc.unityInfoTxt_8.border=true;
				main_mc.unityInfoTxt_8.borderColor=0xffffff;
				(main_mc.unityInfoTxt_8 as TextField).background=true;
				(main_mc.unityInfoTxt_8 as TextField).backgroundColor=0x4D3c13;
				(main_mc.unityInfoTxt_8 as TextField).addEventListener(KeyboardEvent.KEY_DOWN,onKeyDownHandler);
//				main_mc.parent.stage.focus=main_mc.unityInfoTxt_9;
				UIUtils.addFocusLis( main_mc.unityInfoTxt_8 );
				var len:int=main_mc.unityInfoTxt_9.length;
				(main_mc.unityInfoTxt_9 as TextField).setSelection(len,len);
				return;
			}
//			if(str==GameCommonData.wordDic[ "often_used_commit" ])
			if(str==GameCommonData.wordDic[ "mod_unityN_med_newui_sur_2" ])      //确  定
			{
				(main_mc.unityInfoTxt_8 as TextField).removeEventListener(KeyboardEvent.KEY_DOWN,onKeyDownHandler);
				UIUtils.removeFocusLis( main_mc.unityInfoTxt_8 );
				this.main_mc.ChgNotice_txt.text=GameCommonData.wordDic[ "mod_unityN_med_newui_ini_1" ];//修改公告;
				main_mc.unityInfoTxt_8.type=TextFieldType.DYNAMIC;
				main_mc.unityInfoTxt_8.mouseEnabled=false;
				(main_mc.unityInfoTxt_8 as TextField).background=false;
				main_mc.unityInfoTxt_8.border=false;
				var notice:String = ( main_mc.unityInfoTxt_8 as TextField ).text;
				
				if ( oldNotice == notice )
				{
					return;
				}
				oldNotice = notice;
				
				if(UIUtils.checkChat(notice) == false)
				{
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_7" ], color:0xffff00});  // 文本内容不合法
					return;
				}
				var sendObj:Object = new Object();
				sendObj.type = 1107;														//协议号
				sendObj.data = [0 , notice , 218 , 0 , 0];
				UnityActionSend.SendSynAction( sendObj );										//发送修改公告请求
				return;
			}
		}
		
		protected function onKeyDownHandler(e:KeyboardEvent):void
		{
			if(e.keyCode==13)
			{
				chgNotice(null);
			}
		}
		
		//快速回帮
		private function comeBack( evt:MouseEvent ):void
		{
			if(StallConstData.stallSelfId > 0) 
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_com_1" ] );     //摆摊中不能回帮
				return;
			}
			if ( TargetController.IsPKTeam() )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_com_2" ] );     //该场景不能回帮
				return;
			}
			if ( GameCommonData.GameInstance.GameScene.GetGameScene.name == "1048" )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_com_3" ] );      //你已经位于帮派中
				return;
			}
			
			 if(GameCommonData.GameInstance.GameScene.GetGameScene.name != GameCommonData.GameInstance.GameScene.GetGameScene.MapId) 
			 {
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_com_4" ] );        //副本中不能回帮
				return;
			 }
			 
			 if ( GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1026" )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_com_5" ] );             //地府中不能回帮
				return;
			}
			else if ( GameCommonData.GameInstance.GameScene.GetGameScene.MapId == "1027" )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_com_6" ] );             //监狱中不能回帮
				return;
			}
			
			var newDate:Date = new Date();
			var dateDis:int = newDate.getTime() - NewUnityCommonData.lastComeUnityDate.getTime();
			var oldDate:Date = NewUnityCommonData.lastComeUnityDate;
			var info:String;
			if ( dateDis >= NewUnityCommonData.nextComeUnityTime )
			{
				canComeBack = true;
				info = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_com_7" ]+"\n</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_com_8" ]+"</font>";   //每次快速回帮的时间间隔是30分钟          江陵或开封的帮派总管处也可回帮
			}
			else
			{
				var waiteTime:uint = NewUnityCommonData.nextComeUnityTime - dateDis;
				canComeBack = false;         //你还需要等待                                                                                         //分钟才可快速回帮                                  //江陵或开封的帮派总管处也可回帮
				info = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_com_9" ]+"<font color='#ff0000'>" + ( Math.ceil( waiteTime/60000 )).toString() + "</font>"+GameCommonData.wordDic[ "mod_unityN_med_newui_com_10" ]+"\n</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_com_8" ]+"</font>";
			}
				
			//弹出确认框
			sendNotification(EventList.SHOWALERT, {comfrim:commitComeBack, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ] });  //提 示      确 定      取 消
			
		}
		
		private function commitComeBack():void
		{
			if ( !canComeBack ) return;
			sendNotification( NewUnityCommonData.CLOSE_NEW_UNITY_MAIN_PANEL );
//			RequestUnity.send( 320,0 );
			Zippo.SendSkill( GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Id,0,0,21,2501,1 );
		}
		
		//领取福利
		private function gainAward( evt:MouseEvent ):void
		{
			if ( !NewUnityCommonData.myUnityInfo.isStop )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_gai_1" ] );    //帮派暂停维护中，不能领取福利
				return;
			}
			if ( NewUnityCommonData.myUnityInfo.leftBangGong < 100 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_gai_2" ] );    //领取帮派福利需要100帮贡，你的帮贡不够
				return;
			}
			RequestUnity.send( 324,0 );
		}
		
		//帮贡兑换
		private function bangGongChg( evt:MouseEvent ):void
		{
			if ( !NewUnityCommonData.myUnityInfo.isStop )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_ban_1" ] );        //帮派暂停维护中，无法使用
				return;
			}
			RequestUnity.send( 326,0,GameCommonData.Player.Role.Id );
		}
		
		//弹劾帮主
		private function tanHe( evt:MouseEvent ):void
		{
//			RequestUnity.send( 326,0 );
			if ( GameCommonData.Player.Role.unityJob != 90 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_tan_1" ] );          //只有副帮主才可弹劾帮主
				return;
			}
			
			var info:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_tan_2" ]+"</font>";    //弹劾帮主将扣除250点帮贡，是否确认？
			sendNotification(EventList.SHOWALERT, {comfrim:commitTanHe, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});  //提 示      确 定      取 消
		}
		
		private function commitTanHe():void
		{
			if ( NewUnityCommonData.myUnityInfo.leftBangGong<250 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_comm_1" ] );      //您的帮贡不够，弹劾失败！
				return; 
			}
			RequestUnity.send( 336,0,boosId );
		}
		
		//退出帮派
		private function goAway( evt:MouseEvent ):void
		{
			if ( GameCommonData.Player.Role.unityJob == 100 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newui_goa_1" ] );         //帮主不可以退帮
				return;
			}
			var info:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_unityN_med_newui_goa_2" ]+"</font>";    //退出帮派将清空总帮贡和当前帮贡，是否确认？
			sendNotification(EventList.SHOWALERT, {comfrim:commitGoAway, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});  //提 示      确 定      取 消
		}
		
		private function commitGoAway():void
		{
			RequestUnity.send( 214,0 );
		}
		
		//捐献
		private function contributeHandler( evt:MouseEvent ):void
		{
			if(UnityConstData.contributeIsOpen == false)
			{
				facade.sendNotification(UnityEvent.SHOWCONTRIBUTEVIEW);
			}
			else facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);
		}
		
		private function cancelClose():void{}
		
		private function clearMe():void
		{
			if ( main_mc && parentContainer.contains( main_mc ) )
			{
				( main_mc.levelUp_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,levelUp );
				( main_mc.chgNotice_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,chgNotice );
				( main_mc.comeBack_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,comeBack );
				( main_mc.bangGongChg_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,bangGongChg );
				( main_mc.tanHe_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,tanHe );
				( main_mc.goAway_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goAway );
				( main_mc.gainAward_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,gainAward );
				( main_mc.contribute_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,contributeHandler );
				( main_mc.help_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,showHelp );
				
				
				(main_mc.unityInfoTxt_8 as TextField).removeEventListener(KeyboardEvent.KEY_DOWN,onKeyDownHandler);
				UIUtils.removeFocusLis( main_mc.unityInfoTxt_8 );
				this.main_mc.ChgNotice_txt.text=GameCommonData.wordDic[ "mod_unityN_med_newui_ini_1" ];//修改公告;
				main_mc.unityInfoTxt_8.type=TextFieldType.DYNAMIC;
				main_mc.unityInfoTxt_8.mouseEnabled=false;
				(main_mc.unityInfoTxt_8 as TextField).background=false;
				main_mc.unityInfoTxt_8.border=false;
				
				var str:String=this.main_mc.ChgNotice_txt.text;
				if(str==GameCommonData.wordDic[ "often_used_commit" ])
				{
					this.chgNotice(null);
				}
				if ( renameBtn )
				{
					renameBtn.removeEventListener( MouseEvent.CLICK,renameHandler );
					renameBtn.gc();
					main_mc.removeChild( renameBtn );
					renameBtn = null;
				}
				
				parentContainer.removeChild( main_mc );
				openState = false;
			}
		}
		
		private function showHelp( evt:MouseEvent ):void
		{
			if ( !GameCommonData.GameInstance.GameUI.contains( helpPanelBase ) )
			{
				help_mc.closeHelp_btn.addEventListener( MouseEvent.CLICK,closeHelp );
				helpPanelBase.addEventListener( Event.CLOSE,closeHelp );
			}
			GameCommonData.GameInstance.GameUI.addChild( helpPanelBase );
		}
		
		private function closeHelp( evt:Event ):void
		{
			if ( GameCommonData.GameInstance.GameUI.contains( helpPanelBase ) )
			{
				help_mc.closeHelp_btn.removeEventListener( MouseEvent.CLICK,closeHelp );
				helpPanelBase.removeEventListener( Event.CLOSE,closeHelp );
				GameCommonData.GameInstance.GameUI.removeChild( helpPanelBase );
			}
		}
		
		//获取帮主id
		private function get boosId():int
		{
			for ( var i:uint=0; i<NewUnityCommonData.allUnityMemberArr.length; i++ )
			{
				if ( NewUnityCommonData.allUnityMemberArr[i].unityJob == 100 )
				{
					return NewUnityCommonData.allUnityMemberArr[i].id;
				}
			}
			return 0;
		}
		
	}
}