package GameUI.Modules.Friend.command
{
	import GameUI.Modules.Friend.model.proxy.MessageWordProxy;
	import GameUI.Modules.Friend.model.vo.MessageStruct;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.SystemMessage.Data.SysMessageEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	/**
	 * 
	 * @author felix
	 * 
	 */
	public class ReceiveMsgCommand extends SimpleCommand
	{
		
		public function ReceiveMsgCommand()
		{
			super();
		}
		
		override public function execute(notification:INotification):void{
			var objChat:Object=notification.getBody();
			if(objChat.nAtt==2019 || objChat.nAtt==2110){
				var leavelWordPanel:MessageWordProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
				var obj:Object=notification.getBody();
				
				
				var data:Array=obj.talkObj as Array;
				var msg:MessageStruct=new MessageStruct();
				msg.isOnline=obj.nItemTypeID;
				msg.face=obj.nItem;
				msg.isFriend=obj.nGm;
				msg.sendId=obj.nColor;
				msg.action=obj.nAtt;
				msg.sendPersonName=data.shift();	
				msg.receivePersonName=data.shift();
				msg.sendTime=Number(data.shift());
				msg.msg=data.shift();
				msg.style=data.shift();
				msg.feel=data.shift();
				
				
				var friendMediator:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
				var queryObj:Object=friendMediator.searchFriend(friendMediator.dataList,0,0,msg.sendPersonName);
				if(msg.action!=FriendActionList.SYSTEM_MSG){
					if(queryObj!=null && queryObj.i==5)return;  //黑名单消息。不收 
				}
				if(msg.action!=FriendActionList.CHAT_FLAG){
					var t:String=String(msg.sendTime);
					var date:Date=new Date(t.substr(0,4),Number(t.substr(4,2)),Number(t.substr(6,2)),Number(t.substr(8,2)),Number(t.substr(10,2)), Number(t.substr(12,2)));
					msg.sendTime=date.time;
				}
						
				switch (msg.action){
					case FriendActionList.SYSTEM_MSG:
						msg.face=99;
						leavelWordPanel.pushMsg(msg);
						break;
					case FriendActionList.LEAVE_WORD:
						msg.isleave=true;
						leavelWordPanel.pushMsg(msg);
						break;
					case FriendActionList.CHAT_FLAG:
						leavelWordPanel.pushMsg(msg);
						break;
				}
			}else if(objChat.nAtt == FriendActionList.SYSTEM_MSG){
				facade.sendNotification(SysMessageEvent.ADDSYSMESSAGE , {title:objChat.nSty , content:objChat.talkObj[3]});
				var msgProxy:MessageWordProxy=facade.retrieveProxy(MessageWordProxy.NAME) as MessageWordProxy;
				msgProxy.addUnReadMsgId(110);
//				this.sendNotification(FriendCommandList.FRIEND_MESSAGE); 
				sendNotification(SysMessageEvent.SYSMESSAGE_FLASH_MAIN_SENCE);
			}

				
		}
		
	}
}