package GameUI.Command
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleEvent;
	import GameUI.Modules.PetPlayRule.PetRuleController.Mediator.PetRuleControlMediator;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Proxy.DataProxy;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ShowPetRuleBaseCommond extends SimpleCommand
	{
		private var dataProxy:DataProxy = null;
		
		public function ShowPetRuleBaseCommond()
		{
			super();
		}
		
		public override function execute(notification:INotification):void 
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var data:Object = notification.getBody();
			var type:String = data.type;
			var index:int   = data.index;
			if(!judgeState()){
				sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);			//关闭所有打开的宠物系列面板
				return;
			}
			sendNotification(EventList.CLOSE_PET_PLAYRULE_VIEW);			//先关闭所有打开的宠物系列面板
			if(dataProxy.PetIsOpen) {
				sendNotification(EventList.CLOSEPETVIEW);
			}
			dataProxy.PetCanOperate = false;
			if(type) {
				facade.registerMediator(new PetRuleControlMediator());
				sendNotification(PetRuleEvent.SHOW_PET_RULE_BASE, {type:type, index:index});
				
//				switch(type) {
//					case UIConstData.PET_RULE_BASE:		//宠物基本玩法
//						facade.registerMediator(new PetRuleControlMediator(), {type:type});
//						sendNotification(PetRuleEvent.SHOW_PET_RULE_BASE, {type:UIConstData.PET_RULE_BASE});
//						break;
//					case UIConstData.PET_DOUBLE_BREED:	//宠物双繁
//						facade.registerMediator(new PetBreedDoubleMediator());
//						sendNotification(PetRuleEvent.SHOW_PET_RULE_BASE);
//						sendNotification(PetBreedDoubleEvent.SHOW_PETBREEDDOUBLE_VIEW);
//						break;
//				}
			}
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
		
//		/** 关闭双繁 */
//		private function closeDoubleBreed(type:String):void
//		{
//			if(dataProxy.PetBreedDoubleIsOpen) {
//				PetNetAction.opPet(PlayerAction.PET_CANCEL_BREED);
//			}
//		}
	}
}