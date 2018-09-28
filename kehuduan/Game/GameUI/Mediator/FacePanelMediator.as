package GameUI.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.UIConfigData;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class FacePanelMediator extends Mediator
	{
		public static const NAME:String = "FacePanelMediator";
		private var defaultPos:Point = new Point(320, 460);
		private var maxFaces:int = 32; 
		private var type:String = "chat";
		
		public function FacePanelMediator()
		{
			super(NAME);
		}
		
		private function get faceView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOWFACEVIEW,
				EventList.HIDESELECTFACE
			]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.SHOWFACEVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SELECTFACE});
					if(notification.getBody())
					{
						faceView.x = notification.getBody().x;
						faceView.y = notification.getBody().y;
						type = notification.getBody().type;
					}
					else
					{
						faceView.x = defaultPos.x;
						faceView.y = defaultPos.y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
					}
					GameCommonData.GameInstance.GameUI.addChild(faceView);
					if (type == "arena")
					{
						UIConstData.SelectFaceForArenaIsOpen = true;
					}
					else
					{
						UIConstData.SelectFaceIsOpen = true;
					}
					initView();
				break;
				case EventList.HIDESELECTFACE:
					closeHandler();
				break
			}
		}	
		
		private function initView():void
		{
			for(var i:int = 1; i<=maxFaces; i++)
			{
				var name:String = i.toString();
				if(i<10)
				{
					name = "00"+i;
				}
				else
				{
					name = "0"+i;
				}
				faceView["btn_"+name].addEventListener(MouseEvent.CLICK, selectFace);
			}
			GameCommonData.GameInstance.addEventListener(MouseEvent.MOUSE_DOWN, removeList);
		}
		
		private function removeList(event:MouseEvent):void
		{
			for(var i:int = 1; i<=maxFaces; i++)
			{
				if(faceView)
				{
					var name:String = i.toString();
					if(i<10)
					{
						name = "00"+i;
					}
					else
					{
						name = "0"+i;
					}
					if(event.target.name == faceView["btn_"+name].name) return;
				}
			}
			if(event.target.name == "btnFace") return;
			GameCommonData.GameInstance.TooltipLayer.removeEventListener(MouseEvent.MOUSE_DOWN, removeList);
			if(faceView)
			{
				if(GameCommonData.GameInstance.GameUI.contains(faceView))
				{
					if (type == "arena")
					{
						UIConstData.SelectFaceForArenaIsOpen = false;
					}
					else
					{
						UIConstData.SelectFaceIsOpen = false;
					}
					GameCommonData.GameInstance.GameUI.removeChild(faceView);
					this.viewComponent = null;
					facade.removeMediator(NAME);
				}
			}
		}
		
		private function selectFace(event:MouseEvent):void
		{
			var faceName:String = event.currentTarget.name.split("_")[1];
			if(type == "leo")
			{
				facade.sendNotification(ChatEvents.SELECTEDFACETOLEO, faceName);
			}
			else if(type == "chat")
			{
				facade.sendNotification(ChatEvents.SELECTEDFACETOCHAT, faceName);
			}	
			else if(type == "friend")
			{
				facade.sendNotification(FriendCommandList.GET_FACE_NAME, faceName);
			}
			else if (type == "arena")
			{
				facade.sendNotification(ChatEvents.SELECTEDFACETOARENACHAT, faceName);
			}
			closeHandler();
		}
		
		private function closeHandler():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(faceView))
			{
				if (type == "arena")
				{
					UIConstData.SelectFaceForArenaIsOpen = false;
				}
				else
				{
					UIConstData.SelectFaceIsOpen = false;
				}
				GameCommonData.GameInstance.GameUI.removeChild(faceView);
				this.viewComponent = null;
				facade.removeMediator(NAME);
			}
		}
	}
}