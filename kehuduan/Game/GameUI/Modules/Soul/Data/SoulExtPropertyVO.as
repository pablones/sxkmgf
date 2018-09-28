package GameUI.Modules.Soul.Data
{
	public class SoulExtPropertyVO
	{	
		/**技能编号*/
		public var sId:int;
		/**扩展编号*/
		public var number:int;
		/**当前扩展槽状态 0 已使用 1 未学习 2 可开槽*/
		public var state:int;
		/**属性名称*/
		public var name:String = "";
		/**属性等级*/
		public var level:int;
		/**属性加成*/
		public var addProperty:int;
	}
}