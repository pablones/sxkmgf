package GameUI
{
	public class XmlUtils
	{
		public static function createXml():void
		{
			CreateModelOffsetPlayer();
			CreateModelOffsetEnemy();
			CreateModelOffsetNPC();
			CreateModelOffsetMount();
			CreateMapTree();
		}
		/** 创建人物模型偏移值 武器特效数据 */
		public static function CreateModelOffsetPlayer():void
		{
			var xmlData:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.ModelOffset_XML_SWF ).GetDisplayObject()["modelOffsetPlayer_xml"];
			var object:XML;
			//人物模型偏移值
			for each(object in xmlData.P)
	        {
	        	GameCommonData.ModelOffsetPlayer[object.@Id.toString()] = object;
	        }
	        //武器特效数据
	        for each(object in xmlData.W)
	        {
	        	GameCommonData.WeaponEffect[object.@Id.toString()] = object;
	        }
	        
	        //宠物数据
	        for each(object in xmlData.Pet)
	        {
	        	GameCommonData.ModelPet[object.@Id.toString()] = object;
	        }
	        
	       //宠物变异数据
	        for each(object in xmlData.VPet)
	        {
	        	GameCommonData.VariationPet.push( object );
	        }
	        
		}
		
		/** 创建怪物或NPC模型偏移值  */
		public static function CreateModelOffsetEnemy():void
		{
			var xmlData:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.ModelOffset_XML_SWF ).GetDisplayObject()["modelOffsetEnemy_xml"];
			for each(var object:XML in xmlData.E)
	        {
	        	GameCommonData.ModelOffsetEnemy[object.@Id.toString()] = object;
	        }
		}
		
		/** 创建怪物或NPC模型偏移值  */
		public static function CreateModelOffsetNPC():void
		{
			var xmlData:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.ModelOffset_XML_SWF ).GetDisplayObject()["modelOffsetNPC_xml"];
			for each(var object:XML in xmlData.E)
	        {
	        	GameCommonData.ModelOffsetNPC[object.@Id.toString()] = object;
	        }
		}
		
		/** 创建坐骑模型偏移值  */
		public static function CreateModelOffsetMount():void
		{
			var xmlData:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.ModelOffset_XML_SWF ).GetDisplayObject()["modelOffsetMount_xml"];
			for each(var object:XML in xmlData.M)
	        {
	        	GameCommonData.ModelOffsetMount[object.@Id.toString()] = object;
	        }
		}
		
		/** 创建地图关系表  */
		public static function CreateMapTree():void
		{
			var map:XML = GameCommonData.GameInstance.Content.Load( GameConfigData.Other_XML_SWF ).GetDisplayObject()["mapTree_xml"];
			for each(var object:XML in map.Map)
	        {
	        	GameCommonData.MapTree[object.@Id.toString()] = object.@NodeList.toString();
	        }
		}

	}
}