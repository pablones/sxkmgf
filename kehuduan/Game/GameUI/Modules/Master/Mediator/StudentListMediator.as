package GameUI.Modules.Master.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.MasterResource;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Master.View.StudentListCell;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StudentListMediator extends Mediator
	{
		public static const NAME:String = "StudentListMediator";
		
		private var main_mc:MovieClip;
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase;
		private var masterRes:MasterResource;
		private var yellowFrame:Shape;
		private var cellContainer:Sprite = new Sprite();
		private var getOutTimer:Timer = new Timer(1000, 1);	//物品取出计时器
		private var aStudents:Array = [];
		
				
		public function StudentListMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							MasterData.CLICK_STUDENT_NPC,
							MasterData.STUDENT_RES_LOAD_COM,
							MasterData.REC_MY_STUDENT_LIST,
							MasterData.DELETE_MY_OWN_STUDENT
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MasterData.CLICK_STUDENT_NPC:
					clickGrilHandler();
				break;
				
				case MasterData.STUDENT_RES_LOAD_COM:
					var resObj:Object = notification.getBody();
					var resMC:MovieClip = resObj.resMC as MovieClip;
					var resClass:Class = resObj.resClass as Class;
					var resCom:int = resObj.resCom as int;
					resIsDone( resMC,resClass,resCom );
				break;
				
				case MasterData.REC_MY_STUDENT_LIST:
					aStudents = notification.getBody() as Array;
					if ( MasterData.masterResIsDone )
					{
						openPanel();
//						showList();
					}
					else
					{
						clickGrilHandler();
					}
				break;
				
				case MasterData.DELETE_MY_OWN_STUDENT:
					if ( dataProxy && dataProxy.studentListPaneIsOpen )
					{
						delStudent( notification.getBody() as Array );
					}
				break;
			}
		}
		
		private function clickGrilHandler():void
		{
			if ( !MasterData.masterResIsDone )
			{
				masterRes = new MasterResource( 2 );
			}
			else 
			{
				openPanel();
			}
		}
		
		private function openPanel():void
		{
			if ( !dataProxy.studentListPaneIsOpen )
			{
				GameCommonData.GameInstance.GameUI.addChild( panelBase );
				dataProxy.studentListPaneIsOpen = true;
			}
			showList();
		}
		
		//资源加载完成  初始化一些东西
		private function resIsDone( _main_mc:MovieClip,_CellClass:Class,_com:int ):void
		{
			main_mc = _main_mc;
//			CellClass = _CellClass; 
			MasterData.StudentCellClass = _CellClass; 
			MasterData.masterResIsDone = true;
			
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			panelBase = new PanelBase( main_mc, main_mc.width+8, main_mc.height+12 );
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_mas_med_stu_res_1" ]);         //徒弟列表
			
			initUI();
			
			if ( _com == 2 )
			{
				openPanel();
			}
		}
		
		private function initUI():void
		{
			createFrame( 548,22 );
			cellContainer.x = 8;
			cellContainer.y = 60;
			main_mc.addChild( cellContainer );
		}
		
		private function showList():void
		{
			clearContainer();
			for ( var i:uint=0; i<aStudents.length; i++ )
			{
				var cell:StudentListCell = new StudentListCell( aStudents[i] );
				cell.y = i * 23;
				cell.clickFun = clickCellHandler;
				cellContainer.addChild( cell );
			}
		}
		
		//请求数据
		private function checkRequest():void
		{
			if ( startTimer() )
			{
				RequestTutor.requestData( 23,0 );	
			}
		}
		
		//删除徒弟
		private function delStudent( delArr:Array ):void
		{
			if ( !aStudents ) return;
			var index:int;
			for ( var i:uint=0; i<delArr.length; i++ )
			{
				var delId:int = delArr[i].id;
				for ( var j:uint=0; j<aStudents.length; j++ )
				{
					if ( delId == aStudents[j].id )
					{
						aStudents.splice( j,1 );
					}
				}
			}
			showList();
		}
		
		private function clickCellHandler( cell:StudentListCell ):void
		{
			cell.addChild( yellowFrame );
			yellowFrame.x = 0;
			yellowFrame.y = 1;
		}
		
		private function panelCloseHandler( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
			dataProxy.studentListPaneIsOpen = false;
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
		
		private function clearContainer():void
		{
			var des:*;
			while ( cellContainer.numChildren>0 )
			{
				des = cellContainer.removeChildAt( 0 );
				if ( des is StudentListCell )
				{
					( des as StudentListCell ).gc();
				}
				des = null;
			}
		}
		
		//操作延时器
		private function startTimer():Boolean
		{
			if(getOutTimer.running) 
			{
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