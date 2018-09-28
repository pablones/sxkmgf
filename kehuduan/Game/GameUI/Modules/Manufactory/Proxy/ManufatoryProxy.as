package GameUI.Modules.Manufactory.Proxy
{
	import flash.display.MovieClip;
	import flash.utils.Dictionary;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class ManufatoryProxy extends Proxy
	{
		public static const NAME:String = "ManufatoryProxy";
		public var aItemLevel:Array = [ GameCommonData.wordDic[ "mod_man_com_cli_exe_2" ],"1-19","20-29","30-39","40-49","50-59","60-69","70-79","80-89","90-99","100-109" ];					//物品等级
		public var aStockType:Array = [ GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ],GameCommonData.wordDic[ "mod_man_pro_man_aStockType_1" ],GameCommonData.wordDic[ "mod_man_pro_man_aStockType_2" ],GameCommonData.wordDic[ "mod_man_pro_man_aStockType_3" ],GameCommonData.wordDic[ "mod_man_pro_man_aStockType_4" ],GameCommonData.wordDic[ "mod_man_pro_man_aStockType_5" ],GameCommonData.wordDic[ "mod_man_pro_man_aStockType_6" ],GameCommonData.wordDic[ "mod_man_com_che_che" ] ];
		//"物品种类"  "杖"	"剑"		"锤"		"扇"		"刀"		"弓"		"半成品"
		public var aLeatherType:Array = [ GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_1" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_2" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_3" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_4" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_5" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_6" ],GameCommonData.wordDic[ "mod_man_pro_man_aLeatherType_7" ],GameCommonData.wordDic[ "mod_man_com_che_che" ] ];
		//"物品种类"		"头盔"		"衣服"		"护肩"		"护腕"		"手套"		"腰带"		"鞋子"		"半成品"
		public var aRefinementType:Array = [ GameCommonData.wordDic[ "mod_man_com_cli_exe_1" ],GameCommonData.wordDic[ "mod_man_pro_man_aRefinementType_1" ],GameCommonData.wordDic[ "mod_man_pro_man_aRefinementType_2" ],GameCommonData.wordDic[ "mod_man_pro_man_aRefinementType_3" ],GameCommonData.wordDic[ "mod_man_med_man_com_3" ] ];
		//"物品种类"		"项链"		"戒指"	"饰品"		"半成品"
		public var defaultRate:Array = ["35%","40%","20%","5%","0%"];														//默认打造概率
		
		public var allInfoDic:Dictionary = new Dictionary();
		public var skirtListBack:MovieClip;													//下拉列表的背景框
		public var ratioCircle:Class;																//单选按钮
		public var readingBar:Class;															//黄色进度条
		public var aStockInfo:Array = new Array();
		public var aLeatherInfo:Array = new Array();
		public var aRefinementInfo:Array = new Array();
		
		//三种附加材料的数组
		public var aAllAppendDic:Dictionary = new Dictionary();
		public var aStockAppend:Array = new Array();
		public var aLeartherAppend:Array = new Array();
		public var aRefinementAppend:Array = new Array();
		
		public function ManufatoryProxy(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
		}
		
	}
}