package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessConstData;
	import GameUI.Modules.NPCBusiness.Mediator.NPCBusinessMediator;
	import GameUI.Proxy.DataProxy;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	import flash.utils.Dictionary;

	public class NPCBusinessAction extends GameAction
	{
		public function NPCBusinessAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var obj:Object = {};
			if(dataProxy.NPCBusinessIsOpen) {
				sendNotification(EventList.CLOSE_NPC_BUSINESS_SHOP_VIEW);
			}
			
			bytes.position  = 4;
			var npcid:uint = bytes.readUnsignedInt();		//NPC id
			var shoptype:uint  = bytes.readUnsignedInt();	//商店类型
			var itemAmount:uint  = bytes.readUnsignedInt();	//物品数量
			
			for(var i:int = 0 ; i < 30 ; i ++)
			{
				obj = {};
				obj.type	 = bytes.readUnsignedInt();		//物品type
				obj.saleType = bytes.readUnsignedShort();	//买卖类型
				obj.amount 	 = bytes.readUnsignedShort();	//数量
				obj.price 	 = bytes.readUnsignedInt();		//价格
				if(i < itemAmount) {
					if(obj.saleType == 1) {	//用来买的
						NPCBusinessConstData.goodList.push(obj);
					} else {				//卖的商品价格
						NPCBusinessConstData.goodSalePriceDic[obj.type] = obj.price;
					}
				}
			}
			
//			var c:Array = NPCBusinessConstData.goodList;
//			var d:Dictionary = NPCBusinessConstData.goodSalePriceDic;
			
			var szName:String;								//商店名字
			var nDataSeeNum:int = bytes.readByte();			//？
			var nDataSee:int = 0;							//？
			for(var j:int = 0;j < nDataSeeNum; j ++)
			{
				nDataSee = bytes.readByte();
				if(nDataSee != 0)
				{
					if(j==0)
					{
						szName = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); 
					}
				}
			}

			facade.registerMediator(new NPCBusinessMediator());
			sendNotification(EventList.SHOW_NPC_BUSINESS_SHOP_VIEW, {npcId:npcid, shopType:shoptype, shopName:szName});
		}
	}
}