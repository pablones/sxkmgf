package GameUI.Modules.Soul.Proxy
{
	import GameUI.Modules.Soul.Mediator.SoulMediator;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.Person.GameElementTernal;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	
	import flash.display.MovieClip;
	
	import org.puremvc.as3.multicore.patterns.proxy.Proxy;

	public class ShowSoulComponent extends Proxy implements IUpdateable
	{		
		public static const NAME:String = "ShowSoulComponentProxy";
		
		private var soulView:MovieClip;
		private var soulComposeView:MovieClip;
		public var gameElementTernal:GameElementTernal; //武魂对象
	    public var composeElementTernal:GameElementTernal; //武魂合成对象
		
		public function ShowSoulComponent(proxyName:String=null, data:Object=null)
		{
			super(NAME, data);
			
		}		
		
		public function Update(gameTime:GameTime):void
		{
			if(gameElementTernal != null)
			{
				gameElementTernal.Update(gameTime);
			}			
			if(composeElementTernal != null)
			{
				composeElementTernal.Update(gameTime);
			}
		}
		
		public function showView(data:Object):void
		{
			if(!this.soulView)
			{
				soulView = (facade.retrieveMediator(SoulMediator.NAME) as SoulMediator).soulView;
			}
//			GameCommonData.GameInstance.GameUI.Elements.Add(soulView);
		    gameElementTernal = new GameElementTernal(GameCommonData.GameInstance);
	        gameElementTernal.Role = new GameRole();
	        gameElementTernal.Role.WeaponSkinName = "Resources/Player/Ternal/w1.swf";
	        if(data.composeLevel >= 5)
	        {
	      		gameElementTernal.Role.WeaponEffectName = "Resources/Player/Ternal/w1"+data.style+".swf";
	        }
	        gameElementTernal.x = 80;
	        gameElementTernal.y = 75;
	        gameElementTernal.SetParentScene(GameCommonData.GameInstance.GameScene.GetGameScene);
	        gameElementTernal.Initialize();
			soulView.addChild(gameElementTernal);
		    GameCommonData.GameInstance.GameUI.Elements.Add(this);	
		}
		public function deleteView():void
		{
//			GameCommonData.GameInstance.GameUI.Elements.Remove(soulView);
			if(soulView != null && gameElementTernal != null && soulView.contains(gameElementTernal))
			{
				soulView.removeChild(gameElementTernal);
				gameElementTernal.Dispose();
				gameElementTernal = null;
			}
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);	
		}
		
		public function showComposeView(data:Object):void
		{
			if(!this.soulComposeView)
			{
				soulComposeView = (facade.retrieveMediator(SoulMediator.NAME) as SoulMediator).soulComposeView;
			}
//			GameCommonData.GameInstance.GameUI.Elements.Add(soulView);
		    composeElementTernal = new GameElementTernal(GameCommonData.GameInstance);
	        composeElementTernal.Role = new GameRole();
	        composeElementTernal.Role.WeaponSkinName = "Resources/Player/Ternal/w1.swf";
	        if(data.composeLevel >= 5)
	        {
	      		composeElementTernal.Role.WeaponEffectName = "Resources/Player/Ternal/w1"+data.style+".swf";
	        }
	        composeElementTernal.x = 80;
	        composeElementTernal.y = 100;
	        composeElementTernal.SetParentScene(GameCommonData.GameInstance.GameScene.GetGameScene);
	        composeElementTernal.Initialize();
			soulComposeView.addChild(composeElementTernal);
		    GameCommonData.GameInstance.GameUI.Elements.Add(this);	
		}
		
		public function deleteComposeView():void
		{
//			GameCommonData.GameInstance.GameUI.Elements.Remove(soulView);
			if(soulComposeView != null && composeElementTernal != null && soulComposeView.contains(composeElementTernal))
			{
				soulComposeView.removeChild(composeElementTernal);
				composeElementTernal.Dispose();
				composeElementTernal = null;
			}
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);	
		}
		
		public function get Enabled():Boolean{return true;}
		
		public function get UpdateOrder():int{	return 0;	}
		
		public function get EnabledChanged():Function{	return null;	}
		
		public function set EnabledChanged(value:Function):void	{	}
		
		public function get UpdateOrderChanged():Function{	return null;	}
		
		public function set UpdateOrderChanged(value:Function):void	{	}
		
	}
}