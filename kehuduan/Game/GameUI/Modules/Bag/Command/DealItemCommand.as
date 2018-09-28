package GameUI.Modules.Bag.Command
{
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.SetFrame;
	
	import Net.ActionProcessor.OperateItem;
	
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class DealItemCommand extends SimpleCommand
	{
		public static const NAME:String = "DealItemCommand";
		private var parent:MovieClip = null;
		public var comfrim:Function = comfrimFn;
		
		public function DealItemCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			parent = notification.getBody() as MovieClip;
			for(var i:int = 0; i<BagData.GridUnitList.length; i++)
			{
				if(BagData.GridUnitList[i].Item)
				{
					if(BagData.GridUnitList[i].Item.IsLock)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_dea_exe_1" ], color:0xffff00});//有物品已锁定
						return;
					}
				}
			}
			BagData.AllItems[BagData.SelectIndex] = new Array(36);
			SetFrame.RemoveFrame(parent);
			BagData.TmpIndex = -1;
			BagData.SelectedItem = null;
			facade.sendNotification(BagEvents.SHOWBTN, false);				
			OperateItem.IsOrder = true;
			NetAction.OperateItem(OperateItem.DEAL, 1, BagData.SelectIndex+47);
		} 
		
		//丢弃物品处理
		public function comfrimFn():void {  }
		
	}
}