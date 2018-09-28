package GameUI.Modules.Hint.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Hint.Mediator.UI.HintView;
	
	import flash.events.Event;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class HintMediator extends Mediator
	{
		public static const NAME:String = "HintMediator";
		
		private var delayNum:int = 0;
		private var textViews:Array = new Array();
//		private var posList:Array = [[281,100],[281,115],[281,130],[281,145],[281,162]];
//		private var posList:Array = [[270,75],[270,90],[270,105],[270,120],[270,137]];
		private var posList:Array = [[270,45],[270,60],[270,75],[270,90],[270,105]];
		
		public function HintMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
//				EventList.ENTERMAPCOMPLETE, 
				HintEvents.RECEIVEINFO
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case HintEvents.RECEIVEINFO:
					var text:String = notification.getBody().info as String;
					var color:uint = notification.getBody().color as uint;
					setTextInfo(text, color);
				break;
			}
		}
		
		private function setTextInfo(info:String, color:uint):void		
		{
			var hintView:HintView = new HintView(info,color);
			textViews.push(hintView);
			hintView.Pos = textViews.length - 1;
			hintView.StartShow();
			hintView.addEventListener(Event.COMPLETE, showOver); 
//			GameCommonData.GameInstance.GameUI.addChild(hintView);
			GameCommonData.GameInstance.TooltipLayer.addChild(hintView);
			if(textViews.length>5)
			{
				var tmpView:HintView = textViews.shift() as HintView;
//				if(GameCommonData.GameInstance.GameUI.contains(tmpView))
				if(GameCommonData.GameInstance.TooltipLayer.contains(tmpView))
				{
//					GameCommonData.GameInstance.GameUI.removeChild(tmpView);
					GameCommonData.GameInstance.TooltipLayer.removeChild(tmpView);
				}
				tmpView.removeEventListener(Event.COMPLETE, showOver);
				tmpView = null;
				changePos();
			}
			else
			{
				if(textViews[0].Pos != 0)
				{	
					posSort();
					for(var i:int = 0; i<textViews.length; i++)
					{				
						textViews[i].x = posList[textViews[i].Pos-1][0];
						if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
			            {
			                textViews[i].x += (GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth - 80)/2;
		                }
						textViews[i].y = posList[textViews[i].Pos-1][1];
						textViews[i].Pos = textViews[i].Pos - 1;
					}
				}
				else
				{
					for(var j:int = 0; j<textViews.length; j++)
					{				
						textViews[j].x = posList[j][0];
						if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
			            {
			                textViews[j].x += (GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth - 80)/2;
		                }
						textViews[j].y = posList[j][1];
					}
				}
			}			
		}
		
		private function posSort():void
		{
			for(var i:int = 0; i<textViews.length; i++)
			{
				textViews[i].Pos = textViews[0].Pos + i;	
			}
		}
		
		private function showOver(event:Event):void
		{
			var tmpView:HintView = textViews.shift() as HintView;
//			if(GameCommonData.GameInstance.GameUI.contains(tmpView))
			if(GameCommonData.GameInstance.TooltipLayer.contains(tmpView))
			{
//				GameCommonData.GameInstance.GameUI.removeChild(tmpView);
				GameCommonData.GameInstance.TooltipLayer.removeChild(tmpView);
			}
			tmpView.removeEventListener(Event.COMPLETE, showOver);
			tmpView = null;
		}
	
		private function changePos():void
		{
			for(var i:int = 0; i<textViews.length; i++)
			{
				textViews[i].x = posList[i][0];
				if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
			    {
			        textViews[i].x += (GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth - 80)/2;
		        }
				textViews[i].y = posList[i][1];
				textViews[i].Pos = i;
			}
		}
	}
}