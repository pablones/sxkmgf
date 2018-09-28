package GameUI.Modules.Maket.Mediator
{
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Maket.Data.MarketEvent;
	import GameUI.Proxy.DataProxy;
	import GameUI.View.BaseUI.PanelBase;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Role.SkinNameController;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPlayer;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.display.SimpleButton;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.text.TextField;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class MarketPreviewMediator extends Mediator implements IUpdateable
	{
		public static const NAME:String = "MarketPreviewMediator";
		
		private var panelBase:PanelBase;
		private var dataProxy:DataProxy;
		private var marketPreview:MovieClip;
		
		private var _goods:Object;
		private var _direction:int;
		private var leftBtn:SimpleButton;
		private var rightBtn:SimpleButton;
		private var pageTxt:TextField;
		private var previousBtn:SimpleButton;
		private var nextBtn:SimpleButton;
		private var _current:int = 0;
		private var modelNum:int = 0;
		
		private var mask:DisplayObject;

		public static var location:Point = new Point();
				
		public var animal:GameElementAnimal;
		
		public function MarketPreviewMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
				MarketEvent.INIT_MARKET_PREVIEW,
				MarketEvent.SHOW_MARKET_PREVIEW,
				MarketEvent.CLOSE_MARKET_PREVIEW
			];
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case MarketEvent.INIT_MARKET_PREVIEW:
					initView();
					break;
				case MarketEvent.SHOW_MARKET_PREVIEW:
					if(notification.getBody()) _goods = notification.getBody();
					if(!marketPreview) initView();
					modelNum = _goods.previewModel.length;
					isShowPreNext(modelNum);
					current = 0;
					panelBase.x = MarketPreviewMediator.location.x;
					panelBase.y = MarketPreviewMediator.location.y;
					break;
				case MarketEvent.CLOSE_MARKET_PREVIEW:									
					if(MarketConstData.PreviewIsOpen) close();
					break;
			}
		}
		
		private function initView():void
		{
			this.marketPreview = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("MarketPreview");
			dataProxy = facade.retrieveProxy(DataProxy.NAME) as DataProxy;
			panelBase = new PanelBase(marketPreview, marketPreview.width + 8, marketPreview.height + 12);
			panelBase.name = "MarketPreview";
			panelBase.SetTitleTxt(GameCommonData.wordDic[ "Modules_Maket_Mediator_MarketPreviewMediator_initView" ]/**"预览"*/);
			
			leftBtn = marketPreview["btn_marketPreview_left"];
			rightBtn = marketPreview["btn_marketPreview_right"];
			mask = marketPreview["mc_marketPreview_mask"];
			
			pageTxt = marketPreview["txt_marketPreview_page"];
			previousBtn = marketPreview["btn_marketPreview_pre"];
			nextBtn = marketPreview["btn_marketPreview_next"];
		}
		
		private function show(index:int):void
		{
			if(!MarketConstData.PreviewIsOpen)
			{
				GameCommonData.GameInstance.GameUI.addChild(this.panelBase);
				
				addLis();
			}
			removeView();
			updateItem({previewType:_goods.previewType,previewModel:_goods.previewModel[index][0],previewX:_goods.previewModel[index][1],previewY:_goods.previewModel[index][2]});
			MarketConstData.PreviewIsOpen = true;
			if(panelBase && GameCommonData.GameInstance.GameUI.contains(panelBase))
			{
				GameCommonData.GameInstance.GameUI.setChildIndex(panelBase,GameCommonData.GameInstance.GameUI.numChildren - 1);
			}
		}
		
		public function Update(gameTime:GameTime):void
		{
			if(animal != null)
			{		    
				animal.mouseEnabled = false;
		    	animal.mouseChildren = false;
				animal.Update(gameTime);
			}			
		}
		
		public function updateItem(obj:Object):void
		{
			if(obj.previewType == 1)
			{
			 	var petData:XML;
			 	animal = new GameElementPet(GameCommonData.GameInstance);
	            animal.Role = new GameRole();
	            animal.Role.Type = GameRole.TYPE_PET;
	            animal.Role.HP   = 100;
	        
	        	petData = GameCommonData.ModelOffsetEnemy[PlayerSkinsController.GetPetPersonSkinName(obj.previewModel,0)];
 
				if(petData!=null)
				{
					animal.Offset			   = new Point(petData.@X,petData.@Y);						// 时装偏移值
					animal.OffsetHeight		   = petData.@H;
					animal.Role.PersonSkinName = "Resources\\Enemy\\" + petData.@Swf + ".swf";
				}
				
				animal.Role.showSkinPoint = new Point(marketPreview.width/2,marketPreview.height - 50);
				
				if(obj.previewX != null)
				{
					animal.Role.showSkinPoint.x += obj.previewX;
				}
	     
				if(obj.previewY  != null)
				{
					animal.Role.showSkinPoint.y += obj.previewY ;
				}
	
		        animal.Role.isSkinTest = true;
		        animal.SetParentScene(GameCommonData.GameInstance.GameScene.GetGameScene);
		        animal.Initialize();
				marketPreview.addChild(animal);
			    GameCommonData.GameInstance.GameUI.Elements.Add(this);	
			    animal.mouseEnabled = false;
			    animal.mouseChildren = false;
			    animal.SetAction(GameElementSkins.ACTION_STATIC,3);
			    
			    animal.mask = mask;
			 }
			 else
			 {
			 	var playerData:XML;
			 	var playerskin:int = (obj.previewModel as String).split("_")[0];
			    var playersex:int = (obj.previewModel as String).split("_")[1];
			 	animal = new GameElementPlayer(GameCommonData.GameInstance);
	            animal.Role = new GameRole();
	            animal.Role.isSkinTest = true;
	            animal.Role.Type = GameRole.TYPE_PLAYER;
	            animal.Role.Sex = playersex;
	            animal.Role.HP   = 100;
			 	playerData = GameCommonData.ModelOffsetPlayer[obj.previewModel];	   
			    animal.Offset 		        = new Point(playerData.@X,playerData.@Y);					// 时装偏移值
			    animal.Role.showSkinPoint   = new Point(marketPreview.width/2,marketPreview.height - 40);
			    
			    if(obj.previewX != null)
				{
					animal.Role.showSkinPoint.x += obj.previewX;
				}
	     
				if(obj.previewY  != null)
				{
					animal.Role.showSkinPoint.y += obj.previewY ;
				}
			    
			    animal.OffsetHeight			= playerData.@H;
				animal.Role.PersonSkinID    = playerskin;
				animal.Role.WeaponEffectModel = GameCommonData.Player.Role.WeaponEffectModel;
				animal.Role.WeaponDiaphaneity = GameCommonData.Player.Role.WeaponDiaphaneity;
		    	var skinNameController:SkinNameController = PlayerSkinsController.GetSkinName(animal);
				animal.Role.PersonSkinName    = skinNameController.PersonSkinName;	
				animal.Role.WeaponSkinName    = skinNameController.WeaponSkinName;	
				animal.Role.WeaponEffectName  = skinNameController.WeaponEffectSkinName;
				animal.Role.WeaponEffectModelName = skinNameController.WeaponEffectModelName;
			    animal.Role.WeaponDiaphaneity = skinNameController.WeaponDiaphaneity;
			    
			    animal.Role.isSkinTest = true;
		        animal.SetParentScene(GameCommonData.GameInstance.GameScene.GetGameScene);
		        animal.Initialize();
				marketPreview.addChild(animal);
			    GameCommonData.GameInstance.GameUI.Elements.Add(this);	
			    animal.mouseEnabled = false;
			    animal.mouseChildren = false;
			    animal.SetAction(GameElementSkins.ACTION_STATIC,3);
			    
			    animal.mask = mask;
			 }
			 this.direction = 0;
		}
		
		
		public function set direction(direction:int):void
		{
			_direction = direction;
			var realDirection:int = MarketConstData.directions[direction];
			animal.SetAction(GameElementSkins.ACTION_STATIC,realDirection);
		}
		
		public function get direction():int
		{
			return _direction;
		}

		public function set current(current:int):void
		{
			show(current);
			_current = current;
			pageTxt.text = "" + (_current + 1) + "/" + modelNum;
		}
		
		public function get current():int
		{
			return _current;
		}
		
		public function removeView():void
		{
			if(marketPreview != null && animal != null && marketPreview.contains(animal))
			{
				marketPreview.removeChild(animal);
				animal.mask = null;
				animal.Dispose();
				animal = null;
			}
			GameCommonData.GameInstance.GameUI.Elements.Remove(this);	
		}
		
		private function onClick(event:MouseEvent):void
		{
			switch(event.currentTarget.name)
			{
				case "btn_marketPreview_left":
					this.direction = (this.direction + 7) % 8;
					break;
				case "btn_marketPreview_right":
					this.direction = (this.direction + 1) % 8;
					break;
				case "btn_marketPreview_pre":
					current = (current + modelNum - 1) % modelNum;
					break;
				case "btn_marketPreview_next":
					current = (current + 1) % modelNum;
					break;
			}
		}
		
		private function close(event:Event = null):void
		{
			removeView();
			MarketConstData.PreviewIsOpen = false;
			GameCommonData.GameInstance.GameUI.removeChild(this.panelBase);
			removeLis();
		}
		
		private function addLis():void
		{
			panelBase.addEventListener(Event.CLOSE, close);
			leftBtn.addEventListener(MouseEvent.CLICK,onClick);
			rightBtn.addEventListener(MouseEvent.CLICK,onClick);
			previousBtn.addEventListener(MouseEvent.CLICK,onClick);
			nextBtn.addEventListener(MouseEvent.CLICK,onClick);
		}
		
		private function removeLis():void
		{
			panelBase.removeEventListener(Event.CLOSE, close);
			leftBtn.removeEventListener(MouseEvent.CLICK,onClick);
			rightBtn.removeEventListener(MouseEvent.CLICK,onClick);
			previousBtn.removeEventListener(MouseEvent.CLICK,onClick);
			nextBtn.removeEventListener(MouseEvent.CLICK,onClick);
		}
		
		private function isShowPreNext(modelNum:int):Boolean
		{
			
			if(modelNum > 1)
			{
				pageTxt.visible = true;
				previousBtn.visible = true;
				nextBtn.visible = true;
			}
			else
			{
				pageTxt.visible = false;
				previousBtn.visible = false;
				nextBtn.visible = false;
			}
			return modelNum > 1;
		}
		
		public function get Enabled():Boolean{return true;}
		
		public function get UpdateOrder():int{	return 0;	}
		
		public function get EnabledChanged():Function{	return null;	}
		
		public function set EnabledChanged(value:Function):void	{	}
		
		public function get UpdateOrderChanged():Function{	return null;	}
		
		public function set UpdateOrderChanged(value:Function):void	{	}
	}
}