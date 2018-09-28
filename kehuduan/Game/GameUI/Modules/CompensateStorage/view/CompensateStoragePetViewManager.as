package GameUI.Modules.CompensateStorage.view
{
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.View.BaseUI.ListComponent;
	
	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class CompensateStoragePetViewManager implements CSViewManager
	{
		private var _parent:DisplayObjectContainer;
		private var petView:MovieClip;
		
		//补偿仓库宠物栏
		private var listView:ListComponent = null;
		
		public function CompensateStoragePetViewManager(parent:DisplayObjectContainer)
		{
			_parent = parent;
		}
		
		public function init():void
		{
			if( CompensateStorageData.domain.hasDefinition( "CompensateStoragePet" ) )
			{
				var CompensateStorageItem:Class = CompensateStorageData.domain.getDefinition( "CompensateStoragePet" ) as Class;
				petView = new CompensateStorageItem();
			}
			(petView.txt_CompensateStoragePet as TextField).mouseEnabled = false;
			CompensateStorageData.isInitPetView = true;
		}
		
		public function show(object:Object = null):void
		{
			if(!CompensateStorageData.isInitPetView)
			{
				init();
			}
			if(!CompensateStorageData.isShowPetView)
			{
				_parent.addChildAt(petView,0);
				CompensateStorageData.isShowPetView = true;
			}
			showFilterList();
			CompensateStorageData.selectedId = 0;
		}
		
		/** 显示宠物选择列表数据 */
		private function showFilterList():void
		{
			if(listView && petView.contains(listView)) {
				petView.removeChild(listView);
				listView = null;
			}
			listView = new ListComponent(false);
			listView.x = CompensateStorageData.petListView[0];
			listView.y = CompensateStorageData.petListView[1];
			petView.addChild(listView);
			
			if(CompensateStorageData.petList.length > 0)
			{
				petView.txt_CompensateStoragePet.text = CompensateStorageData.word10+"：";
			}else
			{
				petView.txt_CompensateStoragePet.text = CompensateStorageData.word10+"：" + CompensateStorageData.word9;
			}
			var i:int = 0;
			for(i = 0; i < CompensateStorageData.petList.length ; i++)
			{
				var item:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("PetListItemDepot");
				item.name = "CompensateStoragePetList_" + CompensateStorageData.petList[i].id;
				item.mcSelected.visible = false;
				item.mcSelected.mouseEnabled = false;
				item.width = CompensateStorageData.petListView[2];
				item.btnChosePet.width = CompensateStorageData.petListView[2];
				item.mcSelected.width = CompensateStorageData.petListView[2];
				item.txtName.width = CompensateStorageData.petListView[2];
				item.txtName.mouseEnabled = false;
				item.txtName.text = CompensateStorageData.getPetNameByType(CompensateStorageData.petList[i].type%10000000);
				item.addEventListener(MouseEvent.MOUSE_DOWN, selectItem);
				item.addEventListener(MouseEvent.DOUBLE_CLICK, onDouble);
				listView.addChild(item);
			}
			listView.width = CompensateStorageData.petListView[2];
			listView.upDataPos();
		}
		
		private function selectItem(event:MouseEvent):void
		{
			var item:MovieClip = event.currentTarget as MovieClip;
			var id:uint = uint(item.name.split("_")[1]);
			CompensateStorageData.selectedId = id;
			
			for(var i:int = 0; i < listView.numChildren; i++)
			{
				(listView.getChildAt(i) as MovieClip).mcSelected.visible = false;
			}
			item.mcSelected.visible = true;
		}
		
		private function onDouble(event:MouseEvent):void
		{
//			CompensateStorageData.getOut();
		}
		
		public function close(event:Event = null):void
		{
			if(CompensateStorageData.isShowPetView)
			{
				_parent.removeChild(petView);
				CompensateStorageData.isShowPetView = false;
			}
		}
	}
}