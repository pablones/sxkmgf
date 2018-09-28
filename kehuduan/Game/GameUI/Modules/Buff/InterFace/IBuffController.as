package GameUI.Modules.Buff.InterFace
{
	import OopsEngine.Skill.GameSkillBuff;
	
	public interface IBuffController
	{
		function addBuff(buff:GameSkillBuff):void;		/** 添加buff deBuff */
		function deleteBuff(buff:GameSkillBuff):void;	/** 删除buff deBuff */
		function updateBuff():void;						/** 更新排列  */
		function BuffGo():void;							/** 每秒钟的操作 */
	}
}