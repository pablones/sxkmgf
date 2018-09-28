package GameUI.Modules.Login.Data
{
	import flash.display.MovieClip;
	
	public class CreateRoleConstData
	{
		public function CreateRoleConstData()
		{
		}
	
		public static var playerobj:Object = new Object();
		public static var boxIsDisappear:Boolean = false;		/** 消息框是否消息 */
		public static var rolePicList:Array = [];				/** 角色图片缓存*/
		public static var megTotal:int = 6;					/** 最多7条消息 */
		public static var roleLoading:Boolean = false;			/** 加载人物图像 */
		public static var loadNum:int = 0;						/** 点击人物预览时加载的数目 */
		public static var currentLoadMan:MovieClip;				/** 当前加载的人物 */
		public static var currentLoadInfo:MovieClip;			/** 当前加载的简介 */
		public static var roleInfoList:Array = [];				/** 角色简介缓存 */
		public static var roleSoundList:Array = [];				/** 角色声音缓存 */
	}
}