package GameUI.Modules.IdentifyingCode.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyingCode.Data.IdentifyingCodeConst;
	import GameUI.Modules.IdentifyingCode.Data.IdentifyingCodeData;
	import GameUI.Modules.Meridians.Components.MeridiansTimeOutComponent;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	
	import Net.ActionProcessor.EncyptAction;
	import Net.ActionSend.EncryptSend;
	
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.system.ApplicationDomain;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class IdentifyingCodeMediator extends Mediator
	{
		public static const NAME:String="YanZhengMaMediator";

		private var panelBase:PanelBase;
		private var mainView:MovieClip;

		/** 验证码 */
		private var captcha:Captcha;

		private var loader:Loader;
		/** true：正在加载中 */
		private var loading:Boolean=false;
		/** true：加载成功 */
		private var loadSucceed:Boolean=false;
		/** 临时数据 */
		private var data:Object = null;

		public function IdentifyingCodeMediator()
		{
			super(NAME);
		}

		public override function listNotificationInterests():Array
		{
			return [IdentifyingCodeConst.SHOW_YANZHENGMA_VIEW, 
					IdentifyingCodeConst.CLOSE_YANZHENGMA_VIEW, 
					IdentifyingCodeConst.UPDATE_YANZHENGMA_CODE,

				];
		}

		public override function handleNotification(notification:INotification):void
		{
			switch (notification.getName())
			{
				case IdentifyingCodeConst.SHOW_YANZHENGMA_VIEW:
					show();
					break;
				case IdentifyingCodeConst.CLOSE_YANZHENGMA_VIEW:
					close();
					break;
				case IdentifyingCodeConst.UPDATE_YANZHENGMA_CODE:
					data = notification.getBody();
					updata();
					break;
			}
		}

		private function init():void
		{
			if (loading)
				return;
			if (!loadSucceed)
			{
				load();
				return;
			}
			panelBase=new PanelBase(mainView, mainView.width + 8, mainView.height + 12);
			panelBase.disableClose();
			panelBase.SetTitleTxt(IdentifyingCodeData.yanZhengName);
			captcha=new Captcha(IdentifyingCodeData.codeLen,IdentifyingCodeData.codeSize);
			captcha.x=27;
			captcha.y=67;
			captcha.buttonMode = true;
			if(IdentifyingCodeData.YZMSite)
			{
				captcha.x=IdentifyingCodeData.YZMSite[0];
				captcha.y=IdentifyingCodeData.YZMSite[1];
			}
			mainView.addChild(captcha);
			IdentifyingCodeData.isInit=true;
			show();
		}

		private function show():void
		{
			if (!IdentifyingCodeData.isInit)
			{
				init();
				return;
			}
			if (!IdentifyingCodeData.isShowView)
			{
				GameCommonData.GameInstance.WorldMap.addChild(panelBase);
				addLis();
				IdentifyingCodeData.leftTime = IdentifyingCodeData.LEFTMAX;
				MeridiansTimeOutComponent.getInstance().addFun1( "YanZhengMaMediator_upDateLeftTime" , upDateLeftTime);
				/** 让玩家停止移动 */
				GameCommonData.Scene.PlayerStopAtIC();
				panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth-GameConfigData.GameWidth)/2 + 160;
				panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight-GameConfigData.GameHeight)/2 + 20;
			}
			IdentifyingCodeData.isShowView=true;
			updata();
		}

		private function updata():void
		{
			if (!IdentifyingCodeData.isShowView)
			{
				show();
				return;
			}
			var time:int = 0;
			var id:int = 1;
			if(data)
			{
				time = data.time
				id = data.id;
			}
			if(time != IdentifyingCodeData.YanZhengTime)
			{
				IdentifyingCodeData.YanZhengTime = time;
				if(1 == time)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info: IdentifyingCodeData.tiShi1, color: 0xffff00});
				}
				else if(2 == time)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info: IdentifyingCodeData.tiShi2, color: 0xffff00});
				}
			}
			captcha.setCaptcha(id - 1);
		}
		
		private function close(event:Event=null):void
		{
			if (IdentifyingCodeData.isShowView)
			{
				GameCommonData.GameInstance.WorldMap.removeChild(panelBase)
				removeLis();
				IdentifyingCodeData.isShowView=false;
				IdentifyingCodeData.leftTime = 0;
			}
		}

		private function addLis():void
		{
			UIUtils.addFocusLis(mainView.txt_YanZhengMa_code);
			UIConstData.FocusIsUsing=true;
			mainView.txt_YanZhengMa_code.addEventListener(KeyboardEvent.KEY_UP, onKey);
			
			panelBase.addEventListener(Event.CLOSE, close);
			mainView.btn_YanZhengMa.addEventListener(MouseEvent.CLICK, onClick);
			mainView.txt_YanZhengMa_update.addEventListener(MouseEvent.CLICK, updateCode);
			captcha.addEventListener(MouseEvent.MOUSE_DOWN, updateCode);
			
		}

		private function removeLis():void
		{
			UIUtils.removeFocusLis(mainView.txt_YanZhengMa_code);
			UIConstData.FocusIsUsing=false;
			GameCommonData.isFocusIn = false;
			mainView.txt_YanZhengMa_code.removeEventListener(KeyboardEvent.KEY_UP, onKey);
			
			panelBase.removeEventListener(Event.CLOSE, close);
			mainView.btn_YanZhengMa.removeEventListener(MouseEvent.CLICK, onClick);
			mainView.txt_YanZhengMa_update.removeEventListener(MouseEvent.CLICK, updateCode);
			captcha.removeEventListener(MouseEvent.MOUSE_DOWN, updateCode);
			GameCommonData.GameInstance.stage.focus = null;
		}

		
		private function onKey(event:KeyboardEvent):void
		{
			if (13 == event.keyCode)
			{
				send();
			}
		}

		private function onClick(event:Event):void
		{
			send();
		}

		/** 更改验证码 */
		private function updateCode(event:Event):void
		{
			if(getTimer() - IdentifyingCodeData.lastTime > IdentifyingCodeData.IntervalTime)
			{
				var obj:Object = new Object();
				obj.type = EncyptAction.YANZHENGMA_GET_UPDATA;
				obj.oldPwd = "";
				obj.newPwd = "";
				EncryptSend.send(obj);
				IdentifyingCodeData.lastTime = getTimer();
			}
		}

		/** 发送验证码 */
		private function send():void
		{
			var code:String=(mainView.txt_YanZhengMa_code.text as String).toUpperCase();
			if (code.length < 1)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info: IdentifyingCodeData.tiShi, color: 0xffff00});
				return;
			}
			var obj:Object = new Object();
			obj.type = EncyptAction.YANZHENGMA_SEND_ACC;
			obj.oldPwd = code;
			obj.newPwd = "";
			EncryptSend.send(obj);
			mainView.txt_YanZhengMa_code.text = "";
		}

		//更新剩余时间
		public function upDateLeftTime():void
		{
			var needStart:Boolean = false;
			if( IdentifyingCodeData.leftTime> 0 )
			{
				IdentifyingCodeData.leftTime--;
				mainView.txt_YanZhengMa_leftTime.text = IdentifyingCodeData.str1 + IdentifyingCodeData.leftTime + IdentifyingCodeData.str2;
			}
			else
			{
				MeridiansTimeOutComponent.getInstance().removeFun1("YanZhengMaMediator_upDateLeftTime");
			}
		}
		
		/* 加载资源 */
		private function load():void
		{
			loading=true;
			loader=new Loader();
			var request:URLRequest=new URLRequest();
			var adr:String=GameCommonData.GameInstance.Content.RootDirectory + IdentifyingCodeData.resourcePath;
			request.url=adr;
			loader.contentLoaderInfo.addEventListener(Event.COMPLETE, onComplete);
			loader.load(request);
		}

		private function onComplete(event:Event):void
		{
			var domain:ApplicationDomain=event.target.applicationDomain as ApplicationDomain;

			if (domain.hasDefinition("YanZhengMa"))
			{
				var YanZhengMa:Class=domain.getDefinition("YanZhengMa") as Class;
				mainView=new YanZhengMa();
			}
			IdentifyingCodeData.YZM=loader.contentLoaderInfo.content["YZM"] as Array;
			IdentifyingCodeData.YZMSite = loader.contentLoaderInfo.content["YZMSite"] as Array;
			IdentifyingCodeData.colors = loader.contentLoaderInfo.content["colors"] as Array;
			IdentifyingCodeData.YZMBackColor = loader.contentLoaderInfo.content["YZMBackColor"] as Array;
			IdentifyingCodeData.codeLen = loader.contentLoaderInfo.content["codeLen"] as int;
			if(loader.contentLoaderInfo.content["codeSize"])
			{
				IdentifyingCodeData.codeSize = loader.contentLoaderInfo.content["codeSize"] as int;
			}
			
			loader.contentLoaderInfo.removeEventListener(Event.COMPLETE, onComplete);
			loader=null;
			loading=false;
			loadSucceed=true;
			init();
		}
	}
}
