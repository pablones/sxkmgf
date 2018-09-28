package GameUI.Modules.IdentifyTreasure.Command
{
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Proxy.DataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ShowTreasureCommand extends SimpleCommand
	{
		public static const NAME:String = "ShowTreasureCommand";
		private var dataProxy:DataProxy;
		
		public function ShowTreasureCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			//屏蔽北京ip
			if ( GameCommonData.cztype == 1 )
			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"此功能暂未开放", color:0xffff00});	
				return;
			}
			if ( GameCommonData.openTreasureStragety == 1 )
			{
				if ( !TreasureData.TreaResourceLoaded )
				{
					sendNotification( TreasureData.LOAD_TREASURE_RES );
				}
				else
				{
//					if ( dataProxy.treasurePanelIsOpen )
//					{
//						sendNotification( TreasureData.CLOSE_TREASURE_UI );
//					}	
//					else
//					{
						sendNotification( TreasureData.SHOW_TREASURE_UI );
//					}
				}
			}
			else if ( GameCommonData.openTreasureStragety == 2 )
			{ 
				if ( TreasureData.packageDateArr.length == 0 )
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_ide_com_sho_exe" ], color:0xffff00});//"宝物包裹无任何物品"	
					return;
				}
				if ( !TreasureData.TreaResourceLoaded )
				{
					sendNotification( TreasureData.LOAD_TREASURE_RES );
				}
				else
				{
//					if ( dataProxy.treasurePackageIsOpen )
//					{
//						sendNotification( TreasureData.CLOSE_MY_TREA_PACKAGE );
//					}	
//					else
//					{
						sendNotification( TreasureData.OPEN_MY_TREA_PACKAGE );
//					}
				}
			}
//			else if ( GameCommonData.openTreasureStragety == 3 )
//			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"此功能暂未开放", color:0xffff00});	
//			}
		}
		
	}
}