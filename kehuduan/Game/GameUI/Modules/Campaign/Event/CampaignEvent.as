package GameUI.Modules.Campaign.Event
{
	import flash.events.Event;

	public class CampaignEvent extends Event
	{
		public static const CLICK_CELL:String = "CLICK_CAMPAIGN_CELL";
		public var remark:String;
		public function CampaignEvent(type:String, _remark:String, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.remark = _remark;
		}
		
	}
}