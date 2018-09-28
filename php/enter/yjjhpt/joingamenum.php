<?php

define('IN_ECS', true);
require('../includes/init.php');

$mem_array[] = $key;
$time = time();
$s_id = $_CFG['fuwuqi_id'];
$s_name = $_CFG['s_name'];

$sql = "insert into ".TBL_YJ_USERNUMLOG." (`sid` , `server` ,`count3`,`time`) values('".$s_id."','".$s_name."',1,'".$time."')";
$yjjhpt_db -> query($sql);
$yjjhpt_db -> close();



?>