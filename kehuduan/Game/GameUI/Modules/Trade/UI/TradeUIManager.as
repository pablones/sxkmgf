package GameUI.Modules.Trade.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Trade.Data.TradeConstData;
	import GameUI.Modules.Trade.Data.TradeDataProxy;
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	
	public class TradeUIManager
	{
		private var tradePanel:MovieClip 			= null;
		private var petPanel:MovieClip				= null;
		private var moneyPanel:MovieClip 			= null;
		private var tradeDataProxy:TradeDataProxy 	= null;
		
		public function TradeUIManager(tradePanel:MovieClip, petPanel:MovieClip, moneyPanel:MovieClip, tradeDataProxy:TradeDataProxy):void
		{
			this.tradePanel = tradePanel;
			this.petPanel = petPanel;
			this.moneyPanel = moneyPanel;
			this.tradeDataProxy = tradeDataProxy;
			
			init();
		}
		
		private function init():void
		{
			tradePanel.txtRoleName_op.text		  = "";
			tradePanel.mcMoney_op.txtMoney.text   = "";
			tradePanel.mcMoney_self.txtMoney.text = ""; 
			
			tradePanel.txtState_op.text   = GameCommonData.wordDic[ "mod_tra_med_tra_han_5" ];           //尚未锁定
			tradePanel.txtState_self.text = GameCommonData.wordDic[ "mod_tra_med_tra_han_5" ];           //尚未锁定
			tradePanel.txtRoleName_op.mouseEnabled  = false;
			tradePanel.mcMoney_op.mouseEnabled 		= false;
			tradePanel.mcMoney_self.mouseEnabled 	= false;
			tradePanel.txtState_op.mouseEnabled 	= false;
			tradePanel.txtState_self.mouseEnabled 	= false;
			for(var i:int = 0; i < 5; i++) {
				tradePanel["mcGood_op_"+i].txtGoodName.text  	 = "";
				tradePanel["mcGood_"+i+"_self"].txtGoodName.text = "";
				tradePanel["mcGood_op_"+i].mouseEnabled   	 = false;
				tradePanel["mcGood_"+i+"_self"].mouseEnabled = false;
			}
			moneyPanel.txtJin.restrict   = '0-9';
			moneyPanel.txtYin.restrict   = '0-9';
			moneyPanel.txtTong.restrict  = '0-9';
			tradePanel.mcGreenRect.visible        = false;
			tradePanel.mcGreenRect.mouseEnabled   = false;
			tradePanel.mcGreenRectOp.visible      = false;
			tradePanel.mcGreenRectOp.mouseEnabled = false;
			tradePanel.btnSure.visible 			  = false;
			tradePanel.btnRemovePet.visible 	  = false;
			petPanel.btnCancel.visible			  = false;
			petPanel.txtCancel.visible 			  = false;
			tradePanel.btnLock.visible 			  = true;
			tradePanel.btnInputMoney.visible 	  = true;
		}
		
		/** 界面上显示钱 0=对方 1=自己*/
		public function showMoney(type:uint):void {
			if(type == 0) {
				var moneyArr:Array = getMoney(tradeDataProxy.moneyOp);
				var moneyStr:String = ""; 
				if(moneyArr[0] != 0) {
					moneyStr = moneyArr[0] + "\\ce" + moneyArr[1] + "\\cs" + moneyArr[2] + "\\cc";
				} else if(moneyArr[1] != 0) {
					moneyStr = moneyArr[1] + "\\cs" + moneyArr[2] + "\\cc";
				} else {
					moneyStr = moneyArr[2] + "\\cc";
				}
				tradePanel.mcMoney_op.txtMoney.text = moneyStr;
				ShowMoney.ShowIcon(tradePanel.mcMoney_op, tradePanel.mcMoney_op.txtMoney, true);
				tradePanel.mcMoney_op.visible = true;
			} else {
				var moneyArrS:Array = getMoney(tradeDataProxy.moneySelf);
				var moneyStrS:String = ""; 
				if(moneyArrS[0] != 0) {
					moneyStrS = moneyArrS[0] + "\\ce" + moneyArrS[1] + "\\cs" + moneyArrS[2] + "\\cc";
				} else if(moneyArrS[1] != 0) {
					moneyStrS = moneyArrS[1] + "\\cs" + moneyArrS[2] + "\\cc";
				} else {
					moneyStrS = moneyArrS[2] + "\\cc";
				}
				tradePanel.mcMoney_self.txtMoney.text = moneyStrS;
				ShowMoney.ShowIcon(tradePanel.mcMoney_self, tradePanel.mcMoney_self.txtMoney, true);
				tradePanel.mcMoney_self.visible = true;
			}
		}
		
		private function getDeeply(obj:Object):Object
		{
			var ba:ByteArray = new ByteArray();
			ba.writeObject(obj);
			ba.position = 0;
			return ba.readObject();
		}
		
		/** 添加物品 0=对方加物品，1=自己加物品 */
		public function addItem(type:int, good:Object):void
		{
			var good1:Object = getDeeply(good);
			var typeMul:uint = good1.type / 1000;
			if(typeMul == 301 || typeMul == 311 || typeMul == 321) {			//大血大蓝
				good1.noUse  = good1.amount;
				good1.maxUse = good1.maxAmount;
			}
			if(typeMul == 351){   //元宝票，经验票之类的
				good1.amountMoney = good1.amount;
				good1.amount = 1;
			}
			if(type == 0) {
				var index:int = -1;					
				for(var ii:int = 0; ii < TradeConstData.goodOpList.length; ii++) {
					if(!TradeConstData.goodOpList[ii]) {
						index = ii;
						break;
					}
				}
				if(index > -1) {
					TradeConstData.goodOpList[index] = good1;
					TradeConstData.goodOpList[index].userName = TradeConstData.opName;
					var parent:MovieClip = tradePanel["mcOpPhoto_"+index];					
					var useItemOp:UseItem = new UseItem(index, good1.type, parent);
					parent.addChild(useItemOp);
					useItemOp.Id  = good1.id;
					if(good1.type < 300000 || typeMul == 301 || typeMul == 311 || typeMul == 321 || typeMul == 351) {
						useItemOp.Num = 1;
					} else {
						useItemOp.Num = good1.amount;	//数量;
					}
					useItemOp.x   = 2;
					useItemOp.y   = 2;
					TradeConstData.GridUnitListOp[index].Item = useItemOp;
					tradePanel["mcGood_op_"+index].txtGoodName.text = UIConstData.getItem(good1.type).Name;		 			
					(TradeConstData.GridUnitListOp[index] as GridUnit).Grid.mouseEnabled = true;
				}
			} else {
				var indexS:int = -1;					
				for(var i:int = 0; i < TradeConstData.goodSelfList.length; i++) {
					if(!TradeConstData.goodSelfList[i]) {
						indexS = i;
						break;
					}
				}
				if(indexS > -1) {
					TradeConstData.goodSelfList[indexS] = good1;
					var parentS:MovieClip = tradePanel["mcPhoto_"+indexS];					
					
					var useItem:UseItem = new UseItem(indexS, good1.type, parentS);
					parentS.addChild(useItem);
					useItem.Id  = good1.id;
					if(good1.type < 300000 || typeMul == 301 || typeMul == 311 || typeMul == 321 || typeMul == 351) {
						useItem.Num = 1;
					} else {
						useItem.Num = good1.amount;	//数量;
					}
					useItem.x   = 2;
					useItem.y   = 2;
					TradeConstData.GridUnitList[indexS].Item = useItem;
					
					tradePanel["mcGood_"+indexS+"_self"].txtGoodName.text = UIConstData.getItem(good1.type).Name; 
					(TradeConstData.GridUnitList[indexS] as GridUnit).Grid.mouseEnabled = true;
				}
				tradeDataProxy.removeAllFrames();
			}
		}
		
		/** 减物品 0=对方减物品，1=自己减物品 */
		public function delItem(type:int, good:Object):void
		{
			if(type == 0) {
				for(var i:int = 0; i < TradeConstData.goodOpList.length; i++) {
					if(TradeConstData.goodOpList[i] && TradeConstData.goodOpList[i].id == good.id) {
						TradeConstData.goodOpList.splice(i,1);
						TradeConstData.goodOpList[4] = undefined;
						break;
					}
				}
				for(var k:int = 0; k < 5; k++) {
					tradePanel["mcGood_op_"+k].txtGoodName.text = "";
				}
				for(var j:int; j < TradeConstData.goodOpList.length; j++) {
					if(TradeConstData.goodOpList[j]) tradePanel["mcGood_op_"+j].txtGoodName.text = UIConstData.getItem(TradeConstData.goodOpList[j].type).Name;
				}
				removeAllItemOp();
				showItemsOp(TradeConstData.goodOpList);
			} else {
				for(var x:int = 0; x < TradeConstData.goodSelfList.length; x++) {
					if(TradeConstData.goodSelfList[x] && TradeConstData.goodSelfList[x].id == good.id) {
						TradeConstData.goodSelfList.splice(x,1);
						TradeConstData.goodSelfList[4] = undefined;
						break;
					}
				}
				for(var z:int = 0; z < 5; z++) {
					tradePanel["mcGood_"+z+"_self"].txtGoodName.text = "";
				}
				for(var y:int; y < TradeConstData.goodSelfList.length; y++) {
					if(TradeConstData.goodSelfList[y]) tradePanel["mcGood_"+y+"_self"].txtGoodName.text = UIConstData.getItem(TradeConstData.goodSelfList[y].type).Name;
				}
				removeAllItem();
				tradeDataProxy.removeAllFrames();
				showItems(TradeConstData.goodSelfList);
			}
		}
		
		public function removeAllItem():void
		{
			for(var j:int; j < 5; j++) {
				var count:int = tradePanel["mcPhoto_"+j].numChildren-1;
				while(count) {
					if(tradePanel["mcPhoto_"+j].getChildAt(count) is ItemBase) {
						var item:ItemBase = tradePanel["mcPhoto_"+j].getChildAt(count) as ItemBase;
						tradePanel["mcPhoto_"+j].removeChild(item);
						item = null;
					}
					count--;
				}
			}
			for( var i:int = 0; i < 5; i++ ) 
			{
				TradeConstData.GridUnitList[i].Item = null;
				TradeConstData.GridUnitList[i].IsUsed = false;
			}
		}
		
		public function removeAllItemOp():void
		{
			for(var j:int; j < 5; j++) {
				var count:int = tradePanel["mcOpPhoto_"+j].numChildren-1;
				while(count) {
					if(tradePanel["mcOpPhoto_"+j].getChildAt(count) is ItemBase) {
						var item:ItemBase = tradePanel["mcOpPhoto_"+j].getChildAt(count) as ItemBase;
						tradePanel["mcOpPhoto_"+j].removeChild(item);
						item = null;
					}
					count--;
				}
			}
			for( var i:int = 0; i < 5; i++ ) 
			{
				TradeConstData.GridUnitListOp[i].Item = null;
				TradeConstData.GridUnitListOp[i].IsUsed = false;
			}
		}
		
		/**  初始化物品 */
		public function showItems(list:Array):void
		{
			for(var i:int = 0; i<list.length; i++)
			{
				if(list[i] == undefined) {
					continue;
				}
				var useItem:UseItem = new UseItem(i, list[i].type, tradePanel["mcPhoto_"+i]);
				var typeMul:uint = list[i].type / 1000;
				if(list[i].type < 300000 || typeMul == 301 || typeMul == 311 || typeMul == 321 || typeMul == 351) {			//大血大蓝
					useItem.Num = 1;
				} else {
					useItem.Num = list[i].amount;
				}
				//list[i].type < 300000 ? useItem.Num = 1 : 
				useItem.x = 2;
				useItem.y = 2;
				useItem.Id = list[i].id;
				useItem.IsBind = list[i].isBind;
				useItem.Type = list[i].type;
				TradeConstData.GridUnitList[i].Item = useItem;
				TradeConstData.GridUnitList[i].IsUsed = true;
				TradeConstData.GridUnitList[i].Grid.addChild(useItem);
			}
		}
		
		public function showItemsOp(list:Array):void
		{
			for(var i:int = 0; i<list.length; i++)
			{
				if(list[i] == undefined) {
					continue;
				}
				var useItem:UseItem = new UseItem(i, list[i].type, tradePanel["mcOpPhoto_"+i]);
				var typeMul:uint = list[i].type / 1000;
				if(list[i].type < 300000 || typeMul == 301 || typeMul == 311 || typeMul == 321 || typeMul == 351) {			//大血大蓝
					useItem.Num = 1;
				} else {
					useItem.Num = list[i].amount;
				}
//				list[i].type < 300000 ? useItem.Num = 1 : useItem.Num = list[i].amount;
				useItem.x = 2;
				useItem.y = 2;
				useItem.Id = list[i].id;
				useItem.IsBind = list[i].isBind;
				useItem.Type = list[i].type;
				TradeConstData.GridUnitListOp[i].Item = useItem;
				TradeConstData.GridUnitListOp[i].IsUsed = true;
				TradeConstData.GridUnitListOp[i].Grid.addChild(useItem);
			}
		}
		
		public function initPanel():void
		{
			tradePanel.txtState_op.text = GameCommonData.wordDic[ "mod_tra_med_tra_han_5" ];         //尚未锁定
			tradePanel.txtState_self.text = GameCommonData.wordDic[ "mod_tra_med_tra_han_5" ];       //尚未锁定
			for(var i:int = 0; i < 5; i++) {
				tradePanel["mcGood_op_"+i].txtGoodName.text   = "";
				tradePanel["mcGood_"+i+"_self"].txtGoodName.text = "";
			}
			tradePanel.mcMoney_op.txtMoney.text   = "0\\cc";
			tradePanel.mcMoney_self.txtMoney.text = "0\\cc";
			ShowMoney.ShowIcon(tradePanel.mcMoney_op, tradePanel.mcMoney_op.txtMoney, true);
			ShowMoney.ShowIcon(tradePanel.mcMoney_self, tradePanel.mcMoney_self.txtMoney, true);
			tradePanel.mcGreenRect.visible 	  = false;
			tradePanel.mcGreenRectOp.visible  = false;
			tradePanel.btnLock.visible		  = true;
			tradePanel.btnSure.visible 		  = false;
			tradePanel.btnInputMoney.visible  = true;
			tradePanel.btnRemovePet.visible   = false;
			petPanel.btnPetChose.visible	  = true;
		}
				
		public function getMoney(money:uint):Array
		{	
			var arr:Array = [];
			var jin:uint  = 0;
			var yin:uint  = 0;
			var tong:uint = 0;
			jin = money / 10000;
			yin = (money - jin * 10000) / 100;
			tong = money %100;
			arr.push(jin);
			arr.push(yin);
			arr.push(tong);
			return arr;
		}
		
		/** 获取自己交易栏中当前宠物数量 */
		public function getPetCountSelf():uint
		{
			var count:uint = 0;
			for(var key:Object in TradeConstData.petSelfDic) {
				count++;
			}
			return count;
		}
		
		/** 获取自己的宠物选择列表中当前宠物数量 */
		public function getPetCountList():uint
		{
			var count:uint = 0;
			for(var key:Object in TradeConstData.petListSelfDic) {
				count++;
			}
			return count;
		} 
		
		/** 获取对方交易栏中当前宠物数量 */
		public function getPetCountOp():uint
		{
			var count:uint = 0;
			for(var key:Object in TradeConstData.petOpDic) {
				count++;
			}
			return count;
		}
		
	}
}
