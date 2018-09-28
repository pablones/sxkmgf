package GameUI.Modules.Task.Model
{
	import GameUI.ConstData.UIConstData;
	
	public class TaskInfoStruct
	{
		public var id:uint;
		public var taskId:uint;   
		public var isChain:uint;
		public var step:uint;   
		public var type:uint;               //0:MaskID 1：天MaskId
		public var maskIndex:uint;
		public var taskType:String;         //1：主线任务 2：日常任务 3：按地名分
		public var taskLevel:uint;    
		public var taskName:String;
		public var taskArea:String;       
		public var taskDes:String;          //任务接收时的描述文字
		public var taskTip:String;          //任务目标
		public var taskCloneTip:String;     //克隆的任务目标原版
		public var taskProcess1:String;     //任务进度 1  
		public var taskProcess2:String;
		public var taskProcess3:String;
		public var taskProcess4:String;
		public var taskProcess5:String;
		public var taskProcess6:String;
		public var taskProcessFinish:String;     //任务处理完成完成情况显示内容
		public var taskCloneProcessFinish:String;//克隆的任务处理完成情况显示内容
		public var taskTime:String;              //任务时间要求
		public var taskNPC:String;             //任务接收Npc 
		public var taskNpcId:uint;             //任务接收NpcId
		public var taskCommitNpcId:uint;          //任务提交NpcId
		public var taskCommitNPC:String;          //任务提交Npc 
		public var taskFinish:String;            //任务完成Des 
		public var taskCloneFinish:String;       //克隆的任务完成DES
		
		public var taskMoneyX:uint;             //环任务中金钱奖励的基数值
		public var taskExpX:uint;               //经验奖励的基数值
		public var taskPrize1:String;           //任务获得钱
		public var taskPrize2:String;     		//任务经验 
		public var taskProze3:Array=[];         // @0@210000*1,210000*2@;
		public var taskUnFinish:String;        //未完成任务时的任务描述
		public var taskCloneUnFinish:String;   //克隆的未完成任务时的任务描述
		
		public var processMask:uint;            //任务处理掩码(主要是判断任务是否已经完成)  0：代表接收到该任务
		
		public var status:uint=0;               //0： 未接   1：已接任务 2：已经完成（包括已经提交）  3:任务已经做完但并没有提交
		public var isFollow:uint=0;             // 0:追踪  1：不追踪
		public var loopStep:uint;               //当前环数
		public var taskExpandType:uint;         //任务扩展类型，1：提供精力值选择的任务 
		
		public var taskRandBaseExp:String;         //精力值对换经验
		public var taskRandBaseMoney:String;       //精力值对换金钱
		
		public function TaskInfoStruct()
		{
			
		}
		
		/**
		 * 得到金钱奖励的字符串 
		 * @return 
		 * 
		 */		
		public function toPrizeMoney():void{
			var str:String="";
			if(this.id>10001000){
				var tempN:uint=0;
				var step:uint=this.loopStep%10;
				if(step==0)step=10;
				if(loopStep<=10){
					tempN=(this.taskMoneyX+step*GameCommonData.Player.Role.Level*5)*5;
				}else if(loopStep<=20 && GameCommonData.Player.Role.ViceJob!=null && GameCommonData.Player.Role.ViceJob.Job!=0){
					tempN=(this.taskMoneyX+step*GameCommonData.Player.Role.Level*5)*5;
				}else{
					tempN=0;			
				}
				var a:uint=uint(tempN/10000);                                  //金
				var b:uint=uint((tempN%10000)/100)							   //银
				var c:uint=tempN%100;									       //铜		
				str+= a==0 ? "" : a+"\\se";
				str+= b==0 ? "" : b+"\\ss";
				str+=  c+"\\sc";
				if ( tempN==0 )
				{
					this.taskPrize1="";
				}
				else
				{
					this.taskPrize1='<font color="#ffffff">'+str+'</font>'; 
				}
				
				////////////////////修改帮派任务
				if ( UIConstData.TaskTempInfo[ id ].taskMoneyX==undefined )
				{
					this.taskPrize1 = UIConstData.TaskTempInfo[ id ].taskPrize3; 
				}
				///////////////////////////////
			}
		}
		
		/**
		 * 得到经验奖励的字符串 
		 * @return 
		 * 
		 */		
		public function toPrizeExp():void{
			var str:String="";
			if(this.id>10001000){
				var tempN:uint=0;
				var step:uint=this.loopStep%10;
				if(step==0)step=10;
				if(loopStep<=10){
					tempN=(this.taskExpX+step*GameCommonData.Player.Role.Level*5)*10;
				}else if(loopStep<=20 && GameCommonData.Player.Role.ViceJob!=null && GameCommonData.Player.Role.ViceJob.Job!=0){
					tempN=(this.taskExpX+step*GameCommonData.Player.Role.Level*5)*10;
				}else{
					tempN=this.taskExpX+step*GameCommonData.Player.Role.Level*5;			
				}
				str='<font color="#ffffff">'+tempN+GameCommonData.wordDic[ "mod_task_mod_taski_top_1" ]+'</font>';        //经验值
				this.taskPrize2=str;	
			}
		}
		
		/**
		 * 得到任务名称字符串 
		 * @return 
		 * 
		 */		
		public function toName():void{
			var str:String="";
			
			var step:uint=this.loopStep%10;
			if(step==0)step=10;
			if(this.id>10001000 ){
				var reg:RegExp=/\([^\)]*\)/g;
				if(reg.test(this.taskName)){
					str=this.taskName.replace(reg,"("+step+"/10)");	
				}else{
					str=this.taskName+"("+step+"/10)";    //todo(还要做一些处理)
				}
				this.taskName=str;	
			}
			
		}
		
		/**
		 * 根据人物的职业改变显示的内容 
		 * 
		 */		
		public function toChangeDesByJob():void{
			if(this.id>10001000){
				this.taskTip=this.changeDes(this.taskCloneTip);
				this.taskProcessFinish=this.changeDes(this.taskCloneProcessFinish);
				this.taskFinish=this.changeDes(this.taskCloneFinish);
				this.taskUnFinish=this.changeDes(this.taskCloneUnFinish);	
			}
		}
		
		protected function changeDes(source:String):String{
			var str:String=source;
			var reg:RegExp=/@[^@]*@/;
			var arr:Array=str.match(reg);
			while(arr!=null){
				var regIn:RegExp=/[\d]+/;
				var typeStr:String=arr[0];
				var type:uint=uint(typeStr.match(regIn)[0]);
				str=str.replace(arr[0],this.getDesByType(type));
				reg=/@[^@]*@/;
				arr=str.match(reg);
			}
			return str;
		}
		
		/**
		 * 根据Type号获取描述替换字符串 
		 * 
		 */		
		protected function getDesByType(type:uint):String{
			var str:String="";
			switch (type){
				case 101:
					str=TaskProxy.getInstance().linkDesDic[GameCommonData.Player.Role.CurrentJobID];
					str=str.replace("@",this.id);
					break;	
			}	
			return str;
		}
	}
}