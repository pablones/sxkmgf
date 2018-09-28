package Net.ActionProcessor
{
	import GameUI.Modules.Map.SmallMap.SmallMapConst.SmallConstData;
	import GameUI.Modules.VipShow.Data.VipShowData;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	
	public class VipList extends GameAction
	{
		public function VipList(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}

		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			var obj:Object = new Object();
			var arr:Array = new Array();
			
			obj.idUser  = bytes.readUnsignedInt();				//用户ID
			obj.nAction	= bytes.readUnsignedShort();			//action
			obj.nPage	= bytes.readUnsignedByte();				//页数 页码
			obj.nAmount = bytes.readUnsignedByte();				//数量
			
			for(var i:uint = 0 ; i < obj.nAmount ; i ++)
			{
				var idUser:uint = bytes.readUnsignedInt();			//玩家ID
				var idMap:uint = bytes.readUnsignedInt();			//所在地图
				var nLev:uint = bytes.readUnsignedShort(); 			//等级
				var nline:int = bytes.readUnsignedByte();			//线路
				var nVipLev:int = bytes.readUnsignedByte();			//VIP等级
				var name:String = bytes.readMultiByte(16, "ANSI");	//名字
				var _obj:Object = new Object();
				_obj.name = name;
				_obj.level = nLev;
				_obj.address = SmallConstData.getInstance().mapItemDic[ idMap ].name;
				_obj.line = getline( nline );
				_obj.vip = nVipLev;
				arr.push( _obj );
			}
			obj.array = arr ;
			
			switch ( obj.nAction )
			{
				case 1://查询vip列表
					if( VipShowData.IsVipShowOpen )
					{
						facade.sendNotification( VipShowData.UPDATA_VIP_SHOW, obj );
					}else
					{
						facade.sendNotification( VipShowData.LOAD_VIPSHOW_VIEW, obj );
					}
				break;
			}
		}
		
		public function getline( num:int ):String
		{
			var str:String;
			switch( num )
			{
				case 1:
					str = "一线";
					break;
				case 2:
					str = "二线";
					break;
				case 3:
					str = "三线";
					break;
				case 4:
					str = "四线";
					break;
				case 5:
					str = "五线";
					break;
				case 6:
					str = "专线";
					break;
				case 7:
					str = "七线";
					break;
				default:
					str = "大于七线";
					break;
			}
			return str;
		}
	}
}