<?php
define('IN_ECS', true);
require('../includes/init.php');

if($_REQUEST['act'] == 'select'){
	$sql = "select * from sk_paylog  where paynum = '201107232222541791' ";
	$result = $acc_db -> getRow($sql);
	print_r($result); echo "<br/>";
	$sql = "select * from account where name = 'lqhdks' ";
	$result = $acc_db -> getRow($sql);
	print_r($result); echo "<br/>";
	$sql = "select * from sk_vas_table where Account_ID = '".$result['id']."'";
	$result = $acc_db -> getRow($sql); 
	print_r($result); echo "<br/>";
	
}





?>