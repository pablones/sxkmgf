
  package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Trade.Data.TradeConstData;
	import GameUI.Modules.Trade.Data.TradeEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class TradeAction extends GameAction
	{
		public static const APPLY:uint			= 1;	//C-S,S-C	请求交易
		public static const AGREE_TRADE:uint	= 34;	//C-S		同意交易
		public static const QUIT:uint			= 2;	//C-S		取消交易
		public static const OPEN:uint			= 3;	//S-C		打开交易窗
		public static const SUCCESS:uint		= 4;	//S-C		交易成功(同时关闭)
		public static const FALSE:uint			= 5;	//S-C		交易失败(同时关闭)
		public static const ADDITEM:uint		= 6;	//C-S,S-C	自己加物品
		public static const ADDITEM_OP:uint     = 21;	//S-C		对方加了物品
		public static const ADDMONEY:uint		= 7;	//C-S		自己加钱
		public static const MONEYALL:uint		= 8;	//S-C		对方钱总数
		public static const SELFMONEYALL:uint	= 9;	//S-C		自己钱总钱
		public static const OK:uint				= 10;	//S-C		对方已确认交易
		public static const ADDITEMFAIL:uint	= 11;	//S-C		自己加物品失败
		public static const NOTALLOW:uint		= 12;	//			自己不允许交易
		public static const BACK_WU:uint		= 13;	//C-S S-C	自己减少物品
		public static const BACK_WU_OP:uint     = 22;	//S-C		减少物品   对方减了物品
		public static const BACK_MONEY:uint		= 14;	//
		public static const LOCK:uint			= 15;	//S-C		对方已锁定
		public static const ADDVAS:uint			= 16;	//			加vas
		public static const VASOTHER:uint		= 17;	//			对方vas
		public static const VASSELF:uint		= 18;	//			自己vas
		public static const REFUSE:uint			= 19;	//C-S		拒绝交易
		public static const DELITEMFAIL:uint    = 20;	//S-C 		减物品失败
		public static const UNLOCK:uint			= 23;	//S-C 		自己解锁
		public static const UNLOCK_OP:uint		= 24;	//S-C		对方解锁
		public static const OPISTRADING:uint	= 25;	//S-C		对方正在交易中
		
		public static const PET_ADD_SELF:uint			= 26;		//[2010-07-30 goto] 自己增加宠物	to server
		public static const PET_DEL_SELF:uint			= 30;		//[2010-07-30 goto] 自己减少宠物	to server
		
		public static const PET_ADD_SELF_SUCCESS:uint	= 28;		//[2010-07-30 goto] 加宠物成功	to client
		public static const PET_ADD_SELF_FAIL:uint		= 29;		//[2010-07-30 goto] 加宠物失败	to client
		public static const PET_DEL_SELF_SUCCESS:uint	= 31;		//[2010-07-30 goto] 减少宠物成功	to client
		public static const PET_DEL_SELF_FAIL:uint		= 32;		//[2010-07-30 goto] 减少宠物失败	to client
		public static const PET_DEL_OP:uint				= 33;		//[2010-07-30 goto] 对方减少宠物	to client

		public function TradeAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			var obj:Object = new Object();
			
			bytes.position=4;
			
			obj.id = bytes.readUnsignedInt();
			obj.type = bytes.readUnsignedInt();
			
			obj.amount = bytes.readUnsignedInt();
			obj.maxAmount = bytes.readUnsignedInt();
			
			obj.action = bytes.readShort();
			
			// id;		物品ID
			// nData;	物品种类编号
			// idType;	物品数量
			// type;	物品数量上限

//			trace("-----收到交易命令-------------");
//			trace("action:  ", obj.action);
//			trace("id: 		", obj.id);
//			trace("nData: 	", obj.nData);
//			trace("idType: 	", obj.idType);
//			trace("type: 	", obj.type);
//			trace("-----------------------------");
//			
			switch(obj.action) {
				case UNLOCK:
//					trace("自己解锁");
					sendNotification(EventList.TRADEUNLOCK);
					break;
				case UNLOCK_OP:
//					trace("对方解锁");
					sendNotification(EventList.TRADEUNLOCK_OP);
					break;
				case APPLY:													//有人请求与我交易		S-C
//					trace("有人申请交易我");
					sendNotification(TradeEvent.SOMEONETRADEME, obj.id);
					break;
				case OPEN:	
//					trace("OPEN开始交易");									//打开交易窗开始交易		S-C
					TradeConstData.traderId = obj.id;
					sendNotification(EventList.SHOWTRADE, obj); 
					break;
				case SUCCESS:	
//					trace("SUCCESS交易成功");								//交易成功				S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case FALSE:													//交易失败				S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case MONEYALL:	
//					trace("MONEYALL对方总钱数");								//对方总钱数  8			S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case SELFMONEYALL:	
//					trace("SELFMONEYALL自己总钱数");							//自己总钱数  9  			S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case OK:		
//					trace("OK对方已经确认");									//对方已确认交易	10  	S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case ADDITEMFAIL:											//自己物品添加失败  11    S-C
//					trace("ADDITEMFAIL自己添加物品失败");
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case BACK_WU:		
//					trace("BACK_WU自己减少物品");								//自己减少物品	13      S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case BACK_WU_OP:
//					trace("BACK_WU_OP对方减少物品");							//对方减物品		22		S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case LOCK:
//					trace("LOCK对方已锁定");									//对方已锁定   15			S-C
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case ADDITEM:												//自己加物品		6		S-C
//					trace("自己加物品");
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case ADDITEM_OP:	
//					trace("ADDITEM_OP对方加物品");							//对方加物品		21		S-C				
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case OPISTRADING:
//					trace("对方正在交易");
					sendNotification(TradeEvent.SHOWTRADEINFORMATION, {type:12, data:obj.id});
					sendNotification(EventList.UPDATETRADE, obj);
					break;
				case REFUSE:												//我被拒绝了				S-C
					sendNotification(TradeEvent.SOMEONEREFUSEME, obj.id);
					break;
				case PET_ADD_SELF_SUCCESS:									//自己加宠物成功
					sendNotification(TradeEvent.PET_ADD_SELF_TRADE, {result:true, petId:obj.id});
					break;
				case PET_ADD_SELF_FAIL:										//自己加宠物失败
					sendNotification(TradeEvent.PET_ADD_SELF_TRADE, {result:false, petId:obj.id});
					break;
				case PET_DEL_SELF_SUCCESS:									//自己删除宠物成功
					sendNotification(TradeEvent.PET_DEL_SELF_TRADE,	{result:true, petId:obj.id});
					break;
				case PET_DEL_SELF_FAIL:										//自己删除宠物失败
					sendNotification(TradeEvent.PET_DEL_SELF_TRADE, {result:false, petId:obj.id});
					break;
				case PET_DEL_OP:											//对方删除宠物
					sendNotification(TradeEvent.PET_DEL_OP_TRADE,	{petId:obj.id});
					break;
//-------------------------------------------------------------------------------------------------------------------------
//以下暂时没有用到
				case ADDMONEY:												//自己加钱				C-S
					
					break;
				case QUIT:													//我取消交易				C-S   
//					trace("取消交易");
					break;
				case DELITEMFAIL:											//减物品失败				S-C
//					trace("自己减物品失败");
					break;
				case NOTALLOW:												//自己不允许交易	 （未用到）
					
					break;
				case BACK_MONEY:											//对方减少钱   14（未用到
					
					break;
				case ADDVAS:
					
					break;
				case VASOTHER:	
					
					break;
				case VASSELF:	
					
					break;
				default:
				
					break;
			}
		}
		
		
	}
}
