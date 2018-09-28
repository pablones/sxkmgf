package GameUI.Modules.NewerHelp.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.AutoPlay.mediator.AutoPlayMediator;
	import GameUI.Modules.Bag.Datas.BagEvents;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.MainSence.Mediator.MainSenceMediator;
	import GameUI.Modules.MainSence.Proxy.QuickGridManager;
	import GameUI.Modules.Map.SenceMap.SenceMapMediator;
	import GameUI.Modules.NPCChat.Command.NPCChatComList;
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.Modules.NPCShop.Data.NPCShopEvent;
	import GameUI.Modules.NPCShop.Mediator.NPCShopMediator;
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardEvent;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.NewerHelp.UI.NewerHelpForFacebookItem;
	import GameUI.Modules.NewerHelp.UI.NewerHelpGrid;
	import GameUI.Modules.NewerHelp.UI.NewerHelpItem;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PlayerInfo.Mediator.SelfInfoMediator;
	import GameUI.Modules.RoleProperty.Mediator.EquipMediator;
	import GameUI.Modules.Task.Commamd.MoveToCommon;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Mediator.TaskFollowMediator;
	import GameUI.Modules.Task.Mediator.TaskMediator;
	import GameUI.Modules.Task.Model.TaskGroupStruct;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class NewerHelpUIMediaror extends Mediator
	{
		public static const NAME:String = "NewerHelpUIMediaror";
		
		private var _itemView_0:DisplayObject = null;
		private var _itemView_1:DisplayObject = null;
		private var _gridView_0:DisplayObject = null;
		private var _gridView_1:DisplayObject = null;
		private var _gridView_2:DisplayObject = null;
		private var _itemView_3:DisplayObject = null;
		private var _gridView_3:DisplayObject = null;
		private var _curArr:Array = [];
		private var timer:Timer;
		private var specialBool:Boolean = true;   //杀兔子任务专用
		private var toggle:Boolean = true;
		private var isFirst:Boolean = true;
		private var isBoolean:Boolean = true;
		private var roleLevel:Boolean = true;
		private var petLevel:Boolean = true;
		private var point:Point;
		private var obj:Object;
		private var petBag:DisplayObject;
		private var panelBase:Sprite;
		private var apMed:AutoPlayMediator;
		private var showArr:Array = new Array();          //{item:***, startPoint:***, state:***}
		private var showItem3Arr:Array = new Array();     //{item:***, startPoint:***, state:***}
		private var showOtherArr:Array = new Array();
		private var specialArr:Array = new Array();
		private var dataProxy:DataProxy;
		private var _x:Number;        
		private var _y:Number;
		
		/** 经验泡泡 */
		private var _itemExp:DisplayObject = null;
		/** 是否清除过经验泡泡 每次上线只清理一次 清除过则不再显示 */
		public var curExpLevel:int = 0;
		
		/** 新手帮助MC数组 0-好友，1-宠物，2-装备 */ 
		private var _mcShowArr:Array = [];
		/** 加载模块 */
		private var _mcLoader:ImageItem;
		/** 当前加载的模块 */
		private var _curLoadIndex:uint = 0;
		/** 标题数组 */
		private static const MC_TITLE:Array = [GameCommonData.wordDic[ "mod_new_med_newerHelpU_MC_TITLE_1" ], GameCommonData.wordDic[ "mod_new_med_newerHelpU_MC_TITLE_2" ],GameCommonData.wordDic[ "mod_new_med_newerHelpU_MC_TITLE_3" ], GameCommonData.wordDic[ "mod_new_med_newerHelpU_MC_TITLE_4" ]];
//		private static const MC_TITLE:Array = ["添加好友说明", "宠物玩法说明", "装备玩法说明", "技能学习说明"];
		/** 位置增量 */
		private static const MC_SIZE_INCREMENT:Array = [{w:8, h:12}, {w:8, h:12}, {w:8, h:12}, {w:8, h:12}];
		
		public function NewerHelpUIMediaror()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.STAGECHANGE
					];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch( notification.getName() )
			{
				case EventList.STAGECHANGE:
					changeUI();
					break;
			}
		}
		
		private function changeUI():void
		{
			var disObj:DisplayObject;
			if( showArr && showArr.length>0 )
			{
				for(var i:uint; i<showArr.length; i++)
				{
					disObj = showArr[i].item as DisplayObject;
					switch(showArr[0].state)
					{
						case 1:   //只变动x
//							if( GameCommonData.GameInstance.GameUI.stage.stageWidth >= GameConfigData.GameWidth )
//							{
								disObj.x = (showArr[i].startPoint as Point).x + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
//							}
							break;
						case 2:   //只变动y
//							if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
//							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.y = (showArr[i].startPoint as Point).y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
//							}
							break;
						case 3:    //居中处理 或 x和y只移动一半的情况
//							if( GameCommonData.GameInstance.GameUI.stage.stageWidth >= GameConfigData.GameWidth )
//							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.x = (showArr[i].startPoint as Point).x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
//							}
//							if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
//							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.y = (showArr[i].startPoint as Point).y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
//							}
						 	break;
						 case 4:   //x,y都变动（非居中）
//						 	if( GameCommonData.GameInstance.GameUI.stage.stageWidth >= GameConfigData.GameWidth )
//							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.x = (showArr[i].startPoint as Point).x + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
//							}
//							if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
//							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.y = (showArr[i].startPoint as Point).y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
//							}
						 	break;
						 case 5:   //处理特殊情况
						 	if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
							{
								disObj.x = (showArr[i].startPoint as Point).x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2 - 12;
							}
//							if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
//							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.y = (showArr[i].startPoint as Point).y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
//							}
							if( GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth )
							{
//								disObj = showArr[i].item as DisplayObject;
								disObj.x = (showArr[i].startPoint as Point).x + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
							}
						 	break;
						case 6:    //处理特殊情况
							if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
							{
								disObj.y = (showArr[i].startPoint as Point).y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2 + 5;
							}
						 	break;
						case 7:    //处理特殊情况
							disObj.x = (showArr[i].startPoint as Point).x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
							disObj.y = (showArr[i].startPoint as Point).y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/4;
						 	break;
					}
				}
			}
			
			if( specialArr && specialArr.length>0 )
			{
				for(var j:uint; j<specialArr.length; j++)
				{
					disObj = specialArr[j].item as DisplayObject;
					var p:Point = findItemPos(320001);
					if(p) {
						var _gridView:NewerHelpGrid;
						if( disObj is NewerHelpGrid )
						{
							_gridView = disObj as NewerHelpGrid;
							_gridView.x = p.x -7;
							_gridView.y = p.y -7;
						}
						if( disObj is NewerHelpItem )
						{
							disObj.x = _gridView.x - 154;
							disObj.y = _gridView.y - 56;
						}
					}
				}
			}
			
			if( _itemExp && GameCommonData.GameInstance.TooltipLayer.contains(_itemExp) )
			{
				if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
				{
					_itemExp.x = 812 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2 - 10;
				}
//				if( GameCommonData.GameInstance.GameUI.stage.stageHeight >= GameConfigData.GameHeight )
//				{
					_itemExp.y = 478 + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
//				}
				if( GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth )
				{
					_itemExp.x = 812 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
				}
			}
		}
		
		private function getStartPoint( x:Number, y:Number, type:uint ):Point
		{
			var point:Point;
			switch( type )
			{
				case 1:    //居中处理 或 x和y只移动一半的情况
					x -= (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					y -= (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					break;
				case 2:    //只变动x
					x -= GameCommonData.GameInstance.MainStage.stageWidth - GameConfigData.GameWidth;
					break;
				case 3:    //特殊处理
					x -= (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2 - 12;
					y -= GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
					break;
			}
			point = new Point(x, y);
			return point;
		}
		
		public function clearView():void
		{
			//////测试
			/*  if(GameCommonData.wordVersion == 2)
			{
				
				if(!SelfInfoMediator.isShowFBNewerHelp)
				{
					SelfInfoMediator.isShowFBNewerHelp = 2;
					facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:acceptFBTask, info:"<font color='#ffffff'>  馬上把facebook帳號和你的遊戲帳號綁定，讓你的facebook好友都知道你在玩<font color='#00ff00'>『御劍江湖』</font>，邀請他們一同加入捍衛武林的江湖行列！<br/>  綁定帳號可獲得以下道具：<font color='#00ff00'>寵物波浪鼓</font>*5、<font color='#00ff00'>小飛鞋</font>x10、<font color='#00ff00'>小喇叭</font>x10、<font color='#00ff00'>玉液瓶</font>x2</font>",title:"fb绑定",comfirmTxt:"我知道了" });
				}
			}  */        
			//////
			if( !dataProxy )  dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			if( dataProxy.TaskFollowUIIsFolded ) sendNotification( TaskCommandList.SHOW_TASKFOLLOW_UI );
			else if( dataProxy.TaskAccIsOpen )  sendNotification( TaskCommandList.SELECT_TASKFOLLOW_PAGE );
			showArr = new Array();
			specialArr = new Array();
			if( this.showOtherArr.length>0 )
			{
				var displayObject:DisplayObject;
				for( var j:uint=0; j<showOtherArr.length; j++ )
				{
					displayObject = showOtherArr[j].item as DisplayObject;
					if( GameCommonData.GameInstance.TooltipLayer.contains( displayObject ) )  showArr.push( showOtherArr[j] );
				}
			}
			if(_itemView_0 && GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer && GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.contains(_itemView_0)) {
				GameCommonData.GameInstance.GameScene.GetGameScene.TopLayer.removeChild(_itemView_0);
				_itemView_0 = null;
			} else if(_itemView_0 && GameCommonData.GameInstance.TooltipLayer.contains(_itemView_0)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_itemView_0);
				_itemView_0 = null;
			}
			if(_itemView_1 && GameCommonData.GameInstance.TooltipLayer.contains(_itemView_1)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_itemView_1);
				_itemView_1 = null;
			}
			if(_gridView_0 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_0)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_0);
				_gridView_0 = null;
			}
			if(_gridView_1 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_1)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_1);
				_gridView_1 = null;
			}
			if(_gridView_2 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_2)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_2);
				_gridView_2 = null;
			}
			if(_mcShowArr.length > 0) {
				for(var i:int; i < _mcShowArr.length; i++) {
					if(_mcShowArr[i] && GameCommonData.GameInstance.TooltipLayer.contains(_mcShowArr[i])) {
						GameCommonData.GameInstance.TooltipLayer.removeChild(_mcShowArr[i]);
					}
				}
			}
		}
		
		public function clear_View():void
		{
			if( dataProxy.TaskFollowUIIsFolded ) sendNotification( TaskCommandList.SHOW_TASKFOLLOW_UI );
			else if( dataProxy.TaskAccIsOpen )  sendNotification( TaskCommandList.SELECT_TASKFOLLOW_PAGE );
			showArr = new Array();
			specialArr = new Array();
			if( this.showOtherArr.length>0 )
			{
				var displayObject:DisplayObject;
				for( var j:uint=0; j<showOtherArr.length; j++ )
				{
					displayObject = showOtherArr[j].item as DisplayObject;
					if( GameCommonData.GameInstance.TooltipLayer.contains( displayObject ) )  showArr.push( showOtherArr[j] );
				}
			}
			if(_itemView_3 && GameCommonData.GameInstance.TooltipLayer.contains(_itemView_3)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_itemView_3);
				_itemView_3 = null;
			}
			if(_gridView_3 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_3)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_3);
				_gridView_3 = null;
			}
		}
		
		/** 清除经验泡泡 */
		public function clearItemExp():void
		{
			if(_itemExp && GameCommonData.GameInstance.TooltipLayer.contains(_itemExp)) {
				GameCommonData.GameInstance.TooltipLayer.removeChild(_itemExp);
				_itemExp = null;
			}
		}
		
		/** 显示经验泡泡 */
		public function showItemExp():void
		{
			if(_itemExp == null) {
				var mainSence:MovieClip = ( facade.retrieveMediator( MainSenceMediator.NAME ) as MainSenceMediator ).mainSence;
				this.point = mainSence.localToGlobal( new Point( mainSence["mcExp"].x, mainSence["mcExp"].y ) );
				_itemExp = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_sho" ], -1);//"经验已满，可在\n人物界面内手动升级"
//				_itemExp.x = 812;
//				_itemExp.y = 478;
				_itemExp.x = point.x + 449.5;
				_itemExp.y = point.y - 58.2;
				this.point = null;
				GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemExp,0);
			}
		}
		
		/** 任务1 - 屠大勇拿刀 */
		public function doTask_1():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
//					sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
//					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_1" ]);//"你获得了侠客剑\n点击这里打开背包"
//					_itemView_0.x = 642;
//					_itemView_0.y = 472;
//					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification( EventList.SHOWBAG );
				break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_2" ]);//"双击图标穿上装备"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(142000);
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 3:
					if( !GameCommonData.GameInstance.GameUI.getChildByName("Bag") ) return;
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName("Bag") as Sprite;
				    this.point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(panelBase.x, panelBase.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_3" ]);//"点击关闭背包"
//					_itemView_0.x = 650;
//					_itemView_0.y = 10;
					_itemView_0.x = point.x + 64;
					_itemView_0.y = point.y - 45;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(650, 10), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					this.point = null;
					NewerHelpData.point = null;
					break;
				case 4:
		 			MoveToCommon.MoveTo(1001, 77, 88, 302, 116);	                //自动寻路去找洛神
		 			_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_4" ],0,-1);//"点击这里自动寻路\n回去交任务"
					
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(302);
		
					_gridView_0 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 107; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					 
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
//					obj = NewerHelpData.changeId( 1001, 116 );
//					MoveToCommon.sendFlyToCommand({mapId:obj._mapId,tileX:78,tileY:92,taskId:302,npcId:obj._npcId});	// 使用小飞鞋去找洛神
//					obj = null;
//					
//					timer = new Timer(500, 1);
//					timer.addEventListener(TimerEvent.TIMER, TimeFUN);
//					timer.start()
					break;
				case 5:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//禁用任务栏拖动
					break;
			}
		}
		
		private function TimeFUN(event:TimerEvent):void
		{
			timer.removeEventListener(TimerEvent.TIMER, TimeFUN);
			timer = null;	
			_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_4" ],0,-1);//"点击这里自动寻路\n回去交任务"
					
			var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
			var rect:Rectangle = taskFollowMed.getTaskRectangle(302);

			_gridView_0 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
			_gridView_0.x = rect.x + 107; 
			_gridView_0.y = rect.y + 13; 
			
			_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
			_itemView_0.y = _gridView_0.y + _gridView_0.height;
			
			this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
			this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
			GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
			GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
			 
			sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
		}
		
		/** 任务2 - 干掉四只兔子 */
		public function doTask_2():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
				    if(toggle)
				    {
				    	toggle = false;
				    	NewerHelpData.CAN_FLY = true;
				    	clear_View();
				    	_itemView_3 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_4" ],0,-1);//"点击这里自动寻路\n回去交任务"
					
						var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
						var rect:Rectangle = taskFollowMed.getTaskRectangle(303);
	
						_gridView_3 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
						_gridView_3.x = rect.x + 107; 
						_gridView_3.y = rect.y + 13; 
						
						_itemView_3.x = _gridView_3.x - NewerHelpData.VIEW_WIDTH;
						_itemView_3.y = _gridView_3.y + _gridView_3.height;
						
						this.showArr.push( {item:_gridView_3, startPoint:getStartPoint(_gridView_3.x, _gridView_3.y, 2), state:1} );
						this.showArr.push( {item:_itemView_3, startPoint:getStartPoint(_itemView_3.x, _itemView_3.y, 2), state:1} );
						this.showOtherArr.push( {item:_gridView_3, startPoint:getStartPoint(_gridView_3.x, _gridView_3.y, 2), state:1} );
						this.showOtherArr.push( {item:_itemView_3, startPoint:getStartPoint(_itemView_3.x, _itemView_3.y, 2), state:1} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_3,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_3,0);
						sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
				    }
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		/** 任务3 - 获得狭义袍 */
		public function doTask_3():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
				    if(isFirst)
				    {
				    	isFirst = false;
				    	clear_View();
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT3_1" ]);//"你获得了侠义袍\n点击这里打开背包"
//						_itemView_0.x = 642;
//						_itemView_0.y = 472;
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
						
					    this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					    NewerHelpData.point = null;
					}
					break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_2" ]);//"双击图标穿上装备"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(120000);
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 3:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT3_2" ],0,-1);//"点击这里\n领取第一只宠物"
					
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(304);
					if( !rect ) return;
					_gridView_0 = new NewerHelpGrid(57, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 24; 
					_gridView_0.y = rect.y + 13; 
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(EventList.CLOSEBAG);
					(facade.retrieveProxy(DataProxy.NAME) as DataProxy).BagIsOpen = false;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
				case 4:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务4 - 获得第一只宠物 */
		public function doTask_4():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
				    if(isFirst)
				    {
				    	isFirst = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 2);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT4_1" ]);//"你获得了一只宠物\n点击这里打开宠物背包"
//						_itemView_0.x = 578;
//						_itemView_0.y = 472;
					    _itemView_0.x = point.x - 153.05;
					    _itemView_0.y = point.y - 63.55;
					    
					    this.showArr.push( {item:_itemView_0, startPoint:new Point(578, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					    NewerHelpData.point = null;
					}
					break;
				case 2:
				    petBag = GameCommonData.GameInstance.GameUI.getChildByName("PetBag");
				    if( !petBag ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(petBag.x, petBag.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT4_2" ]);//"点击出战召唤宠物"
					if(PetPropConstData.isNewPetVersion)	//新版宠物
					{
						_gridView_0 = new NewerHelpGrid(42, 24);
						_gridView_0.x = point.x + 194;
						_gridView_0.y = point.y + 411;
						_itemView_0.x = point.x + 42;
						_itemView_0.y = point.y + 359;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(274, 469), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(122, 417), state:3} );
					}
					else
					{
						_gridView_0 = new NewerHelpGrid(42, 26);
						_gridView_0.x = point.x + 194;
						_gridView_0.y = point.y + 209;
						_itemView_0.x = point.x + 42;
						_itemView_0.y = point.y + 157;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(276, 267), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(122, 215), state:3} );
					}
						
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(PetEvent.PETPROP_PANEL_STOP_DROG);		//禁止宠物面板拖动
					break;
				case 3:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_4" ],0,-1);//"点击这里自动寻路\n回去交任务"
					
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(305);

					_gridView_0 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 107; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 4:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//禁用任务栏拖动
					break;
			}
		}
		
		/** 任务5 - 获得侠义冠 */
		public function doTask_5():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					if(isFirst)
				    {
				    	isFirst = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT5" ]);//"你获得了侠义冠\n点击这里打开背包"
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
					    
					    this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					}
					break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_2" ]);//"双击图标穿上装备"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(130016);
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务6 - 宠物第一次升级加点 */
		public function doTask_6():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					if(isFirst)
				    {
				    	isFirst = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 2);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT6_1" ]);//"宠物获得了潜能点\n点击这里开始分配"
//						_itemView_0.x = 578;
//						_itemView_0.y = 472;
					    _itemView_0.x = point.x - 153.05;
					    _itemView_0.y = point.y - 63.55;
					    
					    this.showArr.push( {item:_itemView_0, startPoint:new Point(578, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					}
					break;
				case 2:
					petBag = GameCommonData.GameInstance.GameUI.getChildByName("PetBag");
				    if( !petBag ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(petBag.x, petBag.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT6_2" ],-1,0,false,null,0,0,95);//"点击分配潜能点：\n兔子推荐加力量    \n狸猫推荐加力量    \n松鼠推荐加灵力    \n分配完成后点击确定"
					if(PetPropConstData.isNewPetVersion)	//新版宠物
					{
						_gridView_0 = new NewerHelpGrid(177, 115);
						_gridView_0.x = point.x + 236;
						_gridView_0.y = point.y + 52;
						_itemView_0.x = point.x + 570;
						_itemView_0.y = point.y + 8;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(316, 110), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(650, 66), state:3} );
					}
					else
					{
						_gridView_0 = new NewerHelpGrid(153, 111);
						_gridView_0.x = point.x + 236;
						_gridView_0.y = point.y + 46;
						_itemView_0.x = point.x + 546;
						_itemView_0.y = point.y + 2;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(316, 104), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(626, 60), state:3} );
					}
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(PetEvent.PETPROP_PANEL_STOP_DROG);		//禁止宠物面板拖动
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务7 - 获得宠物药 */
		public function doTask_7():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					if(isFirst)
				    {
				    	isFirst = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT7_1" ]);//"你获得了宠物药品\n点击这里打开背包"
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
					    this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					}
					break;
				case 2:
					var quickGridManager:QuickGridManager = facade.retrieveProxy(QuickGridManager.NAME) as QuickGridManager;
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT7_2" ]);//"将宠物药拖到快捷栏\n方便使用"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(320001);
					var pQuick:Point = quickGridManager.getEmptyTilePos();
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						if(pQuick) {
							_gridView_1 = new NewerHelpGrid();
							_gridView_1.x = pQuick.x - 7;
							_gridView_1.y = pQuick.y - 10;
							GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_1,0);
							this.showArr.push( {item:_gridView_1, startPoint:new Point(465.85, 534), state:5} );
						}
						this.specialArr.push( {item:_gridView_0} );
						this.specialArr.push( {item:_itemView_0} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
//						sendNotification(BagEvents.BAG_INIT_POS);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
						
					}
					if(GameCommonData.wordVersion == 2)
					{
						if(SelfInfoMediator.isFBOpen)
						{
							if(!(facade.retrieveMediator(SelfInfoMediator.NAME) as SelfInfoMediator).isBindFacebook)
							{
								if(!SelfInfoMediator.isShowFBNewerHelp)
								{
									SelfInfoMediator.isShowFBNewerHelp = 2;
//									facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:acceptFBTask, info:"<font color='#ffffff'>  馬上把facebook帳號和你的遊戲帳號綁定，讓你的facebook好友都知道你在玩<font color='#00ff00'>『御劍江湖』</font>，邀請他們一同加入捍衛武林的江湖行列！<br/>  綁定帳號可獲得以下道具：<font color='#00ff00'>寵物波浪鼓</font>*5、<font color='#00ff00'>小飛鞋</font>x10、<font color='#00ff00'>小喇叭</font>x10、<font color='#00ff00'>玉液瓶</font>x2</font>",title:"fb绑定",comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ] });
									facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:acceptFBTask, info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_1" ],title:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_2" ],comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ] });
								}
							}
						}
					}
					break;
				case 3: 
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break; 
			}
		}
		private function acceptFBTask():void
		{
			sendNotification(TaskCommandList.SHOW_TASKINFO_UI,{id:4401});	//接受fb任务
		}
		/**台服特有，fb绑定任务新手提示*/
		public function knownForFbBinded():void
		{
			 if(GameCommonData.wordVersion == 2)
			{
				var fbView:DisplayObject = GameCommonData.GameInstance.TooltipLayer.getChildByName("fbItemView");
				if(fbView)
				{
					(fbView as NewerHelpForFacebookItem).dispose();
					fbView = null;
				}
				var fbItemView:DisplayObject = new NewerHelpForFacebookItem(GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_3" ],1.3,1.1,true,iKnowCallBack);
				fbItemView.name = "fbItemView";
				var selfInfo:SelfInfoMediator = facade.retrieveMediator(SelfInfoMediator.NAME) as SelfInfoMediator;
				var fbButton:DisplayObject = selfInfo.SelfInfoUI.getChildByName("facebook_icon");
				if(!fbButton)
				{
					throw new Error(GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_4" ]);
					return;
				}
				fbItemView.x = fbButton.x + fbButton.width - 2;
				fbItemView.y = fbButton.y + fbButton.height - 2;
				GameCommonData.GameInstance.TooltipLayer.addChildAt(fbItemView,0);
				
//				sendNotification(TaskCommandList.RECALL_TASK,{taskID:4401,type:241});
			} 
		}
		
		private function iKnowCallBack():void
		{
			/* var isUseFBBtn:int = (facade.retrieveMediator(SelfInfoMediator.NAME) as SelfInfoMediator).isShowFBNewerHelp;
			if(isUseFBBtn >= 2)	
			{
				return;
			} */
//			facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:FbBindedSend, info:"<font color='#ffffff'>  馬上把facebook帳號和你的遊戲帳號綁定，讓你的facebook好友都知道你在玩<font color='#00ff00'>『御劍江湖』</font>，邀請他們一同加入捍衛武林的江湖行列！<br/>  綁定帳號可獲得以下道具：<font color='#00ff00'>寵物波浪鼓</font>*5、<font color='#00ff00'>小飛鞋</font>x10、<font color='#00ff00'>小喇叭</font>x10、<font color='#00ff00'>玉液瓶</font>x2</font>",title:"fb绑定",comfirmTxt:"绑定",cancelTxt:"下次再绑定",isShowClose:true,cancel:new Function() });
			facade.sendNotification(EventList.DO_FIRST_TIP, {comfrim:FbBindedSend, info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_1" ],title:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_2" ],comfirmTxt:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_5" ],cancelTxt:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_6" ],isShowClose:true,cancel:new Function() });
		}
		
		private function FbBindedSend():void
		{
			(facade.retrieveMediator(SelfInfoMediator.NAME) as SelfInfoMediator).onFaceBookClick(null);
		}
		/** 任务8 - 皇陵湛卢剑 */
		public function doTask_8():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT8_1" ]);//"与洛神对话领取神器"
					_itemView_0.x = 40;
					_itemView_0.y = 184;
		            this.showArr.push( {item:_itemView_0, startPoint:new Point(_itemView_0.x, _itemView_0.y), state:6} );
					if( GameCommonData.GameInstance.TooltipLayer.stage.stageHeight > GameConfigData.GameHeight )
			        {
			            _itemView_0.y += (GameCommonData.GameInstance.TooltipLayer.stage.stageHeight - GameConfigData.GameHeight + 10)/2;
		            }
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 2:
					//do nothing here
					break;
				case 3:
					if(isFirst)
				    {
				    	isFirst = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT8_2" ]);//"你获得了神器-湛卢\n点击这里装备上吧"
//					    _itemView_0.x = 642;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
					    this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					}
					break;
				case 4:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT8_3" ]);//"双击图标装备上\n神器-湛卢"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(144000);
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 5:
					var dataProxy:DataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					if(dataProxy.BagIsOpen) {
						sendNotification(EventList.CLOSEBAG);
						dataProxy.BagIsOpen = false;
					}
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT8_4" ],0,-1);//"点击这里寻找刺客"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(311);

					_gridView_0 = new NewerHelpGrid(100, 40, 0x00ff00,0,3);
					_gridView_0.x = rect.x + 47; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 6:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务9 - 人物10级需手动升级经验 */
		public function doTask_9():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					
					var eqMed:EquipMediator = facade.retrieveMediator(EquipMediator.NAME) as EquipMediator;
					eqMed.playerAttribute.checkLevUp();		//自动升到11级
					
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT9" ]);//"点击这里去找\n门派武功传授人"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					
					var taskObj:Object = getCaptioinNameWidth();
					var taskId:uint = taskObj.taskId;
					var width:uint  = taskObj.width;
					
					var rect:Rectangle = taskFollowMed.getTaskRectangle(taskId);
					
					
					_gridView_0 = new NewerHelpGrid(width, 20, 0x00ff00,0,3);
					_gridView_0.x = rect.x + 24; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y - NewerHelpData.VIEW_HEIGHT;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
			}
		}
		
		/** 任务10 - 升职业等级 */
		public function doTask_10():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					
					var eqMed:EquipMediator = facade.retrieveMediator(EquipMediator.NAME) as EquipMediator;
					eqMed.playerAttribute.checkLevUp()  ;	//自动提升人物等级到12，职业等级到10

					if(_mcLoader) _mcLoader.destroy();
					_curLoadIndex = 3;
					_mcLoader = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.MC_NEWERHELP_ADDR_ARR[3], BulkLoader.TYPE_MOVIECLIP ,"mcNHLEARNSK");
					_mcLoader.addEventListener(Event.COMPLETE, onPicComplete);
					_mcLoader.load();
					break;
			}
		}
		
		/** 任务11 - 领取门派套装 */
		public function doTask_11(pos:uint=0):void
		{
			switch(NewerHelpData.curStep) {
				case 1:
					clearView();
					if( isFirst )
					{
						isFirst = false;
						sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
					}
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT10_1" ]);//"你获得了职业套装\n点击这里装备"
//					    _itemView_0.x = 642;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
					    
						this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isFirst = true;
					    this.point = null;
					}
					break;
				case 2:
					clearView();
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT10_2" ]);//"双击图标穿上职业套装"
					_curArr = NewerHelpData.findCloses();
					_gridView_0 = new NewerHelpGrid(36, 36);
					_gridView_1 = new NewerHelpGrid(36, 36);
					_gridView_2 = new NewerHelpGrid(36, 36);
					if(!_curArr) break;
					var p0:Point = findItemPos(_curArr[0]);
					var p1:Point = findItemPos(_curArr[1]);
					var p2:Point = findItemPos(_curArr[2]);
					if(p0) {
						_gridView_0.x = p0.x;
						_gridView_0.y = p0.y;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					}
					if(p1) {
						_gridView_1.x = p1.x;
						_gridView_1.y = p1.y;
						this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_1,0);
					}
					if(p2) {
						_gridView_2.x = p2.x;
						_gridView_2.y = p2.y;
						_itemView_0.x = _gridView_2.x - 154;
						_itemView_0.y = _gridView_2.y - 56;
						this.showArr.push( {item:_gridView_2, startPoint:getStartPoint(_gridView_2.x, _gridView_2.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_2,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 3:
					switch(pos) {
						case 0:
							if(_gridView_0 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_0)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_0);
								_gridView_0 = null;
								if(!_gridView_0 && !_gridView_1 && !_gridView_2) {
									NewerHelpData.curStep = 4;
									doTask_11();
								} else {
									if(_gridView_2) {
										_itemView_0.x = _gridView_2.x - 154;
										_itemView_0.y = _gridView_2.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
										this.showArr.push( {item:_gridView_2, startPoint:getStartPoint(_gridView_2.x, _gridView_2.y, 1), state:3} );
									} else {
										_itemView_0.x = _gridView_1.x - 154;
										_itemView_0.y = _gridView_1.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
									}
								}
							}
							break;
						case 1:
							if(_gridView_1 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_1)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_1);
								_gridView_1 = null;
								if(!_gridView_0 && !_gridView_1 && !_gridView_2) {
									NewerHelpData.curStep = 4;
									doTask_11();
								} else {
									if(_gridView_2) {
										_itemView_0.x = _gridView_2.x - 154;
										_itemView_0.y = _gridView_2.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_2, startPoint:getStartPoint(_gridView_2.x, _gridView_2.y, 1), state:3} );
									} else {
										_itemView_0.x = _gridView_0.x - 154;
										_itemView_0.y = _gridView_0.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
									}
								}
							}
							break;
						case 2:
							if(_gridView_2 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_2)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_2);
								_gridView_2 = null;
								if(!_gridView_0 && !_gridView_1 && !_gridView_2) {
									NewerHelpData.curStep = 4;
									doTask_11();
								} else {
									if(_gridView_0) {
										_itemView_0.x = _gridView_0.x - 154;
										_itemView_0.y = _gridView_0.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
									} else {
										_itemView_0.x = _gridView_1.x - 154;
										_itemView_0.y = _gridView_1.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );

									}
								}
							}
							break;
					}
					break;
				case 4:
					clearView();
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		
		
		/** 任务12 - 卖垃圾 */
		public function doTask_12():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					sendNotification(BagEvents.BAG_INIT_POS);
					sendNotification(BagEvents.BAG_STOP_DROG);
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName("Bag") as Sprite;
					if( !panelBase ) return;
				    this.point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(panelBase.x, panelBase.y));
				    
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT12_1" ]);//"双击出售不需要的物品"
//					_itemView_0.x = 445;
//					_itemView_0.y = 95;
					_itemView_0.x = point.x - 141;
					_itemView_0.y = point.y + 40;
					panelBase = null;
					point = null;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(445, 95), state:3} );				
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 2:
					var NPCShopMed:NPCShopMediator = facade.retrieveMediator( NPCShopMediator.NAME ) as NPCShopMediator;
				    point = NPCShopMed.shopView.localToGlobal( new Point( NPCShopMed.shopView.btnSale.x, NPCShopMed.shopView.btnSale.x ) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT12_2" ]);//"点击完成出售"
					_gridView_0 = new NewerHelpGrid(67, 28);
//					_gridView_0.x = 333;
//					_gridView_0.y = 450;
//					_itemView_0.x = 178;
//					_itemView_0.y = 395;
					_gridView_0.x = point.x - 4.05;
					_gridView_0.y = point.y + 116.95;
					_itemView_0.x = point.x - 159.05;
					_itemView_0.y = point.y + 61.95;
					this.showArr.push( {item:_gridView_0, startPoint:new Point(333, 450), state:3} );
					this.showArr.push( {item:_itemView_0, startPoint:new Point(178, 395), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(NPCShopEvent.NPC_SHOP_STOP_DRAG);		//禁止NPC商店拖动
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务13 - 开仓库 */
		public function doTask_13():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					sendNotification(BagEvents.BAG_INIT_POS);
					sendNotification(BagEvents.BAG_STOP_DROG);
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName("Bag") as Sprite;
					if( !panelBase ) return;
				    this.point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(panelBase.x, panelBase.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT13_1" ]);//"双击暂时不用的物品\n存入仓库"
//					_itemView_0.x = 445;
//					_itemView_0.y = 95;
					_itemView_0.x = point.x - 141;
					_itemView_0.y = point.y + 40;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(445, 95), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 2:
					if( isBoolean ){
				    	isBoolean = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT13_2" ]);//"你获得了4级背包栏\N点击这里打开背包"
//					    _itemView_0.x = 642;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
					    
						this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    isBoolean = true;
					    this.point = null;
					}
					break;
				case 3:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT13_3" ]);//"双击使用\n4级背包栏扩充背包"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(502003);
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					} else {
						NewerHelpData.curStep++;
						doTask_13();
					}
					break;
				case 4:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务15 - 鼠标左键攻击怪物；自动选怪、1键攻击 */
		public function doTask_15():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					NewerHelpData.curStep++;
					sendNotification(NewerHelpEvent.ALERT_IMG_NEWER_HELP, {comfrim:fighting, title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ], taskStr:"15_1"});//"小提示"		"我知道了"
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		private function fighting():void
		{
//			MoveToCommon.MoveTo(1002,43,102,303,-1);
//			NewerHelpData.curType = 27;
//			NewerHelpData.curStep = 1;
			if(NewerHelpData.curType == 15 && NewerHelpData.curStep == 2)
			{
				NewerHelpData.curType = 27;
				NewerHelpData.curStep = 1;
				doTask_27();
			}
			else
			{
				specialBool = true;
			}
		}
		
		/** 任务15级联 */
		public function doTask_15_2():void
		{
			sendNotification(NewerHelpEvent.ALERT_IMG_NEWER_HELP, {comfrim:null, title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ], taskStr:"15_2"});//"小提示"		"我知道了"
		}
		
		/** 任务16 - 空格拾取物品 */
		public function doTask_16():void
		{
			clearView();
			NewerHelpData.curType = 0;
			NewerHelpData.curStep = 0;
			sendNotification(NewerHelpEvent.ALERT_IMG_NEWER_HELP, {comfrim:null, title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ], taskStr:"16_1"});//"小提示"		"我知道了"
		}
		
		/** 任务17 - 关闭2级提示框 */
		public function doTask_17():void
		{
			switch(NewerHelpData.curStep) {
				case 1:
					clearView();
					NewerHelpData.curStep++;
					doTask_17();
//					sendNotification(NewerHelpEvent.ALERT_IMG_NEWER_HELP, {comfrim:doTask_17, title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ], taskStr:"2"}); //, canOp:1//"小提示"		"我知道了"
					break;
				case 2:
					//rodo(felix)
//					sendNotification(EventList.SELECTED_NPC_ELEMENT,{npcId:realNpcId});

					MoveToCommon.MoveTo(1001, 57, 114, 0, 0);	//自动寻路去找清照 
//					doTask_17();

					//end
//					clearView();
//					_itemView_0 = new NewerHelpItem("点击这里接受任务",-1,-1);
//					_itemView_0.x = 378;
//					_itemView_0.y = 160;
//					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
//					sendNotification(NPCChatComList.SET_NPCTALK_ISMOVE, 1);
					break;
				case 3:
					clearView();
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT17" ],0,-1);//"点击这里接受任务" 
//					_itemView_0.x = 115;
//					_itemView_0.y = 395;
					
					_itemView_0.x = -135 + (GameCommonData.GameInstance.MainStage.stageWidth - GameConfigData.GameWidth)/2;
					_itemView_0.y = 180 + (GameCommonData.GameInstance.MainStage.stageHeight - GameConfigData.GameHeight)/2;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(115, 395), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(TaskCommandList.SET_TASKINFO_DRAG);	//禁用NPC任务拖动
					NewerHelpData.point = null;
					point = null;
					break;
				case 4:
					clearView();
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务18 - 刚进入游戏提示“点击关闭升到2级” */
		public function doTask_18():void
		{
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT18" ],0,-1);//"点击按钮升到2级"
					var viewBack:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("viewBack");
					if( !viewBack ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(viewBack.x, viewBack.y));
					_itemView_0.x = point.x + 313;
			        _itemView_0.y = point.y + 385;
			        
			        this.showArr.push( {item:_itemView_0, startPoint:new Point(313, 385), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					NewerHelpData.point = null;
					point = null;
					break;
				case 2:
					clearView();
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务19 - 收集3片鸭羽毛 */
		public function doTask_19():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "med_lost_8" ],0,-1);//"点击这里杀死鸭子\n获得羽毛"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(306);
					
					_gridView_0 = new NewerHelpGrid(40, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 36; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
//				    this.clear_View();
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务20 - 指向任务追踪屠大勇，“点击这里自动寻路” */
		public function doTask_20():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					
					MoveToCommon.MoveTo(1001, 70, 102, 301, 120);	//自动寻路去找图大勇
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "med_lost_6" ],0,-1);//"点击这里获取武器"

					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(301);
		
					_gridView_0 = new NewerHelpGrid(56, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 24; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + rect.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
//                    obj = NewerHelpData.changeId( 1001, 120 );
//                    MoveToCommon.sendFlyToCommand({mapId:obj._mapId,tileX:108,tileY:119,taskId:301,npcId:obj._npcId});	// 使用小飞鞋去找图大勇
//					timer = new Timer(500, 1);
//					timer.addEventListener(TimerEvent.TIMER, timer_FUN);
//					timer.start();
					break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT20_2" ],-1,-1);//"点击这里完成任务"
					_itemView_0.x = 240 + (GameCommonData.GameInstance.MainStage.stageWidth - GameConfigData.GameWidth)/2;
					_itemView_0.y = 160 + (GameCommonData.GameInstance.MainStage.stageHeight - GameConfigData.GameHeight)/2;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(370, 160), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(NPCChatComList.SET_NPCTALK_ISMOVE, 1);
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		private function timer_FUN(event:TimerEvent):void
		{
			_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "med_lost_6" ],0,-1);//"点击这里获取武器"

			var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
			var rect:Rectangle = taskFollowMed.getTaskRectangle(301);

			_gridView_0 = new NewerHelpGrid(56, 20,0x00ff00,0,3);
			_gridView_0.x = rect.x + 24; 
			_gridView_0.y = rect.y + 13; 
			
			_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
			_itemView_0.y = _gridView_0.y + rect.height;
			
			this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
			this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
			GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
			GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
			
			sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
			timer.removeEventListener(TimerEvent.TIMER, timer_FUN);
			timer = null;
			
		}
		
		/** 任务21 - 进宠物林二层抓孔雀 */
		public function doTask_21():void
		{
			clearView();
			sendNotification(EventList.DO_FIRST_TIP, {comfrim:null, info:DialogConstData.getInstance().getTipDesByType(10), title:GameCommonData.wordDic[ "often_used_smallTip" ], comfirmTxt:GameCommonData.wordDic[ "mod_ale_ale_ComfirmTxt_big" ]});//"小提示"		"我知道了"
			NewerHelpData.curType = 0;
			NewerHelpData.curStep = 0;
		}
		
		/** 任务22 - 商城购物 武道乾坤 */
		public function doTask_22():void
		{
//			clearView();
//			switch(NewerHelpData.curStep) {
//				case 1:
//					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT22_1" ]);//"点击这里打开商城"
//					_itemView_0.x = 790;
//					_itemView_0.y = 480;
//					if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
//					{
//						_itemView_0.x += GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth;
//					}
//					if( GameCommonData.GameInstance.TooltipLayer.stage.stageHeight > GameConfigData.GameHeight )
//					{
//						_itemView_0.y += GameCommonData.GameInstance.TooltipLayer.stage.stageHeight - GameConfigData.GameHeight
//					}
//					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
//					sendNotification(EventList.SHOW_SMALL_MAP);
//					break;
//				case 2:
//					sendNotification(EventList.SHOWMARKETVIEW);
//					sendNotification(MarketEvent.MARKET_STOP_DROG);
//					panelBase = GameCommonData.GameInstance.TooltipLayer.getChildByName( "marketView" ) as Sprite;
//					if( !panelBase ) return;
//					point = GameCommonData.GameInstance.TooltipLayer.localToGlobal(new Point( panelBase.x, panelBase.y ));
//					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT22_2" ]);//"点击这里进入点券商店");
////					_itemView_0.x = 278;
////					_itemView_0.y = 115;
//					_itemView_0.x = point.x + 258;
//					_itemView_0.y = point.y + 115;
//					GameCommonData.GameInstance.TooltipLayer.addChild(_itemView_0);
//					break;
//				case 3: 
//					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT22_3" ]);//"点击这里将武道乾坤\n放入购物车",-1);
////					_itemView_0.x = 0;
////					_itemView_0.y = 138;
//					_itemView_0.x = point.x + 0;
//					_itemView_0.y = point.y + 138;
//					GameCommonData.GameInstance.TooltipLayer.addChild(_itemView_0);
//					break;
//				case 4:
//					sendNotification(MarketEvent.MARKET_STOP_DROG);
//					//-----------
//					_itemView_1 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT22_5" ]);//"点击购买道具，每充值\n50元宝可获赠1点券",0,-1);
//					_itemView_1.x = point.x + 532;
//					_itemView_1.y = point.y + 392;
//					GameCommonData.GameInstance.TooltipLayer.addChild(_itemView_1);
//					//----------
//					var wuDao:Sprite = NewerHelpData.marketUI.getChildByName( NewerHelpData.comboxName ) as Sprite;
//					point = NewerHelpData.marketUI.localToGlobal( new Point( wuDao.x, wuDao.y ) );
////					trace( point.x, point.y );
//					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT22_4" ]);//"点击这里选择支付方式\n点券商店只能使用点券",-1);
////					_itemView_0.x = 565;
////					_itemView_0.y = 156;
//					_itemView_0.x = point.x - 103.85;
//					_itemView_0.y = point.y - 58.05;
//					GameCommonData.GameInstance.TooltipLayer.addChild(_itemView_0);
//					break;
//				case 5:
//					NewerHelpData.curType = 0;
//					NewerHelpData.curStep = 0;
//					break;
//			}
		}
		
		/** 任务23 - 弹出好友介绍 */
		public function doTask_23():void
		{
			clearView();
			if(_mcShowArr[0]) {
				_mcShowArr[0].x = UIUtils.getMiddlePos(_mcShowArr[0]).x;
				_mcShowArr[0].y = UIUtils.getMiddlePos(_mcShowArr[0]).y;//60;
				GameCommonData.GameInstance.TooltipLayer.addChildAt(_mcShowArr[0],0);
			} else {
				if(_mcLoader) _mcLoader.destroy();
				_curLoadIndex = 0;
				_mcLoader = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.MC_NEWERHELP_ADDR_ARR[0], BulkLoader.TYPE_MOVIECLIP ,"mcNHFri");
				_mcLoader.addEventListener(Event.COMPLETE, onPicComplete);
				_mcLoader.load();
			}
			NewerHelpData.curType = 0;
			NewerHelpData.curStep = 0;
		}
		
		/** 任务24 - 弹出宠物玩法介绍 */
		public function doTask_24():void
		{
			clearView();
			if(_mcShowArr[1]) {
				_mcShowArr[1].x = UIUtils.getMiddlePos(_mcShowArr[1]).x;
				_mcShowArr[1].y = UIUtils.getMiddlePos(_mcShowArr[1]).y;//60;
				GameCommonData.GameInstance.TooltipLayer.addChildAt(_mcShowArr[1],0);
			} else {
				if(_mcLoader) _mcLoader.destroy();
				_curLoadIndex = 1;
				_mcLoader = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.MC_NEWERHELP_ADDR_ARR[1], BulkLoader.TYPE_MOVIECLIP ,"mcNHPet");
				_mcLoader.addEventListener(Event.COMPLETE, onPicComplete);
				_mcLoader.load();
			}
			NewerHelpData.curType = 0;
			NewerHelpData.curStep = 0;
		}
		
		/** 任务25 - 弹出装备介绍 */
		public function doTask_25():void
		{
			clearView();
			if(_mcShowArr[2]) {
				_mcShowArr[2].x = UIUtils.getMiddlePos(_mcShowArr[2]).x;
				_mcShowArr[2].y = UIUtils.getMiddlePos(_mcShowArr[2]).y;//60;
				GameCommonData.GameInstance.TooltipLayer.addChildAt(_mcShowArr[2],0);
			} else {
				if(_mcLoader) _mcLoader.destroy();
				_curLoadIndex = 2;
				_mcLoader = new ImageItem(GameCommonData.GameInstance.Content.RootDirectory + GameConfigData.MC_NEWERHELP_ADDR_ARR[2], BulkLoader.TYPE_MOVIECLIP ,"mcNHEqu");
				_mcLoader.addEventListener(Event.COMPLETE, onPicComplete);
				_mcLoader.load();
			}
			NewerHelpData.curType = 0;
			NewerHelpData.curStep = 0;
		}
		
		/** 任务26 - 宠物林1层指导 */
		public function doTask_26():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT26_1" ],0,-1);//"点击这里打开小地图\n快捷键Tab"
					_itemView_0.x = 685;
					_itemView_0.y = 66;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(_itemView_0.x, _itemView_0.y), state:1} );
					if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
					{
						_itemView_0.x += GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth;
					}
					
					this.showArr.push( {item:_itemView_0, startPoint:new Point(685, 66), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(EventList.SHOW_SMALL_MAP);
					break;
				case 2:
					var senceMapMed:SenceMapMediator = facade.retrieveMediator( SenceMapMediator.NAME ) as SenceMapMediator;
				    this.panelBase = GameCommonData.GameInstance.GameUI.getChildByName( "senceMapContainer" ) as Sprite;
				    if( !panelBase ) return;
				    point = GameCommonData.GameInstance.GameUI.localToGlobal( new Point( panelBase.x, panelBase.y ) );
					_itemView_1 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT26_2" ],0,-1);//"点击这里自动寻路\n进入宠物林二层"
//					_itemView_1.x = 245;
//					_itemView_1.y = 432;
					_itemView_1.x = point.x + 5;
					_itemView_1.y = point.y + 335;
					this.showArr.push( {item:_itemView_1, startPoint:new Point(245, 432), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_1,0);
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务27 - 指向任务追踪绒毛图，“点击这里自动寻路” */
		public function doTask_27():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
				    var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(303);
					
				    if(specialBool)
				    {
				    	specialBool = false;
				    	NewerHelpData.CAN_FLY = true;
				    	NewerHelpData.isFirst = true;
				    	_itemView_3 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT20_1" ],0,-1);//"点击这里自动寻路"

						_gridView_3 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
						_gridView_3.x = rect.x + 36; 
						_gridView_3.y = rect.y + 13; 
						
						_itemView_3.x = _gridView_3.x - NewerHelpData.VIEW_WIDTH;
						_itemView_3.y = _gridView_3.y + _gridView_3.height;
						
						this.showArr.push( {item:_gridView_3, startPoint:getStartPoint(_gridView_3.x, _gridView_3.y, 2), state:1} );
						this.showArr.push( {item:_itemView_3, startPoint:getStartPoint(_itemView_3.x, _itemView_3.y, 2), state:1} );
						showOtherArr.push( {item:_gridView_3, startPoint:getStartPoint(_gridView_3.x, _gridView_3.y, 2), state:1} );
						showOtherArr.push( {item:_itemView_3, startPoint:getStartPoint(_itemView_3.x, _itemView_3.y, 2), state:1} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_3,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_3,0);
				    }
				    else
				    {
				    	specialBool = true;
				    	clear_View();
				    	_itemView_3 = new NewerHelpItem(GameCommonData.wordDic[ "med_lost_7" ],0,-1);//"点击这里杀死兔子，获取兔毛"

						_gridView_3 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
						_gridView_3.x = rect.x + 36; 
						_gridView_3.y = rect.y + 13; 
						
						_itemView_3.x = _gridView_3.x - NewerHelpData.VIEW_WIDTH;
						_itemView_3.y = _gridView_3.y + _gridView_3.height;
						
						this.showArr.push( {item:_gridView_3, startPoint:getStartPoint(_gridView_3.x, _gridView_3.y, 2), state:1} );
						this.showArr.push( {item:_itemView_3, startPoint:getStartPoint(_itemView_3.x, _itemView_3.y, 2), state:1} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_3,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_3,0);
				    }
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		/** 任务28 - 指向任务追踪张择端完成大侠之路加入门派，“完成10级大侠之路任务加入门派” */
		public function doTask_28():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT28" ],0,-1);//"完成10级大侠之路任务\n加入门派"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(101);

					_gridView_0 = new NewerHelpGrid(44, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 107; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		/** 任务29 - 指向任务追踪6大门派，“点击这里寻找6大门派指引人” */
		public function doTask_29():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT29" ]);//"点击这里寻找六大\n门派指引人加入门派"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(102);

					_gridView_0 = new NewerHelpGrid(173, 52, 0x00ff00,0,3);
					_gridView_0.x = rect.x + 9; 
					var _Y:int;
					if(GameCommonData.wordVersion == 2)
					{
						_Y = 48;
					}
					else
					{
						_Y = 31;
					}
					_gridView_0.y = rect.y + _Y; 
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y - NewerHelpData.VIEW_HEIGHT;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		/** 任务30 - "与掌门对话加入门派" */
		public function doTask_30():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT30" ]);//"与掌门对话加入门派"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					
					var taskObj:Object = getCaptioinNameWidth();
					var taskId:uint = taskObj.taskId;
					var width:uint  = taskObj.width;
					
					var rect:Rectangle = taskFollowMed.getTaskRectangle(taskId);
					
					
					_gridView_0 = new NewerHelpGrid(width, 20, 0x00ff00,0,3);
					_gridView_0.x = rect.x + 24; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y - NewerHelpData.VIEW_HEIGHT;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		/** 任务31 - 第一次学习技能 */
		public function doTask_31():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					sendNotification(SkillConst.STOP_MOVE_SKILLLEARN_PANEL);	//锁定技能学习面板位置
					//----------
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName( "SkillLearnView" ) as Sprite;
					if( !panelBase ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal( new Point(panelBase.x, panelBase.y) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT31_1" ],-1);//"请选择任意可提升\n的技能"
//					_itemView_0.x = 540;
//					_itemView_0.y = 80;
					_itemView_0.x = point.x + 460;
					_itemView_0.y = point.y + 22;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(540, 80), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT31_2" ],-1);//"请点击学习按钮"
//					_itemView_0.x = 548;
//					_itemView_0.y = 425;
					_itemView_0.x = point.x + 468;
					_itemView_0.y = point.y + 367;
					point = null;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(548, 425), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 4:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务32 - "去找白云飞" */
		public function doTask_32():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT32" ]);//"点击这里去找白云飞"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					
					var rect:Rectangle = taskFollowMed.getTaskRectangle(128);
					
					_gridView_0 = new NewerHelpGrid(57, 20, 0x00ff00,0,3);
					_gridView_0.x = rect.x + 24; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y - NewerHelpData.VIEW_HEIGHT;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, false);	//启用任务栏拖动
					break;
			}
		}
		
		/** 任务33 - "获得师门关怀 4小时双倍经验" */
		public function doTask_33():void
		{
			var item:DisplayObject = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT33" ],0,-1,true,null,0,0,80);//"你获得了师门关怀：\n打怪和挂机经验翻倍\n持续时间4小时"
			item.x = 618;
			item.y = 56;
			this.showArr.push( {item:item, startPoint:new Point(item.x, item.y), state:1} );
			showOtherArr.push( {item:item, startPoint:new Point(item.x, item.y), state:1} );
			if(GameCommonData.GameInstance.GameUI.stage.stageWidth >= GameConfigData.GameWidth)
			{
				item.x = 618 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;   
			}
			GameCommonData.GameInstance.TooltipLayer.addChildAt(item,0);
		}
		
		/** 任务34 - 宠物10、20、30...时提示 */
		public function doTask_34():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					if( petLevel )
				    {
				    	petLevel = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 2);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT34_1" ]);//"提升宠物属性点\n使其发挥更大的威力");
//					    _itemView_0.x = 578;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 153.05;
					    _itemView_0.y = point.y - 63.55;
					    
						this.showArr.push( {item:_itemView_0, startPoint:new Point(578, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    petLevel = true;
					    this.point = null;
					}
					break;
				case 2:
					petBag = GameCommonData.GameInstance.GameUI.getChildByName("PetBag");
				    if( !petBag ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(petBag.x, petBag.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT34_2" ],-1,0);//"推荐参考宠物的资质\n来分配属性点";
					if(PetPropConstData.isNewPetVersion)
					{
						_gridView_0 = new NewerHelpGrid(178, 250);
						_gridView_0.x = point.x + 236;
						_gridView_0.y = point.y + 52;
						_itemView_0.x = point.x + 573;
						_itemView_0.y = point.y + 112;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(316, 110), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(653, 170), state:3} );
					}
					else
					{
						_gridView_0 = new NewerHelpGrid(153, 198);
						_gridView_0.x = point.x + 236;
						_gridView_0.y = point.y + 46;
						_itemView_0.x = point.x + 546;
						_itemView_0.y = point.y + 112;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(316, 104), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(626, 170), state:3} );
					
					}
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(PetEvent.PETPROP_PANEL_STOP_DROG);		//禁止宠物面板拖动
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务35 - 宠物5、15、25...时提示 */
		public function doTask_35():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					if( petLevel )
				    {
				    	petLevel = false;
				    	sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 2);
				    }
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT34_1" ]);//"提升宠物属性点\n使其发挥更大的威力");
//					    _itemView_0.x = 578;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 153.05;
					    _itemView_0.y = point.y - 63.55;
					    
						this.showArr.push( {item:_itemView_0, startPoint:new Point(578, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    petLevel = true;
					    this.point = null;
					}
					break;
				case 2:
					petBag = GameCommonData.GameInstance.GameUI.getChildByName("PetBag");
				    if( !petBag ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(petBag.x, petBag.y));
					if(PetPropConstData.isNewPetVersion)
					{
						_gridView_0 = new NewerHelpGrid(18, 106);
						_gridView_0.x = point.x + 377;
						_gridView_0.y = point.y + 57;
						_itemView_0.x = point.x + 553;
						_itemView_0.y = point.y + 1;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(457, 115), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(633, 59), state:3} );
					}
					else
					{
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT35" ],-1,0);//"一直按住加号\n可加满一项属性"
						_gridView_0 = new NewerHelpGrid(18, 88);
						_gridView_0.x = point.x + 352;
						_gridView_0.y = point.y + 46;
						_itemView_0.x = point.x + 528;
						_itemView_0.y = point.y - 10;
						this.showArr.push( {item:_gridView_0, startPoint:new Point(432, 104), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:new Point(608, 48), state:3} );
						}
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(PetEvent.PETPROP_PANEL_STOP_DROG);		//禁止宠物面板拖动
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务36 - 人物13级 提示15级可领取成就礼包 */
		public function doTask_36():void
		{
//			var item:NewerHelpItem = new NewerHelpItem("你升到了13级\n达到15级时即可\n领取第一个成就礼包",0,0,true,null,0,0,80);
//			item.x = 770;
//			item.y = 390;
//			if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null) {
//				var tmpDis:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton");
//				item.x = tmpDis.x - item.width;
//				item.y = tmpDis.y - NewerHelpData.VIEW_HEIGHT - 20;
//			}
//			item.logo = NewerHelpData.LEV13_BAO_IKNOW;
//			NewerHelpData.iKnowItemDic[NewerHelpData.LEV13_BAO_IKNOW] = item;	//存起  方便删除
//			GameCommonData.GameInstance.TooltipLayer.addChildAt(item,0);
		}
		
		/** 任务37 - 15级领取成就礼包 */
		public function doTask_37():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT37_1" ]);//"恭喜你达到15级\n领取第一个成就礼包");
//					_itemView_0.x = 770;
//					_itemView_0.y = 390;
					if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null) {
						var tmpDis:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton");
						_itemView_0.x = tmpDis.x - _itemView_0.width;
						_itemView_0.y = tmpDis.y - NewerHelpData.VIEW_HEIGHT;
						this.showArr.push( {item:_itemView_0, startPoint:new Point(811.25, 390), state:4} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					}
					break;
				case 2:
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName( "unity" ) as Sprite;
				    if( !panelBase ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal( new Point(panelBase.x, panelBase.y) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT37_2" ]);//"点击按钮\n领取元宝道具奖励");
//					_itemView_0.x = 525;
//					_itemView_0.y = 420;
					_itemView_0.x = point.x + 303;
					_itemView_0.y = point.y + 392;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(525, 420), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(NewAwardEvent.DENY_DRAG_PANEL);	//禁止拖动
					break;
				case 3:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT37_3" ],-1,0,true,null,0,0,70);//"明天登陆并达到26级\n可领取第二份礼包"
//					_itemView_0.x = 690;
//					_itemView_0.y = 77;
					_itemView_0.x = point.x + 468;
					_itemView_0.y = point.y + 49;
					point = null;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(690, 77), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 4:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务39 - 接受山猪任务 */
		public function doTask_39():void
		{
			var item:DisplayObject = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT39" ],0,-1,true,null,0,0,70);//"如果周围玩家过多\n可选择更换线路打怪"
			item.x = 575;
			item.y = 23;
			this.showArr.push( {item:item, startPoint:new Point(item.x, item.y), state:1} );
			this.showOtherArr.push( {item:item, startPoint:new Point(item.x, item.y), state:1} );
			if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
			{
				item.x += GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth;
			}
			GameCommonData.GameInstance.TooltipLayer.addChildAt(item,0);
		}
		
		/** 任务40 - 打死第一只山猪 */
		public function doTask_40():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT40_1" ],0,-1);//"挂机会让打怪更轻松\n请点击“挂”字按钮"
					_itemView_0.x = 738;
					_itemView_0.y = 188;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(738, 188), state:1} );
					if( GameCommonData.GameInstance.TooltipLayer.stage.stageWidth > GameConfigData.GameWidth )
					{
						_itemView_0.x += GameCommonData.GameInstance.TooltipLayer.stage.stageWidth - GameConfigData.GameWidth;
					}
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 2:
					apMed = facade.retrieveMediator( AutoPlayMediator.NAME ) as AutoPlayMediator;
				    point = apMed.viewUI.localToGlobal( new Point( apMed.viewUI.autoSet_btn.x, apMed.viewUI.autoSet_btn.y ) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT40_2" ],0,0,false,null,0,0,63);//"自动设置可满足\n大多数打怪情况\n请点击自动设置"
//					_itemView_0.x = 392;
//					_itemView_0.y = 435;
					_itemView_0.x = point.x - 156;
					_itemView_0.y = point.y - 56;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(392, 435), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					sendNotification(AutoPlayEventList.STOP_DRAG_AUTOPLAY);	//禁止面板拖动
					break;
				case 3:
					point = apMed.viewUI.localToGlobal( new Point( apMed.viewUI.saveSet_btn.x, apMed.viewUI.saveSet_btn.y ) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT40_3" ],-1,0,true,callBackIKnow,40,3);//"可选择是否保存该设置"
//					_itemView_0.x = 830;
//					_itemView_0.y = 435;
					_itemView_0.x = point.x + 212;
					_itemView_0.y = point.y - 56;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(830, 435), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 4:
					point = apMed.viewUI.localToGlobal( new Point( apMed.viewUI.btn_commit.x, apMed.viewUI.btn_commit.y ) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT40_4" ],-1,0);//"点击开始挂机\n进行自动打怪"
//					_itemView_0.x = 902;
//					_itemView_0.y = 435;
					_itemView_0.x = point.x + 214;
					_itemView_0.y = point.y - 56;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(902, 435), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 5:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT40_5" ],0,0,true);//"等待山猪肉打满8个"
					
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(202);

					_itemView_0.x = rect.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = rect.y - NewerHelpData.VIEW_HEIGHT;
					
					this.showArr.push( {item:_itemView_0, startPoint:new Point(639, 227), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 6:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT40_6" ],0,-1,false,null,0,0,70);//"任务完成\n点击王质自动交任务\n并停止挂机"
					
					var taskFollowMedWZ:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					
					var rectWZ:Rectangle = taskFollowMedWZ.getTaskRectangle(202);
					
					_gridView_0 = new NewerHelpGrid(45, 20, 0x00ff00,0,3);
					_gridView_0.x = rectWZ.x + 107; 
					_gridView_0.y = rectWZ.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					break;
				case 7:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务41 - 8级提示框后 提示打开第一个新手大礼包 */
		public function doTask_41(pos:uint=0):void
		{
			switch(NewerHelpData.curStep) {
				case 1:
					clearView();
					if( roleLevel )
					{
						roleLevel = false;
						sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
					}
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT41_1" ]);//"现在可以打开\n第一个新手大礼包！"
//					    _itemView_0.x = 642;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
						this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    roleLevel = true;
					    this.point = null;
					    NewerHelpData.point = null;
					}
					break;
				case 2:
					clearView();
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT41_2" ]);//"双击打开新手大礼包"
					_gridView_0 = new NewerHelpGrid();
					var p:Point = findItemPos(500100);
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 3:
					clearView();
					//do noting here
					break;
				case 4:
					//----------
					clearView();
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT41_3" ]);//"双击穿上装备"
					_curArr = NewerHelpData.FIRST_BAG_ITEMS;
					_gridView_0 = new NewerHelpGrid(36, 36);
					_gridView_1 = new NewerHelpGrid(36, 36);
					if(!_curArr) break;
					var p0:Point = findItemPos(_curArr[0]);
					var p1:Point = findItemPos(_curArr[1]);
					if(p0) {
						_gridView_0.x = p0.x;
						_gridView_0.y = p0.y;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					}
					if(p1) {
						_gridView_1.x = p1.x;
						_gridView_1.y = p1.y;
						this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_1,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 5:
					switch(pos) {
						case 0:
							if(_gridView_0 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_0)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_0);
								_gridView_0 = null;
								if(!_gridView_0 && !_gridView_1) {
									NewerHelpData.curStep = 6;
									doTask_41();
								} else {
									if(_gridView_1) {
										_itemView_0.x = _gridView_1.x - 154;
										_itemView_0.y = _gridView_1.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
									}
								}
							}
							break;
						case 1:
							if(_gridView_1 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_1)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_1);
								_gridView_1 = null;
								if(!_gridView_0 && !_gridView_1) {
									NewerHelpData.curStep = 6;
									doTask_41();
								} else {
									if(_gridView_0) {
										_itemView_0.x = _gridView_0.x - 154;
										_itemView_0.y = _gridView_0.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
									}
								}
							}
							break;
					}
					//--------
					break;
				case 6:
					clearView();
					this.panelBase = GameCommonData.GameInstance.GameUI.getChildByName("Bag") as Sprite;
					if( !panelBase ) return;
				    this.point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(panelBase.x, panelBase.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_3" ]);//"点击关闭背包"
//					_itemView_0.x = 650;
//					_itemView_0.y = 10;
					_itemView_0.x = point.x + 64;
					_itemView_0.y = point.y - 45;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(650, 10), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					this.point = null;
					NewerHelpData.point = null;
					break;
				case 7:
					clearView();
					var status:uint = (GameCommonData.TaskInfoDic[309] as TaskInfoStruct).status;
					if(status == 1 || status == 3) {
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT41_4" ],0,-1);//"找到熊霸天\n进入上古皇陵副本"
						
						var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					
						var rect:Rectangle = taskFollowMed.getTaskRectangle(309);
						
						_gridView_0 = new NewerHelpGrid(57, 20, 0x00ff00,0,3);
						_gridView_0.x = rect.x + 35; 
						_gridView_0.y = rect.y + 13; 
						
						_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
						_itemView_0.y = _gridView_0.y + _gridView_0.height;
						
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						
//						_itemView_0.x = 620;
//						_itemView_0.y = 215;
//						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					} else {
						NewerHelpData.curStep = 8;
						doTask_41();
					}
				case 8:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务44 - 18级提示框后 提示打开第二个新手大礼包 */
		public function doTask_44(pos:uint=0):void
		{
			switch(NewerHelpData.curStep) {
				case 1:
					clearView();
					if( roleLevel )
					{
						roleLevel = false;
						sendNotification(EventList.SHOW_MAINSENCE_BTN_FLASH, 4);
					}
					else
					{
						this.point = NewerHelpData.point;
						_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT44_1" ]);//"现在可以打开\n第二个新手大礼包！"
//					    _itemView_0.x = 642;
//					    _itemView_0.y = 472;
					    _itemView_0.x = point.x - 155.8;
					    _itemView_0.y = point.y - 64.55;
					    
						this.showArr.push( {item:_itemView_0, startPoint:new Point(642, 472), state:4} );
					    GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					    roleLevel = true;
					    this.point = null;
					    NewerHelpData.point = null;
					}
					break;
				case 2:
					clearView();
					var p:Point = findItemPos(500101);
					if(!p) {	//如果当前没有18级大礼包 说明玩家尚未打开8级大礼包，先引导玩家打开8级大礼包
						p = findItemPos(500100);
					}
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT44_2" ]);//"双击打开新手大礼包"
					_gridView_0 = new NewerHelpGrid();
					if(p) {
						_gridView_0.x = p.x -7;
						_gridView_0.y = p.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 3:
					clearView();
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT44_3" ]);//"双击获得大量经验"
					_gridView_0 = new NewerHelpGrid();
					var p9:Point = findItemPos(501026);
					if(p9) {
						_gridView_0.x = p9.x -7;
						_gridView_0.y = p9.y -7;
						_itemView_0.x = _gridView_0.x - 154;
						_itemView_0.y = _gridView_0.y - 56;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
					}
					break;
				case 4:
					//----------
					clearView();
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT41_3" ]);//"双击穿上装备"
					_curArr = NewerHelpData.SECOND_BAG_ITEMS;
					_gridView_0 = new NewerHelpGrid(36, 36);
					_gridView_1 = new NewerHelpGrid(36, 36);
					_gridView_2 = new NewerHelpGrid(36, 36);
					if(!_curArr) break;
					var p0:Point = findItemPos(_curArr[0]);
					var p1:Point = findItemPos(_curArr[1]);
					var p2:Point = findItemPos(_curArr[2]);
					if(p0) {
						_gridView_0.x = p0.x;
						_gridView_0.y = p0.y;
						this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					}
					if(p1) {
						_gridView_1.x = p1.x;
						_gridView_1.y = p1.y;
						this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_1,0);
					}
					if(p2) {
						_gridView_2.x = p2.x;
						_gridView_2.y = p2.y;
						_itemView_0.x = _gridView_2.x - 154;
						_itemView_0.y = _gridView_2.y - 56;
						this.showArr.push( {item:_gridView_2, startPoint:getStartPoint(_gridView_2.x, _gridView_2.y, 1), state:3} );
						this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
						GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_2,0);
						sendNotification(BagEvents.BAG_GOTO_SOME_INDEX, 0);
						sendNotification(BagEvents.BAG_STOP_DROG);		//禁止背包拖动
					}
					break;
				case 5:
					switch(pos) {
						case 0:
							if(_gridView_0 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_0)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_0);
								_gridView_0 = null;
								if(!_gridView_0 && !_gridView_1 && !_gridView_2) {
									NewerHelpData.curStep = 6;
									doTask_44();
								} else {
									if(_gridView_2) {
										_itemView_0.x = _gridView_2.x - 154;
										_itemView_0.y = _gridView_2.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_2, startPoint:getStartPoint(_gridView_2.x, _gridView_2.y, 1), state:3} );
									} else {
										_itemView_0.x = _gridView_1.x - 154;
										_itemView_0.y = _gridView_1.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
									}
								}
							}
							break;
						case 1:
							if(_gridView_1 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_1)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_1);
								_gridView_1 = null;
								if(!_gridView_0 && !_gridView_1 && !_gridView_2) {
									NewerHelpData.curStep = 6;
									doTask_44();
								} else {
									if(_gridView_2) {
										_itemView_0.x = _gridView_2.x - 154;
										_itemView_0.y = _gridView_2.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_2, startPoint:getStartPoint(_gridView_2.x, _gridView_2.y, 1), state:3} );
									} else {
										_itemView_0.x = _gridView_0.x - 154;
										_itemView_0.y = _gridView_0.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
									}
								}
							}
							break;
						case 2:
							if(_gridView_2 && GameCommonData.GameInstance.TooltipLayer.contains(_gridView_2)) {
								GameCommonData.GameInstance.TooltipLayer.removeChild(_gridView_2);
								_gridView_2 = null;
								if(!_gridView_0 && !_gridView_1 && !_gridView_2) {
									NewerHelpData.curStep = 6;
									doTask_44();
								} else {
									if(_gridView_0) {
										_itemView_0.x = _gridView_0.x - 154;
										_itemView_0.y = _gridView_0.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 1), state:3} );
									} else {
										_itemView_0.x = _gridView_1.x - 154;
										_itemView_0.y = _gridView_1.y - 56;
										this.showArr = new Array();
										this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 1), state:3} );
										this.showArr.push( {item:_gridView_1, startPoint:getStartPoint(_gridView_1.x, _gridView_1.y, 1), state:3} );
									}
								}
							}
							break;
					}
					//--------
					break;
				case 6:
					clearView();
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName("Bag") as Sprite;
					if( !panelBase ) return;
				    this.point = GameCommonData.GameInstance.GameUI.localToGlobal(new Point(panelBase.x, panelBase.y));
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT_3" ]);//"点击关闭背包"
//					_itemView_0.x = 650;
//					_itemView_0.y = 10;
					_itemView_0.x = point.x + 64;
					_itemView_0.y = point.y - 45;
					
					this.showArr.push( {item:_itemView_0, startPoint:new Point(650, 10), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 7:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务45 - 将前3个技能提升到10级 */
		public function doTask_45():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					sendNotification(SkillConst.STOP_MOVE_SKILLLEARN_PANEL);	//锁定技能学习面板位置
					//----------
					panelBase = GameCommonData.GameInstance.GameUI.getChildByName( "SkillLearnView" ) as Sprite;
					if( !panelBase ) return;
					point = GameCommonData.GameInstance.GameUI.localToGlobal( new Point(panelBase.x, panelBase.y) );
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_1" ],-1);//"点击选中该技能"
//					_itemView_0.x = 390;
//					_itemView_0.y = 65; 
					_itemView_0.x = point.x + 310;
					_itemView_0.y = point.y + 7; 
					this.showArr.push( {item:_itemView_0, startPoint:new Point(390, 65), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_2" ],-1);//"点击学习按钮\n将技能提升到10级"
//					_itemView_0.x = 548;
//					_itemView_0.y = 425;
					_itemView_0.x = point.x + 468;
					_itemView_0.y = point.y + 382;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(548, 425), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 3:
					//----------
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_1" ],-1);//"点击选中该技能"
//					_itemView_0.x = 540;
//					_itemView_0.y = 65; 
					_itemView_0.x = point.x + 460;
					_itemView_0.y = point.y + 7;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(540, 65), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 4:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_2" ],-1);//"点击学习按钮\n将技能提升到10级"
//					_itemView_0.x = 548;
//					_itemView_0.y = 425;
					_itemView_0.x = point.x + 468;
					_itemView_0.y = point.y + 382;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(548, 425), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 5:
					//----------
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_1" ],-1);//"点击选中该技能"
//					_itemView_0.x = 392;
//					_itemView_0.y = 105; 
					_itemView_0.x = point.x + 312;
					_itemView_0.y = point.y + 47;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(392, 105), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 6:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_2" ],-1);//"点击学习按钮\n将技能提升到10级"
//					_itemView_0.x = 548;
//					_itemView_0.y = 425;
					_itemView_0.x = point.x + 468;
					_itemView_0.y = point.y + 382;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(548, 425), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 7:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT45_3" ],-1,-1);//"点击关闭面板"
//					_itemView_0.x = 558;
//					_itemView_0.y = 67; 
					_itemView_0.x = point.x + 478;
					_itemView_0.y = point.y + 9; 
					point = null;
					this.showArr.push( {item:_itemView_0, startPoint:new Point(558, 67), state:3} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 8:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		/** 任务46 - 收集3片鸭翅膀，引导使用小飞鞋 */
		public function doTask_46():void
		{
			clearView();
			switch(NewerHelpData.curStep) {
				case 1:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT46_1" ],0,-1);//"点击使用小飞鞋\n可直接传送到目标地点"
					var taskFollowMed:TaskFollowMediator = facade.retrieveMediator(TaskFollowMediator.NAME) as TaskFollowMediator;
					var rect:Rectangle = taskFollowMed.getTaskRectangle(306);

					_gridView_0 = new NewerHelpGrid(20, 20,0x00ff00,0,3);
					_gridView_0.x = rect.x + 72; 
					_gridView_0.y = rect.y + 13; 
					
					_itemView_0.x = _gridView_0.x - NewerHelpData.VIEW_WIDTH;
					_itemView_0.y = _gridView_0.y + _gridView_0.height;
					
					this.showArr.push( {item:_gridView_0, startPoint:getStartPoint(_gridView_0.x, _gridView_0.y, 2), state:1} );
					this.showArr.push( {item:_itemView_0, startPoint:getStartPoint(_itemView_0.x, _itemView_0.y, 2), state:1} );
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_gridView_0,0);
					sendNotification(TaskCommandList.SET_TASKFOLLOW_DRAG, true);	//禁用任务栏拖动
					break;
				case 2:
					_itemView_0 = new NewerHelpItem(GameCommonData.wordDic[ "mod_new_med_newerHelpU_doT46_2" ],0,-1);//"点击确认\n使用小飞鞋功能"
					_itemView_0.x = 272;
					_itemView_0.y = 335; 
					this.showArr.push( {item:_itemView_0, startPoint:new Point(_itemView_0.x, _itemView_0.y), state:3} );
					if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
					{
						_itemView_0.x += (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					}
					if( GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight )
					{
						_itemView_0.y += (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}
					
					GameCommonData.GameInstance.TooltipLayer.addChildAt(_itemView_0,0);
					break;
				case 3:
					NewerHelpData.curType = 0;
					NewerHelpData.curStep = 0;
					break;
			}
		}
		
		
		
		/** "我知道了"回调函数 */
		private function callBackIKnow(type:uint, step:uint):void
		{
			if(type == 0 || step == 0) {
				return;
			}
			if(NewerHelpData.curType == 40 && NewerHelpData.curStep == 3 && type == 40 && step == 3) {
				NewerHelpData.curStep++;
				doTask_40();
			}
		}
		
		
		/** 下载项完成事件 */
		private function onPicComplete(e:Event):void 
		{
			var mc:MovieClip = e.target.content.content as MovieClip;
			var panelBase:PanelBase = new PanelBase(mc, mc.width + MC_SIZE_INCREMENT[_curLoadIndex].w, mc.height + MC_SIZE_INCREMENT[_curLoadIndex].h);
			panelBase.SetTitleTxt(MC_TITLE[_curLoadIndex]);
			panelBase.addEventListener(Event.CLOSE, closeHandler);
			panelBase.x = UIUtils.getMiddlePos(panelBase).x;
			panelBase.y = UIUtils.getMiddlePos(panelBase).y;//60;
			_mcShowArr[_curLoadIndex] = panelBase;
			GameCommonData.GameInstance.TooltipLayer.addChildAt(panelBase,0);
			e.target.destroy();
			e.target.removeEventListener(Event.COMPLETE, onPicComplete);
		}
		
		private function closeHandler(e:Event):void
		{
			clearView();
		}
		
		/** 查找背包中某物品的位置坐标 */
		private function findItemPos(type:uint):Point
		{
			var p:Point = null;
			for(var i:int = 0; i< BagData.AllItems.length; i++) {
				for(var j:int = 0; j < BagData.AllItems[i].length; j++) {
					if(BagData.AllItems[i][j] == undefined) continue;
					if(type == BagData.AllItems[i][j].type)  {
						p = (BagData.GridUnitList[j].Grid.parent as MovieClip).localToGlobal(new Point(BagData.GridUnitList[j].Grid.x, BagData.GridUnitList[j].Grid.y));
						break;
					}
				}
			}
			return p;
		}
		
		/** 获取掌门名字宽度 */
		private function getCaptioinNameWidth():Object
		{
			var res:Object	  = {};
			res.taskId 		  = 0;
			res.width		  = 0;
			var geted:Boolean = false;
			
			var taskMediator:TaskMediator = facade.retrieveMediator(TaskMediator.NAME) as TaskMediator;
			var arr:Array = taskMediator.taskTree.dataProvider;
			for each(var group:TaskGroupStruct in arr) {
				var dic:Dictionary = group.taskDic;
				for(var id:* in dic) {
					if(NewerHelpData.TWO_LEN_TASK_ID.indexOf(id) >= 0) {
						res.taskId = id;
						res.width  = 44;
						geted = true;
						break;
					}
				}
				for(var id3:* in dic) {
					if(NewerHelpData.THREE_LEN_TASK_ID.indexOf(id3) >= 0) {
						res.taskId = id3;
						res.width  = 57; 
						geted = true;
						break;
					}
				}
				if(geted) break;
			}
			
			return res;
		}
		
	}
}