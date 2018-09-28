package GameUI.Modules.Unity.InterUnityFace
{
	import GameUI.Modules.Unity.LevelUpCondition.LevelWork;
	
	public interface IState
	{
		function writeCondition(work:LevelWork):Boolean;
	}
}