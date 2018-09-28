package GameUI.Modules.PetPlayRule.PetBreedDouble.UI
{
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetBreedDouble.Data.PetBreedDoubleConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PetBreedDoubleUIManager
	{
		private var panelView:MovieClip;
		private var faceSelf:FaceItem = null;
		private var faceOp:FaceItem = null;
		
		public function PetBreedDoubleUIManager(view:MovieClip)
		{
			this.panelView = view;
			init();
		}
		
		private function init():void
		{
			panelView.txtGradeOp.mouseEnabled = false;
			panelView.txtGradeSelf.mouseEnabled = false;
			panelView.txtPetNameOp.mouseEnabled = false;
			panelView.txtPetNameSelf.mouseEnabled = false;
			panelView.btnLockOp.mouseEnabled = false;
//			panelView.mcSure.mouseEnabled = false;
			panelView.txtSure.mouseEnabled = false;
			clearData(0);
			clearData(1);
//			showMoney(0);
//			showMoney(1);
//			showMoney(2);
		}
		
		/** 显示钱 0-需要花费，1-携带碎银，2-携带银两 */
		public function showMoney(type:int, money:int = 0):void
		{
			switch(type) {
				case 0:
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(PetBreedDoubleConstData.breedCost, ["\\se","\\ss","\\sc"]);
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
		
		/** 添加宠物头像 0-自己的，1-对方的 */
		public function addPet(type:int):void
		{
			if(type == 0) {
				removePetPhoto(type);
				var faceType:int = PetBreedDoubleConstData.petShowSelf.FaceType;
				if(PetBreedDoubleConstData.petShowSelf.Savvy >= 7) {
					faceType = PetPropConstData.getFaceType(faceType);
				}
				faceSelf = new FaceItem(faceType.toString(), panelView.mcPhotoSelf, "EnemyIcon", 1);
				faceSelf.offsetPoint = new Point(0,0);
				panelView.mcPhotoSelf.addChild(faceSelf);
				panelView.txtPetNameSelf.text = PetBreedDoubleConstData.petShowSelf.PetName;
				panelView.txtGradeSelf.htmlText = PetRuleCommonData.getPetGrade(PetBreedDoubleConstData.petShowSelf.Grade);
			} else {
				removePetPhoto(type);
				var faceTypeOp:int = PetBreedDoubleConstData.petShowOp.FaceType;
				if(PetBreedDoubleConstData.petShowOp.Savvy >= 7) {
					faceTypeOp = PetPropConstData.getFaceType(faceTypeOp);
				}
				faceOp = new FaceItem(faceTypeOp.toString(), panelView.mcPhotoOp, "EnemyIcon", 1); 
				faceOp.offsetPoint = new Point(0,0);
				panelView.mcPhotoOp.addChild(faceOp);
				panelView.txtPetNameOp.text = PetBreedDoubleConstData.petShowOp.PetName;
				panelView.txtGradeOp.htmlText = PetRuleCommonData.getPetGrade(PetBreedDoubleConstData.petShowOp.Grade);
			}
		}
		
		/** 移除宠物头像 0-自己的，1-对方的 */
		private function removePetPhoto(type:int):void
		{
			if(type == 0) {
				if(faceSelf && panelView.mcPhotoSelf.contains(faceSelf)) {
					panelView.mcPhotoSelf.removeChild(faceSelf);
					faceSelf = null;
				}
			} else {
				if(faceOp && panelView.mcPhotoOp.contains(faceOp)) {
					panelView.mcPhotoOp.removeChild(faceOp);
					faceOp = null;
				}
			}
		}
		
		/** 设置模式，0-队长未确定，1-队长已确定或确定按钮不可用时，2-队员 */
		public function setModel(type:int):void
		{
			switch(type) {
				case 0:
//					panelView.mcSure.visible = true;
					panelView.txtSure.visible = true;
					panelView.btnSure.visible = true;
					break;
				case 1:
//					panelView.mcSure.visible = true;
					panelView.txtSure.visible = true;
					panelView.btnSure.visible = false;
					break;
				case 2:
//					panelView.mcSure.visible = false;
					panelView.txtSure.visible = false;
					panelView.btnSure.visible = false;
					break;
			}
		}
		
		public function clearData(type:uint):void
		{
			if(type == 0) {
				panelView.txtPetNameSelf.text = "";
				panelView.txtGradeSelf.text = "";
			} else {
				panelView.txtPetNameOp.text = "";
				panelView.txtGradeOp.text = "";
			}
		}
		
		public function showData(type:uint):void
		{
			if(type == 0) {
				panelView.txtPetNameSelf.text = PetBreedDoubleConstData.petShowSelf.PetName;
				panelView.txtGradeSelf.htmlText = PetRuleCommonData.getPetGrade(PetBreedDoubleConstData.petShowSelf.Grade);;
			} else {
				panelView.txtPetNameOp.text = PetBreedDoubleConstData.petShowOp.PetName;
				panelView.txtGradeOp.htmlText = PetRuleCommonData.getPetGrade(PetBreedDoubleConstData.petShowOp.Grade);
			}
		}

	}
}