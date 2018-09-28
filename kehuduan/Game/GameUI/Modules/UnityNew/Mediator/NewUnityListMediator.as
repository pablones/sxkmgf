package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Modules.UnityNew.View.IUnityCell;
	import GameUI.Modules.UnityNew.View.SingleHasUnityListCell;
	import GameUI.UIUtils;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnityListMediator extends Mediator
	{
		public static const NAME:String = "NewUnityListMediator";
		
		private var parentContainer:Sprite;
		private var main_mc:MovieClip;
		
		private var allUnitys:Array = [];
		private var aCurList:Array = [];
		
		private var currentPage:int = 1;
		private var totalPage:int = 1;
		private var openState:Boolean = false;
		private var container:Sprite;
		private var getOutTimer:Timer = new Timer(200, 1);	//物品取出计时器
		private var yellowFrame:Shape;
		
		private var seekUnity:String;
		private 	var seekBoss:String;
		
		public function NewUnityListMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CHANG_NEW_UNITY_PAGE,
							NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL,
							NewUnityCommonData.RECEIVE_ALL_UNITYS_LIST
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
					if ( NewUnityCommonData.currentPage == 3 )
					{
						openState = true;
						openMe();
					}
				break;
				case NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL:
					if ( notification.getBody() == 3 )
					{
						clearMe();
					}
				break;
				case NewUnityCommonData.RECEIVE_ALL_UNITYS_LIST:
					analyData( notification.getBody() as Array );
				break;
			}
		}
		
		//分析数据
		private function analyData( arr:Array ):void
		{
			this.totalPage = Math.ceil( arr.length/14 );		//总页码
			if ( this.totalPage == 0 )
			{
				totalPage = 1;
				return;
			}
			for ( var i:uint=0; i<totalPage; i++ )
			{
				this.allUnitys[ i ] = arr.slice( i*14,i*14+14 );
			}
			this.aCurList = allUnitys[ currentPage-1 ];
			updataMe();
		}
		
		private function initView():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.PleaseJoinMeRes ) as MovieClip;
			( main_mc.page_txt as TextField ).mouseEnabled = false;
			( main_mc.create_btn as SimpleButton ).visible = false;
			( main_mc.createBtn_txt as TextField ).mouseEnabled = false;
			( main_mc.createBtn_txt as TextField ).visible = false;
			main_mc.y = 17;9
			
			container = new Sprite();
			container.x = 10;
//			container.y = 62;
			container.y = 62;
			main_mc.addChild( container );
			yellowFrame = UIUtils.createFrame( 581,22 );
		}
		
		private function openMe():void
		{
			if ( !main_mc )
			{
				initView();
			}
			parentContainer.addChildAt( main_mc,0 );
			
			checkRequest();
			( main_mc.left_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goRight );
			( main_mc.seek_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,seekUnityHandler );
			
			
			main_mc.unityName_txt.addEventListener( Event.CHANGE,textHandler );
			main_mc.unityBoss_txt.addEventListener( Event.CHANGE,textHandler );
			UIUtils.addFocusLis( main_mc.unityName_txt );
			UIUtils.addFocusLis( main_mc.unityBoss_txt );
			UIConstData.FocusIsUsing = true;
		}
		
		private function textHandler( evt:Event ):void
		{
			var targetTxt:TextField = evt.target as TextField;

			if ( evt.target.name == "unityName_txt" )
			{
				this.seekUnity = targetTxt.text;
			}
			else if ( evt.target.name == "unityBoss_txt" )
			{
				this.seekBoss = targetTxt.text;
			}
			if ( this.seekUnity=="" && this.seekBoss=="" )
			{
				this.aCurList = allUnitys[ currentPage-1 ];
				updataMe();
			}
		}
		
		private function checkRequest():void
		{
			NewUnityCommonData.allUnityArr = [];	
			RequestUnity.send( 304,0 );
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
			var cell:SingleHasUnityListCell;
			for ( var i:uint=0; i<aCurList.length; i++ )
			{
				cell = new SingleHasUnityListCell( aCurList[ i ] );
				cell.init();
				cell.clickMe = clickCell;
				cell.y = 23*i;
				container.addChild( cell );	
			}
		}
		
		private function clickCell( cell:SingleHasUnityListCell ):void
		{
			cell.addChild( yellowFrame );
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if ( currentPage > 1 )
			{
				currentPage --;
				showBottomInfo();
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
				checkRequest();
			}
		}
		
		//查找帮派
		private function seekUnityHandler( evt:MouseEvent ):void
		{
			if ( main_mc.unityName_txt.text=="" && main_mc.unityBoss_txt.text=="" )
			{
				sendNotification( HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_unityN_med_joi_see_1" ], color:0xffff00 } );    //请先输入帮派或帮主名称
				return;
			}
			
			seekUnity = main_mc.unityName_txt.text;
			seekBoss = main_mc.unityBoss_txt.text;
			this.aCurList = [];
			if ( allUnitys.length>0 )
			{
				for ( var i:uint=0; i<allUnitys.length; i++ )
				{
					for ( var j:uint=0; j<allUnitys[i].length; j++ )
					{
						if ( seekUnity.length>0 )
						{
							if ( allUnitys[i][j].name.indexOf( seekUnity ) > -1 )
							{
								aCurList.push( allUnitys[i][j] );
							}
						}
						if ( seekBoss.length>0 )
						{
							if ( allUnitys[i][j].boss.indexOf( seekBoss ) > -1 )
							{
								aCurList.push( allUnitys[i][j] );
							}
						}
					}
				}
			}
			updataMe();
		}
		
		private function showBottomInfo():void
		{
			( main_mc.page_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + currentPage + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];     //第         页	
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
			if ( main_mc && parentContainer.contains( main_mc ) )
			{
				( main_mc.left_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goLeft );
				( main_mc.right_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goRight );
				( main_mc.seek_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,seekUnityHandler );
				
				main_mc.unityName_txt.removeEventListener( Event.CHANGE,textHandler );
				main_mc.unityBoss_txt.removeEventListener( Event.CHANGE,textHandler );
				
				parentContainer.removeChild( main_mc );
			}
			openState = false;
			UIConstData.FocusIsUsing = false;
		}
		
	}
}