package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.View.ManufactoryResource;
	import GameUI.Modules.Manufactory.View.ManufatoryView;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class FormulaToolTip implements IToolTip
	{
		private var toolTip:Sprite;
		
		private var back:MovieClip;
		
		private var title:MovieClip;
		private var info:MovieClip;
		private var type:MovieClip;
		private var content:Sprite;
		private var data:Object;
		
		private var closeHandler:Function;
		private var isDrag:Boolean = false;
		
		//关闭按钮
		private var closeBtn:SimpleButton = null;
		
		public function FormulaToolTip(view:Sprite, data:Object, isDrag:Boolean = false, closeHandler:Function = null)
		{
			this.toolTip = view;
			this.data = data;
			this.isDrag = isDrag;
			this.closeHandler = closeHandler;	
		}
		
		public function Show():void
		{
			if ( ManufactoryData.ResourseIsLoaded )
			{
				display();
			} 
			else
			{
				var mRes:ManufactoryResource = new ManufactoryResource();
				mRes.LoadDataComplete = display;
			}
		}
		
		private function display():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			if ( !toolTip ) return;
			toolTip.addChild(back);
			if(!UIConstData.getItem(data.type)) { 
				var t:TextField = new TextField();
				t.width = back.width;
				t.textColor = 0xff0000;
				t.text = GameCommonData.wordDic[ "mod_too_med_ui_bus_sho_1" ];           //暂无该物品说明
				t.x = 8;
				t.y = 8;
				back.addChild(t);
				return;
			}
			setTitle();
			setInfo();
			setType();
			setContent();
			upDatePos();
		}
		
		public function GetType():String
		{
			return "FormulaToolTip";
		}
		
		public function Update(obj:Object):void
		{
			
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
			title.txtTitle.filters = Font.Stroke(0x000000);
			//////////读取物品颜色
			var color:uint = UIConstData.getItem(data.type).Color;
			title.txtTitle.htmlText = '<font size="18" color="' + IntroConst.itemColors[color] + '">' + UIConstData.getItem(data.type).Name + '</font>';
			///////////
			title.txtTitle.autoSize = TextFieldAutoSize.CENTER;
			toolTip.addChild(title);
			title.mouseChildren = false;
			title.mouseEnabled = false;
		}
		
		private function setInfo():void
		{
			info = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipInfo");
			var faceItem:FaceItem = new FaceItem(data.type, info);
			info.addChild(faceItem);
			faceItem.x = 20;
			faceItem.y = 16;
			info.txtItemLevel.filters = Font.Stroke(0x000000);
			info.txtItemLevel.autoSize = TextFieldAutoSize.LEFT;
			
//			info.txtUseLevel.visible = false;
			info.txtJobFilter.visible = false;
			info.txtMaleFilter.visible = false;

			checkKind();
			info.txtUseLevel.htmlText = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_too_med_ui_for_seti_1" ]+"</font>";        //未学会
			
			for ( var i:uint=0; i<ManufactoryData.scenographyList.length; i++ )
			{
				var fId:uint = ManufactoryData.scenographyList[i];
				if ( !ManufactoryData.allInfoDic[fId] ) continue; 
//				trace ( "  sdfads " + ManufactoryData.scenographyList );
				if ( ManufactoryData.allInfoDic[fId].formula == data.type )
				{
					info.txtUseLevel.htmlText = GameCommonData.wordDic[ "mod_too_med_ui_for_seti_2" ];         //已学会
					break;
				}
			}
			
			info.mouseChildren = false;
			info.mouseEnabled = false;
			toolTip.addChild(info);
		}
		
		private function setType():void
		{
			type = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipType");
			type.txtItemType.filters = Font.Stroke(0x000000);
			type.txtItemType.autoSize = TextFieldAutoSize.LEFT;
			type.txtItemType.text = GameCommonData.wordDic[ "mod_too_med_ui_for_sett_1" ];            //配方
		
			type.txtIsBind.text = UIUtils.getBindShow(data);
			
			type.mouseEnabled = false;
			type.mouseChildren = false;
			toolTip.addChild(type);
		}
		
		private function setContent():void
		{
			content = new Sprite();
			var contents:Array = [	{text:UIConstData.getItem(data.type).description, color:IntroConst.COLOR}
									];
			if(String(data.type).indexOf("3") == 0 || String(data.type).indexOf("5") == 0) {	//药品 和其他 可使用物品
				contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_for_setc_1" ], color:0xffff00});           //双击或选中后点击"使用"按钮使用
			}

			showContent(contents);
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameOne");
			content.addChildAt(frame, 0);
			frame.height = content.height + 3;
			content.mouseEnabled = false;
			content.mouseChildren = false;
			toolTip.addChild(content);
		}
		
		private function showContent(contents:Array):void
		{
			for(var i:int = 0; i<contents.length; i++)
			{
				var TF:TextFormat = new TextFormat();
					TF.leading = 3;
				var tf:TextField = new TextField();
				tf.defaultTextFormat = new TextFormat("宋体", 12);             //宋体
				tf.width = toolTip.width;
				tf.wordWrap = true;
				tf.x = 8;
				if(i == 0) {
					tf.y = 4;	//+ 4;  
				} else {
					tf.y = content.height + 4;	//+ 4;  
				}
				tf.filters = Font.Stroke(0);
				tf.textColor = contents[i].color;
				tf.htmlText = contents[i].text;
				tf.autoSize = TextFieldAutoSize.LEFT;
				tf.setTextFormat( TF );
				content.addChild(tf);
			}
		}
		
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:Number = 0;
			for(; i < toolTip.numChildren; i++)
			{
				var child:DisplayObject = toolTip.getChildAt(i);
				child.x = 3;
				child.y = int(ypos+3);
				if(i>3)
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
				}
			}
			back.width = 216;
			back.height = int(toolTip.height + 2);
			if(isDrag)
			{
				this.toolTip.mouseEnabled = true;
				this.toolTip.mouseChildren = true;			
				back.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				closeBtn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("CloseBtn");
				closeBtn.addEventListener(MouseEvent.CLICK, onClose);
				closeBtn.x = this.back.width - closeBtn.width;
				closeBtn.y = 0;
				this.toolTip.addChild(closeBtn);
			}
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
			this.toolTip.startDrag();	
			back.addEventListener(MouseEvent.MOUSE_UP, onMouseUp);
		}
		
		private function onMouseUp(event:MouseEvent):void
		{
			this.toolTip.stopDrag();
		}
		
		private function onClose(event:MouseEvent):void
		{
			closeHandler();
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
			type = null;
			content = null;
			data = null;
		}
		
		public function BackWidth():Number
		{
			return back.width;
		}
		
		//等级提示
		private function checkKind():void
		{
			var ty:uint = uint( data.type );
			var needLevel:uint = uint( UIConstData.getItem(data.type).Level );
			if ( ty>521000 && ty<521999 )					//需要锻造技能
			{
				info.txtItemLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_for_che_1" ]+needLevel;            //锻造等级：
				checkColor(6004,needLevel);
			}
			else if ( ty>522000 && ty<522999 )
			{
				info.txtItemLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_for_che_2" ]+needLevel;            //制皮等级：
				checkColor(6005,needLevel);
			}
			else if ( ty>523000 && ty<523999 )
			{
				info.txtItemLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_for_che_3" ]+needLevel;             //精工等级：
				checkColor(6006,needLevel);
			}
			
		}
		
		private function checkColor(ty:uint,needLevel:uint):void
		{
			if ( !GameCommonData.Player.Role.LifeSkillList[ty] )
			{
				info.txtItemLevel.textColor = 0xff0000;
			}
			else
			{
				if ( (GameCommonData.Player.Role.LifeSkillList[ty] as GameSkillLevel).Level<needLevel )
				{
					info.txtItemLevel.textColor = 0xff0000;
				}
			}
		}
		
	}
}