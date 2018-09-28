package Net.ActionProcessor
{
	import GameUI.Modules.CompensateStorage.data.CompensateStorageConst;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.Modules.CompensateStorage.mediator.CompensateStorageMediator;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class CompensateStorageAction extends GameAction
	{
		public static const GET_ITEMLIST:uint	 =	1;		//获取物品列表
		public static const GET_ITEMLIST_COMPLETE:uint	 =	2;		//获取物品列表完成
		public static const GET_LOG_COMPLETE:uint	 =	3;		//获取补偿日志完成
		public static const GET_COMPENSATE_FAIL:uint	 =	5;		//获取补偿失败
		public static const GET_COMPENSATE_SUCCEED:uint	 =	6;		//获取补偿完成
		
		public function CompensateStorageAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			
			var action:int = bytes.readShort();		// 动做编号
			var usAmount:uint = bytes.readShort();		// 补偿物品数量
			
			var arr:Array = [];
			var i:int = 0;
			for(i = 0; i < usAmount; i++)
			{
				var obj:Object = new Object();
				obj.id = bytes.readUnsignedInt();
				obj.type = bytes.readUnsignedInt();
				obj.nAmount = bytes.readUnsignedInt();
				arr.push(obj);
			}
			switch(action)
			{
				case GET_ITEMLIST:
					if(!facade.hasMediator(CompensateStorageMediator.NAME))
					{
						facade.registerMediator(new CompensateStorageMediator());
					}
					CompensateStorageData.dataList = CompensateStorageData.dataList.concat(arr);
					break;
				case GET_ITEMLIST_COMPLETE:
					CompensateStorageData.setListData();
					sendNotification(CompensateStorageConst.SHOW_COMPENSATESTORGE_VIEW);
					break;
				case GET_LOG_COMPLETE:
					CompensateStorageData.isEmptyOrUpdata = false;
					sendNotification(CompensateStorageConst.SHOW_COMPENSATESTORGE_DETAILS_VIEW);
					break;
				case GET_COMPENSATE_FAIL:
					trace("GET_COMPENSATE_FAIL");
					break;
				case GET_COMPENSATE_SUCCEED:
					CompensateStorageData.isEmptyOrUpdata = true;
					var dataList:Array = CompensateStorageData.dataList;
					CompensateStorageData.clearListData();
					CompensateStorageData.dataList = CompensateStorageData.arraySub(dataList,arr,CompensateStorageData.compare);
					CompensateStorageData.setListData();
					sendNotification(CompensateStorageConst.SHOW_COMPENSATESTORGE_VIEW);
					break;
			}
		}
		
	}
}