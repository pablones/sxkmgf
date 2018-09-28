package GameUI.Modules.Equipment.ui
{
	public interface IDataGridCell
	{
		function set cellData(value:Object):void;
		function dispose():void;
		function get cellWidth():uint;
		function get cellHeight():uint;
	}
}