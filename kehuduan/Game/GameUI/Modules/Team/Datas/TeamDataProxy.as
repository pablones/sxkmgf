package GameUI.Modules.Team.Datas
{
	
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class TeamDataProxy extends Proxy
	{
		public static const NAME:String = "TeamDataProxy";
		
		public function TeamDataProxy()
		{
			super(NAME, data);
		}
		
		public var invitePageCount:int = 0;					/** 邀请队伍页码总数 */
		
		public var curInvitePage:int = 0;					/** 当前邀请队伍信息 页码 */
		
		public var inviteIdList:Array = [];					/** 邀请的不同队伍的ID列表 */
		
		public var isInviting:Boolean = false;				/** 是否正在被邀请 */
		
		public var inviteIsOpen:Boolean = false;			/** 邀请队伍信息面板是否打开 */
		
		public var invitePerson:Object = new Object();		/** 邀请人信息 */
		
		public var inviteMemList:Array = [];				/** 当前邀请我的队伍成员列表 */
		
		public var inviteMemItemList:Array = [];			/** 邀请 组件列表 */
		
		public var teamMemItemList:Array = [];				/** 队伍成员列表组件 */
		
		public var teamReqItemList:Array = [];				/** 申请列表组件 */
		
		public var teamMemberList:Array = [];				/** 队伍成员列表 */
		
		public var teamReqList:Array = [];					/** 请求加入成员列表  队列----处理就删除掉 */
		
		public var teamMemSelected:Object = new Object();	/** 当前选中的队员 */
		
		public var teamReqSelected:Object = new Object();	/** 当前选中的请求人员 */
		/** 队伍坐标信息*/
		public static var teamDataDic:Dictionary=new Dictionary(true);
		
	}
}