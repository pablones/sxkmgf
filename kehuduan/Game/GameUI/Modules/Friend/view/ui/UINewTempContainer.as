package GameUI.Modules.Friend.view.ui
{
	import GameUI.Modules.Friend.command.MenuEvent;
	import GameUI.Modules.Friend.model.vo.PlayerInfoStruct;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ResourcesFactory;
	import GameUI.View.items.FaceItem;
	
	import Net.ActionSend.FriendSend;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	import flash.utils.clearTimeout;
	import flash.utils.setTimeout;
	
	/**
	 * 
	 * @author felix
	 * 
	 */
	public class UINewTempContainer extends Sprite
	{
		/** 渲染器数组  */
		protected var cells:Array = new Array();
		/** 渲染器缓冲  */		
		protected var cacheCells:Array = new Array();

		/**  各组好友的数据提供者（【组】【name,name....】） */
		protected var _fListDataPro:Array;


		/** 选中索引 */
		protected var selectedIndex:uint=0;
		
		private var friendCell:MovieClip;
		
		private var friendView:MovieClip;
		private static var onePageNum:int = 14;
		private var currentPage:int = 0;
		private var pageNum:int = 0;
		private var roleCount:int = 0;		
		private var roleList:Array = new Array();
		private var friendSwf:MovieClip;
		/** 是否已经单击*/
		protected var countClick:uint=0;		
		/** 菜单*/
		public var menu:MenuItem;		
		/** 当前位置 */
		protected var currendPos:Point;
		protected var isShow:Boolean=false;
		
		public var w:Number=148;
		public var h:Number=279;
		
		
		public function UINewTempContainer(friendView:MovieClip)
		{
			super();  
			//this.createChildren();
			this.friendView = friendView;
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf", onPanelLoadComplete);
		}
		
		private function onPanelLoadComplete():void
		{
			friendSwf = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewFriend.swf");			
			this.friendCell = new (friendSwf.loaderInfo.applicationDomain.getDefinition("friendCell"));
		}
		
		/**
		 * 
		 * @param value
		 * 
		 */		
		public function set fListDataPro(value:Array):void{
			this.sortOn(value);
			this._fListDataPro=value;
			this.setRoleList(this._fListDataPro);
			this.setPage();
			this.toRepaint(this.currentPage);	
		}
		
		
		public function get fListDataPro():Array{
			return this._fListDataPro;
		}
		
		public function setPage():void
		{
			
			var num:int = int((this.roleList.length - 1)/onePageNum) + 1;
			var currentNum:int = this.currentPage + 1;
			this.friendView.txt_pagenum.text = "第" + currentNum.toString() + "/" + num.toString() + "页";
			if(currentNum == 1)
			{
				(this.friendView.btn_left as SimpleButton).mouseEnabled = false;
			}else{
				(this.friendView.btn_left as SimpleButton).mouseEnabled = true;
			}
			if(currentNum == num)
			{
				(this.friendView.btn_right as SimpleButton).mouseEnabled = false;
			}else{
				(this.friendView.btn_right as SimpleButton).mouseEnabled = true;
			}
		}
		
		public function setRoleList(arr:Array):void
		{
			//var group:int = 0;
			//for each(var data:Array in arr){
				for(var i:uint=0;i<arr.length;i++){
					if(this.roleList[i] == null)
					{
						var val:PlayerInfoStruct=arr[i].roleInfo as PlayerInfoStruct;	
						this.roleList.push(val);
					}					
				//}
				//group += 1;
			}				
		}
		
		/**
		 *  对一维数组进行排序 
		 * @param arr
		 * 
		 */		
		protected function sortOn(arr:Array):void{
			for(var i:uint=0;i<arr.length;i++){
				for(var j:uint=0;j<arr.length-i-1;j++){
					var val1:PlayerInfoStruct=arr[j].roleInfo as PlayerInfoStruct;
					var val2:PlayerInfoStruct=arr[j+1].roleInfo as PlayerInfoStruct;
					if(val1!=null && val2!=null){
						if(val1.isOnline<val2.isOnline){
							var temp:Object=arr[j];
							arr[j]=arr[j+1];
							arr[j+1]=temp;
						}
					}
						
				}
			}
		}
		
		/**
		 * 创建并初始化UI 
		 * 
		 */
		private var f:FaceItem; 		
		protected function createChildren(data:PlayerInfoStruct):void
		{			
			var friendcell:MovieClip = this.getCell();
			this.addChild(friendcell);
			friendcell.addEventListener(MouseEvent.CLICK,onCellClickHandler);
			//this.cells.push(friendcell);
			var isOnline:uint=data.isOnline;
			if(isOnline != 0){
				if(isOnline == 1){(friendcell.txt_line as TextField).text = "一线";}
				if(isOnline == 2){(friendcell.txt_line as TextField).text = "二线";}
				if(isOnline == 3){(friendcell.txt_line as TextField).text = "三线";}
				if(isOnline == 4){(friendcell.txt_line as TextField).text = "四线";}				
				(friendcell.txt_name as TextField).textColor=0xffffff;
				(friendcell.txt_idea as TextField).textColor=0x00fff6;
				(friendcell.txt_line as TextField).textColor=0x00fff6;
				(friendcell.txt_idea as TextField).filters=Font.Stroke(0x1b03ff);
			}else{
				(friendcell.txt_name as TextField).textColor=0x666666;
				(friendcell.txt_idea as TextField).textColor=0x666666;
				(friendcell.txt_line as TextField).textColor=0x666666;
				(friendcell.txt_idea as TextField).filters=null;
				(friendcell.txt_line as TextField).text = "离线";	
			}
			(friendcell.hight as TextField).mouseEnabled = false;			
			friendcell["roleInfo"]=data;
			friendcell.txt_name.text = data.roleName;//名字
			friendcell.txt_idea.text = data.feel;//心情
			f=new FaceItem(String(data.face),null,"face");
			var scale:Number = friendcell.height/f.height - 0.3;
			f.scaleX = 0.4;
			f.scaleY = 0.4;
			f.x = 7;
			f.y = 2;			
			friendcell.addChild(f);
			friendcell.x = 10;
			friendcell.y = 85 + (friendcell.height)*roleCount;	
			friendcell.txt_name.mouseEnabled = false;
			friendcell.txt_idea.mouseEnabled = false;
			friendcell.txt_line.mouseEnabled = false;			

		}

		/**
		 * 移除所有的渲染器 
		 * 
		 */		
		public function removeAllCells():void{
			for each(var cell:MovieClip in this.cells){
				if(this.contains(cell)){
					if(this.contains(this.shape)){
						this.removeChild(this.shape);
					}
					if(cell.contains(f)){
						cell.removeChild(f);
					}
					cell.removeEventListener(MouseEvent.CLICK,onCellClickHandler);
					this.removeChild(cell);
					this.roleCount = 0;
					//this.cacheCells.push(cell);
				}	
			}
			this.cells=[];				
		}	
				
//		protected function onDoubleClickHandler(e:MenuEvent):void{
//			this.dispatchEvent(new MenuEvent(e.type,false,false,null,e.roleInfo));
//		}
		
		public var shape:Shape= new Shape();		
		protected var id:uint=0;		
		protected function onCellClickHandler(e:MouseEvent):void{
			this.countClick++;
			this.addChild(shape);	
			shape.x = e.currentTarget.x;
			shape.y = e.currentTarget.y;
			shape.graphics.clear();
			shape.graphics.lineStyle(1,0xfffe65);
			shape.graphics.drawRect(0,0,336,23);
			shape.graphics.endFill();			
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

		protected function onMouseDoubleClick(roleInfo:*):void{
			this.dispatchEvent(new MenuEvent(MenuEvent.CELL_DOUBLE_CLICK, false, false,null,roleInfo));
		
		}
		
		/**
		 * 鼠标监听处理器 
		 * @param e
		 * 
		 */		
		protected function onMouseClickHandler():void{
			this.countClick=0;
			
			var localPoint:Point=new Point(arguments[1],arguments[2]);
			var globalPoint:Point=this.localToGlobal(localPoint);
			this.currendPos=globalPoint;
			this.menu.roleInfo=arguments[0];
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
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ]}}];//"分组"		"分组"
					}
					else
					{
						dataList=[{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_2" ]}},//"发送消息"		"发送消息"
						{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_3" ]}},//"设为私聊"		"设为私聊"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onE_4" ]}},//"复制名字"		"复制名字"
//						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_uie_cre" ]}},//"邀请入帮"		"邀请入帮"
						{cellText:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_chat_med_qui_model_1" ]}},//"查看资料"	"查看资料"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ],data:{hasChild:false,type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_onM_3" ]}},//"删除好友"	"删除好友"
						{cellText:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ],data:{type:GameCommonData.wordDic[ "mod_fri_view_med_friendM_han_4" ]}}];//"黑名单"		"黑名单"
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
		 * 重绘 
		 * 
		 */		
		protected function toRepaint(page:int):void{
			this.removeAllCells();
			this.createMenu();
			for(var i:int = this.currentPage * onePageNum;i < this.roleList.length; i++){
				if(i < (this.currentPage +1)*onePageNum){
					this.createChildren(this.roleList[i] as PlayerInfoStruct);
					this.roleCount += 1;
				}
			}
			this.roleList = [];
		}
		
		
		public function pageDown():void
		{
			if(this.currentPage < (this.roleList.length/onePageNum -1))
			{
				this.currentPage += 1;
				this.setPage();
				this.toRepaint(this.currentPage);
			}
		}
		
		public function pageUp():void
		{
			if(this.currentPage > 0)
			{
				this.currentPage -= 1;
				this.setPage();
				this.toRepaint(this.currentPage);
			}
		}
						
		/**
		 * 获取渲染器
		 * @return 渲染器 
		 * 
		 */		
		protected function getCell():*{
			var cell:*=this.cacheCells.shift();
			if(cell==null){
				cell=new (friendSwf.loaderInfo.applicationDomain.getDefinition("friendCell"));
			}
			this.cells.push(cell);
			return cell;
		}
		
		/**
		 * 获取与设置数据提供者
		 * @param value
		 * 
		 */		
		public function set dataPro(value:Array):void{
			//if(value==this._dataPro)return;
			//var obj:Object={};
			//obj["rePaintType"]=this.checkRepaintType(this._dataPro,value);
			//this._dataPro=value;
			//this.toRepaint(obj);
		}
			
	}
}