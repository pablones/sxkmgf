package GameUI.Modules.Buff.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Buff.Data.BuffEvent;
	import GameUI.Modules.Buff.UI.BuffController;
	import GameUI.Modules.Buff.UI.DeBuffController;
	
	import OopsEngine.Skill.GameSkillBuff;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class BuffMediator extends Mediator implements IUpdateable
	{
		public static const NAME:String = "BuffMediator";
		
		private var buffController:BuffController;		/** Buff控制器 */
		private var deBuffController:DeBuffController;	/** DeBuff控制器 */
		private var interval:uint;						/** 间隔循环id*/
		private var isHave:Boolean = false;				/** 是否存在元素buff */
		private var timer:Timer = new Timer();			/** 定时器 */
		private var enabled:Boolean = true;
//		private var i:int;
		
		public function BuffMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.INITVIEW,
					EventList.SHOWBUFF,
					EventList.SHOWDEBUFF,
					BuffEvent.ADDBUFF,
					BuffEvent.DELETEBUFF,
					BuffEvent.ADDDEBUFF,
					BuffEvent.DELETEDEBUFF,
					BuffEvent.DELETEALL
					]
		}
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
		
		public function get Enabled():Boolean
		{
			return enabled;
		}
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))
			{
				buffController.BuffGo();
				deBuffController.BuffGo();
			}
		}
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					buffController   = new BuffController();
					deBuffController = new DeBuffController(); 
					buffController.name = "buffController";
					deBuffController.name = "deBuffController";
					buffController.timeUpdateControll = timeUpdateControll;
					deBuffController.timeUpdateControll = timeUpdateControll;
					GameCommonData.GameInstance.GameUI.addChild(buffController);
					GameCommonData.GameInstance.GameUI.addChild(deBuffController);
					buffController.x = 580;
					buffController.y = 23;
					deBuffController.x = 580;
					deBuffController.y = (31 + 30) * 2;
				break;
				
				case EventList.SHOWBUFF:			//更新
					var updateBuff:GameSkillBuff = notification.getBody() as GameSkillBuff;
					facade.sendNotification(BuffEvent.DELETEBUFF , updateBuff);
					facade.sendNotification(BuffEvent.ADDBUFF ,updateBuff);
				break;
				
				
				case BuffEvent.ADDBUFF:
					var addbuff:GameSkillBuff = notification.getBody() as GameSkillBuff;
					buffController.addBuff(addbuff);
				break;
				
				
				case BuffEvent.DELETEBUFF:
					var deletebuff:GameSkillBuff = notification.getBody() as GameSkillBuff;
					buffController.deleteBuff(deletebuff);
				break;
				
				/////////////////////////DeBuff
				case EventList.SHOWDEBUFF:
					var updateDeBuff:GameSkillBuff = notification.getBody() as GameSkillBuff;
					facade.sendNotification(BuffEvent.DELETEDEBUFF , updateDeBuff);
					facade.sendNotification(BuffEvent.ADDDEBUFF ,updateDeBuff);
				break;
				
				case BuffEvent.ADDDEBUFF:
					var addDeBuff:GameSkillBuff = notification.getBody() as GameSkillBuff;
					deBuffController.addBuff(addDeBuff);
//					timeUpdateControll("add");			//添加心跳
				break;
				
				case BuffEvent.DELETEDEBUFF:
					var deleteDebuff:GameSkillBuff = notification.getBody() as GameSkillBuff;
					deBuffController.deleteBuff(deleteDebuff);
					timeUpdateControll("delete");			//删除心跳
				break;
				
				case BuffEvent.DELETEALL:					//删除全部BUFF
					deleteAllBuff();				
				break;
			}
		}
		/** 控制删除 添加 心跳*/
		private function timeUpdateControll(state:String):void
		{
			switch(state)
			{
				case "add":
					if(buffController.numChildren + deBuffController.numChildren == 1)
					{
						GameCommonData.GameInstance.GameUI.Elements.Add(this);				//添加心跳
						timer.DistanceTime = 1000 * 1;		//循环时间为1秒钟
					}
				break;
				case "delete":
					if(buffController.numChildren + deBuffController.numChildren == 0)
					{
						GameCommonData.GameInstance.GameUI.Elements.Remove(this);			//删除心跳
					}
				break;
			}
		}
		/** 删除全部Buff 和 DeBuff */
		private function deleteAllBuff():void
		{
			while(buffController.numChildren > 0)
			{
				buffController.removeChildAt(buffController.numChildren - 1);
			}
			while(deBuffController.numChildren > 0)
			{
				deBuffController.removeChildAt(deBuffController.numChildren - 1);
			}
			timeUpdateControll("delete");			//全部删除后 删除心跳
		}
	}
}