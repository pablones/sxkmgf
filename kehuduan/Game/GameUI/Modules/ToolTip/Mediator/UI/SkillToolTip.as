package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Skill.GameSkill;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class SkillToolTip implements IToolTip
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
		
		public function SkillToolTip(view:Sprite, data:Object, isLearn:Boolean)
		{
			this.toolTip = view;
			this.data = data;
			this.isLearn = isLearn;
		}

		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			toolTip.addChild(back);
			setTitle();
			setInfo();
			setContent();
			setDescription();	
//			setIntro();
			upDatePos();
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
//			title.txtTitle.defaultTextFormat = IntroConst.fontTf(17, TextFormatAlign.CENTER);
			title.txtTitle.filters = Font.Stroke(0x000000);
			if(this.isLearn)
			{
				title.txtTitle.text = data.gameSkill.SkillName;
			}
			else
			{
				title.txtTitle.text = data.SkillName;				
			}
			title.txtTitle.autoSize = TextFieldAutoSize.CENTER;
			toolTip.addChild(title);
		}
		
		private function setInfo():void
		{
			info = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcSkillInfo");
			var id:String = null;
			if(this.isLearn)
			{
				id = data.gameSkill.SkillID.toString();
			}
			else
			{
				id = data.SkillID.toString();
			}
			var faceItem:FaceItem = new FaceItem(id, info, "Icon");
			info.addChild(faceItem);
			faceItem.x = 21;
			faceItem.y = 10;
			info.txtSkillLevel.filters = Font.Stroke(0x000000);
			info.txtSkillType.filters = Font.Stroke(0x000000);
			info.txtSkillLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtSkillType.autoSize = TextFieldAutoSize.LEFT;
			if(this.isLearn)
			{
				info.txtSkillLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_1" ] + data.Level;       //技能等级：
				info.txtSkillType.text = (data.gameSkill.SkillMode == 0?GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_2" ]:GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_3" ]);       //被动技能         主动技能
			}
			else
			{
				if(data.SkillID == 9508 || data.SkillID == 9509)
				{
					info.txtSkillLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_4" ];         //技能等级：1
				}
				else 
				{
					info.txtSkillLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_for_seti_1" ];            //未学会
				}
				
				if ( data.SkillID == 9510 )				//隐藏技能
				{
					info.txtSkillType.text =  "";
				}
				else
				{
					info.txtSkillType.text =  (data.SkillMode == 0?GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_2" ]:GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_3" ]);	       //被动技能           主动技能
				}
			}			
			toolTip.addChild(info);
		}
		
		private function setContent():void
		{
			if(this.isLearn)
			{
				if(data.gameSkill.SkillMode == 0) return;
			}
			content = new Sprite();
			var contents:Array = new Array();
			if(this.isLearn)
			{
					if(data.GetMP != 0) { contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_1" ]+int(data.GetMP * GameCommonData.Player.Role.MPPercentage /100) , color:IntroConst.COLOR} ) }       //消耗气：
					if(data.gameSkill.SP != 0) { contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_2" ]+data.gameSkill.SP, color:IntroConst.COLOR} ) }        //消耗怒：
					if( data.gameSkill.Distance && (data.gameSkill.Distance != 0) && (data.gameSkill.SkillMode != 22) && (data.gameSkill.Job != -8) ) { contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_3" ]+data.gameSkill.Distance, color:IntroConst.COLOR} ) }  //有效距离：
					if(data.GetCoolTime != 0) 
					{
						if(data.GetCoolTime < 500)
						{
							contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_4" ]+(data.GetCoolTime)+GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_6" ], color:IntroConst.COLOR} );          //冷却时间：         秒
						}else
						{
							contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_4" ]+(data.GetCoolTime/60)+GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_7" ], color:IntroConst.COLOR} );          //冷却时间：       分钟
						}
					}
					if ( data.gameSkill.SkillID>6000 && data.gameSkill.SkillID<7000 )
					{
//						contents.push( {text:"熟练度："+data.Familiar+"/"+data.Level*1000, color:IntroConst.COLOR} );
						if ( data.gameSkill.SkillID >= 6004 && data.gameSkill.SkillID <= 6006 )
						{
							contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_5" ]+data.Familiar+"/"+ManufactoryData.aLifeSkillFam[ data.Level-1 ], color:IntroConst.COLOR } );	        //熟练度：
						}
						else
						{
							contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_5" ]+data.Familiar+"/"+Math.pow( 2,data.Level-1 )*200, color:IntroConst.COLOR} );	           //熟练度：
						}
					}
			}
			else
			{
				if(data.MP != 0) { contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_1" ]+data.MP, color:IntroConst.COLOR} ) }               //消耗气：
				if(data.SP != 0) { contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_2" ]+data.SP, color:IntroConst.COLOR} ) }               //消耗怒：
				if( data.Distance && (data.Distance != 0) && (data.SkillMode != 22) && (data.Job != -8) ) { contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_3" ]+data.Distance, color:IntroConst.COLOR} ) }   //有效距离：
				if(data.CoolTime != 0)
				{
					if(data.CoolTime < 500)
					{
						contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_4" ]+(data.CoolTime)+GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_6" ], color:IntroConst.COLOR} );           //冷却时间：         秒
					}else
					{
						contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_4" ]+(data.CoolTime/60)+GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_7" ], color:IntroConst.COLOR} );          //冷却时间：       分钟
					}
				}
				if ( data.SkillID>6000 && data.SkillID<7000 )
				{
					contents.push( {text:GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_5" ]+0+"/0", color:IntroConst.COLOR} );          //熟练度：
				}
			}
			if(contents.length == 0) return;
			showContent(contents);
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameOne");
			content.addChildAt(frame, 0);
			frame.height = content.height;
			frame.visible = false;
			toolTip.addChild(content);
		}
		
		private function showContent(contents:Array):void
		{
			var TF:TextFormat = new TextFormat();
				TF.leading = 3;
			for(var i:int = 0; i<contents.length; i++)
			{
				var tf:TextField = new TextField();
				tf.defaultTextFormat = new TextFormat("宋体", 12);            //宋体
				tf.width = toolTip.width;
				tf.wordWrap = true;
				tf.x = 4;
				tf.y = content.height+2;
				tf.filters = Font.Stroke(0);
				tf.htmlText = contents[i].text;
				tf.autoSize = TextFieldAutoSize.LEFT;
				tf.textColor = contents[i].color;
				tf.setTextFormat( TF );
				content.addChild(tf);
			}
		}
		
		private function setDescription():void
		{
			description = new Sprite();
			var tf:TextField = new TextField();
			tf.textColor = IntroConst.COLOR;
			tf.filters = Font.Stroke(0x000000);
			tf.autoSize = TextFieldAutoSize.LEFT;
//			tf.width = this.back.width + 10;
			tf.width = this.back.width-11;
			tf.wordWrap = true;
			tf.mouseEnabled = false;
			var info:String = "";
			var tempData:Object;
			if(data is GameSkillLevel)
			{
				tempData = data.gameSkill;
			}
			else if(data is GameSkill)
			{
				tempData = data;
			}
			var boo:Boolean;
			var remarkStr:String = tempData.SkillReamark;
			if(remarkStr.indexOf("N") >= 0)
			{
				boo = true;
			}
			if(this.isLearn)
			{
				if(boo)
				{
					info = remarkStr.replace("N",Math.round((tempData.Attack+ (tempData.LevelAttack*(data.Level-1)))*10)/10);
				}
				else
				{
					info = remarkStr;
				}
			} 
			else
			{
				if(boo)
				{
					info = remarkStr.replace("N",Math.round(tempData.Attack*10)/10);
				}
				else
				{
					info = remarkStr;
				}
			}
			if(data is GameSkillLevel)
			{
				 var skillId:int = (data as GameSkillLevel).gameSkill.SkillID;
				if(8501 <= skillId && skillId <= 8520)	//宠物技能
				{
					var pet:GamePetRole = PetPropConstData.newCurrentPet;
					pet = pet ? pet:PetRuleCommonData.selectedPet;
					var dataProxy:DataProxy = GameCommonData.UIFacadeIntance.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(dataProxy.petRuleIsOpen) pet = PetRuleCommonData.selectedPet ? PetRuleCommonData.selectedPet:pet;
					if(pet)
					{
						info = (data as GameSkillLevel).PetNewReamark(PetPropConstData.getAddExtendValue(pet));
					}
				}
			}
				
			/* if(this.isLearn)
			{
				var info:String = data.gameSkill.SkillReamark.replace("N",Math.round((data.gameSkill.Attack+ (data.gameSkill.LevelAttack*(data.Level-1)))*10)/10);
//				tf.text = data.gameSkill.SkillReamark;
				tf.htmlText = info;
				//tf.text = "GameCommonData.SkillList里面的SkillReamark为空";
			}
			else
			{
				var info2:String = data.SkillReamark.replace("N",Math.round(data.Attack*10)/10);
//				trace("data.SkillReamark = ", data.SkillReamark);
//				tf.text = data.SkillReamark;
				tf.htmlText = info2;
			} */
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
		
		private function setIntro():void
		{
			useIntro = new Sprite();
			var tf:TextField = new TextField();
			tf.textColor = IntroConst.COLOR;
			tf.filters = Font.Stroke(0x000000);
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.mouseEnabled = false;
			tf.text = GameCommonData.wordDic[ "mod_too_med_ui_ski_seti_1" ];            //拖至快捷栏中使用
			tf.y = 2;
			tf.x = 4;
			useIntro.addChild(tf);
			useIntro.mouseEnabled = false;
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameOne");
			useIntro.addChildAt(frame, 0);
			frame.height = content.height;
			toolTip.addChild(useIntro);
		}
				
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:Number = 0;
			for(; i < toolTip.numChildren; i++)
			{
				/* var child:DisplayObject = toolTip.getChildAt(i);
				child.x = 3;
				child.y = int(ypos+3);
				if(i>1)
				{
					if(i == toolTip.numChildren-1)
					{
						ypos += child.height;			
					}
					else
					{
						ypos += child.height+1;	
					}
				}
				else
				{
					ypos += child.height;	
				} */
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