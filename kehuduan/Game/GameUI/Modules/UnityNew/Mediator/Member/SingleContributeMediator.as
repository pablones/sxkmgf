package GameUI.Modules.UnityNew.Mediator.Member
{
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Modules.UnityNew.View.IUnityCell;
	import GameUI.Modules.UnityNew.View.SingleContributeListCell;
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

	public class SingleContributeMediator extends Mediator
	{
		public static const NAME:String = "SingleContributeMediator";
		
		private var container:Sprite;
		private var main_mc:MovieClip;
		
		private var aCurList:Array = [];
		private var currentPage:int = 1;
		private var totalPage:int = 1;
		
		private var getOutTimer:Timer = new Timer(200, 1);	//物品取出计时器
		private var reqestTimer:Timer;									//请求数据计时器
		private var aData:Array = new Array();
		
		private var openState:Boolean = false;						//是否打开状态
		
		private var aDefaultMember:Array = [];				//默认排序
		private var aTotalConMem:Array = [];					//总贡献排序
		private var aMoneyConMem:Array = [];				//资金贡献排序
		private var aJianseConMem:Array = [];				//建设贡献排序
		private var yellowFrame:Shape;
		
		private const defaultSort:String = "defaultSort";
		private const totalConSort:String = "totalConSort";
		private const moneyConSort:String = "moneyConSort";
		private const jianseConSort:String = "jianseConSort";
		
		public static var isRequestState:Boolean = true;					//是否请求数据状态
		
		private var currentSort:String = defaultSort;
		
		public function SingleContributeMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CLICK_MEMBER_PAGE_COME,
							NewUnityCommonData.CLEAR_MEMBER_PAGE_GO,
							NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.CLICK_MEMBER_PAGE_COME:
					if ( NewUnityCommonData.currentMemPage == 1 )
					{
						openState = true;
						showMe();
					}
				break;
				case NewUnityCommonData.CLEAR_MEMBER_PAGE_GO:
					if ( notification.getBody() == 1 )
					{
						openState = false;
						clearMe();
					}
				break;

				case NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS:
					var allMemList:Array = ( notification.getBody() as Array ).concat();
					analyData( allMemList );
				break;

			}
		}
		
		//分析数据
		private function analyData( arr:Array ):void
		{
			totalPage = Math.ceil( arr.length/14 );
			arr = arr.sortOn( "unityJob",Array.NUMERIC ).reverse();
			for ( var i:uint=0; i<totalPage; i++ )
			{
				aDefaultMember[ i ] = arr.slice( i*14,i*14+14 );
			}
			arr = arr.sortOn( "totalContribute",Array.NUMERIC ).reverse();
			for ( i=0; i<totalPage; i++ )
			{
				aTotalConMem[ i ] = arr.slice( i*14,i*14+14 );
			}
			arr = arr.sortOn( "moneyContribute",Array.NUMERIC ).reverse();
			for ( i=0; i<totalPage; i++ )
			{
				aMoneyConMem[ i ] = arr.slice( i*14,i*14+14 );
			}
			arr = arr.sortOn( "jianseContribute",Array.NUMERIC ).reverse();
			for ( i=0; i<totalPage; i++ )
			{
				aJianseConMem[ i ] = arr.slice( i*14,i*14+14 );
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
			currentSort = defaultSort;
			checkRequest();
			initTimer();
			
			( main_mc.left_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goRight );
			
			( main_mc.totalConSort as SimpleButton ).addEventListener( MouseEvent.CLICK,changeSort );
			( main_mc.jianseConSort as SimpleButton ).addEventListener( MouseEvent.CLICK,changeSort );
			( main_mc.moneyConSort as SimpleButton ).addEventListener( MouseEvent.CLICK,changeSort );
		}
		
		private function changeSort( evt:MouseEvent ):void
		{
			if ( currentSort == evt.target.name ) return;
			switch( evt.target.name )
			{
				case "totalConSort":
					currentSort = totalConSort;
				break;
				case "jianseConSort":
					currentSort = jianseConSort;
				break;
				case "moneyConSort":
					currentSort = moneyConSort;
				break;
			}
			updataMe();
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
			NewUnityCommonData.allUnityMemberArr = [];
			isRequestState = true;
			
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
			if ( isRequestState )
			{
				NewUnityCommonData.allUnityMemberArr = [];
				RequestUnity.send( 301,currentPage );
				isRequestState = false;
			}
			else
			{
				updataMe();
//				analyData( NewUnityCommonData.allUnityMemberArr );
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
			switch( currentSort )
			{
				case defaultSort:
					aCurList = aDefaultMember[ currentPage-1 ];
				break;
				case totalConSort:
					aCurList = aTotalConMem[ currentPage-1 ];
				break;
				case moneyConSort:
					aCurList = aMoneyConMem[ currentPage-1 ];
				break;
				case jianseConSort:
					aCurList = aJianseConMem[ currentPage-1 ];
				break;
			}
			
			if ( aCurList == null )
			{
				return;
			}
			
			var cell:SingleContributeListCell;
			for ( var i:uint=0; i<aCurList.length; i++ )
			{
				cell = new SingleContributeListCell( aCurList[ i ] );
				cell.init();
				cell.clickMe = clickCell;
				cell.y = 23*i;
				container.addChild( cell );	
			}
		}
		
		private function clickCell( cell:SingleContributeListCell ):void
		{
			cell.addChild( this.yellowFrame );
			if ( !facade.hasMediator( QuickSelectMediator.NAME ) )
			{
				facade.registerMediator( new QuickSelectMediator() );
			}
			if ( cell.unityVo.name != GameCommonData.Player.Role.Name )
			{
				sendNotification( NewUnityCommonData.SHOW_SINGLE_MEMBLER_SKIRT,cell.unityVo );
			}
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if ( currentPage > 1 )
			{
				currentPage --;
				showBottomInfo();
//				clearContainer();
//				checkRequest();
				updataMe();
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
//				checkRequest();
				updataMe();
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
			
			( main_mc.totalConSort as SimpleButton ).removeEventListener( MouseEvent.CLICK,changeSort );
			( main_mc.jianseConSort as SimpleButton ).removeEventListener( MouseEvent.CLICK,changeSort );
			( main_mc.moneyConSort as SimpleButton ).removeEventListener( MouseEvent.CLICK,changeSort );
		}
		
	}
}