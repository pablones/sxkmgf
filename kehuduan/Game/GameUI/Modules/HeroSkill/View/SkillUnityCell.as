package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityJopChange;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.events.MouseEvent;
	
	//帮派技能学习里单元格
	public class SkillUnityCell extends SkillCell
	{
		public var canLearn:Boolean = false;
		public var learnCellId:uint;
		public var learnCellLevel:uint;
		private var curJob:RoleJob;
		private var dataObj:Object = null;
		private var isMatch:Boolean = false;
		
		public var canLearnTen:Boolean = false;								//是否能学10级
		
		public var clickCellHandler:Function;
		
		public function SkillUnityCell( _id:int,_job:int = -6 )
		{
			this.learnCellId = _id;
			analyType();
			super(_id,_job);
//			this.learnCellLevel = this.level;
		}
		
		protected override function initData():void
		{
			this.skillName = (GameCommonData.SkillList[id] as GameSkill).SkillName;
			this.skillMode = (GameCommonData.SkillList[id] as GameSkill).SkillMode;
		}
		
		protected override function checkLearn():void
		{
//			checkCanPromt();
			if ( GameCommonData.Player.Role.SkillList [ this.learnCellId ] )
			{
				isLearn = true;
				this.learnCellLevel = ( GameCommonData.Player.Role.SkillList [ this.learnCellId ] as GameSkillLevel ).Level;
			}
			else
			{
				isLearn = false;
			}
		}
		
		//判断是否可提升
		public function checkCanPromt():void
		{
			if ( GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0 )
			{
				dealDataObj( this.dataObj );
				canLearn = getCanLearn();							//大家都能学，但不是都能用
			}
			else 
			{
				canLearn = false;
			}
			setTxtLine2();
		}
		
		protected override function setItemBox():void
		{
			if ( this.isLearn )
			{
				this.skillItem.Enabled = true;
			}
			else
			{
				this.skillItem.Enabled = false;
			}
		}
		
		//第二行不要
		protected override function setTxtLine2():void
		{
//			checkLearn();
			if ( this.canLearn )
			{
				content_mc.txt3.htmlText = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ]+"</font>";//可提升	
			}
			else
			{
				content_mc.txt3.htmlText = "";	
			}
		}
		
		protected override function addYellowFrame(evt:MouseEvent):void
		{
			if ( clickCellHandler != null )
			{
				clickCellHandler( this );
			}
		}
		
		public function skillUpDone(newLevel:int):void
		{
			this.level = newLevel;
			this.learnCellLevel = newLevel;
			content_mc.txt2.htmlText = "<font color = '#ffffff'>" + newLevel + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+"</font>";//级
			canLearn = getCanLearn();
			setTxtLine2();
			setItemBox();
			this.skillItem.Enabled = true;
			
		}
		
		private function analyType():void
		{
			if ( ( UnityConstData.greenUnityDataObj.skillIconList as Array ).indexOf( this.learnCellId )>-1 )
			{
				dataObj = UnityConstData.greenUnityDataObj;
			}
			else if ( ( UnityConstData.whiteUnityDataObj.skillIconList as Array ).indexOf( this.learnCellId )>-1 )
			{
				dataObj = UnityConstData.whiteUnityDataObj;
			}
			else if ( ( UnityConstData.redUnityDataObj.skillIconList as Array ).indexOf( this.learnCellId )>-1 )
			{
				dataObj = UnityConstData.redUnityDataObj;
			}
			else if ( ( UnityConstData.xuanUnityDataObj.skillIconList as Array ).indexOf( this.learnCellId )>-1 )
			{
				dataObj = UnityConstData.xuanUnityDataObj;
			}
		}
		 
		private function getCanLearn():Boolean
		{
			var can:Boolean = false;
			var index:int = ( dataObj.skillIconList as Array ).indexOf( this.learnCellId );
			if ( index == -1 ) return false;
			if ( dataObj[ "skillStudySelf"+( index+1 ) ] < dataObj[ "skillStudyCurr"+( index+1 ) ] )
			{
				can = true;
			}
			else
			{
				can = false;
			}
			
			if ( dataObj[ "skillStudySelf"+( index+1 ) ]+10 <= dataObj[ "skillStudyCurr"+( index+1 ) ] )
			{
				canLearnTen = true;
			}
			else
			{
				canLearnTen = false;
			}
			
			return can;
		}
		
		private function dealDataObj( obj:Object ):void
		{
			for(var n:int = 0;n < 3;n++)
			{
				var skillArray:Array = UnityJopChange.getSkillLevel(Number(obj["skillTolExp"+int(n+1)]));
				if(skillArray[0] >= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill")))
				{
					obj["skillStudyCurr"+int(n+1)] 	= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill"));
				}
				else
				{
					obj["skillStudyCurr"+int(n+1)] = skillArray[0];				//得到当前技能等级
				}
			}
		}
	}
}