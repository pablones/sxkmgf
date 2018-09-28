package GameUI.Modules.Master.Proxy
{
	//接收服务器乱七八糟的师徒关系类
	public class ServerMasterRelation
	{
		public var id:int;
		public var mainJob:int;
		public var viceJob:int;
		public var mainJobLevel:int;
		public var viceJobLevel:int;
		public var roleLevel:int;
		public var face:int;		//头像
		public var chuanShouDu:int;		//传授度
		public var vip:int;
		public var hasTeam:int;			//是否组队
		public var line:int;
		public var relation:int;				//1师傅  2徒弟  3自己
		public var name:String;
	}
}