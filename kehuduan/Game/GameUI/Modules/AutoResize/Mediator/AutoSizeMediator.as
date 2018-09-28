package GameUI.Modules.AutoResize.Mediator
{
	import Controller.FlutterController;
	import Controller.SceneController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Arena.Mediator.ArenaPanelMediator;
	import GameUI.Modules.AutoPlay.mediator.OffLinePlayMediator;
	import GameUI.Modules.AutoResize.Data.AutoSizeData;
	import GameUI.Modules.Buff.Mediator.BuffMediator;
	import GameUI.Modules.ChangeLine.ChangeLineMediator;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Mediator.ArenaMsgMediator;
	import GameUI.Modules.Chat.Mediator.ChatMediator;
	import GameUI.Modules.Chat.Mediator.MsgMedaitor;
	import GameUI.Modules.Chat.Mediator.ScrollNoticeMediator;
	import GameUI.Modules.Friend.view.mediator.FriendChatMediator;
	import GameUI.Modules.MainSence.Mediator.MainSenceMediator;
	import GameUI.Modules.Maket.Mediator.MarketMediator;
	import GameUI.Modules.Map.SmallMap.Mediator.SmallMapMediator;
	import GameUI.Modules.OnlineGetReward.Mediator.OnlineRewardMediator;
	import GameUI.Modules.Task.Mediator.TaskFollowMediator;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownData;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.AutoPanelBase;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class AutoSizeMediator extends Mediator
	{
		public static const NAME:String = "AutoSizeMediator";
		private var dataProxy:DataProxy;
		private var mainSceneMed:MainSenceMediator;
		private var smallMapMed:SmallMapMediator;
		private var onlineRewardMed:OnlineRewardMediator;
		private var chatMed:ChatMediator;
		private var taskFollowMed:TaskFollowMediator;
		private var changeLineMed:ChangeLineMediator;
		private var msgMed:MsgMedaitor;
		private var BuffMed:BuffMediator;
		private var friendChatMed:FriendChatMediator;
		private var scrollNoticeMed:ScrollNoticeMediator;
		private var offLinePlayMed:OffLinePlayMediator;
		private var arenaPanelMed:ArenaPanelMediator;
		private var marketMed:MarketMediator;
		private var mainSence:MovieClip;
		
		private const DEFAULT_POS:Point=new Point(780,208);
		private const FOLDED_POS:Point = new Point(779,210);
		private var length:Number;
		private var distance1:Number;
		private var distance2:Number;
		private var distance3:Number;
		private var distance4:Number;
		private var distance5:Number;
		private var distance6:Number;
		private var distance7:Number;
		private var mainScenePoint:Point;
		
		private var oldStageWidth:Number = GameConfigData.GameWidth;
		private var oldStageHeight:Number = GameConfigData.GameHeight;
		
		
		public function AutoSizeMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
						EventList.INITVIEW,
						EventList.STAGECHANGE,
						EventList.CHANGE_UI,
						AutoSizeData.MSG_MED_CREATE_OVER
				   ];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					initMediator();
				break;
				case EventList.STAGECHANGE:
				    change_All_UI();
				break;
				case EventList.CHANGE_UI:
				    var type:uint = notification.getBody() as uint;
				    changeUI(type);
				break;
				case AutoSizeData.MSG_MED_CREATE_OVER:
					msgMed = facade.retrieveMediator( MsgMedaitor.NAME ) as MsgMedaitor;
				break;
			}
		}
		
		private function initMediator():void
		{
			mainSceneMed = facade.retrieveMediator( MainSenceMediator.NAME ) as MainSenceMediator;
			smallMapMed = facade.retrieveMediator( SmallMapMediator.NAME ) as SmallMapMediator;
			onlineRewardMed = facade.retrieveMediator( OnlineRewardMediator.NAME ) as OnlineRewardMediator;
			chatMed = facade.retrieveMediator( ChatMediator.NAME ) as ChatMediator;
			taskFollowMed = facade.retrieveMediator( TaskFollowMediator.NAME ) as TaskFollowMediator;
			changeLineMed = facade.retrieveMediator( ChangeLineMediator.NAME ) as ChangeLineMediator;
			BuffMed = facade.retrieveMediator( BuffMediator.NAME ) as BuffMediator;
			friendChatMed = facade.retrieveMediator( FriendChatMediator.NAME ) as FriendChatMediator;
			scrollNoticeMed = facade.retrieveMediator( ScrollNoticeMediator.NAME ) as ScrollNoticeMediator;
			offLinePlayMed = facade.retrieveMediator( OffLinePlayMediator.NAME ) as OffLinePlayMediator;
			marketMed = facade.retrieveMediator( MarketMediator.NAME ) as MarketMediator;
//			arenaPanelMed = facade.retrieveMediator( ArenaPanelMediator.NAME ) as ArenaPanelMediator;
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			mainSence = mainSceneMed.mainSence as MovieClip;
			
			//游戏舞台自适应			
			length = mainSence.btn_0.x - mainSence.mcQuickBar0.x;
			distance1 = mainSence.mcQuickBar0.x - mainSence.mcQuickBar1.x;
			distance2 = mainSence.mcQuickBar0.x - mainSence.btnQuickLanUp.x;
			distance3 = mainSence.mcQuickBar0.x - mainSence.mcExp.x;
			distance4 = mainSence.mcQuickBar0.x - mainSence.btnQuickLanDown.x;
			distance5 = mainSence.mcQuickBar0.x - mainSence.mcExp2.x;
			distance6 = mainSence.mcQuickBar0.x - mainSence.mcExp3.x;
			distance7 = mainSence.mcQuickBar0.x - mainSence.mcExp4.x;
			mainScenePoint = new Point(mainSence.x, mainSence.y);
		}
		
		private function change_All_UI():void
		{
			changeMainScene();
			changeSmallMap();
			changeOnlineAward();
			changeChat();
			changeTaskFollow();
			changeLine();
			changeBuff();
			changeFriendChat();
			changeScrollNotice();
			changeOffLinePlay();
			changeMarket();
			changeMap();
			changePanelBase();
			changeTimeView();
			changeSceneVerse();
			changeMusicPlayer();
			changeArenaPanel();
			if( msgMed ) changeMsg();
			changeDragRect();
			if( GameCommonData.Player.GameX && GameCommonData.Player.GameY ) GameCommonData.Scene.SetScenePos();
			this.oldStageWidth = GameCommonData.GameInstance.GameUI.stage.stageWidth;
			this.oldStageHeight = GameCommonData.GameInstance.GameUI.stage.stageHeight;
			
			//角色居中
			if(FlutterController.roseMC != null)
			{
				FlutterController.roseMC.x =	GameCommonData.GameInstance.GameUI.stage.stageWidth / 2 - FlutterController.roseMC.width/2;
			}
		}
		
		private function changeUI(type:uint):void
		{
			switch( type )
			{
				case 1:
					changeTaskFollow();
				break;
				case 2:
				    changeMsg();
				break;
			}
		}
		
		private function changeArenaPanel():void
		{
			var arenaPanelMdtr:ArenaPanelMediator = facade.retrieveMediator( ArenaPanelMediator.NAME ) as ArenaPanelMediator;
			if (arenaPanelMdtr)
			{
				var arenaPanel:MovieClip = arenaPanelMdtr.panelSmall;
				if( arenaPanel && GameCommonData.GameInstance.GameUI.contains( arenaPanel ) )
				{
					arenaPanel.x = 853 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
				}
			}
			
			var arenaMsgMdtr:ArenaMsgMediator = facade.retrieveMediator( ArenaMsgMediator.NAME) as ArenaMsgMediator;
			if (arenaMsgMdtr)
			{
				var arenaMsgPanel:MovieClip = arenaMsgMdtr.panel;
				if (arenaMsgPanel && GameCommonData.GameInstance.GameUI.contains( arenaMsgPanel ))
				{
					arenaMsgPanel.x = 747 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth; 
				}
			}
		}
		
		private function changeSceneVerse():void
		{
			var sceneController:SceneController = GameCommonData.Scene;
			if( sceneController.SceneVerse != null )
			{
				sceneController.SceneVerse.x = (GameCommonData.GameInstance.ScreenWidth  - sceneController.SceneVerse.width)  / 2;
//				sceneController.SceneVerse.y = (GameCommonData.GameInstance.ScreenHeight - sceneController.SceneVerse.height) / 2 - 170;;
			}
		}
		
		private function changeMainScene():void
		{		
			if(mainSence.stage.stageWidth <= GameConfigData.GameWidth)
			{
//				mainSence.x = mainSence.stage.stageWidth - GameConfigData.GameWidth;
				mainSence.mcQuickBar0.x = mainSence.btn_0.x - length;
				mainSence.btnQuickLanUp.x = mainSence.mcQuickBar0.x - distance2;
				mainSence.mcExp.x = mainSence.mcQuickBar0.x - distance3;
				mainSence.mcExp2.x = mainSence.mcQuickBar0.x - distance5;
				mainSence.mcExp3.x = mainSence.mcQuickBar0.x - distance6;
				mainSence.mcExp4.x = mainSence.mcQuickBar0.x - distance7;
				mainSence.btnQuickLanDown.x = mainSence.mcQuickBar0.x - distance4;
				mainSence.mcQuickBar1.x = mainSence.mcQuickBar0.x - distance1;
//				mainSence.x = GameConfigData.GameWidth - mainSence.width - 3;
				mainSence.x = mainScenePoint.x + mainSence.stage.stageWidth - GameConfigData.GameWidth;
				
				if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null)
			        GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton").x = 965 + mainSence.stage.stageWidth - GameConfigData.GameWidth; //910
			    if(GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton") != null)
			        GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton").x = 965 + mainSence.stage.stageWidth - GameConfigData.GameWidth;  //910
			}
			if(mainSence.stage.stageHeight <= GameConfigData.GameHeight)
			{
//				mainSence.y = GameConfigData.GameHeight - mainSence.height - 93; 
				mainSence.y = mainScenePoint.y + mainSence.stage.stageHeight - GameConfigData.GameHeight;
				if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null)
				    GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton").y = 445 + mainSence.stage.stageHeight - GameConfigData.GameHeight;
				if(GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton") != null)
				    GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton").y = 405 + mainSence.stage.stageHeight - GameConfigData.GameHeight;
			}
			if( mainSence.stage.stageWidth > GameConfigData.GameWidth )
			{
			    mainSence.x = mainSence.stage.stageWidth - GameConfigData.GameWidth;
			
			    var globalX:Number = (mainSence.stage.stageWidth - 3)/2 - mainSence.mcQuickBar0.width/2;
				var _point:Point = mainSence.globalToLocal(new Point(globalX, 10));
				mainSence.mcQuickBar0.x = _point.x;
			
				mainSence.mcQuickBar1.x = mainSence.mcQuickBar0.x - distance1;
				mainSence.btnQuickLanUp.x = mainSence.mcQuickBar0.x - distance2;
				mainSence.mcExp.x = mainSence.mcQuickBar0.x - distance3;
				mainSence.btnQuickLanDown.x = mainSence.mcQuickBar0.x - distance4;
				mainSence.mcExp2.x = mainSence.mcQuickBar0.x - distance5;
				mainSence.mcExp3.x = mainSence.mcQuickBar0.x - distance6;
				mainSence.mcExp4.x = mainSence.mcQuickBar0.x - distance7;
			    if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null)
			        GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton").x = 965/** 910 */ + mainSence.stage.stageWidth - GameConfigData.GameWidth;
			    if(GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton"))
			        GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton").x = 965/** 910 */ + mainSence.stage.stageWidth - GameConfigData.GameWidth;
		    }
			if( mainSence.stage.stageHeight > GameConfigData.GameHeight )
			{
			    mainSence.y = mainSence.stage.stageHeight - GameConfigData.GameHeight;
			    if(GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton") != null)
			        GameCommonData.GameInstance.GameUI.getChildByName("NewPlayerAwardButton").y = 445 + mainSence.stage.stageHeight - GameConfigData.GameHeight;
			    if(GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton") != null)
			        GameCommonData.GameInstance.GameUI.getChildByName("CopyLeadButton").y = 405  + mainSence.stage.stageHeight - GameConfigData.GameHeight;
			}
			(this.mainSence["btn_expandTeam_up"] as SimpleButton).x = 5 + GameConfigData.GameWidth - mainSence.stage.stageWidth;
			(this.mainSence["btn_expandTeam_down"] as SimpleButton).x = 5 + GameConfigData.GameWidth - mainSence.stage.stageWidth;
			(this.mainSence["btn_expandTeam_up"] as SimpleButton).y = 95 + GameConfigData.GameHeight - mainSence.stage.stageHeight;
			(this.mainSence["btn_expandTeam_down"] as SimpleButton).y = 95 + GameConfigData.GameHeight - mainSence.stage.stageHeight;
		}
		
		private function changeSmallMap():void
		{
			var SmallMap:MovieClip = smallMapMed.SmallMap as MovieClip;
			
			if (!(SmallMap && SmallMap.stage)) return;
			
			var xPos:int;
			
			if ( SmallMapMediator.isFullScreen )
			{
				xPos = 830;
			}
			else
			{
				xPos = 860;
			}
			
			if (GameCommonData.isExe)
			{
				xPos += 15;
			}
			
//			if(SmallMap.stage.stageWidth <= GameConfigData.GameWidth)
//			{
//				SmallMap.x = 0; 
//				GameCommonData.GameInstance.GameUI.getChildByName("btn_SoundOn").x = xPos;  //830
//				GameCommonData.GameInstance.GameUI.getChildByName("btn_SoundOff").x = xPos;   //830
//			}
//			
//			if( SmallMap.stage.stageWidth > GameConfigData.GameWidth )
//			{
			    SmallMap.x = SmallMap.stage.stageWidth - GameConfigData.GameWidth; 
			    GameCommonData.GameInstance.GameUI.getChildByName("btn_SoundOn").x = xPos + SmallMap.stage.stageWidth - GameConfigData.GameWidth;
				GameCommonData.GameInstance.GameUI.getChildByName("btn_SoundOff").x = xPos + SmallMap.stage.stageWidth - GameConfigData.GameWidth;
//		    }
		}
		
		private function changeOnlineAward():void
		{
			var onLineAwardView:MovieClip = onlineRewardMed.onLineAwardView as MovieClip;
			
			if (!(onLineAwardView && onLineAwardView.stage)) return;
			
			if ( GameCommonData.GameInstance.GameUI.contains( onLineAwardView ) )
	        {
//		        if(onLineAwardView.stage.stageWidth <= GameConfigData.GameWidth)
//				{
//					onLineAwardView.x = 400;  
//				}
//				if(onLineAwardView.stage.stageHeight <= GameConfigData.GameHeight)
//				{
//					onLineAwardView.y = 496;
//				}
//				if( onLineAwardView.stage.stageWidth > GameConfigData.GameWidth )
//				{
				    onLineAwardView.x = 400 + (onLineAwardView.stage.stageWidth - GameConfigData.GameWidth)/2; 
//			    }
//				if( onLineAwardView.stage.stageHeight > GameConfigData.GameHeight )
//				{
				    onLineAwardView.y = 496 + onLineAwardView.stage.stageHeight - GameConfigData.GameHeight;
//				}
	        } 
			
		}
		
		private function changeChat():void
		{
			var chatView:MovieClip = chatMed.chatView as MovieClip;
			
			if (!(chatView && chatView.stage)) return;
			
//			if(chatView.stage.stageHeight <= GameConfigData.GameHeight)
//			{
//				chatView.y = 0;  
//			}
//			
//			if( chatView.stage.stageHeight > GameConfigData.GameHeight )
//			{
				GameCommonData.GameInstance.GameUI.addChildAt(chatView, 0);
			    chatView.y = chatView.stage.stageHeight - GameConfigData.GameHeight; 
//		    }
		}
		
		private function changeTaskFollow():void
		{
			var taskFollowUI:MovieClip = taskFollowMed.taskFollowUI as MovieClip;
			
			if (!(taskFollowUI && taskFollowUI.stage)) return;
			
//			if(taskFollowUI.stage.stageWidth <= GameConfigData.GameWidth)
//			{
//				if( dataProxy.TaskFollowIsOpen == true )
//				{
//					taskFollowUI.x=DEFAULT_POS.x;
//					return;
//				}
//				taskFollowUI.x=FOLDED_POS.x;
//			}
//			if( taskFollowUI.stage.stageWidth > GameConfigData.GameWidth )
//			{
			    if( taskFollowUI && GameCommonData.GameInstance.GameUI.contains(taskFollowUI) )
				{
					taskFollowUI.x=DEFAULT_POS.x + taskFollowUI.stage.stageWidth - GameConfigData.GameWidth;
				}
//				taskFollowUI.x=DEFAULT_POS.x + taskFollowUI.stage.stageWidth - GameConfigData.GameWidth;
//		    }
		}
		
		private function changeLine():void
		{
			var changeLineView:MovieClip = changeLineMed.changeLineView as MovieClip;
			
			if (!(changeLineView && changeLineView.stage)) return;
			
//			if(GameCommonData.GameInstance.WorldMap.stage.stageWidth <= GameConfigData.GameWidth)
//			{
//				if ( GameCommonData.GameInstance.WorldMap.getChildByName("keyScreen") )
//				{
//					GameCommonData.GameInstance.WorldMap.getChildByName("keyScreen").x = 658.5;
//				}
//				changeLineView.x = 723.5;
//			}
//			else
//			{
				if ( GameCommonData.GameInstance.WorldMap.getChildByName("keyScreen") )
				{
					GameCommonData.GameInstance.WorldMap.getChildByName("keyScreen").x = 658.5 + GameCommonData.GameInstance.WorldMap.stage.stageWidth - GameConfigData.GameWidth;
				}
				changeLineView.x = 723.5 + GameCommonData.GameInstance.WorldMap.stage.stageWidth - GameConfigData.GameWidth;
//			}
		}
		
		private function changeMsg():void
		{
			var back:Sprite = GameCommonData.GameInstance.GameUI.getChildByName( "msgBack" ) as Sprite;
			var msgArea:Sprite = msgMed.MsgArea;
			var iScrollPane:UIScrollPane = msgMed.ScrollPane;
			
			if (!(back && msgArea && iScrollPane)) return;
			
//			if(GameCommonData.GameInstance.GameUI.stage.stageHeight <= GameConfigData.GameHeight)
//			{
//				msgArea.y = 0;  
//				back.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y;
//				iScrollPane.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y;
//			}
//			
//			if( GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight )
//			{
			    var length:Number = GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight; 
			    msgArea.y = length;  
				back.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y + length;
				iScrollPane.y = ChatData.MsgPosArea[ChatData.CurAreaPos].y + length;
//		    }
		    iScrollPane.refresh();
		    if( GameCommonData.GameInstance.GameUI.getChildByName("leoView") )
		    {
		    	GameCommonData.GameInstance.GameUI.getChildByName("leoView").y = ChatData.MsgPosArea[ChatData.CurAreaPos].y - 34 + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
		    }
		}
		
		private function changeBuff():void
		{
			var buff:Sprite = GameCommonData.GameInstance.GameUI.getChildByName( "buffController" ) as Sprite;
			var deBuff:Sprite = GameCommonData.GameInstance.GameUI.getChildByName( "deBuffController" ) as Sprite;
			
			if (!(buff && deBuff)) return;
			
//			if(GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth)
//			{
//				buff.x = 580;  
//				deBuff.x = 580;  
//			}
//			
//			if(GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth)
//			{
				buff.x = 580 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;  
				deBuff.x = 580 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;  
//			}
		}
		
		private function changeFriendChat():void
		{
			var friendChat:Sprite = friendChatMed.msgIconPanel;
			
			if( friendChat && GameCommonData.GameInstance.GameUI.contains(friendChat) )
			{
//				if(GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth)
//				{
//					friendChat.x = 660;
//				}
//				
//				if(GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth)
//				{
					friendChat.x = 660 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth; 
//				}
				
//				if(GameCommonData.GameInstance.GameUI.stage.stageHeight <= GameConfigData.GameHeight)
//				{
//					friendChat.y = 472;
//				}
//				
//				if(GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight)
//				{
					friendChat.y = 472 + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
//				}
			}
		}
		
		private function changeScrollNotice():void
		{
			var scrollView:MovieClip = scrollNoticeMed.scrollView;
			
			if (!scrollView) return;
			
//			if(GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth)
//		    {
//		    	scrollView.x = 210;
//		    }
//		    if( GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth )
//		    {
		    	scrollView.x = 210 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
//	        }
		}
		
		private function changeMarket():void
		{
//			MarketConstData.view.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
//			MarketConstData.view.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
		}
		
		private function changeOffLinePlay():void
		{
			var pb:PanelBase = offLinePlayMed.getPanelBase();
			
			if( pb && GameCommonData.GameInstance.GameUI.contains(pb) )
			{
				
//				if(GameCommonData.GameInstance.GameUI.stage.stageWidth <= GameConfigData.GameWidth)
//				{
//					pb.x = 350;
//				}
//				
//				if(GameCommonData.GameInstance.GameUI.stage.stageWidth > GameConfigData.GameWidth)
//				{
					pb.x = 350 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
//				}
//				
//				if(GameCommonData.GameInstance.GameUI.stage.stageHeight <= GameConfigData.GameHeight)
//				{
//					pb.y = 180;
//				}
//				
//				if(GameCommonData.GameInstance.GameUI.stage.stageHeight > GameConfigData.GameHeight)
//				{
					pb.y = 180 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
//				}
			}
			
		}
		
		private function changeMap():void
		{
			var disObj:DisplayObject;
			if( GameCommonData.GameInstance.GameUI.getChildByName("senceMapContainer") )
			{
				disObj = GameCommonData.GameInstance.GameUI.getChildByName("senceMapContainer");
				disObj.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - disObj.width) / 2;
				disObj.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - disObj.height) / 2;
			}
			if( GameCommonData.GameInstance.GameUI.getChildByName("BigMap") )
			{
				disObj = GameCommonData.GameInstance.GameUI.getChildByName("BigMap");
				disObj.x = (GameCommonData.GameInstance.GameUI.stage.stageWidth - disObj.width)/2;
				disObj.y = (GameCommonData.GameInstance.GameUI.stage.stageHeight - disObj.height)/2;
			}
		}
		
		private function changePanelBase():void
		{
			var display:DisplayObject;
//			var startX:Number;
//			var startY:Number;
//			var endX:Number;
//			var endY:Number;
			for ( var i:uint=0; i<GameCommonData.GameInstance.GameUI.numChildren; i++ )
			{
				display = GameCommonData.GameInstance.GameUI.getChildAt( i );
				if ( ( display is PanelBase ) || ( display is AutoPanelBase )  )
				{
//					startX = display.x;
//					startY = display.y;
//					display.x = startX * GameCommonData.GameInstance.GameUI.stage.stageWidth / this.oldStageWidth;
//					display.y = startY * GameCommonData.GameInstance.GameUI.stage.stageHeight / this.oldStageHeight;
					if( display.name != "Alert" )
					{
						display.x += int((GameCommonData.GameInstance.GameUI.stage.stageWidth - this.oldStageWidth)/2);
						display.y += int((GameCommonData.GameInstance.GameUI.stage.stageHeight - this.oldStageHeight)/2);
					}
				}
			}
		}
		
		private function changeMusicPlayer():void
		{
			if( GameCommonData.GameInstance.GameUI.getChildByName( "musicPlayerBtn" ) )
			{
				var musicBtn:SimpleButton = GameCommonData.GameInstance.GameUI.getChildByName( "musicPlayerBtn" ) as SimpleButton;
				musicBtn.x = 897 + GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
				musicBtn.y = 511 + GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
			}
		}
		
		//帮派打工、副本的时间倒计时界面
		private function changeTimeView():void
		{
			TimeCountDownData.timeViewContent.x = GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth;
			TimeCountDownData.timeViewContent.y = GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight;
		}
		
		private function changeDragRect():void
		{
			for each( var panel:PanelBase in GameCommonData.panelBaseArr )
			{
				panel.updateDragRect();
				panel.updatePoint();
//				switch( panel.name )
//				{
//					case "HeroProp":
//					case "PetBag":
//					case "TaskView":
//					case "SkillView":
//						panel.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
//						panel.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
//						break;
//					case "Bag":
//					case "Friend":
//						panel.x = UIConstData.DefaultPos2.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
//						panel.y = UIConstData.DefaultPos2.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
//						break;
//					case "unity":
//						
//				}
			}
			
			if( GameCommonData.autoPanelArr.length > 0 )
			{
				for each( var autoPanel:AutoPanelBase in GameCommonData.autoPanelArr )
				{
					autoPanel.updateDragRect();
					autoPanel.updatePoint();
				}
			}
		}
	}
}