package GameUI.Modules.Pet.Data
{
	import GameUI.Modules.Bag.Proxy.GridUnit;
	
	import OopsEngine.Role.GamePetRole;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFormat;
	import flash.utils.Dictionary;
	
	public class PetPropConstData
	{
		public function PetPropConstData()
		{
		}
		/**是否是改版后的宠物界面*/
		public static var isNewPetVersion:Boolean = true;
		
		/**新版宠物面板模型显示数据*/
		public static var newPetModuleInfo:Dictionary; 
		/**新版宠物icon与Swf对应文件*/
		public static var newPetModuleSwf:Dictionary;
		/**当前显示的宠物(新版宠物)*/
		public static var newCurrentPet:GamePetRole;
		/**新版宠物，是否是查看别人宠物信息*/
		public static var isSearchOtherPetInto:Boolean;
		
		/** 宠物背包当前容量 */
		public static var petBagNum:uint = 0;
		
		/** 格子数据 */
		public static var GridUnitList:Array = new Array();
		
		/** 当前宠物技能数据 */
		public static var gridSkillList:Array = new Array();
		
		/** 选择的格子 */
		public static var SelectedItem:GridUnit = null;
		
		public static var TmpIndex:int = 0;
		
		/** 潜力点缓存 */
		public static var potentials:uint = 0;
		
		/** 临时加点数组 */
		public static var points:Array = [0, 0, 0, 0, 0];
		
		/** 换头像字典 */
		public static const ADV_FACE_TYPE:Array = [
													{face_0:926, face_1:1926},
													{face_0:927, face_1:1927}, 
													{face_0:934, face_1:1934},
													{face_0:936, face_1:1936}, 
													{face_0:937, face_1:1936} 
													]
		
		/** 获得高级头像 */
		public static function getFaceType(face:int):int
		{
			var res:int = face;
			for(var i:int = 0; i < ADV_FACE_TYPE.length; i++) {
				if(face == ADV_FACE_TYPE[i].face_0) {
					res = ADV_FACE_TYPE[i].face_1;
					break;
				}
			}
			return res;
		}
		
//		/** 自己的宠物列表 */
//		public static var petList:Dictionary = new Dictionary();
		
//		/** 当前选择的宠物MC */
//		public static var selectedPetMC:MovieClip = null;
		
		/** 当前选择的宠物 */
		public static var selectedPet:GamePetRole = null;
		
		/** 当前查询的别人的宠物快照列表 */
		public static var petListOthers:Dictionary = new Dictionary();
		
		/** 宠物选择列表是否打开（上交宠物任务） */
		public static var petChooseTaskIsOpen:Boolean = false;
		
		/** 当前选择的列表中宠物ID（上交宠物任务） */
		public static var PetIdSelectedChooseTask:uint = 0;
		
		/** 是否需要请求出战宠物详细信息 */
		public static var needRequestPetInfo:Object = {needToReq:false, petId:0};
		
		/** 是否需要请求出战宠物详细信息 */
		public static function addPetBagNum( oldNum:int ):void
		{
			var level:int = GameCommonData.Player.Role.Level;
			if ( level>=30 )
			{
				var addNum:int = ( level-30 )/20 + 1;
				if ( addNum>4 )
				{
					addNum = 4;
				} 
				oldNum += addNum;
			}
			petBagNum = oldNum;
		}
		
		/**新版宠物（获取资质参数取值）*/
		public static function getAddExtendValue(pet:GamePetRole):Object
		{
			var _grade:Number = pet.ForceApt > pet.SpiritPowerApt ? pet.ForceApt:pet.SpiritPowerApt;
			var _gradeValue:Number;
			var _privityValue:Number;
			//资质参数取值
			if(_grade < 1001)
			{
				_gradeValue = 1.1;
			}
			else if(_grade < 1501)
			{
				_gradeValue = 1.3;
			}
			else if(_grade < 2001)
			{
				_gradeValue = 1.6;
			}
			else if(_grade < 2501)
			{
				_gradeValue = 2.1;
			}
			else if(_grade < 3001)
			{
				_gradeValue = 2.7;
			}
			else if(_grade < 3501)
			{
				_gradeValue = 3.3;
			}
			else if(_grade < 4001)
			{
				_gradeValue = 4.0;
			}
			else if(_grade < 4501)
			{
				_gradeValue = 4.1;
			} 
			else if(_grade <= 10000)
			{
				_gradeValue = 5;
			} 
			//默契参数取值
			if(pet.privity <= 2)
			{
				_privityValue = 1.1;
			}
			else if(pet.privity <= 4)
			{
				_privityValue = 1.3;
			}
			else if(pet.privity <= 6)
			{
				_privityValue = 1.8;
			}
			else if(pet.privity <= 8)
			{
				_privityValue = 2.5;
			}
			else if(pet.privity <= 9)
			{
				_privityValue = 3.5;
			}
			else if(pet.privity <= 10)
			{
				_privityValue = 4.2;
			}
			
			return {grid:_grade,grideValue:_gradeValue,privity:pet.privity,privityValue:_privityValue};
		}
		/**
		 * 设置宠物双面板的位置 
		 * @param mainName GameUI层中的主面板名称
		 * @param ShowView 显示的副面板
		 * 
		 */			
		public static function setViewPosition(mainName:String,showView:DisplayObject):void
		{
			var _neighborDisplay:DisplayObject = GameCommonData.GameInstance.GameUI.getChildByName(mainName);
			if((_neighborDisplay.x + _neighborDisplay.width/2) > GameCommonData.GameInstance.stage.stageWidth/2)
			{
				showView.x = _neighborDisplay.x - showView.width - 10;
			}
			else
			{
				showView.x = _neighborDisplay.x + _neighborDisplay.width + 10;
			}
			showView.y = _neighborDisplay.y;
		}	
		/**
		 * 生成宠物技能文字格子
		 * @param isOpen
		 * @return 
		 * 
		 */		
		public static function getExplainShow(isOpen:Boolean):Sprite
		{
			var expStr:String = GameCommonData.wordDic[ "mod_pet_PetConstData_word_1" ];//"未学习";
			if(!isOpen)
			{
				expStr = GameCommonData.wordDic[ "mod_pet_PetConstData_word_2" ];//"未开启";
			}
			var sp:Sprite = new Sprite();
	   		var min_tf:TextField = new TextField();	
			var min_TF:TextFormat = new TextFormat();
	   		min_tf.text = expStr;
	   		min_tf.width = 30;
	   		min_tf.height = 30;
			min_tf.wordWrap = true;
	   		min_tf.autoSize = TextFieldAutoSize.LEFT;
			min_TF.font = "宋体";
			min_TF.indent = 6;
			min_TF.size = 12;   
			min_TF.color = 0x564C35;
			min_tf.setTextFormat(min_TF);
			min_tf.x = 2;
	   		min_tf.y = 2;
	   		sp.addChild(min_tf);
			sp.mouseChildren = false;
			sp.mouseEnabled = false;
	   		return sp;
		}
	}
	
}