<?php
	define('IN_ECS', true);
	require('../includes/init.php');
	
	$sql = "show status";
	$result = $acc_db -> getAll($sql);
	print_r($result);
?>