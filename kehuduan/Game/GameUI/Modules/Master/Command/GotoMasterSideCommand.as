package GameUI.Modules.Master.Command
{
	import GameUI.Modules.ChangeLine.Command.ChgLineSucCommand;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Hint.Events.HintEvents;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class GotoMasterSideCommand extends SimpleCommand
	{
		public static const NAME:String = "GotoMasterSideCommand";
		
		public function GotoMasterSideCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var line:int = notification.getBody() as int;
			var lineName:String = getLineStr( line );				//获得要去的线路
			if ( lineName == GameConfigData.GameSocketName ) return;
			sendNotification( ChgLineData.GO_TO_TARGET_LINE,lineName );
		}
		
		private function getLineStr( line:int ):String
		{
			var _lineStr:String;
			switch ( line )
			{
				case 0:
					_lineStr = "<font color = '#666666'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_1" ]+"</font>";   //离线
				break;
				case 1:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_2" ];       //一线
				break;
				case 2:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_3" ];       //二线     
				break;
				case 3:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_4" ];       //三线
				break;
				case 4:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_5" ];       //四线
				break;
				case 5:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_6" ];       //五线
				break;
				case 6:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_7" ];       //六线
				break;
				case 7:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_8" ];       //七线
				break;
				default:			//防止意外
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_2" ];       //一线
				break;
			}
			return _lineStr;
		}
		
	}
}