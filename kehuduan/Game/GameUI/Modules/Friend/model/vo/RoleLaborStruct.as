package GameUI.Modules.Friend.model.vo
{
	/**
	 *  玩家详细信息
	 * @author FELIX
	 * 
	 */	
	public class RoleLaborStruct
	{
		public var friendID:uint;
		public var face:uint;
		public var action:uint;
		public var subWork:uint;
		public var level:uint;
		public var relation:uint;
		public var friendShip:uint;
		public var profession:uint;
		public var professionLevel:uint;
		public var subWorkLevel:uint;
		public var roleName:String;
		public var feel:String;
		public var faction:String;
		public var title:String;
		public var mate:String;
		public var team:String;
		public var mapId:String;
			
		public function RoleLaborStruct()
		{
			
		}
		
		public function toString():void{
			trace(
			"friendID:"+friendID + 
			"face:"+face + 
			"action:"+action + 
			"subWork:"+subWork + 
			"level:"+level + 
			"relation:"+relation + 
			"friendShip:"+friendShip + 
			"profession:"+profession + 
			"professionLevel:"+professionLevel + 
			"subWorkLevel:"+subWorkLevel + 
			"roleName:"+roleName + 
			"feel:"+feel + 
			"faction:"+faction + 
			"title:"+title + 
			"mate:"+mate + 
			"team:"+team)
		}

	}
}