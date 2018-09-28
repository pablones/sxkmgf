package GameUI.Modules.Equipment.ui
{
	public interface IListCell
	{
		
			function set cellData(value:Object):void;
			function dispose():void;
			function set cellWidth(value:uint):void;
			function set cellHeight(value:uint):void;
			function get cellWidth():uint;
			function get cellHeight():uint;
		

	}
}