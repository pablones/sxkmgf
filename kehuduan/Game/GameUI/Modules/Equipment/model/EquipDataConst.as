package GameUI.Modules.Equipment.model
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Equipment.command.EquipCommandList;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UICore.UIFacade;
	import GameUI.View.items.UseItem;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.Shape;
	import flash.utils.Dictionary;
	
	public class EquipDataConst
	{
		private static var _instance:EquipDataConst;
		public static const isFourthStilettoOpen:Boolean = true;	//第四个孔开发
		private var selectedMask:Shape;
		private var dic:Dictionary=new Dictionary();
		private var feeDic:Dictionary=new Dictionary();
		private var strengDic:Dictionary=new Dictionary();
		public var pageDic:Dictionary=new Dictionary();
		/**宝石雕琢需要的金钱*/
		public var decorateMoneyNeeded:Array;	
		
		
		/** 材料辅助栏的锁定列表  */
		public var lockItems:Dictionary=new Dictionary();
		
		/**  面板打开/关闭状态 */
		private var panelStaus:uint=0;
		
		
		public function EquipDataConst()
		{
			
			/* this.pageDic[0]='<font color = "#32CCFF">强化装备可以增加装备的基础属性数值<br><font color = "#e2cca5">使用的强化符数量越多 成功率越高<br></font><font color = "#00FF00">装备强化等级为</font><font color = "#FF9900">0-2 </font><font color = "#00FF00">使用<font color = "#FF9900">1级强化符</font><br><font color = "#00FF00">装备强化等级为</font><font color = "#FF9900">3-5 </font><font color = "#00FF00">使用<font color = "#FF9900">2级强化符</font><br><font color = "#00FF00">装备强化等级为</font><font color = "#FF9900 ">6-9 </font><font color = "#00FF00">使用<font color = "#FF9900">3级强化符</font></font>';
			this.pageDic[1]='<font color = "#32CCFF">提升星级可以增加装备的附加属性数值<br><font color = "#e2cca5">使用的升星符数量越多 成功率越高<br></font><font color = "#00FF00">装备星级为</font><font color = "#FF9900">0-2 </font><font color = "#00FF00">使用<font color = "#FF9900">1级升星符</font><br><font color = "#00FF00">装备星级为</font><font color = "#FF9900">3-5 </font><font color = "#00FF00">使用<font color = "#FF9900">2级升星符</font><br><font color = "#00FF00">装备星级为</font><font color = "#FF9900">6-9 </font><font color = "#00FF00">使用<font  color = "#FF9900">3级升星符</font></font>';
			this.pageDic[2]='<font color = "#32CCFF">将低级宝石合成为高级宝石<br><font color = "#e2cca5">合成宝石数量越多 成功率越高 最少</font><font color = "#FF9900">3颗</font><font color = "#e2cca5">最多</font><font color = "#FF9900">5颗</font> <br><font color = "#00FF00">合成</font><font color = "#FF9900">4级或以下</font><font color = "#00FF00">宝石 使用<font color = "#FF9900">低级合成符</font><br><font color = "#00FF00">合成</font><font color = "#FF9900">5级或以上</font><font color = "#00FF00">宝石 使用<font color = "#FF9900">高级合成符</font><br><font color = "#00FF00">使用宝石合成符 提高</font><font color = "#FF9900">25%</font><font color = "#00FF00">成功率</font></font>';
			this.pageDic[3]='<font color = "#32CCFF">装备打孔后可镶嵌宝石<br><font color = "#e2cca5">每件装备最多可以打</font><font color = "#FF9900">3个</font><font color = "#e2cca5">孔 打孔有几率</font><font color = "#FF9900">失败</font> <br><font color = "#00FF00">不同等级的装备需要不同的打孔道具</font></font>';
			this.pageDic[4]='<font color = "#32CCFF">镶嵌宝石可以提高装备属性<br><font color = "#e2cca5">每件装备最多镶嵌</font><font color = "#FF9900">3颗</font><font color = "#e2cca5">宝石 同种类宝石</font><font color = "#FF9900">不可重复镶嵌</font> <br><font color="#00ff00">攻击类装备：</font><font color="#ff9900">武器 护腕 戒指 饰品  </font><font color="#00ff00">只可镶嵌</font><font color="#ff9900">水晶</font><br><font color="#00ff00">防御类装备：</font><font color="#ff9900">衣服 头盔 护肩 手套 腰带 鞋子  </font><font color="#00ff00">只可 </font><br><font color="#00ff00">镶嵌</font><font color="#ff9900">宝石</font><br><font color="#ff9900">项链</font><font color="#00ff00">可以自由镶嵌水晶和宝石</font>';
			this.pageDic[5]='<font color = "#32CCFF">可将镶嵌在装备上的宝石取下再次使用<br><font color = "#e2cca5">摘取宝石</font><font color = "#FF9900">不会失败</font><font color = "#e2cca5"> 摘取下的宝石为</font><font color = "#FF9900">绑定</font> <br><font color = "#00FF00">摘取宝石需要</font><font color = "#FF9900">摘取宝符</font></font>';
			this.pageDic[6]='<font color = "#32CCFF">魂印装备死亡后不会掉落<br><font color = "#e2cca5">魂印后提高装备的</font><font color = "#FF9900">基本属性10%</font><br><font color = "#FF9900">蓝色或以下 </font><font color = "#00FF00">装备 需要</font><font color = "#FF9900">1个</font><font color = "#00FF00">魂印宝珠</font><br><font color = "#FF9900">紫色</font><font color = "#00FF00">装备 需要</font><font color = "#FF9900">2个</font><font color = "#00FF00">魂印宝珠</font><br><font color = "#FF9900">橙色</font><font color = "#00FF00">装备 需要</font><font color = "#FF9900">3个</font><font color = "#00FF00">魂印宝珠</font></font>';
			this.pageDic[7]='<font color = "#32CCFF">可大幅提升攻击类水晶的属性<br><font color = "#e2cca5">雕琢后提升宝石的</font><font color = "#FF9900">基本属性100%</font><br><font color = "#00FF00">宝石雕琢需要消耗1个雕琢符 100%成功不会失败</font>';
			 */
			 
			this.pageDic[0]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_1" ];
			this.pageDic[1]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_2" ];
			this.pageDic[2]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_3" ];
//			this.pageDic[3]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_4" ];
			this.pageDic[3]='<font color = "#32CCFF">装备打孔后可以镶嵌宝石<br><font color = "#e2cca5">每件装备最多可以打</font><font color = "#FF9900">3个</font><font color = "#e2cca5">普通孔 打孔有一定几率</font><font color = "#FF9900">失败</font> <br>当<font color = "#FF9900">3个</font>孔全部开启后会激活第<font color = "#FF9900">4个</font>混沌之孔<br>混沌之孔打孔成功率100%<br><font color = "#00FF00">混沌之孔内镶嵌的宝石不会与其他3孔互相排斥</font></font>';
			this.pageDic[4]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_5" ];
			this.pageDic[5]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_6" ];
			this.pageDic[6]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_7" ];
			this.pageDic[7]=GameCommonData.wordDic[ "mod_equ_mod_equ_pageDic_8" ];
			
			 
			//升星费用
			feeDic[0]={fee:'50\\sc',realFee:5000};
			feeDic[1]={fee:'50\\sc',realFee:5000};
			feeDic[2]={fee:'50\\sc',realFee:5000};
			feeDic[3]={fee:'1\\ss',realFee:10000};     
			feeDic[4]={fee:'1\\ss',realFee:10000};
			feeDic[5]={fee:'1\\ss',realFee:10000};
			feeDic[6]={fee:'1\\ss50\\sc',realFee:15000};
			feeDic[7]={fee:'1\\ss50\\sc',realFee:15000};
			feeDic[8]={fee:'1\\ss50\\sc',realFee:15000};
			feeDic[9]={fee:'1\\ss50\\sc',realFee:15000};
			feeDic[10]={fee:'0\\sc',realFee:0};
			
			//强化费用
			strengDic[0]={fee:'80\\sc',realFee:8000};
			strengDic[1]={fee:'80\\sc',realFee:8000};
			strengDic[2]={fee:'80\\sc',realFee:8000};
			strengDic[3]={fee:'1\\ss50\\sc',realFee:15000};
			strengDic[4]={fee:'1\\ss50\\sc',realFee:15000};
			strengDic[5]={fee:'1\\ss50\\sc',realFee:15000};
			strengDic[6]={fee:'2\\ss50\\sc',realFee:25000};
			strengDic[7]={fee:'2\\ss50\\sc',realFee:25000};
			strengDic[8]={fee:'2\\ss50\\sc',realFee:25000};
			strengDic[9]={fee:'2\\ss50\\sc',realFee:25000};
			strengDic[10]={fee:'0\\sc',realFee:0};
			
			
			
			feeDic[10]={fee:'3\\se',realFee:30000}; //打第一个孔
			feeDic[11]={fee:'50\\ss',realFee:5000};
			feeDic[12]={fee:'50\\ss',realFee:5000};		
			feeDic[13]={fee:'6\\se',realFee:60000};//打第二个孔	
			feeDic[14]={fee:'9\\se',realFee:90000};//打第三个孔	
			feeDic[15]={fee:'2\\se',realFee:20000};//打第三个孔
			
			decorateMoneyNeeded = [];
			for(var i:int = 0; i < 10; i++)
			{
				if(i == 0)
				{
					decorateMoneyNeeded.push(500000);
					continue;
				}
				decorateMoneyNeeded.push(i*50000);
			}
		}
		
		
		
		public static function getInstance():EquipDataConst{
			if(_instance==null)_instance=new EquipDataConst();
			return _instance;
		}
		
		public function getDesByLevel(level:uint):String{
			return dic[level];
		}
		
		/**
		 *  
		 * @param level  10:打孔费用（打第一个） 11：镶嵌  12：取出  13：打第二个孔  14：打第三个孔
		 * @return 
		 * 
		 */		
		public function getFeeDesByLevel(level:uint):String{
			return feeDic[level].fee;
		}
		
		public function getFeeDesMoney(level:uint):String{
			return feeDic[level].realFee;
		}
		
		/**
		 * 获取装备强化的费用 
		 * @param level
		 * @return 
		 * 
		 */		
		public function getStrengFeeByLevel(level:uint):String{
			return strengDic[level].fee;
		}
		
		/**
		 *  获取强化所需的金钱数
		 * @param value
		 * @return 
		 * 
		 */		
		public function getStrengFee(value:uint):uint{
			return strengDic[value].realFee;
		}
		
		/**
		 * 获取升星所需的金钱数
		 * @param value
		 * @return 
		 * 
		 */		
		public function getAddStarFee(value:uint):uint{
			return feeDic[value].realFee;
		}
		
		/**
		 * 计算成功率 
		 * @param level :装备等级
		 * @param storeNum ：强化符个数
		 * 
		 */		
		public function getSuccessByStoreLevel(level:uint,storeNum:uint):Number{
			switch (level){
				case 0:
					return (100/4)*storeNum;
				case 1:
					return (84/4)*storeNum;
				case 2:
					return (69/4)*storeNum;
				case 3:
					return (55/4)*storeNum;
				case 4:
					return (40/4)*storeNum;
				case 5:
					return (30/4)*storeNum;
				case 6:
					return (20/4)*storeNum;
				case 7:
					return (10/4)*storeNum;
				case 8:
					return (2/4)*storeNum;
				case 9:
					return (1/4)*storeNum;
			}
			return 0;
		}	
		
		/**
		 * 失败后是否降级 
		 * @param level ：
		 * @return  ：true :是
		 * 
		 */		
		public function isToReset(level:uint):Boolean{
			if(level==5 || level==6 || level==8 || level==9){
				return true;
			}else{
				return false;
			}
				
		}
			
		/**
		 *  
		 * @param level ：装备等级 
		 * @return ：所需宝石等级
		 * 
		 */		
		public function getNeedStoreLevel(level:uint):uint{
			switch (level){
				case 0:
				case 1:
				case 2:
					return 1;
					break;
				case 3:
				case 4:
				case 5:
					return 2;
					break;
				case 6:
				case 7:
				case 8:
				case 9:
					return 3;
					break;	
			}
			return 0;
		}
		
		
		/**
		 * 获取升星石的等级 
		 * @param type ：升星石的类型
		 * @return 
		 * 
		 */		
		public function getUpStarLevel(type:uint):uint{
			if(type==610012){    //一级升星石
				return 1;
			}
			if(type==610013){    //二级升星石
				return 2;
			}
			if(type==610014){    //三级升星石
				return 3;
			}
			if(type==610015){    //四级升星石
				return 4;
			}
			return 0;	
		}
		
		
		/**
		 * 获取强化石的等级 
		 * @param type
		 * @return 
		 * 
		 */		
		public function getStrengenStoreLevel(type:uint):uint{
			if(type==610020){    //一级强化石
				return 1;
			}
			if(type==610021){    //二级强化石
				return 2;
			}
			if(type==610022){    //三级强化石
				return 3;
			}
			if(type==610023){    //四级强化石
				return 4;
			}
			return 0;	
		}
		
		
		/**
		 * 检查宝石类型与是否已经镶嵌了该种宝石 
		 * @param eId 装备ID
		 * @param sType ：宝石类型
		 * @return 
		 * 
		 */		
		public function isRightS(useItem:UseItem,sType:uint,isFourthStiletto:Boolean = false):Boolean{
			
			if(!this.isStore(sType)){
				return false;
			}
			if(isEquipAttack(useItem.Type)==0 || isStoneAttack(sType)==0){
				if(isHasTheTypeStone(useItem.Id,sType,isFourthStiletto)){
					return false; 
				}else{
					return true;
				}
			}else if(isEquipAttack(useItem.Type)==isStoneAttack(sType)){
				if(isHasTheTypeStone(useItem.Id,sType,isFourthStiletto)){
					return false; 
				}else{
					return true;
				}
			}
			return false;		
		}
		
		private function isStore(sType:uint):Boolean{
			if(uint(sType/100000)==4 && uint(sType/10000)!=43)return true;
			return false;
		}
		
		 
		/**
		 * 判断该装备是否已经嵌入了该类型的宝石 
		 *  @param eType ：装备 type
		 * 	@param sType : 宝石 type
		 *  @return  true:已经有该种类型的宝石了
		 * 
		 */		 	
		private function isHasTheTypeStone(eId:uint,sType:uint,isFourthStiletto:Boolean = false):Boolean{
				var obj:Object=IntroConst.ItemInfo[eId];
				var stoneList:Array=obj.stoneList;
				
				var realSType:uint=this.getStoneType(sType);  //宝石类型
				var realEType:uint=0;
				for(var i:uint=0;i<3;i++){
					realEType=this.getStoneType(stoneList[i]); //已经镶嵌好的宝石类型
					if(realEType==realSType){
						if(isFourthStiletto){
							return false;
						}else
						{
							return true;
						} 	
					}
				}
				return false;	
		}
		
		/**
		 *  根据宝石的TYpe号返回宝石的类型号
		 * @param sType ：宝石TYpe
		 * @return  自己规化的宝石类型
		 * 
		 */		
		private function getStoneType(sType:uint):uint{
			var realType:uint=Math.floor((sType%100000)/100);
			switch (realType){
				case 101:
				case 102:
					return 1;
				    break;
				case 201:
				case 202:
					return  2;
					break;
				case 209:
				case 210:
				case 211:
				case 212:
				case 213:
					return 3;
					break;
				case 105:
				case 106:
				case 107:
				case 108:
					return 4;
					break;
				case 205:
				case 206:
				case 207:
				case 208:
					return 5;
					break;
				default :
					return realType;					
			}
		}
		
		/**
		 *  装备的攻防属性
		 * @param eType
		 * @return  1:攻 2：防 0：其它
		 * 
		 */	
		private function isEquipAttack(eType:uint):uint{
			var id:uint=Math.floor(eType/10000);
			if(id==14 || id==16 || id==21 || id==22){
				return 1;
			}else if(id==11 || id==12 || id==13 || id==17 || id==18 ||id==19){
				return 2;
			}else{
				return 0;
			}
		}
		
		/**
		 * 宝石的攻防属性 
		 * @param sType
		 * @return  1:攻 2：防 0：其它
		 * 
		 */		
		private function isStoneAttack(sType:uint):uint{
			var id:uint=Math.floor(sType/10000);
			if(id==42){
				return 2;
			}else if(id==41){
				return 1;
			}else{
				return 0;
			}
		}
		
		
		/**
		 * 是否是装备 
		 * @param type
		 * @return 
		 * 
		 */		
		public function isEquip(type:uint):Boolean{
			if(type==144000 || type==142000 || type==141000)return false;
			var typeId:uint=Math.floor(type/10000); 
			if(typeId>10 && typeId<23 && typeId!=20)
			{
				return true;
			}
			else
			{
				return false;
			}	
		}
		
		//是否是可以打孔的装备，魂魄可以
		public function isStilleEquip(type:uint):Boolean{
			if(type==144000 || type==142000 || type==141000)return false;
			var typeId:uint=Math.floor(type/10000); 
			if(typeId>10 && typeId<23 && typeId!=20)
			{
				return true;
			}
			else if ( typeId>=25 && typeId<30 )
			{
				return true;
			}
			else
			{
				return false;
			}	
		}
		
		/**
		 * 强化或升星后是否清空加成率
		 * @param level : 强化后的等级
		 * 
		 */		
		public function isScaleClear(level:uint):Boolean{
			if(level==3 || level==6){
				return true;
			}else{
				return false;
			}
		}
		
		/**
		 * 
		 * @param pageNum   0:打孔 1：镶嵌 2：取出
		 * @param sType		ID
		 * @param eID:	装装备类型
		 * @return 
		 * 
		 */		
		public function isRightStore(pageNum:uint,sType:uint,eType:uint):Boolean{
			var obj:Object=UIConstData.getItem(eType);  
			switch (pageNum){
				
				case 0:
					var level:int=obj.level;
					if(level<=this.getMaxLevel(sType)){
						return true;
					}
					break;
				case 1:
					if(sType==610018){
						return true;
					}
					break;
				case 2:
					if(sType==610019){
						return true;
					}
					break;			
			}
			return false;		
		}
		
		/**
		 * 获得打孔可以给多少级以下的装备打孔
		 * @param type 打孔石类型
		 * @return int:可以打孔的装备的最高等级
		 * 
		 */		
		public function getMaxLevel(type:uint):int{
			if(type>=610000 && type<=610012){
				var value:uint=type%610000+1;
				return value*10;
			}
			return -1;
		}
		
		/**
		 * 判断是否还有未打和孔 
		 * @param useItem
		 * 
		 */		
		public function hasEmptyBore(useItem:UseItem):Boolean{
			var obj:Object=IntroConst.ItemInfo[useItem.Id];
			var stoneArr:Array=obj.stoneList;
			for each(var i:uint in stoneArr){
				if(i==99999){
					return true;
				}
			}
			return false;
		}
		
		/**
		 * 向宝石添加一个被选中标记 
		 * @param stone
		 * 
		 */		
		public function setStoneSelected(stone:DisplayObjectContainer):void{
			var shape:Shape=stone.getChildByName("selectedStoneMask") as Shape;
			if(shape!=null){
				stone.removeChild(shape);
			}
			this.selectedMask=new Shape();
			this.selectedMask.name="selectedStoneMask";
			this.selectedMask.graphics.clear();
			this.selectedMask.graphics.beginFill(0xffff00,0.6);
			this.selectedMask.graphics.drawRect(1,1,34,34);
			this.selectedMask.graphics.endFill();
			stone.addChild(selectedMask);	
		}
		
		
		/**
		 * 移出宝石被选中标记
		 * @param stone
		 * 
		 */		
		public function removeStoneSelected(stone:DisplayObjectContainer):void{
			var shape:Shape=stone.getChildByName("selectedStoneMask") as Shape;
			if(shape!=null){
				stone.removeChild(shape);
			}
		}
		
		/**
		 * 关闭面板 
		 * @param type  1：宝石合成  2：装备升星 3：装备强化 4：装备打孔，镶嵌，取出
		 * 
		 */		
		public function closePanel(type:uint=0):void{
			this.panelStaus=0;
		}
		
		/**
		 * 打开面板 
		 * @param type 1：宝石合成  2：装备升星 3：装备强化 4：装备打孔，镶嵌，取出
		 * 
		 */		
		public function openPanel(type:uint):void{
			if(this.panelStaus!=0){
				switch(this.panelStaus){
					case 1:
						UIFacade.UIFacadeInstance.sendNotification(EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE);
					    break;
					case 2:
						UIFacade.UIFacadeInstance.sendNotification(EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE);
						break; 
					case 3:
						UIFacade.UIFacadeInstance.sendNotification(EquipCommandList.CLOSE_EQ_PANELS_CHANGE_SENCE);
						break; 
					case 4:
						UIFacade.UIFacadeInstance.sendNotification(EquipCommandList.CLOSE_EQUIP_STILETTO_PANEL);
						break; 
				}
			}
			
			this.panelStaus=type;		
		}
		
		
		/**
		 * 判断是否是与强化有关的符 
		 * @param type
		 * 
		 */		
		public function isStrengenItem(type:uint):Boolean{
			if(type>=610020 && type<=610022 ||type==610048){
				return true;
			}else{
				return false;
			}
		}
		
		/**
		 *  判断是否是与升星有关的符
		 * @param type
		 * @return 
		 * 
		 */		
		public function isAddStarItem(type:uint):Boolean{
			if(type>=610012 && type<=610014 ||type==610048){
				return true;
			}else{
				return false;
			}
		}
		
		
		public function isStilettoItem(type:uint):Boolean{
			if(type>=610000 && type<=610011 || type == 610057){	//虚空破碎针
				return true;
			}else{
				return false;
			}	
		}
		
		public function isEnchaseItem(type:uint):Boolean{
			if(type>=410101 && type<=421310 || type==610018){
				return true;
			}else{
				return false;
			}
		}
		
		/**
		 * 是否在摘取宝石栏 
		 * @param type
		 * @return 
		 * 
		 */		
		public function isExtirpateItem(type:uint):Boolean{
			if(type==610019 || type>410101 && type<=421310){
				return true;
			}else{
				return false;
			} 
				
		}
		
		public function isHunYunItem(type:uint):Boolean{
			if(type==610047){
				return true;
			}else{
				return false;
			}
				
		}
		/**
		 *是否是雕琢符 
		 * @param type
		 * @return 
		 * 
		 */		
		public function isDecorateItem(type:uint):Boolean
		{
			var tag:Boolean;
			if(type == 610056)//	雕琢符
			{
				tag = true;
			}
			else if(type >= 410501 && type <= 410510)
			{
				tag = true;
			}
			else if(type >= 410601 && type <= 410610)
			{
				tag = true;
			}
			else if(type >= 410701 && type <= 410710)
			{
				tag = true;
			}
			else if(type >= 410801 && type <= 410810)
			{
				tag = true;
			}
			return tag;
		}
		/**
		 * 背包中是否有雕琢宝符,有并返回一个
		 * @param type
		 * @return 
		 * 
		 */		
		public function getDecorateItem():Object
		{
			var data:Object;
			if(BagData.isHasItem(610056))
			{
				data = BagData.getItemByType(610056);
			}
			return data;
		}
		/**
		 * 是否是和宝石合成相关的物品
		 * @param type
		 * @return 
		 * 
		 */		
		public function isStoneComposeItem(type:uint):Boolean{
			if(type>=410101 && type<=421310 || type==610016 ||type==610017){
				return true;
			}
			return false;
		}
		
	
	}
}