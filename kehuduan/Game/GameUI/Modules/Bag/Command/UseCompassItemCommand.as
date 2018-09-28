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
	public class UseCompassItemCommand extends SimpleCommand
	{
		public static const NAME:String = "UseCompassItemCommand";
		
		public function UseCompassItemCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var compassData:Object = notification.getBody();
			var targetMap:int = compassData.addAtt[0];
			var targetX:int = compassData.addAtt[1];
			var targetY:int = compassData.addAtt[2];
		
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
			
			var title:String = GameCommonData.wordDic[ "mod_bag_com_useCom_exe_1" ];   //周天罗盘
			var info:String = "<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_bag_com_useCom_exe_2" ]/* 逆师剑妖正在 */ + mapName + GameCommonData.wordDic[ "mod_bag_com_useCom_exe_3" ]/*的*/+"（" + targetX +"，"+targetY+"），"+GameCommonData.wordDic[ "mod_bag_com_useCom_exe_4" ];//请和徒弟一起去消灭他。</font>\n<font color='#00ff00'>小提示：到达指定坐标，再次使用周天罗盘找出隐藏的剑妖。然后使用传功宝珠，会给队伍内的徒弟添加特殊状态，拥有该状态，才能获得杀死剑妖的能力。</font>
			
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