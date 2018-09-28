package GameUI.Modules.Equipment.command
{
	import GameUI.Modules.Equipment.mediator.StrengenMediator;
	
	public class EquipCommandList
	{
		/** 显示装备强化 （升星）*/
		public static const SHOW_EQUIPSTRENGEN_UI:String="SHOW_EQUIPSTRENGEN_UI";
		/** 回复装备强化或加星的结果*/
		public static const RECALL_EQUIPSTRENGEN:String="RECALL_EQUIPSTRENGEN";
		/** 显示装备打孔（镶嵌，取出）*/
		public static const SHOW_EQUIPSTILETTO_UI:String="SHOW_EQUIPSTILETTO_UI";	
		/** 更新失败加成率*/
		public static const UPDATE_FAIL_SCALE:String="UPDATE_FAIL_SCALE";
		/** 装备强化或加星刷新*/
		public static const REFRESH_EQUIP:String="Refresh_Equip";
		/**装备打孔（镶嵌，取出）回复 */
		public static const RECALL_EQUIPSTILETTO:String="RECALL_EQUIPSTILETTO";
		/**宝石合成 */
		public static const COMPOSE_STONE:String="COMPOSE_STONE";
		/** 打开装备升星 */
		public static const SHOW_ADDSTAR_EQUIP:String="SHOW_ADDSTAR_EQUIP";
		
		/** 过场景时移除强化、升星、打孔、镶嵌、取出面板 */
		public static const CLOSE_EQ_PANELS_CHANGE_SENCE:String = "close_eq_panels_change_sence";
		/** 关闭装备打孔，镶嵌，取出面板 */
		public static const CLOSE_EQUIP_STILETTO_PANEL:String="close_equip_stiletto_panel";
		/** 显示combox列表 */
		public static const SHOW_COMBOX_LIST:String="showComboxList";
		/** 回复装备详细信息 */
		public static const RETURN_EQUIP_INFO:String="returnEquipInfo";
		/** 添加强化装备  */
		public static const ADD_STRENGEN_EQUIP:String="addStrengenEquip";
		/** 添加升星装备  */
		public static const ADD_ADDSTAR_EQUIP:String="add_addstar_equip";
		/** 取下强化装备*/
		public static const REMOVE_STRENGEN_EQUIP:String="remove_strengen_equip";
		/** 刷新辅助宝符列表  */
		public static const REFRESH_HELP_ITEM:String="refresh_help_item";
		/** 购买了强化辅助符 */
		public static const BUY_STRENGENHELP_ITEM:String="buyStrengenHelpItem";
		/** 添加强化辅助符 */
		public static const ADD_STRENGEN_ITEM:String="add_strengen_item";
		/** 添加升星辅助符 */
		public static const ADD_STAR_ITEM:String="add_star_item";
		/** 添加打孔符  */
		public static const ADD_STILETTO_ITEM:String="add_stiletto_item";
		/** 添加打孔装备 */
		public static const ADD_STILETTO_EQUIP:String="add_stiletto_equip";
		/** 添加镶嵌装备 */
		public static const ADD_ENCHASE_EQUIP:String="add_enchase_equip";
		/** 添加镶嵌宝石*/
		public static const ADD_ENCHASE_STONE:String="add_enchase_stone";
		/** 添加镶嵌宝符 */
		public static const ADD_ENCHASE_ITEM:String="add_enchase_item";
		/** 添加摘取宝石装备 */
		public static const ADD_EXTIRPATE_EQUIP:String="add_extirpate_equip";
		/** 添加摘取宝石符 */
		public static const ADD_EXTIRPATE_ITEM:String="add_extirpate_item";
		/** 添加魂印装备*/
		public static const ADD_HUNYUN_EQUIP:String="add_hunyun_equip";
		/** 添加魂印符  */
		public static const ADD_HUNYUN_ITEM:String="addHunYunItem";
		/** 回复装备的魂印 */
		public static const RECALL_HUNYUN_EQUIP:String="recall_hunYun_equip";
		/** 添加宝石       */
		public static const ADD_STONECOMPOSE_STONE:String="addStoneComposeStone";
		/** 添加宝石合成符 */
		public static const ADD_STONECOMPOSE_ITEM:String="addStoneComposeItem";
		/** 更新需要的钱   */
		public static const UPDATE_NEEDMONEY_EQUIP:String="updateNeedMoneyEquip";
		/**魂印宝符发生改变 */
		public static const CHANGE_HUNYUN_ITEM:String="changeHunYunItem";
		/** 掉落装备  */
		public static const DROP_EQUIP_ITEM:String="dropEquipItem";
		/**  增加装备 */
		public static const ADD_EQUIP_ITEM:String="addEquipItem";
		/** 关闭装备玩法面板 */
		public static const CLOSE_EQUIP_PANEL:String="closeEquipPanel";
		/**添加装备属性处理完成特效*/
		public static const ADD_SPECIAL_SHOW:String="addSpecialShow";
		/** 收到服务端收到升星命令的回复信息 */
		public static const SERVER_ADDSTAR_UI:String="server_addStar_ui";
		/** 收到服务端收到强化命令的回复信息 */
		public static const SERVER_STRENGTH_UI:String="server_strengen_ui";
		/** 添加雕琢宝符 */
		public static const ADD_DECORATE_ITEM:String="add_decorate_item";
		/** 宝石雕琢服务器返回 */
		public static const STONE_DECORATE_SEND:String="shone_decorate_send";
		
		
		public function EquipCommandList()
		{
			
		}

	}
}