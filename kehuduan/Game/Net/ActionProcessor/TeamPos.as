package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class TeamPos extends GameAction
	{
		public static const MSGTEAMPOS_POS:uint			=	1;	
		public static const MSG_TEAMMAPPOS_POS:uint     =   2;     //队友地图位置
		/////////////////////////// 
		
		public function TeamPos(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			var memberList:Array=[];
			var idUser:uint = bytes.readUnsignedInt();			// 用户ID
			var unAction:uint = bytes.readUnsignedInt();		// Action
			var ucAmount:uint = bytes.readUnsignedInt();		// 成员数量
			
			for (var i:int=0; i<ucAmount; i++)
			{
				var obj:Object={};
				obj.idMember = bytes.readUnsignedInt();//队伍成员ID
				obj.idMap = bytes.readUnsignedInt();	//地图ID
				obj.nPosX = bytes.readUnsignedShort();  //X坐标
				obj.nPosY = bytes.readUnsignedShort();  //Y坐标
				obj.memName = bytes.readMultiByte(16, GameCommonData.CODE);//队友名字
				memberList.push(obj);
				
			}

			switch(unAction)
			{	
				case MSGTEAMPOS_POS:														
						if(memberList.length>0){
							sendNotification(EventList.SHOW_SCENE_TEAMINFO,memberList);
						}
					break;
				case MSG_TEAMMAPPOS_POS:
						if(memberList.length>0){
							sendNotification(PlayerInfoComList.MSG_TEAMPOS_PROCESS,memberList);
						}
					break;
					
				default:
//					trace("其他命令-", obj.unAction);
					break;
			}
		}
	}
}
