package GameUI.Modules.Task.View
{
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	
	import flash.display.Sprite;

	/**
	 *  可接任务内容详细信息面板
	 * @author felix
	 * 
	 */	
	public class AcceptTaskInfoPanel extends Sprite
	{
		protected var container:UIScrollPane;
		protected var content:UISprite;
		protected var padding:uint=20;
		protected var taskId:int;
		
		protected var taskDes:TaskText;
		
		public function AcceptTaskInfoPanel()
		{
			super();
			this.content=new UISprite();
			this.taskDes=new TaskText();
			this.createChildren();
		}
		
		protected function createChildren():void{
			
			this.createCells();
		}
		
		protected function createCells(id:uint=0):void{
			this.clearContent();
			if(id==0)return;
			var task:TaskInfoStruct=GameCommonData.TaskInfoDic[id];
			var str:String='<font color="#fffe65" size="13">'+task.taskName+':</font><br>' + 
			'<font color="#ffffff">'+GameCommonData.wordDic[ "mod_task_view_acc_cre_1" ]       //任务所在地：
			+ '</font>'+task.taskArea+
			'<br><font color="#ffffff">'+GameCommonData.wordDic[ "mod_task_view_acc_cre_2" ]+'</font>'+task.taskNPC+'<br>'        //任务发布人：
			+'<font color="#ffffff">'+GameCommonData.wordDic[ "mod_task_view_acc_cre_3" ]+task.taskLevel+'</font>';            //任务等级：
			
			
			this.taskDes.tfText=str;
			this.content.addChild(taskDes);
			this.content.width=this.taskDes.width;
			this.container=new UIScrollPane(this.content);
			this.addChild(container);
			this.doLayout();
			
		}
		
		public function update(id:uint=0):void{
			this.taskId=id;
			this.createCells(id);
		}
		
		/**
		 * 删除指定的任务内容。如果当前是该任务的话 
		 * @param id :要删除任务内容的ID号
		 * 
		 */		
		public function removeInfo(id:uint):void{
			if(this.taskId==id){
				this.clearContent();
				this.taskId=-1;
			}	
		}
		
		protected function doLayout():void{
			this.content.x=0;
			this.content.y=23;
			this.content.height=this.taskDes.height;
			this.container.width = 315;
			this.container.height = 362;
			this.container.refresh();
		}
		
		/** 清空*/
		public function clearContent():void{
			
			while(this.numChildren>1){
				this.removeChildAt(1);
			}
			this.taskDes.tfText="";
		}
		
	}
}