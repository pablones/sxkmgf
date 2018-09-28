package GameUI.Modules.HeroSkill.View
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	public class SkillPanelCell extends SkillCell
	{
		public var panCellLevel:uint;
		private var jobLevel:uint;																		//职业等级
		private var arrow_mc:MovieClip;
		private var cellIndex:int;																	//格子位置
		
		public function SkillPanelCell(_id:int,_job:int)
		{
			super(_id,_job);
			this.panCellLevel = this.level;															//技能等级
			
			arrow_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillArrow");
			arrow_mc.name = "skillArrow";
			
			setArrow();																	//是否打钩
		}
		
		protected override function setTxtLine1():void
		{
			super.setTxtLine1();
			if ( this.id==9510 )
			{
				content_mc.txt1.textColor = 0x666666;
				content_mc.txt2.htmlText = "<font color = '#ff0000'>未激活</font>";
			}
		}
		
		protected override function setTxtLine2():void
		{
			
			if ( (this.cellJob == -3) || (this.id == 9510) )
			{
				content_mc.txt3.htmlText = "";
			} else
			{
				if ( this.cellJob == GameCommonData.Player.Role.MainJob.Job )						//主职业设置第二行
				{
					jobLevel = GameCommonData.Player.Role.MainJob.Level;
				} 
				else if ( this.cellJob == GameCommonData.Player.Role.ViceJob.Job )
				{
					jobLevel = GameCommonData.Player.Role.ViceJob.Level;
				}
				dealLine2();
			}
		}
		
		private function dealLine2():void
		{
			if ( jobLevel<needLevel )
			{
				content_mc.txt3.htmlText = "<font color = '#ff0000'>"+GameCommonData.wordDic[ "mod_her_med_skillCe_setT2" ]+"："+needLevel+"</font>";//职业等级
			}
			else
			{
				if ( this.level < jobLevel+10 )
				{
					content_mc.txt3.htmlText = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_her_med_skillLe_setT_2" ]+"</font>"//可提升
				}
				else
				{
					content_mc.txt3.htmlText = "";
				}
			}
		}
		
		private function initIndex():void
		{
			var idArr:Array = [];
			if ( this.cellJob == (GameCommonData.Player.Role.RoleList [0] as RoleJob).Job )
			{
				for ( var i:uint=0; i<SkillData.aMainSkillObj.length; i++ )
				{
					idArr.push( SkillData.aMainSkillObj[i].id );
				}
			}
			else if ( this.cellJob == (GameCommonData.Player.Role.RoleList [1] as RoleJob).Job )
			{
				for ( var j:uint=0; j<SkillData.aViceSkillObj.length; j++ )
				{
					idArr.push( SkillData.aViceSkillObj[j].id );
				}
			}
			this.cellIndex = idArr.indexOf( this.id );
		}
		
		private function setArrow():void
		{
			initIndex();
//			trace ( " cell index is +"+this.cellIndex )
			if ( this.isLearn && (this.skillMode != 0) && GameSkillMode.IsPersonAutomatism(this.skillMode) && (this.cellJob != -3))
			{
				arrow_mc.x = 120;
				arrow_mc.y = 20;
				arrow_mc.buttonMode = true;
				arrow_mc.addEventListener(MouseEvent.MOUSE_DOWN,clickArrow);
				if ( !this.content_mc.contains(arrow_mc) )
				{
					this.content_mc.addChild(arrow_mc);
				}
				
				if ( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel )
				{
					if ( ( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel ).IsAutomatism == false )
					{
						arrow_mc.gotoAndStop(1);
						return;
					}
					if ( cellJob == GameCommonData.Player.Role.MainJob.Job )
					{
						if ( SkillData.aMainAutoIndex[cellIndex] == 1 )
						{
							setAuto( true );
						}
						else
						{
							setAuto( false );
						}
					}
					else if ( cellJob == GameCommonData.Player.Role.ViceJob.Job )
					{
						if ( SkillData.aViceAutoIndex[cellIndex] == 1 )
						{
							setAuto( true );
						}
						else
						{
							setAuto( false );
						}
					}

				}
				
			}
		}
		
		private function setAuto(value:Boolean):void
		{
			if ( value )
			{
				(GameCommonData.Player.Role.SkillList[id] as GameSkillLevel).IsAutomatism = true;
				arrow_mc.gotoAndStop(2);
			}
			else
			{
				(GameCommonData.Player.Role.SkillList[id] as GameSkillLevel).IsAutomatism = false;
				arrow_mc.gotoAndStop(1);
			}
		}
		
		private function clickArrow(evt:MouseEvent):void
		{
			evt.stopPropagation();
//			trace ( "this is cellJob "+this.cellJob );
			var deIndex:int = SkillConst.aAutoPlay.indexOf(id);
			if ( arrow_mc.currentFrame == 1 )
			{
				arrow_mc.gotoAndStop(2);
				(GameCommonData.Player.Role.SkillList[id] as GameSkillLevel).IsAutomatism = true;
				if ( deIndex == -1 )
				{
					SkillConst.aAutoPlay.push(this.id);
				}
				if ( cellJob == GameCommonData.Player.Role.MainJob.Job )
				{
					SkillData.aMainAutoIndex[cellIndex] = 1; 
				}
				if ( cellJob == GameCommonData.Player.Role.ViceJob.Job )
				{
					SkillData.aViceAutoIndex[cellIndex] = 1; 
				}
			}
			else
			{
				arrow_mc.gotoAndStop(1);
				(GameCommonData.Player.Role.SkillList[id] as GameSkillLevel).IsAutomatism = false;
				if ( deIndex != -1 )
				{
					SkillConst.aAutoPlay.splice(deIndex,1);
				}
				if ( cellJob == GameCommonData.Player.Role.MainJob.Job )
				{
					SkillData.aMainAutoIndex[cellIndex] = 0; 
				}
				if ( cellJob == GameCommonData.Player.Role.ViceJob.Job )
				{
					SkillData.aViceAutoIndex[cellIndex] = 0; 
				}
			}
//			trace("发送数组是啥："+SkillConst.aAutoPlay);
		}
		
		protected override function setItemBox():void
		{
			super.setItemBox();
			var cJob:int = (GameCommonData.Player.Role.RoleList [GameCommonData.Player.Role.CurrentJob-1] as RoleJob).Job;
			if ( (this.skillMode != 0) && isLearn && ((this.cellJob == cJob) || (this.cellJob == -3)) )
			{
				grid.addEventListener(MouseEvent.MOUSE_DOWN, onClickBox);
//				skillItem.addEventListener(DropEvent.DRAG_DROPPED, onDragDropped);
			}
		}
		
		protected override function addStageHandler(evt:Event):void
		{
			super.addStageHandler( evt );
			var cJob:int = (GameCommonData.Player.Role.RoleList [GameCommonData.Player.Role.CurrentJob-1] as RoleJob).Job;
			if ( (this.skillMode != 0) && isLearn && ((this.cellJob == cJob) || (this.cellJob == -3)) )
			{
				if ( !grid.hasEventListener( MouseEvent.MOUSE_DOWN ) )
				{
					grid.addEventListener(MouseEvent.MOUSE_DOWN, onClickBox);
				}
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
		
//		private function onDragDropped(event:DropEvent):void
//		{
//			if((event.Data.target.name.split("_")[0] == "quick" ||  event.Data.target.name.split("_")[0] == "quickf") && !event.Data.target.contains(event.Data.source))
//			{
////				this.dispatchEvent(new DropEvent(DropEvent.SKILLITEMDRAGED,event.Data)); 
//			}
//		}
		
		protected override function removeStageHandler(evt:Event):void
		{
			super.removeStageHandler(evt);
		}
		
		//升级成功之后执行此函数
		public function skillUpDone(newLevel:uint):void
		{
			this.panCellLevel = newLevel;
			this.level = newLevel;
			this.skillItem.Enabled = true;
			content_mc.txt2.htmlText = "<font color = '#ffffff'>" + newLevel + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+"</font>";//级
			if ( !this.isLearn )
			{
				this.isLearn = true;
				if ( this.id == 1104 || this.id == 1111 || this.id == 1204 || this.id == 1124 )
				{
					setAuto( false );
				}
				else
				{
					setArrow();
				}
			}
			setTxtLine2();
			if ( (this.skillMode != 0) && this.isLearn )
			{
				grid.addEventListener(MouseEvent.MOUSE_DOWN, onClickBox);
//				skillItem.addEventListener(DropEvent.DRAG_DROPPED, onDragDropped);
			}
//			setArrow();
		}
		
		public override function gc():void
		{
			if ( grid.hasEventListener( MouseEvent.MOUSE_DOWN ) )
			{
				grid.removeEventListener(MouseEvent.MOUSE_DOWN, onClickBox);
			}
		}
		
	}
}