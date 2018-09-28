package GameUI.Modules.NewerHelp.Data
{
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.NewerHelp.UI.NewerHelpItem;
	
	import flash.display.MovieClip;
	import flash.geom.Point;
	import flash.utils.Dictionary;
	
	public class NewerHelpData
	{
		public function NewerHelpData()
		{
		}
		
		/** 自动寻路数据 */
		public static var autoPathData:Object = null;	//autoPathData.curType   autoPathData.curStep  
		
		public static function getAutoPathData():Object
		{
			var res:Object = autoPathData;
			if(curType == 20 && curStep == 1) {	//屠大勇 自动寻路
				res = {};
				res.curType = curType;
				res.curStep = curStep;
				return res;
			}
			if(curType == 1 && curStep == 4) {	//拿了屠大勇刀 回去找张老头
				res = {};
				res.curType = curType;
				res.curStep = curStep;
				return res;
			}
			return res;
		}
		
		public static var id:int;
		public static var obj:Object;
		
		/** 武道乾坤新手任务所需变量 */
		public static var marketUI:MovieClip;
		public static var comboxName:String;
		
		/** 全局坐标点 */
		public static var point:Point;
		
		/** 干掉兔子任务能否使用小飞鞋 */
		public static var CAN_FLY:Boolean = false;
		
		/** 宽度 */
		public static const VIEW_WIDTH:int = 153;
		
		/** 高度 */
		public static const VIEW_HEIGHT:int = 55;
		
		/** 名字为2个字的 任务号 */
		public static const TWO_LEN_TASK_ID:Array = [
														105,108,109,110,111,115,122,123,124,125,126
													];
		
		/** 名字为3个字的 任务号 */
		public static const THREE_LEN_TASK_ID:Array = [
														103,104,106,107,112,113,114,116,117,118,119,120,121
														];
		
		/** 新手指导是否开启 */
		public static var newerHelpIsOpen:Boolean = true;
		
		/** 当前任务号 */
		public static var curType:uint = 0;
		 
		/** 当前步骤号 */
		public static var curStep:uint = 0;
		
		/** 第一个新手大礼包装备 */
		public static const FIRST_BAG_ITEMS:Array = [
													210302,
													220302
													];
													
		/** 第二个新手大礼包装备 */
		public static const SECOND_BAG_ITEMS:Array = [
													130902,
													190902,
													120902
													];
		
		/** 查找职业三件套type */
		public static function findCloses():Array
		{
			var result:Array = null;
			switch(GameCommonData.Player.Role.CurrentJobID) {
				case 1:			//唐门
					result = [116991, 176991, 146991]
					break;
				case 2:			//全真
					result = [112991, 172991, 142991]
					break;
				case 4:			//峨眉
					result = [111991, 171991, 141991]
					break;
				case 8:			//丐帮
					result = [113991, 173991, 143991]
					break;
				case 16:		//少林
					result = [115991, 175991, 145991]
					break;
				case 32:		//点苍
					result = [114991, 174991, 144991]
					break;
			}
			return result;
		}
		
		/** 获取单件套装位置 */
		public static function getTypePos(type:uint):int 
		{
			var pos:int = -1;
			var arr:Array = findCloses();
			if(arr) {
				for(var i:int = 0; i < arr.length; i++) {
					if(type == arr[i]) {
						pos = i;
						break;
					}
				}
			}
			return pos;
		}
		
		/** 获取第一个大礼包的装备位置 */
		public static function getFirstBagPos(type:uint):int
		{
			var pos:int = -1;
			var arr:Array = FIRST_BAG_ITEMS;
			if(arr) {
				for(var i:int = 0; i < arr.length; i++) {
					if(type == arr[i]) {
						pos = i;
						break;
					}
				}
			}
			return pos;
		}
		
		/** 获取第二个大礼包的装备位置 */
		public static function getSecondBagPos(type:uint):int
		{
			var pos:int = -1;
			var arr:Array = SECOND_BAG_ITEMS;
			if(arr) {
				for(var i:int = 0; i < arr.length; i++) {
					if(type == arr[i]) {
						pos = i;
						break;
					}
				}
			}
			return pos;
		}
		
		/**
		 * 处理新手掩码 
		 * @param type: 0-判断新手，操作掩码; 1-强制打开掩码; 2-强制关闭掩码
		 */ 
		public static function dealNewerMask(type:uint):void
		{
			switch(type) {
				case 0:
					if(GameCommonData.Player.Role.Level >= 30) {
						newerHelpIsOpen = false;
					}
					break;
				case 1:
					newerHelpIsOpen = true;
					break;
				case 2:
					newerHelpIsOpen = false;
					break;
			}
		}
		
		/**
		 * 查找商城购物车中是否有某商品
		 * @param type: 商品type
		 * @return Boolen
		 */ 
		public static function havaMarketItemByType(type:int):Boolean
		{
			var ret:Boolean = false;
			for(var i:int = 0; i < MarketConstData.shopCarData.length; i++) {
				if(type == MarketConstData.shopCarData[i].type) {
					ret = true; 
					break;
				}
			}
			return ret;
		}
		
		/** 释放我知道了 */
		public static function deleteIKnow(logo:String):void
		{
			if(iKnowItemDic[logo]) {
				(iKnowItemDic[logo] as NewerHelpItem).dispose();
				delete iKnowItemDic[logo];
			}
		}
		
		/** 我知道了字典 Dictionary<String, NewerHelpItem> */
		public static var iKnowItemDic:Dictionary = new Dictionary();
		
		/** 13级提示 15级即可获得新手成就大礼包 */
		public static const LEV13_BAO_IKNOW:String = "LEV13_BAO_IKNOW";
		
		/** 是否第一次去蝴蝶谷 */
		public static var isFirst:Boolean = false;
		
		public static function changeId( mapId:int, npcId:int ):Object
		{
			var mapName:String=GameCommonData.GameInstance.GameScene.GetGameScene.name;
			var realNpcId:int=npcId;
			var realMapId:int=mapId;
			switch(mapName){
				case "1034":
				case "1035":
					realNpcId+=3300;
					realMapId+=33;
					break;
					
				case "1036":
				case "1037":
					realNpcId+=3500;
					realMapId+=35;
					break;
				case "1038":
				case "1039":
					realNpcId+=3700;
					realMapId+=37;
					break;
				case "1040":
				case "1041":
					realNpcId+=3900;
					realMapId+=39;
					break;
			}
			return { _mapId:realMapId, _npcId:realNpcId };
		}
	}
}