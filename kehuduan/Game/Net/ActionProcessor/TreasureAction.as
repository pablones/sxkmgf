package Net.ActionProcessor
{
	import GameUI.Modules.IdentifyTreasure.Data.TreasureData;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	
	public class TreasureAction extends GameAction
	{
		public function TreasureAction( isUsePureMVC:Boolean=true )
		{
			super( isUsePureMVC );
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			var idUser:uint = bytes.readUnsignedInt();		// 用户ID
			var uAction:uint = bytes.readUnsignedInt();		// Action   1增加   2删除   3同步数量   4奖品展示
			var uAmount:uint = bytes.readUnsignedInt();		// 称号数量
			
			var tObj:Object;
			var awardArr:Array = [];
			
			for (var k:int=0; k<uAmount; k++)
			{
				tObj = new Object();
				tObj.id = bytes.readUnsignedInt();//奖品ID
				tObj.type = bytes.readUnsignedInt();  //物品类型ID
				tObj.num = bytes.readUnsignedShort();  //物品数量
				tObj.divide = bytes.readUnsignedShort();  //物品类别
				//奖章特殊处理
				if ( tObj.type>=610049 && tObj.type<=610051 )
				{
					tObj.divide = 5;
				}
				switch ( uAction )
				{
					case 1:
						TreasureData.packageDateArr.push( tObj );
					break;
					case 2:
						var index:int;
						for ( var i:uint=TreasureData.packageDateArr.length-1; i>0; i-- )
						{
							if ( TreasureData.packageDateArr[i].id == tObj.id )
							{
								index = i;
							}
						}
						TreasureData.packageDateArr.splice( index,1 );
				break;
				case 3:
					for ( var j:uint=0; j<TreasureData.packageDateArr.length; j++ )
					{
						if ( TreasureData.packageDateArr[j].id == tObj.id )
						{
							TreasureData.packageDateArr[j] = tObj;
						}
					}
				break;
				case 4:																					//用于奖品展示
					awardArr.push( tObj );
				break;
				}
			}
			if ( TreasureData.TreaResourceLoaded )
			{
				sendNotification( TreasureData.CHANGE_TREA_PACK_DATA );
				sendNotification( TreasureData.UPDATE_TREA_PACKAGE_SPACE );			
				if ( awardArr.length>0 )
				{
					sendNotification( TreasureData.RECEIVE_AWARD_TREA,{ aAward:awardArr } );
				}
			}

		}
	}
}