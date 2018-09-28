package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.NPCShop.Data.NPCShopConstData;
	import GameUI.Modules.NPCShop.Mediator.NPCShopMediator;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class NPCShopAction extends GameAction
	{
		public function NPCShopAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			if(dataProxy.NPCShopIsOpen) {
				sendNotification(EventList.CLOSENPCSHOPVIEW);
			}
			
			bytes.position  = 4;
			var obj:Object  = new Object();
			var npcid:uint = bytes.readUnsignedInt();		//NPC id
			var shoptype:uint  = bytes.readUnsignedInt();	//商店类型
			var itemAmount:uint  = bytes.readUnsignedInt();	//物品数量
			
			var tmpTypeArr:Array = [];
			
			for(var i:int = 0 ; i < 60 ; i ++)
			{
				var itemType:uint  = bytes.readUnsignedInt();
				if(i < itemAmount) {
//					trace("itemType:", itemType);
					tmpTypeArr.push(itemType);
				}
			}
			NPCShopConstData.goodTypeIdList = UIUtils.arrSortByLev(tmpTypeArr);
//			trace("NPCShop  action " +NPCShopConstData.goodTypeIdList);
			
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
			
//			trace("szName:", szName);
//			trace("shoptype:", shoptype);
//			
			facade.registerMediator(new NPCShopMediator());
			
			sendNotification(EventList.SHOWNPCSHOPVIEW, {npcId:npcid, shopType:shoptype, shopName:szName});
		}		
	}
}