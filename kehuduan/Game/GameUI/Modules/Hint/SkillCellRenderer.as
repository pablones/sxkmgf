package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.HeroSkill.Command.SkillDispatch;
	import GameUI.Modules.HeroSkill.Command.SkillEvent;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.SkillItem;
	
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class SkillCellRenderer extends Sprite
	{
		public var id:int;
		public var skillLevel:int = 0;									//应该为0，测试用	
		public var index:int;
				
		protected var content:MovieClip;
		private var redFrame:MovieClip;
		private var yellowFrame:MovieClip;
		public var isLearn:Boolean = false;
		private var learnIndex:int;
		private var skillItem:SkillItem;
		private var grid:MovieClip;
		private var type:String;
		private var canLearn:Boolean;
		private var skillMode:uint;
		
		//已学技能信息
		private var aLearnID:Array;                                      //已学技能ID
		private var aLearnLevel:Array;									 //已学技能等级			
		private var aSkillLearn:Array;									//已学技能所有信息	
		
		public function SkillCellRenderer(index:int,aSkillID:Array,type:String)
		{
			super();
			this.index = index;
			this.id = aSkillID[index];
			this.type = type;
			content=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillCell");
			redFrame=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			yellowFrame=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillYellowFrame");
			grid = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");			
			redFrame.width =content.width +3;
			redFrame.height = content.height +1;
			redFrame.mouseEnabled = false;
			
			yellowFrame.width = 143;
			yellowFrame.height = content.height +1;
			yellowFrame.mouseEnabled = false;
			this.addChild(content);
			content.addChild(grid);
			grid.x = 0;
			grid.y = 1;
			initData();
			//trace("this cell id is " +this.id);
			initUI();
		}
		
		private function initData():void
		{
			aSkillLearn = [];
			aLearnID = [];
			aLearnLevel = [];
			content.txt1.mouseEnabled = false;
			content.txt2.mouseEnabled = false;
			content.txt3.mouseEnabled = false;
			
			skillItem = new SkillItem(this.id.toString(), grid);
			skillItem.x = 2;
			skillItem.y = 2;
			grid.name = "skill_" +this.id;
			grid.addChild(skillItem);
			
			//已学技能信息
			for(var skillRole:* in GameCommonData.Player.Role.SkillList)
			{
				//trace("skill role " +(GameCommonData.Player.Role.SkillList[skillRole] as GameSkillLevel ).gameSkill);
				aSkillLearn.push((GameCommonData.Player.Role.SkillList[skillRole] as GameSkillLevel ).gameSkill);
				aLearnLevel.push((GameCommonData.Player.Role.SkillList[skillRole] as GameSkillLevel ).Level);
			}
			
			for(var i:int = 0;i < aSkillLearn.length;i++)
			{
				aLearnID.push(aSkillLearn[i].SkillID);
				
				if(aSkillLearn[i].SkillID == this.id)
				{
					isLearn = true;
					skillLevel = aLearnLevel[i];
					//skillLevel = 150;
				}
				
			}
			//判断是否是主动技能，如果为0，则是被动技能
			for(var skill:*  in GameCommonData.SkillList)
			{
				if(this.id == (GameCommonData.SkillList[skill] as GameSkill).SkillID)
				{
					this.skillMode = (GameCommonData.SkillList[skill] as GameSkill).SkillMode;
				}
			}
			
//			trace("skillMode ============" +this.skillMode);
			//trace("this cell id is "+this.id +"    aLearnID = " +aLearnID +"Skill level is " +skillLevel);
		}
		
		protected function initUI():void
		{
			this.addEventListener(Event.ADDED_TO_STAGE,addThis);
		}
		
		protected function onclickHandler(e:MouseEvent):void
		{
			this.dispatchEvent(new SkillEvent(SkillConst.CELLCLICK,index));
			SkillDispatch.getInstance().addEventListener(SkillConst.CHANGEF,removeYellowFrame);
			
		}
		
		public function loadSource(name:String,needLevel:int):void
		{
//			initData();
//			isCanLearn(needLevel);
			//trace("need   level is " +needLevel);
			//trace("当前人物等级 " +SkillConst.heroLevel);
			
			if(!isLearn)
			{
				skillItem.Enabled = false;
				content.txt2.text = GameCommonData.wordDic[ "mod_her_med_skillCe_setT1" ];//"未学会";
				changeColor(0xff0000);
//				isCanLearn(needLevel);
				
//				grid.addEventListener(MouseEvent.MOUSE_DOWN, onClickBox);
//				skillItem.addEventListener(DropEvent.DRAG_DROPPED, onDragDropped);     //此2行测试用
			}else
			{
				skillItem.Enabled = true;
				changeColor(0xffffff);
				content.txt2.text = skillLevel.toString() +GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级";
				//判断技能等级和人物等级
				checkLevel(skillLevel,false);
				if(this.skillMode != 0)
				{
					grid.addEventListener(MouseEvent.MOUSE_DOWN, onClickBox);
					skillItem.addEventListener(DropEvent.DRAG_DROPPED, onDragDropped);
				}
			}
			
			content.txt3.text = "" ;
			
		}
		
		private function isCanLearn(needLevel:int):void
		{
			if(SkillConst.heroLevel >= needLevel)
			{
				canLearn = true;
			}else
			{
				canLearn = false;
			}
			checkCanLearn(canLearn,needLevel);
		}
		
		private function checkLevel(level:int,_isLearn:Boolean):void
		{
			//trace("this level is " +SkillConst.heroLevel  +"level  =   "+level);
			if(level < SkillConst.heroLevel + 10)
				{
					if(_isLearn)
					{
						content.txt3.text = GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ];//"可提升";
						content.txt3.textColor = 0x00ff00;
					}else
					{
						content.txt3.text = "";
					}
				}else if(skillLevel == SkillConst.heroLevel+ 10)
				{
					content.txt3.text = "" ;
				}else
				{
					content.txt3.text = "" ;
				}
		}
		
		public function loadLearnSource(name:String,needLevel:int):void
		{
//			initData();
			isCanLearn(needLevel);
			//trace("当前人物等级 " +SkillConst.heroLevel);
			content.txt1.textColor = 0xffffff;
			content.txt2.textColor = 0xffffff;
			
			if(isLearn)
			{
				content.txt2.text = (skillLevel).toString() + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级";
				skillItem.Enabled = true;
				changeColor(0xffffff);
				checkLevel(this.skillLevel,true);
			}else
			{
				content.txt2.text = GameCommonData.wordDic[ "mod_her_med_skillCe_setT1" ];//"未学会";
				skillItem.Enabled = false;
				changeColor(0xff0000);
				checkCanLearn(canLearn,needLevel);
			}
			
		}
		
		//改变文本颜色
		private function changeColor(color:uint):void
		{
			content.txt1.textColor = 0xffffff;
			content.txt2.textColor = color;
			content.txt3.textColor = color;
		}
		
		private function checkCanLearn(canLearn:Boolean,_needLevel:int):void
		{
			
			if(canLearn)
			{
				content.txt3.text = GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ];//"可提升";
				content.txt3.textColor = 0x00ff00;
			}else
			{
				//trace("this .id  "+this.id +" this.needLevel  ++" +_needLevel +"  this . index is "+this.index);
				content.txt3.text = GameCommonData.wordDic[ "mod_hin_med_skillC_che" ]+"："+_needLevel;//需求等级
			}
		}
		
		private function onClickBox(event:MouseEvent):void
		{
			skillItem.onMouseDown();
		}
		
		private function onDragDropped(event:DropEvent):void
		{
			//trace(event.Data);
			for(var i:* in event.Data)
			{
//				trace(i," : ", event.Data[i]);
			}
			if((event.Data.target.name.split("_")[0] == "quick" ||  event.Data.target.name.split("_")[0] == "quickf") && !event.Data.target.contains(event.Data.source))
			{
//				trace("event dispatcher ");
				this.dispatchEvent(new DropEvent(DropEvent.SKILLITEMDRAGED,event.Data));
			}
		}
		
		//技能升级成功后执行
		public function upSkillLevel(level:int):void
		{
			this.skillItem.Enabled = true;
			this.skillLevel = level;
			content.txt2.text = (level).toString() +GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//"级"
			if(level == 1)
			{
				changeColor(0xffffff);
				//this.skillItem.Enabled = true;
			}
			checkLevel(level,true);
			this.skillItem.Enabled = true;
		}
		
		private function addRedFrame(evt:MouseEvent):void
		{
			if(!(content.contains(redFrame)))
			{
				content.addChild(redFrame);
			}
			content.addEventListener(MouseEvent.MOUSE_OUT,removeRedFrame);
		}
		
		private function removeRedFrame(evt:MouseEvent):void
		{
			if(content.contains(redFrame))
			{
				content.removeChild(redFrame);
			}
		}
		
		private function addYellowFrame(evt:MouseEvent):void
		{
			SkillDispatch.getInstance().dispatchEvent(new Event(SkillConst.CHANGEF));
			if(!(content.contains(yellowFrame)))
			{
				content.addChild(yellowFrame);
			}
			
		}
		
		public function showContent(index:int):void
		{
			
		}
		
		private function removeYellowFrame(evt:Event):void
		{
			if(content.contains(yellowFrame))
			{
				content.removeChild(yellowFrame);
			}
		}
		
		private function addThis(evt:Event):void
		{
			this.addEventListener(MouseEvent.CLICK,onclickHandler);
			content.addEventListener(MouseEvent.MOUSE_OVER,addRedFrame);
			//content.addEventListener(MouseEvent.MOUSE_OUT,removeRedFrame);
			content.addEventListener(MouseEvent.CLICK,addYellowFrame);
			this.addEventListener(Event.REMOVED_FROM_STAGE,removeThis);
		}
		
		private function removeThis(evt:Event):void
		{
			this.removeEventListener(MouseEvent.CLICK,onclickHandler);
			content.removeEventListener(MouseEvent.MOUSE_OVER,addRedFrame);
			content.removeEventListener(MouseEvent.MOUSE_OUT,removeRedFrame);
			content.removeEventListener(MouseEvent.CLICK,addYellowFrame);
			this.removeEventListener(Event.REMOVED_FROM_STAGE,removeThis);
			if(content.contains(yellowFrame))
			{
				content.removeChild(yellowFrame);
			}
		}
		
	}
}