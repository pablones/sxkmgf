package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.View.Components.UISprite;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;

	public class UIFriendsList extends UISprite
	{
		/** 数据提供者 */
		protected var _dataPro:Array;
		/** 渲染器数组 */
		protected var cells:Array;
		/** 缓冲区数组 */		
		protected var cacheCells:Array;
		
		/** 菜单*/
		public var menu:MenuItem;
		/** 子菜单 */
		protected var childMenu:MenuItem;
		
		/** 当前位置 */
		protected var currendPos:Point;
		
		protected var flag:Boolean=false;
		
		protected var isShow:Boolean=false;
		
		/** 是否已经单击*/
		protected var countClick:uint=0;
		
		
		public function UIFriendsList(value:Array=null)
		{
			this._dataPro=value;
			this.createChildren();
		}
		
		protected function createChildren():void{
			
			this.cells=[];
			this.cacheCells=[];
			
			for each(var data:Object in this.dataPro){
				this.createCell(data);
			}
			
			this.createMenu();
			this.doLayout();
		}
		
		/**
		 * 创建菜单 
		 * 
		 */		
		protected function createMenu():void{
			this.menu=new MenuItem();
			this.menu.addEventListener(MenuEvent.Cell_Click,onMenuCellClick);
			this.addEventListener(Event.ADDED_TO_STAGE,onAddToStage);
			this.addEventListener(Event.REMOVED_FROM_STAGE,onRemoveStage);
		} 
		
		/**
		 * 点击菜单选项事件
		 * @param e
		 * 
		 */		
		protected function onMenuCellClick(e:MenuEvent):void{
			this.dispatchEvent(new MenuEvent(e.type,false,false,e.cell,this.menu.roleInfo));
		}
		
		
		protected function onRemoveStage(e:Event):void{
			this.stage.removeEventListener(Event.ADDED_TO_STAGE,onAddToStage);
			if(GameCommonData.GameInstance.GameUI.contains(this.menu)){
				GameCommonData.GameInstance.GameUI.removeChild(this.menu);
			}
		}
		
		protected function onAddToStage(e:Event):void{
			this.stage.addEventListener(MouseEvent.CLICK,onStageMouseClickHanlder);
		}
		
		protected function onStageMouseClickHanlder(e:MouseEvent):void{
			if(this.stage==null)return;
			if(GameCommonData.GameInstance.GameUI.contains(this.menu) ){
				GameCommonData.GameInstance.GameUI.removeChild(this.menu);
				this.isShow=false;
			}
		}
		
		/**
		 * 显示菜单 
		 * 
		 */		
		public function showMenu(playerInfo:PlayerInfoStruct):void{
			
			this.menu.roleInfo=playerInfo;
			var childData:Array=[
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"1",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"1"}},//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"2",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"2"}},//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"3",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"3"}},//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"4",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"4"}},	//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ]}}//"黑名单" 	"黑名单"	
			];
			var dataList:Array=[];
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]}});//"发送消息"	"发送消息"
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_3" ]}});//"设为私聊"	"设为私聊"
			if(playerInfo.idTeam==0){
				dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_7" ]}});//"邀请入队"		"邀请入队"
			}
			if(playerInfo.idTeam>0 && GameCommonData.Player.Role.idTeam==0){
				dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_8" ]}});//"申请入队"		"申请入队"
			}  	
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ]}});//"复制名字"		"复制名字"
			if(GameCommonData.wordVersion != 2)
			{
				dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ]}});//"邀请入帮"		"邀请入帮"
			} 
			dataList.push({cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}});//"查看资料"		"查看资料"
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ]}});//"删除好友"	"删除好友"
			dataList.push({cellText:GameCommonData.wordDic[ "mod_fri_view_med_uif2_sho" ]+" >",data:{hasChild:true,data:childData,type:GameCommonData.wordDic[ "mod_fri_view_med_uif2_sho" ]}});//"分组"		"分组"
			
			if(this.menu.roleInfo.friendGroupId==6)dataList.shift();
			this.menu.dataPro=dataList;
			
			var m:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
			if(m!=null){
				GameCommonData.GameInstance.GameUI.removeChild(m);
			}
			GameCommonData.GameInstance.GameUI.addChild(this.menu);
			this.menu.x=this.currendPos.x;
			this.menu.y=this.currendPos.y;
		}
		
		 	
		/**
		 * 获取渲染器 
		 * @return 
		 * 
		 */			
		protected function getCell():MovieClip{
			var cell:MovieClip=this.cacheCells.shift();
			if(cell==null){
				cell=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("FriendCellSprite");
			}
			this.cells.push(cell);
			return cell;
			
		}
		
		/**
		 * 重绘
		 * @param updateHash
		 * 
		 */		
		protected function toRepaint(updateHash:Object=null):void{
			
			if(this._dataPro==null)return;
			
			this.removeAllCells();
			this.cells=[];
			for each(var cellData:Object in this._dataPro){
				this.createCell(cellData);
			}
//			if(this._dataPro.length==0){
//			}else{
//				
//				 switch (updateHash["rePaintType"]){
//					//有新的渲染器生成或渲染数相同
//					case -1:	
//					case 0:
//						this.cacheCells=this.cells;
//						
//						this.cells=[];
//						break;
//					case 1:
//						this.removeAllCells();
//						this.cells=[];
//						break;
//					default:
//							trace("error");	
//				} 
//			}
			
			
			this.doLayout();
		}
		/**
		 * 创建渲染器 
		 * @param data
		 * 
		 */		
		protected function createCell(cellData:Object):void{
			
			if(cellData["name1"]==null)return;
			var cell:MovieClip=this.getCell();
			this.addChild(cell);
			var isOnline:uint=(cellData["roleInfo"] as PlayerInfoStruct).isOnline;
			if(isOnline){
				(cell.userName as TextField).textColor=0xffffff;
				(cell.idea as TextField).textColor=0x00fff6;
				(cell.idea as TextField).filters=Font.Stroke(0x1b03ff);
			}else{
				(cell.userName as TextField).textColor=0x666666;
				(cell.idea as TextField).textColor=0x666666;
				(cell.idea as TextField).filters=null;
				
			}
			cell.userName.text=cellData["name1"];
			cell.idea.text=cellData["name2"];
			cell.idea.mouseEnabled=false;
			cell.userName.mouseEnabled=false;
			cell["roleInfo"]=cellData["roleInfo"];
			cell.bgMc.alpha=0;
			cell.bgMc.addEventListener(MouseEvent.ROLL_OVER,onMouseRollOverHandler);
			cell.bgMc.addEventListener(MouseEvent.ROLL_OUT,onMouseRollOutHandler);
			cell.addEventListener(MouseEvent.CLICK,click);
			
			var f:FaceItem=new FaceItem(String((cellData["roleInfo"] as PlayerInfoStruct).face),null,"face",(28/50));
			f.offsetPoint = new Point(2,2);
			cell.headImg.addChild(f);
			
		}
		
		protected function onMouseDoubleClick(roleInfo:*):void{
			this.dispatchEvent(new MenuEvent(MenuEvent.CELL_DOUBLE_CLICK, false, false,null,roleInfo));
		
		}
			
		protected function onMouseRollOverHandler(e:MouseEvent):void{
			(e.target as MovieClip).alpha=0.7;
		}
		
		protected function onMouseRollOutHandler(e:MouseEvent):void{
			(e.target as MovieClip).alpha=0;
		}
		
		/**
		 * 鼠标监听处理器 
		 * @param e
		 * 
		 */		
		protected function onMouseClickHandler():void{
			this.countClick=0;
			var childData:Array=[
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"1",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"1"}},//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"2",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"2"}},//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"3",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"3"}},//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"4",data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_2" ]+"4"}},	//好友分组		好友分组
			{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ]}}//"黑名单"		"黑名单"	
			];
			
			var localPoint:Point=new Point(arguments[1],arguments[2]);
			var globalPoint:Point=this.localToGlobal(localPoint);
			this.currendPos=globalPoint;
			this.menu.roleInfo=arguments[0] as PlayerInfoStruct;
			var dataList:Array=[];
			if(this.menu.roleInfo.friendGroupId==5){
				dataList=[{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]}},//"发送消息"	"发送消息"
				{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_4" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_4" ]}},//"添加好友"	"添加好友"
				{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_6" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_6" ]}},//"复制名字"		"复制名字"
				{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}}//"查看资料"		"查看资料"
				];
				this.menu.dataPro=dataList;
				var m:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
				if(m!=null){
					GameCommonData.GameInstance.GameUI.removeChild(m);
				}
				
				GameCommonData.GameInstance.GameUI.addChild(this.menu);
				this.menu.x=this.currendPos.x;
				this.menu.y=this.currendPos.y;
			}else{
				if(this.menu.roleInfo.isOnline){
					FriendSend.getInstance().requestRoleTeam(this.menu.roleInfo.frendId,this.menu.roleInfo.roleName);
				}else{
					if(GameCommonData.wordVersion != 2)
					{
						dataList=[{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]}},//"发送消息"		"发送消息"
						{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ]}},//"设为私聊"		"设为私聊"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ]}},//"复制名字"		"复制名字"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ]}},//"邀请入帮"		"邀请入帮"
						{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}},//"查看资料"	"查看资料"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ]}},//"删除好友"	"删除好友"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_uif2_sho" ]+" >",data:{hasChild:true,data:childData,type:GameCommonData.wordDic[ "mod_fri_view_med_uif2_sho" ]}}];//"分组"		"分组"
					}
					else
					{
						dataList=[{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]}},//"发送消息"		"发送消息"
						{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ]}},//"设为私聊"		"设为私聊"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ]}},//"复制名字"		"复制名字"
//						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ]}},//"邀请入帮"		"邀请入帮"
						{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}},//"查看资料"	"查看资料"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ]}},//"删除好友"	"删除好友"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_uif2_sho" ]+" >",data:{hasChild:true,data:childData,type:GameCommonData.wordDic[ "mod_fri_view_med_uif2_sho" ]}}];//"分组"		"分组"
					}
					
					if(this.menu.roleInfo.friendGroupId==6)dataList.shift();
					this.menu.dataPro=dataList;
					
					var m1:DisplayObject=GameCommonData.GameInstance.GameUI.getChildByName("MENU");
					if(m1!=null){
						GameCommonData.GameInstance.GameUI.removeChild(m1);
					}
					GameCommonData.GameInstance.GameUI.addChild(this.menu);
					this.menu.x=this.currendPos.x;
					this.menu.y=this.currendPos.y;
				}
			}
			
		}
		
		/** 廷时器ID */
		protected var id:uint=0;
		protected function click(e:MouseEvent):void{
			this.countClick++;
			if(this.countClick==1){
				id=setTimeout(onMouseClickHandler,200,e.currentTarget["roleInfo"],mouseX,mouseY);
				e.stopPropagation();
			}else if(this.countClick==2){
				this.countClick=0;
				clearTimeout(id);
				e.stopPropagation();
				this.onMouseDoubleClick(e.currentTarget["roleInfo"]);
				
			}
		}
		/**
		 * 鼠标滚入菜单
		 * @param e
		 * 
		 */		
		protected function onMouseRollOverMenu(e:MouseEvent):void{
			this.flag=true;
		}
		
				
		/**
		 * 移除所有的渲染器 
		 * 
		 */		
		protected function removeAllCells():void{
			for each(var cell:MovieClip in this.cells){
				if(this.contains(cell)){
					this.removeChild(cell);
					this.cacheCells.push(cell);
				}	
			}
			this.cells=[];				
		}
		
		/**
		 * 布局 
		 * 
		 */			
		protected function doLayout():void{
			var currentY:Number=0;
			var width:uint;
			for each(var cell:MovieClip in this.cells){
				
				cell.x=0;
				cell.y=currentY;
				width=Math.max(width,cell.width);
				currentY+=Math.floor(cell.height+1);
			}
			this.width=width;
			this.height=currentY;
//			drawBg();
		}
		
		/**
		 * 画一个黑色的背景
		 * 
		 */		
		protected function drawBg():void{
			this.graphics.beginFill(0xffffff,0);
			this.graphics.drawRect(0,0,this.width,this.height+3);
			this.graphics.endFill();
			
		}
		
		/**
		 * 设置数据提供者
		 */ 
		public function set dataPro(value:Array):void{
//			var obj:Object={}
//			obj["rePaintType"]=this.checkRepaintType(this._dataPro,value);
			this._dataPro=value;
			this.toRepaint();
		}
		
		/**
		 * 获得数据提供者
		 * @return 
		 * 
		 */		
		public function get dataPro():Array{
			return this._dataPro;
		}
		
		/**
		 * 检测重绘的类型 
		 * @return -1:旧的数据比新的少  0：新旧数据数量相同  1：旧的数据比新的数据多
		 * 
		 */		
		protected function checkRepaintType(oldData:Array,newData:Array):int{
			return 0;
//			if(oldData.length==newData.length)return 0;
//			return oldData.length>newData.length ? 1 : -1;
		}
		
		
	}
}