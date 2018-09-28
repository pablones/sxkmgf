package GameUI.Modules.Depot.Proxy
{
	import Net.ActionSend.DepotSend;
	import Net.Protocol;
	
	/**
	 * 仓库向服务器发送命令
	 * @
	 * @
	 */ 
	public class DepotNetAction
	{
		public function DepotNetAction()
		{
			
		}
		
		/**
		 * @param id     物品id
		 * @param type   交换位置时使用,目标位置
		 * @param index  页码
		 * @param action 标识字段
		 */ 
		public static function sendDepotOrder(id:int, type:int=0, index:int=0, action:int=0):void
		{
			var o:Object = new Object();
			var data:Array = new Array();
			o.data = data;
			
			o.type = Protocol.DEPOT_OPERATE;
			data.push(id);
			data.push(type);
			data.push(0);
			data.push(0);
			data.push(10);
			data.push(0);
			data.push(index);
			data.push(action);
			
			DepotSend.createMsgDepot(o);
		}
		
	}
}