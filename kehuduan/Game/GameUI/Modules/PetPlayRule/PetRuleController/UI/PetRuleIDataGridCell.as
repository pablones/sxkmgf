package GameUI.Modules.PetPlayRule.PetRuleController.UI
{
	public interface PetRuleIDataGridCell
	{
		function set cellData(value:Object):void;
		function dispose():void;
		function get cellWidth():uint;
		function get cellHeight():uint;
	}
}