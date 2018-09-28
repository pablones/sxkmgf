package GameUI.Modules.PlayerInfo.Mediator
{
	import Controller.PlayerSkinsController;
	import Controller.TargetController;
	
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.Pet.Data.PetEvent;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	import GameUI.Modules.PlayerInfo.Command.PlayerInfoComList;
	import GameUI.Modules.PlayerInfo.UI.HeadImgList;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Role.GamePetRole;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class PetInfoMediator extends Mediator
	{
		public static const NAME:String="PetInfoMediator";
		public static const DEFAULT_POS:Point=new Point(65,80);
		
		/**
		 * 菜单 
		 */		
		private var _menu:MenuItem;
		private var _face:uint;
		private var _head:MovieClip;
		private var _role:GameRole;
		private var _buffs:HeadImgList;
		private var _dataProxy:DataProxy;
		/**宠物是否已经出战 */
		private var isShowPet:Boolean;
		
		public function PetInfoMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get petInfoUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
			PlayerInfoComList.INIT_PLAYERINFO_UI,
			EventList.ENTERMAPCOMPLETE,
			PlayerInfoComList.SHOW_PET_UI,
			PlayerInfoComList.REMOVE_PET_UI,
			PlayerInfoComList.UPDATE_PET_UI
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case PlayerInfoComList.INIT_PLAYERINFO_UI:
					facade.sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"PetInfo"});
					this.petInfoUI.mouseEnabled=false;
					this._dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					(this.petInfoUI.txt_level as TextField).mouseEnabled=false;
					(this.petInfoUI.mc_headImg as MovieClip).addEventListener(MouseEvent.CLICK,onHeadClickHandler);
					this._buffs=new HeadImgList(2);
					this._buffs.x=55;
					this._buffs.y=20;
					this._head=this.petInfoUI.mc_headImg;
					this._head.alpha=1;
					this._menu=new MenuItem();
					this.initMenu();
					break;
				case EventList.ENTERMAPCOMPLETE:	
					this.petInfoUI.x=DEFAULT_POS.x;
					this.petInfoUI.y=DEFAULT_POS.y;
					break;
				case PlayerInfoComList.SHOW_PET_UI:
					if(this.isShowPet){
						return;
					}
					this._role=notification.getBody() as GameRole;
					GameCommonData.GameInstance.GameUI.addChild(this.petInfoUI);
					
					this.isShowPet=true;
					this._head.stage.addEventListener(MouseEvent.CLICK,onStageMouseClickHandler);
					this.updatePetInfo();
					break;
				case PlayerInfoComList.REMOVE_PET_UI:
					this._head.stage.removeEventListener(MouseEvent.CLICK,onStageMouseClickHandler);
					if(GameCommonData.GameInstance.GameUI.contains(this.petInfoUI)){
						GameCommonData.GameInstance.GameUI.removeChild(this.petInfoUI);
						this.isShowPet=false;
					}
					break;
				case PlayerInfoComList.UPDATE_PET_UI:
					if(this.isShowPet){
						this.updatePetInfo();
					}
					break;				
			}
		}
		
		private function onStageMouseClickHandler(e:MouseEvent):void{
			if(this._menu.stage!=null){
				GameCommonData.GameInstance.GameUI.removeChild(this._menu);
			}
		}
		
		/**
		 * 更新信息 
		 * 
		 */		
		private function updatePetInfo():void{
			if(GameCommonData.Player.Role.UsingPet==null)return;
			var role:GamePetRole=GameCommonData.Player.Role.UsingPet;
			this.setBoold(role);
			if(this._role.Savvy>=7){
				var arr:Array=PetPropConstData.ADV_FACE_TYPE;
				var flag:Boolean=false;
				for each(var obj:* in arr){
					if(obj.face_0==this._role.Face){
						this.setFace(obj.face_1);
						flag=true;
						break;
					}
				}
				if(!flag){
					this.setFace(this._role.Face);
				}
			}else{
				this.setFace(this._role.Face);
			}
			if( role.Type > 1 )
			{
				var petV:XML = PlayerSkinsController.GetPetV( role.ClassId.toString() , role.Type - 1 ); 
				if( petV != null)
				{
					var face:String = petV.@Face;
					var f:FaceItem = new FaceItem(face,null,"face",30/50);
					f.x = -1;
					f.y = -1;
					while((this.petInfoUI.mc_headImg as MovieClip).numChildren>0)
					{
						(this.petInfoUI.mc_headImg as MovieClip).removeChildAt(0);
					}
					(this.petInfoUI.mc_headImg as MovieClip).addChild(f);
					
				}
			}
			this.setLevel(role.Level);
			this.setBuffs();
		}
		
		private function initMenu():void{
			var dataPro:Array=[
				{ cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_3" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_3" ] } }, 
				{ cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_4" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_4" ] } },
				{ cellText:GameCommonData.wordDic[ "mod_pla_med_cou_ons_5" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_cou_ons_5" ] } }
			];
			this._menu.dataPro=dataPro;
			this._menu.addEventListener(MenuEvent.Cell_Click,onCellClickHandler);
		}
		
		
		private function onCellClickHandler(e:MenuEvent):void{
			switch (e.cell.data["type"]){
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_3" ]:
					sendNotification(PetEvent.PET_FEED_OUTSIDE_INTERFACE);
					break;	
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_4" ]:
					sendNotification(PetEvent.PET_TRAIN_OUTSIDE_INTERFACE);
					break;
				case GameCommonData.wordDic[ "mod_pla_med_cou_ons_5" ]:
					sendNotification(PetEvent.PET_REST_OUTSIDE_INTERFACE);
					break;
			}
		}
		
		/**
		 * 点击头像弹出菜单 
		 * @param e
		 * 
		 */		
		private function onHeadClickHandler(e:MouseEvent):void{
			var targetPoint:Point=new Point(this._head.mouseX,this._head.mouseY);
			var globalPoint:Point=this._head.localToGlobal(targetPoint);
			var m:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if(m!=null){
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			GameCommonData.GameInstance.GameUI.addChild(this._menu);
			this._menu.x=globalPoint.x;
			this._menu.y=globalPoint.y;
			e.stopPropagation();
			TargetController.SetTarget(GameCommonData.Player.Role.UsingPetAnimal);
			sendNotification(PlayerInfoComList.SELECT_ELEMENT,this.role);
		}
		
		/**
		 * 更改宠物的等级
		 * @param level
		 * 
		 */			
		private function setLevel(level:uint):void{
			var le:String=(this.petInfoUI.txt_level as TextField).text;
			if(le==String(level))return;
			(this.petInfoUI.txt_level as TextField).text=String(level);
		}
		
		/**
		 * 设置血量与经验 
		 * 
		 */		
		private function setBoold(role:GamePetRole):void{
			(this.petInfoUI.redOne as MovieClip).width=Math.min((role.HpNow/role.HpMax),1) *110;
			(this.petInfoUI.greenOne as MovieClip).width=Math.min((role.ExpNow/UIConstData.ExpDic[3000+role.Level]),1)*110;
		}
		
		/**
		 * 设置头像 
		 * @param face
		 * 
		 */		
		private function setFace(face:uint):void{
			if(this._face==face)return;
			this._face=face;
			var f:FaceItem=new FaceItem(String(face),null,"face",30/50);
			f.x = -1;
			f.y = -1;
			while((this.petInfoUI.mc_headImg as MovieClip).numChildren>0){
				(this.petInfoUI.mc_headImg as MovieClip).removeChildAt(0);
			}
			(this.petInfoUI.mc_headImg as MovieClip).addChild(f);
		}
		
		/**
		 * 设置宠物BUFF 
		 * 
		 */		
		private function setBuffs():void{
			var buffs:Array=[];
			var count:int=0;
			
			
			for each(var deBuff:GameSkillBuff in this.role.DotBuff){
				count++;
				if(count>5)break;
				buffs.push({icon:deBuff.TypeID,tip:deBuff.BuffName,isDeBuff:true});	
			}
			
			for each(var buff:GameSkillBuff in this.role.PlusBuff){
				count++;
				if(count>5)break;
				buffs.push({icon:buff.TypeID,tip:buff.BuffName,isDeBuff:false});
			}
			
			this._buffs.dataPro=[buffs];
			this.petInfoUI.addChild(this._buffs);
		}
		
		/** 获得角色信息*/
		public function get role():GameRole{
			return this._role;
		}
		
	}
}