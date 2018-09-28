package GameUI.Modules.Meridians.view
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.model.MeridiansTypeVO;
	import GameUI.Modules.Meridians.tools.Tools;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MeridiansLearnPromptMediator extends Mediator
	{
		public static const NAME:String = "MeridiansLearnPrompt";
		
		private var learnPrompt:MovieClip = null;							//修炼提示
		private var texts:Array = new Array();								//显示文本			0 ~ 从上到下依次数下来
		private var unBindMoney:MoneyItem;
		private var bindMoney:MoneyItem;
		private var needMoney:MoneyItem;
		private	var currMoney:int = 0;										//当前金钱
		private var panelBase:PanelBase;
		private var meridiansId:int = 1;
		private var xuyaodengji:int = 13;									//需要人物等级
		private var reqmoney:int = 0;										//需要银两
		private var xuyaosuoyou:int = 0;									//需要所有经脉等级
		private var dataProxy:DataProxy;
		
		
		public function MeridiansLearnPromptMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
//					MeridiansEvent.INIT_MERIDIANS_LEARN_PROMPT,										//初始化经脉修炼提示界面
//					MeridiansEvent.SHOW_MERIDIANS_LEARN_PROMPT,										//显示经脉修炼提示界面
//					MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT,
//					EventList.UPDATEMONEY,
//					NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP,
				];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MeridiansEvent.INIT_MERIDIANS_LEARN_PROMPT:
					initUI();
					break;
				case MeridiansEvent.SHOW_MERIDIANS_LEARN_PROMPT:
					if(dataProxy.meridianLearnPromptIsOpen)
					{
						closeUI( null );
						break;
					}
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_STRENG);
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_LIST);
					if(notification.getBody() != null)
					{
						this.meridiansId = notification.getBody() as int;
					}
					showUI();
					break;
				case MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT:
					closeUI( null );
					break;
				case EventList.UPDATEMONEY:
					if ( dataProxy.meridianLearnPromptIsOpen )
					{
						upDateMoney();	
					}
					break;
				case NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP:
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT);
					break;
			}
		}
		
		private function initUI():void
		{
			texts = null;
			texts = new Array();
			this.learnPrompt = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("LearnPrompt");		//获取经脉修炼提示
			panelBase = new PanelBase( learnPrompt,learnPrompt.width+8,learnPrompt.height+12 );
			panelBase.SetTitleTxt("开始修炼");
			panelBase.x = UIConstData.DefaultPos1.x + 385;
			panelBase.y = UIConstData.DefaultPos1.y;
//			var roleMediator:DisplayObject = (facade.retrieveMediator(RoleMediator.NAME) as RoleMediator).getViewComponent() as DisplayObject;
//			panelBase.x = roleMediator.width + roleMediator.x;
//			panelBase.y = roleMediator.y;
			//获取经脉学习确认按钮
			
			for(var i:int = 1; i <= 5 ; ++i)
			{
				this.texts.push(this.learnPrompt["learnPrompt_txt_"+i]);
//				(this.texts[i - 1] as TextField).text = ""+i;
				(this.texts[i - 1] as TextField).mouseEnabled = false;
			}
			
			bindMoney = new MoneyItem();
			unBindMoney = new MoneyItem();
			needMoney = new MoneyItem();

			bindMoney.x = 30;
			bindMoney.y = 222;
			
			unBindMoney.x = 30;
			unBindMoney.y = 245;
			
			needMoney.x = 30;
			needMoney.y = 201;
			
			learnPrompt.addChild( bindMoney );
			learnPrompt.addChild( unBindMoney );
			learnPrompt.addChild( needMoney );
			upDateNeedMoney( reqmoney );
			upDateMoney();
			
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
			(this.learnPrompt.learnPrompt_txt_1 as TextField).htmlText = "<font color='#E2CCA5'>修炼<font color='#00FFFF'>督脉无上·二层</font>，需要以下条件，是否开始修炼？</font>";
		}
		
		private function showUI():void
		{
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			dataProxy.meridianLearnPromptIsOpen = true;
			learnPrompt.learnPrompt_btn_ok.addEventListener(MouseEvent.CLICK,onBtnClick);
			learnPrompt.learnPrompt_btn_cancel.addEventListener(MouseEvent.CLICK,onBtnClick);
			panelBase.addEventListener( Event.CLOSE,closeUI );
			
			var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[meridiansId - 1];
			var name:String = "";
			var nLev:int = meridiansTypeVO.nLev + 1;
			if(nLev > 60)
			{
				if(nLev == 90)
				{
					name += "无上"+MeridiansData.meridiansNames[meridiansTypeVO.nType]+"·三十层";
				}else
				{
					name += "无上·"+ MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30]+"层";
				}
//				name += "无上·" + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30]+"层";
			}
			else if(nLev > 30)
			{
				if(nLev == 60)
				{
					name += "真·"+MeridiansData.meridiansNames[meridiansTypeVO.nType]+"三十层";
				}
				else
				{
					name += "真·" +MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30]+"层";
				}
//				name += "真·" + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30]+"层";
			}
			else
			{
				if(nLev == 30)
				{
					name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+"三十层";
				}
				else
				{
					name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30]+"层";
				}
//				name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30]+"层";
			}
			
			var lastLev:int = MeridiansData.meridiansUpgradeCondition[0][0];
			var id:int = (meridiansId - 1 ) * lastLev + nLev;
			xuyaodengji = MeridiansData.meridiansUpgradeCondition[ id ][0]; //需要人物等级
			reqmoney = MeridiansData.meridiansUpgradeCondition[ id ][1];    //需要金钱
			xuyaosuoyou = MeridiansData.meridiansUpgradeCondition[ id ][2];  // 需要所有经脉等级
			
			(this.texts[0] as TextField).htmlText = "<font color='#E2CCA5'>修炼<font color='#00FFFF'>"+name+"</font>，需要以下条件，是否开始修炼？</font>";
			(this.texts[1] as TextField).htmlText = "人物等级"+xuyaodengji+"级";
			if(GameCommonData.Player.Role.Level >= xuyaodengji)
			{
				(this.texts[2] as TextField).htmlText = "<font color='#33FF00'>你当前"+GameCommonData.Player.Role.Level+"级</font>";
			}
			else
			{
				(this.texts[2] as TextField).htmlText = "<font color='#ff0000'>你当前"+GameCommonData.Player.Role.Level+"级</font>";
			}
			
			
//			(this.texts[3] as TextField).htmlText = "所有经脉达到"+ xuyaosuoyou+"级";
//			
//			if(MeridiansData.meridiansVO.nAllLevGrade >= xuyaosuoyou )
//			{
//				(this.texts[4] as TextField).htmlText = "<font color='#33FF00'>你当前所有经脉"+MeridiansData.meridiansVO.nAllLevGrade+"级</font>";
//			}
//			else
//			{
//				(this.texts[4] as TextField).htmlText = "<font color='#999999'>你当前所有经脉"+MeridiansData.meridiansVO.nAllLevGrade+"级</font>";
//			}
			/***** 更换上面几行代码 *****/
			if(xuyaosuoyou > 0)
			{
				name = "";
				nLev = xuyaosuoyou;
				if(nLev > 60)
				{
//					name += "无上·" +MeridiansData.numbers[nLev % 30]+"层";
					if(nLev == 90)
					{
						name += "无上·三十层";
					}else
					{
						name += "无上·" +MeridiansData.numbers[nLev % 30]+"层";
					}
				}
				else if(nLev > 30)
				{
//					name += "真·" +MeridiansData.numbers[nLev % 30]+"层";
					if(nLev == 60)
					{
						name += "真·三十层";
					}
					else
					{
						name += "真·" +MeridiansData.numbers[nLev % 30]+"层";
					}
				}
				else
				{
//					name = MeridiansData.numbers[nLev % 30]+"层";
					if(nLev == 30)
					{
						name = "三十层";
					}
					else
					{
						name = MeridiansData.numbers[nLev % 30]+"层";
					}
				}
				(this.texts[3] as TextField).htmlText = "所有经脉达到"+ name;
//				name = "";
//				nLev = MeridiansData.meridiansVO.nAllLevGrade;
//				if(nLev > 60)
//				{
//					if(nLev == 90)
//					{
//						name += "无上·三十层";
//					}else
//					{
//						name += "无上·" +MeridiansData.numbers[nLev % 30]+"层";
//					}
//				}
//				else if(nLev > 30)
//				{
//					if(nLev == 60)
//					{
//						name += "真·三十层";
//					}
//					else
//					{
//						name += "真·" +MeridiansData.numbers[nLev % 30]+"层";
//					}
//				}
//				else
//				{
//					if(nLev == 30)
//					{
//						name = "三十层";
//					}
//					else
//					{
//						name = MeridiansData.numbers[nLev % 30]+"层";
//					}
//				}
				
				if(MeridiansData.meridiansVO.nAllLevGrade >= xuyaosuoyou )
				{
					
					(this.texts[4] as TextField).htmlText = "<font color='#33FF00'>满足</font>";
				}
				else
				{
					(this.texts[4] as TextField).htmlText = "<font color='#ff0000'>不满足</font>";
				}
			}
			else
			{
				(this.texts[3] as TextField).htmlText = "";
				(this.texts[4] as TextField).htmlText = "";
			}
			/*************/
			
			var heroProp:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("HeroProp");
			if(heroProp != null)
			{
				panelBase.x = heroProp.x + heroProp.width - 35;
				panelBase.y = heroProp.y;
			}
			
			upDateNeedMoney( reqmoney );
			upDateMoney();
		}
		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]));
		}
		
		private function upDateMoney():void
		{
			this.bindMoney.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoney.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		private function onBtnClick(event:MouseEvent):void
		{
			switch(event.target.name)
			{
				case "learnPrompt_btn_ok":
				
					var canLearn:Boolean = true;
				
					currMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
					
					
					if(currMoney >= reqmoney || GameCommonData.Player.Role.BindMoney >= reqmoney || GameCommonData.Player.Role.UnBindMoney >= reqmoney)
					{
						
					}
					else
					{
						//发出缺钱的消息
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"金钱不足，无法修炼", color:0xffff00});
						canLearn = false;
					}
					if(GameCommonData.Player.Role.Level < xuyaodengji || MeridiansData.meridiansVO.nAllLevGrade < xuyaosuoyou)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:"你不满足修炼条件", color:0xffff00});
						canLearn = false;
					}
					if(canLearn)
					{
						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,this.meridiansId,0,143);
						sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT);
					}
					break;
				case "learnPrompt_btn_cancel":
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT);
					break;
			}
		}
		

		private function closeUI( evt:Event ):void
		{
			dataProxy.meridianLearnPromptIsOpen = false;
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				this.learnPrompt.learnPrompt_btn_ok.removeEventListener(MouseEvent.CLICK,onBtnClick);
				panelBase.removeEventListener( Event.CLOSE,closeUI );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
		}
	}
}