package GameUI.Modules.Master.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.MasterResource;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.AutoPanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MasStuMainMediator extends Mediator
	{
		public static const NAME:String = "MasStuMainMediator";
		
		private var dataProxy:DataProxy;
//		private var panelBase:PanelBase;
		private var panelBase:AutoPanelBase;
		private var masterRes:MasterResource;
		private var main_mc:MovieClip;
		
		private var curPage:uint = 0;				//当前页签
		private var maxPage:uint = 4;
		
		private var masListMed:MasterMediator;
		private var masStuMed:MasterAndStudentMediator;
		private var stuAppLisMed:StudentApplyListMediator;
		private var graduateStudentMediator:GraduateStudentMediator;
		
		private var hasGraduateStudent:Boolean = false;
		
		public function MasStuMainMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [MasterData.CLICK_MASTER_NPC,
						MasterData.MASTER_RES_LOAD_COM,
						MasterData.GO_MS_MC_PAGE_CUR,
						MasterData.CLOSE_MASTER_VIEW,
						MasterData.REC_CHECK_HAS_GRADUATE
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MasterData.CLICK_MASTER_NPC:
					clickGirlHandler();
					break;

				case MasterData.MASTER_RES_LOAD_COM:
					var resObj:Object = notification.getBody();
					var resMC:MovieClip = resObj.resMC as MovieClip;
					var resClass:Class = resObj.resClass as Class;
					var resCom:int = resObj.resCom as int;
					resIsDone( resMC,resClass,resCom );
				break;
				
				case MasterData.GO_MS_MC_PAGE_CUR:
					curPage = notification.getBody() as int;
					setPageSign();
				break;
				
				case MasterData.CLOSE_MASTER_VIEW:
					panelCloseHandler( null );
				break;
				
				case MasterData.REC_CHECK_HAS_GRADUATE:
					if ( int( notification.getBody() ) == 1 )
					{
						hasGraduateStudent = true;
						setPageFourVis( true );
					}
				break;
			}
		}
		
		private function clickGirlHandler():void
		{
			if ( !MasterData.masterResIsDone )
			{
				masterRes = new MasterResource( 1 );
			}
			else 
			{
				openPanel();
			}
		}
		
		//资源加载完成  初始化一些东西
		private function resIsDone( _main_mc:MovieClip,_CellClass:Class,_com:int ):void
		{
			main_mc = _main_mc;
//			CellClass = _CellClass; 
			MasterData.MasterCellClass = _CellClass; 
			MasterData.masterResIsDone = true;
			
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			panelBase = new AutoPanelBase( main_mc, main_mc.width+8, main_mc.height+14 );
			
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_mas_med_mass_res_1" ]);        //师  徒
			
			initUI();
			
			if ( _com == 1 )
			{
				openPanel();
			}
		}
		
		private function initUI():void
		{			
			masListMed = facade.retrieveMediator( MasterMediator.NAME ) as MasterMediator;
			masListMed.setViewComponent( main_mc );
			
			masStuMed = new MasterAndStudentMediator();
			masStuMed.setViewComponent( main_mc );
			facade.registerMediator( masStuMed );
			
			stuAppLisMed = new StudentApplyListMediator();
			stuAppLisMed.setViewComponent( main_mc );
			facade.registerMediator( stuAppLisMed );
			
			graduateStudentMediator = new GraduateStudentMediator();
			graduateStudentMediator.setViewComponent( main_mc );
			facade.registerMediator( graduateStudentMediator );
			
			main_mc.mcPage3_txt.mouseEnabled = false;
			main_mc.mcPage2_txt.mouseEnabled = false;
			
			if ( GameCommonData.Player.Role.Level < 35 )
			{
				setPageThreeVis( false );
			}
			else
			{
				setPageThreeVis( true );
			}
			
			sendNotification( MasterData.MASTER_STU_UI_INITVIEW );			//通知界面初始化
			
			setPageSign();
		}
		
		//设置页签
		private function setPageSign():void
		{
			for ( var i:uint=0; i<maxPage; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).gotoAndStop( 2 );
			}
			( main_mc[ "mcPage_"+curPage ] as MovieClip ).gotoAndStop( 1 );
			main_mc.gotoAndStop( curPage+1 );
			sendNotification( MasterData.START_SHOW_MAS_PAGE,{ curPage:curPage } );
			if ( curPage == 1 )
			{
				panelBase.backHeight = main_mc.height + 14;
			}
			else
			{
				panelBase.backHeight = main_mc.height + 16;
			}
		}
		
		private function openPanel():void
		{
			if ( !dataProxy.masterPanelIsOpen )
			{
				setPageFourVis( false );
				for ( var i:uint=0; i<maxPage; i++ )
				{
					( main_mc[ "mcPage_"+i ] as MovieClip ).addEventListener( MouseEvent.CLICK,clickPage );
					( main_mc[ "mcPage_"+i ] as MovieClip ).buttonMode = true;
				}
				sendNotification( MasterData.START_SHOW_MAS_PAGE,{ curPage:curPage } );
				panelBase.addEventListener( Event.CLOSE, panelCloseHandler );
				if( GameCommonData.fullScreen == 2 )
				{
					panelBase.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - panelBase.width)/2;
					panelBase.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - panelBase.height)/2;
				}else{
					panelBase.x = UIConstData.DefaultPos1.x;
					panelBase.y = UIConstData.DefaultPos1.y - 20;
				}
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				dataProxy.masterPanelIsOpen = true;
				checkRequest();
			}
			else
			{
				panelCloseHandler( null );
			}
		}
		
		//请求是否有出师的徒弟，如果有，就只请求一次
		private function checkRequest():void
		{
			if ( hasGraduateStudent )
			{
				setPageFourVis( true );
			}
			else
			{
				RequestTutor.requestData( 30,0 );
			}
		}
		
		private function clickPage( evt:MouseEvent ):void
		{
			var eNameArr:Array = evt.target.name.split( "_" );
			if ( curPage == eNameArr[ 1 ]  ) return;
			sendNotification( MasterData.CLEAR_MASTER_PANEL_PAGE,{ curPage:curPage } );
			curPage = int( eNameArr[ 1 ] );
			setPageSign();
		}
		
		private function panelCloseHandler( evt:Event ):void
		{
			for ( var i:uint=0; i<maxPage; i++ )
			{
				( main_mc[ "mcPage_"+i ] as MovieClip ).removeEventListener( MouseEvent.CLICK,clickPage );
			}
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase) )
			{
				panelBase.removeEventListener( Event.CLOSE, panelCloseHandler );
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				dataProxy.masterPanelIsOpen = false;
			}
			sendNotification( MasterData.CLEAR_MASTER_PANEL_PAGE,{ curPage:curPage } );
		}
		
		//设置第三个页签是否可见
		private function setPageThreeVis( isSee:Boolean ):void
		{
			main_mc.mcPage_2.visible = isSee;
			main_mc.mcPage2_txt.visible = isSee;
		}
		
		private function setPageFourVis( isSee:Boolean ):void
		{
			main_mc.mcPage_3.visible = isSee;
			main_mc.mcPage3_txt.visible = isSee;
		}
		
		public function get masterCurPage():int
		{
			return this.curPage;
		}
		
		public function set masterCurPage( value:int ):void
		{
			this.curPage = value;
		}
		
	}
}