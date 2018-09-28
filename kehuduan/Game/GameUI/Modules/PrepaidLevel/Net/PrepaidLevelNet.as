package GameUI.Modules.PrepaidLevel.Net
{
	import Net.ActionSend.PrepaidLevelSend;
	
	public class PrepaidLevelNet
	{
		//查询充值等级信息  1    //查询游历信息  10   //领取珠宝返还  13    //游历  15  //神游千里  17
		public static function sendPrepaidDemand( index:uint ):void
		{
			var obj:Object = new Object();
			var arr:Array = new Array();
			arr.push(GameCommonData.Player.Role.Id);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			
			obj.type = index;
			obj.data = arr;
			
			PrepaidLevelSend.Send( obj );
		}
		
		//获取礼包
		public static function sendGainGift( index:uint ):void
		{
			var obj:Object = new Object();
			var arr:Array = new Array();
			arr.push(GameCommonData.Player.Role.Id);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(index);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			
			obj.type = 5;
			obj.data = arr;
			
			PrepaidLevelSend.Send( obj );
		}
		
		//提交答案
		public static function sendAnswer( questionId:uint, index:uint ):void
		{
			var obj:Object = new Object();
			var arr:Array = new Array();
			arr.push(GameCommonData.Player.Role.Id);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(0);
			arr.push(questionId);
			arr.push(index);
			
			obj.type = 20;
			obj.data = arr;
			
			PrepaidLevelSend.Send( obj );
		}
	}
}