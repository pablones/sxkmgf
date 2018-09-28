package GameUI.Modules.RoleProperty.Mediator.UI
{
	import GameUI.Modules.Designation.Data.DesignationProxy;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	
	import flash.display.MovieClip;
	import flash.text.TextField;
	
	public class OtherView
	{
		private var main_mc:MovieClip;
		private var designProxy:DesignationProxy;
		
		public function OtherView(_main_mc:MovieClip)
		{
			main_mc = _main_mc;
		}
		
		public function initUI():void
		{
			var unityName:String;
			//获取帮派名称
			if ( GameCommonData.Player.Role.unityId==0 )
			{
				unityName = "";
			}
			else
			{
//				unityName = UnityConstData.mainUnityDataObj.name; 
				unityName = NewUnityCommonData.myUnityInfo.name; 
			}
			
			//获取称号
			if ( !designProxy )
			{
				designProxy = UIFacade.GetInstance( UIFacade.FACADEKEY ).retrieveProxy( DesignationProxy.NAME ) as DesignationProxy;
			}

			( main_mc.heart_txt as TextField ).text 						= GameCommonData.Player.Role.Feel;					//心情
//			( main_mc.errantry_txt as TextField ).text 					= "0";				//江湖侠义值
			( main_mc.designation_txt as TextField ).text 			= designProxy.offerDName();		//称号
			( main_mc.honor_txt as TextField ).text 					= GameCommonData.Player.Role.arenaScore.toString();					//大宋荣誉
			
			( main_mc.unity_txt as TextField ).text 						= unityName;					//帮派
			( main_mc.unityAtt_txt as TextField ).text 					= String(GameCommonData.Player.Role.unityContribution);				//帮派贡献度
			( main_mc.master_txt as TextField ).text 					= "";				//师傅
			( main_mc.impart_txt as TextField ).text 					= "0";					//传道解惑值
			( main_mc.mate_txt as TextField ).text 						= "";					//配偶
//			( main_mc.mainSchool_txt as TextField ).text 			= "";		//主门派贡献
			( main_mc.mateDate_txt as TextField ).text 				= "";			//成亲日期
//			( main_mc.viceSchool_txt as TextField ).text 			= "";			//副门派贡献
			
			( main_mc.amok_txt as TextField ).text 					= GameCommonData.Player.Role.PkValue.toString();					//杀气度
//			( main_mc.doubleTime_txt as TextField ).text 			= "";		//冻结双倍时间
//			( main_mc.vipLevel_txt as TextField ).text 				= GameCommonData.Player.Role.VIP.toString();				//vip等级
			( main_mc.vipLevel_txt as TextField ).htmlText 				= UIUtils.getVipStr( GameCommonData.Player.Role.VIP );				//vip等级
//			( main_mc.vipTime_txt as TextField ).text 					= "";				//vip剩余时间
			( main_mc.bagLevel_txt as TextField ).text 				= ( int(GameCommonData.Player.Role.BagLevel / 10000) % 100 ).toString();			//背包等级
			( main_mc.stuffBagLevel_txt as TextField ).text 		= ( int(GameCommonData.Player.Role.BagLevel / 100)   % 100 ).toString();		//材料包等级
			( main_mc.ally_txt as TextField ).text 						= "";						//武林同盟
			( main_mc.active_txt as TextField ).text 					= "0";					//当前活跃度
			( main_mc.redName_txt as TextField ).text              = RolePropDatas.getDoubleTimeStr( GameCommonData.Player.Role.PkValue*1800 ).toString();  
			
			setEnabled();
		}
		
		public function upDateView(obj:Object):void
		{
			try
			{
				if ( obj.target == "designation_txt" )
				{
					( main_mc.designation_txt as TextField ).text 			= designProxy.offerDName();		//称号
				}
				else
				{
					( main_mc[obj.target] as TextField ).text = obj.value;	
				}
			}
			catch ( e:Error )
			{
				
			}
		}
		
		private function setEnabled():void
		{
//			( main_mc.heart_txt as TextField ).mouseEnabled 						= false;
//			( main_mc.errantry_txt as TextField ).mouseEnabled 					= false;
//			( main_mc.designation_txt as TextField ).mouseEnabled 			= false;
//			( main_mc.honor_txt as TextField ).mouseEnabled 					= false;
//			
//			( main_mc.unity_txt as TextField ).mouseEnabled 						= false;
//			( main_mc.unityAtt_txt as TextField ).mouseEnabled 					= false;
//			( main_mc.master_txt as TextField ).mouseEnabled 					= false;
//			( main_mc.impart_txt as TextField ).mouseEnabled 					= false;
//			( main_mc.mate_txt as TextField ).mouseEnabled 						= false;
//			( main_mc.mainSchool_txt as TextField ).mouseEnabled 			= false;
//			( main_mc.mateDate_txt as TextField ).mouseEnabled 				= false;
//			( main_mc.viceSchool_txt as TextField ).mouseEnabled 			= false;
//			
//			( main_mc.amok_txt as TextField ).mouseEnabled 					= false;
//			( main_mc.doubleTime_txt as TextField ).mouseEnabled 			= false;
//			( main_mc.vipLevel_txt as TextField ).mouseEnabled 				= false;
//			( main_mc.vipTime_txt as TextField ).mouseEnabled 					= false;
//			( main_mc.bagLevel_txt as TextField ).mouseEnabled 				= false;
//			( main_mc.stuffBagLevel_txt as TextField ).mouseEnabled 		= false;
//			( main_mc.ally_txt as TextField ).mouseEnabled 						= false;
//			( main_mc.active_txt as TextField ).mouseEnabled 					= false;
		}

	}
}