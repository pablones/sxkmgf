<?php
define('IN_ECS', true);
require('../includes/init.php');

if($_REQUEST['act'] == 'select'){
	$sql = "select * from `".TBL_ACCOUNT."` where name = '123587 '";
	$result = $acc_db -> getRow($sql);
	print_r($result); echo "<br/>";
	$sql = "select * from `sk_gainaward` where `account_id` = '".$rs['account_id']."'";
	$res = $acc_db -> getRow($sql);
	print_r($res);
}
if($_REQUEST['act'] == 'insert'){
	$sql = "select * from `".TBL_ACCOUNT."` where name = '97250'";
	$result = $acc_db -> getRow($sql);
	$sql ="insert into `sk_gainaward` (`account_id`,`award_code`,`amount`) values(".$result['id'].",'1477','1')";
	$acc_db->query($sql);
//	$sql ="insert into `sk_gainaward` (`account_id`,`award_code`,`amount`) values(".$result['id'].",'1478','1')";
//	$acc_db->query($sql);
	
	$sql = "select * from ".TBL_ACCOUNT." where name = '123587'";
	$result = $acc_db -> getRow($sql);
	$sql ="insert into `sk_gainaward` (`account_id`,`award_code`,`amount`) values(".$result['id'].",'1478','1')";
	$acc_db -> query($sql);
	
	print_r("success");
}
?>