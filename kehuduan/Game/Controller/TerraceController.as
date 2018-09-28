/*****
 * 平台控制类
 */
package Controller
{
	import GameUI.Modules.Chat.Data.ChatData;
	import GameUI.Modules.GmTools.Data.GmToolsEvent;
	import GameUI.Modules.PrepaidLevel.Data.PrepaidUIData;
	import GameUI.View.ResourcesFactory;
	
	import flash.display.MovieClip;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	
	import org.puremvc.as3.multicore.interfaces.INotification;
	import org.puremvc.as3.multicore.patterns.command.SimpleCommand;

	public class TerraceController extends SimpleCommand
	{
		public static const NAME:String = "TerraceController";
		private var fieldMc:MovieClip;
		private var body:String;
//		private var isFirst:Boolean = true;
		public function TerraceController()
		{
			super();
		}
		public override function execute(notification:INotification):void
		{
			body = notification.getBody() as String;
			if(!fieldMc)
			{
				ResourcesFactory.getInstance().getResource(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/field.swf",onComplete);
			}
			else{
				dealMethod();
			}
			
			/* 
			 switch(notification.getBody())
			{
				case "pay":			//平台充值接口
					switch(ChatData.SERVICE_BUSINESS_ID)
					{
						case 0:		//4399	
							navigateToURL( new URLRequest(ChatData.DEPOSIT_WEBSITE_ADDR+"?gamename=yjjh&gameserver="+GameCommonData.ServerId+"&username="+GameCommonData.Accmoute ), "_blank" );
						break;
						case 1:		//91玩	
							navigateToURL( new URLRequest( ChatData.DEPOSIT_WEBSITE_ADDR + "&user_name="+GameCommonData.Accmoute ), "_blank" );
						break;
						case 2:		//37玩
//						 	navigateToURL(new URLRequest(ChatData.DEPOSIT_WEBSITE_ADDR+"?gamename=yjjh&gameserver="+GameCommonData.ServerId+"&username="+GameCommonData.Accmoute), "_blank");
						 	navigateToURL(new URLRequest(ChatData.DEPOSIT_WEBSITE_ADDR+"?gamename=yjjh&gameserver=S"+GameCommonData.ServerId+"&username="+GameCommonData.Accmoute), "_blank");
						 	
						 	///////////////////////////////////////////////小黑板输出
//						 	Debug.ShowHint.display( GameCommonData.GameInstance.GameUI );
//						 	ShowHint.show( ChatData.DEPOSIT_WEBSITE_ADDR+"?gamename=yjjh&gameserver=S"+GameCommonData.ServerId+"&username="+GameCommonData.Accmoute );
						 	//////////////////////////////////////////////
						break;
						case 3:		//5awan    目前还不能用
//							navigateToURL(new URLRequest());
							navigateToURL(new URLRequest(ChatData.DEPOSIT_WEBSITE_ADDR + GameCommonData.ServerId), "_blank");
						break;
						case 4:		//快玩
							var str:String = GameCommonData.ServerId;
							if(str)
							{
								if(str.substr(0,1).toLowerCase() == "s")
								{
									str = str.substr(1);
								}
							}
							navigateToURL(new URLRequest(ChatData.DEPOSIT_WEBSITE_ADDR + str), "_blank");
						break; 
						case 5:   //360
							var url360:String = ChatData.DEPOSIT_WEBSITE_ADDR+"?qid="+GameCommonData.Accmoute+"&server_id=S"+GameCommonData.ServerId;
							navigateToURL( new URLRequest(ChatData.DEPOSIT_WEBSITE_ADDR+"?qid="+GameCommonData.Accmoute+"&server_id="+GameCommonData.ServerId ), "_blank" );
						break;
						case 6:		//奥盛
							navigateToURL( new URLRequest( ChatData.DEPOSIT_WEBSITE_ADDR ) );
						break;
						case 7:		//酷我
							navigateToURL( new URLRequest( ChatData.DEPOSIT_WEBSITE_ADDR), "_blank" );
						break;
						default:
							
						break;
					}
				break;
				case "GMTools":		//平台GM在线BUG提交
					if(ChatData.SERVICE_BUSINESS_ID == 0)
					{
						facade.sendNotification(GmToolsEvent.LOADGMTOOLSVIEW);
					}
					else if(ChatData.SERVICE_BUSINESS_ID > 0)
					{
						navigateToURL(new URLRequest(ChatData.GM_INTERFACE_ADDR), "_blank");
					}
				
					
//					switch(ChatData.SERVICE_BUSINESS_ID) 
//					{
//						case 0:
//							facade.sendNotification(GmToolsEvent.LOADGMTOOLSVIEW);
//						break;
//						case 1:
//							navigateToURL(new URLRequest(ChatData.GM_INTERFACE_ADDR), "_blank");
//						break;
//						case 2:
//							navigateToURL(new URLRequest(ChatData.GM_INTERFACE_ADDR), "_blank");
//						break;
//						case 3:
//							navigateToURL(new URLRequest(ChatData.GM_INTERFACE_ADDR), "_blank");
//						break;
//						case 4:
//							navigateToURL(new URLRequest(ChatData.GM_INTERFACE_ADDR), "_blank");
//						break;
//					}
					
				break;
				case "intoOffical":		//平台官网
					navigateToURL(new URLRequest(ChatData.OFFICIAL_WEBSITE_ADDR), "_blank");
				break;
				case "intoFurom":		//进入论坛
					navigateToURL(new URLRequest(ChatData.FORUM_WEBSITE_ADDR), "_blank");
				break;
			}  */
		}
		private function onComplete():void
		{
			fieldMc = ResourcesFactory.getInstance().getMovieClip(GameCommonData.GameInstance.Content.RootDirectory + "Resources/GameDLC/field.swf");
			fieldMc.SERVICE_BUSINESS_ID = ChatData.SERVICE_BUSINESS_ID;//运营商编号
			fieldMc.DEPOSIT_WEBSITE_ADDR = ChatData.DEPOSIT_WEBSITE_ADDR;//充值接口
			fieldMc.ServerId = GameCommonData.ServerId ;//服务器名称  s1  s2
			fieldMc.LOADGMTOOLSVIEW = GmToolsEvent.LOADGMTOOLSVIEW;//3499GM
			fieldMc.GM_INTERFACE_ADDR = ChatData.GM_INTERFACE_ADDR;//其他平台GM
			fieldMc.Accmoute = GameCommonData.Accmoute;//玩家账号
			fieldMc.makePath();
			dealMethod();
			GameCommonData.payPath = fieldMc.payPath;
		}
		
		private function dealMethod():void
		{
			if(body == "pay") //充值
			{
				/* if(payPath.substr(payPath.length - 4) == "null")
				{
					payPath = payPath.substr(0,payPath.length - 4)+1;
				} */
				
				/* if(!fieldMc.str)
				{
					fieldMc.str = "1";
				}
				else if(fieldMc.str)
				{
					if(fieldMc.str.substr(0,1).toLowerCase() == "s")
					{
						fieldMc.str = fieldMc.str.substr(1);
					}
				} */
				if( PrepaidUIData.isFirst == true )
				{
					PrepaidUIData.isFirst = false;
				} 
				else
				{
					sendNotification( PrepaidUIData.SHOW_LOADINGVIEW );
				}
				
//				navigateToURL(new URLRequest(payPath), "_blank");
			}
			else if(body == "GMTools")		//GM工具
			{
				if(ChatData.SERVICE_BUSINESS_ID == 0)	//4399
				{
					facade.sendNotification(GmToolsEvent.LOADGMTOOLSVIEW);
					return;
				}
				var gmPath:String = fieldMc.gmPath;
				navigateToURL(new URLRequest(gmPath), "_blank");
			}
			else if(body == "intoOffical")	//平台官网
			{
				navigateToURL(new URLRequest(ChatData.OFFICIAL_WEBSITE_ADDR), "_blank");
			}
			else if(body == "intoFurom")	//进入论坛
			{
				navigateToURL(new URLRequest(ChatData.FORUM_WEBSITE_ADDR), "_blank");
			}
			
		}
		
	}
}