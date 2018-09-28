package GameUI.Modules.CompensateStorage.view
{
	import GameUI.Modules.CompensateStorage.data.CompensateStorageData;
	import GameUI.View.Components.UIScrollPane;
	import GameUI.View.ShowMoney;

	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.events.Event;

	public class CompensateStorageOtherViewManager implements CSViewManager
	{
		private var _parent:DisplayObjectContainer;
		private var otherView:MovieClip;

		/**
		 * 滚动面板
		 */
		private var scrollPanel:UIScrollPane;
		/** 补偿详细描述*/
		protected var otherText:DetailsText;

		public function CompensateStorageOtherViewManager(parent:DisplayObjectContainer)
		{
			_parent=parent;
		}

		public function init():void
		{
			if (CompensateStorageData.domain.hasDefinition("CompensateStorageOther"))
			{
				var CompensateStorageItem:Class=CompensateStorageData.domain.getDefinition("CompensateStorageOther") as Class;
				otherView=new CompensateStorageItem();
			}

			otherText=new DetailsText(CompensateStorageData.otherSiteList[0]);
			scrollPanel=new UIScrollPane(otherText);
			scrollPanel.x=CompensateStorageData.otherSiteList[1];
			scrollPanel.y=CompensateStorageData.otherSiteList[2];
			scrollPanel.width=CompensateStorageData.otherSiteList[3];
			scrollPanel.height=CompensateStorageData.otherSiteList[4];
			scrollPanel.scrollPolicy=UIScrollPane.SCROLLBAR_AS_NEEDED;
			scrollPanel.mouseEnabled=false;
			scrollPanel.refresh();
			otherView.addChild(scrollPanel);

			CompensateStorageData.isInitOtherView=true;
		}

		public function show(object:Object=null):void
		{
			if (!CompensateStorageData.isInitOtherView)
			{
				init();
			}
			if (!CompensateStorageData.isShowOtherView)
			{
				_parent.addChildAt(otherView, 0);
				CompensateStorageData.isShowOtherView=true;
			}

			update(getShowString());

		}

		private function getShowString():String
		{
			var str:String="<font color='#E2CCA5'>" + CompensateStorageData.word1 + "：<br><br>";
			if (CompensateStorageData.zhuBao)
			{
				str+=CompensateStorageData.word5 + " " + CompensateStorageData.zhuBao + CompensateStorageData.tuBiaoDic["zhuBao"] + "<br>";
			}
			if (CompensateStorageData.suiYin)
			{
				str+=CompensateStorageData.word7 + " ";
				str+=CompensateStorageData.getSuiYin(CompensateStorageData.suiYin);
				str+="<br>"
			}
			if (CompensateStorageData.yinLiang)
			{
				str+=CompensateStorageData.word8 + " ";
				str+=CompensateStorageData.getYinLiang(CompensateStorageData.yinLiang);
				str+="<br>"
			}
			if (CompensateStorageData.yuanBao)
			{
				str+=CompensateStorageData.word6 + " " + CompensateStorageData.yuanBao + CompensateStorageData.tuBiaoDic["yuanBao"] + "<br>";
			}
			if (CompensateStorageData.bangGong)
			{
				str+=CompensateStorageData.word2 + " " + CompensateStorageData.bangGong + "<br>";
			}
			if (CompensateStorageData.rongYu)
			{
				str+=CompensateStorageData.word3 + " " + CompensateStorageData.rongYu + "<br>";
			}
			if (CompensateStorageData.exp)
			{
				str+=CompensateStorageData.word4 + " " + CompensateStorageData.exp + "<br>";
			}
			
			if (!(CompensateStorageData.bangGong || CompensateStorageData.rongYu || CompensateStorageData.exp || CompensateStorageData.zhuBao || CompensateStorageData.yuanBao || CompensateStorageData.suiYin || CompensateStorageData.yinLiang))
			{
				str ="<font color='#E2CCA5'>" + CompensateStorageData.word1 + "："+ CompensateStorageData.word9 +"<br>";
			}
			str+="</font>";
			return str;
		}

		/**
		 * 更新显示数据
		 */
		private function update(str:String):void
		{
			otherText.tfText=str;
			ShowMoney.ShowIcon(otherText, otherText.textField);
			scrollPanel.refresh();
		}

		public function close(event:Event=null):void
		{
			if (CompensateStorageData.isShowOtherView)
			{
				_parent.removeChild(otherView);
				CompensateStorageData.isShowOtherView=false;
			}
		}
	}
}
