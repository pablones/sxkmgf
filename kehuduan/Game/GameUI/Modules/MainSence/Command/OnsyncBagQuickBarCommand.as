package GameUI.Modules.MainSence.Command
{
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.MainSence.Proxy.QuickGridManager;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Proxy.DataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class OnsyncBagQuickBarCommand extends SimpleCommand
	{
		public function OnsyncBagQuickBarCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var quickGridManager:QuickGridManager=facade.retrieveProxy(QuickGridManager.NAME) as QuickGridManager;	
			quickGridManager.onSyncBag();
			sendNotification(AutoPlayEventList.ONSYN_BAG_NUM,notification.getBody());
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			if(dataProxy.equipPanelIsOpen){
				sendNotification(EquipCommandList.REFRESH_HELP_ITEM);
				var obj:Object=notification.getBody();
				if(obj){
					if(obj==610048 || obj==610018 ||obj==610019 ||obj==610016 ||obj==610017){
						sendNotification(EquipCommandList.BUY_STRENGENHELP_ITEM);
					}
					
					if(obj==610047){
						sendNotification(EquipCommandList.CHANGE_HUNYUN_ITEM);
					}
					
					//打孔所购买的东西
					if(obj>=610000 && obj<=610011){
						sendNotification(EquipCommandList.BUY_STRENGENHELP_ITEM);
					}
				}
				
			}
			
			if(dataProxy.petRuleIsOpen) {
				sendNotification(PetRuleEvent.UPDATE_ITEMS_PET_RULE);  //同步宠物
			}
			
		}
	}
}