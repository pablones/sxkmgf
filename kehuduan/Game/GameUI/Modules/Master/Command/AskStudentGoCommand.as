package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.ChangeLine.Command.ChgLineSucCommand;
	import GameUI.Modules.Hint.Events.HintEvents;
	
	import Net.ActionSend.TutorSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class AskStudentGoCommand extends SimpleCommand
	{
		public static const NAME:String = "AskStudentGoCommand";
		public function AskStudentGoCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var masterName:String = notification.getBody().toString();
			if ( GameCommonData.Player.Role.HP<= 0 )
			{
				sendNotification( HintEvents.RECEIVEINFO,{ info:GameCommonData.wordDic[ "mod_mas_com_agrs_exe_1" ]+"["+masterName+"]"+GameCommonData.wordDic[ "mod_mas_com_askgo_exe_1" ], color:0xffff00 } );  //你的师傅               正在召唤你
				return;
			}
			var info:String = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_com_agrs_exe_1" ]+"<font color = '#ffff00'>["+masterName+"]</font>"+GameCommonData.wordDic[ "mod_mas_com_askgo_exe_2" ]+"</font>";     //你的师傅            正在召唤你，是否前往？    
			var count:int = 0;
			if ( ChgLineSucCommand.countDownText )
			{
				count = 5;
			}
			sendNotification(EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_tra_med_tra_showt_2" ],cancelTxt:GameCommonData.wordDic[ "mod_team_med_teamm_mem_4" ],countDown:count });    //提 示       同 意      拒 绝
			if ( facade.hasCommand( NAME ) )
			{
				facade.removeCommand( NAME );
			}
		}
		
		private function commitHandler():void
		{
			//要等切线倒计时完成才可前往
//			if ( ChgLineSucCommand.countDownText )
//			{
//				sendNotification( HintEvents.RECEIVEINFO,{ info:"切线倒计时完成之后才可前往", color:0xffff00 } );
//				return;
//			}
			sendData( 1 );
		}
		
		private function cancelClose():void
		{
			sendData( 2 );
		}
		
		private function sendData( index:int ):void
		{
			var obj:Object = new Object();
			obj.action = 28;
			obj.amount = 0;
			obj.pageIndex = index;
			obj.mentorId = 0;
			TutorSend.sendTutorAction( obj );
		}
		
	}
}