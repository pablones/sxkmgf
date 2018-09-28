package GameUI.Modules.MainSence.Command
{
	import Controller.CooldownController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.mediator.AutoPlayMediator;
	import GameUI.Modules.Bag.Command.SetCdData;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.items.UseItem;
	
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class ItemCdCommand extends SimpleCommand
	{
		public function ItemCdCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var obj:Object=notification.getBody();
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var autoPlayMediator:AutoPlayMediator=facade.retrieveMediator(AutoPlayMediator.NAME) as AutoPlayMediator;
			
			if(obj.magicid<1000){
				var quickItems:Array=QuickBarData.getInstance().getUseItemByType(obj.magicid);
				for each(var u:UseItem in quickItems){
					this.startUseItemCd(u.Type,obj.privateCdTime);
				}
				var bagDataItem:Array=QuickBarData.getInstance().getItemsFromBag(obj.magicid);
				for each(var t:* in bagDataItem){
					SetCdData.setData(t.position-47,t.index,obj.privateCdTime,t.type,-120);
					this.startUseItemCd(t.type, obj.privateCdTime);
				}
				
				var autoPlayItems:Array=this.getItemsFromAutoPlay(obj.magicid,autoPlayMediator.dataDic);
				for each(var a:* in autoPlayItems){
					this.startUseItemCd(a.Type,obj.privateCdTime);
				}
				if(bagDataItem.length>0 && dataProxy.BagIsOpen){
					sendNotification(EventList.UPDATEBAG);
				}
			}else{
//				var uItem:UseItem=QuickBarData.getInstance().getSkillItemByType(obj.magicid);
//				var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
//				if(dicF[7]!=null && dicF[7].Type==obj.magicid){
//					this.startUseItemCd(dicF[7].Type,obj.privateCdTime);
//				}
//				if(uItem!=null){ 
//					QuickBarData.getInstance().skillsInQuickAddCd(uItem.Type,obj.privateCdTime);

					if (GameCommonData.Player.Role.UsingPet)
					{
						for each (var skill:GameSkillLevel in GameCommonData.Player.Role.UsingPet.SkillLevel)
						{
							if (skill.gameSkill.SkillID == obj.magicid)
							{
								this.startUseItemCd(obj.magicid, obj.privateCdTime, skill.gameSkill.CoolTime * 1000);
								return;
							}
						}
					}

					if (GameCommonData.Player.Role.SkillList[obj.magicid])
					{
						this.startUseItemCd(obj.magicid, obj.privateCdTime, GameCommonData.Player.Role.SkillList[obj.magicid].GetCoolTime * 1000); 
					}
					else
					{
						this.startUseItemCd(obj.magicid,obj.privateCdTime);
					}
//				}
			}	
			
			
		}
		
		/**
		 * 根据cdType启动同种类型的CD（自动打怪中） 
		 * @param cdType
		 * @return 
		 * 
		 */		
		private function getItemsFromAutoPlay(cdType:uint,dic:Dictionary):Array{
			var arr:Array=[];
			if(dic==null)return arr;
			for each(var u:UseItem in dic){
				if(u==null)continue;
				if(QuickBarData.getInstance().getCdType(u.Type)==cdType){
					arr.push(u);
				}
			}
			return arr;
		}
		
		/**
		 * 让指定物品或技能运行CD 
		 * @param useItem
		 * @param time
		 * @param count
		 * 
		 */		
		private function startUseItemCd(type:int, time:uint, totalTime:int=-1):void{
			
//			useItem.startCd(time,-120);
			if (totalTime != -1)
			{
				CooldownController.getInstance().triggerCD(type, time, totalTime);
			}
			else
			{
				CooldownController.getInstance().triggerCD(type, time);
			}
		}
		
	}
}