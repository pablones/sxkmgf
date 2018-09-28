package GameUI.Modules.CopyLead.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.CopyLead.Data.CopyLeadData;
	import GameUI.Modules.CopyLead.Data.CopyLeadEvent;
	import GameUI.Modules.GmTools.Utils.LoadSwfTool;
	import GameUI.Modules.NewPlayerSuccessAward.Data.NewAwardData;
	import GameUI.Modules.NewPlayerSuccessAward.Mediator.NewAwardMediator;
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CopyLeadMediator extends Mediator
	{
		public static const NAME:String = "CopyLeadMediator";
		
		private var loadswfTool:LoadSwfTool;
		private var panelBase:PanelBase;
		private var copyLeadView:MovieClip;
		/** 状态数据 */
		private var stateList:Array;
		/** 可否领取数据 */
		private var awardList:Array;
		private var timeSwitch:Boolean = false;
		/** 从服务器接受到的数据 */
		private var copyLeadData:Array = [];		
		private var intervalId:uint;
		private var _selectIndex:int;
		private var txtPlace:TaskText;
		private var btnCopyLead:SimpleButton = null;
		public static var SIMBOL_TAG:Boolean = true;	//发送消息请求标记
		
		private const POINT:Point = new Point(910, 405);	//new Point(930 , 445);		//奖励按钮的位置 
		public function CopyLeadMediator()
		{
			super(NAME);
		}
		public override function listNotificationInterests():Array
		{
			return [
					CopyLeadEvent.SHOWCOPYLEADVIEW,
					CopyLeadEvent.CLOSECOPYLEADVIEW,
					CopyLeadEvent.HANDLERDATA,
					CopyLeadEvent.SHOWCOPYLEADBTN,
					CopyLeadEvent.CLOSECOPYLEADBTN
			        ];
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				/** 显示副本引导按钮 */
				case CopyLeadEvent.SHOWCOPYLEADBTN:
					if(btnCopyLead != null) return;
					btnCopyLead = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("CopyLeadButton");
					btnCopyLead.name = "CopyLeadButton";
					GameCommonData.GameInstance.GameUI.addChildAt(btnCopyLead, 0); 
					if(NewAwardData.newPlayAwardBtnIsShow == false)
					{
						btnCopyLead.x = 965/**(facade.retrieveMediator(NewAwardMediator.NAME) as NewAwardMediator).POINT.x*/;
						btnCopyLead.y = (facade.retrieveMediator(NewAwardMediator.NAME) as NewAwardMediator).POINT.y;
					}
					else
					{
						btnCopyLead.x = 965/**POINT.x*/;
						btnCopyLead.y = POINT.y;
					}
					if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
			        {
			            btnCopyLead.x += GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
		            }
			        if( GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight )
			        {
			            btnCopyLead.y += GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
			        }
				    btnCopyLead.addEventListener(MouseEvent.CLICK , btnClickHandler);
				break;
				/** 关闭副本引导按钮 */
				case CopyLeadEvent.CLOSECOPYLEADBTN:
					
				break;
				
				case CopyLeadEvent.SHOWCOPYLEADVIEW:
					if(!panelBase)
					{
						loadswfTool = new LoadSwfTool(GameConfigData.CopyLeadSWF , this);
						loadswfTool.sendShow = sendShow;
					}
					else
					{
						sendShow(null);
					}
				break;
				case CopyLeadEvent.CLOSECOPYLEADVIEW:
					gcAll();
				break;
				case CopyLeadEvent.HANDLERDATA:
					stateList = getStateList((notification.getBody() as Array)[0]);
					stateList = stateList.slice(10 , stateList.length-1);
					if((notification.getBody() as Array).length != 1)
					{
						copyLeadData = notification.getBody() as Array;
						awardList = getStateList((notification.getBody() as Array)[1]);
						awardList = awardList.slice(11 , awardList.length);
					}
					//////////
					//控制副本引导按钮是否出现
					if(CopyLeadData.isShowButton == false)
					{
						for(var i:int = 0; i < stateList.length; i++)
						{
							//有一个未领取
							if(stateList[i] == 0)
							{
								CopyLeadData.isShowButton = true;
								break;
							}
						}
						if(CopyLeadData.isShowButton == true)
						{
							facade.sendNotification(CopyLeadEvent.SHOWCOPYLEADBTN);
							return;
						}
					}
					////////////////
				    if(loadswfTool == null)     
				    {
				    	return;
				    }
					timeSwitch = true;
					
					sendShow(copyLeadView);
				break;
			}
		}
		private function sendShow(mc:DisplayObject):void
		{
			if(CopyLeadData.copyLeadIsOpen == false)
			{
//				if(NewAwardData.newPlayAwardIsOpen == true) facade.sendNotification(NewAwardEvent.SHOWNEWPLAYERAWARD);		//关闭
				if(!copyLeadView)
				{
					copyLeadView = mc as MovieClip;
				}
				if(timeSwitch == false)
				{
					sendAction();				//发送请求，请求真数据
					return;
				}
				CopyLeadData.copyLeadIsOpen = true;
				//显示假数据
				initView();
				initData();
				addLis();
				if( GameCommonData.fullScreen == 2 )
				{
					panelBase.x = UIConstData.DefaultPos2.x - 400 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
					panelBase.y = UIConstData.DefaultPos2.y - 43 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
				}else{
					panelBase.x = UIConstData.DefaultPos2.x - 400;
					panelBase.y = UIConstData.DefaultPos2.y - 43;
				}
				GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			}
			else
			{
				gcAll();
			}
		}
		private function initView():void
		{
			if(!panelBase)
			{
				panelBase = new PanelBase(copyLeadView, copyLeadView.width - 10, copyLeadView.height + 14);
				panelBase.name = "copyLeadView";
				panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_cop_med_cop_ini" ]);//"玩法引导"
			}
		}
		/** 数据初始化显示 */
		private function initData():void
		{
			placeTextInit();			//需要小飞鞋的文本初始化
			handlerState();				//处理状态
			selectIndex = 0;			//默认选择第一条
		}
		private function placeTextInit():void
		{
			if(!txtPlace)
			{
				txtPlace = new TaskText(300);
				copyLeadView.mcContent.addChild(txtPlace);
				txtPlace.x = 13;
				txtPlace.y = 85;
			}
		}
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE , panelCloseHandler);
			copyLeadView.btnGet.addEventListener(MouseEvent.CLICK , getClickHandler);
			copyLeadView.btnClose.addEventListener(MouseEvent.CLICK , closeViewHandler);
			for(var i:int = 0; i < 11; i++)
			{
				copyLeadView["mcSelect_" + i].addEventListener(MouseEvent.CLICK , onSelectClick);
				copyLeadView["mcSelect_" + i].addEventListener(MouseEvent.MOUSE_MOVE , onSelectMove);
				copyLeadView["mcSelect_" + i].addEventListener(MouseEvent.MOUSE_OUT , onSelectOut);
			}
		}
		private function gcAll():void
		{
			CopyLeadData.copyLeadIsOpen = false;
			panelBase.removeEventListener(Event.CLOSE , panelCloseHandler);
			copyLeadView.btnClose.removeEventListener(MouseEvent.CLICK , closeViewHandler);
			for(var i:int = 0; i < 11; i++)
			{
				copyLeadView["mcSelect_" + i].removeEventListener(MouseEvent.CLICK , onSelectClick);
				copyLeadView["mcSelect_" + i].removeEventListener(MouseEvent.MOUSE_MOVE , onSelectMove);
			}
//			loadswfTool = null;
			intervalId = setTimeout(timeUp , 1000 * 10);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
		}
		private function panelCloseHandler(e:Event):void
		{
			gcAll();
		}
		private function closeViewHandler(e:MouseEvent):void
		{
			gcAll();
		}
		/** 点击领取 */
		private function getClickHandler(e:MouseEvent):void
		{
			sendGetAward(_selectIndex);
		}
		/** 点击选项 */
		private function onSelectClick(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			selectIndex = i;
			copyLeadView.mcContent.gotoAndStop(_selectIndex + 1);
		}
		/** 点击副本引导按钮 */
		private function btnClickHandler(e:MouseEvent):void
		{
			facade.sendNotification(CopyLeadEvent.SHOWCOPYLEADVIEW);
		}
		/** 鼠标经过选项 */
		private function onSelectMove(e:MouseEvent):void
		{
			for(var i:int = 0; i < 11; i++)
			{
				if(i == _selectIndex) continue;
				copyLeadView["mcSelect_" + i].gotoAndStop(1);
			}
			e.target.gotoAndStop(2);
		}
		/** 鼠标移除 */
		private function onSelectOut(e:MouseEvent):void
		{
			if(e.target.name.split("_")[1] == _selectIndex) return;
			e.target.gotoAndStop(1);
		}
		/** 10秒的时间限制 */
		private function timeUp():void
		{
			timeSwitch = false;
		}
		/** 处理状态 */
		private function handlerState():void
		{
			for(var i:int = 0; i < stateList.length; i++)
			{
				switch(stateList[i])
				{
					//未完成
					case 0:
						if(GameCommonData.Player.Role.Level >= copyLeadView.conditionList[i]) //进行中
						{
							if(awardList[i] == 1)											//进行中，可领取
							{
								(copyLeadView["txtTitle_" + i] as TextField).htmlText = copyLeadView.getList[i];//进行中，不可领取
							}
							else (copyLeadView["txtTitle_" + i] as TextField).htmlText = copyLeadView.onList[i];
						}
						else 				//未开启
						{
							(copyLeadView["txtTitle_" + i] as TextField).htmlText = copyLeadView.unFinishList[i];
							copyLeadView["mcHead_" + i].visible = false;					//未开启时头像为问号
							copyLeadView.btnGet.visible = false;
							copyLeadView.txtGet.visible = false;
						}
					break;
					//完成
					case 1:
						(copyLeadView["txtTitle_" + i] as TextField).htmlText = copyLeadView.finishList[i];
						copyLeadView.btnGet.visible = false;
						copyLeadView.txtGet.visible = false;
					break;
				}
				
			}
		}
		private function sendAction():void
		{
			SIMBOL_TAG = true;
			var obj:Object = new Object();
			obj.type = 1010;
			obj.data = [0 , 0 , 0 , 0 , 0 , 0 , 284, 0 , 0];
			PlayerActionSend.PlayerAction(obj);
		}
		/** 得到状态数组,二进制*/
		private function getStateList(num:int):Array
		{
			var list:Array = [];
			var x:int;
			for(var i:int = 0 ; i < 22 ;i++)
			{
				list.push(int(num % 2));
				num = num / 2; 
				if(num == 0) break;
			}
			while(list.length < 22)
			{
				list[list.length] = 0;
			}
			return list;
		}
		private function set selectIndex(vaule:int):void
		{
			_selectIndex = vaule;
			for(var i:int = 0; i < 11; i++)
			{
				copyLeadView["mcSelect_" + i].gotoAndStop(1);
			}
			this.txtPlace.tfText = copyLeadView.npcPlaceList[vaule];
			copyLeadView["mcSelect_" + vaule].gotoAndStop(2);
			if(awardList[vaule] == 1 && stateList[vaule] == 0)
			{
				copyLeadView.btnGet.visible = true;
				copyLeadView.txtGet.visible = true;
				copyLeadView.mcContent.visible = true;
				copyLeadView.txtUnstart.visible = false;
			}
			else if(awardList[vaule] == 0 && stateList[vaule] == 0 && GameCommonData.Player.Role.Level < copyLeadView.conditionList[vaule])
			{
				copyLeadView.mcContent.visible = false;
				copyLeadView.txtUnstart.visible = true;
			}
			else
			{
				copyLeadView.txtUnstart.visible = false;
				copyLeadView.mcContent.visible = true;
				copyLeadView.btnGet.visible = false;
				copyLeadView.txtGet.visible = false;
			}
			if(stateList[vaule] == 1)
			{
				copyLeadView.mcContent.txtFinish.visible = true;
			}
			else
			{
				copyLeadView.mcContent.txtFinish.visible = false;
			}
		}
		private function get selectIndex():int
		{
			return _selectIndex;
		}
		/** 发送领取奖励请求 */
		public function sendGetAward(type:int):void
		{
			if(BagData.canPushGroupBag(copyLeadView.awardItemList[type]) == true) 
			{
//				 facade.sendNotification(HintEvents.RECEIVEINFO, {info:"背包已满，请清理背包", color:0xffff00});
				return;
			}
			SIMBOL_TAG = true;
			var obj:Object = new Object();
			obj.type = 1010;
			obj.data = [0 , 0 , 0 , 0 , 0 , 0 , 285, int(type+11) , 0];
			PlayerActionSend.PlayerAction(obj);
		}
	}
}