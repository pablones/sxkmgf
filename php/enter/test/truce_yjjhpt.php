<?php
/**
 * 清空流失率统计两个表
 * @author King.L
 */
define('IN_ECS', true);
require('../includes/init.php');

/* 清空这两张表 */
if($_REQUEST['act'] == 'truncate'){
	$sql = "TRUNCATE TABLE `yjjhpt_usernumlog`";	
	$yjjhpt_db -> query($sql);
	$sql = "TRUNCATE TABLE `yjjhpt_usernum`";	
	$yjjhpt_db -> query($sql);
	print_r("success");
	exit;
}
/* 查询表 */
if($_REQUEST['act'] == 'select'){
	$sql = "select sum(count1),sum(count2),sum(count3),sum(count4) from ".TBL_YJ_USERNUMLOG." where time >='1310572799'";
	$result = $yjjhpt_db -> getAll($sql);
	print_r($result);
}
if($_REQUEST['act'] == 'getData'){
	$time = time();
	$sql = "select sum(`count1`),sum(`count2`),sum(`count3`),sum(`count4`) from ".TBL_YJ_USERNUMLOG." where `time` <= ".$time."";
	$result = $yjjhpt_db -> getRow($sql);
	print_r($result); echo "<br/>";
	
	$sql = "select count(count4) from ".TBL_YJ_USERNUMLOG." where time <= ".time()." and count4=1";
	$count4 = $yjjhpt_db -> getRow($sql);
	
	$sql = "select count(count1) from ".TBL_YJ_USERNUMLOG." where time<=".time()." and count3=1";
	$count3 = $yjjhpt_db -> getRow($sql);
	
	print_r($count4); echo "<br/>";
	print_r($count3); echo "<br/>";
}


?>
