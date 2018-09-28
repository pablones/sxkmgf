package Net.ActionSend
{
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.MainSence.Data.QuickBarData;
	import GameUI.Modules.SystemSetting.data.SystemSettingData;
	import GameUI.UIUtils;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class QuickSwitchSend
	{
		public static function sendMsg(param:Array):void{
			
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			//todo 你要发送的内容
			sendBytes.writeShort(172);	//长度   156
			sendBytes.writeShort(1066); //消息类型
			sendBytes.writeByte(param.shift());//nAction
			sendBytes.writeByte(GameCommonData.Player.Role.IsShowFeel);//FeelSwitch   0:心情显示  1：不显示
			param.shift();
			sendBytes.writeByte(param.shift());//musicSwitch
			sendBytes.writeByte(param.shift());//OvlSwitch
			sendBytes.writeUnsignedInt(param.shift());//userid
			
			var key:Array=param.shift();
			var keyF:Array=param.shift();
			var viceJob:Object=QuickBarData.getInstance().viceJobQuickKey;
			var mainJob:Object=QuickBarData.getInstance().mainJobQuickKey;
			if(GameCommonData.Player.Role.CurrentJob==1){
				sendBytes.writeUnsignedInt(key.shift());//key1
				sendBytes.writeUnsignedInt(key.shift());//key2
				sendBytes.writeUnsignedInt(key.shift());//key3
				sendBytes.writeUnsignedInt(key.shift());//key4
				sendBytes.writeUnsignedInt(key.shift());//key5
				sendBytes.writeUnsignedInt(key.shift());//key6
				sendBytes.writeUnsignedInt(key.shift());//key7
				sendBytes.writeUnsignedInt(key.shift());//key8
				sendBytes.writeUnsignedInt(keyF.shift());//keyF1
				sendBytes.writeUnsignedInt(keyF.shift());//keyF2
				sendBytes.writeUnsignedInt(keyF.shift());//keyF3
				sendBytes.writeUnsignedInt(keyF.shift());//keyF4
				sendBytes.writeUnsignedInt(keyF.shift());//keyF5
				sendBytes.writeUnsignedInt(keyF.shift());//keyF6
				sendBytes.writeUnsignedInt(keyF.shift());//keyF7
				sendBytes.writeUnsignedInt(keyF.shift());//keyF8
				
				sendBytes.writeUnsignedInt(viceJob.key1);//key1
				sendBytes.writeUnsignedInt(viceJob.key2);//key2
				sendBytes.writeUnsignedInt(viceJob.key3);//key2
				sendBytes.writeUnsignedInt(viceJob.key4);//key4
				sendBytes.writeUnsignedInt(viceJob.key5);//key5
				sendBytes.writeUnsignedInt(viceJob.key6);//key6
				sendBytes.writeUnsignedInt(viceJob.key7);//key7
				sendBytes.writeUnsignedInt(viceJob.key8);//key8
				sendBytes.writeUnsignedInt(viceJob.keyF1);//keyF1
				sendBytes.writeUnsignedInt(viceJob.keyF2);//keyF2
				sendBytes.writeUnsignedInt(viceJob.keyF3);//keyF3
				sendBytes.writeUnsignedInt(viceJob.keyF4);//keyF4
				sendBytes.writeUnsignedInt(viceJob.keyF5);//keyF5
				sendBytes.writeUnsignedInt(viceJob.keyF6);//keyF6
				sendBytes.writeUnsignedInt(viceJob.keyF7);//keyF7
				sendBytes.writeUnsignedInt(viceJob.keyF8);//keyF8
			}else{
				
				sendBytes.writeUnsignedInt(mainJob.key1);//key1
				sendBytes.writeUnsignedInt(mainJob.key2);//key2
				sendBytes.writeUnsignedInt(mainJob.key3);//key2
				sendBytes.writeUnsignedInt(mainJob.key4);//key4
				sendBytes.writeUnsignedInt(mainJob.key5);//key5
				sendBytes.writeUnsignedInt(mainJob.key6);//key6
				sendBytes.writeUnsignedInt(mainJob.key7);//key7
				sendBytes.writeUnsignedInt(mainJob.key8);//key8
				sendBytes.writeUnsignedInt(mainJob.keyF1);//keyF1
				sendBytes.writeUnsignedInt(mainJob.keyF2);//keyF2
				sendBytes.writeUnsignedInt(mainJob.keyF3);//keyF3
				sendBytes.writeUnsignedInt(mainJob.keyF4);//keyF4
				sendBytes.writeUnsignedInt(mainJob.keyF5);//keyF5
				sendBytes.writeUnsignedInt(mainJob.keyF6);//keyF6
				sendBytes.writeUnsignedInt(mainJob.keyF7);//keyF7
				sendBytes.writeUnsignedInt(mainJob.keyF8);//keyF8
				
				sendBytes.writeUnsignedInt(key.shift());//key1
				sendBytes.writeUnsignedInt(key.shift());//key2
				sendBytes.writeUnsignedInt(key.shift());//key3
				sendBytes.writeUnsignedInt(key.shift());//key4
				sendBytes.writeUnsignedInt(key.shift());//key5
				sendBytes.writeUnsignedInt(key.shift());//key6
				sendBytes.writeUnsignedInt(key.shift());//key7
				sendBytes.writeUnsignedInt(key.shift());//key8
				sendBytes.writeUnsignedInt(keyF.shift());//keyF1
				sendBytes.writeUnsignedInt(keyF.shift());//keyF2
				sendBytes.writeUnsignedInt(keyF.shift());//keyF3
				sendBytes.writeUnsignedInt(keyF.shift());//keyF4
				sendBytes.writeUnsignedInt(keyF.shift());//keyF5
				sendBytes.writeUnsignedInt(keyF.shift());//keyF6
				sendBytes.writeUnsignedInt(keyF.shift());//keyF7
				sendBytes.writeUnsignedInt(keyF.shift());//keyF8
			}
			
			sendBytes.writeUnsignedInt(param.shift());//Mapswitch1
			sendBytes.writeUnsignedInt(param.shift());//Mapswitch2
			sendBytes.writeUnsignedInt(GameCommonData.dialogStatus);            //写对话框状态信息
			
			sendBytes.writeUnsignedInt(ChatData.channelSign);					//聊天
			 
			var firSkill:uint   = UIUtils.ArrayBitwiseAndToInteger(SkillData.aMainAutoIndex);
			var secSkill:uint   = UIUtils.ArrayBitwiseAndToInteger(SkillData.aViceAutoIndex);
			var uinSkill:uint   = UIUtils.ArrayBitwiseAndToInteger(SkillData.aUnityAutoIndex);
			var sysSetting:uint = UIUtils.ArrayBitwiseAndToInteger(SystemSettingData._dataArr);		//SystemSettingData._dataArr   [0, 0, 0, 0, 0, 0, 0]
			 
			sendBytes.writeUnsignedInt(firSkill);					//主职业技能打钩
			sendBytes.writeUnsignedInt(secSkill);					//副职业技能打钩
			sendBytes.writeUnsignedInt(uinSkill);					//帮派技能打钩
			sendBytes.writeUnsignedInt(sysSetting);					//系统设置打钩
			
			//endTodo
		 	GameCommonData.GameNets.Send(sendBytes);
		}
		
		
		/**
		 *  发送自动挂机信息
		 * @param param
		 * 
		 */		
		public static function sendAutoPlayMsg(param:Array):void{
			var sendBytes:ByteArray = new ByteArray();
			sendBytes.endian = Endian.LITTLE_ENDIAN;
			
			sendBytes.writeShort(172);	//长度   156
			sendBytes.writeShort(1066); //消息类型
			sendBytes.writeByte(1);//nAction
			sendBytes.writeByte(0);
		
			sendBytes.writeByte(0);
			sendBytes.writeByte(0);
			sendBytes.writeUnsignedInt(GameCommonData.Player.Role.Id);
			
		
			
			sendBytes.writeUnsignedInt(param.shift());                       //hp_item_type_1
			sendBytes.writeUnsignedInt(param.shift());                       //hp_item_type_2
			sendBytes.writeUnsignedInt(param.shift());                       //hp_item_type_3
			sendBytes.writeUnsignedInt(param.shift());                       //mp_item_type_1
			sendBytes.writeUnsignedInt(param.shift());                       //mp_item_type_2
			sendBytes.writeUnsignedInt(param.shift());                       //mp_item_type_3
			sendBytes.writeUnsignedInt(param.shift());                       //petHp_item_type_1
			sendBytes.writeUnsignedInt(param.shift());                       //petHp_item_type_2
			sendBytes.writeUnsignedInt(param.shift());                       //petHp_item_type_3
			sendBytes.writeUnsignedInt(param.shift());                       //limit 1
			sendBytes.writeUnsignedInt(param.shift());                       //limit 2
			sendBytes.writeUnsignedInt(param.shift());                       //limit 3
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);        
			sendBytes.writeUnsignedInt(0);                    
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			
			
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);
			sendBytes.writeUnsignedInt(0);           
			sendBytes.writeUnsignedInt(0);					
			sendBytes.writeUnsignedInt(0);					
			sendBytes.writeUnsignedInt(0);					
			sendBytes.writeUnsignedInt(0);					
			sendBytes.writeUnsignedInt(0);					
			
			//endTodo
		 	GameCommonData.GameNets.Send(sendBytes);
			
			
		}
		

	}
}