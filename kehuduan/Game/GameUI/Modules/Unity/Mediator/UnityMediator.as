package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Command.UnityStartUpCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class UnityMediator extends Mediator
	{
		public static const NAME:String = "UnityMediator";
		public static const PAGENUM:int = 3;				   	/** 主界面分页个数 */
		
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var unity:MovieClip;							/** 帮派主界面 */
		
		
		public function UnityMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWUNITYVIEW,
				EventList.CLOSEUNITYVIEW,
				EventList.ENTERMAPCOMPLETE,
				EventList.CLOSE_NPC_ALL_PANEL,
				UnityEvent.CLOSEUNTIY

			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					this.unity = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("UnityPageView"); 
					panelBase = new PanelBase(unity, 504, 387);
					panelBase.name = "unity";
					panelBase.x = 322;
					panelBase.y = 58;
					panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_uni_med_um_han_1" ] );	 // 帮  派
					registerMediator();
					if(unity != null)
					{
						unity.mouseEnabled = false;
					}
					facade.sendNotification(UnityStartUpCommand.NAME);				//启动帮派MVC
				break;
				case EventList.SHOWUNITYVIEW:
					UnityConstData.iscreating = int(GameCommonData.Player.Role.unityJob-1) / 100;
					if(GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0)		//如果职业除以100得到0，则创建成功，为1，则正在创建中
					{
						if(UnityConstData.dataSendState == true)				//刚上线，帮派详细数据还没有传完的时候
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_um_han_2" ], color:0xffff00});  // 数据传输中
							return;
						}
						showUnityView();
						addLis();
						setPage();
					}
				break;
				case EventList.CLOSEUNITYVIEW:
					if(GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0)
					{
						gcAll();
					}
				break;	
				case EventList.ENTERMAPCOMPLETE:			//创建帮派频道
					facade.sendNotification(EventList.HASUINTY);
				break;
				case EventList.CLOSE_NPC_ALL_PANEL:      	//关闭NPC面板
					if(UnityConstData.isOpenNpcView == true)
					{
						if(dataProxy.UnityIsOpen)     			this.sendNotification(EventList.CLOSEUNITYVIEW);
						if(dataProxy.UnitInfoIsOpen)  			this.sendNotification(UnityEvent.CLOSEUNITYINFOVIEW);
						if(dataProxy.CreateUnitIsOpen)			this.sendNotification(UnityEvent.CLOSECREATEUNITYVIEW);
						if(UnityConstData.respondViewIsOpen)	this.sendNotification(UnityEvent.CLOSERESPONDUNITYVIEW);
					}	
				break;
				case UnityEvent.CLOSEUNTIY:					//被踢出帮派后，关闭帮会界面
					if(panelBase)
					{
						if(GameCommonData.GameInstance.GameUI.contains(panelBase))
						gcAll();
					}
				break;
			}
		}
		
		private function registerMediator():void
		{
			facade.registerMediator(new UnityMainMediator());
			facade.registerMediator(new UnityMenberMediator());
			facade.registerMediator(new HireMediator());
		}
		
		private function showUnityView():void
		{
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = 322 + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = 58 + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = 322;
				panelBase.y = 58;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
//			panelBase.x = 322;
//			panelBase.y = 58;
			facade.sendNotification(UnityEvent.SHOWUNITYPAGEVIEW , this.unity);
		}
		
		private function setPage():void
		{
			for(var i:int = 0; i < PAGENUM ; i++)
			{
				unity["mcUnity_" + i].gotoAndStop(2);
				unity["mcUnity_" + i].buttonMode = true;
				if(i == UnityConstData.unityPage)
				{
					unity["mcUnity_" + i].gotoAndStop(1);
				}
			}
		}		
		
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
			for(var i:int = 0; i < PAGENUM ; i++)
			{
				unity["mcUnity_" + i].addEventListener(MouseEvent.CLICK , changePageHandler);
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			gcAll();
		}
		
		private function gcAll():void
		{
			dataProxy.UnityIsOpen = false;
			if(panelBase)
			{
				if(GameCommonData.GameInstance.GameUI.contains(panelBase))
				{
					GameCommonData.GameInstance.GameUI.removeChild(panelBase);
				}
			}
			while(this.unity.numChildren > PAGENUM)
			{
				this.unity.removeChildAt(this.unity.numChildren - 1);
			}
			facade.sendNotification(UnityEvent.CLOSEUNITYPAGEVIEW);
			
			panelBase.removeEventListener(Event.CLOSE, panelCloseHandler);
		}
		
		/** 点击换页按钮 */
		private function changePageHandler(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			if(i == 2)
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_um_cha_1" ], color:0xffff00});  // 暂未开放
				return;
			}
			if(UnityConstData.unityPage == i) return;
			UnityConstData.unityPage = i;
			setPage();
			if(this.unity.numChildren >= int(PAGENUM+1)) this.unity.removeChildAt(PAGENUM);
			facade.sendNotification(UnityEvent.SHOWUNITYPAGEVIEW , this.unity);
		}
	}
}