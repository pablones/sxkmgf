package GameUI.Modules.PetPlayRule.PetSavvyUseMoney.UI
{
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetSavvyUseMoney.Data.PetSavvyUseMoneyConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PetSavvyUseMoneyUIManager
	{
		private var petView:MovieClip;
		private var petPhoto:FaceItem;
		
		public function PetSavvyUseMoneyUIManager(view:MovieClip)
		{
			this.petView = view;
			init();
		}
		
		private function init():void
		{
//			petView.txtSucPercent.text = "";
//			petView.txtLostPercent.text = "失败加成：";
//			petView.txtSavvy.text = "";
//			petView.txtPetName.text = "";
			
			petView.mouseEnabled = false;
			petView.txtSucPercent.mouseEnabled = false;
			petView.txtLoseTo.mouseEnabled = false;
			petView.txtVipAdd.mouseEnabled = false;
			petView.txtPetName.mouseEnabled = false;
			petView.txtSavvy.mouseEnabled = false;
			petView.txtForceApt.mouseEnabled = false;
			petView.txtAddForceApt.mouseEnabled = false;
			petView.txtSpiritPowerApt.mouseEnabled = false;
			petView.txtAddSpiritPowerApt.mouseEnabled = false;
			petView.txtPhysicalApt.mouseEnabled = false;
			petView.txtAddPhysicalApt.mouseEnabled = false;
			petView.txtConstantApt.mouseEnabled = false;
			petView.txtAddConstantApt.mouseEnabled = false;
			petView.txtMagicApt.mouseEnabled = false;
			petView.txtAddMagicApt.mouseEnabled = false;
//			var moneyStrNeed:String = "0\\sc";
//			petView.mcCost.txtMoney.text = moneyStrNeed;
			
//			ShowMoney.ShowIcon(petView.mcCost, petView.mcCost.txtMoney, true);
			clearData();
//			showMoney(1);
//			showMoney(2);
		}
		
		/** 显示钱 0-需要花费，1-携带碎银，2-携带银两 */
		public function showMoney(type:int, money:int = 0):void
		{
			switch(type) {
				case 0:
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(PetSavvyUseMoneyConstData.breedCost, ["\\se","\\ss","\\sc"]);
					petView.mcCost.txtMoney.text = moneyStrNeed;
					ShowMoney.ShowIcon(petView.mcCost, petView.mcCost.txtMoney, true);
					break;
				case 1:
					var moneyStrSuiYin:String = UIUtils.getMoneyStandInfo(Number(GameCommonData.Player.Role.UnBindMoney), ["\\se","\\ss","\\sc"]);
					petView.mcSuiYin.txtMoney.text = moneyStrSuiYin;
					ShowMoney.ShowIcon(petView.mcSuiYin, petView.mcSuiYin.txtMoney, true);
					break;
				case 2:
					var moneyStrYinLiang:String = UIUtils.getMoneyStandInfo(Number(GameCommonData.Player.Role.BindMoney), ["\\ce","\\cs","\\cc"]);
					petView.mcYinLiang.txtMoney.text = moneyStrYinLiang;
					ShowMoney.ShowIcon(petView.mcYinLiang, petView.mcYinLiang.txtMoney, true);
					break;
			}
		}
		
		public function clearData():void
		{
			petView.txtSucPercent.text = "";
			petView.txtLoseTo.text = "";
			petView.txtVipAdd.text = "";
			
			petView.txtPetName.text = "";
			petView.txtSavvy.text = "";
			petView.txtForceApt.text = "";
			petView.txtAddForceApt.text = "";
			petView.txtSpiritPowerApt.text = "";
			petView.txtAddSpiritPowerApt.text = "";
			petView.txtPhysicalApt.text = "";
			petView.txtAddPhysicalApt.text = "";
			petView.txtConstantApt.text = "";
			petView.txtAddConstantApt.text = ""; 
			petView.txtMagicApt.text = "";
			petView.txtAddMagicApt.text = "";
			removePetPhoto();
		}
		
		public function showData(pet:GamePetRole):void
		{
//			var pet:GamePetRole = GameCommonData.Player.Role.PetSnapList[id];
			if(pet != null) {
//				petView.txtSucPercent.text = "";
//				petView.txtLoseTo.text = "";
//				petView.txtVipAdd.text = "";
				
				var percent:String = PetRuleCommonData.getSavvyAddPercent(pet.Savvy);
				
				petView.txtPetName.text = pet.PetName; 
				
				var color:String = IntroConst.STENS_INCREMENT[pet.Savvy+1].color;
				
				petView.txtSavvy.htmlText = "<font color='"+color+"'>" + pet.Savvy + "</font>";
				
				petView.txtForceApt.text = pet.ForceApt.toString();
				petView.txtSpiritPowerApt.text = pet.SpiritPowerApt.toString();
				petView.txtPhysicalApt.text = pet.PhysicalApt.toString();
				petView.txtConstantApt.text = pet.ConstantApt.toString();
				petView.txtMagicApt.text = pet.MagicApt.toString();
				
				petView.txtAddForceApt.htmlText = "<font color='"+color+"'>" + percent + "</font>";
				petView.txtAddSpiritPowerApt.htmlText = "<font color='"+color+"'>" + percent + "</font>";
				petView.txtAddPhysicalApt.htmlText = "<font color='"+color+"'>" + percent + "</font>";
				petView.txtAddConstantApt.htmlText = "<font color='"+color+"'>" + percent + "</font>";
				petView.txtAddMagicApt.htmlText = "<font color='"+color+"'>" + percent + "</font>";
				addPetPhoto();
			}
		}
		
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
			
			petPhoto = new FaceItem(faceStr, petView.mcPhotoPet, "EnemyIcon");
			petPhoto.offsetPoint = new Point(0,0);
			petView.mcPhotoPet.addChild(petPhoto);
		}
		
		private function removePetPhoto():void
		{
			if(petPhoto && petView.mcPhotoPet.contains(petPhoto)) {
				petView.mcPhotoPet.removeChild(petPhoto);
				petPhoto = null;
			}
		}
		
	}
}