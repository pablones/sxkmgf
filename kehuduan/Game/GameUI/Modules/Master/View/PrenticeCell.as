package GameUI.Modules.Master.View
{
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Event.MasterEvent;
	
	import Net.ActionSend.TutorSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;

	public class PrenticeCell extends Sprite
	{
		private var main_mc:MovieClip;
		private var pName:String;															//徒弟名字
		private var pLevel:uint;																	//徒弟等级
		private var pRate:uint;																	//传授度
		private var relation:int;																	//2为徒弟  4为已出师
		private var id:int;
		
		public function PrenticeCell(obj:Object)
		{
			super();
			initData(obj);
			initUI();
		}
		
		private function initData(obj:Object):void
		{
			if ( obj )
			{
				this.pName = obj.name;
				this.pLevel = obj.level;
				this.pRate = obj.rate;
				this.relation = obj.relation;
				this.id = obj.id;
			}
		}
		
		private function initUI():void
		{
			main_mc = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MasterMainCell");
			this.addChild( main_mc );
			
			( main_mc.name_txt as TextField ).text = pName;
			( main_mc.level_txt as TextField ).text = pLevel.toString() + GameCommonData.wordDic[ "often_used_level" ];        //级
			( main_mc.rate_txt as TextField ).text = pRate.toString() + "/" +MasterData.maxInitiator;
			if ( pRate>MasterData.maxInitiator )
			{
				pRate = MasterData.maxInitiator;
			}
			( main_mc.scale_mc as MovieClip ).width = pRate/MasterData.maxInitiator * 143;										//143为总长度
			
			if ( this.relation == 2 )
			{
				( main_mc.relation_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_view_pre_ini_1" ];           //徒弟
			}
			else if ( this.relation == 4 )
			{
				( main_mc.relation_txt as TextField ).text = GameCommonData.wordDic[ "mod_mas_view_pre_ini_2" ];         //已出师
			}
			
			this.addEventListener(Event.ADDED_TO_STAGE,addStageHandler);
		}
		
		private function addStageHandler(evt:Event):void
		{
			( main_mc.info_btn as SimpleButton ).addEventListener(MouseEvent.CLICK,showInfo);
			this.removeEventListener(Event.ADDED_TO_STAGE,addStageHandler);
			this.addEventListener(Event.REMOVED_FROM_STAGE,removeStageHandler);
		}
		
		private function showInfo(evt:MouseEvent):void
		{
			//发送请求详细信息
			this.dispatchEvent(new MasterEvent(MasterEvent.CLICK_INFO,this.pRate,this.pLevel));
			
			var obj:Object = new Object();
			obj.id = this.id;
			obj.relation = this.relation;
			MasterData.isRequestAward = true;
			TutorSend.sendTutorAction(obj);
		}
		
		private function removeStageHandler(evt:Event):void
		{
			( main_mc.info_btn as SimpleButton ).removeEventListener(MouseEvent.CLICK,showInfo);
			this.removeEventListener(Event.REMOVED_FROM_STAGE,removeStageHandler);
		}
		
	}
}