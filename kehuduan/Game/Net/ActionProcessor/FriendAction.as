package Net.ActionProcessor
{
	import Controller.DuelController;
	import Controller.PlayerController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Friend.command.FriendActionList;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Friend.model.vo.RoleLaborStruct;
	import GameUI.Modules.Icon.Data.IconData;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.UICore.UIFacade;
	
	import Net.GameAction;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.utils.ByteArray;

	public class FriendAction extends GameAction
	{
		
		public function FriendAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		/**  处理接收到的消息 */
		public override function Processor(bytes:ByteArray):void 
		{
		 	bytes.position=4;
			var roleInfo:PlayerInfoStruct=new PlayerInfoStruct();
		 	
		 	var idFriend:uint=bytes.readUnsignedInt();//8
		 	roleInfo.frendId=idFriend;
			var dwLookFace:uint=bytes.readUnsignedInt();//12
			roleInfo.face=dwLookFace;
			var ucAction:uint=bytes.readShort();//13
			
			
			///////////////////////////////////////////////////
			 roleInfo.ucAction=ucAction;
			var unOnlineFlag:uint=bytes.readShort();//14
			roleInfo.isOnline=unOnlineFlag;
//			if(unOnlineFlag==0){
//				roleInfo.isOnline=false;
//			}
//			if(unOnlineFlag==1){
//				roleInfo.isOnline=true;
//			}
			var ucLevel:uint = bytes.readShort();//15
			roleInfo.level=ucLevel;
			var relation:uint=bytes.readShort();//16
			roleInfo.relation=relation;
			var ucFriendShip:uint = bytes.readUnsignedInt();//20
			roleInfo.friendShip=ucFriendShip;
			var ucProfession:uint = bytes.readUnsignedInt();//24
			roleInfo.profession=ucProfession;
			var sex:uint = bytes.readUnsignedInt();//28
			roleInfo.sex=sex;
			var friendType:uint = bytes.readUnsignedInt();//32
			roleInfo.friendGroupId=friendType;
			var friendName:String = bytes.readMultiByte(16,GameCommonData.CODE);
			roleInfo.roleName=friendName;
			var friendFeel:String = new String();
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int = 0;
			for(var i:int = 0;i < nDataSeeNum; i++)
			{
				nDataSee = bytes.readByte();
				if(nDataSee != 0)
				{		
					if(i==0){
						friendFeel = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); 
					}
				}		
			}
			roleInfo.feel=friendFeel; 
		
			switch (ucAction){
				case FriendActionList.INIT_FRIEND_LIST :
					this.sendNotification(FriendCommandList.UPDATE_FRIEND_LIST,roleInfo);
					break;
				case FriendActionList.CHANGE_GROUP:
					bytes.position=28;
					var obj:Object={};
					obj["newGroupID"]=roleInfo.friendGroupId-1;
					obj["friendId"]=roleInfo.frendId;
					this.sendNotification(FriendCommandList.CHANGE_GROUP,obj);
					break;
				case 22://切磋
				    switch(friendType)
				    {
				    	case 0: //切磋结束
				    	        GameCommonData.DuelAnimal = 0;
				    	        DuelController.CloseTime();
				    	     break;
				    	case 1: //切磋判断 弹出对话框
				    	 	  var player:GameElementAnimal = PlayerController.GetPlayer(idFriend);
				    	 	  //判断对象是否存在
				    	      if(player != null)
				    	      {
				    	      	//判断是否在战斗状态
				    	      	if(!GameCommonData.Player.Role.IsAttack && !GameCommonData.IsDuel)
				    	      	{
				    	      		GameCommonData.IsDuel = true;
				    		 	 	UIFacade.UIFacadeInstance.sendNotification(EventList.SHOWALERT,{comfrim:DuelController.AcceptDuel,cancel:DuelController.CancelDuel,extendsFn:DuelController.CancelDuel,doExtends:1,info:"  "+player.Role.Name+GameCommonData.wordDic[ "net_ap_fa_proc_1" ],params:idFriend, params_cancel:idFriend,params_extendsFn:idFriend,title:GameCommonData.wordDic[ "net_ap_fa_proc_2" ]});
				    	      																																																					//"向你发出了切磋请求"                                                                                                    "切 磋"
				    	      	}
				    	      	else
				    	      	{
				    	      		DuelController.CannotDuel(idFriend); 
				    	      	}
				    	      }
				    	      else	    
				    	      {  				    	      
				    	         DuelController.CancelDuel(idFriend);
				    	      }
				    	      break;
				    	case 2: //切磋对方同意 发信息给服务器
				    	      player = PlayerController.GetPlayer(idFriend);
				    	       if(player != null) 
				    	       {
					    	   	   GameCommonData.DuelAnimal  = idFriend;	
						    	   DuelController.BeginDuel(idFriend);		    	
				    	       }
				    	      break;
				    	case 5: //同意切磋
				    	      player = PlayerController.GetPlayer(idFriend);
				    	      if(player != null)
				    	      {
				    	         sendNotification(EventList.TIMEUP,{taskId:3});
				    	   		 GameCommonData.DuelAnimal  = idFriend;					    	   		 					    	   	 
				    	   	  }	 			    	   		 	    	   		 
				    	      break;
				        case 9: //倒数
				             DuelController.OpenTime();
				             break;
				        case 8: //取消
				             DuelController.CloseTime();
				             break;
				    }	
					break;
				//添加好友成功	
				case FriendActionList.ADD_FRIEND_SUCCESS :
					this.sendNotification(FriendCommandList.UPDATE_FRIEND_LIST,roleInfo);
					break;
					
				//处理好友申请	
				case FriendActionList.ADD_FRIEND :
				 	this.sendNotification( IconData.SHOW_ICON, {index:3, message:roleInfo} );
//					this.sendNotification(FriendCommandList.INVATE_TO_FRIEND,roleInfo);
					break;
				//删除好友成功 	
				case FriendActionList.DELETE_FRIEND:
					facade.sendNotification(FriendCommandList.DELETE_FRIEND_SUCCESS,roleInfo);
					break;
				//删除好友失败	
				case FriendActionList.DELETE_FRIEND_FAIL:
					break;
				//修改心情		
				case FriendActionList.EDIT_FEEL:
					GameCommonData.Player.SetTitle(roleInfo.feel);
					facade.sendNotification(FriendCommandList.EDIT_FEEL_SUCCESS);
					facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"heart_txt",value:roleInfo.feel} );
					break;
				//好友心情改变	
				case FriendActionList.FRIEND_FEEL_CHANGE :
					facade.sendNotification(FriendCommandList.CHANGE_FRIEND_FEEL,roleInfo);
				 	break;	
				 //修改好友头像	
				case FriendActionList.CHANGE_FRIEND_FACE:
					facade.sendNotification(FriendCommandList.CHANGE_FRIEND_FACE,roleInfo);
					break;	
				//好友上线 	
				case FriendActionList.FRIEND_ONLINE:
					facade.sendNotification(FriendCommandList.CHANGE_FRIEND_ONLINE,roleInfo);
					break;
				//好友下线	
				case FriendActionList.FRIEND_DOWNLINE:
					facade.sendNotification(FriendCommandList.CHANGE_FRIEND_ONLINE,roleInfo);
					break; 	
				case FriendActionList.HAVA_TEAM:
					facade.sendNotification(FriendCommandList.FRIEND_TEAM_UPDATE,{idTeam:idFriend,idTeamleader:dwLookFace,teamName:friendFeel});
					break;	
					
				//获好友详细信息	
				case FriendActionList.GET_FRIEND_INFO:
					bytes.position=4;
					var info:RoleLaborStruct=new RoleLaborStruct();
					info.friendID=bytes.readUnsignedInt();
					info.face=bytes.readUnsignedInt();
					info.action=bytes.readShort();
					//副职业
					info.subWork=bytes.readShort();
					info.level=bytes.readShort();
					info.relation=bytes.readShort();
					info.friendShip=bytes.readUnsignedInt();
					//主职业
					info.profession=bytes.readUnsignedInt();
					info.professionLevel=bytes.readUnsignedInt();
//					info.subWork=
					info.subWorkLevel=bytes.readUnsignedInt();
					info.roleName=bytes.readMultiByte(16,GameCommonData.CODE);
					
					
					var Num:int = bytes.readByte();
					var nData:int = 0;
					for(var i1:int = 0;i1 < Num; i1 ++)
					{
						nData = bytes.readByte();
						if(nData != 0)
						{		
							if(i1==0){
								info.feel= bytes.readMultiByte(nData ,GameCommonData.CODE); 
							}
							if(i1==1){
								info.faction= bytes.readMultiByte(nData ,GameCommonData.CODE); 
							}
							if(i1==2){
								info.title= bytes.readMultiByte(nData ,GameCommonData.CODE); 
							}
							if(i1==3){
								info.mate= bytes.readMultiByte(nData ,GameCommonData.CODE); 
							}
							if(i1==4){
								info.team= bytes.readMultiByte(nData ,GameCommonData.CODE); 
							}
							if(i1==5){
								info.mapId=bytes.readMultiByte(nData,GameCommonData.CODE);
								
							}
						}		
					}
					
					var str:String=info.team;
					var arr:Array=str.split("_");
					if(arr[1]=="0"){
						facade.sendNotification(FriendCommandList.SHOW_FRIENDINFO,info);	
					}else if(arr[1]=="1"){
						facade.sendNotification(FriendCommandList.REVEIVE_CHAT_INFO,info);
					}
					break;	
							
			}
			
	
		}
	}
}