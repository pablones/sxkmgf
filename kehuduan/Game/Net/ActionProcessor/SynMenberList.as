package Net.ActionProcessor
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityMemberVo;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class SynMenberList extends GameAction
	{
		public static var isFirst:int;
		private var beijuMember:int;			//被删除的成员id
		private var beijuApply:int;				//被删除的申请人员id
		
		public function SynMenberList(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			var MenberlistInfo:Object 		= new Object();
			MenberlistInfo.usAction		= bytes.readUnsignedByte();
			MenberlistInfo.usMaxAmount	= bytes.readUnsignedByte();
			MenberlistInfo.usThisAmount	= bytes.readUnsignedByte();
			MenberlistInfo.usPage		= bytes.readUnsignedByte();
			var totalPage:int = Math.ceil( MenberlistInfo.usMaxAmount/14 );
			var pageArr:Array = [];
			var unityVo:UnityMemberVo;
						
			switch( MenberlistInfo.usAction )
			{
				case 4:			//更新成员信息
					for( var c:uint = 0 ; c < MenberlistInfo.usThisAmount ; c ++ )
					{
						unityVo = new UnityMemberVo();
						unityVo.id   	 		 			 = bytes.readUnsignedInt();												//id
						unityVo.name 					 = bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						unityVo.roleLevel   		 	 = bytes.readUnsignedShort();											//等级
						unityVo.mainJob   		 	 = bytes.readUnsignedShort();											//主职业
						unityVo.sex   	  				 = bytes.readUnsignedShort();											//性别
						unityVo.unityJob   		     = bytes.readUnsignedShort();											//职位
						unityVo.line   		 			 = bytes.readUnsignedShort();											//线路
						unityVo.vip   	 				 = bytes.readUnsignedShort();											//vip
						unityVo.lastLoginTime   	 = bytes.readUnsignedInt();												//最后登陆时间
						unityVo.totalContribute   	 = bytes.readUnsignedInt();												//总帮贡
						unityVo.moneyContribute = bytes.readUnsignedInt();												//资金贡献
						unityVo.jianseContribute  = bytes.readUnsignedInt();												//建设贡献
						unityVo.fighting   	 		 = bytes.readUnsignedInt();												//战斗力
						
						for ( var k:uint=0; k<NewUnityCommonData.allUnityMemberArr.length; k++ )
						{
							if ( NewUnityCommonData.allUnityMemberArr[k].id == unityVo.id  )
							{
								NewUnityCommonData.allUnityMemberArr[k] = unityVo;
							}
						}
						if ( GameCommonData.Player && GameCommonData.Player.Role && unityVo.id == GameCommonData.Player.Role.Id )
						{
							GameCommonData.Player.Role.unityJob = unityVo.unityJob;
							NewUnityCommonData.myUnityInfo.unityJob = unityVo.unityJob;
						}
					}
					sendNotification( NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS,NewUnityCommonData.allUnityMemberArr );
				break;
				/** 新帮派协议 5为成员列表*/
				case 5:		
//					NewUnityCommonData.allUnityMemberArr = [];
					for( var b:uint = 0 ; b < MenberlistInfo.usThisAmount ; b ++ )
					{
						unityVo = new UnityMemberVo();
						unityVo.id   	 		 			 = bytes.readUnsignedInt();												//id
						unityVo.name 					 = bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						unityVo.roleLevel   		 	 = bytes.readUnsignedShort();											//等级
						unityVo.mainJob   		 	 = bytes.readUnsignedShort();											//主职业
						unityVo.sex   	  				 = bytes.readUnsignedShort();											//性别
						unityVo.unityJob   		     = bytes.readUnsignedShort();											//职位
						unityVo.line   		 			 = bytes.readUnsignedShort();											//线路
						unityVo.vip   	 				 = bytes.readUnsignedShort();											//vip
						unityVo.lastLoginTime   	 = bytes.readUnsignedInt();												//最后登陆时间
						unityVo.totalContribute   	 = bytes.readUnsignedInt();												//总帮贡
						unityVo.moneyContribute = bytes.readUnsignedInt();												//资金贡献
						unityVo.jianseContribute  = bytes.readUnsignedInt();												//建设贡献
						unityVo.fighting   	 		 = bytes.readUnsignedInt();												//战斗力
						
						if ( !isMemExist( unityVo,NewUnityCommonData.allUnityMemberArr ) )
						{
							NewUnityCommonData.allUnityMemberArr.push( unityVo );
						}
					}
					
					var curPage:int = Math.ceil( NewUnityCommonData.allUnityMemberArr.length/14 );
					var serverPage:int = Math.ceil( MenberlistInfo.usMaxAmount/14 );
					if ( curPage == serverPage )
					{
						sortDate( NewUnityCommonData.allUnityMemberArr );
						sendNotification( NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS,NewUnityCommonData.allUnityMemberArr );
					}
					
				break;
				
				case 7:		//申请列表
//					NewUnityCommonData.allApplyMemberArr = [];
					for(var i:uint = 0 ; i < MenberlistInfo.usThisAmount ; i ++)
					{
						unityVo = new UnityMemberVo();
						unityVo.id   	 		 			 = bytes.readUnsignedInt();												//id
						unityVo.name 					 = bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						unityVo.roleLevel   		 	 = bytes.readUnsignedShort();											//等级
						unityVo.mainJob   		 	 = bytes.readUnsignedShort();											//主职业
						unityVo.sex   	  				 = bytes.readUnsignedShort();											//性别
						unityVo.unityJob   		     = bytes.readUnsignedShort();											//职位
						unityVo.line   		 			 = bytes.readUnsignedShort();											//线路
						unityVo.vip   	 				 = bytes.readUnsignedShort();											//vip
						unityVo.lastLoginTime   	 = bytes.readUnsignedInt();												//最后登陆时间
						unityVo.totalContribute   	 = bytes.readUnsignedInt();												//总帮贡
						unityVo.moneyContribute = bytes.readUnsignedInt();												//资金贡献
						unityVo.jianseContribute  = bytes.readUnsignedInt();												//建设贡献
						unityVo.fighting   	 		 = bytes.readUnsignedInt();												//战斗力
						
						if ( !isMemExist( unityVo,NewUnityCommonData.allApplyMemberArr ) )
						{
							NewUnityCommonData.allApplyMemberArr.push( unityVo );	
						}
//						var arr:Array = NewUnityCommonData.allApplyMemberArr;
					}
					if ( NewUnityCommonData.allApplyMemberArr.length == MenberlistInfo.usMaxAmount )
					{
						sendNotification( NewUnityCommonData.UPDATA_UNITY_APPLY_LIST_DATA,NewUnityCommonData.allApplyMemberArr );
					}
				break;
				
				case 8:				//加成员
					for( i = 0 ; i < MenberlistInfo.usThisAmount ; i ++)
					{
						unityVo = new UnityMemberVo();
						unityVo.id   	 		 			 = bytes.readUnsignedInt();												//id
						unityVo.name 					 = bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						unityVo.roleLevel   		 	 = bytes.readUnsignedShort();											//等级
						unityVo.mainJob   		 	 = bytes.readUnsignedShort();											//主职业
						unityVo.sex   	  				 = bytes.readUnsignedShort();											//性别
						unityVo.unityJob   		     = bytes.readUnsignedShort();											//职位
						unityVo.line   		 			 = bytes.readUnsignedShort();											//线路
						unityVo.vip   	 				 = bytes.readUnsignedShort();											//vip
						unityVo.lastLoginTime   	 = bytes.readUnsignedInt();												//最后登陆时间
						unityVo.totalContribute   	 = bytes.readUnsignedInt();												//总帮贡
						unityVo.moneyContribute = bytes.readUnsignedInt();												//资金贡献
						unityVo.jianseContribute  = bytes.readUnsignedInt();												//建设贡献
						unityVo.fighting   	 		 = bytes.readUnsignedInt();												//战斗力
						//判断是否收到相同的人
						if ( !isMemExist( unityVo,NewUnityCommonData.allUnityMemberArr ) )
						{
							NewUnityCommonData.allUnityMemberArr.push( unityVo );
						}
					}
					sortDate( NewUnityCommonData.allUnityMemberArr );
					sendNotification( NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS,NewUnityCommonData.allUnityMemberArr );
				break;
				
				case 9:				//减成员
					for( i = 0 ; i < MenberlistInfo.usThisAmount ; i ++)
					{
						beijuMember   	 		 			 = bytes.readUnsignedInt();												//id
						bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						bytes.readUnsignedShort();											//等级
						bytes.readUnsignedShort();											//主职业
						bytes.readUnsignedShort();											//性别
						bytes.readUnsignedShort();											//职位
						bytes.readUnsignedShort();											//线路
						bytes.readUnsignedShort();											//vip
						bytes.readUnsignedInt();												//最后登陆时间
						bytes.readUnsignedInt();												//总帮贡
						bytes.readUnsignedInt();												//资金贡献
						bytes.readUnsignedInt();												//建设贡献
						bytes.readUnsignedInt();												//战斗力
				
						NewUnityCommonData.allUnityMemberArr = NewUnityCommonData.allUnityMemberArr.filter( isDeleteMem );
					}
					sendNotification( NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS,NewUnityCommonData.allUnityMemberArr );
				break;
				
				case 10:				//加申请
					for( i = 0 ; i < MenberlistInfo.usThisAmount ; i ++)
					{
						unityVo = new UnityMemberVo();
						unityVo.id   	 		 			 = bytes.readUnsignedInt();												//id
						unityVo.name 					 = bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						unityVo.roleLevel   		 	 = bytes.readUnsignedShort();											//等级
						unityVo.mainJob   		 	 = bytes.readUnsignedShort();											//主职业
						unityVo.sex   	  				 = bytes.readUnsignedShort();											//性别
						unityVo.unityJob   		     = bytes.readUnsignedShort();											//职位
						unityVo.line   		 			 = bytes.readUnsignedShort();											//线路
						unityVo.vip   	 				 = bytes.readUnsignedShort();											//vip
						unityVo.lastLoginTime   	 = bytes.readUnsignedInt();												//最后登陆时间
						unityVo.totalContribute   	 = bytes.readUnsignedInt();												//总帮贡
						unityVo.moneyContribute = bytes.readUnsignedInt();												//资金贡献
						unityVo.jianseContribute  = bytes.readUnsignedInt();												//建设贡献
						unityVo.fighting   	 		 = bytes.readUnsignedInt();												//战斗力
						
						if ( !isMemExist( unityVo,NewUnityCommonData.allApplyMemberArr ) )
						{
							NewUnityCommonData.allApplyMemberArr.push( unityVo );	
						}
						
//						NewUnityCommonData.allApplyMemberArr.push( unityVo );
					}
					sendNotification( NewUnityCommonData.UPDATA_UNITY_APPLY_LIST_DATA,NewUnityCommonData.allApplyMemberArr );
				break;
				
				case 11:			//减申请
					for( i = 0 ; i < MenberlistInfo.usThisAmount ; i ++)
					{
						this.beijuApply   	 		 			 = bytes.readUnsignedInt();												//id
						bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						bytes.readUnsignedShort();											//等级
						bytes.readUnsignedShort();											//主职业
						bytes.readUnsignedShort();											//性别
						bytes.readUnsignedShort();											//职位
						bytes.readUnsignedShort();											//线路
						bytes.readUnsignedShort();											//vip
						bytes.readUnsignedInt();												//最后登陆时间
						bytes.readUnsignedInt();												//总帮贡
						bytes.readUnsignedInt();												//资金贡献
						bytes.readUnsignedInt();												//建设贡献
						bytes.readUnsignedInt();												//战斗力
						NewUnityCommonData.allApplyMemberArr = NewUnityCommonData.allApplyMemberArr.filter( isDeleteApply );
					}
					sendNotification( NewUnityCommonData.UPDATA_UNITY_APPLY_LIST_DATA,NewUnityCommonData.allApplyMemberArr );
				break;
				
				case 12:			//改变职位
					for( i = 0 ; i < MenberlistInfo.usThisAmount ; i ++)
					{
						unityVo = new UnityMemberVo();
						unityVo.id   	 		 			 = bytes.readUnsignedInt();												//id
						unityVo.name 					 = bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						unityVo.roleLevel   		 	 = bytes.readUnsignedShort();											//等级
						unityVo.mainJob   		 	 = bytes.readUnsignedShort();											//主职业
						unityVo.sex   	  				 = bytes.readUnsignedShort();											//性别
						unityVo.unityJob   		     = bytes.readUnsignedShort();											//职位
						unityVo.line   		 			 = bytes.readUnsignedShort();											//线路
						unityVo.vip   	 				 = bytes.readUnsignedShort();											//vip
						unityVo.lastLoginTime   	 = bytes.readUnsignedInt();												//最后登陆时间
						unityVo.totalContribute   	 = bytes.readUnsignedInt();												//总帮贡
						unityVo.moneyContribute = bytes.readUnsignedInt();												//资金贡献
						unityVo.jianseContribute  = bytes.readUnsignedInt();												//建设贡献
						unityVo.fighting   	 		 = bytes.readUnsignedInt();												//战斗力
						for ( var k1:uint=0; k1<NewUnityCommonData.allUnityMemberArr.length; k1++ )
						{
							if ( NewUnityCommonData.allUnityMemberArr[k1].id == unityVo.id  )
							{
								NewUnityCommonData.allUnityMemberArr[k1].unityJob = unityVo.unityJob;
							}
						}
						if ( GameCommonData.Player && GameCommonData.Player.Role && unityVo.id == GameCommonData.Player.Role.Id )
						{
							GameCommonData.Player.Role.unityJob = unityVo.unityJob;
							NewUnityCommonData.myUnityInfo.unityJob = unityVo.unityJob;
						}
					}
					sendNotification( NewUnityCommonData.RECEIVE_ALL_UNITY_MEMBERS,NewUnityCommonData.allUnityMemberArr );
				break;
				
				default:
					for(var j:uint = 0 ; j < MenberlistInfo.usThisAmount ; j ++)
					{
						bytes.readUnsignedInt();												//id
						bytes.readMultiByte( 16,GameCommonData.CODE );	//名字
						bytes.readUnsignedShort();											//等级
						bytes.readUnsignedShort();											//主职业
						bytes.readUnsignedShort();											//性别
						bytes.readUnsignedShort();											//职位
						bytes.readUnsignedShort();											//线路
						bytes.readUnsignedShort();											//vip
						bytes.readUnsignedInt();												//最后登陆时间
						bytes.readUnsignedInt();												//总帮贡
						bytes.readUnsignedInt();												//资金贡献
						bytes.readUnsignedInt();												//建设贡献
						bytes.readUnsignedInt();												//战斗力
					}
				break;
			}	
			
		}
		

		private function isDeleteMem( item:*,index:int,arr:Array ):Boolean
		{
			if ( item.id != beijuMember )
			{
				return true;
			}
			return false;
		}
		
		private function isDeleteApply( item:*,index:int,arr:Array ):Boolean
		{
			if ( item.id != beijuApply )
			{
				return true;
			}
			return false;
		}
		
		//排序
		private function sortDate( arr:Array ):void
		{
			arr = arr.sortOn( "unityJob",Array.NUMERIC ).reverse();
		}
		
		//是否有相同的人
		private function isMemExist( vo:UnityMemberVo,arr:Array ):Boolean
		{
			for ( var i:int=0; i<arr.length; i++ )
			{
				if ( arr[ i ].id == vo.id )
				{
//					trace ( "收到相同的人了" + vo.name );
					arr[ i ] = vo;
					return true;
				}
			}
			return false;
		}
		
	}
}