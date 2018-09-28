package Net.ActionProcessor
{
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class PlayerDetail extends GameAction
	{
		public function PlayerDetail(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		override public function Processor(bytes:ByteArray):void{
		
			bytes.position = 4; 
			var obj:Object={};
			obj.id= bytes.readUnsignedInt();  //玩家id
			var arr:Array=[];
			for(var j:int = 0 ; j < 15 ; j ++)
			{	
				var typeID:uint = bytes.readUnsignedInt();		//typeID + color 组合 
				var equipRealID:uint = bytes.readUnsignedInt();	//物品ID
				var type:uint  = typeID / 10;	//typeID
				var color:uint = typeID % 10;	//color
				arr.push({equipTypeID:type, equipRealID:equipRealID, color:color});
			}
			obj.equipments=arr;
			obj.usWarSroce 	        = bytes.readUnsignedShort();      //战力评分
			obj.usUserlev       	= bytes.readUnsignedShort();      //玩家等级
			obj.usFirPro     	    = bytes.readUnsignedShort();      //第一职业
			obj.usFirProLev     	= bytes.readUnsignedShort();      //第一职业等级
			obj.usSecPro 	        = bytes.readUnsignedShort();	  //第二职业
			obj.usSecProLev      	= bytes.readUnsignedShort();      //第二职业等级
			obj.usSex      			= bytes.readUnsignedShort();      //性别
//			bytes.readUnsignedShort();
			
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int = 0;			
			for(var i:int = 0;i < nDataSeeNum; i ++)
			{
				nDataSee = bytes.readByte();
				if(nDataSee != 0)
				{
					if(i == 0)
					{
						var szSyn:String = bytes.readMultiByte(nDataSee ,GameCommonData.CODE); 	//帮派
						obj.szSyn=szSyn;
					}
					else if(i == 1)
					{
						var sztitle:String = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);	//称号
						obj.sztitle=sztitle;
					}
					else if(i == 2)
					{
						var szFeel:String = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);	//心情
						obj.szFeel=szFeel;
					}
					else if(i == 3)
					{
						var szWife:String = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);	//配偶 
						if(GameCommonData.wordVersion == 2) 	//台服
						{
							if ( szWife != "拸" )
							{
								obj.szWife = szWife;
							}
						}
					}
				}		
			}
			facade.sendNotification(PlayerInfoComList.SHOW_PLAYER_DETAILINFO,obj);
		}
		
	}
}