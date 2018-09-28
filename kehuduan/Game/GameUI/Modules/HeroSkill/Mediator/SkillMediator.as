package GameUI.Modules.HeroSkill.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.HeroSkill.Command.SkillEvent;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.HeroSkill.View.SkillCell;
	import GameUI.Modules.HeroSkill.View.SkillLearnCell;
	import GameUI.Modules.HeroSkill.View.SkillLifeCell;
	import GameUI.Modules.HeroSkill.View.SkillPanelCell;
	import GameUI.Modules.HeroSkill.View.SkillUnityPanelCell;
	import GameUI.Modules.UnityNew.Net.RequestUnity;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.DropEvent;
	
	import OopsEngine.Role.RoleJob;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SkillMediator extends Mediator
	{
		public static const NAME:String = "SkillMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var aSkillCell:Array;
		private var cellContainer:Sprite;                               //cell容器
		private var type:String = "skillMediator";
		private var yellowFrame:MovieClip;
		
		//技能相关信息
		private var currentPage:int = 0;
		private var totalPage:int = 1;
		private var currentJob:int;
		private var currentRol:RoleJob;
		private var curCellIndex:int = -1;
		private var lastTime:int;
		
		private var normalJob:int = -3;
		private var mainJob:uint;
		private var viceJob:uint;
		private var aSkillObj:Array;																	//存放技能数据
		private var remark:String;
		private var aBtnName:Array = [ "normal_btn","main_btn","vice_btn","life_btn","unity_btn" ];
		private var currentBtn:String = "normal_btn";
		private var aCurrentObj:Array = [];
		
		public function SkillMediator()
		{
			super(NAME);
		}
		
		public function get skillView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWSKILLVIEW,
				EventList.CLOSESKILLVIEW,
				SkillConst.SKILLUP_SUC,
				SkillConst.REC_UNITY_SKILL_STUDLEV
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.SKILLVIEW});
					panelBase = new PanelBase(skillView, skillView.width+8, skillView.height+12);
					panelBase.name = "SkillView";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_her_med_skillM_han" ]);//"技  能"
					cellContainer = new Sprite();
					skillView.addChild( cellContainer );
				break;
				case EventList.SHOWSKILLVIEW:
					initData();
					showSkillView();
					initCell();
					initBtns();
				break;
				case EventList.CLOSESKILLVIEW:
					panelCloseHandler(null);
				break;		
				case SkillConst.SKILLUP_SUC:
					if ( dataProxy.SkillIsOpen )
					{
						var id:int = notification.getBody().skillID;
						var le:int = notification.getBody().skillLevel;
						skillUpHandler(le,id);
					}
				break;
				case SkillConst.REC_UNITY_SKILL_STUDLEV:
					if ( dataProxy.SkillIsOpen && currentBtn == "unity_btn" )
					{
						receiveUnityInfo();		
					}
				break;
			}
		}
		
		//请求帮派分堂数据 
		private function requestData():void
		{
			var newTime:int = getTimer();
			if ( lastTime == 0 )
			{
//				facade.sendNotification( SendActionCommand.SENDACTION , { type:221 } ); 
				RequestUnity.send( 306,0 );
				lastTime = newTime;
				return;
			}
			if ( newTime - lastTime > 700 )
			{
				RequestUnity.send( 306,0 );
//				facade.sendNotification( SendActionCommand.SENDACTION , { type:221 } );
				lastTime = newTime;
			}
		}
		
		private function receiveUnityInfo():void
		{
			if ( currentBtn == "unity_btn" )
			{
				for ( var i:uint=0; i<aSkillCell.length; i++ )
				{
					( aSkillCell[i] as SkillUnityPanelCell ).checkCanPromt();
				}
			}
		}
		
		private function showSkillView():void
		{
			skillView.content_txt.text = "";
			checkBtnState( currentBtn );
			skillView.page_txt.mouseEnabled = false;
			skillView.title_txt.mouseEnabled = false;
			skillView.content_txt.mouseEnabled = false;
			for ( var i:uint=0; i<aBtnName.length; i++ )
			{
				( skillView[ aBtnName[i] ] as MovieClip ).buttonMode = true;
			}
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase);
			
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillYellowFrame");
			yellowFrame.width = 142;
			yellowFrame.height = 39;
		}
		
		private function initData():void
		{
			//获取当前职业
			mainJob  = (GameCommonData.Player.Role.RoleList [0] as RoleJob).Job;
			viceJob   =  (GameCommonData.Player.Role.RoleList [1] as RoleJob).Job;
			aSkillCell = [];
			SkillData.aNormalSkillObj = [];
			SkillData.aMainSkillObj = [];
			SkillData.aViceSkillObj = [];
			SkillData.aUnitySkillObj = [];
			SkillData.aMainSkillId = [];
			SkillData.aViceSkillId = [];
			SkillData.aNormalSkillId = [];
			
			//获取信息
			for(var j:uint = 0;j < GameCommonData.Skills.length;j++)
			{
				var obj:Object = GameCommonData.SkillList[GameCommonData.Skills[j]];
				var skillObj:Object = new Object();
				skillObj.id = obj.SkillID;
				skillObj.needLevel = obj.NeedLevel;
				if ( obj.BookID==0 ) 
				{
					if ( obj.Job == normalJob )
					{
						SkillData.aNormalSkillObj.push( skillObj );
					}
					else if ( obj.Job == mainJob )
					{
						SkillData.aMainSkillObj.push( skillObj );
					}
					else if ( obj.Job == viceJob )
					{
						SkillData.aViceSkillObj.push( skillObj );
					}
					else if ( obj.Job == -6 )
					{
						SkillData.aUnitySkillObj.push( skillObj );
					}
				}
			}
			addHideSkillInfo(SkillData.aMainSkillObj);
			addHideSkillInfo(SkillData.aViceSkillObj);
			SkillData.aNormalSkillObj.sortOn("needLevel",Array.NUMERIC);
			SkillData.aMainSkillObj.sortOn("needLevel",Array.NUMERIC);
			SkillData.aViceSkillObj.sortOn("needLevel",Array.NUMERIC);
			SkillData.aUnitySkillObj.sortOn( "id",Array.NUMERIC );
		}
		
		private function dragItemHandler(evt:DropEvent):void
		{
			facade.sendNotification(EventList.DROPSKILLINQUICK, {target:evt.Data.target, source:evt.Data.source,type:evt.Data.type});
		}
		
		private function initCell():void
		{
			currentPage = 0;
//			createCell(normalJob,SkillData.aNormalSkillObj);
//			viewCurrentPage();
			showCurrentObj( currentBtn );
		}
		
		private function createCell(_job:int,skillObjArr:Array):void
		{
			this.aSkillObj = skillObjArr.concat();
			clearAcell();
			aSkillCell = [];

			for(var i:int = 0;i < skillObjArr.length;i++)
			{
				var skillCell:SkillCell = new SkillPanelCell(skillObjArr[i].id,_job);
				skillCell.addEventListener(DropEvent.SKILLITEMDRAGED,dragItemHandler);
				aSkillCell.push(skillCell);
			}
		}
		
		private function getSkillInfo(job:int):void
		{
			setTitle(job);
			aSkillObj = [];
			
			for(var j:uint = 0;j < GameCommonData.Skills.length;j++)
			{
				var obj:Object = GameCommonData.SkillList[GameCommonData.Skills[j]];
//				if((obj.Job == job) ) 
				if((obj.Job == job) && (obj.BookID==0)) 
				{
					var skillObj:Object = new Object();
					skillObj.id = obj.SkillID;
					skillObj.needLevel = obj.NeedLevel;
					aSkillObj.push(skillObj);
				}
			}
		}
		
		//添加隐藏技能
		private function addHideSkillInfo(skillObjArr:Array):void
		{
			if ( skillObjArr.length>0 )
			{
				//每个职业加5个隐藏技能
				for ( var k:uint=0; k<5; k++ )
				{
					var hideSkillObj:Object = new Object();
					hideSkillObj.id = 9510;
					hideSkillObj.needLevel = 1000;
					skillObjArr.push(hideSkillObj);
				}
			}
		}
		
		private function initBtns():void     
		{
			skillView.left_btn.addEventListener(MouseEvent.CLICK,goLeft);
			skillView.right_btn.addEventListener(MouseEvent.CLICK,goRight);
			for ( var i:uint=0; i<aBtnName.length; i++ )
			{
				( skillView[ aBtnName[i] ] as MovieClip ).addEventListener( MouseEvent.CLICK,clickBtns );
			}
		}
		
		private function clickBtns( evt:MouseEvent ):void
		{
			if ( evt.currentTarget.name == currentBtn ) return;
			showCurrentObj( evt.currentTarget.name );
		}
		
		//显示当前状态的数据
		private function showCurrentObj( _name:String ):void
		{
			currentPage = 0;
			curCellIndex = -1;
			skillView.content_txt.htmlText = "";
			checkBtnState( _name );
			switch ( _name )
			{
				case "normal_btn":
					createCell( normalJob,SkillData.aNormalSkillObj );
				break;
				case "main_btn":
					createCell( mainJob,SkillData.aMainSkillObj );
				break;
				case "vice_btn":
					createCell( viceJob,SkillData.aViceSkillObj );
				break;
				case "life_btn":
					getLifeSkillInfo();
					creatLifeCell();
				break;
				case "unity_btn":
					requestData();
					creatUnityCell();
				break;
			}
			viewCurrentPage();
		}
		
		private function viewCurrentPage():void
		{
			var len:int = aSkillCell.length;
			if(len > 0)
			{
				totalPage = Math.ceil(len/10);
			}else
			{
				totalPage = 1;
			}
			clearContainer();
			
			if(currentPage < totalPage-1)
			{
				for(var i:int = currentPage*10;i < (currentPage+1)*10;i++)
				{
					placeCell(i,aSkillCell[i]);
				}
			}
			
			if(currentPage == totalPage-1)
			{
				for(var j:int = currentPage*10;j < len;j++)
				{
					placeCell(j,aSkillCell[j]);
				}
			}
			
			skillView.page_txt.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + (currentPage+1) + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第"	"页"  
		}
		
		private function placeCell(i:int,skillCell:SkillCell):void
		{
			var col:int = i%2;
			var row:int = Math.floor((i-currentPage*10)/2);
			if(aSkillCell[i])
			{
				cellContainer.addChild(aSkillCell[i]);
				skillCell.x = 13.5 + 144 * col;
				skillCell.y = 45 + 40 * row;
				skillCell.addEventListener(SkillConst.CELLCLICK,onClickCell);
			}
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if(currentPage >= 1)
			{
				currentPage --;
			}
			viewCurrentPage();
		}
		
		private function goRight(evt:MouseEvent):void
		{
			if((currentPage+1) < totalPage)
			{
				currentPage ++;
			}
			viewCurrentPage();
		}
		
		private function clearContainer():void
		{
			var des:SkillCell;
			if(cellContainer && cellContainer.numChildren > 0)
			{
				for(var i:int = cellContainer.numChildren-1;i >= 0;i--)
				{
					des = cellContainer.removeChildAt(0) as SkillCell;
					des.gc();
					des = null;
				}
			}
		}
		
		//点击显示说明内容
		private function onClickCell(evt:SkillEvent):void
		{
			if (evt.currentTarget is SkillLearnCell)
			{
				return;
			}
			else if ( evt.currentTarget is SkillPanelCell )
			{
				var _id:int = (evt.currentTarget as SkillPanelCell).id;
				remark = (evt.currentTarget as SkillPanelCell).getRemark(evt.currentTarget.panCellLevel);
				skillView.content_txt.htmlText = remark;
				for (var i:uint=0; i<aSkillObj.length; i++)
				{
					if ( _id == aSkillObj[i].id )
					{
						curCellIndex = i;
						break;
					}
				}
			}
			else if ( evt.currentTarget is SkillLifeCell )
			{
				skillView.content_txt.htmlText = ( evt.currentTarget as SkillLifeCell ).getRemark(0);
			}
			if ( yellowFrame && !(evt.currentTarget as SkillCell).contains(yellowFrame) )
			{
				(evt.currentTarget as SkillCell).addChild(yellowFrame);
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			if(!dataProxy.SkillIsOpen)
			{
				return;
			}
			clearContainer();
			
			skillView.left_btn.removeEventListener(MouseEvent.CLICK,goLeft);
			skillView.right_btn.removeEventListener(MouseEvent.CLICK,goRight);
			
			for ( var i:uint=0; i<aBtnName.length; i++ )
			{
				( skillView[ aBtnName[i] ] as MovieClip ).removeEventListener( MouseEvent.CLICK,clickBtns );
			}
			
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			dataProxy.SkillIsOpen = false;
		}
		
		//技能升级成功
		private function skillUpHandler(newLevel:int,newId:int):void
		{
			refreshCell(newLevel,newId);
		}
		
		private function refreshCell(newLevel:int,newId:int):void
		{
			for ( var i:uint=0; i<aSkillObj.length; i++ )
			{
				if ( aSkillObj[i].id == newId )
				{
					(aSkillCell[i] as SkillPanelCell).skillUpDone(newLevel);
					if ( curCellIndex == i )
					{
						skillView.content_txt.htmlText = (aSkillCell[i] as SkillPanelCell).getRemark(newLevel);
					}
					return;
				}
			}
		}
		
			//根据职业显示标题文件
		private function setTitle(id:int):void
		{
			switch(id)
			{
				case 4096:
					skillView.title_txt.text = "";
					break;
					
				case 1:
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_1" ];//"唐门   火属性
					break; 
					
				case 2:
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_2" ];//"全真   冰属性"
					break;
					
				case 4:
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_3" ];//"峨眉   冰属性"
					break;
					
				case 8:
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_4" ];//"丐帮   毒属性"
					break;
					
				case 16:
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_5" ];//"少林   玄属性"
					break;
					
				case 32:
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_6" ];//"点苍   火属性"
					break;
				default:
					skillView.title_txt.text = "";
					break;
			}
		}
		
		//生活技能
		private function getLifeSkillInfo():void
		{
			this.aSkillObj = [];
			//所有技能信息
			for ( var sId:* in GameCommonData.LifeSkillList )
			{
				var lifeSkill:Object = GameCommonData.LifeSkillList[sId];
				var obj:Object = new Object();
				obj.id = lifeSkill.SkillID;
				obj.needLevel = lifeSkill.NeedLevel;
				this.aSkillObj.push(obj);
			}
			aSkillObj.sortOn("needLevel",Array.NUMERIC);
			aSkillObj.sortOn("id",Array.NUMERIC);
		}
		
		private function creatLifeCell():void
		{
			aSkillCell = [];
			currentPage = 0;
			clearAcell();
			
			for ( var i:int = 0; i < aSkillObj.length; i++ )
			{
				var col:int = i%2;
				var row:int = Math.floor(i/2);
				
				var skillCell:SkillCell = new SkillLifeCell(aSkillObj[i].id,1,1);
				skillCell.addEventListener(SkillConst.LIFECELL_CLICK,onClickCell);
				aSkillCell.push(skillCell);
			}
		}
		
		private function creatUnityCell():void
		{
			aSkillCell = [];
			currentPage = 0;
			clearAcell();
			
			for ( var i:int = 0; i < SkillData.aUnitySkillObj.length; i++ )
			{
				var col:int = i%2;
				var row:int = Math.floor(i/2);
				
				var skillCell:SkillCell = new SkillUnityPanelCell( SkillData.aUnitySkillObj[i].id );
				( skillCell as SkillUnityPanelCell ).clickUnityCellHandler = clickUnityCellHandler;
				aSkillCell.push(skillCell);
			}
		}
		
		private function clickUnityCellHandler( cell:SkillUnityPanelCell ):void
		{
			remark = cell.getRemark( cell.learnCellLevel );
			skillView.content_txt.htmlText = remark; 
			if ( yellowFrame && ! cell.contains(yellowFrame) )
			{
				cell.addChild(yellowFrame);
			}
		}
		
		private function checkBtnState( n:String ):void
		{
			for ( var i:uint=0; i<aBtnName.length; i++ )
			{
				if ( n== aBtnName[i] )
				{
					( skillView[ aBtnName[i] ] as MovieClip ).gotoAndStop( 1 );
					currentBtn = aBtnName[i];
				}
				else
				{
					( skillView[ aBtnName[i] ] as MovieClip ).gotoAndStop( 2 );
				}
			}
			switch ( currentBtn )
			{
				case "normal_btn":
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillM_che_1" ];//"普通技能";
				break;
				case "main_btn":
					setTitle( this.mainJob );
				break;
				case "vice_btn":
					setTitle( this.viceJob );
				break;
				case "life_btn":
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillM_che_2" ];//"生活采集";
				break;
				case "unity_btn":
					skillView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_learnU_initS" ];//"帮派技能";
				break;
			}
		}
		
		//清空aCell
		private function clearAcell():void
		{
			if(aSkillCell.length > 0)
			{
				for(var j:int = 0; j < aSkillCell.length;j++)
				{
					aSkillCell[j].removeEventListener(DropEvent.SKILLITEMDRAGED,dragItemHandler);
					aSkillCell[j].removeEventListener(SkillConst.CELLCLICK,onClickCell);
					aSkillCell[j].removeEventListener(SkillConst.LIFECELL_CLICK,onClickCell);
//					aSkillCell[j].gc();
					aSkillCell[j] = null;
				}
			}
		}
		
	}
}