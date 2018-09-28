package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.HeroSkill.Command.SkillEvent;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.View.items.DropEvent;
	
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.DisplayObject;
	import flash.events.MouseEvent;
	
	public class SkillLifeCell extends SkillCell
	{
		public var lifeCellId:uint;							//自己的id
		public var lifeLevel:uint;
		public var canLearn:Boolean = false;      
		public var haveLearned:Boolean = false;
		private var panId:uint = 0;						// 1为技能面板  2为技能学习
		public var familiar:uint = 0;
		
		public function SkillLifeCell(_id:int, _job:int,_panId:uint)
		{
			if ( GameCommonData.Player.Role.LifeSkillList[_id] )
			{
				familiar = ( GameCommonData.Player.Role.LifeSkillList[_id] as GameSkillLevel ).Familiar;
			}
			this.panId = _panId;
			super(_id, _job);
			this.lifeCellId = this.id;
//			this.lifeLevel = this.level;
		}
		
		protected override function checkLearn():void
		{
			if ( GameCommonData.Player.Role.LifeSkillList[id] )
			{
				this.haveLearned = true;	
				this.isLearn = true;
				this.lifeLevel =  ( GameCommonData.Player.Role.LifeSkillList[id] as GameSkillLevel).Level;
			}
			else
			{
				this.haveLearned = false;	
				this.isLearn = false;
				this.lifeLevel = 0;
			}
//			this.haveLearned = true;															//测试
		}
		
		protected override function initData():void
		{
			this.skillName = (GameCommonData.LifeSkillList[id] as GameSkill).SkillName;
			this.needLevel = (GameCommonData.LifeSkillList[id] as GameSkill).NeedLevel;
			this.skillMode = (GameCommonData.LifeSkillList[id] as GameSkill).SkillMode;
			this.cellJob = (GameCommonData.LifeSkillList[id] as GameSkill).Job;
		}
		
		protected override function setGridName():void
		{
			this.grid.name = "lifeSkill_"+this.id;
		}
		
		protected override function setTxtLine1():void
		{
//			this.grid.name = "lifeSkill_"+this.id;  
			
			content_mc.txt1.text = skillName;
			if ( haveLearned )
			{
				if ( GameCommonData.Player.Role.LifeSkillList[id] )
				{
					content_mc.txt2.htmlText = (GameCommonData.Player.Role.LifeSkillList[id] as GameSkillLevel).Level + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级";
				}
			}
			else
			{
				content_mc.txt2.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillCe_setT1" ]+"</font>";//未学会
			}
		}
		
		protected override function setTxtLine2():void
		{
			if ( GameCommonData.Player.Role.Level < this.needLevel )
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillLie_setT" ]+"："+needLevel+"</font>";//人物等级
				canLearn = false;
			}
			else
			{
				canLearn = true;
//				if ( this.cellJob == -8 )
//				{
//					if ( this.familiar >= this.lifeLevel*1000 && this.lifeLevel<10 && GameCommonData.Player.Role.Level>=(this.lifeLevel*5)+20 )
//					{
//						content_mc.txt3.htmlText = "<font color = '#00ff00'>可提升</font>";
//					}
//					else
//					{
//						content_mc.txt3.htmlText = "";
//					}
//				}
//				else if ( this.cellJob == -50 )
//				{
//					if ( this.familiar >= this.lifeLevel*1000 && this.lifeLevel<10 && GameCommonData.Player.Role.Level>=(this.lifeLevel*10)+20 )
//					{
//						content_mc.txt3.htmlText = "<font color = '#00ff00'>可提升</font>";
//					}
//					else
//					{
//						content_mc.txt3.htmlText = "";
//					}
//				}
				if ( checkCanPromote() )
				{
					content_mc.txt3.htmlText = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ]+"</font>";//可提升
				}
				else
				{
					content_mc.txt3.htmlText = "";
				}
			}
		}
		
		protected override function setItemBox():void
		{
			if ( haveLearned )
			{
				this.skillItem.Enabled = true;
//				trace ( "cellJob   : "+this.cellJob );
				if ( this.panId == 1 && (this.cellJob != -50) )
				{
					grid.addEventListener(MouseEvent.MOUSE_DOWN, onClickBox);	
				}
			}
			else
			{
				this.skillItem.Enabled = false;
			}
		}
		
		private function onClickBox(evt:MouseEvent):void
		{
			var displayObj:DisplayObject = evt.target as DisplayObject;
			if(displayObj.mouseX <= 2 || displayObj.mouseX >= displayObj.width - 2){
				return;
			}
			if(displayObj.mouseY <= 2 || displayObj.mouseY >= displayObj.height - 2){
				return;
			}
			if ( skillItem )
			{
				skillItem.onMouseDown();
			}
		}
		
		private function onDragDropped(evt:DropEvent):void
		{
			
		}
		
		public override function getRemark(_level:int):String
		{
			var remark:String;
			remark = ( GameCommonData.LifeSkillList[id] as GameSkill ).SkillReamark;
			return remark;
		}
		
		protected override function addYellowFrame(evt:MouseEvent):void
		{
			dispatchEvent(new SkillEvent(SkillConst.LIFECELL_CLICK,this.id));
		}
		
		public function skillUpDone(newLevel:uint,newFamiliar:int):void
		{
			this.level = newLevel;
			this.lifeLevel = newLevel;
			this.familiar = newFamiliar;
			content_mc.txt2.htmlText = "<font color = '#ffffff'>" + newLevel + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+"</font>";//级
			if ( !this.isLearn )
			{
				this.isLearn = true;
				this.haveLearned = true;
			}
			this.skillItem.Enabled = true;
			setTxtLine2();
		}
		
		//检查是否可提升
		private function checkCanPromote():Boolean
		{
			if ( !isLearn )
			{
				return true;
			}
			else
			{
				if ( this.lifeLevel>=10 )
				{
					return false;
				}
				if ( this.cellJob == -8 )
				{
					if ( GameCommonData.Player.Role.Level<(this.lifeLevel*5)+20 )
					{
						return false;
					}
					if ( familiar < 200*Math.pow( 2,lifeLevel-1 ) )
					{
						return false;
					}
				}
				else if ( this.cellJob == -50 )
				{
					if ( GameCommonData.Player.Role.Level<(this.lifeLevel*10)+20 )
					{
						return false;
					}
					if (  familiar < ManufactoryData.aLifeSkillFam[ this.lifeLevel-1 ] )
					{
						return false;
					}
				}
			}
			return true;
		}
		
	}
}