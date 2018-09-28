package GameUI.Modules.Friend.view.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.UIConfigData;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SelectFontColorMediator extends Mediator
	{
		public static const NAME:String = "SelectColorMediator";
		private var defaultPos:Point = new Point(320, 470);
		private var maxColors:int = 20;
		private var colors:Array = [
			0xff8282, 0xff6ce2, 0xd0a9ff, 0x9a90ff, 0x6da8f8, 0x37e8c0,	
			0x4ed962, 0xa0d73f, 0xdcc4ff, 0xfda54e, 0xfe3333, 0xe121bb,
			0xa257ff, 0x7062fe, 0x2682ff, 0x02a37e, 0x008a13, 0x55ac01,
			0xe1de00, 0xad5803
		]
		
		public function SelectFontColorMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		
		private function get colorSelect():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				FriendCommandList.SHOW_FONT_COLOR,
				FriendCommandList.HIDE_FONT_COLOR
			];
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case FriendCommandList.SHOW_FONT_COLOR:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SELECTCOLOR});
					this.colorSelect.mouseEnabled=false;
					if(notification.getBody())
					{
						colorSelect.x = notification.getBody().x;
						colorSelect.y = notification.getBody().y;
					}
					else
					{
						colorSelect.x = defaultPos.x;
						colorSelect.y = defaultPos.y;
					}
					GameCommonData.GameInstance.GameUI.addChild(colorSelect);
					ChatData.ColorIsOpen = true;
					initView();
				break;
				case FriendCommandList.HIDE_FONT_COLOR:
					closeHandler();
				break
			}
		}
		
		private function initView():void
		{
			for(var i:int = 0; i<maxColors; i++)
			{
				colorSelect["btn_"+i].addEventListener(MouseEvent.CLICK, selectColor);
			}
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_DOWN, removeList);
		}
		
		private function selectColor(event:MouseEvent):void
		{
			var index:int = event.currentTarget.name.split("_")[1];
			var curColor:uint = colors[index];
			facade.sendNotification(FriendCommandList.SELECTED_FONT_COLOR, curColor);
			closeHandler();
		}
		
		private function removeList(event:MouseEvent):void
		{
			//trace("name = ", event.target.name);
			for(var i:int = 0; i<maxColors; i++)
			{
				if(colorSelect)
				{
					if(event.target.name == colorSelect["btn_"+i].name) return;
				}
			}
			if(event.target.name == "btnSelectColor" || event.target.name == "btnColor") return;
			GameCommonData.GameInstance.TooltipLayer.removeEventListener(MouseEvent.MOUSE_DOWN, removeList);
			if(colorSelect)
			{
				if(GameCommonData.GameInstance.GameUI.contains(colorSelect))
				{
					ChatData.ColorIsOpen = false;
					GameCommonData.GameInstance.GameUI.removeChild(colorSelect);
					this.viewComponent = null;
					facade.removeMediator(NAME);
				}
			}
		}
		
		private function closeHandler():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(colorSelect))
			{
				ChatData.ColorIsOpen = false;
				GameCommonData.GameInstance.GameUI.removeChild(colorSelect);
				this.viewComponent = null;
				facade.removeMediator(NAME);
			}
		}
	}
}