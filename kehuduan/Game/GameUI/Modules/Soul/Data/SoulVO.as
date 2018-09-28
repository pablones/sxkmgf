package GameUI.Modules.Soul.Data
{
	public class SoulVO
	{
		/**物品ID*/
		public var id:uint;
		/**魂魄ID*/
		public var soulId:uint;
		/**(2 灵力(九阴之魂)  1 力量(九阳之魄)*/
		public var belong:int;
		/**属相(0 没有 1 地 2 水 3 火 4 风)*/
		public var style:int;
		/**成长率*/
		public var growPercent:int;
		/**合成等级*/
		public var composeLevel:int;
		/**魂魄名*/
		public var name:String = "";
		/**等级*/
		public var level:int;
		/**当前经验经验 */
		public var exp:int;
		/**寿命*/
		public var life:int;
		/**外功*/
		public var phyAttack:int;
		/**内功*/
		public var magAttack:int;
		/**力量*/
		public var force:int; 
		/**体力*/ 
		public var physical:int;
		/**定力*/
		public var constant:int;
		/**灵气*/
		public var spirit:int;
		/**身法*/
		public var magic:int;
		
		/**扩展属性*/
		public var extProperties:Array = [false,false,false,false,false,false,false,false,false,false];
		
		/**魂魄技能*/
		public var soulSkills:Array = [false,false,false];
	}
}