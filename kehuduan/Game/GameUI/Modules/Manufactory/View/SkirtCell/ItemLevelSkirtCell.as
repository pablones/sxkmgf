package GameUI.Modules.Manufactory.View.SkirtCell
{
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.UICore.UIFacade;
	
	import flash.events.MouseEvent;

	public class ItemLevelSkirtCell extends ItemSkirtCell
	{
		public function ItemLevelSkirtCell(sName:String)
		{
			super(sName);
		}
		
		protected override function clickCellHandler(evt:MouseEvent):void
		{
			var obj:Object = new Object();
			obj.limit = "level";
			obj.content = this.contentStr;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( ManufactoryData.CLICK_MANU_SKIRT_CELL,obj );
		}
		
	}
}