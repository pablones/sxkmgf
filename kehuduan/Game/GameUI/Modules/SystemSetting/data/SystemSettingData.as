package GameUI.Modules.SystemSetting.data
{
	public class SystemSettingData
	{
		public static const OPEN_SETTING_UI:String = "openSettingUI";//点击按钮打开系统设置面板
		public static const INIT_SOUND_VIEW:String = "initSoundView";//初始获取系统设置上的movieclip
		public static const CLOSE_SETTING_VIEW:String = "closeSettingView";//关闭系统设置面板
		public static const INIT_SYSTEM_SETTING_DATA:String = "init_system_setting_data";//初始化设定数据
		
		public static var _dataArr:Array = [];
		
		/** 密码输入面板是否打开 */
		public static var pwdInputIsOpen:Boolean = false;
		/** 密码修改面板是否打开 */
		public static var pwdModifyIsOpen:Boolean = false;
		/** 密码操作反馈 */
		public static const SYS_SET_PWD_OP_RETURN:String = "sys_set_pwd_op_return"; 
		
		public function SystemSettingData()
		{
		}
	}
}