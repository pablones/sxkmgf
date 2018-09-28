<?php

define('IN_ECS', true);
require('../includes/init.php');

if($_REQUEST['act'] == 'select'){
	$user_name = $_REQUEST['user_name'];
	$key = "userinfo".$user_name;
	$result = $memcached -> get($key);
	print_r($result);
	
}
?>