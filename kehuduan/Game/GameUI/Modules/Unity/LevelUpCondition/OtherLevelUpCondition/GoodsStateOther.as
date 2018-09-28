package GameUI.Modules.Unity.LevelUpCondition.OtherLevelUpCondition
{
	import GameUI.Modules.Unity.InterUnityFace.IOtherState;
	import GameUI.Modules.Unity.LevelUpCondition.LevelWork;

	public class GoodsStateOther implements IOtherState
	{
		public function GoodsStateOther()
		{
		}

		public function writeCondition(work:OtherLevelWork):Boolean
		{
			return false;
		}
		
	}
}