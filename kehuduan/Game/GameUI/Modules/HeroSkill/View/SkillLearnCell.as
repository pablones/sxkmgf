package GameUI.Modules.HeroSkill.View
{
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkillLevel;
	
	public class SkillLearnCell extends SkillCell
	{
		public var canLearn:Boolean = false;
		public var learnCellId:uint;
		public var learnCellLevel:uint;
		private var curJob:RoleJob;
		
		public function SkillLearnCell(_id:int,_job:int)
		{
			super(_id,_job);
			this.learnCellId = _id;
			this.learnCellLevel = this.level;
		}
		
		protected override function setTxtLine2():void
		{
			curJob = GameCommonData.Player.Role.RoleList [GameCommonData.Player.Role.CurrentJob-1] as RoleJob;
			var mainJob:RoleJob = GameCommonData.Player.Role.RoleList [0] as RoleJob;
			var viceJob:RoleJob = GameCommonData.Player.Role.RoleList [1] as RoleJob;
			var curLevel:uint;
			if ( this.cellJob == mainJob.Job )
			{
				curLevel = mainJob.Level;
			}
			else if ( this.cellJob == viceJob.Job )
			{
				curLevel = viceJob.Level;
			}
			else
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_1" ]+"："+needLevel+"</font>";//职业等级
				canLearn = false;
				return;
			}
//			if ( this.cellJob != curJob.Job )
//			{
//				content_mc.txt3.htmlText = "<font color = '#ff0000'>职业等级："+needLevel+"</font>";
//				canLearn = false;
//				return;
//			}
			
			if ( curLevel<needLevel )
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_1" ]+"："+needLevel+"</font>";//职业等级
				canLearn = false;
			}
			else
			{
				if ( this.level < curLevel+10 )
				{
					content_mc.txt3.htmlText = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ]+"</font>"//可提升
				}else
				{
					content_mc.txt3.htmlText = "";
				}
				canLearn = true;
			}
		}
		
		protected override function setItemBox():void
		{
//			super.setItemBox();
			if ( this.isLearn )
			{
				this.skillItem.Enabled = true;
			}
			else
			{
				this.skillItem.Enabled = false;
			}
		}
		
		public function skillUpDone(newLevel:int):void
		{
			this.level = newLevel;
			this.learnCellLevel = newLevel;
			content_mc.txt2.htmlText = "<font color = '#ffffff'>" + newLevel + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+"</font>";//级
			if ( !this.isLearn )
			{
				this.isLearn = true;
				if ( this.id == 1104 || this.id == 1111 || this.id == 1204 || this.id == 1124 )
				{
					( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel ).IsAutomatism = false;
				}
			}
			setTxtLine2();
			setItemBox();
			this.skillItem.Enabled = true;
			
		}
	}
}