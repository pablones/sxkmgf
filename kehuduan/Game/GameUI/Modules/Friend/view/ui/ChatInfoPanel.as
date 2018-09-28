package GameUI.Modules.Friend.view.ui
{
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class ChatInfoPanel extends Sprite
	{
		protected var _friendFeel:String;
		protected var _friendLevel:String;
		protected var _friendJob:String;
		protected var _friendLine:String;
		protected var _friendMap:String;
		protected var _friendFace:String;
		
		protected var _feel:String;
		protected var _level:String;
		protected var _job:String;
		protected var _line:String;
		protected var _map:String;
		protected var _face:String;
		
		protected var view:MovieClip;
		
		
		public function ChatInfoPanel()
		{
			super();
			this.createChildren();
			
		}
		
		/**
		 * 创建子元素 
		 * 
		 */		
		protected function createChildren():void{
			this.view=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("FriendChatInfoPanel");
			this.view.txt_friendFeel.mouseEnabled=false;
			this.view.txt_friendLevel.mouseEnabled=false;
			this.view.txt_friendJob.mouseEnabled=false;
			this.view.txt_friendLine.mouseEnabled=false;
			this.view.txt_friendMap.mouseEnabled=false;
			
			this.view.txt_feel.mouseEnabled=false;
			this.view.txt_level.mouseEnabled=false;
			this.view.txt_job.mouseEnabled=false;
			this.view.txt_line.mouseEnabled=false;
			this.view.txt_map.mouseEnabled=false;
			
			this.addChild(this.view);
		}
		
		public function set friendFeel(value:String):void{
			if(value==this._friendFeel)return;
			this._friendFeel=value;
			this.view.txt_friendFeel.text=value;
		}
		public function set friendLevel(value:String):void{
			if(value==this._friendLevel)return;
			this._friendLevel=value;
			this.view.txt_friendLevel.text=value;
		}
		public function set friendJob(value:String):void{
			if(value==this._friendJob)return;
			this._friendJob=value;
			this.view.txt_friendJob.text=value;
		}
		public function set friendLine(value:String):void{
			if(value==this._friendLine)return;
			this._friendLine=value;
			this.view.txt_friendLine.text=value;
		}
		public function set friendMap(value:String):void{
			if(value==this._friendMap)return;
			this._friendMap=value;
			this.view.txt_friendMap.text=value;
		}
		public function set friendFace(value:String):void{
			if(value==this._friendFace)return;
			this._face=value;
			var mc_friendFace:MovieClip=this.view.mc_friendFace as MovieClip;
			while(mc_friendFace.numChildren>0){
				mc_friendFace.removeChildAt(0);
			}
			var face:FaceItem=new FaceItem(value,null,"face");
			mc_friendFace.addChild(face);	
		}
		
		public function set feel(value:String):void{
			if(value==this._feel)return;
			this._feel=value;
			this.view.txt_feel.text=value;
		}
		public function set level(value:String):void{
			if(value==this._level)return;
			this._level=value;
			this.view.txt_level.text=value;
		}
		public function set job(value:String):void{
			if(value==this._job)return;
			this._job=value;
			this.view.txt_job.text=value;
		}
		public function set line(value:String):void{
			if(value==this._line)return;
			this._line=value;
			this.view.txt_line.text=value;
			
		}
		public function set map(value:String):void{
			if(value==this._map)return;
			this._map=value;
			this.view.txt_map.text=value;
		}
		public function set face(value:String):void{
			if(value==this._face)return;
			this._face=value;
			var mc_myFace:MovieClip=this.view.mc_myFace as MovieClip;
			while(mc_myFace.numChildren>0){
				mc_myFace.removeChildAt(0);
			}
			var face:FaceItem=new FaceItem(value,null,"face");
			mc_myFace.addChild(face);
		}
		
		
		
		
		
		
	}
}