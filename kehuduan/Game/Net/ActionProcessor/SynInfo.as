package Net.ActionProcessor
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Mediator.NewLookUnityMediator;
	import GameUI.Modules.UnityNew.Mediator.NewUnityInfoMediator;
	import GameUI.Modules.UnityNew.Proxy.UnityBaseInfo;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class SynInfo extends GameAction
	{
		private var arr:Array = new Array();
		public function SynInfo(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		public override function Processor(bytes:ByteArray):void
		{
			bytes.position = 4;
			var unityBaseInfo:UnityBaseInfo = new UnityBaseInfo();
			unityBaseInfo.level 		= bytes.readUnsignedShort(); 		//帮派等级
			unityBaseInfo.unityJob 	= bytes.readUnsignedShort();		//职位
			unityBaseInfo.onLinePeople	= bytes.readUnsignedShort();		//在线人数
			unityBaseInfo.currentPeople 		= bytes.readUnsignedShort();		//帮会人数
			unityBaseInfo.id 	= bytes.readUnsignedInt();			//帮派id
			unityBaseInfo.historyBangGong = bytes.readUnsignedInt();			//历史帮贡   || 创建时间
			unityBaseInfo.leftBangGong 			= bytes.readUnsignedInt();					//现在帮贡
			unityBaseInfo.money 	= bytes.readUnsignedInt();			//帮会资金
			unityBaseInfo.jianShe 	= bytes.readUnsignedInt();			//帮会建设
			
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int = 0;
				
			for(var i:int = 0;i < nDataSeeNum; i ++)
			{
				nDataSee = bytes.readUnsignedByte();
				if(nDataSee != 0)
				{
					if(i == 0)
					{
						unityBaseInfo.name = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);            //帮会名字
					}
					else if(i == 1)
					{
						unityBaseInfo.boss = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);			//帮主
					}
					else if(i == 2)
					{
						unityBaseInfo.viceBoss1 = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);			 //副帮主
					}
					else if(i == 3)
					{
						unityBaseInfo.viceBoss2 = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);	         //副帮主
					}
					else if(i == 4)
					{
						unityBaseInfo.notice = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);	         //公告
					}
					else if ( i==5 )
					{
						unityBaseInfo.stopState = int( bytes.readMultiByte(nDataSee ,GameCommonData.CODE) );	         //是否暂停维护
					}
				}
			}
			if ( unityBaseInfo.id == GameCommonData.Player.Role.unityId )
			{
				NewUnityCommonData.myUnityInfo = unityBaseInfo;
			}
			if ( NewLookUnityMediator.isRequestLookInfo )
			{
				sendNotification( NewUnityCommonData.UPDATE_NEW_UNITY_LOOK_INFO,unityBaseInfo );
			}
			if ( NewUnityInfoMediator.openState )
			{
				sendNotification( NewUnityCommonData.UPDATE_NEW_UNITY_BASE_INFO,unityBaseInfo );	
			}
//			var arr:Array = [];
//			arr[0] = unityBaseInfo.name; 
//			arr[1] = unityBaseInfo.level;
//			arr[2] = unityBaseInfo.boss;
//			arr[3] = unityBaseInfo.money;
//			arr[4] = unityBaseInfo.jianShe;
//			arr[5] = unityBaseInfo.createTime;					//创建时间
//			arr[6] = unityBaseInfo.currentPeople;   			//玩家数
//			arr[7] = unityBaseInfo.boss;         		//现任帮主
//			arr[8] = unityBaseInfo.jianShe;
//			arr[9] = unityBaseInfo.moving;
//			arr[10]= unityBaseInfo.unityNotice;
//			arr[11]= unityBaseInfo.onLinePeople;
//			if(UnityConstData.oneUnityId == unityBaseInfo.id && (facade.retrieveProxy(DataProxy.NAME) as DataProxy).UnitInfoIsOpen == true)		//如果申请的帮派ID和收到的ID一致，说明这是申请帮派所需的信息
//			{
//				facade.sendNotification(UnityEvent.UPTATAINFODATE , arr); 			//发送申请帮派需要的信息
//			} 
//			else if(GameCommonData.Player.Role.unityId != 0 && GameCommonData.Player.Role.unityJob <= 100)						//如果加入帮派
//			{
//				if(UnityConstData.unityMainIsOpen) 	facade.sendNotification(UnityEvent.GETUNITYOTHERDATA , 0);		//得到主堂信息
//				else facade.sendNotification(UnityEvent.GETINFO , arr);				//发送帮派主界面信息
//			}
//			if(SynInfoObj.unitystate == 1 && GameCommonData.Player.Role.unityJob == 1100)     //建帮中
//			{
//				facade.sendNotification(UnityEvent.UPDATECREATINGDATA , SynInfoObj.unityMenber);	//发送响应人数给创建中面板
//			}
//			if(UnityConstData.respondViewIsOpen == true)
//			{
//				facade.sendNotification(UnityEvent.GETNOTICE , [SynInfoObj.unityNotice , SynInfoObj.name , SynInfoObj.unityMenber] );	//发送响应面板所需的通告和帮派名
//			}
		}
	}
}