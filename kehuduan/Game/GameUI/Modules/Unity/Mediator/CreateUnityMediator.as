package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CreateUnityMediator extends Mediator
	{
		public static const NAME:String = "CreateUnityMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var gridSprite:MovieClip;
		private var useItem:UseItem;							   	//帮派令牌的数据	
		private var dataArr:Array = new Array();					//包含帮派名，通告，操作号的数组
		private var _isSendOver:Boolean = true;
		
		public function CreateUnityMediator()
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
				UnityEvent.CREATEUNITYGRID,
				UnityEvent.CREATEUNITY,
				UnityEvent.TIMERPROGRESS,
				UnityEvent.TIMERCOMPLETE
				
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.CREATEUNITYVIEW});
					panelBase = new PanelBase(createUnity, createUnity.width+8,createUnity.height+12);
					panelBase.name = "CreateUnityView";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos2.x - 280;
					panelBase.y = UIConstData.DefaultPos2.y;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_cum_han_1" ] );	 // 创建帮派
					if(createUnity != null)
					{
						createUnity.mouseEnabled = false;
						createUnity.mcBind.mouseEnabled = false;
						createUnity.mcUnBind.mouseEnabled = false;
					}
				break;
				
				case UnityEvent.SHOWCREATEUNITYVIEW:
					showCreateUnityView();                                 					//将创建面板加入游戏中
					showMoney();															//显示人物的金钱
					initGrid(); 															//初始化格子
					inittxt(); 																//默认字段
					createUnity.btnCreateUnity.visible = true;								//初始化按钮
					addLis();
				break;
				
				case UnityEvent.CLOSECREATEUNITYVIEW:
					 gcAll();
				break;
				
				case UnityEvent.CREATEUNITYGRID:											//背包令牌拖进格子中
				    var notiData:Object = notification.getBody();
				    if(useItem)
				    {
				    	if(gridSprite.contains(useItem))
				    	{
				    		sendNotification(EventList.BAGITEMUNLOCK , useItem.Id);
				    		gridSprite.removeChild(useItem);
				    	}
				    }
				    if(notiData.type == 630007)
				    {
				    	useItem = new UseItem(notiData.index, notiData.type, gridSprite);
				    	useItem.Num = notiData.amount;
				    	useItem.x = 2;
						useItem.y = 2;
						useItem.Id = notiData.id;
						useItem.IsBind = notiData.isBind;
						useItem.Type = notiData.type;
						useItem.IsLock = false;
						notiData.Item = useItem;
						notiData.IsUsed = true;
						gridSprite.addChild(useItem);
				    }
				    else sendNotification(EventList.BAGITEMUNLOCK , notiData.id);			//物品解锁
				break;
				
				case UnityEvent.CREATEUNITY:												//创建帮派成功后得到的数据
					var data:int = notification.getBody() as int;
					updateData(data);
				break;
				
				case UnityEvent.TIMERPROGRESS:												//3分钟的间隔时间内的操作通知
		    		 UnityConstData.isRespondUnity = false;
		    		 createUnity.btnCreateUnity.addEventListener(MouseEvent.CLICK , infoHandler);							//点击响应按钮弹出信息
		    	break;
		    	
		    	case UnityEvent.TIMERCOMPLETE:												//3分钟的间隔时间完成后的操作通知
		    		UnityConstData.isRespondUnity = true;
		    		createUnity.btnCreateUnity.removeEventListener(MouseEvent.CLICK , infoHandler);						//点击响应按钮弹出信息
		    	break;
				
			}
		}
		
		private function showCreateUnityView():void
		{
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			dataProxy.CreateUnitIsOpen = true;
		}
		/** 初始化格子 */
		private function initGrid():void
		{
			gridSprite = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			createUnity.addChild(gridSprite);
			gridSprite.x = 34;
			gridSprite.y = 158;
			gridSprite.name = "CreateUnityGrid" ;
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
		}	
		
		private function gcAll():void
		{
			UIUtils.removeFocusLis(createUnity.txtUnityName);
			UIUtils.removeFocusLis(createUnity.txtUnityNotice);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			dataProxy.CreateUnitIsOpen = false;
			if(useItem != null)
			{
				sendNotification(EventList.BAGITEMUNLOCK , useItem.Id);			//物品解锁
				gridSprite.removeChild(useItem);
				useItem = null;
			}
			UnityConstData.isOpenNpcView = false;
		}
		
		private function panelCloseHandler(event:Event):void
		{
			gcAll();
		}
		/** 在文本中显示金钱 */
		private function showMoney():void
		{
			createUnity.mcBind.txtMoney.text = UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]);
			ShowMoney.ShowIcon(createUnity.mcBind, createUnity.mcBind.txtMoney);
			createUnity.mcUnBind.txtMoney.text = UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]);
			ShowMoney.ShowIcon(createUnity.mcUnBind, createUnity.mcUnBind.txtMoney);
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
			if(UnityConstData.isRespondUnity == false)										//如果不能响应，申请，创建帮派
			{
				return;
			}
			if(useItem == null)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_cre_1" ], color:0xffff00});  // 创建帮派还需要建帮令牌
//				facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade, cancel:null, isShowClose:false, info: "创建帮派还需要建帮令牌", title:"警 告", comfirmTxt:"确定", cancelTxt:null})
				return;
			}
			dataArr[0] = createUnity.txtUnityName.text;				//帮派名
			dataArr[1] = createUnity.txtUnityNotice.text;			//通告内容
			dataArr[2] = 201;										//创建的操作号
			dataArr[3] = 0;											//默认为0
			dataArr[4] = useItem.Id;
			UnityConstData.unityObj.type = 1107;
			UnityConstData.unityObj.data = dataArr;
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
				UnityActionSend.SendSynAction(UnityConstData.unityObj);//发送创建请求
				isSendOver = false;
				
			}
		}
		/** 确定对话框 */
		private function applyTrade():void
		{
			createUnity.btnCreateUnity.visible = true;
		}
		/** 接收创建帮派数据 */
		private function updateData(data:int):void
		{
			switch(data)
			{
				case 202:										//202为创建成功
					isSendOver = true;
					facade.sendNotification(UnityEvent.CLOSECREATEUNITYVIEW);
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_cum_upd_1" ], color:0xffff00}); // 申请创建帮派成功，请在24小时内邀请5位玩家响应帮派，方可完成建帮任务。
//					facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade, cancel:null, isShowClose:false, info: "申请创建帮派成功，请在24小时内邀请5位玩家响应帮派，方可完成建帮任务。", title:"提 示", comfirmTxt:"确定", cancelTxt:null}); 
//					sendNotification(EventList.BAGITEMUNLOCK, useItem.Id);			//物品解锁
				break;
				case 203:										//203为创建失败
//					facade.sendNotification(EventList.SHOWALERT, {comfrim:applyTrade, cancel:null, isShowClose:false, info: "创建失败", title:"提 示", comfirmTxt:"确定", cancelTxt:null});
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
		
	}
}