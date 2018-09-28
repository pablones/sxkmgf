package GameUI.Modules.Unity.Data
{
	public class UnityNumTopChange
	{
		/** 帮派人数*/
		public static function change(i:int):int
		{
			var n:int;
			switch(i)
			{
				case 1:						//帮派等级为1
					n = 50;
				break;
				
				case 2:						//帮派等级为2
//					n = 75;
					n = 55;
				break;
				
				case 3:						//帮派等级为3
//					n = 150;
					n = 60;
				break;
				
				case 4:						//帮派等级为4
//					n = 175;
					n = 65;
				break;
				
				case 5:						//帮派等级为5
//					n = 200;
					n = 70;
				break;
				
				case 6:						//帮派等级为5
//					n = 200;
					n = 75;
				break;
				
				case 7:						//帮派等级为5
//					n = 200;
					n = 80;
				break;
				
				case 8:						//帮派等级为5
//					n = 200;
					n = 85;
				break;
				
				case 9:						//帮派等级为5
//					n = 200;
					n = 90;
				break;
				
				case 10:						//帮派等级为5
//					n = 200;
					n = 95;
				break;
				
			}
			return n;
		}
		/** 分堂的等级数值等换 */
		/** type    技能：skill    建筑工人，武学大师，工匠:workMan    建设度，繁荣度:bulit    */
		public static function UnityOtherChange(level:String , type:String):String
		{
			var num:int;
			switch(level)
			{
				case "0":					//创建一个分堂需要的数据
					switch(type)
					{
						case "skill":		//技能能学习的最大值
							num = 0;
						break;				
//						case "skillUp":		//技能升级所需经验
//							num = 0;
//						break;
						case "workMan":		//分堂最大雇佣数
							num = 0;
						break;
						case "bulit":		//分堂升级所需要的建设度繁荣度
							num = 5000;
						break;
						case "otherLevelUpMoney":  //分堂升级所需要的金钱
							num = 60000;
						break;
						
					}
				break;
				case "1":
					switch(type)
					{
						case "skill":		//技能等级上限
							num = 30;
						break;				
//						case "skillUp":		//技能升级所需经验
//							num = 100;
//						break;
						case "workMan":		//分堂最大雇佣数
							num = 10;
						break;
						case "bulit":		//分堂升级所需要的建设度繁荣度
							num = 14500;
						break;
						case "mainBulit":	//主堂建设度，繁荣度上线
							num = 40000;
						break;
						case "mainLevelUpBulit"://主堂升级所需要的建设度，繁荣度
							num = 29000;
						break;
						case "mainLevelUpMoney"://主堂升级所需要的资金
							num = 3000000;
						break;
						case "mianMoneyTop":	//主堂资金上线
							num = 4000000;
						break;
						case "otherLevelUpMoney"://分堂升级所需要的资金
							num = 1500000;
						break;
						case "otherTopNum":		//帮派所能拥有的分堂数量上线
							num = 1;
						break;
						case "workMoney":			//打工得到的基础值，平衡点
							num = 13000;
						break;
						case "repairBulit":			//帮派维护建设度的值
							num = 60;
						break;
						case "repairMoney":			//维护的基本值
							num = 1500;
						break;
					}
				break;
				case "2":
					switch(type)
					{
						case "skill":		//技能等级上限
							num = 60;
						break;
//						case "skillUp":		//技能升级所需经验
//							num = 100;
//						break;
						case "workMan":		//分堂最大雇佣数
							num = 40;
						break;
						case "bulit":		//分堂升级所需要的建设度繁荣度
							num = 115000;
						break;
						case "mainBulit":	//主堂建设度，繁荣度上线
							num = 600000;
						break;
						case "mainLevelUpBulit"://主堂升级所需要的建设度，繁荣度
							num = 460000;
						break;
						case "mainLevelUpMoney"://主堂升级所需要的资金
							num = 50000000;
						break;
						case "mianMoneyTop":	//主堂资金上线
							num = 60000000;
						break;
						case "otherLevelUpMoney"://分堂升级所需要的资金
							num = 12500000;
						break;
						case "otherTopNum":		//帮派所能拥有的分堂数量上线
							num = 2;
						break;
						case "workMoney":			//打工得到的基础值，平衡点
							num = 28000;
						break;
						case "repairBulit":			//帮派维护建设度的值
							num = 480;
						break;
						case "repairMoney":			//维护的基本值
							num = 12000;
						break;
					}
				break;
				case "3":
					switch(type)
					{
						case "skill":		//技能等级上限
							num = 90;
						break;
						case "workMan":		//分堂最大雇佣数
							num = 70;
						break;
						case "bulit":		//分堂升级所需要的建设度繁荣度
							num = 400000;
						break;
						case "mainBulit":	//主堂建设度，繁荣度上线
							num = 3000000;
						break;
						case "mainLevelUpBulit"://主堂升级所需要的建设度，繁荣度
							num = 2400000;
						break;
						case "mainLevelUpMoney"://主堂升级所需要的资金
							num = 250000000;
						break;
						case "mianMoneyTop":	//主堂资金上线
							num = 300000000;
						break;
						case "otherLevelUpMoney"://分堂升级所需要的资金
							num = 42000000;
						break;
						case "otherTopNum":		//帮派所能拥有的分堂数量上线
							num = 3;
						break;
						case "workMoney":			//打工得到的基础值，平衡点
							num = 35000;
						break;
						case "repairBulit":			//帮派维护建设度的值
							num = 1260;
						break;
						case "repairMoney":			//维护的基本值
							num = 31500;
						break;
						
					}
				break;
				case "4":
					switch(type)
					{
						case "skill":		//技能等级上限
							num = 120;
						break;
						case "workMan":		//分堂最大雇佣数
							num = 100;
						break;
						case "bulit":		//分堂升级所需要的建设度繁荣度
							num = 1150000;
						break;
						case "mainBulit":	//主堂建设度，繁荣度上线
							num = 10000000;
						break;
						case "mainLevelUpBulit"://主堂升级所需要的建设度，繁荣度
							num = 9200000;
						break;
						case "mainLevelUpMoney"://主堂升级所需要的资金
							num = 900000000;
						break;
						case "mianMoneyTop":	//主堂资金上线
							num = 1000000000;
						break;
						case "otherLevelUpMoney"://分堂升级所需要的资金
							num = 112500000;
						break;
						case "otherTopNum":		//帮派所能拥有的分堂数量上线
							num = 4;
						break;
						case "workMoney":			//打工得到的基础值，平衡点
							num = 40000;
						break;
						case "repairBulit":			//帮派维护建设度的值
							num = 2400;
						break;
						case "repairMoney":			//维护的基本值
							num = 60000;
						break;
					}
				break;
				case "5":
					switch(type)
					{
						case "skill":		//技能等级上限
							num = 150;
						break;
						case "workMan":		//分堂最大雇佣数
							num = 130;
						break;
						case "bulit":		//分堂升级所需要的建设度繁荣度
							num = 0;
						break;
						case "mainBulit":	//主堂建设度，繁荣度上线
							num = 50000000;
						break;
						case "mainLevelUpBulit"://主堂升级所需要的建设度，繁荣度
							num = 0;
						break;
						case "mainLevelUpMoney"://主堂升级所需要的资金
							num = 0;
						break;
						case "mianMoneyTop":	//主堂资金上线
							num = 5000000000;
						break;
						case "otherLevelUpMoney"://分堂升级所需要的资金
							num = 0;
						break;
						case "otherTopNum":		//帮派所能拥有的分堂数量上线
							num = 4;
						break;
						case "workMoney":			//打工得到的基础值，平衡点
							num = 40000;
						break;
						case "repairBulit":			//帮派维护建设度的值
							num = 3120;
						break;
						case "repairMoney":			//维护的基本值
							num = 78000;
						break;
					}
				break;
			}
			return String(num);
		} 
		//技能所需研究度UIConstData.ExpDic
		
		/** 数值转换 */
		public static  function numToChina(vaule:int):String
		{
			var str:String;
			switch(vaule)
			{
				case 1:
					str = GameCommonData.wordDic[ "mod_uni_dat_unin_num_1" ];  // 一
				break;
				case 2:
					str = GameCommonData.wordDic[ "mod_uni_dat_unin_num_2" ];  // 二
				break;
				case 3:
					str = GameCommonData.wordDic[ "mod_uni_dat_unin_num_3" ]; // 三
				break;
				case 4:
					str = GameCommonData.wordDic[ "mod_uni_dat_unin_num_4" ]; // 四
				break;
				case 5:
					str = GameCommonData.wordDic[ "mod_uni_dat_unin_num_5" ];  // 五
				break;
			}
			return str;
		}

	}
}