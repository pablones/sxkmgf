package OopsEngine.Utils.IO
{
	import flash.display.BitmapData;
	import flash.utils.ByteArray;
	
	public class Binary
	{
		/** 读ByteArray中的二进制文件 */
		public static function Read(bytes:ByteArray):Array
		{
		    var dataArr:Array = new Array();
		    var byteCount:int = bytes.bytesAvailable;
		    while(bytes.position < byteCount)
		    {
			    var data:BinaryData = new BinaryData();
			    data.Name			= bytes.readUTF();
			    data.Length    	    = bytes.readInt();
			    bytes.readBytes(data.Value,0,data.Length);
			    data.Value.uncompress();
			   
			    dataArr.push(data);
		    }
	   		return dataArr;
		}
	
		/** 写二进制文件 */
//		public static function Write(path:String,valueArr:Array):void
//		{
//			var file:File     = new File(path);
//		    var fs:FileStream = new FileStream();
//		    fs.open(file,FileMode.WRITE);
//		    for each(var bd:BinaryData in valueArr)
//		    {
//		   		bd.Value.compress();
//				fs.writeUTF(bd.Name);
//				fs.writeInt(bd.Value.length);
//			    fs.writeBytes(bd.Value, 0, bd.Value.length);
//			}
//		    fs.close();
//		}
		
		public static function GetByteArrayByBitmapData(bmp:BitmapData):ByteArray
		{
			var fileBytes:ByteArray = new ByteArray();
		    fileBytes.writeUnsignedInt(bmp.width);
		    fileBytes.writeUnsignedInt(bmp.height);
		    fileBytes.writeBytes(bmp.getPixels(bmp.rect));
		    return fileBytes;
		}
		
		public static function GetByteArrayByText(text:*):ByteArray
		{
			var fileBytes:ByteArray = new ByteArray();
		    fileBytes.writeUTFBytes(text);
		    return fileBytes;
		}
	}
}