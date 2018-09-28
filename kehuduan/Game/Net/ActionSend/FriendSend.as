package Net.ActionSend
{
	import GameUI.Modules.Friend.command.FriendActionList;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class FriendSend
	{
		private static var _instance:FriendSend;
		
		public static function getInstance():FriendSend{
			if(_instance==null){
				_instance=new FriendSend();
			}
			return _instance;	
		}
		
		/**
		 * 向服务器请求 
		 * @param msgStruct
		 * 
		 */		
		public function sendAction(msgStruct:Array):void{
			if(!GameCommonData.isSend) return;
			
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;   //小端模式
			msgBytes.writeShort(32);   //消息长度
			msgBytes.writeShort(Protocol.PLAYER_ACTION)  //1010
			msgBytes.writeUnsignedInt(msgStruct.shift());
			msgBytes.writeUnsignedInt(msgStruct.shift());
			msgBytes.writeShort(msgStruct.shift());
			msgBytes.writeShort(msgStruct.shift());
			msgBytes.writeShort(msgStruct.shift());
			msgBytes.writeShort(0);       //对齐
			msgBytes.writeUnsignedInt(msgStruct.shift());
			msgBytes.writeShort(msgStruct.shift());
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(msgStruct.shift());
			GameCommonData.GameNets.Send(msgBytes);
			
		}
		
		/**
		 * 发送更新心情的显示与隐藏信息 
		 * @param type
		 * 
		 */		
		public function sendOnsynFeel(type:uint):void{
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(type);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(35);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeMultiByte("0000",GameCommonData.CODE);
			
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(msgBytes);
		}
		
		/**
		 * 获取好友列表命令 
		 * @return 
		 * 
		 */		
		public function getFriendListParam():Array{
			var param:Array=new Array();
			param.push(0);
			param.push(GameCommonData.Player.Role.Id);
			param.push(0);
			param.push(0);
			param.push(0);
			param.push(0);
			param.push(16);
			param.push(0);
			return param;
		}
		
		/**
		 *  修改分组
		 * 
		 */		
		public function changeGroup(msgStruct:PlayerInfoStruct,newGroupId:uint):void{
			
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(msgStruct.frendId);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(FriendActionList.CHANGE_GROUP);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(newGroupId);
			msgBytes.writeMultiByte(msgStruct.roleName,GameCommonData.CODE);
			
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			
			GameCommonData.GameNets.Send(msgBytes);
			
			
		}
		
		/**
		 * 添加好友 
		 * 
		 */		
		public function addFriendAction(friendId:int,groupId:int,roleName:String):void{
			
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(friendId);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(FriendActionList.ADD_FRIEND);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(groupId);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
			UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "net_as_fs_afa" ],0xffff00);      //"添加好友申请已成功发出"   
			
		}
		/**
		 * 删除好友 
		 * 
		 */		
		public function deleteFriendAction(friendId:int,roleName:String):void{
			
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(friendId);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(FriendActionList.DELETE_FRIEND);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(1);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
			
		}
		
		/**
		 * 
		 * 修改好友心情  
		 */
		
		public function editFeel(id:int,roleName:String,feel:String):void{
			if(UIUtils.isPermitedRoleName(feel) == false || UIUtils.legalRoleName(feel) == false)							//有不合格字符
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "net_as_fs_ef" ], color:0xffff00});       //"不能进行非法处理"
				return;
			}
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(id);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(FriendActionList.EDIT_FEEL);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeMultiByte(feel,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
			
		
		}
		
		/**
		 *  
		 * @param id： 好友ID号
		 * @param roleName：好友名称
		 * 
		 */		
		public function getFriendInfo(id:uint=0,roleName:String=""):void{
			
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(id);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(FriendActionList.GET_FRIEND_INFO);   //功能号
			msgBytes.writeShort(0);                       
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);             //0:请求好友详细信息   1：聊天时的好友详细信息  
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
		}
		
		/**
		 *  请求聊天好友详细信息
		 * @param id
		 * @param roleName
		 * 
		 */		
		public function getChatInfo(id:uint,roleName:String=""):void{
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(id);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(FriendActionList.GET_FRIEND_INFO);   //功能号
			msgBytes.writeShort(0);                       
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(1);             //0:请求好友详细信息   1：聊天时的好友详细信息  
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
		}
		
		
		
		//决斗 
		public static function Duel(sender:int,typeID:int,roleName:String=""):void
		{
		   if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(sender);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(22);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(typeID); //0 发起 2 接受 4取消 5开始
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
		}
		
		/**
		 * 
		 * 
		 */		
		public function sendReadLeaveMsg(msgid:uint,friendID:uint,roleName:String,action:uint=33):void{
			
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(friendID);  //填写好友的ID号
			msgBytes.writeUnsignedInt(msgid);
			msgBytes.writeShort(action);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
			
		}
		
		/**
		 *  请求好友的队伍
		 * 
		 */		
		public function requestRoleTeam(id:uint,roleName:String):void{
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(id);  //填写好友的ID号
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeShort(34);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
		}
		
		/**
		 *  回复玩家好友的申请
		 * 
		 */		
		public function recallApplyToFriend(friendId:uint,type:uint,applyGroup:uint,group:uint,roleName:String):void{
			if(!GameCommonData.isSend) return;
			var msgBytes:ByteArray=new ByteArray();
			msgBytes.endian=Endian.LITTLE_ENDIAN;
			msgBytes.writeShort(52);
			msgBytes.writeShort(Protocol.FRIEND_ACTION);
			msgBytes.writeUnsignedInt(friendId);  //填写好友的ID号
			msgBytes.writeUnsignedInt(type);
			msgBytes.writeShort(38);   //功能号
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeShort(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(0);
			msgBytes.writeUnsignedInt(applyGroup);
			msgBytes.writeUnsignedInt(group);
			msgBytes.writeMultiByte(roleName,GameCommonData.CODE);
			while(msgBytes.length<52){
				msgBytes.writeByte(0);
			}
			GameCommonData.GameNets.Send(msgBytes);
		}
	}
}