package GameUI.Modules.UnityNew.Proxy
{
	import GameUI.Modules.Unity.Data.UnityJopChange;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	
	public class UnityBaseInfo
	{
		public var id:int;
		public var name:String;
		public var boss:String;
		public var viceBoss1:String;
		public var viceBoss2:String;
		public var level:int;
		public var onLinePeople:int;			//在线人数
		public var currentPeople:int;
		public var money:int;						//帮派资金
		public var jianShe:int;
		public var notice:String;
		public var unityJob:int;
		public var welfare:Array = [];				//福利
		public var leftBangGong:int;			//剩余帮贡
		public var historyBangGong:int;		//历史帮贡
		public var createTime:int;				//创建时间 
		public var stopState:int;
		public var maxMoneyArr:Array = [ 20000000,50000000,100000000,200000000,500000000,600000000,700000000,800000000,900000000,1000000000 ];		//帮派资金上限 
		
		public function get maxPeople():int
		{
			return UnityNumTopChange.change( level );
		}
		
		public function get unityJobStr():String
		{
			return UnityJopChange.change( unityJob );
		}
		
		public function get weiHuStr():String
		{
			var scale:Number = money / maxMoneyArr[ level-1 ];
			var str:String
			if ( scale>0.7 )
			{
				str = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_unityN_pro_unib_wei_1" ]+"</font>";    //高维护
			}
			else if ( scale>=0.2 && scale<=0.7 )
			{
				str = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_unityN_pro_unib_wei_2" ]+"</font>";    //正  常
			}
			else if ( scale<0.2 && scale>=0.05 )
			{
				str = "<font color='#00ff00'>"+GameCommonData.wordDic[ "mod_unityN_pro_unib_wei_3" ]+"</font>";    //低维护
			}
			else
			{
				str = "<font color='#ff0000'>"+GameCommonData.wordDic[ "mod_unityN_pro_unib_wei_4" ]+"</font>";     //暂  停
			}
			return str;
		}
		
		public function get isStop():Boolean
		{
			var scale:Number = money / maxMoneyArr[ level-1 ];
			if ( scale<0.05 )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
}