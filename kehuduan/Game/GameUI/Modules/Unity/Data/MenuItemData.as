package GameUI.Modules.Unity.Data
{
	public class MenuItemData
	{
		public function MenuItemData()
		{
		}
		public static var dataArr:Array = [];
//		dataArr.push({cellText:"发送消息",data:{type:"发送消息"}});
		dataArr.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ]}});  // 设为私聊  设为私聊
		dataArr.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ],data:{type:GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ] }});  // 组    队  组    队
		dataArr.push({cellText:GameCommonData.wordDic[ "mod_uni_dat_men_men_1" ],data:{type:GameCommonData.wordDic[ "mod_uni_dat_men_men_1" ]}});  // 复制名字   复制名字
		dataArr.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_1" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_1" ]}});  //  查看资料  查看资料
//		dataArr.push({cellText:"帮派任命",data:{type:"帮派任命"}});
//		dataArr.push({cellText:"踢出帮派",data:{type:"踢出帮派"}});
		public static function menuDataInit():void
		{
			dataArr = [];
//			dataArr.push({cellText:"发送消息",data:{type:"发送消息"}});
			dataArr.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ]}});  //  设为私聊  设为私聊
			dataArr.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ],data:{type:GameCommonData.wordDic[ "mod_chat_med_qui_model_6" ]}});  //  组    队  组    队
			dataArr.push({cellText:GameCommonData.wordDic[ "mod_uni_dat_men_men_1" ],data:{type:GameCommonData.wordDic[ "mod_uni_dat_men_men_1" ] }});  //  复制名字  复制名字
			dataArr.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_1" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_1" ]}});  //  查看资料  查看资料
			dataArr.push({cellText:GameCommonData.wordDic[ "mod_pla_med_tea_onc_4" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_tea_onc_4" ] }});  //  加为好友  加为好友
		}
	}
}