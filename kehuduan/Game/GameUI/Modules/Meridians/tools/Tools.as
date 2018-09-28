package GameUI.Modules.Meridians.tools
{
	import Net.ActionSend.PlayerActionSend;
	
	public class Tools
	{
		public static function getTime(time:int):String
		{
			var shi:int = time / 3600;
			var fen:int = time % 3600 /60;
			var miao:int = time % 60;
			/** 
			 * GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_2" ] //“时”
			 * GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ]	//“分”
			 * GameCommonData.wordDic[ "mod_rp_med_em_5" ] //“秒”
			 * */
			if(shi > 0)
			{
				return "" + shi + GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_2" ]+ fen + GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ] + miao +GameCommonData.wordDic[ "mod_rp_med_em_5" ]; //“时” “分” “秒”
			}
			else if(time > 60)
			{
				return ""+ fen +GameCommonData.wordDic[ "mod_rp_dat_rpd_gdt_1" ]+ miao + GameCommonData.wordDic[ "mod_rp_med_em_5" ]; //"分" "秒"
			}
			else
			{
				return ""+ miao + GameCommonData.wordDic[ "mod_rp_med_em_5" ]; //“秒”
			}
		}
		
		/** 向服务起发包
		 * @param roleId  用户ID 自己是 GameCommonData.Player.Role.Id 别人则为别人ID 
		 * @param meridiansType 经脉类型
		 * @param isStrengthProtct 是否强化保护   1 是
		 * @param action 协议编号  140、查看经脉信息，141、提升经脉等级，142、加速完成，143、开始修炼经脉，144、停止经脉修炼，145、强化经脉，146、加入经脉修炼队列，147、移除出经脉修炼队列
		 */		
		public static function showMeridiansNet(roleId:int, meridiansType:int, isStrengthProtct:int, action:int):void
		{
			var data:Array = [0,roleId,meridiansType,isStrengthProtct,0,0,action,0,0];
			var obj:Object = {type:1010,data:data};
			PlayerActionSend.PlayerAction(obj);
		}
	}
}