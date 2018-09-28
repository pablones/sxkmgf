package GameUI.Modules.Buff.UI
{
	
	import GameUI.Modules.Login.SetFrame.SetFrame;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Skill.GameSkillBuff;

	public class DeBuffItem extends BuffItem
	{
		public function DeBuffItem(buff:GameSkillBuff)
		{
			super(buff);
		}
		public override function loadBuff(buff:GameSkillBuff):void
		{
			super.loadBuff(buff);
			SetFrame.UseFrame(this.buffSprite , "RedFrame" , 2 , 2 , 32 ,32);
		}
		public override function createTxt():void
		{
			super.createTxt();
			timeField.filters = Font.Stroke(0xFF0000);
		}
		
	}
}