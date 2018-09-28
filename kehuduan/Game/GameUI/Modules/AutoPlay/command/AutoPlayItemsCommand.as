package GameUI.Modules.AutoPlay.command
{
	import GameUI.Modules.AutoPlay.Data.AutoPlayData;
	import GameUI.Modules.AutoPlay.mediator.AutoPlayMediator;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Relive.Data.ReliveEvent;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.Zippo;
	
	import flash.utils.clearInterval;
	import flash.utils.clearTimeout;
	import flash.utils.setInterval;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;
	
	//挂机物品设置
	public class AutoPlayItemsCommand extends SimpleCommand
	{
		public static const NAME:String = "AutoPlayItemsCommand";
		private var autoPlayMediator:AutoPlayMediator;
		private static var eatChunId:int;
		public static var doneId:int;
		private static var fishId:int;
		public static var count:uint = 60;
		
		public function AutoPlayItemsCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			autoPlayMediator = facade.retrieveMediator( AutoPlayMediator.NAME ) as AutoPlayMediator;
			count = 60;
			clearInterval( doneId );
//			clearTimeout( eatChunId );

			if ( !notification.getBody() )
			{
				return;
			}
			else if ( notification.getBody() == "sky" )
			{
				if ( GameCommonData.Player.IsAutomatism ) 	eatTianLing();
			}
			else if ( notification.getBody() == "earth" )
			{
				if ( GameCommonData.Player.IsAutomatism ) 	eatDiLing();
			}
			else if ( notification.getBody() == "dead" )
			{
				trace ( "收到人物死亡消息" );
//				eatChunGe();
//				deadGo();
				deadHandler();
			}
		}
		
		//处理人物死亡
		private function deadHandler():void
		{
			if ( GameCommonData.Player.IsAutomatism )
			{
				if ( GameCommonData.Player.Role.Level <= 20 )
				{
					showHint( GameCommonData.wordDic[ "mod_autoPl_com_autoPlayI_deadHandler_1" ] );//"5秒后自动复活"
					clearTimeout( fishId );
					fishId = setTimeout( fishRelive,5000 );
					return;
				}
				var limit:uint = AutoPlayData.aSaveNum[10];
				var tick:uint = AutoPlayData.aSaveTick[14];
				if ( tick == 1 )
				{
					if ( limit>0 )
					{
						clearTimeout( eatChunId );
						eatChunId = setTimeout( eatChunGe,5000 );
						if ( BagData.isHasItem( 630000 ) )
						{
						  showHint( GameCommonData.wordDic[ "mod_autoPl_com_autoPlayI_deadHandler_2" ] );//"5秒后自动使用春鸽复活"	
						}
					}
					else
					{
						showDead();
					}
				}
				else
				{
					showDead();
				}
				
//				deadGo();								//没有春鸽死亡掉线
			}
			else
			{
				UIFacade.UIFacadeInstance.showRelive();
			}
		}
		
		private function showDead():void
		{
//			PlayerController.EndAutomatism();
			if ( AutoPlayData.aSaveTick[15] == 1 )
			{
				if ( BagData.isHasItem( 630000 ) )
				{
					trace ( "死了包里还有春哥" );
					UIFacade.UIFacadeInstance.showRelive();	
				}
				else
				{
//					showHint( "1分钟后自动下线" );
//					trace ( "死了包里还有春哥" );
					sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_autoPl_com_autoPlayI_showDead" ], color:0xff0000});//"1分钟后自动下线"
					UIFacade.UIFacadeInstance.showRelive();	
//					clearTimeout( doneId );
//					doneId = setTimeout( done,60000 );
					clearInterval( doneId );
					doneId = setInterval( done,1000 );
				}
			}
			else
			{
				trace ( "没选中死亡后无春哥下线" );
				UIFacade.UIFacadeInstance.showRelive();			
			}
		}
		
		//1分钟后掉线
		private function done():void
		{
//			clearTimeout( doneId );
			if ( GameCommonData.Player.Role.HP>0 )
			{
				clearInterval( doneId );	
				return;
			} 
			count --;
			if ( count < 21 )
			{
				showHint( count.toString() );
			}
			if ( count == 0 )
			{
				clearInterval( doneId );	
				GameCommonData.GameNets.endGameNet();
				sendNotification( ReliveEvent.REMOVERELIVE );
				UIFacade.GetInstance(UIFacade.FACADEKEY).showBreak();
			}
			
		}
		
		//自动吃天灵丹 501000
		private function eatTianLing():void
		{
			var limit:uint = AutoPlayData.aSaveNum[8];
			var tick:uint = AutoPlayData.aSaveTick[12];
			if ( tick==1 )
			{
				var item:Object = BagData.getItemByType( 501000 ) as Object;
				if ( item )
				{
					if ( limit>0 )
					{
						NetAction.UseItem( OperateItem.USE,1,0,item.id );
						AutoPlayData.aSaveNum[8] --;
						autoPlayMediator.viewUI.txt_8.text = AutoPlayData.aSaveNum[8];
					}
				}
			}
		}
		
		//自动吃地灵丹 501003
		private function eatDiLing():void
		{
			var limit:uint = AutoPlayData.aSaveNum[9];
			var tick:uint = AutoPlayData.aSaveTick[13];
			if ( tick==1 )
			{
				var item:Object = BagData.getItemByType( 501003 ) as Object;
				if ( item )
				{
					if ( limit>0 )
					{
						NetAction.UseItem( OperateItem.USE,1,0,item.id );
						AutoPlayData.aSaveNum[9] --;
						autoPlayMediator.viewUI.txt_9.text = AutoPlayData.aSaveNum[9];
					}
				}
			}
		}
		
		//挂机死了自动吃春鸽  630000
		private function eatChunGe():void
		{
			clearTimeout( eatChunId );
			trace("到了吃春鸽的时候");
			if ( GameCommonData.Player.Role.HP == 0)
			{
				trace("判断是否有春哥");
				if ( BagData.isHasItem( 630000 ) )
				{
					trace("吃了1个春鸽");
					Zippo.PlayerRelive( 1 );
					AutoPlayData.aSaveNum[10] --;
					autoPlayMediator.viewUI.txt_10.text = AutoPlayData.aSaveNum[10];
				}
				else
				{
					trace("显示死亡面板或1分钟后下线");
					showDead();
				}
			}
			else
			{
				trace ( "春鸽发生了意外" );
			}
		}
		
		//没有春鸽死亡掉线
		private function deadGo():void
		{
			var tick:uint = AutoPlayData.aSaveTick[15];
			if ( tick==1 )
			{
				if ( !BagData.isHasItem( 630000 ) )
				{
					GameCommonData.GameNets.endGameNet();
				}
			}
		}
		
		private function showHint(info:String):void
		{
			sendNotification(HintEvents.RECEIVEINFO, {info:info, color:0xffff00});
		}
		
		//新手复活
		private function fishRelive():void
		{
			clearTimeout( fishId );
			Zippo.PlayerRelive( 1 );
		}
		
		//自动捡包已设置
	}
}