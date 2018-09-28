package GameUI.Modules.RoleProperty.Net
{
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.OperatorItemSend;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	public class NetAction
	{
		public static function RequestOutfit():void
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
			obj.data.push(PlayerAction.GET_OUTFIT);
			obj.data.push(0);
			obj.data.push(0);
			PlayerActionSend.PlayerAction(obj);
		}
		
		public static function UseItem(op:int, count:int, pos:int, itemID:int):void
		{
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
		
		public static function UnEquip(op:int, count:int, pos:int, packageIndex:int=47, position:int=1):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(op);
			obj.data.push(count);
			obj.data.push(pos);
			obj.data.push(0);
			obj.data.push(packageIndex);
			obj.data.push(position);
			obj.data.push("");
			//
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		public static function LevelUp(leveltype:uint):void
		{
			var obj:Object = new Object();
			var parm:Array = new Array;
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(leveltype);   //升级方式 0角色升级  1主职业  2副职业
			parm.push(0);
			parm.push(PlayerActionSend.LEVELUP);	//升级
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		public static function AddPotential(type:uint, num:uint=0):void
		{
			var obj:Object = new Object();
			var parm:Array = new Array;
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(type);   //加点方式 
			parm.push(num);
			parm.push(PlayerActionSend.ADDPotential);	//加点
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 获取职业信息 
		 * @param type
		 * 
		 */		
		public static function GetRoleInfo(type:uint):void
		{
			/** 0:当前职业 1:主职业 2:副职业  */
			var obj:Object = new Object();
			var parm:Array = new Array;
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);   
			parm.push(type);	//职业
			parm.push(PlayerActionSend.GETROLEINFO);	
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 请求其他人物信息 
		 * 
		 */		
		public static function RequestOtherInfo():void
		{
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);			
			parm.push(286);							   
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
	}
}