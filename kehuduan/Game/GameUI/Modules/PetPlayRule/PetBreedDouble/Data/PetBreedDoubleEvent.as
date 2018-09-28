package GameUI.Modules.PetPlayRule.PetBreedDouble.Data
{
	public class PetBreedDoubleEvent
	{
		public function PetBreedDoubleEvent()
		{
		}
		
		public static const SHOW_PETBREEDDOUBLE_VIEW:String = "show_petDouble_view";
		
		public static const REMOVE_PETBREEDDOUBLE_VIEW:String = "remove_petDouble_view";
		
		public static const LOCKED_OP_BREEDDOUBLE:String = "locked_op_breedDouble";			/** 对方已锁定 */
		
		public static const ADDPET_OP_BREEDDOUBLE:String = "addPet_op_breedDouble";			/** 对方添加宠物 */
		
		public static const ADDPET_SELF_BREEDDOUBLE:String = "addPet_self_breedDouble";		/** 对方添加宠物 */
		
		public static const BEGIN_BREEDDOUBLE:String = "begin_breedDouble";					/** 成功开始繁殖 */
		
		public static const FAIL_BREEDDOUBLE:String = "fail_breedDouble";					/** 繁殖失败(条件不符) */
		
		public static const UNLOCK_SELF_BREEDDOUBLE:String = "unLocked_self_breedDouble";	/** 己方解锁 */
		
		public static const UNLOCK_OP_BREEDDOUBLE:String = "unLocked_op_breedDouble";		/** 对方解锁 */
		
	}
}