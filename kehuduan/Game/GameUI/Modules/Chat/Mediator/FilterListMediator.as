package GameUI.Modules.Chat.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class FilterListMediator extends Mediator
	{
		public static const NAME:String = "FilterListMediator";
		
		private var panelBase:PanelBase;
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		private var curSelectItem:MovieClip;
		
		public function FilterListMediator()
		{
			super(NAME);
		}
		
		private function get filterList():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				ChatEvents.SHOWFILTERLIST,
				ChatEvents.HIDEFILTERLIST,
				ChatEvents.UPDATEFILTER
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case ChatEvents.SHOWFILTERLIST:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.FILTERLIST});
					panelBase = new PanelBase(filterList, filterList.width+8, filterList.height+12);
					panelBase.addEventListener(Event.CLOSE, panelClose);
					panelBase.x = ChatData.tmpFilterPoint.x;
					panelBase.y = ChatData.tmpFilterPoint.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_chat_med_fil_hand" ]);//"屏蔽列表"
					GameCommonData.GameInstance.GameUI.addChild(panelBase);	
					filterList.btnDelete.addEventListener(MouseEvent.CLICK, deleteItem);
					filterList.btnCancel.addEventListener(MouseEvent.CLICK, panelClose);
					initView();
				break;
				case ChatEvents.UPDATEFILTER:
					setFilter(notification.getBody() as String)
				break
				case ChatEvents.HIDEFILTERLIST:
					panelClose(null);
				break;
			}
		} 
		
		private function initView():void
		{
			listView = new ListComponent(false);
			showFilterList();
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = 10;
			iScrollPane.y = 10;
			iScrollPane.width = 133;
			iScrollPane.height = 195;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			
			curSelectItem = null;
			filterList.addChild(iScrollPane);
		}
		
		private function setFilter(name:String):void
		{
			ChatData.FilterList.push(name);
			var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("FilterItem");
			item.name = name;	//ChatData.FilterList.length.toString();
			item.mcSelected.visible = false;
			item.filterBtn.width = 140;
			item.mcSelected.width = 140; 
			item.txtName.text = name;
			item.txtName.mouseEnabled = false;
			item.addEventListener(MouseEvent.CLICK, selectItem);
			listView.SetChild(item);			
//			listView.width = 110;
			listView.upDataPos();
		}
		
		private function showFilterList():void
		{
			for(var i:int = 0; i<ChatData.FilterList.length; i++)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("FilterItem");
				item.name = ChatData.FilterList[i];//i.toString();
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnable = false;
				item.txtName.mouseEnabled = false;
//				item.width = 110;
				item.filterBtn.width = 140;
				item.mcSelected.width = 150; 
				item.txtName.text = ChatData.FilterList[i];
				item.addEventListener(MouseEvent.CLICK, selectItem);
				listView.SetChild(item);
			}
//			listView.width = 110;
			listView.upDataPos();
		}
		
		private function deleteItem(event:MouseEvent):void
		{
			if(curSelectItem == null) return;
			var index:int = ChatData.FilterList.indexOf(curSelectItem.name);
			if(index >= 0) {
				ChatData.FilterList.splice(index, 1);
			}
//			ChatData.FilterList.splice(int(curSelectItem.name), 1);
			upDateName();
			listView.removeChild(curSelectItem);
			curSelectItem = null;
			listView.upDataPos();
			iScrollPane.refresh();
		}
		
		private function upDateName():void
		{
			//for(var i:int = 0; i<listView; i++)
			//{
			//	ChatData.FilterList[i].name = i.toString();
			//}	
		}
		
		private function selectItem(event:MouseEvent):void
		{
			for(var i:int = 0; i<listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			var item:MovieClip = event.currentTarget as MovieClip;
			item.mcSelected.visible = true;
			curSelectItem = item; 
		}
		
		private function panelClose(event:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				iScrollPane = null;
				this.viewComponent = null;
				panelBase = null;
				facade.removeMediator(NAME);
				ChatData.FilterListIsOpen = false;
			}
		}
	}
}