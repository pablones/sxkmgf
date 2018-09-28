package GameUI.Modules.Bag.Command
{
	import GameUI.Modules.Bag.BagUtils;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Mediator.SplitItemMediator;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SplitCommand extends SimpleCommand
	{
		public static const NAME:String = "SplitCommand";
		private var splitMediator:SplitItemMediator;
		
		public function SplitCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			if(BagData.SelectedItem)
			{
				if(BagData.SelectedItem.Item.Type <= 300000 || BagData.SelectedItem.Item.Num == 1)
				{//此物品无法拆分
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_spl_exe_1" ], color:0xffff00});
					return;
				}
				if(BagUtils.TestBagIsFull(BagData.SelectIndex) == BagData.BagNum[BagData.SelectIndex]) 
				{//背包已满,无法拆分
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_bag_com_spl_exe_2" ], color:0xffff00});
					return;
				}
				if(splitMediator != null) splitMediator = null;
				splitMediator = new SplitItemMediator();
				facade.registerMediator(splitMediator);
				facade.sendNotification(BagEvents.SHOWSPLIT, BagData.SelectedItem);
			}
		}
		
	}
}