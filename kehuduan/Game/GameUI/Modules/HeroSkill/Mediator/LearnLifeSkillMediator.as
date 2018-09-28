package GameUI.Modules.HeroSkill.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.HeroSkill.Command.SkillEvent;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.HeroSkill.View.SkillCell;
	import GameUI.Modules.HeroSkill.View.SkillLifeCell;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
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
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class LearnLifeSkillMediator extends Mediator
	{
		public static const NAME:String = "LearnLifeSkillMediator";
		private var dataProxy:DataProxy;
		private var panelBase:PanelBase;
		private var cellContainer:Sprite = new Sprite();                               //cell容器
		private var yellowFrame:MovieClip;
		
		private var learnLifeMoney:uint = 10;							//所需钱基数
		private var leanLifeNum:uint;								//已学生活技能个数
		private var aLearnLifeSkillID:Array;						//所有已学生活技能id
		private var curLifeSkillLevel:uint;
		private var curFamiliar:uint;									//当前熟练度
		
		private var hasMoney:uint;										//拥有钱
		private var hasExp:uint;											//拥有经验
		private var hasBind:int;
		private var hasUnBind:int;
		
		private var aSkillObj:Array;
		private var cJob:int;
		private var needMoney:Number;
		private var needExp:Number;
		private var aSkillCell:Array;
		private var lastTime:Number;
		
		private var currentPage:int;
		private var currentID:int;
		private var currentJob:int;
		private var totalPage:int;
		private var currentRol:RoleJob;
		private var main_mc:MovieClip;
		
		public function LearnLifeSkillMediator(mediatorName:String=null)
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [EventList.INITVIEW,
						SkillConst.LEARN_LIFE_SKILL_PAN,
						SkillConst.LIFE_SKILL_UPDONE,
						EventList.UPDATEMONEY,
						EventList.UPDATEATTRIBUATT
						]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case EventList.INITVIEW:
					initView();
				break;
				case SkillConst.LEARN_LIFE_SKILL_PAN:
					initSkillUI();
					getLifeInfo();
					creatLifeCell();
					initBtns();
				break;
				case SkillConst.LIFE_SKILL_UPDONE:
					lifeSkillUpSuc( notification.getBody() );
					RoleLevUp.PlayLevUp(GameCommonData.Player.Role.Id,true);
				break;
				case EventList.UPDATEMONEY:
					upDateMoney( notification.getBody() );
				break;
				case EventList.UPDATEATTRIBUATT:
					upDateExp();
				break;
			}
		}
		
		private function initView():void
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			facade.sendNotification( EventList.GETRESOURCE, { type:UIConfigData.MOVIECLIP, mediator:this, name:SkillConst.LEARN_LIFESKILL_UIRES } );
			main_mc = this.viewComponent as MovieClip;
			panelBase = new PanelBase( main_mc, main_mc.width+8, main_mc.height+12 );
			panelBase.addEventListener( Event.CLOSE, panelCloseHandler );
			panelBase.x = UIConstData.DefaultPos1.x;
			panelBase.y = UIConstData.DefaultPos1.y;
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "mod_her_med_lea_initV" ]);//"生活技能学习"
			main_mc.learnOne_txt.mouseEnabled = false;
		}
		
		private function initSkillUI():void
		{
			dataProxy.liftLearnPanelIsOpen = true;
			GameCommonData.GameInstance.GameUI.addChild(panelBase); 
			main_mc.addChild(cellContainer);
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
		}
		
		private function initTxt():void
		{
			main_mc.content_txt.text = "";
			main_mc.mcBind.txtMoney.textColor = 0xffffff;
			main_mc.hasExp_txt.textColor = 0xffffff;
			main_mc.mcUnBind.txtMoney.textColor = 0xffffff;
			needMoney = 0;
			showMoneyTxt(needMoney,hasUnBind,hasBind);

			main_mc.hasExp_txt.text = GameCommonData.Player.Role.Exp;
			main_mc.needExp_txt.text = 0;
			main_mc.hasExp_txt.mouseEnabled = false;
			main_mc.needExp_txt.mouseEnabled = false;
			main_mc.content_txt.mouseEnabled = false;
			main_mc.mcBind.txtMoney.textColor = 0xffffff;
			main_mc.page_txt.text = "";
			main_mc.learnOne_txt.textColor = 0xff0000;
			
			( main_mc.needSuiMoney_txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_her_med_lea_initT" ]+ SkillData.addVipTail(GameCommonData.Player.Role.VIP) +"：";//"所需碎银" 
		}
		
		private function initBtns():void     
		{
			main_mc.left_btn.addEventListener(MouseEvent.CLICK,goLeft);
			main_mc.right_btn.addEventListener(MouseEvent.CLICK,goRight);
			main_mc.commit_btn.addEventListener(MouseEvent.CLICK,learnLifeSkill);  
			main_mc.commit_btn.visible = false;
		}
		
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
		
		private function setLearnButton(_canLearn:Boolean):void
		{
			if ( !_canLearn )
			{
				main_mc.commit_btn.visible = false;
				main_mc.learnOne_txt.textColor = 0xff0000;
			}
			else
			{
				if(hasMoney < needMoney)
				{
					main_mc.commit_btn.visible = false;
					main_mc.mcBind.txtMoney.textColor = 0xff0000;
					main_mc.mcUnBind.txtMoney.textColor = 0xff0000;
				}else
				{
					main_mc.mcBind.txtMoney.textColor = 0xffffff;
					main_mc.mcUnBind.txtMoney.textColor = 0xffffff;
				}
			
				if(hasExp < needExp )
				{
					main_mc.commit_btn.visible = false;
					main_mc.hasExp_txt.textColor = 0xff0000;
				}else
				{
					main_mc.hasExp_txt.textColor = 0xffffff;
				}
			
				if(hasMoney >= needMoney && hasExp >= needExp )
				{
					main_mc.commit_btn.visible = true;
					main_mc.learnOne_txt.textColor = 0x00ff00;
				}
			}
		}
		
		private function getLifeInfo():void
		{
			( main_mc.title_txt as TextField ).text = GameCommonData.wordDic[ "mod_her_med_lea_getL" ];//"生活技能";
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
				skillCell.addEventListener( SkillConst.LIFECELL_CLICK,clickLifeCell );
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
			
			main_mc.content_txt.text = ( evt.currentTarget as SkillLifeCell ).getRemark(0);
			countLifeMoney(curLifeSkillLevel);
			showMoneyTxt(needMoney,hasUnBind,hasBind);
			setLearnButton( (evt.currentTarget as SkillLifeCell).canLearn ); 
		}
		
		private function countLifeMoney(_level:uint):void
		{
			this.needExp = ( _level+1 )*100000;	
			this.needMoney = needExp;
			main_mc.needExp_txt.text = needExp.toString();
		}
		
		private function learnLifeSkill(evt:MouseEvent):void
		{
			if ( curLifeSkillLevel>=10  )
			{
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_lea_lea_1" ], color:0xffff00});//"技能已学至最高等级"	
				return;
			}
			
			if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -50 )			//学习打造技能
			{
				if ( GameCommonData.Player.Role.Level<90 )
				{
					if ( GameCommonData.Player.Role.Level<(20+curLifeSkillLevel*10) )
					{
						var info:String = GameCommonData.wordDic[ "mod_her_med_lea_lea_3" ]+"<font color='#ff0000'>"+(curLifeSkillLevel+1)+"</font>"+GameCommonData.wordDic[ "mod_her_med_lea_lea_2" ]+"<font color='#ff0000'>"+(20+curLifeSkillLevel*10)+"</font>"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//学习		级打造技能需要人物等级达到	级         
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:info, color:0xffff00});	
						return;
					}
				}
			}
			else if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -8 )			//学习采集技能
			{
				if ( GameCommonData.Player.Role.Level < (20+curLifeSkillLevel*5) )
				{
					var info2:String = GameCommonData.wordDic[ "mod_her_med_lea_lea_3" ]+"<font color='#ff0000'>"+(curLifeSkillLevel+1)+"</font>"+GameCommonData.wordDic[ "mod_her_med_lea_lea_2" ]+"<font color='#ff0000'>"+(20+curLifeSkillLevel*5)+"</font>"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]; //学习	级采集技能需要人物等级达到	级
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:info2, color:0xffff00});	
					return;
				}
			}

			if ( curLifeSkillLevel == 0 )
			{ 
				sendNotification(SkillConst.LEARN_SKILL_SEND,{ SkillId:currentID,SkillLevel:curLifeSkillLevel,times:1 });
				return;
			}
			else
			{
				if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -8 )			//学习采集技能
				{
					if ( curFamiliar >= 200*Math.pow( 2,curLifeSkillLevel-1 ) )
					{
						sendNotification(SkillConst.LEARN_SKILL_SEND,{SkillId:currentID,SkillLevel:curLifeSkillLevel,times:1});
					}else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_lea_lea_4" ], color:0xffff00});//"技能熟练度不够，不能升级"	
						return;
					}
				}
				else if ( (GameCommonData.LifeSkillList[ currentID ] as GameSkill).Job == -50 )			//学习打造技能
				{
					if ( curFamiliar >= ManufactoryData.aLifeSkillFam[ this.curLifeSkillLevel-1 ] )
					{
						sendNotification(SkillConst.LEARN_SKILL_SEND,{ SkillId:currentID,SkillLevel:curLifeSkillLevel,times:1 });
					}else
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_her_med_lea_lea_4" ], color:0xffff00});//"技能熟练度不够，不能升级"	
						return;
					}
				}
			}
		}
		
		//生活技能升级成功
		private function lifeSkillUpSuc(obj:Object):void
		{
			main_mc.hasExp_txt.text = GameCommonData.Player.Role.Exp;
			hasBind = GameCommonData.Player.Role.BindMoney;
			hasUnBind = GameCommonData.Player.Role.UnBindMoney;
			hasExp = GameCommonData.Player.Role.Exp;
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			curLifeSkillLevel = obj.level;
			countLifeMoney(curLifeSkillLevel);

			//需要碎银文本格式
			var getNeedMoneyStr:String = UIUtils.getMoneyStandInfo(needMoney, ["\\se","\\ss","\\sc"]);
			main_mc.mcSkillNeedMoney.txtMoney.text = getNeedMoneyStr;
			ShowMoney.ShowIcon(main_mc.mcSkillNeedMoney, main_mc.mcSkillNeedMoney.txtMoney, true);
			
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
		
		private function viewCurrentPage():void
		{
			var len:int = aSkillCell.length;
			totalPage = Math.ceil(len/10);
			
			clearContainer();
			
			if(currentPage < totalPage-1)
			{
				for(var i:int = currentPage*10;i < (currentPage+1)*10;i++)
				{
					placeCell( i,aSkillCell[i] );
				}
			}
			
			if(currentPage == totalPage-1)
			{
				for(var j:int = currentPage*10;j < len;j++)
				{
					placeCell( j,aSkillCell[j] );
				}
			}
			
			main_mc.page_txt.text = GameCommonData.wordDic[ "mod_her_med_lea_vie_1" ] + (currentPage+1) + "/" + totalPage + GameCommonData.wordDic[ "mod_her_med_lea_vie_2" ];//"第"	"页"
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
			}
			else
			{
				return;
			}
			viewCurrentPage();
		}
		
		private function goRight(evt:MouseEvent):void
		{
			if((currentPage+1) < totalPage)
			{
				currentPage ++;
			}
			else
			{
				return;
			}
			viewCurrentPage();
		}
		
		private function clearContainer():void
		{
			if(cellContainer && cellContainer.numChildren>0)
			{
				for(var i:int = cellContainer.numChildren-1;i >= 0;i--)
				{
					var destroy:* = cellContainer.removeChildAt(0);
					if ( destroy is SkillCell )
					{
						destroy.removeEventListener( SkillConst.LIFECELL_CLICK,clickLifeCell );
						destroy = null;
					}
				}
			}
		}
		
		private function upDateMoney( obj:Object ):void
		{
			if ( dataProxy.liftLearnPanelIsOpen )
			{
				if ( obj.target == "mcBind" )
				{
					main_mc.mcUnBind.txtMoney.text = obj.money;
					ShowMoney.ShowIcon( main_mc.mcUnBind, main_mc.mcUnBind.txtMoney, true );
				}
				if ( obj.target == "mcUnBind" )
				{
					main_mc.mcBind.txtMoney.text = obj.money;
					ShowMoney.ShowIcon( main_mc.mcBind, main_mc.mcBind.txtMoney, true );
				}
			}
		}
		
		private function upDateExp():void
		{
			if ( dataProxy.liftLearnPanelIsOpen )
			{
				main_mc.hasExp_txt.text = GameCommonData.Player.Role.Exp;
			}
		}
		
		private function panelCloseHandler(evt:Event):void
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
			dataProxy.liftLearnPanelIsOpen = false;
			
			main_mc.left_btn.removeEventListener(MouseEvent.CLICK,goLeft);
			main_mc.right_btn.removeEventListener(MouseEvent.CLICK,goRight);
			main_mc.commit_btn.removeEventListener(MouseEvent.CLICK,learnLifeSkill);   
		}
		
	}
}