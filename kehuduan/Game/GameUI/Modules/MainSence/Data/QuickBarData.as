package GameUI.Modules.MainSence.Data
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.UICore.UIFacade;
	import GameUI.View.items.UseItem;
	
	import flash.utils.Dictionary;
	
	public class QuickBarData
	{
		
		private static var _instance:QuickBarData;
		public var quickKeyDic:Dictionary;
		public var expandKeyDic:Dictionary;
		public var mainJobQuickKey:Object;
		public var viceJobQuickKey:Object;
		protected var petSkillList:Object;
		
		public function QuickBarData()
		{
			this.quickKeyDic=new Dictionary();
			this.expandKeyDic=new Dictionary();
			petSkillList={};
			petSkillList["7031"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_1" ];//"冰天雪地";
			petSkillList["7032"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_2" ];//"烈火燎原";
			petSkillList["7033"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_3" ];//"血毒万里";
			petSkillList["7034"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_4" ];//"五雷轰顶";
			petSkillList["7035"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_5" ];//"血爆";
			petSkillList["7036"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_6" ];//"高级血爆";
			petSkillList["7037"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_7" ];//"圣爆";
			petSkillList["7038"]=GameCommonData.wordDic[ "mod_mai_dat_qui_qui_8" ];//"高级圣爆";
		}
		
		
		/**
		 * 是否是宠物的主动技能（快捷栏中的主动技能） 
		 * @param type
		 * @return 
		 * 
		 */		
		public function isInitiactivePetSkill(type:uint):Boolean{
			return (petSkillList[String(type)]!=null);
		}
		
		/**
		 *  获取单例  
		 * @return 
		 * 
		 */		
		public static function getInstance():QuickBarData{
			if(_instance==null)_instance=new QuickBarData();
			return _instance;
		}
				
		/**
		 *  公共CD时间
		 * 
		 */		
		public function startCommonCoolDown():void{
			
		}
		
		/**
		 * 根据CDType类型找到快栏中的同类型要启动CD的物品 
		 * @param type
		 * @return 
		 * 
		 */		
		public function getUseItemByType(type:uint):Array{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			var arr:Array=[];
			for(var id:* in dic){
				if(dic[id]!=null){
					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
					if(type==this.getCdType(useItem.Type))arr.push(useItem);			
				}
			}
			
			for(var idF:* in dicF){
				if(dicF[idF]!=null){
					var useItemF:UseItem=dicF[idF];
					if(type==this.getCdType(useItemF.Type))arr.push(useItemF);	
				}
			}
			return arr;
		}
		
		/**
		 * 根据传过来的CdType 
		 * @param cdType
		 * @return 
		 * 
		 */		
		public function getSkillItemByType(cdType:uint):UseItem{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			var arr:Array=[];
			for(var id:* in dic){
				if(dic[id]!=null){
					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
					if(cdType==useItem.Type)return useItem;			
				}
			}
			
			for(var idF:* in dicF){
				if(idF==7)continue;
				if(dicF[idF]!=null){
					var useItemF:UseItem=dicF[idF];
					if(cdType==useItemF.Type)return useItemF;	
				}
			}
			return null;
		}
		
		/**
		 * 技能加公共Cd 
		 * 
		 */		
//		 public function skillsInQuickAddCd(cdType:int,cdTime:int):void{
//			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
//			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
//			for each(var typeItem:UseItem in dic){
//				if(typeItem){
//					if(typeItem.Type == cdType){
//						if(typeItem.IsCdTimer){
//							typeItem.clearCd();
//						}
//						typeItem.startCd(cdTime,-120);
//					}
//					else{
//						if(!typeItem.IsCdTimer){
//							if(typeItem.Type < 6000 || (typeItem.Type > 7000 && typeItem.Type < 9000)){
////								typeItem.startCd(1000,-120);
//								typeItem.startCd(1500,-120);
//							}
//						}
//					}
//				}
//			}
//			
//			for each(var typeItemF:UseItem in dicF){
//				if(typeItemF){
//					if(typeItemF.Type == cdType){
//						if(typeItemF.IsCdTimer){
//							typeItemF.clearCd();
//						}
//						typeItemF.startCd(cdTime,-120);
//					}
//					else{
//						if(!typeItemF.IsCdTimer){
//							if(typeItemF.Type < 6000 || (typeItemF.Type > 7000 && typeItemF.Type < 9000)){
////								typeItemF.startCd(1000,-120);
//								typeItemF.startCd(1500,-120);
//							}
//						}
//					}
//				}
//			}
//		} 
		
		/**
		 * 根据物品的type值找到该物品的CdType  
		 * @param itemType:物品type
		 * @return : -1:如果没有找到
		 * 
		 */		
		public function getCdType(itemType:uint):int{
			var obj:Object=UIConstData.getItem(itemType); 
			if(obj==null){
				return -1;
			}else{
				return Math.floor(obj.cdType/10000);		
			}
		}
		
		
		/**
		 * 在背包中找到一个可用的Id号 
		 * @param useItem
		 * @return 
		 * 
		 */		
		public function getItemIdFromBag(useItem:UseItem):uint{
			for each(var obj:*  in (BagData.AllItems[0] as Array)){
				if(obj==null)continue;
				if(obj.type==useItem.Type && obj.isBind==useItem.IsBind){
					return obj.id;
				}		
			}
			return 0;
		}
		
		
		/**
		 * 在背包中找一个相同type的Id,不考虑绑定不绑定 
		 * @param useItem
		 * @return 
		 * 
		 */		
		public function getItemIdFromBagNoBind(useItem:UseItem):uint{
			for each(var obj:*  in (BagData.AllItems[0] as Array)){
				if(obj==null)continue;
				if(obj.type==useItem.Type){
					return obj.id;
				}		
			}
			return 0;
		}
		
		/**
		 * 在背包中查找没有和快捷栏相同的物品同，并且是没有被锁定的 
		 * @param useItem
		 * @return 
		 * 
		 */		
		public function getItemFromBag(useItem:UseItem):uint{
			for each(var obj:*  in (BagData.AllItems[0] as Array)){
				if(obj==null)continue;
				if(obj.type==useItem.Type && obj.isBind==useItem.IsBind && obj){
					return obj.id;
				}		
			}
			return 0;
		}
	
		
		
		
		/**
		 * 查找背包中相同cdType类型的所有物品 
		 * @param cdType：cd类型
		 * @return [{}];
		 * 
		 */		
		public function getItemsFromBag(cdType:uint):Array{
			var arr:Array=[];
			for each(var obj:*  in (BagData.AllItems[0] as Array)){
				if(obj==null)continue;
				if(cdType==getCdType(obj.type)){
					arr.push(obj);
				}	
			}
			return arr;	
		}
		
		/**
		 * 判断一下该物品是否可以使用
		 * @param useItem：物品
		 * @return  true:可以使用
		 * 
		 */		
		public function isEnableUseTheItem(useItem:UseItem):Boolean{
			
			if(useItem.Type > 300000){
				if(useItem.Type<310000&&(GameCommonData.Player.Role.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP) <= GameCommonData.Player.Role.HP)
				{
					GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_dat_qui_isE_1" ], color:0xffff00});//"生命值已满，无需恢复"
					return false;
				}
				if(useItem.Type<320000&& useItem.Type>310000&&(GameCommonData.Player.Role.MaxMp+GameCommonData.Player.Role.AdditionAtt.MaxMP) <= GameCommonData.Player.Role.MP)
				{
					GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_dat_qui_isE_2" ], color:0xffff00});//"气已满，无需恢复"
					return false; 
				}
			}
			
			//坐骑
			if(useItem.Type==9508){
				if(RolePropDatas.ItemList[9]==null){
					GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_dat_qui_isE_3" ], color:0xffff00});//"你还没有装备上坐骑"
					return false; 
				}
				if(StallConstData.stallSelfId>0 || UIFacade.UIFacadeInstance.isLookStall() ){
					UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_mai_dat_qui_isE_4" ],0xffff00);//"摆摊或查看摊位时不能使用坐骑"
					return false;
				}
			
				if(UIFacade.UIFacadeInstance.isTrading()){		
					UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_mai_dat_qui_isE_5" ],0xffff00);//"交易中不能使用坐骑"
					return false;
				}	
			}
			if(useItem.Type==9000){
				
				if(StallConstData.stallSelfId>0 || UIFacade.UIFacadeInstance.isLookStall() ){
					UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_mai_dat_qui_isE_6" ],0xffff00);//"摆摊或查看摊位时不能使用回城"
					return false;
				}
			
				if(UIFacade.UIFacadeInstance.isTrading()){		
					UIFacade.UIFacadeInstance.showPrompt(GameCommonData.wordDic[ "mod_mai_dat_qui_isE_7" ],0xffff00);//"交易中不能使用回城"
					return false;
				}	
				
			}
				
			return true;
		}
		
		
	}
}