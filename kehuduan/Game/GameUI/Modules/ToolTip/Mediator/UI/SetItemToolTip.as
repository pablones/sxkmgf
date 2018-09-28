package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	
	public class SetItemToolTip implements IToolTip
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
		private var lastTimeSp:Sprite;
		
		//关闭按钮
		private var closeBtn:SimpleButton = null;
		
		public function SetItemToolTip(view:Sprite, data:Object, isDrag:Boolean = false, closeHandler:Function = null)
		{
			this.toolTip = view;
			this.data = data;
			this.isDrag = isDrag;
			this.closeHandler = closeHandler;	
		}
		
		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			toolTip.addChild(back);
			if(!UIConstData.getItem(data.type)) { 
				var t:TextField = new TextField();
				t.width = back.width;
				t.textColor = 0xff0000;
				t.text = GameCommonData.wordDic[ "mod_too_med_ui_bus_sho_1" ];         //暂无该物品说明
				t.x = 8;
				t.y = 8;
				back.addChild(t);
				return;
			}
			setTitle();
			setInfo();
			setType();
			setContent();
			setLastTime();
			upDatePos();
		}
		
		public function GetType():String
		{
			return "SetItemToolTip";
		}
		
		public function Update(obj:Object):void
		{
			
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
//			title.txtTitle.defaultTextFormat = IntroConst.fontTf(17, TextFormatAlign.CENTER);
			title.txtTitle.filters = Font.Stroke(0x000000);
			//////////读取物品颜色
			var color:uint = UIConstData.getItem(data.type).Color;
			title.txtTitle.htmlText = '<font size="18" color="' + IntroConst.itemColors[color] + '">' + UIConstData.getItem(data.type).Name + '</font>';
			///////////
//			title.txtTitle.htmlText = '<font size="18">' + UIConstData.ItemDic[data.type].Name + '</font>';
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
			info.txtUseLevel.filters = Font.Stroke(0x000000);
			info.txtJobFilter.filters = Font.Stroke(0x000000);
			info.txtMaleFilter.filters = Font.Stroke(0x000000);
			info.txtItemLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtUseLevel.autoSize = TextFieldAutoSize.LEFT;
			info.txtJobFilter.autoSize = TextFieldAutoSize.LEFT;
			info.txtMaleFilter.autoSize = TextFieldAutoSize.LEFT;
			info.txtItemLevel.text = UIUtils.getLevTitle(data.type)+UIConstData.getItem(data.type).Level;
			if(UIConstData.getItem(data.type).Level > GameCommonData.Player.Role.Level)
			{
				info.txtItemLevel.textColor = 0xff0000;
			}
			info.txtUseLevel.visible = false;
			if(UIConstData.getItem(data.type).Job == 0)  
			{
				info.txtJobFilter.visible = false;	
			}
			else
			{
				if(UIConstData.getItem(data.type).Job != GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob].Job)
				{
					info.txtJobFilter.textColor = 0xff0000;
				}
			}
			if(UIConstData.getItem(data.type).Sex == 0) 
			{
				info.txtMaleFilter.visible = false;
			}
			else
			{
				if(UIConstData.getItem(data.type).Sex != GameCommonData.Player.Role.Sex)
				{
					info.txtMaleFilter.textColor = 0xff0000;
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
			if(data.type == 610024 || data.type == 610040) {	//小喇叭
				type.txtItemType.text = GameCommonData.wordDic[ "mod_too_med_ui_seti_set_1" ];           //其他
			} else {
				type.txtItemType.text = UIConstData.getItem(data.type).UseType;
			}
//			if(data.isBind)
//			{
//				type.txtIsBind.text = "已绑定"; 
//			}
//			else
//			{
//				type.txtIsBind.text = "";
//			}
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
				contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_for_setc_1" ], color:0xffff00});            //双击或选中后点击"使用"按钮使用
			}
			var typeMul:uint = data.type / 1000;
			if(typeMul == 301 || typeMul == 311 || typeMul == 321) {	//大血大蓝	(客户端添加剩余血量)
				if(data.id == undefined || data.id == 0) {
					var maxUse:uint = 0;
					if(typeMul == 311) {	//大蓝
						maxUse = UIConstData.getItem(data.type).MP;
					} else {				//大血
						maxUse = UIConstData.getItem(data.type).HP;
					}
					contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_seti_set_2" ] + maxUse + '/' + maxUse, color:0x0098ff});        //剩余量：
				} else {
					contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_seti_set_2" ] + data.noUse + '/' + data.maxUse, color:0x0098ff});         //剩余量：
				}
			}
			if(typeMul == 351) {
				contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_seti_set_3" ] + data.amountMoney, color:0x0098ff});            //元宝数量：
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
				tf.defaultTextFormat = new TextFormat("宋体", 12);          //宋体
				tf.width = toolTip.width;
				tf.wordWrap = true;
				tf.x = 8;
				if(i == 0) {
					tf.y = 4;	//+ 4;  
				} else {
					tf.y = content.height + 3;	//+ 4;  
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
				tf.defaultTextFormat = new TextFormat("宋体" , 12);            //宋体
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
		
		public function BackWidth():Number
		{
			return back.width;
		}
	}
}