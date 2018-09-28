package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Role.GameRole;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	
	public class SetBloodToolTip implements IToolTip
	{
		private var toolTip:Sprite;
		private var back:MovieClip;
		private var content:Sprite;
		
		private var roleType:String = "";
		private var roleObj:GameRole;				//组队数据位置
		private var isSelf:Boolean = false;
		private var maxWidth:uint;
		
		public function SetBloodToolTip(view:Sprite, role:String, data:GameRole = null, self:Boolean = false)
		{
			toolTip = view;
			roleType = role;
			roleObj = data;
			isSelf = self;
		}
		
		public function GetType():String
		{
			return "SetBloodToolTip";
		}
		
		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackSmall");
			toolTip.addChild(back);
			setContent();
			upDatePos();
		}
		
		public function Update(obj:Object):void
		{
			
		}
		
		private function setContent():void
		{
			//#4E3D14
			content = new Sprite();
			var contents:Array = [];
			if(roleType == "SelfRole")
			{
				contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+GameCommonData.Player.Role.HP+"/"+(GameCommonData.Player.Role.MaxHp + GameCommonData.Player.Role.AdditionAtt.MaxHP), color:IntroConst.COLOR},  //血: 
							{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_2" ]+GameCommonData.Player.Role.MP+"/"+(GameCommonData.Player.Role.MaxMp + GameCommonData.Player.Role.AdditionAtt.MaxMP), color:IntroConst.COLOR},  //气:
							{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_3" ]+GameCommonData.Player.Role.SP+"/"+(GameCommonData.Player.Role.MaxSp + GameCommonData.Player.Role.AdditionAtt.MaxSP), color:IntroConst.COLOR},  //怒: 
							];
			}
			if(roleType == "CounterWorker")
			{
				if(isSelf)
				{
					contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+GameCommonData.Player.Role.HP+"/"+(GameCommonData.Player.Role.MaxHp + GameCommonData.Player.Role.AdditionAtt.MaxHP), color:IntroConst.COLOR}, //血: 
							{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_2" ]+GameCommonData.Player.Role.MP+"/"+(GameCommonData.Player.Role.MaxMp + GameCommonData.Player.Role.AdditionAtt.MaxMP), color:IntroConst.COLOR},   //气:
							{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_3" ]+GameCommonData.Player.Role.SP+"/"+(GameCommonData.Player.Role.MaxSp + GameCommonData.Player.Role.AdditionAtt.MaxSP), color:IntroConst.COLOR},   //怒:
							];
				}
				else
				{
					if(GameCommonData.TargetAnimal.Role.Type == "NPC")
					{
						contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+"0/0", color:IntroConst.COLOR},    //血:
								{text:GameCommonData.wordDic[ "mod_too_med_ui_set_setb_2" ]+"0/0", color:IntroConst.COLOR},      //气:
								{text:GameCommonData.wordDic[ "mod_too_med_ui_set_setb_3" ]+"0/0", color:IntroConst.COLOR},      //怒:
								];
					}
					else if(GameCommonData.TargetAnimal.Role.Type == GameCommonData.wordDic[ "often_used_monster" ])       //怪物
					{
						 contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+GameCommonData.TargetAnimal.Role.HP+"/"+GameCommonData.TargetAnimal.Role.MaxHp, color:IntroConst.COLOR},    //血:
								{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_2" ]+GameCommonData.TargetAnimal.Role.MP+"/"+GameCommonData.TargetAnimal.Role.MaxMp, color:IntroConst.COLOR},     //气:
								{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_3" ]+GameCommonData.TargetAnimal.Role.SP+"/"+GameCommonData.TargetAnimal.Role.MaxSp, color:IntroConst.COLOR},     //怒:
								];
					}
					else if(GameCommonData.TargetAnimal.Role.Type == GameCommonData.wordDic[ "mod_too_med_ui_setb_set_4" ])        //玩家
					{
						 contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+GameCommonData.TargetAnimal.Role.HP+"/"+GameCommonData.TargetAnimal.Role.MaxHp, color:IntroConst.COLOR},     //血:
								{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_2" ]+GameCommonData.TargetAnimal.Role.MP+"/"+GameCommonData.TargetAnimal.Role.MaxMp, color:IntroConst.COLOR},      //气:
								{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_3" ]+GameCommonData.TargetAnimal.Role.SP+"/"+GameCommonData.TargetAnimal.Role.MaxSp, color:IntroConst.COLOR},      //怒:
								]
					}
				}
			}
			
			if(roleType == "Role")
			{
				if(!roleObj) return;
				contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+roleObj.HP+"/"+roleObj.MaxHp, color:IntroConst.COLOR},     //血:
							{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_2" ]+roleObj.MP+"/"+roleObj.MaxMp, color:IntroConst.COLOR}      //气:
							];
				if(GameCommonData.TeamPlayerList[roleObj.Id] && GameCommonData.TeamPlayerList[roleObj.Id]["state"]==1){       
					contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_5" ]+TeamDataProxy.teamDataDic[roleObj.Id],color:IntroConst.COLOR});    //位置：
				}else{
					contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_6" ],color:IntroConst.COLOR});           //位置：未知
				}			
			}
			
			if(roleType=="petRole"){
				if(GameCommonData.Player.Role.UsingPetAnimal==null)return;
				var role:GameRole=GameCommonData.Player.Role.UsingPetAnimal.Role;
				if(role==null)return;
				contents = [{text:GameCommonData.wordDic[ "mod_too_med_ui_setb_set_1" ]+role.HP+"/"+role.MaxHp, color:IntroConst.COLOR},    //血:
							{text:GameCommonData.wordDic[ "mod_too_med_ui_exp_set_1" ]+role.Exp+"/"+UIConstData.ExpDic[3000+role.Level],color:IntroConst.COLOR}];           //经验:  
			}
			showContent(contents);
			toolTip.addChild(content);
		}
		
		private function showContent(contents:Array):void
		{
			for(var i:int = 0; i<contents.length; i++)
			{
				var tf:TextField = new TextField();
//				tf.width = toolTip.width;
				tf.wordWrap = false;
				tf.x = 2;
				tf.y = content.height + 6;
				tf.filters = Font.Stroke(0);
				tf.textColor = contents[i].color; 
				tf.htmlText = contents[i].text;
				tf.autoSize = TextFieldAutoSize.LEFT;
				tf.setTextFormat(setFormat());
				content.addChild(tf);
				maxWidth=Math.max(tf.textWidth+10,maxWidth);
			}
		}
		
		private function setFormat():TextFormat 
		
		{
			var format:TextFormat = new TextFormat();
			format.font = "宋体";           //宋体
			format.align = TextFormatAlign.CENTER;
			return format;
		}
		
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:Number = 0;
			for(; i < toolTip.numChildren; i++)
			{
				var child:DisplayObject = toolTip.getChildAt(i);
				child.y = ypos;
				if(i>1)
				{
					toolTip.graphics.lineStyle(1, 0xffff22);
					toolTip.graphics.moveTo(0, child.y + 4);
					toolTip.graphics.lineTo(toolTip.width, ypos + 4);
				}
				ypos += child.height;
			}
			back.width = maxWidth;
			back.height = toolTip.height+4;
		}
		
		public function Remove():void
		{
			var count:int = toolTip.numChildren-1;
			while(count>=0)
			{
				toolTip.removeChildAt(count);
				count--;	
			}
			toolTip = null;
			back = null;
			content = null;
		}
		
		public function set IsSelf(v:Boolean):void
		{
			isSelf = v;
		}
		
		public function BackWidth():Number
		{
			return back.width;
		}
	}
}