package GameUI.Modules.Designation.Data
{
	import flash.events.Event;

	public class DesignationEvent extends Event
	{
		public static const DESIGNATION_EVENT:String = "designatinEvent";  //小条目点击触发事件
		public static const OPEN_DESIGNATION_PANEL:String = "openDesignationPanel"; //打开面板
		public static const UPDATE_DATAARR:String = "updateDataArr";   //获取玩家当前的称号信息
		public static const DEAL_AFTER_UPDATE:String = "dealAfterUpdate"; //跟新称号后，看是否还拥有头顶的称号
		
		public var _data:Object;
		public function DesignationEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false,data:Object = null)
		{
			this._data = data;
			super(type, bubbles, cancelable);
		}
		public override function clone():Event
		{
			return new DesignationEvent(type,bubbles,cancelable,_data);
		} 
		
	}
}      