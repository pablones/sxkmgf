package Net.ActionProcessor
{
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Maket.Data.MarketEvent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class MarketAction extends GameAction
	{
		public static var MARKET_BUY:uint = 1003;		/** 购买商品 */
		
//		_MSGVAS_QUERY_BALANCE	=	1001,//查询余额 C-S
//		_MSGVAS_RETURN_BALANCE	=	2002,//账号服务器返回余额
//		_MSGVAS_BUY_ITEM		=	1003,//购买商城物品 C-S
//		_MSGVAS_BUY_SUC			=	2004,//购买成功
//		_MSGVAS_BUY_FAIL		=	2005,//购买失败
		
		public function MarketAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(endByteArr:ByteArray):void 
		{
			endByteArr.position = 8;
			
			var VASPoint:int = endByteArr.readUnsignedInt();	//元宝余额
			var idAccount:uint = endByteArr.readUnsignedInt();	//账户ID
			var idUser:uint = endByteArr.readUnsignedInt();		//角色ID
			
			var action:int = endByteArr.readUnsignedShort();	//action
			var goodSize:int = endByteArr.readUnsignedShort();	//物品种类总数
			
			var goodObj:Object;
			var aDiscountObj:Array = [];										//储存打折物品数组
			if (goodSize > 0)
			{
				for (var i:int=0; i < goodSize; i++)
				{
					var itemTypeID:uint = endByteArr.readUnsignedInt();	//物品种类
					var itemAmount:uint = endByteArr.readUnsignedInt();	//物品数量
					var dwData:uint = endByteArr.readUnsignedInt();		//支付方式
					
					goodObj = new Object;
					goodObj.type = itemTypeID;
					goodObj.amount = itemAmount;
					goodObj.price = dwData;
					aDiscountObj.push( goodObj );
				}
			}
			
			switch(action)
			{
				case 2002:
					trace("我发了查询，服务器的返回，返回我的余额");
					break;
//				case 2004:
//						trace("购买成功"); 
//					break; 
				case 2005:
					sendNotification(EquipCommandList.BUY_STRENGENHELP_ITEM,1);
					break;
				case 314:
//					trace ( "打折物品返回成功！" );
					sendNotification( MarketEvent.REC_MARKET_DIS_GOOD,aDiscountObj );
					break; 
				case 315:
					sendNotification( MarketEvent.UPDATE_MARKET_GOODS_NUM,aDiscountObj );
					break;
			}
			
		}
		
	}
}