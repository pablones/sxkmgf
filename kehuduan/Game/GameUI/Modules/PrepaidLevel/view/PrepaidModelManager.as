package GameUI.Modules.PrepaidLevel.view
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidDataProxy;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.Modules.PrepaidLevel.Mediator.TravelHelpMediator;
	import GameUI.Modules.PrepaidLevel.Net.PrepaidLevelNet;
	import GameUI.Modules.SmallWindow.Mediator.FixedTime;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.text.TextField;
	
	public class PrepaidModelManager
	{
		private var prepaidLevel:MovieClip;
		private var dataProxy:PrepaidDataProxy;
		private var timer:FixedTime;
		private var fugue:MovieClip;
		
		public function PrepaidModelManager( prepaidLevel:MovieClip, dataProxy:PrepaidDataProxy )
		{
			this.prepaidLevel = prepaidLevel;
			this.dataProxy = dataProxy;
		}
		
		public function setModel( btnStr:String ):void
		{
			removeAllLis();
			switch( btnStr )
			{
				case "prepaid_btn":
					prepaidModel();
					break;
				case "offline_btn":
					offlineModel();
					break;
				case "travel_btn":
					travelModel();
					break;
			}
			checkBtn(btnStr);
		}
		
		private function prepaidModel():void
		{
			if( dataProxy.giftPage )
			{
				dataProxy.currentPage = dataProxy.giftPage;
				dataProxy.giftPage = 0;
			}else if( dataProxy.prepaidLevel != 10 )
			{
				dataProxy.currentPage = dataProxy.prepaidLevel + 1;
			}
			else
			{
				dataProxy.currentPage = dataProxy.prepaidLevel;
			}
			checkPageTxt();
//			prepaidLevel.giftIcon.addEventListener(MouseEvent.MOUSE_OVER, onOverIcon);
//			prepaidLevel.giftIcon.addEventListener(MouseEvent.MOUSE_OUT, onOutIcon);
			(prepaidLevel.nowPrepaid_btn as SimpleButton).addEventListener(MouseEvent.CLICK, prepaidFun);
			(prepaidLevel.left_btn as SimpleButton).addEventListener(MouseEvent.CLICK, goLeft);
			(prepaidLevel.right_btn as SimpleButton).addEventListener(MouseEvent.CLICK, goRight);
		}
		
		private function offlineModel():void
		{
			(prepaidLevel.gain1_btn as SimpleButton).addEventListener(MouseEvent.CLICK, onClickFun);
			(prepaidLevel.gain2_btn as SimpleButton).addEventListener(MouseEvent.CLICK, onClickFun);
			(prepaidLevel.gain3_btn as SimpleButton).addEventListener(MouseEvent.CLICK, onClickFun);
			(prepaidLevel.gain4_btn as SimpleButton).addEventListener(MouseEvent.CLICK, onClickFun);
		}
		
		private function travelModel():void
		{
			fugue = prepaidLevel.getChildByName("fugue_btn") as MovieClip;
			(prepaidLevel.venture_txt as TextField).mouseEnabled = false;
			if( 0 < dataProxy.questionIndex && dataProxy.questionIndex < 101 )
			{
				(prepaidLevel.question as MovieClip).visible = true;
				(prepaidLevel.confirm_btn as SimpleButton).visible = true;
				(prepaidLevel.confirm_txt as TextField).visible = true;
				(prepaidLevel.confirm_txt as TextField).mouseEnabled = false;
				(prepaidLevel.default_txt as TextField).visible = false;
				(prepaidLevel.none_txt as TextField).visible = false;
				(prepaidLevel.hasYouLi_txt as TextField).visible = false;
				(prepaidLevel.confirm_btn as SimpleButton).addEventListener(MouseEvent.CLICK, confirmFun);
				(prepaidLevel.question.answer1_btn as MovieClip).addEventListener(MouseEvent.CLICK, clickAnswer);
				(prepaidLevel.question.answer2_btn as MovieClip).addEventListener(MouseEvent.CLICK, clickAnswer);
			}else{
				(prepaidLevel.question as MovieClip).visible = false;
				(prepaidLevel.confirm_btn as SimpleButton).visible = false;
				(prepaidLevel.confirm_txt as TextField).visible = false;
				(prepaidLevel.default_txt as TextField).visible = true;
				(prepaidLevel.default_txt as TextField).mouseEnabled = false;
				if(dataProxy.questionIndex == 0)
				{
					(prepaidLevel.none_txt as TextField).visible = true;
					(prepaidLevel.default_txt as TextField).visible = false;
					(prepaidLevel.hasYouLi_txt as TextField).visible = false;
					timer = new FixedTime( 2000, change );
				}
				else if(dataProxy.questionIndex == -1)
				{
					(prepaidLevel.default_txt as TextField).visible = true;
					(prepaidLevel.none_txt as TextField).visible = false;
					(prepaidLevel.hasYouLi_txt as TextField).visible = false;
				}
				else if(dataProxy.questionIndex == -2)
				{
					(prepaidLevel.hasYouLi_txt as TextField).visible = true;
					(prepaidLevel.default_txt as TextField).visible = false;
					(prepaidLevel.none_txt as TextField).visible = false;
					timer = new FixedTime( 2000, change );
				}
			}
			if( dataProxy.usedYouLiCount == 0 )
			{
				MasterData.setGrayFilter( prepaidLevel.venture_btn );
				MasterData.setGrayFilter( prepaidLevel.venture_txt );
				(prepaidLevel.venture_btn as SimpleButton).mouseEnabled = false;
			}
			else
			{
//				(prepaidLevel.venture_btn as SimpleButton).filters = [];
//				(prepaidLevel.venture_txt as TextField).filters = [];
				MasterData.delGrayFilter( prepaidLevel.venture_btn );
				MasterData.delGrayFilter( prepaidLevel.venture_txt );
				(prepaidLevel.venture_btn as SimpleButton).mouseEnabled = true;
			}
			if( dataProxy.fugueNeedMoney > 2000 )
			{
				MasterData.setGrayFilter( fugue );
				fugue.mouseEnabled = false;
				fugue.mouseChildren = false;
			}
			else
			{
				MasterData.delGrayFilter( fugue );
				fugue.mouseEnabled = true;
				fugue.mouseChildren = true;
			}
			if( dataProxy.clickBtn == "venture_btn" )
			{
				(prepaidLevel.youLiTip as MovieClip).gotoAndStop(2);
				dataProxy.clickBtn = "";
			}
			else if( dataProxy.clickBtn == "fugue_btn")
			{
				(prepaidLevel.youLiTip as MovieClip).gotoAndStop(3);
				(prepaidLevel.youLiTip.youLiTip_txt as TextField).htmlText = "消耗<font color='#ff6600'>"+dataProxy.fugueNeedMoney+"</font>点卷，可以直接穿越时空，探索不再受到次数限制。";
				dataProxy.clickBtn = "";
			}
			if( (prepaidLevel.venture_btn as SimpleButton).hasEventListener(MouseEvent.CLICK) == false )
			{
				(prepaidLevel.venture_btn as SimpleButton).addEventListener(MouseEvent.MOUSE_OVER, overFun);
				(prepaidLevel.venture_btn as SimpleButton).addEventListener(MouseEvent.MOUSE_OUT, outFun);
				(prepaidLevel.venture_btn as SimpleButton).addEventListener(MouseEvent.CLICK, youLiFun);
			}
			if( fugue.hasEventListener(MouseEvent.CLICK) == false )
			{
				fugue.addEventListener(MouseEvent.MOUSE_OVER, overFun);
				fugue.addEventListener(MouseEvent.MOUSE_OUT, outFun);
				fugue.addEventListener(MouseEvent.CLICK, youLiFun);
			}
			if( (prepaidLevel.travelHelp_btn as SimpleButton).hasEventListener(MouseEvent.CLICK) == false )
			{
				(prepaidLevel.travelHelp_btn as SimpleButton).addEventListener(MouseEvent.CLICK, changeView);
			}
		}
		
		private function changeView( e:MouseEvent ):void
		{
			if( GameCommonData.UIFacadeIntance.hasMediator( TravelHelpMediator.NAME ) == false )
			{
				GameCommonData.UIFacadeIntance.registerMediator( new TravelHelpMediator() );
			}
			if( PrepaidUIData.travelIsOpen == false )
			{
				dataProxy.sendNotification( PrepaidUIData.SHOW_TRAVELHELP_VIEW );
			}
			else
			{
				dataProxy.sendNotification( PrepaidUIData.CLOSE_TRAVELHELP_VIEW );
			}
		}
		
		private function change():void
		{
			(prepaidLevel.default_txt as TextField).visible = true;
			(prepaidLevel.none_txt as TextField).visible = false;
			(prepaidLevel.hasYouLi_txt as TextField).visible = false;
			timer = null;
		}
		
		private function checkBtn( btnStr:String ):void
		{
			for(var i:uint=0; i<dataProxy.btnArr.length; i++)
			{
				prepaidLevel[dataProxy.btnArr[i]].gotoAndStop(2);
			}
			prepaidLevel[btnStr].gotoAndStop(1);
		}
		
		private function onClickFun( e:MouseEvent ):void
		{
			if( AutoPlayData.offLineTime < 1 )
			{
				dataProxy.sendNotification(HintEvents.RECEIVEINFO, {info:"没有离线经验可以领取", color:0xffff00});
				return;
			}
			switch( e.target.name )
			{
				case "gain1_btn":
					dataProxy.sendNotification( AutoPlayEventList.SHOW_OFFLINEPLAY_FROM_PREPAID, 0 )
					break;
				case "gain2_btn":
					dataProxy.sendNotification(EventList.SHOWALERT,{comfrim:onComfrim,cancel:new Function(),info:PrepaidUIData.countMoney(),extendsFn:null,doExtends:0,canDrag:false} );
					break;
				case "gain3_btn":
					dataProxy.sendNotification( PrepaidUIData.SHOW_ADDXIAOYAO, 1 );
					break;
				case "gain4_btn":
					dataProxy.sendNotification( PrepaidUIData.SHOW_ADDXIAOYAO, 2 );
					break;
			}
		}
		
		private function onComfrim():void
		{
			dataProxy.sendNotification(AutoPlayEventList.SHOW_OFFLINEPLAY_FROM_PREPAID, 1);
		}
		
		private function clickAnswer( e:MouseEvent ):void
		{
			switch( e.target.name )
			{
				case "answer1_btn":
					dataProxy.selectAnswer = 1;
					(prepaidLevel.question.answer1_btn as MovieClip).gotoAndStop(2);
					(prepaidLevel.question.answer2_btn as MovieClip).gotoAndStop(1);
					break;
				case "answer2_btn":
					dataProxy.selectAnswer = 2;
					(prepaidLevel.question.answer1_btn as MovieClip).gotoAndStop(1);
					(prepaidLevel.question.answer2_btn as MovieClip).gotoAndStop(2);
					break;
			}
		}
		
		private function overFun( e:MouseEvent ):void
		{
			switch( e.target.name )
			{
				case "venture_btn":
					(prepaidLevel.youLiTip as MovieClip).gotoAndStop(2);
					(prepaidLevel.venture_btn as SimpleButton).addEventListener(MouseEvent.MOUSE_MOVE, moveFun);
					break;
				case "fugue_btn":
					(prepaidLevel.youLiTip as MovieClip).gotoAndStop(3);
					(prepaidLevel.youLiTip.youLiTip_txt as TextField).htmlText = "消耗<font color='#ff6600'>"+dataProxy.fugueNeedMoney+"</font>点卷，可以直接穿越时空，探索不再受到次数限制。";
					fugue.addEventListener(MouseEvent.MOUSE_MOVE, moveFun);
					break;
			}
		}
		
		private function moveFun( e:MouseEvent ):void
		{
			switch( e.target.name )
			{
				case "venture_btn":
					(prepaidLevel.youLiTip as MovieClip).gotoAndStop(2);
					break;
				case "fugue_btn":
					(prepaidLevel.youLiTip as MovieClip).gotoAndStop(3);
					(prepaidLevel.youLiTip.youLiTip_txt as TextField).htmlText = "消耗<font color='#ff6600'>"+dataProxy.fugueNeedMoney+"</font>点卷，可以直接穿越时空，探索不再受到次数限制。";
					break;
			}
		}
		
		private function outFun( e:MouseEvent ):void
		{
			(prepaidLevel.youLiTip as MovieClip).gotoAndStop(1);
			switch( e.target.name )
			{
				case "venture_btn":
					(prepaidLevel.venture_btn as SimpleButton).removeEventListener(MouseEvent.MOUSE_MOVE, moveFun);
					break;
				case "fugue_btn":
					fugue.removeEventListener(MouseEvent.MOUSE_MOVE, moveFun);
					break;
			}
		}
		
		private function youLiFun( e:MouseEvent ):void
		{
			if( dataProxy.questionIndex>0 && dataProxy.questionIndex<101 )
			{
				dataProxy.sendNotification(EventList.SHOWALERT,{comfrim:new Function(),cancel:null,info:"<font color='#E2CCA5'>您遇到了一个奇遇，请作出选择</font>",extendsFn:null,doExtends:0,canDrag:false} );
				return;
			}
			switch( e.target.name )
			{
				case "venture_btn":
					if( dataProxy.usedYouLiCount == 0 )
					{
						dataProxy.sendNotification(HintEvents.RECEIVEINFO, {info:"今天探索次数已达你身体所能承受极限", color:0xffff00});
					}
					else
					{
						if( dataProxy.isWait == false )
						{
							dataProxy.isWait = true;
							PrepaidLevelNet.sendPrepaidDemand( 15 );
							dataProxy.clickBtn = "venture_btn";
							if( timer ) timer.stop();
						}
					}
					break;
				case "fugue_btn":
					if( dataProxy.isWait == false )
					{
						dataProxy.isWait = true;
						PrepaidLevelNet.sendPrepaidDemand( 17 );
						dataProxy.clickBtn = "fugue_btn";
						if( timer ) timer.stop();
					}
					break;
			}
		}
		
		private function giftFun( e:MouseEvent ):void
		{
			var mc:MovieClip = e.currentTarget as MovieClip;
			dataProxy.giftPage = uint(mc.name);
			mc.mouseEnabled = false;
			mc.mouseChildren = false;
//			dataProxy.getGiftArr[uint(mc.name)-1] = false;
			PrepaidLevelNet.sendGainGift( uint(mc.name) );
		}
		
		private function confirmFun( e:MouseEvent ):void
		{
			if( dataProxy.selectAnswer == 0 )
			{
				dataProxy.sendNotification( HintEvents.RECEIVEINFO, {info:"还没做出选择", color:0xffff00} );
			}
			else
			{
				PrepaidLevelNet.sendAnswer( dataProxy.questionIndex, dataProxy.selectAnswer );
			}
		}
		
		private function prepaidFun( e:MouseEvent ):void
		{
			var payPath:String = GameCommonData.payPath;
			if(payPath.substr(payPath.length - 4) == "null")
			{
				payPath = payPath.substr(0,payPath.length - 4)+1;
			}
			navigateToURL(new URLRequest(payPath), "_self");
		}
		
		private function goLeft( e:MouseEvent ):void
		{
			if( dataProxy.currentPage > 1 )
			{
				dataProxy.currentPage--;
				checkPageTxt();
			}
		}
		
		private function goRight( e:MouseEvent ):void
		{
			if( dataProxy.currentPage <10 )
			{
				dataProxy.currentPage++;
				checkPageTxt();
			}
		}
		
		public function checkPageTxt():void
		{
			(prepaidLevel.VIPLevelView as MovieClip).gotoAndStop( dataProxy.currentPage );
			PrepaidUIData.gift.name = "TaskEqu_" + dataProxy.giftType[dataProxy.currentPage-1];
			(prepaidLevel.page_txt as TextField).text = "第"+dataProxy.currentPage+"/10页";
			(prepaidLevel.page_txt as TextField).mouseEnabled = false;
			if( dataProxy.currentPage/10 >= 1 )
			{
				(prepaidLevel.page_txt as TextField).x = 148;
			}
			else
			{
				(prepaidLevel.page_txt as TextField).x = 148+3;
			}
			if( dataProxy.getGiftArr[dataProxy.currentPage-1] )
			{
				if( !prepaidLevel.contains(PrepaidUIData.giftBtn) )
				{
					var mc:MovieClip = PrepaidUIData.giftBtn;
					mc.x = 170;
					mc.y = 311;
					prepaidLevel.addChild( mc );
				}
				PrepaidUIData.giftBtn.name = dataProxy.currentPage.toString();
				PrepaidUIData.giftBtn.visible = true;
				PrepaidUIData.giftBtn.mouseEnabled = true;
				PrepaidUIData.giftBtn.mouseChildren = true;
				PrepaidUIData.giftBtn.gift_txt.mouseEnabled = false;
				PrepaidUIData.giftBtn.filters = [];
				PrepaidUIData.giftBtn.gift_txt.text = "领取";
				PrepaidUIData.giftBtn.addEventListener(MouseEvent.CLICK, giftFun);
			}
			else
			{
//				PrepaidUIData.giftBtn.gift_txt.mouseEnabled = false;
				if( dataProxy.currentPage > dataProxy.prepaidLevel )
				{
					PrepaidUIData.giftBtn.visible = false;
				}
				else
				{
					PrepaidUIData.giftBtn.mouseEnabled = false;
					PrepaidUIData.giftBtn.mouseChildren = false;
					MasterData.setGrayFilter(PrepaidUIData.giftBtn);
					PrepaidUIData.giftBtn.gift_txt.text = "已领取";
				}
			}
		}
		
		public function removeAllLis():void
		{
//			if( prepaidLevel.giftIcon && prepaidLevel.giftIcon.hasEventListener(MouseEvent.MOUSE_OVER)
//			{
//				prepaidLevel.giftIcon.removeEventListener(MouseEvent.MOUSE_OVER, onOverIcon);
//			}
//			if( prepaidLevel.giftIcon && prepaidLevel.giftIcon.hasEventListener(MouseEvent.MOUSE_OUT)
//			{
//				prepaidLevel.giftIcon.removeEventListener(MouseEvent.MOUSE_OUT, onOutIcon);
//			}
			if( PrepaidUIData.giftBtn && PrepaidUIData.giftBtn.hasEventListener(MouseEvent.CLICK) )
			{
				PrepaidUIData.giftBtn.removeEventListener(MouseEvent.CLICK, giftFun);
			}
			if( PrepaidUIData.giftBtn && prepaidLevel.nowPrepaid_btn && prepaidLevel.nowPrepaid_btn.hasEventListener(MouseEvent.CLICK) )
			{
				prepaidLevel.nowPrepaid_btn.removeEventListener(MouseEvent.CLICK, prepaidFun);
			} 
			if( prepaidLevel.left_btn && prepaidLevel.left_btn.hasEventListener(MouseEvent.CLICK) )
			{
				prepaidLevel.left_btn.removeEventListener(MouseEvent.CLICK, goLeft);
			}  
			if( prepaidLevel.right_btn && prepaidLevel.right_btn.hasEventListener(MouseEvent.CLICK) )
			{
				prepaidLevel.right_btn.removeEventListener(MouseEvent.CLICK, goRight);
			}  
			if( prepaidLevel.confirm_btn && prepaidLevel.confirm_btn.hasEventListener(MouseEvent.CLICK))
			{
				prepaidLevel.confirm_btn.removeEventListener(MouseEvent.CLICK, confirmFun);
			}
			if( prepaidLevel.question && prepaidLevel.question.answer1_btn && prepaidLevel.question.answer1_btn.hasEventListener(MouseEvent.CLICK) )
			{
				prepaidLevel.question.answer1_btn.removeEventListener(MouseEvent.CLICK, clickAnswer);
			}
			if( prepaidLevel.question && prepaidLevel.question.answer2_btn && prepaidLevel.question.answer2_btn.hasEventListener(MouseEvent.CLICK) )
			{
				prepaidLevel.question.answer2_btn.removeEventListener(MouseEvent.CLICK, clickAnswer);
			}
			if( prepaidLevel.travelHelp_btn && prepaidLevel.travelHelp_btn.hasEventListener(MouseEvent.CLICK) )
			{
				(prepaidLevel.travelHelp_btn as SimpleButton).removeEventListener(MouseEvent.CLICK, changeView);
			}
		}
		
		public function removeYouliBtn():void
		{
			if( prepaidLevel.venture_btn && prepaidLevel.venture_btn.hasEventListener(MouseEvent.CLICK) )
			{
				(prepaidLevel.venture_btn as SimpleButton).removeEventListener(MouseEvent.MOUSE_OVER, overFun);
				(prepaidLevel.venture_btn as SimpleButton).removeEventListener(MouseEvent.MOUSE_OUT, outFun);
				(prepaidLevel.venture_btn as SimpleButton).removeEventListener(MouseEvent.CLICK, youLiFun);
			}
			if( prepaidLevel.getChildByName("fugue_btn") && prepaidLevel.getChildByName("fugue_btn").hasEventListener(MouseEvent.CLICK) )
			{
				fugue.removeEventListener(MouseEvent.MOUSE_OVER, overFun);
				fugue.removeEventListener(MouseEvent.MOUSE_OUT, outFun);
				fugue.removeEventListener(MouseEvent.CLICK, youLiFun);
			}
		}
	}
}