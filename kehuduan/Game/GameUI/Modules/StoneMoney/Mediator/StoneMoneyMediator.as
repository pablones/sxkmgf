package GameUI.Modules.StoneMoney.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.LoadingView;
	
	import Net.ActionSend.PlayerActionSend;
	
	import flash.display.DisplayObject;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StoneMoneyMediator extends Mediator
	{
		public static const NAME:String="StoneMoneyMediator";
		/** 打开*/
		public static const SHOW_STONEMONEY_UI:String="show_stoneMoney_ui";
			
		private var basePanel:PanelBase;
		private var loader:Loader;
		private var isSendAction:Boolean;
		public static var  distanceTime:int;	//使用时间间隔
		public static var isSendAction:Boolean;	//请求世界聊天里面的【元宝票】信息  MsgMediator.as，ItemInfo.as
		
		public function StoneMoneyMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get viewUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				SHOW_STONEMONEY_UI,
				EventList.ENTERMAPCOMPLETE,
				EventList.UPDATEMONEY,
				EventList.CLOSE_NPC_ALL_PANEL
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch(notification.getName()){
				case SHOW_STONEMONEY_UI:
					if(!basePanel)
					{
						loader = new Loader();
						loader.contentLoaderInfo.addEventListener(Event.COMPLETE,onLoaderComplete);
						loader.load(new URLRequest(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/BallAndticket.swf"));
						LoadingView.getInstance().showLoading();
					}
					else
					{
						showView();
					}
					break;
				case EventList.ENTERMAPCOMPLETE:
					if(this.basePanel.parent)
					{
						onBasePanelCloseHandler(null);
					}
					break;
				case EventList.UPDATEMONEY:
					if(this.isSendAction)
					{
						dealAfterGetAction();
					}
					break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					if(this.basePanel.parent)
					{
						onBasePanelCloseHandler(null);
					}
					break;
									
			}
		}
		
		private function onLoaderComplete(e:Event):void
		{
			LoadingView.getInstance().removeLoading();
			var viewMc:DisplayObject = new (loader.contentLoaderInfo.applicationDomain.getDefinition("ballAndticket"))();
			this.setViewComponent(viewMc);
			this.basePanel=new PanelBase(this.viewUI,this.viewUI.width-12,this.viewUI.height+12);
			this.basePanel.addEventListener(Event.CLOSE,onBasePanelCloseHandler);
			this.basePanel.SetTitleTxt(GameCommonData.wordDic[ "mod_stonemoney_med_smm_1" ]);   //"元宝兑换"
			
			loader.unload();
			loader.contentLoaderInfo.removeEventListener(Event.COMPLETE,onLoaderComplete);
			loader = null;
			
			initView();
			showView();
		}
		
		
		/**
		 * 初始化设置操作 
		 * 
		 */	
		private function initView():void
		{
			(this.viewUI.txt_input as TextField).restrict="0-9";
			(this.viewUI.txt_input as TextField).multiline=false;
			(this.viewUI.txt_input as TextField).mouseWheelEnabled=false;
			(this.viewUI.txt_input as TextField).maxChars=4;
			(this.viewUI.txt_have as TextField).mouseEnabled = false; 
		}
		
		private function showView():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.basePanel))
			{
				this.onBasePanelCloseHandler(null);
				return;
			}
			GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
			
			this.basePanel.x=200;
			this.basePanel.y=100;
			(this.viewUI.btn_exchange as DisplayObject).addEventListener(MouseEvent.CLICK,onMouseClick);
			(this.viewUI.btn_cancel as DisplayObject).addEventListener(MouseEvent.CLICK,onMouseClick);
			(this.viewUI.txt_have as TextField).text = GameCommonData.Player.Role.UnBindRMB.toString();
			(this.viewUI.txt_input as TextField).text = "";
			(this.viewUI.txt_input as TextField).setSelection(0,(this.viewUI.txt_input as TextField).length);
			GameCommonData.GameInstance.stage.focus = this.viewUI.txt_input;
			UIUtils.addFocusLis(this.viewUI.txt_input);
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			switch(me.target.name)
			{
				case "btn_exchange":
					onCommitHandler();
				break;
				case "btn_cancel":
					this.onBasePanelCloseHandler(null);
				break;
			}
		}
			
		/**
		 * 提交 
		 * @param e
		 * 
		 */		
		private function onCommitHandler():void{
			var num:uint=uint(this.viewUI.txt_input.text);
			if(num==0){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stonemoney_med_smm_2" ], color:0xffff00});   //"输入的元宝数量不能为0"
				return ;
			}
			if(num < 10 || num > 2000)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_Stone_med_stone_onC" ], color:0xffff00});   //"每次兑换数量介于10到2000之间"
				return ;
			}
			if(num>GameCommonData.Player.Role.UnBindRMB){	//元宝
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_stonemoney_med_smm_3" ], color:0xffff00});   //"输入的元宝数量超过了你拥有的元宝数量"
				return;
			}
			var data:Array=[0,GameCommonData.Player.Role.Id,0,0,0,num,279,0,0];
			var obj:Object={type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
			isSendAction = true;
		}
		
		private function dealAfterGetAction():void
		{
				isSendAction = false;
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_Stone_med_stone_deal" ], color:0xffff00});	//"兑换成功"
				this.onBasePanelCloseHandler(null);
		}
		
		/**
		 * 关闭面板 
		 * @param e
		 * 
		 */		
		private function onBasePanelCloseHandler(e:Event):void
		{
	 		(this.viewUI.btn_exchange as DisplayObject).removeEventListener(MouseEvent.CLICK,onMouseClick);
			(this.viewUI.btn_cancel as DisplayObject).removeEventListener(MouseEvent.CLICK,onMouseClick);
			
			isSendAction = false;
			UIUtils.removeFocusLis(this.viewUI.txt_input);
	 		basePanel.parent.removeChild(this.basePanel);
		}
		
	}
}