package GameUI.Modules.HeroSkill.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.HeroSkill.Command.LearnSkillCommand;
	import GameUI.Modules.HeroSkill.Command.MoveSkillIconCommand;
	import GameUI.Modules.HeroSkill.Command.ReceiveSkillCommand;
	import GameUI.Modules.HeroSkill.Command.SkillEvent;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.HeroSkill.View.SkillCell;
	import GameUI.Modules.HeroSkill.View.SkillLearnCell;
	import GameUI.Modules.HeroSkill.View.SkillLifeCell;
	import GameUI.Modules.HeroSkill.View.SkillPanelCell;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.MainSence.Command.MoveQuickfCommand;
	import GameUI.Modules.MainSence.Data.MainSenceData;
	import GameUI.Modules.MainSence.Proxy.QuickGridManager;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpData;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUp;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkill;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class SkillLearnMediator extends Mediator
	{
		public static const NAME:String = "SkillLearnMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		private var type:String = "learnMediator";
		
		private var aSkillCell:Array;
		private var allIdArr:Array;
		private var cellContainer:Sprite = new Sprite();                               //cell容器
		private var currentID:int;										//点击格子的id
		private var currentLevel:int;									//点击格子的等级
		private var currentIndex:int;									//点击格子的序列号
		
			//技能相关信息
		private var aSkillObj:Array;	
		
		private var needExp:uint;										//升级需要经验
		private var needMoney:uint;									//升级需要钱
		
		private var hasMoney:uint;										//拥有钱
		private var hasExp:uint;											//拥有经验
		private var hasBind:int;
		private var hasUnBind:int;
		
		private var currentPage:int;
		private var totalPage:int = 1;
		private var currentJob:int;
		private var currentRol:RoleJob;
		private var maxLevel:int = 150;											//技能最高级别
		private var job:int;
		private var yellowFrame:MovieClip;
		private var cJob:uint;														//人物当前职业
		private var lastTime:Number;
		
		private var money_txt:TextField = new TextField();
		//学习十级需要的钱和经验
		private var needExpTen:int;
		private var needMoneyTen:int;
		
		public function SkillLearnMediator()
		{
			super(NAME);
		}
		
		public function get skillLearnView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				SkillConst.LEARNSKILL,
				SkillConst.SKILLUP_SUC,
				EventList.CLOSE_NPC_ALL_PANEL,
				EventList.CLOSE_LEARNSKILL_VIEW,
				EventList.UPDATEMONEY,													//更新钱		
				EventList.UPDATEATTRIBUATT,											//更新经验
//				SkillConst.LEARN_LIFE_SKILL_PAN,
//				SkillConst.LIFE_SKILL_UPDONE,
				SkillConst.STOP_MOVE_SKILLLEARN_PANEL								//还原并锁定技能学习面板位置    by Ginoo  2010.11.7
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification(EventList.GETRESOURCE, {type:UIConfigData.MOVIECLIP, mediator:this, name:SkillConst.SKILLLEARNVIEW});
					facade.registerCommand(SkillConst.LEARN_SKILL_SEND,LearnSkillCommand);
					facade.registerCommand(SkillConst.NEWSKILL_RECEIVE,ReceiveSkillCommand);
					panelBase = new PanelBase(skillLearnView, skillLearnView.width+8, skillLearnView.height+12);
					panelBase.name = "SkillLearnView";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_her_med_skillL_han" ]);//"技能学习"
					skillLearnView.learnOne_txt.mouseEnabled = false;
					skillLearnView.learnTen_txt.mouseEnabled = false;
				break;
				case SkillConst.LEARNSKILL:
					job = notification.getBody().ID;
					initSkillUI();
					getSkillInfo();
					creatCell();
					initBtns();
					addEventLisen();
				break;
				
				case SkillConst.SKILLUP_SUC:
				//播放特效
					if ( dataProxy.LearnSkillIsOpen )
					{
						var id:int = notification.getBody().skillID;
						var le:int = notification.getBody().skillLevel;
						skillUpHandler(le,id);
					}
					RoleLevUp.PlayLevUp(GameCommonData.Player.Role.Id,true);
				break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					if(dataProxy.LearnSkillIsOpen)
					{
						panelCloseHandler(null);
					}
				break;
				
				case EventList.CLOSE_LEARNSKILL_VIEW:
					if(dataProxy.LearnSkillIsOpen)
					{
						panelCloseHandler(null);
					}
				break;
				
				case EventList.UPDATEMONEY:									//收到更新钱的消息
					if(dataProxy.LearnSkillIsOpen)
					{
						if(notification.getBody().target == "mcBind")
						{
							skillLearnView.mcUnBind.txtMoney.text = notification.getBody().money;
							ShowMoney.ShowIcon(skillLearnView.mcUnBind, skillLearnView.mcUnBind.txtMoney, true);
						}
						if(notification.getBody().target == "mcUnBind")
						{
							skillLearnView.mcBind.txtMoney.text = notification.getBody().money;
							ShowMoney.ShowIcon(skillLearnView.mcBind, skillLearnView.mcBind.txtMoney, true);
						}
					}
				break;
				
				case EventList.UPDATEATTRIBUATT:													//更新经验
					if(dataProxy.LearnSkillIsOpen)
					{
						skillLearnView.hasExp_txt.text = GameCommonData.Player.Role.Exp;
					}
					break;
					
//				case SkillConst.LEARN_LIFE_SKILL_PAN:
//					initSkillUI();
//					getLifeInfo();
//					creatLifeCell();
//					initBtns();
//					addLifeEventListen();
//					break;
//					
//				case SkillConst.LIFE_SKILL_UPDONE:
//					lifeSkillUpSuc( notification.getBody() );
//					RoleLevUp.PlayLevUp(GameCommonData.Player.Role.Id,true);
//					break;
				
				case SkillConst.STOP_MOVE_SKILLLEARN_PANEL:		//还原并锁定技能学习面板位置    by Ginoo  2010.11.7
					resetAndLockPos();
					break;
			}
		}
		
		private function initSkillUI():void
		{
			dataProxy.LearnSkillIsOpen = true;
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.MainStage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.MainStage.stageHeight - GameConfigData.GameHeight)/2;;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			skillLearnView.addChild(cellContainer);
			clearContainer();
			
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillYellowFrame");
			yellowFrame.width = 142;
			yellowFrame.height = 39;
			
			initData();
			initTxt();
		}
		
		private function initData():void
		{
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			hasExp = GameCommonData.Player.Role.Exp;
			hasBind = GameCommonData.Player.Role.BindMoney;
			hasUnBind = GameCommonData.Player.Role.UnBindMoney;
			//获取当前职业
			currentJob = GameCommonData.Player.Role.CurrentJob - 1;
			currentRol =  GameCommonData.Player.Role.RoleList [currentJob] as RoleJob;
			this.cJob = currentRol.Job;
			lastTime = 0;
			this.needExp = 0;
			this.needExpTen = 0;
			this.needMoney = 0;
			this.needMoneyTen = 0;
		}
		
		private function initTxt():void
		{
			skillLearnView.content_txt.text = "";
			skillLearnView.mcBind.txtMoney.textColor = 0xffffff;
			skillLearnView.hasExp_txt.textColor = 0xffffff;
			skillLearnView.mcUnBind.txtMoney.textColor = 0xffffff;
//			needMoney = 0;
			showMoneyTxt(needMoney,hasUnBind,hasBind);

			skillLearnView.hasExp_txt.text = GameCommonData.Player.Role.Exp;
			skillLearnView.needExp_txt.text = 0;
			skillLearnView.hasExp_txt.mouseEnabled = false;
			skillLearnView.needExp_txt.mouseEnabled = false;
			skillLearnView.content_txt.mouseEnabled = false;
			skillLearnView.mcBind.txtMoney.textColor = 0xffffff;
			skillLearnView.page_txt.text = "";
			
			( skillLearnView.needSuiMoney_txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_her_med_lea_initT" ] + SkillData.addVipTail(GameCommonData.Player.Role.VIP) +"：";//"所需碎银"
		}
		
		//初始化银两文本
		private function showMoneyTxt(_needBind:int,_hasUnBind:int,_hasBind:int):void
		{
			//需要碎银文本格式
			var getNeedMoneyStr:String = UIUtils.getMoneyStandInfo(_needBind, ["\\se","\\ss","\\sc"]);
			skillLearnView.mcSkillNeedMoney.txtMoney.text = getNeedMoneyStr;
			ShowMoney.ShowIcon(skillLearnView.mcSkillNeedMoney, skillLearnView.mcSkillNeedMoney.txtMoney, true);
			
			//银两文本
			var getBindMoneyStr:String = UIUtils.getMoneyStandInfo(_hasBind, ["\\ce","\\cs","\\cc"]);
			skillLearnView.mcBind.txtMoney.text = getBindMoneyStr;
			ShowMoney.ShowIcon(skillLearnView.mcBind, skillLearnView.mcBind.txtMoney, true);
			
			var getUnBindMoneyStr:String = UIUtils.getMoneyStandInfo(_hasUnBind, ["\\se","\\ss","\\sc"]);
			skillLearnView.mcUnBind.txtMoney.text = getUnBindMoneyStr;
			ShowMoney.ShowIcon(skillLearnView.mcUnBind, skillLearnView.mcUnBind.txtMoney, true);
		}
		
		private function creatCell():void
		{
			aSkillCell = [];
			currentPage = 0;
			
			for ( var i:int = 0; i < aSkillObj.length; i++ )
			{
				var col:int = i%2;
				var row:int = Math.floor(i/2);
				
				var skillCell:SkillCell = new SkillLearnCell(aSkillObj[i].id,job);
				skillCell.addEventListener(SkillConst.CELLCLICK,clickCellHandler);
				aSkillCell.push(skillCell);
			}
			viewCurrentPage();
		}
		
		private function getSkillInfo():void
		{
			setTitle(job);
			this.aSkillObj = [];
			//所有技能信息
			for(var j:uint = 0;j < GameCommonData.Skills.length;j++)
			{
				var obj:Object = GameCommonData.SkillList[GameCommonData.Skills[j]];
				if ( (obj.Job == this.job) && (obj.BookID == 0) )
				{
					var skillObj:Object = new Object();
					skillObj.id = obj.SkillID;
					skillObj.needLevel = obj.NeedLevel;
					this.aSkillObj.push(skillObj);
				}
			}
			aSkillObj.sortOn("needLevel",Array.NUMERIC);
			if ( this.job == GameCommonData.Player.Role.MainJob.Job )
			{
				SkillData.aMainSkillId = [];
				for ( var k:uint=0; k<aSkillObj.length; k++ )
				{
					SkillData.aMainSkillId.push( aSkillObj[k].id );
				}
			}
		}
		
		private function initBtns():void     
		{
			skillLearnView.left_btn.addEventListener(MouseEvent.CLICK,goLeft);
			skillLearnView.right_btn.addEventListener(MouseEvent.CLICK,goRight);
			skillLearnView.commit_btn.visible = false;
			skillLearnView.learnOne_txt.textColor = 0xff0000;
			skillLearnView.learnTen_btn.visible = false;
			skillLearnView.learnTen_txt.textColor = 0xff0000;
		}
		
		private function addEventLisen():void
		{
			skillLearnView.commit_btn.addEventListener(MouseEvent.CLICK,learnSkill); 
			skillLearnView.learnTen_btn.addEventListener( MouseEvent.CLICK,learnSkill );
		}
		
		private function viewCurrentPage():void
		{
			var len:int = aSkillCell.length;
			totalPage = Math.ceil(len/10);
			
			clearContainer();
			
			if(currentPage < totalPage-1)
			{
				for(var i:int = currentPage*10;i < (currentPage+1)*10;i++)
				{
					placeCell(i,aSkillCell[i]);
				}
			}
			
			if(currentPage == totalPage-1)
			{
				for(var j:int = currentPage*10;j < len;j++)
				{
					placeCell(j,aSkillCell[j]);
				}
			}
			
			skillLearnView.page_txt.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + (currentPage+1) + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第"	"页"
		}
		
		private function placeCell(i:int,skillCell:SkillCell):void
		{
			var col:int = i%2;
			var row:int = Math.floor((i-currentPage*10)/2);
			if(aSkillCell[i])
			{
				cellContainer.addChild(aSkillCell[i]);
				skillCell.x = 13.5 + 144 * col;
				skillCell.y = 30 + 40 * row;
//				skillCell.addEventListener(SkillConst.CELLCLICK,clickCellHandler);
			}
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if(currentPage >= 1)
			{
				currentPage --;
			}
			viewCurrentPage();
		}
		
		private function goRight(evt:MouseEvent):void
		{
			if((currentPage+1) < totalPage)
			{
				currentPage ++;
			}
			viewCurrentPage();
		}
		
		private function clearContainer():void
		{
			var des:SkillCell;
			if(cellContainer && cellContainer.numChildren>0)
			{
				for(var i:int = cellContainer.numChildren-1;i >= 0;i--)
				{
					des = cellContainer.removeChildAt(0) as SkillCell;
					des.gc();
					des = null;
				}
			}
		}
		
		//点击格子之后
		private function clickCellHandler(evt:SkillEvent):void
		{
			if (evt.currentTarget is SkillPanelCell)
			{
				return;
			}
			
			skillLearnView.mcBind.txtMoney.textColor = 0xffffff;
			skillLearnView.mcUnBind.txtMoney.textColor = 0xffffff;
			skillLearnView.hasExp_txt.textColor = 0xffffff;
			
			SkillConst.selectedSkill = evt.currentTarget as SkillCell;
			currentID = evt.currentTarget.learnCellId;
			currentLevel = evt.currentTarget.learnCellLevel;
			//计算需要经验和钱
			countMoney(currentID,currentLevel);
			showMoneyTxt(needMoney,hasUnBind,hasBind);
			
			setLearnButton( evt.currentTarget.canLearn );
			skillLearnView.content_txt.htmlText = (evt.currentTarget as SkillLearnCell).getRemark(evt.currentTarget.learnCellLevel);
			
			if ( !(evt.currentTarget as SkillLearnCell).contains(yellowFrame) )
			{
				(evt.currentTarget as SkillLearnCell).addChild(yellowFrame);
				//通知新手引导系统  by Ginoo  2010.11.7
				if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_SKILLCELL_NOTICE_NEW_HELP, {id:currentID}); 
			}
		}
		
		private function setLearnButton(_canLearn:Boolean):void
		{
			if ( !_canLearn )
			{
				learnOneState( false );
				learnTenState( false );
			}
			else
			{
				if(hasMoney < needMoney)
				{
					learnOneState( false );
					skillLearnView.mcBind.txtMoney.textColor = 0xff0000;
					skillLearnView.mcUnBind.txtMoney.textColor = 0xff0000;
				}else
				{
					skillLearnView.mcBind.txtMoney.textColor = 0xffffff;
					skillLearnView.mcUnBind.txtMoney.textColor = 0xffffff;
				}
			
				if(hasExp < needExp )
				{
					learnOneState( false );
					skillLearnView.hasExp_txt.textColor = 0xff0000;
				}else
				{
					skillLearnView.hasExp_txt.textColor = 0xffffff;
				}
			
				if(hasMoney >= needMoney && hasExp >= needExp )
				{
					learnOneState( true );
				}
				
				if(hasMoney >= needMoneyTen && hasExp >= needExpTen && this.currentLevel <= this.currentRol.Level )
				{
					learnTenState( true );
				}
				else
				{
					learnTenState( false );
				}
			}
		}
		
		//获取技能所需经验和碎银
		private function  GetExp(exp:int,level:int,Exp:uint):int
		{
			if(level == 150)
			{
				return 0;
			}
			else
			{
			 	return exp * Exp; 
			}
		}
		
		private function countMoney(_id:uint,_level:uint):void
		{
			needExpTen = 0;
			needMoneyTen = 0;
			var Exp:uint = (GameCommonData.SkillList[_id] as GameSkill).Exp;
//			var exp:uint = UIConstData.ExpDic[4001+_level] ;							//所需经验 
			var exp:uint = UIConstData.ExpDic[4001+_level] ;							//所需经验 
			needExp = GetExp(exp,_level,Exp);
			skillLearnView.needExp_txt.text = needExp.toString();
			
//			var mExp:uint = UIConstData.ExpDic[5001+_level] ;						//所需钱
			var mExp:uint = UIConstData.ExpDic[10001+_level] ;						//所需钱
			var vip:int = GameCommonData.Player.Role.VIP;
			if ( vip == 4 ) vip = 1; 
			needMoney = Math.floor( GetExp(mExp,_level,Exp)* ( 1-vip*0.05 ) );
			
			for ( var i:uint=0; i<10; i++ )
			{
//				var exp_10:uint = UIConstData.ExpDic[4001+_level];
				needExpTen += GetExp(UIConstData.ExpDic[4001+_level+i],_level+11,Exp);
				needMoneyTen += Math.floor( GetExp(UIConstData.ExpDic[5001+_level+i] ,_level+11,Exp)* ( 1-GameCommonData.Player.Role.VIP*0.05 ) );
			}

//			trace ( "10级钱是："+needMoneyTen,"   :  "+needExpTen );
		}
		
		private function learnSkill(evt:MouseEvent):void
		{
//			trace("learn a skill" +currentID +"currentLevel = " +currentLevel);
			
			if ( this.job != this.currentRol.Job )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_skillL_lea" ], color:0xffff00});//"职业不符！"
				return;
			}
			if ( lastTime == 0 )
			{
				sendToServer( evt.target.name );
				lastTime = new Date().getTime();
			}
			else
			{
				var newTime:Number = new Date().getTime();
				if ( (newTime-lastTime)<800 )
				{
					return;
				}
				else
				{
//					sendNotification(SkillConst.LEARN_SKILL_SEND,{SkillId:currentID,SkillLevel:currentLevel});
					sendToServer( evt.target.name );
					lastTime = newTime;
				}
			}
			
			if( SkillConst.selectedSkill.isNewSkill() ) 
			{
				var quickGridManager:QuickGridManager=facade.retrieveProxy(QuickGridManager.NAME) as QuickGridManager;	
				var object:Object = quickGridManager.getEmptyQuickBar0();
				if( object )
				{
					facade.registerCommand( SkillConst.MOVESKILL, MoveSkillIconCommand );
					sendNotification( SkillConst.MOVESKILL, {point:new Point(Point(object.point).x+2, Point(object.point).y+2), type:object.type, index:object.index, source:SkillConst.selectedSkill.getSkillItem()} );
				}else{
					object = quickGridManager.getEmptyQuickBar1();
					if( object )
					{
						facade.registerCommand( SkillConst.MOVESKILL, MoveSkillIconCommand );
						facade.registerCommand( MainSenceData.MOVEQUICKF, MoveQuickfCommand );
						sendNotification( MainSenceData.MOVEQUICKF, {point:new Point(Point(object.point).x+2, Point(object.point).y+2), type:object.type, index:object.index, source:SkillConst.selectedSkill.getSkillItem()} );
					}
				}
			}
			
			//通知新手引导系统	by Ginoo 11.8 
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLICK_SKILLLEARN_BUTTON_NOTICE_NEW_HELP);
		}
		
		//发送数据到服务器
		private function sendToServer( bName:String ):void
		{
			if ( bName == "commit_btn" )
			{
				sendNotification(SkillConst.LEARN_SKILL_SEND,{ SkillId:currentID,SkillLevel:currentLevel,times:1 } );	
			}
			else if ( bName == "learnTen_btn" )
			{
				sendNotification(SkillConst.LEARN_SKILL_SEND,{ SkillId:currentID,SkillLevel:currentLevel,times:10 } );	
			}
		}
		
		private function panelCloseHandler(event:Event):void
		{
			clearContainer();
		    if ( this.aSkillCell && this.aSkillCell.length>0 )
			{
				for ( var j:uint=0; j<aSkillCell.length; j++ )
				{
					aSkillCell[j] = null;
				}
			} 
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.removeChild(panelBase);
			}
			//通知新手引导系统  by Ginoo  2010.11.7
			if(NewerHelpData.newerHelpIsOpen) sendNotification(NewerHelpEvent.CLOSE_SKILLLEARN_NOTICE_NEW_HELP);
			panelBase.IsDrag = true;		//解除面板位置锁定  by Ginoo  2010.11.7
			dataProxy.LearnSkillIsOpen = false;
			
			skillLearnView.left_btn.removeEventListener(MouseEvent.CLICK,goLeft);
			skillLearnView.right_btn.removeEventListener(MouseEvent.CLICK,goRight);
			
			skillLearnView.commit_btn.removeEventListener(MouseEvent.CLICK,learnLifeSkill);            //测试用
			skillLearnView.commit_btn.removeEventListener(MouseEvent.CLICK,learnSkill);            //测试用
			skillLearnView.learnTen_btn.removeEventListener( MouseEvent.CLICK,learnSkill );
		}
		
		//升级成功
		private function skillUpHandler(level:int,id:int):void
		{
			skillLearnView.hasExp_txt.text = GameCommonData.Player.Role.Exp;
			
			hasBind = GameCommonData.Player.Role.BindMoney;
			hasUnBind = GameCommonData.Player.Role.UnBindMoney;
			hasExp = GameCommonData.Player.Role.Exp;
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			currentLevel = level;
			countMoney(id,currentLevel);

			//需要碎银文本格式
			var getNeedMoneyStr:String = UIUtils.getMoneyStandInfo(needMoney, ["\\se","\\ss","\\sc"]);
			skillLearnView.mcSkillNeedMoney.txtMoney.text = getNeedMoneyStr;
			ShowMoney.ShowIcon(skillLearnView.mcSkillNeedMoney, skillLearnView.mcSkillNeedMoney.txtMoney, true);
			
			// 处理外来学习技能成功
			comeSkillHandler();
			setLearnButton(true);																	//测试完之后重新启用
			refreshCell(level,id);
		}

		private function refreshCell(newLevel:uint,newId:uint):void
		{
			for ( var i:uint=0; i<aSkillObj.length; i++ )
			{
				if ( aSkillObj[i].id == newId )
				{
					skillLearnView.content_txt.htmlText = (aSkillCell[i] as SkillLearnCell).getRemark(newLevel);
					(aSkillCell[i] as SkillLearnCell).skillUpDone(newLevel);
					return;
				}
			}
		}

		private function comeSkillHandler():void
		{
			
		}
		
		//学1级按钮状态
		private function learnOneState(value:Boolean):void
		{
			if ( value )
			{
				skillLearnView.commit_btn.visible = true;
				skillLearnView.learnOne_txt.textColor = 0x00ff00;
			}
			else
			{
				skillLearnView.commit_btn.visible = false;
				skillLearnView.learnOne_txt.textColor = 0xff0000;
			}
		}
		
		private function learnTenState(value:Boolean):void
		{
			if ( value )
			{
				skillLearnView.learnTen_btn.visible = true;
				skillLearnView.learnTen_txt.textColor = 0x00ff00;
			}
			else
			{
				skillLearnView.learnTen_btn.visible = false;
				skillLearnView.learnTen_txt.textColor = 0xff0000;
			}
		}
		
		//标题头部文字显示
		private function setTitle(id:int):void
		{
			switch(id)
			{
				case 4096:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_1" ];//"新手属性";
					break;
					
				case 1:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_2" ];//"唐门技能";
					break;
					
				case 2:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_3" ];//"全真技能";
					break;
					
				case 4:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_4" ];//"峨眉技能";
					break;
					
				case 8:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_5" ];//"丐帮技能";
					break;
					
				case 16:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_6" ];//"少林技能";
					break;
					
				case 32:
					skillLearnView.title_txt.text = GameCommonData.wordDic[ "mod_her_med_skillL_setT_7" ];//"点苍技能";
					break;
			}
		}
		
		//生活技能
		private var learnLifeMoney:uint = 10;							//所需钱基数
		private var leanLifeNum:uint;								//已学生活技能个数
		private var aLearnLifeSkillID:Array;						//所有已学生活技能id
		private var curLifeSkillLevel:uint;
		private var curFamiliar:uint;									//当前熟练度
		
		private function getLifeInfo():void
		{
			( skillLearnView.title_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_getL" ];//"生活技能";
			this.aSkillObj = [];
			//所有技能信息
			for ( var sId:* in GameCommonData.LifeSkillList )
			{
				var lifeSkill:Object = GameCommonData.LifeSkillList[sId];
				var obj:Object = new Object();
				obj.id = lifeSkill.SkillID;
				obj.needLevel = lifeSkill.NeedLevel;
				this.aSkillObj.push(obj);
			}
			aSkillObj.sortOn("needLevel",Array.NUMERIC);
			aSkillObj.sortOn("id",Array.NUMERIC);
		}
		
		private function creatLifeCell():void
		{
			aSkillCell = [];
			currentPage = 0;
			
			for ( var i:int = 0; i < aSkillObj.length; i++ )
			{
				var col:int = i%2;
				var row:int = Math.floor(i/2);
				
				var skillCell:SkillCell = new SkillLifeCell(aSkillObj[i].id,cJob,2);
				skillCell.addEventListener(SkillConst.LIFECELL_CLICK,clickLifeCell);
				aSkillCell.push(skillCell);
			}
			viewCurrentPage();
		}
		
		private function clickLifeCell(evt:SkillEvent):void
		{
			if ( !(evt.currentTarget as SkillLifeCell).contains(yellowFrame) )
			{
				(evt.currentTarget as SkillLifeCell).addChild(yellowFrame);		
			}
			
			curLifeSkillLevel = ( evt.currentTarget as SkillLifeCell ).lifeLevel;
			currentID = ( evt.currentTarget as SkillLifeCell ).lifeCellId;
			curFamiliar = ( evt.currentTarget as SkillLifeCell ).familiar;
			
			skillLearnView.content_txt.text = ( evt.currentTarget as SkillLifeCell ).getRemark(0);
			countLifeMoney(curLifeSkillLevel);
			showMoneyTxt(needMoney,hasUnBind,hasBind);
			this.setLearnButton( (evt.currentTarget as SkillLifeCell).canLearn ); 
		}
		
		private function countLifeMoney(_level:uint):void
		{
			this.needExp = ( _level+1 )*100000;	
			this.needMoney = needExp;
			skillLearnView.needExp_txt.text = needExp.toString();
		}
		
		private function addLifeEventListen():void
		{
			skillLearnView.commit_btn.addEventListener(MouseEvent.CLICK,learnLifeSkill);            //测试用
		}
		
		private function learnLifeSkill(evt:MouseEvent):void
		{
			if ( curLifeSkillLevel>=10  )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_skillL_learnLi_1" ], color:0xffff00});//"技能已学至最高等级"	
				return;
			}
			
			if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -50 )			//学习打造技能
			{
				if ( GameCommonData.Player.Role.Level<90 )
				{
					if ( GameCommonData.Player.Role.Level<(20+curLifeSkillLevel*10) )
					{
						var info:String = GameCommonData.wordDic[ "mod_her_med_lea_lea_3" ]+"<font color='#ff0000'>"+(curLifeSkillLevel+1)+"</font>"+GameCommonData.wordDic[ "mod_her_med_skillL_learnLi_2" ]+"<font color='#ff0000'>"+(20+curLifeSkillLevel*10)+"</font>"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//学习		级打造技能需要人物等级达到		级         
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:info, color:0xffff00});	
						return;
					}
				}
			}
			else if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -8 )			//学习采集技能
			{
				if ( GameCommonData.Player.Role.Level < (20+curLifeSkillLevel*5) )
				{
					var info2:String = GameCommonData.wordDic[ "mod_her_med_lea_lea_3" ]+"<font color='#ff0000'>"+(curLifeSkillLevel+1)+"</font>"+GameCommonData.wordDic[ "mod_her_med_skillL_learnLi_3" ]+"font color='#ff0000'>"+(20+curLifeSkillLevel*5)+"</font>"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//学习	  级采集技能需要人物等级达到<		级         
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:info2, color:0xffff00});	
					return;
				}
			}
			
//			if ( curFamiliar >= curLifeSkillLevel*1000 )
			if ( curLifeSkillLevel == 0 )
			{ 
				sendNotification(SkillConst.LEARN_SKILL_SEND,{SkillId:currentID,SkillLevel:curLifeSkillLevel});
				return;
			}
			else
			{
				if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -8 )			//学习采集技能
				{
					if ( curFamiliar >= 200*Math.pow( 2,curLifeSkillLevel-1 ) )
					{
						sendNotification(SkillConst.LEARN_SKILL_SEND,{SkillId:currentID,SkillLevel:curLifeSkillLevel});
					}else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_skillL_learnLi_4" ], color:0xffff00});	//"技能熟练度不够，不能升级"
						return;
					}
				}
				else if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -50 )			//学习打造技能
				{
					if ( curFamiliar >= ManufactoryData.aLifeSkillFam[ this.curLifeSkillLevel-1 ] )
					{
						sendNotification(SkillConst.LEARN_SKILL_SEND,{SkillId:currentID,SkillLevel:curLifeSkillLevel});
					}else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_skillL_learnLi_4" ], color:0xffff00});//"技能熟练度不够，不能升级"	
						return;
					}
				}
			}
		}
		
		//生活技能升级成功
		private function lifeSkillUpSuc(obj:Object):void
		{
			skillLearnView.hasExp_txt.text = GameCommonData.Player.Role.Exp;
			hasBind = GameCommonData.Player.Role.BindMoney;
			hasUnBind = GameCommonData.Player.Role.UnBindMoney;
			hasExp = GameCommonData.Player.Role.Exp;
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			curLifeSkillLevel = obj.level;
			countLifeMoney(curLifeSkillLevel);

			//需要碎银文本格式
			var getNeedMoneyStr:String = UIUtils.getMoneyStandInfo(needMoney, ["\\se","\\ss","\\sc"]);
			skillLearnView.mcSkillNeedMoney.txtMoney.text = getNeedMoneyStr;
			ShowMoney.ShowIcon(skillLearnView.mcSkillNeedMoney, skillLearnView.mcSkillNeedMoney.txtMoney, true);
			
			setLearnButton(true);																	//测试完之后重新启用 
			refreshLifeCell(obj);
		}
		
		private function refreshLifeCell(obj:Object):void
		{
			for ( var i:uint=0; i<aSkillObj.length; i++ )
			{
				if ( aSkillObj[i].id == obj.id )
				{
					( aSkillCell[i] as SkillLifeCell ).skillUpDone(obj.level,obj.familiar);
					return;
				}
			}
		}
		
		/** 重置并锁定技能学习面板位置（禁止拖动） by Ginoo  2010.11.7 */
		private function resetAndLockPos():void
		{
			if( GameCommonData.fullScreen == 2 )
			{
				panelBase.x = UIConstData.DefaultPos1.x + (GameCommonData.GameInstance.GameUI.stage.stageWidth - GameConfigData.GameWidth)/2;
				panelBase.y = UIConstData.DefaultPos1.y + (GameCommonData.GameInstance.GameUI.stage.stageHeight - GameConfigData.GameHeight)/2;
			}else{
				panelBase.x = UIConstData.DefaultPos1.x;
				panelBase.y = UIConstData.DefaultPos1.y;
			}
			panelBase.IsDrag = false;
		}
	}
}