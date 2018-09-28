package GameUI.Modules.Master.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//徒弟判出师门 ，弹出的对话框
	public class BetrayMasterCommand extends SimpleCommand
	{
		public static const NAME:String = "BetrayMasterCommand";
		private var teamProxy:TeamDataProxy;
			
		public function BetrayMasterCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var masterName:String = notification.getBody() as String;
			
			var info:String = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_com_bet_exe_1" ]+"<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_bet_exe_2" ]+"</font>"+GameCommonData.wordDic[ "mod_mas_com_bet_exe_3" ]+"["+masterName+"]"+GameCommonData.wordDic[ "mod_mas_com_bet_exe_4" ]+"</font>";  //强制解除师徒关系需要        60点活力和精力     。你确定要与师傅      解除师徒关系吗？
			facade.sendNotification(EventList.SHOWALERT, {comfrim:commitHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]}); //提 示       确 定        取 消
		}
		
		private function commitHandler():void
		{
			if ( GameCommonData.Player.Role.Vit < 60 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bet_com_1" ] );   //你的精力不足60点，无法进行强制解除师徒关系
				return;
			}
			if ( GameCommonData.Player.Role.Ene < 60 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bet_com_2" ] );   //你的活力不足60点，无法进行强制解除师徒关系
				return;
			}
			
			RequestTutor.requestData( 19,0 );
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