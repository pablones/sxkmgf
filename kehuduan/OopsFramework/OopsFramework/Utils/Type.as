package OopsFramework.Utils
{
	import flash.utils.getDefinitionByName;
	
	public class Type
	{
		/** 获取一个对象的类名 */
		public static function GetObjectClassName(object:*):String
        {
            return flash.utils.getQualifiedClassName(object);
        }
        
		/** 为给定的类返回Class对象 */
		public static function GetClassFromName(className:String):Class
        {
            return getDefinitionByName(className) as Class;
        }
        
        /** 为给定的类返回Class对象 */
        public static function GetClass(item:*):Class
        {
            if(item is Class || item == null)
                return item;
            
            return Object(item).constructor;
        }
	}
}