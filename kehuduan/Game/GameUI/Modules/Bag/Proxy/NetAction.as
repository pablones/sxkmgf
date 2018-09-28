package GameUI.Modules.Bag.Proxy
{
	import GameUI.Modules.Meridians.tools.Tools;
	
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.OperatorItemSend;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import OopsFramework.Debug.Logger;
	
	public class NetAction
	{
		public static function RequestItems():void
		{
			Logger.Info(NetAction,"RequestItems","请求背包信息");
			
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(PlayerAction.OPERATE_ITEM);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		
		/**
		 * 请求宠物信息
		 * @param action 269：请求宠物
		 */		
		public static function requestPet():void
		{
			Logger.Info(NetAction,"requestPet","请求宠物信息");
			
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(269);
			obj.data.push(0);
			//obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 请求技能信息 
		 * 
		 */		
		public static function requestSkill():void
		{
			Logger.Info(NetAction,"requestSkill","请求技能信息");
			
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(270);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		
		/**
		 * 请求快捷键信息 
		 * 
		 */		
		public static function requestQuickKey():void
		{
			Logger.Info(NetAction,"requestQuickKey","请求快捷键信息");
		
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(271);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/** 请求Cd */		
		public static function requestCd():void
		{
			Logger.Info(NetAction,"requestCd","请求CD信息");

			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(272);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/** 请求任务 */		
		public static function requestTask():void
		{
			Logger.Info(NetAction,"requestTask","请求Task信息");
		
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(273);
			obj.data.push(0);
			//obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 请求其它杂项 
		 * 
		 */		
		public static function requestOther():void
		{
			Logger.Info(NetAction,"requestOther","请求Other信息");
	
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(274);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 请求BUFF信息 
		 * 
		 */		
		public static function requestBuff():void
		{
			Logger.Info(NetAction,"requestBuff","请求buff信息");
		
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(275);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		
		/** 
		 * op 操作符
		 * count  操作物品数量 
		 * 
		 * */
		public static function OperateItem(op:int, count:int, itemProp:int, operateItem:Object = null):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(op);
			obj.data.push(count);
			obj.data.push(itemProp);
			obj.data.push(0);				//归属人 id    //使用物品时如果是给谁送物品，这个字段是目标人物的ID
			obj.data.push(0);
			obj.data.push(0);				
			obj.data.push("");				//归属人 name
			//
			if(!operateItem)
			{
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
				OperatorItemSend.PlayerAction(obj);
			}
		}
		
		public static function UseItem(op:int, count:int, pos:int, itemID:int):void
		{
			Logger.Info(NetAction,"UseItem","使用血瓶");
						
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(op);
			obj.data.push(count);
			obj.data.push(pos);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");
			
			obj.data.push(itemID);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		/**
		 * 给好友赠送物品以增加友好度 
		 * @param itmeId ：物品ID
		 * @param FriendId ： 好友ID
		 * @param pos ：物品位置
		 * 
		 */		
		
		public static function presentRoseToFriend(itmeId:uint,FriendId:uint,pos:int):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(4);
			obj.data.push(1);
			obj.data.push(pos);
			obj.data.push(FriendId);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");
			
			obj.data.push(itmeId);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		
		/**
		 * 请求自动挂机信息 
		 * 
		 */		
		public static function requestAutoPlay():void{
			Logger.Info(NetAction,"requestAutoPlay","请求自动挂机信息");
		
			var obj:Object = new Object();
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = new Array();
			obj.data.push(0);
			obj.data.push(GameCommonData.Player.Role.Id);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(PlayerAction.REQUEST_AUTO_FIGHT_INFO); 
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		/**
		 * 请求称号信息
		 **/
		 public static function requestDesignation():void
		 {
		 	Logger.Info(NetAction,"requestDesignation","请求称号信息");
		 	
		 	var obj:Object = {};
		 	obj.type = Protocol.PLAYER_ACTION;
		 	var _data:Array = [];
		 	_data.push(0);
		 	_data.push(GameCommonData.Player.Role.Id);
		 	_data.push(0);
		 	_data.push(0);
		 	_data.push(0);
		 	_data.push(0);
		 	_data.push(PlayerAction.REQUEST_DESIGNATION_INFO);
		 	_data.push(0);
		 	obj.data = _data;
		 	PlayerActionSend.PlayerAction(obj);
		 }
		 
		 public static function requestMeridiansData():void
		 {
		 	if(GameCommonData.Player.Role.Level >= 13)
			{
		 		Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,140);		//向服务器发送查询经脉信息
		 	}
		 }
	}
}