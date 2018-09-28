package GameUI
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.Proxy.DataProxy;
	
	import Net.AccNet;
	
	import flash.display.DisplayObject;
	import flash.display.Shape;
	import flash.events.FocusEvent;
	import flash.external.ExternalInterface;
	import flash.filters.BitmapFilterQuality;
	import flash.filters.GlowFilter;
	import flash.geom.Point;
	import flash.system.Security;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import flash.utils.ByteArray;
	
	public class UIUtils
	{
		/** 截取指定长度的文本,中文算2长度
		 * @param txt 需要截取的文本
		 * @param length 需要截取的长度
		 * @return 截取后的内容 
		 */    
		public static function getTextByCharLength(txt:String,length:int):String
        {
            if(length<1)return "";
            var byte:ByteArray = new ByteArray();
            byte.writeMultiByte(txt,"gb2312");
            byte.position = 0;
            return byte.readMultiByte(Math.min(length, byte.bytesAvailable), "gb2312");
        }
        
        /** 填充空格  （如果s的长度没达到len,则自动补充空格）
         * @param s 字串
         * @param len 需要的长度 
         */
        public static function textfillWithSpace(s:String,  len:uint):String 
        {
        	if(!s || len == 0) return "";
        	var result:String = s;
        	var byte:ByteArray = new ByteArray();
            byte.writeMultiByte(result, "gb2312");
            byte.position = 0;
            var lenNow:uint = byte.bytesAvailable
            if(lenNow < len) {
            	var sub:uint = len - lenNow;
            	for(var i:int = 0; i < sub; i++) {
            		result += " ";
            	}
            }
            return result;
        }
        
        /** 在文本的中间填补空格 */
        public static function textFillBetweenStr( leftStr:String, rightStr:String, length:uint ):String
        {
        	var result:String = leftStr + rightStr;
			var byte:ByteArray = new ByteArray();
			byte.writeMultiByte( result, "gb2312");
			byte.position = 0;
			var num:uint = byte.bytesAvailable;
			if( num < length )
			{
				result = leftStr;
				var sub:uint = length - num;
				for(var i:uint=0; i<sub; i++)
				{
					result += " ";
				}
				result += rightStr;
			}
			return result;
        }
        
        /** 获取字符串长度(数字、英文、字符占1个，中文占2个) */
        public static function getStrRealLenght(s:String):uint
        {
        	var ret:uint = 0;
        	var byte:ByteArray = new ByteArray();
            byte.writeMultiByte(s, "gb2312");
            byte.position = 0;
            ret = byte.bytesAvailable;
            return ret;
        }
		
		/** 获取发光滤镜 默认红色边框*/
		public static function getGlowFilter(color:int=0xff0000, alpha:Number=1, blurX:Number=1.5, blurY:Number=1.5, strength:Number = 6):GlowFilter {
			var glow:GlowFilter = new GlowFilter();
			glow.color = color;
			glow.alpha = alpha;
			glow.blurX = blurX; 						//水平模糊量
			glow.blurY = blurY; 						//垂直模糊量
			glow.quality = BitmapFilterQuality.MEDIUM;	//质量
			glow.strength = strength;					//应用倍数
			return glow;
		}
		
		/** 获取文本格式 默认12字体白色*/
		public static function getTextFormat(size:int=12, color:int=0xffffff):TextFormat {
			var tf:TextFormat = new TextFormat();
			tf.size = size;
			tf.color = color;
			tf.font = "宋体";
			tf.align = TextFormatAlign.RIGHT;
			tf.bold;
			return tf;
		}
		
		/** 格式化显示资金，用于资金显示*/
		public static function formatMoney(money:String):String {
			var result:String = "";
			if(money) {
				if(money.length < 8) {
					if(money.length > 3) {	//添加","进行分隔
						var arr:Array = new Array();		
						var pos:int = money.length % 3;
						var s1:String = money.substr(0,pos);
						if( s1.length != 0 ) { arr.push(s1); }
						var s2:String = money.substr(pos, money.length - pos);
						for (var i:int = 0; i < s2.length; i += 3) {
							var num:String = s2.slice(i , i + 3);
							arr.push( num );
						}
						for( var j:int = 0; j < arr.length; j++ ) {
							if( j < arr.length - 1 ) {
								result += arr[j] + ",";
							} else {
								result += arr[j];
							}				
						}
					} else {
						result = money;
					}
				} else if(money.length == 8) { //千万
					result = (Number(money) / 10000).toFixed(1) + "W";					
				} else {	//亿
					result = (Number(money) / 100000000).toFixed(2) + "E";					
				}
			}
			return result;
		}
		
		/** 深复制数组 */
		public static function DeeplyCopy(data:Object):Object
		{
			var da:Object = new Object();
			var ba:ByteArray = new ByteArray();
			da = data;
			ba.writeObject(da);
			ba.position = 0;
			return ba.readObject();
		}
		
		/** 将钱分解为 金、银、铜 数组   1金=100银=10000铜 */
		public static function getMoney(money:Number):Array
		{
			var arr:Array = [];
			var jin:uint  = 0;
			var yin:uint  = 0;
			var tong:uint = 0;
			jin = money / 10000;
			yin = (money - jin * 10000) / 100;
			tong = money %100;
			arr.push(jin);
			arr.push(yin);
			arr.push(tong);
			return arr;
		}
		
		/** 将金、银、铜 都转化为铜 */
		public static function formatMoneyToTong(jin:uint, yin:uint, tong:uint):uint
		{
			return (10000 * jin + 100 * yin + tong);	//1金=100银，1银=100铜
		}
		
		/** 获取金钱数量字符串 */
		public static function getMoneyInfo(money:Number):String
		{	
			var result:String = "";
			var moneyArr:Array = getMoney(money);
			var strArr:Array = ["\\ce","\\cs","\\cc"];
			for(var i:int = 0; i < moneyArr.length; i++) {
				if(moneyArr[i] != 0) {
					result += moneyArr[i] + strArr[i];
				}
			}
			return result;
		}
		
		/** 获取金钱数量字符串 */
		public static function getMoneyStandInfo(money:Number, str:Array):String
		{	
			var result:String = "";
			var moneyArr:Array = getMoney(money);
			var strArr:Array = str;
			for(var i:int = 0; i < moneyArr.length; i++) {
				if(i==0 && moneyArr[0] == 0) continue;
				if(i==1 && moneyArr[0] == 0 && moneyArr[1] == 0) continue;
				result += moneyArr[i] + strArr[i];
			}
			return result;
		}
		
		/** 获取单个金钱 */
		public static function getMoneySingle(money:Number, str:String):String
		{
			return money + str;
		}
		
//原战斗评分，先注释备份
//		public static function GetScoreStr(score:uint, isPet:Boolean = false):String
//		{
//			var scoreStr:String = "";
//			if(score > 0 && score <= 1014) {
//				if(isPet) {
//					scoreStr = "E级";
//				} else {
//					scoreStr = "E级" + "("+ score + ")";
//				}
//			} else if(score > 1014 && score <= 1224) {
//				if(isPet) {
//					scoreStr = "D级";
//				} else {
//					scoreStr = "D级" + "("+ score + ")";
//				}				
//			} else if(score > 1224 && score <= 1590) {
//				if(isPet) {
//					scoreStr = "C级";
//				} else {
//					scoreStr = "C级" + "("+ score + ")";
//				}				
//			} else if(score > 1590 && score <= 2394) {
//				if(isPet) {
//					scoreStr = "B级";
//				} else {
//					scoreStr = "B级" + "("+ score + ")";
//				}				
//			} else if(score > 2394 && score <= 3823) {
//				if(isPet) {
//					scoreStr = "A级";
//				} else {
//					scoreStr = "A级" + "("+ score + ")";
//				}
//			} else if(score > 3823 && score <= 7838) {
//				if(isPet) {
//					scoreStr = "S级";
//				} else {
//					scoreStr = "S级" + "("+ score + ")";
//				}				
//			} else if(score > 7838 && score <= 10802) {
//				if(isPet) {
//					scoreStr = "S+级";
//				} else {
//					scoreStr = "S+级" + "("+ score + ")";
//				}				
//			} else if(score > 10802 && score <= 24174) {
//				if(isPet) {
//					scoreStr = "S++级";
//				} else {
//					scoreStr = "S++级" + "("+ score + ")";
//				}				
//			}
//			return scoreStr;
//		}
		
		/** 新战斗评分 */
		public static function GetScoreStr(score:uint, isPet:Boolean = false):String
		{
			if ( !GameCommonData.wordDic ) return null;
			var scoreStr:String = "";
			if(score >= 0 && score < 500) {
				if(isPet) {
					scoreStr = "E"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "E"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级
				}
			} else if(score >= 500 && score < 1000) {
				if(isPet) {
					scoreStr = "D"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "D" + GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级 
				}				
			} else if(score >= 1000 && score < 1500) {
				if(isPet) {
					scoreStr = "C"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "C"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级
				}				
			} else if(score >= 1500 && score < 2000) {
				if(isPet) {
					scoreStr = "B"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "B"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级
				}				
			} else if(score >= 2000 && score < 4500) {
				if(isPet) {
					scoreStr = "A"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "A" +GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ]+ "("+ score + ")";//级
				}
			} else if(score >= 4500 && score < 6000) {
				if(isPet) {
					scoreStr = "S"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "S"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级
				}				
			} else if(score >= 6000 && score < 9500) {
				if(isPet) {
					scoreStr = "S+"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
				} else {
					scoreStr = "S+"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级
				}				
			} else if(score >= 9500) {
				if(isPet) {
					scoreStr = "S++"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ];//级
					if(score > 9999)
					{
						scoreStr = "S++";
					}
				} else {
					scoreStr = "S++"+GameCommonData.wordDic[ "mod_fri_view_med_friendC_han_2" ] + "("+ score + ")";//级
				}
			} 
			if(isPet)
			{
				scoreStr = PetPropConstData.isNewPetVersion ? (scoreStr + "("+score+")"): scoreStr;
			}
			return scoreStr;
		}

		/** 添加焦点管理 */
		public static function addFocusLis(dis:DisplayObject):void
		{
			dis.addEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			dis.addEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
		}
		/** 移除焦点管理 */
		public static function removeFocusLis(dis:DisplayObject):void
		{
			dis.removeEventListener(FocusEvent.FOCUS_IN, focusInHandler);
			dis.removeEventListener(FocusEvent.FOCUS_OUT, focusOutHandler);
		}
		private static function focusInHandler(e:FocusEvent):void
		{
			GameCommonData.isFocusIn = true;
		}
		private static function focusOutHandler(e:FocusEvent):void
		{
			GameCommonData.isFocusIn = false;
		}
		 
		/** 获取物品的等级类型标题 */
		public static function getLevTitle(itemType:uint):String
		{
			if(itemType == 0) return "";
			var result:String = "";
			if(itemType < 300000) {		//装备
				result = GameCommonData.wordDic[ "gUI_UIU_getL_1" ]+"：";//"等级需求："; 
			} else if(String(itemType).indexOf("3") == 0 || String(itemType).indexOf("5") == 0) {	//药品 和其他 可使用物品
				result = GameCommonData.wordDic[ "gUI_UIU_getL_2" ]+"：";//"使用等级："; 
			} else {
				result = GameCommonData.wordDic[ "mod_man_com_cli_exe_2" ]+"：";//物品等级 
			}
			return result;
		}
		
		/** 获取物品的绑定类型标题 */
		public static function getBindShow(data:Object):String
		{
			var ret:String = "";
			var ITEM_BIND_DATA:Array = [
													{data:0x0200, name:GameCommonData.wordDic[ "mod_rp_med_em_2" ] },        	//装备后绑定
													{data:0x0400, name:GameCommonData.wordDic[ "mod_cons_uic_ite_2" ] },			//拾取后绑定
													{data:0x20,   name:GameCommonData.wordDic[ "mod_cons_uic_ite_3" ] },				//不可交易
													{data:0x40,   name:GameCommonData.wordDic[ "mod_cons_uic_ite_4" ] }				//不可丢弃
													];
													
			if(data.isBind && data.isBind == 1) {
				ret = GameCommonData.wordDic[ "gUI_UIU_geB_1" ];//"已绑定";
			} else if(data.isBind && data.isBind == 2) {
				ret = "<font color='"+IntroConst.HUN_YIN_COLOR+"'>"+GameCommonData.wordDic[ "gUI_UIU_geB_2" ]+"</font>";//魂印
			} else {
				var monopoly:int = UIConstData.getItem(data.type).Monopoly; 
				for(var i:int = 0; i < ITEM_BIND_DATA.length; i++) {
					if( (monopoly & ITEM_BIND_DATA[i].data).toString() == Number(ITEM_BIND_DATA[i].data).toString(10)) {
						ret = ITEM_BIND_DATA[i].name;
						break;
					}
				}
			}
			return ret;
		}
		
		/** 数组的排序方法 ， 多维数组*/
		public static function ArraysortOn(state:String , arr:Array , objIndex:int , rankState:Boolean ,amount:int = 10):Array
		{
			var allArr:Array = [];
			var sortedArr:Array = [];				//返回的已排好序的数组
			var secArr:Array = [];					//2级数组
			var num:int;
			for(var i:int = 0;i < arr.length ; i++)
			{
				if(arr[i] is Array)
				{
					for(var n:int = 0;n < arr[i].length ; n++)
					{
						if(i == 0) allArr[n + i*arr[i].length] = arr[i][n];
						else allArr[n + i*arr[i - 1].length] = arr[i][n];
					}
				}
			}
			switch(state)
			{
				//不区分大小写的排序
				case "CASEINSENSITIVE":
					var index0:String = String(objIndex);
					if(rankState == false) allArr.sortOn(index0 , Array.CASEINSENSITIVE);
					else allArr.sortOn(index0 , Array.CASEINSENSITIVE | Array.DESCENDING);;
				break;
				//数值排序
				case "NUMERIC":
					var index1:String = String(objIndex);
					if(rankState == false) allArr.sortOn(index1 , Array.NUMERIC);
					else allArr.sortOn(index1 , Array.NUMERIC | Array.DESCENDING );
				break;
				//降序排序
				case "DESCENDING":
					var index2:String = String(objIndex);
					allArr.sortOn(index2 , Array.CASEINSENSITIVE);
				break;
				//Array唯一排序要求 
				case "UNIQUESORT":
					var index3:String = String(objIndex);
					allArr.sortOn(index3 , Array.CASEINSENSITIVE);
				break;
			}
			for(var p:int = 0;p < allArr.length ; p++)
			{
				sortedArr[int(p/amount)] = [];
			}
			for(var t:int = 0;t < allArr.length ; t++)
			{
				sortedArr[int(t/amount)][int(t%amount)] = allArr[t];
			}
			return sortedArr;
		}					
		
		/** 校验角色名 */
		public static function isPermitedRoleName(name:String):Boolean
		{
			if(!name) return false;
			var result:Boolean = true;
			for(var i:int = 0; i < UIConstData.Filter_role.length; i++) {
				if(name.indexOf(UIConstData.Filter_role[i]) >= 0) {
					result = false;
					break;
				}
			}
			return result; 
		}
		/** 检验合法字符 */
		public static function legalRoleName(name:String):Boolean
		{
			var nameLength:int = name.length;
			var isLegal:Boolean = true;												//是否合法
			var nameList:Array = name.match(/./g);
			for(var i:int = 0; i < nameList.length; i++)
			{
				if(nameList[i].match((/[\u4e00-\u9fa5]+/g))[0] != null) 
				{
					nameLength --;
					continue;		//中文直接跳出本次循环
				}
				if(nameList[i].match((/[a-zA-Z0-9]/g))[0] != null)
				{
					nameLength --;
					continue;			//英文数字直接跳出本次循环
				} 
				for(var n:int = 0; n <  UIConstData.Filter_okName.length; n++)
				{
					if(nameList[i].indexOf(UIConstData.Filter_okName[n]) > -1) {			//包含合法字符
						nameLength --;
						break;
					}
				}
			}
			if(nameLength > 0 ) isLegal = false;
			return isLegal;
		}
		
		/** 校验宠物名 */
		public static function isPermitedPetName(name:String):Boolean
		{
			if(!name) return false;
			var result:Boolean = true;
			for(var i:int = 0; i < UIConstData.Filter_role.length; i++) {
				if(UIConstData.Filter_role[i] == "(" || UIConstData.Filter_role[i] == ")") continue;
				if(name.indexOf(UIConstData.Filter_role[i]) >= 0) {
					result = false;
					break;
				}
			}
			return result; 
		}
		
		/** 过滤广告 */
		public static function filterAD(info:String):String
		{
			if(!info) return "";
			var str_to:String = "*";
			var result:String = "";
			result = info.replace(/^\s*|\s*$/g,"").split(" ").join("");
			for(var i:int = 0; i < UIConstData.Filter_ad.length; i++) {
				var reg:RegExp = new RegExp(UIConstData.Filter_ad[i], "g");
				result = result.replace(reg, str_to);
			}
			return result;
		}
		
		/** 过滤聊天 */
		public static function filterChat(info:String):String
		{
			if(!info) return "";
			var str_to:String = "*";
			var result:String = info;
//			result = info.replace(/^\s*|\s*$/g,"").split(" ").join(""); 
			result = result.replace(/\s*$/g, "");    //去掉后面的空格
			result = result.replace(/^\s*/g, "");    //去掉前面的空格
			for(var i:int = 0; i < UIConstData.Filter_chat.length; i++) {
				var reg:RegExp = new RegExp(UIConstData.Filter_chat[i], "g");
				if(reg.test(result)) {
					result = result.replace(reg, str_to);
					trace("i:",i,UIConstData.Filter_chat[i]);
				}
			}
//			result = result.replace(/\-\*\-/g, " ");
			return result;
		}
		
		/** 校验聊天 */
		public static function checkChat(info:String):Boolean
		{
			var result:Boolean = true;
			if(!info) {
				result = false;
			} else {
				for(var i:int = 0; i < UIConstData.Filter_chat.length; i++) {
					var reg:RegExp = new RegExp(UIConstData.Filter_chat[i], "ig");
					if(info.search(reg) >= 0) {
						result = false;
						break;
					}
				}
			}
			return result;
		} 
		
		/** 过滤骗子信息 */
		public static function checkCheatStr(info:String):Boolean
		{
			var result:Boolean = true;
			var points:Number = 0;
			if(!info) {
				points = 1.0;
			} else {
				for(var i:int = 0; i < ChatData.CHEAT_CHAT_FILTER.length; i++) {
					var repData:Object = ChatData.CHEAT_CHAT_FILTER[i];
					var reg:RegExp = repData.rep as RegExp;
					if(info.search(reg) >= 0) {
						points += repData.point;
					}
				}
			}
			result = (points >= 1.0) ? false : true;
			return result;
		}
		
		/** 获取UI的舞台居中位置 */ 
		public static function getMiddlePos(val:DisplayObject):Point
		{
			var x:Number = (1000 - val.width) >> 1;
			var y:Number = (580 - val.height) >> 1;
			var p:Point  = new Point(x, y); 
			return p;
		}
		
		/**
		 * 整型按位与运算(int 32位)
		 * @param  int    需要按位与的总位数  
		 * @return Array  按位与结果数组 (由低位到高位)
		 */
		public static function IntegerBitwiseAnd(record:uint, bitAmount:int=0):Array
		{
			if(bitAmount == 0) {
				bitAmount = 32;
			}
			var res:Array = [];
			var tmp:int = 0;
			for(var i:int = 0; i < bitAmount; i++) {
				tmp = ((record & Math.pow(2, i)) > 0) ? 1 : 0;
				res.push(tmp);
			}
			return res;
		}
		
		/**
		 * 根据下标数组 转成整型
		 * @param  Array 下标数组
		 * @return int	 转换后的整型
		 */ 
		public static function ArrayBitwiseAndToInteger(arr:Array):uint
		{
			var len:int = arr.length;
			var tmp:uint = 0;
			var res:uint = 0;
			for(var i:int = 0; i < len; i++) {
				tmp = arr[i];
				if(tmp == 1) {
					res += Math.pow(2, i);
				}
			}
			return res;
		}
		
		/** 商店物品数组排序，按照使用由低到高等级排序 */
		public static function arrSortByLev(arr:Array):Array
		{
			var res:Array = [];
			for(var i:int = 0; i < arr.length; i++) {
				var type:uint = arr[i];
				var obj:Object = UIConstData.getItem(type);
				var lev:uint  = obj.Level;
				var PriceIn:uint = obj.PriceIn;
				arr[i] = {PriceIn:PriceIn,type:type, lev:lev };
			}
			if(2 == GameCommonData.wordVersion)
			{
				arr.sortOn(['PriceIn', 'lev', 'type'],Array.NUMERIC );
			}else if(1 == GameCommonData.wordVersion)
			{
				arr.sortOn(['lev', 'type']);
			}
			
			for(var j:int = 0; j < arr.length; j++) {
				res[j] = arr[j].type;
			}
			return res;
		}
		/** 系统时间装换(数值转换中文格式) 日期*/
		public static function timeToYear():String
		{
			var timeStr:String;
			timeStr = GameCommonData.gameYear + GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_1" ]//"年" 
						+ (GameCommonData.gameMonth < 10 ? "0"+GameCommonData.gameMonth:GameCommonData.gameMonth) + GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_2" ]//"月"
						+ (GameCommonData.gameDay < 10 ? "0"+GameCommonData.gameDay:GameCommonData.gameDay) + GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_3" ];//"日";
			return timeStr;
		}
		/** 系统时间装换(数值转换中文格式) 时间*/
		public static function timeToHour():String
		{
			var timeStr:String;
			timeStr = (GameCommonData.gameHour < 10 ? "0"+GameCommonData.gameHour:GameCommonData.gameHour) + ":"
						+ (GameCommonData.gameMinute < 10 ? "0"+GameCommonData.gameMinute:GameCommonData.gameMinute) + ":"
						+ (GameCommonData.gameSecond < 10 ? "0"+GameCommonData.gameSecond:GameCommonData.gameSecond);
			return timeStr;
		}
		/** 本地时间时间装换(数值转换中文格式) 时间 */
		public static function placeTimeToHour():String
		{
			var date:Date = new Date();
			var timeStr:String;
			timeStr = (date.getHours() < 10 ? "0"+date.getHours():date.getHours()) + ":"
						+ (date.getMinutes() < 10 ? "0"+date.getMinutes():date.getMinutes()) + ":"
						+ (date.getSeconds() < 10 ? "0"+date.getSeconds():date.getSeconds());
			return timeStr;
		}
		/** 本地时间时间装换(数值转换中文格式) 日期 */
		public static function placeTimeToYear():String
		{
			var date:Date = new Date();
			var timeStr:String;
			timeStr = date.getFullYear() + GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_1" ]//"年"  
						+ (date.getMonth() < 9 ? "0"+( date.getMonth() + 1 ) : ( date.getMonth() + 1 ) ) + GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_2" ]//"月"
						+ (date.getDate() < 10 ? "0"+date.getDate():date.getDate()) + GameCommonData.wordDic[ "mod_fri_view_med_rec_initD_3" ];//"日";
			return timeStr;
		}
		/** 调用js */
		public static function callJava( fun:String ):void
		{
			try
			{
				ExternalInterface.call( fun );	
			}
			catch ( e:Error )
			{
				
			}
		}
		
		//返回vip字符串
		public static function getVipStr( vip:int ):String
		{
			var _vipStr:String;
			switch ( vip )
			{
				case 0:
					_vipStr = "<font color='#FFFFFF'>"+GameCommonData.wordDic[ "often_used_none" ]+"</font>";      // 无
				break;
				case 1:
					_vipStr = "<font color='#0098FF'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_1" ]+"</font>";   // 月卡
				break;
				case 2:
					_vipStr = "<font color='#7a3fe9'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_2" ]+"</font>";    // 季卡
				break;
				case 3:
					_vipStr = "<font color='#FF6532'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_3" ]+"</font>";   // 半年卡
				break;
				case 4:
					_vipStr = "<font color='#00FF00'>"+GameCommonData.wordDic[ "mod_mas_pro_old_getv_4" ]+"</font>";   // 周卡
				break;
				default:
				break;
			}
			return _vipStr;
		}
		
		//获取男女字符串
		public static function getSexStr( sexIndex:int ):String
		{
			var _sexStr:String;
			if ( sexIndex == 0 )
			{
				_sexStr = GameCommonData.wordDic[ "mod_mas_pro_old_gets_1" ];			//男
			}
			else if ( sexIndex == 1 )
			{
				_sexStr = GameCommonData.wordDic[ "mod_mas_pro_old_gets_2" ];           //女
			}
			return _sexStr;
		}
		
		//获得线路字符串
		public static function getLineStr( line:int ):String
		{
			var _lineStr:String
			switch ( line )
			{
				case 0:
					_lineStr = "<font color = '#666666'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_1" ]+"</font>";    //离线
				break;
				case 1:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_2" ]+"</font>";    //一线
				break;
				case 2:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_3" ]+"</font>";    //二线
				break;
				case 3:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_4" ]+"</font>";    //三线
				break;
				case 4:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_5" ]+"</font>";    //四线
				break;
				case 5:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_6" ]+"</font>";    //五线
				break;
				case 6:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_chat_com_rec_getU_1" ]+"</font>";    //帮派
				break;
				case 7:
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_8" ]+"</font>";    //七线
				break;
				default:			//防止意外
					_lineStr = "<font color = '#00ff00'>"+GameCommonData.wordDic[ "mod_mas_com_got_get_2" ]+"</font>";    //一线
				break;
			}
			return _lineStr;
		}
		
		/** 第一件神器 */
		public static function firstTreasure():void
		{
			var dataProxy:DataProxy = GameCommonData.UIFacadeIntance.retrieveProxy( DataProxy.NAME ) as DataProxy;
			if( !dataProxy.MarketIsOpen )
			{
				GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWMARKETVIEW , 1 );
			}
		}
		
		/** 绘制一个矩形边框 */
		public static function createFrame( w:int,h:int,color:uint = 0xffff00 ):Shape
		{
			var frame:Shape = new Shape();
			frame.graphics.clear();
			frame.graphics.lineStyle( 1,color );
			frame.graphics.lineTo( 0,0 );
			frame.graphics.lineTo( w,0 );
			frame.graphics.lineTo( w,h );
			frame.graphics.lineTo( 0,h );
			frame.graphics.lineTo( 0,0 );
			return frame;
		}
		
		/** vip颜色 */
		public static function getVipColor( vip:int,defaultColor:uint = 0xE2CCA5 ):uint
		{
			var str:uint;
			switch ( vip )
			{
				case 0:
					str = defaultColor;
					break;
					
				case 1:
					str = 0x0098FF;
					break;
					
				case 2:
					str = 0x7a3fe9;
					break;
					
				case 3:
					str = 0xFF6532;
					break;
					
				case 4:
					str = 0x00FF00;
					break;
				
				default:
					break;
			}
			return str;
		} 
		
		//连接账号服务器
		public static function conectAccServer():void
		{
			GameCommonData.IsConnectAcc = false;
			GameCommonData.isReceiveAcc = false;
			GameCommonData.isReceive1052 = false;
			
			Security.loadPolicyFile("xmlsocket://"+GameConfigData.AccSocketIP+":843");
			GameCommonData.AccNets = new AccNet(GameConfigData.AccSocketIP, GameConfigData.AccSocketPort);
		}
		
		/** 根据type从商城中获取物品 */
		public static function getGoodsAtMarket(type:*):Object
		{
			var item:Object;
			for (var i:uint=0; i < UIConstData.MarketGoodList.length; i++)
			{
				var obj:Array=UIConstData.MarketGoodList[i];
				for (var j:uint=0; j < obj.length; j++)
				{
					if (int(obj[j].type) == int(type))
					{
						item=obj[j];
						return item;
					}
				}
			}
			return item;
		}
	}
}