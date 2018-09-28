<?php
define('IN_ECS', true);
require('../includes/init.php');

$push_list = $_REQUEST['adminuser'];
$push_list = stripslashes($push_list);
$push_list = unserialize($push_list);
$push_list_num = count($push_list);
/* 先查询已有数据备份 */
$sql = "select * from ".TBL_ADMIN_USER."";
$old_user_list = $acc_db -> getAll($sql);

$sql = "TRUNCATE TABLE `admin_user`";
$acc_db -> query($sql);

$sql = "insert into ".TBL_ADMIN_USER."(`user_name`,`password`,`action_list`,`nav_list`,`lang_type`,`suppliers_id`,`todolist`,`role_id`)values";
foreach ($push_list as $key => $value){
	if($push_list_num > 1){
		$sql .= "('".$value['user_name']."','".$value['password']."','".$value['action_list']."','".$value['nav_list']."','".$value['lang_type']."','".$value['suppliers_id']."','".$value['todolist']."','".$value['role_id']."'),";
	}else{
		$sql .= "('".$value['user_name']."','".$value['password']."','".$value['action_list']."','".$value['nav_list']."','".$value['lang_type']."','".$value['suppliers_id']."','".$value['todolist']."','".$value['role_id']."');";
	}	
	$push_list_num--;		
}

$result = $acc_db -> query($sql);

if($result){
	echo "ok";
}else{
	echo "error";
}
?>