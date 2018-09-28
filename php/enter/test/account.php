<?php
/**
 * 对account表的查询操作
 * @author King.L
 */
define('IN_ECS', true);
require('../includes/init.php');
if($_REQUEST['act'] == 'select'){
	//$sql = "select * from ".TBL_ACCOUNT."";
	//$result = $acc_db -> getAll($sql);
	//print_r($result);
	$sql = "select count(id) from ".TBL_ACCOUNT."";
	$result = $acc_db -> getRow($sql);
	print_r($result);
}
if($_REQUEST['act'] == 'count'){
	$sql = "select count(id) from ".TBL_ACCOUNT."";
	$result = $acc_db -> getAll($sql);
	print_r($result);
	$sql = "select count(id) from ".TBL_ACCOUNT." where regdate >=1310572799 and regdate<=1310659199";
	$res = $acc_db -> getAll($sql);
	print_r($res);
}
?>