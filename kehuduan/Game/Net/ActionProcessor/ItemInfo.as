package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.Equipment.model.EquipDataConst;
	import GameUI.Modules.StoneMoney.Mediator.StoneMoneyMediator;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;
	import flash.utils.Dictionary;

	public class ItemInfo extends GameAction
	{
		public function ItemInfo(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public static var isPanel:Boolean = false;
		public static var isParallel:Boolean = false;
		public static var isLevelUp:Boolean = false;   //是数据更新，不用于显示
		
		public static var queryIdList:Dictionary = new Dictionary();		//查询的物品ID列表
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position  = 4;
			
//		OBJID		id;
//		DWORD		dwType;
//		DWORD		dwAmount;
//		DWORD 		dwAmountLimit;
//		USHORT		usLevel;
//		USHORT		usActiveType;
//		DWORD		dwLifeTime;
//		USHORT		usQuality;
//		USHORT		usStar;
//		DWORD		dwGemType[3];
//		DWORD		dwBasic[2];
//		DWORD		dwAddtion[8];
			var obj:Object  = new Object();
			obj.id = bytes.readUnsignedInt();				//物品id
			obj.type = bytes.readUnsignedInt();				//物品type
			obj.amount = bytes.readUnsignedInt();			//持久或物品数量 
			obj.maxAmount = bytes.readUnsignedInt();		//持久或物品数量上限
			obj.level = bytes.readUnsignedShort();			// 等级
			obj.isActive = bytes.readUnsignedShort();		//是否激活   0-不用检测剩余时间，1-要检测剩余时间，2-已经过期的
			obj.lifeTime = bytes.readUnsignedInt();			//到期时间 10mmddhhmm
			obj.quality = bytes.readUnsignedShort();		//品质（）
			obj.star = bytes.readUnsignedShort();			//星级
															
			obj.isBind = bytes.readUnsignedShort();	       //是否邦定 1:绑定
			obj.color = bytes.readUnsignedShort();			//（从0--5：白，白，绿，蓝，紫，橙）	 如果是武魂，此字段代表的是魂魄类型									
			var stone1:uint = bytes.readUnsignedInt();      //打孔1//宝石 99999可以开孔，88888打孔未镶嵌，4*****宝石 0:不能打孔
			var stone2:uint = bytes.readUnsignedInt();		//打孔2      
			var stone3:uint = bytes.readUnsignedInt();		//打孔3										
			if(EquipDataConst.isFourthStilettoOpen)
			{
	 			var stone4:int = bytes.readUnsignedInt();		//打孔4		
				if(stone4 == 0 && stone1 > 0)
				{
					stone4 = 99999;
				}
				obj.stoneList = [stone1,stone2,stone3,stone4]; 
			}								
			else
			{
				obj.stoneList = [stone1, stone2, stone3]; 
			}
			 
			obj.baseAtt1 = bytes.readUnsignedInt();			//基本 属性1
			obj.baseAtt2 = bytes.readUnsignedInt();			//基本 属性2
			
			obj.addAtt = [
							bytes.readUnsignedInt(),		//附加
							bytes.readUnsignedInt(),
							bytes.readUnsignedInt(),
							bytes.readUnsignedInt(),
							bytes.readUnsignedInt(),
							bytes.readUnsignedInt(),
							bytes.readUnsignedInt(),
							bytes.readUnsignedInt()
			];
			
			obj.castSpiritLevel = bytes.readUnsignedShort();       //铸灵等级
			obj.addAttribute = bytes.readUnsignedShort();          //附加属性
			obj.castSpiritCount = bytes.readInt();                 //魔灵数量
			
			var szText:String;
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int = 0;
			for(var i:int = 0;i < nDataSeeNum; i ++)
			{
				nDataSee = bytes.readByte();
				if(nDataSee != 0)
				{		
					if(i==0)
					{
						obj.itemName = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); //名字
					}
					if(i==1)
					{
						obj.builder = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); //制作者
					}
				}		
			}
			
			
			//武魂的图片特殊处理
//			if ( obj.type>250000 && obj.type<300000  )
//			{
//				var wuHunObj:Object = UIConstData.getItem( obj.type );
//				if ( wuHunObj )
//				{
//					wuHunObj.img = int( wuHunObj.img.toString() + "_" +  obj.color.toString() );
//				}
//			}
			
			var typeMul:uint = obj.type / 1000;
			if(typeMul == 301 || typeMul == 311 || typeMul == 321) {	//大血大蓝	(客户端添加剩余血量)
				obj.noUse  = obj.amount;
				obj.maxUse = obj.maxAmount;
			}
			if(typeMul == 351) {
				obj.amountMoney = obj.amount;
				obj.amount = 1;
			}
			if(obj.type == 626100) {
				obj.amountMoney = obj.amount;
				obj.amount = 1;
			}

			IntroConst.ItemInfo[obj.id] = obj;
			
			if(obj.type == 351001)	//元宝票
			{
				if(StoneMoneyMediator.isSendAction)
				{
					StoneMoneyMediator.isSendAction = false;
					facade.sendNotification(EventList.SHOWITEMTOOLPANEL, {type:obj.type, isEquip:false, data:obj});
					return;
				}
			}
			if(EquipDataConst.getInstance().isEquip(obj.type)){
				this.sendNotification(EquipCommandList.RETURN_EQUIP_INFO,obj);
			}
			
			if(isPanel)		//带面板的装备
			{
				facade.sendNotification(EventList.SHOWITEMTOOLPANEL, {type:obj.type, isEquip:true, data:obj});
				isPanel = false;
				return;
			}
			
			if(isLevelUp)
			{
				//物品升级请求详细信息	
				sendNotification(EquipCommandList.REFRESH_EQUIP);			
				isLevelUp = false;
				return;
			}
			
			if(!UIConstData.ToolTipShow) return;
			
//			if(isParallel) //对比装备
//			{
//				isParallel = false;
//				facade.sendNotification(IntroConst.SHOWPARALLEL, obj);
//				return;
//			}
			if(queryIdList[obj.id]) {		//对比装备
//				isParallel = false;
				delete queryIdList[obj.id];
				facade.sendNotification(IntroConst.SHOWPARALLEL, obj);
				return;
			}
			else
			{
				facade.sendNotification(EventList.GETINFOCOMPLETE, obj);
			}
		}
	}
}