package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.ChangeLine.Command.ChgLineSucCommand;
	import GameUI.Modules.Hint.Events.HintEvents;
	
	import Net.ActionSend.TutorSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class AskStudentGraduateCommand extends SimpleCommand
	{
		public static const NAME:String = "AskStudentGraduateCommand";
		public function AskStudentGraduateCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var masterName:String = notification.getBody().toString();
			
			var info:String = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_com_agrs_exe_1" ]+"<font color = '#ffff00'>["+masterName+"]</font>"+GameCommonData.wordDic[ "mod_mas_com_askgr_exe_1" ]+"</font>";  //你的师傅          与你举行出师仪式，是否同意出师？
			sendNotification(EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_tra_med_tra_showt_2" ],cancelTxt:GameCommonData.wordDic[ "mod_team_med_teamm_mem_4" ] });  //提 示       同 意        拒 绝
			if ( facade.hasCommand( NAME ) )
			{
				facade.removeCommand( NAME );
			}
		}
		
		private function commitHandler():void
		{
			sendData( 1 );
		}
		
		private function cancelClose():void
		{
			sendData( 2 );
		}
		
		private function sendData( index:int ):void
		{
			var obj:Object = new Object();
			obj.action = 32;
			obj.amount = 0;
			obj.pageIndex = index;
			obj.mentorId = 0;
			TutorSend.sendTutorAction( obj );
		}
		
	}
}