package GameUI.Modules.HeroSkill.Command
{
	import flash.events.Event;

	public class SkillEvent extends Event
	{
		
		public var index:int;
		public function SkillEvent(type:String,index:int, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
			this.index=index;
		}
		
	}
}