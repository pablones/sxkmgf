/** 从聊天里截取数据(建议用协议重做) */
/** --------------*/

package GameUI.Modules.Unity.Command
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class KeepOnCommand extends SimpleCommand
	{
		public static const KEEPON:String = "KEEPON";				//维护的命令
		public function KeepOnCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			var info:String = notification.getBody() as String;
			if(info.indexOf( GameCommonData.wordDic[ "mod_uni_com_kee_exe_1" ] ) >= 0)  // 雇佣人数为：
			{
				facade.sendNotification(UnityEvent.CHANGEHIRENUM , info);
				return;
			}
			var myPattern:RegExp = /\d/igx;///^\d$/g;  
			var infoNoNum:String = info.replace(myPattern , "");
			switch(infoNoNum)
			{
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_2" ]:   // 帮派维护消耗了帮派资金银，建设度，繁荣度。
					keepOnData(info);
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到面板  OK
				break;
				
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_3" ]:   // 本小时雇佣消耗佣金：金
					var i23:int =  Number(info.match(/\d/igx).join(""));
					UnityConstData.mainUnityDataObj.unityMoney -= Number(info.match(/\d/igx).join(""));
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到面板  OK
				break;
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_4" ]:	// 	  资源不足，所有雇佣人员已解雇。
//"帮派维护消耗了帮派资金银，建设度，繁荣度。资源不足，所有雇佣人员已解雇。":
//					keepOnData("帮派维护消耗了帮派资金银，建设度，繁荣度。");
					for(var i:int = 1 ; i < 5 ;i++)
					{
						UnityConstData.otherUnityArray[i].craftsmanNum 		= 0;
						UnityConstData.otherUnityArray[i].businessmanNum 	= 0;
						UnityConstData.otherUnityArray[i].masterNum 		= 0;
					} 
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到面板  OK
				break;  
				
				// 警告：资源不足，帮派停止维护，除捐献功能外所有帮派功能已关闭，小时后将降级！如果当前等级为，小时后帮派将会解散！
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_5" ]:
					UnityConstData.UnityPerformanceClose = true;												//停止维护  OK
				break;
				
				// 本帮由于管理不善，降级为级帮，所有分堂技能上限降低。
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_6" ]:
					unityLevelDown(info);
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		
				break;
				// 本帮由于管理不善，降级为级帮，青龙堂被迫暂时关闭，所有分堂技能上限降低。
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_7" ]:							//关闭青龙堂
					unityLevelDown(info);
					otherInit(UnityConstData.greenUnityDataObj);	
					if(UnityConstData.unityCurSelect == 1) UnityConstData.unityCurSelect = 0;	//如果关闭的是当前分堂就跳到主堂界面上去
					menberJopInit(0);					//青龙堂的成员全变为帮众j
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		
				break;
				// 本帮由于管理不善，降级为级帮，白虎堂被迫暂时关闭，所有分堂技能上限降低。
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_8" ]:							//关闭白虎堂
					unityLevelDown(info);
					otherInit(UnityConstData.whiteUnityDataObj);
					if(UnityConstData.unityCurSelect == 2) UnityConstData.unityCurSelect = 0;	//如果关闭的是当前分堂就跳到主堂界面上去
					menberJopInit(1);					//白虎堂的成员全变为帮众
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		
				break;
				// 本帮由于管理不善，降级为级帮，玄武堂被迫暂时关闭，所有分堂技能上限降低。
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_9" ]:							//关闭玄武堂
					unityLevelDown(info);
					otherInit(UnityConstData.xuanUnityDataObj);
					if(UnityConstData.unityCurSelect == 3) UnityConstData.unityCurSelect = 0;	//如果关闭的是当前分堂就跳到主堂界面上去
					menberJopInit(2);					//玄武堂的成员全变为帮众
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		
				break;
				// 本帮由于管理不善，降级为级帮，朱雀堂被迫暂时关闭，所有分堂技能上限降低。
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_10" ]:							//关闭朱雀堂
					unityLevelDown(info);
					otherInit(UnityConstData.redUnityDataObj);
					if(UnityConstData.unityCurSelect == 4) UnityConstData.unityCurSelect = 0;	//如果关闭的是当前分堂就跳到主堂界面上去
					menberJopInit(3);					//朱雀堂的成员全变为帮众
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		
				break;
				//  本帮由于管理不善被迫解散。11
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_11" ]:
				break;
				//  帮派等级提升到级   12
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_12" ]:
					levelUpOk(0);
				break;
				//  青龙堂等级提升到级  13
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_13" ]:
					levelUpOk(1);
				break;
				//  白虎堂等级提升到级  14
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_14" ]:
					levelUpOk(2);
				break;
				//  玄武堂等级提升到级  15
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_15" ]:
					levelUpOk(3);
				break;
				//  朱雀堂等级提升到级  16
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_16" ]:
					levelUpOk(4);
				break;
				//  青龙堂重新开放  17
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_17" ]:
					synLive(UnityConstData.greenUnityDataObj);
				break;
				//  白虎堂重新开放  18
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_18" ]:
					synLive(UnityConstData.whiteUnityDataObj);
				break;
				// 朱雀堂重新开放  19
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_19" ]:
					synLive(UnityConstData.redUnityDataObj);
				break;
				//  玄武堂重新开放  20
				case GameCommonData.wordDic[ "mod_uni_com_kee_exe_20" ]:
					synLive(UnityConstData.xuanUnityDataObj);
				break;
			}
		}
		/** 基本的维护扣除数据 */
		private function keepOnData(info:String):void
		{
			var strList:Array = info.split("，");
			var unityMoney:int = (strList[0].split( GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ] )[1]).match(/\d/igx).join("");  // 金
			var unityBuilt:int = strList[1].split( GameCommonData.wordDic[ "mod_uni_com_kee_kee_2" ] )[1];  // 度
			var unitybooming:int = (strList[2].split( GameCommonData.wordDic[ "mod_uni_com_kee_kee_2" ] )[1]).match(/\d/igx).join("");  // 度
			UnityConstData.mainUnityDataObj.unityMoney	 	-= unityMoney * 100;
			UnityConstData.mainUnityDataObj.unityBuilt 		-= unityBuilt;
			UnityConstData.mainUnityDataObj.unitybooming 	-= unitybooming;
		}
		/** 基本的帮派降级数据处理*/
		private function unityLevelDown(info:String):void
		{
			var level:int = int(info.slice(12 , 13));
			UnityConstData.mainUnityDataObj.level = int(level);
			facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到面板
		}
		
		/** 分堂停用 */
		private function otherInit(obj:Object):void
		{
			obj.craftsmanNum = 0; 
			obj.businessmanNum = 0;
			obj.masterNum = 0;
//			obj.skillStuding = 0;
			obj.isStop = true;
		}
		/** 分堂成员全成帮众 */
		private function menberJopInit(otherType:int):void
		{
			for(var i:int = 0; i < 4;i++)
			{
				if(UnityConstData.unityOtherJopList[otherType][i] == GameCommonData.Player.Role.unityJob)
				{
					GameCommonData.Player.Role.unityJob = 10;			//变为帮众  需要更新到成员面板
				}
			}
		}
		/** 升级成功后扣除帮派资源 */
		private function levelUpOk(_type:int):void
		{
			//扣除需要消耗的资源
			if(_type != 0)
			{
				UnityConstData.mainUnityDataObj.unityBuilt 		-= int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[_type].level , "bulit"));
				UnityConstData.mainUnityDataObj.unitybooming 	-= int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[_type].level , "bulit"));
				UnityConstData.mainUnityDataObj.unityMoney 		-= int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[_type].level , "otherLevelUpMoney"));
				UnityConstData.otherUnityArray[_type].level += int(1);
			}
			else
			{
				var level:int = UnityConstData.mainUnityDataObj.level;
				UnityConstData.mainUnityDataObj.unityBuilt 		-= int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit"));
				UnityConstData.mainUnityDataObj.unitybooming 	-= int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpBulit"));
				UnityConstData.mainUnityDataObj.unityMoney 		-= int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mainLevelUpMoney"));
				UnityConstData.mainUnityDataObj.level += int(1);
//				if()
//				_parent["mcUnity_" + _type].gotoAndStop(2);
			}
			facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到面板
		}
		/** 分堂复活 */
		private function synLive(obj:Object):void
		{
			obj.isStop = false;
		}
	}
}