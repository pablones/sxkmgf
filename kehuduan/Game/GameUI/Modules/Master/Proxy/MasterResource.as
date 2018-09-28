package GameUI.Modules.Master.Proxy
{
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.LoadingView;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	import flash.net.URLRequest;
	
	public class MasterResource
	{
		private var loader:Loader;
		private var com:int;
		
		public function MasterResource( _com:int )
		{
			if ( MasterData.startLoadMaster ) return;
			MasterData.startLoadMaster = true;
			com = _com;
			loader = new Loader();
			loader.contentLoaderInfo.addEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE,onComplete );
			loader.load( new URLRequest ( GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/GreatMaster.swf" ) );
		}
		
		private function onProgress( evt:ProgressEvent ):void
		{
			LoadingView.getInstance().showLoading();
		}
		
		private function onComplete( evt:Event ):void
		{
			LoadingView.getInstance().removeLoading();
			var MainClass:Class = getClassObj( "NewMasterUI" );
			var _main_mc:MovieClip = new MainClass() as MovieClip;
			var _ClassCell:Class = getClassObj( "MasterCellUI" );
			
			MasterData.masResPro.studentApplyCellClass = getClassObj( "StudentApplyUI" );
			MasterData.masResPro.studentCellClass = getClassObj( "StudentListUI" );
			MasterData.masResPro.myMasterClass = getClassObj( "MyMasterInfo" );
			MasterData.masResPro.myStudentClass = getClassObj( "MyStudentInfo" );
			MasterData.masResPro.studentListCellClass = getClassObj( "StudentsListCellUI" );
			MasterData.masResPro.fiveButtonClass = getClassObj( "FiveButtonRes" );
			
			MasterData.masResPro.noMasterClass = getClassObj( "NoMasterRes" );
			MasterData.masResPro.noStudentOneClass = getClassObj( "NoStudentOne" );
			MasterData.masResPro.noStudentTwoClass = getClassObj( "NoStudentTwo" );
			MasterData.masResPro.noStudentThreeClass = getClassObj( "NoStudentThree" );
			MasterData.masResPro.graduateCellClass = getClassObj( "AlreadyGraduateCell" );
			MasterData.masResPro.graduateBottomUI = getClassObj( "GraduateBottomBtnUI" );

			var obj:Object = new Object();
			obj.resMC = _main_mc;
			obj.resClass = _ClassCell;
			obj.resCom = com;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( MasterData.MASTER_RES_LOAD_COM,obj );
			
			var StudentCellClass:Class = getClassObj( "StudentsListCellUI" );
			var StudentClass:Class = getClassObj( "StudentListUI" );
			var studentList_mc:MovieClip = new StudentClass() as MovieClip;
			var stuListObj:Object = new Object();
			stuListObj.resMC = studentList_mc;
			stuListObj.resClass = StudentCellClass;
			stuListObj.resCom = com;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( MasterData.STUDENT_RES_LOAD_COM,stuListObj );
			
			loader.contentLoaderInfo.removeEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.contentLoaderInfo.removeEventListener( Event.COMPLETE,onComplete );
			loader = null;
		}
		
		private function getClassObj( str:String ):Class
		{
			var classObj:Object;
			if ( loader.contentLoaderInfo.applicationDomain.hasDefinition( str ) )
			{
				classObj = loader.contentLoaderInfo.applicationDomain.getDefinition( str );
			}
			return classObj as Class;
		}

	}
}