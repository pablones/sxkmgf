package GameUI.Modules.Meridians.view
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.model.MeridiansTypeVO;
	import GameUI.Modules.Meridians.tools.Tools;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.FastPurchase.FastPurchase;
	import GameUI.View.items.MoneyItem;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.text.TextField;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MeridiansStrengMediator extends Mediator
	{
		public static const NAME:String = "MeridianStrengMediator";
		
		private var strengthView:MovieClip;							//強化面板
		private var panelBase:PanelBase;
		private var buttons:Array = new Array();
		private var texts:Array = new Array();
		private var unBindMoney:MoneyItem;
		private var bindMoney:MoneyItem;
		private var needMoney:MoneyItem;
		private var dataProxy:DataProxy;
		private var selectedMeridians:int = 1;						//选中的经脉标号
		private var _isUserStringthProtect:Boolean = false;
		private var loader:Loader=new Loader();							//加载经脉强化等级图片
		private	var r:URLRequest=new URLRequest();
		private	var currMoney:int = 0;									//当然金钱
		private	var currStrengthDan:int = 1;							//当然强化丹数
		private	var currProtectDan:int = 1;								//当然强化保护丹数
		private var reqmoney:int = 0;									//强化时需要的金钱
		private var fastPurchase:FastPurchase;							//快速购买
		private var getOutTimer:Timer = new Timer(200, 1);	//物品取出计时器

		public var comfrim:Function = comfrimStrength;
		public var cancel:Function = cancelStrength;
		
		public function MeridiansStrengMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					MeridiansEvent.INIT_MERIDIANS_STRENG,										//初始化强化经脉界面
					MeridiansEvent.SHOW_MERIDIANS_STRENG,										//显示强化经脉界面
					MeridiansEvent.CLOSE_MERIDIANS_STRENG,
					EventList.UPDATEMONEY,
					MeridiansEvent.RESULT_STRENGTHJINMEI_SUC,
					MeridiansEvent.RESULT_STRENGTHJINMEI_FAIL,
					MeridiansEvent.UPDATA_STRENGTH_AND_MAIN,
					EventList.ONSYNC_BAG_QUICKBAR,
					NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP,
					
				];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MeridiansEvent.INIT_MERIDIANS_STRENG:
					initUI();
					break;
				case MeridiansEvent.SHOW_MERIDIANS_STRENG:
					if(dataProxy.meridianStrengthIsOpen)
					{
						closeUI( null );
						break;
					}
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_LIST);
//					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT);
					if(notification.getBody() != null)
					{
						this.selectedMeridians = notification.getBody() as int;
					}
					showUI();
					break;
				case MeridiansEvent.CLOSE_MERIDIANS_STRENG:
					closeUI( null );
					break;
				case EventList.UPDATEMONEY:
					if ( dataProxy.meridianStrengthIsOpen )
					{
						upDateMoney();	
					}
					break;
				case MeridiansEvent.RESULT_STRENGTHJINMEI_SUC:
					updataView(notification.getBody() as int);
					break;
				case MeridiansEvent.RESULT_STRENGTHJINMEI_FAIL:
					updataView(notification.getBody() as int);
					break;
				case MeridiansEvent.UPDATA_STRENGTH_AND_MAIN:
					if(dataProxy.meridianStrengthIsOpen)
					{
						updataView(notification.getBody() as int);
					}
					break;

				case EventList.ONSYNC_BAG_QUICKBAR:
					if(dataProxy.meridianStrengthIsOpen && ( notification.getBody() == 612000 || notification.getBody() == 612001))
					{
						currStrengthDan = BagData.hasItemNum( 612000 );
						currProtectDan = BagData.hasItemNum( 612001 );
						(this.texts[2] as TextField).htmlText = "<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_1"]/*拥有*/ +"<font color='#00ffff'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_2"]/*经脉强化丹*/ +"</font>：<font color='#00FF00'>"+currStrengthDan+"</font>" + GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]/*个*/ +"<font>";
						(this.texts[3] as TextField).htmlText = "<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_1"]/*拥有*/ +"<font color='#00ffff'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_3"]/*经脉保护丹*/ + "</font>：<font color='#00FF00'>"+currProtectDan+"</font>" + GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]/*个*/ +"<font>";
					}
					break;
				case NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP:
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_STRENG);
					break;
			}
		}
		
		private function initUI():void
		{
			this.strengthView = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("StrengthView");	//获取经脉强化界面
			fastPurchase = new FastPurchase("612000");
			fastPurchase.x = 296;
			strengthView.addChild(fastPurchase);
			panelBase = new PanelBase( strengthView,strengthView.width + 60 ,strengthView.height+12 );
			
			panelBase.SetTitleTxt(GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_4"]/*"经脉强化"*/);
			panelBase.x = UIConstData.DefaultPos1.x + 385;
			panelBase.y = UIConstData.DefaultPos1.y;
			
			for(var i:int =1; i <= 11; ++i)
			{
				this.buttons.push(this.strengthView["strengthView_btn_"+i]);
			}
			
			for(i = 1; i <= 5; ++i)
			{
				this.texts.push(this.strengthView["strengthView_txt_"+i]);
//				(this.strengthView["strengthView_txt_"+i] as TextField).text = ""+i;
			}
			//获取经脉强化等级图片
			loader.x = 149;
			loader.y = 128;
			this.strengthView.addChild(loader);
			
			(this.texts[0] as TextField).htmlText = "0%";
			(this.texts[1] as TextField).htmlText = GameCommonData.wordDic[ "often_used_losedownto" ] /*失败降为*/+"0";
			(this.texts[4] as TextField).htmlText = GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_init_1"]/*"强化阳跷"*/;
			
			bindMoney = new MoneyItem();
			unBindMoney = new MoneyItem();
			needMoney = new MoneyItem();

			bindMoney.x = 110;
			bindMoney.y = 319;
			
			unBindMoney.x = 110;
			unBindMoney.y = 342;
			
			needMoney.x = 110;
			needMoney.y = 298;
			
			this.strengthView.addChild( bindMoney );
			this.strengthView.addChild( unBindMoney );
			this.strengthView.addChild( needMoney );
			upDateMoney();
			
			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
		}
		
		private function showUI():void
		{
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			
			dataProxy.meridianStrengthIsOpen = true;

			panelBase.addEventListener( Event.CLOSE,closeUI );
			for(var i:int = 0; i < 9; ++i)
			{
				(this.buttons[i] as MovieClip).addEventListener(MouseEvent.CLICK,onButtonClick);
			}
			(this.buttons[9] as SimpleButton).addEventListener(MouseEvent.CLICK,onButtonClick);
			(this.buttons[10] as SimpleButton).addEventListener(MouseEvent.CLICK,onButtonClick);
			
			updataView(this.selectedMeridians);
		}
		
		public function set isUserStringthProtect(b:Boolean):void
		{
			
			currProtectDan = BagData.hasItemNum( 612001 );
			if(b)
			{
				if(this.currProtectDan > 0)
				{
					this._isUserStringthProtect = true;
					(this.texts[1] as TextField).visible = false;
					(this.buttons[8] as MovieClip).gotoAndStop(2);
				}
				else
				{
					//提示缺少强化保护丹
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_isU_1"]/*"经脉保护丹不足，请补充"*/, color:0xffff00});
				}
			}
			else
			{
				this._isUserStringthProtect = false;
				(this.texts[1] as TextField).visible = true;
				(this.buttons[8] as MovieClip).gotoAndStop(1);
			}
		}
		
		public function get isUserStringthProtect():Boolean
		{
			return this._isUserStringthProtect;
		}	
		
		private function updataView(n:int):void
		{

			this.selectedMeridians = n;
			(this.texts[4] as TextField).text = GameCommonData.wordDic[ "mod_equ_ui_equ_cel_1" ]/*"强化"*/ + MeridiansData.meridiansNames[n];
			for(var i:int = 1; i <= 8; ++i)
			{
				if(n == i )
				{
					(this.buttons[i - 1] as MovieClip).gotoAndStop(2);
				}
				else
				{
					(this.buttons[i - 1] as MovieClip).gotoAndStop(1);
				}
			}
			var nextLev:int = MeridiansData.meridiansVO.meridiansArray[this.selectedMeridians -1 ].nStrengthLev + 1;
			if(nextLev > 10)
			{
				(this.texts[0] as TextField).htmlText = "<font color='#00ff00'>" + GameCommonData.wordDic[ "mod_fri_view_med_pla_initD_1" ]/*无*/ + "</font>";
				reqmoney = 0;
			}
			if(nextLev == 1)
			{
				(this.texts[0] as TextField).htmlText = "<font color='#00ff00'>"+MeridiansData.meridiansGradeDic[nextLev].success+"%</font>";
				reqmoney = MeridiansData.meridiansGradeDic[nextLev].reqmoney;
			}
			else if(nextLev <= 10)
			{
				var success:int = MeridiansData.meridiansGradeDic[nextLev].success;
				(this.texts[0] as TextField).htmlText = "<font color='#ff0000'>"+success+"%</font>";
				reqmoney = MeridiansData.meridiansGradeDic[nextLev].reqmoney;
			}
			
			
			//加载强化等级图片
			loader.unload();
			r.url=GameCommonData.GameInstance.Content.RootDirectory+"Resources/NumberIcon/"+(nextLev - 1)+".png";
			loader.load(r);
			
			currStrengthDan = BagData.hasItemNum( 612000 );
			currProtectDan = BagData.hasItemNum( 612001 );
			(this.texts[2] as TextField).htmlText = "<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_1"]/*拥有*/ +"<font color='#00ffff'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_2"]/*经脉强化丹*/ +"</font>：<font color='#00FF00'>"+currStrengthDan+"</font>" + GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]/*个*/ +"<font>";
			(this.texts[3] as TextField).htmlText = "<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_1"]/*拥有*/ +"<font color='#00ffff'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_hand_3"]/*经脉保护丹*/ + "</font>：<font color='#00FF00'>"+currProtectDan+"</font>" + GameCommonData.wordDic[ "mod_cam_med_ui_UIC1_cre_1" ]/*个*/ +"<font>";
			
			if(nextLev == 1 || nextLev == 11)
			{
				(this.texts[1] as TextField).visible = false;
			}
			else if(currProtectDan < 1)
			{
				this.isUserStringthProtect = false;
			}
			else
			{
				this.isUserStringthProtect = this._isUserStringthProtect;
			}
			
			upDateNeedMoney( reqmoney );
			upDateMoney();
		}
		
		private function upDateMoney():void
		{
			this.bindMoney.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoney.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]));
		}
		
		private function onButtonClick(event:MouseEvent):void
		{
			switch(event.target.name)
			{
				case "strengthView_btn_1":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,1);
					break;
				case "strengthView_btn_2":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,2);
					break;
				case "strengthView_btn_3":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,3);
					break;
				case "strengthView_btn_4":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,4);
					break;
				case "strengthView_btn_5":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,5);
					break;
				case "strengthView_btn_6":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,6);
					break;
				case "strengthView_btn_7":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,7);
					break;
				case "strengthView_btn_8":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,8);
					break;
				case "strengthView_btn_9":
					this.isUserStringthProtect = !this.isUserStringthProtect;
					break;
				case "strengthView_btn_10":
					if ( !startTimer() )
					{
						return;
					}
					if( (MeridiansData.meridiansVO.meridiansArray[this.selectedMeridians - 1] as MeridiansTypeVO).nStrengthLev < 10)
					{
						var isProtect:int = 0;
						if(this._isUserStringthProtect)
						{
							isProtect = 1;
						}
						else
						{
							isProtect = 0;
						}
						
						var canStrength:Boolean = true;	
						
						currMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
						if(currMoney < reqmoney)
						{
							//发出缺钱的消息
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_equ_med_add_onC_2" ]/*"你的银两不足"*/, color:0xffff00});
							canStrength = false;
						}
						if(currStrengthDan < 1)
						{
							//发出没强化丹的消息
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_onB_1"]/*"经脉强化丹不足，请补充"*/, color:0xffff00});
							canStrength = false;
						}
						
						if(canStrength )
						{
							if( (MeridiansData.meridiansVO.meridiansArray[this.selectedMeridians - 1] as MeridiansTypeVO).nStrengthLev < 5)
							{
								Tools.showMeridiansNet(GameCommonData.Player.Role.Id,selectedMeridians,isProtect,145);
							}	
							else if(_isUserStringthProtect)
							{
								Tools.showMeridiansNet(GameCommonData.Player.Role.Id,selectedMeridians,isProtect,145);
							}
							else
							{
								facade.sendNotification(EventList.SHOWALERT, {comfrim:comfrimStrength, cancel:cancelStrength, info:"<font color='#ffffff'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_onB_2"]/*本次强化如果失败会使强化等级*/ + "<font color='#ff0000'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_onB_3"]/*降到*/ + "0</font>。" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_onB_4"]/*是否确认强化*/ + "？</font>" });
							}
						}
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansStrengMediator_onB_5"]/*"强化等级已满"*/, color:0xffff00});
					}
					
					break;
				case "strengthView_btn_11":
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_STRENG);					
					break;
			}
		}
		
		//加速经脉处理
		public function comfrimStrength():void
		{
			var isProtect:int = 0;
			if(this._isUserStringthProtect)
			{
				isProtect = 1;
			}
			else
			{
				isProtect = 0;
			}
			Tools.showMeridiansNet(GameCommonData.Player.Role.Id,selectedMeridians,isProtect,145);
		}
		public function cancelStrength():void {}
		
		private function closeUI(event:Event):void
		{
			dataProxy.meridianStrengthIsOpen = false;
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				for(var i:int =0; i < 9; ++i)
				{
					(this.buttons[i] as MovieClip).removeEventListener(MouseEvent.CLICK,onButtonClick);
				}
				(this.buttons[9] as SimpleButton).removeEventListener(MouseEvent.CLICK,onButtonClick);
				panelBase.removeEventListener( Event.CLOSE,closeUI );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
		}
		
		//操作延时器
		private function startTimer():Boolean
		{
			if(getOutTimer.running) 
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_dep_pro_ite_sta" ], color:0xffff00});//"请稍后"
				return false;
			} 
			else 
			{
				getOutTimer.reset();
				getOutTimer.start();
				return true;
			}
		}

	}
}