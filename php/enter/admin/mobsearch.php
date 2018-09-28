<?php
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');

if($_REQUEST['act'] == 'moblist'){
	admin_priv("search_mob");
	$smarty -> display('mob_list.htm');	
}
if($_REQUEST['act'] == 'query'){
	
	$username = isset($_POST['username'])?$_POST['username']:'';
	
	if(!empty($username)){
		$sql = "select * from ".TBL_ACCOUNT." where name = '".$username."'";
		$userinfo = $acc_db -> getRow($sql);
	}else{
		
	}
	
	$sql = "select * from ".TBL_SK_GENERALIZE." where `account_id` = '".$userinfo['id']."' and (type = '4' or type = '5' or type = '6') ";
	$result = $acc_db -> getRow($sql);
	
	$sql = "select * from ".TBL_WB_USER." where `account_id` = '".$userinfo['id']."'";
	$playerinfo = $wangpu_db -> getRow($sql);

	$result['rolename'] = $playerinfo['name'];
	$key = "m".$result['type'];
	
	$result['typename'] = $_LANG[$key];
	if($result['state'] == 1000){
		$result['lstate'] = $_LANG['notget'];
	}else{
		$result['lstate'] = $_LANG['get'];
	}
	
	$smarty -> assign('result' , $result);
	$smarty -> display('mob_list.htm');
	
}
?>