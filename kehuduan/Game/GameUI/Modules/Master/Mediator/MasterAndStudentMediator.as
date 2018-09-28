package GameUI.Modules.Master.Mediator
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.Master.Proxy.OldMaster;
	import GameUI.Modules.Master.Proxy.RequestTutor;
	import GameUI.Modules.Master.Proxy.ServerMasterRelation;
	import GameUI.Modules.Master.View.MyMasterCell;
	import GameUI.Modules.Master.View.MyStudentCell;
	import GameUI.Modules.Task.View.TaskText;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.UIScrollPane;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.TextEvent;
	import flash.text.TextField;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MasterAndStudentMediator extends Mediator
	{
		public static const NAME:String = "MasterAndStudentMediator";
		private var main_mc:MovieClip;
		
		private var noMaster_mc:MovieClip;
		private var noStudentOne_mc:MovieClip;
		private var noStudentTwo_mc:MovieClip;
		private var noStudentThree_mc:MovieClip;
		private var getOutTimer:Timer = new Timer(200, 1);	//物品取出计时
		
		private var cellContainer:Sprite;
		
		private var isRegist:Boolean = true;					//是否登记过  默认为false 
		private var hasStudent:Boolean = false;				//是否有徒弟
		private var fxText:TaskText;
		
		private var mySelfRelation:ServerMasterRelation;
		private var myMaster:OldMaster;
		private var myStudentsArr:Array = [];
		private var scrollPanel:UIScrollPane;
		private var friendMediator:FriendManagerMediator;
		private var isInit:Boolean = false;							//是否进行过初始化
		
		private var mainMediator:MasStuMainMediator;
		
		private var studentContainer:Sprite = new Sprite();							//'装徒弟的容器
	
		public function MasterAndStudentMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							MasterData.MASTER_STU_UI_INITVIEW,
							MasterData.START_SHOW_MAS_PAGE,
							MasterData.CLEAR_MASTER_PANEL_PAGE,
							MasterData.REC_MAS_STU_INFO,
							MasterData.DELETE_MY_OWN_STUDENT
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case MasterData.MASTER_STU_UI_INITVIEW:
					initUI();
				break;
				case MasterData.START_SHOW_MAS_PAGE:
					if ( notification.getBody().curPage == 1 )
					{
						showUI();
					}
				break;
				case MasterData.CLEAR_MASTER_PANEL_PAGE:
					if ( notification.getBody().curPage == 1 )
					{
						clearUI();
					}
				break;
				case MasterData.REC_MAS_STU_INFO: 
					if ( !mainMediator )
					{
						mainMediator = facade.retrieveMediator( MasStuMainMediator.NAME ) as MasStuMainMediator;
					}
					if ( mainMediator.masterCurPage != 1 ) return;
					var obj:Object = notification.getBody();
					this.mySelfRelation = obj.mySelfRelation;
					this.myMaster = obj.master;
					this.myStudentsArr = obj.aStudents;
					showMyInfo();
					showMyMasterInfo();
					showMyStudentInfo();
				break;
				case MasterData.DELETE_MY_OWN_STUDENT:
					if ( isInit )
					{
						delStudent( notification.getBody() as Array );
					}
				break;
			}
		}
		
		//资源加载后的初始化
		private function initUI():void
		{
			main_mc = this.viewComponent as MovieClip;
			cellContainer = new Sprite();
			cellContainer.mouseEnabled = false;
			main_mc.addChild( cellContainer );
			
			myStudentsArr = [];
			isInit = true;
		}
		
		private function showUI():void
		{
			clearContainer();
			checkRequest();
		}
		
		private function checkRequest():void
		{
			if ( startTimer() )
			{
				requeseData();
			}
			else
			{
				showMyInfo();
				showMyMasterInfo();
				showMyStudentInfo();
			}
		}
		
		//请求信息
		private function requeseData():void
		{
			RequestTutor.requestData( 24,0 );
		}
		
		private function showMyInfo():void
		{
			if ( mySelfRelation )
			{
				main_mc.rate_txt.mouseEnabled = false;
				main_mc.teachLevel_txt.mouseEnabled = false;
				main_mc.masterRate_txt.mouseEnabled = false;
				main_mc.studentNum_txt.mouseEnabled = false;
				
				if ( mySelfRelation.chuanShouDu>UIConstData.ExpDic[ 9000+MasterData.getChuanShouLevel( mySelfRelation.chuanShouDu ) ] )
				{
					mySelfRelation.chuanShouDu = UIConstData.ExpDic[ 9000+MasterData.getChuanShouLevel( mySelfRelation.chuanShouDu ) ];
				}
				main_mc.rate_txt.text = mySelfRelation.chuanShouDu+"/"+UIConstData.ExpDic[ 9000+MasterData.getChuanShouLevel( mySelfRelation.chuanShouDu ) ];
				main_mc.teachLevel_txt.htmlText = MasterData.getPrenticeRemark( mySelfRelation.chuanShouDu );
				main_mc.masterRate_txt.text = mySelfRelation.viceJob;
				main_mc.studentNum_txt.text = mySelfRelation.mainJob;
				if ( mySelfRelation.roleLevel == 0 )
				{
					isRegist = false;
				}
				else
				{
					isRegist = true;	
				}
				
				if ( myStudentsArr.length>0 )
				{
					isRegist = true;
				}
			}
		}
		
		private function showMyMasterInfo():void
		{
			if ( !myMaster )
			{
				noMaster_mc = new ( MasterData.masResPro.noMasterClass ) as MovieClip;
				noMaster_mc.x = 15;
				noMaster_mc.y = 240;
				cellContainer.addChild( noMaster_mc );
				( noMaster_mc.txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_mas_med_masta_smm_1" ]+'<font color ="#00ff00"><a href="event:masterList">'+GameCommonData.wordDic[ "mod_mas_med_masta_smm_2" ]+'</a></font>'+GameCommonData.wordDic[ "mod_mas_med_masta_smm_3" ];   //在        师傅列表       内选择师傅
				( noMaster_mc.txt as TextField ).addEventListener( TextEvent.LINK,goMasterList );
			}
			else
			{
				var masterCell:MyMasterCell = new MyMasterCell( myMaster );
				masterCell.x = 21;
				masterCell.y = 200;
				masterCell.talkMasterFun = talkToStudent;
				cellContainer.addChild( masterCell );
			}
		}
		
		private function showMyStudentInfo():void
		{
			if ( GameCommonData.Player.Role.Level<35 )
			{
				noStudentOne_mc = new ( MasterData.masResPro.noStudentOneClass ) as MovieClip;
				noStudentOne_mc.x = 200;
				noStudentOne_mc.y = 70;
				cellContainer.addChild( noStudentOne_mc );
			}
			else
			{
				if ( isRegist )        //登记过
				{
					if ( myStudentsArr.length == 0 )			//没徒弟
					{
						noStudentThree_mc = new ( MasterData.masResPro.noStudentThreeClass ) as MovieClip;
						noStudentThree_mc.x = 200;
						noStudentThree_mc.y = 70;
						cellContainer.addChild( noStudentThree_mc );
						( noStudentThree_mc.txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_mas_med_masta_sms_1" ]+'<font color ="#00ff00"><a href="event:regist">'+GameCommonData.wordDic[ "mod_mas_med_masta_sms_2" ]+'</a></font>'+GameCommonData.wordDic[ "mod_mas_med_masta_sms_3" ];     //你已在师徒列表中登记，         点击这里           查看徒弟申请
						( noStudentThree_mc.txt as TextField ).addEventListener( TextEvent.LINK,goApplyList );
						fxText = new TaskText( 444 );
						fxText.x = 287;
						fxText.y = 120;                         //也可前往江陵                                                                                                                 桃李老人                                                                         或开封                                                                                                                         天机老人                                                                          处收徒
						fxText.tfText = "<font color='#E2CCA5'>"+GameCommonData.wordDic[ "mod_mas_med_masta_sms_4" ]+"<font color='#0099ff'><a href='event:1001,90,81,0,117'>"+GameCommonData.wordDic[ "mod_mas_med_masta_sms_5" ]+"\\fx</a></font>"+GameCommonData.wordDic[ "mod_mas_med_masta_sms_6" ]+"<font color='#0099ff'><a href='event:1006,43,125,0,614'>"+GameCommonData.wordDic[ "mod_mas_med_masta_sms_7" ]+"\\fx</a></font>"+GameCommonData.wordDic[ "mod_mas_med_masta_sms_8" ]+"</font>";
						cellContainer.addChild( fxText );
					}
					else				//有徒弟了
					{
						addStudent();
					}
				}
				else					//没登记过
				{
					noStudentTwo_mc = new ( MasterData.masResPro.noStudentTwoClass ) as MovieClip;
					noStudentTwo_mc.x = 200;
					noStudentTwo_mc.y = 70;
					( noStudentTwo_mc.txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_mas_med_masta_sms_9" ]+'<font color ="#00ff00"><a href="event:regist">'+GameCommonData.wordDic[ "mod_mas_med_masta_sms_2" ]+'</a></font>'+GameCommonData.wordDic[ "mod_mas_med_masta_sms_10" ];  //你可以收徒了，          点击这里            登记成为师傅
					cellContainer.addChild( noStudentTwo_mc );
					( noStudentTwo_mc.txt as TextField ).addEventListener( TextEvent.LINK,registMaster );
				}
			}
			
		}
		
		//添加徒弟
		private function addStudent():void
		{
			var sWidth:Number = 450;
			var sHeight:Number = 345;
			
			clearStudentContainer();
			var cell:MyStudentCell;
			for ( var i:uint=0; i<myStudentsArr.length; i++ )
			{
				cell = new MyStudentCell( myStudentsArr[i] );
				cell.y = i * 86;
				cell.talkStudentFun = talkToStudent;
				studentContainer.addChild( cell );
			}
			
			var containerHeight:Number = 86*myStudentsArr.length;
			studentContainer.graphics.clear();
			studentContainer.graphics.beginFill( 0xff0000,0 );
			studentContainer.graphics.drawRect( 0,0,sWidth,containerHeight );
			studentContainer.graphics.endFill();
			
			scrollPanel = new UIScrollPane( studentContainer );
			scrollPanel.width = sWidth + 5;
			scrollPanel.height = sHeight;
			scrollPanel.scrollPolicy = UIScrollPane.SCROLLBAR_AS_NEEDED;
			scrollPanel.refresh();
			scrollPanel.scrollBottom();
			scrollPanel.refresh();
			
//			studentContainer.x = 201;
//			studentContainer.y = 55;
			scrollPanel.x = 201;
			scrollPanel.y = 55;
			cellContainer.addChild( scrollPanel );
		}
		
		//删除徒弟
		private function delStudent( delArr:Array ):void
		{
			if ( !myStudentsArr ) return;
			var index:int;
			for ( var i:uint=0; i<delArr.length; i++ )
			{
				var delId:int = delArr[i].id;
				for ( var j:uint=0; j<myStudentsArr.length; j++ )
				{
					if ( delId == myStudentsArr[j].id )
					{
						myStudentsArr.splice( j,1 );
					}
				}
			}
			addStudent();
		}
		
		private function removeMC( mc:MovieClip ):void
		{
			if ( mc && cellContainer.contains( mc ) )
			{
				cellContainer.removeChild( mc );
				mc = null;
			}
		}
		
		private function goApplyList( evt:TextEvent ):void
		{
			clearUI();      //先清空数据
			sendNotification( MasterData.GO_MS_MC_PAGE_CUR,2 );
		}
		
		private function registMaster( evt:TextEvent ):void
		{
			if ( GameCommonData.Player.Role.Level < 35 )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, { info:GameCommonData.wordDic[ "mod_mas_med_masta_reg_1" ], color:0xffff00 } );       //人物等级达到35级才能登记！
				return;
			}
			clearUI();      //先清空数据
			sendNotification( MasterData.GO_MS_MC_PAGE_CUR,0 );
			RequestTutor.requestData( 11,0 );
		}
		
		private function goMasterList( evt:TextEvent ):void
		{
			clearUI();      //先清空数据
			sendNotification( MasterData.GO_MS_MC_PAGE_CUR,0 );
		}
		
		//操作延时器
		private function startTimer():Boolean
		{
			if(getOutTimer.running) 
			{
				return false;
			} 
			else 
			{
				getOutTimer.reset();
				getOutTimer.start();
				return true;
			}
		}
		
		//和徒弟说话  和师傅说话，同一个接口
		private function talkToStudent( _name:String ):void
		{
			var friendList:Array = [];
			var roleInfo:PlayerInfoStruct;
			friendMediator = facade.retrieveMediator( FriendManagerMediator.NAME ) as FriendManagerMediator;
			friendList = friendMediator.dataList.concat();
			
			var enemyList:Array = friendMediator.enemyDataList.concat();
			
			for ( var i:uint=0; i<friendList.length; i++ )
			{
				for ( var j:uint=0; j<friendList[i].length; j++ )
				{
					if ( _name == friendList[i][j].name1 )
					{
						roleInfo = friendList[i][j].roleInfo;
						break;
					}
				}
			} 
			
			if ( !roleInfo )
			{
				for ( var k:uint=0; k<enemyList.length; k++ )
				{
					if ( _name == friendList[k].name1 )
					{
						roleInfo = friendList[k].roleInfo;
						break;
					}
				}
			}
			
			if ( roleInfo )
			{
				sendNotification( FriendCommandList.SHOW_SEND_MSG,roleInfo );
			}
			else
			{
				UIFacade.GetInstance( UIFacade.FACADEKEY ).ShowHint( GameCommonData.wordDic[ "mod_mas_med_masta_tal_1" ] );     //请先加对方为好友
			}
		}
		
		//清除
		private function clearUI():void
		{
			if ( noMaster_mc && cellContainer.contains( noMaster_mc ) )
			{
				( noMaster_mc.txt as TextField ).removeEventListener( TextEvent.LINK,goMasterList );
//				cellContainer.removeChild( noMaster_mc );
//				noMaster_mc = null;
			}
//			if ( noStudentOne_mc )
//			{
//				removeMC( noStudentOne_mc );
//			}
			if ( noStudentTwo_mc )
			{
				( noStudentTwo_mc.txt as TextField ).removeEventListener( TextEvent.LINK,registMaster );
//				removeMC( noStudentTwo_mc );
			}
			if ( noStudentThree_mc )
			{
				( noStudentThree_mc.txt as TextField ).removeEventListener( TextEvent.LINK,goApplyList );
//				fxText = null;
//				removeMC( noStudentThree_mc );
			}
			clearContainer();
		}
		
		private function clearStudentContainer():void
		{
			var des:*;
			while ( studentContainer.numChildren>0 )
			{
				des = studentContainer.removeChildAt( 0 );
				if ( des is MyStudentCell )
				{
					( des as MyStudentCell ).gc();
				}
				des = null;
			}
		}
		
		private function clearContainer():void
		{
			clearStudentContainer();
			var des:*;
			while ( cellContainer.numChildren > 0 )
			{
				des = cellContainer.removeChildAt( 0 );
				if ( des is MyMasterCell )
				{
					( des as MyMasterCell ).gc();
				}
				des = null;
			}
		}
		
	}
}