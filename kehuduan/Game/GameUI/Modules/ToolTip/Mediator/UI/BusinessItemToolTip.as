package GameUI.Modules.ToolTip.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.NPCBusiness.Data.NPCBusinessConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
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
	
	public class BusinessItemToolTip implements IToolTip
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
		private var showPriceOut:Boolean = false;
		
		//关闭按钮
		private var closeBtn:SimpleButton = null;
		
		public function BusinessItemToolTip(view:Sprite, data:Object, isDrag:Boolean = false, closeHandler:Function = null, showPriceOut:Boolean = true)
		{
			this.toolTip = view;
			this.data = data;
			this.isDrag = isDrag;
			this.closeHandler = closeHandler;
			if(data.id) this.showPriceOut = true;
//			this.showPriceOut = showPriceOut;
		}
		
		public function Show():void
		{
			back = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("ToolTipBackBig");
			toolTip.addChild(back);
			if(!UIConstData.getItem(data.type)) { 
				var t:TextField = new TextField();
				t.width = back.width;
				t.textColor = 0xff0000;
				t.text = GameCommonData.wordDic[ "mod_too_med_ui_bus_sho_1" ];            //暂无该物品说明
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
			return "SetItemToolTip";
		}
		
		public function Update(obj:Object):void
		{
			
		}
		
		private function setTitle():void
		{
			title = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("mcToolTipTitle");
			title.txtTitle.filters = Font.Stroke(0x000000);
			var color:uint = UIConstData.getItem(data.type).Color;
			title.txtTitle.htmlText = '<font size="18" color="' + IntroConst.itemColors[color] + '">' + UIConstData.getItem(data.type).Name + '</font>';
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
			type.txtItemType.text = UIConstData.getItem(data.type).UseType;
			if(data.isBind)
			{
				type.txtIsBind.text = GameCommonData.wordDic[ "mod_too_med_ui_bus_sett_1" ];          //已绑定
			}
			else
			{
				type.txtIsBind.text = "";
			}
			type.mouseEnabled = false;
			type.mouseChildren = false;
			toolTip.addChild(type);
		}
		
		private function setContent():void
		{
			content = new Sprite();
			var contents:Array = [	{text:UIConstData.getItem(data.type).description, color:IntroConst.COLOR}
									];
			var typeMul:uint = data.type / 1000;
			if(data.type == 626100) {
				contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_bus_setc_1" ] + data.amountMoney, color:0x0098ff, isShowMoney:1});        //银子数量：
			} else if(typeMul == 626) {
				if(showPriceOut) {
					var amount:int = data.amount;
					var priceIn:int = 0;
					var priceOut:int = 0;
					if(data.id) {	//背包中物品
						var goodToGetPrice:Object = BagData.getItemById(data.id);
						if(goodToGetPrice) {
							priceIn = goodToGetPrice.price;
							contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_bus_setc_2" ] + priceIn, color:0x0098ff, isShowMoney:1});            //买入时均价：
						}
					}
					if(NPCBusinessConstData.goodSalePriceDic[data.type]) {
						priceOut = NPCBusinessConstData.goodSalePriceDic[data.type];
						contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_bus_setc_3" ] + priceOut, color:0x0098ff, isShowMoney:1});              //本店卖出价：
					}
					//计算卖出的盈亏情况
					if(priceIn > 0 && priceOut > 0) {
						var profitOrLoss:int = (priceOut - priceIn) * amount;
						var colorPL:int = 0xff0000;
						var preStr:String = "";
						if(profitOrLoss >= 0) {
							colorPL = 0x00ff00;
							preStr = "+";
						}
						contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_bus_setc_4" ] + preStr + profitOrLoss, color:colorPL, isShowMoney:1});             //本店卖出盈亏：
					}
				} else {
//					if(NPCBusinessConstData.goodList.length > 0) {
//						
//					}
					contents.push({text:GameCommonData.wordDic[ "mod_too_med_ui_bus_setc_5" ] + data.amount, color:0x0098ff});              //库存：
				}
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
				if(!contents[i].isShowMoney) {
					var tf:TextField = new TextField();
					var TF:TextFormat = new TextFormat();
						TF.leading = 3;
					tf.defaultTextFormat = new TextFormat("宋体", 12);             //宋体
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
				} else {
					var moneyMC:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("NPCBusinessMoneyShow");
					(moneyMC.txtMoney as TextField).defaultTextFormat = new TextFormat("宋体", 12);              //宋体
					(moneyMC.txtMoney as TextField).textColor = contents[i].color;
					(moneyMC.txtMoney as TextField).wordWrap = true;
					moneyMC.x = 8;
					if(i == 0) {
						moneyMC.y = 4;	//+ 4;  
					} else {
						moneyMC.y = content.height + 1;	//+ 4;  
					}
					(moneyMC.txtMoney as TextField).filters = Font.Stroke(0);
					(moneyMC.txtMoney as TextField).autoSize = TextFieldAutoSize.LEFT;
					moneyMC.txtMoney.text = contents[i].text + "\\aa";
					ShowMoney.ShowIcon(moneyMC, moneyMC.txtMoney, true);
					content.addChild(moneyMC);
				}
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
	}
}