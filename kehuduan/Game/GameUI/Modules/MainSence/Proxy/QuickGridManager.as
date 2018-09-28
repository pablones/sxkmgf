package GameUI.Modules.MainSence.Proxy
{
	import Controller.CooldownController;
	import Controller.PlayerController;
	import Controller.TargetController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Command.SetCdData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.DropEvent;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	import Net.ActionSend.PlayerActionSend;
	import Net.Protocol;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.display.DisplayObject;
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class QuickGridManager extends Proxy
	{
		public static const NAME:String = "QuickGridManager";
		private var mainSence:MovieClip;
		public function QuickGridManager(mainSence:MovieClip)
		{
			super(NAME);
			this.mainSence=mainSence;
		}
			
		
		/**
		 * 刷新 
		 * 
		 */			
		public function refresh():void{
			this.clearAll();
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			for(var id:* in dic){
				if(QuickBarData.getInstance().quickKeyDic[id]!=null){
					var grid:MovieClip=mainSence.mcQuickBar0["quick_"+id];
					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
					useItem.name="key_"+id;
					useItem.addEventListener(MouseEvent.MOUSE_DOWN,onMosueDowmHandler);
					useItem.addEventListener(MouseEvent.CLICK,onUseItemClick);
					useItem.addEventListener(DropEvent.DRAG_THREW,onDrawThrewHandler);
					useItem.addEventListener(DropEvent.DRAG_DROPPED,onDragedHandler);
					useItem.mouseEnabled=true;
					grid.addChild(useItem);
					useItem.x=2;
					useItem.y=2;
				}
			}
			for(var idF:* in dicF){
				if(dicF[idF]!=null){
					var gridF:MovieClip=mainSence.mcQuickBar1["quickf_"+idF];
					var useItemF:UseItem=dicF[idF];
					useItemF.name="keyF_"+idF;
					useItemF.mouseEnabled=true;
					useItemF.addEventListener(MouseEvent.MOUSE_DOWN,onMosueDowmHandler);
					useItemF.addEventListener(MouseEvent.CLICK,onUseItemClick);
					useItemF.addEventListener(DropEvent.DRAG_THREW,onDrawThrewHandler);
					useItemF.addEventListener(DropEvent.DRAG_DROPPED,onDragedHandler);
					gridF.addChild(useItemF);
					useItemF.x=2;
					useItemF.y=2;
				}
			}
		}
		
		
		/**
		 * 点击使用快捷栏 
		 * @param e
		 * 
		 */		
		protected function onUseItemClick(e:MouseEvent):void{
			var useItem:UseItem=e.currentTarget as UseItem;
			if (useItem.dragged) return;
			this.useQuickKey(useItem);
		}
		
		protected function useQuickKey(useItem:UseItem):void{
			
			if(useItem==null || !UIConstData.KeyBoardCanUse){
				return;
			}
			
			if(!CooldownController.getInstance().cooldownReady(useItem.Type))
			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"技能冷却中", color:0xffff00});//
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "con_keyboard_useQuick" ], color:0xffff00});//技能冷却中
				return;
			}
			
			if(QuickBarData.getInstance().isEnableUseTheItem(useItem)){
				if(useItem.Type>=300000 && QuickBarData.getInstance().getItemIdFromBag(useItem)>0){
					if(useItem.Type >= 320000 && useItem.Type < 340000) {
						if(GameCommonData.Player.Role.UsingPet) {
							NetAction.presentRoseToFriend(QuickBarData.getInstance().getItemIdFromBag(useItem), GameCommonData.Player.Role.UsingPet.Id,0);
						}else{
							GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_pro_qui_onU" ], color:0xffff00});//"当前没有出战宠物"
						}
						return;
					}else{
						NetAction.UseItem(OperateItem.USE, 1, 0,QuickBarData.getInstance().getItemIdFromBag(useItem));
					}		
				}else{ 
					var skillType:int = useItem.Type;
					PlayerController.UseSkill(skillType);
					var skilllevel:GameSkillLevel = GameCommonData.Player.Role.SkillList[skillType] as GameSkillLevel;
					var role:GameRole = GameCommonData.Player.Role;
					var boo:Boolean;
					if(skillType == 1139 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		润脉	
					{
						boo = true;
					}
					else if(skillType == 1102 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		强身
					{
						boo = true;
					}
					else if(skillType == 1301 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		云体		
					{
						boo = true;
					}
					else if(skillType == 1121 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		灵蕴	
					{
						boo = true;
					}
					else if(skillType == 1401 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		定军	
					{
						boo = true;
					}
					else if(skillType == 1201 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		蛟龙劲	
					{
						boo = true;
					}
					else if(skillType == 1303 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)//		腐骨矢
					{	
						if (GameCommonData.TargetAnimal && TargetController.IsAttack(GameCommonData.TargetAnimal))
						{
							boo = true;
						}
					}
					if(boo)
					{
//						QuickBarData.getInstance().skillsInQuickAddCd(skillType,1000);	//触发公共CD
						CooldownController.getInstance().triggerGCD();
					}
				}		
			}
		}
		
		/**
		 * 获得背包中该物品的数量 
		 * @param type
		 * @param isBind
		 * @return 
		 * 
		 */		
		protected function getNumFromBag(type:uint,isBind:uint):uint{
			var count:uint=0;
			for each(var obj:*  in (BagData.AllItems[0] as Array)){
				if(obj==null)continue ;
				if(obj.type==type && obj.isBind==isBind){
					count+=obj.amount;
				}		
			}
			return count;
		}
		
		/**
		 * 添加快捷物品 
		 * @param obj
		 * 
		 */		
		public function addUseItem(obj:Object):void{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			var source:ItemBase=obj.source as ItemBase;
			var index:uint=obj.index;
			var type:String=obj.type;
			var useItem:UseItem;
			
			if((type=="quickf" || type=="keyF") && index==7){
				return ;
			}
			if(source.name.split("_")[0] ==  "petSkillItem")
			{
				if(obj.target.name != "keyF_7")
					return;
			}

			if(this.isInQuick(source.Type,source.IsBind)){
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_pro_qui_add" ], color:0xffff00});//"已经在快捷栏中"  
				return;
			}
			if(this.isInQuickEnable(source.Type)){
				//todo  加入QuickBar
				useItem=new UseItem(-1,String(source.Type),null);
				useItem.mouseEnabled=true;
				useItem.IsBind=source.IsBind;
				useItem.Id=source.Id;
				
				CooldownController.getInstance().registerCDItem(source.Type, useItem);
				
				//如果拖动是宠物技能
				if(QuickBarData.getInstance().isInitiactivePetSkill(source.Type) && !CooldownController.getInstance().cooldownReady(source.Type)){
//					useItem.startCd(source.cdTotalTime,source.curCdCount);
					
					trace(source.Type);
					//++ CooldownController.getInstance().triggerCD(source.Type, CooldownController.getInstance().getCooldownTimeSpan(source.Type));
				}
				
				//背包中的物品（检查是否还在cd）
				if(source.Type>300000){					
					var count:uint=0;
					for each(var obj:*  in (BagData.AllItems[0] as Array)){
						if(obj==null)continue ;
						if(obj.type==useItem.Type && obj.isBind==useItem.IsBind){
							count+=obj.amount;
						}		
					}
					var cdObj:Object=SetCdData.searchCdObjByType(source.Type);
					if(cdObj!=null){
//						useItem.startCd(cdObj.cdtimer,cdObj.count);
//						CooldownController.getInstance().triggerCD(source.Type, cdObj.cdtimer);
					}
					useItem.Num=count;
				}
				
				useItem.addEventListener(MouseEvent.MOUSE_DOWN,onMosueDowmHandler);
				useItem.addEventListener(MouseEvent.CLICK,onUseItemClick);
				useItem.addEventListener(DropEvent.DRAG_THREW,onDrawThrewHandler);
				useItem.addEventListener(DropEvent.DRAG_DROPPED,onDragedHandler);
				useItem.x=2;
				useItem.y=2;
				if(type=="quick" || type=="key"){
					if(dic[index]!=null){
						dic[index].parent.removeChild(dic[index]);
					}
					useItem.name="key_"+index;
					dic[index]=useItem;
					mainSence.mcQuickBar0["quick_"+index].addChild(dic[index]);
				}else if(type=="quickf" || type=="keyF"){
					if(dicF[index]!=null){
						dicF[index].parent.removeChild(dicF[index]);
					}
					useItem.name="keyF_"+index;
					dicF[index]=useItem;
					mainSence.mcQuickBar1["quickf_"+index].addChild(dicF[index]);
				}
				if(NewerHelpData.newerHelpIsOpen){	//通知新手引导系统
					sendNotification(NewerHelpEvent.USE_QUICKBAR_NOTICE_NEWER_HELP,{type:useItem.Type});
				}
				
				sendNotification(EventList.SEND_QUICKBAR_MSG);
			}
		}
		
		/**
		 * 快捷物品与背包同步 
		 * 
		 */		
		public function onSyncBag():void{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			for(var id:* in dic){
				if(QuickBarData.getInstance().quickKeyDic[id]!=null){
					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
					if(useItem.Type>=300000){
						var num:uint=this.getNumFromBag(useItem.Type,useItem.IsBind);
						if(num==0){
							QuickBarData.getInstance().quickKeyDic[id]=null;
							if(useItem.parent!=null){
								useItem.parent.removeChild(useItem);
							}
						}else{
							useItem.Num=num;
						}						
					}
				}
			}
			
			for(var idF:* in dicF){
				if(dicF[idF]!=null){
					var useItemF:UseItem=dicF[idF];
					if(useItemF.Type>=300000){
						var numF:uint=this.getNumFromBag(useItemF.Type,useItemF.IsBind);
						if(numF==0){
							QuickBarData.getInstance().expandKeyDic[idF]=null;
							if(useItemF.parent!=null){
								useItemF.parent.removeChild(useItemF);
							}
						}else{
							useItemF.Num=numF;
						}		
					}
				}
			}
		}
		
		/**
		 * 此类物口是否已经在快捷栏中 
		 * @param type
		 * @return 
		 * 
		 */		
		protected function isInQuick(type:uint,isBind:uint):Boolean{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			for(var i:uint=0;i<8;i++){
				var useItem:UseItem=dic[i];
				if(useItem!=null && useItem.Type==type && useItem.IsBind==isBind){
					return true;
				}
				useItem=dicF[i];
				
				if(useItem!=null && useItem.Type==type && useItem.IsBind==isBind && i!=7){
					return true;
				}
			}	
			return false;
		}
		
		
		/**
		 *  判断该类型的物品能否进入快捷栏
		 * @param type 
		 * @return 
		 * 
		 */		
		public function isInQuickEnable(type:uint):Boolean{
			if(type<100000){
				 return true;
			}else{
				var id:uint=Math.floor(type/100000);
				if(Math.floor(type/1000)==351)return false;        //341类型不能放入快捷栏
				if(type>=502038 && type<=502099)return true;
				if(id==3 && type!=301001 && type!=301002 && type!=311001 && type!=311002 && type!=321001)return true;
			}
			return false;
		}
		
		/**
		 * 拖动后放下处理事件 
		 * @param e
		 * 
		 */		
		protected function onDragedHandler(e:DropEvent):void{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			var index:uint=e.Data.index;
			if(e.Data.source.name=="keyF_7"){//宠物的不能交换
				if(dicF[7]){
					if((dicF[7] as UseItem).name == "keyF_7")
						return;
				}
				this.addUseItem(e.Data);
				return;
			}  
			if(e.Data.type=="quick" || e.Data.type=="key"){
				if(dic[index]!=null){
					var target:DisplayObject=dic[index];   //目标
					var targetName:String=target.name;    
					var source:DisplayObject=e.Data.source;  //源
					var sourceName:String=source.name;
					var sourceParent:DisplayObjectContainer=e.Data.source.parent; //源父对象
					if(target==null)return;
					var targetParent:DisplayObjectContainer=target.parent;        //目标父对象
					if(sourceParent==targetParent)return;
					sourceParent.removeChild(e.Data.source);
					targetParent.removeChild(target);
					var sourceIndex:uint=uint(sourceName.substr(sourceName.length-1,sourceName.length))
					//交换名字
					target.name=sourceName;
					source.name=targetName;
					if(sourceName.indexOf("keyF")!=-1){
						dicF[sourceIndex]=target;
						mainSence.mcQuickBar1["quickf_"+sourceIndex].addChild(dicF[sourceIndex]);
					}else if(sourceName.indexOf("key")!=-1){
						dic[sourceIndex]=target;
						mainSence.mcQuickBar0["quick_"+sourceIndex].addChild(dic[sourceIndex]);
					}
					dic[index]=source;
					mainSence.mcQuickBar0["quick_"+index].addChild(dic[index]);
					
				//目标为空	
				}else{
					var source1:DisplayObject=e.Data.source;  //源
					var sourceName1:String=source1.name; 
					var sourceParent1:DisplayObjectContainer=e.Data.source.parent; //源父对象
					if(sourceParent1.name!="quickf_7"){
						sourceParent1.removeChild(source1);
					}
					var sourceIndex1:uint=uint(sourceName1.substr(sourceName1.length-1,sourceName1.length))
					source1.name="key_"+index;
					if(sourceName1.indexOf("keyF")!=-1){
						dicF[sourceIndex1]=null;
					}else if(sourceName1.indexOf("key")!=-1){
						dic[sourceIndex1]=null;
					}
					dic[index]=source1;
					mainSence.mcQuickBar0["quick_"+index].addChild(dic[index]);
				}
			//上面的互相交换	
			}else if(e.Data.type=="quickf" || e.Data.type=="keyF"){
				if(index==7)return;
				if(dicF[index]!=null){
					var target2:DisplayObject=dicF[index];   //目标
					var targetName2:String=target2.name;
					var source2:DisplayObject=e.Data.source;  //源
					var sourceName2:String=source2.name;
					var sourceParent2:DisplayObjectContainer=e.Data.source.parent; //源父对象
					if(target2==null)return;
					var targetParent2:DisplayObjectContainer=target2.parent;        //目标父对象
					if(sourceParent2==targetParent2)return;
					sourceParent2.removeChild(e.Data.source);
					targetParent2.removeChild(target2);
					var sourceIndex2:uint=uint(sourceName2.substr(sourceName2.length-1,sourceName2.length));
					//交换名字
					target2.name=sourceName2;
					source2.name=targetName2;
					if(sourceName2.indexOf("keyF")!=-1){
						dicF[sourceIndex2]=target2;
						mainSence.mcQuickBar1["quickf_"+sourceIndex2].addChild(dicF[sourceIndex2]);
					}else if(sourceName2.indexOf("key")!=-1){
						dic[sourceIndex2]=target2;
						mainSence.mcQuickBar0["quick_"+sourceIndex2].addChild(dic[sourceIndex2]);
					}
					dicF[index]=source2;
					mainSence.mcQuickBar1["quickf_"+index].addChild(dicF[index]);
					
				}else{
					var source3:DisplayObject=e.Data.source;  //源
					var sourceName3:String=source3.name;
					var sourceParent3:DisplayObjectContainer=e.Data.source.parent; //源父对象
					sourceParent3.addChild(source3);
					var sourceIndex3:uint=uint(sourceName3.substr(sourceName3.length-1,sourceName3.length))
					source3.name="keyF_"+index;
					if(sourceName3.indexOf("keyF")!=-1){
						dicF[sourceIndex3]=null;
					}else if(sourceName3.indexOf("key")!=-1){
						dic[sourceIndex3]=null;
					}
					dicF[index]=source3;
					mainSence.mcQuickBar1["quickf_"+index].addChild(dicF[index]);
				}
			}			
		}
		
		/**
		 * 放弃快捷键处理 
		 * @param e
		 * 
		 */		
		protected function onDrawThrewHandler(e:DropEvent):void{
			var useItem:UseItem=e.Data as UseItem;
			if(useItem.parent.name=="quickf_7")return;
//			if(useItem.IsCdTimer){
//				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_mai_pro_qui_onD" ], color:0xffff00});//"道具正在冷却中，不能丢弃"
//				return;
//			}
			CooldownController.getInstance().unregisterCDItem(useItem.Type, useItem);
			
			useItem.parent.removeChild(useItem);
			var arr:Array=useItem.name.split("_");
			if(arr[0]=="key"){
				QuickBarData.getInstance().quickKeyDic[arr[1]]=null;
			}else if(arr[0]=="keyF"){
				QuickBarData.getInstance().expandKeyDic[arr[1]]=null;
			}
		}
		
		/**
		 * 清除所有宠物技能（当宠物死亡与休息时） 
		 * 
		 */		
		public function clearAllPetSkill():void{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			for(var id:* in dic){
				if(QuickBarData.getInstance().quickKeyDic[id]!=null){
					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
					if(useItem.parent!=null && useItem.Type>7000 && useItem.Type<8999){
						useItem.parent.removeChild(useItem);
						QuickBarData.getInstance().quickKeyDic[id]=null;
						CooldownController.getInstance().unregisterCDItem(useItem.Type, useItem);
					}
				}
			}
			for(var idF:* in dicF){
				if(dicF[idF]!=null){
					var useItemF:UseItem=dicF[idF];
					if(useItemF.parent!=null && useItemF.Type>7000 && useItemF.Type<8999){
						useItemF.parent.removeChild(useItemF);
						CooldownController.getInstance().unregisterCDItem(useItemF.Type, useItemF);
						dicF[idF]=null;
					}
				}
			}
			CooldownController.getInstance().resetPetCDs();
		}
		
		/**
		 * 清除所有玩家的技能CD（主要用了一个特殊技能） 
		 * 
		 */		
		public function clearAllPlayerSkillCd():void{
//			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
//			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
//			for(var id:* in dic){
//				if(QuickBarData.getInstance().quickKeyDic[id]!=null){
//					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
//					if(useItem.IsCdTimer && isPlayerSkill(useItem.Type)){
//						useItem.clearCd();
//					}
//				}
//			}
//			for(var idF:* in dicF){
//				if(dicF[idF]!=null){
//					var useItemF:UseItem=dicF[idF];
//					if(useItemF.IsCdTimer && isPlayerSkill(useItemF.Type)){
//						useItemF.clearCd();
//					}
//				}
//			}
			CooldownController.getInstance().resetAllCDs();
		}
		
		/**
		 * 判断是否是玩家技能 
		 * @param type
		 * @return 
		 * 
		 */		
		private function isPlayerSkill(type:uint):Boolean{
			if(type>1000 && type<7000 || type>=9000 && type<10000){
				return true;
			}else{
				return false;
			}		
		}
		
				
		protected function onMosueDowmHandler(e:MouseEvent):void{
			var useItem:UseItem=e.currentTarget as UseItem;
			if (useItem == QuickBarData.getInstance().expandKeyDic[7]) return;
			if(useItem.mouseX<=2 || useItem.mouseY>=useItem.width-2 || useItem.mouseY<=2 || useItem.mouseY>=useItem.height-2){
				return;
			}
			useItem.onMouseDown();
		}
		
		
		/** 清空一下以前的物品*/
		private function clearAll():void{
			var dic:Dictionary=QuickBarData.getInstance().quickKeyDic;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			for(var id:* in dic){
				if(QuickBarData.getInstance().quickKeyDic[id]!=null){
					var useItem:UseItem=QuickBarData.getInstance().quickKeyDic[id];
					if(useItem.parent!=null){
						useItem.parent.removeChild(useItem);
					}
				}
			}
			for(var idF:* in dicF){
				if(dicF[idF]!=null){
					var useItemF:UseItem=dicF[idF];
					if(useItemF.parent!=null){
						useItemF.parent.removeChild(useItemF);
					}
				}
			}
		}
		
		/**
		 *  改变职业时。调用该方法进行快捷键的切换,模拟发送另一个职业的快捷栏信息
		 * 
		 */		
		public function changeJob():void{
			this.clearAll();
			var obj:Object=GameCommonData.Player.Role.CurrentJob==1 ? QuickBarData.getInstance().mainJobQuickKey : QuickBarData.getInstance().viceJobQuickKey;
			sendNotification(EventList.RECEIVE_QUICKBAR_MSG,obj);
			NetAction.requestCd();  //再请求CD信息
		}
		
		/**
		 * 获取1-8快捷键中第一个空白格子的舞台坐标
		 * @return 
		 * 
		 */		
		public function getEmptyTilePos():Point{
			for(var i:uint=0 ;i<8;i++){
				if(QuickBarData.getInstance().quickKeyDic[i]==null && mainSence.mcQuickBar0["quick_"+i].parent!=null){
					var p:Point=((mainSence.mcQuickBar0["quick_"+i].parent) as DisplayObject).localToGlobal(new Point(mainSence.mcQuickBar0["quick_"+i].x,mainSence.mcQuickBar0["quick_"+i].y));
					return p;
				}
			}
			return null;
		} 
		
		public function getEmptyQuickBar0():Object{
			for(var i:uint=0;i<8;i++){
				if(QuickBarData.getInstance().quickKeyDic[i]==null && mainSence.mcQuickBar0["quick_"+i].parent!=null){
					//临时数据，防止覆盖
					var item:UseItem = new UseItem( i, "", mainSence.mcQuickBar0["quick_"+i]);
					QuickBarData.getInstance().quickKeyDic[i] = item;
					mainSence.mcQuickBar0["quick_"+i].addChild( item );
					item.visible = false;
					
					var p1:Point=((mainSence.mcQuickBar0["quick_"+i].parent) as DisplayObject).localToGlobal(new Point(mainSence.mcQuickBar0["quick_"+i].x,mainSence.mcQuickBar0["quick_"+i].y));
					return {point:p1, index:i, type:"quick"};
				}
			}
			return null;
		}
		
		public function getEmptyQuickBar1():Object{
			for(var i:uint=0;i<7;i++){
				if(QuickBarData.getInstance().expandKeyDic[i]==null && mainSence.mcQuickBar1["quickf_"+i].parent!=null){
					//临时数据，防止覆盖
					var item:UseItem = new UseItem( i, "", mainSence.mcQuickBar1["quickf_"+i]);
					QuickBarData.getInstance().expandKeyDic[i] = item;
					mainSence.mcQuickBar1["quickf_"+i].addChild( item );
					item.visible = false;
					
					var p2:Point=((mainSence.mcQuickBar1["quickf_"+i].parent) as DisplayObject).localToGlobal(new Point(mainSence.mcQuickBar1["quickf_"+i].x,mainSence.mcQuickBar1["quickf_"+i].y));
					return {point:p2, index:i, type:"quickf"};
				}
			}
			return null;
		}
		
		/**
		 * 添加宠物主动技能(特殊的宠物技能)
		 * 
		 */		
		public function addPetInitiativeSkill():void{
			var role:GamePetRole=GameCommonData.Player.Role.PetSnapList[GameCommonData.Player.Role.UsingPet.Id] as GamePetRole;
			var dicF:Dictionary=QuickBarData.getInstance().expandKeyDic;
			if(dicF[7]!=null){
				return ;
			}
			if(role!=null){
				var arr:Array=role.SkillLevel;
//				for each(var a:GameSkillLevel in arr){
				for each(var a:* in arr){
					if(!a) continue; 
					var type:uint=a.gameSkill.SkillID; 
					if(QuickBarData.getInstance().isInitiactivePetSkill(type)){
						var useItem:UseItem=new UseItem(-1,String(type),null);	
						useItem.mouseEnabled=true;
						useItem.Id=type;
						useItem.addEventListener(MouseEvent.MOUSE_DOWN,onMosueDowmHandler);
						useItem.addEventListener(MouseEvent.CLICK,onUseItemClick);
						useItem.addEventListener(DropEvent.DRAG_THREW,onDrawThrewHandler);
						useItem.addEventListener(DropEvent.DRAG_DROPPED,onDragedHandler);
						useItem.x=2;
						useItem.y=2;
						useItem.name="keyF_7";
						dicF[7]=useItem;
						mainSence.mcQuickBar1["quickf_7"].addChild(dicF[7]);
						CooldownController.getInstance().registerCDItem(type, useItem);
						this.getPetSkillCdSend();
					}
					break;
				}
			}
		}
		
		/**
		 *  向服务器发送消息，请求宠物技能CD
		 * 
		 */		
		protected function getPetSkillCdSend():void{
		
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(GameCommonData.Player.Role.UsingPet.Id);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(289);							//进入地图
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**
		 * 给技能栏快捷键添加滤镜
		 **/ 		
		 public function addQuickFlow(note:Object):void
		 {
		 	var key:int = note.keyValue;
		 	var isCtrlKey:Boolean = note.isCtrlKey;
		 	switch(key)
		 	{
		 		case 49: //快捷键1
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_0 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_0 as MovieClip);
		 			}
		 			break;
		 		case 50: //快捷键2
		 			if(isCtrlKey)
		 			{
			 			addFlow(this.mainSence.mcQuickBar1.quickf_1 as MovieClip);
		 			}
		 			else{
		 				addFlow(this.mainSence.mcQuickBar0.quick_1 as MovieClip);
		 			}
		 			break;
		 		case 51: //快捷键3
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_2 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_2 as MovieClip);
		 			}	
		 			break;
		 		case 52: //快捷键4
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_3 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_3 as MovieClip);
		 			}
		 			break;
		 		case 53: //快捷键5
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_4 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_4 as MovieClip);
		 			}
		 			break;
		 		case 54: //快捷键6
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_5 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_5 as MovieClip);
		 			}
		 			break;
		 		case 55: //快捷键7
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_6 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_6 as MovieClip);
		 			}
		 			break;
		 		case 56: //快捷键8
		 			if(isCtrlKey)
		 			{
		 				addFlow(this.mainSence.mcQuickBar1.quickf_7 as MovieClip);
		 			}
		 			else{
			 			addFlow(this.mainSence.mcQuickBar0.quick_7 as MovieClip);
		 			}
		 			break;
		 	}
		 }
		 /**
		 * 移除技能快键键滤镜
		 **/ 
		 public function removeQuickFlow(note:Object):void
		 {
		 	var num:int = (this.mainSence.mcQuickBar0 as MovieClip).numChildren;
		 	for(var i:int = 0; i < num; i ++)
		 	{
		 		if((this.mainSence.mcQuickBar0 as MovieClip).getChildAt(i).filters.length)
		 		{
		 			(this.mainSence.mcQuickBar0 as MovieClip).getChildAt(i).filters = null;
		 		}
		 	}
	 		var expandNum:int = (this.mainSence.mcQuickBar1 as MovieClip).numChildren;
	 		for(var j:int = 0; j < expandNum; j ++)
	 		{
	 			if((this.mainSence.mcQuickBar1 as MovieClip).getChildAt(j).filters.length)
	 			{
	 				(this.mainSence.mcQuickBar1 as MovieClip).getChildAt(j).filters = null;
	 			}	
	 		}
		 }
		 
		 /**
		 * 给mc加滤镜
		 **/ 
		 private function addFlow(mc:MovieClip):void
		 {
		 	var gf:GlowFilter = new GlowFilter();
		 	gf.color = 0xffffff;
		 	mc.filters = [gf];
		 }
	}
}