package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Team.Datas.TeamEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	
	/**
	 * 队伍详细数据 
	 */
	public class TeamInfoAction extends GameAction
	{
		public static const  _MSG_TEAMMEMBER_ANSWERINFO:int = 4;	//队伍成员 和 申请 信息
		
		public static const  INVITE_TEAMINFO:int = 5;				//邀请队伍的队伍信息
		
		public function TeamInfoAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			
			var obj:Object   = new Object();
			var memArr:Array = [];
			var appArr:Array = [];
			
			obj.ucAction  = bytes.readUnsignedInt();		// ucAction
			obj.ucAmount  = bytes.readUnsignedInt();		// ucAmount，广播的人数n
			obj.id 		  = bytes.readUnsignedInt();		// id，请求的目标ID，n
			obj.nData	  = bytes.readInt();				// nData，队伍的状态n
			
			obj.usSizeIn  = bytes.readUnsignedShort();		// usSizeIn，队伍中的人数
			obj.usSizeOut = bytes.readUnsignedShort();		// usSizeOut，申请中的人数
			
			for(var i:int = 0; i<obj.usSizeIn; i++)
			{
				var memObj:Object = new Object;
				
				memObj.szName	  = bytes.readMultiByte(16,GameCommonData.CODE);	// 队员名字
				memObj.id 		  = bytes.readUnsignedInt();		// 队员ID
				memObj.dwLookFace = bytes.readUnsignedInt();		// 队员头像
				memObj.usLev 	  = bytes.readUnsignedInt();		// 队员等级
				////////////////// 2010.9.1 去掉
//				memObj.usMaxHp 	  = bytes.readInt();				// 队员最大HP
//				memObj.usHp		  = bytes.readInt();				// 队员现有的HP
//				memObj.usMaxMp	  = bytes.readInt();				// 队员最大MP
//				memObj.usMp		  = bytes.readInt();				// 队员现有MP
				//--------------------------
				memObj.usPro 	  = bytes.readUnsignedShort();		// 队员职业
				memObj.usProLev	  = bytes.readUnsignedShort();		// 队员职业等级
				
				//--------- 
				//新增功能
				memObj.onlineStatus = bytes.readUnsignedInt();		// 此队员的在线状态,1是在线,0是不在线
				memObj.idMap 		= bytes.readUnsignedInt();		// 下线时此队员所在的地图id
				memObj.usPosX	    = bytes.readUnsignedShort();	// 下线时所在的X坐标
				memObj.usPosY	    = bytes.readUnsignedShort();	// 下线时所在的Y坐标 
				//---------- 
				//////////////// 2010.9.1 去掉 
//				memObj.usPosX	  = bytes.readShort();				// 队员的坐标X
//				memObj.usPosY	  = bytes.readShort();				// 队员的坐标Y
//				memObj.usMapID 	  = bytes.readUnsignedInt();		// 队员所在地图ID
				//-----------------------------
				memArr.push(memObj);
			}
//			trace("成员列表长度: ", memArr.length);
			obj.teamMemList = memArr;
			
			for(var j:int = 0; j < obj.usSizeOut; j++)
			{
				var appObj:Object = new Object;
				
				appObj.szName 	  = bytes.readMultiByte(16,GameCommonData.CODE);	// 申请人名字
				appObj.id 		  = bytes.readUnsignedInt();		// 申请人ID
				appObj.dwLookFace = bytes.readUnsignedInt();		// 申请人头像
				appObj.usLev 	  = bytes.readUnsignedInt();		// 申请人等级
				
				appObj.usPro	  = bytes.readUnsignedShort();		// 申请人职业
				appObj.usProLev   = bytes.readUnsignedShort();		// 申请人职业等级
//				//////////////// 2010.9.1 去掉
//				appObj.idMap	  = bytes.readUnsignedShort();		// 申请人地图ID

				//---------
				//新增功能
				memObj.onlineStatus = bytes.readUnsignedInt();		// 此队员的在线状态,1是在线,0是不在线 
				memObj.idMap 		= bytes.readUnsignedInt();		// 下线时此队员所在的地图id
				memObj.usPosX	    = bytes.readUnsignedShort();	// 下线时所在的X坐标
				memObj.usPosY	    = bytes.readUnsignedShort();	// 下线时所在的Y坐标
				
				//-----------------------------
				appArr.push(appObj);
			}
			obj.teamReqList = appArr;
			switch(obj.ucAction) {
				case _MSG_TEAMMEMBER_ANSWERINFO:
					sendNotification(EventList.UPDATETEAM, obj);	//更新组队界面
					break;
				case INVITE_TEAMINFO:
					sendNotification(TeamEvent.SHOWINVITE, obj);
					break;
				default:
					
			}
		}
		
		
		
	}
}