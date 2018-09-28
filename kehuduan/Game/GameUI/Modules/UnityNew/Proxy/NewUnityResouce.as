package GameUI.Modules.UnityNew.Proxy
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.View.Components.LoadingView;
	
	import flash.display.Loader;
	import flash.events.Event;
	import flash.events.ProgressEvent;
	import flash.net.URLRequest;
	
	public class NewUnityResouce
	{
		public var resDoneHandler:Function;
		
		private var loader:Loader;
		
		public function NewUnityResouce()
		{
			loader = new Loader();
			loader.contentLoaderInfo.addEventListener( ProgressEvent.PROGRESS,onProgress );
			loader.contentLoaderInfo.addEventListener( Event.COMPLETE,onComplete );
			loader.load( new URLRequest ( GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewUnity.swf" ) );
		}
		
		private function onProgress( evt:ProgressEvent ):void
		{
			LoadingView.getInstance().showLoading();
		}
		
		private function onComplete( evt:Event ):void
		{
			LoadingView.getInstance().removeLoading();
			
			NewUnityCommonData.unityResIsDone = true;			
			NewUnityCommonData.newUnityResProvider.UnityInfoRes = getClassObj( "UnityInfoRes" );
//			NewUnityCommonData.newUnityResProvider.UnityListRes = getClassObj( "UnityListRes" );
			NewUnityCommonData.newUnityResProvider.UnityMainRes = getClassObj( "UnityMainRes" );
			NewUnityCommonData.newUnityResProvider.UnitySkillRes = getClassObj( "UnitySkillRes" );
			NewUnityCommonData.newUnityResProvider.MemberList = getClassObj( "MemberList" );
			NewUnityCommonData.newUnityResProvider.PleaseJoinMeRes = getClassObj( "UnityListRes" );
			NewUnityCommonData.newUnityResProvider.UnityFenTangRes = getClassObj( "UnityFenTangRes" );
			NewUnityCommonData.newUnityResProvider.LookUnityInfoRes = getClassObj( "LookUnityInfoRes" );
			NewUnityCommonData.newUnityResProvider.NewUnityOrderRes = getClassObj( "NewUnityOrderRes" );
			NewUnityCommonData.newUnityResProvider.NewUnityContributeRes = getClassObj( "UnityContributeRes" );
			NewUnityCommonData.newUnityResProvider.BossUnityOrderRes = getClassObj( "BossUnityOrderRes" ); 
			NewUnityCommonData.newUnityResProvider.DefendAttackRes = getClassObj( "DefendAttackRes" );
			
			NewUnityCommonData.newUnityResProvider.SingleMemberListCellClass = getClassObj( "UnityMemberListSingle" );
			NewUnityCommonData.newUnityResProvider.SingleContributeListCellClass = getClassObj( "UnityContributeListSingle" );
			NewUnityCommonData.newUnityResProvider.SingleApplyListCellClass = getClassObj( "UnityApplyListSingle" );
			NewUnityCommonData.newUnityResProvider.SingleJoinUnityCellClass = getClassObj( "ApplyUnitySingle" );
			NewUnityCommonData.newUnityResProvider.SingleHasUnityListCellClass = getClassObj( "HasUnityListSingle" );
			NewUnityCommonData.newUnityResProvider.SingleUnitySkillCellClass = getClassObj( "UnitySkillCellRes" );
			NewUnityCommonData.newUnityResProvider.NewUnityHelpRes = getClassObj( "NewUnityHelpRes" );
			
			if ( resDoneHandler != null ) resDoneHandler();

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