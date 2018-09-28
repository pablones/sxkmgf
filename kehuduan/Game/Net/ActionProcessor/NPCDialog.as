package Net.ActionProcessor
{
	import GameUI.Modules.NPCChat.Command.NPCChatComList;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class NPCDialog extends GameAction
	{
		
		public function NPCDialog(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position  = 4;
			var obj:Object  = {};
			var action:uint = bytes.readUnsignedInt();		//action
			var nData:uint  = bytes.readUnsignedInt();		//操作数
			
			var szText:String;
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int = 0;
			for(var i:int = 0;i < nDataSeeNum; i ++)
			{
				nDataSee = bytes.readUnsignedByte();
//				nDataSee = bytes.readByte();
				if(nDataSee != 0)
				{		
					if(i==0)
					{
						szText = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); 
					}
				}		
			}
			
			obj.action=action;
			obj.nData=nData;
			obj.szText=szText;
			
			sendNotification(NPCChatComList.RECEIVE_NPC_MSG,obj);
		}
		
	}
}