package GameUI.Modules.PlayerInfo.UI
{
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.view.ui.MenuItem;
	import GameUI.Modules.PlayerInfo.Command.TeamEvent;
	import GameUI.UICore.UIFacade;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	import OopsEngine.Role.GameRole;
	import OopsEngine.Skill.GameSkillBuff;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	
	/**
	 * 队伍渲染器
	 * @author felix
	 * 
	 * 
	 */
	public class TeamCell extends Sprite
	{
		
		/** MC*/
		public var contentMc:MovieClip;
		/** 角色数据 */
		public var role:GameRole;
		/** 菜单  */	
		public var menu:MenuItem;
		/** 当前头像代号 */		
		protected var face:int=-1;
		/** buff图标  */
		protected var buffs:HeadImgList;
		/** 玩家离线在线描述  */
		protected var lineDes:TextField;
		/** 在线，离线 遮罩*/
		protected var onlineMask:Shape;
		
		public function TeamCell(role:GameRole)
		{
			super();
			
			this.addEventListener(Event.ADDED_TO_STAGE,onaddToStage);
			this.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStage);
			this.contentMc=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("TeamCell");
			this.buffs=new HeadImgList(2);
			this.buffs.x=41;
			this.buffs.y=35;
			this.role=role;	
			(this.contentMc.txt_userName as TextField).text=role.Name;
			(this.contentMc.txt_userName as TextField).mouseEnabled=false;
			(this.contentMc.txt_level as TextField).text=String(role.Level);
			(this.contentMc.txt_level as TextField).mouseEnabled=false;
			
			(this.contentMc.mc_redOne as MovieClip).width=Math.min((role.HP/(role.MaxHp+role.AdditionAtt.MaxHP)),1)*125;
			(this.contentMc.mc_buleOne as MovieClip).width=Math.min((role.MP/(role.MaxMp+role.AdditionAtt.MaxMP)),1)*125;
			(this.contentMc.mc_headImg as MovieClip).addEventListener(MouseEvent.CLICK,onClickHeadHandler);
			(this.contentMc.btn_menu as SimpleButton).addEventListener(MouseEvent.CLICK,onMouseClickHandler);
			(this.contentMc.btn_menu as SimpleButton).upState.width = 19;
			
			onlineMask=new Shape();
			onlineMask.graphics.beginFill(0, .4);
			onlineMask.graphics.drawRect(3,0,32,32);
			onlineMask.graphics.endFill();
			onlineMask.visible=false;
			
			this.contentMc.addChild(onlineMask);
			
			this.lineDes=new TextField();
			this.lineDes.autoSize=TextFieldAutoSize.LEFT;
			this.lineDes.mouseEnabled=false;
			this.lineDes.filters=Font.Stroke();
			this.contentMc.addChild(this.lineDes);
			this.lineDes.x=10;     
			this.lineDes.y=16;
			
			this.setBuffs(role);
			this.setFace(role.Face);
			this.menu=new MenuItem();
			this.menu.addEventListener(MenuEvent.Cell_Click,onClickHandler);
			this.addChild(this.contentMc as DisplayObject);
		}
		
		protected function onClickHeadHandler(e:MouseEvent):void{
			UIFacade.UIFacadeInstance.selectTeam(this.role);
		}
		
		
		/**
		 * 更新在线离线描述 
		 * @param des
		 * 
		 */		
		public function updateLineDes(des:String,status:uint=0):void{
			if(status==0){
				this.onlineMask.visible=true;
			}else{
				this.onlineMask.visible=false;
			}
			this.lineDes.htmlText=des;
		}
		
		
		protected function onClickHandler(e:MenuEvent):void{
			var event:TeamEvent=new TeamEvent(TeamEvent.CELL_CLICK,false,false);
			event.flagStr=e.cell.data["type"];
			event.role=role;
			this.dispatchEvent(event);
		}
		
		protected function onaddToStage(e:Event):void{
			this.stage.addEventListener(MouseEvent.CLICK,onMouseClick);
		}
		
		protected function onMouseClick(e:MouseEvent):void{
			if(GameCommonData.GameInstance.GameUI.contains(this.menu)){
				GameCommonData.GameInstance.GameUI.removeChild(this.menu);
			}
		}
		
		protected function onRemoveFromStage(e:Event):void{
			this.stage.removeEventListener(MouseEvent.CLICK,onMouseClick);
			this.menu.removeEventListener(MenuEvent.Cell_Click,onClickHandler);
			this.menu.dataPro=[];
			if(this.menu.parent!=null){
				this.menu.parent.removeChild(this.menu);
			}	
			(this.contentMc.mc_headImg as MovieClip).removeEventListener(MouseEvent.CLICK,onMouseClickHandler);
			this.stage.removeEventListener(Event.ADDED_TO_STAGE,onaddToStage);
			this.stage.removeEventListener(Event.REMOVED_FROM_STAGE,onRemoveFromStage);
		}
			
		
		protected function onMouseClickHandler(e:MouseEvent):void{
//			UIFacade.UIFacadeInstance.selectTeam(this.role);
			var menuData:Array=[];
			var myRole:GameRole=GameCommonData.Player.Role;
			
			menuData.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_5" ] }});   //  设为私聊  设为私聊
			menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_tea_onc_4" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_tea_onc_4" ]}});   // 加为好友  加为好友
			menuData.push({cellText:GameCommonData.wordDic[ "often_used_trade" ],data:{type:GameCommonData.wordDic[ "often_used_trade" ]}});    // 交易  交易
			//自己是队
			if(myRole.Id==myRole.idTeamLeader && myRole.idTeamLeader!=0){	
				menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_tea_onc_3" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_tea_onc_3" ]}});  // 提升为队长  提升为队长
				menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_tea_onc_2" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_tea_onc_2" ]}});  // 移出队伍  移出队伍
			}
			menuData.push({cellText:GameCommonData.wordDic[ "mod_pla_med_tea_onc_1" ],data:{type:GameCommonData.wordDic[ "mod_pla_med_tea_onc_1" ]}});  // 离开队伍  离开队伍
			this.menu.dataPro=menuData;
			var localPoint:Point=new Point(this.mouseX,this.mouseY);
			var globalPoint:Point=this.localToGlobal(localPoint);
			var m:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if(m!=null){
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			
			GameCommonData.GameInstance.GameUI.addChild(this.menu);
			this.menu.x=globalPoint.x;
			this.menu.y=globalPoint.y;
			e.stopPropagation();
		}
		
		public function setHp(val1:uint,val2:uint):void{
			(this.contentMc.mc_redOne as MovieClip).width=(val1/val2)*125;
		}
		
		public function setMp(val1:uint,val2:uint):void{
			(this.contentMc.mc_buleOne as MovieClip).width=(val1/val2)*125;
		}
		
		public function setLevel(level:uint):void{
			(this.contentMc.txt_level as TextField).text=String(level);
		}
		
		public function setFace(face:uint):void{
			if(this.face==face)return;
			this.face=face;
			
			var faceItem:FaceItem=new FaceItem(String(face),null,"face",30/50);
			faceItem.offsetPoint=new Point(0,0);
			
			var mc:MovieClip=this.contentMc.mc_headImg as MovieClip;
			while(mc.numChildren>0){
				mc.removeChildAt(0);
			}
			mc.addChild(faceItem);
			mc.x=3,
			mc.y=0;
			faceItem.x=faceItem.y=0;			
			if(role.Id==role.idTeamLeader){
				var teamLeader:BitmapData=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData("TeamLeaderSign");
				var bitMap:Bitmap=new Bitmap();
				bitMap.name="TeamLeader";
				bitMap.bitmapData=teamLeader;
				var bm:DisplayObject=this.contentMc.getChildByName("TeamLeader");
				
				if(bm!=null && this.contentMc.contains(bm)){
					this.contentMc.removeChild(bm);
				}
				this.contentMc.addChild(bitMap);
				bitMap.x=39;
				bitMap.y=-2;
			}
		}
		
		/**
		 * 
		 * 设置BUff与DEBUFF的显示（最多显示7个DEBUFF在前）
		 * 
		 */		
		protected function showBuffs(arr:Array):void{
			this.buffs.dataPro=arr;
			this.contentMc.addChild(this.buffs);
		}
		
		/**
		 * 设置Buff
		 * @param role
		 * 
		 */		
		public function setBuffs(role:GameRole):void{
			this.role=role
			var buffData:Array=[];
			var count:uint=0;
			for each(var deBuff:GameSkillBuff in role.DotBuff){
				count++;
				if(count>7)break;
				buffData.push({icon:deBuff.TypeID,tip:deBuff.BuffName,isDeBuff:true});
			}
			for each(var buff:GameSkillBuff in role.PlusBuff){
				count++;
				if(count>7)break;
				buffData.push({icon:buff.TypeID,tip:buff.BuffName,isDeBuff:false});
			}
			this.showBuffs([buffData]);
		}		
	}
}