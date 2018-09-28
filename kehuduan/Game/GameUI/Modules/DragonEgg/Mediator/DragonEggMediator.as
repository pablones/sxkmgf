package GameUI.Modules.DragonEgg.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.DragonEgg.Data.DragonEggData;
	import GameUI.Modules.DragonEgg.View.SingleDragonEggCell;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.BaseUI.AutoPanelBase;
	import GameUI.View.Components.countDown.HourMiniteText;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class DragonEggMediator extends Mediator
	{
		public static const NAME:String = "DragonEggMediator";
		private var panelBase:AutoPanelBase;
		private var main_mc:MovieClip;
		private var container:Sprite;
		
		private var openState:Boolean = false;
		private var aCurList:Array = [];
		
		private var lookDragonEggInfoMediator:LookDragonEggInfoMediator;
		private var lovelyHomeMediator:LovelyHomeMediator;
		
		private var leftTimeTxt:HourMiniteText;
		private var bornTimeTxt:HourMiniteText;
		
		private var leftTime:int;			//剩余时间秒数
		private var bornTime:int;
		private var eggId:int;
		
		private var petName:String;				//宠物名称
		
		public function DragonEggMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							DragonEggData.SHOW_DRAGONEGG_PANEL,
							DragonEggData.CLOSE_DRAGONEGG_PANEL,
							DragonEggData.UPDATA_DRAGONGEE_DATA
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			var obj:Object = notification.getBody();
			
			switch ( notification.getName() )
			{
				case DragonEggData.SHOW_DRAGONEGG_PANEL:
					aCurList = obj.aEggs;
					bornTime = obj.nextTime;
					eggId = obj.eggId;
					petName = obj.name;
					showMe();
				break;
				case DragonEggData.CLOSE_DRAGONEGG_PANEL:
					closeMe( null );
				break;
				case DragonEggData.UPDATA_DRAGONGEE_DATA:
					if ( openState && obj.eggId == this.eggId )
					{
						aCurList = obj.aEggs;
						bornTime = obj.nextTime;
						eggId = obj.eggId;
						petName = obj.name;
						updataMe();
					}
				break;
			}
		}
		
		public function get dataList():Array
		{
			return this.aCurList;
		}
		
		private function initView():void
		{
			main_mc = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByMovieClip( "DragonEggRes" );
			
			if ( !lookDragonEggInfoMediator )
			{
				lookDragonEggInfoMediator = new LookDragonEggInfoMediator();
				lovelyHomeMediator = new LovelyHomeMediator();
			}
			
			panelBase = new AutoPanelBase( main_mc,455,main_mc.height+12 );
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt( "小顽龙蛋" );
			
			container = new Sprite();
			
			container.x = 12;
			container.y = 126;
			
			main_mc.addChild( container );
			
			leftTimeTxt = new HourMiniteText();
			leftTimeTxt.x = 105;
			leftTimeTxt.y = 17;
//			leftTimeTxt.timeDoneHandler = eggOver;
			main_mc.addChild( leftTimeTxt );
			
			bornTimeTxt = new HourMiniteText();
			bornTimeTxt.x = 105;
			bornTimeTxt.y = 77;
			main_mc.addChild( bornTimeTxt );
		}
		
		private function showMe():void
		{
			if ( !main_mc )
			{
				initView();
			} 
		
			facade.registerMediator( lookDragonEggInfoMediator );
			facade.registerMediator( lovelyHomeMediator );
			
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			openState = true;
			
			panelBase.addEventListener( Event.CLOSE,closeMe );
			main_mc.lovelyHome_btn.addEventListener( MouseEvent.CLICK,goLovelyHome );
			
			updataMe();
//			test();
		}
		
		private function upDateTime():void
		{
			//计算寿命剩余时间
//			var item:Object = BagData.getItemById( eggId );
			var item:Object = IntroConst.ItemInfo[ eggId ];
			if ( !item ) return;
			var lastTimeStr:String = item.lifeTime.toString();
			
			var oldYear:int = int( lastTimeStr.slice( 0,2 ) ) + 2000;
			var oldMonth:int = int( lastTimeStr.slice( 2,4 ) ) - 1;
			var oldDay:int = int( lastTimeStr.slice( 4,6 ) );
			var oldHour:int = int( lastTimeStr.slice( 6,8 ) ); 
			var oldMinite:int = int( lastTimeStr.slice( 8,10 ) ); 
			var oldSecond:int = int( lastTimeStr.slice( 10,12 ) ); 
			
			var newDate:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth-1,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
			
			var oldDate:Date = new Date( oldYear,oldMonth,oldDay,oldHour,oldMinite,oldSecond );
			leftTime = ( oldDate.getTime() - ( newDate.getTime() ) )/1000;
			
			leftTimeTxt.start( leftTime,0xff0000 );
			bornTimeTxt.start( bornTime );
		}
		
		//爱心小槽
		private function goLovelyHome( evt:MouseEvent ):void
		{
			sendNotification( DragonEggData.SHOW_LOVELY_HOME,eggId );
		}
		
		private function updataMe():void
		{
			upDateTime();
			clearContainer();
			createCells();
		}
		
		private function clearContainer():void
		{
			var des:*;
			while ( container.numChildren>0 )
			{
				des = container.removeChildAt( 0 );
				if ( des is SingleDragonEggCell )
				{
					( des as SingleDragonEggCell ).gc();
				}
				des = null;
			}
		}
		
		private function createCells():void
		{
			var cell:SingleDragonEggCell;
			for ( var i:uint=0; i<aCurList.length; i++ )
			{
				cell = new SingleDragonEggCell( aCurList[ i ],eggId,petName );
				cell.y = i * 59;
				cell.init();
				container.addChild( cell );
			}
		}
		
		private function closeMe( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeMe );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				
				sendNotification( DragonEggData.CLOSE_LOOK_DRAGONEGG_INFO );
				sendNotification( DragonEggData.CLOSE_LOVELY_HOME );
				facade.removeMediator( LookDragonEggInfoMediator.NAME );
				facade.removeMediator( LovelyHomeMediator.NAME );
				openState = false;
				main_mc = null;
				panelBase = null;
			}
		}
		
	}
}