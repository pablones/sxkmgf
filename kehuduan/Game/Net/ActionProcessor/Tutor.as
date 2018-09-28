package Net.ActionProcessor
{
	import GameUI.Modules.Answer.Const.AnswerConst;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Command.AgreementStudentrCommand;
	import GameUI.Modules.Master.Command.AnalysisMasterRelationCommand;
	import GameUI.Modules.Master.Command.AskStudentGoCommand;
	import GameUI.Modules.Master.Command.AskStudentGraduateCommand;
	import GameUI.Modules.Master.Command.BetrayMasterCommand;
	import GameUI.Modules.Master.Command.GotoMasterSideCommand;
	import GameUI.Modules.Master.Command.ReceiveMasterCommand;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.OldMaster;
	import GameUI.Modules.Master.Proxy.ServerMasterRelation;
	import GameUI.Modules.Master.Proxy.YoungStudent;
	
	import Net.GameAction;
	
	import flash.utils.ByteArray;

	public class Tutor extends GameAction
	{
		public function Tutor(isUsePureMVC:Boolean=true)
		{
			super(isUsePureMVC);
		}
		
		public override function Processor(bytes:ByteArray):void 
		{
			bytes.position = 4;
			var obj:Object = new Object();
			obj.idUser = bytes.readUnsignedInt();				//用户ID
			obj.nAction	= bytes.readUnsignedShort();			//功能
			obj.nAmount 	= bytes.readByte();						//成员数量
			obj.nCount	= bytes.readByte();//页数
			 
			switch ( obj.nAction )
			{
				case 10:   //师傅登记列表
					recMasterRegistList( obj.nAmount,bytes,obj.nCount );
				break;
				case 11:
					sendNotification( MasterData.RECEIVE_REGIST_INFO );
				break;
				case 21:			//徒弟申请列表
					recStudentApplyList( obj.nAmount,bytes,obj.nCount );
				break;
				case 23:		//徒弟列表
					recStudentList( obj.nAmount,bytes );
				break;
//				case 22:		//徒弟列表
//					recStudentList( obj.nAmount,bytes );
//				break;
				case 24:
					recMasStuInfo( obj.nAmount,bytes );
				break;
				case 25:			//删除徒弟申请列表
					sendNotification( MasterData.DELETE_STUDENT_APPLY );
				break;
				case 26:  			//删除我的徒弟
					recDelectStudent( obj.nAmount,bytes );
				break;
				case 17:			//协议解除徒弟
					receiveAgreeStudent( obj.nAmount,bytes );
				break;
				case 19:			//弹出徒弟判师的确认框
					openStudentCommit( obj.nAmount,bytes );
				break;
				case 27:			//收到召唤徒弟消息,弹徒弟确认框
					askStudentGo( obj.nAmount,bytes );
				break;
				case 28:				//徒弟确认，来到师傅身边
					gotoMaster( obj.nAmount,bytes,obj.nCount );
				break;
				case 30:				//收到是否有出师的徒弟
					checkHasGraduate( obj.nAmount,bytes,obj.nCount );
				break;
				case 31:				//收到是否有出师的徒弟
					recGraduatelist( obj.nAmount,bytes,obj.nCount );
				break;
				case 16:
					askStudentGraduate( obj.nAmount,bytes );
				break;
				case 33:				//问徒弟是否要拜师
					askStudentPleaseMaster( obj.nAmount,bytes );
				break;
				case 35:				//师徒答题的随机数
					recAnswer( obj.nAmount,bytes,obj );
				break;
				case 36:				//答题完毕，关闭答题面板
					answerDone( obj.nAmount,bytes,obj );
				break;
			}
//			for(var i:uint = 0 ; i < obj.nAmount ; i ++)
//			{
//				var listObj:Object = new Object();
//				var idUser:uint = bytes.readUnsignedInt();	//玩家ID
//				var mainJob:uint = bytes.readUnsignedInt(); //主职业/战斗力
//				var subJob:uint = bytes.readUnsignedInt(); //副职业/离线时间
//				var nLev:uint = bytes.readUnsignedShort(); //等级
//				var nMainLev:uint = bytes.readUnsignedShort(); //主职业等级//主职业
//				var nSubLev:uint = bytes.readUnsignedShort(); //副职业等级
//				var nLookFace:uint = bytes.readUnsignedShort(); //头像
//				var nImpart:uint  = bytes.readUnsignedInt();	//传授度
//				var vipLev:int = bytes.readUnsignedByte();//vip等级
//				var hasTeam:int = bytes.readUnsignedByte();//是否有队伍/性别
//				var line:int = bytes.readUnsignedByte();//线路
//				var relation:int = bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
//				var name:String = bytes.readMultiByte(16, "ANSI");	//名字
//
//				obj.list.push(listObj);
//			}
			
		}
		
		//获取师傅登记列表
		private function recMasterRegistList( len:int,bytes:ByteArray,pages:int ):void
		{
			var master:OldMaster;
			var allMaster:Array = [];
			var masterOnLine:Array = [];				//在线的师傅
			var masterNotOnLine:Array = [];			//不在线的师傅
			for( var i:uint = 0; i < len; i ++ )
			{
				master= new OldMaster();
				master.id = bytes.readUnsignedInt();	//玩家ID
				master.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				master.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				master.roleLevel = bytes.readUnsignedShort(); //等级
				master.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				master.impart = bytes.readUnsignedInt();	//传授度
				master.vip = bytes.readUnsignedByte();//vip等级
				master.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				master.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
				master.sortIndex = -1*master.outLineTime;
//				if ( master.outLineTime == 0 )
//				{
//					masterOnLine.push( master );
//				}
//				else
//				{
//					masterNotOnLine.push( master );
//				}
				allMaster.push( master );
			}
//			masterOnLine.sortOn( "impart",Array.DESCENDING );
//			masterNotOnLine.sortOn( "sortIndex",Array.DESCENDING );
//			allMaster = masterOnLine.concat( masterNotOnLine );
////			allMaster.sortOn( "impart",Array.DESCENDING );
////			allMaster.sortOn( "sortIndex",Array.DESCENDING );
			sendNotification( MasterData.UPDATA_MASTER_LIST,{ allMaster:allMaster,pages:pages } );
		}
		
		//获取徒弟申请列表
		private function recStudentApplyList( len:int,bytes:ByteArray,pages:int ):void
		{
			var student:YoungStudent;
			var allStudent:Array = [];
			for( var i:uint = 0; i < len; i ++ )
			{
				student= new YoungStudent();
				student.id = bytes.readUnsignedInt();	//玩家ID
				student.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				student.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				student.roleLevel = bytes.readUnsignedShort(); //等级
				student.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				student.vip = bytes.readUnsignedByte();//vip等级
				student.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				student.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
				allStudent.push( student );
			}
			sendNotification( MasterData.REC_STU_APPLY_LIST_DATA,{ allStudent:allStudent,pages:pages } );
		}
		
		//获取我的师徒信息
		private function recMasStuInfo( len:int,bytes:ByteArray ):void
		{
			var severRelation:ServerMasterRelation;
			var allRelation:Array = [];
			for( var i:uint = 0; i < len; i ++ )
			{
				severRelation= new ServerMasterRelation();
				severRelation.id = bytes.readUnsignedInt();	//玩家ID
				severRelation.mainJob = bytes.readUnsignedInt(); //主职业/战斗力 /出徒数量
				severRelation.viceJob = bytes.readUnsignedInt(); //副职业/离线时间 /师德
				severRelation.roleLevel = bytes.readUnsignedShort(); //等级
				severRelation.mainJobLevel = bytes.readUnsignedShort(); //主职业等级//主职业
				severRelation.viceJobLevel = bytes.readUnsignedShort(); //副职业等级
				severRelation.face = bytes.readUnsignedShort(); //头像
				severRelation.chuanShouDu = bytes.readUnsignedInt();	//传授度
				severRelation.vip = bytes.readUnsignedByte();//vip等级
				severRelation.hasTeam = bytes.readUnsignedByte();//是否有队伍/性别
				severRelation.line = bytes.readUnsignedByte();//线路
				severRelation.relation = bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟 0自己
				severRelation.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
				allRelation.push( severRelation );
			}
			facade.registerCommand( AnalysisMasterRelationCommand.NAME,AnalysisMasterRelationCommand );
			sendNotification( AnalysisMasterRelationCommand.NAME,allRelation );
		}
		
		//获取徒弟申请列表
		private function recStudentList( len:int,bytes:ByteArray ):void
		{
			var student:YoungStudent;
			var allStudent:Array = [];
			for( var i:uint = 0; i < len; i ++ )
			{
				student= new YoungStudent();
				student.id = bytes.readUnsignedInt();	//玩家ID
				student.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				student.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				student.roleLevel = bytes.readUnsignedShort(); //等级
				student.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				student.vip = bytes.readUnsignedByte();//vip等级
				student.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				student.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
				student.sortIndex = -1*student.outLineTime;
				allStudent.push( student );
			}
			allStudent.sortOn( "sortIndex",Array.DESCENDING );
			sendNotification( MasterData.REC_MY_STUDENT_LIST,allStudent );
		}
		
		//踢出徒弟
		private function recDelectStudent( len:int,bytes:ByteArray ):void
		{
			var student:YoungStudent;
			var allDelStudent:Array = [];
			for( var i:uint = 0; i < len; i ++ )
			{
				student= new YoungStudent();
				student.id = bytes.readUnsignedInt();	//玩家ID
				student.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				student.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				student.roleLevel = bytes.readUnsignedShort(); //等级
				student.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				student.vip = bytes.readUnsignedByte();//vip等级
				student.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				student.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
				allDelStudent.push( student );
			}
			sendNotification( MasterData.DELETE_MY_OWN_STUDENT,allDelStudent );
		}

		//弹出协议解除时，徒弟的确认框
		private function receiveAgreeStudent( len:int,bytes:ByteArray ):void
		{
			var student:YoungStudent;
			for( var i:uint = 0; i < len; i ++ )
			{
				student= new YoungStudent();
				student.id = bytes.readUnsignedInt();	//玩家ID
				student.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				student.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				student.roleLevel = bytes.readUnsignedShort(); //等级
				student.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				student.vip = bytes.readUnsignedByte();//vip等级
				student.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				student.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( !facade.hasCommand( AgreementStudentrCommand.NAME ) )
			{
				facade.registerCommand( AgreementStudentrCommand.NAME,AgreementStudentrCommand );
			}
			sendNotification( AgreementStudentrCommand.NAME,student.name );
		}
		
		//弹出协议解除时，徒弟的确认框
		private function openStudentCommit( len:int,bytes:ByteArray ):void
		{
			var student:YoungStudent;
			for( var i:uint = 0; i < len; i ++ )
			{
				student= new YoungStudent();
				student.id = bytes.readUnsignedInt();	//玩家ID
				student.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				student.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				student.roleLevel = bytes.readUnsignedShort(); //等级
				student.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				student.vip = bytes.readUnsignedByte();//vip等级
				student.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				student.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( !facade.hasCommand( BetrayMasterCommand.NAME ) )
			{
				facade.registerCommand( BetrayMasterCommand.NAME,BetrayMasterCommand );
			}
			sendNotification( BetrayMasterCommand.NAME,student.name );
		}
		
		//获取师傅登记列表
		private function askStudentGo( len:int,bytes:ByteArray ):void
		{
			var masterName:String;
			
			for( var i:uint = 0; i < len; i ++ )
			{
				bytes.readUnsignedInt();	//玩家ID
				bytes.readUnsignedInt(); //主职业/战斗力
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				masterName = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( !facade.hasCommand( AskStudentGoCommand.NAME ) )
			{
				facade.registerCommand( AskStudentGoCommand.NAME,AskStudentGoCommand );
			}
			sendNotification( AskStudentGoCommand.NAME,masterName );
		}
		
		//去师傅身边
		private function gotoMaster( len:int,bytes:ByteArray,pages:int ):void
		{
			var line:int;
			for( var i:uint = 0; i < len; i ++ )
			{
				bytes.readUnsignedInt();	//玩家ID
				bytes.readUnsignedInt(); //主职业/战斗力
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				line = bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( !facade.hasCommand( GotoMasterSideCommand.NAME ) )
			{
				facade.registerCommand( GotoMasterSideCommand.NAME,GotoMasterSideCommand );
			}
			sendNotification( GotoMasterSideCommand.NAME,line );
		}
		
		//获取徒弟申请列表
		private function checkHasGraduate( len:int,bytes:ByteArray,pages:int ):void
		{
			for( var i:uint = 0; i < len; i ++ )
			{
				bytes.readUnsignedInt();	//玩家ID
				bytes.readUnsignedInt(); //主职业/战斗力
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			sendNotification( MasterData.REC_CHECK_HAS_GRADUATE,pages );
		}
		
		//获取徒弟申请列表
		private function recGraduatelist( len:int,bytes:ByteArray,pages:int ):void
		{
			var student:YoungStudent;
			var allStudent:Array = [];
			for( var i:uint = 0; i < len; i ++ )
			{
				student= new YoungStudent();
				student.id = bytes.readUnsignedInt();	//玩家ID
				student.batNum = bytes.readUnsignedInt(); //主职业/战斗力
				student.outLineTime = bytes.readUnsignedInt(); //副职业/离线时间
				student.roleLevel = bytes.readUnsignedShort(); //等级
				student.job = bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				student.impart = bytes.readUnsignedInt();	//传授度
				student.vip = bytes.readUnsignedByte();//vip等级
				student.sexIndex = bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				student.name = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
				student.sortIndex = -1*student.outLineTime;
				allStudent.push( student );
			}
			allStudent.sortOn( "sortIndex",Array.DESCENDING );
			sendNotification( MasterData.REC_GRADUATE_STUDENT_LIST_DATA,{ allStudent:allStudent,pages:pages } );
		}
		
		//徒弟确认出师
		private function askStudentGraduate( len:int,bytes:ByteArray ):void
		{
			var masterName:String;
			
			for( var i:uint = 0; i < len; i ++ )
			{
				bytes.readUnsignedInt();	//玩家ID
				bytes.readUnsignedInt(); //主职业/战斗力
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				masterName = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( !facade.hasCommand( AskStudentGraduateCommand.NAME ) )
			{
				facade.registerCommand( AskStudentGraduateCommand.NAME,AskStudentGraduateCommand );
			}
			sendNotification( AskStudentGraduateCommand.NAME,masterName );
		}
		
		//徒弟确认拜师
		private function askStudentPleaseMaster( len:int,bytes:ByteArray ):void
		{
			var masterName:String;
			
			for( var i:uint = 0; i < len; i ++ )
			{
				bytes.readUnsignedInt();	//玩家ID
				bytes.readUnsignedInt(); //主职业/战斗力
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				masterName = bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( !facade.hasCommand( ReceiveMasterCommand.NAME ) )
			{
				facade.registerCommand( ReceiveMasterCommand.NAME,ReceiveMasterCommand );
			}
			sendNotification( ReceiveMasterCommand.NAME,masterName );
		}
		
		//弹出答题
		private function recAnswer( len:int,bytes:ByteArray,obj:Object ):void
		{
			var answerObj:Object = new Object(); 
			for( var i:uint = 0; i < len; i ++ )
			{
				answerObj.serial = bytes.readUnsignedInt();	//玩家ID
				answerObj.readSerial = bytes.readUnsignedInt(); //主职业/战斗力
//				trace ( "题号是啥玩意："+ answerObj.readSerial  );
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			answerObj.relation = obj.nCount;
//			if ( !facade.hasMediator( MasterAnswerMediator )
			sendNotification( AnswerConst.OPEN_MASTER_ANSWER_PANEL,answerObj );

		}
		
		//答题完毕
		private function answerDone( len:int,bytes:ByteArray,obj:Object ):void
		{
			for( var i:uint = 0; i < len; i ++ )
			{
				bytes.readUnsignedInt();	//玩家ID
				bytes.readUnsignedInt(); //主职业/战斗力
				bytes.readUnsignedInt(); //副职业/离线时间
				bytes.readUnsignedShort(); //等级
				bytes.readUnsignedShort(); //主职业等级//主职业
				bytes.readUnsignedShort(); //副职业等级
				bytes.readUnsignedShort(); //头像
				bytes.readUnsignedInt();	//传授度
				bytes.readUnsignedByte();//vip等级
				bytes.readUnsignedByte();//是否有队伍/性别
				bytes.readUnsignedByte();//线路
				bytes.readUnsignedByte();//与用户的关系 1.师父;2.徒弟
				bytes.readMultiByte(16, GameCommonData.CODE);	//名字
			}
			if ( obj.nCount == 1 )
			{
				sendNotification( HintEvents.RECEIVEINFO,{ info:GameCommonData.wordDic[ "mod_ans_mas_mas_tim_1" ], color:0xffff00 } );
			}
			sendNotification( AnswerConst.CLOSE_MASTER_ANSWER_PANEL );
		}

	}
}