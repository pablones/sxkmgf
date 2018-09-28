package GameUI.Modules.PlayerInfo.Mediator
{
	import Controller.TargetController;
	import Controller.TerraceController;
	
	import GameUI.ConstData.EventList;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.ReName.Data.ReNameData;
	import GameUI.Modules.ReName.Mediator.ReNameMediator;
	import GameUI.Modules.RoleProperty.Mediator.UI.PlayerAttribute;
	import GameUI.Modules.Stall.Data.StallConstData;
	import GameUI.Modules.Stall.Data.StallEvents;
	import GameUI.Modules.Task.Commamd.TaskCommandList;
	import GameUI.Modules.Task.Model.TaskInfoStruct;
	import GameUI.Modules.Team.Datas.TeamDataProxy;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.UIUtils;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Role.GameRole;
	
	import OopsFramework.Net.BaseHttp;
	
	import flash.display.Bitmap;
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.text.TextField;
	import flash.utils.Dictionary;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class SelfInfoMediator extends Mediator
	{
		public static const NAME:String="SelfInfoMediator";
		protected var menu:MenuItem;
		protected var timerID:uint;
		protected var face:int=-1;
		protected var request:URLRequest;
		
		public function SelfInfoMediator()
		{
			super(NAME);
		}
		
		public function get SelfInfoUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [PlayerInfoComList.INIT_PLAYERINFO_UI,
					EventList.ENTERMAPCOMPLETE,
					PlayerInfoComList.UPDATE_ATTACK,
					PlayerInfoComList.UPDATE_SELF_INFO,
					TaskCommandList.SEND_FB_AWARD
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case PlayerInfoComList.INIT_PLAYERINFO_UI:
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"Self"});
					this.SelfInfoUI.mouseEnabled=false;
					(this.SelfInfoUI.btn_fullMoney as SimpleButton).addEventListener(MouseEvent.CLICK,onLinkButtonClickHandler);
					if(GameCommonData.cztype==0){
						(this.SelfInfoUI.btn_fullMoney as SimpleButton).visible=true;
					}else if(GameCommonData.cztype==1){
						(this.SelfInfoUI.btn_fullMoney as SimpleButton).visible=false;
					}
					(this.SelfInfoUI.btn_official as SimpleButton).addEventListener(MouseEvent.CLICK,onLinkButtonClickHandler);
					(this.SelfInfoUI.btn_forum as SimpleButton).addEventListener(MouseEvent.CLICK,onLinkButtonClickHandler);
					(this.SelfInfoUI.btn_favor as SimpleButton).addEventListener(MouseEvent.CLICK,onFavorButton);
					(this.SelfInfoUI.btn_pk as SimpleButton).addEventListener(MouseEvent.CLICK,onLinkButtonClickHandler);
					if(GameCommonData.wordVersion == 2)
					{
						var facebook_icon:SimpleButton = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btn_FaceBook");
						facebook_icon.name = "facebook_icon";
						facebook_icon.addEventListener(MouseEvent.CLICK,onFaceBookClick);
						SelfInfoUI.addChild(facebook_icon);
						facebook_icon.x = 20;
						facebook_icon.y = 101;
						if(isFBOpen)
						{
							this.isSendFbAward();
						}
					}
					(this.SelfInfoUI.txt_pk_name as TextField).mouseEnabled = false;
					this.menu=new MenuItem();
					this.menu.addEventListener(MenuEvent.Cell_Click,onCellClickHandler);
					(this.SelfInfoUI.mc_headImg as MovieClip).addEventListener(MouseEvent.CLICK,onHeadClickHandler);
					/**合服改名字*/
					if(GameCommonData.wordVersion == 1)
					{
						facade.registerMediator( new ReNameMediator() );
						sendNotification( ReNameData.INIT_RENAME );	 					
					}
					break;
				case EventList.ENTERMAPCOMPLETE:
					GameCommonData.GameInstance.GameUI.addChild(this.SelfInfoUI);
					initSet();
					initData();
					break;
					
				//初攻击时更新	
				case PlayerInfoComList.UPDATE_ATTACK:
					update();
					break;
					 	 
				//玩家信息更新	
				case PlayerInfoComList.UPDATE_SELF_INFO :
					this.timerID=setTimeout(this.updateAll,1000);	
					break;	
				case TaskCommandList.SEND_FB_AWARD :	//台服fb成就
					decorateBeforeRelease(int(notification.getBody()));
					break;	
			}
		}
		
		protected function onFavorButton(e:MouseEvent):void
		{
			UIUtils.callJava( "addfavor" );
		}
		
		protected function onLinkButtonClickHandler(e:MouseEvent):void{
			if(GameCommonData.cztype==1)return;
			
			if(e.currentTarget===this.SelfInfoUI.btn_fullMoney){
				facade.sendNotification(TerraceController.NAME , "pay");		//充值
			}
			if(e.currentTarget===this.SelfInfoUI.btn_official){
				facade.sendNotification(TerraceController.NAME,"intoOffical");		//进入官网
//				this.request=new URLRequest(ChatData.OFFICIAL_WEBSITE_ADDR);
//				this.request=new URLRequest("http://web.4399.com/yjjh/");
			}
			if(e.currentTarget===this.SelfInfoUI.btn_forum){
				facade.sendNotification(TerraceController.NAME,"intoFurom");		//进入论坛
//				this.request=new URLRequest(ChatData.FORUM_WEBSITE_ADDR); 
//				this.request=new URLRequest("http://bbs.youjia.cn/forum-293-1.html"); 
			}
			if(e.currentTarget===this.SelfInfoUI.btn_pk)
			{
				/** 是否竞技场 */
				var isPKTeam:Boolean = TargetController.IsPKTeam();
				if(isPKTeam)
				{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "Modules_PlayerInfo_Mediator_SelfInfoMediator_1"]/**"该场景不能切换pk模式"*/, color:0xffff00});
				}
				else
				{
					sendNotification(EventList.SHOWPKVIEW);
				}
				e.stopPropagation();
			}
//			navigateToURL(this.request, "_blank");	
		}
		
		/*链接facebook  */
		public static var isFBOpen:Boolean = true;
		
		private var baseHttp:BaseHttp;
		private var bindFBPath:String;
		public var isBindFacebook:Boolean;	//是否已经绑定到fb
		private var isSendFbJudge:Boolean;	//获得fb成就时是否已发送验证
		private var returnState:int;	//发送fb请求后状态 0 可发送状态  1 发送后未返回
		private var account:String;
		private var serverId:String
		private var playerName:String;
		public static var isShowFBNewerHelp:int; 	//是否显示了fb任务接受框 1 绑定了  2 进入直接去网站绑定
		public function onFaceBookClick(e:MouseEvent):void{
			if(!isFBOpen)
			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"敬請期待", color:0xffff00});  
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_7" ], color:0xffff00});  
				return;
			}
			if(isBindFacebook)
			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"FB已經綁定", color:0xffff00});  // FB已經綁定
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_8" ], color:0xffff00});  // FB已經綁定
				return;
			}
			if(returnState == 1)
			{
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"驗證中，請稍等", color:0xffff00}); 
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_9" ], color:0xffff00}); 
				return;
			}
			account = GameCommonData.Accmoute;
			serverId = GameCommonData.ServerId;
			playerName = GameCommonData.Player.Role.Name;
			    /* playerName = "new测试";
				account = "aa112233 ";
				serverId = "S1";  */   
			baseHttp = new BaseHttp("http://yjjh1-s.6mp.com.tw/enter/fb.php"); 
			baseHttp.RequestComplete = requestComplete;
			baseHttp.AddUrlVariables("account" , account);
			baseHttp.AddUrlVariables("id" , serverId);
			baseHttp.AddUrlVariables("nickname" , playerName);
			baseHttp.Submit(); 
			if(isSendFbJudge)
			{
				returnState = 1;
			}
		}
		
		private var awardTag:int;
		protected function requestComplete(data:String):void{
			var tag:String = data.substr(data.indexOf("=> ")+3,1);
			 if(tag == "1")
			{
				isBindFacebook = true;
//				facade.sendNotification(HintEvents.RECEIVEINFO, {info:"FB已經綁定", color:0xffff00});  // FB已經綁定
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_7" ], color:0xffff00});  // FB已經綁定
			} 
			else if(tag == "0" || tag == "2")
			{
				bindFBPath = data.substring(data.lastIndexOf("=> ")+3,data.lastIndexOf(")")-1);
				//提示
				if(isSendFbJudge)
				{
					if(isShowFBNewerHelp == 1)
					{
						isShowFBNewerHelp ++;
						dealAfterSend();
					}
					else
					{
//						facade.sendNotification(EventList.SHOWALERT, {comfrim:dealAfterSend, cancel:new Function(), info:"點擊綁定到facebook，與好友一起分享遊戲的喜悅",title:"提 示" });  // 么？
						facade.sendNotification(EventList.SHOWALERT, {comfrim:dealAfterSend, cancel:new Function(), info:GameCommonData.wordDic[ "fb_NewerHelpUIMediaror_10" ],title:GameCommonData.wordDic[ "mod_npcs_med_npcsm_rpa_3" ] });  // 么？
					}	
				}
			}
			if(!isSendFbJudge)
			{
				isSendFbJudge = true;
			}
			returnState = 0;
			baseHttp = null;6
		}
		
		
		protected function dealAfterSend():void{
			navigateToURL(new URLRequest(bindFBPath),"_blank");
		}
		
		private var awardObj:Object;
		/** 
		 * 
		 * @param taskId
		 * @param type 1 新手训练 2 每日任务  3 加入门派 4 副本任务 5 十级通知 6 邀请好友 7 任务需求 10 从可接任务面板请求
		 * 
		 */		
		 
		public function isSendFbAward(taskId:int = 0,type:int = 0):void
		{
			awardObj = {};
			awardObj.taskId = taskId;
			awardObj.type = type;
			if(!isSendFbJudge)	//第一次发送请求
			{
				onFaceBookClick(null);
				return;
			}
			if(!isBindFacebook && type == 10)	//由任务面板发送
			{
				onFaceBookClick(null);
				return;
			}
			judgeAward();
			
		}
		
		private function judgeAward():void
		{
			if(isBindFacebook)
			{
				if(awardObj)
				{
					if(awardObj.type == 10)	//从任务面板请求fb绑定成功后
					{
						sendNotification(TaskCommandList.DEAL_AFTER_SEND_FB_FORM_TASKPANEL);
						return;
					}
					if(awardObj.taskId)
					{
						sendFacebookAward(1);
					}
				}
			}
			awardObj = null;
		}
		
		public static var isFBScene:Boolean;  //是否进入了副本
		public static var isDuelState:Boolean; //是否是切磋状态
		private function decorateBeforeRelease(awardNum:int):void
		{
			var msg1:String;
			var msg2:String; 
			var msg3:String;
			if(awardNum == 1)
			{
				
			}
			else if(awardNum == 2)
			{
				
			}
			else if(awardNum == 3)	//角色每升級10級時發佈
			{
				msg1 = GameCommonData.Player.Role.Name;
				msg2 = GameCommonData.Player.Role.Level.toString();
			} 
			else if(awardNum == 4)	//第一把武器
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 5)	//第一次戰鬥
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 6)	//第一隻寵物
			{
				msg1 = GameCommonData.Player.Role.Name;
				var pets:Dictionary = GameCommonData.Player.Role.PetSnapList
				for each(var gameRole:GamePetRole in pets)
				{
					msg2 = gameRole.PetName;
				} 
			}
			else if(awardNum == 7)	//第一次使用藥物
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 8)	//第一次加入门派
			{
				msg1 = GameCommonData.Player.Role.Name;
				msg2 = PlayerAttribute.setAttributeDescribe(1);
			}
			else if(awardNum == 9)	//一代名侠
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 10)	//向禮物仙子領獎完畢
			{
				 msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 11)	//副本成功
			{
				msg1 = GameCommonData.Player.Role.Name;
				msg2 = GameCommonData.Scene.gameScenePlay.MapName;
			}
			else if(awardNum == 12)	//副本成功
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 13)	//切磋成功
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 14)	//切磋失败
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			else if(awardNum == 15)	//开始挂机
			{
				msg1 = GameCommonData.Player.Role.Name;
			}
			this.sendFacebookAward(awardNum,msg1,msg2,msg3);
		}
		
		public function sendAwardFormTaskPanel(taskId:int):void
		{
			var taskInfo:TaskInfoStruct = GameCommonData.TaskInfoDic[taskId]
			var tempTxt:TextField = new TextField();
			tempTxt.htmlText = taskInfo.taskName;
			var msg1:String = tempTxt.text;
			tempTxt.htmlText = taskInfo.taskArea;
			var msg2:String	= tempTxt.text;
			tempTxt.htmlText = taskInfo.taskNPC;
			var msg3:String = tempTxt.text;
			if(msg3.indexOf("\\fx") > 0)
			{
				msg3 = msg3.replace("\\fx","");
			}
			var msg4:String = taskInfo.taskLevel.toString();
			sendFacebookAward(taskInfo.id+1000,msg1,msg2,msg3,msg4);
		}
		
		private  function sendFacebookAward(event_id:int,msg1:String = null,msg2:String = null,msg3:String = null,msg4:String = null):void{
			var baseHttp:BaseHttp = new BaseHttp("http://yjjh1-s.6mp.com.tw/enter/fbmessage.php");
			baseHttp.RequestComplete = reqSend; 
			baseHttp.AddUrlVariables("id" , account);
			baseHttp.AddUrlVariables("serverid" , serverId);
			baseHttp.AddUrlVariables("nickname" , playerName);
			baseHttp.AddUrlVariables("event_id" , event_id);
			if(msg1)
			{
				baseHttp.AddUrlVariables("msg1" , msg1);
			}
			if(msg2)
			{
				baseHttp.AddUrlVariables("msg2" , msg2);
			}
			if(msg3)
			{
				baseHttp.AddUrlVariables("msg3" , msg3);
			}   
			if(msg4)
			{
				baseHttp.AddUrlVariables("msg4" , msg4);
			}
			baseHttp.Submit();
			
		} 
		
		
		protected function reqSend(data:String):void{
			trace(data);
		}
		
		public function getFBBindedState():Boolean
		{
			return isBindFacebook;
		}
		/**
		 *获得F的图标 
		 * @return 
		 * 
		 */		
		public function getFBNoBindedIcon():DisplayObject
		{
//			return new this.SelfInfoUI.facebook_icon.constructor() as SimpleButton;
			var fb_icon:SimpleButton = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByButton("btn_FaceBook");
			fb_icon.name = "facebook_iconTask";
			return  fb_icon;
		}
		public function getFBHasBindedIcon():DisplayObject
		{
			var bp:Bitmap = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmap("shareToFacebook");
			var sprite:Sprite = new Sprite();
			sprite.name = "facebook_share";
			sprite.addChild(bp);
			sprite.buttonMode = true;
			return sprite;
		}
		/**
		 * 更新所有属性 
		 * 
		 */		
		protected function updateAll():void{
			var role:GameRole=GameCommonData.Player.Role;
			(this.SelfInfoUI.txt_roleName as TextField).text=GameCommonData.Player.Role.Name;
			(this.SelfInfoUI.txt_level as TextField).text=String(GameCommonData.Player.Role.Level);
			this.changeFace(role.Face);
			this.update();
		}
		
		protected function changeFace(face:uint):void{
			if(this.face==face)return;
			this.face=face;
			var faceItem:FaceItem=new FaceItem(String(face),null,"face",(50/50));
			faceItem.offsetPoint=new Point(0,0);
			var mc:MovieClip=this.SelfInfoUI.mc_headImg as MovieClip
			while(mc.numChildren>0){
				mc.removeChildAt(0);
			}
			mc.addChild(faceItem);
		}
		
		/**
		 * 被攻击时更新玩家属性 
		 * 
		 */		
		protected function update():void{
			var role:GameRole=GameCommonData.Player.Role;
			(this.SelfInfoUI.mc_redOne as MovieClip).width=Math.min((role.HP/(role.MaxHp+role.AdditionAtt.MaxHP)),1)*120;
			(this.SelfInfoUI.mc_buleOne as MovieClip).width=Math.min((role.MP/(role.MaxMp+role.AdditionAtt.MaxMP)),1)*120;
			 
			if(role.MaxSp!=0){
				(this.SelfInfoUI.mc_yellowOne as MovieClip).width=Math.min(((role.SP%100)/100),1)*120;
				this.setSpPoint(Math.floor(role.SP/100),role.MaxSp);
			}else if(role.MaxSp==0){
				(this.SelfInfoUI.mc_yellowOne as MovieClip).width=Math.min(((role.SP%100)/100),1)*120;
				setSpPoint(0,role.MaxSp);
			}
			
			this.changeFace(role.Face);
		}
		/**
		 *  
		 * @param type
		 * 0:全部没有
		 * 
		 */		
		protected function setSpPoint(type:uint,maxSp:uint):void{
			
			var p1:MovieClip=this.SelfInfoUI.mc_gasOne as MovieClip;
			var p2:MovieClip=this.SelfInfoUI.mc_gasTwo as MovieClip;
			var p3:MovieClip=this.SelfInfoUI.mc_gasThree as MovieClip;
			if(maxSp<=99){
				p1.visible=p2.visible=p3.visible=false;
			}else if(maxSp>99 && maxSp<=199){
				p1.visible=true;
				p2.visible=p3.visible=false;
			}else if(maxSp>199 && maxSp<=299){
				p1.visible=p2.visible=true;
				p3.visible=false;
			}else if(maxSp>299){
				p1.visible=p2.visible=true;
				p3.visible=true;
			}
			
			switch(type){
				case 0 :
					p1.gotoAndStop(2);
					p2.gotoAndStop(2);
					p3.gotoAndStop(2);
					break;
				case 1 :
					p1.gotoAndStop(1);
					p2.gotoAndStop(2);
					p3.gotoAndStop(2);
					break;
				case 2 :
					p1.gotoAndStop(1);
					p2.gotoAndStop(1);
					p3.gotoAndStop(2);
					break;
				case 3 :
					p1.gotoAndStop(1);
					p2.gotoAndStop(1);
					p3.gotoAndStop(1);
					break;
				default :
					p1.gotoAndStop(1);
					p2.gotoAndStop(1);
					p3.gotoAndStop(1);									
			}	
		}
		
		/**
		 * 初始化设置 
		 * 
		 */		
		protected function initSet():void{
			(this.SelfInfoUI.txt_roleName as TextField).mouseEnabled=false;
			(this.SelfInfoUI.txt_level as TextField).mouseEnabled=true;
			(this.SelfInfoUI.txt_level as TextField).selectable=false;
			this.SelfInfoUI.stage.addEventListener(MouseEvent.CLICK,onStageMouseClick);
		}
		
		
		/**
		 * 点击舞台 
		 * @param e
		 * 
		 */		
		protected function onStageMouseClick(e:MouseEvent):void{
			if(GameCommonData.GameInstance.GameUI.contains(this.menu)){
				GameCommonData.GameInstance.GameUI.removeChild(this.menu);
			}
		}
		
		/**
		 * 初始化数据 
		 * 
		 */		
		protected function initData():void{
			this.update();		
		}
		
		
		protected function onCellClickHandler(e:MenuEvent):void{
			switch (e.cell.data["type"]){
				case GameCommonData.wordDic[ "mod_pla_med_sel_onc_1" ]:   // 摆摊
					facade.sendNotification(EventList.SHOWSTALL);
					break;
				case GameCommonData.wordDic[ "mod_pla_med_sel_onc_2" ]:   // 自建队伍
					sendNotification(EventList.SETUPTEAMCOMMON);
					break;	
				case GameCommonData.wordDic[ "mod_pla_med_sel_onc_3" ]:   //  退出队伍
					facade.sendNotification(EventList.LEAVETEAMCOMMON);
					break;
				case GameCommonData.wordDic[ "mod_pla_med_sel_onc_4" ]:   //  收摊
					facade.sendNotification(StallEvents.REMOVESTALL);
					break;	
			}	
		}
		
		/**
		 * 点击头像，弹出菜单  
		 * @param e
		 * 
		 */		
		protected function onHeadClickHandler(e:MouseEvent):void{
			var dataProxy:DataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			dataProxy.isSelectSelf=true;
			this.sendNotification(PlayerInfoComList.SELECTEDSELF,GameCommonData.Player.Role);
			GameCommonData.TargetAnimal=null;
			var localPoint:Point=new Point(this.SelfInfoUI.mouseX,this.SelfInfoUI.mouseY);
			var globalPoint:Point=this.SelfInfoUI.localToGlobal(localPoint);
			var m:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if(m!=null){
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			GameCommonData.GameInstance.GameUI.addChild(this.menu);
			StallConstData.stallSelfId!=0
			var teamDataProxy:TeamDataProxy=facade.retrieveProxy(TeamDataProxy.NAME) as TeamDataProxy;
			
			var data:Array=[];
			//有摊
			if(!dataProxy.TradeIsOpen) {	//未在交易中
				if(StallConstData.stallSelfId!=0){
					data.push({cellText:GameCommonData.wordDic[ "mod_pla_med_sel_onc_4" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_sel_onc_4" ]}});   // 收摊   收摊
				}else{
					data.push({cellText:GameCommonData.wordDic[ "mod_pla_med_sel_onc_1" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_sel_onc_1" ]}});    // 摆摊  摆摊
				}
			}	
			
			if(GameCommonData.Player.Role.idTeam!=0){
				data.push({cellText:GameCommonData.wordDic[ "mod_pla_med_sel_onc_3" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_sel_onc_3" ]}});  //  退出队伍  退出队伍
			}else if(GameCommonData.Player.Role.idTeam==0 && !teamDataProxy.isInviting){
				data.push({cellText:GameCommonData.wordDic[ "mod_pla_med_sel_onc_2" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_sel_onc_2" ]}});   // 自建队伍 自建队伍
			}
			this.menu.dataPro=data;	
			this.menu.x=globalPoint.x;
			this.menu.y=globalPoint.y;
			e.stopPropagation();
		}
	}
}