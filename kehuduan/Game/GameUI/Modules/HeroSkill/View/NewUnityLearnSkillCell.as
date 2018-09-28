package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.UIUtils;
	import GameUI.View.items.SkillItem;
	
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class NewUnityLearnSkillCell extends Sprite
	{
		public var id:int;
		public var level:int;
		public var clickSkillCell:Function;
		public var remark:String;
		public var gameSkill:GameSkill;
		public var canPromt:Boolean = false; 
		public var canPromt_ten:Boolean = false;				//是否能升到10级
		
		private var content_mc:MovieClip;
		protected var grid:MovieClip;
		protected var skillItem:SkillItem;
		
		private var redFrame:Shape;
		
		public function NewUnityLearnSkillCell( _id:int )
		{
			id = _id;
			gameSkill = GameCommonData.SkillList[ _id ];
		}
		
		public function init():void
		{
			checkSkillLevel();
			initUI();
			
			addEventListener( MouseEvent.MOUSE_OVER,onMouseOver );
			addEventListener( MouseEvent.MOUSE_OUT,onMouseOut );
			addEventListener( MouseEvent.CLICK,onClick );
		}
		
		private function onMouseOver( evt:MouseEvent ):void
		{
			if ( !this.contains( redFrame ) )
			{
				addChild( redFrame );
			}
		}
		
		private function onMouseOut( evt:MouseEvent ):void
		{
			if ( this.contains( redFrame ) )
			{
				removeChild( redFrame );
			}
		}
		
		private function onClick( evt:MouseEvent ):void
		{
			if ( this.clickSkillCell != null )
			{
				clickSkillCell( this );
			}
		}
		
		protected function checkSkillLevel():void
		{
			if ( !GameCommonData.Player.Role.SkillList[ id ] )
			{
				level = 0;
			}
			else
			{
				level = ( GameCommonData.Player.Role.SkillList[ id ] as GameSkillLevel ).Level;
			}
		}
		
		public function initUI():void
		{
			addMC();
			setTxtLine1();
			setTxtLine2();
			setItemBox();
			
			redFrame = UIUtils.createFrame( 185,46,0xff0000 );
		}
		
		protected function addMC():void
		{
			content_mc = new ( NewUnityCommonData.newUnityResProvider.SingleUnitySkillCellClass ) as MovieClip; 
			grid = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");				
			this.addChild(content_mc);
			
			grid.x = 4;
			grid.y = 5;
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
		}
		
		public function getSkillItem():SkillItem
		{
			return this.skillItem;
		}
		
		protected function setGridName():void
		{
			grid.name = "skill_" +this.id;
		}
		
		protected function setTxtLine1():void
		{
			content_mc.txt1.text = gameSkill.SkillName;
			if ( this.level>0 )
			{
				content_mc.txt2.htmlText = "<font color = '#e2cca5'>"+ level.toString() + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "</font>";//"级";
			}
			else
			{
				content_mc.txt2.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillCe_setT1" ]+"</font>";//未学会
			}
		}
		
		//判断是否可提升
		protected function setTxtLine2():void
		{
			//等级是否足够
			if ( GameCommonData.Player.Role.Level<gameSkill.NeedLevel )
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>" + GameCommonData.wordDic[ "mod_hin_med_skillC_che" ] + "："+gameSkill.NeedLevel+"</font>";//需求等级
				canPromt = false;
				return;
			}
			
			if ( SkillData.canPromtUnitySkill( id ) )
			{
				content_mc.txt3.htmlText = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ]+"</font>";//可提升
				canPromt = true;
			}
			else
			{
				content_mc.txt3.htmlText = "";
				canPromt = false;
			}
			
			canPromt_ten = SkillData.canPromtUnitySkill( id,9 );
		}
		//技能面板重写此函数
		protected function setItemBox():void
		{
			if ( level>0 )
			{
				this.skillItem.Enabled = true;
			}
			else
			{
				this.skillItem.Enabled = false;
			}
		}
		
		public function gc():void
		{
			removeEventListener( MouseEvent.MOUSE_OVER,onMouseOver );
			removeEventListener( MouseEvent.MOUSE_OUT,onMouseOut );
			removeEventListener( MouseEvent.CLICK,onClick );
			
			var des:Object;
			while ( this.numChildren>0 )
			{
				des = this.removeChildAt( 0 );
				des = null;	
			}
		}
		
		public function getRemark(_level:int):String
		{
			var remark:String = gameSkill.SkillReamark;
			var skillAtack:int = gameSkill.Attack;
			var levelAtack:Number = gameSkill.LevelAttack;
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
		
		public function skillUpDone( newLevel:int ):void
		{
			checkSkillLevel();
			setTxtLine1();
			setTxtLine2();
			setItemBox();
		}
	}
}