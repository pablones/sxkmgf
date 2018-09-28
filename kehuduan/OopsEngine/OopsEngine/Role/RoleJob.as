package OopsEngine.Role
{
	public class RoleJob
	{
		/** 职业 */
		public var Job:uint;
		/** 职业号 0:主职业 1:副职业 */
		public var JobId:uint;
		/** 等级 */
		public var Level:uint;
		/** 潜力 */
		public var Potential:int;
		/** 物攻 */
		public var PhyAttack:uint;
		/** 经验 */
		public var Exp:uint
		/** 魔攻 */
		public var MagicAttack:uint;
		/** 物防 */
		public var PhyDef:uint;
		/** 魔防 */
		public var MagicDef:uint;
		/** 命中 */
		public var Hit:uint;
		/** 躲避 */
		public var Dodge:uint;
		/** 暴击 */
		public var Crit:uint;
		/** 韧性 */
		public var Toughness:uint;
		/** 力量 */
		public var Force:uint;
		/** 灵力 */
		public var SpiritPower:uint;
		/** 体力 */
		public var Physical:uint;
		/** 定力 */
		public var Constant:uint;
		/** 身法 */
		public var Magic:uint;
		/** 潜力点 */
		public var Points:Array = [0,0,0,0,0];
		/** 最大血量值*/
		public var MaxHp:uint;
		/** 最大MP*/
		public var MaxMp:uint;
		/** 最大Sp*/
		public var MaxSP:uint;
	}
}