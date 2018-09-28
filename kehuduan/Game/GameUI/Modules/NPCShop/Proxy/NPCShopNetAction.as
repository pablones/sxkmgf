package GameUI.Modules.NPCShop.Proxy
{
	import GameUI.Modules.NPCShop.Data.NPCShopConstData;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.OperatorItemSend;
	import Net.Protocol;
	
	public class NPCShopNetAction
	{
		public function NPCShopNetAction()
		{
		}
		
		/** 购买NPC商店物品 */
		public static function buyNPCItem(itemTypeID:int, npcId:int, count:int):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.BUYNPCITEM);
			obj.data.push(1);
			obj.data.push(NPCShopConstData.payWay); 			//支付方式
			obj.data.push(npcId);		//NPC id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");			//归属人 name
			
			obj.data.push(0);			//物品id
			obj.data.push(itemTypeID);
			obj.data.push(count);		//购买数量
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		/** 单修 */
		public static function singleRepare(itemID:int, npcId:int):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.SINGLEREPARE);
			obj.data.push(1);
			obj.data.push(0);
			obj.data.push(npcId);		//NPC id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");			//归属人 name
			
			obj.data.push(itemID);		//物品id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		/** 群修 */
		public static function multiRepare(npcId:int):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.MULTIREPARE);
			obj.data.push(1);
			obj.data.push(0);
			obj.data.push(npcId);		//NPC id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");			//归属人 name
			
			obj.data.push(0);			//物品id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
		/** 出售 */
		public static function saleGood(itemID:int, npcId:int):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.SALEGOOD);
			obj.data.push(1);
			obj.data.push(0);
			obj.data.push(npcId);		//NPC id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push("");			//归属人 name
			
			obj.data.push(itemID);		//物品id
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
	}
}