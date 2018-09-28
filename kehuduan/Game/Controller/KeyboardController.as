package Controller
{
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Bag.Proxy.NetAction;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.IdentifyingCode.Data.IdentifyingCodeData;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.MainSence.Mediator.MainSenceMediator;
	import GameUI.UICore.UIFacade;
	import GameUI.View.items.UseItem;
	
	import Net.ActionProcessor.OperateItem;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Skill.GameSkillLevel;
	
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	
	public class KeyboardController
	{
		public function KeyboardController()
		{
			GameCommonData.GameInstance.KeyUp   = onKeyUp;
			GameCommonData.GameInstance.KeyDown = onKeyDown;
		}
		
		protected function useQuickKey(useItem:UseItem):void {
			if(useItem==null || GameCommonData.isFocusIn)
			{
				return;
			}
			if(!CooldownController.getInstance().cooldownReady(useItem.Type))
			{
//				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:"技能冷却中", color:0xffff00});//
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "con_keyboard_useQuick" ], color:0xffff00});//技能冷却中
				return;
			}
			if(QuickBarData.getInstance().isEnableUseTheItem(useItem)){
				if(useItem.Type>=300000 && QuickBarData.getInstance().getItemIdFromBag(useItem)>0){
					if(useItem.Type >= 320000 && useItem.Type < 340000) {
						if(GameCommonData.Player.Role.UsingPet) {
							NetAction.presentRoseToFriend(QuickBarData.getInstance().getItemIdFromBag(useItem), GameCommonData.Player.Role.UsingPet.Id,0);
						}else{
							GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["con_key_useQuick_1"], color:0xffff00}); // "当前没有出战宠物"
						}
						return;
					}else{
						NetAction.UseItem(OperateItem.USE, 1, 0,QuickBarData.getInstance().getItemIdFromBag(useItem));
					}		 
				}else{
					var skillType:int = useItem.Type;
					PlayerController.UseSkill(skillType);
					var skilllevel:GameSkillLevel = GameCommonData.Player.Role.SkillList[skillType] as GameSkillLevel;
					var role:GameRole = GameCommonData.Player.Role;
					var boo:Boolean;
					if(skillType == 1139 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		润脉	
					{
						boo = true;
					}
					else if(skillType == 1102 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		强身
					{
						boo = true;
					}
					else if(skillType == 1301 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		云体		
					{
						boo = true;
					}
					else if(skillType == 1121 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		灵蕴	
					{
						boo = true;
					}
					else if(skillType == 1401 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		定军	
					{
						boo = true;
					}
					else if(skillType == 1201 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)	//		蛟龙劲	
					{
						boo = true;
					}
					else if(skillType == 1303 && skilllevel.GetMP <= role.MP && skilllevel.gameSkill.SP <= role.SP)//		腐骨矢
					{	
						if (GameCommonData.TargetAnimal && TargetController.IsAttack(GameCommonData.TargetAnimal))
						{
							boo = true;
						}
					}
					if(boo)
					{
//						QuickBarData.getInstance().skillsInQuickAddCd(skillType,1000);	//触发公共CD
						CooldownController.getInstance().triggerGCD();
					}
				}		
			}
		}

		private function onKeyUp(e:KeyboardEvent):void
		{
			////下面这些都是正式用的，如需写测试代码，请写到标注有“测试”行后面
			if(e.keyCode == Keyboard.ENTER)
			{
				GameCommonData.UIFacadeIntance.GetKeyCode(e.keyCode);
				return;
			}
			
			if(!UIConstData.KeyBoardCanUse) return;		//死亡、询问框等状态时 须禁用快捷键
			
			//无需焦点判断的快捷键 
			if(e.keyCode==Keyboard.ESCAPE && !IdentifyingCodeData.isShowView)
			{
				//取消地效果技能
				FloorEffectController.ClearFloorEffect();
				GameCommonData.UIFacadeIntance.GetKeyCode(e.keyCode);
				return;
			}
			if(e.keyCode==Keyboard.TAB) {
				GameCommonData.UIFacadeIntance.GetKeyCode(e.keyCode);
				return;
			}
			/////需要焦点判断的快捷键
			if(GameCommonData.isFocusIn) return;
			
			var keyCode:uint = e.keyCode;
			
			if( keyCode == 67 || keyCode == 83 || keyCode == 88 || keyCode == 81 || keyCode == 69 || keyCode == 70 || keyCode == 71 || keyCode == 84 || keyCode == 65
			     || keyCode == 72 || keyCode == 86 || keyCode == 75 || keyCode == 87 ) 
			{
				GameCommonData.UIFacadeIntance.GetKeyCode(keyCode);
				return;
			}
			if(e.keyCode==49)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[0]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[0]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;	
			}
			if(e.keyCode==50)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[1]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[1]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode==51)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[2]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[2]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode==52)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[3]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[3]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode==53)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[4]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[4]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode==54)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[5]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[5]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode==55)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[6]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[6]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode==56)
			{
				if(e.ctrlKey){
					this.useQuickKey(QuickBarData.getInstance().expandKeyDic[7]);
				}else{
					this.useQuickKey(QuickBarData.getInstance().quickKeyDic[7]);
				}
				GameCommonData.UIFacadeIntance.sendNotification(EventList.REMOVEQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;
			}
			if(e.keyCode == 77)
			{
				GameCommonData.UIFacadeIntance.showBigMap();
				return;
			}
			if(e.keyCode == 192)
			{
				//挂机不能选怪
				if(!GameCommonData.Player.IsAutomatism)
				{
					TargetController.GetTarget();
//					GameCommonData.Scene.Attack();
				}
				return;
			}
			
			//按住空格捡包
			if(e.keyCode == Keyboard.SPACE)
			{
				GameCommonData.UIFacadeIntance.GetKeyCode(keyCode);
				return;
			}
			
			//屏蔽玩家
		    if(e.keyCode == Keyboard.F12)
			{
//				ScreenController.SetScreen();
				UIFacade.UIFacadeInstance.sendNotification( ChgLineData.ONE_KEY_HIDE );
				return;
			}
						
			/** 快捷键R 挑战 */
			if(e.keyCode == 82)
			{
				var mainSenceMediator:MainSenceMediator = GameCommonData.UIFacadeIntance.retrieveMediator( MainSenceMediator.NAME ) as MainSenceMediator;
				mainSenceMediator.useRightQuickBtn(3);
			}
			
			/** 快捷鍵B 快速挂機 */
			if( e.keyCode == 66 )
			{
				GameCommonData.UIFacadeIntance.sendNotification( AutoPlayEventList.START_AUTO_PLAY );
			}
			
/////////////////////////////////////////////////////////
//以下是测试的，测试完成后须删除

//			if(e.keyCode == 221)
//			{
//				PlayerController.BeginAutomatism();
//			} 
			
//			if(e.keyCode == Keyboard.CONTROL)
//			{
//				UIConstData.ControlIsDown = false;
//			}  
		/* 	if(e.keyCode==Keyboard.F1)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[0]);
			}
			if(e.keyCode==Keyboard.F2)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[1]);
			}
			if(e.keyCode==Keyboard.F3)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[2]);
			}
			if(e.keyCode==Keyboard.F4)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[3]);
			}
			if(e.keyCode==Keyboard.F5)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[4]);
			}
			if(e.keyCode==Keyboard.F6)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[5]);
			}
			if(e.keyCode==Keyboard.F7)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[6]);
			}
			if(e.keyCode==Keyboard.F8)
			{
				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[7]);
			} */
			
//			if(e.keyCode == 110)
//			{
//				if(DebugInfo.txt.visible)
//				{
//					DebugInfo.txt.visible = false;
//				}
//				else
//				{
//					DebugInfo.txt.visible = true;
//				}
//			}
			
//			if(e.keyCode == 87)
//			{
// 				PlayerController.UseSkill(9000);
//			}
//			
// J 人物 自身丢 群     U 丢大群  I 宠物自身 O 宠物丢群
//			if(e.keyCode == 89)
//			{
// 				PlayerController.UseSkill(1106);
//			}
//			
//			 if(e.keyCode == 81)
//			{
// 				PlayerController.UseSkill(1101);
//			}
//			
//			 if(e.keyCode == 87)
//			{
// 				PlayerController.UseSkill(1102);
//			}
//			
//			if(e.keyCode == 69)
//			{
// 				PlayerController.UseSkill(1103);
//			}
//			
//		    if(e.keyCode == 82)
//			{
// 				PlayerController.UseSkill(1104);
//			}
//
//		    if(e.keyCode == 84)
//			{
// 				PlayerController.UseSkill(1105);
//			}
//			
//			 if(e.keyCode == 73)
//			{
// 				PlayerController.UseSkill(9443);
//			}
//			
//			if(e.keyCode == 79)
//			{
// 				PlayerController.UseSkill(9442);
//			}

//
//
//			if(e.keyCode==Keyboard.F9)
//			{
//				Zippo.PlayerDie();
//			}
//			if(e.keyCode==Keyboard.F8)
//			{
//				Zippo.PlayerRelive(0);
//			}
//			if(e.keyCode == 90)
//			{
//				 PlayerController.PlayerUseSkill(1101);
//			}
//			
//			if(e.keyCode == 88)
//			{
//				 PlayerController.PlayerUseSkill(1302);
//			}
//				
//			if(e.keyCode == 68)
//			{
//				 PlayerController.PlayerUseSkill(1136);
//			}
//			
//			if(e.keyCode == 86)
//			{
//				 PlayerController.PlayerUseSkill(7018);
//			}
//			
//			if(e.keyCode == 66)
//			{
//				 PlayerController.PlayerUseSkill(4202);
//			}
//			
//			if(e.keyCode == 78)
//			{
//				 PlayerController.PlayerUseSkill(1208);
//			}
			
//	       if(e.keyCode==Keyboard.F2)
//			{
//				PlayerSkinsController.SetMount(4595);
//			
//			}
			
//			if(e.keyCode==Keyboard.F1)
//			{
//				
//			
//				
//				PlayerSkinsController.SetMount(4356);
////				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[0]);
////				PlayerSkinsController.SetSkin(GameElementSkins.EQUIP_MOUNT,70000,GameCommonData.Player);
////				GameCommonData.Player.SetMoveSpend(16,7);
//			}
//			if(e.keyCode==Keyboard.F2)
//			{
//				this.useQuickKey(QuickBarData.getInstance().expandKeyDic[1]);
//				PlayerSkinsController.RemoveSkin(GameElementSkins.EQUIP_MOUNT,GameCommonData.Player);
//				GameCommonData.Player.SetMoveSpend(16,5);
//			}
//			
//			if(e.keyCode == 72)
//			{
//				PetController.PetUseSkill(1101);
//			}
//			
//			if(e.keyCode == 74)
//			{
//				PlayerController.PlayerUseSkill(1303);
//			}

		}
	
		private function onKeyDown(e:KeyboardEvent):void
		{
			////下面这些都是正式用的，如需写测试代码，请写到标注有“测试”行后面
			
			if(!UIConstData.KeyBoardCanUse) return;		//死亡、询问框等状态时 须禁用快捷键
			
			//给技能栏快捷键添加滤镜
			if(GameCommonData.isFocusIn) return;
			var keyCoke:uint = e.keyCode;
			if(keyCoke == 49 || keyCoke == 50 || keyCoke == 51 || keyCoke == 52 || keyCoke == 53 || keyCoke == 54 || keyCoke == 55 || keyCoke == 56)
			{
				GameCommonData.UIFacadeIntance.sendNotification(EventList.ADDQUICKFLOW,{keyValue:e.keyCode,isCtrlKey:e.ctrlKey});
				return;	
			}
			/*  if(keyCoke == Keyboard.F1)
			{
				GameCommonData.UIFacadeIntance.sendNotification(TaskCommandList.SHOW_VIT_PANEL);
				GameCommonData.UIFacadeIntance.sendNotification(TaskCommandList.SHOW_TASKEXPAND_PANEL,2);
			}  */
			
			/////////////////////////////////////////////////////////
			//以下是测试的，测试完成后须删除
			
//			if(e.keyCode == Keyboard.CONTROL)
//			{
//				UIConstData.ControlIsDown = true;
//			}
//			if(e.keyCode==105)
//			{
//				GameCommonData.UIFacadeIntance.GetKeyCode(e.keyCode);
//				return;
//			}
//			if(e.keyCode == 104)
//			{
//				GameCommonData.UIFacadeIntance.GetKeyCode(e.keyCode);
//				return;
//			} 
			
		}
		
	}
}