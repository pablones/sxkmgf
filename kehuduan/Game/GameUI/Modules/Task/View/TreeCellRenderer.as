package GameUI.Modules.Task.View
{
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.View.Components.UISprite;
	
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;

	public class TreeCellRenderer extends UISprite
	{
		/** 任务描述 */
		protected var desTf:TextField;
		/** 任务完成*/
		protected var taskFinishTf:TextField;
		/** 跟踪标记 */
		protected var followTf:TextField;
		
		public var data:Object;
		public var id:uint;
		private var _isSelected:Boolean=false;
		
		public function TreeCellRenderer(id:uint,isSelected:Boolean=false,data:Object=null)
		{
			super();
			this.mouseEnabled=true;
			//this.width=176;
			this.width=170;
			this.height=16;
			this.id=id;
			this.isSelected=isSelected;
			this.data=data;
			this.createChildren();
		}
		
		protected function onRollOverHandler(e:MouseEvent):void{
			if(!this.isSelected){
				this.graphics.clear();
				this.graphics.beginFill(0x4d3c13,.7);
				this.graphics.drawRect(0,0,this.width,this.height);
				this.graphics.endFill();
			}
		}
		
		protected function onRollOutHandler(e:MouseEvent):void{
			if(!this.isSelected){
				this.graphics.clear();
				this.graphics.beginFill(0x00ff00,0);
				this.graphics.drawRect(0,0,this.width,this.height);
				this.graphics.endFill();
			}
		}
		
		public function set isSelected(value:Boolean):void{
			this._isSelected=value;
			if(this.isSelected){
				this.graphics.clear();
				this.graphics.beginFill(0xfffe65,.7);
				this.graphics.drawRect(0,0,this.width,this.height);
				this.graphics.endFill();
			}else{
				this.graphics.clear();
			}	
		}
		
		public function get isSelected():Boolean{
			return this._isSelected;
		}
			 
		protected function onMouseClickHandler(e:MouseEvent):void{

		}
		
		protected function createChildren():void{
			
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[this.id] as TaskInfoStruct;
			if(taskInfo==null)return
			this.addEventListener(MouseEvent.CLICK,onMouseClickHandler);
			this.addEventListener(MouseEvent.MOUSE_OVER,onRollOverHandler);
			this.addEventListener(MouseEvent.MOUSE_OUT,onRollOutHandler);
			
			this.desTf=new TextField();
			this.desTf.autoSize=TextFieldAutoSize.LEFT;
			this.desTf.type=TextFieldType.DYNAMIC;
			this.desTf.selectable=false;
			this.desTf.textColor=0xe2cca5;
			this.desTf.text=taskInfo.taskName;
			this.addChild(this.desTf);
			
			this.taskFinishTf=new TextField();
			this.taskFinishTf.autoSize=TextFieldAutoSize.RIGHT;
			this.taskFinishTf.type=TextFieldType.DYNAMIC;
			this.taskFinishTf.selectable=false;
			this.taskFinishTf.textColor=0x00ff00;
			if(taskInfo.status==3){
				this.taskFinishTf.text=GameCommonData.wordDic[ "mod_task_view_tre_cre_1" ];        //可提交
			}else{
				this.taskFinishTf.text="";
			}
			this.addChild(this.taskFinishTf);
			
			this.followTf=new TextField();
			this.followTf.autoSize=TextFieldAutoSize.LEFT;
			this.followTf.type=TextFieldType.DYNAMIC;
			this.followTf.selectable=false;
			this.followTf.textColor=0x00ff00;
			if(taskInfo.isFollow==1 && taskInfo.status!=0){
				this.followTf.text="√";
			}else{
				this.followTf.text="";
			}
			this.addChild(this.followTf);
			this.doLayout();	
		}
	
		protected function doLayout():void{
			this.followTf.x=0;
			this.followTf.y=Math.floor((this.height-this.followTf.height)/2);
			this.desTf.x=10;
			this.desTf.y=Math.floor((this.height-this.desTf.height)/2);
			this.taskFinishTf.x=this.width-this.taskFinishTf.width-1;
			this.taskFinishTf.y=Math.floor((this.height-this.taskFinishTf.height)/2);
			
			
		}

		
	}
}