package OopsEngine.Skill
{
	import flash.utils.Timer;
	
	public class GameSkillLevel
	{
		/**技能编号*/
		public var gameSkill:GameSkill;
		/**技能级别*/
		public var Level:int;
		/**技能熟练度*/
		public var Familiar:int;
		/**技能上次使用的时间**/
		public var UseTime:Number = 0; //12:00
		/**是否设置自动释放**/
		public var IsAutomatism:Boolean = true;
		/**自动释放可以使用的时间**/
		public var AutomatismUseTime:Number = 0;
		
		public function IsCoolTime():Boolean
		{
			var cooltime:int = GetCoolTime; //1 cd
			
			//12 01
			
			return true;
		}
		
		
		/**耗蓝多少*/
		public function get GetMP():int
		{
			return gameSkill.MP + gameSkill.LevelMP * (Level - 1);
		}
			
		/**冷却时间*/
		public function get GetCoolTime():int
		{
			return gameSkill.CoolTime + Math.round(gameSkill.LevelCoolTime * (Level - 1));
		}	
		
        /**获取技能说明**/
        public function get Reamark():String
        {
        	return gameSkill.SkillReamark.replace("N", gameSkill.Attack + gameSkill.LevelAttack * Level);
        }

        public function PetNewReamark(obj:Object):String
        {
        	var attacknum:int = 0;
        	switch(gameSkill.Attack)
        	{
        		case 1:
        		    attacknum = int(7.35 * Level+0.06 * obj.grid * obj.grideValue+23.34 * obj.privity * obj.privityValue);
        			break;
        	    case 2:
        	    	attacknum = int(5.145 * Level+0.042 * obj.grid * obj.grideValue+16.338 * obj.privity * obj.privityValue);
        			break;
        		case 3:
        		    attacknum = int(0.95 * Level+0.01 * obj.grid * obj.grideValue+3.015 * obj.privity * obj.privityValue);
        			break;
        		case 4:
        		    attacknum = int(0.665 * Level+0.007 * obj.grid * obj.grideValue+2.11 * obj.privity * obj.privityValue);
        			break;
        		case 5:
        		    attacknum = int(1.093 * Level+0.013 * obj.grid * obj.grideValue+3.473 * obj.privity * obj.privityValue);
        			break;
        		case 6:
        		    attacknum = int(0.765 * Level+0.009 * obj.grid * obj.grideValue+2.431 * obj.privity * obj.privityValue);
        			break;		
        	}	
        	 return gameSkill.SkillReamark.replace("N", attacknum);
        }
			
		public function GameSkillLevel(skill:GameSkill)
		{
			gameSkill = skill;
		}

	}
}