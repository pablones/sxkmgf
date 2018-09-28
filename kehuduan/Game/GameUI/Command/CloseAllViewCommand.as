package GameUI.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Artifice.data.ArtificeConst;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.CompensateStorage.data.CompensateStorageConst;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferConst;
	import GameUI.UICore.UIFacade;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;
	
	public class CloseAllViewCommand extends SimpleCommand
	{
		public static const NAME:String = "CloseAllViewCommand";
		public function CloseAllViewCommand()
		{
			super();
			this.initializeNotifier(UIFacade.FACADEKEY);
		}
		
		public override function execute(notification:INotification):void 
		{
			/** 关闭所有NPC */
			sendNotification(EventList.CLOSE_NPC_ALL_PANEL);
			/** 关闭补偿仓库 */
			sendNotification(CompensateStorageConst.CLOSE_COMPENSATESTORGE_VIEW);
			/** 关闭装备炼化 */
			sendNotification(ArtificeConst.CLOSE_ARTIFICE_VIEW);
			/** 关闭装备强化转移 */
			sendNotification(StrengthenTransferConst.CLOSE_STRENGTHENTRANSFER_VIEW);
		} 

	}
}