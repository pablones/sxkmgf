package Net.ActionSend
{
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	
	public class GameSkillSend
	{
		public static function GetUseGameSkill(nPlayerID:int):void
		{
			var sendBytes:ByteArray = new ByteArray();
			var parm:Array = new Array;
			parm.push(0);
			parm.push(nPlayerID);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(28);
			parm.push(0);
	
			sendBytes.writeShort(32);
			sendBytes.writeShort(Protocol.MSG_S_GAMESKILL);
			
			sendBytes.writeUnsignedInt(parm.shift());
        	sendBytes.writeUnsignedInt(parm.shift());
        	sendBytes.writeShort(parm.shift());
	        sendBytes.writeShort(parm.shift());
	        sendBytes.writeShort(parm.shift());

	        sendBytes.writeShort(0); //对齐
			
	        sendBytes.writeUnsignedInt(parm.shift()); 
				
	        sendBytes.writeShort(parm.shift());
         	sendBytes.writeShort(0); //对齐
	        sendBytes.writeUnsignedInt(parm.shift());
	        
	        GameCommonData.AccNets.Send(sendBytes);
	        
		}
				

	}
}