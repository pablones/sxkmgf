package GameUI.Modules.ToolTip.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.ToolTip.Mediator.UI.BallAndTicketToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.EquipToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.IToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.MasterItemTooltip;
	import GameUI.Modules.ToolTip.Mediator.UI.SetItemToolTip;
	import GameUI.Modules.ToolTip.Mediator.UI.SoulTooltip;
	
	import flash.display.Sprite;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class ItemPanelMediator extends Mediator
	{
		public static const NAME:String = "ItemPanelMediator";
		
		private var itemToolTip:Sprite = null;
		private var setItemToolTip:IToolTip = null;
		
		public function ItemPanelMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.SHOWITEMTOOLPANEL
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{			
			switch(notification.getName())
			{
				case EventList.SHOWITEMTOOLPANEL:
					closeHandler();
					itemToolTip = new Sprite();
					itemToolTip.mouseEnabled = false;
					itemToolTip.mouseChildren = false;
					
					var data:Object = notification.getBody();
					if(data.type > 300000 && data.type < 400000)
					{
						if(data.type == 351001)	//元宝票
						{
							setItemToolTip = new BallAndTicketToolTip(itemToolTip, data.data, true, closeHandler);
						}
						else
						{
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data, true, closeHandler);
						}
						setValue();
					}		
					if(data.type >= 250000 && data.type < 300000 )
					{
						setItemToolTip = new SoulTooltip( itemToolTip, data.data, false,true, closeHandler );
						setValue();
					}
					if(data.type < 250000)
					{
						setItemToolTip = new EquipToolTip(itemToolTip, data.data, false, true, closeHandler);
						setValue();
					}
					//宝石type
					if(data.type >= 400000 && data.type < 500000)
					{
						setItemToolTip = new EquipToolTip(itemToolTip, data.data, false, true, closeHandler);
						setValue();	
					}
					//礼包
					if(data.type >= 500000 && data.type < 600000)
					{
						if ( data.type>=506000 && data.type<=506005 )
						{
							setItemToolTip = new MasterItemTooltip( itemToolTip,data.data, true, closeHandler );
							setValue();	
						}
						else
						{
							setItemToolTip = new SetItemToolTip(itemToolTip, data.data, true, closeHandler);
							setValue();	
						}
					}
					//打孔宝石
					if(data.type >= 600000 && data.type < 700000)
					{
						setItemToolTip = new SetItemToolTip(itemToolTip, data.data, true, closeHandler);
						setValue();		
					}
				break;
			}
		}
		
		private function setValue():void
		{
			setItemToolTip.Show();
			GameCommonData.GameInstance.TooltipLayer.addChild(itemToolTip);
			
			itemToolTip.x = GameCommonData.GameInstance.TooltipLayer.mouseX;
			itemToolTip.y = GameCommonData.GameInstance.TooltipLayer.mouseY - itemToolTip.height;
			changePos();
		}
		
		private function changePos():void
		{
			if(itemToolTip.x > GameCommonData.GameInstance.stage.stageWidth - itemToolTip.width)
			{
				itemToolTip.x = itemToolTip.x - setItemToolTip.BackWidth();
			}
			if(itemToolTip.y > GameCommonData.GameInstance.stage.stageHeight - itemToolTip.height)
			{
				itemToolTip.y = itemToolTip.y - itemToolTip.height;
			}
			if(itemToolTip.y < 0)
			{
				itemToolTip.y = 0;
			} 
		}
		
		private function closeHandler():void
		{
			if(itemToolTip)
			{
				if(GameCommonData.GameInstance.TooltipLayer.contains(itemToolTip))
				{
					GameCommonData.GameInstance.TooltipLayer.removeChild(itemToolTip);
					itemToolTip = null;
				}
			}
		}		
	}
}