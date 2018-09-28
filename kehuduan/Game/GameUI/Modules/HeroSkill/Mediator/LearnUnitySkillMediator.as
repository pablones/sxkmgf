package GameUI.Modules.HeroSkill.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.HeroSkill.View.SkillCell;
	import GameUI.Modules.HeroSkill.View.SkillUnityCell;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUp;
	import GameUI.Modules.Unity.Command.SendActionCommand;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.BaseUI.PanelBase;
	import GameUI.View.ShowMoney;
	
	import OopsEngine.Role.RoleJob;
	import OopsEngine.Skill.GameSkill;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.getTimer;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	public class LearnUnitySkillMediator extends Mediator
	{
		public static const NAME:String = "LearnUnitySkillMediator";
		private var panelBase:PanelBase = null;
		private var dataProxy:DataProxy = null;
		
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
		
		private var lastTimeRequest:int;
		private var currentCell:SkillUnityCell;									//当前点击的格子
		
		private var money_txt:TextField = new TextField();
		//学习十级需要的钱和经验
		private var needExpTen:int;
		private var needMoneyTen:int;
		
		public function LearnUnitySkillMediator()
		{
			super(NAME);
		}
		
		public function get main_mc():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				EventList.INITVIEW,
				SkillConst.LEARN_UNITY_SKILL_PAN,
				SkillConst.UNITY_SKILL_UPDONE,
				EventList.CLOSE_NPC_ALL_PANEL,
//				EventList.CLOSE_LEARNSKILL_VIEW,
				EventList.UPDATEMONEY,													//更新钱		
				SkillConst.REC_UNITY_SKILL_STUDLEV
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case EventList.INITVIEW:
					dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					facade.sendNotification( EventList.GETRESOURCE, { type:UIConfigData.MOVIECLIP, mediator:this, name:SkillConst.LEARN_UNITY_SKILL_UI } );
					panelBase = new PanelBase(main_mc, main_mc.width+8, main_mc.height+12);
					panelBase.name = "Unitymain_mc";
					panelBase.addEventListener(Event.CLOSE, panelCloseHandler);
					panelBase.x = UIConstData.DefaultPos1.x;
					panelBase.y = UIConstData.DefaultPos1.y;
					panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_her_med_learnU_han" ]);//"帮派技能学习"
					main_mc.learnOne_txt.mouseEnabled = false;
					main_mc.learnTen_txt.mouseEnabled = false;
				break;
				case SkillConst.LEARN_UNITY_SKILL_PAN:
					if ( dataProxy.learnUnitySkillIsOpen )
					{
						panelCloseHandler( null );
						return;
					}
					requestData();
					initSkillUI();
					getSkillInfo();
					creatCell();
					initBtns();
					addEventLisen();
				break;
				
				case SkillConst.UNITY_SKILL_UPDONE:
				//播放特效
					if ( dataProxy.learnUnitySkillIsOpen )
					{
						var id:int = notification.getBody().skillID;
						var le:int = notification.getBody().skillLevel;
						skillUpHandler(le,id);
					}
					RoleLevUp.PlayLevUp(GameCommonData.Player.Role.Id,true);
				break;
				case EventList.CLOSE_NPC_ALL_PANEL:
					if(dataProxy.learnUnitySkillIsOpen)
					{
						panelCloseHandler(null);
					}
				break;
				
//				case EventList.CLOSE_LEARNSKILL_VIEW:
//					if(dataProxy.learnUnitySkillIsOpen)
//					{
//						panelCloseHandler(null);
//					}
//				break;
				
				case EventList.UPDATEMONEY:									//收到更新钱的消息
					if(dataProxy.learnUnitySkillIsOpen)
					{
						if(notification.getBody().target == "mcBind")
						{
							main_mc.mcUnBind.txtMoney.text = notification.getBody().money;
							ShowMoney.ShowIcon(main_mc.mcUnBind, main_mc.mcUnBind.txtMoney, true);
						}
						if(notification.getBody().target == "mcUnBind")
						{
							main_mc.mcBind.txtMoney.text = notification.getBody().money;
							ShowMoney.ShowIcon(main_mc.mcBind, main_mc.mcBind.txtMoney, true);
						}
					}
				break;
				
				case SkillConst.REC_UNITY_SKILL_STUDLEV:
					if ( dataProxy.learnUnitySkillIsOpen )
					{
						updataCellPromt();
					}
				break;
			}
		}
		
		//请求数据
		private function requestData():void
		{
			var newTime:int = getTimer();
			if ( lastTimeRequest == 0 )
			{
				facade.sendNotification( SendActionCommand.SENDACTION , { type:221 } );
				lastTimeRequest = newTime;
				return;
			}
			if ( newTime - lastTimeRequest > 700 )
			{
				facade.sendNotification( SendActionCommand.SENDACTION , { type:221 } );
				lastTimeRequest = newTime;
			}
		}
		
		private function initSkillUI():void
		{
			dataProxy.learnUnitySkillIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			main_mc.addChild(cellContainer);
			clearContainer();
			
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillYellowFrame");
			yellowFrame.width = 142;
			yellowFrame.height = 39;
			main_mc.title_txt.text = GameCommonData.wordDic[ "mod_her_med_learnU_initS" ];//"帮派技能";
			
			initData();
			initTxt();
		}
		
		private function initData():void
		{
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			hasExp = GameCommonData.Player.Role.unityContribution;
			hasBind = GameCommonData.Player.Role.BindMoney;
			hasUnBind = GameCommonData.Player.Role.UnBindMoney;
		
			lastTime = 0;
			this.needExp = 0;
			this.needExpTen = 0;
			this.needMoney = 0;
			this.needMoneyTen = 0;
		}
		
		private function initTxt():void
		{
			main_mc.content_txt.text = "";
			main_mc.mcBind.txtMoney.textColor = 0xffffff;
			main_mc.hasExp_txt.textColor = 0xffffff;
			main_mc.mcUnBind.txtMoney.textColor = 0xffffff;
//			needMoney = 0;
			showMoneyTxt(needMoney,hasUnBind,hasBind);

			main_mc.hasExp_txt.text = GameCommonData.Player.Role.unityContribution;
			main_mc.needExp_txt.text = 0;
			main_mc.hasExp_txt.mouseEnabled = false;
			main_mc.needExp_txt.mouseEnabled = false;
			main_mc.content_txt.mouseEnabled = false;
			main_mc.mcBind.txtMoney.textColor = 0xffffff;
			main_mc.page_txt.text = "";
			
			( main_mc.needSuiMoney_txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_her_med_lea_initT" ] + SkillData.addVipTail(GameCommonData.Player.Role.VIP) +"：";//"所需碎银"
		}
		
		//初始化银两文本
		private function showMoneyTxt(_needBind:int,_hasUnBind:int,_hasBind:int):void
		{
			//需要碎银文本格式
			var getNeedMoneyStr:String = UIUtils.getMoneyStandInfo(_needBind, ["\\se","\\ss","\\sc"]);
			main_mc.mcSkillNeedMoney.txtMoney.text = getNeedMoneyStr;
			ShowMoney.ShowIcon(main_mc.mcSkillNeedMoney, main_mc.mcSkillNeedMoney.txtMoney, true);
			
			//银两文本
			var getBindMoneyStr:String = UIUtils.getMoneyStandInfo(_hasBind, ["\\ce","\\cs","\\cc"]);
			main_mc.mcBind.txtMoney.text = getBindMoneyStr;
			ShowMoney.ShowIcon(main_mc.mcBind, main_mc.mcBind.txtMoney, true);
			
			var getUnBindMoneyStr:String = UIUtils.getMoneyStandInfo(_hasUnBind, ["\\se","\\ss","\\sc"]);
			main_mc.mcUnBind.txtMoney.text = getUnBindMoneyStr;
			ShowMoney.ShowIcon(main_mc.mcUnBind, main_mc.mcUnBind.txtMoney, true);
		}
		
		private function creatCell():void
		{
			aSkillCell = [];
			currentPage = 0;
			
			for ( var i:int = 0; i < aSkillObj.length; i++ )
			{
				var col:int = i%2;
				var row:int = Math.floor(i/2);
				
				var skillCell:SkillUnityCell = new SkillUnityCell( aSkillObj[i].id );
				skillCell.clickCellHandler = clickCellHandler;
				aSkillCell.push(skillCell);
			}
			viewCurrentPage();
		}
		
		private function getSkillInfo():void
		{
			this.aSkillObj = [];
			//所有技能信息
			for(var j:uint = 0;j < GameCommonData.Skills.length;j++)
			{
				var obj:Object = GameCommonData.SkillList[GameCommonData.Skills[j]];
				if ( ( obj.Job == -6 ) && ( obj.BookID == 0 ) )
				{
					var skillObj:Object = new Object();
					skillObj.id = obj.SkillID;
					skillObj.needLevel = obj.NeedLevel;
					this.aSkillObj.push(skillObj);
				}
			}
			aSkillObj.sortOn("id",Array.NUMERIC);
		}
		
		private function initBtns():void     
		{
			main_mc.left_btn.addEventListener(MouseEvent.CLICK,goLeft);
			main_mc.right_btn.addEventListener(MouseEvent.CLICK,goRight);
			main_mc.commit_btn.visible = false;
			main_mc.learnOne_txt.textColor = 0xff0000;
			main_mc.learnTen_btn.visible = false;
			main_mc.learnTen_txt.textColor = 0xff0000;
		}
		
		private function addEventLisen():void
		{
			main_mc.commit_btn.addEventListener( MouseEvent.CLICK,learnSkill ); 
			main_mc.learnTen_btn.addEventListener( MouseEvent.CLICK,learnSkill );
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
			
			main_mc.page_txt.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + (currentPage+1) + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第"		"页"
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
			}
		}
		
		private function goLeft(evt:MouseEvent):void
		{
			if(currentPage >= 1)
			{
				currentPage --;
//				currentCell = null;
				setLearnButton( false );
				main_mc.content_txt.htmlText = "";
			}
			viewCurrentPage();
		}
		
		private function goRight(evt:MouseEvent):void
		{
			if((currentPage+1) < totalPage)
			{
				currentPage ++;
//				currentCell = null;
				setLearnButton( false );
				main_mc.content_txt.htmlText = "";
			}
			viewCurrentPage();
		}
		
		private function clearContainer():void
		{
			var des:DisplayObjectContainer;
			while ( cellContainer.numChildren>0 )
			{
				des = cellContainer.removeChildAt(0) as DisplayObjectContainer;
				if ( des.contains( yellowFrame ) )
				{
					des.removeChild( yellowFrame );
				}
				des = null;
			}
			if ( currentCell )
			{
				currentCell = null;
			}
		}
		
		//点击格子之后
		private function clickCellHandler( skillCell:SkillUnityCell ):void
		{	
			main_mc.mcBind.txtMoney.textColor = 0xffffff;
			main_mc.mcUnBind.txtMoney.textColor = 0xffffff;
			main_mc.hasExp_txt.textColor = 0xffffff;
			
			currentID = skillCell.learnCellId;
			currentLevel = skillCell.learnCellLevel;
			currentCell = skillCell;
			//计算需要经验和钱
			countMoney(currentID,currentLevel);
			showMoneyTxt(needMoney,hasUnBind,hasBind);
			
			setLearnButton( skillCell.canLearn );
			main_mc.content_txt.htmlText = skillCell.getRemark( skillCell.learnCellLevel );
			
			if ( !skillCell.contains(yellowFrame) )
			{
				skillCell.addChild(yellowFrame);
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
					main_mc.mcBind.txtMoney.textColor = 0xff0000;
					main_mc.mcUnBind.txtMoney.textColor = 0xff0000;
				}else
				{
					main_mc.mcBind.txtMoney.textColor = 0xffffff;
					main_mc.mcUnBind.txtMoney.textColor = 0xffffff;
				}
			
				if(hasExp < needExp )
				{
					learnOneState( false );
					main_mc.hasExp_txt.textColor = 0xff0000;
				}else
				{
					main_mc.hasExp_txt.textColor = 0xffffff;
				}
			
				if( hasMoney >= needMoney && hasExp >= needExp )
				{
					learnOneState( true );
				}
				
				if( hasMoney >= needMoneyTen && hasExp >= needExpTen && currentCell.canLearnTen  )
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
			var exp:uint = UIConstData.ExpDic[7001+_level] ;							//所需经验
			needExp = GetExp(exp,_level,Exp);
			main_mc.needExp_txt.text = needExp.toString();
			
			var mExp:uint = UIConstData.ExpDic[5001+_level] ;						//所需钱
			needMoney = Math.floor( GetExp(mExp,_level,Exp)* ( 1-GameCommonData.Player.Role.VIP*0.05 ) );
			
			for ( var i:uint=0; i<10; i++ )
			{
				needExpTen += GetExp(UIConstData.ExpDic[7001+_level+i],_level+11,Exp);
				needMoneyTen += Math.floor( GetExp(UIConstData.ExpDic[5001+_level+i] ,_level+11,Exp)* ( 1-GameCommonData.Player.Role.VIP*0.05 ) );
			}
		}
		
		private function learnSkill(evt:MouseEvent):void
		{
//			trace("learn a skill" +currentID +"currentLevel = " +currentLevel);
			if ( UnityConstData.UnityPerformanceClose )
			{
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_learnU_learnS_1" ], color:0xffff00});//"帮派暂停维护，无法学习技能"	
				return;
			}
			if ( homeIsStop( currentID ) )
			{
				sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_learnU_learnS_2" ], color:0xffff00});//"本堂暂时关闭，无法学习技能"	
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
					sendToServer( evt.target.name );
					lastTime = newTime;
				}
			}
		}
		
		//发送数据到服务器
		private function sendToServer( bName:String ):void
		{
			if ( bName == "commit_btn" )
			{
				sendNotification( SkillConst.LEARN_SKILL_SEND,{ SkillId:currentID,SkillLevel:currentLevel,times:1 } );	
			}
			else if ( bName == "learnTen_btn" )
			{
				sendNotification( SkillConst.LEARN_SKILL_SEND,{ SkillId:currentID,SkillLevel:currentLevel,times:10 } );	
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
			dataProxy.learnUnitySkillIsOpen = false;
			currentCell = null;
			
			main_mc.left_btn.removeEventListener(MouseEvent.CLICK,goLeft);
			main_mc.right_btn.removeEventListener(MouseEvent.CLICK,goRight);
			
			main_mc.commit_btn.removeEventListener(MouseEvent.CLICK,learnSkill);            //测试用
			main_mc.learnTen_btn.removeEventListener( MouseEvent.CLICK,learnSkill );
		}
		
		//升级成功
		private function skillUpHandler(level:int,id:int):void
		{
			main_mc.hasExp_txt.text = GameCommonData.Player.Role.unityContribution;
			
			hasBind = GameCommonData.Player.Role.BindMoney;
			hasUnBind = GameCommonData.Player.Role.UnBindMoney;
			hasExp = GameCommonData.Player.Role.unityContribution;
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			currentLevel = level;
			countMoney(id,currentLevel);

			//需要碎银文本格式
			var getNeedMoneyStr:String = UIUtils.getMoneyStandInfo(needMoney, ["\\se","\\ss","\\sc"]);
			main_mc.mcSkillNeedMoney.txtMoney.text = getNeedMoneyStr;
			ShowMoney.ShowIcon(main_mc.mcSkillNeedMoney, main_mc.mcSkillNeedMoney.txtMoney, true);
			
			refreshCell(level,id);
			if ( currentCell )
			{
				setLearnButton( currentCell.canLearn );
			}
			else 
			{
				setLearnButton(true);
			}
		}

		private function refreshCell(newLevel:uint,newId:uint):void
		{
			for ( var i:uint=0; i<aSkillObj.length; i++ )
			{
				if ( aSkillObj[i].id == newId )
				{
					main_mc.content_txt.htmlText = ( aSkillCell[i] as SkillUnityCell ).getRemark(newLevel);
					( aSkillCell[i] as SkillUnityCell ).skillUpDone( newLevel );
					return;
				}
			}
		}

		//学1级按钮状态
		private function learnOneState(value:Boolean):void
		{
			if ( value )
			{
				main_mc.commit_btn.visible = true;
				main_mc.learnOne_txt.textColor = 0x00ff00;
			}
			else
			{
				main_mc.commit_btn.visible = false;
				main_mc.learnOne_txt.textColor = 0xff0000;
			}
		}
		
		private function learnTenState(value:Boolean):void
		{
			if ( value )
			{
				main_mc.learnTen_btn.visible = true;
				main_mc.learnTen_txt.textColor = 0x00ff00;
			}
			else
			{
				main_mc.learnTen_btn.visible = false;
				main_mc.learnTen_txt.textColor = 0xff0000;
			}
		}
		
		private function updataCellPromt():void
		{
			for ( var i:uint=0; i<aSkillCell.length; i++ )
			{
				( aSkillCell[i] as SkillUnityCell ).checkCanPromt();
			}
		}
		
		//当前堂是否关闭
		private function homeIsStop( _id:uint ):Boolean
		{
			var curObj:Object;
			if ( ( UnityConstData.greenUnityDataObj.skillIconList as Array ).indexOf( _id ) > -1 )
			{
				curObj = UnityConstData.greenUnityDataObj;
			}
			else if ( ( UnityConstData.whiteUnityDataObj.skillIconList as Array ).indexOf( _id ) > -1 )
			{
				curObj = UnityConstData.redUnityDataObj;
			}
			else if ( ( UnityConstData.redUnityDataObj.skillIconList as Array ).indexOf( _id ) > -1 )
			{
				curObj = UnityConstData.redUnityDataObj;
			}
			else if ( ( UnityConstData.xuanUnityDataObj.skillIconList as Array ).indexOf( _id ) > -1 )
			{
				curObj = UnityConstData.xuanUnityDataObj;
			}
			return curObj.isStop;
			
		}

	}
}