package GameUI.Modules.UnityNew.Proxy
{
	import GameUI.UIUtils;
	
	public class UnityMemberVo
	{
		public var id:int;
		public var name:String;
		public var unityJob:int;
		public var roleLevel:int;				//人物等级
		public var sex:int;
		public var mainJob:int;
		public var lastLoginTime:int;
		public var line:int;						//所在线路
		public var totalContribute:int;		//总贡献
		public var jianseContribute:int;	//建设贡献
		public var moneyContribute:int;	//资金贡献
		public var fighting:int;					//战斗力
		public var vip:int;
		
		public function get sexStr():String
		{
			return UIUtils.getSexStr( this.sex );
		}
		
		public function get lineStr():String
		{
			return UIUtils.getLineStr( this.line );
		}
		
		public function get mainJobStr():String
		{
			return GameCommonData.RolesListDic[ this.mainJob ].toString();
		}
		
		public function get unityJobStr():String
		{
			var str:String;
			switch ( unityJob )
			{
				case 100:
					str = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_1" ];     //帮主
				break;
				case 90:
					str = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_2" ];    //副帮主
				break;
				case 85:
					str = GameCommonData.wordDic[ "mod_uni_dat_uni_ord_3" ];      //精英
				break;
				default:
					str = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_20" ];      //帮众
				break;
			}
			return str;
		}
		
		public function get lastLoginTimeStr():String
		{
			var startStr:String = lastLoginTime.toString();
			var s1:String = startStr.slice( 0,4 );
			var s2:String = startStr.slice( 4,6 );
			var s3:String = startStr.slice( -2 );
			return s1+"-"+s2+"-"+s3;
		}
	}
}