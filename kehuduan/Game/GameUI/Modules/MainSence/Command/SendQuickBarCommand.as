package GameUI.Modules.MainSence.Command
{
	import GameUI.Modules.MainSence.Data.QuickBarData;
	
	import Net.ActionSend.QuickSwitchSend;
	
	import OopsEngine.Role.GameRole;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SendQuickBarCommand extends SimpleCommand
	{
		
		public function SendQuickBarCommand()
		{
			super();
		}
		
		
		public override function execute(notification:INotification):void{
			var role:GameRole=GameCommonData.Player.Role;
			var key:Array=[];
			var keyF:Array=[];
			for(var i:uint=0;i<8;i++){
				if(QuickBarData.getInstance().expandKeyDic[i]!=null){
					keyF[i]=QuickBarData.getInstance().expandKeyDic[i].Type*10+QuickBarData.getInstance().expandKeyDic[i].IsBind;
				}else{
					keyF[i]=0
				}	
				if(QuickBarData.getInstance().quickKeyDic[i]!=null){
					key[i]=QuickBarData.getInstance().quickKeyDic[i].Type*10+QuickBarData.getInstance().quickKeyDic[i].IsBind;
				}else{
					key[i]=0;	
				}	
			}
			var dataArr:Array=[0,0,0,0,role.Id,key,keyF,GameCommonData.BigMapMaskLow,GameCommonData.BigMapMaskHi];
			QuickSwitchSend.sendMsg(dataArr);
		}
		
	}
}