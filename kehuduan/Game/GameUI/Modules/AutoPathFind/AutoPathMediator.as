package GameUI.Modules.AutoPathFind
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPathFind.Datas.AutoPathData;
	import GameUI.Modules.AutoPathFind.View.AutoPathCell;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class AutoPathMediator extends Mediator
	{
		public static const NAME:String = "AutoPathMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		
		private var aRoleId:Array = null;
		private var cellContainer:Sprite = null;
		private var scroll:UIScrollPane = null;
		
		public function AutoPathMediator()
		{
			super(NAME);
		}
		
		public function get autoPathView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOW_AUTOPATH_UI,
				EventList.HIDE_AUTOPATH_UI,
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					initUI();
					break;

				case EventList.SHOW_AUTOPATH_UI:
					initData();
					showUI();
					sendNotification(EventList.PLAY_SOUND_OPEN_PANEL);
					break;
					
				case EventList.HIDE_AUTOPATH_UI :
					closeUI(null);
					break;
			}
		}
		
		private function initUI():void
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.AUTOPATHPANEL});
			panelBase = new PanelBase(autoPathView, autoPathView.width+8, autoPathView.height+12);
			panelBase.name = "autoPathView";
			panelBase.addEventListener(Event.CLOSE, closeUI);
			panelBase.x = UIConstData.DefaultPos2.x + 224;
			panelBase.y = UIConstData.DefaultPos2.y + 142;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_autoPa_aut_initUI_1" ]);//自动寻路
		}
		
		private function initData():void
		{
			aRoleId = [];
			var xml:XML = GameCommonData.GameInstance.GameScene.GetGameScene.ConfigXml;
			for(var i:int = 0; i< xml.Location.length();i++)
			{
				var obj:Object={};
				obj.Id=xml.Location[i].@Id;
				obj.Name=xml.Location[i].@Name;
				obj.X=xml.Location[i].@X;
				obj.Y=xml.Location[i].@Y;
				obj.Type=xml.Location[i].@Type;
				obj.Remark=xml.Location[i].@Remark;
				aRoleId.push(xml.Location[i].@Id);
				AutoPathData.autoPathDic[uint(obj.Id)]=obj;
			}
		}
		
		private function showUI():void
		{
			dataProxy.AutoRoadIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			panelBase.x = UIConstData.DefaultPos2.x + 224 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
			panelBase.y = UIConstData.DefaultPos2.y + 142;
			creatCells();
		}
		
		private function creatCells():void
		{	
			cellContainer = new Sprite();
			
			for(var i:int = 0; i<aRoleId.length;i++)
			{
				var cell:AutoPathCell = new AutoPathCell(aRoleId[i]);
//				cell.y =24+ i * 16;
				cell.y = i *16;
				cellContainer.addChild(cell);
			}
			var cellHeight:uint = (aRoleId.length)*16 + 5;
			
			cellContainer.graphics.beginFill(0xff0000,0);
//			cellContainer.graphics.drawRect(0,0,155,cellHeight);
			cellContainer.graphics.drawRect(0,0,155,cellHeight);
			cellContainer.graphics.endFill();
			
			scroll = new UIScrollPane(cellContainer);
			autoPathView.addChild(scroll);
//			scroll.refresh();
			scroll.width = 170;
			scroll.height = 209;
			scroll.y = 24;
			scroll.refresh();
		}
		
		private function clearContainer():void
		{
			if(cellContainer && cellContainer.numChildren > 0)
			{
				for(var i:int = cellContainer.numChildren - 1;i >= 0;i--)
				{
					cellContainer.removeChildAt(0);
				}
			}
		}
		
		private function closeUI(evt:Event):void
		{
			if(dataProxy && (!dataProxy.AutoRoadIsOpen))
			{
				return;
			}
			clearContainer();
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
			}
			dataProxy.AutoRoadIsOpen = false;
			scroll = null;
			if(cellContainer)
			{
				cellContainer = null;
			}
		}

	}
}