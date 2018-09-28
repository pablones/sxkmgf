package Net.ActionProcessor
{
	import GameUI.Mediator.UiNetAction;
	import GameUI.Modules.Artifice.data.ArtificeConst;
	import GameUI.Modules.CastSpirit.Data.CastSpiritData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.StrengthenTransfer.data.StrengthenTransferConst;
	
	import Net.GameAction;
	
	import flash.geom.Point;
	import flash.utils.ByteArray;

	public class EquipAction extends GameAction
	{
		public static const MSGCOMPOUND_ARTIFIC:int					= 88;//装备炼化
		public static const MSGCOMPOUND_ARTIFIC_SUC:int				= 89;
		public static const MSGCOMPOUND_ARTIFIC_FAIL:int			= 90;
	
		public static const MSGCOMPOUND_EQUIP_MOVE:int				= 91;//强化转移
		public static const MSGCOMPOUND_EQUIP_MOVE_SUC:int			= 92;
		public static const MSGCOMPOUND_EQUIP_MOVE_FAIL:int			= 93;
		
		public function EquipAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void{
			bytes.position = 4;
			var action:int = bytes.readUnsignedInt();//action
			var idItem:uint = bytes.readUnsignedInt();//装备ID
			var itemLev:uint = bytes.readUnsignedInt();//最后一位为强化或升星的标志

			switch(action)
			{
				case 71:
					sendNotification(EquipCommandList.RECALL_EQUIPSTILETTO,{type:71,eId:idItem});
					break;
				case 72:
					sendNotification(EquipCommandList.RECALL_EQUIPSTILETTO,{type:72,eId:idItem});
					break;	
				case 9:
					sendNotification(EquipCommandList.RECALL_EQUIPSTILETTO,{type:9,eId:idItem});
					break;
				case 10:
					sendNotification(EquipCommandList.RECALL_EQUIPSTILETTO,{type:10,eId:idItem});
					break;		
		
				case 24:
					sendNotification(EquipCommandList.RECALL_EQUIPSTILETTO,{type:24,eId:idItem});
					break;
				case 25:
					sendNotification(EquipCommandList.RECALL_EQUIPSTILETTO,{type:25,eID:idItem});
					break;
				case 62:
					sendNotification(EquipCommandList.SERVER_STRENGTH_UI);
					break;	
				case 63:  //装备强化成功
					sendNotification(EquipCommandList.RECALL_EQUIPSTRENGEN,{type:1,level:itemLev});
					break; 
				case 64: //装备强化失败
					sendNotification(EquipCommandList.RECALL_EQUIPSTRENGEN,{type:2,level:itemLev});
					break;
				case 65: //装备升星服务端返回消息
					sendNotification(EquipCommandList.SERVER_ADDSTAR_UI);
					break;	
				case 66: //升星成功
					sendNotification(EquipCommandList.RECALL_EQUIPSTRENGEN,{type:3,level:itemLev});
					break;
				case 67: //升星失败
					sendNotification(EquipCommandList.RECALL_EQUIPSTRENGEN,{type:4,level:itemLev});
					break;
					//强化加成率
				case 69:
					sendNotification(EquipCommandList.UPDATE_FAIL_SCALE,itemLev); 
					break;
				case 74: //宝石合成成功
					sendNotification(EquipCommandList.ADD_SPECIAL_SHOW,new Point(164,118));
					break;
				//魂印成功	
				case 77:	
					sendNotification(EquipCommandList.RECALL_HUNYUN_EQUIP,{type:1,eId:idItem});
					break;
				//魂印失败
				case 78:
				 	sendNotification(EquipCommandList.STONE_DECORATE_SEND,{type:1,eId:idItem});
					break;
				//宝石雕琢成功	
				case 80:
				 	sendNotification(EquipCommandList.STONE_DECORATE_SEND,{type:1,eId:idItem});
					break;	
				//宝石雕琢失败
				case 81:
				 	sendNotification(EquipCommandList.RECALL_HUNYUN_EQUIP,{type:2,eId:idItem});
					break;	
				case 83:
					sendNotification(CastSpiritData.SUCCESS_CASTSPIRIT, {amount:idItem, level:itemLev});
					break;
				case 84:
					sendNotification(CastSpiritData.FAILD_CASTSPIRIT);
					break;
				case 86:
					sendNotification(CastSpiritData.SUCCESS_CASTSPIRIT_TRANSFER);
					break;
				case 87:
					sendNotification(CastSpiritData.FAILD_CASTSPIRIT_TRANSFER);
					break;
				//装备打造
				case 101://配方列表
					for ( var i:int=0; i<itemLev; i++ )
					{
						var idPeifang1:uint = bytes.readUnsignedInt();//配方ID
						bytes.readUnsignedInt();
						ManufactoryData.scenographyList.push( idPeifang1 );				
					}
					break;
				case 102://新加配方
					for ( var j:uint=0; j<itemLev; j++ )
					{
						var idPeifang2:uint = bytes.readUnsignedInt();//配方ID
						bytes.readUnsignedInt();	
						ManufactoryData.scenographyList.push( idPeifang2 );			
					}
					break;
				case 103://删除配方
					for ( var k:uint=0; k< itemLev; k++ )
					{
						var idPeifang3:uint = bytes.readUnsignedInt();//配方ID
						bytes.readUnsignedInt();
						var index:int = 	ManufactoryData.scenographyList.indexOf( idPeifang3 );
						if ( index>-1 )
						{
							ManufactoryData.scenographyList.splice( index,1 );
						}			
					}
					break;
				case 14:	//打造装备成功
					sendNotification( ManufactoryData.MANUFACTORY_SUCEED );
					break;
				
				case MSGCOMPOUND_ARTIFIC_SUC:
					sendNotification(ArtificeConst.ARTIFIC_SUC);
					trace("MSGCOMPOUND_ARTIFIC_SUC");
					break;
				case MSGCOMPOUND_ARTIFIC_FAIL:
					trace("MSGCOMPOUND_ARTIFIC_FAIL");
					break;
				case MSGCOMPOUND_EQUIP_MOVE_SUC:
					UiNetAction.GetItemInfo(idItem,GameCommonData.Player.Role.Id,GameCommonData.Player.Role.Name);
					sendNotification(StrengthenTransferConst.STRENGTHENTRANSFER_SUC);
					break;
				case MSGCOMPOUND_EQUIP_MOVE_FAIL:
					trace("MSGCOMPOUND_EQUIP_MOVE_FAIL");
					break;
					
				default:
					trace("undeclare msgcompound:"+action);
					break;
			}
		}
	}
}