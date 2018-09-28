package OopsEngine.Scene
{
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.geom.Point;
	
	/**游戏人物行为职责链*/
	public class Handler
	{
		/**当前对象*/
		public var Animal:GameElementAnimal;
		/**技能信息*/
		public var SkillInfo:GameSkill;
		/**buff信息*/
		public var gameSkillBuff:GameSkillBuff; 
		/**技能效果列表*/
		public var SkillEffectList:Array = new Array();
		/**下个职责链信息*/
		public var Next:Handler;
		/**职责链的步骤*/
		public var Process:int = 1;
		/**是否监视平滑移动*/
		public var IsSmoothMoving:Boolean = false;
		/**目标点*/
		public var TargerPoint:Point = new Point(0,0);
        /**职责链层次*/
		public var floor:int = 0;		
		/**职责链时间编号*/
		public var TimeID:int = 0;
		/**职责链类别**/
		public var Type:int = 0; //1 动作 2 技能打击效果包括死亡
		/**攻击产生的效果**/
		public var InfoHandler:Handler;
		
		/**执行职责链*/
		public virtual function Run():void{}	
		
		
		/**清空职责链*/
		public  virtual function Clear():void
		{
		    NextHendler();	    
			if(this.Animal.handler != null)
				this.Animal.handler.Clear();
		}
		
		/**获取当前职责链的层次*/
		public function get Floor():int
		{
			floor = 0;
			if(this.Next != null)
			{
				GetNext(this.Next);
			}
			return floor;
		} 		
		public function GetNext(h:Handler):void
		{
			floor += 1;
				
			if(h.Next != null)
			{
				GetNext(h.Next);
			}
		}
		
		/**添加到职责链的尾部*/
		public static function FindEndHendler(h:Handler,newh:Handler):void
		{			
			if(h.Next != null)
			{
				FindEndHendler(h.Next,newh);
			}		
			else
			{
				h.Next = newh;  
			}
		}
		
		/**下个职责链*/
		public  function  NextHendler():void
		{
			Animal.handler = Next;
		}
		
		/** 构造函数
		 *  animal                       施法对象
		 *  next                         下个职责链
		 *  skillEffectList              技能打击列表
		 *  sendAction                   施法对象动作
		 *  skillID                      技能编号
		 *  point                        击打点
		 *  process                      职责链步骤
		 * */
		public function Handler(animal:GameElementAnimal,next:Handler,skillEffectList:Array,skill:GameSkill,buff:GameSkillBuff,timeID:int,point:Point,process:int = 1):void
		{
			SkillInfo       = skill;
			Animal          = animal;
			Process         = process;
			Next            = next;
			TargerPoint     = point;
			SkillEffectList = skillEffectList;
			TimeID          = timeID;
			gameSkillBuff   = buff;			
		}

		
	}
}