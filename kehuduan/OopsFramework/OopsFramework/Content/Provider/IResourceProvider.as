package OopsFramework.Content.Provider
{
	import OopsFramework.Content.ContentTypeReader;
	
    /** 该接口应实施的对象，可以提供资源到的ResourceManager */
    public interface IResourceProvider
    {
        /** 是否加载完成 用于判断资源提供器是否加载完成，加载完成后才可以获取资源。 */
        function get IsLoaded():Boolean;
        
        /** 此方法将检查资料在资料集合中是否存在（true 为存在） */
        function IsResourceExist(name:String):Boolean;

        /** 这种方法被称为请求时从ResourceManager的已知ResourceProvider资源 */
        function GetResource(name:String):ContentTypeReader;
    }
}