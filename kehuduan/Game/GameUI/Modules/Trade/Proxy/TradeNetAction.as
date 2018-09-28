package GameUI.Modules.Trade.Proxy
{
	import Net.ActionSend.TradeSend;
	import Net.Protocol;
	
	public class TradeNetAction
	{
		public function TradeNetAction()
		{
		}
		
		/** 交易命令 */
		public static function tradeOperate(action:int, id:uint):void
		{
			var o:Object = new Object();
			var param:Array = new Array();
			o.type = Protocol.TRADE_INFO;
			o.data = param;
			
			param.push(id);		//id  (编号或钱数)
			
			param.push(0);		//type
			param.push(0);		//amount 
			param.push(0);		//maxAmount
			param.push(action);	//action
			
			TradeSend.createMsgTeam(o);
		}

	}
}