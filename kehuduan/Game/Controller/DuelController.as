package Controller
{
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	
	public class DuelController
	{	
        //开启时间		
		public static var IsOpenTime:Boolean  = false;
        //时间		
		public static var TimeCount:int  = 0;
		
		public static var timer:Timer;
		
		
		//发起决斗
		public static function InitiateDuel(target:GameElementAnimal):void
		{
			//7格之内是可以决斗的
			if(DistanceController.PlayerTargetAnimalDistance(target,7))
			{	
				if(!GameCommonData.Player.Role.IsDuel)	
				{	
					//攻击状态不能切磋
					if(!GameCommonData.Player.Role.IsAttack)
					{
						//更新切磋时间
						GameCommonData.Player.Role.UpdateDuelTime();
				   		FriendSend.Duel(target.Role.Id,1);
				    }
				    else
				    {
				    	UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_duel_Initiate_1"],0xffff00);  // 暂不能切磋
				    }
			    }
			    else
			    {
			    	UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_duel_Initiate_2"],0xffff00); // 5秒后才能再次发起切磋
			    }
			}
			else
			{
				UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic["con_duel_Initiate_3"],0xffff00); // 距离过远不能切磋
			}
		}
		
		//开始决斗
		public static function BeginDuel(playerID:int):void
		{
			FriendSend.Duel(playerID,3);
		}
		
		//开始计时
		public static function OpenTime():void
		{
			timer = new Timer(950,9);
			timer.addEventListener(TimerEvent.TIMER,TimeSet);
			TimeCount = 10;
			timer.start();
		}
		
		public static function CloseTime():void
		{
			if(timer != null)
				timer.stop();
		}

        /** 时间提示 */
		private static function TimeSet(e:TimerEvent):void
		{
			TimeCount -= 1 ;
			var text:String = GameCommonData.wordDic["con_duel_timeSet_1"]+"，"+TimeCount+GameCommonData.wordDic["con_duel_timeSet_2"];  // 超出切磋范围  秒后结束切磋
			UIFacade.UIFacadeInstance.showPrompt(text,0xffff00);
		}

		
		
		//接受决斗
		public static function AcceptDuel(playerID:int):void
		{
			//控制是否打开对话框的阀值
			GameCommonData.IsDuel = false;
			FriendSend.Duel(playerID,2);
		}

		//取消决斗
		public static function CancelDuel(playerID:int):void
		{
			GameCommonData.IsDuel = false;
			//控制是否打开对话框的阀值
			FriendSend.Duel(playerID,4);
		}
		
		
		//改状态不能决斗
		public static function CannotDuel(playerID:int):void
		{
			  FriendSend.Duel(playerID,99); 
		}
	}
}