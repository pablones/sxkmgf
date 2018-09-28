package GameUI.Modules.NPCChat.Proxy
{
	import flash.utils.Dictionary;
	
	public class DialogConstData
	{	
		private static var _instance:DialogConstData;
		private var dic:Dictionary;
		private var newPersonTipDic:Dictionary;
		/** 锁定选中的Npc，如果为true就不发送请求NPC的消息  */
	    public var lockNpcSelected:Boolean=false;
	    
	    public static var finishTask:Array;
	    public static var unAcceptTask:Array;
		
		public function DialogConstData()
		{
			dic=new Dictionary();
			dic[1]="symbol_talk";
			dic[2]="symbol_finish";
			dic[3]="symbol_unFinish";
			dic[4]="symbol_unAccpet";
			dic[5]="symbol_transfer";
			dic[6]="symbol_shop";
			dic[7]="symbol_ask"; 
			newPersonTipDic=new Dictionary();                                                                                                 //event:1001,57,114,0,144
			newPersonTipDic[1]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_1" ] + '</font><font color="#0099ff"><a href="">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_3" ]+'</a></font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_4" ]+'</font><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_4_2" ]+'</font>'; 
													   // '欢迎来到《御剑江湖》。左键点地面可以移动。'                '左键单击'                                                                                                          '新手指引洛神'                                                                    '与其对话。'                                                                              '阅读完以上内容后，请点击按钮。'
			newPersonTipDic[2]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_6" ]+'</font><font color="#00ff00">2'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_7" ]+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_8" ]+'</font><font color="#0099ff"><a href="event:1001,78,90,0,116">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_9" ]+'</a></font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_10" ]+'</font><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_11" ]+'</font>';
													  //'恭喜你提升到了'                                                                   '级'                                                                           '。下面请单击新手指引'                                                                                             '洛神'                                                                                '吧，他会引导你开始游戏内容。'                                                      '小提示：按一下F12键可以屏蔽其他玩家'
			newPersonTipDic[3]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_6" ]+'</font><font color="#00ff00">3'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_7" ]+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_12" ]+'</font><font color="#0099ff"><a href="event:1001,78,90,0,116">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_9" ]+'</a></font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_13" ]+'</font>';
			                                          // '恭喜你提升到了'                                                                 '级'                                                                             '。现在可以继续找'                                                                                                '洛神'                                                                               '完成下面的任务，奖励会非常丰厚哦。'
			newPersonTipDic[4]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_6" ]+'</font><font color="#00ff00">5'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_7" ]+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_14" ]+'</font><font color="#0099ff"><a href="event:1001,78,90,0,116">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_9" ]+'</a></font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_15" ]+'</font>';
			                                            //'恭喜你提升到了'                                                                '级'                                                                             '。完成'                                                                                                         '洛神'                                                                              '所有的新手任务，将会获得完整的新手套装。'
			newPersonTipDic[5]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_6" ]+'</font><font color="#00ff00">6'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_7" ]+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_16" ]+'</font><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_17" ]+'</font><font color="#0099ff"><a href="event:1001,75,87,0,116">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_9" ]+'</a></font><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_18" ]+'</font>';
													  //'恭喜你提升到了'                                                                        '级'                                                                       '。现在你应该有了第一只宠物吧，有了宠物的帮助战斗将变得极为轻松。'                         '小提示：如果你还没有得到宠物，赶快去找'                                                                                  '洛神'                                                                            '完成新手系列任务吧。'
			newPersonTipDic[6]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_6" ]+'</font><font color="#00ff00">8'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_7" ]+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_19" ]+'</font><font color="#FF6532">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_20" ]+'</font><font color="#ffffff">。</font><br><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_21" ]+'</font>';
			                                            //'恭喜你提升到了'                                                                 '级'                                                                           '。下一个“一代名侠”任务可得到游戏世界里的'                                                 '上古神兵'                                                                                                       '小提示：达到10级可选择加入六大门派。'
			newPersonTipDic[7]='<font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_6" ]+'</font><font color="#00ff00">10'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_7" ]+'</font><font color="#ffffff">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_22" ]+'</font><br><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_23" ]+'</font>';
			                                            // '恭喜你提升到了'                                                                 '级'                                                                           '。完成“大侠之路”任务开始选择门派。'                                                         '小提示：加入门派即可升到12级。领取时装、装备、金钱。'
			newPersonTipDic[8]='<font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_24" ]+'</font>'; 
			                                            //'行走江湖怎能没有绝技傍身？提升好职业等级后快去找门派武功传授人学习技能吧！'
			newPersonTipDic[9]='<font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_25" ]+'</font><font color="#0099ff"><a href="event:1006,100,96,0,630">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_26" ]+'\\fx</a></font><font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_27" ]+'</font>';
			                                             //'江湖之大，无奇不有。如何在这江湖中扬名立万，坐拥一席之地？开封城的'                                                        '狄青'                                                                                   '会告诉你想要的答案。'
			newPersonTipDic[10]='<font color="#00ff00">'+GameCommonData.wordDic[ "mod_npcc_pro_dcd_28" ]+'</font>';
														//'杀死云雀后会有一定几率掉落野生云雀或云雀宝宝，现在需要耐心消灭一些云雀，然后捡取一只云雀宠物。'
		}
		
		public static function getInstance():DialogConstData{
			if(_instance==null)_instance=new DialogConstData();
			return _instance;
		}
		
		public function getSymbolName(type:uint):String{
			return dic[type];
		}
		public function getTipDesByType(type:uint):String{
			return newPersonTipDic[type];
		}
		
	}
}