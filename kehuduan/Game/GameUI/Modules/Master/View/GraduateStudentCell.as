package GameUI.Modules.Master.View
{
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Master.Proxy.YoungStudent;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;

	public class GraduateStudentCell extends Sprite
	{
		public var youngStudent:YoungStudent;
		public var clickFun:Function;
		
		private var dateNow:Date;
		private var main_mc:MovieClip;
		
		public function GraduateStudentCell( _young:YoungStudent )
		{
			youngStudent = _young;
			this.addEventListener( Event.ADDED_TO_STAGE,addStageHandler );
		}
		
		private function initUI():void
		{
			main_mc = new ( MasterData.masResPro.graduateCellClass ) as MovieClip;
			addChild( main_mc );
			
			main_mc.txt_0.text = youngStudent.name;
			main_mc.txt_1.text = youngStudent.sexStr;
			main_mc.txt_2.text = youngStudent.roleLevel;
			main_mc.txt_3.text = GameCommonData.RolesListDic[ youngStudent.job ].toString();
			main_mc.txt_4.text = youngStudent.batNum;
			main_mc.txt_5.htmlText = youngStudent.vipStr;
			main_mc.txt_6.htmlText = getLeftTime( youngStudent.outLineTime );
			for ( var i:uint=0; i<8; i++ )
			{
				main_mc[ "txt_"+i ].mouseEnabled = false;
			}
			main_mc.gainAward_txt.mouseEnabled = false;
			analysisCanGain();
			main_mc.gainAward_btn.addEventListener( MouseEvent.CLICK,gainAward );
		}
		
		//获取奖励
		private function gainAward( evt:MouseEvent ):void
		{
			RequestTutor.requestData( 29,youngStudent.id );
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
				return "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_mas_view_gra_get_1" ]+"</font>";     //在线
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
						str = "1"+GameCommonData.wordDic[ "mod_too_med_ui_ski_setc_7" ];     //1分钟
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
		
		//计算是否可领奖
		private function analysisCanGain():void
		{
			if ( youngStudent.roleLevel < 60 )
			{
				setBtnVis( false );
			}
			else
			{
				var nIndex:int;
				if(youngStudent.roleLevel > 49)
				{
					nIndex = Math.floor( ( youngStudent.roleLevel - youngStudent.roleLevel%10 - 15 ) / 5 );	
				}
				else
				{
					nIndex = Math.floor( ( youngStudent.roleLevel - 15 ) / 5 );
				}
				//越界
				if ( nIndex >= 27 )
				{
					setBtnVis( false );
					setBtnText( GameCommonData.wordDic[ "mod_mas_view_gra_ana_1" ] );              //奖励全部领取
				}
				if ( ( youngStudent.impart & (1<<nIndex) ) == 0 )
				{
					setBtnVis( true );
				}
				else
				{
					setBtnVis( false );
					var needLevel:uint = Math.floor( youngStudent.roleLevel/10 ) * 10 + 10;
					var info:String = GameCommonData.wordDic[ "mod_mas_view_gra_ana_2" ] + needLevel + GameCommonData.wordDic[ "mod_mas_view_gra_ana_3" ];         //达到              级可领取
					setBtnText( info );
				}
			}
		}
		
		private function setBtnText( str:String ):void
		{
			main_mc.txt_7.text = str;
		}
		
		//设置按钮的是否可见
		private function setBtnVis( isSee:Boolean ):void
		{
			main_mc.gainAward_btn.visible = isSee;
			main_mc.gainAward_txt.visible = isSee;
		}
		
	}
}