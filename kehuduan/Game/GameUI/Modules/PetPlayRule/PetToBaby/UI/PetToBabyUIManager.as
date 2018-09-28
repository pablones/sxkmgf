package GameUI.Modules.PetPlayRule.PetToBaby.UI
{
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetToBaby.Data.PetToBabyConstData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PetToBabyUIManager
	{
		private var panelView:MovieClip;
		private var petPhoto:FaceItem;
		
		public function PetToBabyUIManager(view:MovieClip)
		{
			this.panelView = view.mianView;
			init();
		}
		
		private function init():void
		{
			panelView.mouseEnabled = false;
			panelView.txtGrade.mouseEnabled = false;
			panelView.txtForce.mouseEnabled = false;
			panelView.txtSpiritPower.mouseEnabled = false;
			panelView.txtPhysical.mouseEnabled = false;
			
			panelView.txtConstant.mouseEnabled = false;
			panelView.txtMagic.mouseEnabled = false;
			
			panelView.txtPetName.mouseEnabled = false;
			panelView.txtForceInit.mouseEnabled = false;
			panelView.txtSpiritPowerInit.mouseEnabled = false;
			panelView.txtPhysicalInit.mouseEnabled = false;
			panelView.txtConstantInit.mouseEnabled = false;
			panelView.txtMagicInit.mouseEnabled = false;
			
			panelView.mcForce.mouseEnabled = false;
			panelView.mcSpiritPower.mouseEnabled = false;
			panelView.mcPhysical.mouseEnabled = false;
			panelView.mcConstant.mouseEnabled = false;
			panelView.mcMagic.mouseEnabled = false;
			
			clearData();
//			updatePetNum();
		}
		
		/** 显示钱 0-需要花费，1-携带碎银，2-携带银两 */
		public function showMoney(type:int, money:int = 0):void
		{
			switch(type) {
				case 0:
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(PetToBabyConstData.breedCost, ["\\se","\\ss","\\sc"]);
					panelView.mcCost.txtMoney.text = moneyStrNeed;
					ShowMoney.ShowIcon(panelView.mcCost, panelView.mcCost.txtMoney, true);
					break;
				case 1:
					var moneyStrSuiYin:String = UIUtils.getMoneyStandInfo(Number(GameCommonData.Player.Role.UnBindMoney), ["\\se","\\ss","\\sc"]);
					panelView.mcSuiYin.txtMoney.text = moneyStrSuiYin;
					ShowMoney.ShowIcon(panelView.mcSuiYin, panelView.mcSuiYin.txtMoney, true);
					break;
				case 2:
					var moneyStrYinLiang:String = UIUtils.getMoneyStandInfo(Number(GameCommonData.Player.Role.BindMoney), ["\\ce","\\cs","\\cc"]);
					panelView.mcYinLiang.txtMoney.text = moneyStrYinLiang;
					ShowMoney.ShowIcon(panelView.mcYinLiang, panelView.mcYinLiang.txtMoney, true);
					break;
			}
		}
		
		/** 清除所有数据 */
		public function clearData():void
		{
			panelView.txtGrade.text = "";
			panelView.txtForce.text = "";
			panelView.txtSpiritPower.text = "";
			panelView.txtPhysical.text = "";
			
			panelView.txtConstant.text = "";
			panelView.txtMagic.text = "";
			
			panelView.txtPetName.text = "";
			panelView.txtForceInit.text = "";
			panelView.txtSpiritPowerInit.text = "";
			panelView.txtPhysicalInit.text = "";
			panelView.txtConstantInit.text = "";
			panelView.txtMagicInit.text = "";
			
			panelView.mcForce.width = 0;
			panelView.mcSpiritPower.width = 0;
			panelView.mcPhysical.width = 0;
			panelView.mcConstant.width = 0;
			panelView.mcMagic.width = 0;
			
			removePetPhoto();
		}
		
		/** 添加宠物头像 */
		public function addPetPhoto():void
		{
			removePetPhoto();
			var faceType:int = PetRuleCommonData.selectedPet.FaceType;
			if(PetRuleCommonData.selectedPet.Savvy >= 7) 
			{
				faceType = PetPropConstData.getFaceType(faceType);
			}
			
			var faceStr:String = faceType.toString();
			if( PetRuleCommonData.selectedPet.Type > 1 )
			{
				var petV:XML = PlayerSkinsController.GetPetV( PetRuleCommonData.selectedPet.ClassId.toString() , PetRuleCommonData.selectedPet.Type - 1 ); 
				if( petV != null)
				{
					faceStr = petV.@Face;
				}
			}
			
			petPhoto = new FaceItem(faceStr, panelView.mcPhotoPet, "EnemyIcon");
			petPhoto.offsetPoint = new Point(0,0);
			panelView.mcPhotoPet.addChild(petPhoto); 
		}
		
		private function removePetPhoto():void
		{
			if(petPhoto && panelView.mcPhotoPet.contains(petPhoto)) {
				panelView.mcPhotoPet.removeChild(petPhoto);
				petPhoto = null;
			}
		}
		
		/** 显示宠物快照数据 */
		public function showData():void
		{
			addPetPhoto();
			
			var pet:GamePetRole = PetRuleCommonData.selectedPet;
			
			panelView.txtPetName.text = pet.PetName;
			panelView.txtGrade.htmlText = PetRuleCommonData.getPetGrade(pet.Grade);
			
			panelView.txtForce.text = pet.Force.toString();
			panelView.txtSpiritPower.text = pet.SpiritPower.toString();
			panelView.txtPhysical.text = pet.Physical.toString();
			panelView.txtConstant.text = pet.Constant.toString();
			panelView.txtMagic.text = pet.Magic.toString();
			
			setMclenght(pet);
			
//			switch(PetToBabyConstData.toBabyPet.Type) {
//				case 0:
//					panelView.txtType.text = "野生";
//					break;
//				case 1:
//					panelView.txtType.text = "宝宝";
//					break;
//				default:
//					panelView.txtType.text = "二代";
//					break;
//			}
//			panelView.txtSex.text = (PetToBabyConstData.toBabyPet.Sex == 0) ? "雄性" : "雌性";
//			panelView.txtTakeLevel.htmlText = '<font color="#00ff00">' + PetToBabyConstData.toBabyPet.TakeLevel + '级</font><font color="#ffffff">可携带</font>';
//			switch(PetToBabyConstData.toBabyPet.Character) {
//				case 0:
//					panelView.txtCharacter.text = "勇猛";
//					break;
//				case 1:
//					panelView.txtCharacter.text = "胆小";
//					break;
//				case 2:
//					panelView.txtCharacter.text = "精明";
//					break;
//				case 3:
//					panelView.txtCharacter.text = "谨慎";
//					break;
//				case 4:
//					panelView.txtCharacter.text = "忠诚";
//					break;
//			}
		}
		
		/** 设置属性条长度  len-当前值，max-最大值，type-类型(0力量 1灵力 2体质 3定力 4身法) */
		private function setMclenght(petRole:GamePetRole):void
		{
			var max:Number = petRole.ForceInitMax;
			var now:Number = petRole.ForceInit;
			var width:Number = (now/max) * 132;
			if(width > 132) {
				panelView.mcForce.width = 132;
			} else if(width < 1){
				panelView.mcForce.width = 1;
			} else {
				panelView.mcForce.width = width;
			}
			panelView.txtForceInit.htmlText = "<font color='#E2CCA5'>" + now + "</font>/<font color='#00ff00'>" + max + "</font>";
			
			max = petRole.SpiritPowerInitMax;
			now = petRole.SpiritPowerInit;
			width = (now/max) * 132;
			if(width > 132) {
				panelView.mcSpiritPower.width = 132;
			} else if(width < 1){
				panelView.mcSpiritPower.width = 1;
			} else {
				panelView.mcSpiritPower.width = width;
			}
			panelView.txtSpiritPowerInit.htmlText = "<font color='#E2CCA5'>" + now + "</font>/<font color='#00ff00'>" + max + "</font>";
			
			max = petRole.PhysicalInitMax;
			now = petRole.PhysicalInit;
			width = (now/max) * 132;
			if(width > 132) {
				panelView.mcPhysical.width = 132;
			} else if(width < 1){
				panelView.mcPhysical.width = 1;
			} else {
				panelView.mcPhysical.width = width;
			}
			panelView.txtPhysicalInit.htmlText = "<font color='#E2CCA5'>" + now + "</font>/<font color='#00ff00'>" + max + "</font>";
			
			max = petRole.ConstantInitMax;
			now = petRole.ConstantInit;
			width = (now/max) * 132;
			if(width > 132) {
				panelView.mcConstant.width = 132;
			} else if(width < 1){
				panelView.mcConstant.width = 1;
			} else {
				panelView.mcConstant.width = width;
			}
			panelView.txtConstantInit.htmlText = "<font color='#E2CCA5'>" + now + "</font>/<font color='#00ff00'>" + max + "</font>";
			
			max = petRole.MagicInitMax;
			now = petRole.MagicInit;
			width = (now/max) * 132;
			if(width > 132) {
				panelView.mcMagic.width = 132;
			} else if(width < 1){
				panelView.mcMagic.width = 1;
			} else {
				panelView.mcMagic.width = width;
			}
			panelView.txtMagicInit.htmlText = "<font color='#E2CCA5'>" + now + "</font>/<font color='#00ff00'>" + max + "</font>";
		}
		
		
//		public function updatePetNum():void
//		{
//			var count:int;
//			for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
//				count++;
//			}
//			panelView.txtPetCount.text = count + "/" + PetPropConstData.petBagNum;
//		}
			
	}
}