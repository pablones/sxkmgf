package GameUI.Modules.Soul.View
{
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	
	/**
	 * 获取魂魄按钮组件  
	 * @author Administrator
	 * 
	 */	
	public class SoulComponents
	{
		private static var _instance:SoulComponents;
		private var btn_notLearn:Object;//魂魄扩展属性不可以学习按钮
		private var btn_canLearn:Object;//魂魄扩展属性可学习的按钮
		private var btn_hasLearn:Object;//魂魄扩展属性已学习按钮
		private var btn_upProperty:Object;//魂魄扩展属性已学习升级按钮
		private var skillArrow:Object;//技能沟
		private var skillGrid:Object;//技能格子
		private var soulStyle:Object;//魂魄属相格子
		
		public function SoulComponents()
		{
			init();
		}
		
		private function init():void
		{
			this.btn_notLearn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btn_notLearn");
			this.btn_canLearn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btn_canLearn");
			this.btn_hasLearn = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btn_hasLearn");
			this.btn_upProperty = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btn_upProperty");
			this.skillArrow = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("SkillArrow");
			this.skillGrid = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("GridUnit");
			this.soulStyle = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("soulStyle");
		}
		
		/**
		 * 获取单例 
		 * @return 
		 * 
		 */		
		public static function getInstance():SoulComponents
		{
			if(!_instance)
			{
				_instance = new SoulComponents();
			}
			return _instance;
		}
		
		/**
		 *获取不可开槽按钮
		 * @return 
		 * 
		 */		
		public function getNotLearn():SimpleButton
		{
			return new btn_notLearn.constructor();
		}
		
		/**
		 *获取可学习按钮 
		 * @return 
		 * 
		 */		
		public function getCanLearn():SimpleButton
		{
			return new btn_canLearn.constructor();
		}
		
		/**
		 *获取已学习按钮（已使用和已学习） 
		 * @return 
		 * 
		 */		
		public function getHasLearn():SimpleButton
		{
			return new btn_hasLearn.constructor();
		}
		
		/**
		 *获取已学习升级按钮 
		 * @return 
		 * 
		 */		
		public function getUpProperty():SimpleButton
		{
			return new btn_upProperty.constructor();
		}
		/**
		 *获取已学习升级按钮 
		 * @return 
		 * 
		 */		
		public function getSkillArrow():MovieClip
		{
			return new skillArrow.constructor();
		}
		/**
		 *	技能格子框 
		 * @return 
		 * 
		 */		
		public function getSkillGrid():MovieClip
		{
			return new skillGrid.constructor();
		}
		/**
		 *	魂魄属相
		 * @return 
		 * 
		 */		
		public function getSoulStyle():MovieClip
		{
			return new soulStyle.constructor();
		}
		
		

	}
	
}