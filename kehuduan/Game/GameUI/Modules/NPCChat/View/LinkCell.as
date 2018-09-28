package GameUI.Modules.NPCChat.View
{
	import GameUI.ConstData.EventList;
	import GameUI.MouseCursor.SysCursor;
	import GameUI.UICore.UIFacade;
	import GameUI.View.Components.UISprite;
	
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.events.MouseEvent;
	import flash.events.TextEvent;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.utils.Dictionary;

	public class LinkCell extends UISprite
	{
		protected var iconUrl:String;
		protected var linkTf:TextField;
		protected var data:Object={};
		private var linkStr:String;
		private var showStr:String;
		private var icon:BitmapData
		private var linkColor:uint;
		private var dic:Dictionary;
		private var colorDic:Dictionary;
		private var arrDes:Array;
		public var onLinkClick:Function;
		
		
		public function LinkCell(iconUrl:String,linkStr:String,showStr:String,linkColor:uint=0)
		{
			super();
			
			this.colorDic=new Dictionary();
			this.colorDic[0]=0xffffff;
			this.colorDic[1]=0xff0000;
			this.colorDic[2]=0x00ff00;
			this.colorDic[3]=0x00ffff;
			this.colorDic[4]=0xffff00;
			
			
				
			this.dic=new Dictionary();
			this.dic[1]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_1" ];              //"传送苍山--加入点苍"
			this.dic[2]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_2" ];     //"传送终南山--拜入全真"
			this.dic[3]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_3" ];    //"传送峨眉山--拜入峨眉"
			this.dic[4]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_4" ];   //"传送唐家堡--加入唐门"
			this.dic[5]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_5" ];    //"传送君山--加入丐帮"
			this.dic[6]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_6" ];    //"传送少室山--拜入少林"
			this.dic[7]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_7" ];   //"入职任务--初习武功"
			
			this.dic[8]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_8" ];    //"拜入点苍，成为点苍弟子"
			this.dic[9]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_9" ];    //"拜入全真，成为全真弟子"
			this.dic[10]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_10" ];    //"拜入峨眉，成为峨眉弟子"
			this.dic[11]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_11" ];    //"拜入唐门，成为唐门弟子"
			this.dic[12]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_12" ];       //"拜入丐帮，成为丐帮弟子"
			this.dic[13]=GameCommonData.wordDic[ "mod_npcc_view_lc_lc_13" ];             //"拜入少林，成为少林弟子"
			
			arrDes=[
			{},
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_1" ],area:GameCommonData.wordDic[ "often_used_dc" ]},               //"柳沧樱"      "点苍"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_3" ],area:GameCommonData. wordDic[ "often_used_qz" ]},              //"王重阳"       "全真"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_5" ],area:GameCommonData.wordDic[ "often_used_em" ]},                 //"鸿陵"        "峨眉"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_6" ],area:GameCommonData.wordDic[ "often_used_tm" ]},                //"唐妍"         "唐门"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_2" ],area:GameCommonData.wordDic[ "often_used_gb" ]},              //"金不换"        "丐帮"
			{name:GameCommonData.wordDic[ "mod_npcc_cmd_ttu_4" ],area:GameCommonData.wordDic[ "often_used_sl" ]}                 //"玄慈"           "玄慈"
			];
			
			this.iconUrl=iconUrl;
			this.linkStr=linkStr;
			this.showStr=showStr;
			this.linkColor=colorDic[linkColor];
			this.createChildren();
		}
		
		protected function createChildren():void{
			this.icon=GameCommonData.GameInstance.Content.Load(GameConfigData.UILibrary).GetClassByBitmapData(this.iconUrl);
			var bitMap:Bitmap=new Bitmap(this.icon);
			this.addChild(bitMap);
			bitMap.x=20;
			this.linkTf=new TextField();
			this.linkTf.autoSize=TextFieldAutoSize.CENTER;
			this.linkTf.selectable=false;
			this.linkTf.textColor=this.linkColor;
			var str:String='<a href="event:'+this.linkStr+'"><u><font size="12" face="宋体">'+this.showStr+'</font></u></a>';
			this.linkTf.htmlText=str;
			this.addChild(this.linkTf);
			this.linkTf.x=50;
			this.linkTf.addEventListener(MouseEvent.ROLL_OVER,onMouseRollOver); 
			this.linkTf.addEventListener(MouseEvent.ROLL_OUT,onMouseRollOut); 
			this.linkTf.addEventListener(TextEvent.LINK,onClickLink);
		}
		
		protected function onMouseRollOver(e:MouseEvent):void{
			SysCursor.GetInstance().showSystemCursor();
		}
		
		protected function onMouseRollOut(e:MouseEvent):void{
			SysCursor.GetInstance().revert();
		}
		
		protected function onClickLink(e:TextEvent):void{
			if(isInDictionary(this.showStr)!=-1){
				var index:int=isInDictionary(this.showStr);
				if(index==7){
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(EventList.DO_FIRST_TIP, {comfrim:onLinkClick,cancel:null, info:"<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_1" ]+"<font color='#00ff00'>12"+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_2" ]+"</font><font color='#ffff00'>15000"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_3" ]+"</font><br><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_4" ]+"</font>",title:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_5" ], params:e.text,width:245,comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_6" ]});
																																									//"你即将升到"                                                                    "级"                                                            "，并获得"                                                                            "经验值。"                                                                                 "小提示：与武功传授人对话，学习门派武功。"                               "小提示"                                                                                "我知道了"
				}else if(index>=8 && index<=13){ 
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(EventList.DO_FIRST_TIP, {comfrim:onLinkClick,cancel:null, info:"<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_1" ]+"<font color='#00ff00'>11"+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_7" ]+arrDes[index-7].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_8" ]+"</font><font color='#ffff00'>"+arrDes[index-7].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_9" ]+"</font><font color='#ffffff'>。</font><br><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_10" ]+arrDes[index-7].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_11" ]+"</font>",title:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_12" ]+arrDes[index-7].area,params:e.text, width:320,comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_6" ]});
																																									//"你即将升到"                                                                   "级"                                                       "，加入"                                                                  "后，掌门为你穿上了"                                                                                       "服饰"                                                                                                                "小提示：下面请跟随指引学习"                                                  "武功。"                                                             "加入"                                                                                                      "我知道了"
																																											
				}else if(index>=1 && index<=6){	
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(EventList.DO_FIRST_TIP, {comfrim:onLinkClick,cancel:new Function(), info:"<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_13" ]+"</font><font color='#e46d0a'>"+arrDes[index].area+"</font><font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_14" ]+"</font><br><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_15" ]+arrDes[index].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_16" ]+"</font><font color='#0099ff'>"+arrDes[index].area+"掌门"+arrDes[index].name+"</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_17" ]+arrDes[index].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_18" ]+"</font>", title:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_19" ], comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_20" ],params:e.text });
																																												//"你是否决定前去"		                                                                           	                                       "拜师？"			                                                                        "小提示：传送至"                   	                                   "后找"												       "掌门"								                                                                 "拜师就可以成为"				                                          "弟子。"		                                                                           "加入门派"	                                   "传送"	
				}
			}else{
				this.onLinkClick(e.text);
			}
		}
		
		public function onClickBtn():void{
			if(isInDictionary(this.showStr)!=-1){
				var index:int=isInDictionary(this.showStr);
				if(index==7){
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(EventList.DO_FIRST_TIP, {comfrim:onLinkClick,cancel:null, info:"<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_1" ]+"<font color='#00ff00'>12"+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_2" ]+"</font><font color='#ffff00'>15000"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_3" ]+"</font><br><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_4" ]+"</font>",title:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_5" ], params:this.linkStr,width:245,comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_6" ]});
																																									//"你即将升到"                                                                    "级"                                                            "，并获得"                                                                            "经验值。"                                                                                 "小提示：与武功传授人对话，学习门派武功。"                               "小提示"                                                                                "我知道了"
				}else if(index>=8 && index<=13){ 
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(EventList.DO_FIRST_TIP, {comfrim:onLinkClick,cancel:null, info:"<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_1" ]+"<font color='#00ff00'>11"+GameCommonData.wordDic[ "mod_rp_med_ui_pa_spd_1" ]+"</font>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_7" ]+arrDes[index-7].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_8" ]+"</font><font color='#ffff00'>"+arrDes[index-7].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_9" ]+"</font><font color='#ffffff'>。</font><br><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_10" ]+arrDes[index-7].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_11" ]+"</font>",title:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_12" ]+arrDes[index-7].area,params:this.linkStr, width:320,comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_6" ]});
																																									//"你即将升到"                                                                   "级"                                                       "，加入"                                                                  "后，掌门为你穿上了"                                                                                       "服饰"                                                                                                                "小提示：下面请跟随指引学习"                                                  "武功。"                                                             "加入"                                                                                                      "我知道了"
																																											
				}else if(index>=1 && index<=6){	
					UIFacade.GetInstance(UIFacade.FACADEKEY).sendNotification(EventList.DO_FIRST_TIP, {comfrim:onLinkClick,cancel:new Function(), info:"<font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_13" ]+"</font><font color='#e46d0a'>"+arrDes[index].area+"</font><font color='#ffffff'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_14" ]+"</font><br><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_15" ]+arrDes[index].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_16" ]+"</font><font color='#0099ff'>"+arrDes[index].area+"掌门"+arrDes[index].name+"</font><font color='#00ff00'>"+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_17" ]+arrDes[index].area+GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_18" ]+"</font>", title:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_19" ], comfirmTxt:GameCommonData.wordDic[ "mod_npcc_view_lc_ocl_20" ],params:this.linkStr });
																																												//"你是否决定前去"		                                                                           	                                       "拜师？"			                                                                        "小提示：传送至"                   	                                   "后找"												       "掌门"								                                                                 "拜师就可以成为"				                                          "弟子。"		                                                                           "加入门派"	                                   "传送"	
				}
			}else{
				this.onLinkClick( this.linkStr ); 
			}
		}
		
		/**
		 * 查看是哪个门派和特殊选项
		 * @param str ：字符
		 * @return  ：选项ID
		 * 
		 */		
		protected function isInDictionary(str:String):int{
			for (var id:* in this.dic){
				if(this.dic[id]==str)return id;
			}
			return -1;
		}
	}
}