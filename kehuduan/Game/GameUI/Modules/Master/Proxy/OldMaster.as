package GameUI.Modules.Master.Proxy
{
	import GameUI.Modules.Master.Data.MasterData;
	
	public class OldMaster extends MentorRelation
	{
//		public var name:String;
//		public var id:int;
		public var sexIndex:int;			//0为男，1为女
//		public var roleLevel:int;			//等级
		public var job:int;						//门派
		public var batNum:int;				//战斗力
		public var impart:int;				//传授度
		public var vip:int;						//vip
		public var outLineTime:int;		//离线时长
		public var masterRate:int;		//师德
		public var awardMask:int;		//师傅奖励掩码
		public var line:int;					//在几线
		public var sortIndex:int;
		
		public var mainJob:int;
		public var mainJobLevel:int;
		public var viceJob:int;
		public var viceJobLevel:int;
		public var face:int;
		
		private var _vipStr:String;
		private var _sexStr:String;
		private var _outLineTimeStr:String;
		private var _lineStr:String;
		private var _impartLevelStr:String;
		
		//返回vip字符串
		public function get vipStr():String
		{
			switch ( vip )
			{
				case 0:
					_vipStr = "<font color='#FFFFFF'>"+GameCommonData.wordDic[ "often_used_none" ]+"</font>";      // 无
				break;
				case 1:
					_vipStr = "<font color='#0098FF'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_1" ]+"</font>";   // 月卡
				break;
				case 2:
					_vipStr = "<font color='#7a3fe9'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_2" ]+"</font>";    // 季卡
				break;
				case 3:
					_vipStr = "<font color='#FF6532'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_3" ]+"</font>";   // 半年卡
				break;
				case 4:
					_vipStr = "<font color='#00FF00'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_4" ]+"</font>";   // 周卡
				break;
				default:
				break;
			}
			return _vipStr;
		}
		
		//返回男女字符串
		public function get sexStr():String
		{
			if ( sexIndex == 0 )
			{
				_sexStr = GameCommonData.wordDic[ "mod_mas_pro_old_gets_1" ];			//男
			}
			else if ( sexIndex == 1 )
			{
				_sexStr = GameCommonData.wordDic[ "mod_mas_pro_old_gets_2" ];           //女
			}
			return _sexStr;
		}
		
		//返回线路str
		public function get lineStr():String
		{
			switch ( line )
			{
				case 0:
					_lineStr = "<font color = '#666666'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_1" ]+"</font>";    //离线
				break;
				case 1:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_2" ]+"</font>";    //一线
				break;
				case 2:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_3" ]+"</font>";    //二线
				break;
				case 3:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_4" ]+"</font>";    //三线
				break;
				case 4:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_5" ]+"</font>";    //四线
				break;
				case 5:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_6" ]+"</font>";    //五线
				break;
				case 6:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_7" ]+"</font>";    //六线
				break;
				case 7:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_8" ]+"</font>";    //七线
				break;
				default:			//防止意外
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_2" ]+"</font>";    //一线
				break;
			}
			return _lineStr;
		}
		
		//获取传授等级字符串
		public function get impartLevelStr():String
		{
			 this._impartLevelStr = MasterData.getPrenticeRemark( impart );
			 return _impartLevelStr;
		}
		
	}
}