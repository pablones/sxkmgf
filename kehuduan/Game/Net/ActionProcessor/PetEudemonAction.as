package Net.ActionProcessor
{
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Depot.Data.DepotEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Mediator.PetPlayMediator;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Data.PetBreedDoubleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetWinningUp.Data.PetWinningEvent;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.Trade.Data.TradeConstData;
	import GameUI.Modules.Trade.Data.TradeEvent;
	import GameUI.MouseCursor.RepeatRequest;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.utils.ByteArray;

	public class PetEudemonAction extends GameAction
	{
		public function PetEudemonAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		/**  处理接收到的消息 */
		public override function Processor(bytes:ByteArray):void 
		{
			var isOthers:Boolean = false;
			
		 	bytes.position = 4;
			var eudemoninfo:Object = new Object();
		 	
		 	eudemoninfo.action  = bytes.readUnsignedShort();
		 	eudemoninfo.amount  = bytes.readUnsignedShort();
		 	eudemoninfo.ownerId = bytes.readUnsignedInt();		//主人ID
		 	
		 	RepeatRequest.getInstance().petItemCount+=eudemoninfo.amount;
		 	for(var i:int = 0 ; i < eudemoninfo.amount; i ++)
		 	{
		 		var petEu:GamePetRole = new GamePetRole();
		 		petEu.Id = bytes.readUnsignedInt();				//宠物ID
		 		if(PetPropConstData.isNewPetVersion)
		 		{
		 			petEu.Type = bytes.readUnsignedByte();			//类型 0野生 1宝宝 2二代
		 			petEu.playNumber = bytes.readUnsignedByte();		//玩耍次数
		 		}
		 		else
		 		{
			 		petEu.Type = bytes.readUnsignedShort();			//类型 0野生 1宝宝 2二代
		 		}
		 		petEu.FaceType = bytes.readUnsignedShort();		//头像
		 		petEu.TakeLevel = bytes.readUnsignedByte();		//携带等级
		 		petEu.LoseTime = bytes.readUnsignedByte();		//合成失败次数
		 		petEu.Sex = bytes.readUnsignedByte();			//性别
		 		if(PetPropConstData.isNewPetVersion)	//新版宠物
		 		{ 
		 			if(petEu.Sex == 2)	//已幻化
		 			{
		 				petEu.isFantasy = true;	
		 			}
		 		 }    
		 		petEu.Character = bytes.readUnsignedByte();		//性格  勇猛==
		 		petEu.Level = bytes.readUnsignedByte(); 		//宠物等级
		 		petEu.Genius = bytes.readUnsignedByte(); 		//天赋
		 		petEu.BreedNow = bytes.readUnsignedByte(); 		//当前繁殖代
		 		petEu.Savvy = bytes.readUnsignedByte();			//悟性
		 		petEu.Price = bytes.readUnsignedInt();			//宠物的出售价格（摆摊）
		 		if(PetPropConstData.isNewPetVersion)
		 		{
			 		petEu.Grade = bytes.readUnsignedByte();	//宠物成长值
			 		petEu.State = bytes.readUnsignedByte();		//宠物状态
			 		petEu.privity = bytes.readUnsignedByte();		//默契
			 		petEu.winning = bytes.readUnsignedByte();		//灵性
		 		}
		 		else
		 		{
			 		petEu.Grade = bytes.readUnsignedInt();			//宠物成长值
			 		petEu.State = 0;								//宠物默认休息状态
		 		}
		 		petEu.PetName = bytes.readMultiByte(16 ,GameCommonData.CODE);//名字
		 		
		 		petEu.OwnerId = eudemoninfo.ownerId;			//主人ID
		 		
		 		if(petEu.Type != 1) petEu.BreedMax = 0;
		 		
		 		if(eudemoninfo.action == 1) {								//更新信息
					if(petEu.OwnerId == GameCommonData.Player.Role.Id) {			//自己的
		 				GameCommonData.Player.Role.PetSnapList[petEu.Id] = petEu;
//		 				if(GameCommonData.Player.Role.PetList[petEu.Id]) {
//		 					delete GameCommonData.Player.Role.PetList[petEu.Id];
//		 				}8.27
		 				sendNotification(PetEvent.PET_UPDATE_SHOW_INFO);			//更新画面
					} else {														//别人的宠物列表
						PetPropConstData.petListOthers[petEu.Id] = petEu;
						isOthers = true;
					}
		 		} else if(eudemoninfo.action == 2) {								//删除宠物
		 			if(GameCommonData.Player.Role.PetSnapList[petEu.Id]) {
		 				delete GameCommonData.Player.Role.PetSnapList[petEu.Id];
		 			}
//		 			if(GameCommonData.Player.Role.PetSnapList[petEu.Id]) {
//		 				delete GameCommonData.Player.Role.PetSnapList[petEu.Id];
//		 			}8.27
		 			sendNotification(PetEvent.PET_DELETE_SUCCESS, petEu.Id);
		 		} else if(eudemoninfo.action == 3) {							//繁殖 添加快照
		 			if(eudemoninfo.ownerId == GameCommonData.Player.Role.Id) {	//自己快照
		 				sendNotification(PetBreedDoubleEvent.ADDPET_SELF_BREEDDOUBLE, petEu.Id);
		 			} else {													//对方快照
		 				sendNotification(PetBreedDoubleEvent.ADDPET_OP_BREEDDOUBLE, petEu);
		 			}
		 		} else if(eudemoninfo.action == 10) {							//仓库宠物快照
		 			DepotConstData.petListDepot[petEu.Id] = petEu;
		 		} else if(eudemoninfo.action == 12) {							//仓库+宠物
		 			DepotConstData.petListDepot[petEu.Id] = petEu;
		 			sendNotification(DepotEvent.IN_OUT_PET_UPDATE_DEPOT);
		 		} else if(eudemoninfo.action == 13) {							//仓库-宠物
		 			delete DepotConstData.petListDepot[petEu.Id];
		 			sendNotification(DepotEvent.IN_OUT_PET_UPDATE_DEPOT);
		 		} else if(eudemoninfo.action == 11) {							//交易，对方加宠物的快照
		 			TradeConstData.petOpDic[petEu.Id] = petEu;
		 			sendNotification(TradeEvent.PET_ADD_OP_TRADE);
		 		} else if(eudemoninfo.action == 14) {							//摆摊，加宠物快照
		 			StallConstData.petListSale[petEu.Id] = petEu;
		 			sendNotification(StallEvents.UPDATE_SALE_PET_STALL);
		 		} else if(eudemoninfo.action == 15) {							//摆摊，减宠物快照
		 			delete StallConstData.petListSale[petEu.Id];
		 			sendNotification(StallEvents.UPDATE_SALE_PET_STALL);
		 		} else if(eudemoninfo.action == 16){							//宠物玩耍，加宠物快照
		 			if(petEu.OwnerId != GameCommonData.Player.Role.Id)
		 			{
		 				sendNotification(PetEvent.SHOW_PLAY_PET_OTHER_PET_INFO,petEu);
		 			}
		 		}
		 		if(PetRuleCommonData.curPageIndex == 7)	//万兽 提升灵性打开着,灵性提升成功后，反馈
		 		{
		 			if(PetRuleCommonData.selectedPet && PetRuleCommonData.selectedPet.Id == petEu.Id) 
		 			{
			 			sendNotification(PetWinningEvent.PET_WINNING_FEEDBACK,petEu.Id);
		 			}
		 		}
		 	}
		 	
		 	if(isOthers) {			//查询别人宠物 列表
		 		sendNotification(PetEvent.SHOW_PET_LIST_OF_PLAYER);
		 	}
		 	
		 	if(eudemoninfo.action == 4) {							//双繁 加锁
	 			if(eudemoninfo.ownerId == GameCommonData.Player.Role.Id) {	//自己加锁
	 				
	 			} else {													//对方加锁
	 				sendNotification(PetBreedDoubleEvent.LOCKED_OP_BREEDDOUBLE);
	 			}
	 		} else if(eudemoninfo.action == 5) {							//双繁 解锁
	 			if(eudemoninfo.ownerId == GameCommonData.Player.Role.Id) {	//自己解锁
	 				sendNotification(PetBreedDoubleEvent.UNLOCK_SELF_BREEDDOUBLE);
	 			} else {													//对方解锁
	 				sendNotification(PetBreedDoubleEvent.UNLOCK_OP_BREEDDOUBLE);
	 			}
	 		} else if(eudemoninfo.action == 6) {							//双繁 取消繁殖（繁殖失败）
	 			sendNotification(PetBreedDoubleEvent.FAIL_BREEDDOUBLE);
	 		} else if(eudemoninfo.action == 7) {							//双繁 开始繁殖，开始成功
	 			sendNotification(PetBreedDoubleEvent.BEGIN_BREEDDOUBLE);
	 		} else if(eudemoninfo.action == 17) {							//玩耍加锁		
	 			if(eudemoninfo.ownerId != GameCommonData.Player.Role.Id)
	 			{
	 				sendNotification(PetEvent.SHOW_PLAY_PET_OTHER_LOCK_PET);
	 			}
	 		} else if(eudemoninfo.action == 18) {							////玩耍解锁		
	 			
	 		} else if(eudemoninfo.action == 19) {		  					////停止玩耍		
	 			if(eudemoninfo.ownerId != GameCommonData.Player.Role.Id)
	 			{
		 			sendNotification(PetEvent.SHOW_PET_PLAY_VIEW,19,PetPlayMediator.CLOSE_BOTH_VIEW);
	 			}
	 		} else if(eudemoninfo.action == 20) {							////开始玩耍	
	 			if(eudemoninfo.ownerId != GameCommonData.Player.Role.Id)
	 			{
		 			sendNotification(PetEvent.SHOW_PET_PLAY_VIEW,20,PetPlayMediator.CLOSE_BOTH_VIEW);
	 			}
	 		}
		} 
	}
}