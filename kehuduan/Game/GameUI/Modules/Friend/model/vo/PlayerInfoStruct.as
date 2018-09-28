package GameUI.Modules.Friend.model.vo
{
	public class PlayerInfoStruct
	{
		/** 好友ID */
		public var frendId:Number=0;
		/** 好友头像 */
 		public var face:Number=0;   
 		/** 功能号  */
 		public var ucAction:uint;
 		/** 是否在线 */
 		public var isOnline:uint;
 		/** 等级 */
 		public var level:uint;
 		/** 友好度 */
 		public var friendShip:uint;
 		/** 职业 */
 		public var profession:uint;
 		/** 性别*/
 		public var sex:uint;
		/** 分组编号 */
		public var friendGroupId:uint;
		/** 好友名字 */
		public var roleName:String;
		/** 帮派  */
		public var faction:uint;
		/** 好友心情  */
		public var feel:String;
		/** 关系  */
		public var relation:uint;
		/** 队伍Id*/
		public var idTeam:uint;
		/** 队长的ID */
		public var idTeamLeader:uint
		
		
		public function PlayerInfoStruct()
		{
			
			
		}

	}
}