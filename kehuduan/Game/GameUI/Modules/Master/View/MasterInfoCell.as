package GameUI.Modules.Master.View
{
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.OldMaster;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class MasterInfoCell extends Sprite
	{
		public var clickFun:Function;
		public var oldMaster:OldMaster;
		
		private var main_mc:MovieClip;
		private var dateNow:Date;
		
		public function MasterInfoCell( _oldMaster:OldMaster )
		{
			oldMaster = _oldMaster;
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function initUI():void
		{
			main_mc = new ( MasterData.MasterCellClass ) as MovieClip;
			addChild( main_mc );
			
			main_mc.txt_0.text = oldMaster.name;
			main_mc.txt_1.text = oldMaster.sexStr;
			main_mc.txt_2.text = oldMaster.roleLevel;
			main_mc.txt_3.text = GameCommonData.RolesListDic[ oldMaster.job ].toString();
			main_mc.txt_4.text = oldMaster.batNum;
			main_mc.txt_5.htmlText = MasterData.getPrenticeRemark( oldMaster.impart );
			main_mc.txt_6.htmlText = oldMaster.vipStr;
			main_mc.txt_7.htmlText = getLeftTime( oldMaster.outLineTime );
			for ( var i:uint=0; i<8; i++ )
			{
				main_mc[ "txt_"+i ].mouseEnabled = false;
			}
			
			( main_mc.pleaseMaster_btn as SimpleButton ).addEventListener( MouseEvent.CLICK,pleaseMaster );
		}
		
		private function pleaseMaster( evt:MouseEvent ):void
		{
			if ( GameCommonData.Player.Role.Level < 10 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bot_exe_3" ] );      //人物等级不能小于10级
				return;
			}
			
			if ( GameCommonData.Player.Role.Level + 10 > oldMaster.roleLevel )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bot_exe_4" ] );       //只能拜等级超过你10级的玩家为师
				return;
			}

			RequestTutor.requestData( 13,oldMaster.id );
		}
		
		private function onClick( evt:MouseEvent ):void
		{
			if ( clickFun != null )
			{
				clickFun( this );
			}
		}
		
		private function addStageHandler( evt:Event ):void
		{
			dateNow = new Date();
			initUI();
			addEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			addEventListener( MouseEvent.CLICK,onClick );
		}
		
		private function removeStageHandler( evt:Event ):void
		{
			removeEventListener( Event.REMOVED_FROM_STAGE,removeStageHandler );
			removeEventListener( MouseEvent.CLICK,onClick );
			( main_mc.pleaseMaster_btn as SimpleButton ).removeEventListener( MouseEvent.CLICK,pleaseMaster );
		}
		
		public function gc():void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			clickFun = null;
		}
		
		//计算离线时长
		private function getLeftTime( lastTime:uint ):String
		{
			if ( lastTime == 0 )
			{
				return "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_view_gra_get_1" ]+"</font>";           //在线
			}
			var str:String = "";
			var lastTimeStr:String = lastTime.toString();
			
			var oldYear:int = int( lastTimeStr.slice( 0,2 ) );
			var oldMonth:int = int( lastTimeStr.slice( 2,4 ) );
			var oldDay:int = int( lastTimeStr.slice( 4,6 ) );
			var oldHour:int = int( lastTimeStr.slice( 6,8 ) ); 
			
			var yearDis:int = dateNow.fullYear - ( oldYear+ 2000 );			//年间隔
			var monthDis:int = dateNow.month - oldMonth +1;                    //0代表1月
			var dayDis:int = dateNow.date - oldDay;
			var hourDis:int = dateNow.hours - oldHour;
			
			var outDays:int;						//离线的天数
			var outHour:int;					 //离线的小时数
			if ( hourDis >= 0 )
			{
				if ( monthDis >= 0 )
				{
					outDays = yearDis*365 + monthDis*30 + dayDis;
				}
				else
				{
					outDays = yearDis*365 + monthDis*30 + dayDis + 30;
				}
				outHour = hourDis;
			}
			else
			{
				if ( monthDis >= 0 )
				{
					outDays = yearDis*365 + monthDis*30 + dayDis -1;
					if ( outDays<0 )
					{
						outDays = 0;
					}
				}
				else
				{
					outDays = yearDis*365 + monthDis*30 + dayDis + 29;
				}
				outHour = hourDis + 24;
			}
			if ( outDays == 0 )
			{
				if ( outHour == 0 )
				{
					var outMini:int = dateNow.minutes - int( lastTimeStr.slice( 8,10 ) ); 
					if ( outMini <= 0 )
					{
						str = "1"+GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_7" ];     //分钟
					}
					else
					{
						str = outMini + GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_7" ];     //分钟
					}
				}
				else
				{
					str = outHour + GameCommonData.wordDic[ "mod_mas_view_gra_get_2" ];     //小时
				}
			}
			else
			{
				str = outDays + GameCommonData.wordDic[ "mod_wis_med_wis_11" ] + outHour + GameCommonData.wordDic[ "mod_mas_view_gra_get_2" ];   //天           小时
			}
			return str;
		}
		
	}
}