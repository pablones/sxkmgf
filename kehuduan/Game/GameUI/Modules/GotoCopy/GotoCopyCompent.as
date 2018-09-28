package GameUI.Modules.GotoCopy
{
	import GameUI.View.Components.UISprite;
	
	import flash.display.DisplayObject;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;

	public class GotoCopyCompent extends UISprite
	{
		private var _copyData:Array;
		private var _playerData:Object;
		/**
		 * 列表组件
		 * @param copyData 副本条件列表
		 * @param playerData 玩家数据
		 * 
		 */		
		public function GotoCopyCompent(copyData:Array,playerData:Object)
		{
			_copyData = copyData;
			_playerData = playerData;
			super();
			init();
		}
		private function init():void
		{
			
			var tName:TextField  = new TextField();
			tName.text = GameCommonData.wordDic[ "mod_got_got_init_1" ]+"  "+_playerData.name;	//"队员"
			tName.textColor = 0x00B4FF;
			tName.autoSize = TextFieldAutoSize.LEFT;
			tName.y = 0;
			tName.x = 0;
			this.addChild(tName);
			
			var tag:int = _playerData.condition;
//			tag = 101;//注释 里++++++++++++++++++++++++++++++++++++++++
			for(var i:int = 1; i < _copyData.length; i++)
			{
				var tCon:TextField = new TextField();
				var str:String;
				var color:uint;

				if(int(tag%Math.pow(10,i)/Math.pow(10,i-1)) == 1) 
				{
					str = _copyData[i]+"  "+ GameCommonData.wordDic[ "mod_got_got_init_2" ];//"满足";
					color = 0x0000FF00;
				}
				else  
				{
					str = _copyData[i]+"  "+ GameCommonData.wordDic[ "mod_got_got_init_3" ];//"不满足";
					color = 0xFF0000;
				}
				tCon.text = str;
				tCon.textColor = color;
				tCon.autoSize = TextFieldAutoSize.LEFT;
				tCon.x = 20;
				tCon.y = i*tCon.height;
				this.addChild(tCon);
			}
			this.width = 226;
			this.height = tName.height*_copyData.length;
		}
		
		/**
		 * 清除垃圾
		 */		
		public function dipose():void
		{
			var num:int = this.numChildren - 1;
			while(num >= 0)
			{
				var dis:DisplayObject = this.getChildAt(0);
				this.removeChild(dis);
				dis = null;
				num --;
			}
			
			_copyData = null;
			_playerData = null;
			
		} 
	}
}