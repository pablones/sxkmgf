package GameUI.Modules.Build.mediator
{
	import GameUI.ConstData.EventList;
	import GameUI.ConstData.UIConstData;
	import GameUI.Modules.Build.command.BuildCommonList;
	import GameUI.Proxy.DataProxy;
	import GameUI.UIConfigData;
	import GameUI.View.BaseUI.PanelBase;
	
	import flash.display.MovieClip;
	import flash.events.Event;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;

	public class BuildMediator extends Mediator
	{
		public static const NAME:String="BuildMediator";
		
		protected var basePanel:PanelBase;
		protected var dataProxy:DataProxy;
		public function BuildMediator(mediatorName:String=null, viewComponent:Object=null)
		{
			super(NAME, viewComponent);
		}
		
		public function get viewUI():MovieClip{
			return this.viewComponent as MovieClip;
		}
		
		public override function listNotificationInterests():Array{
			return [
				EventList.ENTERMAPCOMPLETE,
				BuildCommonList.CLOSE_BUILD_UI,
				BuildCommonList.SHOW_BUILD_UI
			];
		}
		
		public override function handleNotification(notification:INotification):void{
			switch (notification.getName()){
				case EventList.ENTERMAPCOMPLETE:
					sendNotification(EventList.GETRESOURCE,{type:UIConfigData.MOVIECLIP,mediator:this,name:"buildTotalPanel"});
					this.basePanel=new PanelBase(this.viewUI,this.viewUI.width-19,this.viewUI.height+10);
					this.basePanel.SetTitleTxt("修 炼");
					this.viewUI.x=-16;
					this.basePanel.addEventListener(Event.CLOSE,onBasePanelClose);
					this.dataProxy=facade.retrieveProxy(DataProxy.NAME) as DataProxy;
					break;
				case BuildCommonList.SHOW_BUILD_UI:
					this.dataProxy.buildPanelIsOpen=true;
					GameCommonData.GameInstance.GameUI.addChild(this.basePanel);
					this.basePanel.x=UIConstData.DefaultPos1.x;
					this.basePanel.y=UIConstData.DefaultPos1.y;
					break;
				case BuildCommonList.CLOSE_BUILD_UI:
					if(this.dataProxy.buildPanelIsOpen){
						this.onBasePanelClose(null);
					}
					break;
			}
		}
		
		
		protected function onBasePanelClose(e:Event):void{
			this.dataProxy.buildPanelIsOpen=false;
			GameCommonData.GameInstance.GameUI.removeChild(this.basePanel);
			
			
			
		}

	}
}