package GameUI.Modules.HeroSkill.Command
{
	import flash.events.EventDispatcher;
	
	public class SkillDispatch extends EventDispatcher
	{
		public static var instance:SkillDispatch;
		
		public static function getInstance():SkillDispatch
		{
			if(!instance)
			{
				instance = new SkillDispatch();
			}
			return instance;
		}
		public function SkillDispatch()
		{
		}

	}
}