package GameUI.Modules.NPCChat.Command
{
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.MouseCursor.DelayOperation;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SelectedNPCCommand extends SimpleCommand
	{
		
		public function SelectedNPCCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var nobj:Object=notification.getBody();
			var id:int=nobj.npcId;
			if( id==0 || id == 3300 || id == 3500 || id == 3700 || id == 3900 )
			{
				if( NewerHelpData.curType == 17 && NewerHelpData.curStep == 2 )
				{
					sendNotification(TaskCommandList.RECEIVE_TASK,{id:301});
				}
				return;
			}
			if(nobj==null)return;
			if(DelayOperation.getInstance().isNpcTalkLock){
				return;
			}
//			//锁定NPC对话
			DelayOperation.getInstance().lockNpcTalk();
			GameCommonData.talkNpcID=id;
			if(id>0 && id<299999){
				var obj:Object = new Object();
				var parm:Array = new Array;
				parm.push(0);
				parm.push(id);
				parm.push(0);
				parm.push(0);
				parm.push(0);
				parm.push(0);
				parm.push(240);							//NPC对话
				parm.push(0);
				obj.type = Protocol.PLAYER_ACTION;
				obj.data = parm;
				PlayerActionSend.PlayerAction(obj);	
				//通知新手引导系统
				if( NewerHelpData.curType == 20 && NewerHelpData.curStep == 1 )
				{
					NewerHelpData.id = id;
					NewerHelpData.obj = nobj.newerData;
				}	
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.SELECT_NPC_NOTICE_NEWER_HELP, {id:id, newerData:nobj.newerData}); 		
			}
		}	
	}
}