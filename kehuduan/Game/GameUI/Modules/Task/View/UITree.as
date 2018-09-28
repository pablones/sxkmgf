package GameUI.Modules.Task.View
{
	import GameUI.Modules.Task.Commamd.TreeEvent;
	import GameUI.Modules.Task.Model.TaskGroupStruct;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.View.Components.UISprite;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.utils.Dictionary;
 
	public class UITree extends UISprite
	{
		/** 数据提供者*/
		private var _dataProvider:Array;
		/**渲染集合 */
		private var cells:Array=[];
		/** 分组 组标签*/
		private var groupCell:Array=[];
		private var selectedID:uint;
		protected var padding:uint=20;
		
		/** 没有任务可以显示文本框  */
		protected var noTaskTextField:TaskText;
		
		
		public function UITree(dataPro:Array=null)
		{
			super();
			this._dataProvider=dataPro;
			if(this._dataProvider==null)this._dataProvider=[];
			this.width=258;
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.noTaskTextField=new TaskText();
			this.cells=[];
			if(this.dataProvider==null || this.dataProvider.length==0)return;
			this.createCells();	
		}
		
		protected function toRepaint():void{
			
			 this.removeAll();  //清空
			 this.createCells();
		}	
		
		public function refresh():void{
			this.toRepaint();
		}
		
		
		/**
		 * 创建渲染器开始渲染 
		 * 
		 */		
		protected function createCells():void{
			if(this.dataProvider==null || this.dataProvider.length==0)return;
			var currentY:Number=0;
			this.selectedID=0;
			this._dataProvider=this.sortTaskType(this.dataProvider);
			for each(var group:TaskGroupStruct in this.dataProvider){
				var gCell:TaskGroupCellRenderer=this.getGroupCell(group.des,group.isExpand);
				gCell.y=currentY;
				currentY+=gCell.height;
				if(group.isExpand){
					var arr:Array=this.sortTaskLevel(group.taskDic);
					for each(var obj:Object in arr){
						var cell:TreeCellRenderer=this.getCell(obj.id,obj.isSelected,obj.data);
						cell.y=currentY;
						cell.x=padding;
						currentY+=cell.height;
						if(obj.isSelected==true){
							this.selectedID=obj.id;
						}
					}
				}
			}
			this.height=currentY;
			
		}
		
		
		/**
		 * 对任务分组按等级进行排序 
		 * @param dic
		 * @return 
		 * 
		 */		
		protected function sortTaskLevel(dic:Dictionary):Array{
			var target:Array=[];
			for each(var obj:* in dic){
				target.push(obj);
			}	
			target=target.sortOn("taskLevel",Array.NUMERIC);
			return target;
		}
		
		
		/**
		 *  对任务按类型进行排序
		 * @param arr
		 * @return 
		 * 
		 */		
		protected function sortTaskType(arr:Array):Array{
			var target:Array=[];
			var source:Array=arr;
			
			var taskType_mainLine:Array=[];
			var taskType_subLine:Array=[];
			var taskType_dayLine:Array=[];
			var taskType_sm:Array=[];
			var taskType_ps:Array=[];
			var taskType_fb:Array=[];
			var taskType_sq:Array=[];
			var taskType_bp:Array=[];
			
			for each(var obj:* in source){
				if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_1" ]){       //主线任务
					taskType_mainLine.push(obj);
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_2" ]){          //支线任务
					taskType_subLine.push(obj);
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_3" ]){         //日常任务
					taskType_dayLine.push(obj);
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_4" ]){        //师门任务
					taskType_sm.push(obj);
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_5" ]){         //跑商任务
					taskType_ps.push(obj);
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_6" ]){          //副本任务
					taskType_fb.push(obj);	
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_7" ]){          //神器任务
					taskType_sq.push(obj);
				}else if(obj.des==GameCommonData.wordDic[ "mod_task_view_taskf_sor_8" ]){          //帮派任务
					taskType_bp.push(obj);
				}		
			}			
			return target.concat(taskType_mainLine).concat(taskType_subLine).concat(taskType_dayLine).concat(taskType_sm).concat(taskType_ps).concat(taskType_fb).concat(taskType_sq).concat(taskType_bp);
		}
		
		
		protected function getGroupCell(des:String,isExpand:Boolean=false):TaskGroupCellRenderer{
			var cell:TaskGroupCellRenderer=new TaskGroupCellRenderer(des,isExpand);
			cell.addEventListener(MouseEvent.CLICK,onClickTaskGroup);
			this.groupCell.push(cell);
			this.addChild(cell);
			return cell;
		}
		
		
		/**
		 * 设置选中的选项 
		 * @param id
		 * 
		 */		
		public function setSelected(id:uint):void{
			this.clearAllSelected();
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[id] as TaskInfoStruct;
			if(taskInfo==null)return;
			var group:TaskGroupStruct=this.searchGroupByDes(taskInfo.taskType,this.dataProvider);
			if(group==null)return;
			var obj:Object=group.taskDic[id];
			if(obj==null)return;
			obj.isSelected=true;
			group.isExpand=true;
			this.refresh();
			this.dispatchEvent(new TreeEvent(TreeEvent.CHANGE_SELECTED,id,false,false));
		}
		
		/**
		 * 清除所有选中的
		 * 
		 */		
		public function clearAllSelected():void{
			for each(var group:TaskGroupStruct in this.dataProvider){
				if(group.isExpand){
					for each(var obj:Object in group.taskDic){
						obj.isSelected=false;
					}
				}
			}
		}
		
		
		/**
		 * 点击分组图标，如果选中的在这个组。。将选中选项清空。并发一事件通知选择索引
		 * 改变 
		 * @param e
		 * 
		 */		
		protected function onClickTaskGroup(e:MouseEvent):void{
			var index:int=this.groupCell.indexOf(e.currentTarget);
			if(index!=-1){
				var group:TaskGroupStruct=this.dataProvider[index];
				if(group.isExpand && this.checkSelectedInGroup(group)){
					this.selectedID=0; //发一事件出去
					this.dispatchEvent(new TreeEvent(TreeEvent.CHANGE_SELECTED,0));
				}
				group.isExpand=!group.isExpand;
				this.refresh();
			}		
		}
		
		/** 检查该组中是否有选中的选项*/
		protected function checkSelectedInGroup(group:TaskGroupStruct):Boolean{
			for each(var obj:Object in group.taskDic){
				if(obj.isSelected==true){
				   obj.isSelected=false;
					return true;
				}	
			}
			return false;
		}
		
		
		/**
		 * 添加一个任务                                       
		 * @param taskInfo ：任务数据结构
		 * 
		 */		
		public function addTask(taskInfo:TaskInfoStruct):void{
			var g:TaskGroupStruct=this.searchGroupByDes(taskInfo.taskType,this.dataProvider);
			if(g!=null){
				g.taskDic[taskInfo.id]={id:taskInfo.id,isSelected:false,data:null,taskLevel:taskInfo.taskLevel};
			}else{
				var dic:Dictionary=new Dictionary();
				dic[taskInfo.id]={id:taskInfo.id,isSelected:false,data:null,taskLevel:taskInfo.taskLevel};
				var group:TaskGroupStruct=new TaskGroupStruct(dic,true,taskInfo.taskType);
				this.dataProvider.push(group);
			}
			this.toRepaint();
		}
		
		/**
		 * 册除一个任务 
		 * @param id ：任务ID
		 * 
		 */		
		
		public function delTask(id:uint):void{
			//将这个组中的任务删除
			for each(var g:TaskGroupStruct in this.dataProvider){
				delete g.taskDic[id];
			}
			
			//去掉没有任务的组
			for each(var g1:TaskGroupStruct in this.dataProvider){
				var dic:Dictionary=g1.taskDic;
				var flag:Boolean=false;
				for each(var obj:* in dic){
					flag=true;
					break;
				}
				if(!flag){
					var index:uint=this.dataProvider.indexOf(g1);
					this.dataProvider.splice(index,1);
				}	
			}
			this.toRepaint();
			this.checkIsNoTask();
		}
		
		/**
		 * 根據任務組名字查找任務所屬的組 
		 * @param str ：
		 * @param groups ：
		 * @return 
		 * 
		 */		
		public function searchGroupByDes(str:String,groups:Array):TaskGroupStruct{
			for each(var t:TaskGroupStruct in groups){
				if(t.des==str){
					return t;
				}
			}
			return null;
		}
		
		/**
		 * 点击了某一个任务选项,发任务选中事件，更新选中状态 
		 * @param e
		 * 
		 */		
		protected function onSelectedTaskCell(e:MouseEvent):void{
			var curSelectObj:Object=this.searchObjById(e.currentTarget.id);
			if(curSelectObj==null)return;
			curSelectObj.isSelected=true;
			
			var preSelectedObj:Object=this.searchObjById(this.selectedID);
			if(preSelectedObj!=null && e.currentTarget.id != this.selectedID){//当前选中的和已经选中为同一对象时，不改变状态
				preSelectedObj.isSelected=false;
			}
			this.refresh();
			this.dispatchEvent(new TreeEvent(TreeEvent.CHANGE_SELECTED,e.currentTarget.id,false,false));
		}
		
		
		/**
		 * 根据ID号找相对应选项的数据对象 
		 * @param id
		 * @return 
		 * 
		 */		
		public function searchObjById(id:uint):Object{
			for each(var group:TaskGroupStruct in this.dataProvider){
				if(group.taskDic[id]!=null){
					return group.taskDic[id];
				}
			}
			return null;
		}
		
		protected function getCell(id:uint,isSelected:Boolean=false,data:Object=null):TreeCellRenderer{
			var cell:TreeCellRenderer=new TreeCellRenderer(id,isSelected,data);
			
			this.cells.push(cell);
			this.addChild(cell);
			cell.addEventListener(MouseEvent.CLICK,onSelectedTaskCell);
			return cell;
		}
		
		protected function removeAll():void{
			while(this.numChildren>1){
				this.removeChildAt(1);
			}
			this.cells=[];
			this.groupCell=[];
		}
		
		public function set dataProvider(dataPro:Array):void{
			this._dataProvider=dataPro;
			this.toRepaint();
			this.checkIsNoTask();			
		}
		
		public function get dataProvider():Array{
			return this._dataProvider;
		}
		
		/** 检查是否已经没有可显示的任务了 */
		private function checkIsNoTask():void{
			if(this._dataProvider==null || this._dataProvider.length==0){
				this.noTaskTextField.tfText='<font color="#fffe65">'+GameCommonData.wordDic[ "mod_task_view_uit_che_1" ]+'</font>';       //你现在还没有任务可显示
				this.addChild(this.noTaskTextField);
			}else{
				if(this.contains(this.noTaskTextField)){
					this.removeChild(this.noTaskTextField);
				}
			}
		}
		
	}
}