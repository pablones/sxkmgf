package GameUI.Modules.Unity.LevelUpCondition
{
	/** 帮派升级的工作控制类*/
	import GameUI.Modules.Unity.InterUnityFace.IState;
	
	public class LevelWork
	{
		private var condition:IState;
		public function LevelWork()
		{
			condition = new BuiltState();
		}
		public function setState(conditionState:IState):void
		{
			condition = conditionState;
		}
		public function showCondition():Boolean
		{
			return(condition.writeCondition(this));
		}
	}
}