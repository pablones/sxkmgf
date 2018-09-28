package GameUI.Modules.Unity.Data
{
	import GameUI.ConstData.UIConstData;
	
	public class UnityJopChange
	{
		public static function change(i:int):String
		{
			var jop:String;
			switch(i)
			{
				case 100:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_1" ];  // 帮主 1
				break;
				case 90:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_2" ];  // 副帮主 2 
				break;
				case 85:
					jop = "精英";
				break;
				case 81:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_3" ];  // 青龙堂堂主 3
				break;
				case 82:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_4" ];  // 白虎堂堂主  4
				break;
				case 84:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_5" ];  // 朱雀堂堂主  5
				break;
				case 83:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_6" ];  //  玄武堂堂主 6
				break;
				case 71:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_7" ]; //  青龙堂副堂主 7
				break;
				case 72:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_8" ];  //  白虎堂副堂主 8 
				break;
				case 74:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_9" ];  //  朱雀堂副堂主 9
				break;
				case 73:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_10" ];  //  玄武堂副堂主 10
				break;
				case 61:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_11" ];  //  青龙堂精英 11
				break;
				case 62:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_12" ];   //  白虎堂精英 12
				break;
				case 64:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_13" ];   //  朱雀堂精英 13
				break;
				case 63:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_14" ];  //  玄武堂精英 14
				break;
				case 51:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_15" ];  //  青龙堂帮众 15
				break;
				case 52:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_16" ];  //  白虎堂帮众 16
				break;
				case 54:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_17" ];  //  朱雀堂帮众 17
				break;
				case 53:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_18" ];  //  玄武堂帮众 18
				break;
				case 20:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_19" ];   // 果农 19
				break;
				case 10:
					jop = GameCommonData.wordDic[ "mod_uni_dat_unij_cha_20" ];  //  帮众 20
				break;
			}
			return jop;
		} 
		
		public static function changeBack(jop:String):int
		{
			var i:int;
			switch(jop)
			{
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_1" ]:   //  帮主  1
					i = 100;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_2" ]:   //  副帮主 2
					i = 90;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_3" ]:  //  青龙堂堂主 3
					i = 81;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_4" ]:  // 白虎堂堂主 4
					i = 82;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_5" ]: // 朱雀堂堂主 5
					i = 84;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_6" ]: // 玄武堂堂主 6
					i = 83;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_7" ]: // 青龙堂副堂主 7
					i = 71;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_8" ]:  // 白虎堂副堂主 8
					i = 72;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_9" ]:  // 朱雀堂副堂主 9
					i = 74;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_10" ]:  // 玄武堂副堂主 10
					i = 73;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_11" ]:  // 青龙堂精英 11
					i = 61;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_12" ]:  // 白虎堂精英 12
					i = 62;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_13" ]:  // 朱雀堂精英 13
					i = 64;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_14" ]:  // 玄武堂精英 14
					i = 63;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_15" ]: // 青龙堂帮众 15
					i = 51;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_16" ]:  // 白虎堂帮众 16
					i = 52;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_17" ]:  // 朱雀堂帮众 17
					i = 54;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_18" ]:  // 玄武堂帮众 18
					i = 53;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_19" ]:  // 果农 19
					i = 20;
				break;
				case GameCommonData.wordDic[ "mod_uni_dat_unij_cha_20" ]:  // 帮众 20
					i = 10;
				break;
			}
			return i; 
		} 
		/** 根据总经验值算出技能等级*/
		public static function getSkillLevel(tolExp:Number):Array
		{
			if(tolExp == 0) return [0,0];
			var curExp:Number = 0;
			var curTolExp:Number = UIConstData.ExpDic[6001];
			var curLevel:Number = 0;
			curExp = tolExp;
			while(curExp >= curTolExp)
			{
				curLevel ++;
				curExp -= UIConstData.ExpDic[6000 + curLevel];
				curTolExp = UIConstData.ExpDic[6001 + curLevel];
			}
			return [curLevel , curExp];
		}

	}
}