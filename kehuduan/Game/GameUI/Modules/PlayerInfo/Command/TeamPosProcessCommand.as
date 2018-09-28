package GameUI.Modules.PlayerInfo.Command
{
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class TeamPosProcessCommand extends SimpleCommand
	{
		public function TeamPosProcessCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var arr:Array=notification.getBody() as Array;
			var len:uint=arr.length;
			for(var i:uint=0;i<len;i++){
				var obj:Object=arr[i];
				TeamDataProxy.teamDataDic[obj.idMember]=SmallConstData.getInstance().getSceneNameByMapId(obj.idMap);	
			}
		}
	}
}