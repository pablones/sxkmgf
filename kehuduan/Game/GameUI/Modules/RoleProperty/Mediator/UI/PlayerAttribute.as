package GameUI.Modules.RoleProperty.Mediator.UI
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Net.NetAction;
	import GameUI.UICore.UIFacade;
	
	import OopsEngine.Role.RoleJob;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.text.TextFormat;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	public class PlayerAttribute
	{
		private var equip:MovieClip = null;
		private var curJob:RoleJob = null;
		private var _levUpLock:Boolean = false;	//升级锁
		private var intervalId:uint;			//点击加减属性的计时器
		
		public function PlayerAttribute(view:MovieClip)
		{
			equip = view;
		}
		
		public function set levUpLock(val:Boolean):void
		{
			this._levUpLock = val;	
		}
		
		public function get levUpLock():Boolean
		{
			return this._levUpLock;
		}
		
		
		/** 
		 * 初始化
		 *  */		
		public function ShowPropData():void
		{
			curJob = GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob-1];
			equip.txtLevel.text = GameCommonData.Player.Role.Level+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ];   //"级"
			equip.txtHp.text = GameCommonData.Player.Role.HP;
			equip.mcRoleMale.gotoAndStop(GameCommonData.Player.Role.Sex+1);
			var roleName:String = GameCommonData.RolesListDic[GameCommonData.Player.Role.MainJob.Job];
			equip.txtMainRole.text = roleName + (roleName==GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_2" ]?GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_3" ]:GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_4" ]);    //"新手"    ""    "(主)"
			equip.txtMainRole.mouseEnabled = false;
			//人物门派属性描述
			equip.txtSchoolAttribute.mouseEnabled = false;
			equip.txtSchoolAttribute.text = setAttributeDescribe();
			equip.txtSubRole.mouseEnabled = false;
			equip.txtSubRole.text = GameCommonData.RolesListDic[GameCommonData.Player.Role.ViceJob.Job]+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_5" ];    //"(副)"
			
			if(GameCommonData.Player.Role.CurrentJob == 1)
			{
				equip.mcRole_0.gotoAndStop(1);
				equip.mcRole_1.gotoAndStop(2);
				(equip.txtMainRole as TextField).textColor=0x00ff00;
				(equip.txtSubRole as TextField).textColor=0xff0000;
				RolePropDatas.selectedPageIndex=1;
			}
			else
			{
				equip.mcRole_0.gotoAndStop(2);
				equip.mcRole_1.gotoAndStop(1);
				(equip.txtMainRole as TextField).textColor=0xff0000;
				(equip.txtSubRole as TextField).textColor=0x00ff00;
				RolePropDatas.selectedPageIndex=2;
			}
			//
			equip.mcCheckBox.gotoAndStop(1);
			//
			equip.txtPower.text = GameCommonData.Player.Role.Ene	+ "/" + GameCommonData.Player.Role.MaxEne;
			equip.txtEnergy.text	= GameCommonData.Player.Role.Vit + "/" + GameCommonData.Player.Role.MaxVit;
			equip.txtHp.text = Math.min(GameCommonData.Player.Role.HP,(GameCommonData.Player.Role.MaxHp + GameCommonData.Player.Role.AdditionAtt.MaxHP)) + "/" + (GameCommonData.Player.Role.MaxHp + GameCommonData.Player.Role.AdditionAtt.MaxHP);		
			equip.txtMp.text = 	Math.min(GameCommonData.Player.Role.MP,(GameCommonData.Player.Role.MaxMp + GameCommonData.Player.Role.AdditionAtt.MaxMP)) + "/" + (GameCommonData.Player.Role.MaxMp + GameCommonData.Player.Role.AdditionAtt.MaxMP);	
			equip.txtSp.text = Math.min(GameCommonData.Player.Role.SP,(GameCommonData.Player.Role.MaxSp  + GameCommonData.Player.Role.AdditionAtt.MaxSP)) + "/" + (GameCommonData.Player.Role.MaxSp  + GameCommonData.Player.Role.AdditionAtt.MaxSP);
			var expStr:uint = GameCommonData.Player.Role.Exp;
			if(expStr > (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4))
			{
				expStr = (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
			}
			equip.txtExp.text = expStr + "/" + (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
//			equip.txtExp.text = GameCommonData.Player.Role.Exp + "/" + (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
			equip.txtRoleExp.text = UIConstData.ExpDic[GameCommonData.Player.Role.Level];
			var add:uint = (this.curJob == GameCommonData.Player.Role.MainJob)?1000:2000;
			equip.txtProExp.text = UIConstData.ExpDic[int(this.curJob.Level) + add];
			//门派 属性描述
			//
			addLister();
			showProp();
			showLevelUp();
			showAddBtn();
		}
				
		/**
		 * 更新附加属性
		 *   */
		public function UpDateExtendAttribute(data:Object):void
		{
			if(this.curJob == GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob - 1])
			{
				equip[data.target].text = data.data;
			}
		}
		
		/**
		 * 更新属性
		 *   */
		public function UpDateAttribute(data:Object):void
		{
			if(!this.curJob) {
				this.curJob = GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob-1];
			}
			equip[data.target].text = data.data;
			if(data.target == "txtLevel")
			{
				equip.txtRoleExp.text = UIConstData.ExpDic[GameCommonData.Player.Role.Level];
				var expStr:uint = GameCommonData.Player.Role.Exp;
				if(expStr > (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4))
				{
					expStr = (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
				}
				equip.txtExp.text = expStr + "/" + (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
	//			equip.txtExp.text = GameCommonData.Player.Role.Exp + "/" + (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
//				var exp:String = GameCommonData.Player.Role.Exp + "/" + (UIConstData.ExpDic[GameCommonData.Player.Role.Level] * 4);
//				equip.txtExp.text = exp;
				showLevelUp();
			}
			if(data.target == "txtExp" || data.target == "txtJobLevel")
			{
				showLevelUp();
			}
			if(data.target == "txtPotential")
			{
				if(this.curJob == GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob-1])
				{
					equip.txtPotential.text = data.data;
					showAddBtn();
				}
			}
			if(data.target == "txtMainRole")
			{
				equip.txtSchoolAttribute.text = setAttributeDescribe();
			}
		}
				
		private function textFormat():TextFormat 
		{
			var tf:TextFormat = new TextFormat();
			tf.size = 12;
			tf.font = GameCommonData.wordDic[ "mod_rp_med_ui_pa_6" ];         //"楷体"
			return tf;
		}
		
		private function addLister():void
		{
			equip.mcRole_1.addEventListener(MouseEvent.CLICK, onSelectPage);
			equip.mcRole_0.addEventListener(MouseEvent.CLICK, onSelectPage);
			equip.mcRoleLevelUp.addEventListener(MouseEvent.CLICK, levelUpRole);
			equip.mcProLevelUp.addEventListener(MouseEvent.CLICK, levelUpPro);
			equip.btnAddStrong.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			equip.btnSubStrong.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			equip.btnAddSprite.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			equip.btnSubSprite.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			equip.btnAddPhysical.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			equip.btnSubPhysical.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			equip.btnAddConstant.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			equip.btnSubConstant.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			equip.btnAddMagic.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			equip.btnSubMagic.addEventListener(MouseEvent.MOUSE_DOWN, subPotential);
			equip.txtSure.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
			equip.btnModify.addEventListener(MouseEvent.MOUSE_DOWN, addPotential);
		}
		
		/**
		 * 人物门派属性描述 
		 * 
		 */		
		public static function setAttributeDescribe(type:int = 0):String
		{
			var txt:String = "";
			var cJob:uint = GameCommonData.Player.Role.CurrentJob;
			var jobId:uint = (GameCommonData.Player.Role.RoleList[cJob - 1] as RoleJob).Job;
			if(jobId == 1)
			{
				txt = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_1" ];//" 唐门    火属性";
			}
			else if(jobId == 2)
			{
				txt = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_2" ];//" 全真    冰属性";
			}
			else if(jobId == 4)
			{
				txt = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_3" ];//" 峨眉    冰属性";
			}
			else if(jobId == 8)
			{
				txt = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_4" ];//" 丐帮    毒属性";
			}
			else if(jobId == 16)
			{
				txt = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_5" ];//" 少林    玄属性";
			}
			else if(jobId == 32)
			{
				txt = GameCommonData.wordDic[ "mod_hero_med_skillM_setT_6" ];//" 点苍    火属性";
			}
			if(type == 1) //获得门派名称
			{
				txt = txt.substr(0,2);
			}
			return txt;
		} 
		
		/**
		 * 选中职业页
		 *   */
		private function onSelectPage(event:MouseEvent):void
		{
			switch(event.currentTarget.name)
			{
				case "mcRole_1":
					if(GameCommonData.Player.Role.ViceJob.Job == 0) return;	 
					if(equip.mcRole_1.currentFrame == 1) return;
					RolePropDatas.selectedPageIndex=2;
					this.curJob = GameCommonData.Player.Role.ViceJob;
					equip.mcRole_0.gotoAndStop(2);
					equip.mcRole_1.gotoAndStop(1);
					
				break;
				case "mcRole_0":
					RolePropDatas.selectedPageIndex=1;
					if(equip.mcRole_0.currentFrame == 1) return;
					this.curJob = GameCommonData.Player.Role.MainJob;
					equip.mcRole_0.gotoAndStop(1);
					equip.mcRole_1.gotoAndStop(2);	
				break;
			}
			equip.txtAddForce.visible = this.curJob.Points[0] == 0?false:true;
			equip.txtAddForce.text = "(+"+this.curJob.Points[0]+")";
			equip.txtAddSpiritPower.visible = this.curJob.Points[1] == 0?false:true;
			equip.txtAddSpiritPower.text = "(+"+this.curJob.Points[1]+")";
			equip.txtAddPhysical.visible = this.curJob.Points[2] == 0?false:true;
			equip.txtAddPhysical.text = "(+"+this.curJob.Points[2]+")";
			equip.txtAddConstant.visible = this.curJob.Points[3] == 0?false:true;
			equip.txtAddConstant.text = "(+"+this.curJob.Points[3]+")";
			equip.txtAddMagic.visible = this.curJob.Points[4] == 0?false:true;
			equip.txtAddMagic.text = "(+"+this.curJob.Points[4]+")";
			var add:uint = (this.curJob == GameCommonData.Player.Role.MainJob)?1000:2000;
			equip.txtProExp.text = UIConstData.ExpDic[int(this.curJob.Level) + add];
			showProp();
			showLevelUp();
			showAddBtn();
		}
		
		/**
		 * 显示升级按钮
		 *   */
		private function showLevelUp():void
		{
			if(!this.curJob) {
				this.curJob = GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob-1];
			}
			
			if(GameCommonData.Player.Role.Exp >= UIConstData.ExpDic[GameCommonData.Player.Role.Level] && GameCommonData.Player.Role.Level >= 25) {
				equip.mcRoleLevelUp.visible = true;
			} else {
				equip.mcRoleLevelUp.visible = false;
			}
			
			//
			var add:uint = (this.curJob == GameCommonData.Player.Role.MainJob)?1000:2000;
			if(GameCommonData.Player.Role.Exp >= UIConstData.ExpDic[int(this.curJob.Level) + add])
			{
				if(curJob.Job > 4000 || GameCommonData.Player.Role.Level < 25) {
					equip.mcProLevelUp.visible = false;
				} else {
					equip.mcProLevelUp.visible = true;
				}
			}
			else
			{
				equip.mcProLevelUp.visible = false;
			}
			//检查自动升级
			checkLevUp();
		}
		
		/** 25级之前自动升级 */
		public function checkLevUp():void
		{
			if(levUpLock == true) {	//已加锁
				return;
			}
			var roleLev:uint = GameCommonData.Player.Role.Level;
			if(roleLev >=  25) {
				return;
			}
			var mainJobLev:uint = GameCommonData.Player.Role.MainJob.Level;
			var expNow:uint 	= GameCommonData.Player.Role.Exp;
			if(roleLev == 10 && (GameCommonData.Player.Role.MainJob.Job > 0 && GameCommonData.Player.Role.MainJob.Job < 4096)) {				//人物等级小于11
				if(expNow >= UIConstData.ExpDic[roleLev]) {
					levUpLock = true;
					NetAction.LevelUp(0);
				}
			} else if(roleLev >= 11 && mainJobLev < 10) {	//职业等级小于10
				if(expNow >= UIConstData.ExpDic[1000+mainJobLev]) {
					levUpLock = true;
					NetAction.LevelUp(1);
				}
			} else if(roleLev >= 11){						//人物等级小于25
				if(expNow >= UIConstData.ExpDic[roleLev]) {
					levUpLock = true;
					NetAction.LevelUp(0);
				}
			}
		}
		
		/**
		 * 显示玩家属性
		 * 该属性页为当前职业的时候 会显示附加的点
		 * 不是当前职业 不显示附加点
		 *   */
		private function showProp():void
		{
			if(this.curJob != GameCommonData.Player.Role.RoleList[GameCommonData.Player.Role.CurrentJob - 1])
			{
				equip.txtForce.text = this.curJob.Force;
				equip.txtSpiritPower.text = this.curJob.SpiritPower;
				equip.txtPhysical.text = this.curJob.Physical;
				equip.txtConstant.text = this.curJob.Constant;
				equip.txtMagic.text = this.curJob.Magic;
				equip.txtPhyAttack.text = this.curJob.PhyAttack ;	
				equip.txtMagicAttack.text = this.curJob.MagicAttack;
				equip.txtPhyDef.text = this.curJob.PhyDef;
				equip.txtMagicDef.text = this.curJob.MagicDef;
				equip.txtHit.text = this.curJob.Hit;
				equip.txtHide.text = this.curJob.Dodge;
				equip.txtCrit.text = this.curJob.Crit;		
				equip.txtToughness.text = this.curJob.Toughness;	
			}
			else
			{
				equip.txtForce.text = this.curJob.Force + GameCommonData.Player.Role.AdditionAtt.Force;
				equip.txtSpiritPower.text = this.curJob.SpiritPower + GameCommonData.Player.Role.AdditionAtt.SpiritPower;
				equip.txtPhysical.text = this.curJob.Physical + GameCommonData.Player.Role.AdditionAtt.Physical;
				equip.txtConstant.text = this.curJob.Constant + GameCommonData.Player.Role.AdditionAtt.Constant;
				equip.txtMagic.text = this.curJob.Magic + GameCommonData.Player.Role.AdditionAtt.Magic;
				equip.txtPhyAttack.text = this.curJob.PhyAttack + GameCommonData.Player.Role.AdditionAtt.PhyAttack;	
				equip.txtMagicAttack.text = this.curJob.MagicAttack + GameCommonData.Player.Role.AdditionAtt.MagicAttack;
				equip.txtPhyDef.text = this.curJob.PhyDef + GameCommonData.Player.Role.AdditionAtt.PhyDef;
				equip.txtMagicDef.text = this.curJob.MagicDef + GameCommonData.Player.Role.AdditionAtt.MagicDef;
				equip.txtHit.text = this.curJob.Hit + GameCommonData.Player.Role.AdditionAtt.Hit;
				equip.txtHide.text = this.curJob.Dodge + GameCommonData.Player.Role.AdditionAtt.Dodge;
				equip.txtCrit.text = this.curJob.Crit + GameCommonData.Player.Role.AdditionAtt.Crit;		
				equip.txtToughness.text = this.curJob.Toughness + GameCommonData.Player.Role.AdditionAtt.Toughness;
			}
			var maxHp:uint;
			if(RolePropDatas.selectedPageIndex==1){
//				trace("主："+GameCommonData.Player.Role.MainJob.Potential);
				equip.txtPotential.text = GameCommonData.Player.Role.MainJob.Potential; 
				equip.txtJobLevel.text =  GameCommonData.Player.Role.MainJob.Level+GameCommonData.wordDic[ "mod_rp_med_ui_pa_7" ];    //"级"
				maxHp=GameCommonData.Player.Role.MainJob.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP;
				equip.txtHp.text =	Math.min(GameCommonData.Player.Role.HP,maxHp)+"/"+maxHp;
				equip.txtMp.text =	Math.min(GameCommonData.Player.Role.MP,GameCommonData.Player.Role.MainJob.MaxMp)+"/"+GameCommonData.Player.Role.MainJob.MaxMp;
				equip.txtSp.text =	Math.min(GameCommonData.Player.Role.SP,GameCommonData.Player.Role.MainJob.MaxSP)+"/"+GameCommonData.Player.Role.MainJob.MaxSP;
			}else if(RolePropDatas.selectedPageIndex==2){
//				trace("副："+GameCommonData.Player.Role.ViceJob.Potential);
				equip.txtPotential.text = GameCommonData.Player.Role.ViceJob.Potential; 
				equip.txtJobLevel.text = GameCommonData.Player.Role.ViceJob.Level+GameCommonData.wordDic[ "mod_rp_med_ui_pa_7" ];    //"级"
				maxHp=GameCommonData.Player.Role.ViceJob.MaxHp+GameCommonData.Player.Role.AdditionAtt.MaxHP;
				equip.txtHp.text =	Math.min(GameCommonData.Player.Role.HP,maxHp)+"/"+maxHp;
				equip.txtMp.text =	Math.min(GameCommonData.Player.Role.MP,GameCommonData.Player.Role.ViceJob.MaxMp)+"/"+GameCommonData.Player.Role.ViceJob.MaxMp;
				equip.txtSp.text =	Math.min(GameCommonData.Player.Role.SP,GameCommonData.Player.Role.ViceJob.MaxSP)+"/"+GameCommonData.Player.Role.ViceJob.MaxSP;
			}
			
		}
		
		/** 玩家升级  */
		private function levelUpRole(event:MouseEvent):void
		{
			if(RoleLevelUpCondition.getIsRoleLevelUp(GameCommonData.Player.Role.Level) == false) return;			//没达到升级条件
			//60级限制
//			if(GameCommonData.Player.Role.Level >= 60)
//			{
//				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:"你已达到等级上限", color:0xffff00});
//				return;
//			}
			//
			NetAction.LevelUp(0);
			//通知新手引导系统
//			if(NewerHelpData.newerHelpIsOpen && NewerHelpData.curType == 9 && NewerHelpData.curStep == 2) {
//				UIFacade.GetInstance(UIFacade.FACADEKEY).clickLevUpRole();
//			}
		}
		
		/** 职业升级  */
		private function levelUpPro(event:MouseEvent):void
		{
			// 1:主职业 2：副职业
			var type:uint = (this.curJob == GameCommonData.Player.Role.MainJob)?1:2;
			var mainJobLevel:uint=GameCommonData.Player.Role.MainJob.Level;
			var viceJobLevel:uint=GameCommonData.Player.Role.ViceJob.Level;
			var level:uint=GameCommonData.Player.Role.Level;
			if(type==1){
				if(RoleLevelUpCondition.getIsJopLevelUp(GameCommonData.Player.Role.MainJob.Level) == false) return;
				if(mainJobLevel>=level+5){
					UIFacade.UIFacadeInstance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_ui_pa_8" ], color:0xffff00});  //"主职业等级不能高于玩家等级5级以上"
					return ;
				}
			}else if(type==2){
				if(RoleLevelUpCondition.getIsJopLevelUp(GameCommonData.Player.Role.ViceJob.Level) == false) return;
				if(viceJobLevel>=level+5){
					UIFacade.UIFacadeInstance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_med_ui_pa_9" ], color:0xffff00});   //"副职业等级不能高于玩家等级5级以上"
					return ;
				}
				
			}
			
			NetAction.LevelUp(type);
		}
		
		/** 显示加点按钮  */
		private function showAddBtn():void
		{
			if(!this.curJob) return;
			if(this.curJob.Potential == 0 && testAddIsNull())
			{
				equip.btnAddStrong.visible = false;
				equip.btnSubStrong.visible = false;
				equip.btnAddSprite.visible = false;
				equip.btnSubSprite.visible = false;
				equip.btnAddPhysical.visible = false;
				equip.btnSubPhysical.visible = false;
				equip.btnAddConstant.visible = false;
				equip.btnSubConstant.visible = false;
				equip.btnAddMagic.visible = false;
				equip.btnSubMagic.visible = false;
				equip.txtSure.visible = false;
				equip.btnModify.visible = false;
			}
			else
			{
				equip.btnAddStrong.visible = true;
				equip.btnSubStrong.visible = true;
				equip.btnAddSprite.visible = true;
				equip.btnSubSprite.visible = true;
				equip.btnAddPhysical.visible = true;
				equip.btnSubPhysical.visible = true;
				equip.btnAddConstant.visible = true;
				equip.btnSubConstant.visible = true;
				equip.btnAddMagic.visible = true;
				equip.btnSubMagic.visible = true;
				equip.txtSure.visible = true;
				equip.txtSure.mouseEnabled = false;
				equip.btnModify.visible = true;
			}
		}
		
		/**
		 * 未提交加点的时候，有一个临时的数组保存之前加过的点数，
		 * 初始化的时候判断是否存在加过的点
		 *   */		
		private function testAddIsNull():Boolean
		{
			for(var i:int = 0; i<this.curJob.Points.length; i++)
			{
				if(this.curJob.Points[i] != 0)
				{
					return false;
				}
			}
			return true;
		}
		
		//加点按钮处理
		private function addPotential(event:MouseEvent):void
		{
			(event.currentTarget as DisplayObject).addEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			intervalId = setTimeout(addAll , 1000 * 2 , event.target.name);
		}
		//减点按钮处理
		private function subPotential(event:MouseEvent):void
		{
			(event.currentTarget as DisplayObject).addEventListener(MouseEvent.MOUSE_UP , stopAddPotential);
			intervalId = setTimeout(subAll , 1000 * 2 , event.target.name);
		}
		private function handlPotential(name:String):void
		{
			switch(name)
			{
				case "btnAddStrong":
					setPotential(equip.txtAddForce, true, 0);
				break;	
				case "btnSubStrong":
					setPotential(equip.txtAddForce, false, 0);
				break;
				case "btnAddSprite":
					setPotential(equip.txtAddSpiritPower, true, 1);
				break;	
				case "btnSubSprite":
					setPotential(equip.txtAddSpiritPower, false, 1);
				break;
				case "btnAddPhysical":
					setPotential(equip.txtAddPhysical, true, 2);
				break;	
				case "btnSubPhysical":
					setPotential(equip.txtAddPhysical, false, 2);
				break;
				case "btnAddConstant":
					setPotential(equip.txtAddConstant, true, 3);
				break;	
				case "btnSubConstant":
					setPotential(equip.txtAddConstant, false, 3);
				break;
				case "btnAddMagic":
					setPotential(equip.txtAddMagic, true, 4);
				break;	
				case "btnSubMagic":
					setPotential(equip.txtAddMagic, false, 4);
				break;	
				case "btnModify":
					modifyPotential();
				break;
			}
		}
		private function stopAddPotential(e:MouseEvent):void
		{
			clearTimeout(intervalId);
			handlPotential(e.target.name);
		}
		/** 点数全加满 */
		private function addAll(name:String):void
		{
			var total:int = this.curJob.Potential;
			for(var i:int = 0; i < total; i++)
			{
				handlPotential(name);
			}
		}
		/** 点数全减完 */
		private function subAll(name:String):void
		{
			var total:int;
			switch(name)
			{
				case "btnSubStrong":															//力量 减点
					total = this.curJob.Points[0];
				break;
				case "btnSubSprite":															//灵力 减点
					total = this.curJob.Points[1];
				break;
				case "btnSubPhysical":															//体力 减点
					total = this.curJob.Points[2];
				break;
				case "btnSubConstant":															//定力 减点
					total = this.curJob.Points[3];
				break;
				case "btnSubMagic":																//身法 减点
					total = this.curJob.Points[4];
				break;
			}
			for(var i:int = 0; i < total; i++)
			{
				handlPotential(name);
			}
		}
		
		//手动加点
		private function setPotential(target:TextField, bool:Boolean, index:uint):void
		{
			if(bool)
			{
				this.curJob.Potential--;
				if(this.curJob.Potential < 0)
				{
					this.curJob.Potential = 0;
					return;
				}
				this.curJob.Points[index]++;
			}
			else
			{
				if(this.curJob.Points[index] == 0) return;
				this.curJob.Potential++;
				this.curJob.Points[index]--;
				if(this.curJob.Points[index] < 0)
				{
					this.curJob.Potential--;
					return;
				}
			}			
			target.visible = (this.curJob.Points[index] == 0)?false:true;
			target.text = "(+"+this.curJob.Points[index]+")";
			equip.txtPotential.text = this.curJob.Potential;
		}
		
		//提交加点
		private function modifyPotential():void
		{
			for(var i:int = 0; i<this.curJob.Points.length; i++)
			{
				if(this.curJob.Points[i] == 0) continue;
				var type:uint = i+1 + (GameCommonData.Player.Role.MainJob == this.curJob?0:5);
				NetAction.AddPotential(type, this.curJob.Points[i]);
			}
			var end:uint = GameCommonData.Player.Role.MainJob == this.curJob?11:12;
			equip.txtAddForce.text = "";
			equip.txtAddSpiritPower.text = "";
			equip.txtAddPhysical.text = "";
			equip.txtAddConstant.text = "";
			equip.txtAddMagic.text = "";
			NetAction.AddPotential(end);
			this.curJob.Points = [0,0,0,0,0];
		}
		
		//设置动态文本MouseEnable
		private function setMouseEnable():void
		{
			equip.txtForce.mouseEnabled = false;
			equip.txtSpiritPower.mouseEnabled = false;
			equip.txtPhysical.mouseEnabled = false;
			equip.txtConstant.mouseEnabled = false;
			equip.txtMagic.mouseEnabled = false;
			equip.txtPhyAttack.mouseEnabled = false
			equip.txtMagicAttack.mouseEnabled = false
			equip.txtPhyDef.mouseEnabled = false
			equip.txtMagicDef.mouseEnabled = false
			equip.txtHit.mouseEnabled = false
			equip.txtHide.mouseEnabled = false
			equip.txtCrit.mouseEnabled = false
			equip.txtPotential.mouseEnabled = false
			equip.txtToughness.mouseEnabled = false
			equip.txtJobLevel.mouseEnabled = false
		}
	}
}