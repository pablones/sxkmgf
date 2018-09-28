package GameUI.Modules.HeroSkill.Command
{
	import GameUI.Modules.HeroSkill.SkillConst.SkillData;
	
	import OopsEngine.Skill.GameSkillLevel;
	import OopsEngine.Skill.GameSkillMode;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class SkillIsAutoPlayCommand extends SimpleCommand
	{
		public static const NAME:String = "SkillIsAutoPlayCommand";
		private var aMainSkillObj:Array;
		private var aViceSkillObj:Array;
		
		public function SkillIsAutoPlayCommand()
		{
			super();
		}
		
		public override function execute(notification:INotification):void
		{
			aMainSkillObj = [];
			aViceSkillObj = [];
//			trace ( "mainJob:   " +GameCommonData.Player.Role.MainJob );
			for ( var key:* in GameCommonData.SkillList )
			{
				var obj:Object = GameCommonData.SkillList[key];
				var skillObj:Object = new Object();
				skillObj.id = obj.SkillID;
				skillObj.needLevel = obj.NeedLevel;
				skillObj.mode = obj.SkillMode;
				if ( obj.BookID==0 ) 
				{
					 if ( obj.Job == GameCommonData.Player.Role.MainJob.Job )
					{
						aMainSkillObj.push( skillObj );
					}
					else if ( obj.Job == GameCommonData.Player.Role.ViceJob.Job )
					{
						aViceSkillObj.push( skillObj );
					}
				}
			}
			aMainSkillObj.sortOn("needLevel",Array.NUMERIC);
			aViceSkillObj.sortOn("needLevel",Array.NUMERIC);
			checkMainAutoPlay();
			checkViceAutoPlay();
		}		
		
		private function checkMainAutoPlay():void
		{
			for ( var i:uint=0; i<aMainSkillObj.length; i++ )
			{
				var id:int = aMainSkillObj[i].id;
				var mode:int = aMainSkillObj[i].mode;
				if ( !GameCommonData.Player.Role.SkillList[id] )
				{
					continue;
				}
				if ( SkillData.aMainAutoIndex[i] == 1 && GameSkillMode.IsPersonAutomatism( mode ) && mode != 0 )
				{
					( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel ).IsAutomatism = true;
				}
				else
				{
					( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel ).IsAutomatism = false;
				}
			}
		}
		
		private function checkViceAutoPlay():void
		{
			for ( var i:uint=0; i<aViceSkillObj.length; i++ )
			{
				var id:int = aViceSkillObj[i].id;
				var mode:int = aViceSkillObj[i].mode;
				if ( !GameCommonData.Player.Role.SkillList[id] )
				{
					continue;
				}
				if ( SkillData.aViceAutoIndex[i] == 1 && GameSkillMode.IsPersonAutomatism( mode ) && mode != 0 )
				{
					( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel ).IsAutomatism = true;
				}
				else
				{
						( GameCommonData.Player.Role.SkillList[id] as GameSkillLevel ).IsAutomatism = false;
				}
			}
		}
	}
}