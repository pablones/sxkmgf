package GameUI.Modules.NewPlayerSuccessAward.Data
{
	import flash.display.MovieClip;
	
	public class NewAwardData
	{
		public static var newPlayAwardIsOpen:Boolean = false;		//新手成就面板是否打开
		public static var newPlayAwardBtnIsShow:Boolean = false;	//新手成就按钮是否显示
		public static var CoordinatesEquipList:Array = [			//套装数据
														[126921,136921,156921,166921,186921,196921]
														,[122921 ,132921 ,152921 ,162921 ,182921 ,192921]
														,[121921,131921,151921,161921,181921,191921]
														,[123921,133921,153921,163921,183921,193921]
														,[125921,135921,155921,165921,185921,195921]
														,[124921,134921,154921,164921,184921,194921]
														]
		public static var awardItemList:Array = [					//奖品的TYPEID
													[630006 , 502038]	
													,[610024 , 501000 , 630000]
													,[630014 , 610013 , 610021]
													,[650001 , 610024]
													,[630014 , 610030 , 301001]
													,[610030 , 610041]
													,[630014 , 501000 , 610042]
													,[610014 , 610030]
													,[321001 , 175]
													,[610014 , 610017]				
													];

	}
}