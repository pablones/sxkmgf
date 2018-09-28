package GameUI.Modules.IdentifyTreasure.Mediator
{
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Chat.UI.ChatCellEvent;
	import GameUI.Modules.Chat.UI.ChatCellText;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Modules.IdentifyTreasure.Net.TreasureNet;
	import GameUI.Modules.IdentifyTreasure.Proxy.TreaResource;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	//开箱子主类
	public class IdentifyTreaMediator extends Mediator
	{
		public static const NAME:String = "IdentifyTreaMediator";
		private var treaRes:TreaResource;														//资源加载器
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		private var dataProxy:DataProxy;
		
		private var resourceObj:Object;																//资源存放器    暂时没用上
		private var treasureGridMediator:TreasureGridMediator = null;
		private var treasurePackageMediator:TreasurePackageMediator = null;
		private var treasureAwardMediator:TreasureAwardMediator = null;
		
		//需要的元宝
		private var needMoneyArr:Array;
		private var bestAwardArr:Array;
		private var bestAward_txt:ChatCellText;
		
		//当前页签
		public static var curBtn:uint = 0;
		private var aCurMoney:Array = new Array(); 
		
		private var currentTimes:int = -1;
		private var roundId:int;
		//发生意外延时
		private var outMindId:int;
		
		public function IdentifyTreaMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [ 	TreasureData.LOAD_TREASURE_RES,
							TreasureData.TREA_RES_LOAD_COM,
							TreasureData.SHOW_TREASURE_UI,
							TreasureData.CLOSE_TREASURE_UI,
							TreasureData.RECEIVE_AWARD_TREA,
							TreasureData.UPDATE_TREA_PACKAGE_SPACE,
							EventList.UPDATEMONEY
						];	
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case TreasureData.LOAD_TREASURE_RES:
					if ( GameCommonData.wordVersion == 1 && GameCommonData.Player.Role.Level<30 && GameCommonData.Player.Role.VIP == 0 )
					{
						showHint( "30级以后才能开启" );//"30级以后才能开启" 
						return;
					}
					loadRes();
				break;
				case TreasureData.TREA_RES_LOAD_COM:
					resLoadCom( notification.getBody() );
				break;
				case TreasureData.SHOW_TREASURE_UI:
					initUI();
					showUI();
				break;
				case TreasureData.CLOSE_TREASURE_UI:
					if ( GameCommonData.openTreasureStragety == 2 )
					{
						sendNotification( TreasureData.CLOSE_MY_TREA_PACKAGE );
						return;
					}
					closeUI( null );
				break;
				case TreasureData.RECEIVE_AWARD_TREA:
					clearTimeout( outMindId );
					receiveAwardHandler( notification.getBody() );
				break;
				case TreasureData.UPDATE_TREA_PACKAGE_SPACE:
					upDataPackageSpace();
				break;
				case EventList.UPDATEMONEY:
					if ( main_mc )
					{
						updataMoney();		
					}
				break;
			}
		}
		
		private function loadRes():void
		{
			treaRes = new TreaResource();
		}
		
		//获取外部资源数据
		private function resLoadCom( obj:Object ):void
		{
			resourceObj = obj;
			main_mc = obj.main_mc;
			setViewComponent( main_mc );
			
			treasureGridMediator = new TreasureGridMediator();
			treasureGridMediator.setViewComponent( main_mc );
			facade.registerMediator( treasureGridMediator );
			sendNotification( TreasureData.CREATE_TREA_GRID );
			//包裹mediator
			treasurePackageMediator = new TreasurePackageMediator();
			treasurePackageMediator.setViewComponent( obj.package_mc );
			facade.registerMediator( treasurePackageMediator );
			
			//开箱子策略，临时撤下时
			if ( GameCommonData.openTreasureStragety == 2 )
			{
				goAway();
				return;
			}
			
			//奖品显示mediator
			treasureAwardMediator = new TreasureAwardMediator();
			treasureAwardMediator.setViewComponent( obj.showAward_mc );
			treasureAwardMediator.initialize();
			facade.registerMediator( treasureAwardMediator );
			
			needMoneyArr = obj.needMoneyArr;
			bestAwardArr = obj.bestAwardArr;
			
			panelBase = new PanelBase( main_mc, 700, 437 );
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.name = "TiaoZhan";
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_ide_med_ide_res" ]);//"江湖挑战"
			var point:Point = new Point( 350,0 );
			panelBase.setTitlePos( point );
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			createAwardTxt();
			initUI();
			initData();
			
			showUI();
		}
		
		//创建最好奖励文本
		private function createAwardTxt():void
		{
			if ( bestAward_txt && main_mc.contains( bestAward_txt ) )
			{
				bestAward_txt.removeEventListener( ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler );
				main_mc.removeChild( bestAward_txt );
				bestAward_txt = null;
			}
			var itemType:Array = bestAwardArr[ curBtn ];
			var name0:String = UIConstData.getItem( itemType[0] ).Name;
			var color0:uint = UIConstData.getItem( itemType[0] ).Color;
			
			var name1:String = UIConstData.getItem( itemType[1] ).Name;
			var color1:uint = UIConstData.getItem( itemType[1] ).Color;
			
			var info:String;
			switch ( curBtn )
			{
				case 0:
					info = GameCommonData.wordDic[ "mod_ide_med_ide_cre_1" ]+"<1_[" + name0 + "]_0_"+itemType[0]+"_0_"+color0+">，"+"<1_["+ name1 + "]_0_"+itemType[1]+"_0_"+color1+">";//挑战剑徒，有机会获得
				break;
				case 1:
					info = GameCommonData.wordDic[ "mod_ide_med_ide_cre_2" ]+"<1_[" + name0 + "]_0_"+itemType[0]+"_0_"+color0+">，"+"<1_["+ name1 + "]_0_"+itemType[1]+"_0_"+color1+">";//挑战剑客，有机会获得
				break;
				case 2:
					info = GameCommonData.wordDic[ "mod_ide_med_ide_cre_3" ]+"<1_[" + name0 + "]_0_"+itemType[0]+"_0_"+color0+">，"+"<1_["+ name1 + "]_0_"+itemType[1]+"_0_"+color1+">";//挑战剑豪，有机会获得
				break;
			}
			bestAward_txt = new ChatCellText( info,0xf7a52b );
			bestAward_txt.addEventListener( ChatCellEvent.CHAT_CELLLINK_CLICK, getLinkHandler );
			bestAward_txt.x = 102;
//			bestAward_txt.x = 110;
			bestAward_txt.y = 304;
			main_mc.addChild( bestAward_txt );
		}
		
		private function showUI():void
		{
//			//开箱子策略，临时撤下时
//			if ( GameCommonData.openTreasureStragety == 2 )
//			{
//				goAway();
//				return;
//			}
			setMainEnable( true );
			
			panelBase.addEventListener( Event.CLOSE,closeUI );	
			panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width) / 2;
			panelBase.y = ( GameCommonData.GameInstance.GameUI.stage.stageHeight - 457.8 ) / 2; 

//			trace ( "舞台宽度  " + GameCommonData.GameInstance.GameUI.stage.stageHeight,"   panelbase宽度: "+ panelBase.height);

			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			dataProxy.treasurePanelIsOpen = true;
//			main_mc.gotoAndStop( curBtn+1 );
			( main_mc[ "circle_"+curBtn ] as MovieClip ).gotoAndStop( 1 );
			upDateMoneyAndSpace();
			upDateNeedMoney();
			//侦听事件
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip )	.buttonMode = true;
				( main_mc[ "mcPage_"+i ] as MovieClip )	.addEventListener( MouseEvent.CLICK,clickPage );
				
				( main_mc[ "challenge_"+i ] as SimpleButton ).addEventListener( MouseEvent.CLICK,startChallenge );
			}
			( main_mc.cz_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goChongZhi );
			( main_mc.lookPackage_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,lookPackage );
		}
		
		//根据页签获取数据
		private function initData():void
		{
			aCurMoney = this.needMoneyArr[ curBtn ];
			//通知另外一个mediator
			sendNotification( TreasureData.CHG_TREA_GRID_DATA,{ curBtn:curBtn } );
		}
		
		//根据页签初始化
		private function initUI():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				if ( i == curBtn )
				{
					( main_mc[ "mcPage_"+i ] as MovieClip )	.gotoAndStop( 1 );
				}
				else
				{
					( main_mc[ "mcPage_"+i ] as MovieClip )	.gotoAndStop( 2 );
				}
			}
			main_mc.gotoAndStop( curBtn+1 );
			//圈圈 
			( main_mc[ "circle_"+curBtn ] as MovieClip ).gotoAndStop( 1 );
			( main_mc[ "circle_"+curBtn ] as MovieClip ).mouseEnabled = false;
		}
		
		//点击页签
		private function clickPage( evt:MouseEvent ):void
		{
			var index:uint = uint( evt.target.name.split( "_" )[1] );
			if ( index == curBtn ) return;
			curBtn = index;
			initData();
			initUI();
			createAwardTxt();
			upDateNeedMoney();
		}
		
		//更新钱和包裹容量
		private function upDateMoneyAndSpace():void
		{
			( main_mc.haveMoney_txt as TextField ).text = GameCommonData.Player.Role.UnBindRMB.toString();
			( main_mc.packageSpace_txt as TextField ).text = GameCommonData.wordDic[ "mod_ide_med_ide_upd" ]+"："+TreasureData.packageDateArr.length+"/200";//包裹容量
		}
		
		private function updataMoney():void
		{
			( main_mc.haveMoney_txt as TextField ).text = GameCommonData.Player.Role.UnBindRMB.toString();
		}
		
		//更新需要的元宝
		private function upDateNeedMoney():void
		{
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "needMoneyTxt_"+i ] as TextField ).text = aCurMoney[i].toString();
			}
		}
		
		//点击挑战按钮
		private function startChallenge( evt:MouseEvent ):void
		{
			if ( SystemSettingData._dataArr[6] == 1 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_ide_med_ide_sta_1" ], color:0xffff00});//"请先关闭账号保护锁"
				return;
			}
			
			var index:uint = evt.target.name.split( "_" )[1];
			if ( GameCommonData.Player.Role.UnBindRMB < aCurMoney[ index ] )
			{
				facade.sendNotification(EventList.SHOWALERT, { comfrim:this.gotoWeb, cancel:cancelClose, info:GameCommonData.wordDic[ "mod_ide_med_ide_sta_2" ], title:GameCommonData.wordDic[ "often_used_tip" ],comfirmTxt:GameCommonData.wordDic[ "often_used_yes" ],cancelTxt:GameCommonData.wordDic[ "often_used_no" ],params:null });
				//"元宝不足，是否充值？"		"提 示"		"是"		"否"
				return;
			}
			//判断背包容量是否足够  额外有个奖章
			var hasSpace:int = 199 - TreasureData.packageDateArr.length;
			
			switch ( index )
			{
				case 0:
					currentTimes = 1;
				break;
				case 1:
					currentTimes = 10;
				break;
				case 2:
					currentTimes = 50;
				break;
			}
			if ( currentTimes>hasSpace )
			{
				showHint( GameCommonData.wordDic[ "mod_ide_med_ide_sta_3" ] );//"请先清理包裹"
				return;
			}
			( main_mc[ "circle_"+curBtn ] as MovieClip ).play();
			setMainEnable( false );
			roundId = setTimeout( sendDataToServer,2000 );
			outMindId = setTimeout( outMind,12000 );
		}
		
		private function sendDataToServer():void
		{
			clearTimeout( roundId );
			TreasureNet.sendChallenge( currentTimes,(curBtn+1) );
		}
		
		//10秒还没收到服务器消息，自动停止
		private function outMind():void
		{
			clearTimeout( outMindId );
			( main_mc[ "circle_"+curBtn ] as MovieClip ).gotoAndStop( 1 );
			setMainEnable( true );
		}
		
		//查看包裹
		private function lookPackage( evt:MouseEvent ):void
		{
			sendNotification( TreasureData.OPEN_MY_TREA_PACKAGE );
		}
		
		//充值
		private function goChongZhi( evt:Event ):void
		{
			this.gotoWeb();
		}
		
		private function gotoWeb():void
		{
			facade.sendNotification(TerraceController.NAME , "pay");
		}
		
		private function cancelClose():void
		{
			
		}
		
		/** 点击链接 */
		private function getLinkHandler( e:ChatCellEvent ):void
		{
			var data:String = e.data as String;
			var dataArr:Array = data.split("_");
			
			if ( dataArr[0] == 0 ) 
			{				//玩家名
				if ( dataArr[1] == "["+GameCommonData.Player.Role.Name+"]" || dataArr[1] == GameCommonData.Player.Role.Name || ChatData.QuickChatIsOpen ) return;
				var quickSelectMediator:QuickSelectMediator = new QuickSelectMediator();
			 	facade.registerMediator(quickSelectMediator);
				facade.sendNotification(ChatEvents.SHOWQUICKOPERATOR, dataArr[1].substring(1, dataArr[1].length-1));
			} 
			else if ( dataArr[0] == 1 ) 
			{		//物品
				var type:int = int( dataArr[3] );
				var dataItem:Object = null;
				if ( type < 700000 ) 
				{
					dataItem = UIConstData.getItem(type);
					dataItem.id = undefined; 
					dataItem.isBind = int(dataArr[5]);
					facade.sendNotification( EventList.SHOWITEMTOOLPANEL, { type:dataItem.type, data:dataItem } );
				} 
			} 
			else if ( dataArr[0] == 2 ) 
			{		//宠物
				if ( dataArr[2]  >= 2000000000 && dataArr[2] <= 3999999999 ) 
				{
					if ( GameCommonData.Player.Role.Id == int(dataArr[4]) ) return;
					facade.sendNotification( PetEvent.PET_LOOK_OUTSIDE_INTERFACE, { petId:int(dataArr[2]),ownerId:int(dataArr[4]) } );
				}
			}
		}
		
		//收到奖励
		private function receiveAwardHandler( obj:Object ):void
		{
//			( main_mc[ "circle_"+curBtn ] as MovieClip ).stop();
			( main_mc[ "circle_"+curBtn ] as MovieClip ).gotoAndStop( 1 );
			setMainEnable( true );
			sendNotification( TreasureData.SHOW_TREA_AWARD_PANEL,{ aAward:obj.aAward } );
			sendNotification( TreasureData.SHOW_HIGHT_HAND_GIVES,{ aAward:obj.aAward,curBtn:curBtn } );
			upDateMoneyAndSpace();
		}
		
		private function showHint( info:String ):void
		{
			facade.sendNotification(HintEvents.RECEIVEINFO, { info:info, color:0xffff00 } );
		}
		
		private function closeUI( evt:Event ):void
		{
			clearTimeout( roundId );
			clearTimeout( outMindId );
			panelBase.removeEventListener( Event.CLOSE,closeUI );	
			for ( var i:uint=0; i<3; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip )	.removeEventListener( MouseEvent.CLICK,clickPage );
				( main_mc[ "challenge_"+i ] as SimpleButton ).removeEventListener( MouseEvent.CLICK,startChallenge );
			}
			( main_mc.cz_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goChongZhi );
			( main_mc.lookPackage_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,lookPackage );
			
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );	  			
			}
			dataProxy.treasurePanelIsOpen = false;
		}
		
		private function setMainEnable( value:Boolean ):void
		{
			main_mc.mouseChildren = value;
		}
		
		private function upDataPackageSpace():void
		{
			( main_mc.packageSpace_txt as TextField ).text = GameCommonData.wordDic[ "mod_ide_med_ide_upd" ]+"："+TreasureData.packageDateArr.length+"/200";//包裹容量
		}
		
		//处理开箱子策略
		private function goAway():void
		{
			if ( TreasureData.packageDateArr.length>0 )
			{
				sendNotification( TreasureData.OPEN_MY_TREA_PACKAGE );
			}
			else
			{
				showHint( GameCommonData.wordDic[ "mod_ide_com_sho_exe" ] );//"宝物包裹无任何物品"
			}
		}
		
	}
}