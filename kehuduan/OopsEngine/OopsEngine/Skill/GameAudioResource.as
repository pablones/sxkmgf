package OopsEngine.Skill
{
	import OopsFramework.Audio.AudioEngine;
	import OopsFramework.Content.Provider.BulkLoaderResourceProvider;
	
	public class GameAudioResource
	{
		/**声音**/
		public var AudioName:String;
		/**职业声音地址**/
		public var AudioPath:String;
		/** 技能声音效果动画BulkLoaderResourceProvider */
		public var AudioBR:BulkLoaderResourceProvider = new BulkLoaderResourceProvider();
        /**加载成功**/
        public var OnLoadAudio:Function;  

		public function onAudioComplete():void
		{
			var skillAudio:AudioEngine = new AudioEngine(AudioBR.GetResource(AudioPath).GetSound());
			if(OnLoadAudio != null)
			   OnLoadAudio(skillAudio,AudioName);
		}
	}
}