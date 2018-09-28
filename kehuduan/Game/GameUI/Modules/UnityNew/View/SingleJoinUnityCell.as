package GameUI.Modules.UnityNew.View
{
	import GameUI.Modules.Unity.Data.UnityConstData;
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityVo;
	import GameUI.UICore.UIFacade;
	
	import Net.ActionSend.UnityActionSend;
	
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class SingleJoinUnityCell extends Sprite implements IUnityCell
	{
		private var vo:UnityVo;
		public var clickMe:Function;
		
		private var main_mc:MovieClip;
		
		public function SingleJoinUnityCell( _vo:UnityVo )
		{
			vo = _vo;
		}

		public function init():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.SingleJoinUnityCellClass ) as MovieClip;
			
			( main_mc[ "txt_0" ] as TextField ).htmlText = vo.rank.toString();
			( main_mc[ "txt_1" ] as TextField ).htmlText = vo.name;
			( main_mc[ "txt_2" ] as TextField ).htmlText = vo.boss;
			( main_mc[ "txt_3" ] as TextField ).htmlText = vo.level.toString();
			( main_mc[ "txt_4" ] as TextField ).htmlText = vo.currentPeople.toString() + "/" + vo.maxPeople.toString();
			( main_mc[ "join_btn" ] as SimpleButton ).addEventListener( MouseEvent.CLICK,pleaseJoin );
			( main_mc[ "look_btn" ] as SimpleButton ).addEventListener( MouseEvent.CLICK,lookInfo );
			
			for ( var i:uint=0; i<5; i++ )
			{
				( main_mc[ "txt_" + i ] as TextField ).mouseEnabled = false;
			}
			
			
			addChild( main_mc );
			
			this.addEventListener( MouseEvent.CLICK,onClick );
		}
		
		private function pleaseJoin( evt:MouseEvent ):void
		{
			if ( GameCommonData.Player.Role.Level<15 )
			{
				GameCommonData.UIFacadeIntance.ShowHint( GameCommonData.wordDic[ "mod_unityN_vie_unib_sin_1" ] );    //15级才可以加入帮派
				return;
			}
			var obj:Object = new Object();
			obj.type = 1107;								
			obj.data = [ 0, 0, 206, 0, vo.id ];
			UnityActionSend.SendSynAction( obj );	
		}
		
		private function lookInfo( evt:MouseEvent ):void
		{
			UnityConstData.oneUnityId = this.vo.id;
			UIFacade.GetInstance( UIFacade.FACADEKEY ).sendNotification( NewUnityCommonData.SHOW_NEW_UNITY_LOOK_INFO,this.vo.id );
		}
		
		public function gc():void
		{
			this.removeEventListener( MouseEvent.CLICK,onClick );
			( main_mc[ "join_btn" ] as SimpleButton ).removeEventListener( MouseEvent.CLICK,pleaseJoin );
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