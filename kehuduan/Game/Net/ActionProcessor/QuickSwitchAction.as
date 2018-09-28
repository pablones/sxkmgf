package Net.ActionProcessor
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.AutoPlay.command.AutoPlayEventList;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.HeroSkill.Command.SkillIsAutoPlayCommand;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.NPCChat.Proxy.DialogConstData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	import GameUI.MouseCursor.RepeatRequest;
	import GameUI.UIUtils;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.GameAction;
	import Net.Protocol;
	
	import flash.utils.ByteArray;

	public class QuickSwitchAction extends GameAction
	{
		
		public function QuickSwitchAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		public override function Processor(bytes:ByteArray):void{
			bytes.position=4;
			var obj:Object = new Object;
			var expandObj:Object=new Object();
			obj.nAction = bytes.readByte();   //1;
			if(obj.nAction==1){
				obj.FeelSwitch = bytes.readByte();   //0:隐藏  1：显示
				obj.MusicSwitch = bytes.readByte();  //0：关  1：开
				obj.OvlSwitch = bytes.readByte();    //音效  
				obj.idUser = bytes.readUnsignedInt();  //玩家ID
				
				//主职业
				obj.key1 = bytes.readUnsignedInt();    //物品ID号 
				obj.key2 = bytes.readUnsignedInt();
				obj.key3 = bytes.readUnsignedInt();
				obj.key4 = bytes.readUnsignedInt();
				obj.key5 = bytes.readUnsignedInt();
				obj.key6 = bytes.readUnsignedInt();
				obj.key7 = bytes.readUnsignedInt();
				obj.key8 = bytes.readUnsignedInt();
				obj.keyF1 = bytes.readUnsignedInt();
				obj.keyF2 = bytes.readUnsignedInt();
				obj.keyF3 = bytes.readUnsignedInt();
				obj.keyF4 = bytes.readUnsignedInt();
				obj.keyF5 = bytes.readUnsignedInt();
				obj.keyF6 = bytes.readUnsignedInt();
				obj.keyF7 = bytes.readUnsignedInt();
				bytes.readUnsignedInt();
				obj.keyF8 = 0;
				
				//副职业
				expandObj.key1 = bytes.readUnsignedInt();    
				expandObj.key2 = bytes.readUnsignedInt();
				expandObj.key3 = bytes.readUnsignedInt();
				expandObj.key4 = bytes.readUnsignedInt();
				expandObj.key5 = bytes.readUnsignedInt();
				expandObj.key6 = bytes.readUnsignedInt();
				expandObj.key7 = bytes.readUnsignedInt();
				expandObj.key8 = bytes.readUnsignedInt();
				expandObj.keyF1 = bytes.readUnsignedInt();
				expandObj.keyF2 = bytes.readUnsignedInt();
				expandObj.keyF3 = bytes.readUnsignedInt();
				expandObj.keyF4 = bytes.readUnsignedInt();
				expandObj.keyF5 = bytes.readUnsignedInt();
				expandObj.keyF6 = bytes.readUnsignedInt();
				expandObj.keyF7 = bytes.readUnsignedInt();
				bytes.readUnsignedInt();
				expandObj.keyF8 = 0;
				
				GameCommonData.BigMapMaskLow = bytes.readUnsignedInt();   //低位
				GameCommonData.BigMapMaskHi = bytes.readUnsignedInt();	  //高位
				obj.DialogStatus = bytes.readUnsignedInt();   //对话掩码
				GameCommonData.dialogStatus=obj.DialogStatus;   //将快捷栏的展开方式保存到2位
				
				ChatData.channelSign = bytes.readUnsignedInt();	//聊天自定义频道位
				
				var firSkill:uint   = bytes.readUnsignedInt();	//主职业技能打钩位
				var secSkill:uint   = bytes.readUnsignedInt();	//副职业技能打钩位
				var uinSkill:uint   = bytes.readUnsignedInt();	//帮派技能打钩位
				var sysSetting:uint = bytes.readUnsignedInt();	//系统设置打钩位
				
				SkillData.aMainAutoIndex   = UIUtils.IntegerBitwiseAnd(firSkill);
				SkillData.aViceAutoIndex   = UIUtils.IntegerBitwiseAnd(secSkill);
				SkillData.aUnityAutoIndex  = UIUtils.IntegerBitwiseAnd(uinSkill);
				SystemSettingData._dataArr = UIUtils.IntegerBitwiseAnd(sysSetting);
				
				facade.registerCommand(SkillIsAutoPlayCommand.NAME, SkillIsAutoPlayCommand);
				sendNotification(SkillIsAutoPlayCommand.NAME);					//初始化技能面板自动打钩设置
				sendNotification(SystemSettingData.INIT_SYSTEM_SETTING_DATA);	//初始化系统设置面板打钩设置
				
				RepeatRequest.getInstance().quickKeyCount+=1;     //收到快捷键
				
				if(GameCommonData.Player.Role.CurrentJob==1){
					sendNotification(EventList.RECEIVE_QUICKBAR_MSG,obj);
				}else if(GameCommonData.Player.Role.CurrentJob==2){
					sendNotification(EventList.RECEIVE_QUICKBAR_MSG,expandObj);
				}
				
				QuickBarData.getInstance().mainJobQuickKey=obj;
				QuickBarData.getInstance().viceJobQuickKey=expandObj;
				
				if((obj.DialogStatus & 1)==0){
					sendNotification(EventList.DO_FIRST_TIP, {comfrim:sureDoNoting, info:DialogConstData.getInstance().getTipDesByType(1), title:GameCommonData.wordDic[ "net_ap_qsa_proc_1" ], comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_6" ], canDrag:1, extendsFn:sureFun, width:260});
																																			   //"欢迎进入游戏世界"                                             "我知道了"
					if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 18);	//通知新手指导
				}
				this.getPetSkillCdSend();
				sendNotification(ChatEvents.INIT_CHAT_CHANNEL_VIEW);		//初始化聊天自定义频道位
			}else if(obj.nAction==2){
				bytes.readByte();  
				bytes.readByte(); 
				bytes.readByte();     
				bytes.readUnsignedInt();  
				
				
				obj.hp0 = bytes.readUnsignedInt();    
				obj.hp1 = bytes.readUnsignedInt();
				obj.hp2 = bytes.readUnsignedInt();
				obj.mp0 = bytes.readUnsignedInt();
				obj.mp1 = bytes.readUnsignedInt();
				obj.mp2 = bytes.readUnsignedInt();
				obj.petHp0 = bytes.readUnsignedInt();
				obj.petHp1 = bytes.readUnsignedInt();
				obj.petHp2 = bytes.readUnsignedInt();
				obj.limit0 = bytes.readUnsignedInt();
				obj.limit1 = bytes.readUnsignedInt();
				obj.limit2 = bytes.readUnsignedInt();
				sendNotification(AutoPlayEventList.AUTOPLAY_PROCESS_COMMAND,obj);
			}
				
		}
		
		public function sureDoNoting():void
		{
			
		}
		
		public function sureFun():void{
			GameCommonData.dialogStatus=GameCommonData.dialogStatus | 1;
			sendNotification(EventList.DONE_FIRST_TIP);
			sendNotification(EventList.SEND_QUICKBAR_MSG);
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.REMOVE_NEWER_HELP_BY_TYPE, 18);	// 通知新手指导
		}
		 
		/**
		 *  向服务器发送消息，请求宠物技能CD
		 * 
		 */		
		protected function getPetSkillCdSend():void{
		
			if(GameCommonData.Player.Role.UsingPet==null)return;
			var obj:Object = new Object();
			var parm:Array = [];
			parm.push(GameCommonData.Player.Role.UsingPet.Id);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(289);							//进入地图
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		}
		
	}
}