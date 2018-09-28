package GameUI.Modules.IdentifyTreasure.UI
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.TextEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	import org.puremvc.as3.multicore.patterns.facade.Facade;

	public class TreaTextCell extends Sprite
	{
		private var str:String;
		private var color:uint;
		private var txt:TextField;
		private var format:TextFormat;
		
		public function TreaTextCell( _str:String,_color:uint=0xffff99 )
		{
			super();
			str = _str;
			color = _color;
			initUI();
		}
		
		private function initUI():void
		{
			format = new TextFormat();
			format.size = 12;
			format.font = "宋体";
			format.leading = 4;
//			format.color = color;
			
			txt = new TextField();
			txt.width = 160;
			txt.wordWrap = true;
			txt.autoSize = TextFieldAutoSize.LEFT;
			txt.multiline = true;
			txt.selectable = false;
			txt.htmlText = str;
			txt.filters = Font.Stroke( 0x000000 );
			txt.setTextFormat( format );
			addChild( txt );
			
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			
		}
		
		private function addStageHandler( evt:Event ):void
		{
//			removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			
			txt.addEventListener( TextEvent.LINK,onLink );
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			txt.removeEventListener( TextEvent.LINK,onLink );
			removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function onLink( evt:TextEvent ):void
		{
			var arr:Array = evt.text.split( "_" );
			var facade:Facade = UIFacade.GetInstance( UIFacade.FACADEKEY );
			//暂时没有判断物品的type 范围
			if ( arr[0].toString() == "type" )
			{
				var type:int = int( arr[1] );
				var dataItem:Object = null;
				dataItem = UIConstData.getItem( type );
				dataItem.id = undefined; 
				dataItem.isBind = 0;
				facade.sendNotification( EventList.SHOWITEMTOOLPANEL, { type:type, data:dataItem } );
			}
			else if ( arr[0].toString() == "name" )
			{
				var quickSelectMediator:QuickSelectMediator = new QuickSelectMediator();
			 	facade.registerMediator( quickSelectMediator );
				facade.sendNotification( ChatEvents.SHOWQUICKOPERATOR, arr[1] );
			}
		}
		
		public function gc():void
		{
			removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			format = null;
			txt = null;
		}
		
	}
}