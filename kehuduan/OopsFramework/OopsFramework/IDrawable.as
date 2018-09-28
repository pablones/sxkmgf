package OopsFramework
{
	public interface IDrawable
	{
        function get Visible():Boolean			// 是否显示
		function get DrawOrder():int			// 绘制优选级
		
		function get VisibleChanged():Function;
		function set VisibleChanged(value:Function):void;
        function get DrawOrderChanged():Function;
        function set DrawOrderChanged(value:Function):void;
	}
}