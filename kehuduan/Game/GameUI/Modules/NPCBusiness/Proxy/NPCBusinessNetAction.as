package GameUI.Modules.NPCBusiness.Proxy
{
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.OperatorItemSend;
	import Net.Protocol;
	
	public class NPCBusinessNetAction
	{
		public function NPCBusinessNetAction()
		{
		}
		
		/** 购买NPC商店物品 */
		public static function buyNPCItem(itemTypeID:int, npcId:int, count:int, price:int=0):void
		{
			var obj:Object = new Object();
			obj.type = Protocol.OPERATE_ITEMS;
			obj.data = new Array();
			obj.data.push(OperateItem.BUYNPCITEM);
			obj.data.push(1);
			obj.data.push(4); 
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
			obj.data.push(price);		//2011.1.25 add by 冯
			obj.data.push(0);
			
			OperatorItemSend.PlayerAction(obj);
		}
		
		/** 出售 */
		public static function saleGood(itemID:int, npcId:int, price:int=0):void
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
			obj.data.push(price);			//2011.1.25 add by 冯
			obj.data.push(0);
			OperatorItemSend.PlayerAction(obj);
		}
		
	}
}