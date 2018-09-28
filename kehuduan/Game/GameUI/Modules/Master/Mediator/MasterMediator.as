package GameUI.Modules.Master.Mediator
{
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Command.BottomButtonController;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.OldMaster;
	import GameUI.Modules.Master.View.MasterInfoCell;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.Components.countDown.CountDownText;
	
	import Net.ActionSend.TutorSend;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.utils.Timer;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MasterMediator extends Mediator
	{
		public static const NAME:String = "MasterMediator";
		
		private var cellContainer:Sprite = new Sprite();
		private var main_mc:MovieClip;
		private var CellClass:Class;
		private var dataProxy:DataProxy;
		
		private var currentPage:uint = 1;
		private var totalPage:uint = 10;
//		private var totalPage:uint = 1;
		private var aData:Array = new Array( 10 );
		
		private var yellowFrame:Shape;
		private var aCurList:Array;					//当前数据列表
		private var lastReqTime:uint;
		private var newTime:uint;
		private var selectMaster:OldMaster;
		private var getOutTimer:Timer = new Timer(200, 1);	//物品取出计时器
		private var reqestTimer:Timer;									//请求数据计时器
		private var refreshDownTxt:CountDownText;
		private var bottomBtnControl:BottomButtonController;
		
		//师傅登记列表
		public function MasterMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get masterView():MovieClip
		{
			return this.main_mc as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							MasterData.MASTER_STU_UI_INITVIEW,
							MasterData.START_SHOW_MAS_PAGE,
							MasterData.UPDATA_MASTER_LIST,
							MasterData.RECEIVE_REGIST_INFO,
							MasterData.CLEAR_MASTER_PANEL_PAGE
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MasterData.MASTER_STU_UI_INITVIEW:
					initUI();
					break;
				case MasterData.START_SHOW_MAS_PAGE:
					if ( notification.getBody().curPage == 0 )
					{
						openPanel();
					}
					break;
				case MasterData.UPDATA_MASTER_LIST:											//更新数据列表 
					if ( this.main_mc.currentFrame != 1 ) return;
					aCurList = [];
					var sObj:Object = notification.getBody();
					aCurList = sObj.allMaster as Array;
					totalPage = sObj.pages;
					if ( totalPage == 0 ) totalPage = 1;
					aData[ currentPage - 1 ] = aCurList.concat();
					updataList();
					break;
				case MasterData.RECEIVE_REGIST_INFO:
					receiveRegistInfo();
					break;
				case MasterData.CLEAR_MASTER_PANEL_PAGE:
					if ( notification.getBody().curPage == 0 )
					{
						gc();
					}
					break;
			}
		}
		
		private function initUI():void
		{
			main_mc = this.viewComponent as MovieClip;
			createFrame( 643,21 );
			cellContainer.x = 21;
			cellContainer.y = 83;
			main_mc.addChild( cellContainer );
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			
			bottomBtnControl = new BottomButtonController( 1 );
			bottomBtnControl.x = 124;
			bottomBtnControl.y = 420;
			bottomBtnControl.refreshFun = refreshUI;
		}
		
		private function openPanel():void
		{
			currentPage = 1;
			( main_mc.left_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,goRight );
			( main_mc.regist_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,registMaster );			//登记
			
			checkRequest();
			
			initTimer();
			bottomBtnControl.listen();
			main_mc.addChild( bottomBtnControl );
			bottomBtnControl.fourBtnVisble = false;
			bottomBtnControl._mentor = null;
			initBottomPage();
		}
		
		private function initTimer():void
		{
			if ( !reqestTimer )
			{
//				reqestTimer = new Timer( 1800 * 1000,1 );
				reqestTimer = new Timer( 1000,1 );
				reqestTimer.addEventListener( TimerEvent.TIMER_COMPLETE,timerComplete );
				reqestTimer.start();
			}
		}
		
		private function timerComplete( evt:TimerEvent ):void
		{
			for ( var i:uint=0; i<aData.length; i++ )
			{
				aData[ i ] = null;
			}
			if ( dataProxy.masterPanelIsOpen )
			{
				reqestTimer.start();
			}
			else
			{
				reqestTimer.removeEventListener( TimerEvent.TIMER_COMPLETE,timerComplete );
				reqestTimer = null;
			}
		}
		
		//请求数据
		private function checkRequest():void
		{
			if ( !aData[ currentPage - 1 ] )
			{
				requestData( 10 );
				lastReqTime = getTimer();
			}
			else
			{
				aCurList = aData[ currentPage - 1 ];
				updataList();
			}
		}
		
		private function initBottomPage():void
		{
			( main_mc.page_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + currentPage + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];    //第         页
			( main_mc.page_txt as TextField ).mouseEnabled = false;
		}
		
		private function requestData( action:uint ):void
		{
			var obj:Object = new Object();
			obj.action = action;
			obj.amount = 14;
			obj.pageIndex = currentPage;
			obj.mentorId = 0;
			TutorSend.sendTutorAction( obj );
		}
		
		private function updataList():void
		{
			clearContainer();
			initBottomPage();
			if ( aCurList.length<1 ) return;
			var masterCell:MasterInfoCell;
			for ( var i:uint=0; i<aCurList.length; i++ )
			{
				masterCell = new MasterInfoCell( aCurList[ i ] );
				masterCell.y = i * 23;
				masterCell.clickFun = clickCellHandler;
				cellContainer.addChild( masterCell );
			}
		}
		
		private function clickCellHandler( cell:MasterInfoCell ):void
		{
			cell.addChild( yellowFrame );
			yellowFrame.x = -11;
			yellowFrame.y = -3;
			bottomBtnControl._mentor = cell.oldMaster;
			bottomBtnControl.fourBtnVisble = true;
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if ( currentPage > 1 )
			{
				currentPage --;
				initBottomPage();
				clearContainer();
				bottomBtnControl.fourBtnVisble = false;
				checkRequest();
			}
		}
		
		private function goRight(evt:MouseEvent):void
		{
			if ( currentPage < totalPage )
			{
				if ( !startTimer() ) return;
				currentPage ++;
				initBottomPage();
				clearContainer();
				bottomBtnControl.fourBtnVisble = false;
				checkRequest();
			}
		}
		
		private function clearContainer():void
		{
			var des:*;
			while ( cellContainer.numChildren > 0 )
			{
				des = cellContainer.removeChildAt( 0 );
				if ( des is MasterInfoCell )
				{
					( des as MasterInfoCell ).gc();
				}
				des = null;
			}
		}
		
		private function setBtnVisble(isSee:Boolean):void
		{
			( masterView.right_btn as SimpleButton ).visible = isSee;
		}
		
		private function createFrame( fWidth:uint,fHeight:uint ):void
		{
			yellowFrame = new Shape();
			yellowFrame.graphics.clear();
			yellowFrame.graphics.lineStyle( 1,0xffff00 );
			yellowFrame.graphics.lineTo( 0,0 );
			yellowFrame.graphics.lineTo( fWidth,0 );
			yellowFrame.graphics.lineTo( fWidth,fHeight );
			yellowFrame.graphics.lineTo( 0,fHeight );
			yellowFrame.graphics.lineTo( 0,0 );
		}
		
		//登记师傅
		private function registMaster( evt:MouseEvent ):void
		{
			if ( GameCommonData.Player.Role.Level < 35 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_mas_med_masta_reg_1" ], color:0xffff00 } );      //人物等级达到35级才能登记！
				return;
			}
			if ( isOnboard() )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_mas_med_mastm_reg_1" ], color:0xffff00 } );       //你已经登记过了！
				return;
			}
			requestData( 11 );
		}
		
		//收到服务器返回的登记消息
		private function receiveRegistInfo():void
		{
			requestData( 10 );
		}
		
		private function refreshUI():void
		{
			requestData( 10 );
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
		
		//判断是否榜上有名
		private function isOnboard():Boolean
		{
			for ( var i:uint=0; i<aData.length; i++ )
			{
				if ( !aData[ i ] ) continue;
				for ( var j:uint=0; j<aData[i].length; j++ )
				{
					if ( GameCommonData.Player.Role.Name == aData[i][j].name )
					{
						return true;
					}
				}
			}
			return false;
		}
		
		//当前选中的是不是自己
//		private function isMyself():Boolean
//		{
//			if ( selectMaster && selectMaster.id == GameCommonData.Player.Role.Id )
//			{
//				return true;
//			}
//			return false;
//		}
		
		private function gc():void
		{
			clearContainer();
			( main_mc.left_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,goRight );
			
			if ( bottomBtnControl && main_mc.contains( bottomBtnControl ) )
			{
				bottomBtnControl.gc();
				main_mc.removeChild( bottomBtnControl );
			}
		}
		
	}
}