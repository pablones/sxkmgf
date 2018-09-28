package GameUI.Modules.Designation.Data
{
	import GameUI.Modules.Designation.view.mediator.DesignationMediator;
	
	import Net.ActionProcessor.UserTitle;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class DesignationChangeCommand extends SimpleCommand
	{
		public static const NAME:String = "DesignationChangeCommand";
		public function DesignationChangeCommand()
		{
		}
		
		public override function execute(notification:INotification):void
		{
			var obj:Object = notification.getBody();
			if(obj.type == 1)
			{
				(facade.retrieveMediator(DesignationMediator.NAME) as DesignationMediator).otherShowDesignation(obj.id,obj.value);
			}
			else{
				if(obj.value == 0)
				{
					(facade.retrieveMediator(DesignationMediator.NAME) as DesignationMediator).hideSucced();
				}
				else{
					(facade.retrieveMediator(DesignationMediator.NAME) as DesignationMediator).reNameDesignationSucced(); 
				}
			}
		}
		
	}
}