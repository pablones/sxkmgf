package GameUI.Modules.Task.View
{
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.Dictionary;

	/**
	 *  任务追踪管理面板
	 * @author felix
	 * 
	 */	
	public class TaskFollowPanel extends Sprite
	{
//		public static var timer:Number = 0;
		/**
		 * 数据字典
		 * dic[taskInfo.taskType]=[{taskInfo},{},{}...];
		 */		
		protected var _dataDic:Dictionary;
		/**
		 * 面板容器 
		 */		
		protected var container:UISprite;
		/**
		 * 滚动面板  
		 */		
		protected var scrollPanel:UIScrollPane;
		
		protected var cells:Array;
		protected var _maxHeight:uint;
		protected var padding:uint=10;
		
		public function TaskFollowPanel(maxHeight:uint)
		{
			super();
			
			this.mouseEnabled=false;
			this._maxHeight=maxHeight;
			this.createChildren();
		}
		
		protected function createChildren():void{
			this._dataDic=new Dictionary();
			this.cells=[];
			this.container=new UISprite(); 
			this.container.width=200;
			this.container.mouseEnabled=false;
			this.scrollPanel=new UIScrollPane(this.container);
			this.scrollPanel.width=214;
			this.scrollPanel.height=this._maxHeight;
			this.scrollPanel.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			this.addChild(this.scrollPanel);
			this.scrollPanel.mouseEnabled=false;
			this.createCells();
		}
		
		/** 刷新*/
		public function refresh():void{
			this.toRepaint();
		}
		
		protected function toRepaint():void{
//			var t:Number = new Date().getTime();
			this.removeAll();
			this.createCells();
			this.doLayout();
//			t = (new Date().getTime()) - t;
//			TaskFollowPanel.timer += t;
//			trace(">>> " + t + " >>> " + TaskFollowPanel.timer);
		}
		
		protected function removeAll():void{
			while(this.container.numChildren>0){
				this.container.removeChildAt(0);
			}
		}
		
		/**
		 * 根据任务的类型进行分类 
		 * @param dic
		 * @return 
		 * 
		 */		
		protected function sortTaskType(dic:Dictionary):Array{
			var target:Array=[];
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_1" ]]){         //主线任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_1" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //主线任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_2" ]]){         //支线任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_2" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //支线任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_3" ]]){         //日常任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_3" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //日常任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_4" ]]){         //师门任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_4" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //师门任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_5" ]]){         //跑商任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_5" ]] as Array).sortOn("taskLevel",Array.NUMERIC));	     //跑商任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_6" ]]){         //副本任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_6" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //副本任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_7" ]]){         //神器任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_7" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //神器任务
			}
			if(dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_8" ]]){         //帮派任务
				target.push((dic[GameCommonData.wordDic[ "mod_task_view_taskf_sor_8" ]] as Array).sortOn("taskLevel",Array.NUMERIC));        //神器任务
			}
			
			return target;
		}
		
		
		/**
		 * 创建可视元素 
		 * 
		 */		
		protected function createCells():void{
			
			var currentY:Number=0;
			var targetArr:Array=this.sortTaskType(this._dataDic);
			var cellWidth:int;
			if(GameCommonData.wordVersion != 2)
			{
				cellWidth = 200; 
			}
			else 
			{
				cellWidth = 180;
			}
			for each(var arr:Array in targetArr){
				
				if(arr==null ||arr.length==0)continue;			
				var tf:TextField=this.getAreaTf();
				container.addChild(tf);
				tf.text=arr[0].taskType;
				tf.y=currentY;
				currentY+=tf.height;
				
				if(arr!=null && arr.length>0){
					for each(var taskInfo:TaskInfoStruct in arr){
						var headTf:TextField=this.getAreaTf();
						headTf.name=String(taskInfo.id);
						headTf.mouseEnabled=true;
						headTf.doubleClickEnabled=true;
						
						headTf.selectable=false;
						headTf.addEventListener(MouseEvent.CLICK,onDoubleClickTask);
						if(taskInfo.status==3){
							headTf.text="["+taskInfo.taskLevel+"]"+taskInfo.taskName+GameCommonData.wordDic[ "mod_task_view_taskf_cre_1" ];           // (可提交)
						}else{
							headTf.text="["+taskInfo.taskLevel+"]"+taskInfo.taskName;
						}
							
						headTf.textColor=0x00ffff;
						container.addChild(headTf);
						headTf.x=padding;
						headTf.y=currentY;
						currentY+=headTf.height;
						
						
						if(taskInfo.status==3){
							var cellFinish:TaskText = new TaskText(cellWidth);
							cellFinish.tfText=this.changeStr(taskInfo.taskProcessFinish);
							container.addChild(cellFinish);
							cellFinish.x=padding;
							cellFinish.y=currentY;
							currentY+=cellFinish.height;
							continue;
						}
						
						if(taskInfo.taskProcess1!=""){
							var cell1:TaskText=new TaskText(cellWidth);
							cell1.tfText=this.changeStr(taskInfo.taskProcess1);
							container.addChild(cell1);
							cell1.x=padding;
							cell1.y=currentY;
							currentY+=cell1.height;
							
						}
						if(taskInfo.taskProcess2!=""){
							var cell2:TaskText=new TaskText(cellWidth);
							cell2.tfText=this.changeStr(taskInfo.taskProcess2);
							container.addChild(cell2);
							cell2.x=padding;
							cell2.y=currentY;
							currentY+=cell2.height;
							
						}
						if(taskInfo.taskProcess3!=""){
							var cell3:TaskText=new TaskText(cellWidth);
							cell3.tfText=this.changeStr(taskInfo.taskProcess3);
							container.addChild(cell3);
							cell3.x=padding;
							cell3.y=currentY;
							currentY+=cell3.height;
							
						}
						if(taskInfo.taskProcess4!=""){
							var cell4:TaskText=new TaskText(cellWidth);
							cell4.tfText=this.changeStr(taskInfo.taskProcess4);
							container.addChild(cell4);
							cell4.x=padding;
							cell4.y=currentY;
							currentY+=cell4.height;
							
						}
						if(taskInfo.taskProcess5!=""){
							var cell5:TaskText=new TaskText(cellWidth);
							cell5.tfText=this.changeStr(taskInfo.taskProcess5);
							container.addChild(cell5);
							cell5.x=padding;
							cell5.y=currentY;
							currentY+=cell5.height;
							
						}
						if(taskInfo.taskProcess6!=""){
							var cell6:TaskText=new TaskText(cellWidth);
							cell6.tfText=this.changeStr(taskInfo.taskProcess6);
							container.addChild(cell6);
							cell6.x=padding;
							cell6.y=currentY;
							currentY+=cell6.height;
						}
							 
					}
				}
			}
			this.container.height=currentY;	
			this.scrollPanel.refresh();
		}
		
		
		
		
		protected function onDoubleClickTask(e:MouseEvent):void{
			UIFacade.UIFacadeInstance.selectedTask(uint(e.currentTarget.name));
		}
		
		protected function changeStr(soure:String):String{
			var s:String;
			var arr:Array = soure.split("$");
			s = arr.join("");
			return s;
		}
		
		protected function getAreaTf():TextField{
			var tf:TextField=new TextField();
			tf.autoSize=TextFieldAutoSize.LEFT;
			tf.filters=Font.Stroke();
			tf.mouseEnabled=false;
			tf.textColor=0xffff00;
			tf.defaultTextFormat=this.textFormat();
			return tf;
		}
		
		protected function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.font = "宋体";       //宋体
			return tf;
		}
		
		protected function doLayout():void{
//			this.scrollPanel.refresh();	
		}
		
		public function set dataDic(value:Dictionary):void{
			this._dataDic=value;
			this.toRepaint();
		}
		
		public function get dataDic():Dictionary{
			return this._dataDic;
		}
		
		public function set maxHeight(value:uint):void{
			this._maxHeight=value;
			this.scrollPanel.height=value;
			this.scrollPanel.refresh();
			this.toRepaint();
		}
		
		public function get maxHeight():uint{
			return this._maxHeight;
		}
		
		
		/**
		 * 返回指定任务的位置 
		 * @param name
		 * @return 
		 * 
		 */		
		public function getTaskRectangle(name:String):Rectangle{
			
			var disObj:DisplayObject=this.container.getChildByName(name);
			if(disObj){
				var x:Number=disObj.x;
				var y:Number=disObj.y;
				var globalPoint:Point=this.container.localToGlobal(new Point(x,y));
				return new Rectangle(globalPoint.x,globalPoint.y,disObj.width,disObj.height);
			}else{
				return null;
			}
			 
		}
		
		
		
	}
}