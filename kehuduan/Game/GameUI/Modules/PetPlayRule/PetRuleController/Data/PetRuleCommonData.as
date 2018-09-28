package GameUI.Modules.PetPlayRule.PetRuleController.Data
{
	import GameUI.Modules.ToolTip.Const.IntroConst;
	
	import OopsEngine.Role.GamePetRole;
	
	public class PetRuleCommonData
	{
		public function PetRuleCommonData()
		{
		}
		
		
		public static var curPageIndex:uint = 0;
		
		public static var selectedPet:GamePetRole = null;
		
		public static const MARKET_INDEX:Array = [19, 18, 20, null, 21, 22, null,32];
		
		/** 面板格子数据 */
		public static const PAGE_PANEL_DATA:Array = [
													{data:[630008, 630009, 630010], index:1},	//还童
													{data:[630001], index:1},					//单繁
													{data:[630011, 630012, 630013,612002], index:1},	//悟性提升
													null,										//合成
													{data:["640010_640499"], index:0},			//技能学习
													{data:[630015, 630018, 630020,630036], index:1},	//技能提升
													null,										//双繁
													{data:[630034], index:1}					//灵性提示（灵犀丹）
													];
		
		/** 当前物品是否合法 */
		public static function isPermitItem(pageIndex:uint, type:int):Boolean
		{
			var res:Boolean = false;
			if(PAGE_PANEL_DATA[pageIndex] != null) {
				var dataArr:Array = PAGE_PANEL_DATA[pageIndex].data;
				var bagIndex:uint = PAGE_PANEL_DATA[pageIndex].index;
				if(dataArr[0] is int) {				//单个
					var len:int = dataArr.length;
					for(var i:int = 0; i < len; i++) {
						if(dataArr[i] == type) {
							res = true;
							break;
						}
					}
				} else if(dataArr[0] is String) {	//范围
					var tmp:Array = (dataArr[0] as String).split("_");
					var min:int = int(tmp[0]);
					var max:int = int(tmp[1]);
					if(type >= min && type <= max) {
						res = true;
					}
				}
			}
			return res;
		}
		
		
		public static var selectLock:Boolean = false;
		
		
		public static function getPetGrade(grade:uint):String
		{
			var res:String = "";
			if(grade >= 81) {
				res = "<font color='"+IntroConst.itemColors[5]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_12" ] + "</font>";    // 完美
			} else if (grade >= 61){
				res = "<font color='"+IntroConst.itemColors[4]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_13" ] + "</font>";    // 卓越
			} else if(grade >= 41) {
				res = "<font color='"+IntroConst.itemColors[3]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_14" ]  + "</font>";    // 杰出
			} else if(grade >= 21) {
				res = "<font color='"+IntroConst.itemColors[2]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_15" ] + "</font>";   // 优秀
			} else {
				res = "<font color='"+IntroConst.itemColors[1]+"'>" + GameCommonData.wordDic[ "mod_pet_med_petu_sho_16" ] + "</font>";    // 普通
			}
			return res;
		}
		
		public static function getSavvyAddPercent(savvy:uint):String 
		{
			var savvyAddPercent:String;
			switch(savvy) {
				case 1:
					savvyAddPercent = "(+1%)";
					break;					
				case 2:
					savvyAddPercent = "(+1.6%)";
					break;					
				case 3:
					savvyAddPercent = "(+2.6%)";
					break;					
				case 4:
					savvyAddPercent = "(+4.1%)";
					break;					
				case 5:
					savvyAddPercent = "(+6.6%)";
					break;					
				case 6:
					savvyAddPercent = "(+10.5%)";
					break;					
				case 7:
					savvyAddPercent = "(+16.8%)";
					break;					
				case 8:
					savvyAddPercent = "(+26.8%)";
					break;					
				case 9:
					savvyAddPercent = "(+42.9%)";
					break;					
				case 10:
					savvyAddPercent = "(+68.7%)";
					break;					
				default:
					savvyAddPercent = "";
					break;
			}
			return savvyAddPercent;
		}
		
	}
}