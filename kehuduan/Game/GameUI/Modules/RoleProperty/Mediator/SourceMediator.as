package GameUI.Modules.RoleProperty.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.UIConfigData;
	
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SourceMediator extends Mediator
	{
		
		public static const NAME:String = "SourceMediator";
		public static const TYPE:int = 1;
		private var parentView:MovieClip = null;
		
		public function SourceMediator(parent:MovieClip)
		{
			parentView = parent;
			super(NAME);
		}
		
		private function get SourcePane():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//				RoleEvents.INITROLEVIEW,
////				RoleEvents.SHOWPROPELEMENT,
//				RoleEvents.MEDIATORGC
			]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case RoleEvents.INITROLEVIEW:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.MERIDIANPANE});
					this.SourcePane.mouseEnabled=false;
				break;
				case RoleEvents.SHOWPROPELEMENT:
					if(notification.getBody() as int != TYPE) return;
					parentView.addChildAt(SourcePane, 4);
				break;
				case RoleEvents.MEDIATORGC:
				break;
			}
		}
	}
}