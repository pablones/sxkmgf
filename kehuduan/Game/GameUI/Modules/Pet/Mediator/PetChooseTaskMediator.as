package GameUI.Modules.Pet.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.ListComponent;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.Components.UIScrollPane;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetChooseTaskMediator extends Mediator
	{
		public static const NAME:String = "PetChooseTaskMediator";
		
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase = null;
		private var listView:ListComponent;
		private var iScrollPane:UIScrollPane;
		private var taskNeedInfo:Object = null;
		private var showType:String;
		private  const TASK_TYPE:String = "taskShow";
		public static const PLAY_TYPE:String = "playShow";
		public function PetChooseTaskMediator()
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
				PetEvent.OPEN_PET_CHOICE_PANEL_SINGLE,
				PetEvent.CLOSE_PET_CHOICE_PANEL_SINGLE
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetEvent.OPEN_PET_CHOICE_PANEL_SINGLE:
					if(notification.getType() == PLAY_TYPE)	//宠物玩耍
					{
						showType = PLAY_TYPE;
					}
					else
					{
						showType = TASK_TYPE;
						taskNeedInfo = notification.getBody();		//爪宠物任务
					}
					initView();
					break;
				case PetEvent.CLOSE_PET_CHOICE_PANEL_SINGLE:
					gc();
					break;
			}
		}
		
		private function initView():void
		{
			if(!PetPropConstData.petChooseTaskIsOpen) {
				facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:"PetListForTask"});
				dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
				panelBase = new PanelBase(petPanel, petPanel.width-6, petPanel.height+11);
				panelBase.name = "PetPanelStallChoice";
				if(showType == TASK_TYPE)
				{
					panelBase.x = 380;
					panelBase.y = 212;
				}
				else if(showType == PLAY_TYPE)
				{
					PetPropConstData.setViewPosition("petPlayView",this.panelBase);
				}
				panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_med_petc_han_1" ] );    // 宠物选择列表
				panelBase.addEventListener(Event.CLOSE, petCloseHandler);
				panelBase.disableClose();
				init();
				GameCommonData.GameInstance.GameUI.addChild(panelBase);
				PetPropConstData.petChooseTaskIsOpen = true;
				dataProxy.PetCanOperate = false;
			}
		}
		
		private function init():void
		{
			initPetChoice();
		}
		
		private function initPetChoice():void
		{
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
			if(showType) {
				var gamePet:GamePetRole;
				for(var i:Object in GameCommonData.Player.Role.PetSnapList) {
					gamePet = GameCommonData.Player.Role.PetSnapList[i];
					if(gamePet.IsLock == false ) {	
						if(!filterPet(gamePet)) continue;
						var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemSmall");
						item.name = "petChoiceTask_"+i.toString();
						item.doubleClickEnabled = true;
						item.mcSelected.visible = false;
						item.mcSelected.mouseEnabled = false;
						item.txtName.mouseEnabled = false;
						item.btnChosePet.mouseEnabled = false;
						var petName:String = GameCommonData.Player.Role.PetSnapList[i].PetName;
						if(showType == PLAY_TYPE)
						{
							if(gamePet.Type == 1)  petName = '<font color="#00FFFF">'+petName+'</font>';
						}
						item.txtName.htmlText = petName;
						item.addEventListener(MouseEvent.CLICK, selectItem);
						item.addEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
						listView.addChild(item);
					}
				}
				listView.width = 115;
				listView.upDataPos();
			}
		}
		private function filterPet(pet:GamePetRole):Boolean
		{
			var boo:Boolean = true;
			if(showType == TASK_TYPE)
			{
				if(taskNeedInfo)
				{
					if(GameCommonData.Player.Role.UsingPet &&  GameCommonData.Player.Role.UsingPet.Id == pet.Id)
					{
						boo = false;
					}
					else if(pet.FaceType != taskNeedInfo.type)	//是指定类型的宠物
					{
						boo = false;
					}
				}
			}
			if(showType == PLAY_TYPE)
			{
				if(GameCommonData.Player.Role.UsingPet &&  GameCommonData.Player.Role.UsingPet.Id == pet.Id)
				{
					boo = false;
				}
			}
			return boo;
		}
		
		private function selectItem(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			var selectPet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
			if(selectPet.State == 4) 	//附体状态
			{
				var showStr:String = showType == PLAY_TYPE ? GameCommonData.wordDic[ "mod_pet_PetChoose_word_1" ]:GameCommonData.wordDic[ "mod_pet_PetChoose_word_8" ];//附体状态的宠物不能玩耍，请先分离该宠物":"附体状态的宠物无法提交任务，请先分离该宠物
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:showStr, color:0xffff00});        
				return;
			}
			if(showType == PLAY_TYPE)
			{
				if(selectPet.Type != 1) 	//不是宝宝 
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_PetChoose_word_2" ], color:0xffff00});        //选择的宠物不是宝宝
					return;
				}
				if(!isPetCanChoose(selectPet)) return;
				if(selectPet.playNumber >= 3)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_pet_PetChoose_word_3" ], color:0xffff00});//宠物已经玩耍3次        
					return;
				}
			}
			for(var i:int = 0; i<listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
			if(showType == TASK_TYPE)
			{
				PetPropConstData.PetIdSelectedChooseTask = id;
			}
			else if(showType == PLAY_TYPE)
			{
				sendNotification(PetEvent.SHOW_PLAY_PET_MODULE_FROM_SELECT,selectPet.Id,PetPlayMediator.SHOW_SELF_PET);
			}
		}
		
		private function isPetCanChoose(pet:GamePetRole):Boolean
		{
			var boo:Boolean = true;
			var showStr:String;
			if(pet.Level < 50)
			{
				boo = false;
				showStr = GameCommonData.wordDic[ "mod_pet_PetChoose_word_4" ];//"等级低于50，等级不足50不能玩耍";
			}
			else if(pet.Level < 70)
			{
				if(pet.playNumber >= 1)
				{
					boo = false;
					showStr = GameCommonData.wordDic[ "mod_pet_PetChoose_word_5" ];//"该宠物已经玩耍过一次，请给宠物升级";
				}
			}
			else if(pet.Level < 90)
			{
				if(pet.playNumber >= 2)
				{
					boo = false;
					showStr = GameCommonData.wordDic[ "mod_pet_PetChoose_word_6" ];//"该宠物已经玩耍过两次，请给宠物升级";
				}
			}
			else 
			{
				if(pet.playNumber >= 3)
				{
					boo = false;
					showStr = GameCommonData.wordDic[ "mod_pet_PetChoose_word_7" ];//"该宠物已经玩耍过三次，不能再玩耍";
				}
			}
			if(!boo)
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:showStr, color:0xffff00});  
				
			return boo;
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
			PetPropConstData.PetIdSelectedChooseTask = 0;
			var count:int = listView.numChildren - 1;
			while(count>=0)
			{
				if(listView.getChildAt(count))
				{
					var item:MovieClip = listView.getChildAt(count) as MovieClip;
					item.removeEventListener(MouseEvent.CLICK, selectItem);
					item.removeEventListener(MouseEvent.DOUBLE_CLICK, lookPetInfoHandler);
					listView.removeChild(item);
					item = null;
				}
				count--;
			}
			iScrollPane = null;
			listView = null;
			panelBase.removeEventListener(Event.CLOSE, petCloseHandler);
			GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			viewComponent = null;
			panelBase.dispose();
			panelBase = null;
			PetPropConstData.petChooseTaskIsOpen = false;
			facade.removeMediator(NAME);
			dataProxy.PetCanOperate = true;
			showType = null;
			taskNeedInfo = null;
		}
		
	}
}



