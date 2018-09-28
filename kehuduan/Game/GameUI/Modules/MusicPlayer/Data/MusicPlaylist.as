package GameUI.Modules.MusicPlayer.Data
{
	import GameUI.View.UIKit.data.ArrayList;
	
	public class MusicPlaylist extends ArrayList
	{
		public function MusicPlaylist(listXML:XML):void
		{
			var list:XMLList = listXML.music;
			for each (var item:XML in list)
			{
				addItem({name:String(item.@name), nameWithScene:String(item.@nameWithScene), sceneID:String(item.@sceneID), length:int(item.@length)});
			}
		}
		
		override public function query(property:String, value:*):Array
		{
			if (property == "sceneID")
			{
				var l:int = length;
				var result:Array = [];
				
				for (var i:int = 0; i < l; i ++)
				{
					if (getItemAt(i)[property].indexOf(value.toString()) != -1)
					{
						result.push(i);
					}
				}
				
				return result;
			}
			else
			{
				return super.query(property, value);
			}
		}
	}
}