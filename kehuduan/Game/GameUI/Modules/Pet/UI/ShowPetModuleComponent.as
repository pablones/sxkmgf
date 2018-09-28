package GameUI.Modules.Pet.UI
{
	import Controller.PlayerSkinsController;
	
	import GameUI.Modules.Maket.Data.MarketConstData;
	import GameUI.Modules.Pet.Data.PetPropConstData;
	
	import OopsEngine.Role.GameRole;
	import OopsEngine.Scene.StrategyElement.GameElementAnimal;
	import OopsEngine.Scene.StrategyElement.GameElementSkins;
	import OopsEngine.Scene.StrategyElement.Person.GameElementPet;
	
	import OopsFramework.GameTime;
	import OopsFramework.IUpdateable;
	
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.geom.Point;

	public class ShowPetModuleComponent implements IUpdateable
	{
		private var _backView:Sprite;
		private var animal:GameElementAnimal;
		private var currentDirection:int;
		private var _mask:Shape;
		private var moduleInfo:Object;
		public function ShowPetModuleComponent(backView:Sprite)
		{
			_backView = backView;
			initMask();
		}
		
		private function initMask():void
		{
			_mask = new Shape();
			_mask.graphics.beginFill(0);
			_mask.graphics.drawRect(0,0,226,160);
			_mask.graphics.endFill();
			_backView.parent.addChildAt(_mask,_backView.parent.getChildIndex(_backView)+1);
			_mask.x = 0;
			_mask.y = 2;
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
		
		public function turnRight():void
		{
			animal.SetAction(GameElementSkins.ACTION_STATIC,changeDirection(1));
		}
		
		public function turnLeft():void
		{
			animal.SetAction(GameElementSkins.ACTION_STATIC,changeDirection(0));
		}
		
		private function changeDirection(tag:int):int
		{
			if(tag == 1)	//右转
			{
				currentDirection = (currentDirection + 1) % 8;
			}
			else if(tag == 0)	//左转
			{
				currentDirection = (currentDirection + 7) % 8;
			}
			return MarketConstData.directions[currentDirection];
		}
		
		public function showView(petClass:String):void
		{   
		    var petData:XML;
			animal = new GameElementPet(GameCommonData.GameInstance);
            animal.Role = new GameRole();
            animal.Role.Type = GameRole.TYPE_PET;
            animal.Role.HP   = 100;
        
        	petData = GameCommonData.ModelOffsetEnemy[PlayerSkinsController.GetPetPersonSkinName(petClass,0)];
 
			if(petData!=null)
			{
				animal.Offset			   = new Point(petData.@X,petData.@Y);						// 时装偏移值
				animal.OffsetHeight		   = petData.@H;
				animal.Role.PersonSkinName = "Resources\\Enemy\\" + petData.@Swf + ".swf";
			} 
			
			_backView.parent.addChildAt(animal,2);
	        animal.Role.isSkinTest = true;
	        animal.SetParentScene(GameCommonData.GameInstance.GameScene.GetGameScene);
			animal.Role.showSkinPoint = setPosition(petClass);
	        animal.Initialize();
		    animal.mouseEnabled = false;
		    animal.mouseChildren = false;
			GameCommonData.GameInstance.GameUI.Elements.Add(this);	
			animal.SetAction(GameElementSkins.ACTION_STATIC,2);
			animal.mask = _mask;  
			currentDirection = 0;
		} 
 		private function setPosition(petClass:String):Point
		{
			var _w:int;
			var _h:int;
			var _scaleX:Number;
			var _scaleY:Number;
			moduleInfo = PetPropConstData.newPetModuleInfo[petClass];
			if(moduleInfo)
			{
				_w = 113 + moduleInfo.movieW;
				_h = 120 + moduleInfo.movieH;
				_scaleX = moduleInfo.scalX;
				_scaleY = moduleInfo.scalY;
			}
			else
			{
				_w = 113;
				_h = 120;
				_scaleX = 1;
				_scaleY = 1;
			}
			animal.scaleX = _scaleX;
			animal.scaleY = _scaleY;
			var _X:int = int(_w - animal.width/2);
			var _Y:int = int(_h - animal.height/2);
			return new Point(_X,_Y); 
		}
		
		
		public function deleteView():void
		{
			if(animal)
			{
				animal.parent.removeChild(animal);
				animal.Dispose();
				animal.mask = null;
				animal = null;
				GameCommonData.GameInstance.GameUI.Elements.Remove(this);	
			}
			moduleInfo = null;
		}
		
		public function get Enabled():Boolean{return true;}
		
		public function get UpdateOrder():int{	return 0;	}
		
		public function get EnabledChanged():Function{	return null;	}
		
		public function set EnabledChanged(value:Function):void	{	}
		
		public function get UpdateOrderChanged():Function{	return null;	}
		
		public function set UpdateOrderChanged(value:Function):void	{	}
		
	}
}