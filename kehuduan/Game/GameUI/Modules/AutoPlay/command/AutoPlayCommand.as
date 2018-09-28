package GameUI.Modules.AutoPlay.command
{
	import org.puremvc.as3.multicore.patterns.command.MacroCommand;

	public class AutoPlayCommand extends MacroCommand
	{
		public static const NAME:String = "AutoPlayCommand";
		
		public function AutoPlayCommand()
		{
			super();
		}
		
		protected override function initializeMacroCommand():void
		{
			addSubCommand( AutoEatDragCommand );
			addSubCommand( AutoPlayTeamCommand );
		}
		
	}
}