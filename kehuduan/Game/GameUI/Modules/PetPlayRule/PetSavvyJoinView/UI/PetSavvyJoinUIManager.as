package GameUI.Modules.PetPlayRule.PetSavvyJoinView.UI
{
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PetPlayRule.PetSavvyJoinView.Data.PetSavvyJoinConstData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	
	public class PetSavvyJoinUIManager
	{
		private var petView:MovieClip;
		private var faceMale:FaceItem = null;
		private var faceFemale:FaceItem = null;
		
		public function PetSavvyJoinUIManager(view:MovieClip)
		{
			this.petView = view;
			init();
		}
		
		private function init():void
		{
//			petView.txtSucPercent.text = "";
//			petView.txtLostPercent.text = "失败加成：";
//			petView.txtPetName_0.text = "";
//			petView.txtPetName_1.text = ""; 
//			var moneyStrNeed:String = "0\\sc";
//			petView.mcCost.txtMoney.text = moneyStrNeed;
//			ShowMoney.ShowIcon(petView.mcCost, petView.mcCost.txtMoney, true);
//			showMoney(1);
//			showMoney(2);
			petView.txtSucPercent.mouseEnabled = false;
			petView.txtLoseTo.mouseEnabled = false;
			petView.txtPetName_0.mouseEnabled = false;
			petView.txtPetName_1.mouseEnabled = false;
			petView.txtCurSavvy.mouseEnabled = false;
			petView.txtCurGenius.mouseEnabled = false;
			clearData(0);
			clearData(1);
		}
		
		/** 显示钱 0-需要花费，1-携带碎银，2-携带银两 */
		public function showMoney(type:int, money:int = 0):void
		{
			switch(type) {
				case 0:
					var moneyStrNeed:String = UIUtils.getMoneyStandInfo(PetSavvyJoinConstData.breedCost, ["\\se","\\ss","\\sc"]);
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
		
		public function clearData(type:uint):void
		{
			if(type == 0) {
//				petView.txtSucPercent.text = "";
//				petView.txtLoseTo.text = "";
				petView.txtPetName_0.text = "";
				petView.txtCurSavvy.text = "";
				removePetPhoto(type);
			} else {
				petView.txtPetName_1.text = "";
				petView.txtCurGenius.text = "";
				removePetPhoto(type);
			}
		}
		
		public function showData(pet:GamePetRole, type:uint):void
		{
			if(type == 0) {
//				petView.txtSucPercent.text = "";
//				petView.txtLoseTo.text = "";
				petView.txtPetName_0.text = pet.PetName;
				petView.txtCurSavvy.text = pet.Savvy.toString();
				addPetPhoto(type);
			} else {
				petView.txtPetName_1.text = pet.PetName;
				petView.txtCurGenius.text = pet.Genius.toString();
				addPetPhoto(type);
			}
		}
		
		private function removePetPhoto(type:int):void
		{
			if(type == 0) {
				if(faceMale && petView.mcPhotoMale.contains(faceMale)) {
					petView.mcPhotoMale.removeChild(faceMale);
					faceMale = null;
				}
			} else {
				if(faceFemale && petView.mcPhotoFemale.contains(faceFemale)) {
					petView.mcPhotoFemale.removeChild(faceFemale);
					faceFemale = null;
				}
			}
		}
		
		public function addPetPhoto(type:int):void
		{
			if(type == 0) {
				removePetPhoto(type);
				var faceType:int = PetSavvyJoinConstData.petShow_0.FaceType;
				if(PetSavvyJoinConstData.petShow_0.Savvy >= 7) 
				{
					faceType = PetPropConstData.getFaceType(faceType); 
				}
				
				var faceStr:String = faceType.toString();
				if( PetSavvyJoinConstData.petShow_0.Type > 1 )
				{
					var petV:XML = PlayerSkinsController.GetPetV( PetSavvyJoinConstData.petShow_0.ClassId.toString() , PetSavvyJoinConstData.petShow_0.Type - 1 ); 
					if( petV != null)
					{
						faceStr = petV.@Face;
					}
				}
				
				faceMale = new FaceItem(faceStr, petView.mcPhotoMale, "EnemyIcon", 1);
				faceMale.offsetPoint = new Point(0, 0);
				petView.mcPhotoMale.addChild(faceMale);
			} else {
				removePetPhoto(type);
				var faceTypeOp:int = PetSavvyJoinConstData.petShow_1.FaceType;
				if(PetSavvyJoinConstData.petShow_1.Savvy >= 7) 
				{
					faceTypeOp = PetPropConstData.getFaceType(faceTypeOp);
				}
				
				var faceOpStr:String = faceTypeOp.toString();
				if( PetSavvyJoinConstData.petShow_1.Type > 1 )
				{
					var petV1:XML = PlayerSkinsController.GetPetV( PetSavvyJoinConstData.petShow_1.ClassId.toString() , PetSavvyJoinConstData.petShow_1.Type - 1 ); 
					if( petV1 != null)
					{
						faceOpStr = petV1.@Face;
					}
				}
				
				faceFemale = new FaceItem(faceOpStr, petView.mcPhotoFemale, "EnemyIcon", 1);
				faceFemale.offsetPoint = new Point(0, 0);
				petView.mcPhotoFemale.addChild(faceFemale);
			}
		}
		
	}
}