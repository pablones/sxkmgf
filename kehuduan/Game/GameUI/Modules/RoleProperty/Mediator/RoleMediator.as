package GameUI.Modules.RoleProperty.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Designation.view.mediator.DesignationMediator;
	import GameUI.Modules.Meridians.view.MeridiansMediator;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class RoleMediator extends Mediator
	{
		public static const NAME:String = "RoleMediator";
		private var panelBase:PanelBase;
		private var dataProxy:DataProxy;
		private var isGet:Boolean = false;
		
		//pane
		private var equipMediator:EquipMediator = null;
		private var meridiansMediator:MeridiansMediator = null;
		private var otherMediator:OtherMediator = null;
		private var sourceMediator:SourceMediator = null;
		
		public function RoleMediator()
		{
			super(NAME);
		}
		
		public function get heroProp():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				EventList.SHOWHEROPROP,
				EventList.CLOSEHEROPROP,
				RoleEvents.HEROPROP_PANEL_INIT_POS,
				RoleEvents.HEROPROP_PANEL_STOP_DRAG
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.HEROPROPPANE});
					this.heroProp.mouseEnabled=false;
					panelBase = new PanelBase(heroProp, 370, 383+10);
					panelBase.name = "HeroProp";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_rp_med_rm_hn_1" ]);   //"人物属性"
					registerMediator();
					break;
				case EventList.SHOWHEROPROP:
					facade.sendNotification(RoleEvents.GETFITOUTBYBAG);
					initView();
					//通知新手引导系统
					sendNotification(NewerHelpEvent.OPEN_HEROPROP_PANEL_NOTICE_NEWER_HELP);  //if(NewerHelpData.newerHelpIsOpen) 
				break;
				case EventList.CLOSEHEROPROP:
					panelCloseHandler(null);
				break;
				case RoleEvents.HEROPROP_PANEL_INIT_POS:	
					panelBase.x = UIConstData.DefaultPos1.x;
					panelBase.y = UIConstData.DefaultPos1.y;
				break;
				case RoleEvents.HEROPROP_PANEL_STOP_DRAG:
					if(panelBase) panelBase.IsDrag = false;
				break;
				
			}
		} 
		 
		public function registerMediator():void
		{
			equipMediator = new EquipMediator(heroProp);
			facade.registerMediator(equipMediator);
			meridiansMediator = new MeridiansMediator(heroProp);
			facade.registerMediator(meridiansMediator);
			facade.registerMediator(new SoulMediator(heroProp)); //注册魂魄Mediator
			facade.sendNotification(SoulProxy.INITSOULMEDIATOR); 
			otherMediator = new OtherMediator(heroProp);
			facade.registerMediator(otherMediator);
			sourceMediator = new SourceMediator(heroProp);
			facade.registerMediator(sourceMediator);
			facade.sendNotification(RoleEvents.INITROLEVIEW);
			//注册称号mediat
			facade.registerMediator(new DesignationMediator(heroProp));
		}
		
		private function initView():void
		{
//			SoundManager.PlaySound(SoundList.PANEOPEN);
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			setPage();
			facade.sendNotification(RoleEvents.SHOWPROPELEMENT, RolePropDatas.CurView);
		}
		
		private function setPage():void
		{
			for( var i:int = 0; i<5; i++ )
			{
				heroProp["prop_"+i].gotoAndStop(2);
				heroProp["prop_"+i].buttonMode = true;
				heroProp["prop_"+i].addEventListener(MouseEvent.CLICK, selectView);
				if(i == RolePropDatas.CurView)
				{
					heroProp["prop_"+i].gotoAndStop(1);
				}
			}
		}
		
		private function selectView(event:MouseEvent):void
		{
			if(RolePropDatas.CurView == int(event.target.name.split("_")[1])) return;
			if(RolePropDatas.CurView == SoulMediator.TYPE)
			{
				(facade.retrieveMediator(SoulMediator.NAME) as SoulMediator).gc();
				sendNotification(SoulProxy.CLOSE_ALL_SOUL_PANEL);
			}
			RolePropDatas.CurView = int(event.target.name.split("_")[1]);
			var dis:DisplayObject = heroProp.getChildAt(4);
			if(dis)
			{
				if(dis.name == "designationView")
				{
					sendNotification(DesignationMediator.GC_DESIGNATION_MEDIATOR);
				} 
				if(dis.name != "prop_3")
				{
					heroProp.removeChildAt(4);
				}
			}
			for( var i:int = 0; i<5; i++ )
			{
				heroProp["prop_"+i].gotoAndStop(2);
			}
			heroProp["prop_"+RolePropDatas.CurView].gotoAndStop(1);
			facade.sendNotification(RoleEvents.SHOWPROPELEMENT, RolePropDatas.CurView);
		}
		
		private function panelCloseHandler(event:Event):void
		{
			if(GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				for( var i:int = 0; i<5; i++ )
				{
					heroProp["prop_"+i].removeEventListener(MouseEvent.CLICK, selectView);
				}
				facade.sendNotification(RoleEvents.MEDIATORGC);
				GameCommonData.GameInstance.GameUI.removeChild(panelBase); 
//				SoundManager.PlaySound(SoundList.PANECLOSE);
				dataProxy.HeroPropIsOpen = false;
				panelBase.IsDrag = true;
				//通知新手引导系统
				sendNotification(NewerHelpEvent.CLOSE_HEROPROP_PANEL_NOTICE_NEWER_HELP);   //if(NewerHelpData.newerHelpIsOpen) 
			}
		}
	}
}