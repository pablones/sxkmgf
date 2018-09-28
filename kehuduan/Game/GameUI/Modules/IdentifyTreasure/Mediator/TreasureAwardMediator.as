package GameUI.Modules.IdentifyTreasure.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Modules.IdentifyTreasure.UI.StaticGridGroup;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class TreasureAwardMediator extends Mediator
	{
		public static const NAME:String = "TreasureAwardMediator";
		private var main_mc:MovieClip;
		private var panelBase:PanelBase;
		private var gridGroup:StaticGridGroup;
		
		private var aCurData:Array = [];
		private var currentPage:uint = 1;
		private var totalPage:uint = 1;
		
		public function TreasureAwardMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							TreasureData.SHOW_TREA_AWARD_PANEL,
							TreasureData.CLOSE_TREA_AWARD_PANEL
						]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case TreasureData.SHOW_TREA_AWARD_PANEL:
					showUI( notification.getBody() );
				break;
				case TreasureData.CLOSE_TREA_AWARD_PANEL:
					closeUI( null );
				break;
			}
		}
		
		public function initialize():void
		{
			main_mc = this.viewComponent as MovieClip;
			if ( !panelBase )
			{
				panelBase = new PanelBase( main_mc, main_mc.width+8, main_mc.height+15);
				panelBase.x = UIConstData.DefaultPos1.x+110;
				panelBase.y = UIConstData.DefaultPos1.y+120;
				panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_ide_med_tre_ini" ]);//"恭喜你获得了"	
			}
			
			gridGroup = new StaticGridGroup( TreasureData.BlackRectGrid,4,8 );
			gridGroup.colDis = 0;
			gridGroup.rowDis = 0;
			gridGroup.placeGrids();
			gridGroup.x = 5;
			gridGroup.y = 5;
//			gridGroup.x = 0;
//			gridGroup.y = 0;
			main_mc.addChild( gridGroup );
		}
		
		private function showUI( obj:Object ):void
		{
			currentPage = 1;
//			gridGroup.dataGrid = obj.aAward;
			aCurData = obj.aAward;
			trace ( aCurData.length );
			panelBase.addEventListener( Event.CLOSE,closeUI );
			GameCommonData.GameInstance.GameUI.addChild( panelBase );
			
			totalPage = Math.ceil( aCurData.length/32 );
			
			showCurrentPage();
			
			//翻页按钮
			( main_mc.left_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goLeft );
			( main_mc.right_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,goRight );
		}
		
		private function closeUI( evt:Event ):void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeUI );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
				
				( main_mc.left_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goLeft );
				( main_mc.right_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,goRight );
			}	
		}
		
		//翻页
		private function goLeft( evt:MouseEvent ):void
		{
			if ( currentPage > 1 )
			{
				currentPage --;
				showCurrentPage();
			}
		}
		
		private function goRight( evt:MouseEvent ):void
		{
			if ( currentPage < totalPage )
			{
				currentPage ++;
				showCurrentPage();
			}
		}
		
		//显示当前页
		private function showCurrentPage():void
		{
			( main_mc.page_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ]+currentPage+"/"+totalPage+GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第"		"页"
			
			var startIndex:uint = ( currentPage-1 )*32;
			var endIndex:uint;
			if ( aCurData.length>currentPage*32 )
			{
				endIndex = currentPage*32;
			}
			else
			{
				endIndex = aCurData.length;
			}
			gridGroup.dataGrid = aCurData.slice( startIndex,endIndex );
		}
		
	}
}