package GameUI.Modules.Unity.LevelUpCondition.OtherLevelUpCondition
{
	/** 分堂升级的工作控制类*/
	import GameUI.Modules.Unity.InterUnityFace.IOtherState;
	import GameUI.Modules.Unity.LevelUpCondition.LevelWork;

	public class OtherLevelWork
	{
		private var _obj:Object;
		private var condition:IOtherState;
		public function OtherLevelWork(obj:Object)
		{
			_obj  	  = obj;
			condition = new BuiltStateOther(obj);
		}
		public function setState(_condition:IOtherState):void
		{
			condition = _condition;
		}

		public function showCondition():Boolean
		{
			return condition.writeCondition(this);
		}
		
	}
}