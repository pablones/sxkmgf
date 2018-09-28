package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//协议解除弹出师傅对话框
	public class AgreementMasterCommand extends SimpleCommand
	{
		public static const NAME:String = "AgreementMasterCommand";
		private var teamProxy:TeamDataProxy;
			
		public function AgreementMasterCommand()
		{
			super();
		}
		
		public override function execute( notification:INotification ):void
		{
			teamProxy = facade.retrieveProxy( TeamDataProxy.NAME ) as TeamDataProxy;
			if ( teamProxy.teamMemberList.length <= 0 ) return;
			var studentName:String;
			for ( var i:uint=0; i<teamProxy.teamMemberList.length; i++ )
			{
				if ( teamProxy.teamMemberList[i].szName != GameCommonData.Player.Role.Name )
				{
					studentName = teamProxy.teamMemberList[i].szName;
				}
			}
			
			var info:String = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_com_agr_exe_1" ]+"<font color = '#ffff00'>["+studentName+"]</font>"+GameCommonData.wordDic[ "mod_mas_com_agr_exe_2" ]+"</font>";    //是否要与            解除师徒关系？
			sendNotification(EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});  //提 示      确 定      取 消  
		}
		
		private function commitHandler():void
		{
			RequestTutor.requestData( 17,0 );
			if ( facade.hasCommand( NAME ) )
			{
				facade.removeCommand( NAME );
			}
		}
		
		private function cancelClose():void 
		{
			if ( facade.hasCommand( NAME ) )
			{
				facade.removeCommand( NAME );
			}
		}
		
	}
}