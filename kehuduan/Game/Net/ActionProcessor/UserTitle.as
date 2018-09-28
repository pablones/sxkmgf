// ActionScript file
package Net.ActionProcessor
{
	import GameUI.Modules.Designation.Data.DesignationChangeCommand;
	import GameUI.Modules.Designation.Data.DesignationCommand;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class UserTitle extends GameAction
	{
		public static const MSG_SYNCHRO:uint	 =	1;		//同步称号
		public static const MSG_ADDTITLE:uint    =  2;     	//增加一个称号
		public static const MSG_UPDATETITLE:uint =  3; 		//称号升级
		public static const MSG_DELETETITLE:uint =  4;		//删除称号
		/////////////////////////// 
		
		public function UserTitle(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			var UserTitleList:Array=[];
			var idUser:uint = bytes.readUnsignedInt();		// 用户ID
			var uAction:uint = bytes.readUnsignedInt();		// Action
			var uAmount:uint = bytes.readUnsignedInt();		// 称号数量
			
			for (var i:int=0; i<uAmount; i++)
			{
				var obj:Object={};
				obj.titleType = bytes.readUnsignedInt();//称号类型
				obj.titleLev = bytes.readUnsignedInt();	//称号等级
				UserTitleList.push(obj);
			}
			switch(uAction)
			{	
				case MSG_SYNCHRO:			
					sendNotification(DesignationCommand.NAME,{type:MSG_SYNCHRO,value:UserTitleList});										
					break;
				case MSG_ADDTITLE:
					sendNotification(DesignationCommand.NAME,{type:MSG_ADDTITLE,value:UserTitleList});
					break;
				case MSG_UPDATETITLE:
					sendNotification(DesignationCommand.NAME,{type:MSG_UPDATETITLE,value:UserTitleList});
					break;
				case MSG_DELETETITLE:
					sendNotification(DesignationCommand.NAME,{type:MSG_DELETETITLE,value:UserTitleList});
					break;
				default:
					break;
			}
		}
	}
}

