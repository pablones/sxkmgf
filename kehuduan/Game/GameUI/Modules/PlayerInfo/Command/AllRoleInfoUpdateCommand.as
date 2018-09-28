package GameUI.Modules.PlayerInfo.Command
{
	import GameUI.Modules.PlayerInfo.Mediator.CounterWorkerInfoMediator;
	import GameUI.Modules.PlayerInfo.Mediator.PetInfoMediator;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class AllRoleInfoUpdateCommand extends SimpleCommand
	{
		public function AllRoleInfoUpdateCommand()
		{
			super();
		}
		
		/**
		 * type:
		 * 	1001 :  场景中有玩家信息更新	 
		 * @param notification
		 * 
		 */		
		override public function execute(notification:INotification):void{
			var id:uint=notification.getBody()["id"];
			var type:uint=notification.getBody()["type"];
			var counterMediator:CounterWorkerInfoMediator=facade.retrieveMediator(CounterWorkerInfoMediator.NAME) as CounterWorkerInfoMediator;
			var petMediator:PetInfoMediator=facade.retrieveMediator(PetInfoMediator.NAME) as PetInfoMediator;
			
			switch (type){
				//玩家进入场景（如果是队友，信息要立刻更新）
				case 0:
					if(GameCommonData.TeamPlayerList!=null && GameCommonData.TeamPlayerList[id]!=null){
						this.sendNotification(PlayerInfoComList.UPDATE_TEAM_UI,{id:id});
					}
					break;
				case 1:
					this.sendNotification(PlayerInfoComList.UPDATE_TEAM);
					break;
				case 6:
					this.sendNotification(PlayerInfoComList.UPDATE_ATTACK);
					break;
				case 31:
					this.sendNotification(PlayerInfoComList.HIDE_COUNTERWORKER_UI);
					break;
				case 1001:
					//更新的ID号
					if(counterMediator.role!=null && counterMediator.role.Id==id){
						if(counterMediator.role.Id==GameCommonData.Player.Role.Id){
							sendNotification(PlayerInfoComList.UPDATE_COUNTER_INFO,GameCommonData.Player.Role);
						}else if(GameCommonData.SameSecnePlayerList[id]!=null ){
							sendNotification(PlayerInfoComList.UPDATE_COUNTER_INFO,GameCommonData.SameSecnePlayerList[id].Role);
						}
					}
					//自己
					if(id==GameCommonData.Player.Role.Id){
					
						sendNotification(PlayerInfoComList.UPDATE_SELF_INFO,GameCommonData.Player.Role);
					}
					//队友
					if(GameCommonData.TeamPlayerList!=null && GameCommonData.TeamPlayerList[id]!=null){
						this.sendNotification(PlayerInfoComList.UPDATE_TEAM_UI,{id:id});
						
					}
					//宠物
					if(petMediator.role){
						if(petMediator.role.Id==id){
							//todo更新宠物信息
							sendNotification(PlayerInfoComList.UPDATE_PET_UI);
						}
					}
					
					break;	
					//清除宠物	
				case 41:
					if(petMediator.role) {
						if(petMediator.role.Id==id){
							sendNotification(PlayerInfoComList.REMOVE_PET_UI);
						}
					}
					if(counterMediator.role) {
						if(counterMediator.role.Id==id){
							this.sendNotification(PlayerInfoComList.HIDE_COUNTERWORKER_UI);
						}	
					}
					break;
				default:
					break;
			}				
		
		}
		
	}
}