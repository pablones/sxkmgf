package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class SoulSkillToolTip implements IToolTip
	{
		private var toolTip:Sprite;
		private var back:MovieClip;
		private var title:MovieClip;
		private var info:MovieClip;
		private var content:Sprite;
		private var description:Sprite;
		private var useIntro:Sprite;
		private var data:Object;
		private var isLearn:Boolean;
		
		private var soulSkillVo:SoulSkillVO;
		
		public function SoulSkillToolTip( view:Sprite, data:Object )
		{
			this.toolTip = view;
			this.soulSkillVo = data as SoulSkillVO;
			this.isLearn = isLearn;
		}

		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			toolTip.addChild(back);
			setTitle();
			setInfo();
			setDescription();	
			upDatePos();
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
			title.txtTitle.filters = Font.Stroke(0x000000);

			title.txtTitle.text = this.soulSkillVo.name;
			title.txtTitle.autoSize = TextFieldAutoSize.CENTER;
			toolTip.addChild(title);
		}
		
		private function setInfo():void
		{
			info = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcSkillInfo");
			var id:String = this.soulSkillVo.gameskill.SkillID.toString();
		
			var faceItem:FaceItem = new FaceItem(id, info, "Icon");
			info.addChild(faceItem);
			faceItem.x = 21;
			faceItem.y = 10;
			info.txtSkillLevel.filters = Font.Stroke(0x000000);
			info.txtSkillType.filters = Font.Stroke(0x000000);
			info.txtSkillLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtSkillType.autoSize = TextFieldAutoSize.LEFT;
			
			info.txtSkillLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_1" ] + soulSkillVo.level;       //技能等级：
			info.txtSkillType.text = GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_2" ];       //被动技能   全是被动	
			toolTip.addChild(info);
		}
		
		private function setDescription():void
		{
			description = new Sprite();
			var tf:TextField = new TextField();
			tf.textColor = IntroConst.COLOR;
			tf.filters = Font.Stroke(0x000000);
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.width = this.back.width-11;
			tf.wordWrap = true;
			tf.mouseEnabled = false;
			
			var info:String = this.soulSkillVo.gameskill.SkillReamark.replace( "N",this.soulSkillVo.num  );
			tf.htmlText = info;
			
			tf.y = 4;
			tf.x = 7;
			description.addChild(tf);
			description.mouseEnabled = false;
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameTow");
			description.addChildAt(frame, 0);
			frame.height = description.height;
			toolTip.addChild(description);
		}
				
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:Number = 0;
			for(; i < toolTip.numChildren; i++)
			{
				var child:DisplayObject = toolTip.getChildAt(i);
				child.x = 3;
				if(i == 1 || i == 2)
				{
					child.y = int(ypos+3);
				}
				else
				{
					child.y = int(ypos+1);
				}
				ypos += child.height;
			}
			back.width = 216;
			if(toolTip.numChildren == 4)
			{
				back.height = toolTip.height+2;
			}
			else
			{
				back.height = toolTip.height+3;
			}
		}
	
		public function Update(obj:Object):void
		{
			
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
			title = null;
			info = null;
			content = null;
			description = null;
			useIntro = null;
			data = null;
		}
		
		public function GetType():String
		{
			return null;
		}
		
		public function BackWidth():Number
		{
			return back.width;
		}
	}
}