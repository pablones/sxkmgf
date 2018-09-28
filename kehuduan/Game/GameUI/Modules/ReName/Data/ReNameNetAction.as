package GameUI.Modules.ReName.Data
{
	import Net.ActionSend.EncryptSend;
	
	public class ReNameNetAction
	{
		public static function sendReName(reName:String, index:int):void
		{
			
			var obj:Object = new Object();
			obj.type = 5;
			obj.index = index;                              //0为个人改名， 1为帮派改名
			obj.reName = reName;                            //名字要改为*****
			EncryptSend.send(obj);
		}
	}
}

