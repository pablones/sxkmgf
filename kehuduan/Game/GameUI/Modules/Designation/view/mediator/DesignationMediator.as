package GameUI.Modules.Designation.view.mediator
{
	import Controller.AppellationController;
	
	import GameUI.Modules.Designation.Data.DesignationChangeCommand;
	import GameUI.Modules.Designation.Data.DesignationCommand;
	import GameUI.Modules.Designation.Data.DesignationEvent;
	import GameUI.Modules.Designation.Data.DesignationProxy;
	import GameUI.Modules.Designation.view.ui.DesignationItem;
	import GameUI.Modules.Designation.view.ui.ScrollContainer;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.Master.Data.MasterData;
	import GameUI.Modules.RoleProperty.Datas.RoleEvents;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.Components.UISprite;
	
	import Net.ActionProcessor.PlayerAction;
	import Net.ActionSend.PlayerActionSend;
	
	import OopsEngine.Role.Appellation;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	/**称号面板，游戏初始化过程中，在playerAction中274里面发送请求，
	 * NetAction.requestDesignation();
	 * 玩家的事件库的称号为userInfo里面的var uUseTitle:uint 	   = bytes.readUnsignedInt();
	 * UserTitle为玩家拥有的称号信息
	 * */
	public class DesignationMediator extends Mediator
	{
		public static const NAME:String = "DesignationMediator";
		public static const GC_DESIGNATION_MEDIATOR:String = "gcDesignationMediator";
		private var dataProxy:DataProxy;//获取数据
		private var listContainer:UISprite;//存放选项条和文本框的容器
		private var scrollContainer:ScrollContainer;//滚动容器
		private var dItemArr:Array;//存放选项条的容器
		private var isFirstOpenUI:Boolean = true;
		private var _selectDataObj:Object;//选择的小条目的数据
		private var currentDid:int = -1; //当前的称号id
		private var dataArr:Array;//储存条目数据的数组
		private var tempDesignationItem:DesignationItem;
		private var isItemClicked:Boolean = false;
		private var isChange:Boolean = false;
		
		public static var TYPE:int = 3;
		private var parentView:MovieClip;
		public function DesignationMediator(parentMc:MovieClip)
		{
			parentView = parentMc;
			super(NAME);
		}
		
		public function get designationView():MovieClip
		{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					RoleEvents.SHOWPROPELEMENT,
					DesignationEvent.UPDATE_DATAARR,
					DesignationEvent.DEAL_AFTER_UPDATE,
					GC_DESIGNATION_MEDIATOR
				];
		}
		
		override public function handleNotification(notification:INotification):void
		{
			switch ( notification.getName() )
			{
				case RoleEvents.SHOWPROPELEMENT:
					if(notification.getBody() as int != TYPE) 
					{
						return;	
					}
					initUI();
				break;
				case GC_DESIGNATION_MEDIATOR:
					if(parentView.contains(designationView))
					{
						this.gc();
					}
				break;
				case DesignationEvent.UPDATE_DATAARR:
					updateDataArr();
				break;
				case DesignationEvent.DEAL_AFTER_UPDATE:
					isHaveCurrentDesignation();
				break;
			}
		}

	    override public function onRegister():void
		{
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			var mainView:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip(UIConfigData.DESIGNATIONPANEL);
			mainView.name = "designationView";
			this.setViewComponent(mainView);
			facade.registerProxy(new DesignationProxy());
			facade.registerCommand(DesignationCommand.NAME,DesignationCommand);
			facade.registerCommand(DesignationChangeCommand.NAME,DesignationChangeCommand);
			initTxt();
		}
		/**初始化*/
		private function initUI():void
		{
			 if(isFirstOpenUI)
			{
				isFirstOpenUI = false;
				designationView.txt_current.text = "";
				designationView.txt_current.mouseEnabled = false;
				designationView.txt_introduce.text = "";
				designationView.txt_introduce.mouseEnabled = false;
				designationView.txt_condition.text = "";
				designationView.txt_condition.mouseEnabled = false;
				designationView.txt_explain.mouseEnabled = false;
				
			} 
			if(parentView.contains(designationView))
			{
				gc();
			}
			listContainer = new UISprite();
			listContainer.x = 10;
			listContainer.y = 35;
			listContainer.width = 145;
			listContainer.height = 320;
			designationView.addChild(listContainer);
			designationView.btn_change.txt_isShow.mouseEnabled = false;
			
			initItem();
			dataProxy.designationPanIsOpen = true;
			parentView.addChildAt(designationView,4);
			if(currentDid > 0)
			{
				var playerId:int = GameCommonData.Player.Role.Id;
				_selectDataObj = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerPlayerDesignationInfo(currentDid,playerId);
			
				if(_selectDataObj)
				{
				  	designationView.txt_current.text = _selectDataObj.name;
				  	designationView.txt_introduce.text = _selectDataObj.introduce;
					designationView.txt_condition.text = _selectDataObj.condition;
				}
			}else{
				designationView.txt_current.text = GameCommonData.wordDic[ "mod_des_med_des_initU" ];//"您当前没有称号"
				designationView.txt_introduce.text = "";
				designationView.txt_condition.text = "";
			}
			addEventListeners();
			if(tempDesignationItem)
			{
				tempDesignationItem.dispatchEvent(new MouseEvent(MouseEvent.CLICK));
			}
		}
		
		
		/**初始化类型条目*/
		private function initItem():void
		{
			dataArr = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerData();
			dItemArr = [];
			var tag:int = 0;
			for each(var obj:Object in dataArr)
			{
				var dItem:DesignationItem = new DesignationItem(obj);
				dItem.addEventListener(MouseEvent.CLICK,itemClickHandler);
				dItem.x = 1;
				dItem.y = 17*tag;
				listContainer.addChild(dItem);
				dItemArr.push(dItem);
				tag ++;
			}
		}
		
		/**第一次打开面板时赋值*/
		private function initTxt():void
		{
			currentDid = int(GameCommonData.Player.Role.DesignationCall);//初始化时获取玩家称号
			var currentPlayerId:int = GameCommonData.Player.Role.Id;
			_selectDataObj = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerPlayerDesignationInfo(currentDid,currentPlayerId);
			if(GameCommonData.Player.IsShowAppellation = true)//若称号是显示就 发送一条消息，让其显示
			{
				if(currentDid != 0)
				{
					reNameDesignationSucced();
				}
			}
			if(currentDid == 0)//打开面板是看起是否有称号
			{
				designationView.btn_change.txt_isShow.text = GameCommonData.wordDic[ "mod_des_med_des_initT_1" ];//"显示"
				return;
			}else{
				designationView.btn_change.txt_isShow.text = GameCommonData.wordDic[ "mod_des_med_des_initT_2" ];//"隐藏"
			}
		}

		private function addEventListeners():void
		{
			GameCommonData.GameInstance.GameScene.addEventListener(DesignationEvent.DESIGNATION_EVENT,designatonEventHandler);
			designationView.btn_change.addEventListener(MouseEvent.CLICK,btnChangeClickHandler);
			designationView.btn_select.addEventListener(MouseEvent.CLICK,btnSelectHandler);
			
		}
		private function removeEventListeners():void
		{
			GameCommonData.GameInstance.GameScene.removeEventListener(DesignationEvent.DESIGNATION_EVENT,designatonEventHandler);
			designationView.btn_change.removeEventListener(MouseEvent.CLICK,btnChangeClickHandler);
			designationView.btn_select.removeEventListener(MouseEvent.CLICK,btnSelectHandler);
		}
		
		/**点击分类条目时候触发的事件*/
		private function itemClickHandler(me:MouseEvent):void
		{
			designationView.txt_explain.text = (me.currentTarget as DesignationItem).getExplain();
			if(tempDesignationItem)
			{
				if(me.currentTarget!==tempDesignationItem)
				{
					tempDesignationItem.content.txt_name.textColor = 0xFFFFFF;
				}
			}
			tempDesignationItem = me.currentTarget as DesignationItem;
			tempDesignationItem.content.txt_name.textColor = 0xFFCC00;
			update(tempDesignationItem);
		}
		private function panelCloseHandler(e:Event):void
		{
			gc();			      
		}
		
		 /**点击显示（隐藏按钮）时触发的事件*/
		private function btnChangeClickHandler(me:MouseEvent):void
		{
			if(currentDid == 0)
			{
				if(_selectDataObj)
				{
					if(_selectDataObj.isSelect == 0)
					{
						facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_des_med_des_btnC_1" ], color:0xffff00});//"您当前不拥有此称号"
					}
				}
				else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_des_med_des_btnC_2" ], color:0xffff00});//"请先选择一项称号"
				}
				return;
			}
			if(designationView.btn_change.txt_isShow.text == GameCommonData.wordDic[ "mod_des_med_des_initT_1" ])//"显示" 
			{
				showDesignationNet(true,(me.currentTarget as DisplayObject));
				designationView.btn_change.txt_isShow.text = GameCommonData.wordDic[ "mod_des_med_des_initT_2" ];//"隐藏"
			}else 
			{
				showDesignationNet(false,(me.currentTarget as DisplayObject));
				
				designationView.btn_change.txt_isShow.text = GameCommonData.wordDic[ "mod_des_med_des_initT_1" ];//"显示"
			}
		}
		
		/**点击选择按钮式触发*/
		private function btnSelectHandler(e:MouseEvent):void
		{
			if(_selectDataObj)
			{
				if(1 == _selectDataObj.isSelect)
				{
					if(isSameItem(currentDid, _selectDataObj))
					{
						facade.sendNotification(HintEvents.RECEIVEINFO,{info:GameCommonData.wordDic[ "mod_des_med_des_btnS_1" ], color:0xffff00});//"请选择与您当前不同的称号"
					}
					else{
						showDesignationNet(true,e.currentTarget as DisplayObject);
					}
				}
				else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_des_med_des_btnC_1" ], color:0xffff00});//"您当前不拥有此称号"
				}
			}else{
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_des_med_des_btnS_2" ], color:0xffff00});//"请选择称号"
			}
		}
		
		/**判断当前的称号与选择要改变的称号是不是同一称号*/
		private  function isSameItem(cId:int,sObj:Object):Boolean
		{
			var boo:Boolean = false;
			if(int(cId/100) == int(sObj.id))
			{
				if(int(cId%100) == int(sObj.level))
				{
					boo = true;
				}
			}
			return boo;
		}
		
		
		/**向服务起发包 */
		private function showDesignationNet(isShow:Boolean,me:DisplayObject = null):void
		{
			var palyerId:int = GameCommonData.Player.Role.Id;
			var obj:Object={type:1010};
			if(isShow)
			{
				if(me == designationView.btn_select)
				{
					obj.data = [0,palyerId,_selectDataObj.id,_selectDataObj.level,0,0,PlayerAction.SEND_DESIGNATION_INFO,0];//SEND_DESIGNATION_INFO == 295
					isChange = false;
				}
				else if(me == designationView.btn_change)
				{
					obj.data = [0,palyerId,int(currentDid/100),int(currentDid%100),0,0,PlayerAction.SEND_DESIGNATION_INFO,0];//SEND_DESIGNATION_INFO == 295
					isChange = true;
				}
			}
			else{
				obj.data = [0,palyerId,0,0,0,0,PlayerAction.SEND_DESIGNATION_INFO,0];//SEND_DESIGNATION_INFO == 295
			}
			PlayerActionSend.PlayerAction(obj);
		}
		
		/**修改称号成功后调用,userAtt中的编号为77*/
		public function reNameDesignationSucced():void
		{
			var tempOjb:Object;
			if(isChange)
			{ 
				var playerId:int = GameCommonData.Player.Role.Id;
				tempOjb = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerPlayerDesignationInfo(currentDid,playerId);
				if(!tempOjb) return;
				designationView.txt_current.text = tempOjb.name;
			}
			else{
				if(!_selectDataObj) return;
				currentDid = int(_selectDataObj.id)*100 + int(_selectDataObj.level);
				designationView.txt_current.text = _selectDataObj.name;
				designationView.txt_introduce.text = _selectDataObj.introduce;
				designationView.txt_condition.text = _selectDataObj.condition ;
			}
				
			//读取信息
		    var appellation:Appellation = new Appellation();
		    appellation.appellationID   = currentDid;
		    appellation.color           = 1;
		    appellation.borderColor     = 2;
		    if(isChange)
		    {
		    	appellation.name 			= tempOjb.name;
		    }
		    else{
			    appellation.name            = _selectDataObj.name;    
		    }
		    
		    AppellationController.ShowAppellation(GameCommonData.Player.Role.Id,appellation,true);
		     
		     if(!isFirstOpenUI)
		     {
			    designationView.btn_change.txt_isShow.text = GameCommonData.wordDic[ "mod_des_med_des_initT_2" ];//"隐藏"
		     }
		}
		
		/**称号更新后，看是否还拥有玩家头顶的称号，若没有，就隐藏称号*/
		private function isHaveCurrentDesignation():void
		{
			var _playerId:int = GameCommonData.Player.Role.Id;
			var tempObj:Object = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerPlayerDesignationInfo(currentDid,_playerId);
			if(!tempObj)
			{
				designationView.btn_change.dispatchEvent(new MouseEvent(MouseEvent.CLICK));
				designationView.txt_current.text = GameCommonData.wordDic[ "mod_des_med_des_initU" ];//"您当前没有称号"
				designationView.txt_introduce.text = "";
				designationView.txt_condition.text = "";
			}
			if(tempDesignationItem)
			{
				if(dataProxy.designationPanIsOpen)
				{
					tempDesignationItem.dispatchEvent(new MouseEvent(MouseEvent.CLICK));
				}
			}
		}
		
		/**其他玩家称号的显示*/
		public function otherShowDesignation(playerId:int,value:int):void
		{	
			var isShow:Boolean;
	  	   	var appellation2:Appellation = new Appellation();
	        appellation2.appellationID   = playerId;
		    appellation2.color           = 1;
		   	appellation2.borderColor     = 2;
		   	if(value == 0)
			{
				isShow = false;
				appellation2.name = "";
			}
			else{
				isShow = true;
				var dataObj:Object = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerPlayerDesignationInfo(value,playerId);
				if(dataObj)
				{
					appellation2.name = dataObj.name;
				}
				else{
					appellation2.name = "";
				}
			}
			AppellationController.ShowAppellation(playerId,appellation2,isShow);
			
		}
		public function hideSucced():void
		{
			GameCommonData.Player.IsShowAppellation = false;
			GameCommonData.Player.ShowAppellation(null);
			designationView.btn_change.txt_isShow.text = GameCommonData.wordDic[ "mod_des_med_des_initT_1" ];//"显示"
		}
		
		
		/**类容更新*/
		private function update(dItem:DesignationItem):void
		{
			if(!scrollContainer)
			{
				scrollContainer = new ScrollContainer();
				listContainer.addChild(scrollContainer);
			}
			scrollContainer.gc();
			scrollContainer.createCells(dItem.getDataArr(),listContainer.width,listContainer.height - 17*dItemArr.length);
			var tag:int = 0;
			for each(var tempItem:Object in dItemArr)
			{
				if((tempItem as DesignationItem).getNum() == dItem.getNum())
				{
					tempItem.y = tag*17;
					scrollContainer.y = (tag+1)*17;
				}
				else if((tempItem as DesignationItem).getNum() < dItem.getNum())
				{
					tempItem.y = tag*17;
				}
				else if((tempItem as DesignationItem).getNum() > dItem.getNum())
				{
					tempItem.y = tag*17 + scrollContainer.height+1;
				} 
				tag ++;
			}
		}
		
		
		/**点击具体小条目时触发的事件*/
		private function designatonEventHandler(de:DesignationEvent):void
		{
			_selectDataObj = de._data;
			designationView.txt_introduce.text = de._data.introduce;
			designationView.txt_condition.text = de._data.condition;
			if(_selectDataObj.isSelect == 1)
			{
				(designationView.btn_select as MovieClip).mouseEnabled = true;
				(designationView.btn_select as MovieClip).mouseChildren = true;
				(designationView.btn_select as MovieClip).alpha = 1;
				(designationView.btn_select as MovieClip).filters = null;
				
				(designationView.btn_change as MovieClip).mouseEnabled = true;
				(designationView.btn_change as MovieClip).mouseChildren = true;
				(designationView.btn_change as MovieClip).alpha = 1;
				(designationView.btn_change as MovieClip).filters = null;
			}
			else{
				(designationView.btn_select as MovieClip).mouseChildren = false;
				(designationView.btn_select as MovieClip).mouseEnabled = false;
//				(designationView.btn_select as MovieClip).alpha = 0.6;
				MasterData.setGrayFilter( designationView.btn_select );
				
				(designationView.btn_change as MovieClip).mouseChildren = false;
				(designationView.btn_change as MovieClip).mouseEnabled = false;
//				(designationView.btn_change as MovieClip).alpha = 0.6;
				MasterData.setGrayFilter( designationView.btn_change );
			}
		}
		
		
		/**更新称号后跟新从新获得dataArr*/
		private function updateDataArr():void
		{
			dataArr = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).offerData();
			var bool:Boolean = (facade.retrieveProxy(DesignationProxy.NAME) as DesignationProxy).isHaveDesignation(_selectDataObj);
			if(!bool)
			{
				designationView.txt_current.text = GameCommonData.wordDic[ "mod_des_med_des_initU" ];//"您当前没有称号";
				designationView.txt_introduce.text = "";
				designationView.txt_condition.text = "";
				_selectDataObj = null;
			}
			sendNotification(DesignationEvent.DEAL_AFTER_UPDATE);
		}
		
		
		/**获取当前的玩家的称号id*/
		public function getDesignatinId():int
		{
			return currentDid;
		}
		
		private function gc():void
		{
			removeEventListeners();
			if(scrollContainer)
			{
				scrollContainer.gc();
				if(listContainer.contains(scrollContainer))
				{
					listContainer.removeChild(scrollContainer);
					scrollContainer = null;
				}
			}
			designationView.removeChild(listContainer);
			for each(var obj:DesignationItem in dItemArr)
			{
				if(tempDesignationItem)
				{
					if(obj===tempDesignationItem)
					{
						listContainer.removeChild(obj);
						continue;
					}
				}
				listContainer.removeChild(obj);
				obj.removeEventListener(MouseEvent.CLICK,itemClickHandler);
				obj.gc();
				obj = null;
			}
			dItemArr = null;
			dataArr = null;
			listContainer = null;
			isItemClicked = false;
			dataProxy.designationPanIsOpen = false;

		}
	}
}

