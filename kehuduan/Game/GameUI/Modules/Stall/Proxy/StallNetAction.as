package GameUI.Modules.Stall.Proxy
{
	import Net.ActionSend.Chat;
	import Net.ActionSend.OperatorItemSend;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	public class StallNetAction
	{
		public function StallNetAction()
		{
		}
		
		public static function OperateStall(action:int):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(action);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/** 
		 * op 操作符
		 * count  操作物品数量 
		 * 
		 * */
		public static function OperateItem(action:int, count:int, price:int, id:int, operateItem:Object = null):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(action);
			obj.data.push(count);
			obj.data.push(price);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");
			//
			if(!operateItem)
			{
				obj.data.push(id);
				obj.data.push(0);
				obj.data.push(0);
				obj.data.push(0);
				obj.data.push(0);
				obj.data.push(0);
				obj.data.push(0);
				obj.data.push(0);
				OperatorItemSend.PlayerAction(obj);
			}
			else
			{
				obj.data.push(operateItem.id);
				obj.data.push(operateItem.type);
				obj.data.push(operateItem.amount);
				obj.data.push(operateItem.maxAmount);
				obj.data.push(operateItem.position);
				obj.data.push(operateItem.isBind);
				obj.data.push(operateItem.index);
				obj.data.push(operateItem.price);
				OperatorItemSend.PlayerAction(obj);
			}
		}
		
		public static function OperateMsg(action:int, msgStr:String, stallId:int):void
		{
			var obj:Object = new Object();
			var parm:Array = new Array();
			
			obj.type = Protocol.PLAYER_CHAT;
			obj.data = parm;
			
			parm.push(GameCommonData.Player.Role.Name);
			parm.push("SYSTEM");
			parm.push("");
			parm.push(msgStr);		//文字
			parm.push("");
			parm.push(stallId);		//摊位ID
			parm.push(action);		//action
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			Chat.SendChat(obj);
		}
		

	}
}