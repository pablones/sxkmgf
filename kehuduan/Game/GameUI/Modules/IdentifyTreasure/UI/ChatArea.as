package GameUI.Modules.IdentifyTreasure.UI
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Chat.Mediator.QuickSelectMediator;
	import GameUI.Modules.Chat.UI.ChatCellEvent;
	import GameUI.Modules.Chat.UI.ChatCellText;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.UIScrollPane;
	
	import Net.ActionProcessor.ItemInfo;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	
	import org.puremvc.as3.multicore.patterns.facade.Facade;
	
	public class ChatArea extends Sprite
	{
		private var container:Sprite;
		private var scrollPanel:UIScrollPane;
		
		private var areaWidth:Number;
		private var areaHeight:Number;
		
		private var isBottom:Boolean = true;
		
		public function ChatArea( _width:Number,_height:Number )
		{
			areaWidth = _width;
			areaHeight = _height;
			initContainer();
			initScroll();
		}
		
		private function initContainer():void
		{
			container = new Sprite();
			container.graphics.clear();
			container.graphics.beginFill( 0xff0000,0 );
			container.graphics.drawRect( 0,0,areaWidth - 26,areaHeight - 20 );
			container.graphics.endFill();
			container.mouseEnabled = false;
		}
		
		private function initScroll():void
		{
			scrollPanel = new UIScrollPane( container );
			scrollPanel.width = areaWidth;
			scrollPanel.height = areaHeight;
			scrollPanel.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
//			scrollPanel.setScrollPos();
			scrollPanel.refresh();
			scrollPanel.scrollBottom();
			scrollPanel.refresh();
			this.addChild( scrollPanel );
		}

		//显示
		public function show( info:String ):void
		{
			isBottom = scrollPanel.isBottom();
			clearFirstChild();
			var treaText:TreaTextCell = new TreaTextCell( info );
			treaText.x = 2;
			container.addChild( treaText );
			showElements();
			scrollPanel.refresh();
			if ( isBottom ) 
			{	
				scrollPanel.scrollBottom();
				scrollPanel.refresh();
			}
		}
		
		private function clearFirstChild():void
		{
			var desChatCell:TreaTextCell;
			if(container.numChildren > 50) 
			{
				desChatCell = container.getChildAt( 0 ) as TreaTextCell;
				container.removeChildAt(0);
				desChatCell.gc();
				desChatCell = null;
			}
		}
		
		private function showElements():void 
		{
			var ypos:Number=1;
			for (var i:int = 0; i < container.numChildren; i++) 
			{
				var child:DisplayObject = container.getChildAt(i);
				child.y = ypos;
				ypos += child.height;
			}
		}
		
	}
}