package GameUI.Modules.UnityNew.View
{
	import GameUI.Modules.UnityNew.Data.NewUnityCommonData;
	import GameUI.Modules.UnityNew.Proxy.UnityMemberVo;
	import GameUI.UIUtils;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.text.TextField;
	
	public class SingleContributeListCell extends Sprite implements IUnityCell
	{
		public var clickMe:Function;
		public var unityVo:UnityMemberVo; 
		
		private var main_mc:MovieClip;
		
		public function SingleContributeListCell( _vo:UnityMemberVo )
		{
			unityVo = _vo;
		}

		public function init():void
		{
			main_mc = new ( NewUnityCommonData.newUnityResProvider.SingleContributeListCellClass ) as MovieClip;
			
			( main_mc[ "txt_0" ] as TextField ).htmlText = unityVo.name;
			( main_mc[ "txt_1" ] as TextField ).htmlText = unityVo.roleLevel.toString();
			( main_mc[ "txt_2" ] as TextField ).htmlText = unityVo.unityJobStr;
			( main_mc[ "txt_3" ] as TextField ).htmlText = unityVo.totalContribute.toString();
			( main_mc[ "txt_4" ] as TextField ).htmlText = unityVo.jianseContribute.toString();
			( main_mc[ "txt_5" ] as TextField ).htmlText = unityVo.moneyContribute.toString();
			( main_mc[ "txt_6" ] as TextField ).htmlText = unityVo.lastLoginTimeStr;
			
			for ( var i:uint=0; i<7; i++ )
			{
				( main_mc[ "txt_"+i ] as TextField ).textColor = UIUtils.getVipColor( unityVo.vip );
				( main_mc[ "txt_"+i ] as TextField ).mouseEnabled = false;
			}
			addChild( main_mc );
			
			this.addEventListener( MouseEvent.CLICK,onClick );
		}
		
		public function gc():void
		{
			this.removeEventListener( MouseEvent.CLICK,onClick );
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