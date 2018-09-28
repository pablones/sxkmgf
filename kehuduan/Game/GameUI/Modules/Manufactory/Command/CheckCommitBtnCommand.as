package GameUI.Modules.Manufactory.Command
{
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Manufactory.Data.ManufactoryData;
	import GameUI.Modules.Manufactory.Mediator.ManufactoryMediator;
	import GameUI.Modules.Manufactory.Proxy.ManufatoryProxy;
	
	import OopsEngine.Skill.GameSkillLevel;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class CheckCommitBtnCommand extends SimpleCommand
	{
		public static const NAME:String = "CheckCommitBtnCommand";
		private var manuProxy:ManufatoryProxy;
		private var manuMediator:ManufactoryMediator;
		
		//三个打造技能的type
		private var stockSkillType:uint;
		private var leatherSkillType:uint;
		private var refinementSkillType:uint;
		
		public function CheckCommitBtnCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			manuProxy = facade.retrieveProxy( ManufatoryProxy.NAME ) as ManufatoryProxy;
			manuMediator = facade.retrieveMediator( ManufactoryMediator.NAME ) as ManufactoryMediator;
			
			if ( (ManufactoryData.clickScenoType == 0) || (ManufactoryData.ManufatoryCount == 0) )
			{
				manuMediator.mainView.ctrlCommitBtn( false );
				return;
			}
				
//			var obj:Object = manuProxy.allInfoDic[ ManufactoryData.selectScenoType ];
			var obj:Object = manuProxy.allInfoDic[ ManufactoryData.clickScenoType ];
			if ( !obj ) return;
			
			if ( !checkPower(obj) )
			{
				manuMediator.mainView.ctrlCommitBtn( false );
				return;
			}
			
//			if ( !checkSkillLevel(obj) )
//			{
//				manuMediator.mainView.ctrlCommitBtn( false );
//				return;
//			}
			
			if ( !checkMaterialNum(obj) )
			{
				manuMediator.mainView.ctrlCommitBtn( false );
				return;
			}
			
			manuMediator.mainView.ctrlCommitBtn( true );
		}
	
		//所需物品是否足够
		private function checkMaterialNum(obj:Object):Boolean
		{
			var bArr:Array = obj.needBase as Array;
			for ( var i:uint=0; i<bArr.length; i++ )
			{
				var hasNum:uint = BagData.hasItemNum( bArr[i].type );
				var needNum:uint = ManufactoryData.ManufatoryCount*bArr[i].num;
				if ( hasNum<needNum )
				{
					return false;
				}
			}
			
			if ( ManufactoryData.clickAppendType != 0 )
			{
				var hasAppendNum:uint = BagData.hasItemNum( ManufactoryData.clickAppendType );
				var needAppendNum:uint = ManufactoryData.ManufatoryCount;
				if ( hasAppendNum<needAppendNum )
				{
					return false;
				}
			}
			return true;
		}
		
		//活力是否足够
		private function checkPower(obj:Object):Boolean
		{
			manuMediator.mainView.main_mc.power_txt.text = obj.power*ManufactoryData.ManufatoryCount+"/"+GameCommonData.Player.Role.Ene;
			if ( GameCommonData.Player.Role.Ene<obj.power*ManufactoryData.ManufatoryCount )
			{
				manuMediator.mainView.main_mc.power_txt.textColor = 0xff0000;
				return false;
			}
			else
			{
				manuMediator.mainView.main_mc.power_txt.textColor = 0xffffff;
				return true;
			}
		}
		
		//技能等级是否足够
		private function checkSkillLevel(obj:Object):Boolean
		{
			if ( obj.classTpye == "stock" )
			{
				if ( !GameCommonData.Player.Role.LifeSkillList[ stockSkillType ] )
				{
					return false;
				}
				else
				{
					var sLevel:uint = ( GameCommonData.Player.Role.LifeSkillList[ stockSkillType ] as GameSkillLevel ).Level;
					if ( obj.kind == GameCommonData.wordDic[ "mod_man_com_che_che" ] )//"半成品"
					{
						if ( obj.level>sLevel )
						{
							return false;
						}
					}
					else
					{
						if ( obj.level>sLevel*10+30 )
						{
							return false;
						}
					}
				}
			}
			
			if ( obj.classTpye == "leather" )
			{
				if ( !GameCommonData.Player.Role.LifeSkillList[ leatherSkillType ] )
				{
					return false;
				}
				else
				{
					var leLevel:uint = ( GameCommonData.Player.Role.LifeSkillList[ leatherSkillType ] as GameSkillLevel ).Level;
					if ( obj.kind == GameCommonData.wordDic[ "mod_man_com_che_che" ] )//"半成品"
					{
						if ( obj.level>leLevel )
						{
							return false;
						}
					}
					else
					{
						if ( obj.level>leLevel*10+30 )
						{
							return false;
						}
					}
				}
			}
			
			if ( obj.classTpye == "refinement" )
			{
				if ( !GameCommonData.Player.Role.LifeSkillList[ refinementSkillType ] )
				{
					return false;
				}
				else
				{
					var reLevel:uint = ( GameCommonData.Player.Role.LifeSkillList[ refinementSkillType ] as GameSkillLevel ).Level;
					if ( obj.kind == GameCommonData.wordDic[ "mod_man_com_che_che" ] )//"半成品"
					{
						if ( obj.level>reLevel )
						{
							return false;
						}
					}
					else
					{
						if ( obj.level>reLevel*10+30 )
						{
							return false;
						}
					}
				}
			}
			
			return true;
		}
		
	}
}