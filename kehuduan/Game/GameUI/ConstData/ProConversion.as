package GameUI.ConstData
{
	import flash.events.EventDispatcher;
	import flash.events.IEventDispatcher;
	import flash.utils.Dictionary;

	public class ProConversion extends EventDispatcher
	{
		protected static var _instance:ProConversion;
		/** 职业名称  */
		public var RolesListDic:Dictionary = new Dictionary();
		
		public function ProConversion(target:IEventDispatcher=null)
		{
			super(target);
		
			RolesListDic[0]    = GameCommonData.wordDic["often_used_zw"];   
			RolesListDic[1]    = GameCommonData.wordDic["often_used_tm"];              //唐门     近战
			RolesListDic[2]    = GameCommonData.wordDic["often_used_qz"];              //全真     远战
			RolesListDic[4]    = GameCommonData.wordDic["often_used_em"];              //峨嵋     远战  
			RolesListDic[8]    = GameCommonData.wordDic["often_used_gb"];              //丐帮     近战
			RolesListDic[16]   = GameCommonData.wordDic["often_used_sl"];              //少林     近战
			RolesListDic[32]   = GameCommonData.wordDic["often_used_dc"];              //点苍     远战
//			RolesListDic[64]   = "法师";
//			RolesListDic[128]  = "法师";
//			RolesListDic[256]  = "牧师";
//			RolesListDic[512]  = "刺客";
//			RolesListDic[1024] = "战士";
//			RolesListDic[2048] = "弓手";
			RolesListDic[4096] = GameCommonData.wordDic["often_used_xs"];               //新手     近战
//			RolesListDic[8192] = "新手";
		}
		
		public static function getInstance():ProConversion{
			if(_instance==null){
				_instance=new ProConversion();
			}
			return _instance;
		}
		
		public function getJob(type:uint):String{
			return RolesListDic[type].toString();
		}
	
	}
}