package GameUI.Modules.PrepaidLevel.Data
{
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class PrepaidDataProxy extends Proxy
	{
		public static const NAME:String = "prepaidDataProxy";
		
		public function PrepaidDataProxy(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
		}
		
		public var needPrepaid:uint;       //升级还需要的元宝
		
		public var upLevelPrepaid:uint;     //升级总共需要的元宝
		
		public var allPrepaid:uint;         //累计充值
		
		public var hasPrepaid:Boolean;       //是否有充值
		
		public var fugueNeedMoney:uint;    //神游千里所需元宝
		
		public var canYouLiCount:uint;     //总共可以游历次数
		
		public var usedYouLiCount:uint     //可以游历次数
		
		public var questionIndex:int;      //问题代号
		
		public var selectAnswer:uint;          //选择的答案
		
		public var getGiftArr:Array;          //可以领取的礼包
		
		public var canGetZhuBao:uint;         //是否可以领取珠宝返还
		
		public var giftType:Array = [500110, 500111, 500112, 500113, 500114, 500115, 500116, 500117, 500118, 500119];            //每种礼包的Type
		
		public var btnArr:Array;             //显示的主按钮
		
		public var unShowBtnArr:Array;       //不显示的主按钮
		
		public var btnTxtArr:Array;          //显示的主按钮文本
		
		public var unShowTxtArr:Array;       //不显示的主按钮文本
		
		public var currrentBtn:String = "prepaid_btn";     //当前按钮
		
		public var prepaidLevel:uint;      //充值等级
		
		public var currentPage:uint;       //当前页数
		
		public var prepaidIsOpen:Boolean = false;      //充值等级面板是否打开
		
		public var xiaoyaoIsOpen:uint = 0;      //逍遥丹快速购买面板是否打开       0为没打开      1为一个逍遥丹       2为两个逍遥丹
		
		public var wishState:uint;              //0为可以许愿     1为可以领取回礼     2为今天已经许愿过了
		
		public var isWait:Boolean;              //是否等待服务器返回
		
		public var clickBtn:String = "";            //游历点击的按钮
		
		public var giftPage:uint;                //领取礼包时的页数
		
		public function updatePrepaid( obj:Object ):void
		{
			needPrepaid = obj.needPrepaid as uint;
			allPrepaid = obj.allPrepaid as uint;
			upLevelPrepaid = obj.upLevelPrepaid as uint;
			prepaidLevel = obj.prepaidLevel as uint;
			getGiftArr = obj.arr as Array;
			canGetZhuBao = obj.canGetZhuBao as uint;
			if( prepaidLevel > 1 )
			{
				GameCommonData.couponsCanOpen = true;
			}
			else
			{
				GameCommonData.couponsCanOpen = false;
			}
		}
		
		public function updateTravel( obj:Object ):void
		{
			usedYouLiCount = obj.usedYouLiCount as uint;
			canYouLiCount = obj.canYouLiCount as uint;
			fugueNeedMoney = obj.fugueNeedMoney as uint;
			questionIndex = obj.questionIndex as int;
			this.selectAnswer = 0;
		}
	}
}