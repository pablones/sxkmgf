package Net.ActionProcessor  
{
	import GameUI.ConstData.CommandList;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	
	/** 聊天频道 */
	public class Chat extends GameAction
	{
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position  = 4;
			var obj:Object  = new Object();
			obj.nColor      = bytes.readUnsignedInt();	  	 	 //话的颜色
			obj.nAtt 	    = bytes.readUnsignedShort(); 		 //话的频道			
			obj.nSty 	    = bytes.readUnsignedShort(); 	 	 //话的风格
			obj.nTimet 	    = bytes.readUnsignedInt(); 			 //时间
			obj.nGm 	    = bytes.readUnsignedInt();    		 //GM工具所用
			obj.nItem 		= bytes.readUnsignedInt();  		 //话里面带的物品ID
			obj.nItemTypeID = bytes.readUnsignedInt(); 	 		 //话里面带的物品typeID
			obj.talkObj	= [];
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int    = 0;	
			for(var i:int = 0;i < nDataSeeNum; i++) 
			{
				nDataSee = bytes.readUnsignedByte();	//发言内容长度  
				//0 说话人
				//1 说话对象
				//2 action(表情，动作)
				//3 说话内容
				//4 item(物品)
				//5 好友心情
				obj.talkObj[i] = 0;
				if(nDataSee != 0) 
				{
					obj.talkObj[i] = bytes.readMultiByte(nDataSee , GameCommonData.CODE);
					//trace("obj.talkObj["+i+"] = ", obj.talkObj[i]);
				}		
			}
			if(obj.nAtt == 2100)
			{
//				trace("消息内容 = ", obj.talkObj[3]);
			    //如果报错，可以注释掉下一行
			    facade.sendNotification(CommandList.CREATEOVER, obj);
				return;
			}
			if(obj.nAtt == 2111)												//帮会通告
			{
//				trace("消息内容 = ", obj.talkObj[3]);
				facade.sendNotification(UnityEvent.UPDATANOTICE, obj.talkObj[3]);
				return;
			}
			if(obj.nAtt == 2117)												//帮会分堂的技能研究返回的数据
			{
				facade.sendNotification(UnityEvent.SKILLSTUDIED, int(obj.talkObj[3]));
				return;
			}
			if(obj.nAtt == 2118)												//打工成功,发的后台通告
			{ 
				if ( obj.nColor == GameCommonData.Player.Role.Id )
				{
					facade.sendNotification(UnityEvent.WORKFINISH , {Id:obj.nColor , type:obj.nSty , addVaule:obj.nItem , addUnityMoney:obj.nItemTypeID});
				}
//				facade.sendNotification(TimeCountDownEvent.CLOSEWORKCOUNTDOWN);		//关闭打工倒计时
			}
			if(obj.nAtt == 2119)												//捐献所得的金钱
			{
				facade.sendNotification(UnityEvent.CONTRIBUTEFINISH , {Id:obj.nColor , addUnityBuilt:obj.nItem , addUnityMoney:obj.nItemTypeID , addUnityContribute:obj.nGm});//发送捐献所得到的钱 和 角色ID
			}

			if(obj.nAtt == 2045)												//开箱子江湖惊闻
			{
				if ( TreasureData.TreaResourceLoaded )
				{
					facade.sendNotification( TreasureData.SHOW_RIVERS_HEARD,obj );		
				}
				obj.nAtt = 2035;
				obj.nColor = 0x0099ff;
			}
			
			facade.sendNotification(CommandList.RECEIVECOMMAND, obj);
			facade.sendNotification(CommandList.STALLBBSRECEIVECOMMAND, obj);
			facade.sendNotification(CommandList.FRIEND_CHAT_MESSAGE,obj);
		
		}
	}
}