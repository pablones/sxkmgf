package GameUI.Modules.UnityNew.Command
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Mediator.NewUnityMainMediator;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class UntiyNewStartupCommand extends SimpleCommand
	{
		public static const NAME:String = "UntiyNewStartupCommand";
		
		public function UntiyNewStartupCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			facade.registerMediator( new NewUnityMainMediator() );
			sendNotification( NewUnityCommonData.CLICK_NEW_UNITY_BTN );
		}
		
	}
}