package GameUI.Modules.Task.View
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.ShowMoney;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat; 

	/**
	 *  任务管理器信息界面
	 * @author felix
	 * 
	 */	
	public class TaskInfoPanel extends Sprite
	{
		
		protected var container:UIScrollPane;
		protected var content:UISprite;
		protected var padding:uint=20;
		protected var shortPadding:uint = 9;
		
		/** 任务描述*/
		protected var taskDes:TaskText;
		/** 任务目标*/
		protected var taskTarget:TaskText;
		/** 任务完成情况*/
		protected var taskProcess:TaskText;
		/** 任务获得 */
		protected var taskObtain:TaskText;
		/** 金钱*/
		protected var money:TextField;
		protected var moneySprite:Sprite;
		/** 经验值 */
		protected var exp:TaskText;
		/** 物品 */
		protected var goodsDes:TaskText;
		/** 物品 */
		protected var goodsPanel:EquPanel;
		/** 任务ID*/
		protected var id:int;
		/** 任务数据结构 */
		protected var taskInfo:TaskInfoStruct;
		
		private var taskSwf:MovieClip;
		private var targetLine:MovieClip;
		private var taskProcessLine:MovieClip;
		private var taskObtainLine:MovieClip;
		private var w:Number = 292;
		
		public function TaskInfoPanel(id:uint=0)
		{
			super();
			this.id=id;
			this.content=new UISprite();
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewTask.swf", onPanelLoadComplete);
			//this.createChildren();
		}
		
		private function getTaskTextFormat():TextFormat{
			var tf:TextFormat=new TextFormat("宋体" ,12);           //宋体
			tf.leading=5;
			return tf;
		}
		
		protected function createChildren():void{
			this.createDataPro();
			if(this.taskInfo==null)return;	
			this.taskDes=new TaskText(298);
			
			this.taskDes.tfText=taskInfo.taskDes;//任务内容			 
			this.content.addChild(this.taskDes);
			
			this.targetLine = new (taskSwf.loaderInfo.applicationDomain.getDefinition("line"));//分界线
			this.targetLine.width = w;
			this.targetLine.x = 4;
			this.content.addChild(targetLine);
			
			this.taskTarget=new TaskText(298);
			this.taskTarget.tfText=taskInfo.taskTip;//任务目标
			content.addChild(this.taskTarget);
			
			this.taskProcessLine = new (taskSwf.loaderInfo.applicationDomain.getDefinition("line"));
			this.taskProcessLine.width = w;
			this.taskProcessLine.x =4;
			this.content.addChild(taskProcessLine);
			
			
			this.taskProcess=new TaskText(298);
			var str:String='<font color="#fffe65">'+GameCommonData.wordDic[ "mod_task_view_taski_cre_1" ]+'</font><br>';            //完成情况：
			if(taskInfo.status==3){
				str+=taskInfo.taskProcessFinish;
			}else{
				if(taskInfo.taskProcess1!=null && taskInfo.taskProcess1!=""){
					str+=this.changeStr(taskInfo.taskProcess1)+'<br>';
				}
				if(taskInfo.taskProcess2!=null && taskInfo.taskProcess2!="" ){
					str+=this.changeStr(taskInfo.taskProcess2)+'<br>';
				}
				if(taskInfo.taskProcess3!=null && taskInfo.taskProcess3!=""){
					str+=this.changeStr(taskInfo.taskProcess3)+'<br>';
				}
				if(taskInfo.taskProcess4!=null && taskInfo.taskProcess4!=""){
					str+=this.changeStr(taskInfo.taskProcess4)+'<br>';
				}
				if(taskInfo.taskProcess5!=null && taskInfo.taskProcess5!=""){
					str+=this.changeStr(taskInfo.taskProcess5)+'<br>';
				}
				if(taskInfo.taskProcess6!=null && taskInfo.taskProcess6!=""){
					str+=this.changeStr(taskInfo.taskProcess6)+'<br>';
				}
			}
			
			this.taskProcess.tfText=str;
			this.content.addChild(this.taskProcess);
			
			this.taskObtainLine = new (taskSwf.loaderInfo.applicationDomain.getDefinition("line"));
			this.taskObtainLine.width = w;
			this.taskObtainLine.x = 4;			
			this.content.addChild(taskObtainLine);
			
			
			this.taskObtain=new TaskText(298);
			this.taskObtain.tfText='<font color="#fffe65">'+GameCommonData.wordDic[ "mod_task_view_taski_cre_2" ]+'</font>';          //任务奖励：
			this.content.addChild(this.taskObtain);
			
			this.moneySprite=new Sprite();
			this.money=new TextField();
			this.money.filters=Font.Stroke();
			this.money.defaultTextFormat=this.textFormat(); 
			this.money.width=600;
			this.money.autoSize=TextFieldAutoSize.LEFT;
			this.money.wordWrap=false;
			this.money.mouseEnabled=false;
			this.money.selectable=false;
			this.moneySprite.addChild(this.money);
			this.money.htmlText=taskInfo.taskPrize1;
			this.content.addChild(this.moneySprite);
			ShowMoney.ShowIcon(this.moneySprite,this.money,true);
			
			this.exp=new TaskText();
			this.exp.tfText=taskInfo.taskPrize2;
			this.content.addChild(this.exp);
			
			this.goodsDes=new TaskText();
			if(taskInfo.taskProze3.length==0){
				this.goodsDes.tfText="";
			}else{
				var type:uint=taskInfo.taskProze3[0];
				if(type==0){
					this.goodsDes.tfText="";
				}else if(type==1){
					this.goodsDes.tfText='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_task_view_taski_cre_3" ]+'</font>';	            //你可以选择获得下列物品之一：
				}
			}
			this.content.addChild(this.goodsDes);
			this.goodsPanel=new EquPanel(taskInfo.taskProze3,false);
			this.content.addChild(this.goodsPanel);
			this.content.width=this.taskTarget.width;
			if(this.container!=null && this.contains(this.container) ){
				this.removeChild(this.container);
			}
			this.container=new UIScrollPane(this.content);
			this.addChild(container);
			this.doLayout();
		}
		
		protected function changeStr(soure:String):String{
			var s:String;
			var arr:Array = soure.split("$");
			s = arr.join("");
			return s;
		}
		
		protected function createDataPro():void{
			taskInfo=GameCommonData.TaskInfoDic[this.id] as TaskInfoStruct;
			if(taskInfo==null){
				return;
			}
		}
		
		private function onPanelLoadComplete():void
		{
			this.taskSwf = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewTask.swf");
			this.createChildren();
		}
		
		/**
		 *  布局 
		 * 
		 */		
		protected function doLayout():void{
			this.content.x=0;
			this.content.y=23;
			this.container.width = 315;
			this.container.height = 362;
			
			this.taskDes.y = 5;
			this.targetLine.y=this.taskDes.y+this.taskDes.height+padding;
			this.taskTarget.y=this.targetLine.y + this.shortPadding;
			
			this.taskProcessLine.y=this.taskTarget.y+this.taskTarget.height+padding;
			this.taskProcess.y=this.taskProcessLine.y+ this.shortPadding;
			
			this.taskObtainLine.y=this.taskProcess.y+this.taskProcess.height+padding;
			this.taskObtain.y=this.taskObtainLine.y+ this.shortPadding;
			
			if ( UIConstData.TaskTempInfo[ id ].taskMoneyX==undefined )
			{
				this.moneySprite.y=this.taskObtain.y+this.taskObtain.height-4; 
			}
			else
			{
				this.moneySprite.y=this.taskObtain.y+this.taskObtain.height-8; 
			}
			
			this.exp.y=moneySprite.y+this.moneySprite.height;
			this.goodsDes.y=exp.y+this.exp.height;
			this.goodsPanel.y=goodsDes.y+this.goodsDes.height;
			
			this.content.height=this.goodsPanel.y+this.goodsPanel.height + 10;
			this.container.refresh();
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.font = "宋体";        //宋体
			return tf;
		}
		
		
		/**
		 * 
		 * 根据ID号显示任务内容细节 
		 * @param id ：id=0 ;什么任务也没有，不显示 
		 * 
		 */		
		public function update(id:uint=0):void{
			this.id=id;
			while(this.content.numChildren>1){
				this.content.removeChildAt(1);
			}
			this.createChildren();
		}
		
		/**
		 * 删除指定的任务内容。如果当前是该任务的话 
		 * @param id :要删除任务内容的ID号
		 * 
		 */	
		public function removeInfo(id:uint):void{
			if(this.id==id){
				this.clearContent();
				this.id=-1;
			}
		}
			
		
		/**
		 * 清空所有显示对象 
		 * 
		 */		
		public function clearContent():void{
			while(this.content.numChildren>1){
				this.content.removeChildAt(1);
			}
			this.content.height=10;
			this.container.refresh();
		}
		
	}
}