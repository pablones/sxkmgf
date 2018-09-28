<?php

define('IN_ECS', true);

require(dirname(__FILE__) . '/includes/init.php');

/* 载入加密类 */
require(dirname(__FILE__) . '/includes/lib_code.php');
/* 载入登录类 */
if(extension_loaded('memcache')){
	require(dirname(__FILE__) . '/class/login_class.php');	
	
}else{
	require(dirname(__FILE__) . '/class/oldlogin_class.php');
	
}

/* 载入语言文件 */
require_once(ROOT_PATH . 'languages/' .$_CFG['lang']. '/login.php');
$login = new userLogin($_GET);

if($_REQUEST['act'] == 'sign'){
	$smarty -> assign('f_id' , $_CFG['fuwuqi_id']);
	$smarty -> display('login.dwt');
}
if($_REQUEST['act'] == 'deal'){
	$data = $_GET;
	if($data['adminname'] == '' || $data['admin_pwd'] ==''){
		exit("You have no right to do this");
	}else{
		$data['admin_pwd'] = md5($data['admin_pwd']);
		$sql = "select * from ".TBL_ADMIN_USER." where `user_name` = '".$data['adminname']."' and `password` = '".$data['admin_pwd']."'";
		$result = $acc_db -> getRow($sql);
		if(!$result){
			exit("account or password error");
		}
	}
	$data['time'] = time();
	$data['serverid'] = $_CFG['fuwuqi_id'];
	$data['fcmPower'] = $_CFG['fcmPower'];
	$data['ex_power'] = $_CFG['power'];
	$data['modes_key'] = $_CFG['modes_key'];
	$data['setserverpage'] = $_CFG['setserverpage'];
	$data['userid'] = $_REQUEST['pwd'];	

	$login = new userLogin($data);   
	$login -> dealLogin();
}



?>