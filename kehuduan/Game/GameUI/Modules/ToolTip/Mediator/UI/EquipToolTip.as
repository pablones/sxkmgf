package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Modules.ToolTip.Mediator.ToolTipUtils.ToolTipUtil;
	import GameUI.UIUtils;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class EquipToolTip implements IToolTip
	{
		/** 装备ToolTip显示容器 */
		private var toolTip:Sprite;
		/** 装备ToolTip背景*/
		private var back:MovieClip;
		/** 标题*/
		private var title:MovieClip;
		/** 信息显示MC*/
		private var info:MovieClip;
		private var star:MovieClip;
		private var stone:MovieClip;
		private var showEquipMc:MovieClip;
		private var content:Sprite;
		private var buidler:MovieClip;
		private var description:Sprite;
		private var lastTimeSp:Sprite;
		private var data:Object;
		
		/** 是否该装备已经加在角色身上了 */
		private var isEquiped:Boolean = false;
//		private var color:Array = ["#ffffff", "#ffffff", "#00ff00", "#0098FF", "#9727ff", "#FF6532"]; //["#ffffff", "#00ff00", "#0066FF", "#9727ff", "#ff9333"]
		private var qualities:Array = ["", GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_1" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_2" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_3" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_4" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_5" ]];         //优秀的       杰出的       卓越的       完美的        神圣的
		private var closeHandler:Function;
		private var isDrag:Boolean = false;
		private var showEquip:Boolean = false;
		
		//关闭按钮
		private var closeBtn:SimpleButton = null;
		
		//装备评分
		private var scoreMc:MovieClip;	//评分MC
		private var score:uint = 0;		//分数
		
		public function EquipToolTip(view:Sprite, data:Object, isEquip:Boolean = false, isDrag:Boolean = false, closeHandler:Function = null, showEquip:Boolean = false)
		{
			this.toolTip = view;
			this.data = data;
			this.isEquiped = isEquip;
			this.isDrag = isDrag;
			this.closeHandler = closeHandler;
			this.showEquip = showEquip;
			
//			this.data.isBind = 2;			//测试魂印
		}
		
		public function GetType():String
		{
			return this.data.type.toString();
		}
		
		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			toolTip.addChild(back);
			setTitle();
			setInfo();
			//setType();
			setStar();
			setStone();
			setScore();
			setContent();	
			setBuidler();
			setDescription();
			setLastTime();		//显示过期时间
			upDatePos();
		}
		
		public function Update(obj:Object):void
		{
			
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
//			title.txtTitle.defaultTextFormat = IntroConst.fontTf(17, TextFormatAlign.CENTER);
			title.name = "title"; 
//			title.txtType.defaultTextFormat = IntroConst.fontTf(18, TextFormatAlign.CENTER);
			
			title.txtTitle.filters = Font.Stroke(0x000000);
			title.txtTitle.multiline = false;
			title.txtTitle.wordWrap = false;
			var titleTxt:String = "";
			//名字颜色
			if(data.color && data.id != undefined) {
				titleTxt = '<font color="'+IntroConst.itemColors[data.color]+'" size="18">' + UIConstData.getItem(data.type).Name + '</font>';
			} else { 
				titleTxt = '<font color="'+IntroConst.itemColors[UIConstData.getItem(data.type).Color]+'" size="18">' + UIConstData.getItem(data.type).Name + '</font>';
			}
			var levelTxt:String = ""; 
			var typeTxt:String = "";
			if(data.id != undefined)
			{
				if(data.level != 0) levelTxt = '<font color="' + IntroConst.STENS_INCREMENT[data.level].color + '" size="18">+' + data.level + '</font>';		//#'+ IntroConst.COLOR.toString(16) +'
				typeTxt = '<font color="#'+ IntroConst.COLOR.toString(16) +'"  size="18">' + qualities[data.quality] + '</font>';	//if(data.quality != 0)
			}
			title.txtTitle.htmlText = typeTxt + titleTxt + levelTxt;
			title.mouseChildren = false;
			title.mouseEnabled = false;
			toolTip.addChild(title);		 	 
		}
				
		private function setInfo():void
		{
			info = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipInfo");
			var faceItem:FaceItem = new FaceItem(data.type, info);
			info.addChild(faceItem);
			faceItem.x = 20;
			faceItem.y = 16;
			info.txtItemLevel.filters = Font.Stroke(0x000000);
			info.txtUseLevel.filters = Font.Stroke(0x000000);
			info.txtJobFilter.filters = Font.Stroke(0x000000);
			info.txtMaleFilter.filters = Font.Stroke(0x000000);
			info.txtItemLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtUseLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtItemLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtMaleFilter.autoSize = TextFieldAutoSize.LEFT;
			if(UIConstData.getItem(data.type).Level == 0) {
				info.txtItemLevel.visible = false;
			} else {
				info.txtItemLevel.visible = true;
				info.txtItemLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_seti_1" ]+UIConstData.getItem(data.type).Level;           //等级需求：
			}
//			info.txtItemLevel.text = "等级需求："+UIConstData.ItemDic[data.type].Level;
			if(UIConstData.getItem(data.type).Level > GameCommonData.Player.Role.Level)
			{
				info.txtItemLevel.textColor = 0xff0000;
			}
			if(data.id == undefined)
			{
				info.txtUseLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_seti_2" ]+ int(UIConstData.getItem(data.type).UpperLimit/100) + "/" + int(UIConstData.getItem(data.type).UpperLimit/100);      //耐久：
			}
			else
			{
				info.txtUseLevel.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_seti_2" ]+ int(data.amount/100) + "/" + int(data.maxAmount/100);        //耐久：
			}
			if(data.type > 230000 || String(data.type).indexOf("20") == 0) {	//不显示耐久的物品
				info.txtUseLevel.text = "";
			}
//			if((String(data.type).indexOf("20") == 0) || (String(data.type).indexOf("23") == 0))
//			{
//				info.txtUseLevel.text = "";
//			}
			if(UIConstData.getItem(data.type).Job == 0) 
			{
				info.txtJobFilter.visible = false;	
				info.txtMaleFilter.visible = false;		//性别限制该为职业等级
			}
			else
			{
				/** 增加职业等级限制 2010.9.14*/
				if(UIConstData.getItem(data.type).Job != GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob-1].Job)
				{
					info.txtJobFilter.textColor = 0xff0000;
				} else {
					info.txtJobFilter.textColor = 0xe2cca5;
				}
				
				info.txtJobFilter.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_seti_3" ] + GameCommonData.RolesListDic[UIConstData.getItem(data.type).Job] + "";          //职业限制：
				info.txtJobFilter.visible = true;
				
				if(UIConstData.getItem(data.type).PlayerLevel == 0) {
					info.txtMaleFilter.visible = false;
				} else {
					if(UIConstData.getItem(data.type).PlayerLevel > GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob-1].Level) {
						info.txtMaleFilter.textColor = 0xff0000;
					} else {
						info.txtMaleFilter.textColor = 0xe2cca5;
					}
					var jobLevReq:int = UIConstData.getItem(data.type).PlayerLevel;
					info.txtMaleFilter.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_seti_4" ]+jobLevReq;                 //职业等级：
					info.txtMaleFilter.visible = true;
				}
			}
//			if(UIConstData.ItemDic[data.type].Sex == 0) 
//			{
//				info.txtMaleFilter.visible = false;
////				info.txtMaleFilter.visible = true;
////				info.txtMaleFilter.textColor = 0xe2cca5;
////				info.txtMaleFilter.text = "性别限制：不限";
//			}
//			else
//			{
//				if(UIConstData.ItemDic[data.type].Sex != GameCommonData.Player.Role.Sex + 1)
//				{
//					info.txtMaleFilter.textColor = 0xff0000;
//				} else {
//					info.txtMaleFilter.textColor = 0xe2cca5;
//				}
//				var male:String = UIConstData.ItemDic[data.type].Sex==1?"男":"女";
//				info.txtMaleFilter.text = "性别限制："+male;
//				info.txtMaleFilter.visible = true;
//			}
			info.mouseChildren = false;
			info.mouseEnabled = false;
			toolTip.addChild(info);
		}	
		
		private function setStar():void
		{
//			if(data.id == undefined) return;
			star = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcEquipStar");
//			trace("data.type = ", data.type);
		
			for(var i:int = 0; i<10; i++)
			{
				star["mcStar_"+i].stop();
				star["mcStar_"+i].gotoAndStop(2);
				if(data.id != undefined) {
					if(i<data.star)
					{
						star["mcStar_"+i].gotoAndStop(3);
					}
				}
				if(String(data.type).indexOf("4") == 0){
					star["mcStar_"+i].visible = false;
				}
			}
			star.txtItemType.filters = Font.Stroke(0x000000);
			star.txtItemType.autoSize = TextFieldAutoSize.LEFT;
			star.txtItemType.text = UIConstData.getItem(data.type).UseType;
			star.mouseChildren = false;
			star.mouseEnabled = false;
			toolTip.addChild(star);
		}
		
		/**
		 *  显示装备镶嵌的宝石信息(同时给data数据加上宝石附加属性值 )
		 * 
		 */		
		
		private function setStone():void
		{
			var bindStr:String = UIUtils.getBindShow(data);
			if(!data.id || data.id == undefined) {
				if(bindStr) {
					stone = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcStoneBind");
					stone["mcStone_0"].visible = false;
					stone["mcStone_1"].visible = false;
					stone["mcStone_2"].visible = false;
					if(EquipDataConst.isFourthStilettoOpen)
					{
						stone["mcStone_3"].visible = false;
					}
					stone.txtIsBind.htmlText = bindStr;
					stone.txtIsBind.filters = Font.Stroke(0x000000);
					stone.mouseChildren = false;
					stone.mouseEnabled = false;
					
					toolTip.addChild(stone);
				}
				return;
			}
			stone = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcStoneBind");
			
			stone["mcStone_0"].visible = false;
			stone["mcStone_1"].visible = false;
			stone["mcStone_2"].visible = false;
			if(EquipDataConst.isFourthStilettoOpen)
			{
				stone["mcStone_3"].visible = false;
			}
			var count:int = 0;
			stone.txtIsBind.htmlText = UIUtils.getBindShow(data);
			
//			if(data.isBind)
//			{
//				stone.txtIsBind.text = "已绑定";
//			}
//			else
//			{
//				stone.txtIsBind.text = " ";
//			} 
			if(data.stoneList && data.stoneList.length > 0) {
				var stoneBaseList:Array=[];
				for(var j:int = 0; j<data.stoneList.length; j++)
				{
					
					if(data.stoneList[j] == 88888)
					{
						stone["mcStone_"+j].visible = true;
						
					}else if(data.stoneList[j] == 99999)
					{
						stone["mcStone_"+j].visible = false;
						count++;
					}else if(data.stoneList[j]==0){
						count++;
						continue;
					}else{
						var s:StoneImg=new StoneImg(String(data.stoneList[j]),"Resources/icon/",0.4);
						stone["mcStone_"+j].visible = true;
						stone["mcStone_"+j].addChild(s);
						s.x=s.y=2;
						var stoneItem:Object=UIConstData.getItem(data.stoneList[j]);
						if(stoneItem!=null){
							stoneBaseList.push({stoneAtt:stoneItem.BaseList[0],stoneName:stoneItem.Name});
						}
					}	
				} 
				data["stoneBaseList"]=stoneBaseList;
				if(count == data.stoneList.length && !bindStr) return;	// && !data.isBind
			} 
			stone.txtIsBind.filters = Font.Stroke(0x000000);
			stone.txtIsBind.autoSize = TextFieldAutoSize.RIGHT;			
			stone.mouseChildren = false;
			stone.mouseEnabled = false;
			
			toolTip.addChild(stone);
		}
		
		public function get getScore():uint
		{
			return this.score;
		}
		
		private function setScore():void
		{
			if(!data.id || data.id == undefined) return;
			var type:uint = data.type;
			if(type < 300000 && int(type/10000) != 20) { //装备（非坐骑）
				score = ToolTipUtil.getEquipScore(data);
				if(score > 0) {
					scoreMc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipScore");
					scoreMc.mouseEnabled = false;
					scoreMc.mouseChildren = false;
					scoreMc.txtScore.htmlText = "<font color='#00CCCC'>"+GameCommonData.wordDic[ "mod_too_med_ui_equ_sets_1" ]+score+"</font>";         //装备评价：
					toolTip.addChild(scoreMc);
				}
			}
		}
		
		public function compareScore(scoreArr:Array):void
		{
			var scoreStr:String = "<font color='#00CCCC'>"+GameCommonData.wordDic[ "mod_too_med_ui_equ_sets_1" ]+score+"</font>";                //装备评价：
			var tmpScore:uint = 0;
			var tmpStr:String = "";
//			for(var i:int = 0; i < scoreArr.length ; i++) {
			for(var i:int = scoreArr.length-1; i >= 0 ; i--) {
				tmpScore = scoreArr[i];
				tmpStr = (score < tmpScore) ? "<font color='#ff0000'> ↓"+(tmpScore-score)+"</font>" : "<font color='#00ff00'> ↑"+(score-tmpScore)+"</font>";
				scoreStr += tmpStr;
			}
			if(scoreMc) {
				scoreMc.txtScore.htmlText = scoreStr;
			}
		}
		
		private function setContent():void
		{
			content = new Sprite();
			var contents:Array = [];
			if(!isEquiped)
			{
				contents = ToolTipUtil.GetNoEquiedContent(data);
			}
			else
			{
				contents = ToolTipUtil.GetIsEquiedContent(data);
			}
//			var hideProp:uint = (UIConstData.ItemDic[data.type].ADes && UIConstData.ItemDic[data.type].ADes > 0) ? UIConstData.ItemDic[data.type].ADes : 0;
//			if(hideProp > 0) {		//附加高级属性   10.9.2010
//				var levNeed:uint  = hideProp % 100 / 10;
//				var starNeed:uint = hideProp % 10;
//				if(data.level >= levNeed && data.star >= starNeed) {
//					contents.push({text:"<font color='#00ff00'>" + UIConstData.ItemDic[data.type].SDes + "</font>"});
//				} else {
//					var desStr:String = "";
//					var endStr:String = "时激活此属性。";
//					if(levNeed > 0) desStr += "强化到+"+levNeed;
//					if(starNeed > 0) desStr += "升星到+"+starNeed;
//					desStr += endStr;
//					contents.push({text:"<font color='#7a7a7a'>"+desStr+"</font>"});
//				}
//			}
			if(contents.length == 0 || int(data.type/10000) == 23) return;
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameTow");
			showContent(contents);
//			trace("content.numChildren = ", content.numChildren);
//			trace("content.getChildAt(0) = ", content.getChildAt(0));
			if(content.numChildren == 1)  
			{
				content.getChildAt(0).y = 6;	
			}
			content.addChildAt(frame, 0);
			frame.height = content.height + 2;
			content.mouseChildren = false;
			content.mouseEnabled = false;
			toolTip.addChild(content);
		}
		
		private function showContent(contents:Array):void
		{
			for(var i:int = 0; i<contents.length; i++)
			{
				var tf:TextField = new TextField();
				tf.defaultTextFormat = new TextFormat("宋体", 12);         //宋体
				tf.width = toolTip.width;
				tf.wordWrap = true;
				tf.x = 8;
				tf.y = content.height + 4;
				if(i>=0)
				{
					tf.y -= 2;
				}
				tf.filters = Font.Stroke(0);
//				tf.textColor = 0xffffff;
				tf.htmlText = contents[i].text;
				tf.autoSize = TextFieldAutoSize.LEFT;
//				trace("htmlText: ", contents[i].text); 
				content.addChild(tf);
			}
			
		}
		
		private function setBuidler():void
		{			
			if(data.id == undefined || data.builder == GameCommonData.wordDic[ "often_used_none" ] || data.builder=="0" || data.builder=="拸" ) return;           //无 
			buidler = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcBuidler");
			buidler.txtBuilder.filters = Font.Stroke(0x000000);
			buidler.txtBuilder.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_setb_1" ] + data.builder;               //制造人：
			buidler.txtBuilder.mouseEnabled = false;
			buidler.mouseEnabled  = false;
			toolTip.addChild(buidler);
		}
		
		private var noDescription:Boolean = false;
		
		private function setDescription():void
		{
			if(UIConstData.getItem(data.type).description == "")
			{
				noDescription = true;
				return;
			} 
			description = new Sprite();
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameOne");
			var tf:TextField = new TextField();
			tf.textColor = IntroConst.COLOR;
			tf.filters = Font.Stroke(0x000000);
			tf.autoSize = TextFieldAutoSize.LEFT;
			tf.mouseEnabled = false;
			tf.htmlText = UIConstData.getItem(data.type).description;
			tf.y = 3;
			tf.x = 5;
			description.addChild(tf);
			description.addChildAt(frame, 0);
			frame.height = description.height + 3;
			description.mouseEnabled = false;
			toolTip.addChild(description);
		}	
		
		/** 显示过期时间 */
		private function setLastTime():void
		{
			if(data.isActive == 0) {							//0-不需要检测剩余时间
				return;
			} else if(data.isActive == 1) {						//1-需检测剩余时间
				var lastTime:String = data.lifeTime.toString();
				var year:String   = "20" + lastTime.substr(0, 2);
				var month:String  = lastTime.substr(2, 2);
				var day:String    = lastTime.substr(4, 2);
				var hour:String	  = lastTime.substr(6, 2);
				var minite:String = lastTime.substr(8, 2);
				if(month.charAt(0) == "0")  month = month.substr(1, 1);
				if(day.charAt(0) == "0") 	day = day.substr(1, 1);
				if(hour.charAt(0) == "0")   hour = hour.substr(1, 1);
				if(minite.charAt(0) == "0") minite = minite.substr(1, 1);
				
				var timeDes:String = GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_1" ]+year+GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_2" ]+month+GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_3" ]+day+GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_4" ]+hour+GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_5" ]+minite+GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_6" ];       //过期时间：     年     月     日      时      分
				
				lastTimeSp = new Sprite();
				
				var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameOne");
				var tf:TextField = new TextField();
				tf.defaultTextFormat = new TextFormat("宋体", 12);            //宋体
				tf.textColor = IntroConst.TIMER_COLOR_ARR[2];
				tf.filters = Font.Stroke(0x000000);
				tf.autoSize = TextFieldAutoSize.LEFT;
				tf.mouseEnabled = false;
				tf.text = timeDes;
				tf.y = 3;
				tf.x = 5;
				lastTimeSp.addChild(tf);
				lastTimeSp.addChildAt(frame, 0);
				frame.height = lastTimeSp.height;
				lastTimeSp.mouseEnabled = false;
				toolTip.addChild(lastTimeSp);
			} else if(data.isActive == 2) {											//2-已过期的
				lastTimeSp = new Sprite();
				
				var frame1:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameOne");
				var tf1:TextField = new TextField();
				tf1.textColor = IntroConst.TIMER_COLOR_ARR[3];
				tf1.filters = Font.Stroke(0x000000);
				tf1.autoSize = TextFieldAutoSize.LEFT;
				tf1.mouseEnabled = false;
				tf1.text = GameCommonData.wordDic[ "mod_too_med_ui_equ_setl_7" ];               //该物品已过期
				tf1.y = 3;
				tf1.x = 5;
				lastTimeSp.addChild(tf1);
				lastTimeSp.addChildAt(frame1, 0);
				frame1.height = lastTimeSp.height;
				lastTimeSp.mouseEnabled = false;
				toolTip.addChild(lastTimeSp);
			}
		}
		
		private function upDatePos():void
		{
			var i:int = 1;
			var ypos:int = 0;
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
			if(noDescription)
			{
				back.height = int(toolTip.height + 3);
			}
			else
			{
				back.height = int(toolTip.height + 2);
			}
			if(isDrag)
			{
				this.toolTip.mouseEnabled = true
				this.toolTip.mouseChildren = true;;			
				back.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				closeBtn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("CloseBtn");
				closeBtn.addEventListener(MouseEvent.CLICK, onClose);
				closeBtn.x = this.back.width - closeBtn.width;
				closeBtn.y = 0;
				this.toolTip.addChild(closeBtn);
			}
			if(this.showEquip)
			{
				showEquipMc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcHasEquiped");
				this.toolTip.addChild(showEquipMc);
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
				var child:DisplayObjectContainer = toolTip.getChildAt(count) as DisplayObjectContainer;
				var count2:int = child.numChildren - 1;
				toolTip.removeChildAt(count);
				while(count2>=0)
				{
					var child2:DisplayObject = child.getChildAt(count2);
					child.removeChild(child2);
					child2 = null;
					count2--;
				}
				count--;	
			}
			toolTip = null;
			back = null;
			title = null;
			info = null;
			star = null;
			stone = null;
			content = null;
			buidler = null;
			description = null;
			showEquipMc = null;
			data = null;
		}
		
		public function BackWidth():Number
		{
			return back.width;
		}
		//____________________________________________________________________________
		
		/* public function set Type(v:String):void
		{
			
		}
		
		public function get Type():String
		{
			
		} */
	}
}