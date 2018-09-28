package GameUI.Modules.Maket.Proxy
{
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.MarketSend;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	public class MarketNetAction
	{
		public function MarketNetAction()
		{
		}
		
		/** 商城操作(购买商品) */
		public static function opMarket(obj:Object):void
		{
//			MarketSend.createMsg(null);
			MarketSend.newCreateMsg( null );
		}
		
		/** 外部购买商品接口 */
		public static function buyItem(action:uint, type:uint, count:uint, payType:uint):void
		{
			var o:Object = new Object();
			
			o.action  = action;
			o.type 	  = type;
			o.count   = count;
			o.payType = payType;
			
			MarketSend.buyItem(o);
		}
		
		/** 查询玩家元宝 */
		public static function queryYB():void
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
			obj.data.push(PlayerAction.QUERY_MONEY);
			obj.data.push(0);
			//obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
	}
}