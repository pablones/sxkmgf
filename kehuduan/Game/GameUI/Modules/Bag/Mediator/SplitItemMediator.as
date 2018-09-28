package GameUI.Modules.Bag.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Mediator.DealItem.SplitItem;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SplitItemMediator extends Mediator
	{
		public static const NAME:String = "SplitItemMediator";
		private var grid:GridUnit = null;
		private var panelBase:PanelBase = null;
		private var itemData:Object = null;
		private var curNum:int = 1;
		
		public function SplitItemMediator()
		{
			super(NAME);
		}
		
		private function get splitView():MovieClip
		{
			return this.viewComponent as MovieClip
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				BagEvents.SHOWSPLIT,
				BagEvents.REMOVE_SPLIT
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case BagEvents.SHOWSPLIT:
					BagData.SplitIsOpen = true;
					grid = notification.getBody() as GridUnit;						//拆分的背包格
					itemData = BagData.AllItems[BagData.SelectIndex][grid.Index];	//物品数据
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SPLITVIEW});
					panelBase = new PanelBase(splitView, splitView.width-9, splitView.height+12);
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_bag_med_spl_han_1" ] );   //拆 分
					GameCommonData.GameInstance.GameUI.addChild(panelBase); 
					var point:Point = getSplitPos();
					panelBase.x = point.x;
					panelBase.y = point.y;
					curNum = 1;
					initView();
					BagData.lockBagGridUnit(false);
					BagData.lockBtnCleanAndPage(false);
				break;
				case BagEvents.REMOVE_SPLIT:
					if(BagData.SplitIsOpen)
						panelCloseHandler(null);
				break;
			}
		}
		
		private function initView():void
		{
			splitView.txtNum.restrict = '0-9';
			splitView.txtNum.addEventListener(Event.CHANGE, onTextInput);
			splitView.txtNum.text = curNum;
			UIUtils.addFocusLis(splitView.txtNum);
			splitView.stage.focus = splitView.txtNum;
			(splitView.txtNum as TextField).setSelection(splitView.txtNum.length, splitView.txtNum.length);
			splitView.btnAdd.addEventListener(MouseEvent.CLICK, addHandler);
			splitView.btnSub.addEventListener(MouseEvent.CLICK, subHandler);
			splitView.btnComfirm.addEventListener(MouseEvent.CLICK, comFirmHandler);
			splitView.btnCancel.addEventListener(MouseEvent.CLICK, cancelHandler);
		}
		
		private function getSplitPos():Point
		{
			var point:Point = new Point();
			var tmpX:Number = BagData.SelectedItem.Grid.x
			var tmpY:Number = BagData.SelectedItem.Grid.y;
			point = GameCommonData.GameInstance.GameUI.getChildByName(UIConfigData.BAG).localToGlobal(new Point(BagData.SelectedItem.Grid.x+42, BagData.SelectedItem.Grid.y));
			var pointX:Number = point.x;
			var pointY:Number = point.y;
			if(pointX < 0) {
				point.x = 0;
			} else if(pointX + panelBase.width > 1000) {
				point.x = 1000 - panelBase.width;
			}
			if(pointY < 0) {
				point.y = 0;
			} else if(pointY + panelBase.height > 580) {
				point.y = 580 - panelBase.height;
			}
			return point;
		}
		
		private function onTextInput(event:Event):void
		{
			var num:int = int(splitView.txtNum.text);
			if(num > itemData.amount-1)
			{
				num = itemData.amount-1;
			}		
			if(num < 1)
			{
				num = 1;
			}
			splitView.txtNum.text = num.toString();
		}
		
		private function addHandler(event:MouseEvent):void
		{
			curNum = int(splitView.txtNum.text);
			curNum++;
			if(curNum>itemData.amount-1)
			{
				curNum = itemData.amount-1;
			}
			splitView.txtNum.text = curNum;
		}
		
		private function subHandler(event:MouseEvent):void
		{
			curNum = int(splitView.txtNum.text);
			curNum--;
			if(curNum<1)
			{
				curNum = 1;
			}
			splitView.txtNum.text = curNum;
		}
		
		private function comFirmHandler(event:MouseEvent):void
		{
			curNum = int(splitView.txtNum.text);
			if(curNum>itemData.amount-1 || curNum<1) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_bag_med_spl_com_1" ], color:0xffff00 } );  //  请输入正确的拆分数字
				return;
			}
			SplitItem.Split(grid, int(splitView.txtNum.text));
			panelCloseHandler(null);
		}
		
		private function cancelHandler(event:MouseEvent):void
		{
			panelCloseHandler(null);
		}
		
		private function removeListener():void
		{
			splitView.btnAdd.removeEventListener(MouseEvent.CLICK, addHandler);
			splitView.btnSub.removeEventListener(MouseEvent.CLICK, subHandler);
			splitView.btnComfirm.removeEventListener(MouseEvent.CLICK, subHandler);
			splitView.btnCancel.removeEventListener(MouseEvent.CLICK, subHandler);
			UIUtils.removeFocusLis(splitView.txtNum);
		}
		
		private function panelCloseHandler(event:Event):void
		{
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			removeListener();
			panelBase = null;
			this.viewComponent = null;
			BagData.SplitIsOpen = false;
			facade.removeMediator(NAME);
			BagData.lockBagGridUnit(true);
			BagData.lockBtnCleanAndPage(true);
		}
	}
}