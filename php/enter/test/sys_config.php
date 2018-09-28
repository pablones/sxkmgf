<?php
/**
 * sys_config 表操作
 * @author King.L
 */
define('IN_ECS', true);
require('../includes/init.php');

if($_REQUEST['act'] == 'select'){
	$sql = "select * from ".TBL_SYS_CONFIG."";
	$result = $acc_db -> getAll($sql);
	print_r($result);
}
if($_REQUEST['act'] == 'add'){
	$sql = "INSERT INTO `acc_db`.`sys_config` (`id`, `parent_id`, `code`, `type`, `store_range`, `store_dir`, `value`, `sort_order`) VALUES (NULL, '1', 'licence', 'hidden', '', '', '1319990399', '1')";
	$acc_db -> query($sql);
}
if($_REQUEST['act'] == 'update'){
	$sql = "UPDATE `acc_db`.`sys_config` SET `value` = '1' WHERE code = 'power' LIMIT 1 ;";
	$acc_db -> query($sql);
}
if($_REQUEST['act'] == 'push_all'){
	$sql = "select * from ".TBL_SYS_CONFIG."";
	$result = $acc_db -> getAll($sql);
	foreach($result as $key => $value){
		print_r($value); echo "<br/>";
	}
}
?>