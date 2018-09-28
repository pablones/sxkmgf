package GameUI.Modules.Friend.command
{
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Friend.view.ui.MenuItemCell;
	
	import flash.events.Event;

	/**
	 * 菜单自定义事件类 
	 * @author felix
	 * 
	 */	
	public class MenuEvent extends Event
	{
		public static const Cell_Click:String="Cell_Click";
		public static const Cell_RollOver:String="Cell_RollOver";
		public static const Cell_RollOut:String="Cell_RollOut";
		
		public static const CELL_DOUBLE_CLICK:String="cellDoubleClick";
		/** 事件源 */
		public var cell:MenuItemCell;
		/** 玩家信息 */
		public var roleInfo:PlayerInfoStruct;
		
		public function MenuEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false,menuItemCell:MenuItemCell=null,info:PlayerInfoStruct=null)
		{
			super(type, bubbles, cancelable);
			this.cell=menuItemCell;
			this.roleInfo=info;
		}
	}
}