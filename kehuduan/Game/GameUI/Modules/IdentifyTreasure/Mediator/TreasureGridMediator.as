package GameUI.Modules.IdentifyTreasure.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	import GameUI.Modules.IdentifyTreasure.UI.ChatArea;
	import GameUI.Modules.IdentifyTreasure.UI.StaticGridGroup;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	//拆分出来的mediator，主要处理开箱子上的格子和右边文本
	public class TreasureGridMediator extends Mediator
	{
		public static const NAME:String = "TreasureGridMediator";
		private var leftGridGroup:StaticGridGroup;
		private var rightGridGroup:StaticGridGroup;
		private var main_mc:MovieClip;
		
		private var upChatArea:ChatArea;
		private var downChatArea:ChatArea;
		private var reg:RegExp = /(<\d_.*?>)/g;
		
		public function TreasureGridMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							TreasureData.CREATE_TREA_GRID,
							TreasureData.CHG_TREA_GRID_DATA,
							TreasureData.SHOW_HIGHT_HAND_GIVES,
//							TreasureData.SHOW_TREA_AWARD_PANEL,
							TreasureData.SHOW_RIVERS_HEARD
			
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case TreasureData.CREATE_TREA_GRID:
					creatGrid();
				break;
				case TreasureData.CHG_TREA_GRID_DATA:
					chgGridDate( notification.getBody() );
				break;
				case TreasureData.SHOW_HIGHT_HAND_GIVES:
					showHightHands( notification.getBody() );
				break;
//				case TreasureData.SHOW_TREA_AWARD_PANEL:
//					showHightHands( notification.getBody() );
//				break;
				case TreasureData.SHOW_RIVERS_HEARD:
					showRiversHeard( notification.getBody() );
				break;
			}
		}
		
		//创建格子
		private function creatGrid():void
		{
			main_mc = this.viewComponent as MovieClip;
			
			leftGridGroup = new StaticGridGroup( TreasureData.TreasureGrid,3,3 );
			leftGridGroup.colDis = 5;
			leftGridGroup.rowDis = 15;
			leftGridGroup.placeGrids();
			leftGridGroup.x = 23;
			leftGridGroup.y = 123;
						
			rightGridGroup = new StaticGridGroup( TreasureData.TreasureGrid,3,3 );
			rightGridGroup.colDis = 5;
			rightGridGroup.rowDis = 15;
			rightGridGroup.placeGrids();
			rightGridGroup.x = 333;
			rightGridGroup.y = 123;
			
			main_mc.addChild( leftGridGroup );
			main_mc.addChild( rightGridGroup );
			
			upChatArea = new ChatArea( 190,160 );
			upChatArea.x = 498;
			upChatArea.y = 43;
			
			downChatArea = new ChatArea( 190,168 );
			downChatArea.x = 498;
			downChatArea.y = 250;
			
			main_mc.addChild( upChatArea );
			main_mc.addChild( downChatArea );
		}
		
		//更改格子数据
		private function chgGridDate( obj:Object ):void
		{
			var curAward:Array;
			switch ( obj.curBtn )
			{
				case 0:
					curAward = TreasureData.aJiantuAward;
				break;
				case 1:
					curAward = TreasureData.aJiankeAward;
				break;
				case 2:
					curAward = TreasureData.aJianhaoAward;
				break;
			}	
			leftGridGroup.dataGrid = curAward[0];
			rightGridGroup.dataGrid = curAward[1];
		}
		
		//高手馈赠
		private function showHightHands( obj:Object ):void
		{
			var aAward:Array = obj.aAward;
			var dataObj:Object = new Object();
			var curStr:String = getHeroName( obj.curBtn );

			for ( var i:uint=0; i<aAward.length; i++ )
			{
				var id:int = aAward[i].id;
				var type:int = aAward[i].type;
				var num:int = aAward[i].num;
				dataObj = UIConstData.getItem( type );
				if ( !dataObj ) continue;
				var name:String = dataObj.Name;
				var color:uint = dataObj.Color;
//				trace ( "颜色是什么："+ ChatData.ITEM_COLORS[ color ] ); IntroConst.itemColors
				var str:String = '<font color="#ffff99"><a href="event:type_'+type+'">[<font color="'+IntroConst.itemColors[ color ].toString()+'">'+name+'</font>]</a></font>';
				var info:String = "<font color='#ffff99'>"+GameCommonData.wordDic[ "mod_ide_med_treasureG_showH_1" ]+curStr+GameCommonData.wordDic[ "mod_ide_med_treasureG_showH_2" ]+str+"<font color='#00ff00'>×"+num+"</font></font>";//"你与"		"一较高低，他送给你"	
//				var info:String = "他送你一个"+str;
				upChatArea.show( info );
			}
		}
		
		//江湖惊闻
		private function showRiversHeard( obj:Object ):void
		{
			if ( downChatArea == null )
			{
				return;
			} 								
			var nameInfo:String = obj.talkObj[3];
			var type:int = obj.nItemTypeID;
			var pName:String
			var infoArr:Array = nameInfo.split( reg );
			for ( var i:uint=0; i<infoArr.length; i++ )
			{
				if ( reg.test( infoArr[i] ) )
				{
					var signArr:Array = infoArr[i].split( "_" );
					if ( signArr[0] == "<0" )
					{
						pName = signArr[1];
					}
				}
			}
			if ( pName && pName.length>0 )
			{
				pName = pName.slice( 1,pName.length-1 );
			}
			
			//人名信息
//			var pName:String = nameInfo.split( "_" )[0];
			var heroName:String = getHeroName( obj.nColor - 1 );
			var pNameInfo:String;
			if ( pName == GameCommonData.Player.Role.Name )
			{
				pNameInfo = "<font color='#ffff99'>"+GameCommonData.wordDic[ "mod_chat_com_rec_mak_4" ]+"</font>";//你
			}
			else 
			{
				pNameInfo = '<font color="#ffff99"><a href="event:name_'+pName+'">[<font color="#00ff00">'+pName+'</font>]</a></font>';
//				pNameInfo = '<font color="#ffff99"><a href="event:name_'+pName+'"><font color="#00ff00">'+pName+'</font></a></font>';
			}
			
			var dataObj:Object = new Object;
			dataObj = UIConstData.getItem( type );
			if ( !dataObj ) return;
			var itemName:String = dataObj.Name;
			var color:uint = dataObj.Color;
			var typeStr:String = '<font color="#ffff99"><a href="event:type_'+type+'">[<font color="'+IntroConst.itemColors[ color ].toString()+'">'+itemName+'</font>]</a></font>';
			var totalInfo:String = '<font color="#ffff99">'+pNameInfo+GameCommonData.wordDic[ "mod_ide_med_treasureG_showR_1" ]+heroName+"，"+GameCommonData.wordDic[ "mod_ide_med_treasureG_showR_2" ]+typeStr+"</font>" ;//"挑战"	"获得世外高人赠予的"
			downChatArea.show( totalInfo );
		}
		
		private function getHeroName( index:uint ):String
		{
			var s:String = "";
			switch ( index )
			{
				case 0:
					s = "<font color='#ff9900'>"+GameCommonData.wordDic[ "mod_chat_med_msg_getL_1" ]+"</font>";//剑徒
				break;
				case 1:
					s = "<font color='#ff9900'>"+GameCommonData.wordDic[ "mod_chat_med_msg_getL_2" ]+"</font>";//剑客
				break;
				case 2:
					s = "<font color='#ff9900'>"+GameCommonData.wordDic[ "mod_chat_med_msg_getL_3" ]+"</font>";//剑豪
				break;
			}
			return s;
		}
		
	}
}