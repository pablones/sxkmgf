 package Net.ActionProcessor
{
	import Controller.PKController;
	import Controller.PlayerController;
	
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Campaign.Data.CampaignData;
	import GameUI.Modules.Depot.Data.DepotConstData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.UIUtils;
	
	import Net.ActionSend.PlayerActionSend;
	import Net.GameAction;
	import Net.Protocol;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	
	import flash.utils.ByteArray;
	import flash.utils.Dictionary;
	import flash.utils.setTimeout;
	
	/** 获取选择后的玩家角色信息（些时是没有坐标的，只先初始化场景） */
	public class UserInfo extends GameAction
	{
		public override function Processor(bytes:ByteArray):void 
		{		
			//是否要登录切线
			if ( GameConfigData.IsLoginChangeLine )
			{
				isSpecialLineOpen();
				GameConfigData.IsLoginChangeLine = false;
				GameCommonData.GameNets.endGameNet();				//掉线重连
				setTimeout( UIUtils.conectAccServer,500 );
				return;
			}
			
			if(GameCommonData.Player == null)		// 第一次进来时会创建 GameRole对象
		    {
				GameCommonData.Player	   = new GameElementPlayer(GameCommonData.GameInstance);
				GameCommonData.Player.Role = new GameRole();	 
		    }
			
			bytes.position = 4;
 
			var id:uint 		   = bytes.readUnsignedInt();  		//id
			var ucLookFace:uint    = bytes.readUnsignedByte(); 		//头像
			var reName:uint        = bytes.readUnsignedByte(); 		//心情，以后要改成字符      //是否能改名
			var ucCurJob:uint 	   = bytes.readUnsignedByte();		//当前选职业  1主 2副
			var nSex:uint 		   = bytes.readUnsignedByte(); 		//性别 0男  1女
			//bytes.readUnsignedByte();
//			var usHp:uint 			= bytes.readUnsignedShort();
//			var usMaxHp:uint 		= bytes.readUnsignedShort();
//			var usMp:uint 			= bytes.readUnsignedShort();
//			var usMaxMp:uint 		= bytes.readUnsignedShort();
			var usHp:uint 			= bytes.readUnsignedInt();
			var usMaxHp:uint 		= bytes.readUnsignedInt();
			var usMp:uint 			= bytes.readUnsignedInt();
			var usMaxMp:uint 		= bytes.readUnsignedInt();
			var usSp:uint 			= bytes.readUnsignedShort();
			var usMaxSp:uint 		= bytes.readUnsignedShort();
			var usLevel:uint 	   = bytes.readUnsignedShort();		//人物等级
			var usFirJob:uint 	   = bytes.readUnsignedShort();		//主职业
			var usFirJobLev:uint   = bytes.readUnsignedShort();		//主职业等级
			var usFirJobPhase:uint = bytes.readUnsignedShort();		//主职业阶段
			var usSecJob:uint 	   = bytes.readUnsignedShort();
			var usSecJobLev:uint   = bytes.readUnsignedShort();
			var usSecJobPhase:uint = bytes.readUnsignedShort();
			var Ene:uint 		   = bytes.readUnsignedShort();		//活力
			var MaxEne:uint 	   = bytes.readUnsignedShort();
			var Vit:uint 		   = bytes.readUnsignedShort(); 	//精力     活力
			var MaxVit:uint 	   = bytes.readUnsignedShort();
			var reUnityName:uint = bytes.readUnsignedShort();
			var nDress:uint		   = bytes.readUnsignedInt();		//时装
			var nWeapen:uint 	   = bytes.readUnsignedInt();		//武器
			var nMount:uint 	   = bytes.readUnsignedInt();		//坐骑
			var uHoldTitle:uint    = bytes.readUnsignedInt();		//拥有称号
			var uUseTitle:uint 	   = bytes.readUnsignedInt();		//使用
			var uExp:uint		   = bytes.readUnsignedInt();		//经验
			var nPk:uint		   = bytes.readUnsignedInt();		//PK值
			var PkProc:uint		   = bytes.readUnsignedInt();		//PK保护状态
			var nMoney1:uint	   = bytes.readUnsignedInt();		//绑定货币
			var nMoney2:uint	   = bytes.readUnsignedInt();		//非绑定货币
			var nMoney3:uint 	   = bytes.readUnsignedInt();		//赠卷
			var nMoney4:uint	   = bytes.readUnsignedInt();		//绑定元宝
			var nMoney5:uint 	   = bytes.readUnsignedInt();		//非绑定元宝
			var nPackLev:uint	   = bytes.readUnsignedInt();		//背包等级
			var nBankLev:uint	   = bytes.readUnsignedInt();		//仓库等级 			8.9 冯
			/////////////////////////////////////////////////////////////////(范加伟)
			
  			var nTaskMaskHi:uint 	  = bytes.readUnsignedInt();
			var nTaskMaskLow:uint	  = bytes.readUnsignedInt();
			var nDayTaskMaskHi:uint   = bytes.readUnsignedInt();
			var nDayTaskMaskLow:uint  = bytes.readUnsignedInt();
			
			GameCommonData.MaskHi	  = nTaskMaskHi;
			GameCommonData.MaskLow	  = nTaskMaskLow;
			GameCommonData.DayMaskHi  = nDayTaskMaskHi;
			GameCommonData.DayMaskLow = nDayTaskMaskLow;
			/////////////////////////////////////////////////////////////////
			
			var unityId:uint   = bytes.readUnsignedInt();		// 帮会ID
			var unityJob:uint  = bytes.readUnsignedInt(); 		// 帮会职位
			var nVip:uint 	   = bytes.readUnsignedInt();		// 是否为VIP（0不是VIP，1为VIP）
			var onLineAwardTime:uint = bytes.readUnsignedInt();				//在线奖励次数				
			var isShowDress:uint  = bytes.readUnsignedInt();	
			var unityContribution:uint   = bytes.readUnsignedInt();		//帮派贡献度  杨龙改
			var campaignData_0:uint = bytes.readUnsignedInt();	//日程表的完成数据前32位
			var campaignData_1:uint = bytes.readUnsignedInt();	//日程表的完成数据后32位
			
//			trace(isShowDress)
			var nHasNum:uint   = bytes.readUnsignedByte();
			var nStr:uint      = 0;
			
			if(nHasNum == 3)
			{
				//名字
				nStr = bytes.readUnsignedByte();
				var szName:String = bytes.readMultiByte(nStr,GameCommonData.CODE);
			
				//目前无用
				nStr = bytes.readUnsignedByte();
				var szMate:String = bytes.readMultiByte(nStr,GameCommonData.CODE);
				
				nStr = bytes.readUnsignedByte();
				var ucFeel:String = bytes.readMultiByte(nStr,GameCommonData.CODE);
			}

			GameCommonData.canReName = reName;
			GameCommonData.Player.ActionPlayComplete    = PlayerController.onActionPlayComplete;	
			GameCommonData.Player.Role.Sex		        = nSex;
			GameCommonData.Player.Role.Id	  	   	    = id;
			GameCommonData.Player.Role.Name  	  	    = szName; 								// "一二三四五六七八九"
			GameCommonData.Player.Role.NameColor        = "#ffffff";
			GameCommonData.Player.Role.NameBorderColor  = 0x000000;
			GameCommonData.Player.Role.Title            = ucFeel;
			GameCommonData.Player.Role.TitleColor       = 0x00fff6;
			GameCommonData.Player.Role.TitleBorderColor = 0x1b03ff;
			GameCommonData.Player.Role.Level		    = usLevel;
			GameCommonData.Player.Role.HP			    = usHp;
			GameCommonData.Player.Role.MaxHp            = usMaxHp;
			GameCommonData.Player.Role.MP			    = usMp;
			GameCommonData.Player.Role.MaxMp            = usMaxMp;
			GameCommonData.Player.Role.SP               = usSp;
			GameCommonData.Player.Role.MaxSp            = usMaxSp
			GameCommonData.Player.Role.Feel			    = ucFeel;
			GameCommonData.Player.Role.Exp			    = uExp;
			GameCommonData.Player.Role.CurrentJob	    = ucCurJob;
			GameCommonData.Player.Role.MainJob.Job	    = usFirJob;
			GameCommonData.Player.Role.MainJob.Level    = usFirJobLev;
			GameCommonData.Player.Role.ViceJob.Job	    = usSecJob;
			GameCommonData.Player.Role.ViceJob.Level    = usSecJobLev;
			GameCommonData.Player.Role.Vit 			    = Vit;
			GameCommonData.Player.Role.MaxVit		    = MaxVit;
			GameCommonData.Player.Role.MaxEne		    = MaxEne;
			GameCommonData.Player.Role.Ene   		    = Ene;
			GameCommonData.Player.Role.PkValue	        = nPk;
			GameCommonData.Player.Role.PkState		    = PkProc;
			GameCommonData.Player.Role.BindMoney	    = nMoney1;
			GameCommonData.Player.Role.UnBindMoney      = nMoney2;
			GameCommonData.Player.Role.GiveAway	        = nMoney3;
			GameCommonData.Player.Role.BindRMB		    = nMoney4;
			GameCommonData.Player.Role.UnBindRMB        = nMoney5;
			GameCommonData.Player.Role.Face             = ucLookFace;
			GameCommonData.Player.Role.BagLevel		    = nPackLev;
			GameCommonData.Player.Role.DepotLevel       = nBankLev;
			GameCommonData.Player.Role.unityId          = unityId;
			GameCommonData.Player.Role.unityJob         = unityJob;
			GameCommonData.Player.Role.unityContribution= unityContribution;			//杨龙改
			GameCommonData.Player.Role.NameColor        = PKController.GetFontColor(GameCommonData.Player.Role.PkValue);	
			GameCommonData.Player.Role.NameBorderColor  = PKController.GetBorderColor(GameCommonData.Player.Role.PkValue);			
			GameCommonData.Player.Role.Type 		    = GameRole.TYPE_OWNER;
			GameCommonData.Player.Role.VIP				= nVip;
			GameCommonData.Player.Role.OnLineAwardTime = onLineAwardTime;
			GameCommonData.Player.Role.DesignationCall  = uUseTitle;
			CampaignData.finishList = [campaignData_0 , campaignData_1];		//日程表数据数组
			NewUnityCommonData.reUnityName = reUnityName;
		
		    if((isShowDress & 1) > 0)
		    	GameCommonData.Player.Role.IsShowDress = true;
		    else
		        GameCommonData.Player.Role.IsShowDress = false;
		    
			if(GameCommonData.Player.Role.CurrentJob == 1)
			{
				GameCommonData.Player.Role.CurrentJobID = GameCommonData.Player.Role.MainJob.Job;
			}
			else
			{
				GameCommonData.Player.Role.CurrentJobID = GameCommonData.Player.Role.ViceJob.Job;
			}

			GameCommonData.Player.Role.PersonSkinID      = nDress;
			GameCommonData.Player.Role.WeaponSkinID      = nWeapen / 10;	
			GameCommonData.Player.Role.WeaponEffectModel = nWeapen % 10;
			
			GameCommonData.Player.Role.PersonSkinName = nDress.toString();
			GameCommonData.Player.Role.WeaponSkinName = null;
			if(nMount != 0)
			{
				GameCommonData.Player.Role.MountSkinName  = nMount.toString();
			}
			
			BagData.BagNum = [int(GameCommonData.Player.Role.BagLevel / 10000) % 100 + 24,			//物品背包容量
							  int(GameCommonData.Player.Role.BagLevel / 100)   % 100 + 24,
							  int(GameCommonData.Player.Role.BagLevel)		   % 100 + 24];
							  
			PetPropConstData.petBagNum = (GameCommonData.Player.Role.BagLevel/1000000)%100 + 3;		//宠物背包容量
			/** 宠物背包增量  */
			PetPropConstData.addPetBagNum( PetPropConstData.petBagNum );
			DepotConstData.gridCount   = int(GameCommonData.Player.Role.DepotLevel%100) * 6 + 24;	//仓库物品容量
			DepotConstData.petDepotNum = (GameCommonData.Player.Role.DepotLevel/ 100) % 100 + 2 	//仓库宠物栏容量
			 
			/** 创建其它在线玩家集合  */
			GameCommonData.SameSecnePlayerList = new Dictionary();
			
			if(GameCommonData.Tiao)
			{
				if ( GameCommonData.wordVersion == 1 )
				{
					GameCommonData.Tiao.content_txt.text = "已获取玩家信息.....";
				}
				else if ( GameCommonData.wordVersion == 2 )
				{
					GameCommonData.Tiao.content_txt.text = "已獲取玩家信息.....";
				}
			}			
			
			/** 处理进入地图  */
			var obj:Object = new Object();
			var parm:Array = new Array();
			parm.push(0);
			parm.push(GameCommonData.Player.Role.Id);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(0);
			parm.push(PlayerAction.PLAYER_ONLINE);							// 进入地图
			parm.push(0);
			obj.type = Protocol.PLAYER_ACTION;
			obj.data = parm;
			PlayerActionSend.PlayerAction(obj);
		} 
		
		//帮派专线是否开启
		private function isSpecialLineOpen():void
		{
			var lineArr:Array = [];
			var aLineName:Array = [];
			for ( var i:uint=0; i<GameCommonData.GameServerArr.length; i++ )
			{
				lineArr = GameCommonData.GameServerArr[i].split( ":" );
				if ( lineArr[0] == GameConfigData.specialLineName )
				{
					return;
				}
				if ( lineArr[0].toString().length>0 )
				{
					aLineName.push( lineArr[0].toString() );
				}
			}
			for ( var j:uint=0; j<aLineName.length; j++ )
			{
				var index:int = Math.random() * ( aLineName.length-1 );
				GameConfigData.GameSocketName = aLineName[ index ].toString();
			}
		}
	}
}