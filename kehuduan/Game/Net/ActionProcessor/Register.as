package Net.ActionProcessor
{
	import GameUI.Modules.Login.Data.CreateRoleEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class Register extends GameAction
	{
		public function Register(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;

			var userName:String = bytes.readMultiByte(16,GameCommonData.CODE);//姓名
			var userPwd:String = bytes.readMultiByte(16,GameCommonData.CODE);
			var unAction:uint = bytes.readUnsignedInt();		// Action
			var ucLook:uint = bytes.readShort();				// 头像
			var ucGender:uint = bytes.readShort();				// 性别
			facade.sendNotification(CreateRoleEvent.ADDMESSAGE , {sex:ucGender , name:userName});
		}
		
	}
}