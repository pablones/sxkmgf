package GameUI.Modules.Meridians.view
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Meridians.Components.MeridiansTimeOutComponent;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.model.MeridiansTypeVO;
	import GameUI.Modules.Meridians.tools.Tools;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.SmallWindow.Data.SmallWindowData;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MeridiansLearnListMediator extends Mediator
	{
		public static const NAME:String = "MeridiansLearnList";
		
		private var learnList:MovieClip;							//修炼队列界面
		private var panelBase:PanelBase;
		private var listLength:int = 3;								//修炼队列长度		普通：3，真：4，无上：5
		private var texts:Array = new Array();						//文本数组
		private var buttons:Array = new Array();					//按钮数组
		
		private var learnQueue:Array = new Array();					//修炼队列
		private var currentMeridians:MeridiansTypeVO ;				//当然选中的经脉
		private var dataProxy:DataProxy;
		
		public var comfrim:Function = comfrimQuicken;
		public var cancel:Function = cancelQuicken;
		
		private var timeOutCom:MeridiansTimeOutComponent;
		
		public function MeridiansLearnListMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}

		public override function listNotificationInterests():Array
		{
			return [
					MeridiansEvent.INIT_MERIDIANS_LEARN_LIST,										//初始化经脉修炼队列界面
					MeridiansEvent.SHOW_MERIDIANS_LEARN_LIST,										//显示经脉修炼队列界面
					MeridiansEvent.CLOSE_MERIDIANS_LEARN_LIST,										//关闭经脉修炼队列界面
					MeridiansEvent.INIT_LRARN_NUM,													//初始化经脉最大修炼数目
					MeridiansEvent.RESULT_UPLEV_SUC,
					MeridiansEvent.RESULT_MOVEWAITQUEUE_SUC,
					MeridiansEvent.COMPLETE_MERIDIANS_UPGRADE,
					MeridiansEvent.RESULT_ADDWAITQUEUE_SUC,
					MeridiansEvent.RESULT_STARTLEARN_SUC,
					MeridiansEvent.AUTO_MERIDIANS_LEARN,
					MeridiansEvent.RESULT_UPLEV_PART,
					MeridiansEvent.GET_ARCHEAUS_SUC,
					MeridiansEvent.GET_ARCHEAUS_FAIL,
					MeridiansEvent.UPDATA_ARCHEAUS,												//更新真元值
					NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP,
				];
		}

		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MeridiansEvent.INIT_MERIDIANS_LEARN_LIST:
					initUI();
					break;
				case MeridiansEvent.SHOW_MERIDIANS_LEARN_LIST:
					if(dataProxy.meridiansLearnListIsOpen)
					{
						closeUI( null );
						break;
					}
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_STRENG);
//					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT);
					initQueue();
					showUI();
					break;
				case MeridiansEvent.CLOSE_MERIDIANS_LEARN_LIST:
					sendNotification(MeridiansEvent.CLOSE_ADDZHENYUAN);
					closeUI( null );
					break;
				case MeridiansEvent.INIT_LRARN_NUM:
					var length:int = notification.getBody() as int;
					if(length != this.listLength -2)
					{
						removeUI();				//移除本界面
						this.listLength = length + 2;
						initUI();				//初始化本界面
					}
					initQueue();
					break;
				case MeridiansEvent.RESULT_UPLEV_SUC:
					initQueue();
					if(dataProxy.meridiansLearnListIsOpen)
					{
						showUI();
					}
					break;
				case MeridiansEvent.RESULT_MOVEWAITQUEUE_SUC:
					initQueue();
					if(dataProxy.meridiansLearnListIsOpen)
					{
						showUI();
					}
					break;
				case MeridiansEvent.COMPLETE_MERIDIANS_UPGRADE:
				
					var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[(notification.getBody() as int) - 1];
					var name:String = "";
					var nLev:int = meridiansTypeVO.nLev + 1;
					if(nLev == 90)
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_1" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"无上·" "层" 
					}
					else if(nLev > 60)
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_1" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"无上·" "层" 
					}
					else if( nLev == 60 )
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_3" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"真·" "层" 
					}
					else if(nLev > 30)
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_3" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"真·" "层" 
					}
					else if(nLev == 30)
					{
						name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ] ;// "层" 
					}
					else
					{
						name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ] ;// "层" 
					}
			
					sendNotification(SmallWindowData.SHOW_WINDOW, GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_4" ] /* 恭喜你的 */+ " <font color='#00ffff'>"+name+"</font> "+ GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_5" ] /*已修炼完成*/+"！<font color='#00ff00'><a href='event:_meridian_'><u>" +  GameCommonData.wordDic["mod_mas_med_masta_sms_2"]/*点击这里*/+ "</u></a></font>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_6"]/*修炼下一经脉*/+ "。"); 
					initQueue();
					if(dataProxy.meridiansLearnListIsOpen)
					{
						showUI();
					}
					break;
				case MeridiansEvent.RESULT_ADDWAITQUEUE_SUC:
					initQueue();
					showUI();
					break;
				case MeridiansEvent.RESULT_STARTLEARN_SUC:
					initQueue();
					showUI();
					break;
				case MeridiansEvent.AUTO_MERIDIANS_LEARN:
					initQueue();
					if(dataProxy.meridiansLearnListIsOpen)
					{
						showUI();
					}
					break;
				case MeridiansEvent.RESULT_UPLEV_PART:
					initQueue();
					if(dataProxy.meridiansLearnListIsOpen)
					{
						showUI();
					}
					break;
				case MeridiansEvent.GET_ARCHEAUS_SUC:
					initQueue();
					if(dataProxy.meridiansLearnListIsOpen)
					{
						showUI();
					}
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_7"]/*"补充真元成功"*/, color:0xffff00});
					break;
				case MeridiansEvent.GET_ARCHEAUS_FAIL:
					sendNotification(MeridiansEvent.SHOW_ADDZHENYUAN);
					break;
				case MeridiansEvent.UPDATA_ARCHEAUS:
					(this.texts[1] as TextField).htmlText = GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_8"] /*当前真元*/+"：<font color='#00FF00'>"+ GameCommonData.Player.Role.archaeus+"</font>";
					break;
				case NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP:
					sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_LIST);
					break;
			}
		}

		private function initQueue():void
		{
			while(this.learnQueue.pop());
			
			learnQueue = null;
			learnQueue = new Array();	
			for(var i:int = 0; i < 8; ++i)
			{
				var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[i] as MeridiansTypeVO;
				if(meridiansTypeVO.nState != 0 )
				{
					this.learnQueue.push(meridiansTypeVO);
				}
			}
			this.learnQueue.sortOn("nOrderTimer",Array.NUMERIC);
			
		}

		private function initUI():void
		{			
			this.learnList = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("LearnList"+listLength);		//获取经脉修炼队列
			panelBase = new PanelBase( learnList,learnList.width+8,learnList.height+12 );
			panelBase.SetTitleTxt(GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_9"]/*"修炼队列"*/);
			panelBase.x = UIConstData.DefaultPos1.x + 385;
			panelBase.y = UIConstData.DefaultPos1.y;
			
			texts = null;
			texts = new Array();						//文本数组
			buttons = null;
			buttons = new Array();					//按钮数组
			
			this.texts.push(this.learnList.learnList_txt_1);
			this.texts.push(this.learnList.learnList_txt_2);
			this.buttons.push(this.learnList.learnList_btn_1);
			
			for(var i:int = 1; i <= listLength; ++i)
			{
				this.texts.push(this.learnList["learnList_txt_"+(4*i-1)]);
				this.texts.push(this.learnList["learnList_txt_"+(4*i)]);
				this.texts.push(this.learnList["learnList_txt_"+(4*i+1)]);
				this.texts.push(this.learnList["learnList_txt_"+(4*i+2)]);
				this.buttons.push(this.learnList["learnList_btn_"+(i+1)]);
				
				this.learnList["learnList_txt_"+(4*i-1)].visible = false;
				this.learnList["learnList_txt_"+(4*i)].visible = false;
				this.learnList["learnList_txt_"+(4*i+1)].visible = false;
				this.learnList["learnList_txt_"+(4*i+2)].visible = false;
				this.learnList["learnList_btn_"+(i+1)].visible = false;
				
				this.learnList["learnList_txt_"+(4*i-1)].mouseEnabled = false;
				this.learnList["learnList_txt_"+(4*i)].mouseEnabled = false;
				this.learnList["learnList_txt_"+(4*i+1)].mouseEnabled = false;
				this.learnList["learnList_txt_"+(4*i+2)].mouseEnabled = false;
				(this.learnList["learnList_txt_"+(4*i+2)] as TextField).mouseEnabled = false;

			}

			(this.texts[0] as TextField).htmlText = "<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_10"] /*点击按钮立即补充*/+ "<font color='#00FF00'>50</font>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_11"]/* 点的真元，每天都可免费补充一次。使用*/ + "<font color='#00FFFF'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_12"]/* 真元丹 */ + "</font>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_13"]/*可获得真元 */ + "。<font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_14"]/* 真元可以用来加速经脉的修炼*/ + "。</font></font>";
			(this.texts[1] as TextField).htmlText = GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_8"]/* 当前真元*/ + "：<font color='#00FF00'>0</font>";

			dataProxy = facade.retrieveProxy( DataProxy.NAME ) as DataProxy;
		}

		private function showUI():void
		{
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			panelBase.addEventListener( Event.CLOSE,closeUI );
						
			for(var i:int = 0; i < this.buttons.length; ++i)
			{
				(this.buttons[i] as SimpleButton).addEventListener(MouseEvent.CLICK,onButtonClick);
			}
			
			for(i = 1; i <= this.listLength ; i++)
			{
				if(i <= this.learnQueue.length)
				{
					this.learnList["learnList_txt_"+(4*i-1)].visible = true;
					this.learnList["learnList_txt_"+(4*i)].visible = true;
					this.learnList["learnList_txt_"+(4*i+1)].visible = false;
					this.learnList["learnList_txt_"+(4*i+2)].visible = true;
					this.learnList["learnList_btn_"+(i+1)].visible = true;		
					
					var meridiansTypeVO:MeridiansTypeVO = this.learnQueue[i - 1];
					var name:String = "";
					var nLev:int = meridiansTypeVO.nLev + 1;
					
					if(nLev == 90)
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_1" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"无上·" "层" 
					}
					else if(nLev > 60)
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_1" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"无上·" "层" 
					}
					else if( nLev == 60 )
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_3" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"真·" "层" 
					}
					else if(nLev > 30)
					{
						name += GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_3" ] + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ]; //"真·" "层" 
					}
					else if(nLev == 30)
					{
						name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ] ;// "层" 
					}
					else
					{
						name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic[ "Mod_Mer_mod_MeridiansLearnListMediator_hand_2" ] ;// "层" 
					}			
					
					var time:String = "";
//					meridiansTypeVO.nLeaveTime
					var state:String;
					switch(meridiansTypeVO.nState)
					{
						case 1:
							state = "<font color='#00FF00'>" + GameCommonData.wordDic[ "often_used_cancel2" ]/*取消*/ + "</font>";
							time = "<font color='#00FFFF'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_15"]/* 将耗时*/ + " " + Tools.getTime(meridiansTypeVO.nLeaveTime) + "</font>";
						break;
						case 2:
							time = "<font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_16"]/*还剩*/ + " " + Tools.getTime(meridiansTypeVO.nLeaveTime) + "</font>";
							state = "<font color='#FFFE65'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_17"]/*加速*/ + "</font>";
						break;
						case 3:
							time = "<font color='#00FF00'>" + GameCommonData.wordDic[ "mod_newerPS_med_new_ini_1" ]/*已完成*/ + "</font>";
							state = "<font color='#FFFE65'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_18"]/*完成*/ + "</font>";
						break;
					}
					
					this.learnList["learnList_txt_"+(4*i-1)].htmlText = name;
					this.learnList["learnList_txt_"+(4*i)].htmlText = time;
					this.learnList["learnList_txt_"+(4*i+2)].htmlText = state;
				}
				else
				{
					this.learnList["learnList_txt_"+(4*i-1)].visible = false;
					this.learnList["learnList_txt_"+(4*i)].visible = false;
					this.learnList["learnList_txt_"+(4*i+1)].visible = true;
					this.learnList["learnList_txt_"+(4*i+2)].visible = false;
					this.learnList["learnList_btn_"+(i+1)].visible = false;
				}
			}
			(this.texts[1] as TextField).htmlText = GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_8"]/*当前真元*/ + "：<font color='#00FF00'>"+ GameCommonData.Player.Role.archaeus+"</font>";
			updataTime_MeridiansLearnList();			//倒计时
			
			var heroProp:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName("HeroProp");
			if(heroProp != null)
			{
				panelBase.x = heroProp.x + heroProp.width -35;
				panelBase.y = heroProp.y;
			}
			dataProxy.meridiansLearnListIsOpen = true;
		}
		
		//加速修炼
		private function quicken():void
		{
//			archaeus
//			GameCommonData.Player.Role.archaeus
			var archaeusNum:int = GameCommonData.Player.Role.archaeus; //当然真元数
			if(archaeusNum < 1 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_19"]/*"你的真元不足，请尽快补充真元"*/, color:0xffff00});
			}
			else if(archaeusNum * 30 < this.currentMeridians.nLeaveTime)
			{
				var sortTime:String = Tools.getTime(archaeusNum * 30);
				facade.sendNotification(EventList.SHOWALERT, {comfrim:comfrimQuicken, cancel:cancelQuicken, info:"<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_20"]/*你当前拥有真元*/ + "<font color='#00FF00'>"+archaeusNum+"</font>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_21"]/*点，可加快的修炼时间是*/ + "<font color='#00FFFF'>"+sortTime+"</font>，"+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_22"]/*是否确认加速*/ + "？</font>" });
			}
			else
			{
				//消耗真元
				var reqArchaeus:int = this.currentMeridians.nLeaveTime / 30 + ((this.currentMeridians.nLeaveTime % 30) > 0? 1:0);
				facade.sendNotification(EventList.SHOWALERT, {comfrim:comfrimQuicken, cancel:cancelQuicken, info:"<font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_23"]/*该经脉加速修炼后可直接完成，需要消耗真元*/ + "<font color='#00FF00'>"+reqArchaeus+"</font>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_24"]/* "点，剩余"*/ + (archaeusNum- reqArchaeus) + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_25"]/*点，是否确认加速*/ + "？</font>" });
			}
		}
		
		private function onButtonClick(event:MouseEvent):void
		{
			
			switch(event.target.name)
			{
				case "learnList_btn_1":
					Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,152);
//					sendNotification(MeridiansEvent.SHOW_ADDZHENYUAN);
					break;					
				case "learnList_btn_2":
					currentMeridians = this.learnQueue[0] as MeridiansTypeVO;
					onBtn(currentMeridians);
					break;
				case "learnList_btn_3":
					currentMeridians = this.learnQueue[1] as MeridiansTypeVO;
					onBtn(currentMeridians);
					break;
				case "learnList_btn_4":
					currentMeridians = this.learnQueue[2] as MeridiansTypeVO;
					onBtn(currentMeridians);
					break;
				case "learnList_btn_5":
					currentMeridians = this.learnQueue[3] as MeridiansTypeVO;
					onBtn(currentMeridians);
					break;
				case "learnList_btn_6":
					currentMeridians = this.learnQueue[4] as MeridiansTypeVO;
					onBtn(currentMeridians);
					break;
			}
		}
		
		/****** 为简化 onButtonClick 函数 *****/
		private function onBtn(meridiansTypeVO:MeridiansTypeVO):void
		{
			var action:int = 141;
			switch(meridiansTypeVO.nState)
			{
				//等待 显示取消按钮
				case 1:
					action = 147;
					break;
				//进行 显示加速按钮
				case 2:
					quicken();
					return ;
				//完成 显示完成
				case 3:
					action = 141;
					break;
			}
			Tools.showMeridiansNet(GameCommonData.Player.Role.Id,currentMeridians.nType,0,action);
		}
		
		//加速经脉处理
		public function comfrimQuicken():void
		{
			Tools.showMeridiansNet(GameCommonData.Player.Role.Id,currentMeridians.nType,0,142);
		}
		public function cancelQuicken():void {}
		
		private function closeUI(event:Event):void
		{
			for(var i:int = 0; i < this.buttons.length; ++i)
			{
				(this.buttons[i] as SimpleButton).removeEventListener(MouseEvent.CLICK,onButtonClick);
			}
			
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				panelBase.removeEventListener( Event.CLOSE,closeUI );
				GameCommonData.GameInstance.GameUI.removeChild( panelBase );
			}
			dataProxy.meridiansLearnListIsOpen = false;
		}
		
		public function updataTime_MeridiansLearnList():void
		{
			var needUpdata:Boolean = false;
			for(var i:int = 1; i <= this.listLength ; i++)
			{
				if(i <= this.learnQueue.length)
				{
					var meridiansTypeVO:MeridiansTypeVO = this.learnQueue[i - 1];
					if( 2 == meridiansTypeVO.nState )
					{
						this.learnList["learnList_txt_"+(4*i)].htmlText = "<font color='#00FF00'>"+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_16"]/*还剩*/ + " "+Tools.getTime(meridiansTypeVO.nLeaveTime)+"</font>";
						needUpdata = true;
					}
				}
			}
			if(needUpdata)
			{
				MeridiansTimeOutComponent.getInstance().addFun2("updataTime_MeridiansLearnList",updataTime_MeridiansLearnList);
			}
			else
			{
				MeridiansTimeOutComponent.getInstance().removeFun2("updataTime_MeridiansLearnList");
			}
		}
		
		public function isFull():Boolean
		{
			if(this.learnQueue.length < this.listLength)
			{
				return false;
			}
			return true;
		}
		
		private function removeUI():void
		{
			closeUI( null );
			while(texts.pop());
			while(buttons.pop());

			this.learnList = null;
			panelBase = null;

		}
	}
}