package GameUI.Modules.Equipment.ui
{
	import GameUI.Modules.Equipment.ui.event.GridCellEvent;
	import GameUI.Modules.ToolTip.Const.IntroConst;
	import GameUI.View.Components.UISprite;
	import GameUI.View.items.UseItem;
	
	import flash.display.MovieClip;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class EquipCell extends UISprite implements IDataGridCell
	{
		protected var _colorArray:Array= ["", GameCommonData.wordDic[ "mod_equ_ui_equ_colorArray_1" ], GameCommonData.wordDic[ "mod_equ_ui_equ_colorArray_2" ], GameCommonData.wordDic[ "mod_equ_ui_equ_colorArray_3" ],GameCommonData.wordDic[ "mod_equ_ui_equ_colorArray_4" ] ,GameCommonData.wordDic[ "mod_equ_ui_equ_colorArray_5" ] ]
		//"优秀的"		"杰出的"		"卓越的"		"完美的"		"神圣的"
		protected var _view:MovieClip;
		protected var _imgType:String="";
		protected var useItem:UseItem;
		protected var _data:*;
		protected var isLock:Boolean;
		
		public function EquipCell()
		{
			this._view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("EquipCell");
			this.addChild(this._view);	
			(this._view.txt_des1 as TextField).textColor=0xfffe99;	
			(this._view.txt_des2 as TextField).textColor=0xfffe99;
		}
		
		public function set cellData(value:Object):void
		{
			this._data=value;
			var obj:Object=IntroConst.ItemInfo[value.id];
			if(this.useItem!=null)this._view.removeChild(this.useItem);
			var str1:String=this._colorArray[obj.quality];
			str1+='<font color="'+IntroConst.itemColors[obj.color]+'">'+obj.itemName+'</font>';
			this._view.txt_des2.htmlText='<font color="#e2cca5">'+str1+'</font>';
			var str2:String="";
			useItem=new UseItem(-1,obj.type,null);
			useItem.mouseEnabled=true;
			useItem.addEventListener(MouseEvent.CLICK,onUseItemClick);
			useItem.name="bagQuickKey_"+value.id;
			useItem.Id=obj.id;
			useItem.Type=obj.type;
			useItem.IsLock=value.isLock;
			useItem.x=useItem.y=4;
			var arr:Array=obj.stoneList;
			this._view.addChild(useItem);
			switch(value.page){
				case 0:
					str2=GameCommonData.wordDic[ "mod_equ_ui_equ_cel_1" ]+':'+'<font color="#00ff00">+'+obj.level+'</font>';//强化
					break;
				case 1:
					str2=GameCommonData.wordDic[ "mod_equ_ui_equ_cel_2" ]+'：'+'<font color="#00ff00">'+obj.star+GameCommonData.wordDic[ "mod_equ_ui_equ_cel_8" ]+'</font>';//星级		星
					break;	
				case 2:
					str2="";
					break;
				case 3:
					var count:uint=0;
					for each(var i:uint in arr){
						if(i!=99999 && i!=0){
							count++;
						}
					}
					if(count==3){
						str2=GameCommonData.wordDic[ "mod_equ_ui_equ_cel_3" ]+'：<font color="#00ff00">'+count+'</font>';//孔数
					}else{
						str2=GameCommonData.wordDic[ "mod_equ_ui_equ_cel_3" ]+'：'+count;//孔数
					}
					
					break;		
				case 4:
					
					var canEnchase:uint=0;
					var enchased:uint=0;
					for each(var k:uint in arr){
						if(k!=99999 && k!=0){
							if(k!=88888){
								enchased++;
							}
							canEnchase++;							
						}
					}
					str2=GameCommonData.wordDic[ "mod_equ_ui_equ_cel_4" ]+'：'+enchased+'/'+'<font color="#00ff00">'+canEnchase+'</font>';//可镶嵌数
					break;	
				case 5:
					var canExtirpate:uint=0;
					for each(var j:uint in arr){
						if(j!=99999 && j!=0 && j!=88888){
							canExtirpate++;
						}
					}
					
					str2=GameCommonData.wordDic[ "mod_equ_ui_equ_cel_5" ]+'：<font color="#00ff00">'+ canExtirpate+'</font>';//宝石数
					break;	
				case 6:
					if(obj.isBind==2){
						str2='<font color="#00ff00">'+GameCommonData.wordDic[ "mod_equ_ui_equ_cel_6" ]+'</font>';//已魂印
					}else{
						str2='<font color="#ff0000">'+GameCommonData.wordDic[ "mod_equ_ui_equ_cel_7" ]+'</font>';//未魂印
					}
					break;			
			}
			this._view.txt_des1.htmlText='<font color="#e2cca5">'+str2+'</font>';
		}
		
		public function dispose():void
		{
//			this.useItem.removeEventListener(MouseEvent.CLICK,onUseItemClick);
		}
		
		protected function onUseItemClick(e:MouseEvent):void{
			if(this.useItem.IsLock)return;
			var e1:GridCellEvent=new GridCellEvent(GridCellEvent.GRIDCELL_CLICK);
			e1.data=this._data;
			this.dispatchEvent(e1);	
		}
		
		
		public function get cellWidth():uint
		{
			return this._view.width;
		}
		
		
		public function get cellHeight():uint
		{
			return this._view.height;
		}
		
	}
}