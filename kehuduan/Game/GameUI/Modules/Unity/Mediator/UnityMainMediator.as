package GameUI.Modules.Unity.Mediator
{
	import GameUI.ConstData.CommandList;
	import GameUI.ConstData.EventList;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.TimeCountDown.TimeData.TimeCountDownEvent;
	import GameUI.Modules.Unity.Command.SendActionCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityJopChange;
	import GameUI.Modules.Unity.UnityUtils.UnityDataShow;
	import GameUI.Modules.Unity.UnityUtils.UnityDoWork;
	import GameUI.Modules.Unity.UnityUtils.UnityUtils;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.filters.GlowFilter;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/*** 分堂页签    */
	/***         ***/
	public class UnityMainMediator extends Mediator
	{
		public static const NAME:String = "UnityMainMediator";
		
		private var parentView:MovieClip;					/** 父级面板 */
		public var unityMian:MovieClip;					/** 主帮派面板 */
		private var type:int = 0;
		private var masterDataShow:UnityDataShow;			/** 控制数据的显示 */
		private var showTime:Number = 0;					/** 打开面板的时间 */
		private var closeTime:Number = 0;					/** 关闭面板的时间 */
		private var timeSwitch:Boolean = false;				/** 时间开关 */
		private var oneMainMc:MovieClip;					/** 一个页签的mc */
		private var loadedOver:Boolean = true;				/** 是否加载完成 */
		
		private var dataType:int = 4;						/** 请求数据的顺序号 */
		
		
		
		public function UnityMainMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					UnityEvent.SHOWUNITYPAGEVIEW,
					UnityEvent.GETUNITYOTHERDATA,
					UnityEvent.CLOSEUNITYPAGEVIEW,
					UnityEvent.CLEARALL,
					UnityEvent.UPDATEOTHERDATA,
					UnityEvent.SKILLSTUDIED,
					UnityEvent.SUBDOWORK,
					UnityEvent.WORKFINISH,
					UnityEvent.CONTRIBUTEFINISH,
					UnityEvent.SHOWMYJOP
					]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case UnityEvent.SHOWUNITYPAGEVIEW:
					if(UnityConstData.firstOnline)
					{
						UnityConstData.firstOnline =false;
						clearData();		//第一次打开帮派清空缓存
					}
					if(type != UnityConstData.unityPage) 
					{
						gcAll();
						return;
					}
					oneMainMc = notification.getBody() as MovieClip;
					UnityConstData.unityMainIsOpen = true;
					controllerTime();										//控制时间间隔，判断是否刷新
					getData();												//得到所有数据
				break;
				
				case UnityEvent.CLOSEUNITYPAGEVIEW:
					gcAll();
				break;
				case UnityEvent.GETUNITYOTHERDATA:							//得到数据以后的操作
					switch(dataType)
					{
						case 0:
							dataType++;
							facade.sendNotification(SendActionCommand.SENDACTION , {type:221}); 	//请求分堂数据
							UnityConstData.unityIsSend = true;				//请求是从帮派这里发出
							//得到主堂数据，请求分堂数据
						break;
						case 1:
							dataType++;
							facade.sendNotification(UnityEvent.GETUNITYOTHERDATA);
							//得到分堂数据
						break;
						case 2:
							//得到玄武堂数据，完成显示工作
//							dataType = 0;
							//全部请求完成
							if(UnityConstData.unityMainIsOpen == false) return;	//如果面板没打开的，就不显示数据
							show(oneMainMc);
							btnInit();										//按钮初始化
							addLis();
							showCondition();								//显示分堂的条件
//							remenberSkillTime();							//记下得到技能的新数据时间
//							if(UnityConstData.lastTimeList_Built[0] != null)  masterDataShow.getNewDate(UnityConstData.lastTimeList_Built);		//根据时间差计算相应的增长值
						break;
					}
				break;
				case UnityEvent.CLEARALL:									//退帮后清空缓存
//					clearData();
				break;
				case UnityEvent.UPDATEOTHERDATA:							//更新分堂显示数据，并且可以升级
					if(UnityConstData.unityCurSelect == (notification.getBody() as int))
					{
						if(masterDataShow != null && UnityConstData.unityMainIsOpen == true) masterDataShow.show(UnityConstData.unityCurSelect);
						if(UnityConstData.unityMainIsOpen) showCondition();
					}
				break;
				case UnityEvent.SKILLSTUDIED:								//技能研究成功
					if(masterDataShow) 
					{
						var _type:int = (notification.getBody() as int) / 10; 
						var date1:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
						UnityConstData.changeSkillList[int(_type - 1)].start = date1.getTime();
						date1 = null;
						masterDataShow.skillStudied((notification.getBody() as int));
					}
				break;
				case UnityEvent.SUBDOWORK:									//开始打工
					UnityDoWork.workGo(1);
				break;
				case UnityEvent.WORKFINISH:									//打工结束
					var workData:Object = notification.getBody() as Object;
					UnityConstData.subWorkType = workData.type;				//打工类型，服务器发的
					UnityConstData.mainUnityDataObj.unityMoney 		+= workData.addUnityMoney;
					switch(UnityDoWork.WORKAWARD[UnityConstData.subWorkType])
					{
						case GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_1" ]:   //建设度
							UnityConstData.mainUnityDataObj.unityBuilt 		+= workData.addVaule;
						break;
						case GameCommonData.wordDic[ "mod_uni_lev_oth_bui_wri_3" ]:  //繁荣度
							UnityConstData.mainUnityDataObj.unitybooming 	+= workData.addVaule;
						break;
						case GameCommonData.wordDic[ "mod_uni_med_umm_han_1" ]:  // 研究度
							if(UnityConstData.otherUnityArray[UnityDoWork.subType].skillStuding == 0) return;		//没有研究的技能
							UnityConstData.otherUnityArray[UnityDoWork.subType]["skillTolExp" + UnityConstData.otherUnityArray[UnityDoWork.subType].skillStuding] += workData.addVaule; 
						break;
					}
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到帮派面板
					if(workData.Id == GameCommonData.Player.Role.Id)//自己
					{
						var msgStr:String = UnityDoWork.WORKMGS[UnityConstData.subWorkType*2 + UnityUtils.getWorkVaule(workData.addUnityMoney)]
											 + GameCommonData.wordDic[ "mod_uni_med_umm_han_2" ]+ UnityUtils.moneyToString(workData.addUnityMoney)  // 此次打工帮派获得\n金  钱：
											 + "\n"+ UnityDoWork.WORKAWARD[UnityConstData.subWorkType] +"："+ workData.addVaule
						facade.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_uni_med_umm_han_3" ] , content:msgStr});  // 帮派打工
						facade.sendNotification(TimeCountDownEvent.CLOSEWORKCOUNTDOWN);		//关闭打工倒计时
					}
				break;
				case UnityEvent.CONTRIBUTEFINISH:									//捐献成功
//					UnityConstData.contributeGetNum ++;								//得到数据后消息个数叠加
//					if(UnityConstData.contributeGetNum < UnityConstData.curContributeTotal)		//消息没发完
//					{
//						return;
//					}
//					UnityConstData.contributeGetNum == 0;							//消息个数清0
					var contributeData:Object = notification.getBody() as Object;
					if(contributeData.addUnityBuilt == 0  && contributeData.addUnityBuilt == 0 && contributeData.addUnityMoney == 0 && contributeData.addUnityContribute == 0) return;
					UnityConstData.mainUnityDataObj.unityBuilt 		+= contributeData.addUnityBuilt;		//累计叠加
					UnityConstData.mainUnityDataObj.unitybooming 	+= contributeData.addUnityBuilt;
					UnityConstData.mainUnityDataObj.unityMoney 		+= contributeData.addUnityMoney;
					facade.sendNotification(UnityEvent.UPDATEOTHERDATA , UnityConstData.unityCurSelect);		//更新到帮派面板
					if(contributeData.Id == GameCommonData.Player.Role.Id)//自己
					{
						if(UnityConstData.contributeIsOpen) facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);//关闭捐献面板
						facade.sendNotification(CommandList.RECEIVECOMMAND,{htmlText:GameCommonData.wordDic[ "mod_uni_med_umm_han_4" ]  // 捐献成功，帮派获得：\n金  钱：
																					+ UnityUtils.moneyToString(contributeData.addUnityMoney)
																					+ GameCommonData.wordDic[ "mod_uni_med_umm_han_5" ] + contributeData.addUnityBuilt  // \n建设度：
											 										+ GameCommonData.wordDic[ "mod_uni_med_umm_han_6" ] + contributeData.addUnityBuilt  // \n繁荣度：
											 										+ GameCommonData.wordDic[ "mod_uni_med_umm_han_7" ] +  contributeData.addUnityContribute  // \n你获得帮派贡献度：
																						, name:"", nAtt:9999});//发送到个人频道
					}
				break;
				case UnityEvent.SHOWMYJOP:				//显示我的职位
					if(UnityConstData.unityMainIsOpen) 
					{
						this.unityMian.txtJop.text = GameCommonData.wordDic[ "mod_uni_med_umm_han_8" ] + UnityJopChange.change(GameCommonData.Player.Role.unityJob);  // 我的职位：
					}
					if(UnityConstData.unityMenberIsOpen) 
					{
						(facade.retrieveMediator(UnityMenberMediator.NAME) as UnityMenberMediator).unity.txtMyJop.text = GameCommonData.wordDic[ "mod_uni_med_umm_han_8" ] + UnityJopChange.change(GameCommonData.Player.Role.unityJob);   // 我的职位：
					}
				break;
			}
		}
		
		private function show(mc:MovieClip):void
		{
			this.unityMian = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("UnityMainView"); 
			parentView = mc;
			parentView.addChildAt(this.unityMian , UnityMediator.PAGENUM);
			masterDataShow = new UnityDataShow(this.unityMian);
			masterDataShow.show(UnityConstData.unityCurSelect);
			facade.sendNotification(UnityEvent.SHOWMYJOP);					//显示我的职位
		} 
		
		private function btnInit():void
		{
			 if(GameCommonData.Player.Role.unityJob == 100)													//帮主不可退帮
			 {
			 	unityMian.btnOut.visible 	= false;
			 }
		}
		
		private function addLis():void
		{
			for(var i:int = 0;i < UnityConstData.unityOtherNum;i++)
			{
				this.unityMian["mcUnitySyn_" + i].addEventListener(MouseEvent.CLICK , showOtherHandler);
				this.unityMian["mcUnitySyn_" + i].addEventListener(MouseEvent.MOUSE_OVER , mouseOverHandler);
				this.unityMian["mcUnitySyn_" + i].addEventListener(MouseEvent.MOUSE_OUT , mouseOutHandler);
			}
			this.unityMian.btnApply.addEventListener(MouseEvent.CLICK , applyListHandler);					//申请列表
			this.unityMian.btnOut.addEventListener(MouseEvent.CLICK , outHandler);							//退出帮派
			this.unityMian.btnContribute.addEventListener(MouseEvent.CLICK, contributeHandler);				//捐献功能
			this.unityMian.btnStudySkill.addEventListener(MouseEvent.CLICK, studySkillHandler);				//学习技能
		}
		/** 显示时间间隔，判断是否刷新 */
		private function controllerTime():void
		{
//			if(this.timeSwitch == false)
//			{
//				var date_1:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond );
//				this.closeTime = date_1.getTime();
//				date_1 = null;
//				this.timeSwitch = true;
//			}
			var date_0:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond );
			this.showTime = date_0.getTime();
			if(this.showTime - this.closeTime >= 1000 * 60 * 2)												//打开面板后2分钟重新请求 ， 2分钟的缓存时间
			{
				this.closeTime = date_0.getTime();
				this.timeSwitch = false;
				clearData();
			}
			date_0 = null;
		}
		
		private function gcAll():void
		{
			UnityConstData.unityCurSelect = 0;
			if(UnityConstData.unityMainIsOpen == false) return;
			UnityConstData.unityMainIsOpen = false;
			
			if(masterDataShow != null)masterDataShow.gcAll();
			clearView();
			if(unityMian != null)
			{
				this.unityMian.btnApply.removeEventListener(MouseEvent.CLICK , applyListHandler);
				this.unityMian.btnOut.removeEventListener(MouseEvent.CLICK , outHandler);
				this.unityMian.btnContribute.removeEventListener(MouseEvent.CLICK, contributeHandler);				
				for(var i:int = 0;i < UnityConstData.unityOtherNum;i++)
				{
					this.unityMian["mcUnitySyn_" + i].removeEventListener(MouseEvent.CLICK , showOtherHandler);
					this.unityMian["mcUnitySyn_" + i].removeEventListener(MouseEvent.MOUSE_OVER , mouseOverHandler);
				}
			}
			remenberBuiltTime();			//记住建设度 ， 繁荣度最后操作的时间 ， 因为公用性，建设度繁荣度只有在面板关闭的时候才能重记时间
		}
		
		/** 显示分堂的条件*/
		private function showCondition():void
		{
			this.unityMian["mcUnitySyn_" + 5].gotoAndStop(1);
			this.unityMian["mcUnitySyn_" + 5].visible = false;			//校场初始化
			for(var i:int = 1;i < UnityConstData.unityOtherNum;i++)
			{
				if(UnityConstData.otherUnityArray[i].level == 0)
				{
					this.unityMian["mcUnitySyn_" + i].gotoAndStop(1);
				}
				else
				{
					this.unityMian["mcUnitySyn_" + i].gotoAndStop(2);
				}
			}
		}
		/** 鼠标经过,分堂滤镜 */
		private function mouseOverHandler(e:MouseEvent):void
		{
			var mc:MovieClip = e.currentTarget as MovieClip;
			var glFilter:GlowFilter = new GlowFilter(0xffff00, 0.5, 0.5, 0.5, 2, 1, true, false);
			mc.filters=[glFilter];
		}
		/** 鼠标移出分堂 */
		private function mouseOutHandler(e:MouseEvent):void
		{
			(e.currentTarget as MovieClip).filters = null;
		}
		/** 点击学习技能按钮 */
		private function studySkillHandler(e:MouseEvent):void
		{
//			facade.sendNotification(HintEvents.RECEIVEINFO, {info:"帮派技能即将推出，敬请期待", color:0xffff00});
			sendNotification( SkillConst.LEARN_UNITY_SKILL_PAN );
		}
		/** 点击分堂 */
		private function showOtherHandler(e:MouseEvent):void
		{
			if(UnityConstData.mainUnityDataObj == null)
			{
				getData();
				facade.sendNotification(HintEvents.RECEIVEINFO , { info:GameCommonData.wordDic[ "mod_uni_med_umm_sho_1" ] } );  // 正在更新数据，请稍后操作
				return;
			}
			var type:int = e.target.name.split("_")[1];
			if(UnityConstData.otherUnityArray[type] == null) return;					//分堂数据为空时，不可点
			if(UnityConstData.unityCurSelect == type) return;
			UnityConstData.unityCurSelect = type;
			masterDataShow.show(UnityConstData.unityCurSelect);
			
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
		/** 点击捐献按钮 */
		private function contributeHandler(e:MouseEvent):void
		{
			if(UnityConstData.contributeIsOpen == false)
			{
				clearView();
				facade.sendNotification(UnityEvent.SHOWCONTRIBUTEVIEW);
			}
			else facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);
		}
		/** 点击退出帮派按钮*/
		private function outHandler(e:MouseEvent):void
		{
			unityMian.btnOut.visible = false;
			// 确定要退出帮派吗？  提示  确定  取消
			facade.sendNotification(EventList.SHOWALERT, {comfrim:outTrade, cancel:outClose, isShowClose:false, info: GameCommonData.wordDic[ "mod_uni_med_umm_out_1" ], title:GameCommonData.wordDic[ "often_used_tip" ], comfirmTxt:GameCommonData.wordDic[ "often_used_commit" ], cancelTxt:GameCommonData.wordDic[ "often_used_cancel2" ] } );
		}
		/** 退出帮派弹出面板的确定方法*/
		private function outTrade():void
		{
			unityMian.btnOut.visible = true;
			UnityConstData.unityObj.type = 1107;														//协议号
			UnityConstData.unityObj.data = [0 , 0 , 214, 0 , 0];
			UnityActionSend.SendSynAction(UnityConstData.unityObj);										//发送创建请求后三分钟后才能再次请求
			facade.sendNotification(EventList.CLOSEUNITYVIEW);											//关闭帮会面板
		}
		/** 退出帮派取消方法*/
		private function outClose():void
		{
			unityMian.btnOut.visible = true;
		}
		/** 清楚所有的功能界面 */
		private function clearView():void
		{
			if(UnityConstData.contributeIsOpen == true) facade.sendNotification(UnityEvent.CLOSECONTRIBUTETVIEW);
			else if(UnityConstData.applyViewIsOpen == true) facade.sendNotification(UnityEvent.CLOSEAPPLYLISTVIEW);
		}
		/** 清空缓存 */
		private function clearData():void
		{
			UnityConstData.mainUnityDataObj 	= null;
		}
		/** 得到所有数据(主堂数据) */
		private function getData():void
		{
			if(UnityConstData.mainUnityDataObj)
			{
				facade.sendNotification(UnityEvent.GETUNITYOTHERDATA , 2);
			}
			else
			{
				facade.sendNotification(SendActionCommand.SENDACTION , {type:208}); 	//请求主堂数据
				dataType = 0;
			}
		}
		/** 记住技能最后操作的时间 ，每换一次分堂是重置一次时间*/
		private function remenberSkillTime():void
		{
			var date0:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond );
			for(var i:int = 1; i < 5 ; i++)
			{
				UnityConstData.lastTimeList_Skill[i] = {type:i , time:date0.getTime()};
			}
			var list:Array = UnityConstData.lastTimeList_Skill;
			date0 = null;
		}
		/** 记住建设度 ， 繁荣度最后操作的时间 ， 因为公用性，建设度繁荣度只有在面板关闭的时候重置时间 */
		private function remenberBuiltTime():void
		{
			var date0:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
			for(var i:int = 0; i < 5 ; i++)
			{
				UnityConstData.lastTimeList_Built[i] = {type:i , time:date0.getTime()};
			}
			date0 = null;
		}			
	}
}