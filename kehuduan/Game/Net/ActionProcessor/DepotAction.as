package Net.ActionProcessor
{
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class DepotAction extends GameAction
	{
		public function DepotAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public static const QUERY_LIST:uint   	= 0;			//请求某页所有物品，有返回
		public static const ITEM_IN:uint 	 	= 1;			//存入物品，有返回
		public static const ITEM_OUT:uint     	= 2;			//取出物品，有返回
		public static const MONEY_AMOUNT:uint	= 7;			//金钱结束符
		public static const MONEY_IN:uint	  	= 5;			//存钱
		public static const MONEY_OUT:uint	 	= 6;			//取钱
		public static const MOVE_POS:uint	 	= 8;			//仓库物品移到空位置
		public static const TRADE_POS:uint	 	= 9;			//仓库内物品交换位置
		
		public static const PET_IN_DEPOT:uint	= 10;			//向仓库存入宠物
		public static const PET_OUT_DEPOT:uint	= 11;			//向仓库取出宠物
		
		/** 下表减除的数量 */
		public static var indexDelCount:int = 0;
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			
			var obj:Object  = new Object();
			obj.id		  = bytes.readUnsignedInt();				//id
			obj.type 	  = bytes.readUnsignedInt();				//类型   300001
			obj.amount    = bytes.readUnsignedInt();				//数量(持久)			
			obj.maxAmount = bytes.readUnsignedInt();				//最大数量（最大持久）
			obj.position  = bytes.readUnsignedByte() ;				//位置
			obj.isBind 	  = bytes.readUnsignedByte();				//是否绑定
			obj.index 	  = bytes.readUnsignedShort();				//物品下标(页码)  从1开始
			
			var action:uint = bytes.readUnsignedByte();				//action
			
//			trace("==仓库=========================================");
//			trace("action:", action);
//			trace("id:", obj.id);
//			trace("type:", obj.type);
//			trace("amount:", obj.amount);
//			trace("maxAmount:", obj.maxAmount);
//			trace("position:", obj.position);
//			trace("isBind:", obj.isBind);
//			trace("index:", obj.index);
//			trace("===========================================");
			
			indexDelCount = DepotConstData.curDepotIndex * 36; 
			
			switch(action) {
				case 0:												//请求某页所有物品
					obj = dealBigDrug(obj);		//处理大药
					DepotConstData.goodList[obj.index -1 - indexDelCount] = obj;
					break;
				case 1:												//存入物品
					obj = dealBigDrug(obj);		//处理大药
					var index:int = obj.index -1 - indexDelCount
					DepotConstData.goodList[index] = obj;
					sendNotification(DepotEvent.ADDITEM, index);
					break;
				case 2:												//取出物品
					DepotConstData.goodList[obj.index -1 - indexDelCount] = null;
					sendNotification(DepotEvent.DELITEM, obj.id);
					break;
				case 7:												//金钱结束符
					DepotConstData.moneyDepot = obj.id;
					sendNotification(DepotEvent.INITMONEY);
					break;
				case 5:												//存钱
					DepotConstData.moneyDepot = obj.id;
					sendNotification(DepotEvent.UPDATEMONEY);
					break;
				case 6:												//取钱
					DepotConstData.moneyDepot = obj.id;
					sendNotification(DepotEvent.UPDATEMONEY);
					break;
			}
		}
		
		/** 处理大药 */
		private function dealBigDrug(obj:Object):Object
		{
			if(obj.type == 351001) 	//元宝票
			{
				obj.amountMoney = obj.amount;
				obj.amount = 1;
			}
			var typeAdd:uint = obj.type / 1000;
			if(typeAdd == 301 || typeAdd == 311 || typeAdd == 321) {	//大血大蓝	(客户端添加剩余血量)
				obj.noUse = obj.amount;			//剩余血魔
				obj.maxUse = obj.maxAmount;		//血魔总量
				obj.amount = 1;
				obj.maxAmount = 1;
			}
			return obj;
		}
		
		
		
	}
}