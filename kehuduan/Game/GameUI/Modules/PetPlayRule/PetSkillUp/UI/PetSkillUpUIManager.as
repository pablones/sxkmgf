package GameUI.Modules.PetPlayRule.PetSkillUp.UI
{
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.Modules.PetPlayRule.PetSkillUp.Data.PetSkillUpConstData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PetSkillUpUIManager
	{
		private var petView:MovieClip = null;
		private var petPhoto:FaceItem = null;
		
		public function PetSkillUpUIManager(view:MovieClip)
		{
			this.petView = view;
			init();
		}
		
		private function init():void
		{
//			petView.txtType.text = "";
//			petView.txtSex.text = "";
//			petView.txtTakeLevel.text = "";
//			petView.txtCharacter.text = "";
//			var moneyStrNeed:String = "0\\sc";
//			petView.mcCost.txtMoney.text = moneyStrNeed;
//			ShowMoney.ShowIcon(petView.mcCost, petView.mcCost.txtMoney, true);
//			showMoney(0);
//			showMoney(1);
//			showMoney(2);
//			updatePetNum();
			petView.mouseEnabled = false;
			petView.txtPetName.mouseEnabled = false;
			petView.txtHp.mouseEnabled = false;
			petView.txtPhyAttack.mouseEnabled = false;
			petView.txtMagicAttack.mouseEnabled = false;
			petView.txtHit.mouseEnabled = false;
			petView.txtCrit.mouseEnabled = false;
			petView.txtPhyDef.mouseEnabled = false;
			petView.txtMagicDef.mouseEnabled = false;
			petView.txtHide.mouseEnabled = false;
			petView.txtDragName.mouseEnabled = false;
			clearData();
		}
		
		/** 显示钱 0-需要花费，1-携带碎银，2-携带银两 */
		public function showMoney(type:int, money:int = 0):void
		{
			switch(type) {
				case 0:
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(PetSkillUpConstData.breedCost, ["\\se","\\ss","\\sc"]);
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
		
		/** 清除所有数据 */
		public function clearData():void
		{
//			petView.txtType.text = "";
//			petView.txtSex.text = "";
//			petView.txtTakeLevel.text = "";
//			petView.txtCharacter.text = "";
			petView.txtPetName.text = "";
			petView.txtHp.text = "";
			petView.txtPhyAttack.text = "";
			petView.txtMagicAttack.text = "";
			petView.txtHit.text = "";
			petView.txtCrit.text = "";
			petView.txtPhyDef.text = "";
			petView.txtMagicDef.text = "";
			petView.txtHide.text = "";
			petView.txtDragName.text = "";
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
			
			petPhoto = new FaceItem( faceStr, petView.mcPhotoPet, "EnemyIcon", 1 ); 
			petPhoto.offsetPoint = new Point(0, 0);
			petView.mcPhotoPet.addChild(petPhoto);
		}
		
		private function removePetPhoto():void
		{
			if(petPhoto && petView.mcPhotoPet.contains(petPhoto)) {
				petView.mcPhotoPet.removeChild(petPhoto);
				petPhoto = null;
			}
		}
		
		/** 显示宠物快照数据 */
		public function showData():void
		{
			addPetPhoto();
			
			var pet:GamePetRole = PetRuleCommonData.selectedPet;
			
			petView.txtPetName.text = pet.PetName;
			petView.txtHp.text = pet.HpMax.toString();
			petView.txtPhyAttack.text = pet.PhyAttack.toString();
			petView.txtMagicAttack.text = pet.MagicAttack.toString();
			petView.txtHit.text = pet.Hit.toString();
			petView.txtCrit.text = pet.Crit.toString();
			petView.txtPhyDef.text = pet.PhyDef.toString();
			petView.txtMagicDef.text = pet.MagicDef.toString();
			petView.txtHide.text = pet.Hide.toString();
			
//			switch(PetSkillUpConstData.petSelected.Type) {
//				case 0:
//					petView.txtType.text = "野生";
//					break;
//				case 1:
//					petView.txtType.text = "宝宝";
//					break;
//				default:
//					petView.txtType.text = "二代";
//					break;
//			}
//			petView.txtSex.text = (PetSkillUpConstData.petSelected.Sex == 0) ? "雄性" : "雌性";
//			petView.txtTakeLevel.htmlText = '<font color="#00ff00">' + PetSkillUpConstData.petSelected.TakeLevel + '级</font><font color="#ffffff">可携带</font>';
//			switch(PetSkillUpConstData.petSelected.Character) {
//				case 0:
//					petView.txtCharacter.text = "勇猛";
//					break;
//				case 1:
//					petView.txtCharacter.text = "胆小";
//					break;
//				case 2:
//					petView.txtCharacter.text = "精明";
//					break;
//				case 3:
//					petView.txtCharacter.text = "谨慎";
//					break;
//				case 4:
//					petView.txtCharacter.text = "忠诚";
//					break;
//			}
		}
		
//		public function updatePetNum():void 
//		{
//			var count:int;
//			for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
//				count++;
//			}
//			petView.txtPetCount.text = count + "/" + PetPropConstData.petBagNum;
//		}
		
	}
}