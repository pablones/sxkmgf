package GameUI.Modules.Unity.LevelUpCondition
{
	/** 帮派升级需要相应等级的分堂扩建图纸一张(暂为空) */
	import GameUI.Modules.Unity.InterUnityFace.IState;

	public class GoodsState implements IState
	{
		public function writeCondition(work:LevelWork):Boolean
		{
			work.setState(new GoodsState());
			return work.showCondition();
		}
		
	}
}