package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//师徒双方协议解除师徒关系
	public class RavelMasterCommand extends SimpleCommand
	{
		public static const NAME:String = "RavelMasterCommand";
		private var teamProxy:TeamDataProxy;
		
		public function RavelMasterCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			teamProxy = facade.retrieveProxy( TeamDataProxy.NAME ) as TeamDataProxy;
			if ( teamProxy.teamMemberList.length <= 0 ) return;
			var masterName:String;
			for ( var i:uint=0; i<teamProxy.teamMemberList.length; i++ )
			{
				if ( teamProxy.teamMemberList[i].szName != GameCommonData.Player.Role.Name )
				{
					masterName = teamProxy.teamMemberList[i].szName;
				}
			}
			var info:String = GameCommonData.wordDic[ "mod_mas_com_agr_exe_1" ]+"<font color = '#ffff00'>"+masterName+"</font>"+GameCommonData.wordDic[ "mod_mas_com_agr_exe_2" ];  //是否要与           解除师徒关系？
			facade.sendNotification(EventList.SHOWALERT, {comfrim:ravelMaster, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});   //提 示      确 定      取 消  
		}
		
		private function ravelMaster():void
		{
			RequestTutor.requestTutorAction(225);												//解除师徒关系
		}
		
		private function cancelClose():void
		{
			
		}
		
	}
}