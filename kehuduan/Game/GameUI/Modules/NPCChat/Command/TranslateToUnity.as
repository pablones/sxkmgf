package GameUI.Modules.NPCChat.Command
{
	import GameUI.ConstData.EventList;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;


	/**
	 *  传送到某个门派命令
	 * @author felix
	 * 
	 */	
	public class TranslateToUnity extends SimpleCommand
	{
		public function TranslateToUnity()
		{
			super();
		}
		
		public override function execute(notification:INotification):void{
			var index:uint=uint(notification.getBody())
			var arr:Array=[
			{},
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_1" ],area:GameCommonData.wordDic[ "often_used_qz" ]},      //"柳沧樱"       "点苍"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_3" ],area:GameCommonData.wordDic[ "often_used_dc" ]},     //"王重阳"       "全真"           
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_5" ],area:GameCommonData.wordDic[ "often_used_em" ]},     //"鸿陵"        "峨眉"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_6" ],area:GameCommonData.wordDic[ "often_used_tm" ]},     //"唐妍"       "唐门"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_2" ],area:GameCommonData.wordDic[ "often_used_gb" ]},     //"金不换"      "丐帮"    
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_4" ],area:GameCommonData.wordDic[ "often_used_sl" ]}      //"玄慈"    "少林"  
			];
			sendNotification(EventList.DO_FIRST_TIP, {comfrim:translate, info:"<font color='#ffffff'>" + GameCommonData.wordDic[ "mod_npcc_cmd_ttu_7" ] + "</font><font color='#e46d0a'>"+arr[index].name+"</font><font color='#ffffff'>" + GameCommonData.wordDic[ "mod_npcc_cmd_ttu_8" ]+"？" + "</font><br><font color='#00ff00'>" + "（"+GameCommonData.wordDic[ "mod_npcc_cmd_ttu_9" ]+"："+GameCommonData.wordDic[ "mod_npcc_cmd_ttu_10" ] +arr[index].name+GameCommonData.wordDic[ "mod_npcc_cmd_ttu_11" ] + "</font><font color='#0099ff'>"+arr[index].name+GameCommonData.wordDic[ "mod_npcc_cmd_ttu_12" ]+arr[index].area+"</font><font color='#00ff00'>" +"拜师就可以成为"+arr[index].name+GameCommonData.wordDic[ "mod_npcc_cmd_ttu_13" ]+"。）</font>", title:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_9" ], comfirmTxt:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_14" ] ,params:(index+2000) });
																									 //"你是否决定前去"																						                                "拜师"													                                         "小提示"		                                         "传送至"                                                           "后找"                                                                                            "掌门"                                                                                          "拜师就可以成为"                  "弟子"                                                                 "小提示"                                                   "我知道了"
		}
		
		private function translate(type:uint):void{
			sendNotification(NPCChatComList.SEND_NPC_MSG,type);
		}
		
		
	}
}