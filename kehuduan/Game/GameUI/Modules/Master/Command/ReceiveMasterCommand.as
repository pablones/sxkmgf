package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	
	import Net.ActionSend.TutorSend;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ReceiveMasterCommand extends SimpleCommand
	{
		public static const NAME:String = "ReceiveMasterCommand";
		private var masterName:String;													
		
		public function ReceiveMasterCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			masterName = notification.getBody().toString();
			var info:String = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_com_rec_exe_1" ]+"<font color = '#ffff00'>"+masterName+"</font>"+GameCommonData.wordDic[ "mod_mas_com_rec_exe_2" ]+"</font>";  //真的要拜          为师吗？ 
			facade.sendNotification(EventList.SHOWALERT, {comfrim:kneelMaster, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_bot_exe_1" ],cancelTxt:GameCommonData.wordDic[ "mod_team_med_teamm_mem_4" ]});  //提 示      拜 师     拒 绝
		}
		
		private function kneelMaster():void
		{
			//收到师傅的消息，发送徒弟确认消息
			sendInfo(1);
		}
		
		private function cancelClose():void
		{
			sendInfo(2);
		}
		
		private function sendInfo(index:uint):void
		{
			var obj:Object = new Object();
			obj.action = 34;
			obj.amount = 0;
			obj.pageIndex = index;
			obj.mentorId = 0;
			TutorSend.sendTutorAction( obj );
		}
		
	}
}