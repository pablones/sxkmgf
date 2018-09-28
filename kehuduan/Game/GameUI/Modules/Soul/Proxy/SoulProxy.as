package GameUI.Modules.Soul.Proxy
{
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulExtPropertyVO;
	import GameUI.Modules.Soul.Data.SoulSkillVO;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.Modules.Soul.Mediator.ComposeSoulMediator;
	import GameUI.Modules.Soul.Mediator.GrowUpPercentMediator;
	import GameUI.Modules.Soul.Mediator.ImproveExtendProMediator;
	import GameUI.Modules.Soul.Mediator.ImproveSkillMediator;
	import GameUI.Modules.Soul.Mediator.LearnExtendProMediator;
	import GameUI.Modules.Soul.Mediator.LearnSoulSkillMediator;
	import GameUI.Modules.Soul.Mediator.RepeatExtendProMediator;
	import GameUI.Modules.Soul.Mediator.RepeatSkillMediator;
	import GameUI.Modules.Soul.Mediator.RepeatStyleMediator;
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	import GameUI.Modules.Soul.Mediator.UseExtendGrooveMediator;
	
	import Net.ActionSend.SoulDetailInfoSend;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;
 
	public class SoulProxy extends Proxy
	{ 
		public static const NAME:String = "SoulProxy";
		
		public static const INITSOULMEDIATOR:String = "init_soul_mediator";
		public static const SHOW_SOULVIEW:String = "showSoulView"; //初始化魂魄面板
		public static const SHOW_AFTER_GET_INFO:String = "showAfterGetInfo"; //获得魂魄数据更新后初始化界面
		public static const CLOSE_ALL_SOUL_PANEL:String = "closeAllSoulPanel";
		
		public static const GET_SOUL_DETA_INFO:int = 101;	//请求魂魄详细信息 
		public static const GET_COMPOSE_INFO:int = 102;	//合成魂魄
		public static const GET_EXTEND_INFO:int = 103;	//开启属性槽
		public static const GET_REPEAT_INFO:int = 104;	// 随机获得扩展属性（重洗）
		public static const GET_LEARN_INFO:int = 105;	// 学习扩展属性
		public static const GET_UPDATE_INFO:int = 106;	// 扩展属性升级
		public static const GET_SKILL_LEARN_INFO:int = 107;	// 魂魄技能学习
		public static const GET_SKILL_UPDATE_INFO:int = 108;	// 魂魄技能升级
		public static const GET_SKILL_REPEAT_INFO:int = 109;	// 扩展技能重洗
		public static const GET_SOUL_STYLE_INFO:int = 110;	// 魂魄属相重洗
		public static const GET_GROW_UP_INFO:int = 111;	// 成长率提升
		public static const GET_COMPOSE_TONG_RUN_INFO:int = 112;	// 合成通灵玉（润魂石）
		public function SoulProxy(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
		}
		
		/**
		 *根据服务器传过来的值获取魂魄技能和扩展属性对象 
		 * @param extProData
		 * @param skillData
		 * 
		 */		
		public function initSoulVO(extProData:Array,skillData:Array,soulVo:SoulVO):void
		{
			for(var i:int = 0; i < soulVo.composeLevel; i ++)
			{
				var tag:int = extProData[i];
				var extProVo:SoulExtPropertyVO = new SoulExtPropertyVO();
				extProVo.number = i;
				if(tag == 0)	//可开槽
				{
					extProVo.state = 2;
				}
				else if(tag == 88888) //可学习
				{
					extProVo.state = 1;
				}
				else
				{
					extProVo.state = 0;		//已经开启属性
					var skillId:int = int(tag.toString().substr(0,4));
					var skillLevel:int = int(tag.toString().substr(4));
					
					var skillObj:Object = SoulData.computeSkillAttack(skillId,skillLevel,soulVo.composeLevel,soulVo.level);
					extProVo.sId = skillId;
					extProVo.name = skillObj.skillName;
					extProVo.level = skillLevel;
					extProVo.addProperty = skillObj.nTempData;
				}
				soulVo.extProperties[i] = extProVo;
			}
			
			for(var j:int = 0; j < 3; j ++)
			{
				var tag2:int = skillData[j];
				var soulSkillVo:SoulSkillVO = new SoulSkillVO();
				soulSkillVo.number = i;
				if(tag2 == 0)
				{
					continue;	//没有此技能
				}
				else if(tag2 == 88888)
				{
					soulSkillVo.state = 1; //可学习
				}
				else
				{
					soulSkillVo.state = 0; 	//可升级
					var sId:int = int(tag2.toString().substr(0,4));
					var sLevel:int = int(tag2.toString().substr(4));
					
					var sObj:Object = SoulData.computeSkillAttack(sId,sLevel,soulVo.composeLevel,soulVo.level);
					soulSkillVo.sId = sId;
					soulSkillVo.name = sObj.skillName;
					soulSkillVo.level = sLevel;
				}
				soulVo.soulSkills[j] = soulSkillVo;
			}
			
			if(ComposeSoulMediator.isComposeSoulSend)	//魂魄合成
			{
				facade.sendNotification(ComposeSoulMediator.DEAL_AFTER_SEND_COMPSE_SOUL);
			}
			//跟新魂魄信息信息
			if(RolePropDatas.ItemList[15])  //当前是否装备了魂魄
			{
				if(RolePropDatas.ItemList[15].id == soulVo.id) //装备魂魄 或跟新当前魂魄信息
				{
					if(RolePropDatas.ItemList[15].id != SoulMediator.soulVO.id)	//装备魂魄
					{
						SoulMediator.soulVO = soulVo;
					}
					if(soulVo.id == SoulMediator.soulVO.id)		//跟新魂魄信息
					{
						SoulMediator.soulVO = soulVo;
						if(SoulData.SoulDetailInfos[soulVo.id])
						{
							delete SoulData.SoulDetailInfos[soulVo.id]; 
						}
						SoulData.SoulDetailInfos[soulVo.id] = soulVo;
					}
				}
				else			//鼠标指针，或其他请求魂魄信息（不是准备魂魄）
				{
					if(!SoulData.SoulDetailInfos[soulVo.id])
					{
						SoulData.SoulDetailInfos[soulVo.id] = soulVo;
					}
					return;
				}
			}
			else
			{
				if(!SoulData.SoulDetailInfos[soulVo.id])
				{
					SoulData.SoulDetailInfos[soulVo.id] = soulVo;
				}
				return;
			}
			
			
			if(RepeatExtendProMediator.isRepeatExtendProSend)	//扩展属性重洗
			{
				facade.sendNotification(RepeatExtendProMediator.DEAL_AFTER_SEND_REPEAT_EXTEND_PRO);
			}
			if(ImproveExtendProMediator.isImproveExtendSend)	//扩展属性升级
			{
				facade.sendNotification(ImproveExtendProMediator.DEAL_AFTER_SEND_IMPROVE_EXT_PRO);
			}
			if(UseExtendGrooveMediator.isUseExtendGrooveSend)	//扩展属性槽开启
			{
				facade.sendNotification(UseExtendGrooveMediator.DEAL_AFTER_SEND_USE_EXT_GROOVE);
			}
			if(LearnExtendProMediator.isLearnExtendProSend)	//扩展属性学习
			{
				facade.sendNotification(LearnExtendProMediator.DEAL_AFTER_SEND_LEARN_EXTEND_PRO);
			}
			if(LearnSoulSkillMediator.isLearnSoulSkillSend)	//魂魄技能学习
			{
				facade.sendNotification(LearnSoulSkillMediator.DEAL_AFTER_SEND_LEARN_SKILL);
			}
			if(ImproveSkillMediator.isImproveSkillSend)		//魂魄技能升级
			{
				facade.sendNotification(ImproveSkillMediator.DEAL_AFTER_SEND_IMPROVE_SKILL);
			}
			if(RepeatSkillMediator.isRepeatSkillSend)	//魂魄技能重洗
			{
				facade.sendNotification(RepeatSkillMediator.DEAL_AFTER_SEND_REPEAT_SKILL);
			}
			if(GrowUpPercentMediator.isGrowUpPercentSend)	//成长率提升
			{
				facade.sendNotification(GrowUpPercentMediator.DEAL_AFTER_SEND_GROW_UP_PERCENT);
			}
			if(RepeatStyleMediator.isRepeatStyleSend)	//属相重洗
			{
				facade.sendNotification(RepeatStyleMediator.DEAL_AFTER_SEND_REPEAT_STYLE);
			}
			
			if(RolePropDatas.CurView != SoulMediator.TYPE)		//魂魄面板未开启
			{
				return;
			}
			
			facade.sendNotification(SoulProxy.SHOW_AFTER_GET_INFO);
		} 
		
		/**
		 *请求当前装备魂魄详细信息 
		 * 
		 */		
		public static function getSoulDetailInfo():void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_SOUL_DETA_INFO,obj.id,0,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
				
		} 
		
		public static function getPeopleSoulDetail( itemId:int,playerId:int ):void
		{
			//请求魂魄详细信息
			var reqData:Array = [ 101,itemId,playerId,0 ];
			SoulDetailInfoSend.createSoulDetailInfoMsg( reqData );
		}
		
		/**
		 *请求当前装备魂魄详细信息 
		 * 
		 */		
		public static function getSoulDetailInfoFromBag(id:int):void
		{
			var data:Array = [GET_SOUL_DETA_INFO,id,0,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
				
		}
		
		/**
		 *合成魂魄
		 * 
		 */		
		public static function getComposeSoulInfo(mainId:int,secondId:int):void
		{
			var data:Array = [GET_COMPOSE_INFO,mainId,secondId,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		
		/**
		 *开启属性槽
		 * 
		 */		
		public static function getExtendInfo(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_EXTEND_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		/**
		 *	随机获得扩展属性
		 * 
		 */		
		public static function getExtendRandom(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_REPEAT_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		/**
		 *	学习扩展属性
		 * 
		 */		
		public static function getLearnExtend(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_LEARN_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		/**
		 *	扩展属性升级
		 * 
		 */		
		public static function getUpdateExtend(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_UPDATE_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		
			
		/**
		 * 魂魄技能学习
		 
		 * 
		 */		 
		public static function getSkillLearn(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_SKILL_LEARN_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		
		/**
		 * 魂魄技能升级
		 * @param num 
		 * 
		 */		
		public static function getSkillUpdate(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_SKILL_UPDATE_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		/**
		 * 魂魄技能重洗
		 * 
		 */		
		public static function getSkillReapeat():void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_SKILL_REPEAT_INFO,obj.id,0,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		
		
		/**
		 * 合成通灵玉(通灵玉)
		 * @param id 物品id
		 * 
		 */
		public static function getComposeStone(id:int):void
		{
			var data:Array = [GET_COMPOSE_TONG_RUN_INFO,id,0,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		/**
		 * 魂魄属相重洗
		 * 
		 */		
		public static function getRepeatStyle(num:int):void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_SOUL_STYLE_INFO,obj.id,num,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		/**
		 * 魂魄成长率提示
		 * 
		 */		
		public static function getGrowUp():void
		{
			var obj:* = RolePropDatas.ItemList[15];
			var data:Array = [GET_GROW_UP_INFO,obj.id,0,0];
			SoulDetailInfoSend.createSoulDetailInfoMsg(data);
		}
		
		/**
		 * 获取当前的金钱
		 * */
		public static function getPlayTotalMoney():int
		{
			return GameCommonData.Player.Role.BindMoney+GameCommonData.Player.Role.UnBindMoney;
		}
	}
}