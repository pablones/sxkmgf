package Net.ActionProcessor
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class SynSubInfo extends GameAction
	{
		public function SynSubInfo(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			var arr:Array = [];
			
			arr[0] = bytes.readUnsignedInt(); 		//青龙   
			arr[1] = bytes.readUnsignedInt(); 		//青龙    钱
			arr[2] = bytes.readUnsignedInt(); 		//青龙     
			arr[3] = bytes.readUnsignedInt(); 		//青龙

			if ( arr[0]<100 )
			{
				NewUnityCommonData.unityPlaceLevelArr = arr.concat();
				sendNotification( NewUnityCommonData.UPDATE_SYN_PLACE_INFO );
				sendNotification( SkillConst.REC_UNITY_SKILL_STUDLEV );
			}
			else if ( arr[0] == 101 )   //钱
			{
				NewUnityCommonData.myUnityInfo.money = arr[1];
				sendNotification( NewUnityCommonData.UPDATE_ON_TIME_UNITY_BASEINFO );
			}
			else if ( arr[0] == 102 )			//建设
			{
				NewUnityCommonData.myUnityInfo.jianShe = arr[1];
				sendNotification( NewUnityCommonData.UPDATE_ON_TIME_UNITY_BASEINFO );
			}
			else if ( arr[0] == 103 )			//帮贡
			{
				NewUnityCommonData.myUnityInfo.leftBangGong = arr[1];
				NewUnityCommonData.myUnityInfo.historyBangGong = arr[2];
				sendNotification( NewUnityCommonData.UPDATE_ON_TIME_UNITY_BASEINFO );
				GameCommonData.Player.Role.unityContribution = arr[1];
				facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"unityAtt_txt",value:arr[1]} );
			}
		}
		
//		public override function Processor(bytes:ByteArray):void
//		{
//			bytes.position = 4;
////			var SynSubInfoObj:Object 	= new Object();
//			UnityConstData.greenUnityDataObj.level = bytes.readUnsignedShort(); 		//等级
//			UnityConstData.whiteUnityDataObj.level = bytes.readUnsignedShort(); 		//等级
//			UnityConstData.xuanUnityDataObj.level  = bytes.readUnsignedShort(); 		//等级
//			UnityConstData.redUnityDataObj.level   = bytes.readUnsignedShort(); 		//等级
//			
//			
//			UnityConstData.greenUnityDataObj.skillStuding	= bytes.readUnsignedInt();
//			UnityConstData.whiteUnityDataObj.skillStuding	= bytes.readUnsignedInt();		//正在升级的技能编号	0,1,2,3
//			UnityConstData.xuanUnityDataObj.skillStuding	= bytes.readUnsignedInt();
//			UnityConstData.redUnityDataObj.skillStuding		= bytes.readUnsignedInt();
//			////////////////////////////////////////////
//			UnityConstData.greenUnityDataObj.skillTolExp1 = bytes.readUnsignedInt();			//技能升级经验
//			UnityConstData.greenUnityDataObj.skillTolExp2 = bytes.readUnsignedInt();
//			UnityConstData.greenUnityDataObj.skillTolExp3 = bytes.readUnsignedInt();
//			
//			UnityConstData.whiteUnityDataObj.skillTolExp1 = bytes.readUnsignedInt();
//			UnityConstData.whiteUnityDataObj.skillTolExp2 = bytes.readUnsignedInt();
//			UnityConstData.whiteUnityDataObj.skillTolExp3 = bytes.readUnsignedInt();
//			
//			UnityConstData.xuanUnityDataObj.skillTolExp1 = bytes.readUnsignedInt();
//			UnityConstData.xuanUnityDataObj.skillTolExp2 = bytes.readUnsignedInt();
//			UnityConstData.xuanUnityDataObj.skillTolExp3 = bytes.readUnsignedInt();
//			
//			UnityConstData.redUnityDataObj.skillTolExp1 = bytes.readUnsignedInt();
//			UnityConstData.redUnityDataObj.skillTolExp2 = bytes.readUnsignedInt();
//			UnityConstData.redUnityDataObj.skillTolExp3 = bytes.readUnsignedInt();
//			/////////////////////////////////////////////
//			UnityConstData.greenUnityDataObj.masterNum = bytes.readUnsignedInt();		//武师
//			UnityConstData.whiteUnityDataObj.masterNum = bytes.readUnsignedInt();
//			UnityConstData.xuanUnityDataObj.masterNum = bytes.readUnsignedInt();
//			UnityConstData.redUnityDataObj.masterNum = bytes.readUnsignedInt();
//			
//			UnityConstData.greenUnityDataObj.businessmanNum = bytes.readUnsignedInt();	//商人
//			UnityConstData.whiteUnityDataObj.businessmanNum = bytes.readUnsignedInt();
//			UnityConstData.xuanUnityDataObj.businessmanNum = bytes.readUnsignedInt();
//			UnityConstData.redUnityDataObj.businessmanNum = bytes.readUnsignedInt();
//			
//			UnityConstData.greenUnityDataObj.craftsmanNum = bytes.readUnsignedInt();	//建筑工人
//			UnityConstData.whiteUnityDataObj.craftsmanNum = bytes.readUnsignedInt();
//			UnityConstData.xuanUnityDataObj.craftsmanNum = bytes.readUnsignedInt();
//			UnityConstData.redUnityDataObj.craftsmanNum = bytes.readUnsignedInt();
//			
//			var otherStop:int 			= bytes.readUnsignedInt();						//分堂是否暂时关闭
//			var PerformanceStop:int 	= bytes.readUnsignedInt();						//帮派功能是否暂时停用 0是可用 ， 1是停用
//			
//			UnityConstData.UnityPerformanceClose = PerformanceStop == 0 ? false : true; //帮派功能状态
//			for(var i:int = 0; i < UnityConstData.otherCloseList.length ; i++)
//			{
//				if(int(otherStop & UnityConstData.otherCloseList[i]) == UnityConstData.otherCloseList[i]) UnityConstData.otherUnityArray[(i+1)].isStop = true;
//				else UnityConstData.otherUnityArray[(i+1)].isStop = false;
//			}
//			
//			var obj1:Object = UnityConstData.greenUnityDataObj;
//			var obj2:Object = UnityConstData.whiteUnityDataObj;
//			var obj3:Object = UnityConstData.xuanUnityDataObj;
//			var obj4:Object = UnityConstData.redUnityDataObj;
//			if(UnityConstData.unityIsSend)	
//			{
//				UnityConstData.unityIsSend = false;
//				facade.sendNotification(UnityEvent.GETUNITYOTHERDATA , 1);		//得到主堂信息
//			}
//			facade.sendNotification( SkillConst.REC_UNITY_SKILL_STUDLEV );
//
//		}
		
	}
}