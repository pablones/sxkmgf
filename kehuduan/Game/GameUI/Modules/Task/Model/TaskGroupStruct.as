package GameUI.Modules.Task.Model
{
	import GameUI.Mediator.UiNetAction;
	
	import flash.events.EventDispatcher;
	import flash.utils.Dictionary;

	public class TaskGroupStruct extends EventDispatcher
	{
		public var taskDic:Dictionary;
		public var isExpand:Boolean;
		public var des:String;
		public var desColor:uint=0xffffff;
			
		public function TaskGroupStruct(taskDic:Dictionary,isExpand:Boolean,des:String,desColor:uint=0xffffff)
		{
			super(null);
			this.des=des;
			this.desColor=desColor;
			this.taskDic=taskDic;
			this.isExpand=isExpand;
			
		}
		
	}
}