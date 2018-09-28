package Net.ActionProcessor
{
	import GameUI.Modules.ChangeLine.ChangeLineMediator;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Mediator.Member.SingleContributeMediator;
	import GameUI.Modules.UnityNew.Mediator.Member.SingleMemberListMediator;
	import GameUI.Proxy.DataProxy;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class SynAction extends GameAction
	{
		private var dataProxy:DataProxy;
		public function SynAction(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			bytes.position  = 4;
			var obj:Object  = new Object();
			obj.nAction 	= bytes.readUnsignedShort();		
			obj.nData 	    = bytes.readUnsignedShort(); 
			obj.nUser       = bytes.readUnsignedInt();		//进入的专线
			
			var nDataSeeNum:int = bytes.readByte();
			var nDataSee:int    = 0;	
			for(var i:int = 0;i < nDataSeeNum; i++) 
			{
				nDataSee = bytes.readByte();
				obj.talkObj[i] = 0;
				if(nDataSee != 0) 
				{
					obj.talkObj[i] = bytes.readMultiByte(nDataSee ,GameCommonData.CODE);
				}		
			}
//			var iscreating:int;
//			var iscreating:int = int(GameCommonData.Player.Role.unityJob-1) / 100;										//如果(职业-1)除以100得到0，则创建成功，不为0，则正在创建中
//			if(iscreating != 0 ||GameCommonData.Player.Role.unityId == 0)												//如果进入要响应的状态或还没有帮派，就执行
//			{
//				facade.sendNotification(UnityEvent.CREATEUNITY , obj.nAction);											//发送到创建帮派后更新的数据
//			}
//			else																										//已经加帮的情况
//			{
//				if(UnityConstData.applyViewIsOpen == true)																//如果申请列表面板打开
//				{
//					facade.sendNotification(UnityEvent.APPLYUPDATA , obj.nAction);											//更新面板
//				}
//				if(dataProxy.UnityIsOpen == true)																		//如果加入了帮派
//				{
//					SynMenberList.isFirst = 0;		
//					facade.sendNotification(UnityEvent.UNITYUPDATA , obj.nAction);
//				}
//			}
			//响应人数达到6人后发送广播
//			if(obj.nAction == 219)
//			{
//				facade.sendNotification(UnityEvent.RESPONDCHANGEJOP);
//				facade.sendNotification(UnityEvent.CLOSERESPONDUNITYVIEW);
//			}
//			/** 打工操作返回打工类型 */
//			if(obj.nAction == 228)
//			{
//				UnityDoWork.workGo(obj.nUser);		//开始打工
//			}
			/** 帮派创建成功 */
			if ( obj.nAction == 202 )
			{
				GameCommonData.Player.Role.unityId = obj.nUser;
				sendNotification( UnityEvent.CREATEUNITY,obj.nAction ); 
				return;
			}
			
			/** 帮派创建失败 */
			if ( obj.nAction == 203 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "net_act_syn_pro_1" ] );   //很遗憾，创建帮派失败
				return;
			}

			/** 进入帮派地图 */
			if ( obj.nAction == 235 ) 
			{
				
			}
			/** 加入帮派成功 */
			if ( obj.nAction == 216 ) 
			{
				if ( obj.nUser != GameCommonData.Player.Role.Id )
				{
					return;
				}
				SingleMemberListMediator.isRequestState = true;
				SingleContributeMediator.isRequestState = true;
				switch ( obj.nData )
				{
					case 0:  //允许加入
						sendNotification( NewUnityCommonData.CLOSE_JOIN_UNITY_NEW );
					break;
					case 1:		//不允许加入
						
					break;
					case 2:		//离开帮派
						sendNotification( NewUnityCommonData.CLOSE_NEW_UNITY_MAIN_PANEL );
//						SingleMemberListMediator.isRequestState = true;
//						SingleContributeMediator.isRequestState = true;
						NewUnityCommonData.allUnityMemberArr = [];
						NewUnityCommonData.allApplyMemberArr = []; 
						GameCommonData.Player.Role.unityContribution = 0;
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"unityAtt_txt",value:0} );
					break;
					case 3: 	//被踢啦
//						SingleMemberListMediator.isRequestState = true;
//						SingleContributeMediator.isRequestState = true;
						sendNotification( NewUnityCommonData.CLOSE_NEW_UNITY_MAIN_PANEL );
						NewUnityCommonData.allUnityMemberArr = [];
						NewUnityCommonData.allApplyMemberArr = [];
						GameCommonData.Player.Role.unityContribution = 0;
						facade.sendNotification( RoleEvents.UPDATE_OTHER_INFO,{target:"unityAtt_txt",value:0} );
					break;
				}
			}
			
			/** 进帮派专线 0是随机，7是专线 */
			if ( obj.nAction == 335 )  
			{
				try
				{
					if ( NewUnityCommonData.closeUnity )
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_MainSence_Mediator_MainSenceMediator_7" ]/** = "帮派功能正在调试，暂时关闭";*/, color:0xffff00});	
						return;
					}
				}
				catch ( e:Error )
				{
					
				}
				if ( facade.hasMediator( ChangeLineMediator.NAME ) )
				{
					sendNotification( ChgLineData.REC_UNITY_MAP_ORDER,obj.nUser );
				}
				else
				{
					if ( !ChgLineData.isChgLine )
					{
						GameConfigData.IsLoginChangeLine = true;
						GameConfigData.GameSocketName = getLineStr( obj.nUser );
					}
				}
			}
		}
		
		private function getLineStr( line:int ):String
		{
			var _lineStr:String
			switch ( line )
			{
				case 0:
					var aLineNames:Array = [];
					for ( var i:uint=0; i<GameCommonData.GameServerArr.length; i++ )
					{
						var aSingleLine:Array = GameCommonData.GameServerArr[i].split( ":" );
						if ( aSingleLine[0] != GameConfigData.specialLineName )
						{
							aLineNames.push( aSingleLine[0] );
						}
					}
					var index:int = Math.random()*( aLineNames.length - 1 );
					_lineStr = aLineNames[ index ];
				break;
				case 1:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_2" ];    //一线
				break;
				case 2:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_3" ];    //二线
				break;
				case 3:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_4" ];    //三线
				break;
				case 4:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_5" ];    //四线
				break;
				case 5:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_6" ];    //五线
				break;
				case 6:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_7" ];    //六线
				break;
				case 7:
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_8" ];    //七线
				break;
				case 20:
					_lineStr = GameConfigData.specialLineName;
				break;
				default:			//防止意外
					_lineStr = GameCommonData.wordDic[ "mod_mas_com_got_get_2" ];    //一线
				break;
			}
			return _lineStr;
		}
		
	}
}