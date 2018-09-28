package GameUI.Modules.Task.View
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.Components.UISprite;
	import GameUI.View.ShowMoney;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;


	/**
	 *  任务接受信息面板
	 * @author felix
	 * 
	 */	
	public class TaskPanel extends UISprite
	{
		protected var container:UIScrollPane;
		protected var content:UISprite;
		
		/** 各段间距 */
		protected var padding:uint=20;
		/** 任务描述*/
		protected var taskDes:TaskText;
		/** 任务目标*/
		protected var taskTarget:TaskText;
		/** 任务获得 */
		protected var taskObtain:TaskText;
		/** 金钱*/
		protected var money:TextField;
		protected var moneySprite:Sprite;
		/** 经验值 */
		protected var exp:TaskText;
		/** 物品描述 */
		protected var goodsDes:TaskText;
		/** 物品图标 */
		protected var goodsPanel:EquPanel;
		/** 任务ID  */
		protected var taskId:uint;
		/** 选择物品type*/
		public var selectedGoodType:int=-1;
		
		public function TaskPanel(id:uint=1)
		{
			super();
			this.taskId=id;
			this.content=new UISprite();
			this.createChildren();
		}	
		protected function createChildren():void{
			
			var taskInfo:TaskInfoStruct=GameCommonData.TaskInfoDic[taskId] as TaskInfoStruct;
			this.taskDes=new TaskText();
			if(taskInfo.status==3){
				this.taskDes.tfText=GameCommonData.TaskInfoDic[taskId].taskFinish;	
			}else{
				this.taskDes.tfText=GameCommonData.TaskInfoDic[taskId].taskDes;			 
			}
			
			this.content.addChild(this.taskDes);
			this.taskTarget=new TaskText();
			this.taskTarget.tfText=taskInfo.taskTip;
			content.addChild(this.taskTarget);
		
			this.taskObtain=new TaskText();
			this.taskObtain.tfText='<font color="#fffe65">'+GameCommonData.wordDic[ "mod_task_view_taski_cre_2" ]+'</font>';         //任务奖励：
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
				this.goodsPanel=new EquPanel([],false);
			}else{
				var type:uint=taskInfo.taskProze3[0];
				if(type==0){
					this.goodsDes.tfText='';
					this.goodsPanel=new EquPanel(taskInfo.taskProze3,false);
				}else if(type==1){
					this.goodsDes.tfText='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_task_view_taski_cre_3" ]+'</font>';            //你可以选择获得下列物品之一：
					this.goodsPanel=new EquPanel(taskInfo.taskProze3,true);
					this.goodsPanel.changeSelectedGood=this.changeSelectedGood;
				}
			}
			
			this.content.addChild(this.goodsDes);
			this.content.addChild(this.goodsPanel);
			this.content.width=this.taskTarget.width;
			this.container=new UIScrollPane(this.content);
			this.addChild(container);
			this.doLayout();
		}
		
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.font = "宋体";        //宋体
			return tf;
		}
		
		protected function doLayout():void{			
			this.content.x=0;
			this.content.y=20;
			this.container.width=230;
			this.container.height=270; 
			
			this.taskTarget.y=this.taskDes.y+this.taskDes.height+padding;
			this.taskObtain.y=this.taskTarget.y+this.taskTarget.height+padding;
			if ( UIConstData.TaskTempInfo[ this.taskId ].taskMoneyX==undefined )
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
		
		/**  更新显示的数据 */
		protected function updateShow():void{
			
		}
		
		/**
		 * 选择物品改变 
		 * @param type ：选择物品typek号
		 * 
		 */		
		protected function changeSelectedGood(type:uint):void{
			this.selectedGoodType=type;
		}
		
	}
}