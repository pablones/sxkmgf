<?php
/**
 * 防沉迷处理函数
 */
define('IN_ECS', true);
require('../includes/init.php');

$user_name = $_POST['username'];

if(extension_loaded('memcache')){
	$key = USERINFO.$user_name;
	$serverid = $_REQUEST['serverid'];
	$sql = "update ".TBL_ACCOUNT." set `fcm` = 1 where `name` = '".$user_name."'";
	$acc_db -> query($sql);
	$memcached -> delete($key);
	$sql = "select * from ".TBL_ACCOUNT." where name = '".$user_name."'";
	$result = $acc_db -> getRow($sql);
	$result['serverid'] = $serverid;
	$acc_db -> close();
	$memcached -> set($key,$result,0,300);
}else{
	$sql = "update ".TBL_ACCOUNT." set `fcm` = 1 where `name` = '".$user_name."'";
	$acc_db -> query($sql);
	$acc_db -> close();
}



?>