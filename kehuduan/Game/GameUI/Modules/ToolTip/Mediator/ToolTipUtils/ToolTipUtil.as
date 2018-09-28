package GameUI.Modules.ToolTip.Mediator.ToolTipUtils
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.UIUtils;
	
	public class ToolTipUtil
	{
		
		public function ToolTipUtil()
		{
		}
		 
		/**
		 * 还没有装备到身上的装备  
		 * @param data
		 * @return 
		 * 
		 */		
		public static function GetNoEquiedContent(data:Object):Array
		{
			var type:uint = data.type;
			var result:Array = new Array();
			var obj:Object = UIConstData.getItem(type);	
			var s:String = "";
			if(data.id == undefined)
			{
				
				for(var m:int = 0; m<obj.BaseList.length; m++)
				{		
					if(obj.BaseList[m] != 0)
					{	
						s = (int(obj.BaseList[m] / 10000) == 28) ? "%" : ""; 
						var baseName:String = IntroConst.AttDic[int(obj.BaseList[m] / 10000)-1];
						var baseValue:int = int(obj.BaseList[m] % 10000)
						result.push({text:"<font color='#e2cca5'>" + baseName + "：+" + baseValue + s + "</font>"});
					}
				}
			}
			else
			{
				if(int(data.baseAtt1 % 10000) != 0)
				{
					s = (int(data.baseAtt1 / 10000) == 28) ? "%" : ""; 
					var realStren:String = IntroConst.AttDic[int(data.baseAtt1 / 10000)-1] + "：+" + int(data.baseAtt1 % 10000) + s;
					var retStren:String = "";
					if(data.level && data.level > 0) {		//有强化
//						realStren = UIUtils.textfillWithSpace(realStren, 13);
						retStren  = "<font color='#e2cca5'>" + realStren + "</font>";
						realStren += IntroConst.STENS_INCREMENT[data.level].str;
						retStren  += "<font color='" + IntroConst.STENS_INCREMENT[data.level].color + "'>" + IntroConst.STENS_INCREMENT[data.level].str + "</font>";
					} else {
						retStren = "<font color='#e2cca5'>" + realStren + "</font>";
					}
					if(data.isBind && data.isBind == 2) {	//有魂印
//						var spaceLen:uint = 20 - UIUtils.getStrRealLenght(realStren);
//						var tmpSpace:String = "";
//						for(var j:int = 0; j < spaceLen; j++) {
//							tmpSpace += " ";
//						}
//						tmpSpace += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>(+10%)</font>";
//						retStren += tmpSpace;
						retStren += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>" + IntroConst.HUN_YIN_DATA + "</font>";
					}
					result.push({text:retStren});
				}
				if(int(data.baseAtt2 % 10000) != 0)
				{
					s = (int(data.baseAtt2 / 10000) == 28) ? "%" : ""; 
					var realStren_2:String = IntroConst.AttDic[int(data.baseAtt2 / 10000)-1] + "：+" + int(data.baseAtt2 % 10000) + s;
					var retStren_2:String = "";
					if(data.level && data.level > 0) {		//有强化
//						realStren_2 = UIUtils.textfillWithSpace(realStren_2, 13);
						retStren_2 = "<font color='#e2cca5'>" + realStren_2 + "</font>";
						realStren_2 += IntroConst.STENS_INCREMENT[data.level].str;
						retStren_2 += "<font color='" + IntroConst.STENS_INCREMENT[data.level].color + "'>" + IntroConst.STENS_INCREMENT[data.level].str + "</font>";
					} else {
						retStren_2 = "<font color='#e2cca5'>" + realStren_2 + "</font>";
					}
					if(data.isBind && data.isBind == 2) {	//有魂印
//						var spaceLen_2:uint = 20 - UIUtils.getStrRealLenght(realStren_2);
//						var tmpSpace_2:String = "";
//						for(var x:int = 0; x < spaceLen_2; x++) {
//							tmpSpace_2 += " ";
//						}
//						tmpSpace_2 += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>(+10%)</font>";
//						retStren_2 += tmpSpace_2;
						retStren_2 += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>" + IntroConst.HUN_YIN_DATA + "</font>";
					}
					result.push({text:retStren_2});
				}
			}
		/* 	if(data.stoneBaseList!=null){
				for(var k:uint=0;k<data.stoneBaseList.length;k++){
					var baseName1:String = IntroConst.AttDic[int(data.stoneBaseList[k]["stoneAtt"] / 10000)-1];
					var baseValue1:int = int(data.stoneBaseList[k]["stoneAtt"] % 10000);
					var baseStr:String= baseName1 + "：+" + baseValue1;
					baseStr=UIUtils.textfillWithSpace(baseStr,20);
					
					result.push({text:"<font color='#fffe65'>" + baseStr + "</font><font color='#fffe65'>"+data.stoneBaseList[k]["stoneName"]+"</font>"});	
				}		
			} */
			
				
			var i:int = 0;
			//附加属性
			if(data.id == undefined)
			{
				for(i = 0; i<obj.AALIST.length; i++)
				{		
					if(obj.AALIST[i] != 0)
					{	
						if(int(obj.AALIST[i] % 10000) != 0)
						{
							result.push({text:"<font color='#ff7e00'>" + IntroConst.AttDic[int(obj.AALIST[i] / 10000)-1] + "：+" + int(obj.AALIST[i] % 10000) +"</font>"});
						}
//						var appendObj:Object = UIConstData.AppendAttribute[obj.AALIST[i]];
//						result.push({text:"<font color='#ff7e00'>" + appendObj.Name + "：+" + appendObj.Value + "</font>"});
					}
				}
			}  
			else
			{
				if(data.addAtt) {
					for(i = 0; i<data.addAtt.length; i++)
					{
						if(data.addAtt[i] != 0)
						{
							if(int(data.addAtt[i] % 10000) != 0)
							{
								var starStr:String = "";
								var starTmp:String = IntroConst.AttDic[int(data.addAtt[i] / 10000)-1] + "：+" + int(data.addAtt[i] % 10000);
								if(data.star && data.star > 0) {
									starTmp = UIUtils.textfillWithSpace(starTmp, 20);
									var starAdd:int = int(data.addAtt[i] % 10000) - int((Number(data.addAtt[i] % 10000) / IntroConst.STARS_INCREMENT[data.star].str).toFixed(0));
									if(starAdd == 0) starAdd = 1;
									starStr = "<font color='" + IntroConst.STARS_INCREMENT[data.star].color + "'>(+" + starAdd  + ")</font>"
								}
								result.push({text:"<font color='#ff7e00'>" + starTmp +"</font>" + starStr});			//IntroConst.AttDic[int(data.addAtt[i] / 10000)-1] + "：+" + int(data.addAtt[i] % 10000)
							}
						}
					}
				}
			}	
			
			if(data.stoneBaseList!=null){
				for(var k:uint=0;k<data.stoneBaseList.length;k++){
					var baseName1:String = IntroConst.AttDic[int(data.stoneBaseList[k]["stoneAtt"] / 10000)-1];
					var baseValue1:int = int(data.stoneBaseList[k]["stoneAtt"] % 10000);
					var baseStr:String= baseName1 + "：+" + baseValue1;
					baseStr=UIUtils.textfillWithSpace(baseStr,20);
					
					result.push({text:"<font color='#fffe65'>" + baseStr + "</font><font color='#fffe65'>"+data.stoneBaseList[k]["stoneName"]+"</font>"});	
				}		
			} 
			if( data.castSpiritLevel > 0 )
			{
				var castStr:String;
				if( data.castSpiritLevel == 10 )
				{
					castStr = data.castSpiritLevel + GameCommonData.wordDic["mod_tool_cs_1"];//级铸灵
					if( 8 < data.addAttribute && data.addAttribute < 14 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textfillWithSpace(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritDeffAtt[data.castSpiritLevel-1], 20)+ castStr+"</font>"});
					}
					else if( 19 < data.addAttribute && data.addAttribute < 24 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textfillWithSpace(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritAtt[data.castSpiritLevel-1], 20)+castStr +"</font>"});
					}
				}
				else
				{
					castStr = "(" + data.castSpiritCount + "/" + IntroConst.castSpiritLevelCount[data.castSpiritLevel-1] +" "+ data.castSpiritLevel +GameCommonData.wordDic["mod_tool_cs_1"] + ")";//级铸灵
					if( 8 < data.addAttribute && data.addAttribute < 14 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textFillBetweenStr(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritDeffAtt[data.castSpiritLevel-1], castStr, 33)+"</font>"});
					}
					else if( 19 < data.addAttribute && data.addAttribute < 24 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textFillBetweenStr(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritAtt[data.castSpiritLevel-1], castStr, 33) +"</font>"});
					}
				}
			}
			////
			var hideProp:uint = (UIConstData.getItem(data.type).ADes && UIConstData.getItem(data.type).ADes > 0) ? UIConstData.getItem(data.type).ADes : 0;
			if(hideProp > 0) {		//附加高级属性   10.9.2010
				var levNeed:uint  = hideProp % 100 / 10;
				var starNeed:uint = hideProp % 10;
				if(levNeed == 0 && starNeed == 0) {
					result.push({text:"<font color='#00CCCC'>" + UIConstData.getItem(data.type).SDes + "</font>"});
				} else if(data.level != null && data.star != null && data.level >= levNeed && data.star >= starNeed) {			//data.level && data.star && 
					result.push({text:"<font color='#00CCCC'>" + UIConstData.getItem(data.type).SDes + "</font>"});
				} else {
					var startStr:String = GameCommonData.wordDic[ "mod_too_med_too_too_get_1" ];        //隐藏属性：
					var desStr:String = "";
					var endStr:String = GameCommonData.wordDic[ "mod_too_med_too_too_get_2" ];           //时激活
					if(levNeed > 0) {
						desStr += GameCommonData.wordDic[ "mod_too_med_too_too_get_3" ]+levNeed;          //强化到+
					}
					if(starNeed > 0) {
						if(desStr != "") {
							desStr += GameCommonData.wordDic[ "mod_too_med_too_too_get_4" ]+starNeed+GameCommonData.wordDic[ "mod_too_med_too_too_get_5" ];         //且升到          星
						} else {
							desStr += GameCommonData.wordDic[ "mod_too_med_too_too_get_6" ]+starNeed+GameCommonData.wordDic[ "mod_too_med_too_too_get_5" ];           //升到            星
						}
					}
					desStr = startStr + desStr;
					desStr += endStr;
					result.push({text:"<font color='#7a7a7a'>"+desStr+"</font>"});
				}	//隐藏属性：强化到+x且升到x星时激活
			}
			////
//			result.push({text:"<font color='#ff7e00'>物理攻击：+200</font'>"});
//			result.push({text:"<font color='#ff7e00'>物理防御：+200</font'>"});	
			if(obj.Registering != 0)
			{
				var coordinates:Object = UIConstData.CoordinatesEquip[obj.Registering];
				result.push({text:"<font color='#7a7a7a'>" + coordinates.Name + "(0" + "/" + coordinates.Count + ")</font>"});
				var tmpSuitName:Array = [];
				for(var z:int = 0; z<coordinates.EquidList.length; z++)
				{
					if(coordinates.EquidList[z] != 0)
					{
//						var sdf:Object = UIConstData.ItemDic;
						var suitList:Array = coordinates.EquidList[z].split(",");
						var equidObj:Object = UIConstData.getItem(suitList[0]);
						tmpSuitName.push("<font color='#7a7a7a'>  " + UIUtils.textfillWithSpace(equidObj.Name,11) + "</font>");
//						result.push({text:"<font color='#7a7a7a'>	" + equidObj.Name + "</font>"});
					}
				}
				var count:int = 0;
				var strToPush:String = "";
				while(tmpSuitName.length > 0) {
					var tmpStr1:String = tmpSuitName.shift();
					var tmpStr2:String = tmpSuitName.shift();
					if(tmpStr2) {
						strToPush = tmpStr1 + tmpStr2;
					} else {
						strToPush = tmpStr1;
					}
					result.push({text:strToPush});
				}
				for(var n:int = 0; n<coordinates.CeAAList.length;n++)
				{
					if(coordinates.CeAAList[n] != 0)
					{
//						var extendObj:Object =  UIConstData.AppendAttribute[coordinates.CeAAList[n]];
						//套装数据修改
						var extendObj:Object = new Object(); 
						var extendNum:int = coordinates.CeAAList[n] as int;
						if(extendNum > 100000)
						{
							extendObj.Value = int(extendNum % 10000);
							extendObj.Name  = IntroConst.AttDic[int(extendNum / 10000 - 1)];
						}
						else
						{
							extendObj.Value = int(extendNum % 10000);                                                     //by  陈明
							extendObj.Name  = IntroConst.AttDic[int(extendNum / 10000 - 1)];
						}
						/////////
						
						if(coordinates.CeAAList[n] == 0) continue;
						result.push({text:"<font color='#7a7a7a'>("+(n+1)+")" + extendObj.Name + "：+" + extendObj.Value + "</font>"});
					}
				}
			}
			if(obj.Effect != undefined && obj.Effect != 0)
			{
				result.push({text:"<font color='#00c0ff'>" + obj.Effect + "</font>"});
			}
			
			
			if(data.type==144000){
				result.push({text:"<font color='#00CCCC'>"+GameCommonData.wordDic[ "mod_too_med_too_too_get_7" ]+"</font>"});	    //攻击时有很高机率触发人剑合一，打\n出3倍伤害
			}
			
			return result;
		}
		
		
		/**
		 *  已经装备到身上的装备
		 * @param data：obj.BaseList  [数值代号1,数值代号2,数值代号3.....];
		 * 数值代号：前面代表名称，后四位代表数值
		 * @return 
		 * 
		 */		
		
		public static function GetIsEquiedContent(data:Object):Array
		{
			var type:uint = data.type;
			var result:Array = new Array();
			var obj:Object = UIConstData.getItem(type);	
			var s:String = "";
			if(data.id == undefined)
			{
				for(var m:int = 0; m<obj.BaseList.length; m++)
				{		
					if(obj.BaseList[m] != 0)
					{	
						var baseName2:String = IntroConst.AttDic[int(obj.BaseList[m] / 10000)-1];
						var baseValue2:int = int(obj.BaseList[m] % 10000)
						s = (int(obj.BaseList[m] / 10000) == 28) ? "%" : ""; 		//坐骑加%
						result.push({text:"<font color='#e2cca5'>" + baseName2 + "：+" + baseValue2 + s + "</font>"});
					}
				}
			}	
			else
			{
//				result.push({text:"<font color='#ff7e00'>" + IntroConst.AttDic[int(data.baseAtt1 / 10000)-1] + "：+" + int(data.baseAtt1 % 10000) + "</font>"});
//				result.push({text:"<font color='#ff7e00'>" + IntroConst.AttDic[int(data.baseAtt2 / 10000)-1] + "：+" + int(data.baseAtt2 % 10000) + "</font>"});
//				if(int(data.baseAtt1 % 10000) != 0)
//				{
//					s = (int(data.baseAtt1 / 10000) == 28) ? "%" : ""; 
//					result.push({text:"<font color='#e2cca5'>" + IntroConst.AttDic[int(data.baseAtt1 / 10000)-1] + "：+" + int(data.baseAtt1 % 10000) + s + "</font>"});
//				}
//				if(int(data.baseAtt2 % 10000) != 0)
//				{
//					s = (int(data.baseAtt2 / 10000) == 28) ? "%" : ""; 
//					result.push({text:"<font color='#e2cca5'>" + IntroConst.AttDic[int(data.baseAtt2 / 10000)-1] + "：+" + int(data.baseAtt2 % 10000) + s + "</font>"});
//				}
				//-------------------------------------------
				if(int(data.baseAtt1 % 10000) != 0)
				{
					s = (int(data.baseAtt1 / 10000) == 28) ? "%" : ""; 
					var realStren:String = IntroConst.AttDic[int(data.baseAtt1 / 10000)-1] + "：+" + int(data.baseAtt1 % 10000) + s;
					var retStren:String = "";
					if(data.level && data.level > 0) {		//有强化
//						realStren = UIUtils.textfillWithSpace(realStren, 13);
						retStren  = "<font color='#e2cca5'>" + realStren + "</font>";
						realStren += IntroConst.STENS_INCREMENT[data.level].str;
						retStren  += "<font color='" + IntroConst.STENS_INCREMENT[data.level].color + "'>" + IntroConst.STENS_INCREMENT[data.level].str + "</font>";
					} else {
						retStren = "<font color='#e2cca5'>" + realStren + "</font>";
					}
					if(data.isBind && data.isBind == 2) {	//有魂印
//						var spaceLen:uint = 20 - UIUtils.getStrRealLenght(realStren);
//						var tmpSpace:String = "";
//						for(var j:int = 0; j < spaceLen; j++) {
//							tmpSpace += " ";
//						}
//						tmpSpace += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>(+10%)</font>";
//						retStren += tmpSpace;
						retStren += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>" + IntroConst.HUN_YIN_DATA + "</font>";
					}
					result.push({text:retStren});
				}
				if(int(data.baseAtt2 % 10000) != 0)
				{
					s = (int(data.baseAtt2 / 10000) == 28) ? "%" : ""; 
					var realStren_2:String = IntroConst.AttDic[int(data.baseAtt2 / 10000)-1] + "：+" + int(data.baseAtt2 % 10000) + s;
					var retStren_2:String = "";
					if(data.level && data.level > 0) {		//有强化
//						realStren_2 = UIUtils.textfillWithSpace(realStren_2, 13);
						retStren_2 = "<font color='#e2cca5'>" + realStren_2 + "</font>";
						realStren_2 += IntroConst.STENS_INCREMENT[data.level].str;
						retStren_2 += "<font color='" + IntroConst.STENS_INCREMENT[data.level].color + "'>" + IntroConst.STENS_INCREMENT[data.level].str + "</font>";
					} else {
						retStren_2 = "<font color='#e2cca5'>" + realStren_2 + "</font>";
					}
					if(data.isBind && data.isBind == 2) {	//有魂印
//						var spaceLen_2:uint = 20 - UIUtils.getStrRealLenght(realStren_2);
//						var tmpSpace_2:String = "";
//						for(var x:int = 0; x < spaceLen_2; x++) {
//							tmpSpace_2 += " ";
//						}
//						tmpSpace_2 += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>(+10%)</font>";
//						retStren_2 += tmpSpace_2;		
						retStren_2 += "<font color='" + IntroConst.HUN_YIN_COLOR + "'>" + IntroConst.HUN_YIN_DATA + "</font>";
					}
					result.push({text:retStren_2});
				}
				//----------------------------------------------------
				
			}	
			
			/* if(data.stoneBaseList!=null){
				for(var k:uint=0;k<data.stoneBaseList.length;k++){
					var baseName:String = IntroConst.AttDic[int(data.stoneBaseList[k]["stoneAtt"] / 10000)-1];
					var baseValue:int = int(data.stoneBaseList[k]["stoneAtt"] % 10000)
					var baseStr:String= baseName + "：+" + baseValue;
					baseStr=UIUtils.textfillWithSpace(baseStr,20);
					result.push({text:"<font color='#fffe65'>" + baseStr + "</font><font color='#fffe65'>"+data.stoneBaseList[k]["stoneName"]+"</font>"});	
				}		
			} */
			var i:int = 0;
			//附加属性
			if(data.id == undefined)
			{
				for(i = 0; i<obj.AALIST.length; i++)
				{		
					if(obj.AALIST[i] != 0)
					{	
						if(int(obj.AALIST[i] % 10000) != 0)
						{
							result.push({text:"<font color='#ff7e00'>" + IntroConst.AttDic[int(obj.AALIST[i] / 10000)-1] + "：+" + int(obj.AALIST[i] % 10000) +"</font>"});
						}
//						var appendObj:Object = UIConstData.AppendAttribute[obj.AALIST[i]];
//						result.push({text:"<font color='#ff7e00'>" + appendObj.Name + "：+" + appendObj.Value + "</font>"});
					}
				}
			}
			else
			{
				for(i = 0; i<data.addAtt.length; i++)
				{
					if(data.addAtt[i] != 0)
					{
//						result.push({text:"<font color='#e2cca5'>" + IntroConst.AttDic[int(data.addAtt[i] / 10000)-1] + "：+" + int(data.addAtt[i] % 10000) + "</font>"});
						if(int(data.addAtt[i] % 10000) != 0)
						{
							var starStr:String = "";
							var starTmp:String = IntroConst.AttDic[int(data.addAtt[i] / 10000)-1] + "：+" + int(data.addAtt[i] % 10000);
							if(data.star && data.star > 0) {
								starTmp = UIUtils.textfillWithSpace(starTmp, 20);
//								var starAdd:int = int(data.addAtt[i] % 10000) - (int(data.addAtt[i] % 10000) / IntroConst.STARS_INCREMENT[data.star].str);
								var starAdd:int = int(data.addAtt[i] % 10000) - int((Number(data.addAtt[i] % 10000) / IntroConst.STARS_INCREMENT[data.star].str).toFixed(0));
								if(starAdd == 0) starAdd = 1;
								starStr = "<font color='" + IntroConst.STARS_INCREMENT[data.star].color + "'>(+" + starAdd  + ")</font>"
							}
							result.push({text:"<font color='#ff7e00'>" + starTmp +"</font>" + starStr});			//IntroConst.AttDic[int(data.addAtt[i] / 10000)-1] + "：+" + int(data.addAtt[i] % 10000)
//							result.push({text:"<font color='#ff7e00'>" + IntroConst.AttDic[int(data.addAtt[i] / 10000)-1] + "：+" + int(data.addAtt[i] % 10000) + "</font>"});
						}
					}
				}
			}
			//
			////
			var hideProp:uint = (UIConstData.getItem(data.type).ADes && UIConstData.getItem(data.type).ADes > 0) ? UIConstData.getItem(data.type).ADes : 0; 
			if(hideProp > 0) {		//附加高级属性   10.9.2010 
				var levNeed:uint  = hideProp % 100 / 10;
				var starNeed:uint = hideProp % 10;
				if(levNeed == 0 && starNeed == 0) {
					result.push({text:"<font color='#00CCCC'>" + UIConstData.getItem(data.type).SDes + "</font>"});
				} else if(data.level != null && data.star != null && data.level >= levNeed && data.star >= starNeed) {
					result.push({text:"<font color='#00CCCC'>" + UIConstData.getItem(data.type).SDes + "</font>"});
				} else {
					var startStr:String = GameCommonData.wordDic[ "mod_too_med_too_too_get_1" ];             //隐藏属性：
					var desStr:String = "";
					var endStr:String = GameCommonData.wordDic[ "mod_too_med_too_too_get_2" ];                  //时激活
					if(levNeed > 0) {
						desStr += GameCommonData.wordDic[ "mod_too_med_too_too_get_3" ]+levNeed;               //强化到
					}
					if(starNeed > 0) {
						if(desStr != "") {
							desStr += GameCommonData.wordDic[ "mod_too_med_too_too_get_4" ]+starNeed+GameCommonData.wordDic[ "mod_too_med_too_too_get_5" ];      //且升到        星
						} else {
							desStr += GameCommonData.wordDic[ "mod_too_med_too_too_get_6" ]+starNeed+GameCommonData.wordDic[ "mod_too_med_too_too_get_5" ];       //升到           星
						}
					}
					desStr = startStr + desStr;
					desStr += endStr;
					result.push({text:"<font color='#7a7a7a'>"+desStr+"</font>"});
				}	//隐藏属性：强化到+x且升到x星时激活
			}
			//
			if(data.stoneBaseList!=null){
				for(var k:uint=0;k<data.stoneBaseList.length;k++){
					var baseName:String = IntroConst.AttDic[int(data.stoneBaseList[k]["stoneAtt"] / 10000)-1];
					var baseValue:int = int(data.stoneBaseList[k]["stoneAtt"] % 10000)
					var baseStr:String= baseName + "：+" + baseValue;
					baseStr=UIUtils.textfillWithSpace(baseStr,20);
					result.push({text:"<font color='#fffe65'>" + baseStr + "</font><font color='#fffe65'>"+data.stoneBaseList[k]["stoneName"]+"</font>"});	
				}		
			} 
			
			if( data.castSpiritLevel > 0 )
			{
				var castStr:String;
				if( data.castSpiritLevel == 10 )
				{
					castStr = data.castSpiritLevel + GameCommonData.wordDic["mod_tool_cs_1"];//级铸灵
					if( 8 < data.addAttribute && data.addAttribute < 14 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textfillWithSpace(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritDeffAtt[data.castSpiritLevel-1], 20)+ castStr+"</font>"});
					}
					else if( 19 < data.addAttribute && data.addAttribute < 24 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textfillWithSpace(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritAtt[data.castSpiritLevel-1], 20)+castStr +"</font>"});
					}
				}
				else
				{
					castStr = "(" + data.castSpiritCount + "/" + IntroConst.castSpiritLevelCount[data.castSpiritLevel-1] +" "+ data.castSpiritLevel +GameCommonData.wordDic["mod_tool_cs_1"] + ")";//级铸灵
					if( 8 < data.addAttribute && data.addAttribute < 14 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textFillBetweenStr(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritDeffAtt[data.castSpiritLevel-1], castStr, 33)+"</font>"});
					}
					else if( 19 < data.addAttribute && data.addAttribute < 24 )
					{
						result.push({text:"<font color='" + IntroConst.itemColors[7] + "'>" + UIUtils.textFillBetweenStr(IntroConst.AttDic[data.addAttribute-1] +"：+"+ IntroConst.castSpiritAtt[data.castSpiritLevel-1], castStr, 33) +"</font>"});
					}
				}
			}
			
			if(obj.Registering != 0)
			{ 
				var tmpEquipList:Array = new Array();
				for(var z:int = 0; z<RolePropDatas.ItemList.length; z++)
				{
					if(RolePropDatas.ItemList[z] == undefined) continue;
					var tmp:int = UIConstData.getItem(RolePropDatas.ItemList[z].type).Registering;					
					if(obj.Registering == tmp)
					{
						tmpEquipList.push(RolePropDatas.ItemList[z].type);
					}
				}
				var coordinates:Object = UIConstData.CoordinatesEquip[obj.Registering];
				result.push({text:"<font color='#00fe1a'>" + coordinates.Name + "(" + tmpEquipList.length + "/" + coordinates.Count + ")</font>"});
				var tmpSuitName:Array = [];
				for(var y:int = 0; y<coordinates.EquidList.length; y++)
				{
					if(coordinates.EquidList[y] != 0)
					{
						var suitList:Array = coordinates.EquidList[y].split(",");
						var equidObj:Object = UIConstData.getItem(suitList[0]);
						if(testHasEquip(equidObj.Name, tmpEquipList))
						{
							var typeCut:int = equidObj.type / 10000;
							var typeCount:uint = 1;
							if(typeCut == 21 || typeCut == 22) {
								typeCount = getTypeCount(typeCut, tmpEquipList);
							}
							tmpSuitName.push("<font color='#00fe1a'>  " + UIUtils.textfillWithSpace(equidObj.Name+" x "+typeCount, 13) + "</font>");
//							result.push({text:"<font color='#00fe1a'>	" + equidObj.Name + "</font>"});
						}
						else
						{
							tmpSuitName.push("<font color='#7a7a7a'>  " + UIUtils.textfillWithSpace(equidObj.Name, 13) + "</font>");
//							result.push({text:"<font color='#7a7a7a'>	" + equidObj.Name + "</font>"});
						}
					}
				}
				var count:int = 0;
				var strToPush:String = "";
				while(tmpSuitName.length > 0) {
					var tmpStr1:String = tmpSuitName.shift();
					var tmpStr2:String = tmpSuitName.shift();
					if(tmpStr2) {
						strToPush = tmpStr1 + tmpStr2;
					} else {
						strToPush = tmpStr1;
					}
					result.push({text:strToPush});
				}
				for(var n:int = 0; n<coordinates.CeAAList.length; n++)
				{
					if(coordinates.CeAAList[n] != 0)
					{
//						var extendObj:Object =  UIConstData.AppendAttribute[coordinates.CeAAList[n]];
						//套装数据修改
						var extendObj:Object = new Object(); 
						var extendNum:int = coordinates.CeAAList[n] as int;
//						trace("  extendNum  " +extendNum);
						if(extendNum > 100000)
						{
							extendObj.Value = int(extendNum % 10000);
						}
						else
						{
							extendObj.Value = int(extendNum % 10000);                     
						}
						extendObj.Name  = IntroConst.AttDic[int(extendNum / 10000) - 1];
						////////
						
						if(n < tmpEquipList.length)
						{
							result.push({text:"<font color='#00fe1a'>("+(n+1)+")" + extendObj.Name + "：+" + extendObj.Value + "</font>"});//绿色
//							result.push({text:"<font color='#bdff67'>	" + extendObj.Name + "：+" + extendObj.Value + "</font>"});//浅黄
						}
						else
						{
							result.push({text:"<font color='#7a7a7a'>("+(n+1)+")" + extendObj.Name + "：+" + extendObj.Value + "</font>"});
						}
					}
				}
			}	
			if(obj.Effect != undefined && obj.Effect != 0){
				result.push({text:"<font color='#00c0ff'>" + obj.Effect + "</font>"});	
			}
			
			if(data.type==144000){
				result.push({text:"<font color='#00CCCC'>"+GameCommonData.wordDic[ "mod_too_med_too_too_get_7" ]+"</font>"});	        //攻击时有很高机率触发人剑合一，打\n出3倍伤害
			}
			
			return result;
		}
		
		private static function testHasEquip(name:String, list:Array):Boolean
		{
			var bool:Boolean = false;
			for(var i:int = 0; i<list.length; i++)
			{
				if(name == UIConstData.getItem(list[i]).Name)
				{
					return true;
				}
			}
			return false;
		}
		
		private static function getTypeCount(type:uint, list:Array):uint
		{
			var count:uint = 0; 
			for(var i:int = 0; i < list.length; i++) {
				var typeCut:int = int(list[i]) / 10000;
				if(type == typeCut) {
					count++;
				}
			}
			return count;
		}
		
		/** 装备评分 */
		public static function getEquipScore(data:Object):uint
		{
			var score:Number = 0;
			var jobIndex:uint = IntroConst.USER_JOB_ARR.indexOf(GameCommonData.Player.Role.CurrentJobID);
			var itemData:Object = UIConstData.getItem(data.type);	
			//------------------------------------------------------
			//基本属性
			if(int(data.baseAtt1 % 10000) != 0) {
				var baseAtt1:int = int(data.baseAtt1 / 10000);
				var baseValue1:int = int(data.baseAtt1 % 10000);
				score += getSingleScore(baseAtt1, baseValue1, jobIndex);
			}
			if(int(data.baseAtt2 % 10000) != 0) {
				var baseAtt2:int = int(data.baseAtt2 / 10000);
				var baseValue2:int = int(data.baseAtt2 % 10000);
				score += getSingleScore(baseAtt2, baseValue2, jobIndex);
			}
			//附加属性
			if(data.addAtt) {
				for(var i:int = 0; i < data.addAtt.length; i++) {
					if(data.addAtt[i] != 0) {
						var addAtt:int = int(data.addAtt[i] / 10000);
						var addValue:int = int(data.addAtt[i] % 10000);
						if(addValue != 0) {
							score += getSingleScore(addAtt, addValue, jobIndex);
						}
					}
				}
			}
			//宝石属性
			if(data.stoneBaseList != null) {
				for(var k:uint=0; k < data.stoneBaseList.length; k++) {
					var baseName:uint = int(data.stoneBaseList[k]["stoneAtt"] / 10000);
					var stoneValue:int = int(data.stoneBaseList[k]["stoneAtt"] % 10000);
					score += getSingleScore(baseName, stoneValue, jobIndex);
				}
			}

			if(itemData.Registering != 0) {
				var tmpScore:Number = 0;
				var coordinates:Object = UIConstData.CoordinatesEquip[itemData.Registering];
				var suitLen:int = coordinates.CeAAList.length;
				for(var n:int = 0; n < suitLen; n++) {
					if(coordinates.CeAAList[n] != 0) {
						var suitData:int  = coordinates.CeAAList[n] as int;
						var suitType:int  = int(suitData / 10000);
						var suitValue:int = int(suitData % 10000);
						tmpScore += getSingleScore(suitType, suitValue, jobIndex);
					}
				}
				score += tmpScore/coordinates.Count;
			}
			//附加高级属性  神器效果
			var hideProp:uint = (itemData.ADes && itemData.ADes > 0) ? itemData.ADes : 0;
			if(hideProp > 0 || data.type == 144000) {		
				score *= 1.1;
			}
			
			score /= 300;
			
			//铸灵属性
			if(data.castSpiritLevel > 0) 
			{
				if( 8 < data.addAttribute && data.addAttribute < 14 )
				{
					score += IntroConst.castSpiritDeffScore[data.castSpiritLevel-1];
				}
				else if( 19 < data.addAttribute && data.addAttribute < 24 )
				{
					score += IntroConst.castSpiritScore[data.castSpiritLevel-1];
				}
			}
			return uint(score);
		}
		
		/** 单个属性评分 */
		private static function getSingleScore(type:uint, count:uint, jobIndex:uint):Number
		{
			var score:Number = 0;
			if(type != 28) {
				score = (IntroConst.PROP_JOB_ARR[type-1] as Array)[jobIndex] * count;
			}
			return score;
		}
		
	}
}