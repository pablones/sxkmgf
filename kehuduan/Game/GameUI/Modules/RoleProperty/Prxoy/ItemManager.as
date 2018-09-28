package GameUI.Modules.RoleProperty.Prxoy
{
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Bag.BagUtils;
	import GameUI.Modules.Bag.Proxy.BagData;
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.Chat.Data.ChatEvents;
	import GameUI.Modules.Hint.Events.HintEvents;
	import GameUI.Modules.RoleProperty.Datas.RolePropDatas;
	import GameUI.Modules.RoleProperty.Net.NetAction;
	import GameUI.Modules.Soul.Data.SoulData;
	import GameUI.Modules.Soul.Data.SoulVO;
	import GameUI.View.items.DropEvent;
	
	import Net.ActionProcessor.OperateItem;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.utils.Timer;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class ItemManager extends Proxy
	{
		public static const NAME:String = "ItemManager";
		private var redFrame:MovieClip = null;
		private var yellowFrame:MovieClip = null;
		private var changeEqTimer:Timer = new Timer(2000, 1);
		
		public function ItemManager(list:Array=null)
		{
			super(NAME);
		}
		
		public function Initialize():void
		{
			redFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("RedFrame");
			redFrame.name = "redFrame";
			redFrame.mouseEnabled = false;
			yellowFrame = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("YellowFrame");
			yellowFrame.name = "yellowFrame";
			yellowFrame.mouseEnabled = false;		
			for(var i:int = 0; i<RolePropDatas.ItemUnitList.length; i++)
			{
				var itemUnit:ItemUnit = RolePropDatas.ItemUnitList[i] as ItemUnit;
//				itemUnit.Grid.addEventListener(MouseEvent.MOUSE_OVER, onMouseMove); 
//				itemUnit.Grid.addEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				itemUnit.Grid.addEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				itemUnit.Grid.doubleClickEnabled = true;
				itemUnit.Grid.addEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
		
		private function onMouseMove(event:MouseEvent):void
		{
			event.currentTarget.parent.addChild(redFrame);
			redFrame.x = event.currentTarget.x;
			redFrame.y = event.currentTarget.y; 
		}
		
		private function onMouseOut(event:MouseEvent):void
		{
			if(event.currentTarget.parent.getChildByName("redFrame")) 
    		{
    			event.currentTarget.parent.removeChild(event.currentTarget.parent.getChildByName("redFrame"));
    		}
		}
		
		private function onMouseDown(event:MouseEvent):void
		{
			var count:int = event.currentTarget.parent.numChildren-1;
			while(count)
			{
				if(event.currentTarget.parent.getChildByName("yellowFrame")) 
    			{
    				event.currentTarget.parent.removeChild(event.currentTarget.parent.getChildByName("yellowFrame"));
    			}
    			count--;
   			}
   			if(GameCommonData.Player.Role.HP == 0)
   			{
   				return;
   			}
			var index:int = int(event.target.name.split("_")[1])-1;
			if(RolePropDatas.ItemUnitList[index].Item)
			{
				var displayObj:DisplayObject = event.target as DisplayObject;
				if(displayObj.mouseX <= 2 || displayObj.mouseX >= displayObj.width - 2){
					return;
				}
				if(displayObj.mouseY <= 2 || displayObj.mouseY >= displayObj.height - 2){
					return;
				}
				//-------------
				var clickItem:Object = RolePropDatas.ItemUnitList[index].Item;
				if(event.ctrlKey)
				{
					if(UIConstData.getItem(clickItem.Type) == null) return;
					var id:int = clickItem.Id;
					var type:int = clickItem.Type;
					var name:String = UIConstData.getItem(clickItem.Type).Name;
					var isBind:int = clickItem.IsBind;
					//当按下Ctrl键单击物品的时候，创建快速链接物品，目前只做装备的快速链接，以后需要再加上type区间
					//格式:id_type_name_玩家id_pos
					//id:物品ID，type:物品类型，name:物品名称, isBind:物品是否绑定
					var color:uint = 0;
					var obj:Object = BagData.getItemDataByIdInIntroConst(id);
					if(obj) color = obj.color;
					
					if ( type>=250000 && type<300000 )					//魂魄的名字特殊处理
					{
						var soulVo:SoulVO = SoulData.SoulDetailInfos[ id ];
						if ( soulVo )
						{
							if ( soulVo.belong==1 )
							{
								name = "九阳之魄";
							}
							else
							{
								name = "九阴之魂";
							}
						}
					}
					
					if(ChatData.SetLeoIsOpen) {		//小喇叭打开状态
						facade.sendNotification(ChatEvents.ADD_ITEM_LEO, "<1_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_"+isBind+"_"+color+">");
					} else {
						facade.sendNotification(ChatEvents.ADDITEMINCHAT, "<1_["+name+"]_"+id+"_"+type+"_"+GameCommonData.Player.Role.Id+"_"+isBind+"_"+color+">");
					}
				}
				//-------------
				RolePropDatas.ItemUnitList[index].Item.onMouseDown();
//				RolePropDatas.ItemUnitList[index].Item.addEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
				RolePropDatas.ItemUnitList[index].Item.addEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
//				RolePropDatas.SelectedOutfit = RolePropDatas.ItemUnitList[index];
			}
		}
		
		private function dragThrewHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_THREW, dragThrewHandler);
			if(RolePropDatas.SelectedOutfit.Item)
			{
				//facade.sendNotification(EventList.SHOWALERT, {comfrim:bagMediator.comfrim, cancel:bagMediator.cancel, info:"是否扔掉该物品"});
			}
		}
		
		private function dragDroppedHandler(e:DropEvent):void
		{
			e.target.removeEventListener(DropEvent.DRAG_DROPPED, dragDroppedHandler);
			switch(e.Data.type)
			{
				case "bag":
					if(BagData.SelectIndex == 0)
					{
						if(BagData.GridUnitList[e.Data.index].Item ==null)
						{
							NetAction.UnEquip(OperateItem.UNEQUIP, 1, e.Data.source.Pos+1, 47, e.Data.index+1);
						}
					}
				break;
			}
		}
		
		private function doubleClickHandler(e:MouseEvent):void
		{
			var nullPos:int = BagUtils.getNullItemIndex(0);
			var index:uint = uint((e.target as MovieClip).name.split("_")[1]);
			if(RolePropDatas.ItemUnitList[index-1] == undefined || RolePropDatas.ItemUnitList[index-1].Item == null) {
				return;
			}
			if(nullPos > -1) {
				if(changeEqTimer.running) {
					facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "often_used_waite" ], color:0xffff00});  //"请稍候"
					return;
				}
				NetAction.UnEquip(OperateItem.UNEQUIP, 1, index, 47, nullPos+1); 
				changeEqTimer.reset();
				changeEqTimer.start();
			} else {
				facade.sendNotification(HintEvents.RECEIVEINFO, {info:GameCommonData.wordDic[ "mod_rp_prx_im_1" ], color:0xffff00});   //"道具背包已满"
			}
		}
		
		public function Gc():void
		{
			for(var i:int = 0; i<RolePropDatas.ItemUnitList.length; i++)
			{
				var itemUnit:ItemUnit = RolePropDatas.ItemUnitList[i] as ItemUnit;
//				itemUnit.Grid.removeEventListener(MouseEvent.MOUSE_OVER, onMouseMove);
//				itemUnit.Grid.removeEventListener(MouseEvent.MOUSE_OUT, onMouseOut);
				itemUnit.Grid.removeEventListener(MouseEvent.MOUSE_DOWN, onMouseDown);
				itemUnit.Grid.removeEventListener(MouseEvent.DOUBLE_CLICK, doubleClickHandler);
			}
		}
	}
}