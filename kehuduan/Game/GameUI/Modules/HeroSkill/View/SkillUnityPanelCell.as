package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.DisplayObject;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class SkillUnityPanelCell extends SkillCell
	{
		public var canLearn:Boolean = false;
		public var learnCellId:uint;
		public var learnCellLevel:uint;
		private var curJob:RoleJob;
		private var dataObj:Object = null;
		private var isMatch:Boolean = false;
		
		public var clickUnityCellHandler:Function;
		
		public function SkillUnityPanelCell( _id:int,_job:int = -6 )
		{
			this.learnCellId = _id;
			this.id = _id;
			checkSkillLevel();
			super(_id,_job);
		}
		
		protected override function checkLearn():void
		{
			canLearn = SkillData.canPromtUnitySkill( this.id ); 
		}
		
		protected function checkSkillLevel():void
		{
			if ( !GameCommonData.Player.Role.SkillList[ id ] )
			{
				level = 0; 
				learnCellLevel = 0;
			}
			else
			{
				level = ( GameCommonData.Player.Role.SkillList[ id ] as GameSkillLevel ).Level;
				learnCellLevel = level;
			}
		}
		
		protected override function setTxtLine1():void
		{
			content_mc.txt1.text = skillName;
			if ( this.level>0 )
			{
				content_mc.txt2.htmlText = "<font color = '#ffffff'>"+ level.toString() + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "</font>";//"级";
			}
			else
			{
				content_mc.txt2.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillCe_setT1" ]+"</font>";//未学会
			}
		}
		
		//判断是否可提升
		public function checkCanPromt():void
		{
			setTxtLine2();
		}
		
		//第二行不要
		protected override function setTxtLine2():void
		{
			//等级是否足够
			if ( GameCommonData.Player.Role.Level<this.needLevel )
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>" + "需求等级" + "："+this.needLevel+"</font>";//职业等级
				canLearn = false;
				return;
			}
			
			if ( SkillData.canPromtUnitySkill( this.id ) )
			{
				content_mc.txt3.htmlText = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ]+"</font>";//可提升	
			}
			else
			{
				content_mc.txt3.htmlText = "";	
			}
		}
		
		protected override function setItemBox():void
		{
			if ( this.level>0 )
			{
				this.skillItem.Enabled = true;
				if ( skillMode != 0 )
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
				try
				{
					skillItem.onMouseDown();	
				}
				catch (e:Error)
				{
					
				}
				
			}
		}
		
		protected override function removeStageHandler(evt:Event):void
		{
			super.removeStageHandler(evt);
		}
		
		protected override function addYellowFrame( evt:MouseEvent ):void
		{
			if ( clickUnityCellHandler != null )
			{
				clickUnityCellHandler( this );
			}
		}
		
		public function skillUpDone(newLevel:int):void
		{
			this.level = newLevel;
			this.learnCellLevel = newLevel;
			checkSkillLevel();
			setTxtLine1();
			setTxtLine2();
			setItemBox();
		}
		
	}
}