package
{
	import Controller.AudioController;
	import Controller.CooldownController;
	import Controller.KeyboardController;
	import Controller.PlayerController;
	import Controller.SceneController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Login.SoundUntil.SoundController;
	import GameUI.XmlUtils;
	
	import OopsFramework.Content.Loading.BulkProgressEvent;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	import flash.external.ExternalInterface;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.setTimeout;

	public class GameInit extends BulkLoaderResourceProvider
	{	
        /** 测试代码开始 */	// 孔亮	 // 赵人杰 	//徐敏     //古小鑫 陈明  刘虎
		private var loadPro:LoadProgress;													//这两个黄色的进度条完全是为了测试用
		private var loadProItem:LoadProgress;
/** 测试代码结束 */
		private var soundController:SoundController;
		private var loadItemArr:Array = [];
		//加载地址集合
		private var aResourceURL:Array;
		
		public function GameInit(game:Game)
		{
			super(1, game);
/** 测试代码开始 */				
			loadPro     = new LoadProgress();
			loadProItem = new LoadProgress();
/** 测试代码结束 */	
			setLoadItemArr();
			soundController = new SoundController("" , new Point(900,30) , true , GameCommonData.soundOn_bmp , GameCommonData.soundOff_bmp , true);		//声音开关
//			AudioController.SoundLoginOn();								 				//播放背景音乐  原版本

			///////////////////////   靓仔改音乐
			if ( GameCommonData.loginSoundFromLoader )
			{
				AudioController.SoundLoginOn( GameCommonData.loginSoundFromLoader,GameCommonData.loginSoundChannel,GameCommonData.loginSoundTransform);	
			}
			else
			{
				AudioController.SoundLoginOn();												//播放背景音乐
			}
			/////////////////////////
			
			initURL();													//初始化资源地址
			
			if ( GameCommonData.GameVersion != null )
			{
				if(UIConstData.Filter_Switch == false)
				{
					var filter_url:String = this.Games.Content.RootDirectory + GameConfigData.FilterDic;
					this.Download.Add(  filter_url + GameCommonData.GameVersion,false,filter_url );					// 过滤字典											4
				}
				for ( var i:uint=0; i<aResourceURL.length; i++ )
				{
					var url_version:String = aResourceURL[i] + GameCommonData.GameVersion;
					this.Download.Add( url_version , false , aResourceURL[i] );
				}
				var ui_url:String = this.Games.Content.RootDirectory + GameConfigData.UILibrary;
				this.Download.Add( ui_url + GameCommonData.GameVersion, false ,ui_url,2);								// UI库(SWF)
			}
			else
			{
				if(UIConstData.Filter_Switch == false)
				{
					this.Download.Add( this.Games.Content.RootDirectory + GameConfigData.FilterDic );				// 过滤字典											4
				}
				for ( var j:uint=0; j<aResourceURL.length; j++ )
				{
					var url:String = aResourceURL[j];
					this.Download.Add( url );
				}
				this.Download.Add(this.Games.Content.RootDirectory + GameConfigData.UILibrary,false,null,2);		// UI库(SWF)
			}	
			 
			this.Download.Load();
		}
		
		protected override function onBulkProgress(e:BulkProgressEvent):void 
		{
			if(GameCommonData.Tiao)
			{
				showJindu(e);
			}
			else
			{
/** 测试代码开始 */				
				loadPro.startLoad(e.PercentLoaded,400,400);
				loadProItem.startLoad(e.ItemsLoaded / e.ItemsTotal,400,430);
/** 测试代码结束 */
			}
			super.onBulkProgress(e);
		}
		
		private function showJindu(e:BulkProgressEvent):void
		{
			var current:uint = e.ItemsLoaded;
			var total:uint   = e.ItemsTotal;
			var info:String  = loadItemArr[current];
			if(GameCommonData.Tiao)
			{
				if(info)
				{
					GameCommonData.Tiao.content_txt.text = info;
				}
				GameCommonData.Tiao.tiao_mc.scale_mc.width = e.WeightPercent * 379;

				if(e.ItemsSpeed < 2000)
				{
					GameCommonData.Tiao.numPercent_txt.text    = Math.round(e.WeightPercent * 100) + "%" + " (" + Math.round(e.ItemsSpeed) + "kb/s)"; 
//					GameCommonData.Tiao.num_txt.text = "(" + Math.round(e.ItemsSpeed) + "kb/s)"; 
				}else
				{
					GameCommonData.Tiao.numPercent_txt.text    = Math.round(e.WeightPercent * 100) + "%"+" (" + Math.round(e.ItemsSpeed/1024) + "mb/s)"; ;
//					GameCommonData.Tiao.num_txt.text = "(" + Math.round(e.ItemsSpeed/1024) + "mb/s)"; 
				}
				if ( GameCommonData.wordVersion == 1 )
				{
					( GameCommonData.Tiao.time_txt as TextField ).htmlText = "剩余：<font color='#ffff00'>"+( 20 - Math.floor( Math.round(e.WeightPercent*100)/5 ) )+"</font> 秒";
				}
				else if ( GameCommonData.wordVersion == 2 )
				{
					( GameCommonData.Tiao.time_txt as TextField ).htmlText = "剩餘：<font color='#ffff00'>"+( 20 - Math.floor( Math.round(e.WeightPercent*100)/5 ) )+"</font> 秒";
				}
			}
		}
		
		protected override function onBulkComplete(e:BulkProgressEvent):void
	    {
	    	GameCommonData.IsFirstLoadGame = false;
	    	super.onBulkComplete(e);
	    }  
	    
		/** 下载项完成事件 */
		protected override function onBulkCompleteAll():void 
		{
			super.onBulkCompleteAll();
			
			// 删除加载背景界面
			setTimeout(removeLoad, 100);
			
			if(!GameCommonData.isLoginFromLoader) GameCommonData.GServerInfo = new Object();
			GameCommonData.UserInfo = new Object();
			
			GameCommonData.wordDic = this.GetResource( this.Games.Content.RootDirectory + GameConfigData.Word_Config_SWF ).GetDisplayObject()["wordDic"];					//海外运营文字配置
			UIConstData.ItemDic_1	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.Properties_1).GetDisplayObject()["itemDic"];			// 道具数据
			UIConstData.ItemDic_2	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.Properties_2).GetDisplayObject()["itemDic"];			// 道具数据
			UIConstData.MarketGoodList = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.Market).GetDisplayObject()["marketGoodList"];	 			// 商城数据
			if( GameCommonData.noticeFarmat == 1 )
			{
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				ChatData.CHAT_COLORS		  = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["NOTICE_COLORS"];					//聊天颜色配置数据
				ChatData.NOTICE_ARR 		  = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["NOTICE_HELP_ARR"];				//聊天帮助小提示数据
				ChatData.WELCOME_ARR 		  = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["WELCOME_ARR"];					//欢迎公告数据
				ChatData.NOTICE_HELP_INTERVAL = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["NOTICE_HELP_INTERVAL"];	//帮助小提示时间间隔数据
				ChatData.WELCOME_INTERVAL     = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["WELCOME_INTERVAL"];			//公告时间间隔数据
				ChatData.CHEAT_CHAT_FILTER 	  = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["CHEAT_CHAT_FILTER"];		//聊天防骗过滤字典
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				ChatData.SERVICE_BUSINESS_ID 	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["SERVICE_BUSINESS_ID"];		//运营商编号
				ChatData.FAT_URL 				   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["FAT_URL"];								//防沉迷地址
				ChatData.FATIGUE_STR			   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["FATIGUE_STR"];						//防沉迷说明
				ChatData.OFFICIAL_WEBSITE_ADDR	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["OFFICIAL_WEBSITE_ADDR"];	//游戏官网地址
				ChatData.FORUM_WEBSITE_ADDR 	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["FORUM_WEBSITE_ADDR"];		//论坛地址
				ChatData.DEPOSIT_WEBSITE_ADDR 	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["DEPOSIT_WEBSITE_ADDR"];	//充值地址
				ChatData.GM_INTERFACE_ADDR 		   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["GM_INTERFACE_ADDR"];			//GM工具提交BUG接口地址
				ChatData.NEWER_CARD_INTERFACE_ADDR = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["NEWER_CARD_INTERFACE_ADDR"];//领取新手卡接口地址
				ChatData.FAT_TEST_URL			   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["FAT_TEST_URL"];			//防沉迷平台验证库地址
				ChatData.FAT_CODE			   	   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["FAT_CODE"];				//防沉迷平台密钥
				//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				ChatData.GAME_SCROLL_NOTICE_DIC = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[0]).GetDisplayObject()["GAME_SCROLL_NOTICE_DIC"];		//活动公告数据
			}
			
			//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			if(UIConstData.Filter_Switch == false)
			{
				UIConstData.Filter_ad     = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_ad"];	 // 广告字典
				UIConstData.Filter_chat	  = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_chat"];// 聊天字典
				UIConstData.Filter_role   = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_name"];// 角色字典
				UIConstData.Filter_okName = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.FilterDic).GetDisplayObject()["filter_dic_okName"];// 合格的字符
			}
			UIConstData.TaskTempInfo = this.GetResource(this.Games.Content.RootDirectory + GameConfigData.GameTaskInfoList).GetDisplayObject()["Dic"];// 任务字典
			
			// 后期把多余的写段屏蔽了，等服务器程序通知在修改
			GameCommonData.RolesListDic[0]    = GameCommonData.wordDic[ "often_used_zw" ];           	 //暂无
			GameCommonData.RolesListDic[1]    = GameCommonData.wordDic[ "often_used_tm" ];             //唐门     近战 
			GameCommonData.RolesListDic[2]    = GameCommonData.wordDic[ "often_used_qz" ];              //全真     远战
			GameCommonData.RolesListDic[4]    = GameCommonData.wordDic[ "often_used_em" ];              //峨嵋     远战  
			GameCommonData.RolesListDic[8]    = GameCommonData.wordDic[ "often_used_gb" ];              //丐帮     近战
			GameCommonData.RolesListDic[16]   = GameCommonData.wordDic[ "often_used_sl" ];              //少林     近战
			GameCommonData.RolesListDic[32]   = GameCommonData.wordDic[ "often_used_dc" ];              //点苍     远战
			GameCommonData.RolesListDic[64]   = "法师";       		 // 法师
			GameCommonData.RolesListDic[128]  = "法师";  			// 法师
			GameCommonData.RolesListDic[256]  = "牧师";				//牧师
			GameCommonData.RolesListDic[512]  = "刺客";				// 刺客
			GameCommonData.RolesListDic[1024] = "战士";            // 战士
			GameCommonData.RolesListDic[2048] = "弓手";   			// 弓手
			GameCommonData.RolesListDic[4096] = GameCommonData.wordDic[ "often_used_xs" ];             //新手
			GameCommonData.RolesListDic[8192] = GameCommonData.wordDic[ "often_used_xs" ];				//新手
			
			CooldownController.getInstance();
			
		     XmlUtils.createXml();
			/** 启动MVC  */
			GameCommonData.UIFacadeIntance.StartUp();
			 
			/** 初始化游戏场景*/
			var keyboardController:KeyboardController = new KeyboardController();
			var playerController:PlayerController 	  = new PlayerController();
			GameCommonData.Scene     				  = new SceneController(GameCommonData.enterGameObj.nMapId.toString(),GameCommonData.enterGameObj.nRoleId.toString());		// 初始化游戏场景
//			AudioController.SoundLoginOff();
			GameCommonData.UIFacadeIntance.sendNotification(EventList.ENTERMAPCOMPLETE);
			try
			{
				ExternalInterface.call( "loadGameTitle" );
			}
			catch ( e:Error )
			{
				
			}
			
			soundController.clearSoundSwitch();
		}
		
		private function removeLoad():void
		{ 
/** 测试代码开始 */
			if(loadPro) 
			{
				loadPro.endLoad();
				loadPro = null;
			}
			if(loadProItem)
			{
				loadProItem.endLoad();
				loadProItem = null;
			} 
/** 测试代码结束 */

			if(GameCommonData.BackGround && GameCommonData.GameInstance.GameScene.contains(GameCommonData.BackGround)) 
			{
				GameCommonData.GameInstance.GameScene.removeChild(GameCommonData.BackGround);
				GameCommonData.BackGround = null;
			}
			if(GameCommonData.Tiao && GameCommonData.GameInstance.GameScene.contains(GameCommonData.Tiao)) 
			{
				GameCommonData.GameInstance.GameScene.removeChild(GameCommonData.Tiao);
				GameCommonData.Tiao = null;
			}

			if(GameCommonData.soundOn_bmp) 
			{
				GameCommonData.soundOn_bmp = null; 
			}
			if(GameCommonData.soundOff_bmp) 
			{
				GameCommonData.soundOff_bmp = null;
			}
		}
		
		private function initURL():void
		{
			aResourceURL = [this.Games.Content.RootDirectory + GameConfigData.Properties_1,            		// 物品基本属性(SWF)            		
							this.Games.Content.RootDirectory + GameConfigData.Properties_2,					// 物品基本属性(SWF)

							//新加的4个xml——swf文件
							this.Games.Content.RootDirectory + GameConfigData.Word_Config_SWF,
							this.Games.Content.RootDirectory + GameConfigData.Cloth_XML_SWF,			   	// 套装数据
							this.Games.Content.RootDirectory + GameConfigData.Skill_XML_SWF,			   	// 技能数据
							this.Games.Content.RootDirectory + GameConfigData.ModelOffset_XML_SWF,			// 模型偏移值
							this.Games.Content.RootDirectory + GameConfigData.Other_XML_SWF,			    //  其他一些东西
							
							this.Games.Content.RootDirectory + GameConfigData.GameTaskInfoList,				// 任务数据swf)
							this.Games.Content.RootDirectory + GameConfigData.Market,						// 商城(SWF)	
							this.Games.Content.RootDirectory + GameConfigData.NOTICE_DATA[GameCommonData.noticeFarmat-1]		   	 		// 公告数据
							]
		}
		
		private function setLoadItemArr():void
		{
			if ( GameCommonData.wordVersion == 1 )
			{
				if(UIConstData.Filter_Switch == false)
				{
					 loadItemArr = [
				 							"正在加载过滤字典信息(1/12).....",									
				 							"正在加载道具信息(2/12).....",	
				 							"正在加载道具信息(3/12).....",	
				 							"正在加载游戏文本配置(4/12).....",									
							 				"正在加载套装数据(5/12).....",										
							 				"正在加载技能数据(6/12).....",									
							 				"正在加载模型数据(7/12).....",										
							 				"正在加载地图信息(8/12).....",										
							 				"正在加载任务信息(9/12).....",																		
							 				"正在加载商城数据(10/12).....",										
							 				"正在加载公告数据(11/12).....",										
											"正在加载资源库(12/12)....."											
							];
				}
				else
				{
					loadItemArr = [							
				 							"正在加载道具信息(1/11).....",	
				 							"正在加载道具信息(2/11).....",		
				 							"正在加载游戏文本配置(3/11).....",								
							 				"正在加载套装数据(4/11).....",										
							 				"正在加载技能数据(5/11).....",									
							 				"正在加载模型数据(6/11).....",										
							 				"正在加载地图信息(7/11).....",										
							 				"正在加载任务信息(8/11).....",																		
							 				"正在加载商城数据(9/11).....",										
							 				"正在加载公告数据(10/11).....",										
											"正在加载资源库(11/11)....."												
							];
				}
			}
			else if ( GameCommonData.wordVersion == 2 )
			{
				if(UIConstData.Filter_Switch == false)
				{
					 loadItemArr = [
				 							"正在加載過濾字典信息(1/12).....",									
				 							"正在加載道具信息(2/12).....",	
				 							"正在加載道具信息(3/12).....",	
				 							"正在加載遊戲文本配置(4/12).....",									
							 				"正在加載套裝數據(5/12).....",										
							 				"正在加載技能數據(6/12).....",									
							 				"正在加載模型數據(7/12).....",										
							 				"正在加載地圖信息(8/12).....",										
							 				"正在加載任務信息(9/12).....",																		
							 				"正在加載商城數據(10/12).....",										
							 				"正在加載公告數據(11/12).....",										
											"正在加載資源庫(12/12)....."											
							];
				}
				else
				{
					loadItemArr = [							
				 							"正在加載道具信息(1/11).....",	
				 							"正在加載道具信息(2/11).....",		
				 							"正在加載遊戲文本配置(3/11).....",								
							 				"正在加載套裝數據(4/11).....",										
							 				"正在加載技能數據(5/11).....",									
							 				"正在加載模型數據(6/11).....",										
							 				"正在加載地圖信息(7/11).....",										
							 				"正在加載任務信息(8/11).....",																		
							 				"正在加載商城數據(9/11).....",										
							 				"正在加載公告數據(10/11).....",										
											"正在加載資源庫(11/11)....."												
							];
				}
			}
		}
	}
}
