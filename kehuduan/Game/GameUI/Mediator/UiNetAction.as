package GameUI.Mediator
{
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.OperatorItemSend;
	import Net.Protocol;
	
	public class UiNetAction
	{
		public function UiNetAction()
		{
		}
		
		public static function GetItemInfo(itemID:int, playerID:int, playerName:String=""):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.GETINFO);
			obj.data.push(1);
			obj.data.push(0);
			obj.data.push(playerID);		//归属人 id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(playerName);		//归属人 name
			
			obj.data.push(itemID);			//物品id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}

	}
}