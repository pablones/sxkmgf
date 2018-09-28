package Net.ActionProcessor
{
	import GameUI.Modules.DragonEgg.Data.DragonEggData;
	import GameUI.Modules.DragonEgg.Data.DragonEggVo;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	
	public class PetEgg extends GameAction
	{
		public function PetEgg(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			var obj:Object = new Object();
			obj.aEggs = new Array();
			
			obj.idUser  = bytes.readUnsignedInt();				//用户ID
			obj.eggId  = bytes.readUnsignedInt();				//宠物蛋ID
			obj.nextTime = bytes.readUnsignedInt();				//距离下次刷新需等待的秒数
			obj.nPetType = bytes.readUnsignedShort();			//宠物类型
			obj.nAction  = bytes.readUnsignedByte();			//action
			obj.nAmount = bytes.readUnsignedByte();				//数量
			
			var vo:DragonEggVo;
//			var aEggs:Array = [];
//			var eggId:int = obj.idItem;
//			var nextTime:int = obj.nNeedSec;
			obj.name = bytes.readMultiByte( 16,GameCommonData.CODE );
			for(var i:uint = 0 ; i < obj.nAmount ; i ++)
			{
				vo = new DragonEggVo();
				vo.id = bytes.readUnsignedInt();			//宠物ID
				vo.level  = bytes.readUnsignedByte();			//类型，档次
				vo.lookface  = bytes.readUnsignedByte();			//lookface
				vo.grow  = bytes.readUnsignedShort();			//成长
				vo.streng = bytes.readUnsignedShort(); 			//力量资质
				vo.force = bytes.readUnsignedShort(); 			//灵力资质
				vo.physical = bytes.readUnsignedShort(); 			//体力资质
				vo.concentration = bytes.readUnsignedShort(); 			//定力资质
				vo.waza = bytes.readUnsignedShort(); 			//身法资质
				
				var nMaxStr:uint = bytes.readUnsignedShort(); 			//力量资质
				var nMaxInt:uint = bytes.readUnsignedShort(); 			//灵力资质
				var nMaxSta:uint = bytes.readUnsignedShort(); 			//体力资质
				var nMaxSpi:uint = bytes.readUnsignedShort(); 			//定力资质
				var nMaxAgi:uint = bytes.readUnsignedShort(); 			//身法资质
				
				var addPow:Number = Math.pow(1.06, vo.level ); 
				vo.maxStreng = nMaxStr * addPow;
				vo.maxForce = nMaxInt * addPow;
				vo.maxPhysical = nMaxSta * addPow;
				vo.maxConcentration = nMaxSpi * addPow;
				vo.maxWaza = nMaxAgi * addPow;
				vo.type = obj.nPetType;
				
				vo.streng *= addPow;			//力量资质
				vo.force *= addPow;				//灵力资质
				vo.physical *= addPow;			//体力资质
				vo.concentration *= addPow;	 			//定力资质
				vo.waza *= addPow;				//身法资质
				
				obj.aEggs.push( vo );
				
			}
			
			switch ( obj.nAction )
			{
				case 1://查询宠物蛋里宠物数据  打开界面  2刷新  3 领养  4时间到了刷新
					sendNotification( DragonEggData.SHOW_DRAGONEGG_PANEL,obj );
				break;
				case 2:			//刷新
				case 4:
					sendNotification( DragonEggData.UPDATA_DRAGONGEE_DATA,obj );
				break;
				default:
				break;
			}
		}
	}
}