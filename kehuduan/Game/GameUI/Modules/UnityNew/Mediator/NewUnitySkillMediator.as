package GameUI.Modules.UnityNew.Mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.HeroSkill.SkillConst.SkillConst;
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	import GameUI.Modules.HeroSkill.View.NewUnityLearnSkillCell;
	import GameUI.Modules.RoleProperty.Mediator.RoleUtils.RoleLevUp;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.UIUtils;
	import GameUI.View.Components.YellowButton;
	import GameUI.View.items.MoneyItem;
	
	import OopsEngine.Skill.GameSkill;
	
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class NewUnitySkillMediator extends Mediator
	{
		public static const NAME:String = "NewUnitySkillMediator";
		
		private var parentContainer:Sprite;
		private var main_mc:MovieClip;
		private var aSkillObj:Array;
		private var openState:Boolean = false;
		
		private var container:Sprite;
		
		private var needMoneyItem:MoneyItem;				//所需
		private var hasBindMoneyItem:MoneyItem;		//碎银
		private var hasUnBindMoeyItem:MoneyItem;		//银两
		
		private var currentRemark:String;
		private var yellowFrame:Shape;
		private var needBangGong:int;
		private var hasBangGong:int;
		private var needBangGongTen:int;
		private var hasMoney:int;
		private var needMoney:int;
		private var needMoneyTen:int;
		
		private var chooseCell:NewUnityLearnSkillCell;
		private var learnOneBtn:YellowButton;
		private var learnTenBtn:YellowButton;
		
		private var skillCellDic:Dictionary;
		
		public function NewUnitySkillMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super( NAME, viewComponent );
		}
		
		public override function listNotificationInterests():Array
		{
			return [
							NewUnityCommonData.CHANG_NEW_UNITY_PAGE,
							NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL,
							EventList.UPDATEMONEY,
							SkillConst.UNITY_SKILL_UPDONE
						];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case NewUnityCommonData.CHANG_NEW_UNITY_PAGE:
					if ( !parentContainer )
					{
						parentContainer = notification.getBody() as Sprite;
					}
					if ( NewUnityCommonData.currentPage == 2 )
					{
						openMe();
					}
				break;
				case NewUnityCommonData.CLEAR_UNITY_LAST_OPEN_PANEL:
					if ( notification.getBody() == 2 )
					{
						clearMe();
					}
				break;
				case EventList.UPDATEMONEY:
					if ( openState )
					{
						updateMyMoney();
					}
				break;
				case SkillConst.UNITY_SKILL_UPDONE:
					if ( openState )
					{
						var id:int = notification.getBody().skillID;
						var le:int = notification.getBody().skillLevel;
						skillUpHandler( le,id );
					}
					RoleLevUp.PlayLevUp(GameCommonData.Player.Role.Id,true);
				break;
			}
		}
		
		private function initView():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.UnitySkillRes ) as MovieClip;
			container = new Sprite();
			container.x = 15;
			container.y = 61;
			main_mc.addChild( container );
			skillCellDic = new Dictionary();
			getSkillInfo();
			
			main_mc.hasBangGong_txt.mouseEnabled = false;
			main_mc.needBangGong_txt.mouseEnabled = false;
			main_mc.hasBangGong_txt.text = "";
			main_mc.needBangGong_txt.text = "";
			
			this.needMoneyItem = new MoneyItem();
			needMoneyItem.x = 322;
			needMoneyItem.y = 354;
			main_mc.addChild( needMoneyItem );
			this.hasBindMoneyItem = new MoneyItem();
			hasBindMoneyItem.x = 322;
			hasBindMoneyItem.y = 377;
			main_mc.addChild( hasBindMoneyItem );
			this.hasUnBindMoeyItem = new MoneyItem();
			hasUnBindMoeyItem.x = 322;
			hasUnBindMoeyItem.y = 400;
			main_mc.addChild( hasUnBindMoeyItem );
			
			learnOneBtn = new YellowButton();
			learnTenBtn = new YellowButton();
			
			yellowFrame = UIUtils.createFrame( 185,46,0xffff00 );
		}
		
		private function openMe():void
		{
			if ( !main_mc )
			{
				initView();
			}
			
			learnOneBtn.init();
			learnOneBtn.text = GameCommonData.wordDic[ "mod_unityN_med_newus_ope_1" ];     //学 1 级
			learnOneBtn.name = "1";
			learnOneBtn.textColor = 0x00ff00;
			learnOneBtn.x = 515;
			learnOneBtn.y = 361;
			learnOneBtn.isSee = false;
			main_mc.addChild( learnOneBtn );
			
			learnTenBtn.init();
			learnTenBtn.text = GameCommonData.wordDic[ "mod_unityN_med_newus_ope_2" ];    //学 10 级
			learnTenBtn.name = "10";
			learnTenBtn.textColor = 0x00ff00;
			learnTenBtn.x = 515;
			learnTenBtn.y = 391;
			learnTenBtn.isSee = false;
			main_mc.addChild( learnTenBtn );
			learnTenBtn.addEventListener( MouseEvent.CLICK,learnSkillHandler );
			learnOneBtn.addEventListener( MouseEvent.CLICK,learnSkillHandler );
			
			initData();
			updataTxts();
			updateMyMoney();
			parentContainer.addChildAt( main_mc,0 );
			upDataMe();
			openState = true;
			
			( main_mc.needSuiMoney_txt as TextField ).htmlText = GameCommonData.wordDic[ "mod_her_med_lea_initT" ] + SkillData.addVipTail(GameCommonData.Player.Role.VIP) +"：";//"所需碎银"
			( main_mc.needSuiMoney_txt as TextField ).mouseEnabled = false;
			
			main_mc.hasBangGong_txt.textColor = 0xe2cca5;
			hasBindMoneyItem.textColor = 0xe2cca5;
			hasUnBindMoeyItem.textColor = 0xe2cca5;
		}
		
		private function initData():void
		{
			hasMoney = GameCommonData.Player.Role.BindMoney + GameCommonData.Player.Role.UnBindMoney;
			hasBangGong = GameCommonData.Player.Role.unityContribution;
		
			this.needBangGong = 0;
			this.needBangGongTen = 0;
			this.needMoney = 0;
			this.needMoneyTen = 0;
		}
		
		//获取所有技能信息
		private function getSkillInfo():void
		{
			this.aSkillObj = [];
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
			aSkillObj.sortOn( "id",Array.NUMERIC );
		}
		
		private function upDataMe():void
		{
			var cell:NewUnityLearnSkillCell;
			var row:int;
			var col:int;
			skillCellDic = new Dictionary();
			
			for ( var i:uint=0; i<aSkillObj.length; i++ )
			{
				cell = new NewUnityLearnSkillCell( aSkillObj[ i ].id );
				cell.init();
				cell.clickSkillCell = clickSkillCell;
				row = Math.ceil( i % 5 );
				col = Math.ceil( (i+1) / 5 );
				cell.x = (col-1) * 192;
				cell.y = ( row ) * 50;
				this.container.addChild( cell );	
				skillCellDic[ aSkillObj[ i ].id ] = cell;
			}
		}
		
		private function clickSkillCell( cell:NewUnityLearnSkillCell ):void
		{
			if ( !cell.contains( yellowFrame ) )
			{
				cell.addChild( yellowFrame );
			}
			this.main_mc.remark_txt.text = cell.getRemark( cell.level );
			chooseCell = cell;
			countMoney( cell.id,cell.level );
			updataTxts();
			setLearnButton();
		}
		
		//算钱
		private function countMoney( _id:uint,_level:uint ):void
		{
			needBangGongTen = 0;
			needMoneyTen = 0;
			var Exp:uint = ( GameCommonData.SkillList[ _id ] as GameSkill ).Exp;
			var exp:uint = UIConstData.ExpDic[7001+_level] ;							//所需经验
//			needBangGong = GetExp( exp,_level,Exp );
			needBangGong = Math.floor( GetExp( exp,_level,Exp )/10 );
			
			var mExp:uint = UIConstData.ExpDic[5001+_level] ;						//所需钱
//			needMoney = Math.floor( GetExp(mExp,_level,Exp)* ( 1-GameCommonData.Player.Role.VIP*0.05 ) );
			var vip:int = GameCommonData.Player.Role.VIP;
			if ( vip == 4 ) vip = 1;
			needMoney = Math.floor( GetExp(mExp,_level,Exp) / 10 *  ( 1-vip*0.05 ) );
			
			for ( var i:uint=0; i<10; i++ )
			{
				needBangGongTen += Math.floor( GetExp( UIConstData.ExpDic[7001+_level+i],_level+11,Exp )/10 );
				needMoneyTen += Math.floor( GetExp(UIConstData.ExpDic[5001+_level+i] ,_level+11,Exp) /10 );
			}
			needMoneyTen *=  ( 1-GameCommonData.Player.Role.VIP*0.05 );
		}
		
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
		
		//更新所有的文本
		private function updataTxts():void
		{
			main_mc.hasBangGong_txt.text = hasBangGong;
			main_mc.needBangGong_txt.text = needBangGong;
			this.needMoneyItem.update( UIUtils.getMoneyStandInfo( this.needMoney, ["\\se","\\ss","\\sc"] ) );
		}
		
		//更新我自己的钱
		private function updateMyMoney():void
		{
			this.hasBindMoneyItem.update( UIUtils.getMoneyStandInfo( GameCommonData.Player.Role.UnBindMoney, ["\\se","\\ss","\\sc"] ) );
			this.hasUnBindMoeyItem.update( UIUtils.getMoneyStandInfo( GameCommonData.Player.Role.BindMoney, ["\\ce","\\cs","\\cc"] ) );
		}
		
		//设置学习按钮
		private function setLearnButton():void
		{
			if ( !chooseCell.canPromt )
			{
				learnOneBtn.isSee = false;
				learnTenBtn.isSee = false;
//				learnOneBtn.mouseEnabled = false;
//				learnTenBtn.mouseEnabled = false;
				return;	
			}
			
			var moneyBool:Boolean = isMoneyEn();
			var bangGongBool:Boolean = isBangGongEn();
			
			if ( moneyBool && bangGongBool )
			{
				learnOneBtn.isSee = true;	
//				learnOneBtn.mouseEnabled = false;
				if ( hasBangGong>=needBangGongTen && hasMoney>=needMoneyTen && chooseCell.canPromt_ten )
				{
					learnTenBtn.isSee = true;
//					learnOneBtn.mouseEnabled = true;
				}
				else
				{
					learnTenBtn.isSee = false;
//					learnOneBtn.mouseEnabled = false;
				}
			}
			else
			{
				learnOneBtn.isSee = false;
				learnTenBtn.isSee = false;	
//				learnOneBtn.mouseEnabled = false;
//				learnTenBtn.mouseEnabled = false;
			}
		}
		
		//帮贡是否足够
		private function isBangGongEn():Boolean
		{
			if ( hasBangGong < needBangGong )
			{
				main_mc.hasBangGong_txt.textColor = 0xff0000;
				return false;
			}	
			else
			{
				main_mc.hasBangGong_txt.textColor = 0xe2cca5;
				return true;
			}
		}
		
		//钱是否够
		private function isMoneyEn():Boolean
		{
			if ( hasMoney < needMoney )
			{
				hasBindMoneyItem.textColor = 0xff0000;
				hasUnBindMoeyItem.textColor = 0xff0000;
				return false;
			}
			else
			{
				hasBindMoneyItem.textColor = 0xe2cca5;
				hasUnBindMoeyItem.textColor = 0xe2cca5;
				return true;
			}
		}
		
		private function learnSkillHandler( evt:MouseEvent ):void
		{
			if ( NewUnityCommonData.myUnityInfo.isStop )
			{
				var times:int = int( evt.currentTarget.name );
				sendNotification( SkillConst.LEARN_SKILL_SEND,{ SkillId:chooseCell.id,SkillLevel:chooseCell.level,times:times } );	
			}
			else
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_med_newus_lea_1" ] );     //帮派暂停维护中，不能学习技能
			}
		}
		
		private function clearContainer():void
		{
			var des:Object;
			while ( container.numChildren>0 )
			{
				des = container.removeChildAt( 0 );
				if ( des is NewUnityLearnSkillCell )
				{
					( des as NewUnityLearnSkillCell ).gc();
				}
				des = null;
			}
		}
		
		//升级成功
		private function skillUpHandler( level:int,id:int ):void
		{
			initData();
			countMoney( id,level );
			updataTxts();
			refreshCell( level,id );
			if ( chooseCell )
			{
				setLearnButton();
			}
		}
		
		private function refreshCell( newLevel:uint,newId:uint ):void
		{
			var cell:NewUnityLearnSkillCell = skillCellDic[ newId ];
			cell.skillUpDone( newLevel );
			this.main_mc.remark_txt.text = cell.getRemark( newLevel );
		}
		
		private function clearMe():void
		{
			if ( main_mc && parentContainer.contains( main_mc ) )
			{
				skillCellDic = null;
				clearContainer();
				learnOneBtn.gc();
				learnTenBtn.gc();
				this.main_mc.remark_txt.text = "";
				learnTenBtn.removeEventListener( MouseEvent.CLICK,learnSkillHandler );
				learnOneBtn.removeEventListener( MouseEvent.CLICK,learnSkillHandler );
				parentContainer.removeChild( main_mc );
			}
			openState = false;
		}
		
	}
}