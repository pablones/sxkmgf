package GameUI.Modules.NewPlayerSuccessAward.Mediator
{
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardData;
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardEvent;
	import GameUI.Modules.NewPlayerSuccessAward.ViewInit.ViewInit;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewAwardMediator extends Mediator
	{
		public static const NAME:String = "NewAwardMediator";
		
		private var newAwardView:MovieClip;			
		private var awardButton:SimpleButton;					//视图按钮	
		private var panelBase:PanelBase;	
		private var newAwardData:Array; 
		private var isFirstGetData:Boolean = true;				//判断是否是第一次得到数据，如果是，则不弹出面板，否则弹出
		private var intervalId:uint;
		private var selectType:int;								//选择目标的序列
		private var playingList:Array = [0,0,0,0,0,0,0,0,0,0];	//进行中数组，进行为TRUE
		private var gameInit:ViewInit
		public const POINT:Point =  new Point(965 , 445);//new Point(910, 445);			//奖励按钮的位置  
		public static var SIMBOL_TAG:Boolean;	//发送消息请求标记
		
		private var timeSwitch:Boolean = false;
		public function NewAwardMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					NewAwardEvent.VIEWINIT,
					NewAwardEvent.SHOWNEWPLAYERAWARD,
					NewAwardEvent.DENY_DRAG_PANEL,	//禁止拖动面板
					NewAwardEvent.HANDLERDATA
					];
		} 
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case NewAwardEvent.VIEWINIT:
					if(newAwardView == null)	gameInit = new ViewInit();			
					else facade.sendNotification(NewAwardEvent.SHOWNEWPLAYERAWARD);
				break;
				case NewAwardEvent.SHOWNEWPLAYERAWARD:
					if(newAwardView == null) newAwardView = notification.getBody() as MovieClip;
					if(NewAwardData.newPlayAwardIsOpen == false)
					{
//						if(CopyLeadData.copyLeadIsOpen == true) facade.sendNotification(CopyLeadEvent.CLOSECOPYLEADVIEW);		//关闭副本引导
						if(timeSwitch == false){
							sendAction();				//发送请求，请求真数据
							return;
						}
						//显示假数据
						timeSwitch = true;
						showView();
						addLis();
						newAwardView.showData(newAwardData);			//显示数据,传递数据，和判断是否达到目标的方法
						handlerState();
						
					}
					else
					{
						gcAll();
					}
				break;
				case NewAwardEvent.HANDLERDATA:
//					if(CopyLeadData.copyLeadIsOpen == true) return;
					if((notification.getBody() as Array).length != 1)
					{
						newAwardData = notification.getBody() as Array;
					}
					newAwardData[0] = getStateList((notification.getBody() as Array)[0]);
					if(newAwardData[1] < 720 * 2 * 18) showButton();		//显示打开视图的奖励按钮
					else return;
					if(isFirstGetData == false && NewAwardData.newPlayAwardIsOpen == false)		//关闭面板 且 不是第一次接受数据 才会打开面板
					{
						isLoadedComplete(true);
						timeSwitch = true;
						if(newAwardView == null) return;
						showView();
						addLis();
						newAwardView.showData(newAwardData);			//显示数据,传递数据，和判断是否达到目标的方法
						handlerState();
						//通知新手引导
//						if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.OPEN_NEWER_AWARD_NEWER_HELP);
					}
					else if(NewAwardData.newPlayAwardIsOpen == true)
					{
						newAwardView.showData(newAwardData);			//显示数据,传递数据，和判断是否达到目标的方法
						handlerState();
					}
					isFirstGetData = false;
				break;
				case NewAwardEvent.DENY_DRAG_PANEL:		//禁止拖动面板
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = 222 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						panelBase.y = 28 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}else{
						panelBase.x = 222;
						panelBase.y = 28;
					}
					panelBase.IsDrag = false;		
				break;
			}
		}
		
		private function showView():void
		{
			NewAwardData.newPlayAwardIsOpen = true;
			panelBase = new PanelBase(newAwardView, 650, 454);
			panelBase.name = "unity";
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = 222 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = 28 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = 222;
				panelBase.y = 28;
			}
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_newerPS_med_new_show" ]);//"新手成就大礼包"	
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
//			initIsAward(selectType);								//是否可以领奖
			handlerState();
		}
		
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			for(var i:int = 0; i<10; i++)
			{
				newAwardView["mcSelect_" + i].addEventListener(MouseEvent.CLICK , selectClickHandler);
				newAwardView["mcSelect_" + i].addEventListener(MouseEvent.MOUSE_OVER , selectOverHandler);
				newAwardView["mcSelect_" + i].addEventListener(MouseEvent.MOUSE_OUT, selectOutHandler);
			}
			newAwardView.btnGc.addEventListener(MouseEvent.CLICK , gcHandler);
			newAwardView.btnGetAward.addEventListener(MouseEvent.CLICK , getAwardHandler);
		}
		
		private function gcHandler(e:MouseEvent):void
		{
			gcAll();
		}
		/** 点击选择条 */
		private function selectClickHandler(e:MouseEvent):void
		{
			selectType = e.target.name.split("_")[1];
			selectMcInit();
			(e.target as MovieClip).gotoAndStop(3);
			newAwardView.mcAward.gotoAndStop(selectType+1);
			
			newAwardView.currentTime = selectType;
			this.showTxt();			//换页后改变时间文本
			newAwardView.mcAward.txtTime.visible = this.playingList[selectType];		//时间限制是否可视;
			initIsAward(selectType);
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_ITEM_NEWER_AWARD_NEWER_HELP, selectType);
		}
		private function selectOverHandler(e:MouseEvent):void
		{
			if((e.target as MovieClip).currentFrame == 3) return;
			(e.target as MovieClip).gotoAndStop(2);
		}
		private function selectOutHandler(e:MouseEvent):void
		{
			if((e.target as MovieClip).currentFrame == 3) return;
			(e.target as MovieClip).gotoAndStop(1);
		}
		/** 点击领取奖励 */
		private function getAwardHandler(e:MouseEvent):void
		{
			sendGetAward(int(selectType + 1));
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_GET_NEWER_AWARD_NEWER_HELP);
		}
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		/** 选择条初始化 */
		private function selectMcInit():void
		{
			for(var i:int = 0; i<10; i++)
			{
				newAwardView["mcSelect_" + i].gotoAndStop(1);
			}
			newAwardView.txtGetAward.mouseEnabled = false;
		}
		
		private function gcAll():void
		{
			NewAwardData.newPlayAwardIsOpen = false;
			selectType = 0;
			gameInit = null;
			clearTimeout(intervalId);
			intervalId = setTimeout(timeUp , 1000 * 10);
//			newAwardView = null;
			panelBase.IsDrag = true;		
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLOSE_NEWER_AWARD_NEWER_HELP);
		}
		/** 是否可以领取奖励 */
		private function initIsAward(type:int):void
		{
			//领取按钮是否可视
			if(targetIsGet(type) == true && this.playingList[type] == 1)
			{
				newAwardView.btnGetAward.visible = true;
				//通知新手引导
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.OPEN_NEWER_AWARD_NEWER_HELP);
			}
			else
			{
				newAwardView.btnGetAward.visible = false;
			}
			//灰色按钮是否可视
			if(newAwardView["txtState_" + type].text == GameCommonData.wordDic[ "mod_newerPS_med_new_ini_1" ] || newAwardView["txtState_" + type].text == GameCommonData.wordDic[ "mod_newerPS_med_new_ini_2" ] || newAwardView["txtState_" + type].text == GameCommonData.wordDic[ "mod_newerPS_med_new_ini_3" ])
			//"已完成"		"已过期"		"未开启"
			{
				newAwardView.grayBtnVisible(false);
			}
			else newAwardView.grayBtnVisible(true);
		}
		/** 发送获取数据的请求*/
		private function sendAction():void
		{
			SIMBOL_TAG = true;
			isLoadedComplete(false); 
			var obj:Object = new Object();
			obj.type = 1010;
			obj.data = [0 , 0 , 0 , 0 , 0 , 0 , 284, 0 , 0];
			PlayerActionSend.PlayerAction(obj);
//			if(awardButton) awardButton.removeEventListener(MouseEvent.CLICK , showHandler);
		}
		/** 得到状态数组,二进制*/
		private function getStateList(num:int):Array
		{
			var list:Array = [];
			var x:int;
			for(var i:int = 0 ; i < 10 ;i++)
			{
				list.push(int(num % 2));
				num = num / 2; 
				if(num == 0) break;
			}
			while(list.length < 10)
			{
				list[list.length] = 0;
			}
//			list.sort(Array.DESCENDING);
			return list;
		}
		/** 判断是否达成目标*/
		public function targetIsGet(type:int):Boolean
		{
			var isGet:Boolean = false;
			switch(type)
			{
				case 0:
					if(GameCommonData.Player.Role.Level >= 15)	isGet = true;
				break;
				case 1:
					if(GameCommonData.Player.Role.Level >= 26)	isGet = true;
				break;
				case 2:
					if(GameCommonData.Player.Role.Level >= 37)	isGet = true;
				break;
				case 3:
					if(GameCommonData.Player.Role.ViceJob.Job > 0) isGet = true;
				break;
				case 4:
					if((facade.retrieveMediator(FriendManagerMediator.NAME) as Object).getFriendNum() > 20) isGet = true;
				break;
				case 5:
					if(GameCommonData.wordVersion != 2)
					{
						if(newAwardData[2] > 720 * 6) isGet = true;
					}
					else
					{
						if(getAttack() > 5000)  isGet = true;
					}
				break;
				case 6:
					if(getAttack() > 6000)  isGet = true;
				break;
				case 7:
					if(getHaveEquipList() >= 3)  isGet = true;
				break;
				case 8:
					if(judgePetInfo() == true) isGet = true;
				break;
				case 9:
					if(GameCommonData.Player.Role.Level >= 55)	isGet = true;
				break;
			}
			return isGet;
		}
		/** 得到内功外功*/
		private function getAttack():uint{
			if(GameCommonData.Player.Role.CurrentJob==1){
				return Math.max(GameCommonData.Player.Role.MainJob.PhyAttack,GameCommonData.Player.Role.MainJob.MagicAttack);
			}else{
				return Math.max(GameCommonData.Player.Role.ViceJob.PhyAttack,GameCommonData.Player.Role.ViceJob.MagicAttack)
			}
			
		}
		/** 查询是否拥有50级套装 */
		private function getHaveEquipList():int
		{
			var equipNum:int = 0;
			var dfd:Array = RolePropDatas.ItemList;
			var er:Array = NewAwardData.CoordinatesEquipList;
			for(var n:int = 0; n < NewAwardData.CoordinatesEquipList.length ;n++)
			{
				for(var i:int = 0 ; i < 15 ; i++)
				{
					for(var k:int = 0 ; k < NewAwardData.CoordinatesEquipList[n].length; k++)
					{
						if(RolePropDatas.ItemList[i] == null) break;
						 if(RolePropDatas.ItemList[i].type == NewAwardData.CoordinatesEquipList[n][k])
						 {
						 	equipNum ++;
						 }
					}

				}
			}
			return equipNum;
			
		}
		
		/** 查询宠物信息 */
		private function judgePetInfo():Boolean
		{
			var ret:Boolean = false;
			if(GameCommonData.Player.Role.UsingPet) {
				if(GameCommonData.Player.Role.PetSnapList[GameCommonData.Player.Role.UsingPet.Id].Level >= 50 && GameCommonData.Player.Role.PetSnapList[GameCommonData.Player.Role.UsingPet.Id].SkillLevel.length >= 6) {
					ret = true;
				} 
			}
			return ret;
		}
		/** 发送领取奖励请求 */
		public function sendGetAward(type:int):void
		{
			if(BagData.canPushGroupBag(NewAwardData.awardItemList[type-1]) == true) 
			{
//				 facade.sendNotification(HintEvents.RECEIVEINFO, {info:"背包已满，请清理背包", color:0xffff00});
				return;
			}
			SIMBOL_TAG = true;
			var obj:Object = new Object();
			obj.type = 1010;
			obj.data = [0 , 0 , 0 , 0 , 0 , 0 , 285, type , 0];
			PlayerActionSend.PlayerAction(obj);
		}
		/** 10秒的时间限制 */
		private function timeUp():void
		{
			timeSwitch = false;
		}
		/** 显示打开视图的按钮*/
		private function showButton():void
		{
			if(awardButton == null)
			{
				awardButton = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("NewPlayerAwardButton");
				GameCommonData.GameInstance.GameUI.addChild(awardButton);
				awardButton.name = "NewPlayerAwardButton";
				awardButton.x = POINT.x;
				awardButton.y = POINT.y;
			    if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
			    {
			        awardButton.x = POINT.x + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
		        }
			    if( GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight )
			    {
			        awardButton.y = POINT.y + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
			    }
				
				NewAwardData.newPlayAwardBtnIsShow = true;		//按钮显示的状态
				awardButton.addEventListener(MouseEvent.CLICK , showHandler);
			}
		}
		/** 点击新手大礼包按钮 */
		private function showHandler(e:MouseEvent):void
		{
			facade.sendNotification(NewAwardEvent.VIEWINIT);
//			sendNotification(EventList.PLAY_SOUND_OPEN_PANEL); 
		}
		/** 设置是否加载完成*/
		public function isLoadedComplete(isLoaded:Boolean):void
		{
			if(isLoaded == false)
			{
				if(awardButton.hasEventListener(MouseEvent.CLICK)) awardButton.removeEventListener(MouseEvent.CLICK , showHandler);
			} 
			else
			{
				awardButton.addEventListener(MouseEvent.CLICK , showHandler);
			}
		}
		/** 处理状态 */
		private function handlerState():void
		{
			for(var i:int = 0; i < newAwardData[0].length ; i++)
			{
				if(newAwardData[0][i] == 1) 		//任务已完成
				{
					txtClocr(i , 0x00CBFF ,GameCommonData.wordDic[ "mod_newerPS_med_new_ini_1" ]);//"已完成"
					playingList[i] = 0;
				}
				else
				{
					if(newAwardData.length == 1) continue;		//如果领取成功只发掩码，数组就只有一个长度
					if(newAwardData[1] > int(newAwardView.timeList[i] + newAwardView.startTimeList[i]))	//已过期
					{
						txtClocr(i , 0x666666 ,GameCommonData.wordDic[ "mod_newerPS_med_new_ini_2" ] );//"已过期"
						playingList[i] = 0;
					}
					else if(newAwardData[1] < newAwardView.startTimeList[i])	//未开启
					{
						txtClocr(i , 0xFFFFFF ,GameCommonData.wordDic[ "mod_newerPS_med_new_ini_3" ] );//"未开启"
						newAwardView["txtState_" + i].textColor = 0xFF0000;
						playingList[i] = 0;
					}
					else
					{
						txtClocr(i , 0x00FF00 ,GameCommonData.wordDic[ "mod_newerPS_med_new_han" ] );//"进行中"
						playingList[i] = 1;
					}
				}
				newAwardView.mcAward.txtTime.visible = playingList[selectType];		//时间限制是否可视;
				initIsAward(selectType);									//默认第一个
				this.showTxt();
			}
		}
		/** 文本颜色处理 , 领取奖励(灰色)是否可视 */
		private function txtClocr(i:int , clocr:uint , stateStr:String):void
		{
			newAwardView["txtName_" + i].textColor = clocr;
			newAwardView["txtState_" + i].textColor = clocr;
			newAwardView["txtState_" + i].text = stateStr;
		}
		/** 显示文本 */
		private function showTxt():void
		{
			var time:int = newAwardView.startTimeList[selectType] + newAwardView.timeList[selectType] - newAwardData[1];
			newAwardView.mcAward.txtTime.text = GameCommonData.wordDic[ "med_lost_5" ]+"：" + getVipTime(time);//"剩余时间
		}
		private function getVipTime(time:int):String
		{
			if (time<=0)
			{
				return GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ];//"无";
			}
			var dayStr:String = ( int(time/(720*2)) ).toString();
			var hourStr:String = ( int((time%(720*2))/60)).toString();
			var miniteStr:String = ( int( (time%(720*2))%60 ) ).toString();
			if ( dayStr == "0" )
			{
				if ( hourStr == "0" )
				{
					return miniteStr+GameCommonData.wordDic[ "mod_newerPS_med_new_show_1" ];//"分";
				}
						else
				{
					return hourStr+GameCommonData.wordDic[ "mod_newerPS_med_new_show_2" ]+miniteStr+GameCommonData.wordDic[ "mod_newerPS_med_new_show_1" ];//"分";		"时"
				}
			}
			else
			{
				return dayStr+GameCommonData.wordDic[ "mod_newerPS_med_new_show_3" ]+hourStr+GameCommonData.wordDic[ "mod_newerPS_med_new_show_2" ]+miniteStr+GameCommonData.wordDic[ "mod_newerPS_med_new_show_1" ];//"分";		"天"		"时"	
			}
		}
	}
}