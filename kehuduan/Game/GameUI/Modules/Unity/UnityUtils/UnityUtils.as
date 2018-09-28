package GameUI.Modules.Unity.UnityUtils
{
	import GameUI.ConstData.CommandList;
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityEvent;
	import GameUI.Modules.Unity.Data.UnityJopChange;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.UICore.UIFacade;
	import GameUI.UIUtils;
	
	import Net.ActionProcessor.Chat;
	import Net.Protocol;
	
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class UnityUtils
	{
		private var _obj:Object;
		private var _arr:Array;
		
		//  ["被踢出帮派" , "离开帮派" , "被升职为" , "被降职为" , "加入帮派"]
		public static var UnityImportStrArr:Array = [GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_1" ] , GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_2" ] , GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_3" ] , GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_4" ] , GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_5" ]];
		private static var charProcessor:Chat;			//接受聊天的对象
		
		/** 1为删除 , 2为添加 , 3为任命 */
		public function UnityUtils(obj:Object , arr:Array)
		{
			_obj = obj;
			_arr = arr;
		}
		/** 操作 */
		/** 1为删除 , 2为添加 , 3为任命 */
		public function operating(type:int):Array
		{
			var arr:Array = [];
			switch(type)
			{
				/** 删除*/
				case 1:
					arr = del();
				break;
				/** 添加 */
				case 2:
					arr = add();
				break;
				/** 任命 */
				case 3:
					arr = ord();
				break;
				/** 更新 */
				case 4:
					arr = upDate();
				break;
			}
			return arr;
		} 
		
		/** 删除(包括退出和踢出) */
		/** obj的属性6为角色id */
		private function del():Array
		{
			for(var i:int = 0; i < _arr.length; i++)
			{
				if(_arr[i] is Array)							//二维数组
				{
					for(var n:int = 0; n < (_arr[i] as Array).length; n++)
					{
						if((_arr[i][n] as Object)["6"] == _obj["6"])
						{
							(_arr[i] as Array).splice(n , 1);
							if(i != (_arr.length - 1) as int)									//不是最后一页的情况
							{
								var lastPlayer:Array = (_arr[_arr.length - 1] as Array).splice(_arr[(_arr.length - 1) as int].length - 1 , 1);
								_arr[i][9] = lastPlayer[0];
							}
						}
					}
				}
			}
			if(_arr[(_arr.length - 1) as int].length == 0)						//移除后存有空页，需要删除
			{
				_arr.splice((_arr.length - 1) as int , 1);
			}
			return _arr;
		}
		/** 添加 */
		private function add():Array
		{
			if(_arr[(_arr.length - 1) as int].length == 10)							//需要多加一页的情况			
			{
				_obj["4"] = 10;
				_arr[_arr.length] = [_obj];
			}
			else
			{
				var _arrPage:Array = _arr[(_arr.length - 1) as int] as Array;		//最后一页的数组
				_obj["4"] = 10;
				_arrPage.push(_obj);
			}
			return _arr;
		}
		/** 任命 */
		/** obj的属性4为角色已任命的帮派职位 */
		private function ord():Array
		{
			var jopSwitch:Boolean = false;
			for(var i:int = 0; i < _arr.length; i++)
			{
				if(_arr[i] is Array)							//二维数组
				{
					for(var n:int = 0; n < (_arr[i] as Array).length; n++)
					{
						var index:int = _obj["4"];
						if((_arr[i][n] as Object)["6"] == _obj["6"])			
						{
							(_arr[i][n] as Object)["4"] = _obj["4"];
							if(GameCommonData.Player.Role.Id == _obj["6"])			//如果是自己
							{
								GameCommonData.UIFacadeIntance.sendNotification(UnityEvent.SHOWMYJOP);
							}
						}
					}
				}
			}
			return _arr;
		}
		/** 更新 */
		/** obj的属性7为角色上下线的状态 */
		private function upDate():Array
		{
			for(var i:int = 0; i < _arr.length; i++)
			{
				if(_arr[i] is Array)							//二维数组
				{
					for(var n:int = 0; n < (_arr[i] as Array).length; n++)
					{
						var index:int = _obj["4"];
						if((_arr[i][n] as Object)["6"] == _obj["6"])			
						{
							(_arr[i][n] as Object)["7"] = _obj["7"];
						}
					}
				}
			}
//			}
			return _arr;
		}
		
		/** 通过帮派系统信息 获取 玩家数据*/
		public static function getPlayObj(info:String):void
		{
			var obj:Object;
			var name:String;
			var jop:int;
//			//加人
//			if(info.indexOf("加入帮派") != -1)
//			{
//				if(info.indexOf("请求") != -1) return;
//				name = info.slice(0 , info.length -4);
//				obj = getSelectObj(name);
//				UnityConstData.updataArr[0] = obj;
//				UnityConstData.updataArr[1] = 2;
//			}
			//踢人
			if(info.indexOf( GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_1" ] ) != -1)  // 被踢出帮派
			{
				name = info.slice(0 , info.length -5);
				obj = getSelectObj(name);
				if(obj == null) return;
				UnityConstData.updataArr[0] = obj;
				UnityConstData.updataArr[1] = 1;
			}
			//退出帮派
			if(info.indexOf( GameCommonData.wordDic[ "mod_uni_uni_uutil_wor_2" ] ) != -1)  // 离开帮派
			{
				name = info.slice(0 , info.length -4);
				obj = getSelectObj(name);
				if(obj == null) return;
				UnityConstData.updataArr[0] = obj;
				UnityConstData.updataArr[1] = 1;
			}
			
			//任命
			if(info.indexOf( GameCommonData.wordDic[ "mod_uni_uni_uutil_get_1" ] ) != -1 || info.indexOf( GameCommonData.wordDic[ "mod_uni_uni_uutil_get_2" ] ) != -1)  // 升职  降职
			{
				var jopString:String = (info.split( GameCommonData.wordDic[ "mod_uni_uni_uutil_get_3" ] )[1] as String).split( GameCommonData.wordDic[ "mod_uni_uni_uutil_get_4" ] )[1] as String;  // 被  为
				name = info.split( GameCommonData.wordDic[ "mod_uni_uni_uutil_get_3" ] )[0];  // 被
				jop =  UnityJopChange.changeBack(jopString) as int;
				obj = getSelectObj(name);
				if(obj == null) return;
				obj["4"] = jop;
				UnityConstData.updataArr[0] = obj;
				UnityConstData.updataArr[1] = 3;
				if(jop == 100) UnityConstData.mainUnityDataObj.newBoss = name;
				if(name == GameCommonData.Player.Role.Name)	GameCommonData.UIFacadeIntance.sendNotification(UnityEvent.SHOWMYJOP);		//更改我的职位
			}
		}
		/** 获取Object*/
		/** 0为人名 */
		public static function getSelectObj(name:String):Object
		{
			var obj:Object = null;
			var a:Array = UnityConstData.allMenberList;
			for(var n:int = 0;n < (UnityConstData.allMenberList as Array).length;n++)
			{
				for(var k:int = 0;k < (UnityConstData.allMenberList[n] as Array).length;k++)
				{
					if((UnityConstData.allMenberList[n][k] as Object)["0"] == name)
								obj = UnityConstData.allMenberList[n][k] as Object;
				}
			}
//			if(obj == null) UnityConstData.allMenberList[
			return obj;
		}
		/** 获取所有建造的堂(不包括主堂) */
		public static function getOtherNum():int
		{
			var num:int = 0;
			for(var i:int = 0; i < UnityConstData.otherUnityArray.length ; i++)
			{
				if(i == 0) continue;
				if(UnityConstData.otherUnityArray[i].level > 0) num ++;
			}
			return num;
		}
		/** 是否有本堂副堂主以上的权限 */
		public static function getHasSelfTopJop(_type:int):Boolean
		{
			var isTop:Boolean = false;
			if(GameCommonData.Player.Role.unityJob < 70)			//果农 ， 帮众 ， 精英
			{
				isTop = false;
			}
			else if(GameCommonData.Player.Role.unityJob == int(70 + _type) ||GameCommonData.Player.Role.unityJob == int(80 + _type) ||GameCommonData.Player.Role.unityJob >= 90)
			{//本堂副堂主，堂主 ， 副帮主，帮主
				isTop = true;
			}
			else
			{//其它堂堂主，副堂主
				isTop = false;
			}
			return isTop;
		}
		/** 是否是本堂人员 */
		public static function getSelfMenber(_type:int):Boolean
		{
			var isSelf:Boolean = false;
			if(GameCommonData.Player.Role.unityJob % 10 == _type)			
			{
				isSelf = true;
			}
			else if(GameCommonData.Player.Role.unityJob >= 90)		//帮主 副帮主
			{
				isSelf = true;
			}
			return isSelf;
		}
		/** 悬浮框数据初始化*/
		public static function levelUpNeedData(obj:Object):void
		{
			var repairMoney:int = int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "repairMoney")) + int(
									int(UnityConstData.greenUnityDataObj.craftsmanNum) 
									+ int(UnityConstData.whiteUnityDataObj.craftsmanNum) 
									+ int(UnityConstData.xuanUnityDataObj.craftsmanNum) 
									+ int(UnityConstData.redUnityDataObj.craftsmanNum)
									+ int(UnityConstData.greenUnityDataObj.businessmanNum) 
									+ int(UnityConstData.whiteUnityDataObj.businessmanNum) 
									+ int(UnityConstData.xuanUnityDataObj.businessmanNum) 
									+ int(UnityConstData.redUnityDataObj.businessmanNum)
									+ int(UnityConstData.greenUnityDataObj.masterNum) 
									+ int(UnityConstData.whiteUnityDataObj.masterNum) 
									+ int(UnityConstData.xuanUnityDataObj.masterNum) 
									+ int(UnityConstData.redUnityDataObj.masterNum)
									) * 10000;
			if(obj.level >= UnityConstData.unityLevelTop) UnityConstData.levelUpData = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_1" ];  // 已达到顶级
			else  // 升级要求  升级消耗
			UnityConstData.levelUpData = "<font color = '#00CBFF'>"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_2" ]+"：</font>" + UnityConstData.levelNeedList[0] + UnityConstData.levelNeedList[1] + UnityConstData.levelNeedList[2] + UnityConstData.levelNeedList[3] 
												+ "<font color = '#00CBFF'>\n\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_3" ]+"：    </font>" + UnityConstData.levelNeedList[4] + UnityConstData.levelNeedList[5] + UnityConstData.levelNeedList[6] + UnityConstData.levelNeedList[7];
			
			/** 分堂繁荣度 */
			UnityConstData.infoBooming = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_4" ];  // 帮派升级所需
			/** 分堂建设度 */
			UnityConstData.infoBuilt = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_4" ];  // 帮派升级所需
			/** 帮派资金 */   // 上限 金  每小时维护消耗
			UnityConstData.infoMoney = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_5" ]+"<font color='#00FF00'>"+int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "mianMoneyTop")) / 10000 + GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ]+" </font>"
			                           + "\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_6" ]+"："+"<font color = '#00FF00'>"+ moneyToString(repairMoney)+ "</font>";;
			/** 成员数量 */  // 当前在线  当前帮派成员总数  帮派成员总上限
			UnityConstData.infoMenber = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_7" ]+"：" + "<font color = '#00FF00'>"+UnityConstData.mainUnityDataObj.onlineMenber + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_8" ]+"："+ "<font color = '#00FF00'> "+UnityConstData.mainUnityDataObj.unityMenber+"</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_9" ]+"：" + "<font color = '#00FF00'>"+UnityNumTopChange.change(UnityConstData.mainUnityDataObj.level)+"</font>";
			/** 分堂建筑工匠 */  // 雇佣每分钟增加建设度  点
			UnityConstData.infoCraftsman = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_11" ] + "<font color = '#00FF00'>"+obj.craftsmanNum + "</font>"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_10" ];
			/** 分堂武学大师 */  // 雇佣每分钟增加技能研究度  点 
			UnityConstData.infoMasterNum = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_12" ] + "<font color = '#00FF00'>"+obj.masterNum +"</font>"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_10" ];
			/** 分堂贸易商人 */  // 雇佣每分钟增加繁荣度  点
			UnityConstData.infoBusinessman = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_13" ] + "<font color = '#00FF00'>"+obj.businessmanNum + "</font>"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_10" ] ;
			/** 建设度进度条悬浮框 */  // 帮派总建设度  分堂升级建设度 主堂  每小时维护消耗 
			UnityConstData.infoBuiltBar 	= GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_14" ] + "："+"<font color = '#00FF00'>"+UnityConstData.mainUnityDataObj.unityBuilt + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_15" ]+"：" +"<font color = '#00FF00'>"+UnityNumTopChange.UnityOtherChange(obj.level , (obj.name == GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_16" ] ? "mainLevelUpBulit": "bulit"))
												+ "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_6" ]+"："+"<font color = '#00FF00'>"+ UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "repairBulit")+ "</font>";
			/** 繁荣度进度条悬浮框 */  // 帮派总繁荣度 分堂升级繁荣度  主堂  每小时维护消耗  
			UnityConstData.infoBoomingBar 	= GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_17" ] + "："+"<font color = '#00FF00'>"+UnityConstData.mainUnityDataObj.unitybooming + "</font>\n" + GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_18" ] + "：" +"<font color = '#00FF00'>"+UnityNumTopChange.UnityOtherChange(obj.level , (obj.name == GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_16" ] ? "mainLevelUpBulit": "bulit"))
												+ "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_6" ]+"："+"<font color = '#00FF00'>"+ UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "repairBulit")+ "</font>";
			/** 技能等级悬浮框1 */  // 已学习等级  已研究等级  可研究上限 
			UnityConstData.infoSkill_1 = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_19" ]+"："+"<font color = '#00FF00'>"+obj["skillStudySelf" + 1] + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_20" ]+"：" + "<font color = '#00FF00'>"+obj["skillStudyCurr" + 1] + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_21" ]+"：" + "<font color = '#00FF00'>"+UnityNumTopChange.UnityOtherChange(obj.level , "skill") ;
			/** 技能等级悬浮框2 */  // 已学习等级  已研究等级  可研究上限 
			UnityConstData.infoSkill_2 = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_19" ]+"："+"<font color = '#00FF00'>"+obj["skillStudySelf" + 2] + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_20" ]+"：" + "<font color = '#00FF00'>"+obj["skillStudyCurr" + 2] + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_21" ]+"：" + "<font color = '#00FF00'>"+UnityNumTopChange.UnityOtherChange(obj.level , "skill");
			/** 技能等级悬浮框3 */  // 已学习等级  已研究等级  可研究上限
			UnityConstData.infoSkill_3 = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_19" ]+"："+"<font color = '#00FF00'>"+obj["skillStudySelf" + 3] + GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_20" ]+"："+"<font color = '#00FF00'>"+obj["skillStudyCurr" + 3] + "</font>\n"+GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_21" ]+"：" + "<font color = '#00FF00'>"+UnityNumTopChange.UnityOtherChange(obj.level , "skill") ;
			/** 青龙堂悬浮框 */
			UnityConstData.infoSynQ = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_22" ];  // 可研究<font color='#00FF00'>青龙毒煞、青龙摆尾、青龙斩月</font>技能
			/** 白虎堂悬浮框 */
			UnityConstData.infoSynB = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_23" ];  // 可研究<font color='#00FF00'>白虎冰啸、白虎星降、猛虎一击</font>技能
			/** 玄武堂悬浮框 */
			UnityConstData.infoSynX = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_24" ];  // 可研究<font color='#00FF00'>玄影武袭、玄武战甲、玄武霸体</font>技能
			/** 朱雀堂悬浮框 */
			UnityConstData.infoSynZ = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_25" ];  // 可研究<font color='#00FF00'>朱雀焰舞、朱雀真火、朱雀灵羽</font>技能
			/** 停止维护的悬浮框 */
			UnityConstData.infoUnityState = GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_26" ];  // 帮派停止维护，分堂无法打工及学习技能
			/** 分堂关闭的悬浮框 */
			UnityConstData.infoSubState= GameCommonData.wordDic[ "mod_uni_uni_uutil_lev_27" ];  //帮派等级下降，分堂自动关闭，帮派等级上升后可重新开放
			
		}
		/** 返回已建造了哪些分堂数组*/
		public static function getBuiltedSubList():Array
		{
			var subList:Array = [];
			for(var i:int = 1;i < 5;i++)
			{
				if(UnityConstData.otherUnityArray[i].level > 0 && UnityConstData.otherUnityArray[i].isStop == false)	//停用或未建造时，不能任命
				{
					subList.push(UnityConstData.otherUnityArray[i].name);
				}
			}
			return subList;
		}
		/** 模拟邮件 */
		public static function sendMeg(msg:String):void 
		{
			var num:int = Math.ceil((msg.length * 2) / 50);
			var date:Date = new Date();
			charProcessor = GameCommonData.GameNets.Gam.ActionList[Protocol.PLAYER_CHAT];
			var bytes:ByteArray = new ByteArray();
			bytes.endian=Endian.LITTLE_ENDIAN;
			bytes.position=4;
			bytes.writeUnsignedInt(0); 					 //话的颜色
			bytes.writeShort(2040);     				//话的频道	
			bytes.writeShort(0); 	 					 //话的风格
			bytes.writeUnsignedInt(date.getTime()); 	//时间
			bytes.writeUnsignedInt(0);    		 		//GM工具所用
			bytes.writeUnsignedInt(0);  		 		//话里面带的物品ID
			bytes.writeUnsignedInt(0); 	 		 		//话里面带的物品typeID
			bytes.writeByte(num+3);							//循环的次数，第3次为谈话类容
			bytes.writeByte(4);	
			bytes.writeMultiByte( GameCommonData.wordDic[ "mod_chat_com_rec_getD" ] , GameCommonData.CODE);			// 系统
			bytes.writeByte(0);		
			bytes.writeByte(0);	
			for(var i:int = 0; i < num ; i++)
			{
				var msgPart:String = msg.slice(i * 25 , (i+1) * 25);
				bytes.writeByte(UIUtils.getStrRealLenght(msgPart));
				bytes.writeMultiByte(msg.slice(i * 25 , (i+1) * 25) , GameCommonData.CODE);
			}
			charProcessor.Processor(bytes);
		}
		/** 金钱数值转换 */
		public static function moneyToString(value:int):String
		{
			var money:String;
			var jin:String =  value / Math.pow(100 , 2) > 0 ? (int(value / Math.pow(100 , 2))+GameCommonData.wordDic[ "mod_uni_com_kee_kee_1" ]).toString():"";   // 金
			var yin:String = int(value % 10000) / Math.pow(100 , 1) > 0 ? (int(int(value % 10000) / Math.pow(100 , 1))+ GameCommonData.wordDic[ "mod_uni_uni_uutil_mon_1" ] ).toString():""; // 银
			var tong:String = int(value % 100) / Math.pow(100 , 0) > 0 ? (int(int(value % 100) / Math.pow(100 , 0))+ GameCommonData.wordDic[ "mod_uni_uni_uutil_mon_2" ] ).toString():"";   // 铜
			money = jin + yin + tong;
			if(value == 0) money = "0"+GameCommonData.wordDic[ "mod_uni_uni_uutil_mon_2" ];  //铜
			return money;
		}
		/** 发送邮件消息 */
		public static function sendSysMsg(msg:String):void{
			var obj:Object={};
			var arr:Array=[];
			arr.push("system");
			arr.push("fsddfsd");  //接收的玩家
			arr.push(timeToNum());
			arr.push(msg);
			arr.push("unknow");
			arr.push("unknow");
			obj.talkObj=arr;
			obj.nItemTypeID=1;
			obj.nItem=1;
			obj.nGm=0;
			obj.nColor=0;
			obj.nAtt=2040;
			UIFacade.UIFacadeInstance.sendNotification(CommandList.FRIEND_CHAT_MESSAGE,obj);
		}
		/** 计算打工完成后是低利润还是高利润 ， 0是低，1是高*/
		public static function getWorkVaule(addMoney:int):int
		{
			var vaule:int;
			if(addMoney > int(UnityNumTopChange.UnityOtherChange(UnityConstData.mainUnityDataObj.level , "workMoney")))
			{
				vaule = 1;		//高利润
			}
			else vaule = 0;		//低利润
			return vaule;
		}
		/** 将时间转为服务器所需格式 */
		public static function timeToNum():String
		{
			var time:String;
			time = String(GameCommonData.gameYear) 
				  + (int(GameCommonData.gameMonth - 1) < 10 ? "0" + String(int(GameCommonData.gameMonth) - 1) : String(int(GameCommonData.gameMonth) - 1))
				  + (int(GameCommonData.gameDay) < 10 ? "0" + GameCommonData.gameDay : String(GameCommonData.gameDay))
				  + (int(GameCommonData.gameHour) < 10 ? "0" + GameCommonData.gameHour : String(GameCommonData.gameHour))
				  + (int(GameCommonData.gameMinute) < 10 ? "0" + GameCommonData.gameMinute : String(GameCommonData.gameMinute))
				  + (int(GameCommonData.gameSecond) < 10 ? "0" + GameCommonData.gameSecond : String(GameCommonData.gameSecond));
			return time;
		}
		/** 根据技能ID取主角是否可用该技能 */
		public static function getIsUseUnitySkill(id:int):Boolean
		{
			var isUseSkill:Boolean = false;
			id = int(id);
			var synId_0:int = int(id % 1000);
			var synId:int   = int(synId_0 / 100);
			isUseSkill = getSelfMenber(synId - 1);
			return isUseSkill;
		}
		
	}
}