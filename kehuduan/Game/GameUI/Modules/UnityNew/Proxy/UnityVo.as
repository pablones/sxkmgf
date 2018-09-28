package GameUI.Modules.UnityNew.Proxy
{
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	
	public class UnityVo
	{
		public var id:uint;
		public var rank:int;			//排名
		public var name:String;
		public var boss:String;
		public var level:int;
		public var currentPeople:int;
//		public var maxPeople:int;
		
		public function get maxPeople():int
		{
			return UnityNumTopChange.change( level );
		}
	}
}