package GameUI.Modules.UnityNew.View
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityVo;
	import GameUI.UICore.UIFacade;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class SingleHasUnityListCell extends Sprite implements IUnityCell
	{
		private var vo:UnityVo;
		public var clickMe:Function;
		
		private var main_mc:MovieClip;
		
		public function SingleHasUnityListCell( _vo:UnityVo )
		{
			vo = _vo;
		}

		public function init():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.SingleHasUnityListCellClass ) as MovieClip;
			
			( main_mc[ "txt_0" ] as TextField ).htmlText = vo.rank.toString();
			( main_mc[ "txt_1" ] as TextField ).htmlText = vo.name;
			( main_mc[ "txt_2" ] as TextField ).htmlText = vo.boss;
			( main_mc[ "txt_3" ] as TextField ).htmlText = vo.level.toString();
			( main_mc[ "txt_4" ] as TextField ).htmlText = vo.currentPeople.toString() + "/" + vo.maxPeople.toString();
			( main_mc[ "look_btn" ] as SimpleButton ).addEventListener( MouseEvent.CLICK,lookInfo );
			
			for ( var i:uint=0; i<5; i++ )
			{
				( main_mc[ "txt_" + i ] as TextField ).mouseEnabled = false;
			}
			
			
			addChild( main_mc );
			
			this.addEventListener( MouseEvent.CLICK,onClick );
		}
		
		private function lookInfo( evt:MouseEvent ):void
		{
			UnityConstData.oneUnityId = this.vo.id;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( NewUnityCommonData.SHOW_NEW_UNITY_LOOK_INFO,this.vo.id );
		}
		
		public function gc():void
		{
			this.removeEventListener( MouseEvent.CLICK,onClick );
			( main_mc[ "look_btn" ] as SimpleButton ).removeEventListener( MouseEvent.CLICK,lookInfo );
			
			clickMe = null;
			var des:*;
			while ( this.numChildren> 0 )
			{
				des = removeChildAt( 0 );
				des = null;
			}
		}
		
		private function onClick( evt:MouseEvent ):void
		{
			if ( clickMe != null )
			{
				clickMe( this );
			}
		}
		
	}
}