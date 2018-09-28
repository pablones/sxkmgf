package GameUI.Modules.Unity.UnityUtils
{
	/** 没用到 */
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.Unity.Data.UnityNumTopChange;
	import GameUI.Modules.Unity.Mediator.UnityMainMediator;
	
	import flash.display.MovieClip;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;
	
	/** 帮派数据计时器 */
	public class UnityTimer
	{
		private var _mc:MovieClip;
		private var _type:int;
		private var _skillType:int;
		private var obj:Object;
		private var intervalId:uint;					//帮派总建设，繁荣计时
		private var intervalSkillId:uint;				//帮派总建设，技能计时
		
		public var levelUp:Function;					//升级方法委托
		
		public static var onlyOne:Boolean = false;
		public function UnityTimer(mc:MovieClip , type:int , skillType:int)
		{
			_mc = mc;
			_type = type;
			this._skillType = skillType;
			obj = UnityConstData.otherUnityArray[type];
		}
		/** 开始计时，每分钟数值变化 */
		public function startTimer():void
		{
			if(onlyOne == true)
			{
				intervalId 		=  	setInterval(showBar, 1000 * 1, 2 );
				intervalSkillId =   setInterval(unityTimer , 1000 * 1 ,1);
				return;
			}
			intervalId = setInterval(unityTimer, 1000 * 1 , 2 );
			intervalSkillId =setInterval(unityTimer , 1000 * 1 , 1);
			onlyOne = true;
		}
		/** 增加技能计时 */
		public function addSkillTimer(skillType:int):void
		{
			clearInterval(intervalSkillId);
			if(skillType == _skillType)return;
			_skillType = skillType;
			intervalSkillId = setInterval(unityTimer, 1000 * 1 , 1 );
		}
		/** 删除技能计时 */
		public function skillDeleteTimer():void
		{
			clearInterval(intervalSkillId);
		}
		/** 第一次打开面板，每分钟调用一次 */
		/** i=1显示技能  i=2显示建设度，繁荣度*/
		private function unityTimer(type:int):void
		{
			//技能
			if(type == 1)
			{
				if(_skillType != 0)
				{
					_mc["txtSkillBar_" + (_skillType-1)].text   = obj["skillStudyNum" + int(_skillType)] + "/" + UnityNumTopChange.UnityOtherChange(obj.level , "skillUp");//技能升级经验
				} 
				skillTimer();									//技能进度条
				showBar(1);	
			}
			else
			{
				UnityConstData.mainUnityDataObj.unityBuilt 	+= int(UnityConstData.greenUnityDataObj.businessmanNum) + int(UnityConstData.whiteUnityDataObj.businessmanNum) + int(UnityConstData.xuanUnityDataObj.businessmanNum) + int(UnityConstData.redUnityDataObj.businessmanNum);
				UnityConstData.mainUnityDataObj.unitybooming 	+= int(UnityConstData.greenUnityDataObj.craftsmanNum) + int(UnityConstData.whiteUnityDataObj.craftsmanNum) + int(UnityConstData.xuanUnityDataObj.craftsmanNum) + int(UnityConstData.redUnityDataObj.craftsmanNum);
				showBar(2);										//显示进度条
			}
		}

		private function skillTimer():void
		{
			for(var i:int = 1;i < UnityConstData.unityOtherNum;i++)
			{
				
//				obj["skillStudyNum" + int(obj.skillStuding)]   	+= obj.masterNum;	
				UnityConstData.otherUnityArray[i]["skillStudyNum" + int(UnityConstData.otherUnityArray[i].skillStuding)]   	+= UnityConstData.otherUnityArray[i].masterNum;	
			}		
			if(_skillType == 0)return;
			_mc["txtSkillBar_" + (_skillType-1)].text   = obj["skillStudyNum" + int(_skillType)] + "/" + UnityNumTopChange.UnityOtherChange(obj.level , "skillUp");	//技能升级经验
			showBar(1);
		}
		
		/** 显示进度条 */
		/** i=1显示技能  i=2显示建设度，繁荣度*/
		private function showBar(type:int):void
		{
			if(type == 1)
			{
				if(_skillType == 0)return;
				
				//技能自动升级
				if(obj["skillStudyNum" + int(_skillType)] >= int(UnityNumTopChange.UnityOtherChange(obj.level , "skillUp")))
				{
					obj["skillStudyNum" + int(_skillType)] = 0;
					obj["skillStudyCurr" + int(_skillType)] += 1;
					_mc["txtSkillLevel_"+ int(_skillType -1)].text   = obj["skillStudySelf"+int(_skillType)]+"/"+obj["skillStudyCurr"+int(_skillType)]+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "skill")	//技能等级
				}
				//监听是否可以升级，技能满级
				if(obj["skillStudyCurr" + int(_skillType)] >= int(UnityNumTopChange.UnityOtherChange(obj.level , "skill")) )
				{
					obj["skillStudyCurr" + int(_skillType)] = int(UnityNumTopChange.UnityOtherChange(obj.level , "skill"));
					skillDeleteTimer();							//删除技能计时
					if(levelUp != null)
					{
						levelUp();
					}
					_mc["mcSkillBar_" + (_skillType-1)].width = 120;
					obj["skillStudyNum" + int(_skillType)] = int(UnityNumTopChange.UnityOtherChange(obj.level , "skillUp"));
						_mc["txtSkillLevel_"+ int(_skillType -1)].text   = obj["skillStudySelf"+int(_skillType)]+"/"+obj["skillStudyCurr"+int(_skillType)]+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "skill")	//技能等级
				}
				_mc["txtSkillBar_" + (_skillType-1)].text   = obj["skillStudyNum" + int(_skillType)] + "/" + UnityNumTopChange.UnityOtherChange(obj.level , "skillUp");//技能升级经验
				_mc["mcSkillBar_" + (_skillType-1)].width  = int(obj["skillStudyNum" + int(_skillType)]) / int(UnityNumTopChange.UnityOtherChange(obj.level , "skillUp")) * 120;
				if(_mc["mcSkillBar_" + (_skillType-1)].width <= 0) _mc["mcSkillBar_" + (_skillType-1)].width = 0;
				else if(_mc["mcSkillBar_" + (_skillType-1)].width >= 120) _mc["mcSkillBar_" + (_skillType-1)].width = 120;
			}
			else
			{
				//建设度
				_mc.txtBulit.text 	= UnityConstData.mainUnityDataObj.unityBuilt+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "bulit");  
				_mc.txtBooming.text = UnityConstData.mainUnityDataObj.unitybooming+"/"+UnityNumTopChange.UnityOtherChange(obj.level , "bulit");								//分堂升级所需的繁荣度
				_mc["mcBulitBar"].width  = int(UnityConstData.mainUnityDataObj.unityBuilt) / int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")) * 133;
				if(_mc["mcBulitBar"].width <= 0) _mc["mcBulitBar"].width = 0;
				else if(_mc["mcBulitBar"].width >= 133) _mc["mcBulitBar"].width = 133;
				//繁荣度
				_mc["mcBoomingBar"].width  = int(UnityConstData.mainUnityDataObj.unitybooming) / int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")) * 133;
				if(_mc["mcBoomingBar"].width <= 0) _mc["mcBoomingBar"].width = 0;
				else if(_mc["mcBoomingBar"].width >= 133) _mc["mcBoomingBar"].width = 133;
				
				//监听是否可以升级
				if(int(UnityConstData.mainUnityDataObj.unitybooming)> int(UnityNumTopChange.UnityOtherChange(obj.level , "bulit")))
				{
					if(levelUp != null)
					{
						levelUp();
					}
				}
			}
		}
	}
}