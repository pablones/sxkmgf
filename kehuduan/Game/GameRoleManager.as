package
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Login.SoundUntil.SoundController;
	import GameUI.UICore.UIFacade;
	
	import OopsFramework.Content.Loading.BulkProgressEvent;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.geom.Point;
	
	public class GameRoleManager extends BulkLoaderResourceProvider
	{	
//		private var loadRole:LoadProgress;						//杨龙的进度条 
		private var soundController:SoundController;
		
		public function GameRoleManager(game:Game)
		{
			super(1, game);
			
//			if(GameCommonData.BackGround)  
//			{
//				GameCommonData.GameInstance.GameScene.addChildAt(GameCommonData.BackGround,0);
//			}

//			loadRole = new LoadProgress();
			this.Download.Add(this.Games.Content.RootDirectory + GameConfigData.UILibraryRole);					// UI库(SWF)
			this.Download.Add(this.Games.Content.RootDirectory + GameConfigData.FilterDic);						// 加载过滤字典
			
			this.Download.Load();
			
			soundController = new SoundController("" , new Point(900,30) , true , GameCommonData.soundOn_bmp , GameCommonData.soundOff_bmp , true);
//			if(GameCommonData.isLoginFromLoader == true)
//			{
//				soundController.soundOnOrOff(false);		//logo加上
//				GameCommonData.isOpenSoundSwitch = false;
//				GameCommonData.isOpenFightSoundSwitch = false;
//			}
		}
		
		protected override function onBulkProgress(e:BulkProgressEvent):void 
		{
//			loadRole.startLoad(e.PercentLoaded,540,480);
			super.onBulkProgress(e);
			//显示杨龙的进度条
			if(GameCommonData.Tiao)
			{
				GameCommonData.Tiao.tiao_mc.scale_mc.width = e.BytesLoaded / e.BytesTotal * 376.3;
				GameCommonData.Tiao.numPercent_txt.text =  Math.round(e.BytesLoaded / e.BytesTotal*100) + "%"; 
				GameCommonData.Tiao.content_txt.text =  "正在加载创建角色信息.....";
				if(e.ItemsSpeed < 1024)
				{
					GameCommonData.Tiao.num_txt.text = Math.round(e.ItemsSpeed) + "kb/s"; 
				}
				else
				{
					GameCommonData.Tiao.num_txt.text = Math.round(e.ItemsSpeed / 1024) + "mb/s";  
				}
			}
		}
		
		protected override function onBulkComplete(e:BulkProgressEvent):void
	    {
	    	super.onBulkComplete(e);
//	    	if(loadRole) loadRole.endLoad();
//			loadRole = null;
			//移除进度条															暂时不需要移除
//			if(GameCommonData.Tiao && GameCommonData.GameInstance.GameScene.contains(GameCommonData.Tiao))  {
//				GameCommonData.GameInstance.GameScene.removeChild(GameCommonData.Tiao);
//				GameCommonData.Tiao = null;
//			}
	    } 
	    
		/** 下载项完成事件 */
		protected override function onBulkCompleteAll():void 
		{
			soundController.clearSoundSwitch();
			GameCommonData.UIFacadeIntance	  = UIFacade.GetInstance(UIFacade.FACADEKEY);
			GameCommonData.UIFacadeIntance.sendNotification(EventList.SHOWCREATEROLE);
			if(GameCommonData.Tiao)
			{
				GameCommonData.Tiao.content_txt.text =  "正在验证角色信息.....";
			}
			UIConstData.Filter_ad      = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_ad"];	 // 广告字典
			UIConstData.Filter_chat	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_chat"];// 聊天字典
			UIConstData.Filter_role    = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_name"];// 角色字典
			UIConstData.Filter_okName  = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_okName"];// 合格的字符
			UIConstData.Filter_Switch  = true;
			/** 启动角色管理command  */
			GameCommonData.UIFacadeIntance.roleStartUp(); 
		}
	}
}