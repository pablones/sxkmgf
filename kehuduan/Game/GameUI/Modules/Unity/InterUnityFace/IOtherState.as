package GameUI.Modules.Unity.InterUnityFace
{
	import GameUI.Modules.Unity.LevelUpCondition.OtherLevelUpCondition.OtherLevelWork;
	
	public interface IOtherState
	{
		function writeCondition(work:OtherLevelWork):Boolean

	}
}