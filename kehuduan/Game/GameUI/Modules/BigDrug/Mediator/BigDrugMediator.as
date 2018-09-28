package GameUI.Modules.BigDrug.Mediator
{
	import GameUI.Modules.BigDrug.Data.BigDrugData;
	import GameUI.Modules.BigDrug.Data.BigDrugEvent;
	import GameUI.View.items.FaceItem;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.geom.Point;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.mediator.Mediator;
	
	/**
	 * 大红大蓝Mediator
	 * @author:Ginoo
	 * @version:1.0
	 * @langVersion:3.0
	 * @playerVersion:10.0
	 * @date:8/27/2010
	 */ 
	public class BigDrugMediator extends Mediator
	{
		public static const NAME:String = "BigDrugMediator";
		
		private const POINT:Point = new Point(210 , 25);
		
		private var dis:int = 5;		//间距
		private var content:Sprite = new Sprite();
		private var drugMctArr:Array = [];
		private var ishas:Array = [];
		
		public function BigDrugMediator()
		{
			super(NAME);
		}
		
		public override function listNotificationInterests():Array
		{
			return [
					BigDrugEvent.BIG_DRUG_UPDATE
					]
		}
		
		public override function handleNotification(notification:INotification):void
		{
			switch(notification.getName())
			{
				case BigDrugEvent.BIG_DRUG_UPDATE:
					BigDrugData.drugList = notification.getBody() as Array;
					init();
					show(BigDrugData.drugList);
				break;
			}
		}

		private function gcAll():void
		{
			
		}
		
		private function init():void
		{
			GameCommonData.GameInstance.GameUI.addChild(content);
		}
		private function show(list:Array):void
		{
			for(var i:int = 0;i < list.length; i++)
			{
				var drugmc:MovieClip = GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByMovieClip("BuffGrid") as MovieClip;
				drugmc.width  = 16 ;
				drugmc.height = 16 ;
				drugmc.name = BigDrugData.drugName[i];
				drugMctArr[i] = drugmc;
				if(list[i].data == 0)		                //删除
				{
					deletedrug(list[i] , i );
				}
				if(list[i].type == 0)			            //没有
				{
					
				}
				if(list[i].data != 0 && list[i].type != 0)	//添加
				{
					addDrug(list[i] , i); 	
				}
			}
		}
		
		/** 添加 */
		private function addDrug(druhgObj:Object , index:int):void
		{
//			trace("(drugMctArr[index] as MovieClip).getChildAt(0)",(drugMctArr[index] as MovieClip).getChildAt(0).name)
			if(this.ishas[index] == true)		//有图标就不用再加载图标
			{
			}
			else								//没有图标
			{
				var blrP:FaceItem = new FaceItem(BigDrugData.typeId[index],null,"icon",32/16);
				drugMctArr[index].addChildAt(blrP , 0);
				(drugMctArr[index] as MovieClip).removeChildAt(1);
				var have:Boolean = true;
				this.ishas[index] = have;
				content.addChild(drugMctArr[index]);
				drugMctArr[index].x = this.POINT.x + (content.numChildren - 1) * (this.dis + 16);
				drugMctArr[index].y = this.POINT.y;
			}
			
			if(druhgObj.type == 1)				//大瓶
			{
				// 储量   ，最多每6秒补充
				(BigDrugData.drugDataList[index] as Object).name = (BigDrugData.drugDataList[index] as Object).big +GameCommonData.wordDic[ "mod_big_med_big_add_1" ]+ druhgObj.data+GameCommonData.wordDic[ "mod_big_med_big_add_2" ]+(BigDrugData.drugDataList[index] as Object).numsmall;
			}
			else if(druhgObj.type == 2)			//小瓶
			{
				// 储量   ，最多每6秒补充
				(BigDrugData.drugDataList[index] as Object).name = (BigDrugData.drugDataList[index] as Object).small +GameCommonData.wordDic[ "mod_big_med_big_add_1" ]+ druhgObj.data + GameCommonData.wordDic[ "mod_big_med_big_add_2" ]+(BigDrugData.drugDataList[index] as Object).numbig;
			}
		}
		
		/** 删除 */
		private function deletedrug(drugObj:Object ,index:int):void
		{
			if(this.ishas[index] == true )
			{
				var mc:MovieClip = content.getChildByName(BigDrugData.drugName[index]) as MovieClip;
				content.removeChild(mc);
				this.ishas[index] = false;
				updataRank();
			}
		}
		/** 刷新排序 */
		private function updataRank():void
		{
			for(var i:int = 0;i < content.numChildren ; i++)
			{
				content.getChildAt(i).x = this.POINT.x + i * (this.dis + 16);
				content.getChildAt(i).y = this.POINT.y ;
			}
		}
	}
}