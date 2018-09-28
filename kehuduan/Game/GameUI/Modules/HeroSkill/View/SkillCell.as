package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.HeroSkill.Command.SkillEvent;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.View.items.SkillItem;
	
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class SkillCell extends Sprite
	{
		public var id:int;
		protected var level:uint = 0;																					//初始等级为0
		protected var isLearn:Boolean = false;
		protected var skillMode:uint;
		protected var cellJob:int;																						//单元格的职业
		
		protected var content_mc:MovieClip;
		protected var redFrame:MovieClip;
		protected var yellowFrame:MovieClip;
		protected var grid:MovieClip;
		protected var skillItem:SkillItem;
		
		protected var skillName:String;
		protected var needLevel:uint;
		
		public function SkillCell(_id:int,_job:int)
		{
			this.id = _id;
			this.cellJob = _job;
			checkLearn();
			initData();
			initUI();
		}
		
		protected function checkLearn():void
		{
			//普通技能
			if ( this.id==9508 || this.id==9509 || this.id==9000 )
			{
				isLearn = true;
				level = 1;
				return;
			}
			
			//隐藏技能
			if ( this.id == 9510 )
			{
				isLearn = false;
				return;
			}
			
			for (var skillRole:* in GameCommonData.Player.Role.SkillList)
			{
				var obj:Object = (GameCommonData.Player.Role.SkillList[skillRole] as GameSkillLevel);
				if ( this.id == obj.gameSkill.SkillID )
				{
					level = obj.Level;
					isLearn = true;
					return;
				}
			}
		}
		
		protected function initData():void
		{
			this.skillName = (GameCommonData.SkillList[id] as GameSkill).SkillName;
			this.needLevel = (GameCommonData.SkillList[id] as GameSkill).NeedLevel;
			this.skillMode = (GameCommonData.SkillList[id] as GameSkill).SkillMode;
		}
		
		protected function initUI():void
		{
			addMC();
			setTxtLine1();
			setTxtLine2();
			setItemBox();
		}
		
		protected function addMC():void
		{
			content_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillCell");
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillYellowFrame");
			grid = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");				
			redFrame.width = content_mc.width +3;
			redFrame.height = content_mc.height +1;
			redFrame.mouseEnabled = false;
			redFrame.mouseChildren = false;
			redFrame.x = content_mc.x -3;
			
			yellowFrame.width = 142;
			yellowFrame.height = content_mc.height +1;
			yellowFrame.mouseEnabled = false;
			yellowFrame.mouseChildren = false;
			this.addChild(content_mc);
			
			grid.x = 0;
			grid.y = 1;
			content_mc.addChild(grid);
			setGridName();
			skillItem = new SkillItem(this.id.toString(), grid);
			skillItem.x = 2;
			skillItem.y = 2;
//			grid.name = "skill_" +this.id;
			
			if ( !grid.contains(skillItem) )
			{
				grid.addChild(skillItem);
			}
			
			content_mc.txt1.mouseEnabled = false;
			content_mc.txt2.mouseEnabled = false;
			content_mc.txt3.mouseEnabled = false;
			content_mc.txt3.text = "";
			
			this.addEventListener(Event.ADDED_TO_STAGE,addStageHandler);
		}
		
		public function getSkillItem():SkillItem
		{
			return this.skillItem;
		}
		
		public function isNewSkill():Boolean
		{
			if( !this.isLearn && this.skillMode != 0) return true;
			return false;
		}
		
		protected function setGridName():void
		{
			grid.name = "skill_" +this.id;
		}
		
		protected function setTxtLine1():void
		{
			content_mc.txt1.text = skillName;
			if ( isLearn )
			{
				if ( GameCommonData.Player.Role.SkillList[id] )
				{
					content_mc.txt2.htmlText = (GameCommonData.Player.Role.SkillList[id] as GameSkillLevel).Level + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级";
				}else
				{
					content_mc.txt2.htmlText = "1"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				}
			}else
			{
				content_mc.txt2.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillCe_setT1" ]+"</font>";//未学会
			}
		}
		
		//技能学习cell需要重写此函数
		protected function setTxtLine2():void
		{
			if ( SkillData.curJobLevel<needLevel )
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillCe_setT2" ]+"："+needLevel+"</font>";//职业等级
			}
		}
		//技能面板重写此函数
		protected function setItemBox():void
		{
			var curJob:int = (GameCommonData.Player.Role.RoleList [GameCommonData.Player.Role.CurrentJob-1] as RoleJob).Job;
			if ( this.cellJob == -3 )
			{
				this.skillItem.Enabled = true;
			}
			else
			{
				if ( isLearn && (this.cellJob == curJob) )
				{
					this.skillItem.Enabled = true;
				}else
				{
					this.skillItem.Enabled = false;
//					this.skillItem.Enabled = false;
				}
			}
		}
		
		protected function addStageHandler(evt:Event):void
		{
			content_mc.addEventListener(MouseEvent.MOUSE_OVER,addRedFrame);
			content_mc.addEventListener(MouseEvent.CLICK,addYellowFrame);
			
			this.addEventListener(Event.REMOVED_FROM_STAGE,removeStageHandler);
		}
		
		protected function removeStageHandler(evt:Event):void
		{
			content_mc.removeEventListener(MouseEvent.CLICK,addYellowFrame);
			this.removeEventListener(Event.REMOVED_FROM_STAGE,removeStageHandler);
		}
		
		protected function addYellowFrame(evt:MouseEvent):void
		{
			this.dispatchEvent(new SkillEvent(SkillConst.CELLCLICK,id));
		}
		
		protected function addRedFrame(evt:MouseEvent):void
		{
			if ( !(content_mc.contains(redFrame)) )
			{
				content_mc.addChild(redFrame);
			}
			content_mc.addEventListener(MouseEvent.MOUSE_OUT,removeRedFrame);
			content_mc.removeEventListener(MouseEvent.MOUSE_OVER,addRedFrame);
		}
		
		protected function removeRedFrame(evt:MouseEvent):void
		{
			if ( content_mc.contains(redFrame) )
			{
				content_mc.removeChild(redFrame);
			}
			content_mc.removeEventListener(MouseEvent.MOUSE_OUT,removeRedFrame);
			content_mc.addEventListener(MouseEvent.MOUSE_OVER,addRedFrame);
		}
		
		public function getRemark(_level:int):String
		{
			if ( this.id == 12345678 )
			{
				return GameCommonData.wordDic[ "mod_her_med_skillCe_getR" ];//隐藏技能，需要技能书才能习得
			}
			
			var obj:Object = GameCommonData.SkillList[id] as GameSkill;
			var remark:String = obj.SkillReamark;
			var skillAtack:int = obj.Attack;
			var levelAtack:Number = obj.LevelAttack;
			var le:int;
			if ( _level-1 <= 0 )
			{
				le = 0;
			}else
			{
				le = _level - 1;
			}
			var str:String = remark.replace("N",Math.round((skillAtack+levelAtack*le)*10)/10);
			return str;
		}
		
		public function gc():void
		{
//			this.removeEventListener(Event.ADDED_TO_STAGE,addStageHandler);
		}
	}
}