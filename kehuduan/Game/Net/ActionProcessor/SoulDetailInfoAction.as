package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.Soul.Proxy.SoulProxy;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class SoulDetailInfoAction extends GameAction
	{
		public static var isPanel:Boolean = false;
		
		public function SoulDetailInfoAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position  = 4;
			
			/* var obj:Object  = new Object();
			obj.usAction		  = bytes.readUnsignedShort();				//usAction  1:得到武魂详细信息
			bytes.readUnsignedShort();
			obj.idItem 	  = bytes.readUnsignedInt();				//idItem 玩家物品ID
			obj.idUser    = bytes.readUnsignedInt();				//idUser	玩家ID
			
			obj.ID   = bytes.readUnsignedInt();			//武魂ID
			obj.idItem   = bytes.readUnsignedInt();	
			obj.nType   = bytes.readUnsignedInt();	//力量 + 属相	
			obj.level   = bytes.readUnsignedInt();  //等级
			obj.exp   = bytes.readUnsignedInt();	//经验
			obj.lifespan   = bytes.readUnsignedInt();	//寿命
			obj.grow   = bytes.readUnsignedInt();	//成长率
			obj.mixlev   = bytes.readUnsignedInt();	//合成等级
			obj.phyattlev   = bytes.readUnsignedInt();	//外功攻击
			obj.magattlev   = bytes.readUnsignedInt();	//内功
			obj.basestr   = bytes.readUnsignedInt();	//力量
			obj.baseint   = bytes.readUnsignedInt();	//灵气
			obj.basesta   = bytes.readUnsignedInt();	//体力
			obj.basespi   = bytes.readUnsignedInt();	//定力
			obj.baseagi   = bytes.readUnsignedInt();	//身法
			obj.addition1   = bytes.readUnsignedInt();	//扩展属性1
			obj.addition2   = bytes.readUnsignedInt();	
			obj.addition3   = bytes.readUnsignedInt();
			obj.addition4   = bytes.readUnsignedInt();
			obj.addition5   = bytes.readUnsignedInt();
			obj.addition6   = bytes.readUnsignedInt();
			obj.addition7   = bytes.readUnsignedInt();
			obj.addition8   = bytes.readUnsignedInt();
			obj.addition9   = bytes.readUnsignedInt();
			obj.addition10   = bytes.readUnsignedInt();	//扩展属性1
			obj.skill1   = bytes.readUnsignedInt();//魂魄技能1
			obj.skill2   = bytes.readUnsignedInt();
			obj.skill3   = bytes.readUnsignedInt();//魂魄技能3 */
			
			//////////////////////////////////
			var soulVo:SoulVO = new SoulVO();
			bytes.readUnsignedShort();				//usAction  1:得到武魂详细信息
			bytes.readUnsignedShort();
			soulVo.id 	  = bytes.readUnsignedInt();				//idItem 玩家物品ID
			bytes.readUnsignedInt();				//玩家ID
			
			
			soulVo.soulId   = bytes.readUnsignedInt();			//武魂ID
			bytes.readUnsignedInt();			//备用id
			
			var tag1:int = bytes.readUnsignedInt();
			
			soulVo.belong  = int(tag1/10);
			soulVo.style = tag1%10;
			
			soulVo.level   = bytes.readUnsignedInt();  //等级
			soulVo.exp   = bytes.readUnsignedInt();	//经验
			soulVo.life   = bytes.readUnsignedInt();	//寿命
			soulVo.growPercent   = bytes.readUnsignedInt();	//成长率
			soulVo.composeLevel   = bytes.readUnsignedInt();	//合成等级
			soulVo.phyAttack   = bytes.readUnsignedInt();	//外功攻击
			soulVo.magAttack   = bytes.readUnsignedInt();	//内功
			soulVo.force   = bytes.readUnsignedInt();	//力量
			soulVo.spirit   = bytes.readUnsignedInt();	//灵气
			soulVo.physical   = bytes.readUnsignedInt();	//体力
			soulVo.constant   = bytes.readUnsignedInt();	//定力
			soulVo.magic   = bytes.readUnsignedInt();	//身法

			var extendPropertyData:Array = [];	//获取扩展属性
			
			for(var i:int = 0; i < 10; i++)	
			{
				extendPropertyData.push(bytes.readUnsignedInt());
			}
			
			var soulSkills:Array = [];	//获取魂魄技能
			for(var j:int = 0; j < 3; j++)
			{
				soulSkills.push(bytes.readUnsignedInt());
			}
			
			(facade.retrieveProxy(SoulProxy.NAME) as SoulProxy).initSoulVO(extendPropertyData,soulSkills,soulVo);
			
			if( isPanel )		//带面板的装备
			{
				if ( IntroConst.ItemInfo[ soulVo.id ] )
				{
					var obj:Object = IntroConst.ItemInfo[ soulVo.id ];
					facade.sendNotification(EventList.SHOWITEMTOOLPANEL, {type:obj.type, isEquip:true, data:obj});
				}
				isPanel = false;
//				return;
			}


		}
	}
		
}
	
	