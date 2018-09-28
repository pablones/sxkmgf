package GameUI.Modules.Master.View
{
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.OldMaster;
	import GameUI.View.ResourcesFactory;
	
	import Net.ActionSend.FriendSend;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	public class MyMasterCell extends Sprite
	{
		public var talkMasterFun:Function;
		
		public var oldMaster:OldMaster;
		private var main_mc:MovieClip;
		private var faceBmp:Bitmap;
		private var qMask:Bitmap;
		
		public function MyMasterCell( _oldMaster:OldMaster )
		{
			oldMaster = _oldMaster;
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function initUI():void
		{
			main_mc = new ( MasterData.masResPro.myMasterClass ) as MovieClip;
			addChild( main_mc );
			
			( main_mc.name_txt as TextField ).mouseEnabled = false;
			( main_mc.line_txt as TextField ).mouseEnabled = false;
			( main_mc.level_txt as TextField ).mouseEnabled = false;
			( main_mc.mainJob_txt as TextField ).mouseEnabled = false;
			( main_mc.viceJob_txt as TextField ).mouseEnabled = false;
			( main_mc.rate_txt as TextField ).mouseEnabled = false;
			
			( main_mc.name_txt as TextField ).text = oldMaster.name;
			( main_mc.line_txt as TextField ).htmlText = oldMaster.lineStr;
			( main_mc.level_txt as TextField ).text = oldMaster.roleLevel+GameCommonData.wordDic[ "often_used_level" ];     //级
			( main_mc.rate_txt as TextField ).htmlText = oldMaster.impartLevelStr;
			
			if ( oldMaster.mainJob == 0 )
			{
				( main_mc.mainJob_txt as TextField ).text = GameCommonData.wordDic[ "often_used_none" ];       //无
			}
			else
			{
				( main_mc.mainJob_txt as TextField ).text = GameCommonData.RolesListDic[ oldMaster.mainJob ] + " "+oldMaster.mainJobLevel + GameCommonData.wordDic[ "often_used_level" ];         //级
			}
			if ( oldMaster.viceJob == 0 )
			{
				( main_mc.viceJob_txt as TextField ).text = GameCommonData.wordDic[ "often_used_none" ];          //无
			}
			else
			{
				( main_mc.viceJob_txt as TextField ).text = GameCommonData.RolesListDic[ oldMaster.viceJob ] + " "+oldMaster.viceJobLevel + GameCommonData.wordDic[ "often_used_level" ];	    //级
			}
			
			var bmpData:BitmapData = GameCommonData.GameInstance.Content.Load( GameConfigData.UILibrary ).GetClassByBitmapData("qMark");
			qMask = new Bitmap( bmpData );
			qMask.x = 16;
			qMask.y = 13;
			main_mc.addChild( qMask );
			loadSource();
			
			( main_mc.lookInfo_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,lookInfo );
			( main_mc.sendMsg_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,sendMsg );
		}
		
		public function loadSource():void
		{
			try
			{
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Face/" + oldMaster.face + ".png",onLoabdComplete);	
			}
			catch(e:Error)
			{
			}
		}
		
		private function onLoabdComplete():void
		{
			faceBmp = ResourcesFactory.getInstance().getBitMapResourceByUrl(GameCommonData.GameInstance.Content.RootDirectory + "Resources/Face/" + oldMaster.face + ".png");
			if ( faceBmp )
			{
				faceBmp.x = 7;
				faceBmp.y = 4;
				main_mc.addChild( faceBmp );
				if ( qMask && main_mc.contains( qMask ) )
				{
					main_mc.removeChild( qMask );
					qMask = null;
				}
 			}
		}
		
		private function lookInfo( evt:MouseEvent ):void
		{
			FriendSend.getInstance().getFriendInfo( oldMaster.id, oldMaster.name );
		}
		
		private function sendMsg( evt:MouseEvent ):void
		{
//			trace ( "发送消息" );
			if ( talkMasterFun != null )
			{
				talkMasterFun( oldMaster.name );
			}
		}
		
		private function addStageHandler( evt:Event ):void
		{
			initUI();
			addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			( main_mc.lookInfo_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,lookInfo );
			( main_mc.sendMsg_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,sendMsg );
		}
		
		public function gc():void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			var des:*;
			while ( main_mc && main_mc.numChildren > 0 )
			{
				des = main_mc.removeChildAt( 0 );
				des = null;
			}
		}
		
	}
}