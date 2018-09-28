package GameUI.Modules.Master.View
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Master.Proxy.YoungStudent;
	import GameUI.UICore.UIFacade;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class StudentListCell extends Sprite
	{
		public var youngStudent:YoungStudent;
		public var clickFun:Function;
		
		private var dateNow:Date;
		private var main_mc:MovieClip;
		
		public function StudentListCell( _youngStudent:YoungStudent )
		{
			youngStudent = _youngStudent;
			addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function initUI():void
		{
			main_mc = new ( MasterData.masResPro.studentListCellClass ) as MovieClip;
			addChild( main_mc );
			
			main_mc.txt_0.text = youngStudent.name;
			main_mc.txt_1.text = youngStudent.sexStr;
			main_mc.txt_2.text = youngStudent.roleLevel;
			main_mc.txt_3.text = GameCommonData.RolesListDic[ youngStudent.job ].toString();
			main_mc.txt_4.text = youngStudent.batNum;
			main_mc.txt_5.htmlText = youngStudent.vipStr;
			main_mc.txt_6.htmlText = getLeftTime( youngStudent.outLineTime );
			for ( var i:uint=0; i<7; i++ )
			{
				main_mc[ "txt_"+i ].mouseEnabled = false;
			}
			
			main_mc.kickOut_btn.addEventListener( MouseEvent.CLICK,kickOut );
		}
		
		//踢出徒弟要有脚本
		private function kickOut( evt:MouseEvent ):void
		{                                             //强制解除师徒关系需要                                                                     60点活力和精力                                                     。你确定要踢出徒弟                                                                                                          吗？
			var info:String = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_com_bet_exe_1" ]+"<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_bet_exe_2" ]+"</font>"+GameCommonData.wordDic[ "mod_mas_view_stul_kic_1" ]+"<font color='#ffff00'>["+youngStudent.name+"]</font>"+GameCommonData.wordDic[ "mod_mas_view_stul_kic_1" ]+"</font>";
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification(EventList.SHOWALERT, {comfrim:kickOutHandler, cancel:cancelClose, info:info, title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ],comfirmTxt:GameCommonData.wordDic[ "mod_mas_com_agr_exe_3" ],cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ]});   //提 示      确 定      取 消  
		}
		
		private function kickOutHandler():void
		{
			if ( GameCommonData.Player.Role.Vit < 60 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bet_com_1" ] );   //你的精力不足60点，无法进行强制解除师徒关系
				return;
			}
			if ( GameCommonData.Player.Role.Ene < 60 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_mas_com_bet_com_2" ] );    //你的活力不足60点，无法进行强制解除师徒关系
				return;
			}
			RequestTutor.requestData( 22,youngStudent.id );
		}
		
		private function cancelClose():void{};
		
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
			main_mc.kickOut_btn.removeEventListener( MouseEvent.CLICK,kickOut );
		}
		
		private function onClick( evt:MouseEvent ):void
		{
			if ( clickFun != null )
			{
				clickFun( this );
			}
		}
		
		public function gc():void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE,addStageHandler );
			this.clickFun = null;
		}
		
		//计算离线时长
		private function getLeftTime( lastTime:uint ):String
		{
			if ( lastTime == 0 )
			{
				return "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_view_gra_get_1" ]+"</font>";            //在线
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
				str = outDays + GameCommonData.wordDic[ "mod_wis_med_wis_11" ] + outHour + GameCommonData.wordDic[ "mod_mas_view_gra_get_2" ];   //天              小时
			}
			return str;
		}
		
	}
}