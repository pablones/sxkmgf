package GameUI.Modules.Stall.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class StallPetMediator extends Mediator
	{
		public static const NAME:String = "StallPetMediator";
		
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase = null;
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		
		public function StallPetMediator()
		{
			super(NAME);
		}
		
		private function get petPanel():MovieClip
		{
			return this.viewComponent as MovieClip
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				StallEvents.SHOWPETLIST,
				StallEvents.REMOVEPETLIST,
//				StallEvents.REFRESH_PET_LIST_STALL
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case StallEvents.SHOWPETLIST:
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.PETLIST});
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					panelBase = new PanelBase(petPanel, petPanel.width-6, petPanel.height+11);
					panelBase.name = "PetPanelStallChoice";
					panelBase.addEventListener(Event.CLOSE, petCloseHandler);
					panelBase.x = StallConstData.PET_DEFAULT_POS.x;
					panelBase.y = StallConstData.PET_DEFAULT_POS.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_stall_med_spm_1" ]);      //"宠物列表"
					init();
					GameCommonData.GameInstance.GameUI.addChild(panelBase);
					StallConstData.PetListPanelIsOpen = true;
					
					break;
				case StallEvents.REMOVEPETLIST:
					gc();
					break;
//				case StallEvents.REFRESH_PET_LIST_CHOICE:
//					initPetChoice();
//					break;
			}
		}
		
		private function init():void
		{
			for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
				StallConstData.petListChoice[key] = UIUtils.DeeplyCopy(GameCommonData.Player.Role.PetSnapList[key]);
			}
			
			initPetChoice();
			
			petPanel.btnPetChose.addEventListener(MouseEvent.CLICK, chosePetHandler);
			petPanel.btnCancel.addEventListener(MouseEvent.CLICK, cancelHandler);
			petPanel.txtCancel.mouseEnabled = false;
		}
		
		private function initPetChoice():void
		{
			StallConstData.SelectedPetSF = null;
			
			if(iScrollPane && petPanel.contains(iScrollPane)) {
				petPanel.removeChild(iScrollPane);
				iScrollPane = null;
				listView = null;
			}
			listView = new ListComponent(false);
			showFilterList();
			iScrollPane = new UIScrollPane(listView);
			iScrollPane.x = 3;
			iScrollPane.y = 3;
			iScrollPane.width = 118;
			iScrollPane.height = 135;
			iScrollPane.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			iScrollPane.refresh();
			petPanel.addChild(iScrollPane);
		}
		
		private function showFilterList():void
		{
			for(var i:Object in StallConstData.petListChoice)
			{
				if(GameCommonData.Player.Role.PetSnapList[i].IsLock == false) {
					var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
					item.name = "petStallChoice_"+i.toString();
					item.doubleClickEnabled = true;
					item.mcSelected.visible = false;
					item.txtName.mouseEnabled = false;
					item.btnChosePet.mouseEnabled = false;
					item.txtName.text = StallConstData.petListChoice[i].PetName;
					item.addEventListener(MouseEvent.CLICK, selectItem);
					item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
					listView.addChild(item);
				}
			}
			listView.width = 115;
			listView.upDataPos();
		}
		
		private function selectItem(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			for(var i:int = 0; i<listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			StallConstData.SelectedPetSF = StallConstData.petListChoice[id];
		}
		
		/** 双击查看宠物属性 */
		private function lookPetInfoHandler(e:MouseEvent):void
		{
			var item:MovieClip = e.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			if(id > 0) {
				PetPropConstData.isSearchOtherPetInto = true;
				dataProxy.PetEudemonTmp = GameCommonData.Player.Role.PetSnapList[id];
				sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:id, ownerId:GameCommonData.Player.Role.Id});
			}
		}
		
		private function chosePetHandler(e:MouseEvent):void
		{
			if(StallConstData.SelectedPetSF && StallConstData.SelectedPetSF.Id) {
				//发送增加宠物的命令
//				sendNotification(StallEvents.PETMOVETOSTALL);
				lockBtn(false);
//				sendNotification(StallEvents.PETMOVETOSTALL);
			}
		}
		
		private function cancelHandler(e:MouseEvent):void
		{
			gc();
		}
		
		private function petCloseHandler(e:Event):void
		{
			gc();
		}
		
		private function gc():void
		{
			var count:int = listView.numChildren - 1;
			while(count>=0)
			{
				if(listView.getChildAt(count))
				{
					var item:MovieClip = listView.getChildAt(count) as MovieClip;
					item.removeEventListener(MouseEvent.CLICK, selectItem);
					listView.removeChild(item);
					item = null;
				}
				count--;
			}
			StallConstData.SelectedPetSF = null;
			iScrollPane = null;
			listView = null;
			StallConstData.PetListPanelIsOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			facade.removeMediator(NAME);
		}
		
		/** 加锁按钮 */
		private function lockBtn(mouseEnabled:Boolean=false):void
		{
			petPanel.btnPetChose.mouseEnabled = mouseEnabled;
		}
		
		
		
		
	}
}



