package GameUI.Modules.PetPlayRule.PetBreedSingle.UI
{
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetBreedSingle.Data.PetBreedSingleConstData;
	import GameUI.Modules.PetPlayRule.PetRuleController.Data.PetRuleCommonData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PetBreedSingleUIManager
	{
		private var panelView:MovieClip
		private var faceMale:FaceItem = null;
		private var faceFemale:FaceItem = null;
		
		public function PetBreedSingleUIManager(view:MovieClip)
		{
			this.panelView = view;
			init();
		}
		
		/** 初始化 */
		private function init():void
		{
//			panelView.mcPhotoMale.mouseEnabled = false;
//			panelView.mcPhotoFemale.mouseEnabled = false;
			panelView.mouseEnabled = false;
			panelView.petNameMale.mouseEnabled = false;
			panelView.petNameFemale.mouseEnabled = false;
			panelView.txtGradeMale.mouseEnabled = false;
			panelView.txtGradeFemale.mouseEnabled = false;
			
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
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(PetBreedSingleConstData.breedCost, ["\\se","\\ss","\\sc"]);
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
		
		/** 添加宠物头像 0-雄性，1-雌性 */
		public function addPetPhoto(type:int):void
		{
			if(type == 0) {
				removePetPhoto(type);
				var faceType:int = PetBreedSingleConstData.petMaleShow.FaceType;
				if(PetBreedSingleConstData.petMaleShow.Savvy >= 7) {
					faceType = PetPropConstData.getFaceType(faceType);
				}
				faceMale = new FaceItem(faceType.toString(), panelView.mcPhotoMale, "EnemyIcon", 1);
				faceMale.offsetPoint = new Point(0, 0);
				panelView.mcPhotoMale.addChild(faceMale);
			} else {
				removePetPhoto(type);
				var faceTypeOp:int = PetBreedSingleConstData.petFemaleShow.FaceType;
				if(PetBreedSingleConstData.petFemaleShow.Savvy >= 7) {
					faceTypeOp = PetPropConstData.getFaceType(faceTypeOp);
				}
				faceFemale = new FaceItem(faceTypeOp.toString(), panelView.mcPhotoFemale, "EnemyIcon", 1);
				faceFemale.offsetPoint = new Point(0, 0);
				panelView.mcPhotoFemale.addChild(faceFemale);
			}
		}
		
		/** 移除宠物头像 0-雄性，1-雌性 */
		private function removePetPhoto(type:int):void
		{
			if(type == 0) {
				if(faceMale && panelView.mcPhotoMale.contains(faceMale)) {
					panelView.mcPhotoMale.removeChild(faceMale);
					faceMale = null;
				}
			} else {
				if(faceFemale && panelView.mcPhotoFemale.contains(faceFemale)) {
					panelView.mcPhotoFemale.removeChild(faceFemale);
					faceMale = null;
				}
			}
		}
		
		/** 清除数据 */
		public function clearData(type:uint):void
		{
			if(type == 0) {
				panelView.petNameMale.text = "";
				panelView.txtGradeMale.text = "";
				removePetPhoto(type);
			} else {
				panelView.petNameFemale.text = "";
				panelView.txtGradeFemale.text = "";
				removePetPhoto(type);
			}
		}
		
		/** 显示数据 */
		public function showData(type:uint):void
		{
			if(type == 0) {
				if(PetBreedSingleConstData.petMaleShow) {
					panelView.petNameMale.text = PetBreedSingleConstData.petMaleShow.PetName;
					panelView.txtGradeMale.htmlText = PetRuleCommonData.getPetGrade(PetBreedSingleConstData.petMaleShow.Grade);
				} 
			} else {
				if(PetBreedSingleConstData.petFemaleShow) {
					panelView.petNameFemale.text = PetBreedSingleConstData.petFemaleShow.PetName;
					panelView.txtGradeFemale.htmlText = PetRuleCommonData.getPetGrade(PetBreedSingleConstData.petFemaleShow.Grade);
				}
			}
		}

	}
}