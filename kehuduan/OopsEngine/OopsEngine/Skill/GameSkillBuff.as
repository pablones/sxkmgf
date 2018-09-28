package OopsEngine.Skill
{
	public class GameSkillBuff
	{
		//持续加血    1
		//持续掉血    2
		//加增益状态  3
		//减益状态    4
		
		//Buff的编号 
		public var BuffID:int;
		//buff类别 用于控制播放动画
		public var BuffType:int;
		//buff名称
		public var BuffName:String
		//buff时间
		public var BuffTime:int;
		//buff图片编号
	    public var TypeID:uint;
	    //buff特效啊
	    public var BuffEffect:String;
	    
	    public static function IsBuff(buffType:int):Boolean
	    {
	    	if(buffType == 1 || buffType == 3)
	    	{
	    		return true;
	    	}
	    	return false;
	    }
		
		/**用于判断BUFF显示的状态 1 绿字 2红字 3不显示**/
		public static function IsShowState(buffType:int):int
		{
			var state:int = 3
			
			if(buffType == 1)
			{
				state = 1;
			}
			
			if(buffType == 2)
			{
				state = 2;
			}
			
			return state;
		}
	}
}