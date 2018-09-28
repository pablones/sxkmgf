<?php
/**
 * 测试功能脚本
 * 为平时日常测试功能用
 * @author King.L
 */
define('IN_ECS', true);
require(dirname(__FILE__) . '/includes/init.php');
//$sql = "TRUNCATE TABLE `yjjhpt_usernumlog`";
//$yjjhtp_db -> query($sql);
//$sql = "TRUNCATE TABLE `yjjhpt_usernum`";
//$yjjhtp_db -> query($sql);
//print_r("success");

$sql = "select * from log_2011_7_1";
$result = $gdb -> getAll($sql);
print_r($result);
?>
