package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.ToolTip.Const.IntroConst;
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
	
	public class SoulTooltip implements IToolTip
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
		
//		private var color:Array = ["#ffffff", "#ffffff", "#00ff00", "#0098FF", "#9727ff", "#FF6532"]; //["#ffffff", "#00ff00", "#0066FF", "#9727ff", "#ff9333"]
		private var qualities:Array = ["", GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_1" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_2" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_3" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_4" ], GameCommonData.wordDic[ "mod_too_med_ui_equ_qua_5" ]];         //优秀的       杰出的       卓越的       完美的        神圣的
		private var closeHandler:Function;
		private var isDrag:Boolean = false;
		private var showEquip:Boolean = false;
		
		private var soulVo:SoulVO;
		
		//关闭按钮
		private var closeBtn:SimpleButton = null;
		
		//装备评分
		private var scoreMc:MovieClip;	//评分MC
		private var score:uint = 0;		//分数
		
		public function SoulTooltip(view:Sprite, data:Object, isEquip:Boolean = false, isDrag:Boolean = false, closeHandler:Function = null, showEquip:Boolean = false)
		{
			this.toolTip = view;
			this.data = data;
			
			if ( !data.id ) return;
			if ( !SoulData.SoulDetailInfos[ data.id ] ) return;
			this.soulVo = SoulData.SoulDetailInfos[ data.id ];
			
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
			if ( !this.soulVo ) return;
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			toolTip.addChild(back);
			setTitle();
			setInfo();
			setStar();
			setStone();
//			setScore();				//不需要评分
			setContent();	
			setBuidler();				//设置属相
			setDescription();
			setLastTime();		//显示过期时间
			upDatePos();
		}
		
		//接口方法
		public function Update(obj:Object):void
		{
			
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
			title.name = "title"; 
			
			title.txtTitle.filters = Font.Stroke(0x000000);
			title.txtTitle.multiline = false;
			title.txtTitle.wordWrap = false;
			var titleTxt:String = "";
			
			var titleName:String = "";
			
			if ( this.soulVo.belong == 1 )
			{
				titleName = GameCommonData.wordDic[ "mod_bag_pro_gridMa_onMouseDow_1" ];//"九阳之魄";
			}
			else
			{
				titleName = GameCommonData.wordDic[ "mod_bag_pro_gridMa_onMouseDow_2" ];//"九阴之魂";
			}
			//名字颜色
			if(data.color && data.id != undefined) {
//				titleTxt = '<font color="'+IntroConst.itemColors[data.color]+'" size="18">' + UIConstData.getItem(data.type).Name + '</font>';
				titleTxt = '<font color="'+IntroConst.itemColors[data.color]+'" size="18">' + titleName + '</font>';
			} else { 
//				titleTxt = '<font color="'+IntroConst.itemColors[UIConstData.getItem(data.type).Color]+'" size="18">' + UIConstData.getItem(data.type).Name + '</font>';
				titleTxt = '<font color="'+IntroConst.itemColors[UIConstData.getItem(data.type).Color]+'" size="18">' + titleName + '</font>';
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
				info.txtItemLevel.text = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_setInfo_1" ]+"："+UIConstData.getItem(data.type).Level;           //携带等级
			}

			if ( !this.soulVo )  return;

			info.txtUseLevel.text = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_setInfo_2" ]+"："+this.soulVo.level;//魂魄等级
			
			if ( soulVo.belong == 1 )
			{
				info.txtJobFilter.htmlText = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_soul_med_soulM_setV_2" ]+"</font>";//力量型
			}
			else if ( soulVo.belong == 2 )
			{
				info.txtJobFilter.htmlText = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_soul_med_soulM_setV_1" ]+"</font>";//灵力型
			}
			
			info.txtMaleFilter.text = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_setInfo_3" ]+"：" + soulVo.life + "/300";//寿命

			info.mouseChildren = false;
			info.mouseEnabled = false;
			toolTip.addChild(info);
		}	
		
		private function setStar():void
		{
			star = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcEquipStar");
		
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
			var count:int = 0;
			stone.txtIsBind.htmlText = UIUtils.getBindShow(data);
			
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

		private function setContent():void
		{
			content = new Sprite();
			var contents:Array = [];
			
			contents = getSoulContent();
		
			if( contents.length == 0 ) return;
			var frame:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcFrameTow");
			showContent(contents);
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
				tf.htmlText = contents[i].text;
				tf.autoSize = TextFieldAutoSize.LEFT;
				content.addChild(tf);
			}
			
		}
		
		//设置属相
		private function setBuidler():void
		{	
			if  ( soulVo.style == 0 ) return;		
			buidler = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcBuidler");
			buidler.txtBuilder.filters = Font.Stroke(0x000000);
			buidler.txtBuilder.htmlText = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_setB_1" ]+"：" + getStyStr( soulVo.style ) ;               //属相
			buidler.txtBuilder.mouseEnabled = false;
			buidler.mouseEnabled  = false;
			toolTip.addChild(buidler);
		}
		
		private var noDescription:Boolean = false;
		
		private function setDescription():void
		{
			if ( UIConstData.getItem(data.type).description == "" || UIConstData.getItem(data.type).description == "0" )
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
				this.toolTip.mouseEnabled = true;
				this.toolTip.mouseChildren = true;
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
		
		//获取中间说明信息
		private function getSoulContent():Array
		{
			var result:Array = new Array();
			
			//成长率
			if ( soulVo.growPercent >=0 )
			{
				var growPercentStr:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_getS_1" ]+"：</font>" + getGrowPercentStr( soulVo.growPercent );//成长率
				result.push( { text:growPercentStr } );
			}
			
			//合成等级
			if ( soulVo.composeLevel >=0 )
			{
				var composeLevelStr:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_soul_med_soulM_initExt_1" ]+"：</font>" + getComposeLevelStr( soulVo.composeLevel );//合成等级
				result.push( { text:composeLevelStr } );
			}
			
			//外攻
			if ( soulVo.phyAttack >=0 )
			{
				var phyAttackStr:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_rank_dat_rcd_14" ]+"：+" + soulVo.phyAttack + "</font>";//外攻
				result.push( { text:phyAttackStr } );
			}
			//内攻
			if ( soulVo.magAttack >=0 )
			{
				var magAttackStr:String = "<font color='#e2cca5'>"+GameCommonData.wordDic[ "mod_rank_dat_rcd_16" ]+"：+" + soulVo.magAttack + "</font>";//内攻
				result.push( { text:magAttackStr } );
			}
			
			//力量
			if ( soulVo.force >=0 )
			{
				var forceStr:String = "<font color='#ff7e00'>"+GameCommonData.wordDic[ "mod_too_con_int_con_9" ]+"：+" + soulVo.force + "</font>";//力量
				result.push( { text:forceStr } );
			}
			
			//灵力
			if ( soulVo.spirit >=0 )
			{
				var spiritStr:String = "<font color='#ff7e00'>"+GameCommonData.wordDic[ "mod_too_con_int_con_10" ]+"：+" + soulVo.spirit + "</font>";//灵力
				result.push( { text:spiritStr } );
			}
			//体力
			if ( soulVo.physical >=0 )
			{
				var physicalStr:String = "<font color='#ff7e00'>"+GameCommonData.wordDic[ "mod_too_con_int_con_11" ]+"：+" + soulVo.physical + "</font>";//体力
				result.push( { text:physicalStr } );
			}
			
			//定力
			if ( soulVo.constant >=0 )
			{
				var constantStr:String = "<font color='#ff7e00'>"+GameCommonData.wordDic[ "mod_too_con_int_con_12" ]+"：+" + soulVo.constant + "</font>";//定力
				result.push( { text:constantStr } );
			}
			//身法
			if ( soulVo.magic >=0 )
			{
				var magicStr:String = "<font color='#ff7e00'>"+GameCommonData.wordDic[ "mod_too_con_int_con_13" ]+"：+" + soulVo.magic + "</font>";//身法
				result.push( { text:magicStr } );
			}
			
			//扩展属性槽数
			var extendNumStr:String = "<font color='#28fc96'>"+GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_getS_2" ]+"：" + getExtNum() + "</font>";//扩展属性槽数
			result.push( { text:extendNumStr } );
			
//			var addAttStrArr:Array = [ "冰攻击","内攻","火攻击","内防","暴击",
//														"外攻","命中","外防","减冰抗性","减火抗性" ];
			for ( var i:uint=0; i<soulVo.extProperties.length; i++ )
			{
				if ( soulVo.extProperties[i] )
				{
					var attStr:String;
					var extPro:SoulExtPropertyVO = soulVo.extProperties[i];
					var extColor:String = IntroConst.itemColors[ getColorIndex( extPro.level ) ];
					if ( i<8 )
					{
						attStr = "<font color='" + extColor+ "'>" + extPro.name + "(" +extPro.level + GameCommonData.wordDic[ "often_used_level" ]+")：+" +extPro.addProperty  + "</font>";//级 
					}
					else
					{
//						attStr = "<font color='#fffe65'>" + addAttStrArr[i] + "："  + extPro.addProperty + "(" +extPro.level + "级)"+ "</font>";
						attStr = "<font color='" + extColor+ "'>" + extPro.name + "(" +extPro.level + GameCommonData.wordDic[ "often_used_level" ]+")：" +extPro.addProperty  + "</font>";//级
					}
					if ( extPro.level > 0 )
					{
						result.push( { text:attStr } );
					}
				}
			}
			
			//是否打了宝石
			if(data.stoneBaseList!=null)
			{
				for(var k:uint=0;k<data.stoneBaseList.length;k++)
				{
					var baseName:String = IntroConst.AttDic[int(data.stoneBaseList[k]["stoneAtt"] / 10000)-1];
					var baseValue:int = int(data.stoneBaseList[k]["stoneAtt"] % 10000)
					var baseStr:String= baseName + "：+" + baseValue;
					baseStr=UIUtils.textfillWithSpace(baseStr,20);
					result.push({text:"<font color='#fffe65'>" + baseStr + "</font><font color='#fffe65'>"+data.stoneBaseList[k]["stoneName"]+"</font>"});	
				}		
			} 
			
			//添加技能说明
			for ( var j:uint=0; j<soulVo.soulSkills.length; j++ )
			{
				if ( soulVo.soulSkills[ j ] )
				{
					var skill:SoulSkillVO = soulVo.soulSkills[ j ];
					if(skill.state == 1)
					{
						continue;
					}
					result.push( { text:"<font color='#ff00ff'>" + skill.name + "("+ skill.level + GameCommonData.wordDic[ "often_used_level" ]+")</font>" } );//级
					//技能说明
//					var skillRemark:String = skill.gameskill.SkillReamark.replace( "N",skill.num  );
//					result.push( { text:"<font color='#e2cca5'>" + skillRemark + "</font>" } );
				}
			}
			return result;
		}
		
		//获取扩展属性槽数
		private function getExtNum():int
		{
			var num:int = 0;
			for ( var i:uint=0; i<soulVo.extProperties.length; i++ )
			{
				if ( soulVo.extProperties[i] )
				{
					var extPro:SoulExtPropertyVO = soulVo.extProperties[i];
					if ( extPro.state <= 1  )
					{
						num ++;
					}
				}
			}
			return num;
		}
		
		private function getGrowPercentStr( num:int ):String
		{
			var pecentStr:String;
			if ( num >= 500 && num <= 649 )
			{
				pecentStr = "<font color='" + IntroConst.itemColors[ 1 ] + "'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_16" ]+"("+num+")</font>";//普通
			}
			else if ( num >= 650 && num <= 749 )
			{
				pecentStr = "<font color='" + IntroConst.itemColors[ 2 ] + "'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_15" ]+"("+num+")</font>";//优秀
			}
			else if ( num >= 750 && num <= 849 )
			{
				pecentStr = "<font color='" + IntroConst.itemColors[ 3 ] + "'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_14" ]+"("+num+")</font>";//杰出
			}
			else if ( num >= 850 && num <= 949 )
			{
				pecentStr = "<font color='" + IntroConst.itemColors[ 4 ] + "'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ]+"("+num+")</font>";//卓越
			}
			else if ( num >= 950 && num <= 1000 )
			{
				pecentStr = "<font color='" + IntroConst.itemColors[ 5 ] + "'>"+GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ]+"("+num+")</font>";//完美
			}
			return pecentStr;
		}
		
		private function getComposeLevelStr( num:int ):String
		{
			var str:String;
			if ( num<5 )
			{
				str = "<font color='" + IntroConst.itemColors[ num ] + "'>"+num+"</font>";
			}
			else
			{
				str = "<font color='" + IntroConst.itemColors[ 5 ] + "'>"+num+"</font>";
			}
			return str;
		}
		
		private function getStyStr( num:int ):String
		{
			var str:String;
			switch ( num )
			{
				case 1:
					str = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_getStyS_1" ];//"地。克制水，被风克制";
				break;
				case 2:
					str = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_getStyS_2" ];//"水。克制火，被地克制";
				break;
				case 3:
					str = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_getStyS_3" ];//"火。克制风，被水克制";
				break;
				case 4:
					str = GameCommonData.wordDic[ "mod_Too_med_UI_soulToo_getStyS_4" ];//"风。克制地，被火克制";
				break;
			}
			return str;
		}
		
		private function getColorIndex( num:int ):int
		{
			var index:int;
			if ( num<5 )
			{
				index = num;
			}
			else
			{
				index = 5;	
			}
			return index;
		}
		
	}
}