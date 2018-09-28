package GameUI.Modules.Task.View
{
	import GameUI.Modules.Chat.UI.FaceText;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.View.Components.UISprite;

	public class TaskFollowCell extends UISprite
	{
		protected var head:TaskText;
		protected var content:TaskText;
		protected var taskInfo:TaskInfoStruct;
		protected var padding:uint=10;
		
		public function TaskFollowCell(taskInfo:TaskInfoStruct)
		{
			super();
			this.taskInfo=taslInfo;
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.head=new FaceText();
			this.head.tfText='<font color="#fffe65">'+taskInfo.taskName+'</font><font>     </font>'+'<font color="#00ff00">['+taskInfo.taskLevel+']</font>';
			this.addChild(this.head);
			this.content=new TaskText();
			if(taskInfo.taskProcess1!=""){
				this.content.tfText=taskInfo.taskProcess1;
			}
			
			this.addChild(this.content);	
		}
		
		protected function doLayout():void{
			this.content.y=this.head.height;
			this.content.x=padding;
			this.height=this.content.y+this.content.height;
		}
	}
}