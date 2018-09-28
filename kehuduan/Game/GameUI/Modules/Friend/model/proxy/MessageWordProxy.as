package GameUI.Modules.Friend.model.proxy
{
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Friend.view.mediator.FriendChatMediator;
	import GameUI.Proxy.DataProxy;
	
	import Net.ActionSend.FriendSend;	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	
	/**
	 * 好友消息留言板 
	 * @author felix
	 * 
	 */	
	public class MessageWordProxy extends Proxy
	{
		public static const NAME:String="MessageWordProxy";
		
		/**
		 * dic[id]={};
		 */		
		protected var _msgDic:Dictionary=new Dictionary();
		/**
		 * [id,id,id....]
		 */		
		protected var _unReadMsgs:Array=[];
		/**
		 * dic[id]=name;
		 */		
		protected var _messageDic:Dictionary=new Dictionary();
		
		
		public function MessageWordProxy(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
	
		}
		
		/**
		 * 向留言版压数据 
		 * @param value
		 * 
		 */		
		public function pushMsg(obj:Object):void{
			if(this._msgDic[obj.sendId]==null)this._msgDic[obj.sendId]=[];
			var arr:Array=this._msgDic[obj.sendId];
			arr.push(obj);
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var friendChatMediator:FriendChatMediator=facade.retrieveMediator(FriendChatMediator.NAME) as FriendChatMediator;
			

			var info:PlayerInfoStruct=new PlayerInfoStruct();
			info.roleName=obj.sendPersonName;
			info.face=obj.face;
			info.feel=obj.feel;
			//info.isOnline=(obj.isOnline==1? true:false);
			info.isOnline = obj.isOnline;
			
			if(obj.isFriend==1){
				facade.sendNotification(FriendCommandList.ADD_TEMP_FRIEND,info);
			}
			
			if(obj.isleave){
				this._messageDic[obj.sendId]=obj.sendPersonName;
			}
			if(dataProxy.FriendReveiveMsgIsOpen && friendChatMediator.roleInfo!=null && obj.sendId==friendChatMediator.roleInfo.frendId){
				sendNotification(FriendCommandList.ADD_MSG_CHAT,obj.sendId);
			}else{
				this.addUnReadMsgId(obj.sendId);
				this.sendNotification(FriendCommandList.FRIEND_MESSAGE);
			}

		}
		
		/**
		 * 添加未读信息的ID 
		 * @param id
		 * 
		 */		
		public function addUnReadMsgId(id:uint):void{
			var index:int=this._unReadMsgs.indexOf(id);
			if(index==-1){
				this._unReadMsgs.push(id);
			}
		}
		
		/**
		 * 弹出最顶端的消息ID 
		 * @return 
		 * 
		 */		
		protected function popUnReadMsgId():uint{
			if(this._unReadMsgs.length==0)return 0;
			return this._unReadMsgs.shift();
		}
		
		
		/**
		 * 从留方板中取一个消息进行读取
		 * @return 
		 * 
		 */		
		public function popMsgId():uint{
			var id:uint=this.popUnReadMsgId();
			if(id!=0){
				if(id==110){
					if(this._unReadMsgs.length==0){
						facade.sendNotification(FriendCommandList.READED_MESSAGE);     //发送消息，留言消息已空
					}
					return 110;
				}
				if(this._messageDic[id]!=null){
					FriendSend.getInstance().sendReadLeaveMsg(id,GameCommonData.Player.Role.Id,this._messageDic[id]);
					delete this._messageDic[id];
				}
				if(this._unReadMsgs.length==0){
					facade.sendNotification(FriendCommandList.READED_MESSAGE);     //发送消息，留言消息已空
				}
				return id;
			}
			return null;
		}
		
		
		/**
		 * 获取该玩家的聊天信息 
		 * @param id
		 * @return 
		 * 
		 */		
		public function getMsgs(id:uint):Array{
			if(id==0)return null;
			if(this._msgDic[id]==null)this._msgDic[id]=[];
			return this._msgDic[id] as Array;
		}
			
	}
}