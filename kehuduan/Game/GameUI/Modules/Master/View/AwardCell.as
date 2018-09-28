package GameUI.Modules.Master.View
{
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;

	public class AwardCell extends Sprite
	{
		private var main_mc:MovieClip;
		private var level:uint;
		private var pLevel:uint;
		private var giftMask:uint = 8;
		
		public function AwardCell(obj:Object)
		{
			super();
			initData(obj);
			initUI();
		}
		
		private function initData(obj:Object):void
		{
			if ( obj )
			{
				this.level = obj.level;
				this.pLevel = obj.pLevel;
				this.giftMask = obj.giftMask;
			}
		}
		
		private function initUI():void
		{
			main_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MasterInfoCell");
			this.addChild(main_mc);
			( main_mc.btn_txt as TextField ).text = "";
			( main_mc.main_txt as TextField ).text = this.level.toString() + GameCommonData.wordDic[ "mod_mas_view_awa_ini_1" ];       //级出师奖励
			( main_mc.btn_txt as TextField ).mouseEnabled = false;
			( main_mc.btn_txt as TextField ).autoSize = TextFieldAutoSize.CENTER;
			switch ( giftMask )
			{
				case 0:
					( main_mc.btn_txt as TextField ).text = GameCommonData.wordDic[ "mod_med_getExp_3" ];              //领取
					setBtnVisble(true);
				break;
				case 1:
					( main_mc.btn_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_view_awa_ini_2" ];            //已领取
					setBtnVisble(false);
				break;
				case 2:
					( main_mc.btn_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_view_awa_ini_3" ];            //未达成
					setBtnVisble(false);
				break;
				default:
				break;
			}
			
			this.addEventListener(Event.ADDED_TO_STAGE,addStageHandler);
		}
		
		private function addStageHandler(evt:Event):void
		{
			if ( this.giftMask == 0 )
			{
				( main_mc.gain_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,gainAward);
			}
			this.removeEventListener(Event.ADDED_TO_STAGE,addStageHandler);
			this.addEventListener(Event.REMOVED_FROM_STAGE,removeStageHandler);
		}
		
		private function removeStageHandler(evt:Event):void
		{
			if ( this.giftMask == 0 )
			{
				( main_mc.gain_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,gainAward);
			}
			this.removeEventListener(Event.REMOVED_FROM_STAGE,removeStageHandler);
			this.main_mc = null;
		}
		
		private function gainAward(evt:MouseEvent):void
		{
//			trace( "获取奖励" );          
		}
		
		private function setBtnVisble(isSee:Boolean):void
		{
			( main_mc.gain_btn as SimpleButton ).visible = isSee;
		}
		
	}
}