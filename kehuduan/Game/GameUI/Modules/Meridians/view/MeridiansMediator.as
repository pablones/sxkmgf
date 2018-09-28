package GameUI.Modules.Meridians.view
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.ChangeLine.Data.ChgLineData;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Meridians.Components.DisplayObjectGlow;
	import GameUI.Modules.Meridians.Components.MeridiansTimeOutComponent;
	import GameUI.Modules.Meridians.model.MeridiansData;
	import GameUI.Modules.Meridians.model.MeridiansEvent;
	import GameUI.Modules.Meridians.model.MeridiansTypeVO;
	import GameUI.Modules.Meridians.tools.Tools;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Modules.SmallWindow.Data.SmallWindowData;
	import GameUI.Modules.SmallWindow.Mediator.SmallWindowMediator;
	
	import OopsFramework.Content.Loading.BulkLoader;
	import OopsFramework.Content.Loading.ImageItem;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Loader;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.net.URLRequest;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MeridiansMediator extends Mediator
	{
		public static const NAME:String = "MeridiansMediator";
		
		public static const TYPE:int = 1;
		private var parentView:MovieClip = null;					//父容器
		
		private var view:MovieClip = null;							//经脉面板
		private var _meridians:MovieClip = new MovieClip();			//经脉主界面
		private var learn0:MovieClip;								//经脉初始
		private var learn:MovieClip;								//修炼控制面板
		private var buttons:Array = new Array();					//经脉按钮			0 ~ 7 经脉按钮 8 ~  经脉帮助  开始修炼 强化经脉 打开修炼队列
		private var grades:Array = new Array();						//经脉等级
		private var texts:Array = new Array();						//显示文本			0 ~ 从上到下依次数下来
		private var learnState:TextField ;							//显示修炼状态
		private var selectedMeridians:int = 1;						//默认选中第一个经脉
		private var loader:Loader=new Loader();							//加载经脉强化等级图片
		private	var r:URLRequest=new URLRequest();
		private var glows:Array = new Array();						//发光效果
		private var isFirst:Boolean = true;
		private var sexLoader:ImageItem;
		private var sexUrl:String;							//男女背景图片
		private var sexIsLoad:Boolean = false;

		public function MeridiansMediator(parent:MovieClip)
		{
			parentView = parent;
			super(NAME, viewComponent);
		}
		
		//经脉主界面
		public function get meridians():MovieClip		
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					EventList.INITVIEW,
					RoleEvents.INITROLEVIEW,								//初始化人物属性界面
			
					MeridiansEvent.INIT_MERIDIANS_MAIN,						//初始化本界面
					RoleEvents.SHOWPROPELEMENT,								//显示本界面
					MeridiansEvent.UPDATA_MERIDIANS_DATA,					//更新显示信息
					RoleEvents.MEDIATORGC,
					MeridiansEvent.RESULT_UPLEV_SUC,						//点击修炼完成按钮并且成功升级完成
					MeridiansEvent.RESULT_MOVEWAITQUEUE_SUC,				//点击队列面板中 移除队列成功
					MeridiansEvent.COMPLETE_MERIDIANS_UPGRADE,
					MeridiansEvent.RESULT_ADDWAITQUEUE_SUC,			
					MeridiansEvent.RESULT_ADDWAITQUEUE_FAIL,				//加入修炼队列失败
					MeridiansEvent.RESULT_STARTLEARN_SUC,					//点击 开始修炼 直接进入修炼状态
					MeridiansEvent.AUTO_MERIDIANS_LEARN,					//经脉自动开始修炼
					MeridiansEvent.RESULT_UPLEV_PART,						//使用真元加速
					MeridiansEvent.RESULT_STRENGTHJINMEI_SUC,
					MeridiansEvent.RESULT_STRENGTHJINMEI_FAIL,
					MeridiansEvent.UPDATA_STRENGTH_AND_MAIN,
					MeridiansEvent.COMPLETE_MERIDIANS_UPGRADE,
					ChgLineData.CHG_LINE_SUC,
				];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					if(GameCommonData.Player.Role.Level >= 13)
					{
//						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,140);		//向服务器发送查询经脉信息
					}
					break;
				case RoleEvents.INITROLEVIEW:
					initRegisters();
					MeridiansData.initDic();
					sendNotification(MeridiansEvent.INIT_MERIDIANS_MAIN);
					break;
				case MeridiansEvent.INIT_MERIDIANS_MAIN:
					MeridiansData.setTestData();							//设置测试数据
					if(GameCommonData.Player.Role.Level >= 13)
					{
						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,140);		//向服务器发送查询经脉信息
					}
					Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,286);		//向服务器发送查询真元信息

					initView();
					initViewData();
					break;
				case RoleEvents.SHOWPROPELEMENT:
					if(notification.getBody() as int != TYPE) {
						for(var i:int = 0; i < 8 ; ++i)
						{
							var glow:DisplayObjectGlow = glows[i - 1] as DisplayObjectGlow;
							if(glow)
							{
								glow.lightOff();
							}
						}
						return;
					}
					if( isFirst )
					{
						isFirst = false;
						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,140);		//向服务器发送查询经脉信息
					}
					if(!sexIsLoad)
					{
						sexLoad();
						sexIsLoad = true;
					}
					this.selectedMeridians = getNextLearn();
					makeSynchro();
					initViewData();
					parentView.addChildAt(meridians, 4);
					break;
				case MeridiansEvent.UPDATA_MERIDIANS_DATA:							//更新显示数据
					if(this.view!=null)
					{
						initViewData();
					}
					break;
				case RoleEvents.MEDIATORGC:	
					gc();
					break;
				case MeridiansEvent.RESULT_UPLEV_SUC:
					this.selectedMeridians = getNextLearn();
					initViewData();
					break;
				case MeridiansEvent.RESULT_MOVEWAITQUEUE_SUC:
					if(notification.getBody() != null)
					{
						showMeridiansView(notification.getBody() as int);
					}
					break;
				case MeridiansEvent.COMPLETE_MERIDIANS_UPGRADE:
					showMeridiansView(getNextLearn());
					break;
				case MeridiansEvent.RESULT_ADDWAITQUEUE_SUC:
					makeSynchro();
					showMeridiansView(getNextLearn());
					break;
				case MeridiansEvent.RESULT_ADDWAITQUEUE_FAIL:
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_hand_1"]/*"加入修炼队列失败"*/ , color:0xffff00});
					break;
				case MeridiansEvent.RESULT_STARTLEARN_SUC:
					makeSynchro();
					showMeridiansView(getNextLearn());
					break;
				case MeridiansEvent.AUTO_MERIDIANS_LEARN:
					showMeridiansView(notification.getBody() as int);
					makeSynchro();
					break;
				case MeridiansEvent.RESULT_UPLEV_PART:
					showMeridiansView(notification.getBody() as int);
					break;
				case MeridiansEvent.RESULT_STRENGTHJINMEI_SUC:
					selectedMeridians = notification.getBody() as int;
					initViewData();
					break;
				case MeridiansEvent.RESULT_STRENGTHJINMEI_FAIL:
					selectedMeridians = notification.getBody() as int;
					initViewData();
					break;	
				case MeridiansEvent.UPDATA_STRENGTH_AND_MAIN:
					showMeridiansView(notification.getBody() as int);
					break;
				case ChgLineData.CHG_LINE_SUC:
					Tools.showMeridiansNet(GameCommonData.Player.Role.Id,0,0,140);
					break;
			}
		}
	
		private function initRegisters():void
		{
			/** 弹框 **/
			facade.registerMediator( new SmallWindowMediator() );
			facade.sendNotification( SmallWindowData.INIT_SMALLWINDOW );
			/*****/
			facade.registerMediator(new MeridiansStrengMediator());
			facade.registerMediator(new MeridiansLearnListMediator());
//			facade.registerMediator(new MeridiansLearnPromptMediator());
			facade.registerMediator(new AddzhenYuanMediator());
		}
		
		private function initView():void
		{
			if(GameCommonData.Player.Role.Sex == 1)
			{
				this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Female");				//获取女性经脉修炼界面
				sexUrl = GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/FemaleJingMai.swf";
			}
			else
			{
				this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Male");				//获取男性经脉修炼界面
				sexUrl = GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/maleJingMai.swf";
			}
			this._meridians.addChild(this.view);
			
			//获取经脉图上的3*8个组件
			for(var i:int = 1; i <= 8; ++i)
			{
				var mc:MovieClip = this.view.mc_meridians["btn_meridians_" + i];
				mc.gotoAndStop(1);
				mc.mcSelect.visible = false;
				mc.mcSelect.mouseEnabled = false;
				
				var glow_mc:MovieClip = mc.btn_touM;
				
				var child:Shape = new Shape();
	            child.graphics.beginFill(0xffffff);
//	            child.graphics.lineStyle(borderSize, borderColor);
	            child.graphics.drawCircle(0, 0, 11);
	            child.graphics.endFill();
	            child.x = 11;
				child.y = 11;
				var glow:DisplayObjectGlow = new DisplayObjectGlow( child );
//				var glow:DisplayObjectGlow = new DisplayObjectGlow( mc );
	            mc.addChildAt(child,0);
				glows.push(glow);
				
				mc.buttonMode = true;
				this.buttons.push(mc);
				mc.addEventListener(MouseEvent.CLICK,onMeridiansClick);
				this.grades.push(this.view.mc_meridians["txt_grade_" + i]);
				(this.view.mc_meridians["txt_grade_" + i] as TextField).mouseEnabled = false;
				mc.mcSelect.mouseEnabled = false;
				mc.btn_touM.mouseEnabled = false;
			}
			

			this.learn0 = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Meridians_learn0");		//获取经脉修炼面板
			this.learn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("Meridians_learn");		//获取经脉修炼面板
			//获取经脉强化等级图片
			loader.x = 244;
			loader.y = 242;
			this.learn.addChild(loader);
			
			//获取 经脉帮助  开始修炼 强化经脉 打开修炼队列 等按钮并注册监听器
			this.buttons.push(this.learn.btn_startLearn);
			this.buttons.push(this.learn.btn_strength);
			this.buttons.push(this.learn.btn_openLearnList);
			
			this.learn0.btn_instantStart.addEventListener(MouseEvent.CLICK,onBtnClick);
			this.learn.btn_startLearn.addEventListener(MouseEvent.CLICK,onBtnClick);
			this.learn.btn_strength.addEventListener(MouseEvent.CLICK,onBtnClick);
			this.learn.btn_openLearnList.addEventListener(MouseEvent.CLICK,onBtnClick);
			//获取文本数组
			for(i = 1; i <= 20 ; ++i)
			{
				this.texts.push(this.learn["txt_"+i])
				this.learn["txt_"+i].mouseEnabled = false;
			}
			this.learnState = this.learn.txt_learnState;
			this.learnState.mouseEnabled = false;
			this.learn.txt_strength.mouseEnabled = false;
				
			sendNotification(MeridiansEvent.INIT_MERIDIANS_STRENG);
			sendNotification(MeridiansEvent.INIT_MERIDIANS_LEARN_LIST);
//			sendNotification(MeridiansEvent.INIT_MERIDIANS_LEARN_PROMPT);
			sendNotification(MeridiansEvent.INIT_ADDZHENYUAN);
			
			this.buttons.push(this.learn.btn_meridianshelp);
			this.learn.btn_meridianshelp.addEventListener(MouseEvent.CLICK,onBtnClick);
			this._meridians.addChild(this.learn0);
			this._meridians.addChild(this.learn);
			setViewComponent( _meridians );
			
		}
		
		private function sexLoad():void
		{
			this.sexLoader = new ImageItem( sexUrl,BulkLoader.TYPE_IMAGE,"" );
			sexLoader.addEventListener( Event.COMPLETE,sexLoadCom );
//			sexLoader.load( new URLRequest( sexUrl ) );
			sexLoader.load();
		}
		
		//男女资源加载完成
		private function sexLoadCom( evt:Event ):void
		{
			sexLoader.removeEventListener( Event.COMPLETE,sexLoadCom );
			
			var sexBmp:Bitmap = new Bitmap();
			var bgClass:Class;
			if ( GameCommonData.Player.Role.Sex == 1 )
			{

				bgClass = sexLoader.GetDefinitionByName( "JingMaiWoman" );
			}
			else
			{
				bgClass = sexLoader.GetDefinitionByName( "JingMaiMan" );
			} 
			
			if ( bgClass )
			{
				sexBmp.bitmapData = new bgClass(192,365) as BitmapData;
			}
			sexBmp.x = 0;
			sexBmp.y = 0;
			this.view.addChildAt( sexBmp,0 );
			
		}
		
		private function initViewData():void
		{
			if(GameCommonData.Player.Role.Level >= 13)
			{
				this.learn0.visible = false;
				this.view.visible = true;
				this.learn.visible = true;
			}
			else
			{
				this.view.visible = false;
				this.learn0.visible = true;
				this.learn.visible = false;
			}
			
			var meridiansTypeVO:MeridiansTypeVO;
			//设置经脉按钮上的经脉等级
			for(var i:int = 0; i < 8; ++i)
			{
				meridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[i] as MeridiansTypeVO;
				if(meridiansTypeVO.nLev % 30 != 0 || meridiansTypeVO.nLev == 0)
				{
					(this.grades[i] as TextField).text = ""+ ( meridiansTypeVO.nLev % 30 );
				}
				else
				{
					(this.grades[i] as TextField).text = ""+ 30;
				}
				
				setMeridiansState( i+1 ,1);
			}
			
				//显示经脉境界信息
				var nAllLevGrade:int = MeridiansData.getLowest(MeridiansData.meridiansVO.nAllLevGrade);	//获取当前经脉境界的最低等级
				(this.texts[0] as TextField).text = MeridiansData.allMeridiansGradeDic[nAllLevGrade].condition;
				(this.texts[1] as TextField).text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_1"]/*"技能消耗气减少"*/;

				GameCommonData.Player.Role.MPPercentage = 100 - MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect2;

				(this.texts[2] as TextField).text = ""+MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect2+"%";
				(this.texts[3] as TextField).text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_2"]/*"受到伤害减免"*/;
				(this.texts[4] as TextField).text = MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect3+"%";
				(this.texts[5] as TextField).text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_3"]/*"造成伤害增加"*/;
				(this.texts[6] as TextField).text = MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect4+"%";
				if(0 == MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect2)
				{
					(this.texts[1] as TextField).htmlText = GameCommonData.wordDic["gameui_med_uil_getT_alllMeridiansGrade_4"]/*全部经脉修炼到*/ + "<font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_4"]/*一层*/ + "</font>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_5"]/*"开启"*/ ;
					(this.texts[2] as TextField).visible = false;
				}
				else
				{
					
					(this.texts[2] as TextField).visible = true;
				}
				if(0 == MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect3 )
				{
					(this.texts[3] as TextField).visible = false;
					(this.texts[4] as TextField).visible = false;
				}
				else
				{
					(this.texts[3] as TextField).visible = true;
					(this.texts[4] as TextField).visible = true;
					
				}
				if( 0 == MeridiansData.allMeridiansGradeDic[nAllLevGrade].effect4)
				{
					(this.texts[5] as TextField).visible = false;
					(this.texts[6] as TextField).visible = false;
				}
				else
				{
					(this.texts[5] as TextField).visible = true;
					(this.texts[6] as TextField).visible = true;
				}
				(this.texts[7] as TextField).visible = false;
				(this.texts[8] as TextField).visible = false;
				
				//显示灵根信息
				var isShow:Boolean = false;			//true 表示有经脉强化过
				for(var ii:int = 0; ii < 8; ++ii)
				{
					if((MeridiansData.meridiansVO.meridiansArray[ii] as MeridiansTypeVO).nStrengthLev > 0)
					{
						isShow = true;
					}
				}
				
				//经脉有强化过
				if(MeridiansData.meridiansVO.nAllLevGrade > 0 && isShow)
				{
					var nAllStrengthLevAdd:int = MeridiansData.meridiansVO.nAllStrengthLevAdd;
					if( nAllStrengthLevAdd > 0)
					{
						(this.texts[9] as TextField).text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_6"] /*灵根*/ +"："+MeridiansData.allMeridiansStrengthDic[nAllStrengthLevAdd].mingcheng+"("+nAllStrengthLevAdd+ GameCommonData.wordDic[ "often_used_level" ]/*级*/ +")";
						(this.texts[10] as TextField).htmlText = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_7"]/*"所有经脉效果"*/;
						(this.texts[11] as TextField).text = ""+MeridiansData.allMeridiansStrengthDic[nAllStrengthLevAdd].effect1+"%";
						(this.texts[11] as TextField).visible = true;
					}
					else
					{
						(this.texts[9] as TextField).text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_8"]/*"灵根：无"*/;
						(this.texts[10] as TextField).htmlText = "<font color='#00FF00'>"+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_init_9"]/*全部经脉强化到+1开启*/ + "<font>";
						(this.texts[11] as TextField).visible = false;
					}
					(this.texts[9] as TextField).visible = true;
					(this.texts[10] as TextField).visible = true;
				}
				else
				{
					(this.texts[9] as TextField).visible = false;
					(this.texts[10] as TextField).visible = false;
					(this.texts[11] as TextField).visible = false;
				}
				
			showMeridiansView(selectedMeridians);
			
		}
		
		private function showMeridiansView(n:int):void
		{
			this.selectedMeridians = n;
			showData(n);					//显示第n个经脉的信息
			setButtonsFilters(n);				//显示选中文本特效
			updataTime_Meridians();					//更新倒计时
		}
		
		private function showData(n:int):void
		{

				//显示某经脉信息
				var meridiansTypeVO:MeridiansTypeVO;
				meridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[n - 1] as MeridiansTypeVO;
				if(meridiansTypeVO.nLev > 0)
				{
					var name:String = "";
					if(meridiansTypeVO.nLev > 60)
					{
						if(meridiansTypeVO.nLev == 90)
						{
							name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_1"]/*"无上·"*/ + MeridiansData.meridiansNames[meridiansTypeVO.nType]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_1"]/*"三十层"*/;
						}
						else
						{
							name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_1"]/*"无上·"*/ + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[meridiansTypeVO.nLev % 30]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_2"]/*"层"*/;
						}
					}
					else if(meridiansTypeVO.nLev > 30)
					{
						if(meridiansTypeVO.nLev == 60)
						{
							name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_3"]/*"真·"*/ + MeridiansData.meridiansNames[meridiansTypeVO.nType]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_1"] /*"三十层"*/;
						}
						else
						{
							name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_3"]/*"真·"*/ + MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[meridiansTypeVO.nLev % 30]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_2"]/*"层"*/;
						}
					}
					else
					{
						if(meridiansTypeVO.nLev == 30)
						{
							name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_1"]/*"三十层"*/;
						}
						else
						{
							name = MeridiansData.meridiansNames[meridiansTypeVO.nType]+MeridiansData.numbers[meridiansTypeVO.nLev % 30]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_2"]/*"层"*/;
						}
					}
					(this.texts[12] as TextField).text = name;
					var addRate:String = "";
					if(MeridiansData.meridiansGradeDic[meridiansTypeVO.nStrengthLev].addition > 0)
					{
						addRate = "<font color='#00FF00'>(+"+MeridiansData.meridiansGradeDic[meridiansTypeVO.nStrengthLev].addition+"%)</font>";
					}
					var StrengEffect:int = 0;
					if(MeridiansData.meridiansVO.nAllStrengthLevAdd > 0)
					{
						StrengEffect = MeridiansData.allMeridiansStrengthDic[MeridiansData.meridiansVO.nAllStrengthLevAdd].effect1;
					}
					
					(this.texts[14] as TextField).text = MeridiansData.meridianEffectDic[meridiansTypeVO.nType].effect+"：";
					
					var lastLev:int = MeridiansData.meridiansUpgradeCondition[0][0];
					var id:int = (meridiansTypeVO.nType - 1 ) * lastLev + meridiansTypeVO.nLev;
					var meridiansValue:int = MeridiansData.meridiansUpgradeCondition[ id ][3];
					var nextMeridiansValue:int;
					if( meridiansTypeVO.nLev < lastLev)
					{
						nextMeridiansValue = MeridiansData.meridiansUpgradeCondition[ id+1 ][3];
					}
					else
					{
						nextMeridiansValue = MeridiansData.meridiansUpgradeCondition[ id ][3];
					}
					
					var currValue:int = getMeridiansValue(meridiansValue,meridiansTypeVO.nStrengthLev);
					
					var nextValue:int = getMeridiansValue(nextMeridiansValue,meridiansTypeVO.nStrengthLev);
				
					(this.texts[15] as TextField).text = "+" + currValue;
					(this.texts[16] as TextField).text = "+" + nextValue;
					(this.texts[17] as TextField).text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_2"]/*"下一级："*/;
					(this.texts[13] as TextField).htmlText = "";
				}
				else
				{
					(this.texts[12] as TextField).text = MeridiansData.meridiansNames[meridiansTypeVO.nType];
//					(this.texts[13] as TextField).htmlText = "增加"+ meridianEffectDic[meridiansTypeVO.nType].effect;
					(this.texts[13] as TextField).htmlText = MeridiansData.MeridiansExplain[meridiansTypeVO.nType];
					(this.texts[14] as TextField).text = "";
					(this.texts[15] as TextField).text = "";
					(this.texts[16] as TextField).text = "";
					(this.texts[17] as TextField).text = "";
				}

				
				(this.texts[18] as TextField).text = MeridiansData.meridiansNames[meridiansTypeVO.nType] + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_3"]/*"效果"*/;
				if(meridiansTypeVO.nStrengthLev < 4)
				{
					(this.texts[19] as TextField).htmlText = "<font color='#00ff00'>"+MeridiansData.meridiansGradeDic[meridiansTypeVO.nStrengthLev].addition+"%</font>";
				}
				else if(meridiansTypeVO.nStrengthLev < 8)
				{
					(this.texts[19] as TextField).htmlText = "<font color='#00cbff'>"+MeridiansData.meridiansGradeDic[meridiansTypeVO.nStrengthLev].addition+"%</font>";
				}
				else
				{
					(this.texts[19] as TextField).htmlText = "<font color='#ff0000'>"+MeridiansData.meridiansGradeDic[meridiansTypeVO.nStrengthLev].addition+"%</font>";
				}
	
				switch(meridiansTypeVO.nState)
				{
				//	0：未修炼或未等待修炼 ，1：等待修炼 ， 2：正在修炼 ，3：修炼完成
	
					case 0:
						if(meridiansTypeVO.nLev != 90)
						{
							(this.learn.btn_startLearn as SimpleButton).visible = true;
							this.learnState.htmlText = "<font color='#FFFE65'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_4"]/*开始修炼*/ + "</font>";
						}else
						{
							(this.learn.btn_startLearn as SimpleButton).visible = false;
							this.learnState.htmlText = "";
						}
						break;
					case 1:
						(this.learn.btn_startLearn as SimpleButton).visible = false;
						this.learnState.htmlText = "<font color='#00FFFF'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_15"]/*将耗时*/ + " "+ Tools.getTime(meridiansTypeVO.nLeaveTime) +"</font>";
						break;
					case 2:
						(this.learn.btn_startLearn as SimpleButton).visible = false;
						this.learnState.htmlText = "<font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_16"]/*还剩*/ + " "+Tools.getTime(meridiansTypeVO.nLeaveTime)+"</font>";
						break;
					case 3:
						(this.learn.btn_startLearn as SimpleButton).visible = true;
						this.learnState.htmlText = "<font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_18"]/*完成*/ + "</font>";
						break;
				}
				
				//加载强化等级图片
				loader.unload();
				r.url=GameCommonData.GameInstance.Content.RootDirectory+"Resources/NumberIcon/"+meridiansTypeVO.nStrengthLev+".png";
				loader.load(r);	
				
				if(MeridiansData.meridiansVO.nAllLevGrade > 0)
				{
					this.learn.btn_strength.visible = true;
					this.learn.txt_strength.visible = true;
				}
				else
				{
					this.learn.txt_strength.visible = false;
					this.learn.btn_strength.visible = false;
				}
				
				if(meridiansTypeVO.nStrengthLev > 0 && MeridiansData.meridiansVO.nAllLevGrade > 0)
				{
					loader.visible = true;					
				}
				else
				{
					(this.texts[18] as TextField).text = "";
					(this.texts[19] as TextField).text = "";
					loader.visible = false;
				}
				
				if(meridiansTypeVO.nStrengthLev == 0 && MeridiansData.meridiansVO.nAllLevGrade > 0 )
				{
					this.learn.txt_strength0.text = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_5"]/*"经脉强化可提高对应经脉的效果，最高到10级"*/;
				}
				else
				{
					this.learn.txt_strength0.text = "";
				}
		}
		
		//设置经脉按钮状态
		private function setMeridiansState(i1:int, i2:int):void
		{
			var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[ i1-1 ] as MeridiansTypeVO;
			var grade:int = meridiansTypeVO.nStrengthLev;
			var vi:int = 0;
			if(grade < 4)
			{
				vi = 0;
			}
			else if(grade < 7)
			{
				vi = 2;
			}			
			else if(grade < 9)
			{
				vi = 4;
			}
			else
			{
				vi = 6;
			}
			
			(this.buttons[i1 - 1] as MovieClip).gotoAndStop(vi + i2);
			if(i2 == 2)
			{
				(this.buttons[i1 - 1] as MovieClip).mcSelect.visible = true;
			}
			else
			{
				(this.buttons[i1 - 1] as MovieClip).mcSelect.visible = false;
			}
			var glow:DisplayObjectGlow = glows[i1 - 1] as DisplayObjectGlow;
			if( 2 == meridiansTypeVO.nState)
			{
				if(glow.color != 0xffff00)
				{
					glow.lightOff();
					glow.color = 0xffff00;
				}
				glow.lightOn();
			}
			else if(1 == meridiansTypeVO.nState)
			{
				if(glow.color != 0x00CBFF)
				{
					glow.lightOff();
					glow.color = 0x00CBFF;
				}
				glow.lightOn();
			}else
			{
				glow.lightOff();
			}
		}
		
		//使按钮发光同步
		private function makeSynchro():void
		{
			for(var i:int = 0; i < glows.length ; i++)
			{
				var glow:DisplayObjectGlow = glows[i] as DisplayObjectGlow;
				glow.blur = 0;
			}
		}
		
		//选中经脉的按钮显示效果
		private function setButtonsFilters(n:int):void
		{
			for(var i:int = 0; i < 8; ++i)
			{
				setMeridiansState(i + 1,1);
			}
			setMeridiansState(n,2);
		}

		/** 处理经脉按钮事件 */		
		private function onMeridiansClick(event:MouseEvent):void
		{
			switch(event.currentTarget.name)
			{
				case "btn_meridians_1":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,1);
				break;
				case "btn_meridians_2":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,2);
				break;
				case "btn_meridians_3":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,3);
				break;
				case "btn_meridians_4":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,4);
				break;
				case "btn_meridians_5":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,5);
				break;
				case "btn_meridians_6":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,6);
				break;
				case "btn_meridians_7":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,7);				
				break;
				case "btn_meridians_8":
					sendNotification(MeridiansEvent.UPDATA_STRENGTH_AND_MAIN ,8);
				break;
				
			}
		}

		private function onBtnClick(event:MouseEvent):void
		{
			switch(event.target.name)
			{
				case "btn_meridianshelp":
//					sendNotification(EventList.SHOW_MERIDIANS_HELP);
						this.learn0.visible = true;
						this.learn.visible = false;
						this.view.visible = false;
					break;
					
				case "btn_strength":
					sendNotification(MeridiansEvent.SHOW_MERIDIANS_STRENG,this.selectedMeridians);

					break;
				case "btn_openLearnList":
					sendNotification(MeridiansEvent.SHOW_MERIDIANS_LEARN_LIST);
					
					break;
				case "btn_startLearn":
					var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[this.selectedMeridians - 1];
					if(meridiansTypeVO.nState == 0)
					{
						var canLearn:Boolean = true;
						/** 经脉最高等级 */
						var lastLev:int = MeridiansData.meridiansUpgradeCondition[0][0];
						/** 经脉最高等级 */
						var id:int = (selectedMeridians - 1) * lastLev + meridiansTypeVO.nLev + 1;			
						var xuyaodengji:int = MeridiansData.meridiansUpgradeCondition[ id ][0]; //需要人物等级
						var reqmoney:int = MeridiansData.meridiansUpgradeCondition[ id ][1];	//需要金钱
						var xuyaosuoyou:int = MeridiansData.meridiansUpgradeCondition[ id ][2];  // 需要所有经脉等级
						
						var currMoney:int = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
					
						var meridiansLearnListMediator:MeridiansLearnListMediator = facade.retrieveMediator(MeridiansLearnListMediator.NAME) as MeridiansLearnListMediator;
						var isFull:Boolean = meridiansLearnListMediator.isFull();
						
						if(currMoney >= reqmoney || GameCommonData.Player.Role.BindMoney >= reqmoney || GameCommonData.Player.Role.UnBindMoney >= reqmoney)
						{
							
						}
						else
						{
							//发出缺钱的消息
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_onBtnC_1"]/*"金钱不足，无法修炼"*/, color:0xffff00});
							canLearn = false;
							break;
						}
						if(GameCommonData.Player.Role.Level < xuyaodengji || MeridiansData.meridiansVO.nAllLevGrade < xuyaosuoyou)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_onBtnC_2"]/*"你不满足修炼条件"*/, color:0xffff00});
							canLearn = false;
							break;
						}
						
						if(isFull)
						{
							facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_onBtnC_3"]/*"修炼队列已满"*/, color:0xffff00});
							canLearn = false;
							break;
						}
						
						if(canLearn)
						{
							Tools.showMeridiansNet(GameCommonData.Player.Role.Id,selectedMeridians,0,143);
//							sendNotification(MeridiansEvent.CLOSE_MERIDIANS_LEARN_PROMPT);
						}
					/*************/
						
						
					}	
					else if(meridiansTypeVO.nState == 3)
					{
						Tools.showMeridiansNet(GameCommonData.Player.Role.Id,selectedMeridians,0,141);		//向服务器发送经脉升级
					}
					break;
				case "btn_instantStart":
					if(GameCommonData.Player.Role.Level >= 13)
					{
						this.learn.visible = true;
						this.view.visible = true;
						this.learn0.visible = false;
					}
					else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_onBtnC_4"]/*"满13级便可修炼经脉"*/, color:0xffff00});
					}
					break;
			}
		}
		
		public function updataTime_Meridians():void
		{
			var meridiansTypeVO:MeridiansTypeVO = MeridiansData.meridiansVO.meridiansArray[this.selectedMeridians - 1] as MeridiansTypeVO;
			if( 2 == meridiansTypeVO.nState )
			{
				this.learnState.htmlText = "<font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_16"]/*还剩*/ + " "+Tools.getTime(meridiansTypeVO.nLeaveTime)+"</font>";
				MeridiansTimeOutComponent.getInstance().addFun2("updataTime_Meridians",updataTime_Meridians);
			}
			else
			{
				MeridiansTimeOutComponent.getInstance().removeFun2("updataTime_Meridians");
			}
		}
		
		/** 获取加成之后的经脉属性值
		 * @param lev	经脉等级
		 * @param linggenLev	灵根等级
		 * @return  加成之后的经脉属性值
		 */		
		private function getMeridiansValue( initValue:int, strengthLev:int):int
		{
			var strengthAdd:int = MeridiansData.meridiansGradeDic[strengthLev].addition;//强化加成
			var linggenLev:int = MeridiansData.meridiansVO.nAllStrengthLevAdd;
			var linggenAdd:int = 0;
			if(MeridiansData.meridiansVO.nAllStrengthLevAdd > 0)
			{
				linggenAdd = MeridiansData.allMeridiansStrengthDic[MeridiansData.meridiansVO.nAllStrengthLevAdd].effect1;//灵根加成
			}	
			
			var Value:int = initValue + initValue * strengthAdd / 100 + initValue * linggenAdd/ 100;
			return Value;
		}
		
		private function getNextLearn():int
		{
			var meridiansTypeVO:MeridiansTypeVO ;
			
			var lastLev:int = MeridiansData.meridiansUpgradeCondition[0][0];

			
			
			for(var i:int=0 ;i <8; i++)
			{
				
				meridiansTypeVO= MeridiansData.meridiansVO.meridiansArray[i];
				
				var id:int = i * lastLev + meridiansTypeVO.nLev + 1;			
				var xuyaodengji:int = MeridiansData.meridiansUpgradeCondition[ id ][0]; //需要人物等级
				var xuyaosuoyou:int = MeridiansData.meridiansUpgradeCondition[ id ][2];  // 需要所有经脉等级
				
				if(GameCommonData.Player.Role.Level >= xuyaodengji && MeridiansData.meridiansVO.nAllLevGrade >= xuyaosuoyou && 0 == meridiansTypeVO.nState)
				{
					return i+1;
				}

			}
			return 1;
		}
		
//		private function deleteView():void
//		{
//			
//			while(glows.pop());
//			while(buttons.pop());
//			while(grades.pop());
//			
//			this.learn.btn_meridianshelp.removeEventListener(MouseEvent.CLICK,onBtnClick);
//			
//			this._meridians.removeChild(this.learn);
//			
//			this._meridians.removeChild(this.view);
//			
//			this.learn = null;
//			
//			this.view = null;
//			
//		}
		
		/**
		 * @param meridiansId 1 到 8
		 */		
		public function getNextMSG( meridiansId:int ):String
		{
			/** 选中经脉id */
			var nType:int = meridiansId;
			/** 选中经脉的名称 */
			var meridiansName:String = MeridiansData.meridiansNames[nType];	
			/** 选中经脉等级 */
			var nLev:int = ( MeridiansData.meridiansVO.meridiansArray[ nType -1] as MeridiansTypeVO ).nLev;	
			/** 选中经脉强化等级 */
			var nStrengthLev:int = ( MeridiansData.meridiansVO.meridiansArray[ nType -1] as MeridiansTypeVO ).nStrengthLev;	
			/** 经脉效果 如： 生 命     暴 击 等*/
			var effect:String = MeridiansData.meridianEffectDic[meridiansId].effect;
			/** 经脉最高等级 */
			var lastLev:int = MeridiansData.meridiansUpgradeCondition[0][0];
			
			if(  nLev >= lastLev )
			{
				return GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_1"]/*"已满级"*/;
			}
			
			/** 下一级数据的行号 */
			var id:int = (nType - 1 ) * lastLev + nLev + 1 ;
			/** 下一级需求等级 */
			var nextReqLev:int = MeridiansData.meridiansUpgradeCondition[ id ][0];
			/** 下一级需要钱 */
			var nextReqMoney:int = MeridiansData.meridiansUpgradeCondition[ id ][1];
			/** 下一级需要所有等级 */
			var nextReqAllLev:int = MeridiansData.meridiansUpgradeCondition[ id ][2];
			/** 下一级经脉效果值 +100 */
			var nextValue:int = MeridiansData.meridiansUpgradeCondition[ id ][3];
			/** 下一级需要花费时间 */
			var nextNeedTime:int = MeridiansData.meridiansUpgradeCondition[ id ][4];
			/** 下一级可获得经验  */
			var nextExp:int = MeridiansData.meridiansUpgradeCondition[ id ][5];
			
			var str:String = "";
			var addStr:String = "";
			
			addStr = "         <font color='#FFCC00' size='14' >" + meridiansName + "</font>         \n"+
								"</font><font color='#00FFFF'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_2"]/*提升后效果*/ + "</font><font size='24'> </font>\n";
			str += addStr;
			addStr = "<font size='14'> </font><font color='#E2CCA5'>" + effect + " +" + getMeridiansValue(nextValue,nStrengthLev) + "</font>\n";
			str += addStr;
			addStr =			"<font size='14'> </font><font color='#E2CCA5'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_3"]/*"可获得"*/ + nextExp + GameCommonData.wordDic[ "often_used_exp" ]/*经验*/ + "</font>\n\n";
			str += addStr;
			addStr =			"<font color='#00FFFF'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_4"]/*修炼时间*/ + "</font>\n"+
						"<font size='14'> </font><font color='#E2CCA5'>" + Tools.getTime( nextNeedTime ) + "</font>\n\n";
			str += addStr;
			addStr =					"<font color='#00FFFF'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_5"]/*修炼需求*/ + "</font>\n";
			str += addStr;
			if(GameCommonData.Player.Role.Level < nextReqLev)
			{
				addStr =					"<font size='14'> </font><font color='#FF0000'>" + GameCommonData.wordDic[ "mod_her_med_skillLie_setT" ]/*"人物等级"*/ + nextReqLev + GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ] /*级*/+"</font>\n";
			}
			else
			{
				addStr =					"<font size='14'> </font><font color='#00FF00'>" + GameCommonData.wordDic[ "mod_her_med_skillLie_setT" ]/*"人物等级"*/  + nextReqLev + GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ] /*级*/+"</font>\n";
			}
			str += addStr;    
			if( nextReqAllLev > 0 )
			{
				if( MeridiansData.meridiansVO.nAllLevGrade < nextReqAllLev )
				{
					addStr =					"<font size='14'> </font><font color='#FF0000'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_6"]/*"所有经脉"*/  + getAllLevName( nextReqAllLev ) +"</font>\n";
				}
				else
				{
					addStr =					"<font size='14'> </font><font color='#00FF00'>" + GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getN_6"]/*"所有经脉"*/ + getAllLevName( nextReqAllLev ) +"</font>\n";
				}
	        	str += addStr;
	  		}
	  		var jin:int = int(nextReqMoney / 10000);
	  		var yin:int = int( (nextReqMoney % 10000) / 100);
	  		var tong:int = int( nextReqMoney % 100);
	  		var jinStr:String = "";
	  		var yinStr:String = "";
	  		var tongStr:String = "";
	  		
	  		var jinPath:String = "";
	  		var yinPath:String = "";
	  		var tongPath:String = "";
	  		
	  		var jinx:int = 0;
	  		var yinx:int = 0;
	  		var tongx:int = 0;
	  		var currx:int = 9;
	  		var col:String = "FF0000";
	  		if( GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney < nextReqMoney )
	  		{
				col = "FF0000";
     		}
     		else
     		{
     			col = "00FF00";
     		}
     		jinx = currx ;
     		if( jin > 0)
     		{
     			jinStr = "" + jin;
     			jinx += jin.toString().length * 6;
     		}
     		if( jinx > currx ) currx = jinx;
     		yinx = currx;
     		if( yin > 0)
     		{
     			if( jin > 0)
     			{
     				yinStr = "   ";
     				yinx += 18;
     			}
     			yinStr += yin;
     			yinx += yin.toString().length * 6;
     		}
     		if( yinx > currx ) currx = yinx;
     		tongx = currx;
     		if( tong > 0)
     		{
     			if( yin > 0 )
     			{
     				tongStr = "   ";
     				tongx += 18;
     			}
     			tongStr += tong;
     			tongx += tong.toString().length * 6;
     		}
     		if( tongx > currx ) currx = tongx;
     		
   			addStr =					"<font size='14'> </font><font color='#" + col + "'>" + jinStr +  yinStr + tongStr + "</font>"; 
     		str += addStr;
          /** 9+6*4 */ 
          	if( jin > 0)
          	{
          		addStr = "<img hspace='" + jinx + "' vspace='-19'  width='15' height='15' src='" + GameCommonData.GameInstance.Content.RootDirectory + "Resources/NumberIcon/money3.png' />";
          		str += addStr;
          	}
         /** 9+6*4+ 18 +6*2 */	
         	if( yin > 0)
         	{
         		addStr = "<img hspace='" + yinx + "' vspace='-19'  width='15' height='15' src='" + GameCommonData.GameInstance.Content.RootDirectory + "Resources/NumberIcon/money1.png' />";
         		str += addStr;
         	}
         	if( tong > 0)
         	{
         		addStr = "<img hspace='" + tongx + "' vspace='-19'  width='15' height='15' src='" + GameCommonData.GameInstance.Content.RootDirectory + "Resources/NumberIcon/money2.png' />";
         		str += addStr;
         	}
			return str;
			
		}
		
		private function getAllLevName( nLev:int ):String
		{
			var name:String ="";
				if(nLev > 60)
				{
//					name += "无上·" +MeridiansData.numbers[nLev % 30]+"层";
					if(nLev == 90)
					{
						name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getA_1"]/*"无上·三十层"*/;
					}else
					{
						name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_1"]/*"无上·"*/ +MeridiansData.numbers[nLev % 30] + GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_2"]/*"层"*/;
					}
				}
				else if(nLev > 30)
				{
//					name += "真·" +MeridiansData.numbers[nLev % 30]+"层";
					if(nLev == 60)
					{
						name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_getA_2"]/*"真·三十层"*/;
					}
					else
					{
						name += GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_3"]/*"真·"*/ +MeridiansData.numbers[nLev % 30]+ GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_2"]/*"层"*/;
					}
				}
				else
				{
//					name = MeridiansData.numbers[nLev % 30]+"层";
					if(nLev == 30)
					{
						name = GameCommonData.wordDic["Mod_Mer_mod_MeridiansMediator_showD_1"]/*"三十层"*/;
					}
					else
					{
						name = MeridiansData.numbers[nLev % 30]+GameCommonData.wordDic["Mod_Mer_mod_MeridiansLearnListMediator_hand_2"]/*"层"*/;
					}
				}
			return name;
		}
		
		private function gc():void
		{
			for(var i:int = 0; i < 8 ; ++i)
			{
				var glow:DisplayObjectGlow = glows[i - 1] as DisplayObjectGlow;
				if(glow)
				{
					glow.lightOff();
				}
			}
//			for(var i:int = 1; i <= 8; ++i)
//			{
//				var mc:MovieClip = this.view.mc_meridians["btn_meridians_" + i];
//				mc.removeEventListener(MouseEvent.CLICK,onMeridiansClick);
//			}
//			this.learn.btn_meridianshelp.removeEventListener(MouseEvent.CLICK,onBtnClick);
//			this.learn.btn_startLearn.removeEventListener(MouseEvent.CLICK,onBtnClick);
//			this.learn.btn_strength.removeEventListener(MouseEvent.CLICK,onBtnClick);
//			this.learn.btn_openLearnList.removeEventListener(MouseEvent.CLICK,onBtnClick);
		}
	}
}