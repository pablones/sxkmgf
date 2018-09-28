package GameUI.Modules.CompensateStorage.data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	import GameUI.Modules.Hint.Events.HintEvents;

	import Net.ActionSend.CompensateStorageSend;

	import flash.system.ApplicationDomain;
	import flash.utils.Dictionary;

	public class CompensateStorageData
	{
		public static var CompensateStorageName:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_1"] /**= "补偿仓库"*/;
		public static var CompensateStorageDetailsName:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_2"] /** = "补偿详情"*/;
		public static var word1:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_3"] /** =  "补偿内容"*/;
		public static var word2:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_4"] /** =  "帮贡"*/;
		public static var word3:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_5"] /** =  "荣誉"*/;
		public static var word4:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_6"] /** =  "经验"*/;
		public static var word5:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_7"] /** =  "珠宝"*/;
		public static var word6:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_8"] /** =  "元宝"*/;
		public static var word7:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_9"] /** =  "碎银"*/;
		public static var word8:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_10"] /** =  "银两"*/;
		public static var word9:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_11"] /** =  "无"*/;
		public static var word10:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_12"] /** =  "补偿宠物"*/;
		public static var word11:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_13"] /** =  "请先点选一样道具"*/;
		public static var word12:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_14"] /** =  "请先点选一个宠物"*/;
		public static var word13:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_15"] /** =  "当前没有可以领取的东西"*/;
		public static var word14:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_16"] /** =  "年"*/;
		public static var word15:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_17"] /** =  "月"*/;
		public static var word16:String=GameCommonData.wordDic["GameUI_Modules_CompensateStorage_data_CompensateStorageData_18"] /** =  "日"*/;

		/** 补偿日志 */
		public static var compensateDetails:String="";
		/** 补偿日志是否为空或者有修改 */
		public static var isEmptyOrUpdata:Boolean=true;
		/** 是否已经请求补偿数据 */
		public static var isRequestData:Boolean=false;
		/** 补偿数据  */
		public static var dataList:Array=[];
		/** 道具数据 */
		public static var itemList:Array=[];
		/** 宠物数据 */
		public static var petList:Array=[];

		/****** 道具 *******/
		/** 格子数 */
		public static var MAXNUM:int=20;
		/** 格子数据列表 */
		public static var gridUnitList:Array=new Array();
		/** 格子背景列表 */
		public static var gridBackList:Array=new Array();
		/** 选择的物品 */
		public static var selectedItem:GridUnit=null;
		/** 选择的物品type */
		public static var selectedId:int=0;

		/** 打开界面索引 */
		public static var openPageIndex:int=0;

		/****** 宠物 *****/
		/** 宠物列表位置（x,y）和宽度 */
		public static var petListView:Array=[19, 54, 160];

		/****** 其他 *****/
		/** 其他面板中 文本宽度 滚动面板x、y、width、height  */
		public static var otherSiteList:Array=[167, 10, 30, 179, 199];


		/** 非绑定人民币 (珠宝)*/
		public static var zhuBao:uint=0;
		/** 绑定货币 */
		public static var suiYin:uint=0;
		/** 非绑定货币 */
		public static var yinLiang:uint=0;
		/** 绑定人民币 (元宝)*/
		public static var yuanBao:uint=0;
		/** 帮贡 */
		public static var bangGong:uint=0;
		/** 荣誉 */
		public static var rongYu:uint=0;
		/** 经验 */
		public static var exp:uint=0;

		/** 当前显示的分页 */
		public static var currentPage:int=-1;

		public static var isInitView:Boolean=false;
		public static var isInitItemView:Boolean=false;
		public static var isInitPetView:Boolean=false;
		public static var isInitOtherView:Boolean=false;

		public static var isShowView:Boolean=false;
		public static var isShowItemView:Boolean=false;
		public static var isShowPetView:Boolean=false;
		public static var isShowOtherView:Boolean=false;

		/** 补偿详细界面 */
		public static var isInitDetailsView:Boolean=false;
		public static var isShowDetailsView:Boolean=false;

		/** 资源地址 */
		public static var resourcePath:String="Resources/GameDLC/CompensateStorage.swf";
		/** 资源 */
		public static var domain:ApplicationDomain=null;

		/** 图标 */
		public static var tuBiaoDic:Dictionary=new Dictionary();
		/** 宠物 type与name关联表 */
		public static var petTypeAndNameList:Dictionary=null;

		/** 请求补偿物品列表 */
		public static function getCompensateList():void
		{
			if (!isRequestData)
			{
				clearListData();
				CompensateStorageSend.listSend(null);
			}
			else
			{
				GameCommonData.UIFacadeIntance.sendNotification(CompensateStorageConst.SHOW_COMPENSATESTORGE_VIEW);
			}
		}

		/** 向服务器请求提取物品 */
		public static function getOut():void
		{
			if (CompensateStorageData.isShowDetailsView)
			{
				GameCommonData.UIFacadeIntance.sendNotification(CompensateStorageConst.CLOSE_COMPENSATESTORGE_DETAILS_VIEW);
			}

			if (CompensateStorageData.selectedId)
			{
				CompensateStorageSend.getCompensateById(CompensateStorageData.selectedId);
				CompensateStorageData.selectedId=0;
				CompensateStorageData.selectedItem=null;
			}
		}

		/** 清空日志 */
		public static function clearLog():void
		{
			CompensateStorageData.compensateDetails="";
		}



		/** 清空补偿列表数据 */
		public static function clearListData():void
		{
			/** 补偿数据  */
			CompensateStorageData.dataList=[];
			/** 道具数据 */
			CompensateStorageData.itemList=[];
			/** 宠物数据 */
			CompensateStorageData.petList=[];

			/** 非绑定货币 */
			CompensateStorageData.yinLiang=0;
			/** 绑定货币 */
			CompensateStorageData.suiYin=0;
			/** 非绑定人民币 (珠宝)*/
			CompensateStorageData.zhuBao=0;
			/** 绑定人民币 (元宝)*/
			CompensateStorageData.yuanBao=0;
			/** 帮贡 */
			CompensateStorageData.bangGong=0;
			/** 荣誉 */
			CompensateStorageData.rongYu=0;
			/** 经验 */
			CompensateStorageData.exp=0;

			CompensateStorageData.tuBiaoDic["suiYin1"]="\\se";
			CompensateStorageData.tuBiaoDic["suiYin2"]="\\ss";
			CompensateStorageData.tuBiaoDic["suiYin3"]="\\sc";
			CompensateStorageData.tuBiaoDic["yinLiang1"]="\\ce";
			CompensateStorageData.tuBiaoDic["yinLiang2"]="\\cs";
			CompensateStorageData.tuBiaoDic["yinLiang3"]="\\cc";
			CompensateStorageData.tuBiaoDic["yuanBao"]="\\ab";
			CompensateStorageData.tuBiaoDic["zhuBao"]="\\zb";
		}

		/** 设置补偿列表数据 */
		public static function setListData():void
		{
			CompensateStorageData.isRequestData=true;
			var i:int=0;
			for (i=0; i < CompensateStorageData.dataList.length; i++)
			{
				switch (int(CompensateStorageData.dataList[i].type / 10000000))
				{
					/** 道具 */
					case 1:
						CompensateStorageData.itemList.push(CompensateStorageData.dataList[i]);
						break;
					/** 宠物 */
					case 2:
						CompensateStorageData.petList.push(CompensateStorageData.dataList[i]);
						break;
					/** 银两 */
					case 3:
						CompensateStorageData.yinLiang+=CompensateStorageData.dataList[i].nAmount;
						break;
					/** 碎银 */
					case 4:
						CompensateStorageData.suiYin+=CompensateStorageData.dataList[i].nAmount;
						break;
					/** 珠宝 */
					case 5:
						CompensateStorageData.zhuBao+=CompensateStorageData.dataList[i].nAmount;
						break;
					/** 元宝 */
					case 6:
						CompensateStorageData.yuanBao+=CompensateStorageData.dataList[i].nAmount;
						break;
					/** 荣誉 */
					case 7:
						CompensateStorageData.rongYu+=CompensateStorageData.dataList[i].nAmount;
						break;
					/** 帮贡 */
					case 8:
						CompensateStorageData.bangGong+=CompensateStorageData.dataList[i].nAmount;
						break;
					/** 经验 */
					case 9:
						CompensateStorageData.exp+=CompensateStorageData.dataList[i].nAmount;
						break;
				}
			}
		}

		/** 根据宠物类型获取名称 */
		public static function getPetNameByType(type:int):String
		{
			if (CompensateStorageData.petTypeAndNameList == null)
			{
				CompensateStorageData.petTypeAndNameList=GameCommonData.GameInstance.Content.Load(GameConfigData.ModelOffset_XML_SWF).GetDisplayObject()["petTypeAndNameList"];
			}
			if (CompensateStorageData.petTypeAndNameList)
			{
				return CompensateStorageData.petTypeAndNameList[type];
			}
			else
			{
				return "ModelOffsetXML.fla";
			}
		}

		/** 字符串转换 */
		public static function stringTransform(str:String):String
		{
			//2011-07-04 11:11:11,你获得了官方奖励补偿，$10000银两$.
			var index1:int=str.indexOf(",");
			var str1:String=str.slice(0, index1); //str1 = "2011-07-04 11:11:11"
			var str1arr1:Array=str1.split(" "); //"2011-07-04" "11:11:11"
			var str1arr2:Array=str1arr1[0].split("-"); //"2011" "07" "04"
			str1="" + int(str1arr2[0]) + CompensateStorageData.word14 /**"年"*/ + int(str1arr2[1]) + CompensateStorageData.word15 /**"月"*/ + int(str1arr2[2]) + CompensateStorageData.word16 /**"日"*/;
			var str1arr3:Array=str1arr1[1].split(":"); //"11" "11" "11"
			str1+="" + str1arr3[0] + ":" + str1arr3[1];
			var str2:String=str.slice(index1); //str2 = ",你获得了官方奖励补偿，$10000银两$."
			var str2arr1:Array=str2.split("$"); //",你获得了官方奖励补偿，" "10000银两" "."
			if (str2arr1.length > 1)
			{
				str2arr1[1]=str2arr1[1].replace(CompensateStorageData.word5 /**"珠宝"*/, "\\zb");
				str2arr1[1]=str2arr1[1].replace(CompensateStorageData.word6 /**"元宝"*/, "\\ab");
				var str2arr1arr1:Array;
				if (str2arr1[1].indexOf(CompensateStorageData.word7 /**"碎银"*/) != -1)
				{
					str2arr1[1]=CompensateStorageData.getSuiYin(int(str2arr1[1].split(CompensateStorageData.word7 /**"碎银"*/)[0]));
				}
				if (str2arr1[1].indexOf(CompensateStorageData.word8 /**"银两"*/) != -1)
				{
					str2arr1[1]=CompensateStorageData.getYinLiang(int(str2arr1[1].split(CompensateStorageData.word8 /**"银两"*/)[0]));
				}
				str2=str2arr1[0] + str2arr1[1] + str2arr1[2];
			}
			str=str1 + str2; //str = "2011年7月4日11:11,你获得了官方奖励补偿，$10000银两$."
			return "<font color='#E2CCA5'>" + str + "</font><br>";
			//2011年07月04日 11:11,你获得了官方奖励补偿，10000银两(图标格式).
		}

		/** 根据碎银量获取对应的字符串 */
		public static function getSuiYin(num:int):String
		{
			var str:String="";
			var suiyin:int=num;
			if (int(suiyin / 10000) > 0)
			{
				str+="" + int(suiyin / 10000) + CompensateStorageData.tuBiaoDic["suiYin1"];
				suiyin=suiyin % 10000;
			}
			if (int(suiyin / 100) > 0)
			{
				str+="" + int(suiyin / 100) + CompensateStorageData.tuBiaoDic["suiYin2"];
				suiyin=suiyin % 100;
			}
			if (suiyin > 0)
			{
				str+="" + suiyin + CompensateStorageData.tuBiaoDic["suiYin3"];
			}
			return str;
		}

		/** 根据银两量获取对应的字符串 */
		public static function getYinLiang(num:int):String
		{
			var str:String="";
			var yinliang:int=num;
			if (int(yinliang / 10000) > 0)
			{
				str+="" + int(yinliang / 10000) + CompensateStorageData.tuBiaoDic["yinLiang1"];
				yinliang=yinliang % 10000;
			}
			if (int(yinliang / 100) > 0)
			{
				str+="" + int(yinliang / 100) + CompensateStorageData.tuBiaoDic["yinLiang2"];
				yinliang=yinliang % 100;
			}
			if (yinliang > 0)
			{
				str+="" + yinliang + CompensateStorageData.tuBiaoDic["yinLiang3"];
			}
			return str;
		}

		/** 从一个数组中删除与另一个数组中相同的元素 */
		public static function arraySub(arr1:Array, arr2:Array, fun:Function):Array
		{
			var arr:Array=arr1.concat();
			var i:int;
			var j:int;
			var length:int=arr2.length;
			for (i=0; i < length; i++)
			{
				for (j=0; j < arr.length; j++)
				{
					if (fun(arr2[i], arr[j]))
					{
						if (arr2[i].nAmount == 0)
						{
							arr.splice(j, 1);
						}
						else
						{
							arr[j].nAmount=arr2[i].nAmount;
						}
					}
				}
			}
			return arr;
		}

		public static function compare(obj1:Object, obj2:Object):Boolean
		{
			return obj1.id == obj2.id;
		}
	}
}
