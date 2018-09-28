package GameUI.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Pet.Proxy.PetNetAction;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Data.PetBreedDoubleEvent;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Mediator.PetBreedDoubleMediator;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleEvent;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Mediator.PetBreedSingleMediator;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.Data.PetSavvyJoinEvent;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.Mediator.PetSavvyJoinMediator;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyEvent;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Mediator.PetSavvyUseMoneyMediator;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Data.PetSkillLearnEvent;
	import GameUI.Modules.PetPlayRule.PetSkillLearn.Mediator.PetSkillLearnMediator;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpEvent;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Mediator.PetSkillUpMediator;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyEvent;
	import GameUI.Modules.PetPlayRule.PetToBaby.Mediator.PetToBabyMediator;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionProcessor.PlayerAction;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ShowPetViewCommand extends SimpleCommand
	{
		private var dataProxy:DataProxy = null;
		
		public function ShowPetViewCommand()
		{
			super();
			this.initializeNotifier(UIFacade.FACADEKEY);
		}
		
		public override function execute(notification:INotification):void 
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var type:String = notification.getBody() as String;
			if(!judgeState()){
				closeDoubleBreed(type);
				return;
			}
			sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);			//先关闭所有打开的宠物系列面板
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
			dataProxy.PetCanOperate = false;
			if(type) {
				switch(type) {
					case UIConstData.PETBREEDSINGLE:						//单人繁殖
						facade.registerMediator(new PetBreedSingleMediator());
						sendNotification(PetBreedSingleEvent.SHOW_PETBREEDSINGLE_VIEW);
						break;
					case UIConstData.PETBREEDDOUBLE:						//双人繁殖
						facade.registerMediator(new PetBreedDoubleMediator());
						sendNotification(PetBreedDoubleEvent.SHOW_PETBREEDDOUBLE_VIEW);
						break;
					case UIConstData.PETTOBABY:								//还童
						facade.registerMediator(new PetToBabyMediator());
						sendNotification(PetToBabyEvent.SHOW_PETTOBABY_VIEW);
						break;
					case UIConstData.PETSKILLLEARN:							//技能学习
						facade.registerMediator(new PetSkillLearnMediator());
						sendNotification(PetSkillLearnEvent.SHOW_PETSKILLLEARN_VIEW);
						break;
					case UIConstData.PETSKILLUP:							//技能提升
						facade.registerMediator(new PetSkillUpMediator());
						sendNotification(PetSkillUpEvent.SHOW_PETSKILLLUP_VIEW);
						break;
					case UIConstData.PETSAVVYUSEMONEY:						//悟性提升
						facade.registerMediator(new PetSavvyUseMoneyMediator());
						sendNotification(PetSavvyUseMoneyEvent.SHOW_PETSAVVYUSEMONEY_VIEW);
						break;
					case UIConstData.PETSAVVYJOIN:							//合成
						facade.registerMediator(new PetSavvyJoinMediator());
						sendNotification(PetSavvyJoinEvent.SHOW_PETSAVVYJOIN_VIEW);
						break;
				}
			}
			dataProxy = null;
		}
		
		/** 判断是否可以进行宠物操作 */
		private function judgeState():Boolean
		{
			if(dataProxy.TradeIsOpen) {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["ui_com_showrule_judge_1"], color:0xffff00}); // "交易中无法进行宠物操作"
				return false;
			}
			if(StallConstData.stallSelfId != 0) {	//自己正在摆摊
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["ui_com_showrule_judge_2"], color:0xffff00}); // "摆摊中无法进行宠物操作"
				return false;
			}
			if(dataProxy.DepotIsOpen) {				//关闭仓库
				sendNotification(EventList.CLOSEDEPOTVIEW);
			}
			if(dataProxy.NPCShopIsOpen) {			//关闭商店
				sendNotification(EventList.CLOSENPCSHOPVIEW);
			}
			return true;
		}
		
		/** 关闭双繁 */
		private function closeDoubleBreed(type:String):void
		{
			if(type == UIConstData.PETBREEDDOUBLE) {
				PetNetAction.opPet(PlayerAction.PET_CANCEL_BREED);
			}
		}
		
	}
}