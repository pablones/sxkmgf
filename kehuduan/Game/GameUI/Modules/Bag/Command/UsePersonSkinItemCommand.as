package GameUI.Modules.Bag.Command
{
	import Controller.DistanceController;
	
	import GameUI.Modules.Alert.Data.AlertData;
	import GameUI.Modules.Alert.PanelAlertMediator;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	
	import Net.ActionProcessor.OperateItem;
	
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	//使用师徒道具罗盘
	public class UsePersonSkinItemCommand extends SimpleCommand
	{
		public static const NAME:String = "UsePersonSkinItemCommand";
		
		public function UsePersonSkinItemCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var skinData:Object = notification.getBody();
			var targetMap:int = skinData.addAtt[0];
			var targetX:int = skinData.addAtt[1];
			var targetY:int = skinData.addAtt[2];
		
			var mapId:int = int( GameCommonData.GameInstance.GameScene.GetGameScene.MapId );
			var roleX:int = GameCommonData.Player.Role.TileX;
			var roleY:int = GameCommonData.Player.Role.TileY;
			var mapName:String;
			var obj:Object = SmallConstData.getInstance().mapItemDic[ targetMap ];
			if ( obj )
			{
				mapName = obj.name;
			}
			else
			{
				mapName = "";
			}
			
			var title:String = GameCommonData.wordDic[ "mod_bag_UseP_execute_1" ];   //人皮面具
			var info:String = "<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_bag_UseP_execute_2" ]+mapName+GameCommonData.wordDic[ "mod_bag_com_useCom_exe_3" ]+"（" + targetX + "," + targetY + GameCommonData.wordDic[ "mod_bag_UseP_execute_3" ];
//			var info:String = "<font color='#ffffff'>司空摘星躲在"+mapName+"的（" + targetX + "," + targetY + "）处,赶紧去消灭他吧。\n<font color='#00ff00'>小提示：到达人皮面具上标记的坐标，点击使用人皮面具就能引出躲在隐秘处的司空摘星。消灭他你就完成了帮主交给你的任务。</font></font>";
			
			if ( targetMap != mapId ) 
			{
				registMed();
				sendNotification( AlertData.SHOW_PANEL_ALERT_VIEW,{ title:title,info:info } );
			}
			else
			{
				var point:Point = new Point( targetX,targetY );
				if ( DistanceController.PlayerDistance( point,4 ) )
				{
//					trace ( "刷怪" );
					NetAction.UseItem(OperateItem.USE, 1, BagData.SelectedItem.Index+1, BagData.SelectedItem.Item.Id);
				}
				else
				{
					registMed();
					sendNotification( AlertData.SHOW_PANEL_ALERT_VIEW,{ title:title,info:info } );
				}
			}
			
			facade.removeCommand( NAME );
		}
		
		private function registMed():void
		{
			if ( !facade.hasMediator( PanelAlertMediator.NAME ) )
			{
				facade.registerMediator( new PanelAlertMediator() );
			}
		}
		
	}
}