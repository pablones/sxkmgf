package GameUI.Modules.UnityNew.Mediator.Member
{
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Modules.UnityNew.View.IUnityCell;
	import GameUI.Modules.UnityNew.View.SingleApplyListCell;
	import GameUI.UIUtils;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.text.TextField;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SingleApplyListMediator extends Mediator
	{
		public static const NAME:String = "SingleApplyListMediator";
		
		private var container:Sprite;
		private var main_mc:MovieClip;
		
		private var aCurList:Array = [];
		private var currentPage:int = 1;
		private var totalPage:int = 1;
		
		private var getOutTimer:Timer = new Timer(200, 1);	//物品取出计时器
		private var reqestTimer:Timer;									//请求数据计时器
		private var aData:Array = new Array();
		private var allMembers:Array = [];								//所有的申请成员
		
		private var openState:Boolean = false;						//是否打开状态
		
		private var isRequest:Boolean = false;
		private var yellowFrame:Shape;
		
		public function SingleApplyListMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CLICK_MEMBER_PAGE_COME,
							NewUnityCommonData.CLEAR_MEMBER_PAGE_GO,
							NewUnityCommonData.UPDATA_UNITY_APPLY_LIST_DATA
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.CLICK_MEMBER_PAGE_COME:
					if ( NewUnityCommonData.currentMemPage == 2 )
					{
						openState = true;
						showMe();
					}
				break;
				case NewUnityCommonData.CLEAR_MEMBER_PAGE_GO:
					if ( notification.getBody() == 2 )
					{
						openState = false;
						clearMe();
					}
				break;
				case NewUnityCommonData.UPDATA_UNITY_APPLY_LIST_DATA:
//					if ( openState )
//					{
//						var obj:Object = notification.getBody();
//						this.totalPage = obj.totalPage;
//						this.aCurList = obj.dataList;
//						aData[ currentPage - 1 ] = aCurList.concat();
//						updataMe();
//					}
					isRequest = true;
					analyData( notification.getBody() as Array );
				break;
			}
		}
		
		//分析数据
		private function analyData( arr:Array ):void
		{
			this.totalPage = Math.ceil( arr.length/14 );		//总页码
			if ( totalPage==0 )
			{
				totalPage = 1; 
				clearContainer();
				return;
			} 
			for ( var i:uint=0; i<totalPage; i++ )
			{
				this.allMembers[ i ] = arr.slice( i*14,i*14+14 );
			}
			updataMe();
		}
		
		private function showMe():void
		{
			if ( !container )
			{
				initView();
			}
			
			currentPage = 1;
			checkRequest();
//			initTimer();
			
			( main_mc.left_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goRight );
		}
		
		private function initView():void
		{
			main_mc = this.viewComponent as MovieClip;
			
			container = new Sprite();
			container.x = 10;
			container.y = 79;
			main_mc.addChild( container );
			( main_mc.page_txt as TextField ).mouseEnabled = false;
			yellowFrame = UIUtils.createFrame( 581,22 );
		}
		
		private function initTimer():void
		{
			if ( !reqestTimer )
			{
				reqestTimer = new Timer( 1800 * 1000,1 );
//				reqestTimer = new Timer( 1000,1 );
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
			if ( openState )
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
			if ( !isRequest )
			{
				NewUnityCommonData.allApplyMemberArr = [];
				RequestUnity.send( 303,0 );
//				isRequest = true;
			}
			else
			{
//				updataMe();
				analyData( NewUnityCommonData.allApplyMemberArr );
			}
		}
		
		private function updataMe():void
		{
			if ( openState )
			{
				clearContainer();
				createCells();
				showBottomInfo();
			}
		}
		
		private function clearContainer():void
		{
			var des:*;
			while ( container.numChildren>0 )
			{
				des = container.removeChildAt( 0 );
				if ( des is IUnityCell )
				{
					( des as IUnityCell ).gc();
				}
				des = null;
			}
		}
		
		private function createCells():void
		{
			this.aCurList = allMembers[ currentPage-1 ];
			if ( !aCurList ) return;
			var cell:SingleApplyListCell;
			for ( var i:uint=0; i<aCurList.length; i++ )
			{
				cell = new SingleApplyListCell( aCurList[ i ] );
				cell.init();
				cell.y = 23*i;
				cell.clickMe = clickCell;
				container.addChild( cell );	
			}
		}
		
		private function clickCell( cell:SingleApplyListCell ):void
		{
			cell.addChild( this.yellowFrame );
			if ( !facade.hasMediator( QuickSelectMediator.NAME ) )
			{
				facade.registerMediator( new QuickSelectMediator() );
			}
			sendNotification( ChatEvents.SHOWQUICKOPERATOR,cell.unityVo.name );
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if ( currentPage > 1 )
			{
				currentPage --;
				showBottomInfo();
//				clearContainer();
				checkRequest();
			}
		}
		
		private function goRight(evt:MouseEvent):void
		{
			if ( currentPage < totalPage )
			{
				if ( !startTimer() ) return;
				currentPage ++;
				showBottomInfo();
//				clearContainer();
				checkRequest();
			}
		}
		
		private function showBottomInfo():void
		{
			( main_mc.page_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + currentPage + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];     //第         页
//			( main_mc.page_txt as TextField ).mouseEnabled = false;
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
		
		private function clearMe():void
		{
			clearContainer();
			( main_mc.left_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,goRight );
		}
		
	}
}