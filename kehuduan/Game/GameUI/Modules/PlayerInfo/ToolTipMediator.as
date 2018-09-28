package GameUI.Modules.PlayerInfo
{
	import GameUI.ConstData.EventList;
	import GameUI.UIConfigData;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ToolTipMediator extends Mediator
	{
		public static const NAME:String="ToolTipMediator";
		
		public static const PADDING:uint=0;
		
		public function ToolTipMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get contentMC():Sprite{
			return this.viewComponent as Sprite;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EventList.INITVIEW,
				EventList.HIDETOOLTIP
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.INITVIEW:
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"ToolTip"});
					this.contentMC.mouseEnabled=false;
					break;
				case EventList.SHOWTOOLTIP:
					while(this.contentMC.numChildren>1){
						this.contentMC.removeChildAt(1);
					}
					
					var contentData:Object=notification.getBody();
					var contentDisplay:DisplayObject=contentData["content"];
					contentDisplay.x=PADDING;
					contentDisplay.y=PADDING;
					this.contentMC.width=contentDisplay.width+2*PADDING;
					this.contentMC.height=contentDisplay.height+2*PADDING;
					
					this.contentMC.addChild(contentDisplay);
					GameCommonData.GameInstance.GameUI.stage.addChild(this.contentMC);
					this.contentMC.x=contentData.x;
					this.contentMC.y=contentData.y;
					break;
				case EventList.HIDETOOLTIP:
					if(GameCommonData.GameInstance.GameUI.stage.contains(this.contentMC)){
						GameCommonData.GameInstance.GameUI.stage.removeChild(this.contentMC);
					}
				 	break;	
			}
		}
		
		
		
	}
}