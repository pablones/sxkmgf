package GameUI.Modules.Map.SmallMap.SmallMapConst
{
	import flash.utils.Dictionary;
	
	public class SmallConstData
	{
		private static var _instance:SmallConstData;
		public  var smallMapDic:Dictionary=new Dictionary();
		public var mapItemDic:Dictionary=new Dictionary();
		public function SmallConstData()
		{	
			
			//副本，地府等。。。宠物岛,
			mapItemDic[2001]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2001" ] };//"副本"}; 
			mapItemDic[2002]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2002" ] };//"古老皇陵"}; 
			
			mapItemDic[2020]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2020" ] };//"点苍副本"}; 
			mapItemDic[2021]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2021" ] };//"全真副本"}; 
			mapItemDic[2022]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2022" ] };//"峨眉副本"}; 
			mapItemDic[2023]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2023" ]};//"唐门副本"}; 
			mapItemDic[2024]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2024" ] };//"丐帮副本"}; 
			mapItemDic[2025]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2025" ] };//"少林副本"}; 
			
			mapItemDic[2003]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2003" ] };//"海市"}; 
			mapItemDic[2004]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2004" ] };//"蜃楼"}; 
			mapItemDic[2005]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2005" ] };//"长寿龙台"}; 
			mapItemDic[2006]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2006" ] };//"巫蛊祭坛"}; 
			mapItemDic[2007]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2007" ] };//"荒漠宝藏"};
			mapItemDic[2008]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2008" ] };//"皇家狩猎"};
			mapItemDic[2009]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2009" ] };//"逆天龙脉"};
			
			mapItemDic[2010]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2010" ] };//"情人节副本"};
			mapItemDic[2011]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2011" ] };//"年兽副本"};
			mapItemDic[2012]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2012" ] };//"蜃楼寻宝一层"};
			mapItemDic[2013]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2013" ] };//"蜃楼寻宝二层"};
			mapItemDic[2014]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2014" ] };//"蜃楼寻宝三层"};
			mapItemDic[2016]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2016" ] };//"绝归岛"
			mapItemDic[2017]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2017" ] };//"青龙古墓"
			mapItemDic[2018]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item2018" ] };//"绝归岛怨魂"
			 
			mapItemDic[1026]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1026" ] };//"地府"};
			mapItemDic[1027]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1027" ] };//"公共监狱"};
			
			mapItemDic[1028]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1028" ] };//"宠物林一层"};
			mapItemDic[1029]={id:32,name:GameCommonData.wordDic [ "mod_map_sma_sma_sma_Item1029" ] };//"宠物林二层"};
			mapItemDic[1030]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1030" ] };//"宠物林三层"};
			mapItemDic[1031]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1031" ] };//"宠物林四层"};
			mapItemDic[1032]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1032" ] };//"宠物林五层"};
			mapItemDic[1033]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1033" ]};//"宠物林六层"};
			
			
			//end				
			mapItemDic[1001]={id:26,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1001" ] ,tileX:90,tileY:49};//"江陵" 
			mapItemDic[1002]={id:1,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1002" ] ,tileX:90,tileY:89};//};//"蝴蝶谷"
			mapItemDic[1003]={id:2,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1003" ] ,tileX:94,tileY:64};//};//"云渡山"
			mapItemDic[1004]={id:3,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1004" ] ,tileX:129,tileY:77};//};//"西野郡郊"
			mapItemDic[1005]={id:4,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1005" ] ,tileX:87,tileY:124};//};//"正气坡"
			mapItemDic[1006]={id:23,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1006" ] ,tileX:94,tileY:58};//};//"开封"
			mapItemDic[1007]={id:5,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1007" ] ,tileX:113,tileY:46};//};//"雁门关"
			mapItemDic[1008]={id:6,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1008" ] ,tileX:126,tileY:91};//};//"昆仑山"
			mapItemDic[1009]={id:7,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1009" ] ,tileX:52,tileY:65};//};//"云梦泽"
			mapItemDic[1010]={id:8,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1010" ] ,tileX:49,tileY:83};//};//"诸葛八卦村"
			mapItemDic[1011]={id:9,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1011" ] ,tileX:74,tileY:56};//};//"幽州"
			mapItemDic[1012]={id:10,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1012" ] ,tileX:77,tileY:53};//};//"高昌古城"
			mapItemDic[1013]={id:11,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1013" ] ,tileX:125,tileY:61};//};//"大理"
			mapItemDic[1014]={id:12,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1014" ] ,tileX:118,tileY:85};//};//"苗疆"
			mapItemDic[1015]={id:13,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1015" ] ,tileX:43,tileY:69};//};//"楼兰"
			mapItemDic[1016]={id:14,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1016" ] ,tileX:21,tileY:78};//"忘川荒村"
			mapItemDic[1017]={id:15,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1017" ] ,tileX:77,tileY:60};//};//"琼州"
			mapItemDic[1018]={id:16,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1018" ] ,tileX:106,tileY:94};//};//"辽上京"
			mapItemDic[1019]={id:17,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1019" ] ,tileX:42,tileY:53};//};//"南塔"
			mapItemDic[1020]={id:20,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1020" ] ,tileX:48,tileY:45};//};//"苍山"
			mapItemDic[1021]={id:22,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1021" ] ,tileX:48,tileY:42};//};//"终南山"	
			mapItemDic[1022]={id:25,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1022" ] ,tileX:50,tileY:47};//};//"峨眉山"	
			mapItemDic[1023]={id:28,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1023" ] ,tileX:59,tileY:52};//};//"唐家堡"	
			mapItemDic[1024]={id:24,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1024" ] ,tileX:76,tileY:43};//};//"君山"	
			mapItemDic[1025]={id:27,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1025" ] ,tileX:51,tileY:43};//};//"少室山"	
			 
			mapItemDic[1034]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1034" ] };//"江陵1"};	
			mapItemDic[1035]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1035" ] };//"蝴蝶谷1"};	
			mapItemDic[1036]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1036" ] };//"江陵2"};	
			mapItemDic[1037]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1037" ] };//"蝴蝶谷2"};	
			mapItemDic[1038]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1038" ] };//"江陵3"};	
			mapItemDic[1039]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1039" ]};//"蝴蝶谷3"};	
			mapItemDic[1040]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1040" ] };//"江陵4"};	
			mapItemDic[1041]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1041" ] };//"蝴蝶谷4"};	
			
			mapItemDic[1042]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1042" ] };//"低级比武场"};	
			mapItemDic[1043]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1043" ] };//"中级比武场"};	
			mapItemDic[1044]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1044" ] };//"高级比武场"};	
			mapItemDic[1045]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1045" ] };//"大师级比武场"};	
			mapItemDic[1046]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1046" ] };//"宗师级比武场"};
			mapItemDic[1047]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1047" ] };//"戏院"};		
			mapItemDic[1048]={id:32,name: GameCommonData.wordDic[ "mod_chat_com_rec_getU_1" ] };//"帮派"};	
			mapItemDic[1049]={id:32,name: GameCommonData.wordDic[ "mod_lost_1" ] };//"初级星浮宫"};
			mapItemDic[1050]={id:32,name: GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1050" ] };//"中级星浮宫"
			mapItemDic[1051]={id:32,name: GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1051" ] };//"高级星浮宫"	
			mapItemDic[1052]={id:32,name: GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1052" ] };//"英雄台"
			mapItemDic[1055]={id:32,name: GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1055" ] };//"帮派历练场"	
			
			
			mapItemDic[1101]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1101" ] };//"伐木场一层"};	
			mapItemDic[1102]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1102" ] };//"伐木场二层"};	
			mapItemDic[1103]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1103" ] };//"伐木场三层"};	
			mapItemDic[1104]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1104" ] };//"伐木场四层"};	
			mapItemDic[1105]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1105" ] };//"伐木场五层"};	
			mapItemDic[1106]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1106" ] };//"伐木场六层"};	
			mapItemDic[1107]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1107" ] };//"伐木场七层"};	
			mapItemDic[1108]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1108" ] };//"伐木场八层"};	
			mapItemDic[1109]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1109" ] };//"伐木场九层"};	
			mapItemDic[1110]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1110" ] };//"伐木场十层"};	
			mapItemDic[1201]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1201" ] };//"夷陵矿场一层"};	
			mapItemDic[1202]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1202" ] };//"夷陵矿场二层"};	
			mapItemDic[1203]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1203" ] };//"夷陵矿场三层"};	
			mapItemDic[1204]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1204" ] };//"夷陵矿场四层"};	
			mapItemDic[1205]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1205" ] };//"夷陵矿场五层"};	
			mapItemDic[1206]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1206" ] };//"夷陵矿场六层"};	
			mapItemDic[1207]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1207" ] };//"夷陵矿场七层"};	
			mapItemDic[1208]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1208" ] };//"夷陵矿场八层"};	
			mapItemDic[1209]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1209" ] };//"夷陵矿场九层"};	
			mapItemDic[1210]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1210" ] };//"夷陵矿场十层"};	
			mapItemDic[1301]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1301" ] };//"幽谷猎场一层"};	
			mapItemDic[1302]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1302" ] };//"幽谷猎场二层"};	
			mapItemDic[1303]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1303" ] };//"幽谷猎场三层"};	
			mapItemDic[1304]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1304" ] };//"幽谷猎场四层"};	
			mapItemDic[1305]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1305" ] };//"幽谷猎场五层"};	
			mapItemDic[1306]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1306" ] };//"幽谷猎场六层"};	
			mapItemDic[1307]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1307" ] };//"幽谷猎场七层"};	
			mapItemDic[1308]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1308" ] };//"幽谷猎场八层"};	
			mapItemDic[1309]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1309" ] };//"幽谷猎场九层"};	
			mapItemDic[1310]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item1310" ] };//"幽谷猎场十层"};	
			
			mapItemDic[3001]={id:21,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3001" ] ,tileX:64,tileY:116};//};//"斩魔塔一层"	
			mapItemDic[3002]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3002" ] };//"斩魔塔二层"};	
			mapItemDic[3003]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3003" ] };//"斩魔塔三层"};	
			mapItemDic[3004]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3004" ] };//"斩魔塔四层"};	
			mapItemDic[3005]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3005" ] };//"斩魔塔五层"};	
			mapItemDic[3006]={id:29,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3006" ] ,tileX:17,tileY:77};//};//"靖江王陵一层"	
			mapItemDic[3007]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3007" ] };//"靖江王陵二层"};	
			mapItemDic[3008]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3008" ] };//"靖江王陵三层"};	
			mapItemDic[3009]={id:19,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3009" ] ,tileX:71,tileY:116};//};//"高昌迷宫一层"
			mapItemDic[3010]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3010" ] };//"高昌迷宫二层"};
			mapItemDic[3011]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3011" ] };//"高昌迷宫三层"};
			
			mapItemDic[3012]={id:30,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3012" ] ,tileX:71,tileY:123};//};//"苗疆异冢一层"
			mapItemDic[3013]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3013" ] };//"苗疆异冢二层"};
			mapItemDic[3014]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3014" ] };//"苗疆异冢三层"};
			mapItemDic[3015]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3015" ] };//"苗疆异冢四层"};
			mapItemDic[3016]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3016" ] };//"苗疆异冢五层"};
			mapItemDic[3017]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3017" ] };//"苗疆异冢六层"};
			mapItemDic[3018]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3018" ] };//"苗疆异冢七层"};
			mapItemDic[3019]={id:31,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3019" ] ,tileX:35,tileY:102};//};//"千龙洞一层"
			mapItemDic[3020]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3020" ] };//"千龙洞二层"};
			mapItemDic[3021]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3021" ] };//"千龙洞三层"};
			mapItemDic[3022]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3022" ] ,tileX:84,tileY:17};//};//"九宫一层"
			mapItemDic[3023]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3023" ] };//"九宫二层"};
			mapItemDic[3024]={id:32,name:GameCommonData.wordDic[ "mod_map_sma_sma_sma_Item3024" ] };//"九宫三层"}; 
			
			smallMapDic["1001"]=[12,32,0,12,0,0,false,0,false,0];
			smallMapDic["1002"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1003"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1004"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1005"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1006"]=[0,32,0,0,0,0,false,0,false,0];
			smallMapDic["1007"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1008"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1009"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1010"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1011"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1012"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1013"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1014"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1015"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1016"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1017"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1018"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1019"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1020"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1021"]=[5,29,0,0,0,0,false,0,false,0];
			smallMapDic["1022"]=[5,29,0,0,0,0,false,0,false,0];
			smallMapDic["1023"]=[23,-24,0,0,0,0,true,581,false,0];
			smallMapDic["1024"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1025"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1026"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1027"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1028"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1029"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1030"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1031"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1032"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1033"]=[0,0,0,0,0,0,false,0,false,0];
			
			smallMapDic["1034"]=[12,32,0,12,0,0,false,0,false,0];
			smallMapDic["1035"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1036"]=[12,32,0,12,0,0,false,0,false,0];
			smallMapDic["1037"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1038"]=[12,32,0,12,0,0,false,0,false,0];
			smallMapDic["1039"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1040"]=[12,32,0,12,0,0,false,0,false,0];
			smallMapDic["1041"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["1042"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1043"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1044"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1045"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1046"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1047"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1048"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1049"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1050"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1051"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1052"]=[0,32,0,0,0,0,false,0,false,0];
			smallMapDic["1055"]=[0,0,0,0,0,0,false,0,false,0];
			
			smallMapDic["1101"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1102"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1103"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1104"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1105"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1106"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1107"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1108"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1109"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1110"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1201"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1202"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1203"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1204"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1205"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1206"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1207"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1208"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1209"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1210"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1301"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1302"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1303"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1304"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1305"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1306"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1307"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1308"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1309"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["1310"]=[0,0,0,0,0,0,false,0,false,0];
		
			
			smallMapDic["2001"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2002"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2003"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2004"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2005"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2006"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2007"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2008"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2009"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["2010"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2011"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2012"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2013"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2014"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["2016"]=[21,43,0,0,0,0,false,0,false,0];
			smallMapDic["2017"]=[3,20,0,0,0,0,false,0,false,0];
			smallMapDic["2018"]=[3,20,0,0,0,0,false,0,false,0];
			
			smallMapDic["2020"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["2021"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["2022"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["2023"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["2024"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["2025"]=[-16,16,0,0,0,0,true,581,false,0];
			
			
			smallMapDic["3001"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3002"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3003"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3004"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3005"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3006"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3007"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3008"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3009"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3010"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3011"]=[-16,16,0,0,0,0,true,581,false,0];
			smallMapDic["3012"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3013"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3014"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3015"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3016"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3017"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3018"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3019"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3020"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3021"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3022"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3023"]=[0,0,0,0,0,0,false,0,false,0];
			smallMapDic["3024"]=[0,0,0,0,0,0,false,0,false,0];
		}
		

		public static function getInstance():SmallConstData{
			if(_instance==null)_instance=new SmallConstData();
			return _instance;
		}
		
		
		
		/**
		 * 根据ID号获得地图的名子 
		 * @param id
		 * @return 
		 * 
		 */
		 		
		public function getSceneNameById(id:uint):String{
			for(var name:* in mapItemDic){
				var obj:Object=mapItemDic[name];
				if(obj.id==id){
					return name;
				}
			}
			return null;
		}
		
		/**
		 * 根据地图ID获取地图的名字 
		 * @param mapId
		 * @return 
		 * 
		 */		
		public function getSceneNameByMapId(mapId:uint):String{
			if(mapItemDic[mapId]!=null){
				return mapItemDic[mapId].name;
			}else{
				return GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_3" ];//"未知";
			}
		}
		
	}
}