package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.UseItem;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewCreateUnityMediator extends Mediator
	{
		public static const NAME:String = "NewCreateUnityMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var useItem:UseItem;							   	//帮派令牌的数据	
		private var dataArr:Array = new Array();					//包含帮派名，通告，操作号的数组
		private var _isSendOver:Boolean = true;
		
		private var tokenType:int = 630007;							//建帮令牌的type
		private var time:int;
		private var timeupId:uint;
		
		public function NewCreateUnityMediator()
		{
			super(NAME);
		}
		
		private function get createUnity():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
        
        public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				UnityEvent.SHOWCREATEUNITYVIEW,
				UnityEvent.CLOSECREATEUNITYVIEW, 
				UnityEvent.CREATEUNITY,
				EventList.TIMEUP
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification( EventList.GETRESOURCE, { type:UIConfigData.MOVIECLIP, mediator:this, name:"NewCreateUnityViewRes" } );
					panelBase = new PanelBase(createUnity, createUnity.width+8,createUnity.height+12);
					panelBase.name = "NewCreateUnityView";
//					panelBase.x = UIConstData.DefaultPos2.x - 280;
//					panelBase.y = UIConstData.DefaultPos2.y;
					
					if( GameCommonData.fullScreen == 2 )
					{
						panelBase.x = UIConstData.DefaultPos2.x - 280 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
						panelBase.y = UIConstData.DefaultPos2.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
					}else
					{
						panelBase.x = UIConstData.DefaultPos2.x - 280;
						panelBase.y = UIConstData.DefaultPos2.y;
					}
					
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_cum_han_1" ] );	 // 创建帮派
					if(createUnity != null)
					{
						createUnity.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWCREATEUNITYVIEW:
					if ( dataProxy.CreateUnitIsOpen )
					{
						gcAll();
					}
					else
					{
						showCreateUnityView();                                 					//将创建面板加入游戏中
						inittxt(); 																//默认字段
						addLis();	
					}
				break;
				
				case UnityEvent.CLOSECREATEUNITYVIEW:
					 gcAll();
				break;
				
				case UnityEvent.CREATEUNITY:												//创建帮派成功后得到的数据
					var data:int = notification.getBody() as int;
					updateData(data);
				break;
				
				/** 副本时间警告*/
				case EventList.TIMEUP:
					time = (notification.getBody() as Object).taskId;
					timeupId = setInterval(timeset , 1000);
				break;
			}
		}
		
		private function showCreateUnityView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			dataProxy.CreateUnitIsOpen = true;
		}

		/** 默认文本框内的内容 */
		private function inittxt():void
		{
			createUnity.txtUnityName.text   = GameCommonData.wordDic[ "mod_uni_med_cum_ini_1" ];  // 点击输入帮派名称
			createUnity.txtUnityNotice.text = GameCommonData.wordDic[ "mod_uni_med_cum_ini_2" ];  // 点击输入帮派公告
		}
		
		private function addLis():void
		{
			UIUtils.addFocusLis(createUnity.txtUnityName);
			UIUtils.addFocusLis(createUnity.txtUnityNotice);
			createUnity.txtUnityName.addEventListener(MouseEvent.CLICK,clearnametxtHandLer);
			createUnity.txtUnityNotice.addEventListener(MouseEvent.CLICK,clearnoticetxtHandLer);
			createUnity.btnCancel.addEventListener(MouseEvent.CLICK,panelCloseHandler);
			createUnity.btnCreateUnity.addEventListener(MouseEvent.CLICK,CreateUnityHandler);
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
		}	
		
		private function gcAll():void
		{
			if ( panelBase && GameCommonData.GameInstance.GameUI.contains( panelBase ) )
			{
				UIUtils.removeFocusLis(createUnity.txtUnityName);
				UIUtils.removeFocusLis(createUnity.txtUnityNotice);
				
				createUnity.txtUnityName.removeEventListener(MouseEvent.CLICK,clearnametxtHandLer);
				createUnity.txtUnityNotice.removeEventListener(MouseEvent.CLICK,clearnoticetxtHandLer);
				createUnity.btnCancel.removeEventListener(MouseEvent.CLICK,panelCloseHandler);
				createUnity.btnCreateUnity.removeEventListener(MouseEvent.CLICK,CreateUnityHandler);
				panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
				
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				dataProxy.CreateUnitIsOpen = false;
				UnityConstData.isOpenNpcView = false;
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			gcAll();
		}

		/** 点击帮派名称文本，默认字段消失*/
		private function clearnametxtHandLer(e:MouseEvent):void
		{
			createUnity.txtUnityName.text = "";
			createUnity.txtUnityName.maxChars = 7; 
			createUnity.txtUnityName.removeEventListener(MouseEvent.CLICK,clearnametxtHandLer);
		}
		/** 点击帮派公告文本，默认字段消失*/
		private function clearnoticetxtHandLer(e:MouseEvent):void
		{
			createUnity.txtUnityNotice.text = "";
			createUnity.txtUnityNotice.maxChars = 70;
			createUnity.txtUnityNotice.removeEventListener(MouseEvent.CLICK,clearnoticetxtHandLer);
		}
		/** 点击创建按钮 */
		private function CreateUnityHandler(e:MouseEvent):void
		{
			if(_isSendOver == false)
			{
				return;
			}
			
			var tokenItem:Object = BagData.getItemByType( this.tokenType );
			if( !tokenItem )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_unityN_med_newc_cre_1" ], color:0xffff00 } );  //你没有建帮令牌
				return;
			}
			var sendObj:Object = new Object();
			dataArr[0] = createUnity.txtUnityName.text;				//帮派名
			dataArr[1] = createUnity.txtUnityNotice.text;			//通告内容
			dataArr[2] = 201;										//创建的操作号
			dataArr[3] = 0;											//默认为0
			dataArr[4] = tokenItem.id;
			sendObj.type = 1107;
			sendObj.data = dataArr;
			//如果等级未达到25级，就弹出对话框，不发送请求
			var i :int = GameCommonData.Player.Role.Level;
			createUnity.txtUnityName.text = createUnity.txtUnityName.text.replace(/^\s*|\s*$/g,"").split(" ").join("");					//去掉空格
			if(GameCommonData.Player.Role.Level <25)  
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_2" ], color:0xffff00});  // 你的等级未达到25级，不能创建帮派
			else if(GameCommonData.Player.Role.UnBindMoney + GameCommonData.Player.Role.BindMoney < 1000000)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_3" ], color:0xffff00});  // 你的金钱不足，不能创建帮派
			}
			else if(createUnity.txtUnityName.text == "")
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_4" ], color:0xffff00}); // 帮派名不能为空
			}
			else if(createUnity.txtUnityNotice.text == "")
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_5" ], color:0xffff00}); // 帮派公告不能为空
			}
			else if(createUnity.txtUnityName.text  == GameCommonData.wordDic[ "mod_uni_med_cum_ini_1" ] || createUnity.txtUnityNotice.text == GameCommonData.wordDic[ "mod_uni_med_cum_ini_2" ] ) // 点击输入帮派名称  点击输入帮派公告
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_6" ], color:0xffff00});  // 请填写帮派信息
			}
			else if(UIUtils.isPermitedRoleName(createUnity.txtUnityName.text) == false  || UIUtils.isPermitedRoleName(createUnity.txtUnityNotice.text) == false)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_7" ], color:0xffff00});  // 文本内容不合法
			}															
			else
			{
				UnityActionSend.SendSynAction( sendObj );//发送创建请求
				isSendOver = false;
			}
		}

		/** 接收创建帮派数据 */
		private function updateData(data:int):void
		{
			switch(data)
			{
				case 202:										//202为创建成功
					isSendOver = true;
					gcAll();
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_unityN_med_newc_upd_1" ], color:0xffff00}); //恭喜你创建帮派成功             申请创建帮派成功，请在24小时内邀请5位玩家响应帮派，方可完成建帮任务。
					sendNotification( NewUnityCommonData.CLOSE_JOIN_UNITY_NEW );
				break;
			}
		}
		/** 点击响应按钮弹出不可用信息*/
		private function infoHandler(e:MouseEvent):void
		{
			 facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_aum_inf_1" ], color:0xffff00}); // 两次操作需要间隔三分钟
		}
		/** 帮派数据是否接受成功 */
		private function set isSendOver(isOk:Boolean):void
		{
			_isSendOver = isOk;
			if(isOk == false)
			{
				setTimeout(changeSendState , 1000 * 4);		//发送请求后的4秒内，改变状态
			}
		}
		/** 改变数据传输状态 */
		private function changeSendState():void
		{
			_isSendOver = true;
		}
		
			/** 副本时间提示 */
		private function timeset():void
		{
			time -= 1 ;
			facade.sendNotification(HintEvents.RECEIVEINFO, {info:String(time), color:0xffff00});
			if(time <= 0) clearInterval(timeupId);
		}
		
	}
}