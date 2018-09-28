package GameUI.Modules.PlayerInfo.Mediator
{
	import Controller.DuelController;
	import Controller.PlayerController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.ProConversion;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Friend.command.FriendCommandList;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.mediator.FriendManagerMediator;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.PlayerInfo.Command.QueryPlayerDetailInfoCommand;
	import GameUI.Modules.PlayerInfo.UI.HeadImgList;
	import GameUI.UIConfigData;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.system.System;
	import flash.text.TextField;
	import flash.utils.setTimeout;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class CounterWorkerInfoMediator extends Mediator
	{
		public static const NAME:String="CounterWorkerInfoMediator";
		public static const DEFAULTPOSITION:Point=new Point(300,0);
	
		/** 菜单*/
		protected var menu:MenuItem;
		/** 目标角色 */
		public var role:GameRole;
		/** 头像  */
		protected var face:int=-1;
		/** buff列表 */
		private var buffs:HeadImgList;
		
		public function CounterWorkerInfoMediator()
		{
			super(NAME);
		}
		
		public function get CounterWorkerInfoUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [PlayerInfoComList.INIT_PLAYERINFO_UI,
					PlayerInfoComList.HIDE_COUNTERWORKER_UI,
					PlayerInfoComList.SELECT_ELEMENT,
					PlayerInfoComList.UPDATE_COUNTER_INFO,
					PlayerInfoComList.UPDATE_ATTACK,
					PlayerInfoComList.SELECTEDSELF,
					PlayerInfoComList.UPDATE_COUNTERWORKER_BUFF
					];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case PlayerInfoComList.INIT_PLAYERINFO_UI:
					facade.registerCommand(PlayerInfoComList.QUERY_PLAYER_DETAILINFO,QueryPlayerDetailInfoCommand);
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"Counter"});
					this.CounterWorkerInfoUI.mouseEnabled=false;
					this.menu=new MenuItem();
					this.buffs=new HeadImgList();
					this.menu.addEventListener(MenuEvent.Cell_Click,onClickHandler);
					this.buffs.x=155;
					this.buffs.y=46;
					(this.CounterWorkerInfoUI.mc_headImg as MovieClip).addEventListener(MouseEvent.CLICK,onShowMenuHandler);
					break;
				case PlayerInfoComList.SELECT_ELEMENT:
					this.role=notification.getBody() as GameRole;
					GameCommonData.GameInstance.GameUI.addChild(this.CounterWorkerInfoUI);
					GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.CLICK,onStageClickHandler);
					this.CounterWorkerInfoUI.x=DEFAULTPOSITION.x;
					this.CounterWorkerInfoUI.y=DEFAULTPOSITION.y;
					this.update();
					break;
					
				case PlayerInfoComList.SELECTEDSELF:
					this.role=notification.getBody() as GameRole;
					GameCommonData.GameInstance.GameUI.addChild(this.CounterWorkerInfoUI);
					GameCommonData.GameInstance.GameUI.stage.addEventListener(MouseEvent.CLICK,onStageClickHandler);
					this.CounterWorkerInfoUI.x=DEFAULTPOSITION.x;
					this.CounterWorkerInfoUI.y=DEFAULTPOSITION.y;
					this.update();
					break;
						
				//目标的属性更新	
				case PlayerInfoComList.UPDATE_COUNTER_INFO:
					this.role=notification.getBody() as GameRole;;
					this.delayUpdate();
					break;
				//攻击目标时更新攻击属性	
				case PlayerInfoComList.UPDATE_ATTACK:
					if(GameCommonData.TargetAnimal==null)return;
					this.role=GameCommonData.TargetAnimal.Role;
					this.update();
					break;
				case PlayerInfoComList.HIDE_COUNTERWORKER_UI:
					this.role=null;
					if(GameCommonData.GameInstance.GameUI.contains(this.CounterWorkerInfoUI)){
						if(GameCommonData.GameInstance.GameUI.contains(this.menu)){
							GameCommonData.GameInstance.GameUI.removeChild(this.menu);
						}
						GameCommonData.GameInstance.GameUI.removeChild(this.CounterWorkerInfoUI);
					}
					break;
				case PlayerInfoComList.UPDATE_COUNTERWORKER_BUFF:
					this.showBuff();
					break;					
			}
		}
		
		/**
		 * 显示BUFF与DeBUFF 
		 * 
		 */		
		private function showBuff():void{
			this.buffs.visible=true;
			if(this.role==null)return;
					var arr:Array=[];
					var count:int=1;
					var a:Array=[];
					for each(var deBuff:GameSkillBuff in this.role.DotBuff){
						arr.push({icon:deBuff.TypeID,tip:deBuff.BuffName,isDeBuff:true});
					}
					for each(var buff:GameSkillBuff in this.role.PlusBuff){
						arr.push({icon:buff.TypeID,tip:buff.BuffName,isDeBuff:false});
					}
					var realArr:Array=[];
					var len:uint=Math.min(arr.length,6);
					for(var i:uint=0;i<len;i++){
						a.push(arr[i]);
						if(i==2){
							realArr.push(a);
							a=new Array();
						}
					}
					realArr.push(a);
					this.buffs.dataPro=realArr;
					this.CounterWorkerInfoUI.addChild(this.buffs);	
		}
		
		/**
		 * 更新 
		 * 
		 */		
		protected function update():void{
			if(this.role==null)return;
			this.setMenuStaus();
			switch (role.Type)
			{
				case GameRole.TYPE_OWNER:
					if(GameCommonData.TargetAnimal){
						GameCommonData.TargetAnimal.IsSelect(false);
					}
				case GameRole.TYPE_PLAYER:
					this.updatePlayer();
					this.showBuff();
					break;
				 case GameRole.TYPE_NPC:
					this.updateNPC();
					this.buffs.visible=false;
				 	break;
				 case GameRole.TYPE_ENEMY:
				 	this.updateAnimal();
				 	this.showBuff();
//				 	this.buffs.visible=false;
				 	break;
				 case GameRole.TYPE_PET:
				 	this.updatePet();
				 	this.showBuff();
				 	break;	
			}
		}
		
		protected function setMenuStaus():void{
			if(role.Type==GameRole.TYPE_PLAYER && role.Id!=GameCommonData.Player.Role.Id || role.Type==GameRole.TYPE_PET){
				(this.CounterWorkerInfoUI.mc_headImg as MovieClip).mouseEnabled=true;
			}else{
				(this.CounterWorkerInfoUI.mc_headImg as MovieClip).mouseEnabled=false;
			}
		}
		
		
		/** 设置血条显示类型 */  
		protected function setBlood(type:uint=0):void{
			var redOne:MovieClip=this.CounterWorkerInfoUI.mc_redOne as MovieClip;
			var redTwo:MovieClip=this.CounterWorkerInfoUI.mc_redTwo as MovieClip;
			var redThree:MovieClip=this.CounterWorkerInfoUI.mc_redThree as MovieClip;
			var buleOne:MovieClip=this.CounterWorkerInfoUI.mc_buleOne as MovieClip;
			var yellowOne:MovieClip=this.CounterWorkerInfoUI.mc_yellowOne as MovieClip;
			var gasOne:MovieClip=this.CounterWorkerInfoUI.mc_gasOne as MovieClip;
			var gasTwo:MovieClip=this.CounterWorkerInfoUI.mc_gasTwo as MovieClip;
			var gasThree:MovieClip=this.CounterWorkerInfoUI.mc_gasThree as MovieClip;
			var greenOne:MovieClip=this.CounterWorkerInfoUI.greenOne as MovieClip;
			
			
			
			switch (type){
				case 0:
					redOne.visible=false;
					redTwo.visible=false;
					redThree.visible=false;
					buleOne.visible=false;
					yellowOne.visible=false;
					gasOne.visible=false;
					gasTwo.visible=false;
					gasThree.visible=false;
					greenOne.visible=false;
					break;
					//玩家
				case 1:
					redOne.visible=true;
					redTwo.visible=false;
					redThree.visible=false;
					buleOne.visible=true;
					yellowOne.visible=true;
					gasOne.visible=true;
					gasTwo.visible=true;
					gasThree.visible=true;
					greenOne.visible=false;
					redOne.width=Math.min((role.HP/(role.MaxHp+role.AdditionAtt.MaxHP)),1)*120;
					buleOne.width=Math.min((role.MP/(role.MaxMp+role.AdditionAtt.MaxMP)),1)*120;
					yellowOne.width=Math.min(((role.SP%100)/100),1)*120;
					this.setSpPoint(Math.floor(role.SP/100),role.MaxSp);
					
					break;
					//NPC
				case 2:	
					redOne.width=120;
					redTwo.width=120;
					redThree.width=120;
					redOne.visible=true;
					redTwo.visible=true;
					redThree.visible=true;
					buleOne.visible=false;
					yellowOne.visible=false;
					gasOne.visible=false;
					gasTwo.visible=false;
					gasThree.visible=false;
					greenOne.visible=false;
					break;
				//普通怪	
				case 3:
					redOne.visible=true;
					redTwo.visible=false;
					redThree.visible=false;
					buleOne.visible=false;
					yellowOne.visible=false;
					gasOne.visible=false;
					gasTwo.visible=false;
					gasThree.visible=false;
					greenOne.visible=false;
					redOne.width=Math.min((role.HP/(role.MaxHp+role.AdditionAtt.MaxHP)),1)*120;
					break;
				//中	  或宠物	
				case 4:
					redOne.visible=true;
					redTwo.visible=true;
					redThree.visible=false;
					buleOne.visible=false;
					yellowOne.visible=false;
					gasOne.visible=false;
					gasTwo.visible=false;
					gasThree.visible=false;
					greenOne.visible=false;
					
					var halfBlood:Number=this.role.MaxHp/2;
					var redOneBlood:Number=this.role.HP>halfBlood ? halfBlood :this.role.HP;
					var redTwoBlood:Number=(this.role.HP-halfBlood)>0 ? (this.role.HP-halfBlood) : 0;
					redOne.width=Math.min(redOneBlood/halfBlood,1) *120;
					redTwo.width=Math.min(redTwoBlood/halfBlood,1) *120;
					break;
				//宠物	
				case 6:
					redOne.visible=true;
					greenOne.visible=false;
					redTwo.visible=false;
					redThree.visible=false;
					buleOne.visible=false;
					yellowOne.visible=false;
					gasOne.visible=false;
					gasTwo.visible=false;
					gasThree.visible=false;
					redOne.width=Math.min((role.HP/role.MaxHp),1)*120;
					greenOne.width=Math.min((role.HP/UIConstData.ExpDic[3000+role.Level]),1)*120;
					break;	
				//大	(精英，头目，首领)
				case 5:
					var everyHp:uint=Math.floor(this.role.MaxHp/3)
					var realMaxHp:uint=everyHp*3;
					var currentHp:uint=Math.min(role.HP,realMaxHp);
					
					var hp3:uint=(currentHp-2*everyHp)>=0 ? (currentHp-2*everyHp) :0;
					currentHp-=hp3;
					var hp2:uint=(currentHp-everyHp)>=0 ?(currentHp-everyHp):0;
					currentHp-=hp2;
					var hp1:uint=currentHp;
					redOne.visible=true;
					redOne.width=Math.min((hp1/everyHp),1)*120;
					redTwo.visible=true;
					redTwo.width=Math.min((hp2/everyHp),1)*120;
					redThree.visible=true;
					redThree.width=Math.min((hp3/everyHp),1)*120;
					buleOne.visible=false;
					yellowOne.visible=false;
					gasOne.visible=false;
					gasTwo.visible=false;
					gasThree.visible=false;
					greenOne.visible=false;
					break;		
			}
		}
		
		/**
		 * 廷时更新 
		 * @param delayTime
		 * 
		 */		
		protected function delayUpdate(delayTime:uint=1000):void{
			setTimeout(this.update,delayTime);
		}
	
		/**
		 * 更新宠物 
		 * 
		 */		
		protected function updatePet():void{
			this.setBlood(6);
			this.setLevel(this.role.Level);
			
			if(this.role.Savvy>=7){
				var arr:Array=PetPropConstData.ADV_FACE_TYPE;
				var flag:Boolean=false;
				for each(var obj:* in arr){
					if(obj.face_0==this.role.Face){
						this.setFace(obj.face_1);
						flag=true;
						break;
					}
				}
				if(!flag){
					this.setFace(this.role.Face);
				}
			}else{
				this.setFace(this.role.Face);
			}
			
			
			this.setEmployeement( GameCommonData.wordDic[ "often_used_pet" ] );     //宠物
			this.setName(this.role.Name);
		}
		
		/**
		 *  更新玩家
		 * 
		 */		
		protected function updatePlayer():void{
			this.setBlood(1);
			this.setEmployeement(ProConversion.getInstance().RolesListDic[role.CurrentJobID]);
			this.setLevel(role.Level);
			this.setFace(role.Face);
			this.setName(role.Name);
		}
		
		/**
		 * 更新怪物
		 * 
		 */		
		protected function updateAnimal():void{
			
			if(this.role.idMonsterType<=1000){
				this.setEmployeement( GameCommonData.wordDic[ "often_used_monster" ] );    // 怪物
				this.setBlood(3);
			}else if(this.role.idMonsterType>1000 && this.role.idMonsterType<=2000){
				this.setEmployeement( GameCommonData.wordDic[ "often_used_headfirst" ] );   // 头目
				this.setBlood(5);
			}else if(this.role.idMonsterType>2000 && this.role.idMonsterType<=3000){
				this.setEmployeement( GameCommonData.wordDic[ "often_used_brother" ] );   // 首领
				this.setBlood(5);
			}else if(this.role.idMonsterType>3000){
				if(this.role.idMonsterType>=3451 && this.role.idMonsterType<=3600){
					this.setEmployeement( GameCommonData.wordDic[ "often_used_brother" ] );   // 首领
				}else{
					this.setEmployeement( GameCommonData.wordDic[ "often_used_elite" ] );   // 精英
				} 
				this.setBlood(5);
			}
			this.setLevel(role.Level);
			this.setFace(role.Face);
			this.setName(role.Name);
		}
		
		/**
		 * 更新NPC 
		 * 
		 */		
		protected function updateNPC():void{
			this.setBlood(2);
			this.setEmployeement( GameCommonData.wordDic[ "mod_pla_med_cou_upd_1" ] );   // 大宋
			this.setFace(this.role.Face);
			this.setLevel(this.role.Level);
			this.setName(this.role.Name);
		}
		
		/**
		 * 设置职业
		 * @param value
		 * 
		 */		
		private function setEmployeement(value:String):void{
			var tf:TextField=this.CounterWorkerInfoUI.txt_employeement as TextField;
			tf.mouseEnabled=false;
			if(tf.text==value)return;
			tf.text=value;
		}
		
		/**
		 * 
		 * @param value
		 * 
		 */		
		private function setLevel(value:uint):void{
			var tf:TextField=this.CounterWorkerInfoUI.txt_level as TextField;
			tf.mouseEnabled=false;
			if(uint(tf.text)==value)return;
			tf.text=String(value);
		}
		
		private function setFace(faceId:uint):void{
			if(faceId==0)faceId=150;
			if(faceId==this.face)return;
			this.face=faceId;
			var face:FaceItem=new FaceItem(String(this.face),null,"face", (50/50),new Point(0,0));
			var mc:MovieClip=this.CounterWorkerInfoUI.mc_headImg as MovieClip;
			while(mc.numChildren>0){
				mc.removeChildAt(0);
			}
			mc.addChild(face);
		}
		
		private function setName(value:String):void{
			var tf:TextField=this.CounterWorkerInfoUI.txt_roleName as TextField;
			tf.mouseEnabled=false;
			if(tf.text==value)return;
			tf.text=value;
		}
		
		
	
		/**
		 * 怒气点  
		 * @param type
		 * 
		 */		
		protected function setSpPoint(type:uint,maxSp:uint):void{
			
			var p1:MovieClip=this.CounterWorkerInfoUI.mc_gasOne as MovieClip;
			var p2:MovieClip=this.CounterWorkerInfoUI.mc_gasTwo as MovieClip;
			var p3:MovieClip=this.CounterWorkerInfoUI.mc_gasThree as MovieClip;
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
				
	
		protected function onShowMenuHandler(e:MouseEvent):void{
			var num:uint=GameCommonData.GameInstance.GameUI.numChildren-1;
			GameCommonData.GameInstance.GameUI.setChildIndex(this.CounterWorkerInfoUI,num);
			var localPoint:Point=new Point(this.CounterWorkerInfoUI.mouseX,this.CounterWorkerInfoUI.mouseY);
			var globalPoint:Point=this.CounterWorkerInfoUI.localToGlobal(localPoint);
			
			var m:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if(m!=null){
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			GameCommonData.GameInstance.GameUI.addChild(this.menu);
			
			var myRole:GameRole=GameCommonData.Player.Role;
		 	var role:GameRole=this.role;
			var menuData:Array=[];
			
			if(this.role.Type==GameRole.TYPE_PLAYER){
				menuData.push({cellText:GameCommonData.wordDic[ "often_used_trade" ],data:{type:GameCommonData.wordDic[ "often_used_trade" ]}});   // 交易  交易
				menuData.push({ cellText:GameCommonData.wordDic[ "often_used_follow" ],data:{type:GameCommonData.wordDic[ "often_used_follow" ] }});   //  跟随  跟随
				menuData.push({cellText:GameCommonData.wordDic[ "often_used_emulating" ],data:{type:GameCommonData.wordDic[ "often_used_emulating" ] }});   // 切磋   切磋
				menuData.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ],data:{type:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ]}});   // 设为私聊  设为私聊
				menuData.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_5" ],data:{type:GameCommonData.wordDic[ "mod_chat_med_qui_model_5" ]}});   // 复制姓名  复制姓名
				var friendMediator:FriendManagerMediator=facade.retrieveMediator(FriendManagerMediator.NAME) as FriendManagerMediator;
				var queryObj:Object=friendMediator.searchFriend(friendMediator.dataList,0,0,role.Name);
				if(queryObj==null || queryObj.i==4){
					menuData.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_2" ],data:{type:GameCommonData.wordDic[ "mod_chat_med_qui_model_2" ]}});  // 加为好友  加为好友
				}
				// 对家没有队伍
				if(role.idTeam==0){
					menuData.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ]}});  // 邀请入队  邀请入队
				}else if(GameCommonData.Player.Role.idTeam==0){
					menuData.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ]}});  // 申请入队  申请入队
				}
				menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_1" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_1" ]}});  // 查看属性  查看属性
				menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_2" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_2" ]}});  // 查看宠物  查看宠物
			}else if(this.role.Type==GameRole.TYPE_PET){
				var pet:*=myRole.PetSnapList[this.role.Id];
				if(pet!=null){
					menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_3" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_3" ]}});   // 喂食  喂食
					menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_4" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_4" ]}});   // 驯养  驯养
					menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_5" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_5" ]}});   // 休息  休息
				}else{
					menuData.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}});  // 查看资料  查看资料
				}
			}
			this.menu.dataPro=menuData;
			this.menu.x=globalPoint.x;
			this.menu.y=globalPoint.y;
			e.stopPropagation();
		}  
		
		/**
		 * 菜单事件 
		 * @param e
		 * 
		 */		
		protected function onClickHandler(e:MenuEvent):void{
		
			if(this.role==null)return;
			switch (String(e.cell.data["type"])){
				case GameCommonData.wordDic[ "often_used_trade" ]:    // 交易 
					if(this.role!=null) {
						sendNotification(EventList.APPLYTRADE, {id:role.Id});
					}
					break;
				case GameCommonData.wordDic[ "often_used_follow" ]:   // 跟随
						PlayerController.SetFollowTargetAnimal();
					break;	
				case GameCommonData.wordDic[ "mod_chat_med_qui_model_2" ]:   // 加为好友
					sendNotification(FriendCommandList.ADD_TO_FRIEND,{id:role.Id,name:role.Name});
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ]:  // 邀请入队
					sendNotification(EventList.INVITETEAM,{id:this.role.Id});
					break;	
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ]:   // 申请入队
					sendNotification(EventList.APPLYTEAM,{id:this.role.Id});			
					break;
				case GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ]:  //  设为私聊
					facade.sendNotification(ChatEvents.QUICKCHAT,this.role.Name);
					break;		
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_1" ]:   // 查看属性
					facade.sendNotification(PlayerInfoComList.QUERY_PLAYER_DETAILINFO,{id:this.role.Id});
					break;
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_2" ]:  // 查看宠物
					facade.sendNotification(PetEvent.GET_PETINFO_OF_PLAYER, this.role.Id);
					break;
				case GameCommonData.wordDic[ "mod_chat_med_qui_model_5" ]:   // 复制姓名
					System.setClipboard(this.role.Name);
					break;
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_3" ]:   // 喂食
					sendNotification(PetEvent.PET_FEED_OUTSIDE_INTERFACE);
					break;	
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_4" ]:  // 驯养
					sendNotification(PetEvent.PET_TRAIN_OUTSIDE_INTERFACE);
					break;
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_5" ]:  // 休息
					sendNotification(PetEvent.PET_REST_OUTSIDE_INTERFACE);
					break;
				case GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]:   //  查看资料
					if(this.role.MasterPlayer==null || this.role.MasterPlayer.Role==null)return;
					var ownerId:uint = this.role.MasterPlayer.Role.Id;
					sendNotification(PetEvent.PET_LOOK_OUTSIDE_INTERFACE, {petId:this.role.Id, ownerId:ownerId});
					break;
				case GameCommonData.wordDic[ "often_used_emulating" ]:   // 切磋
					DuelController.InitiateDuel(GameCommonData.TargetAnimal);
					break;	
			}
		}
		
		protected function onStageClickHandler(e:MouseEvent):void{
			if(this.menu==null)return;
			if(this.CounterWorkerInfoUI.stage==null)return;
			if(GameCommonData.GameInstance.GameUI.contains(this.menu)){
				GameCommonData.GameInstance.GameUI.removeChild(this.menu);
			}
		}	
		
	}
}