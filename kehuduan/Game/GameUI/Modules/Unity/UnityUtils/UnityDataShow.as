package GameUI.Modules.Unity.UnityUtils
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Unity.Command.SendActionCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityJopChange;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.LevelUpCondition.LevelWork;
	import GameUI.Modules.Unity.LevelUpCondition.OtherLevelUpCondition.OtherLevelWork;
	import GameUI.UIUtils;
	import GameUI.View.ShowMoney;
	import GameUI.View.items.FaceItem;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	import OopsFramework.Utils.Timer;
	
	import flash.display.Bitmap;
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	/** 显示主堂，分堂详细数据 */
	public class UnityDataShow implements IUpdateable 
	{
		private var _parent:MovieClip;			/** 帮派分堂主界面 */
		private var _type:int;					/** 选中分堂的序号 */
		private var mcUnityOther:MovieClip; 	
		private var mcUnity:MovieClip;
		private var obj:Object;         		/** 分堂的数据对象 */
		private var lightBit:Bitmap				/** 选中高光 */
		private var bulitBar:ProgressBarUtil;
		private var boomingBar:ProgressBarUtil;
		private var skillBar_0:ProgressBarUtil;
		private var skillBar_1:ProgressBarUtil;
		private var skillBar_2:ProgressBarUtil;
		private var skillBarArr:Array = [skillBar_0 , skillBar_1 , skillBar_2];
		private var unitylevelWork:LevelWork;	/** 帮派升级控制对象*/
		private var otherlevelWork:OtherLevelWork;	/** 帮派升级控制对象*/
		private var isFirst:Boolean = false;			/** 第一次打开*/
		private var skillIconList:Array = [];		/** 技能图标数组 */
		
		private static var lastTypeArray:Array = [];			/** 上一个分堂序列数组*/
		
		private var timer:Timer = new Timer();		/** 定时器 */
		
//		private var skillArrayBar:Array = []
//		private var timer:UnityTimer;			/** 数值定时器*/
		
		private const LIGHTOPINTARR:Array = [new Point(206,-33) , new Point(206,9) , new Point(206 , 51)];
		 
		private var enabled:Boolean = true;
		
		public function get Enabled():Boolean
		{
			return enabled; 
		}
		public function Update(gameTime:GameTime):void
		{
			if(timer.IsNextTime(gameTime))// || isFirst)//为什么价格first
			{
				isFirst = false;
//				barSkillGrow();
//				barBuiltGrow();
				
			}
		}
		
		public function get UpdateOrder():int{return 0}			// 更新优先级（数值小的优先更新）
		public function get EnabledChanged():Function{return null};
		public function set EnabledChanged(value:Function):void{};
        public function get UpdateOrderChanged():Function{return null};
        public function set UpdateOrderChanged(value:Function):void{};
		
		public function UnityDataShow(parent:MovieClip )
		{
			_parent = parent;
		}
		
		public function show(type:int):void
		{
			timer.DistanceTime = 1000 * 60;		//循环时间为1分钟
			gcAll();
			isFirst = true;
			_type = type;
			moneyOut();							//金钱超出上限
			if(_type == 0) showUnity();			//主帮的显示
			else otherBuilt();					//分堂的显示 ,含有showOther方法
//			if(obj.level >0) GameCommonData.GameInstance.GameUI.Elements.Add(this);			//添加心跳				//数据静止了
			///得到新数据
//			getNewDate(UnityConstData.lastTimeList_Skill);
			////
		}
		
		public function gcAll():void
		{
//			GameCommonData.GameInstance.GameUI.Elements.Remove(this);						//数据静止了
			deleteSkillIcon();				//删除所有的技能图标
			bulitBar = null;
			boomingBar = null;
			unitylevelWork = null;
			for(var i:int = 0;i < 3;i++)
			{
				this.skillBarArr[i] = null;
			}
			this.mcUnity = null;
			this.mcUnityOther = null;
			for(var k:int = 0; k < 5;k++)
			{
				var deleteMc:DisplayObject = _parent.getChildByName("mcUnity" + k);
				if(deleteMc == null) continue;
				_parent.removeChild(deleteMc);
			} 
			for(var n:int = 0;n < 3; n++)
			{
				if(mcUnityOther && mcUnityOther["btnSkill_" + n].hasEventListener(MouseEvent.CLICK)) mcUnityOther["btnSkill_" + n].removeEventListener(MouseEvent.CLICK , btnSkillHandler); 
			}
			if(mcUnityOther && mcUnityOther.btnHire.hasEventListener(MouseEvent.CLICK)) mcUnityOther.btnHire.removeEventListener(MouseEvent.CLICK , hireHandler);
			if(mcUnityOther && mcUnityOther.btnWork.hasEventListener(MouseEvent.CLICK)) mcUnityOther.btnWork.removeEventListener(MouseEvent.CLICK , workHandler);
			if(UnityConstData.hireViewIsOpen == true) GameCommonData.UIFacadeIntance.sendNotification(UnityEvent.CLOSEHIREVIEW);			//关闭雇佣面板
		}
		
		/** 显示主帮的数据 */
		private function showUnity():void
		{
			remove();
			if(mcUnity == null) mcUnity = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("UnityClass_Main") as MovieClip;
			mcUnity.name = "mcUnity" + this._type;		//实例名
			////后期去掉
			mcUnity.mcUnityLevel.stop();
			////	
			_parent.addChild(mcUnity);
			mcInit(mcUnity);
			unityDataUpdate();
		}
		/** 显示分堂的数据*/
		private function showOther():void
		{
			remove();
			if(mcUnityOther == null) mcUnityOther = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("UnityClass_Qing") as MovieClip; 
			mcUnityOther.name = "mcUnity" + this._type;		//实例名
			////后期去掉
			mcUnityOther.mcUnityLevel.stop();
			////		
			_parent.addChild(mcUnityOther);
			mcInit(mcUnityOther);
			otherDataUpdate();
		}
		/** 数据mc的初始化 */
		private function mcInit(mc:MovieClip):void
		{
			mc.x = 10;
			mc.y = 225;
			mc.mouseEnabled = false;
		}
		/** 移除所有的数据面板 */
		private function remove():void
		{
			if(mcUnityOther != null && _parent.contains(mcUnityOther))
			{
				
				 _parent.removeChild(mcUnityOther);
			}
			if(mcUnity != null && _parent.contains(mcUnity))
			{
				_parent.removeChild(mcUnity);
			} 
		}
		/** 主帮数据的排列更新 */
		private function unityDataUpdate():void
		{
			UnityConstData.mainUnityDataObj.craftsmanNum = int(UnityConstData.greenUnityDataObj.craftsmanNum) + int(UnityConstData.whiteUnityDataObj.craftsmanNum) + int(UnityConstData.xuanUnityDataObj.craftsmanNum) + int(UnityConstData.redUnityDataObj.craftsmanNum);
			UnityConstData.mainUnityDataObj.businessmanNum = int(UnityConstData.greenUnityDataObj.businessmanNum) + int(UnityConstData.whiteUnityDataObj.businessmanNum) + int(UnityConstData.xuanUnityDataObj.businessmanNum) + int(UnityConstData.redUnityDataObj.businessmanNum);
			UnityConstData.mainUnityDataObj.masterNum = int(UnityConstData.greenUnityDataObj.masterNum) + int(UnityConstData.whiteUnityDataObj.masterNum) + int(UnityConstData.xuanUnityDataObj.masterNum) + int(UnityConstData.redUnityDataObj.masterNum);
			UnityConstData.mainUnityDataObj.hireNum = this.getOtherNum();																					//得到帮派当前可雇用的总数
			
			/** 数据显示 */
			this.obj = UnityConstData.mainUnityDataObj;
			mcUnity.txtLevel.text              	= GameCommonData.wordDic[ "mod_uni_uni_uds_udu_1" ] + GameCommonData.wordDic[ "mod_uni_uni_uds_udu_2" ] + UnityNumTopChange.numToChina(obj.level)  + GameCommonData.wordDic[ "often_used_level" ];  // 聚义堂  等级:  级
			mcUnity.unityName.text				= obj.name;
			mcUnity.txtUnityMainLevel.text      = UnityNumTopChange.numToChina(obj.level) + GameCommonData.wordDic[ "often_used_level" ];  // 级
//			mcUnity.txtOldBoss.text				= obj.oldBoss;
			mcUnity.txtNowBoss.text				= obj.newBoss;
			mcUnity.txtCreateTime.text			= String(obj.createTime).slice(0,4) + "/" + String(obj.createTime).slice(4,6) + "/" +String(obj.createTime).slice(6,8) + "/" +String(obj.createTime).slice(8,11);;
			mcUnity.TxtMenberNum.text			= obj.unityMenber;
			mcUnity.txtCraftsmanNum.text 		= obj.craftsmanNum + "/" + obj.hireNum;																		//分堂的工匠总数上线
			mcUnity.txtBusinessmanNum.text 		= obj.businessmanNum + "/" + obj.hireNum;																	//分堂的商人总数上线
			mcUnity.txtMasterNum.text 			= obj.masterNum + "/" + obj.hireNum;																		//分堂的武学大师总数上线
			mcUnity.txtAddBuilt.text      		= "+"+obj.craftsmanNum+GameCommonData.wordDic[ "mod_uni_uni_uds_udu_3" ];					// 		/分													//每分钟追加的建设度
			mcUnity.txtBoomingAdd.text   	 	= "+"+obj.businessmanNum+GameCommonData.wordDic[ "mod_uni_uni_uds_udu_3" ];															   					//每分钟追加的繁荣度  /分
			mcUnity.txtMoving.text      	 	= obj.moving + "/100";																						//帮派行动力
			if(UnityConstData.UnityPerformanceClose)     mcUnity.txtUnityState.text				= GameCommonData.wordDic[ "mod_uni_uni_uds_udu_4" ];  // （暂停维护）
			else  mcUnity.txtUnityState.text				= "";
			mcUnity.txtUnityState.mouseEnabled = false;
			
			mcUnity.mcUnityBind.txtMoney.text  		= UIUtils.getMoneyStandInfo(UnityConstData.mainUnityDataObj.unityMoney, ["\\se","\\ss","\\sc"]);			//帮派资金
			 
			for(var i:int = 1; i < 5 ; i++)				//给分堂的技能等级赋值
			{
			 	var subObj:Object = UnityConstData.otherUnityArray[i];
				for(var n:int = 0;n < 3;n++)
				{
					var skillArray:Array = UnityJopChange.getSkillLevel(Number(subObj["skillTolExp"+int(n+1)]));
					subObj["skillStudyCurr"+int(n+1)] = skillArray[0];				//得到当前技能等级
					subObj["skillStudyNum"+int(n+1)] = skillArray[1];				//得到当前技能经验
				}
			}
			
			/**  数据操作 */
			(mcUnity.mcUnityBind as MovieClip).mouseEnabled = true;
			(mcUnity.mcUnityBind as MovieClip).mouseChildren = false;
			UnityConstData.levelNeedList = [];																												//升级悬浮框数据清空
			showBar();																																		//显示进度条
			unitylevelWork 						= new LevelWork();																							//升级控制类	
			levelBtnEnabled(mcUnity,unitylevelWork.showCondition());																						//显示主堂是否可以升级
			UnityUtils.levelUpNeedData(obj);																												//升级按钮悬浮框数据初始化
			ShowMoney.ShowIcon(mcUnity.mcUnityBind, mcUnity.mcUnityBind.txtMoney);																					//显示金钱
			
		}
		/** 分堂数据的排列更新 */
		private function otherDataUpdate():void
		{
			synLevelTest();																																	//检测分堂等级是否大于主堂等级
			this.obj = UnityConstData.otherUnityArray[_type];
			/** ----- 数据显示 -----*/
			mcUnityOther.txtOtherLevel.text   	= obj.name + GameCommonData.wordDic[ "mod_uni_uni_uds_udu_2" ] + UnityNumTopChange.numToChina(obj.level)  + GameCommonData.wordDic[ "often_used_level" ];					// 	等级:  	级					//分堂等级
			mcUnityOther.mcUnityBind.txtMoney.text   = UIUtils.getMoneyStandInfo(UnityConstData.mainUnityDataObj.unityMoney, ["\\se","\\ss","\\sc"]);		//帮派资金
			mcUnityOther.txtCraftsmanNum.text 	= obj.craftsmanNum+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "workMan");							//分堂的工匠总数上线
			mcUnityOther.txtBusinessmanNum.text = obj.businessmanNum+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "workMan");							//分堂的商人总数上线
			mcUnityOther.txtMasterNum.text 		= obj.masterNum+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "workMan");								//分堂的武学大师总数上线
			mcUnityOther.txtBulitAdd.text      	= "+"+obj.craftsmanNum+GameCommonData.wordDic[ "mod_uni_uni_uds_udu_3" ];					// 			/分												//每分钟追加的建设度
			mcUnityOther.txtBoomingAdd.text     = "+"+obj.businessmanNum+GameCommonData.wordDic[ "mod_uni_uni_uds_udu_3" ];							// 		/分						   					//每分钟追加的繁荣度
			if(this.obj.isStop) mcUnityOther.txtSubState.text = GameCommonData.wordDic[ "mod_uni_uni_uds_odu_1" ];  // （已关闭）
			else mcUnityOther.txtSubState.text = "";
			mcUnityOther.txtSubState.mouseEnabled = false;
			
			for(var n:int = 0;n < 3;n++)
			{
				var skillArray:Array = UnityJopChange.getSkillLevel(Number(obj["skillTolExp"+int(n+1)]));
				if(skillArray[0] >= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill")))
				{
					obj["skillStudyCurr"+int(n+1)] 	= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill"));
					obj["skillStudyNum"+int(n+1)] 	= 0;
				}
				else
				{
					obj["skillStudyCurr"+int(n+1)] = skillArray[0];				//得到当前技能等级
					obj["skillStudyNum"+int(n+1)] = skillArray[1];				//得到当前技能经验
				}
				mcUnityOther["txtSkillName_" + n].text   = obj["skillName" + int(n+1)];																		//技能名
				mcUnityOther["txtSkillLevel_"+ n].htmlText  = "<font color='#00FF00'>"+obj["skillStudySelf"+int(n+1)]+"</font><font color='#FFFFFF'>/"+obj["skillStudyCurr"+int(n+1)]+'/'+UnityNumTopChange.UnityOtherChange(obj.level , 'skill')+"</font>";	//技能等级
				mcUnityOther["txtSkillAdd_"+n].text    	 = "+"+obj.masterNum+GameCommonData.wordDic[ "mod_uni_uni_uds_udu_3" ];				// /分
				//下面一句加的有点莫名其妙，暂时屏蔽掉			去广州之前												//每分钟追加的技能值
//				if(obj["skillStudyNum" + int(n+1)] > UIConstData.ExpDic[6001 + obj["skillStudyCurr" + obj.skillStuding]]) 
//				{
//					obj["skillStudyNum" + int(n+1)] = UIConstData.ExpDic[6000 + obj["skillStudyCurr" + obj.skillStuding]];
//				}
				////////////////////////////////////////////////////////////////
			}
			
			/** ----- 数据操作 -----*/
			(mcUnityOther.mcUnityBind as MovieClip).mouseEnabled = true;
			(mcUnityOther.mcUnityBind as MovieClip).mouseChildren = false;
			UnityConstData.levelNeedList = [];																								//升级悬浮框数据清空
			addOtherLis();																													//添加按钮侦听器
			showBar();																														//显示进度条
			ShowMoney.ShowIcon(mcUnityOther.mcUnityBind, mcUnityOther.mcUnityBind.txtMoney);															//显示金钱
			skillUp();																														//研究技能操作
			otherlevelWork = new OtherLevelWork(obj);																						//升级控制类
			levelBtnEnabled(mcUnityOther,otherlevelWork.showCondition());																	//显示主堂是否可以升级
			UnityUtils.levelUpNeedData(obj);																							       //升级按钮悬浮框数据初始化
			loadSkillIcon();																												//显示技能图标
		}
		/** 添加按钮侦听器 */
		private function addOtherLis():void
		{
			mcUnityOther.btnHire.addEventListener(MouseEvent.CLICK , hireHandler);
			mcUnityOther.btnWork.addEventListener(MouseEvent.CLICK , workHandler);
		}
		
		/** 数据操作：技能研究 */
		private function skillUp():void
		{
//			this.lightBit = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("UnitySkillLight") as Bitmap;
//			this.lightBit.width  = 306;
//			this.lightBit.height = 105;
//			this.lightBit.alpha  = .5;
			for(var i:int = 0; i < 3; i++)
			{
				mcUnityOther["mcSkillSelect_" + i].gotoAndStop(1);
				(mcUnityOther["txtUp_" + i] as TextField).mouseEnabled = false;
				mcUnityOther["txtUp_" + i].text = GameCommonData.wordDic[ "mod_uni_uni_uds_ski_1" ];   // 研究
				mcUnityOther["txtUp_" + i].textColor = 0xFFFFFF;
				mcUnityOther["btnSkill_" + i].addEventListener(MouseEvent.CLICK , btnSkillHandler); 
			}
			if(obj.skillStuding != 0)
			{
				mcUnityOther["mcSkillSelect_" + int(obj.skillStuding - 1)].gotoAndStop(2);
				mcUnityOther["txtUp_" + int(obj.skillStuding - 1)].textColor = 0x00FF00;
//				mcUnityOther.addChild(this.lightBit);
//				this.lightBit.x = LIGHTOPINTARR[obj.skillStuding - 1].x;
//				this.lightBit.y = LIGHTOPINTARR[obj.skillStuding - 1].y;
			}
		}
		/** 得到帮派已创建的分堂数量 */
		private function getOtherNum():int
		{
			var n:int = 0;
			for(var i:int = 1; i < 5; i++)
			{
				if(UnityConstData.otherUnityArray[i].level != 0)
				{
					n += int(UnityNumTopChange.UnityOtherChange(UnityConstData.otherUnityArray[i].level , "workMan"))
				} 
			}
			return n;
		}
		/** 升级按钮是否可用 */
		private function levelBtnEnabled(mc:MovieClip , isUse:Boolean):void
		{
			if(mc.getChildByName("mcUnityLevel") as MovieClip != null && mc.contains(mc.getChildByName("mcUnityLevel") as MovieClip))
			{
				if(isUse == true)
				{
					mc.mcUnityLevel.gotoAndStop(1);
					(mc.mcUnityLevel as MovieClip).btnUnityLevel.addEventListener(MouseEvent.CLICK , levelUpHandler);
				}
				else
				{
//					(mc.mcLevel as MovieClip).addEventListener(MouseEvent.MOUSE_OVER , levelUpNeedDataHandler);			//监听升级悬浮框功能
					mc.mcUnityLevel.gotoAndStop(2);
				}
			}
		}
		/** 点击升级按钮 */
		private function levelUpHandler(e:MouseEvent):void
		{
			if(obj.isStop)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_lev_1" ], color:0xffff00});  // 该堂暂时关闭
				return;
			}
			if(UnityUtils.getHasSelfTopJop(_type) == false)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00});   // 你的权限不够
				return;
			}
			GameCommonData.UIFacadeIntance.sendNotification(SendActionCommand.SENDACTION , {type:224 , data:_type});		//	发送升级请求
		}
		/** 点击技能研究按钮 */
		private function btnSkillHandler(e:MouseEvent):void
		{
//			GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:"帮派技能即将推出，敬请期待", color:0xffff00});
//			return;
			if(obj.isStop)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_lev_1" ], color:0xffff00});  // 该堂暂时关闭
				return;
			}
			if(UnityUtils.getHasSelfTopJop(_type) == false)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00}); // 你的权限不够
				return;
			}
			var type:int = e.target.name.split("_")[1];
			if(this.obj.masterNum == 0)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_btn_1" ], color:0xffff00});  // 没有雇佣武师
				return;
			} 
			if(type == (obj.skillStuding - 1))
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_btn_2" ], color:0xffff00});  // 当前正在研究该技能
				return;
			}
			//切换间隔
			var date0:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
			UnityConstData.changeSkillList[int(_type - 1)].over = date0.getTime();
			date0 = null;
			if((UnityConstData.changeSkillList[int(_type - 1)].over - UnityConstData.changeSkillList[int(_type - 1)].start) / (1000 * 60) <= 3 )	//间隔小于3分钟
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_btn_3" ], color:0xffff00});  // 研究该堂的技能需间隔3分钟
				return;
			}
			GameCommonData.UIFacadeIntance.sendNotification(SendActionCommand.SENDACTION , {type:225 , data:int(_type * 10 + type + 1)});
//			var date1:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
//			UnityConstData.changeSkillList[int(_type - 1)].start = date1.getTime();
//			date1 = null;
		}
		/** 点击雇佣按钮*/
		private function hireHandler(e:MouseEvent):void
		{
			if(obj.isStop)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_lev_1" ], color:0xffff00});  // 该堂暂时关闭
				return;
			}
			if(UnityUtils.getHasSelfTopJop(_type) == false)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00});  // 你的权限不够
				return;
			}
			GameCommonData.UIFacadeIntance.sendNotification(UnityEvent.SHOWHIREVIEW , _type);
		}
		/** 点击打工按钮*/
		private function workHandler(e:MouseEvent):void
		{
			if(obj.isStop)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_lev_1" ], color:0xffff00});  // 该堂暂时关闭
				return;
			}
			if(UnityUtils.getSelfMenber(_type) == false)			
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_wor_1" ], color:0xffff00});  // 不能在其它堂打工
			}
			else if(UnityConstData.UnityPerformanceClose == true)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_wor_2" ], color:0xffff00}); // 帮派停止维护，打工暂时关闭
			}
			else UnityDoWork.beginWork(_type);
		}
		/** 点击建造分堂按钮 */
		private function builtHandler(e:MouseEvent):void
		{
			var ob:Object = UnityConstData.mainUnityDataObj;
			if(GameCommonData.Player.Role.unityJob < 90)
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_med_alm_agr_1" ], color:0xffff00});  // 你的权限不够
				return;
			}
			if(UnityUtils.getOtherNum() >= int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "otherTopNum")))			//如果分堂数已达到上限
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_uni_uds_bui_1" ], color:0xffff00});  // 分堂数已达到上限
				return;
			}
			if(int(UnityConstData.mainUnityDataObj.unityBuilt) < int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")))
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_uni_uds_bui_2" ], color:0xffff00});  // 帮派建设度不足
				return;
			}
			else if(UnityConstData.mainUnityDataObj.unityMoney < UnityNumTopChange.UnityOtherChange(obj.level , "otherLevelUpMoney"))
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_uni_uds_bui_3" ], color:0xffff00});  // 帮派资金不足
				return;
			}
			else if(int(UnityConstData.mainUnityDataObj.unitybooming) < int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")))
			{
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_uni_uni_uds_bui_4" ], color:0xffff00});  // 帮派繁荣度不足
				return;
			}
			GameCommonData.UIFacadeIntance.sendNotification(SendActionCommand.SENDACTION , {type:224 , data:_type});		//	发送升级请求
//			otherBuilt();
		}
		
		/** 显示进度条 */
		private function showBar():void
		{
			//上线限制
			if(UnityConstData.mainUnityDataObj.unityBuilt >= UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , 'mainBulit'))  						//大于建筑度上线
			{
				UnityConstData.mainUnityDataObj.unityBuilt = UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , 'mainBulit');
			}
			if(UnityConstData.mainUnityDataObj.unitybooming >= UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , 'mainBulit'))				//大于繁荣度上线
			{
				UnityConstData.mainUnityDataObj.unitybooming = UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , 'mainBulit');
			}
			//技能所需研究度xml表UIConstData.ExpDic[4001+_level]
			//_type不为0代表其它分堂
			if(_type != 0)
			{
				//技能进度条
				for(var i:int = 0;i < 3;i++)
				{
					var skillStudy:int = int(obj["skillStudyNum" + int(i+1)]);
					var skillTop:int = UIConstData.ExpDic[6001 + obj["skillStudyCurr" + int(i + 1)]];			//经验xml表中6000开头的值
					if(skillBarArr[i] == null) skillBarArr[i] = new ProgressBarUtil(mcUnityOther , skillStudy , skillTop,120,292,int(22+i*42));
					skillBarArr[i].showBar(skillStudy , skillTop);
				}
				//建设度
				if(this.bulitBar  == null) this.bulitBar = new ProgressBarUtil(mcUnityOther,int(UnityConstData.mainUnityDataObj.unityBuilt),int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")),133,52,45);
				bulitBar.showBar(int(UnityConstData.mainUnityDataObj.unityBuilt) , int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")));
				bulitBar.name = "bulitBar";
				//繁荣度
				if(this.boomingBar == null) this.boomingBar = new ProgressBarUtil(mcUnityOther,int(UnityConstData.mainUnityDataObj.unitybooming),int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")),133,52,65);
				boomingBar.showBar(int(UnityConstData.mainUnityDataObj.unitybooming) , int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")));
				boomingBar.name = "boomingBar" ;
				
				//显示是否升级
				otherlevelWork = new OtherLevelWork(obj);																						//升级控制类
				levelBtnEnabled(mcUnityOther,otherlevelWork.showCondition());																	//显示主堂是否可以升级
			}
			//type为0代表主堂
			else
			{
				//建设度
				if(this.bulitBar  == null) this.bulitBar = new ProgressBarUtil(mcUnity,int(UnityConstData.mainUnityDataObj.unityBuilt),int(UnityNumTopChange.UnityOtherChange(obj.level , "mainBulit")),133,52,45);
				bulitBar.showBar(int(UnityConstData.mainUnityDataObj.unityBuilt) , int(UnityNumTopChange.UnityOtherChange(obj.level , "mainBulit")));
				bulitBar.name = "bulitBar";
				//繁荣度
				if(this.boomingBar == null) this.boomingBar = new ProgressBarUtil(mcUnity,int(UnityConstData.mainUnityDataObj.unitybooming),int(UnityNumTopChange.UnityOtherChange(obj.level , "mainBulit")),133,52,65);
				boomingBar.showBar(int(UnityConstData.mainUnityDataObj.unitybooming) , int(UnityNumTopChange.UnityOtherChange(obj.level , "mainBulit")));
				boomingBar.name = "boomingBar" ;
				
				//显示是否升级
				unitylevelWork 						= new LevelWork();																							//升级控制类	
				levelBtnEnabled(mcUnity,unitylevelWork.showCondition());																						//显示主堂是否可以升级
			}
		}
		/** 判断当前分堂是否已建造 */
		private function otherBuilt():void
		{
			//分堂尚未建造
			if((UnityConstData.otherUnityArray[_type] as Object).level == 0)
			{
				showOther();
				(mcUnityOther.txtCreate as TextField).mouseEnabled = false;
				mcUnityOther.btnBuilt.visible	= true;
				mcUnityOther.txtCreate.visible 	= true;
				mcUnityOther.btnHire.visible 	= false;
				mcUnityOther.btnWork.visible 	= false;
				for(var i:int = 0;i < 3;i++)
				{
					mcUnityOther["btnSkill_" + i].visible = false;
				}
				mcUnityOther.txtOtherLevel.text   	= obj.name +GameCommonData.wordDic[ "mod_uni_uni_uds_oth_1" ] ;	 // :未建造
				mcUnityOther.btnBuilt.addEventListener(MouseEvent.CLICK , builtHandler);
			}
			//分堂已建造
			else
			{
				showOther();											//分堂的显示
				mcUnityOther.btnBuilt.visible 	= false;
				mcUnityOther.txtCreate.visible 	= false;
				mcUnityOther.btnHire.visible 	= true;
				mcUnityOther.btnWork.visible 	= true;
				for(var n:int = 0;n < 3;n++)
				{
					mcUnityOther["btnSkill_" + n].visible = true;																		
				}
				if(mcUnityOther.btnBuilt.hasEventListener(MouseEvent.CLICK)) mcUnityOther.btnBuilt.removeEventListener(MouseEvent.CLICK , builtHandler);
//				usePerformance(mcUnityOther.btnHire);																					//是否有雇佣的权限	
//				otherIsClose();											//判断分堂是否已关闭
				
			}
		}
		/** 增加技能的进度 i=1为每分钟加的值*/
//		private function barSkillGrow(i:int = 1):void
//		{
//			if(i == 0) return;
//			obj["skillTolExp" + obj.skillStuding] += i*obj.masterNum;
//			var topExp:int = UIConstData.ExpDic[6001 + obj["skillStudyCurr" + obj.skillStuding]];
//			
//			if(_type == 0 || obj.skillStuding == 0) return;		
//			obj["skillStudyCurr"+obj.skillStuding] = UnityJopChange.getSkillLevel(obj["skillTolExp" + obj.skillStuding])[0];
//			obj["skillStudyNum"+obj.skillStuding] = UnityJopChange.getSkillLevel(obj["skillTolExp" + obj.skillStuding])[1]
//			mcUnityOther["txtSkillLevel_"+ (obj.skillStuding -1)].htmlText  = "<font color='#00FF00'>"+obj["skillStudySelf"+obj.skillStuding]+"</font><font color='#FFFFFF'>/"+obj["skillStudyCurr"+obj.skillStuding]+'/'+UnityNumTopChange.UnityOtherChange(obj.level , 'skill')+"</font>";	//技能等级
//			
//			if(obj["skillStudyCurr"+obj.skillStuding] >= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill")))							//显示主堂是否可以升级
//			{
//				obj["skillStudyCurr"+obj.skillStuding] 	= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill"));							//技能等级不能超过上限
//				obj["skillStudyNum"+obj.skillStuding] 	= 0;
//				levelBtnEnabled(mcUnityOther,otherlevelWork.showCondition());	
//			}
//			showBar();
//		}
//		/** 增加建设度繁荣度的进度 i=1为每分钟加的值 */
//		private function barBuiltGrow(i:int = 1):void
//		{
//			var obi:Object = UnityConstData.mainUnityDataObj;
//			UnityConstData.mainUnityDataObj.unityBuilt += i * int(UnityConstData.mainUnityDataObj.craftsmanNum);
//			UnityConstData.mainUnityDataObj.unitybooming += i*int(UnityConstData.mainUnityDataObj.businessmanNum);
//			showBar();
//		}
//		/** 得到计算后的新数据  根据时间差计算增长值，技能跟建设度的时间不一样，根据形参来判断*/
//		public function getNewDate(timeList:Array):void		//UnityConstData.lastTimeList_Skill
//		{
//			for(var i:int = 0;i < timeList.length; i++)
//			{
//				if(timeList[i] == null) continue;
//				if(_type == timeList[i].type)
//				{
//					var date:Date = new Date(GameCommonData.gameYear,GameCommonData.gameMonth,GameCommonData.gameDay,GameCommonData.gameHour,GameCommonData.gameMinute,GameCommonData.gameSecond);
//					var time:int = (date.getTime() - timeList[i].time) / (1000 * 60) ;			//间隔1分钟
//					
//					if(timeList == UnityConstData.lastTimeList_Skill) barSkillGrow(time);
//					else barBuiltGrow(time);
//					timeList[i].time = date.getTime();
//					date = null;
//				}
//			}
//		}/** 如果想一得到数据就全部开始计时的话，就得在得到所有数据后将lastTypeArray数组添满(目前是同时计时)*/
		
		/** 技能研究成功后的显示 */
		public function skillStudied(value:int):void 
		{
			var subIndex:int = value / 10;
			var studingIndex:int = value % 10;
			UnityConstData.otherUnityArray[subIndex].skillStuding = studingIndex;
			if(_type == subIndex && mcUnityOther)
			{
				for(var i:int = 0; i < 3; i++)
				{
					mcUnityOther["txtUp_" + i].textColor = 0xFFFFFF;
					mcUnityOther["mcSkillSelect_" + i].gotoAndStop(1);
				}
				mcUnityOther["txtUp_" + (studingIndex -1)].textColor = 0x00FF00;
				mcUnityOther["mcSkillSelect_" + int(obj.skillStuding - 1)].gotoAndStop(2);
			}
		}
		/** 金钱超出上限 */
		private function moneyOut():void
		{
			if(UnityConstData.mainUnityDataObj.unityMoney > int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mianMoneyTop")))
			{
				UnityConstData.mainUnityDataObj.unityMoney  = int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mianMoneyTop"));
				GameCommonData.UIFacadeIntance.sendNotification(HintEvents.RECEIVEINFO , {info:GameCommonData.wordDic[ "mod_uni_uni_uds_mon_1" ], color:0xffff00});  // 帮派金钱已达到上限
			}
		}
		/** 加载技能图标 */
		private function loadSkillIcon():void
		{
			for(var i:int = 0; i < obj.skillIconList.length ; i++)
			{
				var blrP:FaceItem = new FaceItem(obj.skillIconList[i]);
				blrP.mouseEnabled = true;
				blrP.name = "skill_" + obj.skillIconList[i];
				blrP.x = 252;
				blrP.y = i * 42 + 0.5;
				if(mcUnityOther) mcUnityOther.addChild(blrP);
				blrP = null;
			}
		}
		/** 删除技能图标 */
		private function deleteSkillIcon():void
		{
			for(var i:int = 1; i < UnityConstData.otherUnityArray.length ; i++)
			{
				for(var n:int = 0; n < UnityConstData.otherUnityArray[i].skillIconList.length ; n++)
				{
					if(mcUnityOther && mcUnityOther.getChildByName("skill_" + UnityConstData.otherUnityArray[i].skillIconList[n]) != null)
					{
						mcUnityOther.removeChild(mcUnityOther.getChildByName("skill_" + UnityConstData.otherUnityArray[i].skillIconList[n]));
					}
				}
			}
		}
		/** 检测分堂等级是否大于主堂等级(分堂等级不能大于主堂等级) */
		private function synLevelTest():void
		{
			for(var i:int = 1; i < UnityConstData.otherUnityArray.length; i++)
			{
				if(UnityConstData.otherUnityArray[i].level > UnityConstData.mainUnityDataObj.level)
				{
					UnityConstData.otherUnityArray[i].level = UnityConstData.mainUnityDataObj.level;
				}
			} 
		}
	}
}
