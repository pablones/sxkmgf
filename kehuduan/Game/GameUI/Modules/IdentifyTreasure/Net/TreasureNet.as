package GameUI.Modules.IdentifyTreasure.Net
{
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	public class TreasureNet
	{
		//发送开始挑战
		public static function sendChallenge( times:int,page:int ):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(times);
			parm.push(302);							//301请求宝物包裹      302摇奖
			parm.push(page);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		//取出物品    id 为0 全部取出
		public static function quchuItems( _id:int ):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push( _id );
			parm.push(305);							//301请求宝物包裹      302摇奖
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		//丢弃物品
		public static function diuqiItems( _id:int ):void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push( _id );
			parm.push(306);							//301请求宝物包裹      302摇奖
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		//请求包裹信息
		public static function requestTreaPack():void
		{
//			trace ( "请求包裹的宝物信息！！！！！！" );
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(301);		
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
	}
}