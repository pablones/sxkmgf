package GameUI.Modules.RoleProperty.Mediator.RoleUtils
{
	import GameUI.ConstData.EventList;
	import GameUI.Modules.NewerHelp.Data.NewerHelpEvent;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Role.RoleJob;
	
	public class RoleLevUpMessage
	{
		public function RoleLevUpMessage()
		{
		}
		/** 发送系统消息 */
		public static function sendUpMessage(obj:Object):void
		{
			if(obj is RoleJob)  			//人物
			{
				switch(obj.Level)
				{
					case 25:
					case 30:
					case 35:
						GameCommonData.UIFacadeIntance.sendNotification(EventList.DO_FIRST_TIP, {comfrim:null, info:GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_1" ] + obj.Level +GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_2" ], title:GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_3" ], comfirmTxt:GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_4" ]});
																													//"<font color='#00FF00'>恭喜您的职业等级提升到了"			     //"级，请打开人物面板分配您的属性点。</font>"		              "小提示"				                                            "我知道了"
						GameCommonData.UIFacadeIntance.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_5" ] , content:GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_6" ]+ obj.Level +GameCommonData.wordDic[ "mod_rp_med_ru_rlum_sum_7" ]});
																											//"系统消息"			                                        "   恭喜您的职业等级已经达到"				                      "级，您已经拥有可自由分配的属性点。请鼠标点击屏幕下方的<font color='#00FF00'>人物面板</font>或按<font color='#00FF00'>快捷键C</font>进行查看并加点。职业等级每提升一级可拥有2个可分配的属性点。"
					break;
				}
			}
			else if(obj is GamePetRole)		//宠物
			{
				var lev:uint = obj.Level % 10;
				
				switch(lev)
				{
					
					case 0:			//10 20 30 ...
						GameCommonData.UIFacadeIntance.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 34);
						break;
					case 5:			//5  15  25 ...
						if(obj.Level != 5) {
							GameCommonData.UIFacadeIntance.sendNotification(NewerHelpEvent.SHOW_NEWER_HELP, 35);
						}
						break;
					
//					case 15:
//					case 20:
//					case 25:
//					case 30:
//						GameCommonData.UIFacadeIntance.sendNotification(EventList.DO_FIRST_TIP, {comfrim:null, info:"<font color='#00FF00'>恭喜您的宠物已经达到" + obj.Level +"级，请打开宠物面板分配宠物的属性点。</font>", title:"小提示", comfirmTxt:"关 闭"});
//						GameCommonData.UIFacadeIntance.sendNotification(EventList.SENDSYSTEMMESSAGE , {title:"系统消息" , content:"   恭喜您的宠物已经达到" + obj.Level + "级，您可以打开屏幕下方的<font color='#00FF00'>宠物面板</font>或按<font color='#00FF00'>快捷键X</font>进行查看并加点。宠物等级每提升一级可拥有5个可分配的属性点。"});
//					break;
				}
			}
//			switch(obj.JobId)
//			{
//				case 0:		//主职业
//					switch(obj.Level)
//					{
//						case 25:
//						break;
//						case 30:
//						break;
//						case 35:
//						break;
//					}
//				break;
//				case 1:     //副职业
//					switch(obj.Level)
//					{
//						case 25:
//						break;
//						case 30:
//						break;
//						case 35:
//						break;
//					}
//				break;
//			}
		}

	}
}