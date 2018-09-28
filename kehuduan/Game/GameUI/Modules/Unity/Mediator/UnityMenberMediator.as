package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Team.Datas.TeamEvent;
	import GameUI.Modules.Unity.Data.MenuItemData;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityJopChange;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	
	import Net.ActionProcessor.SynMenberList;
	import Net.ActionSend.FriendSend;
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.system.System;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class UnityMenberMediator extends Mediator
	{
		public static const NAME:String = "UnityMenberMediator";
		public var loadedOver:Boolean = true;				//是否加载了全部的数据
		
		private var currentPage:int = 1;
		private var totalPage:int;
		private var Amount:int;								//一页的个数
		private var pageMenberID:Array = new Array();		//一页的成员ID号
		private var oneMenberID:int;						//选定的成员ID号
		private var menberName:String;						//选定成员的姓名
		private var menu:MenuItem;
		private var getDataNum:int;							//请求成员数据的次数
		private var rankStateList:Array = [] ;
		private var rankIndex:int = 0;						//哪一种排序
		private var isclearRank:Boolean = false;			//是否清掉了缓存，排序用
		private var isclearPage:Boolean = false;			//是否清掉了缓存，翻页用
		private var isAddLis:Boolean = false;               //是否可以在开场加监听器
		private var timeSwitch:Boolean = false;				//计时开关
		private var updataSingle:int = 0;       			//短时间内只能更新一次
		private var selectObj:Object;                       //选中的人物对象
		private var closeTime:Number = 0;					//关闭面板的时间
		
		private var parentView:MovieClip;					//父级面板
		private const type:int = 1;
		
		public function UnityMenberMediator()
		{
			super(NAME);
		}
		
		public function get unity():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				UnityEvent.SHOWUNITYPAGEVIEW,
				EventList.CLOSEUNITYVIEW,
				UnityEvent.GETMENBERLIST,
				UnityEvent.GETPAGEINFO,
				UnityEvent.GETINFO,
				UnityEvent.UNITYUPDATA,
				UnityEvent.UPDATANOTICE,
//				UnityEvent.CLOSEUNTIY,
				EventList.ENTERMAPCOMPLETE,
				EventList.CLOSE_NPC_ALL_PANEL,
				UnityEvent.CLOSEUNITYPAGEVIEW,
				UnityEvent.ADDMENBER,
				UnityEvent.MENBERUPDOWNLINE,
				UnityEvent.CLEARALL

			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case UnityEvent.SHOWUNITYPAGEVIEW:
					UnityConstData.iscreating = int(GameCommonData.Player.Role.unityJob-1) / 100;
					if(type != UnityConstData.unityPage) 
					{
						gcAll(); 
						return;
					}
					UnityConstData.unityMenberIsOpen = true;						//成员界面已打开
					initView();/** 初始化 */
					parentView = notification.getBody() as MovieClip;
					parentView.addChildAt(this.unity , UnityMediator.PAGENUM);		//添加到父级界面上
					
//					setBtnEnable(true);											    //没加载完成的的翻页按钮，不可见
					showUnityView();
					btninit();														//不可用的功能按钮初始化
					mcselectinit();													//每次都要判断一下选择条的可用性
					clearTxt();
					currentPage = 1;
//			    	totalPage   = 1;
			    	unity.txtPage.text = currentPage+"/"+totalPage;
			    	if(isAddLis == true) addLis();
			    	//排序正反状态
			    	for(var q:int = 0;q < 6;q++)
			    	{
			    		var rankState:Boolean = false;
				    	rankStateList[q] = rankState;
			    	}
					if(this.loadedOver == false)											//如果数据传输没有完成
					{
						if(UnityConstData.allMenberList[currentPage - 1] != null)  updateMenberData(UnityConstData.allMenberList[currentPage - 1]);
						else facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_um_han_2" ], color:0xffff00});
						return;	
					}
					sendAction(208 , 0 , 0);						        		//发送请求得到帮派信息
					sendAction(209 , 1 , 0);										//发送请求得到成员信息
				break;
				case UnityEvent.CLOSEUNITYPAGEVIEW:
//					if(GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0)
//					{
						gcAll();
//					}
				break;	
				case UnityEvent.GETMENBERLIST:												//得到成员列表
					var menberDataList:Array = notification.getBody() as Array;
					var menberArr:Array = menberDataList[0] as Array;
					this.getDataNum = menberDataList[1] as int;
					if(this.getDataNum > this.totalPage) return;
					
					if(this.getDataNum == 1 && isclearRank == false && isclearPage == false)
					{
						updateMenberData(menberArr);
					}
					if(this.getDataNum < this.totalPage)
					{
						sendAction(209 , this.getDataNum + 1 , 0);								//发送请求得到成员信息
						//侦听第一页的按钮条
					}
					UnityConstData.allMenberList.push(menberArr);
					if(this.getDataNum >= this.totalPage )
					{
						this.loadedOver = true;												//全部加载完成
						
						var arr34:Array = UnityConstData.allMenberList;
						if(isclearRank == true)												//重新排序
						{
							isclearRank = false;	
							
							rank(this.rankIndex);											//更新固定排行
						}
						else if(isclearPage == true)
						{
							isclearPage = false;
							if(parentView.contains(unity))  								//如果打开了面板，才更新数据
							{
								updateMenberData(UnityConstData.allMenberList[currentPage - 1]);
							}
						}
						if(updataSingle > 0)												//如果是有操作的更新
						{
							updataSingle = 0;
						}
//						setBtnEnable(true);													//加载完成所有的数据才可见翻页按钮
						selectBtnEnabled(true);												//没加载完成的话选择目标按钮不可用
						SynMenberList.isFirst = 0;///////////////////////////////1234,1
					}
				break;
				case UnityEvent.GETINFO:
					var infoArr:Array = notification.getBody() as Array;
					UnityConstData.mainUnityDataObj.name    		= infoArr[0];
					UnityConstData.mainUnityDataObj.level   		= infoArr[1] + 1;
					UnityConstData.mainUnityDataObj.oldBoss 		= infoArr[2];
					UnityConstData.mainUnityDataObj.unityMoney 		= infoArr[3];
					UnityConstData.mainUnityDataObj.unitybooming    = infoArr[4];
					UnityConstData.mainUnityDataObj.createTime   	= String(infoArr[5]);
					UnityConstData.mainUnityDataObj.unityMenber    	= infoArr[6];
					UnityConstData.mainUnityDataObj.onlineMenber   	= infoArr[11];
					UnityConstData.mainUnityDataObj.newBoss      	= infoArr[7];
					UnityConstData.mainUnityDataObj.unityBuilt    	= infoArr[8];
					UnityConstData.mainUnityDataObj.moving  	    = infoArr[9] ;
					UnityConstData.mainUnityDataObj.unityNotice     = infoArr[10]
					updateInfoData(UnityConstData.mainUnityDataObj);											//得到帮派信息
				break;
				case UnityEvent.GETPAGEINFO:											//得到页码信息，页面总数，一页的个数
					var arr:Array = notification.getBody() as Array;
					totalPage = arr[1];
					Amount    = arr[0];
					mcselectinit();
					btninit();															//不可用的功能按钮初始化
					txtinit();															//文本初始化
					addLis();
					unity.txtPage.text = currentPage+"/"+totalPage;
				break;
				case UnityEvent.UNITYUPDATA:											//更新帮派界面
					if(GameCommonData.Player.Role.unityId != 0 && UnityConstData.iscreating == 0)		//如果职业除以100得到0，则创建成功，为1，则正在创建中
					{
						if(UnityConstData.allMenberList[0] == null) return;				//如果没有缓存，就不用更新
						var utilArr:Array = notification.getBody() as Array;
						if(utilArr[1] == 0 || utilArr[1] == null || utilArr[0] == null){
							return;
						}
						if(utilArr[1] == 1) 											//删除
						{
							UnityConstData.mainUnityDataObj.unityMenber --;
							if(utilArr[0]["7"] == 1)									//上线状态
						    {
								UnityConstData.mainUnityDataObj.onlineMenber --;
						    }
							this.clearTxt();
							clearMenu();
							if(UnityConstData.allMenberList[UnityConstData.allMenberList.length - 1].length == 1)			//最后一页只有一个数据
							{
								if(this.currentPage != 1) this.currentPage -= 1;
								this.totalPage -= 1;
								unity.txtPage.text = currentPage+"/"+totalPage;
							}
						}
						else if(utilArr[1] == 2)										//添加
						{
							if(UnityConstData.allMenberList[UnityConstData.allMenberList.length - 1].length == 10)			//最后一页数据满了
							{
								this.totalPage += 1;
								unity.txtPage.text = currentPage+"/"+totalPage;
							}
						}
						var util:UnityUtils = new UnityUtils(utilArr[0] , UnityConstData.allMenberList);
						UnityConstData.allMenberList = util.operating(utilArr[1]);
						updateMenberData(UnityConstData.allMenberList[currentPage - 1]);
						updateInfoData(UnityConstData.mainUnityDataObj);				//更新帮派信息
						UnityConstData.updataArr[1] = 0;
					}
				break;
				case UnityEvent.UPDATANOTICE:											//更新公告
					if(unity != null) unity.txtUnityNotice.text = notification.getBody() as String;
					UnityConstData.mainUnityDataObj.unityNotice  = notification.getBody() as String;
				break;
				
				case UnityEvent.ADDMENBER:									//获得加入成员的数据信息
				    UnityConstData.updataArr[0] = notification.getBody() as Object;
				    UnityConstData.updataArr[1] = 2;
				    UnityConstData.mainUnityDataObj.unityMenber ++;
				    
				    if(UnityConstData.updataArr[0]["7"] == 1)									//上线状态
				    {
						UnityConstData.mainUnityDataObj.onlineMenber ++;
				    }
				    updateInfoData(UnityConstData.mainUnityDataObj);					//更新帮派信息
				    
				    facade.sendNotification(UnityEvent.UNITYUPDATA ,UnityConstData.updataArr)
		       	break;
		       	case UnityEvent.MENBERUPDOWNLINE:										//成员上下线显示
		       		UnityConstData.updataArr[0] = notification.getBody() as Object;
				    UnityConstData.updataArr[1] = 4;
				    playerOnline(notification.getBody() as Object);						//玩家上下线状态改变
				    updateInfoData(UnityConstData.mainUnityDataObj);					//更新帮派信息
//				    if(UnityConstData.updataArr[0]["7"] == 0)	 facade.sendNotification(HintEvents.RECEIVEINFO, {info:"帮派成员" + UnityConstData.updataArr[0]["0"]+"下线了", color:0xffff00});		//下线			
//				    else	facade.sendNotification(HintEvents.RECEIVEINFO, {info:"帮派成员" + UnityConstData.updataArr[0]["0"]+"上线了", color:0xffff00});	
				    facade.sendNotification(UnityEvent.UNITYUPDATA ,UnityConstData.updataArr)
		       	break;
		       	case UnityEvent.CLEARALL:									//退帮后清空缓存
					timeUp();
				break;
			}
		}
		private function initView():void
		{
			facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:UIConfigData.UNITYVIEW});
			if(unity != null)
			{
				unity.mouseEnabled = false;
				unity.txtUnityName.mouseEnabled = false;
				unity.txtUnityLevel.mouseEnabled = false;
				unity.txtOldBoss.mouseEnabled = false;
				unity.mcBind.txtMoney.mouseEnabled = false;
				unity.txtRich.mouseEnabled = false;  
				unity.txtCreateTime.mouseEnabled = false;
				unity.txtMenberNum.mouseEnabled = false;
				unity.txtNowBoss.mouseEnabled = false;
				unity.txtCreate.mouseEnabled = false;
//				unity.txtMoving.mouseEnabled = false;
				for(var i:int = 0;i < 10 ; i++)
				{
					unity["txtName_"+i].mouseEnabled = false;
					unity["txtManLevel_"+i].mouseEnabled = false;
					unity["txtUnity_"+i].mouseEnabled = false;
					unity["txtLevel_"+i].mouseEnabled = false;
					unity["txtJop_"+i].mouseEnabled = false;
					unity["txtCribute_"+i].mouseEnabled = false;
				}
				
				facade.sendNotification(UnityEvent.SHOWMYJOP);					//显示我的职位
			}
		}
		private function showUnityView():void
		{
//			if(closeTime == 0) return;
		     if(this.timeSwitch == false)
		    {
		    	var dateTime_1:Date = new Date();										//时间对象
			    this.closeTime = dateTime_1.getTime();								//关闭面板后开始计时
			    dateTime_1 = null;
			    timeSwitch = true;						
		    }
			
			var dateTime_0:Date = new Date();
		    if((dateTime_0.getTime() - this.closeTime) >= 1000 * 60)				//打开面板后1分钟请求
		    {
		    	timeUp();
		    	timeSwitch = false;
		    }
		    dateTime_0 = null;
		}		
		
		private function addLis():void
		{
			/** 功能按钮侦听器*/
			unity.btnApply.addEventListener(MouseEvent.CLICK, applyListHandler);
			unity.btnContribute.addEventListener(MouseEvent.CLICK, contributeHandler);	//捐献功能需要
			unity.btnPerfect.addEventListener(MouseEvent.CLICK, perfectHandler);
			unity.btnOut.addEventListener(MouseEvent.CLICK, outHandler);
			unity.btnStudySkill.addEventListener(MouseEvent.CLICK, studySkillHandler);	//学习技能
			/** 数据翻页选中侦听器*/
			unity.btnFrontPage.addEventListener(MouseEvent.CLICK,FrontHandler);			//点击向左按钮
			unity.btnBackPage.addEventListener(MouseEvent.CLICK,BackHandler);			//点击向右按钮
			if(unity.stage) unity.stage.addEventListener(MouseEvent.CLICK , stageHandler);	
			for(var i:int = 0;i < 10;i++)
			{
				unity["btnselect_" + i].addEventListener(MouseEvent.CLICK, selectHandler); 
			}
			for(var n:int = 0;n < 6 ; n++)
			{
				unity["btnSort_" + n].addEventListener(MouseEvent.CLICK , sortHandler);
			}
			mcselectinit();					//每次都要判断一下选择条的可用性
		}
		
		private function gcAll():void
		{
			if(UnityConstData.unityMenberIsOpen == false) return;
			UnityConstData.unityMenberIsOpen = false;						//成员界面已打开
			/** 功能按钮侦听器*/
			unity.btnApply.removeEventListener(MouseEvent.CLICK, applyListHandler);
			unity.btnPerfect.removeEventListener(MouseEvent.CLICK, perfectHandler);
			unity.btnOut.removeEventListener(MouseEvent.CLICK, outHandler);
			/** 数据翻页选中侦听器*/
			unity.btnFrontPage.removeEventListener(MouseEvent.CLICK,FrontHandler);			//点击向左按钮
			unity.btnBackPage.removeEventListener(MouseEvent.CLICK,BackHandler);			//点击向右按钮
			if(unity.stage) unity.stage.removeEventListener(MouseEvent.CLICK , stageHandler);	
			for(var i:int = 0;i < 10;i++)
			{
				unity["btnselect_" + i].removeEventListener(MouseEvent.CLICK, selectHandler); 
			}
			for(var n:int = 0;n < 6 ; n++)
			{
				unity["btnSort_" + n].removeEventListener(MouseEvent.CLICK , sortHandler);
			}
			clearView();							
		    pageMenberID = [];		
		    oneMenberID = 0;
		    menberName = "";
		    clearTxt();
//		    totalPage = 0;
		    clearMenu();
//		    SynMenberList.isFirst = 0;
		    UnityConstData.isOpenNpcView = false;
		    this.isAddLis = true;
		}
		/** 选中按钮条初始化 */
		private function mcselectinit():void
		{
			for(var i:int = 0;i < 10;i++)
			{
				unity["mcselect_"+i].visible       = false;
				unity["btnselect_"+i].visible       = false;
				unity["btnselect_"+i].mouseEnabled = true;	
				unity["mcselect_"+i].doubleClickEnabled = true; 
			}
			for(var n:int = 0; n < Amount; n++) {
				unity["btnselect_"+n].visible = true;
			}
		}
		/** 动态文本不可选初始化*/
		private function txtinit():void
		{
			for(var i:int ;i < 10;i++)
			{
				unity["txtName_" + i].mouseEnabled     = false;
				unity["txtManLevel_" + i].mouseEnabled = false;
				unity["txtUnity_" + i].mouseEnabled    = false;
				unity["txtLevel_" + i].mouseEnabled    = false;
				unity["txtJop_" + i].mouseEnabled      = false;
				unity["txtCribute_" + i].mouseEnabled  = false;
			}
		}
		/** 点击学习技能按钮 */
		private function studySkillHandler(e:MouseEvent):void
		{
//			facade.sendNotification(HintEvents.RECEIVEINFO, {info:"帮派技能即将推出，敬请期待", color:0xffff00});
			sendNotification( SkillConst.LEARN_UNITY_SKILL_PAN );
		}
		/** 点击申请列表按钮 */
		private function applyListHandler(e:MouseEvent):void
		{
			if(UnityConstData.applyViewIsOpen == false)
			{
				clearView();															//关闭所有的功能面板
				facade.sendNotification(UnityEvent.SHOWAPPLYLISTVIEW);
			}
			else 
			{
				facade.sendNotification(UnityEvent.CLOSEAPPLYLISTVIEW);
			}
			
		}
		/** 点击任命按钮 */
		private function ordainHandler():void
		{
			if(UnityConstData.ordainIsOpen == false)
			{
				this.selectObj = getSelectObj();		//获得选中对象
				
				clearView();
				facade.sendNotification(UnityEvent.SHOWORDAINVIEW , [oneMenberID , menberName , this.selectObj]);
			}
			else
				facade.sendNotification(UnityEvent.CLOSEORDAINVIEW);
		}
		/** 点击捐款按钮 */
		private function contributeHandler(e:MouseEvent):void
		{
			if(UnityConstData.contributeIsOpen == false)
			{
				clearView();
				facade.sendNotification(UnityEvent.SHOWCONTRIBUTEVIEW);
			}
			else
			{
				facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);
			}
		} 
		/** 点击修改公告按钮 */
		private function perfectHandler(e:MouseEvent):void
		{
			if(UnityConstData.perfectNoticeIsOpen == false)
			{
				clearView();
				facade.sendNotification(UnityEvent.SHOWPERFECTVIEW);
			}
			else
				facade.sendNotification(UnityEvent.CLOSEPERFECTVIEW);
		}
		/** 点击踢出帮派按钮*/
		private function letOutHandler():void
		{
//			unity.btnLetOut.visible = false;
			// 确定要将  踢出帮派吗？ 提示  确定  取消
			facade.sendNotification(EventList.SHOWALERT, {comfrim:letOutTrade, cancel:letOutClose, isShowClose:false, info: GameCommonData.wordDic[ "mod_uni_med_umme_let_1" ] + menberName+GameCommonData.wordDic[ "mod_uni_med_umme_let_2" ], 
																							title:GameCommonData.wordDic[ "often_used_tip" ], comfirmTxt:GameCommonData.wordDic[ "often_used_commit" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel2" ] } );
		}
		/** 点击退出帮派按钮*/
		private function outHandler(e:MouseEvent):void
		{
			unity.btnOut.visible = false;
			// 确定要退出帮派吗？ 提示 确定  取消
			facade.sendNotification(EventList.SHOWALERT, {comfrim:outTrade, cancel:outClose, isShowClose:false, info: GameCommonData.wordDic[ "mod_uni_med_umm_out_1" ], title:GameCommonData.wordDic[ "often_used_tip" ], 
																							comfirmTxt:GameCommonData.wordDic[ "often_used_commit" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel" ] } ); 
		}
		/** 清楚所有的功能界面 */
		private function clearView():void
		{
			if(UnityConstData.contributeIsOpen == true) facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);
			else if(UnityConstData.ordainIsOpen == true) facade.sendNotification(UnityEvent.CLOSEORDAINVIEW);
			else if(UnityConstData.perfectNoticeIsOpen == true) facade.sendNotification(UnityEvent.CLOSEPERFECTVIEW);
			else if(UnityConstData.applyViewIsOpen == true) facade.sendNotification(UnityEvent.CLOSEAPPLYLISTVIEW);
		}
		/** 点击向左按钮翻页 */
		private function FrontHandler(e:MouseEvent):void
		{
			if(currentPage > 1)
			{
				currentPage--;
				page();
			}
		}
		/** 点击向右按钮翻页 */
		private function BackHandler(e:MouseEvent):void
		{
			if(UnityConstData.allMenberList[currentPage] == null || UnityConstData.allMenberList[currentPage - 1] == null)		//如果下一页或当前数据为空，就看不见翻页按钮
			{
				if(currentPage < totalPage) facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_med_um_han_2" ], color:0xffff00});  // 数据传输中
				return;
			} 
			if(currentPage < totalPage)
			{
				currentPage++;
				page();
			}
		}
		/** 点击人物条*/
		private function selectHandler(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			oneMenberID = pageMenberID[i];
			menberName  = unity["txtName_"+i].text;	
			clearMenu();
			if(GameCommonData.Player.Role.Id == oneMenberID) return;
			if(GameCommonData.Player.Role.unityJob < 70 )	//精英以下都没有任命和提出帮派功能
			{
				MenuItemData.menuDataInit();
				this.menu = new MenuItem(MenuItemData.dataArr);
			}
			else if(GameCommonData.Player.Role.unityJob > 70 && GameCommonData.Player.Role.unityJob < 90)	//副堂主和堂主就显示任命功能
			{
				MenuItemData.menuDataInit();
				MenuItemData.dataArr.push({cellText:GameCommonData.wordDic[ "mod_uni_med_umme_sel_1" ],data:{type:GameCommonData.wordDic[ "mod_uni_med_umme_sel_1" ] } } );  // 帮派任命  帮派任命
				this.menu = new MenuItem(MenuItemData.dataArr);
			}
			else if(GameCommonData.Player.Role.unityJob >= 90 && GameCommonData.Player.Role.unityJob <= 100)//副帮主和帮主有任命和提出帮派功能
			{
				MenuItemData.menuDataInit();
				MenuItemData.dataArr.push({cellText:GameCommonData.wordDic[ "mod_uni_med_umme_sel_1" ],data:{type:GameCommonData.wordDic[ "mod_uni_med_umme_sel_1" ]}});  // 帮派任命  帮派任命 
				MenuItemData.dataArr.push({cellText:GameCommonData.wordDic[ "mod_uni_med_umme_sel_2" ],data:{type:GameCommonData.wordDic[ "mod_uni_med_umme_sel_2" ]}});		  			   //选中某个人物后任命和提出帮派功能才可用  踢出帮派  踢出帮派
				this.menu = new MenuItem(MenuItemData.dataArr);
			}
			menu.addEventListener(MenuEvent.Cell_Click , menuHandler);  
			unity.addChild(menu);
			menu.x = unity.mouseX;
			menu.y = unity.mouseY;
			e.stopPropagation();				//点击舞台
//			selectedUnity(i);
		}
		/** 点击弹出的菜单 */
		private function menuHandler(e:MenuEvent):void
		{
			switch(e.cell.data.type)
			{
//				case "发送消息":
//					clearMenu();
//					facade.sendNotification(EventList.UNITY_SEND_MSG , menberName);
//				break;
				case GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ]:  // 设为私聊
					clearMenu();
					facade.sendNotification(ChatEvents.QUICKCHAT , menberName);
				break;
				case GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ]:  // 组    队
					clearMenu();
					facade.sendNotification(TeamEvent.SUPER_MAKE_TEAM , oneMenberID);
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_men_men_1" ]:  // 复制名字
					clearMenu();
					System.setClipboard(menberName);
				break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_1" ]:  // 查看资料
					clearMenu();
					FriendSend.getInstance().getFriendInfo(oneMenberID , menberName);
				break;
				case GameCommonData.wordDic[ "mod_pla_med_tea_onc_4" ]:  // 加为好友
					clearMenu();
					sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:oneMenberID  , name:menberName});
				break;
				case GameCommonData.wordDic[ "mod_uni_med_umme_sel_1" ]:  // 帮派任命
					clearMenu();
					ordainHandler();
				break;
				case GameCommonData.wordDic[ "mod_uni_med_umme_sel_2" ]:  // 踢出帮派
					clearMenu();
					letOutHandler();
				break;
			}
		}
		/** 点击舞台 */
		private function stageHandler(e:MouseEvent):void
		{
			clearMenu();
		}
		/**  点击排序 */
		private function sortHandler(e:MouseEvent):void
		{
			var i:int = e.target.name.split("_")[1];
			rank(i);
		}
		/** 任命，踢出帮派的按钮初始化*/
		private function btninit():void
		{
			 unity.btnOrdain.visible  = false;
			 unity.btnLetOut.visible  = false;
			 if(GameCommonData.Player.Role.unityJob == 100)													//帮主不可退帮
			 {
			 	unity.btnOut.visible 	= false;
			 }
			 else
			 unity.btnOut.visible 	= true;
			 if(GameCommonData.Player.Role.unityJob == 90 || GameCommonData.Player.Role.unityJob == 100)	//只有正负帮主才可以修改公告
			 unity.btnPerfect.visible 	= true;
			 else
			 unity.btnPerfect.visible 	= false;
		}
		/** 翻页按钮设置 */
		private function setBtnEnable(isOver:Boolean):void
		{
			unity.btnFrontPage.visible = isOver;
			unity.btnBackPage.visible = isOver;
		}
		/** 选中某个人物 */
		private function selectedUnity(i:int):void
		{   																	
			mcselectinit();																					//选中按钮初始化																
			oneMenberID = pageMenberID[i];
			menberName  = unity["txtName_"+i].text;															//存储姓名
			unity["mcselect_"+i].visible = true;
			if(GameCommonData.Player.Role.unityJob <= 20 || GameCommonData.Player.Role.Id == oneMenberID)	//没有权限或是选择了自己
			{
				unity.btnOrdain.visible = false;																
				unity.btnLetOut.visible = false;
			}
			else																							//如果职位不是果农和帮众 并且 没有选中自己
			{
				unity.btnOrdain.visible = true;																//选中某个人物后任命和提出帮派功能才可用
				unity.btnLetOut.visible = true;
			}
			for(var n:int = 0;n < 10;n++)
			{
				unity["mcselect_"+n].visible == true ? unity["btnselect_"+n].mouseEnabled = false:unity["btnselect_"+n].mouseEnabled = true;			//选中的按钮不需要鼠标移上效果
			}
		}
		/** 踢出帮派弹出面板的确定取消方法*/
		private function letOutTrade():void
		{
			
			sendAction(213 , 0 , oneMenberID);																//发送踢出帮派的请求
		}
		private function letOutClose():void
		{
		}
		/** 退出帮派弹出面板的确定取消方法*/
		private function outTrade():void
		{
			unity.btnOut.visible = true;
			sendAction(214 , 0 , 0);																		//发送退出帮派的请求
			facade.sendNotification(EventList.CLOSEUNITYVIEW);												//关闭帮会面板
//			gcAll();																						//因为退出帮派后，状态为没有加入帮派，所以不能发送关闭事件
		}
		private function outClose():void
		{
			unity.btnOut.visible = true;
		}
		/** 发送请求帮派成员 和 详细数据信息方法 */
		private function sendAction(type:int , currentPage:int , id:int):void
		{
			//如果帮派详细数据有缓存，也不打扰服务器
			if(UnityConstData.mainUnityDataObj != null &&type == 208)
			{
				updateInfoData(UnityConstData.mainUnityDataObj);
				return;
			}
			//如果成员有缓存 , 就不打扰服务器了
			if(UnityConstData.allMenberList[currentPage - 1] != null && type == 209 )
			{
				updateMenberData(UnityConstData.allMenberList[currentPage - 1]);
				setBtnEnable(true);											//翻页按钮，可见
				return;
			}
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , type, currentPage , id];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求后三分钟后才能再次请求
			if(type == 209) 
			{
				this.loadedOver = false;												
	//			setBtnEnable(false);																		//没加载完成的的翻页按钮，不可见
				selectBtnEnabled(false);																	//没加载完成的话选择目标按钮不可用
			}
		}
		/** 更新帮派成员数据 */
		private function updateMenberData(menberArr:Array):void
		{
			if(menberArr == null) return;
			Amount = menberArr.length;
//			setBtnEnable(true);											//踢人后发送踢人请求会让按钮变灰
//			this.totalPage = UnityConstData.allMenberList.length == 0 ? totalPage : UnityConstData.allMenberList.length;//UnityConstData.allUnityList.length;
			Amount = menberArr.length;
			mcselectinit();												//每次都要判断一下选择条的可用性
			selectBtnEnabled(true);				                       //选择目标按钮 , 排序按钮可用
			btninit();													//每次都判断一下权限按钮是否可用
			for(var i:int; i < Amount ;i++)
			{
				if(menberArr[i]["7"] == 1)
				{
					unity["txtName_"+i].htmlText 	 = "<font color='#00FF00'>" + menberArr[i]["0"] + "</font>";
					unity["txtManLevel_"+i].htmlText = "<font color='#00FF00'>" + menberArr[i]["1"] + "</font>";
					unity["txtUnity_"+i].htmlText    = "<font color='#00FF00'>" + GameCommonData.RolesListDic[menberArr[i]["2"]] + "</font>";
					unity["txtLevel_"+i].htmlText    = "<font color='#00FF00'>" + menberArr[i]["3"]+ "</font>";
					unity["txtJop_"+i].htmlText      = "<font color='#00FF00'>" + UnityJopChange.change(menberArr[i]["4"])+ "</font>";
					unity["txtCribute_"+i].htmlText  = "<font color='#00FF00'>" + menberArr[i]["5"]+ "</font>";
				}
				else
				{
					unity["txtName_"+i].htmlText 	 = "<font color='#CCCCCC'>" + menberArr[i]["0"] + "</font>";
					unity["txtManLevel_"+i].htmlText = "<font color='#CCCCCC'>" + menberArr[i]["1"] + "</font>";
					unity["txtUnity_"+i].htmlText    = "<font color='#CCCCCC'>" + GameCommonData.RolesListDic[menberArr[i]["2"]] + "</font>";
					unity["txtLevel_"+i].htmlText    = "<font color='#CCCCCC'>" + menberArr[i]["3"]+ "</font>";
					unity["txtJop_"+i].htmlText      = "<font color='#CCCCCC'>" + UnityJopChange.change(menberArr[i]["4"])+ "</font>";
					unity["txtCribute_"+i].htmlText  = "<font color='#CCCCCC'>" + menberArr[i]["5"]+ "</font>";
				}
				pageMenberID[i] = menberArr[i][6];															//将一页的成员ID储存在pageMenberID数组里面
			}
		}
		/** 更新帮派信息 */
		private function updateInfoData(obj:Object):void
		{
			if(unity == null || UnityConstData.unityMenberIsOpen == false) return;
			unity.txtUnityName.text    = obj.name;
//			unity.txtUnityName.text = "天下第一的帮派"
			unity.txtUnityLevel.text   = obj.level;
			unity.txtOldBoss.text      = obj.oldBoss;
			unity.mcBind.txtMoney.text = UIUtils.getMoneyStandInfo(obj.unityMoney, ["\\se","\\ss","\\sc"]);
			ShowMoney.ShowIcon(unity.mcBind, unity.mcBind.txtMoney);
			unity.txtRich.text         = obj.unitybooming;
			unity.txtCreateTime.text   = String(obj.createTime).slice(0,4) + "/" + String(obj.createTime).slice(4,6) + "/" +String(obj.createTime).slice(6,8) + "/" +String(obj.createTime).slice(8,11);
			unity.txtMenberNum.text    = obj.onlineMenber + "/" + obj.unityMenber + "/" + UnityNumTopChange.change(unity.txtUnityLevel.text) ;
			unity.txtNowBoss.text      = obj.newBoss;
			unity.txtCreate.text       = obj.unityBuilt;
//			unity.txtMoving.text       = obj.moving + "/100";
			UnityConstData.unityLevel = unity.txtUnityLevel.text;			//储存帮派等级数据
			if(obj.unityNotice == null)
				unity.txtUnityNotice.text = "";
			else
			unity.txtUnityNotice.text  = obj.unityNotice ;
			UnityConstData.mainUnityDataObj.unityNotice  = obj.unityNotice ;
		}
		/** 清空缓存 */
		private function clearTxt():void
		{
			for(var i:int; i < Amount ;i++)
			{
				unity["txtName_"+i].text     = "";
				unity["txtManLevel_"+i].text = "";
				unity["txtUnity_"+i].text    = "";
				unity["txtLevel_"+i].text    = "";
				unity["txtJop_"+i].text      = "";
				unity["txtCribute_"+i].text  = "";
				pageMenberID[i] = "";															//将一页的成员ID储存在pageMenberID数组里面
			}
		}
		private function clearMenu():void
		{
			if(menu)
			{
				if(unity.contains(menu))
				{
					unity.removeChild(menu);
				}
			}
		}
		/** 心跳执行的函数*/
		private function timeUp():void
		{
			UnityConstData.allMenberList = [];
			SynMenberList.isFirst = 0;
		}
		/** 排序方法 */
		private function rank(i:int):void
		{
			if(UnityConstData.allMenberList[0] == null)												
			{
				clearTxt();
				clear();
				rankIndex = i;
//				sendAction(209 , 1 , 0);														//发送请求;
				selectBtnEnabled(false);														//没加载完成的话选择目标按钮不可用
				isclearRank = true;																//设置状态，重新排序
				return;
			} 
			if(rankStateList[i] == false)
			{
				rankStateList[i] = true;
				UnityConstData.allMenberList = UIUtils.ArraysortOn(UnityConstData.SORTLIST[i] , UnityConstData.allMenberList , i , true);
			}
			else
			{
				rankStateList[i] = false;
				UnityConstData.allMenberList = UIUtils.ArraysortOn(UnityConstData.SORTLIST[i] , UnityConstData.allMenberList , i , false);
			}
//			mcselectinit();																					//初始化按钮条
			btninit();																						//初始化任命，提出帮派按钮
			clearTxt();
			updateMenberData(UnityConstData.allMenberList[0]);
			this.currentPage = 1;
			unity.txtPage.text = currentPage+"/"+totalPage;
		}
		/** 翻页方法 */
		private function page():void
		{
			unity.txtPage.text = currentPage+"/"+totalPage;
			btninit();																						//初始化任命，提出帮派按钮
			clearTxt();
			if(UnityConstData.allMenberList[currentPage - 1] == null)												
			{
				clear();
				isclearPage = true;
				return;
			}
			updateMenberData(UnityConstData.allMenberList[currentPage - 1]);
		}
		/** 清除 */
		private function clear():void
		{
			currentPage = 1;
			this.totalPage = 1;
		}
		/** 选择角色按钮是否可用*/
		private function selectBtnEnabled(isUse:Boolean):void
		{
			//数据没来之前，按钮禁用
			for(var i:int = 0;i < Amount;i++)
			{
				unity["btnselect_"+i].visible       = isUse;
				unity["btnselect_"+i].mouseEnabled  = isUse;	
			}
			for(var n:int = 0;n < 6 ; n++)
			{
				unity["btnSort_" + n].mouseEnabled  = isUse;	
			}
		}
		/** 提出选中者的Object*/
		private function getSelectObj(id:int = 0):Object
		{
			var obj:Object = null;
			if(id == 0)	id = this.oneMenberID;
			for(var i:int = 0;i < (UnityConstData.allMenberList[currentPage - 1] as Array).length;i++)
			{
//				trace((UnityConstData.allMenberList[currentPage - 1][i] as Object)["6"])
				if((UnityConstData.allMenberList[currentPage - 1][i] as Object)["6"] == id)
				   obj = UnityConstData.allMenberList[currentPage - 1][i] as Object;
			}
			//退出帮派 角色ID不是在当前页
			if(obj == null)
			{
				for(var n:int = 0;n < (UnityConstData.allMenberList as Array).length;n++)
				{
					if(n == currentPage - 1) continue;
					for(var k:int = 0;k < (UnityConstData.allMenberList[n] as Array).length;k++)
					{
						if((UnityConstData.allMenberList[n][k] as Object)["6"] == id)
						obj = UnityConstData.allMenberList[n][k] as Object;
					}
				}
			}
			return obj;
		}
		/** 玩家上下线信息控制 */
		private function playerOnline(obj:Object):void
		{
			if(obj["7"] == 0)							//下线
		    {
				if(UnityConstData.mainUnityDataObj) UnityConstData.mainUnityDataObj.onlineMenber --;
		    }
		    else										//上线
		    {
				if(UnityConstData.mainUnityDataObj) UnityConstData.mainUnityDataObj.onlineMenber ++;
		    }
		}
	}
}