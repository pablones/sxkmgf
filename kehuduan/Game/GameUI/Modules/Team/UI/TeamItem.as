package GameUI.Modules.Team.UI
{
	import GameUI.View.BaseUI.ItemBase;
	import GameUI.View.items.FaceItem;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.filters.GlowFilter;
	import flash.geom.Rectangle;
	import flash.text.TextField;
	
	/**
	 * 列表组件
	 * @author:Ginoo
	 * @date:05/25/2010
	 */
	public class TeamItem extends Sprite
	{
		public var item:MovieClip = null;
		public var index:int = 0;			//头像名字
		public var faceItem:FaceItem;		//头像
		private var lineInfo_txt:TextField;
		
		public function TeamItem(item:MovieClip, index:int)	//index 下标
		{
			this.item = item;
			this.index = index;
			init();
		}
		
		/** 初始化 */
		private function init():void 
		{
			item.mcMemberPhoto.mouseEnabled = false;
			item.txtMemberName.mouseEnabled = false;
			item.txtBusiness.mouseEnabled = false;
			item.txtRoleLevel.mouseEnabled = false;
			item.txtBusinessLevel.mouseEnabled = false;
			item.mcMouseOver.visible = false;
			item.mcMouseDown.visible = false;
			this.addChild(item);
		}
		
		/** 设置头像 */
		public function setPhoto(faceItem:FaceItem):void
		{
			if(faceItem == null) {	//删除头像
				var count:int = item.mcMemberPhoto.numChildren - 1;
				while(count >= 0)
				{
					if(item.mcMemberPhoto.getChildAt(count) is ItemBase)
					{
						var face:ItemBase = item.mcMemberPhoto.getChildAt(count) as ItemBase;
						item.mcMemberPhoto.removeChild(face);
						face = null;
					}
					count--;
				}
				this.faceItem = null;
				this.lineInfo_txt = null;
			} else {			//添加头像
				faceItem.MaskRect = new Rectangle(2, 2, 32, 32);
				this.faceItem = faceItem;
				lineInfo_txt = new TextField();
				lineInfo_txt.x = 8;
				lineInfo_txt.y = 18;
				lineInfo_txt.filters = Font.Stroke();
				lineInfo_txt.mouseEnabled = false;
				item.mcMemberPhoto.addChild(faceItem);
			}
		}
		
		/** 设置队员在线状态 */
		public function setOnlineState(type:uint):void
		{
			if(faceItem) {
				if(type == 0) {				//离线
					faceItem.Enabled = false;
					lineInfo_txt.htmlText = "<font color='#ff0000'>"+GameCommonData.wordDic[ "mod_team_ui_teami_set_1" ]+"</font>";           //离开
					if(!faceItem.contains(lineInfo_txt)) faceItem.addChild(lineInfo_txt);
				} else if(type == 1) {		//在线
					faceItem.Enabled = true;
					lineInfo_txt.text = "";
					if(faceItem.contains(lineInfo_txt)) faceItem.removeChild(lineInfo_txt);
				}
			}
		}
		
		/** 根据名称设置属性 */
		public function setTxtByName(txtName:String, txt:String="", color:String = ""):void
		{
			if(txtName) item[txtName].htmlText = "<font color='" +color+ "'>" + txt + "</font>";  
		}
		
		/** 根据名称读取属性 */
		public function getTxtByName(txtName:String):String
		{
			if(txtName) {
				return item[txtName].text;
			} else {
				return "";
			}
		}
		
		/** 设置滤镜 */
		public function setFilter(ifUse:Boolean=true, glowFilter:GlowFilter=null):void
		{
			if(ifUse) {
				item.filters = [glowFilter];
			} else {
				item.filters = null;
			}
		}
		
	}
}