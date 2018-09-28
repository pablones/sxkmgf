package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.command.AutoEatDragCommand;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayPetCommand;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Mediator.PetChangeSexMediator;
	import GameUI.Modules.Pet.Mediator.PetFantasyMediator;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUp;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUpMessage;
	import GameUI.Proxy.DataProxy;
	
	import Net.GameAction;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.utils.ByteArray;

	public class PetAtt extends GameAction
	{
		private var dataProxy:DataProxy;
		
		public function PetAtt(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
		}
		
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			var obj:Object 			 = new Object();
			obj.idEudemon     		 = bytes.readUnsignedInt(); 	//宠物ID
			var dwAttributeNum:uint  = bytes.readUnsignedInt(); 	//
			var ucAttributeType:uint = 0; 							//数据类型
			var dwAttributeData:uint = 0;							//变更的数据
			
			var typeArr:Array = [];
			var dataArr:Array = [];
			
			
			if(!GameCommonData.Player) return;	//忽略服务器缓冲区数据
			if(GameCommonData.Player.Role.PetSnapList[obj.idEudemon]) {			//自己的宠物
				var potentialUpdate:Boolean = false;
				for(var i:int=0 ; i < dwAttributeNum ; i ++)
				{ 
					ucAttributeType = bytes.readUnsignedInt();
					dwAttributeData = bytes.readUnsignedInt();
					typeArr.push(ucAttributeType);
					dataArr.push(dwAttributeData);
					
					switch(ucAttributeType)
					{
						case 1://life  生命(血)
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].HpNow = dwAttributeData;
								if(dwAttributeData == 0) sendNotification(AutoPlayPetCommand.NAME, {id:obj.idEudemon});
							}
							break;
						case 2://happy 快乐
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].HappyNow = dwAttributeData;
								sendNotification(AutoEatDragCommand.NAME, {happy:dwAttributeData, type:4}); 
							}
							break;
						case 4://lev 等级
							{
								if(GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Level != dwAttributeData)
								{
									RoleLevUp.PlayLevUp(obj.idEudemon , false);						//播放升级特效
									
									if(GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Level == 1) {
										if(GameCommonData.Player.Role.Level <= 10 && NewerHelpData.newerHelpIsOpen) {		//人物等级低于10级，宠物第一次升级
											sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 6);
										}
									}
									GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Level = dwAttributeData;
									RoleLevUpMessage.sendUpMessage(GameCommonData.Player.Role.PetSnapList[obj.idEudemon]);
								}
							}
							break;
						case 3://exp 经验
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].ExpNow = dwAttributeData;
							}
							break;
						case 5://addpoint 潜力点
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Potential = dwAttributeData;
								potentialUpdate = true;
							}
							break;
						case 6://str 力量
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Force = dwAttributeData;
								potentialUpdate = true;
							}
							break;
						case 7://int  灵力
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].SpiritPower = dwAttributeData;
								potentialUpdate = true;
							}
							break;
						case 8://sta 体力
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Physical = dwAttributeData;
								potentialUpdate = true;
							}
							break;
						case 9://spi 定力
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Constant = dwAttributeData;
								potentialUpdate = true;
							}
							break;
						case 10://agi 身法
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Magic = dwAttributeData;
								potentialUpdate = true;
							}
							break;
						case 11://maxlife  最大生命
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].HpMax = dwAttributeData;
							}
							break;
						case 12://phyatt  外功
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].PhyAttack = dwAttributeData;
							}
							break;
						case 13://magatt  内攻
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].MagicAttack = dwAttributeData;
							}
							break;
						case 14://phydef  外防
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].PhyDef = dwAttributeData;
							}
							break;
						case 15://magdef  内防
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].MagicDef = dwAttributeData;
							}
							break;
						case 16://hit  命中
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Hit = dwAttributeData;
							}
							break;
						case 17://doge  躲闪
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Hide = dwAttributeData;
							}
							break;
						case 18://crit  暴击
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Crit = dwAttributeData;
							}
							break;
						case 19://tough  坚韧
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].Toughness = dwAttributeData;
							}
							break;
						case 20://lifespan 当前寿命
							{
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].LifeNow = dwAttributeData;
							}
							break;
						case 21://state 状态
							{
								if(GameCommonData.Player.Role.PetSnapList[obj.idEudemon]) {
									GameCommonData.Player.Role.PetSnapList[obj.idEudemon].State = dwAttributeData;
								}
//								if(GameCommonData.Player.Role.PetList[obj.idEudemon]) {
//									GameCommonData.Player.Role.PetList[obj.idEudemon].State = dwAttributeData;
//								}
								if(dwAttributeData == 1) {	//出战了 
									if(!GameCommonData.Player.Role.UsingPet) {
										PetPropConstData.needRequestPetInfo.needToReq = true;
										PetPropConstData.needRequestPetInfo.petId = obj.idEudemon;
//										sendNotification(PetEvent.LOOKPETINFO_BYID, {petId:obj.idEudemon, ownerId:GameCommonData.Player.Role.Id});
									} else {
										sendNotification(PetEvent.PET_TO_FIGHT_AFTER_GETINFO);
									}
								} 
								else if(dwAttributeData == 0)//宠物休息了
								{					
									if(GameCommonData.Player.Role.UsingPet && GameCommonData.Player.Role.UsingPet.Id == obj.idEudemon) {
										GameCommonData.Player.Role.UsingPet = null;
										sendNotification(EventList.PET_RESTORDEAD_MSG);
									}
								}
								else if(dwAttributeData == 4)	//附体
								{
									(GameCommonData.Player.Role.PetSnapList[obj.idEudemon] as GamePetRole).State == 4;
//									sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:obj.idEudemon, ownerId:GameCommonData.Player.Role.Id});
									sendNotification(PetEvent.RETURN_TO_SHOW_PET_INFO,GameCommonData.Player.Role.PetSnapList[obj.idEudemon]);
								}
								else if(dwAttributeData == 5)	//宠物分离
								{
									(GameCommonData.Player.Role.PetSnapList[obj.idEudemon] as GamePetRole).State == 0;
//									sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:obj.idEudemon,ownerId:GameCommonData.Player.Role.Id}); 
									sendNotification(PetEvent.RETURN_TO_SHOW_PET_INFO,GameCommonData.Player.Role.PetSnapList[obj.idEudemon]);
								}
									
							}
							break;
							case 22:	//幻化（变性）
								var petId:int = obj.idEudemon;
								sendNotification(PetEvent.LOOKPETINFO_BYID,{petId:petId, ownerId:GameCommonData.Player.Role.Id});
								if(PetFantasyMediator.isFantasySend)
								{
									sendNotification(PetEvent.PET_FANTASY_FEEDBACK);
								}
								else if(PetChangeSexMediator.isChangeSexSend)
								{
									sendNotification(PetEvent.PET_CHANGE_SEX_FEEDBACK);
								}
							break;
							case 23:	//繁殖次数
								GameCommonData.Player.Role.PetSnapList[obj.idEudemon].BreedNow = dwAttributeData;
							break;
					}
				}
				sendNotification(PetEvent.PET_ATT_UPDATE_UI, {id:obj.idEudemon, potUpdate:potentialUpdate});	//sendNotification(PetEvent.PET_ATT_UPDATE_UI, {id:obj.idEudemon, type:ucAttributeType, att:dwAttributeData});
//				sendNotification(PetEvent.PET_UPDATE_SHOW_INFO);	//更新UI界面
				//更新指定Att属性 8.27
			}
			
			if(!GameCommonData.SameSecnePlayerList)return;
			var player:GameElementAnimal=GameCommonData.SameSecnePlayerList[obj.idEudemon] as GameElementAnimal;
			var useingPlayer:GameElementAnimal=GameCommonData.Player.Role.UsingPetAnimal;
			var len:uint=typeArr.length;
			for(var j:uint=0;j<len;j++){
				var type:uint=typeArr[j]
				switch (type){
					case 1:
						if(useingPlayer!=null){
							useingPlayer.Role.HP=dataArr[j];
							if(GameCommonData.Player.IsAutomatism){
								sendNotification(AutoPlayEventList.ATT_CHANGE_EVENT,3);
							}
						}
						if(player!=null){
							player.Role.HP=dataArr[j];
						}
						break;
					case 3:             //经验
						if(useingPlayer!=null){
							useingPlayer.Role.Exp=dataArr[j];
						}
						if(player!=null){
							player.Role.Exp=dataArr[j];
						}
						break;	
					case 4:             //宠物等级
						if(useingPlayer!=null){
							useingPlayer.Role.Level=dataArr[j];
						}
						
						if(player!=null){
							player.Role.Level=dataArr[j];
						}
						break;
					case 11:           //最大血量
						if(useingPlayer!=null){
							useingPlayer.Role.MaxHp=dataArr[j];
						}
						if(player!=null){
							player.Role.MaxHp=dataArr[j];
						}
						break;	
				}
			}
			
			if(GameCommonData.Player.Role.UsingPet && obj.idEudemon==GameCommonData.Player.Role.UsingPet.Id){
				sendNotification(EventList.ALLROLEINFO_UPDATE,{id:obj.idEudemon,type:1001});
			}
			
		}
		
		
	}
}