package GameUI.Modules.Task.View
{
	import GameUI.View.Components.UISprite;
	import GameUI.View.ResourcesFactory;
	
	import OopsEngine.Graphics.Font;
	
	import flash.display.MovieClip;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;


	/**
	 * 任务组渲染器 
	 * @author net
	 * 
	 */	
	public class TaskGroupCellRenderer extends UISprite
	{
		private var isExpand:Boolean;
		private var des:String;
		private var icon:MovieClip;
		private var desTf:TextField;
		private var _expandIcon:MovieClip;
		private var _unexpandIcon:MovieClip;
		
		public function TaskGroupCellRenderer(des:String,isExpand:Boolean=false)
		{
			super();
			this.des=des;
			this.isExpand=isExpand;
			this.cacheAsBitmap=true;
			ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewTask.swf", onPanelLoadComplete);
			//this.createChildren();
		}
		
		public function createChildren():void{
			this.desTf=new TextField();
			var textFormat:TextFormat=new TextFormat("宋体",14);        //宋体
			this.desTf.filters=Font.Stroke();
			this.desTf.defaultTextFormat=textFormat;
			this.desTf.autoSize=TextFieldAutoSize.LEFT;
			this.desTf.type=TextFieldType.DYNAMIC;
			this.desTf.selectable=false;
			this.desTf.textColor = 0xffff00; 
			this.desTf.text=this.des;
			this.addChild(this.desTf);
			if(isExpand){
				this.icon = this._expandIcon;
			}else{
				this.icon = this._unexpandIcon;
			}
			this.addChild(this.icon);
			this.doLayout();	
		}
		
		protected function doLayout():void{
			 this.width=214;
			 this.height=16;
			 this.icon.width=14;
			 this.icon.height=14;
			 this.icon.x=0;
			 this.icon.y=2;
			 this.desTf.x=20;
			
		}
		
		public function onPanelLoadComplete():void
		{
			var taskSwf:MovieClip = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/NewTask.swf");
			this._unexpandIcon = new (taskSwf.loaderInfo.applicationDomain.getDefinition("unExpand"));
			this._expandIcon = new (taskSwf.loaderInfo.applicationDomain.getDefinition("expand"));
			this.createChildren();
		}

		
	}
}