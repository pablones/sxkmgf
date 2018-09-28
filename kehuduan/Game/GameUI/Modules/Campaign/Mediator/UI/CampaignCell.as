package GameUI.Modules.Campaign.Mediator.UI
{
	import GameUI.Modules.Campaign.Event.CampaignEvent;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	public class CampaignCell extends Sprite
	{
		private var main_mc:MovieClip = new MovieClip();
		private var bg_mc:MovieClip = new MovieClip();
		private var dataObj:Object;
		
		public function CampaignCell(_main_mc:MovieClip,_bg_mc:MovieClip,_dataObj:Object)
		{
			main_mc = _main_mc;
			bg_mc = _bg_mc;
			dataObj = _dataObj;
			initUI();
		}
		
		private function initUI():void
		{
//			checkBackGround();
			showMain();
			setText();
		}
		
		private function checkBackGround():void
		{
			if ( (dataObj.id%2)==0 )
			{
				bg_mc.x = 0;
				bg_mc.y = 0;
				this.addChild( bg_mc );
			}
		}
		
		private function showMain():void
		{
			this.addChild( main_mc );
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function addStageHandler(evt:Event):void
		{
			( main_mc.single_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,clickCellHandler );
//			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			this.addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler);
		}
		
		private function removeStageHandler(evt:Event):void
		{
			this.removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler);
			( main_mc.single_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,clickCellHandler );
		}
		
		private function clickCellHandler(evt:MouseEvent):void
		{
			this.dispatchEvent( new CampaignEvent(CampaignEvent.CLICK_CELL,dataObj.remark) );
		}
		
		private function setText():void
		{
			( main_mc.time_txt as TextField ).text = dataObj.startTime + "—" + dataObj.endTime;
			( main_mc.name_txt as TextField ).text = dataObj.comName;
			( main_mc.local_txt as TextField ).text = dataObj.local;
			( main_mc.require_txt as TextField ).text = dataObj.require;
			( main_mc.award_txt as TextField ).text = dataObj.award;
			
			( main_mc.time_txt as TextField ).mouseEnabled = false;
			( main_mc.name_txt as TextField ).mouseEnabled = false;
			( main_mc.local_txt as TextField ).mouseEnabled = false;
			( main_mc.require_txt as TextField ).mouseEnabled = false;
			( main_mc.award_txt as TextField ).mouseEnabled = false;
		}
		
		private function chgTextColor(color:uint):void
		{
			( main_mc.time_txt as TextField ).textColor = color;
			( main_mc.name_txt as TextField ).textColor = color;
			( main_mc.local_txt as TextField ).textColor = color;
			( main_mc.require_txt as TextField ).textColor = color;
			( main_mc.award_txt as TextField ).textColor = color;
		}
		
		public function checkTime():void
		{
			var startArr:Array = dataObj.startTime.split(":");
			var startHour:uint = uint(startArr[0]);
			var startMin:uint = uint(startArr[1]);
			
			var endArr:Array = dataObj.endTime.split(":");
			var endHour:uint = uint(endArr[0]);
			var endMin:uint = uint(endArr[1]);
			
			if ( dataObj.id == 15 )
			{
				endHour = 24;
			}
			
			var startSecond:uint = startHour*3600+startMin*60;
			var endSecond:uint = endHour*3600+endMin*60;
			var nowSecond:uint = GameCommonData.gameHour*3600+GameCommonData.gameMinute*60;
			
			if ( nowSecond>endSecond )
			{
				chgTextColor(0x666666);
			}
			else if ( nowSecond<startSecond )
			{
				chgTextColor(0xff0000);
			}
			else
			{
				chgTextColor(0x00ff00);
			}
		}
		
	}
}