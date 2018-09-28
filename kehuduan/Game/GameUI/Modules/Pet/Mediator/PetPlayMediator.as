package GameUI.Modules.Pet.Mediator
{
	import Controller.PlayerSkinsController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.items.FaceItem;
	import GameUI.View.items.MoneyItem;
	
	import Net.ActionProcessor.PlayerAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetPlayMediator extends Mediator
	{
		public static const NAME:String = "PetPlayMediator";
		private var panelBase:PanelBase;
		private var bindMoneyItem:MoneyItem;
		private var unBindMoneyItem:MoneyItem;
		private var needMoney:MoneyItem;
		private var dataProxy:DataProxy = null;
		public static const SHOW_SELF_PET:String = "showSelfPet";
		public static const SHOW_OTHER_PET:String = "showOtherPet";
		public static const CLOSE_BOTH_VIEW:String = "closeBothView";	//关闭两个面板
		private var showType:String;
		private var isCanSendPlay:int;	//为2即两边都锁定
		private var playPetData:Array;	//玩耍的宠物  0 自己宠物  1 对方宠物
		private var isPetLocked:Boolean;	//是否锁定
		public function PetPlayMediator()
		{
			super(NAME);
		}
		
		private function get mainView():MovieClip
		{
			return viewComponent as MovieClip;
		}
		
		override public function listNotificationInterests():Array
		{
			return [
				PetEvent.SHOW_PET_PLAY_VIEW,
				PetEvent.SHOW_PLAY_PET_MODULE_FROM_SELECT,
				PetEvent.SHOW_PLAY_PET_OTHER_PET_INFO,
				PetEvent.SHOW_PLAY_PET_OTHER_LOCK_PET,
				EventList.UPDATEMONEY
			];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case PetEvent.SHOW_PET_PLAY_VIEW:
					showType = notification.getType();
					if(showType == CLOSE_BOTH_VIEW)
					{
						if(notification.getBody() == 20)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_PetPlay_word_1" ], color:0xffff00 } );   // "开始玩耍"
						}
						onPanelClose(null);
						return;
					}
					if(!panelBase)
					{
						initView();	
					}
					else
					{
						showView();
					}
				break;
				case PetEvent.SHOW_PLAY_PET_MODULE_FROM_SELECT:
					if(isPetLocked)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_dep_med_dep_btn_3" ], color:0xffff00 } );   // "宠物已锁定"
						return;
					}
					showType = notification.getType();
					var petId:int = int(notification.getBody());
					updateViewState(petId);
					playPetData[0] = GameCommonData.Player.Role.PetSnapList[petId];
				break;
				case PetEvent.SHOW_PLAY_PET_OTHER_PET_INFO:		//显示对方玩家快照
					showType = SHOW_OTHER_PET;
					var pet:GamePetRole = notification.getBody() as GamePetRole;
					updateViewState(0,pet);
					playPetData[1] = pet;
				break;
				case PetEvent.SHOW_PLAY_PET_OTHER_LOCK_PET:
					addFrame(SHOW_OTHER_PET);
				break;
				case EventList.UPDATEMONEY:
					upDataMoney();
				break;
			}
		}
		
		private function initView():void
		{
			var view:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("petPlayView");
			setViewComponent(view);
			panelBase = new PanelBase( mainView,375,mainView.height+12 );
			panelBase.name = "petPlayView";
			panelBase.SetTitleTxt( GameCommonData.wordDic[ "mod_pet_PetPlay_word_2" ] );   //宠物玩耍
			dataProxy = facade.retrieveProxy( DataProxy.NAME) as DataProxy;
			
			mainView.txt_selfPetName.mouseEnabled = false;
			mainView.txt_otherPetName.mouseEnabled = false;
			mainView.txt_selfPetPlayNum.mouseEnabled = false;
			mainView.txt_otherPetPlayNum.mouseEnabled = false;
			mainView.btnLockOther.mouseEnabled = false;
			MasterData.setGrayFilter(mainView.btnLockOther);
			mainView.mcSelf_Photo.mouseEnabled = false;
			mainView.mcOther_Photo.mouseEnabled = false;
			
			bindMoneyItem = new MoneyItem();
			bindMoneyItem.x = 151;
			bindMoneyItem.y = 392;
			unBindMoneyItem = new MoneyItem();
			unBindMoneyItem.x = 151;
			unBindMoneyItem.y = 414;
			needMoney = new MoneyItem();
			needMoney.x = 84;
			needMoney.y = 370;
			mainView.addChild(bindMoneyItem);
			mainView.addChild(unBindMoneyItem);
			mainView.addChild(needMoney);
			upDataMoney();
			upDateNeedMoney( 0 );
			
			showView();
		}
		
		private function upDataMoney():void
		{
			this.bindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"]));
			this.unBindMoneyItem.update(UIUtils.getMoneyStandInfo(GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"]));
		}
		
		private function upDateNeedMoney( money:uint ):void
		{
			this.needMoney.update(UIUtils.getMoneyStandInfo( money, ["\\se","\\ss","\\sc"]) );
		}
		
		private function showView():void
		{
			if(GameCommonData.GameInstance.GameUI.contains(this.panelBase))
			{
				this.onPanelClose(null);
				return;
			}
			if(dataProxy.petRuleIsOpen) 
			{
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);
			} 
			if(dataProxy.PetIsOpen) 
			{
				sendNotification(EventList.CLOSEPETVIEW);
			}
			dealEventListeners(true);
			setViewState();
			playPetData = [2];
			GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
			facade.registerMediator(new PetChooseTaskMediator());
			sendNotification(PetEvent.OPEN_PET_CHOICE_PANEL_SINGLE,null,PetChooseTaskMediator.PLAY_TYPE);	//打开宠物选择面板
		}
		
		private function setViewState():void
		{
			mainView.btnLockSelf.mouseEnabled = false;
			MasterData.setGrayFilter(mainView.btnLockSelf);
			mainView.btnSure.mouseEnabled = false;
			MasterData.setGrayFilter(mainView.btnSure);
			panelBase.x = 150;
			panelBase.y = 50;
			mainView.txt_selfPetName.text = "";
			mainView.txt_otherPetName.text = "";
			mainView.txt_selfPetPlayNum.text = "";
			mainView.txt_otherPetPlayNum.text = "";
			upDateNeedMoney(20000);
		}
		private function dealEventListeners(isAddEventLis:Boolean):void
		{
			if(isAddEventLis)
			{
				this.panelBase.addEventListener(Event.CLOSE,onPanelClose);
				mainView.btnLockSelf.addEventListener(MouseEvent.CLICK,onMouseClick);
				mainView.btnSure.addEventListener(MouseEvent.CLICK,onMouseClick);
			}
			else
			{
				this.panelBase.removeEventListener(Event.CLOSE,onPanelClose);
				mainView.btnLockSelf.removeEventListener(MouseEvent.CLICK,onMouseClick);
				mainView.btnSure.removeEventListener(MouseEvent.CLICK,onMouseClick);
			}
		}
		
		/**
		 * 从列表选择宠物后在界面显示 
		 * @param petId
		 * 
		 */		
		private function updateViewState(petId:int = 0,otherPet:GamePetRole = null):void
		{
			var showPet:GamePetRole = petId ? GameCommonData.Player.Role.PetSnapList[petId]:otherPet;
			var photo:FaceItem = getPetPhoto(showPet,showType);
			if(showType == SHOW_SELF_PET)
			{
				mainView.txt_selfPetName.text = showPet.PetName;
				mainView.txt_selfPetPlayNum.text = showPet.playNumber;
				mainView.btnLockSelf.mouseEnabled = true; 
				mainView.btnLockSelf.filters = null;
				mainView.mcSelf_Photo.addChild(photo)
				PetNetAction.opPet(PlayerAction.NEWPET_PLAY_SELECT, petId);
			}
			else
			{
				mainView.txt_otherPetName.text = showPet.PetName;
				mainView.txt_otherPetPlayNum.text = showPet.playNumber;
				mainView.mcOther_Photo.addChild(photo);
			}
		}
		/**
		 *获得头像 
		 * @param showPet
		 * @param showType
		 * @return 
		 * 
		 */		
		private function getPetPhoto(showPet:GamePetRole,showType:String):FaceItem
		{
			removeDisplayByName(showType + "_photo");
			var faceType:int = showPet.FaceType;
			if(showPet.Savvy >= 7) 
			{
				faceType = PetPropConstData.getFaceType(faceType); 
			}
			
			var faceStr:String = faceType.toString();
			if( showPet.Type > 1 )
			{
				var petV:XML = PlayerSkinsController.GetPetV( showPet.ClassId.toString() , showPet.Type - 1 ); 
				if( petV != null)
				{
					faceStr = petV.@Face;
				}
			}
			var petPhoto:FaceItem = new FaceItem(faceStr,mainView, "EnemyIcon");
			petPhoto.name = showType + "_photo";
			petPhoto.addEventListener(MouseEvent.CLICK,onMouseClick);
			petPhoto.buttonMode = true;
			petPhoto.offsetPoint = new Point(0,0);
			return petPhoto;
		}
		
		
		/**
		 *锁定后加绿格子 
		 * @param showType
		 * 
		 */		
		private function addFrame(showType:String):void
		{
			var shape:Shape = new Shape();
			shape.name = showType + "_frame";
			shape.graphics.lineStyle(2,0x00FF00);
			shape.graphics.drawRect(0,0,120,134);
			shape.graphics.endFill();
			if(showType == SHOW_SELF_PET)
			{
				shape.x = mainView.mcSelf_Photo.x - 5;
				shape.y = mainView.mcSelf_Photo.y - 17;
			}
			else
			{
				shape.x = mainView.mcOther_Photo.x - 5;
				shape.y = mainView.mcOther_Photo.y - 17;
			}
			mainView.addChild(shape);
			isCanSendPlay++
			if(isCanSendPlay == 2)	//两边玩家都锁定
			{
				mainView.btnSure.mouseEnabled = true;
				mainView.btnSure.filters = null;
			}
		}
		
		private function onMouseClick(me:MouseEvent):void
		{
			var name:String = me.target.name;
			switch(name)
			{
				case "btnLockSelf":
					sendInfo("lockSelf");
				break;
				case "btnSure":
					sendInfo("sure");
				break;
				default:
					if(name.split("_")[1] == "Photo")
					{
						removeDisplayByName(SHOW_SELF_PET + "_photo");
					} 
				break;
			}
		}
		private function sendInfo(method:String):void
		{
			if(method == "lockSelf")
			{
				if(!dataProxy.isTeamMember)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_2" ], color:0xffff00 } );   // 需组队才能繁殖
					return;
				}	
				this.addFrame(SHOW_SELF_PET);
				mainView.btnLockSelf.mouseEnabled = false;
				MasterData.setGrayFilter(mainView.btnLockSelf);
//				mainView.mcSelf_Photo.removeEventListener(MouseEvent.CLICK,onMouseClick);
				PetNetAction.opPet(PlayerAction.NEWPET_PLAY_LOCKED);	//锁定宠物
				isPetLocked = true;
				(playPetData[0] as GamePetRole).IsLock = true;
			}
			else if(method == "sure")
			{
				if(!dataProxy.isTeamLeader)	//是队长
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_pet_pbd_med_petb_sur_3" ], color:0xffff00 } );//"你不是队长"
					return;
				}
				PetNetAction.newPetOperate(PlayerAction.NEWPET_PLAY_SURE, playPetData[0].Id, playPetData[1].Id, GameCommonData.Player.Role.Id);
			}
		}
		/**
		 *根据名称移除界面显示对象 
		 * @param type
		 * 
		 */		
		private function removeDisplayByName(type:String):void
		{
			
			var parent:DisplayObjectContainer;
			if(type.split("_")[1] == "photo")
			{
				parent = type.split("_")[0] == SHOW_SELF_PET ? mainView.mcSelf_Photo:mainView.mcOther_Photo;
			}
			else
			{
				parent = mainView;
			}
			var phote:DisplayObject = parent.getChildByName(type);
			if(phote)
			{
				phote.parent.removeChild(phote);
				phote = null;
			}
		}
		
		private function onPanelClose(e:Event):void
		{
			if(this.panelBase.parent)
			{
				dealEventListeners(false);
				removeDisplayByName(SHOW_SELF_PET + "_photo");
				removeDisplayByName(SHOW_OTHER_PET + "_photo");
				removeDisplayByName(SHOW_SELF_PET + "_frame");
				removeDisplayByName(SHOW_OTHER_PET + "_frame");
				sendNotification(PetEvent.CLOSE_PET_CHOICE_PANEL_SINGLE);
				if(showType != CLOSE_BOTH_VIEW)
				{
					PetNetAction.opPet(PlayerAction.NEWPET_PLAY_CANCEL);
				}
				this.panelBase.parent.removeChild(this.panelBase);
				showType = null;
				isPetLocked = false;
				isCanSendPlay = 0;
			}
		}
	}
}