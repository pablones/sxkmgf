package GameUI.Modules.Pet.UI
{
	import Controller.PlayerSkinsController;
	
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.Pet.Proxy.PetSkillGridManager;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Point;
	
	public class PetUIManager
	{
		private var petView:MovieClip = null;
		private var petPhoto:FaceItem = null;
		private var _tempMC:MovieClip; // 用于更多信息面板的元件
		private var _showPetModule:ShowPetModuleComponent;
		private var backView:Sprite;	//宠物显示背景
//		private var petPhoto:PetSkinPlayer = null;
		
		private var petSkillGridManager:PetSkillGridManager = null;
		
		public function PetUIManager(petView:MovieClip, gridManager:PetSkillGridManager,tempMC:MovieClip,showPetModule:ShowPetModuleComponent )
		{
			this.petView = petView;
			this.petSkillGridManager = gridManager;
			_tempMC = tempMC;
			_showPetModule = showPetModule;
			initView();
		}
		
		/** 初始化 */
		private function initView():void
		{
			backView = this.petView.getChildByName("backView") as Sprite;
			disableMouse();
		}
		
		/** 禁用无关鼠标 */
		private function disableMouse():void
		{
			petView.txtModifyName.mouseEnabled = false;
			petView.txtFeed.mouseEnabled = false;
			petView.txtTraining.mouseEnabled = false;
//			petView.txtAlive.mouseEnabled = false;
			_tempMC.txtAlive.mouseEnabled = false;
			petView.txtState.mouseEnabled = false;
			petView.txtSure.mouseEnabled = false;
			petView.txtExp.mouseEnabled = false;
			petView.txtGrade.mouseEnabled = false;
			petView.txtType.mouseEnabled = false;
//			petView.txtExtLife.mouseEnabled = false;
			_tempMC.txtExtLife.mouseEnabled = false;
			if(PetPropConstData.isNewPetVersion)
			{
				_tempMC.txtDesignation.mouseEnabled = false;
				petView.txtDependence.mouseEnabled = false;
				petView.txtFantasy.mouseEnabled = false;
				petView.txtWinningName.mouseEnabled = false;
				petView.txtWinning.mouseEnabled = false;
				petView.txtPrivityName.mouseEnabled = false;
				petView.txtPrivity.mouseEnabled = false;
				petView.txtSavvyName.mouseEnabled = false;
				petView.txtSavvy.mouseEnabled = false;
				petView.txtMorePetInfo.mouseEnabled = false;
			}
		}
		
		/** 设置模式 0=自己宠物有潜力点战斗状态，1-自己宠物有潜力休息状态，2-自己宠物无潜力点战斗状态，3-自己宠物无潜力点休息状态，4-查看别人的宠物资料或不可操作状态下查看自己的宠物资料(不可见所有按钮)，5-查看自己的宠物资料(可见宠物数量) */
		public function setModel(type:int):void
		{
			switch(type) {
				case 0:
					model0();
					break;
				case 1:
					model1();
					break;
				case 2:
					model2();
					break;
				case 3:
					model3();
					break;
				case 4:
					model4();
					break;
				case 5:
					model5();
					break;
				case 6:
					model6();
					break;
				case 7:
					model7();
					break;
			}
		}
		
		private function model0():void
		{
			setAddSubVisable(true);
			setSeflBtnVisable(true);
			petView.txtSure.visible = true;
			petView.btnSure.visible	= true;
			petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_2" ];    //  休息
			petView.txtPetName.mouseEnabled = true;
			petView.txtMypet.visible 	  = true;
		}
		private function model1():void
		{
			setAddSubVisable(true);
			setSeflBtnVisable(true);
			petView.txtSure.visible = true;
			petView.btnSure.visible	= true;
			petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_1" ];   // 出战
			petView.txtPetName.mouseEnabled = true;
			petView.txtMypet.visible 	  = true;
		}
		private function model2():void
		{
			setAddSubVisable(false);
			setSeflBtnVisable(true);
			petView.txtSure.visible = false;
			petView.btnSure.visible	= false;
			petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_2" ];    //  休息
			petView.txtPetName.mouseEnabled = true;
			petView.txtMypet.visible 	  = true;
		}
		private function model3():void
		{
			setAddSubVisable(false);
			setSeflBtnVisable(true);
			petView.txtSure.visible = false;
			petView.btnSure.visible	= false;
			petView.txtState.text = GameCommonData.wordDic[ "mod_pet_med_petp_upd_1" ];  // 出战
			petView.txtPetName.mouseEnabled = true;
			petView.txtMypet.visible 	  = true;
		}
		private function model4():void
		{
			setAddSubVisable(false);
			setSeflBtnVisable(false);
			petView.txtMypet.visible 	  = false;
			petView.txtPetCount.visible   = false;
			petView.txtSure.visible 	  = false;
			petView.btnSure.visible		  = false;
			petView.txtPetName.mouseEnabled = false;
		}
		private function model5():void
		{
			setAddSubVisable(false);
			setSeflBtnVisable(false);
			petView.txtMypet.visible 	  = true;
			petView.txtPetCount.visible   = true;
			petView.txtSure.visible 	  = false;
			petView.btnSure.visible		  = false;
			petView.txtPetName.mouseEnabled = false;
		}
		
		/** 有可加潜力点。更新 */
		private function model6():void
		{
			setAddSubVisable(true);
			petView.txtSure.visible = true;
			petView.btnSure.visible	= true;
		}
		
		/** 无可加潜力点。更新 */
		private function model7():void
		{
			setAddSubVisable(false);
			petView.txtSure.visible = false;
			petView.btnSure.visible	= false;
		}
		/** 设置+-按钮是否可见 */
		private function setAddSubVisable(value:Boolean):void
		{
			petView.btnAddStrong.visible 	= value;
			petView.btnSubStrong.visible	= value;
			petView.btnAddSprite.visible 	= value;
			petView.btnSubSprite.visible	= value;
			petView.btnAddPhysical.visible  = value;
			petView.btnSubPhysical.visible  = value;
			petView.btnAddConstant.visible  = value;
			petView.btnSubConstant.visible  = value;
			petView.btnAddMagic.visible 	= value;
			petView.btnSubMagic.visible 	= value;
		}
		
		/** 设置 修改、喂食、驯养、放生、出战、延寿 幻化按钮是否可见 */
		private function setSeflBtnVisable(value:Boolean):void
		{
			petView.txtModifyName.visible = value;
			petView.txtFeed.visible   	  = value;
			petView.txtTraining.visible   = value;
//			petView.txtAlive.visible 	  = value;
			_tempMC.txtAlive.visible 	  = value;
			petView.txtState.visible 	  = value;
			
//			petView.txtExtLife.visible 	  = value;
			_tempMC.txtExtLife.visible 	  = value;
			
			petView.btnModifyName.visible = value;
			petView.btnFeed.visible 	  = value;
			petView.btnTraining.visible   = value;
//			petView.btnAlive.visible 	  = value;
			_tempMC.btnAlive.visible 	  = value;
			petView.btnState.visible 	  = value;
//			petView.btnExtLife.visible 	  = value;
			_tempMC.btnExtLife.visible 	  = value;
			if(PetPropConstData.isNewPetVersion)
			{
				_tempMC.txtDesignation.visible = value;
				_tempMC.btnDesignation.visible = value;
				petView.txtDependence.visible = value;
				petView.btnDependence.visible = value;
				petView.txtFantasy.visible = value;
				petView.btnFantasy.visible = value;
				petView.btn_changeSex.visible = value;
				petView.btnMorePetInfo.visible = value;
				petView.txtMorePetInfo.visible = value;
			}
		}
		
		/** 设置属性条长度  len-当前值，max-最大值，type-类型(0力量 1灵力 2体质 3定力 4身法) */
		private function setMclenght(petRole:GamePetRole):void
		{
			var max:Number = petRole.ForceInitMax;
			var now:Number = petRole.ForceInit;
			var newWidth:Number;	//颜色条长度
			if(PetPropConstData.isNewPetVersion)
			{
				newWidth = 100;
			}
			else
			{
				newWidth = 132;
			}
			var width:Number = (now/max) * newWidth;
			if(width > newWidth) {
//				petView.mcForce.width = newWidth;
				_tempMC.mcForce.width = newWidth;
			} else if(width < 1){
//				petView.mcForce.width = 1;
				_tempMC.mcForce.width = 1;
			} else {
//				petView.mcForce.width = width;
				_tempMC.mcForce.width = width;
			}
//			petView.txtForceInit.text = now + "/" + max;
			_tempMC.txtForceInit.text = now + "/" + max;
			
			max = petRole.SpiritPowerInitMax;
			now = petRole.SpiritPowerInit;
			width = (now/max) * newWidth;
			if(width > newWidth) {
//				petView.mcSpiritPowerer.width = newWidth;
				_tempMC.mcSpiritPower.width = newWidth;
			} else if(width < 1){
//				petView.mcSpiritPower.width = 1;
				_tempMC.mcSpiritPower.width = 1;
			} else {
//				petView.mcSpiritPower.width = width;
				_tempMC.mcSpiritPower.width = width;
			}
//			petView.txtSpiritPowerInit.text = now + "/" + max;
			_tempMC.txtSpiritPowerInit.text = now + "/" + max;
			
			max = petRole.PhysicalInitMax;
			now = petRole.PhysicalInit;
			width = (now/max) * newWidth;
			if(width > newWidth) {
//				petView.mcPhysical.width = newWidth;
				_tempMC.mcPhysical.width = newWidth;
			} else if(width < 1){
//				petView.mcPhysical.width = 1;
				_tempMC.mcPhysical.width = 1;
			} else {
//				petView.mcPhysical.width = width;
				_tempMC.mcPhysical.width = width;
			}
//			petView.txtPhysicalInit.text = now + "/" + max;
			_tempMC.txtPhysicalInit.text = now + "/" + max;
			
			max = petRole.ConstantInitMax;
			now = petRole.ConstantInit;
			width = (now/max) * newWidth;
			if(width > newWidth) {
//				petView.mcConstant.width = newWidth;
				_tempMC.mcConstant.width = newWidth;
			} else if(width < 1){
//				petView.mcConstant.width = 1;
				_tempMC.mcConstant.width = 1;
			} else {
//				petView.mcConstant.width = width;
				_tempMC.mcConstant.width = width;
			}
//			petView.txtConstantInit.text = now + "/" + max;
			_tempMC.txtConstantInit.text = now + "/" + max;
			
			max = petRole.MagicInitMax;
			now = petRole.MagicInit;
			width = (now/max) * newWidth;
			if(width > newWidth) {
//				petView.mcMagic.width = newWidth;
				_tempMC.mcMagic.width = newWidth;
			} else if(width < 1){
//				petView.mcMagic.width = 1;
				_tempMC.mcMagic.width = 1;
			} else {
//				petView.mcMagic.width = width;
				_tempMC.mcMagic.width = width;
			}
//			petView.txtMagicInit.text = now + "/" + max;
			_tempMC.txtMagicInit.text = now + "/" + max;
			if(PetPropConstData.isNewPetVersion)
			{
				
			}
		}
		
		/** 显示宠物数据 0-正常查看(可加点可操作)，1-查看宠物资料(不可操作)*/
		public function showPetData(pet:GamePetRole, type:int):void
		{
			if(type == 0) {
				petView.txtPetCount.visible = true;
				petView.txtMypet.visible 	= true;
			} else {
				petView.txtPetCount.visible = false;
				petView.txtMypet.visible 	= false;
			}
			var count:int;
			for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
				count++;
			}
			petView.txtPetCount.text = count + "/" + PetPropConstData.petBagNum;
			addPetPhoto(pet);
			petView.txtPetName.text = pet.PetName;
			switch(pet.Type) {
				case 0:
					petView.txtType.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_1" ];    //  野生
					break;
				case 1:
					petView.txtType.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_2" ];   //   宝宝
					break;
				default:
					petView.txtType.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_3" ];   //  二代
					break;
			} 
			if(pet.isFantasy)
			{
				petView.txtSex.text = GameCommonData.wordDic[ "mod_pet_med_petPro_setView_1"];//"幻兽";
			}
			else
			{
//				petView.txtSex.text = (pet.Sex == 0) ? GameCommonData.wordDic[ "mod_pet_med_petu_sho_4" ]: GameCommonData.wordDic[ "mod_pet_med_petu_sho_5" ];    // 雄性   雌性
				petView.txtSex.text = (pet.Sex == 0) ? GameCommonData.wordDic[ "mod_pet_med_petPro_setView_2"]:GameCommonData.wordDic[ "mod_pet_med_petPro_setView_3"];//"雄":"雌";    // 雄性   雌性
			}
			petView.txtTakeLevel.htmlText = '<font color="#00ff00">' + pet.TakeLevel + GameCommonData.wordDic[ "mod_pet_med_petu_sho_6" ];   //  级</font><font color="#ffffff">可携带</font>
			switch(pet.Character) {
				case 0:
					petView.txtCharacter.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_7" ];    //  勇猛
					break;
				case 1:
					petView.txtCharacter.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_8" ];   // 胆小
					break;
				case 2:
					petView.txtCharacter.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_9" ];    //  精明
					break;
				case 3:
					petView.txtCharacter.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_10" ];    //  谨慎
					break;
				case 4:
					petView.txtCharacter.text = GameCommonData.wordDic[ "mod_pet_med_petu_sho_11" ];    //  忠诚
					break;
			}
			petView.txtHp.text = pet.HpNow + "/" + pet.HpMax;
			petView.txtHappy.text = pet.HappyNow + "/" + pet.HappyMax;
//			petView.txtLife.text = pet.LifeNow + "/" + pet.LifeMax;
			_tempMC.txtLife.text = pet.LifeNow + "/" + pet.LifeMax;
			petView.txtExp.text = pet.ExpNow + "/" + UIConstData.ExpDic[3000+pet.Level];
			petView.txtAppraise.text = UIUtils.GetScoreStr(pet.Appraise, true);
			petView.txtLevel.text = pet.Level.toString();
			petView.txtGenius.text = pet.Genius.toString();
			if(PetPropConstData.isNewPetVersion)
			{
				petView.txtHadBreed.text = pet.BreedNow + "/" + pet.BreedMax;
				petView.txtPlayNum.text = pet.playNumber;
			}
			else
			{
				petView.txtBreed.text = pet.BreedNow + "/" + pet.BreedMax;
			}
			
			setMclenght(pet);
			var gradeStr:String = "";
			if(pet.Grade <= 20)
			{
				gradeStr = '<font color="'+IntroConst.itemColors[0]+'">'+GameCommonData.wordDic[ "mod_pet_med_petu_sho_16" ]+'</font>';	//普通
			} 
			else if(pet.Grade <= 40)
			{
				gradeStr = '<font color="'+IntroConst.itemColors[2]+'">'+GameCommonData.wordDic[ "mod_pet_med_petu_sho_15" ]+'</font>';	//优秀
			}
			else if(pet.Grade <= 60)
			{
				gradeStr = '<font color="'+IntroConst.itemColors[3]+'">'+GameCommonData.wordDic[ "mod_pet_med_petu_sho_14" ]+'</font>';	//杰出
			}
			else if(pet.Grade <= 80)
			{
				gradeStr = '<font color="'+IntroConst.itemColors[4]+'">'+GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ]+'</font>';	//卓越
			}
			else if(pet.Grade <= 100)
			{
				gradeStr = '<font color="'+IntroConst.itemColors[5]+'">'+GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ]+'</font>';	//完美
			}
			petView.txtGrade.htmlText = gradeStr;
			petView.txtSavvy.text = pet.Savvy.toString();
			petView.txtForce.text = pet.Force.toString();
			petView.txtSpiritPower.text = pet.SpiritPower.toString();
			petView.txtPhysical.text = pet.Physical.toString();
			petView.txtConstant.text = pet.Constant.toString();
			petView.txtMagic.text = pet.Magic.toString();
			
			//潜力值   需判断状态
			if(type == 0) {//可操作
				if(pet.Potential > 0) {
					PetPropConstData.potentials = pet.Potential;
					PetPropConstData.points = [0,0,0,0,0];
				}
			}
			petView.txtPotential.text = (pet.Potential == 0) ? "" : pet.Potential.toString();
			
			petView.txtForceApt.text = pet.ForceApt.toString();
			petView.txtSpiritPowerApt.text = pet.SpiritPowerApt.toString(); 
			petView.txtPhysicalApt.text = pet.PhysicalApt.toString();
			petView.txtConstantApt.text = pet.ConstantApt.toString();
			petView.txtMagicApt.text = pet.MagicApt.toString();
			
			var savvyAddPercent:Number;
			switch(pet.Savvy) {
				case 1:
					savvyAddPercent = 1;
					break;					
				case 2:
					savvyAddPercent = 1.6;
					break;					
				case 3:
					savvyAddPercent = 2.6;
					break;					
				case 4:
					savvyAddPercent = 4.1;
					break;					
				case 5:
					savvyAddPercent = 6.6;
					break;					
				case 6:
					savvyAddPercent = 10.5;
					break;					
				case 7:
					savvyAddPercent = 16.8;
					break;					
				case 8:
					savvyAddPercent = 26.8;
					break;					
				case 9:
					savvyAddPercent = 42.9;
					break;					
				case 10:
					savvyAddPercent = 68.7;
					break;					
				default:
					savvyAddPercent = 0;
					break;
			}
			var winningPercent:Number;
			switch(pet.winning)
			{
				case 1:
					winningPercent = 1;
				break;
				case 2:
					winningPercent = 3;
				break;
				case 3:
					winningPercent = 5;
				break;
				case 4:
					winningPercent = 7;
				break;
				case 5:
					winningPercent = 9;
				break;
				case 6:
					winningPercent = 11;
				break;
				case 7:
					winningPercent = 13;
				break;
				case 8:
					winningPercent = 15;
				break;
				case 9:
					winningPercent = 17;
				break;
				case 10:
					winningPercent = 20;
				break;
				default:
					winningPercent = 0;
				break;
			}
			var percentStr:String = (savvyAddPercent+winningPercent) == 0 ? "":"(+"+(savvyAddPercent+winningPercent).toString()+"%)";
			petView.txtAddForceApt.text = percentStr;
			petView.txtAddSpiritPowerApt.text = percentStr;
			petView.txtAddPhysicalApt.text = percentStr;
			petView.txtAddConstantApt.text = percentStr;
			petView.txtAddMagicApt.text = percentStr;
			
			petView.txtPhyAttack.text = pet.PhyAttack.toString();
			petView.txtMagicAttack.text = pet.MagicAttack.toString();
			petView.txtPhyDef.text = pet.PhyDef.toString();
			petView.txtMagicDef.text = pet.MagicDef.toString();
			petView.txtHit.text = pet.Hit.toString();
			petView.txtHide.text = pet.Hide.toString();
			petView.txtCrit.text = pet.Crit.toString();
			petView.txtToughness.text = pet.Toughness.toString();
			
			if(pet.OwnerId != GameCommonData.Player.Role.Id) {
				setModel(4);
				petSkillGridManager.lockAllGrid(false);
				petView.txtPetCount.visible = false;
				petView.txtMypet.visible 	= false;
			}
		}
		
		/** 清除所有数据 */
		public function clearAllData():void
		{
			removePetPhoto();
			petView.txtType.text = "";
			petView.txtSex.text = "";
			petView.txtTakeLevel.htmlText = "";
			petView.txtCharacter.text = "";
		
			petView.txtPetName.text = "";
			petView.txtHp.text = "";
			petView.txtHappy.text = "";
//			petView.txtLife.text = "";
			_tempMC.txtLife.text = "";
			petView.txtExp.text = "";
			petView.txtAppraise.text = "";
			petView.txtLevel.text =  "";
			petView.txtGenius.text = "";
			if(PetPropConstData.isNewPetVersion)
			{
				petView.txtHadBreed.text = "0/0";
				petView.txtPlayNum.text = "";
			}
			else
			{
				petView.txtBreed.text = "";
			}
			
//			setMclenght(pet);
			petView.txtGrade.text = "";

			petView.txtSavvy.text = "";
			petView.txtForce.text = "";
			petView.txtSpiritPower.text = "";
			petView.txtPhysical.text = "";
			petView.txtConstant.text = "";
			petView.txtMagic.text = "";
			
			petView.txtPotential.text = "";
			
			petView.txtForceApt.text = "";
			petView.txtSpiritPowerApt.text = "";
			petView.txtPhysicalApt.text = "";
			petView.txtConstantApt.text = "";
			petView.txtMagicApt.text = "";
			
			petView.txtAddForceApt.text = "";
			petView.txtAddSpiritPowerApt.text = "";
			petView.txtAddPhysicalApt.text = "";
			petView.txtAddConstantApt.text = "";
			petView.txtAddMagicApt.text = "";
			
			petView.txtPhyAttack.text = "";
			petView.txtMagicAttack.text = "";
			petView.txtPhyDef.text = "";
			petView.txtMagicDef.text = "";
			petView.txtHit.text = "";
			petView.txtHide.text = "";
			petView.txtCrit.text = "";
			petView.txtToughness.text = "";

			/* petView.txtForceInit.text = "";
			petView.txtSpiritPowerInit.text = "";
			petView.txtPhysicalInit.text = "";
			petView.txtConstantInit.text = "";
			petView.txtMagicInit.text = "";
			petView.mcForce.width = 0;
			petView.mcSpiritPower.width = 0;
			petView.mcPhysical.width = 0;
			petView.mcConstant.width = 0;
			petView.mcMagic.width = 0; */
			_tempMC.txtForceInit.text = "";
			_tempMC.txtSpiritPowerInit.text = "";
			_tempMC.txtPhysicalInit.text = "";
			_tempMC.txtConstantInit.text = "";
			_tempMC.txtMagicInit.text = "";
			_tempMC.mcForce.width = 0;
			_tempMC.mcSpiritPower.width = 0;
			_tempMC.mcPhysical.width = 0;
			_tempMC.mcConstant.width = 0;
			_tempMC.mcMagic.width = 0;
		}
		
		/** 添加宠物头像 */
		public function addPetPhoto(pet:GamePetRole):void
		{
			removePetPhoto();
			var faceType:int = pet.FaceType;
			var petClass:String;
			if(pet.Savvy >= 7) {
				faceType = PetPropConstData.getFaceType(faceType);
			}
			petClass = faceType.toString();
			if( pet.Type > 1 )
			{
				var petV:XML = PlayerSkinsController.GetPetV( pet.ClassId.toString() , pet.Type - 1 );
				if( petV != null )
				{
					petClass = petV.@EnemyIcon.toString();
				}
			}
			if(PetPropConstData.isNewPetVersion)
			{
				if(PetPropConstData.newPetModuleSwf[petClass])
				{
					petClass = PetPropConstData.newPetModuleSwf[petClass];
				}
				_showPetModule.showView(petClass);
			 }
			else
			{ 
				petPhoto = new FaceItem(petClass, petView.mcPhotoPet, "EnemyIcon", 1);
				petPhoto.offsetPoint = new Point(0,0);
				backView.addChild(petPhoto);
			}
			
		}
		
		private function removePetPhoto():void
		{
			if(!PetPropConstData.isNewPetVersion)
			{
				if(petPhoto && backView.contains(petPhoto)) {
					backView.removeChild(petPhoto);
					petPhoto = null;
				}
			}
			else
			{ 
				_showPetModule.deleteView();
			}
		}
		
		/** 其他玩家宠物列表，宠物数量  */
		public function countPetOthers():uint
		{
			var count:uint = 0;
			for(var id:Object in PetPropConstData.petListOthers) {
				count++;
			}
			return count;
		}
		
		/** 更新宠物数量显示 */
		public function updatePetNum():void
		{
			var count:int;
			for(var key:Object in GameCommonData.Player.Role.PetSnapList) {
				if(GameCommonData.Player.Role.PetSnapList[key].IsLock == false) {
					count++;
				}
			}
			petView.txtPetCount.text = count + "/" + PetPropConstData.petBagNum;
			petView.txtMypet.visible = true;
			petView.txtPetCount.visible = true;
		}
				
	}
}